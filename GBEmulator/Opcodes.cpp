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
		
		// Load immediate in data (2 bytes) into register SP
		regs->SP.lo = LOW8_MASK(data);
		regs->SP.hi = HIGH8_MASK(data);
		return true;
	}

	bool opcode_handle_xor_a(RegisterBank *regs, unsigned int data)
	{
		cout << hex << "XOR A handler called, data is " << data << endl;
		cout << dec << "PC in regbank is currently at " << static_cast<int>(regs->PC.word) << endl;
		cout << hex << "SP is " << static_cast<int>(regs->SP.word) << endl;
		return true;
	}

}