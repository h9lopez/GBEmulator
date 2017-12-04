#!/usr/bin/env python

import json

class FlagStatus(object):
    UNMODIFIED = 2
    ZERO = 3
    ONE = 4

FLAG_VAL_MAP = {
    "U" : FlagStatus.UNMODIFIED,
    "0" : FlagStatus.ZERO,
    "1" : FlagStatus.ONE
}

VALID_REG_VALS = [
    "A",
    "BC",
    "DE",
    "HL",
    "SP",
    "PC",
    "B",
    "C",
    "D",
    "E",
    "H",
    "L"
]

class SingleOpcodeTest(object):
    opcode = 0
    sequence = []
    flagResult = []
    cycles = 0
    regsResult = {}
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
                'before.flagZero()' if self.flagResult[idx] == FlagStatus.UNMODIFIED else
                '0' if self.flagResult[idx] == FlagStatus.ZERO else '1'
            )
         )

        idx += 1
        # Subtract flag
        res.append(
            'ASSERT_EQ(after.flagSubtract(), {});'.format(
                'before.flagSubtract()' if self.flagResult[idx] == FlagStatus.UNMODIFIED else
                '0' if self.flagResult[idx] == FlagStatus.ZERO else '1'
            )
        )

        idx += 1
        # Half carry
        res.append(
            'ASSERT_EQ(after.flagHalfCarry(), {});'.format(
                'before.flagHalfCarry()' if self.flagResult[idx] == FlagStatus.UNMODIFIED else
                '0' if self.flagResult[idx] == FlagStatus.ZERO else '1'
            )
        )

        idx += 1
        # Carry flag
        res.append(
            'ASSERT_EQ(after.flagCarry(), {});'.format(
                'before.flagCarry()' if self.flagResult[idx] == FlagStatus.UNMODIFIED else
                '0' if self.flagResult[idx] == FlagStatus.ZERO else '1'
            )
        )

        return "\t" + ("\n\t".join(res))

    def _buildRegAsserts(self):
        """Function returns a series of C++ code of ASSERT_*()'s that check the values
        after the test is complete.
        """
        res = []

        form = 'ASSERT_EQ(after.{reg}(), {val});'

        for regKey in self.regsResult:
            repMap = {'reg': regKey, 
                      'val': self.regsResult[regKey]
                     }
            res.append( form.format(**repMap) )
        
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
            'test_name': self.opcode,
            'romData': ", ".join(['0x%02x' % ord(x) for x in self.sequence]),
            'cycleAmount': self.cycles,
            'flagAsserts': self._buildFlagAsserts(),
            'regAsserts': self._buildRegAsserts()
        }

        return data.format(**replaceMap)


    @staticmethod
    def fromJsonObj(jobj):
        """Function is passed in a json object from file,
        Parses and validates info passed in and transform into a
        business SingleOpcodeTest object for C++ code generation.
        This does not generate C++ code.

        Arguments:
        jobj - The json object (not string) that contains all info for
                a single opcode test
        
        Returns - A SingleOpcodeTest object.
        """
        nObj = None

        try:
            nObj = SingleOpcodeTest()

            # Opcode, required    
            nObj.opcode = hex(int(jobj["opcode"], 16))

            # Sequence of bytes, can EITHER be an array of hex values
            # or a long string of hex
            if type(jobj["sequence"]) == type([]):
                nObj.sequence = [ hex(int(x, 16)) for x in jobj["sequence"] ]
            elif type(jobj["sequence"]) == type(u""):
                nObj.sequence = list(jobj["sequence"].decode("hex"))

            # Flag results after test. Required as a whole but individual
            # flags can be set as 'unmodified' from before-test values
            nObj.flagResult = [ FLAG_VAL_MAP[x] for x in jobj["flags"] ]

            # Number of cycles taken, required
            nObj.cycles = int(jobj["cycles"])

            # Register vaues after test run, not required.
            if "regs" in jobj:
                nObj.regsResult = jobj["regs"]

                regKeys = set(jobj["regs"])

                if len(regKeys) != len(jobj["regs"].keys()):
                    # Duplicate items, reject
                    raise Exception("Duplicate regs in set")

                # Check to see if all reg values are valid
                if not set.issubset(regKeys, VALID_REG_VALS):
                    raise Exception("Unrecognized regs in list")

                for key in nObj.regsResult.keys():
                    nObj.regsResult[key] = hex(int(nObj.regsResult[key], 16))

        except Exception as e:
            print "Failed parsing json object"
            print type(e)
            print e
            print (jobj)
            return None

        return nObj


def main():
    fil = open("gbtestgen.json")

    filData = json.load(fil)

    testObj = SingleOpcodeTest.fromJsonObj(filData["single_opcode"]["tests"][1])

    if testObj != None:
        print testObj.opcode
        print testObj.sequence
        print testObj.flagResult
        print testObj.cycles
        print testObj.regsResult

        print testObj.generateCpp()

    fil.close()

if __name__ == "__main__":
    main()