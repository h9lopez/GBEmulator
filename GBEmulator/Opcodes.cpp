#include <iostream>
#include <map>
#include "CPU.h"

using namespace std;

/*******************************INSTRUCTION HANDLERS**************************/

// reg -> register to store value into
// data -> assumed to have been gotten straight from the instruction
//		   i.e. implying the little-endian format of the Gameboy
void CPU::reg_store_immediate(Register *reg, unsigned int data)
{
	reg->lo = LOW8_MASK(data);
	reg->hi = HIGH8_MASK(data);
}

// =============== FLAG GETTERS/SETTERS ============================
inline void CPU::set_zero_flag(bool set)
{
	this->regs.zero = set;
}

inline void CPU::set_subtract_flag(bool set)
{
	this->regs.subtract = set;
}

inline void CPU::set_half_carry_flag(bool set)
{
	this->regs.half_carry = set;
}

inline void CPU::set_carry_flag(bool set)
{
	this->regs.carry = set;
}

inline bool CPU::is_zero_flag_set()
{
	return this->regs.zero;
}

inline bool CPU::is_subtract_flag_set()
{
	return this->regs.subtract;
}

inline bool CPU::is_half_carry_flag_set()
{
	return this->regs.half_carry;
}

inline bool CPU::is_carry_flag_set()
{
	return this->regs.carry;
}
// =============== END FLAG GETTERS/SETTERS ============================


void CPU::opcode_handle_ld_sp_imm(unsigned int data)
{
	cout << hex << "LD SP, d16 handler called. Encoded data is " << data << endl;
	cout << dec << "PC in regbank is currently at " << static_cast<int>(regs.PC.word) << endl;
		
	// Load immediate in data (2 bytes) into register SP
	reg_store_immediate(&this->regs.SP, data);
}

void CPU::opcode_handle_xor_a(unsigned int data)
{
	cout << hex << "XOR A handler called, data is " << data << endl;
	cout << dec << "PC in regbank is currently at " << static_cast<int>(regs.PC.word) << endl;
		
	// A = A ^ A
	// Reduced to A = 0 b/c a reg XOR'd by itself is always 0
	this->regs.A = 0;
}

void CPU::opcode_handle_ld_hl_imm(unsigned int data)
{
	// Load imm in data (2 bytes) into register HL
	reg_store_immediate(&this->regs.HL, data);
}

void CPU::opcode_handle_ld_hl_minus_a(unsigned int data)
{
	// LD (HL-), A; alternatively: LDD (HL), A
	// (HL) <- A, HL--
	mem[regs.HL.word] = regs.A;
	regs.HL.word--;
}

void CPU::opcode_handle_bit_check(unsigned int data)
{
	// NOTE: This is an all-encompassing method, i.e. we need to check second byte to
	//		 check which register we're looking at/which bit we're checking
	
	// TODO: Implement efficient way to extract both target register and target bit
	//		Actually, NOTE: Check that one colorful page with the things. Yeah. It's encoded

	// Flag modifications:
	// z-> opposite of what bit 7 is
	// n-> 0
	// h-> 1
	// z-> left alone
	this->set_zero_flag(true);
	this->set_subtract_flag(false);
	this->set_half_carry_flag(true);
}

