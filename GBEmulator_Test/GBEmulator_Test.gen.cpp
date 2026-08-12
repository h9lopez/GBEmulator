#include "gtest/gtest.h"
#include <iostream>
#include <array>
#include <cpu/CPUCore.h>
#include "OpcodesTest.hpp"


TEST_F(CPUOpcodeTest, opcode_test_0x0_35)
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

TEST_F(CPUOpcodeTest, opcode_test_0x1_83)
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

TEST_F(CPUOpcodeTest, opcode_test_0x11_65)
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

TEST_F(CPUOpcodeTest, opcode_test_0x21_63)
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

TEST_F(CPUOpcodeTest, opcode_test_0x31_70)
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

TEST_F(CPUOpcodeTest, opcode_test_0x2_74)
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

TEST_F(CPUOpcodeTest, opcode_test_0x12_31)
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

TEST_F(CPUOpcodeTest, opcode_test_0x22_10)
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

TEST_F(CPUOpcodeTest, opcode_test_0x32_74)
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

TEST_F(CPUOpcodeTest, opcode_test_0x3_86)
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

TEST_F(CPUOpcodeTest, opcode_test_0x13_56)
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

TEST_F(CPUOpcodeTest, opcode_test_0x23_71)
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

TEST_F(CPUOpcodeTest, opcode_test_0x33_57)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4_100)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4_89)
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

TEST_F(CPUOpcodeTest, opcode_test_0x14_20)
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

TEST_F(CPUOpcodeTest, opcode_test_0x14_19)
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

TEST_F(CPUOpcodeTest, opcode_test_0x24_96)
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

TEST_F(CPUOpcodeTest, opcode_test_0x24_43)
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

TEST_F(CPUOpcodeTest, opcode_test_0x34_13)
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

TEST_F(CPUOpcodeTest, opcode_test_0x34_2)
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

TEST_F(CPUOpcodeTest, opcode_test_0xc_37)
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

TEST_F(CPUOpcodeTest, opcode_test_0xc_56)
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

TEST_F(CPUOpcodeTest, opcode_test_0x1c_55)
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

TEST_F(CPUOpcodeTest, opcode_test_0x1c_32)
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

TEST_F(CPUOpcodeTest, opcode_test_0x2c_28)
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

TEST_F(CPUOpcodeTest, opcode_test_0x2c_26)
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

TEST_F(CPUOpcodeTest, opcode_test_0x3c_16)
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

TEST_F(CPUOpcodeTest, opcode_test_0x3c_2)
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

TEST_F(CPUOpcodeTest, opcode_test_0x3f_60)
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

TEST_F(CPUOpcodeTest, opcode_test_0x37_47)
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

TEST_F(CPUOpcodeTest, opcode_test_0x40_100)
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

TEST_F(CPUOpcodeTest, opcode_test_0x41_96)
{
    SetUp({ 0x41 });

	regs.B(0x69);
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
	ASSERT_EQ(after.B(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x42_85)
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

TEST_F(CPUOpcodeTest, opcode_test_0x43_30)
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

TEST_F(CPUOpcodeTest, opcode_test_0x44_62)
{
    SetUp({ 0x44 });

	regs.B(0x69);
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
	ASSERT_EQ(after.B(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x45_48)
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

TEST_F(CPUOpcodeTest, opcode_test_0x47_28)
{
    SetUp({ 0x47 });

	regs.B(0x69);
	regs.A(0x40);
	
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

TEST_F(CPUOpcodeTest, opcode_test_0x48_92)
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

TEST_F(CPUOpcodeTest, opcode_test_0x49_47)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4a_50)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4b_30)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4c_15)
{
    SetUp({ 0x4c });

	regs.C(0x69);
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
	ASSERT_EQ(after.C(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x4d_3)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4f_13)
{
    SetUp({ 0x4f });

	regs.C(0x69);
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
	ASSERT_EQ(after.C(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x50_17)
{
    SetUp({ 0x50 });

	regs.D(0x69);
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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x51_22)
{
    SetUp({ 0x51 });

	regs.D(0x69);
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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x52_30)
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

TEST_F(CPUOpcodeTest, opcode_test_0x53_43)
{
    SetUp({ 0x53 });

	regs.D(0x69);
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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x54_86)
{
    SetUp({ 0x54 });

	regs.D(0x69);
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
	ASSERT_EQ(after.D(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x55_22)
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

TEST_F(CPUOpcodeTest, opcode_test_0x57_16)
{
    SetUp({ 0x57 });

	regs.D(0x69);
	regs.A(0x40);
	
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

TEST_F(CPUOpcodeTest, opcode_test_0x58_10)
{
    SetUp({ 0x58 });

	regs.E(0x69);
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
	ASSERT_EQ(after.E(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x59_74)
{
    SetUp({ 0x59 });

	regs.E(0x69);
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
	ASSERT_EQ(after.E(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x5a_78)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5b_53)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5c_68)
{
    SetUp({ 0x5c });

	regs.E(0x69);
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
	ASSERT_EQ(after.E(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x5d_28)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5f_65)
{
    SetUp({ 0x5f });

	regs.E(0x69);
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
	ASSERT_EQ(after.E(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x60_68)
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

TEST_F(CPUOpcodeTest, opcode_test_0x61_29)
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

TEST_F(CPUOpcodeTest, opcode_test_0x62_35)
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

TEST_F(CPUOpcodeTest, opcode_test_0x63_17)
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

TEST_F(CPUOpcodeTest, opcode_test_0x64_15)
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

TEST_F(CPUOpcodeTest, opcode_test_0x65_78)
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

TEST_F(CPUOpcodeTest, opcode_test_0x67_13)
{
    SetUp({ 0x67 });

	regs.H(0x69);
	regs.A(0x40);
	
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

TEST_F(CPUOpcodeTest, opcode_test_0x68_53)
{
    SetUp({ 0x68 });

	regs.L(0x69);
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
	ASSERT_EQ(after.L(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x69_38)
{
    SetUp({ 0x69 });

	regs.L(0x69);
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
	ASSERT_EQ(after.L(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6a_17)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6b_63)
{
    SetUp({ 0x6b });

	regs.L(0x40);
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
	ASSERT_EQ(after.L(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6c_48)
{
    SetUp({ 0x6c });

	regs.L(0x69);
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
	ASSERT_EQ(after.L(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x6d_66)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6f_1)
{
    SetUp({ 0x6f });

	regs.L(0x69);
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
	ASSERT_EQ(after.L(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x78_98)
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

TEST_F(CPUOpcodeTest, opcode_test_0x79_40)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7a_82)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7b_32)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7c_61)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7d_79)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7f_33)
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

TEST_F(CPUOpcodeTest, opcode_test_0x46_30)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4e_94)
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

TEST_F(CPUOpcodeTest, opcode_test_0x56_95)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5e_44)
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

TEST_F(CPUOpcodeTest, opcode_test_0x66_2)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6e_39)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7e_87)
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

TEST_F(CPUOpcodeTest, opcode_test_0x70_46)
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

TEST_F(CPUOpcodeTest, opcode_test_0x71_41)
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

TEST_F(CPUOpcodeTest, opcode_test_0x72_97)
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

TEST_F(CPUOpcodeTest, opcode_test_0x73_36)
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

TEST_F(CPUOpcodeTest, opcode_test_0x74_52)
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

TEST_F(CPUOpcodeTest, opcode_test_0x75_2)
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

TEST_F(CPUOpcodeTest, opcode_test_0x88_50)
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
	ASSERT_EQ(after.A(), 0x44);
	ASSERT_EQ(after.B(), 0x22);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x90_57)
{
    SetUp({ 0x90 });

	regs.A(0xa);
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x5);
	ASSERT_EQ(after.B(), 0x5);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x90_97)
{
    SetUp({ 0x90 });

	regs.A(0x0);
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 1);
	ASSERT_EQ(after.A(), 0xf1);
	ASSERT_EQ(after.B(), 0xf);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x96_64)
{
    SetUp({ 0x96 });

	regs.A(0x72);
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

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x9);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xd6_94)
{
    SetUp({ 0xd6, 0x69 });

	regs.A(0x72);
	
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

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x9);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x98_34)
{
    SetUp({ 0x98 });

	regs.A(0x70);
	regs.B(0x10);
	
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x5f);
	ASSERT_EQ(after.B(), 0x10);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xa0_32)
{
    SetUp({ 0xa0 });

	regs.A(0x69);
	regs.B(0x44);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x40);
	ASSERT_EQ(after.B(), 0x44);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xa4_18)
{
    SetUp({ 0xa4 });

	regs.A(0x69);
	regs.H(0x44);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x40);
	ASSERT_EQ(after.H(), 0x44);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xe6_32)
{
    SetUp({ 0xe6, 0x44 });

	regs.A(0x69);
	
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

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x40);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xb4_24)
{
    SetUp({ 0xb4 });

	regs.A(0x69);
	regs.H(0x44);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x6d);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xb6_53)
{
    SetUp({ 0xb6 });

	regs.A(0x69);
	regs.HL(0xbeef);
	mem.writeByte(0xBEEF, 0x44);
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

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x6d);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xb7_16)
{
    SetUp({ 0xb7 });

	regs.A(0x69);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x69);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xb8_65)
{
    SetUp({ 0xb8 });

	regs.A(0x0);
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 1);
	ASSERT_EQ(after.A(), 0x0);
	ASSERT_EQ(after.B(), 0xf);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xbe_88)
{
    SetUp({ 0xbe });

	regs.A(0x72);
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

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x72);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xc1_5)
{
    SetUp({ 0xc1 });

	regs.SP(0xcafe);
	regs.BC(0xffff);
	mem.writeByte(0xCAFE, 0x69);
	mem.writeByte(0xCAFF, 0x70);
	mem.writeByte(0xCB00, 0x71);
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
	ASSERT_EQ(after.SP(), 0xcb00);
	ASSERT_EQ(after.BC(), 0x7069);
	ASSERT_EQ(memAfter.readByte(0xCAFE), 0x69);
	ASSERT_EQ(memAfter.readByte(0xCAFF), 0x70);
	ASSERT_EQ(memAfter.readByte(0xCB00), 0x71);
}

TEST_F(CPUOpcodeTest, opcode_test_0xf1_24)
{
    SetUp({ 0xf1 });

	regs.SP(0xcafe);
	regs.A(0x21);
	mem.writeByte(0xCAFE, 0x69);
	mem.writeByte(0xCAFF, 0x70);
	mem.writeByte(0xCB00, 0x71);
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.SP(), 0xcb00);
	ASSERT_EQ(after.A(), 0x70);
	ASSERT_EQ(memAfter.readByte(0xCAFE), 0x69);
	ASSERT_EQ(memAfter.readByte(0xCAFF), 0x70);
	ASSERT_EQ(memAfter.readByte(0xCB00), 0x71);
}

TEST_F(CPUOpcodeTest, opcode_test_0xc5_82)
{
    SetUp({ 0xc5 });

	regs.SP(0xcafe);
	regs.BC(0xbeef);
	mem.writeByte(0xCAFC, 0x68);
	mem.writeByte(0xCAFD, 0x69);
	mem.writeByte(0xCAFE, 0x70);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 16);

	ASSERT_EQ(after.flagZero(), snapshot.flagZero());
	ASSERT_EQ(after.flagSubtract(), snapshot.flagSubtract());
	ASSERT_EQ(after.flagHalfCarry(), snapshot.flagHalfCarry());
	ASSERT_EQ(after.flagCarry(), snapshot.flagCarry());
	ASSERT_EQ(after.SP(), 0xcafc);
	ASSERT_EQ(after.BC(), 0xbeef);
	ASSERT_EQ(memAfter.readByte(0xCAFC), 0xEF);
	ASSERT_EQ(memAfter.readByte(0xCAFD), 0xBE);
	ASSERT_EQ(memAfter.readByte(0xCAFE), 0x70);
}

TEST_F(CPUOpcodeTest, opcode_test_0xf5_24)
{
    SetUp({ 0xf5 });

	regs.SP(0xcafe);
	regs.A(0xbe);
	mem.writeByte(0xCAFC, 0x68);
	mem.writeByte(0xCAFD, 0x69);
	mem.writeByte(0xCAFE, 0x70);
	regs.flagZero(1);
	regs.flagSubtract(1);
	regs.flagHalfCarry(1);
	regs.flagCarry(0);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 16);

	ASSERT_EQ(after.flagZero(), 1);
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.SP(), 0xcafc);
	ASSERT_EQ(after.A(), 0xbe);
	ASSERT_EQ(memAfter.readByte(0xCAFC), 0xE0);
	ASSERT_EQ(memAfter.readByte(0xCAFD), 0xBE);
	ASSERT_EQ(memAfter.readByte(0xCAFE), 0x70);
}

TEST_F(CPUOpcodeTest, opcode_test_0xc0_42)
{
    SetUp({ 0xc0 });

	regs.SP(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	mem.writeByte(0xBABF, 0x70);
	mem.writeByte(0xBAC0, 0x71);
	regs.flagZero(0);
	regs.flagSubtract(1);
	regs.flagHalfCarry(1);
	regs.flagCarry(1);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 20);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 1);
	ASSERT_EQ(after.SP(), 0xbac0);
	ASSERT_EQ(after.PC(), 0x7069);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xc0_63)
{
    SetUp({ 0xc0 });

	regs.SP(0xbabe);
	mem.writeByte(0xBABE, 0x69);
	mem.writeByte(0xBABF, 0x70);
	mem.writeByte(0xBAC0, 0x71);
	regs.flagZero(1);
	regs.flagSubtract(0);
	regs.flagHalfCarry(1);
	regs.flagCarry(1);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 8);

	ASSERT_EQ(after.flagZero(), 1);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 1);
	ASSERT_EQ(after.flagCarry(), 1);
	ASSERT_EQ(after.SP(), 0xbabe);
	ASSERT_EQ(after.PC(), 0x1);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x80_70)
{
    SetUp({ 0x80 });

	regs.A(0x10);
	regs.B(0x20);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x30);
	ASSERT_EQ(after.B(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x81_57)
{
    SetUp({ 0x81 });

	regs.A(0x10);
	regs.C(0x20);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x30);
	ASSERT_EQ(after.C(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x82_77)
{
    SetUp({ 0x82 });

	regs.A(0x10);
	regs.D(0x20);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x30);
	ASSERT_EQ(after.D(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x83_9)
{
    SetUp({ 0x83 });

	regs.A(0x10);
	regs.E(0x20);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x30);
	ASSERT_EQ(after.E(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x84_83)
{
    SetUp({ 0x84 });

	regs.A(0x10);
	regs.H(0x20);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x30);
	ASSERT_EQ(after.H(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x85_83)
{
    SetUp({ 0x85 });

	regs.A(0x10);
	regs.L(0x20);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x30);
	ASSERT_EQ(after.L(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x86_73)
{
    SetUp({ 0x86 });

	regs.A(0x10);
	regs.HL(0xbeef);
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

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x30);
	ASSERT_EQ(after.HL(), 0xbeef);
	ASSERT_EQ(memAfter.readByte(0xBEEF), 0x20);
}

TEST_F(CPUOpcodeTest, opcode_test_0x87_21)
{
    SetUp({ 0x87 });

	regs.A(0x10);
	
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
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x89_41)
{
    SetUp({ 0x89 });

	regs.A(0x10);
	regs.C(0x20);
	
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
	ASSERT_EQ(after.A(), 0x31);
	ASSERT_EQ(after.C(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x8a_81)
{
    SetUp({ 0x8a });

	regs.A(0x10);
	regs.D(0x20);
	
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
	ASSERT_EQ(after.A(), 0x31);
	ASSERT_EQ(after.D(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x8b_96)
{
    SetUp({ 0x8b });

	regs.A(0x10);
	regs.E(0x20);
	
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
	ASSERT_EQ(after.A(), 0x31);
	ASSERT_EQ(after.E(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x8c_70)
{
    SetUp({ 0x8c });

	regs.A(0x10);
	regs.H(0x20);
	
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
	ASSERT_EQ(after.A(), 0x31);
	ASSERT_EQ(after.H(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x8d_98)
{
    SetUp({ 0x8d });

	regs.A(0x10);
	regs.L(0x20);
	
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
	ASSERT_EQ(after.A(), 0x31);
	ASSERT_EQ(after.L(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x8e_50)
{
    SetUp({ 0x8e });

	regs.A(0x10);
	regs.HL(0xbeef);
	mem.writeByte(0xBEEF, 0x20);
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(1);

	
	

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 8);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x31);
	ASSERT_EQ(after.HL(), 0xbeef);
	ASSERT_EQ(memAfter.readByte(0xBEEF), 0x20);
}

TEST_F(CPUOpcodeTest, opcode_test_0x8f_64)
{
    SetUp({ 0x8f });

	regs.A(0x10);
	
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
	ASSERT_EQ(after.A(), 0x21);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x91_55)
{
    SetUp({ 0x91 });

	regs.A(0x30);
	regs.C(0x20);
	
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x10);
	ASSERT_EQ(after.C(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x92_99)
{
    SetUp({ 0x92 });

	regs.A(0x30);
	regs.D(0x20);
	
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x10);
	ASSERT_EQ(after.D(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x93_54)
{
    SetUp({ 0x93 });

	regs.A(0x30);
	regs.E(0x20);
	
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x10);
	ASSERT_EQ(after.E(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x94_61)
{
    SetUp({ 0x94 });

	regs.A(0x30);
	regs.H(0x20);
	
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x10);
	ASSERT_EQ(after.H(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x95_35)
{
    SetUp({ 0x95 });

	regs.A(0x30);
	regs.L(0x20);
	
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
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x10);
	ASSERT_EQ(after.L(), 0x20);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x97_57)
{
    SetUp({ 0x97 });

	regs.A(0x30);
	
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

	ASSERT_EQ(after.flagZero(), 1);
	ASSERT_EQ(after.flagSubtract(), 1);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x0);
	
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}