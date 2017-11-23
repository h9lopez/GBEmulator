#pragma once

#include "gtest/gtest.h"
#include "CPUCore.h"
#include "gb_rom.h"

#include <vector>


class CPUOpcodeTest : public ::testing::Test
{
protected:

	CPUOpcodeTest() : regs(), ram(), cpu(ram, regs)
	{

	}

	virtual void SetUp(const std::vector<ByteType> vec)
	{
		// Take snapshot of the old register bank
		this->regs.copyCurrentState(this->snapshot);

		// Load the byte vector into ram 
		ASSERT_TRUE(ROMLoader::fromRawBytes(vec.begin(), vec.end(), this->ram));
	}

	virtual void TearDown()
	{

	}

	RegBank snapshot;
	CPUCore cpu;
	RegBank regs;
	RAM ram;

};