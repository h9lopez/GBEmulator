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
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

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
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 12);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), snapshot.flagSubtract());
	ASSERT_EQ(after.flagHalfCarry(), snapshot.flagHalfCarry());
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x11)
{
    SetUp({ 0x11, 0xbe, 0xca });

	
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 12);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), snapshot.flagSubtract());
	ASSERT_EQ(after.flagHalfCarry(), snapshot.flagHalfCarry());
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x21)
{
    SetUp({ 0x21, 0xbe, 0xba });

	
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 12);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), snapshot.flagSubtract());
	ASSERT_EQ(after.flagHalfCarry(), snapshot.flagHalfCarry());
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x31)
{
    SetUp({ 0x31, 0xbe, 0xba });

	
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 12);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), snapshot.flagSubtract());
	ASSERT_EQ(after.flagHalfCarry(), snapshot.flagHalfCarry());
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x2)
{
    SetUp({ 0x02 });

	regs.A(0x21);
	regs.BC(0xbeef);
	mem.writeByte(0xBEEF, 0x69);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 8);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), snapshot.flagSubtract());
	ASSERT_EQ(after.flagHalfCarry(), snapshot.flagHalfCarry());
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	
	ASSERT_EQ(memAfter.readByte(0xBEEF), 0x21);
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}