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
	reg->hi = LOW8_MASK(data);
	reg->lo = HIGH8_MASK(data);
}

inline uint8_t* CPU::decode_register_bits(unsigned int data)
{
	switch (data) {
		case 0x0:
			cout << "\tRegister selecting B" << endl;
			return &this->regs.BC.hi;
		case 0x1:
			cout << "\tRegister selecting C" << endl;
			return &this->regs.BC.lo;
		case 0x2:
			cout << "\tRegister selecting D" << endl;
			return &this->regs.DE.hi;
		case 0x3:
			cout << "\tRegister selecting E" << endl;
			return &this->regs.DE.lo;
		case 0x4:
			cout << "\tRegister selecting H" << endl;
			return &this->regs.HL.hi;
		case 0x5:
			cout << "\tRegister selecting L" << endl;
			return &this->regs.HL.lo;
		case 0x6:
			cout << "ERROR: UNIMPLEMENTED - loading from (HL)" << endl;
			// TODO: Load from mem into temp reg
			return nullptr;
		case 0x7:
			cout << "\tRegister selecting A" << endl;
			return &this->regs.A;
		default:
			cout << hex << "ERROR: register encoding is not of proper type (" << static_cast<int>(data) << ")" << endl;
	}
	return nullptr;
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
	
	uint8_t* target_reg = this->decode_register_bits(data & 0x7);
	int target_bit = (data & 0x38) >> 3;

	uint8_t tempreg = *target_reg;
	bool bit_on = (tempreg >> target_bit) & 0x1;
	
	cout << "\tBit " << (target_bit) << " is " << (bit_on ? "on" : "off") << endl;
	// Flag modifications:
	// z-> opposite of what bit 7 is
	// n-> 0
	// h-> 1
	// z-> left alone
	this->set_zero_flag(!bit_on);
	this->set_subtract_flag(false);
	this->set_half_carry_flag(true);
}

