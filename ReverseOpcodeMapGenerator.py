import urllib2
from bs4 import BeautifulSoup

# Relative to current directory
TARGET_FILE = "GBEmulator/ReverseOpcodeMap.h"


def generate_from_list(opobj_list, name):
    """Generates valid C++ code in the form of an array of structs from the dictionary passed in

    Arguments:
    opobj_list  -- The list of Python dictionary objects that contain valid data for the struct
    name        -- The name of the resulting C++ array to be generated
    """
    res = ''
    for op in opobj_list:
        res += generate_cpp_struct(op) + ',\n\t\t'
    code = "static const OpcodeMetadata %s[%s] = {\n\t\t%s\n};" % (str(name), str(len(opobj_list)), res)
    return code

def parse_flag(flag):
    """Parses the direct string value from the HTML table for a single flag.
    Most commonly look like '-', '0', '1', or 'H', 'S', 'Z', etc
    This will spit out the corresponding C++ enum value in generation.

    Returns a string.

    Arguments:
    flag        -- The singular string flag value directly from parsing
    """
    if flag == '-':
        return 'FLAG_UNMODIFIED'

    if flag == '0':
        return 'FLAG_ZERO'

    if flag == '1':
        return 'FLAG_ONE'

    return 'FLAG_MODIFIED'


def generate_cpp_struct(opobj):
    """Generates a single C++ struct based on the single dictionary object of values passed in.
    Expected format of the dictionary is: 
        "name": val,
        "length": val,
        "cycles": val,
        "flags": { "zero": val, "subtract": val, "half_carry": val, "carry": val }
    The returned value is a C++ line of code that instantiates the struct.
        i.e. { "hello", 1, 2, "world" }
    
    Arguments:
    opobj       -- The single Python dictionary that contains values for generating struct
    """
    # Check to see if there is a long/short cycle 
    cycle_split = opobj["cycles"].split('/')
    short_cycle = cycle_split[0]
    long_cycle = cycle_split[-1]

    # Wrap the name in string quotes
    name = "\"" + opobj["name"] + "\""

    # Parse the flags
    fZero = parse_flag(opobj["flags"]["zero"])
    fSub = parse_flag(opobj["flags"]["subtract"])
    fHalfCarry = parse_flag(opobj["flags"]["half_carry"])
    fCarry = parse_flag(opobj["flags"]["carry"])


    builder = [name, opobj["length"], short_cycle, long_cycle, fZero, fSub, fHalfCarry, fCarry]

    return "{ " + ", ".join(builder) + " }"


# Parses a table in order
def enumerate_table(table, ignore_first_row = True, ignore_first_column = True):
    """Grabs the raw HTML table element and combs through it extracting each cell into a Python dictionary object.
    Returns a list of all parsed dictionary objects found in the table.
    Resulting length of the list should be table len x width (or 256 in our specific case)

    Arguments:
    table               -- Raw HTML table element from BeautifulSoup
    ignore_first_row    -- Ignores the header objects if desired (i.e. header is first row)
    ignore_first_column -- Ignores the header obejcts if desired (i.e. first column has header info)
    """
    if table == None:
        print "Table not valid"
        return

    # Determining whether or not to ignore the first row and column of the table
    row_start_index = 0 if ignore_first_row == False else 1
    col_start_index = 0 if ignore_first_column == False else 1

    # Results of the parsing
    res_array = []

    # Get all of the rows in the table
    children = table.findAll("tr")
    
    for row in children[row_start_index:]: 
        for col in row.findAll("td")[col_start_index:]: 
            # Parse the stuff inside the tags, ignoring weird characters

            if len(col.contents) < 5:
                res_obj = {'name': 'ERR', 'length': '0', 'cycles': '0',
                           'flags': {
                               'zero': '-',
                               'subtract': '-',
                               'half_carry': '-',
                               'carry': '-'
                           }}
                res_array.append(res_obj)
                continue

            # Main string text for the opcode
            opcode_name_str = col.contents[0].encode('ascii', errors='ignore')
            # Grab the next line which contains instruction length and cycle count
            opcode_metainfo = col.contents[2]
            byte_length_str, cycle_count_str = opcode_metainfo.split()
            # Next line is what flags the opcode modifies
            opcode_flagline = col.contents[4].encode('ascii', errors='ignore')
            flag_zero_str, flag_sub_str, flag_halfcarry_str, flag_carry_str = opcode_flagline.split()

            # Synthesisze the information gotten into a nice dictionary
            res_obj = {'name': opcode_name_str,
                       'length': byte_length_str,
                       'cycles': cycle_count_str,
                       'flags': {  
                                    'zero': flag_zero_str,
                                    'subtract': flag_sub_str,
                                    'half_carry': flag_halfcarry_str,
                                    'carry': flag_carry_str
                                } 
                       }


            # Ignore empty strings
            res_array.append(res_obj)
    
    return res_array


###############################################################################################
# Beginning of fetching and parsing ###########################################################
###############################################################################################

# Grab the raw HTML from the source
raw_html = urllib2.urlopen("http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html").read()
tableDoc = BeautifulSoup(raw_html, 'html.parser')

# Find the two target tables - one filled with regular instructions and the other with CB instrs
tables = tableDoc.findAll("table")
if len(tables) == 0:
    print "Error in getting tables"
    exit()

main_instruction_table = tables[0]
cb_prefix_table = tables[1]

if main_instruction_table == None or cb_prefix_table == None:
    print "Tables dont match expected values"
    exit()


# Enumerate each of the tables, i.e. extract their information and put into a usable format
parsed_main_table = enumerate_table(main_instruction_table)
parsed_cb_table = enumerate_table(cb_prefix_table)

print "Grabbed " + str(len(parsed_main_table)) + " main table objects"
print "Grabbed " + str(len(parsed_cb_table)) + " CB table objects"

# Use the parsed information to generate valid C++ code for writing
gen_code = generate_from_list(parsed_main_table, 'INSTR_META')
gen_cb_code = generate_from_list(parsed_cb_table, "INSTR_CB_META")


# Prepare to write the generated code to the file
file_preamble = "#pragma once\n#include <string>\n\n/*\nReverse map for mapping opcodes -> readable instruction text\n*/\n\n// Useful to see the state at which the flags should be at after the opcode is executed.\nenum FlagMod {\n    FLAG_UNMODIFIED,\n    FLAG_MODIFIED, // this value will be determined by the operation itself.\n    FLAG_ZERO,\n    FLAG_ONE\n};\n\nstruct OpcodeMetadata \n{\n    std::string name;\n    unsigned int length;\n    // The number of cycles taken if a short exec path (untaken branch i.e.)\n    unsigned int cyclesTaken; \n    // Number of cycles for a long exec path\n    unsigned int cyclesUntaken;\n    enum FlagMod flagZero;\n    enum FlagMod flagSubtract;\n    enum FlagMod flagHalfCarry;\n    enum FlagMod flagCarry;\n};\n\n\n"

print "Writing generated code to file %s" % (TARGET_FILE)
f = open(TARGET_FILE, 'w')
f.write(file_preamble)
f.write(gen_code)
f.write("\n\n")
f.write(gen_cb_code)
f.close()
print "File closed. Generation complete."
print "Goodbye"