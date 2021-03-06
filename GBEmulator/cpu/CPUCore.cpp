#include <iostream>
#include <iomanip>
#include <arpa/inet.h>
#include <sstream>
#include <boost/log/trivial.hpp>

#include <cpu/CPUCore.h>
#include <cpu/opcodes/OpcodeResultContext.h>

#include <utils/ReverseOpcodeMap.h>
#include <utils/gb_typeutils.h>

using namespace Core;

CPUCore::CPUCore(RAM& ram, RegBank& regs)
	: d_ram(&ram), d_regs(&regs), d_cycles(0)
{
	initOpcodes();
	initCBOpcodes();
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
	
	BOOST_LOG_TRIVIAL(debug) << "Read single byte from ROM: " << std::setfill('0') << std::setw(2) << std::hex << +res;
	//return d_ram->readByte(d_regs->IncPC());
	return res;
}

WordType CPUCore::readNextTwoBytes() const
{
	WordType res = d_ram->readWord(d_regs->PC());
	BOOST_LOG_TRIVIAL(debug) << "Reading two bytes of ROM: " << std::setfill('0') << std::setw(4) << std::hex << +res;

	d_regs->IncPCBy(2);
	return res;
}


void CPUCore::reportOpcodeCoverage(const OpcodeMetadata mapReference[], const OpcodeContainer &container)
{
	// Cycle through opcode map and report on range of opcodes covered and which are uncovered
	BOOST_LOG_TRIVIAL(info) << "--------OPCODE COVERAGE REPORT--------";

	// Main opcode table
	ByteType cursor = 0x00;
	ByteType begin = 0x00;

	std::vector<ByteType> unfoundMap;
	for (begin = cursor; cursor != 0xFF; ++cursor)
	{
		// There are some opcodes that aren't implemented. These are designated by the "ERR" name that I gave it.
		// Could definitely be a better title or even just a global constant instead of this hardcoded value.
		if (container.find(cursor) == container.end()
			&& mapReference[cursor].name != "ERR")
		{
			// Opcode not found at this location
			unfoundMap.push_back(cursor);
			// Print out the range up to this point
			BOOST_LOG_TRIVIAL(info) << "[FOUND] \t0x" << std::hex << (unsigned int)begin << "\t - \t0x" << (cursor - 1);
			begin = cursor + 0x01;
		}
	}

	BOOST_LOG_TRIVIAL(info) << "[FOUND] \t0x" << std::hex << (unsigned int)begin << "\t - \t0x" << (cursor - 1);

	// Start printing out all the ones that WEREN'T found
	for (std::vector<ByteType>::const_iterator cit = unfoundMap.begin();
		 cit != unfoundMap.end();
		 ++cit)
	{
		BOOST_LOG_TRIVIAL(info) << "[MISSING] \t0x" << std::hex << (unsigned int)(*cit) << "\t - \t" << mapReference[(unsigned int)(*cit)].name;
	}

	BOOST_LOG_TRIVIAL(info) << "--------END OPCODE COVERAGE REPORT--------";
}

void CPUCore::cycle()
{
	BOOST_LOG_TRIVIAL(info) << "----------BEGIN CYCLE----------";
	BOOST_LOG_TRIVIAL(debug) << "Current PC is: 0x" << std::setfill('0') << std::setw(4) << std::hex << +d_regs->PC();

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
			int8_t result = context.properties.pcIncrement;
			BOOST_LOG_TRIVIAL(debug) << "Interpreting pcIncrement as signed value: " << std::hex << +result;
			d_regs->PC(d_regs->PC() + result);
		}
	}
	else 
	{
		BOOST_LOG_TRIVIAL(info) << "Observing instruction -> opcode=" << std::hex << +opcode << ", instr=" << INSTR_META[opcode].name;

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
			int8_t result = context.properties.pcIncrement;
			BOOST_LOG_TRIVIAL(debug) << "Interpreting pcIncrement as signed value: " << std::hex << +result;
			d_regs->PC(d_regs->PC() + result);
		}
	}
	BOOST_LOG_TRIVIAL(info) << "----------END CYCLE----------" << std::endl << std::endl;
}
