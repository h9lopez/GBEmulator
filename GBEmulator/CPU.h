#include <stdint.h>
#include <map>
#include <functional>
#include <tuple>
#include "Opcodes.h"
#include "Registers.h"
#pragma once


#define FIRST8_MASK(x) (x & 0xFF)
#define SECOND8_MASK(x) ((x & 0xFF00) >> 8)
#define THIRD8_MASK(x) ((x & 0xFF0000) >> 16)

typedef std::function<void(unsigned int)> opcode_handle_func;

// (int, functionptr) = length of instruction (in bytes), function pointer to handler
typedef std::tuple< int, opcode_handle_func > opcode_handler_info;


class CPU
{
	uint8_t *mem;
	int mem_size;
	RegisterBank regs;

public:
	CPU();
	CPU(uint8_t *memPtr, int memSize);
	void decode_and_exec(uint8_t leading_opcode, unsigned int full_opcode);
	unsigned int fetch_next();
	void setMemoryLocation(uint8_t *memPtr, int memSize);


	// ================================ HELPERS ===============================
	static void reg_store_immediate(Register *reg, unsigned int data);
	uint8_t* decode_register_bits(unsigned int data);
	Register* decode_16bit_register_bits(unsigned int data);

	// ========================== CLASS VARS GETTERS/SETTERS ==================
	uint8_t* get_mem_ptr();
	RegisterBank* get_reg_bank();

	// ========================== FLAG GETTERS/SETTERS ========================
	void set_zero_flag(bool set);
	void set_subtract_flag(bool set);
	void set_half_carry_flag(bool set);
	void set_carry_flag(bool set);
	bool is_zero_flag_set();
	bool is_subtract_flag_set();
	bool is_half_carry_flag_set();
	bool is_carry_flag_set();

	// ================================ HANDLERS ==============================
	void opcode_handle_xor(unsigned int data);
	void opcode_handle_ld_hl_minus_a(unsigned int data);
	void opcode_handle_bit_check(unsigned int data);
	void opcode_handle_ld_16bit_imm(unsigned int data);


	// ============================ OPCODE MAP ================================
	const std::map<uint8_t, opcode_handler_info> global_opcode_vtable =
	{
		// 16-bit immediate load instructions
		{ 0x01,{ 3, [this](unsigned int a) {this->opcode_handle_ld_16bit_imm(a);	}}},
		{ 0x11,{ 3, [this](unsigned int a) {this->opcode_handle_ld_16bit_imm(a);	} } },
		{ 0x21,{ 3, [this](unsigned int a) {this->opcode_handle_ld_16bit_imm(a);	} } },
		{ 0x31,{ 3, [this](unsigned int a) {this->opcode_handle_ld_16bit_imm(a);	} } },

		// XOR reg instructions
		{ 0xA8,{ 1, [this](unsigned int a) {this->opcode_handle_xor(a);				} } },
		{ 0xA9,{ 1, [this](unsigned int a) {this->opcode_handle_xor(a);				} } },
		{ 0xAA,{ 1, [this](unsigned int a) {this->opcode_handle_xor(a);				} } },
		{ 0xAB,{ 1, [this](unsigned int a) {this->opcode_handle_xor(a);				} } },
		{ 0xAC,{ 1, [this](unsigned int a) {this->opcode_handle_xor(a);				} } },
		{ 0xAD,{ 1, [this](unsigned int a) {this->opcode_handle_xor(a);				} } },
		{ 0xAE,{ 1, [this](unsigned int a) {this->opcode_handle_xor(a);				} } },
		{ 0xAF,{ 1, [this](unsigned int a) {this->opcode_handle_xor(a);				} } },


		{ 0x32,{ 1, [this](unsigned int a) {this->opcode_handle_ld_hl_minus_a(a);	} } },
		{ 0xCB,{ 2, [this](unsigned int a) {this->opcode_handle_bit_check(a);		}}}
	};
};