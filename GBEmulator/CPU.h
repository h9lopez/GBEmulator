#include <stdint.h>
#include <map>
#include <functional>
#include <tuple>
#include "Opcodes.h"
#include "Registers.h"
#pragma once


#define LOW8_MASK(x) (x & 0xFF)
#define HIGH8_MASK(x) ((x & 0xFF00) >> 8)

typedef std::function<void(unsigned int)> opcode_handle_func;

// (int, functionptr) = length of instruction (in bytes), function pointer to handler
typedef std::tuple< int, opcode_handle_func > opcode_handler_info;


class CPU
{
	uint8_t *mem;
	int mem_size;
	RegisterBank regs;

public:
	CPU(uint8_t *memPtr, int memSize);
	uint8_t fetch_and_decode();


protected:
	// ================================ HELPERS ===============================
	static inline void reg_store_immediate(Register *reg, unsigned int data);

	// ================================ HANDLERS ==============================
	void opcode_handle_ld_sp_imm(unsigned int data);
	void opcode_handle_xor_a(unsigned int data);
	void opcode_handle_ld_hl_imm(unsigned int data);
	void opcode_handle_ld_hl_minus_a(unsigned int data);


	// ============================ OPCODE MAP ================================
	const std::map<uint8_t, opcode_handler_info> global_opcode_vtable =
	{
		{ 0x31,{ 3, [this](unsigned int a) {this->opcode_handle_ld_sp_imm(a);		} }},
		{ 0xAF,{ 1, [this](unsigned int a) {this->opcode_handle_xor_a(a);			} } },
		{ 0x21,{ 3, [this](unsigned int a) {this->opcode_handle_ld_hl_imm(a);		} } },
		{ 0x32,{ 1, [this](unsigned int a) {this->opcode_handle_ld_hl_minus_a(a);	} } } 
	};
};