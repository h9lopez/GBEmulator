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
	// Grab empty snapshot of regs before test starts
	RegBank snapshot;
	regs.copyCurrentState(snapshot);

	// Opcode list to execute
	std::array<ByteType, 1> oplist = { 0x00 };
	// Load opcodes into ram
	ASSERT_TRUE(ROMLoader::fromRawBytes(oplist.begin(), oplist.end(), ram));
	
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
	// Grab empty snapshot of regs before test starts
	RegBank snapshot;
	regs.copyCurrentState(snapshot);

	// Opcode list to execute, opcode then byte to load
	std::array<ByteType, 3> oplist = { 0x01, 0xFE, 0xCA };
	// Load opcodes into ram
	ASSERT_TRUE(ROMLoader::fromRawBytes(oplist.begin(), oplist.end(), ram));

	// Perform exactly 1 cycle
	cpu.cycle();

	// Grab after state of register bank
	RegBank after;
	regs.copyCurrentState(after);

	ASSERT_TRUE(flags_are_same_state(snapshot, after));
	ASSERT_EQ(after.BC(), 0xCAFE);
	ASSERT_EQ(cpu.cycleCount(), 12);
}


int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}

