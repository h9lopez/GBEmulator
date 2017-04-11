#include "CPU.h"
#include <iostream>
using namespace std;

CPU::CPU(uint8_t *memLoc, int memSize)
{
	if (memLoc == nullptr)
	{
		cout << "ERROR: ROM MEMORY POINTER NULL" << endl;
	}
	this->mem = memLoc;
	this->mem_size = memSize;
	this->pc = 0;
}

uint8_t CPU::fetch_and_decode()
{
	// TODO: Use map to decide which instructions are 1/2/3 bytes long and read the proper amount
	uint8_t b1 = this->mem[this->pc++];
	uint8_t b2 = this->mem[this->pc++];
	uint8_t b3 = this->mem[this->pc++];

	
	cout << hex << "Read opcode 0x" << static_cast<int>(b1) << static_cast<int>(b2) << static_cast<int>(b3) << endl;
	return b1;
}
