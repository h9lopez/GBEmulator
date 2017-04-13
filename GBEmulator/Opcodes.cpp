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
