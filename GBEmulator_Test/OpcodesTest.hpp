#pragma once

#include "gtest/gtest.h"
#include "CPUCore.h"
#include "gb_rom.h"

#include <vector>


class CPUOpcodeTest : public ::testing::Test
{
protected:

	CPUOpcodeTest() : regs(), mem(), cpu(mem, regs)
	{

	}

	virtual void SetUp(const std::vector<ByteType> vec)
	{
		// Take snapshot of the old register bank
		this->regs.copyCurrentState(this->snapshot);
		this->mem.copyCurrentState(this->memSnapshot);

		// Load the byte vector into ram 
		ASSERT_TRUE(ROMLoader::fromRawBytes(vec.begin(), vec.end(), this->mem));
	}

	virtual void TearDown()
	{

	}

	RegBank snapshot;
	RAM memSnapshot;
	Core::CPUCore cpu;
	RegBank regs;
	RAM mem;

};