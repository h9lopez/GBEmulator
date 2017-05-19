#include <iostream>
#include "CPU.h"

using namespace std;

/*******************************INSTRUCTION HANDLERS**************************/

// reg -> register to store value into
// data -> assumed to have been gotten straight from the instruction
//		   i.e. implying the little-endian format of the Gameboy


uint8_t* CPU::get_mem_ptr()
{
	return this->mem;
}

RegisterBank* CPU::get_reg_bank()
{
	return &this->regs;
}

// =============== FLAG GETTERS/SETTERS ============================
void CPU::set_zero_flag(bool set)
{
	this->regs.zero = set;
}

void CPU::set_subtract_flag(bool set)
{
	this->regs.subtract = set;
}

void CPU::set_half_carry_flag(bool set)
{
	this->regs.half_carry = set;
}

void CPU::set_carry_flag(bool set)
{
	this->regs.carry = set;
}

bool CPU::is_zero_flag_set()
{
	return this->regs.zero;
}

bool CPU::is_subtract_flag_set()
{
	return this->regs.subtract;
}

bool CPU::is_half_carry_flag_set()
{
	return this->regs.half_carry;
}

bool CPU::is_carry_flag_set()
{
	return this->regs.carry;
}
// =============== END FLAG GETTERS/SETTERS ============================





//void CPU::opcode_handle_bit_check(unsigned int data)
//{
//	// NOTE: This is an all-encompassing method, i.e. we need to check second byte to
//	//		 check which register we're looking at/which bit we're checking
//	
//	uint8_t* target_reg = this->decode_register_bits(data & 0x7);
//	int target_bit = (data & 0x38) >> 3;
//
//	uint8_t tempreg = *target_reg;
//	bool bit_on = (tempreg >> target_bit) & 0x1;
//	
//	cout << "\tBit " << (target_bit) << " is " << (bit_on ? "on" : "off") << endl;
//	// Flag modifications:
//	// z-> opposite of what bit 7 is
//	// n-> 0
//	// h-> 1
//	// z-> left alone
//	this->set_zero_flag(!bit_on);
//	this->set_subtract_flag(false);
//	this->set_half_carry_flag(true);
//}

