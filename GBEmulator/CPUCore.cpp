#include "CPUCore.h"
#include <iostream>

CPUCore::CPUCore(RAM& ram, RegBank& regs)
	: d_ram(&ram), d_regs(&regs)
{}

const RAM * CPUCore::ram() const
{
	return d_ram;
}

CPUCore::CycleCount CPUCore::cycleCount() const
{
	return CycleCount(d_cycles);
}

void CPUCore::cycle()
{
}
