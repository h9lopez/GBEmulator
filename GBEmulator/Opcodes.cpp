#include <stdint.h>
#include <iostream>
#include "Opcodes.h"

using namespace std;

bool opcode_handle_ld_sp_imm(unsigned int data)
{
	cout << hex << "I'm in this function! Encoded data is " << data << endl;
	return true;
}

bool opcode_handle_xor_a(unsigned int data)
{
	cout << hex << "XOR A opcode called, data is " << data << endl;
	return true;
}
