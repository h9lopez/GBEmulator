#include "CPU.h"
#include <iostream>
using namespace std;

#define READ_MEM_BYTE(REG) ((mem[REG.word]))

CPU::CPU()
{
	this->regs.PC.word = 0;
}

CPU::CPU(uint8_t *memLoc, int memSize)
{
	if (memLoc == nullptr)
	{
		cout << "ERROR: ROM MEMORY POINTER NULL" << endl;
	}
	this->mem = memLoc;
	this->mem_size = memSize;
	this->regs.PC.word = 0;
}

void CPU::setMemoryLocation(uint8_t * memLoc, int memSize)
{
	if (memLoc == nullptr)
	{
		cout << "ERROR: ROM MEMORY POINTER NULL" << endl;
	}
	this->mem = memLoc;
	this->mem_size = memSize;
}


inline uint16_t CPU::READ_ROM_2BYTE()
{
	//uint16_t res = (uint16_t)mem[regs.PC.word];
	uint16_t res = ((uint16_t*)(mem + regs.PC.word))[0];
	regs.PC.word += 2;
	return res;
}


// ============================================================================
// ============================================================================
// ============================================================================
// ===========================OPCODE HANDLERS==================================
// ============================================================================
// ============================================================================
// ============================================================================
inline void CPU::reg_store_immediate(Register *reg, unsigned int data)
{
	reg->hi = FIRST8_MASK(data);
	reg->lo = SECOND8_MASK(data);
}


inline void CPU::opcode_handle_xor(uint8_t regval)
{
	this->regs.A.lo = (regval) ^ this->regs.A.lo;
	this->set_zero_flag(this->regs.A.lo == 0x0);
}


// ============================================================================
// =======================END HANDLERS=========================================
// ============================================================================


void CPU::cpu_cycle()
{
	// Grab an opcode first
	uint8_t opcode = 0x76;
	
	for (;;) {
		cout << "CPU Cycle Begin" << endl;
		opcode = this->mem[this->regs.PC.word++];
		// Switch statement into the proper instruction handler
		switch (opcode)
		{
			// 16-bit immediate load into 16-bit regs
			case 0x01:
				// TODO: Second parameter is wrong. Needs to be 2 BYTES not just the 1
				reg_store_immediate(&this->regs.BC, READ_ROM_2BYTE());
				continue;
			case 0x11:
				reg_store_immediate(&this->regs.DE, READ_ROM_2BYTE());
				continue;
			case 0x21:
				reg_store_immediate(&this->regs.HL, READ_ROM_2BYTE());
				continue;
			case 0x31:
				reg_store_immediate(&this->regs.SP, READ_ROM_2BYTE());
				continue;

			// XOR instructions
			case 0xA8:
				opcode_handle_xor(this->regs.BC.hi);
				continue;
			case 0xA9:
				opcode_handle_xor(this->regs.BC.lo);
				continue;
			case 0xAA:
				opcode_handle_xor(this->regs.DE.hi);
				continue;
			case 0xAB:
				opcode_handle_xor(this->regs.DE.lo);
				continue;
			case 0xAC:
				opcode_handle_xor(this->regs.HL.hi);
				continue;
			case 0xAD:
				opcode_handle_xor(this->regs.HL.lo);
				continue;
			case 0xAE:
				opcode_handle_xor(READ_MEM_BYTE(regs.HL));
				continue;
			case 0xAF:
				opcode_handle_xor(this->regs.A.lo);
				continue;

			// LD (HL-), A
			case 0x32:
				mem[regs.HL.word] = regs.A.lo;
				regs.HL.word--;

			// NEXT: Bit checking instructions?


			// HALT Instruction
			case 0x76:
				cout << "CPU Power Down" << endl;
				exit(0);

			default:
				continue;
		}
	}
	
}


//// NOTE: fully_opcode parameter can be multi-byte depending on the length of the instruction
//void CPU::decode_and_exec(uint8_t leading_opcode, unsigned int full_opcode)
//{
//	int instr_len;
//	opcode_handle_func handler;
//
//	// Grab the opcode handler associated with the leading opcode
//	try
//	{
//		std::tie(instr_len, handler) = global_opcode_vtable.at(leading_opcode);
//	}
//	catch (std::out_of_range &) {
//		cout << hex << "UNSUPPORTED OPCODE 0x" << static_cast<int>(leading_opcode) << endl;
//		// TODO: Throw global exception to stop CPU?
//		return;
//	}
//
//
//	// Call the handler
//	handler(full_opcode);
//
//}
//
//unsigned int CPU::fetch_next()
//{
//	int instr_len;
//	opcode_handle_func handler;
//	unsigned int encoded_args = 0;
//
//	// TODO: Use map to decide which instructions are 1/2/3 bytes long and read the proper amount
//	uint8_t opcode = this->mem[this->regs.PC.word++];
//	
//	cout << hex << "Read opcode 0x" << static_cast<int>(opcode) << endl;
//	cout << "\tLooking for opcode handler in vtable" << endl;
//	// Vector into the table to get function handler and size of full instruction (in case we need to fetch 1 or 2 more bytes before exec)
//	try
//	{
//		std::tie(instr_len, handler) = global_opcode_vtable.at(opcode);
//	} catch (std::out_of_range &) {
//		cout << hex << "UNSUPPORTED OPCODE 0x" << static_cast<int>(opcode) << endl;
//		// TODO: Throw global exception to stop CPU?
//		return 0x0;
//	}
//
//	// Read the full amount of bytes necessary for instruction and encode them as arguments in single integer
//	// TODO: Decouple instruction lengths from the global opcode vtable? Double access in here and
//	//		 then again in decode_and_execute()
//	encoded_args = opcode;
//	instr_len--;
//	while (instr_len > 0)
//	{
//		encoded_args <<= 8;
//		uint8_t next_byte = this->mem[this->regs.PC.word++];
//		encoded_args |= next_byte;
//		instr_len--;
//	}
//
//	cout << hex << "\tEncoded args: " << encoded_args << endl;
//	return encoded_args;
//}

