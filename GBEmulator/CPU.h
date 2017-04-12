#include <stdint.h>
#include <map>
#include <functional>
#include <tuple>
#include "Opcodes.h"
#include "Registers.h"
#pragma once


class CPU
{
	uint8_t *mem;
	int mem_size;
	RegisterBank regs;

public:
	CPU(uint8_t *memPtr, int memSize);
	uint8_t fetch_and_decode();

};