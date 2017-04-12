#include "CPU.h"
#include <iostream>
using namespace std;

std::map<uint8_t, opcode_handler_info> global_opcode_vtable = {
	{ 0x31, opcode_handler_info{ 3, opcode_handle_ld_sp_imm }	},
	{ 0xAF, opcode_handler_info{ 1, opcode_handle_xor_a}		}
};

CPU::CPU(uint8_t *memLoc, int memSize)
{
	if (memLoc == nullptr)
	{
		cout << "ERROR: ROM MEMORY POINTER NULL" << endl;
	}
	this->mem = memLoc;
	this->mem_size = memSize;
	this->pc = 0;
}

uint8_t CPU::fetch_and_decode()
{
	int instr_len;
	opcode_handle_func handler;
	unsigned int encoded_args = 0;

	// TODO: Use map to decide which instructions are 1/2/3 bytes long and read the proper amount
	uint8_t opcode = this->mem[this->pc++];
	
	cout << hex << "Read opcode 0x" << static_cast<int>(opcode) << endl;
	cout << "Looking for opcode handler in vtable" << endl;
	std::tie(instr_len, handler) = global_opcode_vtable[opcode];

	// We got the length of the instruction in bytes from the global table
	// so we read the rest of the bytes needed and encode them as 'arguments' in a single integer
	// Although we just really need 16-bits more.
	instr_len--;
	while (instr_len > 0)
	{
		encoded_args <<= 8;
		uint8_t next_byte = this->mem[this->pc++];
		encoded_args |= next_byte;
		instr_len--;
	}

	cout << hex << "Encoded args: " << encoded_args << endl;

	// After all arguments have been gotten, dispatch to the handler
	handler(encoded_args);
	cout << "End handler call" << endl;
	return opcode;
}
