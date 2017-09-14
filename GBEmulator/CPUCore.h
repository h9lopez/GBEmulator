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
	typedef unsigned long CycleCount;
	typedef std::map< ByteType, std::function<WordType(void)> >  OpcodeContainer;

public:
    CPUCore(RAM& ram,
			RegBank& regs);

    // Member getters 
    const RAM* ram() const;
    CycleCount cycleCount() const;

    // Execution functions
    void cycle();
    
private:
    RAM* d_ram;
    RegBank* d_regs;
    CycleCount d_cycles;
	static OpcodeContainer d_opcodes;
    static OpcodeContainer d_cbOpcodes;
};



#endif