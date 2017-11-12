#include "CPUCore.h"
#include "gb_typeutils.h"
#include "ReverseOpcodeMap.h"
#include <iostream>

CPUCore::CPUCore(RAM& ram, RegBank& regs)
	: d_ram(&ram), d_regs(&regs), d_cycles(0)
{
	initOpcodes();
}

const RAM * CPUCore::ram() const
{
	return d_ram;
}

CPUCore::CycleCount CPUCore::cycleCount() const
{
	return CycleCount(d_cycles);
}

ByteType CPUCore::readNextByte() const
{
	return d_ram->readByte(d_regs->IncPC());
}

WordType CPUCore::readNextTwoBytes() const
{
	WordType res = d_ram->readWord(d_regs->PC());
	d_regs->IncPCBy(2);
	return res;
}


void CPUCore::cycle()
{
	ByteType opcode = d_ram->readByte(d_regs->IncPC());

	OpcodeContainer::iterator it = d_opcodes.find(opcode);
	if (it == d_opcodes.end())
	{
		std::cerr << "OPCODE " << opcode << " NOT FOUND, SKIPPING\n";
	}

	// Check whether or not its a single byte instr or double
	CycleCount pcIncrement = 0;
	if (opcode == 0xCB)
	{
		// Two byte prefix
		ByteType secondOpcode = d_ram->readByte(d_regs->IncPC());

		std::cout << "OBSERVING CB: " << GENERATED_MAIN_INSTRUCTION_NAMES[secondOpcode] << '\n';

		pcIncrement = d_cbOpcodes[opcode]();
	}
	else 
	{
		std::cout << "OBSERVING: " << std::hex << (int)opcode << " - " << GENERATED_MAIN_INSTRUCTION_NAMES[opcode] << '\n';

		// One byte instruction
		pcIncrement = d_opcodes[opcode]();
	}

	// Increment PC at the end
	d_regs->PC(d_regs->PC() + pcIncrement);
}
