#include <string>
#include "Registers.h"
#include "CPU.h"

#pragma once

#define GB_MEMSIZE			0xFFFF
#define GB_ROMSIZE_MAX		0x8000

class Gameboy
{
	uint8_t			mem[GB_MEMSIZE];
	RegisterBank	regs;
	CPU				*cpu;

public:
	Gameboy();
	// Load ROM from file into memory regions
	bool loadROM(std::string filename);
	void beginExec();

};