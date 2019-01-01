#include "CPUCore.h"
#include "gb_typeutils.h"
#include "ReverseOpcodeMap.h"
#include "OpcodeResultContext.h"
#include <iostream>
#include <iomanip>
#include <arpa/inet.h>
#include <sstream>
#include <boost/log/trivial.hpp>

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
	ByteType res = d_ram->readByte(d_regs->IncPC());
	
	BOOST_LOG_TRIVIAL(debug) << "Read single byte from ROM: " << std::setfill('0') << std::setw(2) << std::hex << res;
	//return d_ram->readByte(d_regs->IncPC());
	return res;
}

WordType CPUCore::readNextTwoBytes() const
{
	WordType res = d_ram->readWord(d_regs->PC());
	BOOST_LOG_TRIVIAL(debug) << "Reading two bytes of ROM: " << std::setfill('0') << std::setw(4) << std::hex << res;

	d_regs->IncPCBy(2);
	return res;
}

void CPUCore::cycle()
{
	ByteType opcode = d_ram->readByte(d_regs->IncPC());

	OpcodeContainer::iterator it = d_opcodes.find(opcode);
	if (it == d_opcodes.end())
	{
		BOOST_LOG_TRIVIAL(error) << "OPCODE " << std::hex << +opcode << " NOT FOUND, SKIPPING";
	}

	// Check whether or not its a single byte instr or double
	if (opcode == 0xCB)
	{
		// Two byte prefix
		ByteType secondOpcode = d_ram->readByte(d_regs->IncPC());

		BOOST_LOG_TRIVIAL(info) << "Observing CB instruction -> " << INSTR_CB_META[secondOpcode].name;

		// Looking at two separate things. PC increment and cycles taken
		// PC increment is mostly static (usually length of opcode) except for JR/JP
		// Cycles can vary based on whether a branch is taken or untaken (again, for JR/JP)
		
		if (d_cbOpcodes.find(secondOpcode) == d_cbOpcodes.end())
		{
			std::ostringstream stream;
			stream << "CB opcode not located, opcode: " << std::hex << +secondOpcode;
			throw std::runtime_error(stream.str());
		}
		OpcodeResultContext context = d_cbOpcodes[secondOpcode]();

		// The CB instruction by itself always takes up 4 cycles PLUS the actual operation on top
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
	else 
	{
		BOOST_LOG_TRIVIAL(info) << "Observing instruction -> opcode=" << std::hex << +opcode << ", instr=" << INSTR_META[opcode].name;
		BOOST_LOG_TRIVIAL(debug) << "Current PC is: " << d_regs->PC();
		
		// One byte instruction
		if (d_opcodes.find(opcode) == d_opcodes.end())
		{
			std::ostringstream stream;
			stream << "Opcode not located, opcode: " << std::hex << +opcode;
			throw std::runtime_error(stream.str());
		}
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
