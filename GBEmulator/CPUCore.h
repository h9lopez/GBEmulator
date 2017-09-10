#ifndef INCLUDED_GB_CPU
#define INCLUDED_GB_CPU

#include <memory>
#include "gb_ram.h"
#include "gb_regs.h"

class CPUCore
{
private:
	typedef unsigned long CycleCount;

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
};


#endif