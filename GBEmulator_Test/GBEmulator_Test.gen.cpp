#include "gtest/gtest.h"
#include "stdafx.h"
#include <iostream>
#include <array>
#include "CPUCore.h"
#include "OpcodesTest.hpp"


TEST_F(CPUOpcodeTest, opcode_test_0x0_96)
{
    SetUp({ 0x00 });

	
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x1_4)
{
    SetUp({ 0x01, 0xfe, 0xca });

	
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x11_28)
{
    SetUp({ 0x11, 0xbe, 0xca });

	
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x21_77)
{
    SetUp({ 0x21, 0xbe, 0xba });

	
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x31_33)
{
    SetUp({ 0x31, 0xbe, 0xba });

	
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x2_55)
{
    SetUp({ 0x02 });

	regs.A(0x21);
	regs.BC(0xbeef);
	mem.writeByte(0xBEEF, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x12_34)
{
    SetUp({ 0x12 });

	regs.A(0x10);
	regs.DE(0xbeef);
	mem.writeByte(0xBEEF, 0x20);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x22_24)
{
    SetUp({ 0x22 });

	regs.A(0x77);
	regs.HL(0xbeef);
	mem.writeByte(0xBEEF, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x32_80)
{
    SetUp({ 0x32 });

	regs.A(0x77);
	regs.HL(0xbeef);
	mem.writeByte(0xBEEF, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x3_9)
{
    SetUp({ 0x03 });

	regs.BC(0x19);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x13_22)
{
    SetUp({ 0x13 });

	regs.DE(0x19);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x23_79)
{
    SetUp({ 0x23 });

	regs.HL(0x19);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x33_91)
{
    SetUp({ 0x33 });

	regs.SP(0x19);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x4_55)
{
    SetUp({ 0x04 });

	regs.B(0x5);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x4_86)
{
    SetUp({ 0x04 });

	regs.B(0xf);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x14_10)
{
    SetUp({ 0x14 });

	regs.D(0x5);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x14_74)
{
    SetUp({ 0x14 });

	regs.D(0xf);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x24_13)
{
    SetUp({ 0x24 });

	regs.H(0x5);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x24_80)
{
    SetUp({ 0x24 });

	regs.H(0xf);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x34_56)
{
    SetUp({ 0x34 });

	regs.HL(0xbeef);
	mem.writeByte(0xBEEF, 0x05);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x34_27)
{
    SetUp({ 0x34 });

	regs.HL(0xbeef);
	mem.writeByte(0xBEEF, 0x0F);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0xc_6)
{
    SetUp({ 0x0c });

	regs.C(0x5);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0xc_35)
{
    SetUp({ 0x0c });

	regs.C(0xf);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x1c_77)
{
    SetUp({ 0x1c });

	regs.E(0x5);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x1c_11)
{
    SetUp({ 0x1c });

	regs.E(0xf);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x2c_33)
{
    SetUp({ 0x2c });

	regs.L(0x5);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x2c_99)
{
    SetUp({ 0x2c });

	regs.L(0xf);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x3c_87)
{
    SetUp({ 0x3c });

	regs.A(0x5);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x3c_30)
{
    SetUp({ 0x3c });

	regs.A(0xf);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x2f_76)
{
    SetUp({ 0x2f });

	regs.A(0x45);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x3f_9)
{
    SetUp({ 0x3f });

	
	
	regs.flagZero(0);
	regs.flagSubtract(1);
	regs.flagHalfCarry(1);
	regs.flagCarry(1);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x37_60)
{
    SetUp({ 0x37 });

	
	
	regs.flagZero(0);
	regs.flagSubtract(1);
	regs.flagHalfCarry(1);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x40_11)
{
    SetUp({ 0x40 });

	regs.B(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x41_12)
{
    SetUp({ 0x41 });

	regs.C(0x40);
	regs.B(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x42_91)
{
    SetUp({ 0x42 });

	regs.B(0x69);
	regs.D(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x43_5)
{
    SetUp({ 0x43 });

	regs.B(0x69);
	regs.E(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x44_50)
{
    SetUp({ 0x44 });

	regs.H(0x40);
	regs.B(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x45_52)
{
    SetUp({ 0x45 });

	regs.B(0x69);
	regs.L(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x47_93)
{
    SetUp({ 0x47 });

	regs.A(0x40);
	regs.B(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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

TEST_F(CPUOpcodeTest, opcode_test_0x48_20)
{
    SetUp({ 0x48 });

	regs.C(0x69);
	regs.B(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.C(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x49_96)
{
    SetUp({ 0x49 });

	regs.C(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.C(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4a_65)
{
    SetUp({ 0x4a });

	regs.C(0x69);
	regs.D(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.C(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4b_40)
{
    SetUp({ 0x4b });

	regs.C(0x69);
	regs.E(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.C(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4c_89)
{
    SetUp({ 0x4c });

	regs.H(0x21);
	regs.C(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.C(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4d_74)
{
    SetUp({ 0x4d });

	regs.C(0x69);
	regs.L(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.C(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4f_43)
{
    SetUp({ 0x4f });

	regs.A(0x21);
	regs.C(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.C(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x50_26)
{
    SetUp({ 0x50 });

	regs.B(0x40);
	regs.D(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x51_14)
{
    SetUp({ 0x51 });

	regs.C(0x40);
	regs.D(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x52_67)
{
    SetUp({ 0x52 });

	regs.D(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.D(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x53_12)
{
    SetUp({ 0x53 });

	regs.E(0x40);
	regs.D(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x54_69)
{
    SetUp({ 0x54 });

	regs.H(0x40);
	regs.D(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x55_77)
{
    SetUp({ 0x55 });

	regs.D(0x69);
	regs.L(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x57_42)
{
    SetUp({ 0x57 });

	regs.A(0x40);
	regs.D(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x58_66)
{
    SetUp({ 0x58 });

	regs.B(0x21);
	regs.E(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.E(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x59_37)
{
    SetUp({ 0x59 });

	regs.C(0x69);
	regs.E(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.E(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x5a_39)
{
    SetUp({ 0x5a });

	regs.E(0x69);
	regs.D(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.E(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x5b_19)
{
    SetUp({ 0x5b });

	regs.E(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.E(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x5c_43)
{
    SetUp({ 0x5c });

	regs.H(0x21);
	regs.E(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.E(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x5d_41)
{
    SetUp({ 0x5d });

	regs.E(0x69);
	regs.L(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.E(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x5f_94)
{
    SetUp({ 0x5f });

	regs.A(0x21);
	regs.E(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.E(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x60_99)
{
    SetUp({ 0x60 });

	regs.H(0x69);
	regs.B(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.H(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x61_93)
{
    SetUp({ 0x61 });

	regs.H(0x69);
	regs.C(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.H(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x62_26)
{
    SetUp({ 0x62 });

	regs.H(0x69);
	regs.D(0x60);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.H(), 0x60);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x63_73)
{
    SetUp({ 0x63 });

	regs.H(0x69);
	regs.E(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.H(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x64_6)
{
    SetUp({ 0x64 });

	regs.H(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.H(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x65_86)
{
    SetUp({ 0x65 });

	regs.H(0x69);
	regs.L(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.H(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x67_54)
{
    SetUp({ 0x67 });

	regs.A(0x40);
	regs.H(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.H(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x68_41)
{
    SetUp({ 0x68 });

	regs.B(0x21);
	regs.L(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.L(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x69_4)
{
    SetUp({ 0x69 });

	regs.C(0x69);
	regs.L(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.L(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6a_68)
{
    SetUp({ 0x6a });

	regs.D(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.L(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6b_48)
{
    SetUp({ 0x6b });

	regs.E(0x69);
	regs.L(0x40);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.L(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6c_70)
{
    SetUp({ 0x6c });

	regs.H(0x21);
	regs.L(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.L(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6d_14)
{
    SetUp({ 0x6d });

	regs.L(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.L(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6f_20)
{
    SetUp({ 0x6f });

	regs.A(0x21);
	regs.L(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.L(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x78_23)
{
    SetUp({ 0x78 });

	regs.A(0x69);
	regs.B(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.A(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x79_21)
{
    SetUp({ 0x79 });

	regs.A(0x69);
	regs.C(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.A(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x7a_28)
{
    SetUp({ 0x7a });

	regs.A(0x18);
	regs.D(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.A(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x7b_46)
{
    SetUp({ 0x7b });

	regs.A(0x40);
	regs.E(0x69);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.A(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x7c_96)
{
    SetUp({ 0x7c });

	regs.A(0x69);
	regs.H(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.A(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x7d_68)
{
    SetUp({ 0x7d });

	regs.A(0x19);
	regs.L(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.A(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x7f_48)
{
    SetUp({ 0x7f });

	regs.A(0x21);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.A(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x46_66)
{
    SetUp({ 0x46 });

	regs.B(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.B(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4e_59)
{
    SetUp({ 0x4e });

	regs.C(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.C(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x56_45)
{
    SetUp({ 0x56 });

	regs.D(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.D(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x5e_10)
{
    SetUp({ 0x5e });

	regs.E(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.E(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x66_94)
{
    SetUp({ 0x66 });

	regs.H(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.H(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6e_20)
{
    SetUp({ 0x6e });

	regs.L(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.L(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x7e_43)
{
    SetUp({ 0x7e });

	regs.A(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	ASSERT_EQ(after.A(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x70_50)
{
    SetUp({ 0x70 });

	regs.B(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	
	ASSERT_EQ(memAfter.readByte(0xBABE), 0x21);
}

TEST_F(CPUOpcodeTest, opcode_test_0x71_50)
{
    SetUp({ 0x71 });

	regs.C(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	
	ASSERT_EQ(memAfter.readByte(0xBABE), 0x21);
}

TEST_F(CPUOpcodeTest, opcode_test_0x72_3)
{
    SetUp({ 0x72 });

	regs.D(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	
	ASSERT_EQ(memAfter.readByte(0xBABE), 0x21);
}

TEST_F(CPUOpcodeTest, opcode_test_0x73_84)
{
    SetUp({ 0x73 });

	regs.E(0x21);
	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	
	ASSERT_EQ(memAfter.readByte(0xBABE), 0x21);
}

TEST_F(CPUOpcodeTest, opcode_test_0x74_16)
{
    SetUp({ 0x74 });

	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	
	ASSERT_EQ(memAfter.readByte(0xBABE), 0xBA);
}

TEST_F(CPUOpcodeTest, opcode_test_0x75_39)
{
    SetUp({ 0x75 });

	regs.HL(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

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
	
	ASSERT_EQ(memAfter.readByte(0xBABE), 0xBE);
}

TEST_F(CPUOpcodeTest, opcode_test_0x88_2)
{
    SetUp({ 0x88 });

	regs.A(0x21);
	regs.B(0x22);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(1);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 4);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x43);
	ASSERT_EQ(after.B(), 0x22);
	
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}