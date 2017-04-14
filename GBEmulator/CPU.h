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
	inline uint8_t* decode_register_bits(unsigned int data);

	// ========================== FLAG GETTERS/SETTERS ========================
	inline void set_zero_flag(bool set);
	inline void set_subtract_flag(bool set);
	inline void set_half_carry_flag(bool set);
	inline void set_carry_flag(bool set);
	inline bool is_zero_flag_set();
	inline bool is_subtract_flag_set();
	inline bool is_half_carry_flag_set();
	inline bool is_carry_flag_set();

	// ================================ HANDLERS ==============================
	void opcode_handle_ld_sp_imm(unsigned int data);
	void opcode_handle_xor_a(unsigned int data);
	void opcode_handle_ld_hl_imm(unsigned int data);
	void opcode_handle_ld_hl_minus_a(unsigned int data);
	void opcode_handle_bit_check(unsigned int data);


	// ============================ OPCODE MAP ================================
	const std::map<uint8_t, opcode_handler_info> global_opcode_vtable =
	{
		{ 0x31,{ 3, [this](unsigned int a) {this->opcode_handle_ld_sp_imm(a);		} }},
		{ 0xAF,{ 1, [this](unsigned int a) {this->opcode_handle_xor_a(a);			} } },
		{ 0x21,{ 3, [this](unsigned int a) {this->opcode_handle_ld_hl_imm(a);		} } },
		{ 0x32,{ 1, [this](unsigned int a) {this->opcode_handle_ld_hl_minus_a(a);	} } },
		{ 0xCB,{ 2, [this](unsigned int a) {this->opcode_handle_bit_check(a);		}}}
	};
};