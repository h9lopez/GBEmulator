#include <functional>
#include "Registers.h"
#pragma once

#define LOW8_MASK(x) (x & 0xFF)
#define HIGH8_MASK(x) ((x & 0xFF00) >> 8)

typedef std::function<void(RegisterBank *regs, unsigned int)> opcode_handle_func;

// (int, functionptr) = length of instruction (in bytes), function pointer to handler
typedef std::tuple< int, opcode_handle_func > opcode_handler_info;

namespace OpcodeHandlers
{

	void opcode_handle_ld_sp_imm(RegisterBank *regs, unsigned int data);
	void opcode_handle_xor_a(RegisterBank *regs, unsigned int data);
	void opcode_handle_ld_hl_imm(RegisterBank *regs, unsigned int data);

	const std::map<uint8_t, opcode_handler_info> global_opcode_vtable =
	{
		{ 0x31, { 3, opcode_handle_ld_sp_imm }	},
		{ 0xAF, { 1, opcode_handle_xor_a }		},
		{ 0x21, { 3, opcode_handle_ld_hl_imm }	}
	};
}