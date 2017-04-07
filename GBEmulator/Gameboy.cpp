#include <stdint.h>
#include <iostream>
#include <fstream>
#include "Registers.h"
#include "Gameboy.h"

using namespace std;

Gameboy::Gameboy()
{
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


	// PROBLEM: This is reading up until the 1A hex val, which is EOF in windows.
	// SOLUTION: Ignore EOF and just read by size
	cout << "Rom size: " << romSize << endl;
	romFile.read((char*)this->mem, romSize);

	cout << hex << "Byte should be 01 but is: " << (int)this->mem[32703] << endl;


	romFile.close();
	return true;
}
