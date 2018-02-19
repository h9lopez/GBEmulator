#include "gtest/gtest.h"
#include "stdafx.h"
#include <iostream>
#include <array>
#include "CPUCore.h"
#include "OpcodesTest.hpp"


TEST_F(CPUOpcodeTest, opcode_test_0x0_83)
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

TEST_F(CPUOpcodeTest, opcode_test_0x1_84)
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
	ASSERT_EQ(after.BC(), 0xcafe);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x11_75)
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
	ASSERT_EQ(after.DE(), 0xcabe);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x21_30)
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
	ASSERT_EQ(after.HL(), 0xbabe);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x31_80)
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
	ASSERT_EQ(after.SP(), 0xbabe);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x2_38)
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

TEST_F(CPUOpcodeTest, opcode_test_0x12_78)
{
    SetUp({ 0x12 });

	regs.A(0x10);
	regs.DE(0xbeef);
	mem.writeByte(0xBEEF, 0x20);

	
	

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
	
	ASSERT_EQ(memAfter.readByte(0xBEEF), 0x10);
}

TEST_F(CPUOpcodeTest, opcode_test_0x22_94)
{
    SetUp({ 0x22 });

	regs.A(0x77);
	regs.HL(0xbeef);
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
	ASSERT_EQ(after.HL(), 0xbef0);
	ASSERT_EQ(memAfter.readByte(0xBEEF), 0x77);
}

TEST_F(CPUOpcodeTest, opcode_test_0x32_34)
{
    SetUp({ 0x32 });

	regs.A(0x77);
	regs.HL(0xbeef);
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
	ASSERT_EQ(after.HL(), 0xbeee);
	ASSERT_EQ(memAfter.readByte(0xBEEF), 0x77);
}

TEST_F(CPUOpcodeTest, opcode_test_0x3_54)
{
    SetUp({ 0x03 });

	regs.BC(0x19);
	

	
	

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
	ASSERT_EQ(after.BC(), 0x1a);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x13_92)
{
    SetUp({ 0x13 });

	regs.DE(0x19);
	

	
	

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
	ASSERT_EQ(after.DE(), 0x1a);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x23_52)
{
    SetUp({ 0x23 });

	regs.HL(0x19);
	

	
	

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
	ASSERT_EQ(after.HL(), 0x1a);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x33_59)
{
    SetUp({ 0x33 });

	regs.SP(0x19);
	

	
	

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
	ASSERT_EQ(after.SP(), 0x1a);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4_47)
{
    SetUp({ 0x04 });

	regs.B(0x5);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.B(), 0x6);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4_84)
{
    SetUp({ 0x04 });

	regs.B(0xf);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.B(), 0x10);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x14_79)
{
    SetUp({ 0x14 });

	regs.D(0x5);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.D(), 0x6);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x14_9)
{
    SetUp({ 0x14 });

	regs.D(0xf);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.D(), 0x10);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x24_53)
{
    SetUp({ 0x24 });

	regs.H(0x5);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.H(), 0x6);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x24_89)
{
    SetUp({ 0x24 });

	regs.H(0xf);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.H(), 0x10);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x34_100)
{
    SetUp({ 0x34 });

	regs.HL(0xbeef);
	mem.writeByte(0xBEEF, 0x05);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 12);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	
	ASSERT_EQ(memAfter.readByte(0xBEEF), 0x06);
}

TEST_F(CPUOpcodeTest, opcode_test_0x34_7)
{
    SetUp({ 0x34 });

	regs.HL(0xbeef);
	mem.writeByte(0xBEEF, 0x0F);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 12);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	
	ASSERT_EQ(memAfter.readByte(0xBEEF), 0x10);
}

TEST_F(CPUOpcodeTest, opcode_test_0xc_76)
{
    SetUp({ 0x0c });

	regs.C(0x5);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.C(), 0x6);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xc_57)
{
    SetUp({ 0x0c });

	regs.C(0xf);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.C(), 0x10);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x1c_22)
{
    SetUp({ 0x1c });

	regs.E(0x5);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.E(), 0x6);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x1c_62)
{
    SetUp({ 0x1c });

	regs.E(0xf);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.E(), 0x10);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x2c_22)
{
    SetUp({ 0x2c });

	regs.L(0x5);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.L(), 0x6);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x2c_19)
{
    SetUp({ 0x2c });

	regs.L(0xf);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.L(), 0x10);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x3c_61)
{
    SetUp({ 0x3c });

	regs.A(0x5);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.A(), 0x6);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x3c_80)
{
    SetUp({ 0x3c });

	regs.A(0xf);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.A(), 0x10);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x2f_65)
{
    SetUp({ 0x2f });

	regs.A(0x45);
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.A(), 0xba);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x3f_77)
{
    SetUp({ 0x3f });

	
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x37_82)
{
    SetUp({ 0x37 });

	
	

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 1);
	
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x40_20)
{
    SetUp({ 0x40 });

	regs.B(0x69);
	

	
	

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
	ASSERT_EQ(after.B(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x41_69)
{
    SetUp({ 0x41 });

	regs.C(0x40);
	regs.B(0x69);
	

	
	

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
	ASSERT_EQ(after.B(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x42_88)
{
    SetUp({ 0x42 });

	regs.B(0x69);
	regs.D(0x40);
	

	
	

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
	ASSERT_EQ(after.B(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x43_82)
{
    SetUp({ 0x43 });

	regs.B(0x69);
	regs.E(0x40);
	

	
	

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
	ASSERT_EQ(after.B(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x44_39)
{
    SetUp({ 0x44 });

	regs.H(0x40);
	regs.B(0x69);
	

	
	

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
	ASSERT_EQ(after.B(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x45_32)
{
    SetUp({ 0x45 });

	regs.B(0x69);
	regs.L(0x40);
	

	
	

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
	ASSERT_EQ(after.B(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x47_97)
{
    SetUp({ 0x47 });

	regs.A(0x40);
	regs.B(0x69);
	

	
	

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
	ASSERT_EQ(after.B(), 0x40);
	
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}