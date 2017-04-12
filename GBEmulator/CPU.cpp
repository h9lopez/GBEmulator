#include "CPU.h"
#include "Opcodes.h"
#include <iostream>
using namespace std;



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


uint8_t CPU::fetch_and_decode()
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
		std::tie(instr_len, handler) = OpcodeHandlers::global_opcode_vtable.at(opcode);
	} catch (std::out_of_range &) {
		cout << hex << "UNSUPPORTED OPCODE 0x" << static_cast<int>(opcode) << endl;
		// TODO: Throw global exception to stop CPU?
		return 0x0;
	}

	// Read the full amount of bytes necessary for instruction and encode them as arguments in single integer
	instr_len--;
	while (instr_len > 0)
	{
		encoded_args <<= 8;
		uint8_t next_byte = this->mem[this->regs.PC.word++];
		encoded_args |= next_byte;
		instr_len--;
	}

	cout << hex << "\tEncoded args: " << encoded_args << endl;
	cout << "\tDISPATCHING OPCODE HANDLER" << endl;

	// After all arguments have been gotten, dispatch to the handler
	handler(&this->regs, encoded_args);
	cout << "End handler call" << endl << endl;
	return opcode;
}