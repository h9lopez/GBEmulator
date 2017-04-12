#include <functional>
#include "Registers.h"
#pragma once

typedef std::function<bool(RegisterBank *regs, unsigned int)> opcode_handle_func;

// (int, functionptr) = length of instruction (in bytes), function pointer to handler
typedef std::tuple< int, opcode_handle_func > opcode_handler_info;

namespace OpcodeHandlers
{

	bool opcode_handle_ld_sp_imm(RegisterBank *regs, unsigned int data);
	bool opcode_handle_xor_a(RegisterBank *regs, unsigned int data);

	const std::map<uint8_t, opcode_handler_info> global_opcode_vtable =
	{
		{ 0x31, opcode_handler_info{ 3, opcode_handle_ld_sp_imm } },
		{ 0xAF, opcode_handler_info{ 1, opcode_handle_xor_a } }
	};
}