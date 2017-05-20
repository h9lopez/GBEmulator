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