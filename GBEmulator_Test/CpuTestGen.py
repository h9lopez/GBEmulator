#!/usr/bin/env python

# TODO: Have support for ASSERTING memory locations i.e. (DE) after test ends
#       Have support for setting reg values BEFORE opcode begins running

import json
import random
from JValidate import *

class SingleOpcodeTest(object):
    opcode = 0
    sequence = []
    flagResult = []
    cycles = 0
    regsResult = {}
    memResult = {}
    startingState = {"mem": {}, "regs": {}}
    testTemplateFileName = "CpuTestTemplate.cpp"

    def __init__(self):
        pass

    def _buildFlagAsserts(self):
        """Function returns a series of C++ code of ASSERT_*() that
        correspond to the flags that were specified in the object, checking
        their values after the test
        """
        res = []

        # It's always a static set so no need to do dynamic looping

        idx = 0
        # Zero flag
        res.append( 
            'ASSERT_EQ(after.flagZero(), {});'.format(
                'snapshot.flagZero()' if self.flagResult[idx] == FlagStatus.UNMODIFIED else
                '0' if self.flagResult[idx] == FlagStatus.ZERO else '1'
            )
         )

        idx += 1
        # Subtract flag
        res.append(
            'ASSERT_EQ(after.flagSubtract(), {});'.format(
                'snapshot.flagSubtract()' if self.flagResult[idx] == FlagStatus.UNMODIFIED else
                '0' if self.flagResult[idx] == FlagStatus.ZERO else '1'
            )
        )

        idx += 1
        # Half carry
        res.append(
            'ASSERT_EQ(after.flagHalfCarry(), {});'.format(
                'snapshot.flagHalfCarry()' if self.flagResult[idx] == FlagStatus.UNMODIFIED else
                '0' if self.flagResult[idx] == FlagStatus.ZERO else '1'
            )
        )

        idx += 1
        # Carry flag
        res.append(
            'ASSERT_EQ(after.flagCarry(), {});'.format(
                'snapshot.flagCarry()' if self.flagResult[idx] == FlagStatus.UNMODIFIED else
                '0' if self.flagResult[idx] == FlagStatus.ZERO else '1'
            )
        )

        return "\t" + ("\n\t".join(res))

    def _buildRegAsserts(self, token, regMap):
        """Function returns a series of C++ code of ASSERT_*()'s that check the values
        of registers

        Arguments:
        token - Variable to check against. Most likely will be 'before', 'beforeReg',
        'after', or 'afterReg'
        regMap - The actual mapping from registers -> regvalues to use in the assertions
        """
        res = []

        form = 'ASSERT_EQ({token}.{reg}(), {val});'

        for regKey in regMap:
            repMap = {
                      'token': token,
                      'reg': regKey, 
                      'val': regMap[regKey]
                     }
            res.append( form.format(**repMap) )
        
        return "\t" + ("\n\t".join(res))

    def _buildRegSets(self, token, regMap):
        """Function returns a series of C++ code of SETS that setcheck the values
        of registers before a test runs

        Arguments:
        token - Variable to check against. Most likely will be 'before', 'beforeReg',
        'after', or 'afterReg'
        regMap - The actual mapping from registers -> regvalues to use in the assertions
        """
        res = []

        form = '{token}.{reg}({val});'

        for regKey in regMap:
            repMap = {
                      'token': token,
                      'reg': regKey, 
                      'val': regMap[regKey]
                     }
            res.append( form.format(**repMap) )
        
        return "\t" + ("\n\t".join(res))

    def _buildMemAsserts(self, token, memMap):
        """Function returns a series of C++ code for ASSERT*()'s that check the values
        of memory

        Arguments:
        token - The variable which to check against, this will most commonly be either
                'before' or 'after', signifying the snapshot of the result before or after
                test execution
        memMap - The actual mappings from memory address -> values
        """
        res = []

        form = 'ASSERT_EQ({token}.readByte({addr}), {val});'

        for memKey in memMap:
            repMap = {
                        'token': token,
                        'addr' : memKey,
                        'val'  : memMap[memKey]
                     }
            res.append( form.format(**repMap) )
        
        return "\t" + ("\n\t".join(res))

    def _buildMemSets(self, token, memMap):
        """Function returns a series of C++ code for SETTING!!! memory before a test runs
        This does not perform any asserts.
        """
        res = []

        form = '{token}.writeByte({addr}, {val});'

        for memKey in memMap:
            repMap = {
                        'token': token,
                        'addr' : memKey,
                        'val'  : memMap[memKey]
                     }
            res.append( form.format(**repMap) )
        
        return "\t" + ("\n\t".join(res))

    def _buildFlagSets(self, accessToken, valMap):
        """Function returns a series of C++ calls that sets flag values PRIOR to command exec.
        No assertions, just sets.
        """

        # This is identical in structure to flag asserts function
        # TODO: Generalize this into one function?
        res = []

        # It's always a static set so no need to do dynamic looping

        idx = 0
        # Zero flag
        res.append( 
            '{accessToken}.flagZero({newVal});'.format(accessToken=accessToken, newVal='1' if valMap[idx] == FlagStatus.ONE else '0')
         )

        idx += 1
        # Subtract flag
        res.append( 
            '{accessToken}.flagSubtract({newVal});'.format(accessToken=accessToken, newVal='1' if valMap[idx] == FlagStatus.ONE else '0')
         )

        idx += 1
        # Half carry
        res.append( 
            '{accessToken}.flagHalfCarry({newVal});'.format(accessToken=accessToken, newVal='1' if valMap[idx] == FlagStatus.ONE else '0')
         )

        idx += 1
        # Carry flag
        res.append( 
            '{accessToken}.flagCarry({newVal});'.format(accessToken=accessToken, newVal='1' if valMap[idx] == FlagStatus.ONE else '0')
         )

        return "\t" + ("\n\t".join(res))


    def generateCpp(self):
        """Function assumes the object has been filled and ready to be generated.
        This will spit out a single C++ Gtest function to test this specific opcode
        It'll return the C++ function as a string.
        """
        data = None

        # Open template file and get string
        with open(self.testTemplateFileName) as tempFile:
            data = tempFile.read()

        # Replace placeholders in file with those available to us.
        # First, the standard oens.
        replaceMap = {
            'test_name': self.opcode + "_" + str(random.randint(1,100)),
            'romData': ", ".join(['0x%02x' % ord(x) for x in self.sequence]),
            'cycleAmount': self.cycles,
            'flagAsserts': self._buildFlagAsserts(),
            'preRegSets': self._buildRegSets('regs', self.startingState["regs"]),
            'preMemSets': self._buildMemSets('mem', self.startingState["mem"]),
            'preFlagSets': self._buildFlagSets('regs', self.startingState["flags"] if "flags" in self.startingState else [0,0,0,0]),
            'preRegAsserts': self._buildRegAsserts('snapshot', []),
            'preMemAsserts': self._buildMemAsserts('memSnapshot', []),
            'regAsserts': self._buildRegAsserts('after', self.regsResult),
            'memAsserts': self._buildMemAsserts('memAfter', self.memResult )
        }

        return data.format(**replaceMap)


def main():
    filData = None

    with open("gbtestgen.json") as fil:
        filData = json.load(fil)


    resultingCode = ''
    # A bookkeeping list to keep track of which opcodes were processed so far
    processed_opcodes = []

    # Generate single opcodes tests
    for test in filData["single_opcode"]["tests"]:
        print "*****Beginning parsing for opcode {op}".format(op=test["opcode"])
        print "\t- Validating------",
        validator = JSONValidator(SingleOpcodeTest())
        validateRes = validator.populateTest(test)
        if validateRes == False:
            print "ERROR: Could not parse test-> " + str(test)
            break
        else:
            print "\t[OK]"
        # If succeeds, the return type is of type SingleOpcodeTest
        testObj = validateRes

        processed_opcodes.append( testObj.opcode )
        
        print "\t- Generating CPP------",
        resultingCode += testObj.generateCpp() + "\n\n"
        print "\t[OK]"
        

    # Output generated code to file.
    with open("GBEmulator_Test.gen.cpp", "w") as fil:
        fil.writelines([
            "#include \"gtest/gtest.h\"\n",
            '#include <iostream>\n',
            '#include <array>\n',
            '#include <cpu/CPUCore.h>\n',
            '#include "OpcodesTest.hpp"\n\n\n',
            resultingCode,
            "int main(int argc, char *argv[])\n{\n\ttesting::InitGoogleTest(&argc, argv);\n\tRUN_ALL_TESTS();\n\tstd::getchar();\n    return 0;\n}"
        ])

    print "Processed opcodes: "
    print str(processed_opcodes)



if __name__ == "__main__":
    main()