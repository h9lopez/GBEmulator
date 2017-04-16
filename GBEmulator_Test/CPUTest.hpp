#include <gtest/gtest.h>
#include "CPU.h"
#include "Gameboy.h"
#pragma once

class CPUTest : public ::testing::Test {
protected:
	virtual void SetUp()
	{
		gbcpu.setMemoryLocation(this->mem, GB_MEMSIZE);
	}

	virtual void TearDown()
	{

	}
	CPU gbcpu;
	uint8_t mem[GB_MEMSIZE]{ 0 };
};