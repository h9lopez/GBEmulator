#ifndef INCLUDED_GB_CPU
#define INCLUDED_GB_CPU

#include <memory>
#include <map>
#include <functional>
#include "gb_ram.h"
#include "gb_regs.h"
#include "gb_typeutils.h"
#include "OpcodeResultContext.h"

namespace Core {
typedef std::map< ByteType, std::function<OpcodeResultContext(void)> >  OpcodeContainer;

class CPUCore
{
public:
	typedef ByteType CycleCount;


public:
    CPUCore(RAM& ram,
			RegBank& regs);

    // Member getters 
    const RAM* ram() const;
    CycleCount cycleCount() const;
	/** Get the map containing all of the mapped opcode -> handler functions.
	 */
	const OpcodeContainer &getPrimaryOpcodes() const
	{
		return this->d_opcodes;
	}
	/** Get the map containing all of the mapped opcode -> handler functions for CB opcodes.
	 */
	const OpcodeContainer &getSecondaryOpcodes() const
	{
		return this->d_cbOpcodes;
	}


	// Helper functions

	/** Reports on the implemented range of opcodes for a specific OpcodeMetadata map out to log.
	 * 
	 * Note: Probably not the most optimized or prettiest. I just needed some sort of knowledge.
	 * 
	 * Does two steps:
	 * 1. Sifts through the container and prints out the ranges of elements that are covered
	 * 2. After printing out found ranges, it'll individually print out opcodes NOT implemented.
	 * 
	 * \param[in] mapReference - Map object to refer to individual opcodes metadata (i.e. name to print)
	 * \param[in] container - Container to check the existance of opcodes against.
	 */
	static void reportOpcodeCoverage(const OpcodeMetadata mapReferece[], const OpcodeContainer &container);

    // Execution functions
    void cycle();
    
private:
	// The pair is returned from an opcode lambda
	// of the form CYCLE
	//		CYCLE	-> How many cycles did the instr take

    RAM* d_ram;
    RegBank* d_regs;
    CycleCount d_cycles;
	OpcodeContainer d_opcodes;
    OpcodeContainer d_cbOpcodes;
	ByteType d_opcodeCycles;

	void initOpcodes();
	ByteType readNextByte() const;
	WordType readNextTwoBytes() const;
};

}


#endif