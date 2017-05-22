
import urllib2
from bs4 import BeautifulSoup

# Relative to current directory
TARGET_FILE = "GBEmulator/ReverseOpcodeMap.h"

# Formats a string of elements into a C++ string of those elements
def format_as_code(opcode_list, array_name):
    elements_str = "\n\t".join( str(opcode_list)[1:-1].split(',') )

    res = ''
    for opcode in opcode_list:
        res += ('\t"%s",\n' % (str(opcode)))
    code = "std::string %s[%s] = {\n%s\n};" % (str(array_name), str(len(opcode_list)), res)
    return code

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
            tag_text = col.contents[0].encode('ascii', errors='ignore')
            # Ignore empty strings
            res_array.append(tag_text)
    
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

# Main parsing and conjoining
arr1 = format_as_code(enumerate_table(main_instruction_table), "GENERATED_MAIN_INSTRUCTION_NAMES")
arr2 = format_as_code(enumerate_table(cb_prefix_table), "GENERATED_CB_INSTRUCTION_NAMES")

file_preamble = "#pragma once\n#include <string>\n\n/*\nReverse map for mapping opcodes -> readable instruction text\n*/\n\n"

print "Writing generated code to file %s" % (TARGET_FILE)
f = open(TARGET_FILE, 'w')
f.write(file_preamble)
f.write(arr1)
f.write("\n\n")
f.write(arr2)
f.close()
print "File closed. Generation complete."