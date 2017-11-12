#ifndef INCLUDED_GB_CPU
#define INCLUDED_GB_CPU

#include <memory>
#include <map>
#include <functional>
#include "gb_ram.h"
#include "gb_regs.h"

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

    // Execution functions
    void cycle();
    
private:
	// The pair is returned from an opcode lambda
	// of the form CYCLE
	//		CYCLE	-> How many cycles did the instr take
	typedef std::map< ByteType, std::function<CycleCount(void)> >  OpcodeContainer;

    RAM* d_ram;
    RegBank* d_regs;
    CycleCount d_cycles;
	OpcodeContainer d_opcodes;
    OpcodeContainer d_cbOpcodes;

	void initOpcodes();
	ByteType readNextByte() const;
	WordType readNextTwoBytes() const;
};



#endif