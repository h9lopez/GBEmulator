#include "gtest/gtest.h"
#include "stdafx.h"
#include <iostream>
#include <array>
#include "CPUCore.h"
#include "OpcodesTest.hpp"


TEST_F(CPUOpcodeTest, opcode_test_0x0)
{
    SetUp({ 0x00 });

    cpu.cycle();

    RegBank after;
    regs.copyCurrentState(after);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), snapshot.flagSubtract());
	ASSERT_EQ(after.flagHalfCarry(), snapshot.flagHalfCarry());
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x1)
{
    SetUp({ 0x01, 0xfe, 0xca });

    cpu.cycle();

    RegBank after;
    regs.copyCurrentState(after);

    ASSERT_EQ(cpu.cycleCount(), 12);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), snapshot.flagSubtract());
	ASSERT_EQ(after.flagHalfCarry(), snapshot.flagHalfCarry());
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.BC(), 0xcafe);
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}