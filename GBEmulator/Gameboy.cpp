#include <stdint.h>
#include <iostream>
#include <fstream>
#include "Registers.h"
#include "Gameboy.h"

using namespace std;

Gameboy::Gameboy()
{
	memset(this->mem, 0xFF, GB_MEMSIZE);
}

// Load ROM from file into memory regions
bool Gameboy::loadROM(std::string filename)
{
	// Open file on disk
	ifstream romFile(filename, ios::binary);


	auto romSize = romFile.tellg();
	romFile.seekg(0, ios::end);

	// Check if the size is valid
	romSize = romFile.tellg() - romSize;
	if (romSize > GB_ROMSIZE_MAX)
	{
		cout << "ERROR: ROM size (" << romSize << ") exceeds limit of " << GB_ROMSIZE_MAX;
		romFile.close();
		return false;
	}

	// Else dump ROM contents into mem
	romFile.clear();
	romFile.seekg(0, ios::beg);


	// Read ROM from beginning to romSize, ignoring any EOF
	romFile.read((char*)this->mem, romSize);



	romFile.close();
	return true;
}

void Gameboy::beginExec()
{
	// TODO: Add check to see if ROM was actually loaded
	cout << "Beginning execution of loaded ROM" << endl;

	/*this->cpu = new CPU(this->mem, GB_MEMSIZE);

	for (;;)
	{
		this->cpu->cpu_cycle();
	}
	*/
}
