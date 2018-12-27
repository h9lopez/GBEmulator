#include "gtest/gtest.h"
#include <iostream>
#include <array>
#include "CPUCore.h"
#include "OpcodesTest.hpp"


TEST_F(CPUOpcodeTest, opcode_test_0x0_3)
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

TEST_F(CPUOpcodeTest, opcode_test_0x1_77)
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

TEST_F(CPUOpcodeTest, opcode_test_0x11_55)
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

TEST_F(CPUOpcodeTest, opcode_test_0x21_58)
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

TEST_F(CPUOpcodeTest, opcode_test_0x31_85)
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

TEST_F(CPUOpcodeTest, opcode_test_0x2_95)
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

TEST_F(CPUOpcodeTest, opcode_test_0x12_11)
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

TEST_F(CPUOpcodeTest, opcode_test_0x22_26)
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

TEST_F(CPUOpcodeTest, opcode_test_0x32_39)
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

TEST_F(CPUOpcodeTest, opcode_test_0x3_22)
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

TEST_F(CPUOpcodeTest, opcode_test_0x13_78)
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

TEST_F(CPUOpcodeTest, opcode_test_0x23_21)
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

TEST_F(CPUOpcodeTest, opcode_test_0x33_8)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4_36)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4_30)
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

TEST_F(CPUOpcodeTest, opcode_test_0x14_98)
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

TEST_F(CPUOpcodeTest, opcode_test_0x14_9)
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

TEST_F(CPUOpcodeTest, opcode_test_0x24_59)
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

TEST_F(CPUOpcodeTest, opcode_test_0x24_19)
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

TEST_F(CPUOpcodeTest, opcode_test_0x34_31)
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

TEST_F(CPUOpcodeTest, opcode_test_0x34_11)
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

TEST_F(CPUOpcodeTest, opcode_test_0xc_89)
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

TEST_F(CPUOpcodeTest, opcode_test_0xc_75)
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

TEST_F(CPUOpcodeTest, opcode_test_0x1c_82)
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

TEST_F(CPUOpcodeTest, opcode_test_0x1c_31)
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

TEST_F(CPUOpcodeTest, opcode_test_0x2c_51)
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

TEST_F(CPUOpcodeTest, opcode_test_0x2c_52)
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

TEST_F(CPUOpcodeTest, opcode_test_0x3c_22)
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

TEST_F(CPUOpcodeTest, opcode_test_0x3c_100)
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

TEST_F(CPUOpcodeTest, opcode_test_0x2f_84)
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

TEST_F(CPUOpcodeTest, opcode_test_0x3f_77)
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

TEST_F(CPUOpcodeTest, opcode_test_0x37_30)
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

TEST_F(CPUOpcodeTest, opcode_test_0x40_68)
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

TEST_F(CPUOpcodeTest, opcode_test_0x41_19)
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

TEST_F(CPUOpcodeTest, opcode_test_0x42_88)
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

TEST_F(CPUOpcodeTest, opcode_test_0x43_82)
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

TEST_F(CPUOpcodeTest, opcode_test_0x44_9)
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

TEST_F(CPUOpcodeTest, opcode_test_0x45_64)
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

TEST_F(CPUOpcodeTest, opcode_test_0x47_46)
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

TEST_F(CPUOpcodeTest, opcode_test_0x48_63)
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

TEST_F(CPUOpcodeTest, opcode_test_0x49_62)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4a_29)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4c_23)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4d_25)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4f_92)
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

TEST_F(CPUOpcodeTest, opcode_test_0x50_50)
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

TEST_F(CPUOpcodeTest, opcode_test_0x51_1)
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

TEST_F(CPUOpcodeTest, opcode_test_0x52_89)
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

TEST_F(CPUOpcodeTest, opcode_test_0x53_15)
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

TEST_F(CPUOpcodeTest, opcode_test_0x54_49)
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

TEST_F(CPUOpcodeTest, opcode_test_0x55_3)
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

TEST_F(CPUOpcodeTest, opcode_test_0x57_55)
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

TEST_F(CPUOpcodeTest, opcode_test_0x58_90)
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

TEST_F(CPUOpcodeTest, opcode_test_0x59_76)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5a_13)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5b_39)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5c_87)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5d_11)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5f_12)
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

TEST_F(CPUOpcodeTest, opcode_test_0x60_59)
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

TEST_F(CPUOpcodeTest, opcode_test_0x61_6)
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

TEST_F(CPUOpcodeTest, opcode_test_0x62_48)
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

TEST_F(CPUOpcodeTest, opcode_test_0x63_30)
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

TEST_F(CPUOpcodeTest, opcode_test_0x64_84)
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

TEST_F(CPUOpcodeTest, opcode_test_0x65_38)
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

TEST_F(CPUOpcodeTest, opcode_test_0x67_50)
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

TEST_F(CPUOpcodeTest, opcode_test_0x68_32)
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

TEST_F(CPUOpcodeTest, opcode_test_0x69_1)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6a_3)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6b_56)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6c_99)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6d_74)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6f_34)
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

TEST_F(CPUOpcodeTest, opcode_test_0x78_92)
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

TEST_F(CPUOpcodeTest, opcode_test_0x79_42)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7a_69)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7b_22)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7c_57)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7d_32)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7f_67)
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

TEST_F(CPUOpcodeTest, opcode_test_0x46_22)
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

TEST_F(CPUOpcodeTest, opcode_test_0x4e_82)
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

TEST_F(CPUOpcodeTest, opcode_test_0x56_78)
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

TEST_F(CPUOpcodeTest, opcode_test_0x5e_51)
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

TEST_F(CPUOpcodeTest, opcode_test_0x66_66)
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

TEST_F(CPUOpcodeTest, opcode_test_0x6e_80)
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

TEST_F(CPUOpcodeTest, opcode_test_0x7e_25)
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

TEST_F(CPUOpcodeTest, opcode_test_0x70_54)
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

TEST_F(CPUOpcodeTest, opcode_test_0x71_27)
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

TEST_F(CPUOpcodeTest, opcode_test_0x72_41)
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

TEST_F(CPUOpcodeTest, opcode_test_0x73_57)
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

TEST_F(CPUOpcodeTest, opcode_test_0x74_56)
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

TEST_F(CPUOpcodeTest, opcode_test_0x75_63)
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

TEST_F(CPUOpcodeTest, opcode_test_0x88_72)
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

TEST_F(CPUOpcodeTest, opcode_test_0x90_80)
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

TEST_F(CPUOpcodeTest, opcode_test_0x90_93)
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

TEST_F(CPUOpcodeTest, opcode_test_0x96_13)
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

TEST_F(CPUOpcodeTest, opcode_test_0xd6_67)
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

TEST_F(CPUOpcodeTest, opcode_test_0x98_37)
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

TEST_F(CPUOpcodeTest, opcode_test_0xa0_11)
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

TEST_F(CPUOpcodeTest, opcode_test_0xa4_100)
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

TEST_F(CPUOpcodeTest, opcode_test_0xe6_24)
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

TEST_F(CPUOpcodeTest, opcode_test_0xb4_30)
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

TEST_F(CPUOpcodeTest, opcode_test_0xb6_78)
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

TEST_F(CPUOpcodeTest, opcode_test_0xb7_26)
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

TEST_F(CPUOpcodeTest, opcode_test_0xb8_100)
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

TEST_F(CPUOpcodeTest, opcode_test_0xbe_63)
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

TEST_F(CPUOpcodeTest, opcode_test_0xc1_66)
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

TEST_F(CPUOpcodeTest, opcode_test_0xf1_49)
{
    SetUp({ 0xf1 });

	regs.A(0x21);
	regs.SP(0xcafe);
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
	ASSERT_EQ(after.A(), 0x70);
	ASSERT_EQ(after.SP(), 0xcb00);
	ASSERT_EQ(memAfter.readByte(0xCAFE), 0x69);
	ASSERT_EQ(memAfter.readByte(0xCAFF), 0x70);
	ASSERT_EQ(memAfter.readByte(0xCB00), 0x71);
}

TEST_F(CPUOpcodeTest, opcode_test_0xc5_44)
{
    SetUp({ 0xc5 });

	regs.SP(0xcafe);
	regs.BC(0xbeef);
	mem.writeByte(0xCAFD, 0x69);
	mem.writeByte(0xCAFE, 0x70);
	mem.writeByte(0xCAFC, 0x68);
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
	ASSERT_EQ(memAfter.readByte(0xCAFD), 0xBE);
	ASSERT_EQ(memAfter.readByte(0xCAFE), 0x70);
	ASSERT_EQ(memAfter.readByte(0xCAFC), 0xEF);
}

TEST_F(CPUOpcodeTest, opcode_test_0xf5_28)
{
    SetUp({ 0xf5 });

	regs.A(0xbe);
	regs.SP(0xcafe);
	mem.writeByte(0xCAFD, 0x69);
	mem.writeByte(0xCAFE, 0x70);
	mem.writeByte(0xCAFC, 0x68);
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
	ASSERT_EQ(after.A(), 0xbe);
	ASSERT_EQ(after.SP(), 0xcafc);
	ASSERT_EQ(memAfter.readByte(0xCAFD), 0xBE);
	ASSERT_EQ(memAfter.readByte(0xCAFE), 0x70);
	ASSERT_EQ(memAfter.readByte(0xCAFC), 0xE0);
}

TEST_F(CPUOpcodeTest, opcode_test_0xc0_78)
{
    SetUp({ 0xc0 });

	regs.SP(0xbabe);
	mem.writeByte(0xBAC0, 0x71);
	mem.writeByte(0xBABF, 0x70);
	mem.writeByte(0xBABE, 0x69);
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
	ASSERT_EQ(after.PC(), 0x7069);
	ASSERT_EQ(after.SP(), 0xbac0);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0xc0_15)
{
    SetUp({ 0xc0 });

	regs.SP(0xbabe);
	mem.writeByte(0xBAC0, 0x71);
	mem.writeByte(0xBABF, 0x70);
	mem.writeByte(0xBABE, 0x69);
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
	ASSERT_EQ(after.PC(), 0x1);
	ASSERT_EQ(after.SP(), 0xbabe);
	
}

TEST_F(CPUOpcodeTest, opcode_test_0x3c_84)
{
    SetUp({ 0x3c, 0x3c, 0x3c });

	regs.A(0x0);
	
	regs.flagZero(0);
	regs.flagSubtract(0);
	regs.flagHalfCarry(0);
	regs.flagCarry(0);

	
	

	cpu.cycle();
	cpu.cycle();
	cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), 12);

	ASSERT_EQ(after.flagZero(), 0);
	ASSERT_EQ(after.flagSubtract(), 0);
	ASSERT_EQ(after.flagHalfCarry(), 0);
	ASSERT_EQ(after.flagCarry(), 0);
	ASSERT_EQ(after.A(), 0x3);
	
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}
