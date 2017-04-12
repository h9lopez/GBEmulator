#include <stdint.h>
#include <iostream>
#include "Opcodes.h"

using namespace std;

bool opcode_handle_ld_sp_imm(unsigned int data)
{
	cout << hex << "LD SP, d16 handler called. Encoded data is " << data << endl;
	return true;
}

bool opcode_handle_xor_a(unsigned int data)
{
	cout << hex << "XOR A handler called, data is " << data << endl;
	return true;
}
