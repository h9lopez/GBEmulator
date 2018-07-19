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

class FlagStatus(object):
    UNMODIFIED = 2
    ZERO = 3
    ONE = 4

FLAG_VAL_MAP = {
    "U" : FlagStatus.UNMODIFIED,
    "0" : FlagStatus.ZERO,
    "1" : FlagStatus.ONE
}



MEM_UPPER_LIMIT = 0xFFFF

class JSONValidator(object):
    def __init__(self, testObj):
        self.testObj = testObj

    def populateTest(self, jobj):
        """Function is passed in a json object from file,
        Parses and validates info passed in and transform into a
        business SIngleOpcodeTest object for C++ code generation.
        This does not generate C++ code.

        Arguments
        jobj - The json object (not string) that contains all info for a single opcode test

        Returns - A SingleOpcodeTest object
        """
        self.testObj.opcode = self.__valHex(jobj["opcode"])

        self.testObj.cycles = self.__valCycles(jobj["cycles"])

        self.testObj.sequence = self.__valByteSequence(jobj["sequence"])

        self.testObj.flagResult = self.__valFlags(jobj["flags"])

        if "regs" in jobj:
            self.testObj.regsResult = self.__valRegs(jobj["regs"])

        if "mem" in jobj:
            self.testObj.memResult = self.__valMem(jobj["mem"])

        if "startingState" in jobj:
            self.testObj.startingState = self.__valStartingState(jobj["startingState"])

        return self.testObj

    def __valStartingState(self, start):
        """Validates the collection of settings under the 
        starting state umbrella. Right now includes registers and mem settings
        that are asserted before the actual test runs.
        """
        startingStateDict = {"mem": {}, "regs": {}}
        # Look for memory state first
        if "mem" in start:
            startingStateDict["mem"] = self.__valMem(start["mem"])

        if "regs" in start:
            startingStateDict["regs"] = self.__valRegs(start["regs"])

        if "flags" in start:
            startingStateDict["flags"] = self.__valFlags(start["flags"])

        return startingStateDict

        


    def __valHex(self, op):
        """Validates a hexadecimal value
        
        Throws exception if statement is bad
        """
        try:
            return hex(int(op, 16))
        except Exception as e:
            print "Could not validate hex object " + str(op)
            raise e
        return False

    def __valFlags(self, flagArray):
        """Validates that flag config are properly set

        Array should be in order
        """
        try:
            return [ FLAG_VAL_MAP[x] for x in flagArray ]
        except Exception as e:
            print "Could not validate flags " + str(flagArray)
            raise e
        return False
    
    def __valByteSequence(self, sequence):
        """Validates the sequence of flags either in array form or one
        continuous string form
        """
        if type(sequence) == type([]):
            try:
                return [ x.decode("hex") for x in sequence ]
            except Exception as e:
                raise e
        elif type(sequence) == type(u""):
            try:
                return list(sequence.decode("hex"))
            except Exception as e:
                print "Sequence " + str(sequence)
                raise Exception("Failed on validating string sequence of {seq}, excp={exc}".format(seq=str(sequence), exc=str(e)))
        
        return False

    def __valCycles(self, cycles):
        """Validate the number of CPU cycles taken.

        Should just be a regular integer.
        """
        try:
            return int(cycles)
        except Exception as e:
            print "Error validating number of cycles " + str(cycles)
            raise e
        return False

    def __valRegs(self, regs):
        """Validates registers and what their corresponding values should be.

        Not all registers have to be present, only ones present will be validated
        All others will be ignored.
        """
        regKeys = set(regs)

        # Test whether there are duplicate items
        if len(regKeys) != len(regs.keys()):
            print "Duplicate regs in set"
            raise Exception("Duplicate regs")
        
        # Check to see if all reg values are valid
        if not set.issubset(regKeys, VALID_REG_VALS):
            print "Unrecognized register values in set"
            print "\t" + str(regKeys)  
            raise Exception("Unrecognized register values, vals={vals}".format(str(regKeys)))

        # Then just set the values and return them
        regDict = {}

        for key in regs.keys():
            regDict[key] = hex(int(regs[key], 16))
        return regDict
        
    def __valMem(self, mem):
        """Validate the sections of memory that were specified in the dictionary.
        """

        # For every address in the object, make sure that it's a valid hex address
        # AND it's within the upper memory limit
        for key in mem.keys():
            # Make sure it's valid hex
            realVal = int(key, 16)

            # Make sure it's within limit
            if realVal > MEM_UPPER_LIMIT:
                errStr = "Memory adderss " + str(key) + " is out of bounds. (UPPER LIMIT " + str(MEM_UPPER_LIMIT) + ")"
                print errStr
                raise MemoryError(errStr)

            # Check the value to make sure it's valid hex too
            realVal = int(mem[key], 16)

        return mem