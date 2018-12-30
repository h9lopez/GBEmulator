#include "CPUCore.h"
#include "gb_typeutils.h"
#include "ReverseOpcodeMap.h"
#include "OpcodeResultContext.h"
#include <iostream>
#include <iomanip>
#include <arpa/inet.h>

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
	std::cout << "Reading initial ROM value as: " << std::setfill('0') << std::setw(4) << std::hex << res << std::endl;
	// NOTE: This works out perfectly because WordType is defined as a uint16_t, which htons returns
	// If ripping this out for use on system of different length, replace this!
	//res = htons(res);
	//std::cout << "htons returned value of " << std::hex << res << std::endl;
	d_regs->IncPCBy(2);
	return res;
}

void CPUCore::cycle()
{
	ByteType opcode = d_ram->readByte(d_regs->IncPC());

	OpcodeContainer::iterator it = d_opcodes.find(opcode);
	if (it == d_opcodes.end())
	{
		std::cerr << "OPCODE " << std::hex << opcode << " NOT FOUND, SKIPPING\n";
	}

	// Check whether or not its a single byte instr or double
	if (opcode == 0xCB)
	{
		// Two byte prefix
		ByteType secondOpcode = d_ram->readByte(d_regs->IncPC());

		std::cout << "OBSERVING CB: " << INSTR_CB_META[secondOpcode].name << '\n';

		// Looking at two separate things. PC increment and cycles taken
		// PC increment is mostly static (usually length of opcode) except for JR/JP
		// Cycles can vary based on whether a branch is taken or untaken (again, for JR/JP)
		
		OpcodeResultContext context = d_cbOpcodes[opcode]();

		// The CB instruction by itself always takes up 4 cycles PLUS the actual operation on top
		this->d_cycles += context.properties.cycles;
		
		d_regs->PC(d_regs->PC() + context.properties.pcIncrement);
	}
	else 
	{
		std::cout << "OBSERVING: " << std::hex << (int)opcode << " - " << INSTR_META[opcode].name << '\n';
		std::cout << "Current PC is: " << d_regs->PC() << std::endl;
		
		// One byte instruction
		OpcodeResultContext context = d_opcodes[opcode]();

		// TODO: Come back and re-evaluate this. 
		//		 Might be a better design to have a separate class that does CPU manipulation
		this->d_cycles += context.properties.cycles;
		if (context.properties.pc_action_taken == OpcodeResultContext::PCAction::EXPLICIT_SET)
		{
			d_regs->PC(context.properties.pcSet);
		}
		else
		{
			d_regs->PC(d_regs->PC() + context.properties.pcIncrement);
		}
	}
}
