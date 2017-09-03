#include <stdint.h>
#include <map>
#include <functional>
#include <tuple>
#include "Registers.h"
#pragma once


#define FIRST8_MASK(x) (x & 0xFF)
#define SECOND8_MASK(x) ((x & 0xFF00) >> 8)
#define THIRD8_MASK(x) ((x & 0xFF0000) >> 16)


class CPU
{
	uint8_t *mem;
	int mem_size;
	RegisterBank regs;
	unsigned long cycle_count;

public:
	CPU();
	CPU(uint8_t *memPtr, int memSize);
	void cpu_cycle();
	void handle_cb_prefix();
	void setMemoryLocation(uint8_t *memPtr, int memSize);


	// ================================ HELPERS ===============================
	inline uint16_t READ_ROM_2BYTE();
	inline uint8_t  READ_ROM_BYTE();
	inline void INC_CYCLE_COUNT(unsigned int num);
	inline void WRITE_MEM_BYTE(Register addr, uint8_t val);
	inline void WRITE_MEM_BYTE(uint16_t addr, uint8_t val);
	inline uint8_t READ_MEM_BYTE(Register reg);
	inline uint8_t READ_MEM_BYTE(uint16_t addr);
	inline uint8_t* GET_MEM_PTR(Register reg);


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

	// =======================HANDLERS 2.0=====================================
	inline void opcode_handle_xor(uint8_t regval);
	inline void reg_store_immediate(Register *reg, unsigned int data);
	inline void check_bit(uint8_t val, unsigned int bitn);
	inline void opcode_handle_inc_8bit(uint8_t *target);
	inline void CPU::opcode_handle_dec_8bit(uint8_t *target);
};