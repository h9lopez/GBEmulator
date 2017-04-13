#include <functional>
#include "Registers.h"
#pragma once



//namespace OpcodeHandlers
//{
//	// ================================ HELPERS ===============================
//	inline void reg_store_immediate(Register *reg, unsigned int data);
//
//	// ================================ HANDLERS ==============================
//	void opcode_handle_ld_sp_imm(RegisterBank *regs, unsigned int data);
//	void opcode_handle_xor_a(RegisterBank *regs, unsigned int data);
//	void opcode_handle_ld_hl_imm(RegisterBank *regs, unsigned int data);
//	void opcode_handle_ld_hl_minus_a(RegisterBank *regs, unsigned int data);
//
//
//	// ============================ OPCODE MAP ================================
//	const std::map<uint8_t, opcode_handler_info> global_opcode_vtable =
//	{
//		{ 0x31, { 3, opcode_handle_ld_sp_imm }	},
//		{ 0xAF, { 1, opcode_handle_xor_a }		},
//		{ 0x21, { 3, opcode_handle_ld_hl_imm }	},
//		{ 0x32, { 1, opcode_handle_ld_hl_minus_a }}
//	};
//}