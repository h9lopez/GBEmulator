// GBEmulator_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "gtest/gtest.h"
#include "CPU.h"
#include "CPUTest.hpp"
#include "OpcodesTest.hpp"

// Tests CPU flags zero, subtract, half carry, carry
TEST_F(OpcodesTest, SetAndUnsetFlags)
{
	// Zero flag set and unset
	this->gbcpu.set_zero_flag(true);
	EXPECT_TRUE(this->gbcpu.is_zero_flag_set());

	this->gbcpu.set_zero_flag(false);
	EXPECT_FALSE(this->gbcpu.is_zero_flag_set());

	// Subtract flag set and unset
	this->gbcpu.set_subtract_flag(true);
	EXPECT_TRUE(this->gbcpu.is_subtract_flag_set());

	this->gbcpu.set_subtract_flag(false);
	EXPECT_FALSE(this->gbcpu.is_subtract_flag_set());

	// Half carry flag set and unset
	this->gbcpu.set_half_carry_flag(true);
	EXPECT_TRUE(gbcpu.is_half_carry_flag_set());

	this->gbcpu.set_half_carry_flag(false);
	EXPECT_FALSE(gbcpu.is_half_carry_flag_set());

	// Carry flag set and unset
	gbcpu.set_carry_flag(true);
	EXPECT_TRUE(gbcpu.is_carry_flag_set());

	gbcpu.set_carry_flag(false);
	EXPECT_FALSE(gbcpu.is_carry_flag_set());

}

TEST_F(OpcodesTest, RegisterImmediateStore)
{
	Register reg;
	gbcpu.reg_store_immediate(&reg, 0xCDAB);

	ASSERT_TRUE(reg.word == 0xABCD);
	ASSERT_TRUE(reg.hi = 0xAB && reg.lo == 0xCD);
}

TEST_F(OpcodesTest, DecodeRegisterEncoding)
{
	RegisterBank* reg_bank = gbcpu.get_reg_bank();
	uint8_t *device_mem = gbcpu.get_mem_ptr();

	// For when we access (HL) later
	device_mem[0x0304] = 0xCA;

	// Load registers with values first
	gbcpu.reg_store_immediate(&reg_bank->A, 0xAB00);
	gbcpu.reg_store_immediate(&reg_bank->BC, 0xEFCD);
	gbcpu.reg_store_immediate(&reg_bank->DE, 0x0201);
	gbcpu.reg_store_immediate(&reg_bank->HL, 0x0403);

	ASSERT_TRUE(reg_bank->A.word == 0xAB);
	ASSERT_TRUE(reg_bank->BC.hi == 0xCD && reg_bank->BC.lo == 0xEF);
	ASSERT_TRUE(reg_bank->DE.hi == 0x01 && reg_bank->DE.lo == 0x02);
	ASSERT_TRUE(reg_bank->HL.hi == 0x03 && reg_bank->HL.lo == 0x04);
}

//TEST_F(OpcodesTest, Opcode_BitCheck)
//{
//	uint8_t base_second_byte = 0x40;
//	RegisterBank* reg_bank = gbcpu.get_reg_bank();
//	bool old_carry_flag = gbcpu.is_carry_flag_set();
//
//	// Set up registers for the bit check
//	gbcpu.reg_store_immediate(&reg_bank->A, 0x8000);
//	gbcpu.reg_store_immediate(&reg_bank->BC, 0x0040);
//
//	ASSERT_TRUE(reg_bank->A.word == 0x80);
//	ASSERT_TRUE(reg_bank->BC.hi == 0x40 && reg_bank->BC.lo == 0x00);
//
//	// Perform actual bit check instructions
//	gbcpu.opcode_handle_bit_check((base_second_byte | 0x38) | 0x07); // Bit 7 on reg A
//	// Check that the Z flag was UNSET, Subtract was UNSET, and Half Carry SET, while Carry stays the same
//	ASSERT_FALSE(gbcpu.is_zero_flag_set());
//	ASSERT_FALSE(gbcpu.is_subtract_flag_set());
//	ASSERT_TRUE(gbcpu.is_half_carry_flag_set());
//	ASSERT_EQ(gbcpu.is_carry_flag_set(), old_carry_flag);
//
//	// Check a non-set bit
//	gbcpu.opcode_handle_bit_check(base_second_byte | 0x28); // Bit 5 on reg 
//	// Check that the Z flag was SET, Subtract was UNSET, and Half Carry SET, while Carry stays the same
//	ASSERT_TRUE(gbcpu.is_zero_flag_set());
//	ASSERT_FALSE(gbcpu.is_subtract_flag_set());
//	ASSERT_TRUE(gbcpu.is_half_carry_flag_set());
//	ASSERT_EQ(gbcpu.is_carry_flag_set(), old_carry_flag);
//}

TEST_F(OpcodesTest, Opcode_LoadImmediate16)
{
	RegisterBank *regs = gbcpu.get_reg_bank();

	// LD SP, imm
	gbcpu.opcode_handle_ld_16bit_imm(0x31FECA);
	EXPECT_EQ(regs->SP.word, 0xCAFE);

	// LD HL, imm
	gbcpu.opcode_handle_ld_16bit_imm(0x21CDAB);
	EXPECT_EQ(regs->HL.word, 0xABCD);

	// LD DE, imm
	gbcpu.opcode_handle_ld_16bit_imm(0x11BEBA);
	EXPECT_EQ(regs->DE.word, 0xBABE);

	// LD BC, imm
	gbcpu.opcode_handle_ld_16bit_imm(0x01AAFA);
	EXPECT_EQ(regs->BC.word, 0xFAAA);
}

TEST_F(OpcodesTest, Opcode_XorReg)
{
	RegisterBank *regs = gbcpu.get_reg_bank();

	// Set reg A to a random value so that we can clear it later
	regs->A.word = 0xCA;

	gbcpu.opcode_handle_xor(0xAF);
	EXPECT_EQ(regs->A.word, 0x0); // A^A=0 no matter what A is
	// We also expect the zero flag to be set if the result is 0
	EXPECT_TRUE(gbcpu.is_zero_flag_set());
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}

