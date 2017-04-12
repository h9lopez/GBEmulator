#include <iostream>
#include <map>
#include "Opcodes.h"

using namespace std;
namespace OpcodeHandlers
{

	/*******************************INSTRUCTION HANDLERS**************************/

	bool opcode_handle_ld_sp_imm(RegisterBank *regs, unsigned int data)
	{
		cout << hex << "LD SP, d16 handler called. Encoded data is " << data << endl;
		cout << dec << "PC in regbank is currently at " << static_cast<int>(regs->PC.word) << endl;
		return true;
	}

	bool opcode_handle_xor_a(RegisterBank *regs, unsigned int data)
	{
		cout << hex << "XOR A handler called, data is " << data << endl;
		cout << dec << "PC in regbank is currently at " << static_cast<int>(regs->PC.word) << endl;
		return true;
	}

}