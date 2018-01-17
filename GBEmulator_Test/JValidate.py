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
        res = self.__valHex(jobj["opcode"])
        if res == False:
            return False
        self.testObj.opcode = res

        res = self.__valCycles(jobj["cycles"])
        if res == False:
            return False
        self.testObj.cycles = res

        res = self.__valByteSequence(jobj["sequence"])
        if res == False:
            return False
        self.testObj.sequence = res

        res = self.__valFlags(jobj["flags"])
        if res == False:
            return False
        self.testObj.flagResult = res

        if "regs" in jobj:
            res = self.__valRegs(jobj["regs"])
            if res == False:
                return False
            self.testObj.regsResult = res

        if "mem" in jobj:
            res = self.__valMem(jobj["mem"])
            if res == False:
                return False
            self.testObj.memResult = res

        if "startingState" in jobj:
            res = self.__valStartingState(jobj["startingState"])
            if res == False:
                return False
            self.testObj.startingState = res

        return self.testObj

    def __valStartingState(self, start):
        """Validates the collection of settings under the 
        starting state umbrella. Right now includes registers and mem settings
        that are asserted before the actual test runs.
        """
        startingStateDict = {"mem": {}, "regs": {}}
        
        # Look for memory state first
        if "mem" in start:
            res = self.__valMem(start["mem"])
            if res == False:
                return res
            startingStateDict["mem"] = res

        if "regs" in start:
            res = self.__valRegs(start["regs"])
            if res == False:
                return res
            startingStateDict["regs"] = res

        return startingStateDict

        


    def __valHex(self, op):
        """Validates a hexadecimal value
        
        Throws exception if statement is bad
        """
        try:
            return hex(int(op, 16))
        except:
            print "Could not validate hex object " + str(op)
        return False

    def __valFlags(self, flagArray):
        """Validates that flag config are properly set

        Array should be in order
        """
        try:
            return [ FLAG_VAL_MAP[x] for x in flagArray ]
        except:
            print "Could not validate flags " + str(flagArray)
        return False
    
    def __valByteSequence(self, sequence):
        """Validates the sequence of flags either in array form or one
        continuous string form
        """
        if type(sequence) == type([]):
            try:
                return [ x.decode("hex") for x in sequence ]
            except:
                pass
        elif type(sequence) == type(u""):
            try:
                return list(sequence.decode("hex"))
            except:
                pass
        
        return False

    def __valCycles(self, cycles):
        """Validate the number of CPU cycles taken.

        Should just be a regular integer.
        """
        try:
            return int(cycles)
        except:
            print "Error validating number of cycles " + str(cycles)
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
            return False
        
        # Check to see if all reg values are valid
        if not set.issubset(regKeys, VALID_REG_VALS):
            print "Unrecognized register values in set"
            print "\t" + str(regKeys)  
            return False

        # Then just set the values and return them
        regDict = {}

        try:
            for key in regs.keys():
                regDict[key] = hex(int(regs[key], 16))
            return regDict
        except:
            print "Error parsing values for reg dict"
        return False
        
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
                print "Memory adderss " + str(key) + " is out of bounds. (UPPER LIMIT " + str(MEM_UPPER_LIMIT) + ")"
                return False

            # Check the value to make sure it's valid hex too
            realVal = int(mem[key], 16)

        return mem