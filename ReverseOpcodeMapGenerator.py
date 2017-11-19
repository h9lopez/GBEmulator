
import urllib2
from bs4 import BeautifulSoup

# Relative to current directory
TARGET_FILE = "GBEmulator/ReverseOpcodeMap.h"

""" 
@param opcode_list is a list of dictionary objects that contain info about individual opcodes
@param array_name the C++ variable name to give the resulting array
"""
def format_as_code(opcode_list, array_name):
    # NOTE: I have no idea what this next line was supposed to be for
    #elements_str = "\n\t".join( str(opcode_list)[1:-1].split(',') )
    res = ''
    for opcode in opcode_list:
        res += ('\t"%s",\n' % (str(opcode)))
    code = "std::string %s[%s] = {\n%s\n};" % (str(array_name), str(len(opcode_list)), res)
    return code

"""Function will grab the incoming list of dictionary objects and produce an 
 array of lists of names of the opcodes (i.e. 'JR x, y', 'NOP', etc)
 @param opcode_list is the list of dictionary objects produced by enumerate_table
"""
def spit_opcode_names(opobj_list, array_name):
    return format_as_code( [x['name'] for x in opobj_list], array_name )


def generate_from_list(opobj_list, name):
    res = ''
    for op in opobj_list:
        res += generate_cpp_struct(op) + ',\n\t\t'
    code = "OpcodeMetadata %s[%s] = {\n\t\t%s\n};" % (str(name), str(len(opobj_list)), res)
    return code

def parse_flag(flag):
    if flag == '-':
        return 'FLAG_UNMODIFIED'

    if flag == '0':
        return 'FLAG_ZERO'

    if flag == '1':
        return 'FLAG_ONE'

    return 'FLAG_MODIFIED'

def generate_cpp_struct(opobj):
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

"""Function will produce a string of C++ code that is an array of all of the
 cycle counts of the opcodes in the list
 @param opobj_list - The list of dictionary objects returned by enumerate_table
 @param array_name_short - The name of the C++ array to be generated for short (untaken) paths
 @param array_name_long - Name of C++ array to be generated for long (taken) paths
"""
def spit_opcode_cycles(opobj_list, array_name_short, array_name_long):
    # This one's a little trickier. Opcode cycles can be in format '8/12'
    # Meaning it could be 8 cycles OR 12. So we'll provide two lists,
    # one for short execs, the other for long execs.

    short_execs = []
    long_execs = []
    # Seperate into two lists
    short_execs = [x['cycles'].split('/')[0] if x['cycles'].find('/') > 0 else x['cycles'] \
                                                                       for x in opobj_list]
    long_execs = [x['cycles'].split('/')[1] if x['cycles'].find('/') > 0 else x['cycles'] \
                                                                       for x in opobj_list]

    # Transform to individual arrays
    short_code = format_as_code(short_execs, array_name_short)
    long_code = format_as_code(long_execs, array_name_long)

    # Concat and return
    return short_code + "\n\n" + long_code

# Parses a table in order
def enumerate_table(table, ignore_first_row = True, ignore_first_column = True, begin_counter = 0):
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
raw_html = urllib2.urlopen("http://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html").read()
tableDoc = BeautifulSoup(raw_html, 'html.parser')


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
gen_code = generate_from_list(parsed_main_table, 'GENERATED_MAIN_INSTRUCTION_NAMES')
gen_cb_code = generate_from_list(parsed_cb_table, "GENERATED_CB_INSTRUCTION_NAMES")

file_preamble = "#pragma once\n#include <string>\n\n/*\nReverse map for mapping opcodes -> readable instruction text\n*/\n\n// Useful to see the state at which the flags should be at after the opcode is executed.\nenum FlagMod {\n    FLAG_UNMODIFIED,\n    FLAG_MODIFIED, // this value will be determined by the operation itself.\n    FLAG_ZERO,\n    FLAG_ONE\n};\n\nstruct OpcodeMetadata \n{\n    std::string name;\n    unsigned int byteLength;\n    // The number of cycles taken if a short exec path (untaken branch i.e.)\n    unsigned int cyclesTaken; \n    // Number of cycles for a long exec path\n    unsigned int cyclesUntaken;\n    enum FlagMod flagZero;\n    enum FlagMod flagSubtract;\n    enum FlagMod flagHalfCarry;\n    enum FlagMod flagCarry;\n};\n\n\n"

print "Writing generated code to file %s" % (TARGET_FILE)
f = open(TARGET_FILE, 'w')
f.write(file_preamble)
f.write(gen_code)
f.write("\n\n")
f.write(gen_cb_code)
f.close()
print "File closed. Generation complete."