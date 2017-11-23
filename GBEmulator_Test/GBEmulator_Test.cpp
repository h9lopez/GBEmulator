// GBEmulator_Test.cpp : Defines the entry point for the console application.
//

#include "gtest/gtest.h"
#include "stdafx.h"
#include <iostream>
#include <array>
#include "CPUCore.h"
#include "OpcodesTest.hpp"


// Checks whether the flags and *only* the flags between the two registers
// are identical. If they aren't then will return false, otherwise true.
// @before register state before modifying opcode call
// @after register state immediately after opcode processed
bool flags_are_same_state(const RegBank& before, const RegBank& after)
{
	return (before.flagCarry() == after.flagCarry() &&
		before.flagHalfCarry() == after.flagHalfCarry() &&
		before.flagSubtract() == after.flagSubtract() &&
		before.flagZero() == after.flagZero());
}


TEST_F(CPUOpcodeTest, opcode_test_0x00)
{
	SetUp({0x00});
	
	// Perform exactly 1 cycle
	cpu.cycle();

	// Grab after state of register bank
	RegBank after;
	regs.copyCurrentState(after);
	
	ASSERT_TRUE(flags_are_same_state(snapshot, after));
	ASSERT_EQ( (int)cpu.cycleCount(), 4);
}

TEST_F(CPUOpcodeTest, opcode_test_0x01)
{
	SetUp({0x01, 0xFE, 0xCA});

	// Perform exactly 1 cycle
	cpu.cycle();

	// Grab after state of register bank
	RegBank after;
	regs.copyCurrentState(after);

	ASSERT_TRUE(flags_are_same_state(snapshot, after));
	ASSERT_EQ(after.BC(), 0xCAFE);
	ASSERT_EQ(cpu.cycleCount(), 12);
}

TEST_F(CPUOpcodeTest, opcode_test_0x02)
{
}


int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}

