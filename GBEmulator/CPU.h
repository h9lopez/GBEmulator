#include <stdint.h>
#pragma once

class CPU
{
	uint8_t *mem;
	int mem_size;
	int pc;
public:
	CPU(uint8_t *memPtr, int memSize);
	uint8_t fetch_and_decode();

};