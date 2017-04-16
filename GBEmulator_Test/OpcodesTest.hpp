#include "gtest/gtest.h"
#include "Gameboy.h"
#include "CPU.h"
#pragma once

class OpcodesTest : public ::testing::Test
{
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