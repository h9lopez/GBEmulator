#ifndef INCLUDED_GB_CPU
#define INCLUDED_GB_CPU

#include <memory>
#include <map>
#include <functional>

#include <cpu/opcodes/OpcodeResultContext.h>

#include <ram/gb_ram.h>
#include <registers/gb_regs.h>

#include <utils/gb_typeutils.h>
#include <utils/ReverseOpcodeMap.h>

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

	/**Initialize handlers for the main opcode table.
	 * 
	 * This function attaches ALL handlers to each of the opcodes in the primary opcode table.
	 * This handles the main 0x00 - 0xFF opcodes.
	 * 
	 * Opcodes indexed under the 0xCB primary opcode are initialized in the initCBOpcodes function.
	 * 
	 * After this function is called, the d_opcodes container will be initialized.
	 */
	void initOpcodes();

	/**Initialize handlers for the CB opcode table.
	 * 
	 * All opcodes under the primary 0xCB key are initialized under here. This handles 0x00 - 0xFF
	 * under that 0xCB subtable.
	 */
	void initCBOpcodes();

	ByteType readNextByte() const;
	WordType readNextTwoBytes() const;
};

}


#endif