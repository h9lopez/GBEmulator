#include "gtest/gtest.h"
#include "CPUCore.h"
#include "gb_rom.h"
#pragma once

class CPUOpcodeTest : public ::testing::Test
{
protected:

	CPUOpcodeTest() : regs(), ram(), cpu(ram, regs)
	{

	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{

	}

	CPUCore cpu;
	RegBank regs;
	RAM ram;

};