#include "CPU.h"
#include "Opcodes.h"
#include <iostream>
using namespace std;



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


// NOTE: fully_opcode parameter can be multi-byte depending on the length of the instruction
void CPU::decode_and_exec(uint8_t leading_opcode, unsigned int full_opcode)
{
	int instr_len;
	opcode_handle_func handler;

	// Grab the opcode handler associated with the leading opcode
	try
	{
		std::tie(instr_len, handler) = global_opcode_vtable.at(leading_opcode);
	}
	catch (std::out_of_range &) {
		cout << hex << "UNSUPPORTED OPCODE 0x" << static_cast<int>(leading_opcode) << endl;
		// TODO: Throw global exception to stop CPU?
		return;
	}


	// Call the handler
	handler(full_opcode);

}

unsigned int CPU::fetch_next()
{
	int instr_len;
	opcode_handle_func handler;
	unsigned int encoded_args = 0;

	// TODO: Use map to decide which instructions are 1/2/3 bytes long and read the proper amount
	uint8_t opcode = this->mem[this->regs.PC.word++];
	
	cout << hex << "Read opcode 0x" << static_cast<int>(opcode) << endl;
	cout << "\tLooking for opcode handler in vtable" << endl;
	// Vector into the table to get function handler and size of full instruction (in case we need to fetch 1 or 2 more bytes before exec)
	try
	{
		std::tie(instr_len, handler) = global_opcode_vtable.at(opcode);
	} catch (std::out_of_range &) {
		cout << hex << "UNSUPPORTED OPCODE 0x" << static_cast<int>(opcode) << endl;
		// TODO: Throw global exception to stop CPU?
		return 0x0;
	}

	// Read the full amount of bytes necessary for instruction and encode them as arguments in single integer
	// TODO: Decouple instruction lengths from the global opcode vtable? Double access in here and
	//		 then again in decode_and_execute()
	encoded_args = opcode;
	instr_len--;
	while (instr_len > 0)
	{
		encoded_args <<= 8;
		uint8_t next_byte = this->mem[this->regs.PC.word++];
		encoded_args |= next_byte;
		instr_len--;
	}

	cout << hex << "\tEncoded args: " << encoded_args << endl;
	return encoded_args;
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
