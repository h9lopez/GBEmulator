#include <stdint.h>
#include <map>
#include <functional>
#include <tuple>
#include "Opcodes.h"
#pragma once

typedef std::function<bool(unsigned int)> opcode_handle_func;

// (int, functionptr) = length of instruction (in bytes), function pointer to handler
typedef std::tuple< int, opcode_handle_func > opcode_handler_info;

class CPU
{
	uint8_t *mem;
	int mem_size;
	int pc;
public:
	CPU(uint8_t *memPtr, int memSize);
	uint8_t fetch_and_decode();

};