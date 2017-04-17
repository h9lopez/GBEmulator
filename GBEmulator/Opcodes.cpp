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
	reg->hi = FIRST8_MASK(data);
	reg->lo = SECOND8_MASK(data);
}

uint8_t* CPU::decode_register_bits(unsigned int data)
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
			cout << hex << "\tRegister selecting (HL) - loading from mem addr " << static_cast<int>(regs.HL.word) << endl;
			return &this->mem[regs.HL.word];
		case 0x7:
			cout << "\tRegister selecting A" << endl;
			return &this->regs.A.lo;
		default:
			cout << hex << "ERROR: register encoding is not of proper type (" << static_cast<int>(data) << ")" << endl;
	}
	return nullptr;
}

Register* CPU::decode_16bit_register_bits(unsigned int data)
{
	switch (data) {
		case 0x0: // BC
			cout << "Selecting BC" << endl;
			return &this->regs.BC;
		case 0x1: // DE
			cout << "Selecting DE" << endl;
			return &this->regs.DE;
		case 0x2: // HL
			cout << "Selecting HL" << endl;
			return &this->regs.HL;
		case 0x3: // SP
			cout << "Selecting SP" << endl;
			return &this->regs.SP;
	}
	return nullptr;
}

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


void CPU::opcode_handle_xor(unsigned int data)
{
	// Decode the target register and get data ptr
	uint8_t* target_reg = decode_register_bits(data & 0x7);

	if (target_reg == nullptr)
	{
		cout << hex << "ERROR in decoding register bits " << static_cast<int>(data & 0x7) << endl;
		return;
	}

	this->regs.A.lo = (*target_reg) ^ this->regs.A.lo;
	
	// Set the zero flag if the result is 0, unset if nonzero
	this->set_zero_flag(this->regs.A.lo == 0x0);
}


void CPU::opcode_handle_ld_hl_minus_a(unsigned int data)
{
	// LD (HL-), A; alternatively: LDD (HL), A
	// (HL) <- A, HL--
	mem[regs.HL.word] = regs.A.lo;
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

void CPU::opcode_handle_ld_16bit_imm(unsigned int data)
{
	uint8_t opcode = THIRD8_MASK(data);
	unsigned int immediate = data ^ (opcode << 16); // Get rid of that last byte to leave only first two

	// Decode which register it's targeting
	Register *target_reg = this->decode_16bit_register_bits( (opcode & 0x30) >> 4);
	if (target_reg == nullptr)
	{
		cout << "UNKNOWN register destination " << ((opcode & 0x30) >> 4) << endl;
		return;
	}

	reg_store_immediate(target_reg, immediate);
}

