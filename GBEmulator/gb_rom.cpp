#include <string>
#include <iostream>
#include <fstream>
#include "gb_rom.h"


ROMLoader::ROMLoader() {}

bool ROMLoader::fromFile(const std::string &filename, RAM &ram)
{
	using namespace std;

	// Open file on disk
	ifstream romFile(filename, ios::binary);


	auto romSize = romFile.tellg();
	romFile.seekg(0, ios::end);

	// Check if the size is valid
	romSize = romFile.tellg() - romSize;
	if (romSize > MAX_ROMSIZE)
	{
		cout << "ERROR: ROM size (" << romSize << ") exceeds limit of " << MAX_ROMSIZE;
		romFile.close();
		return false;
	}

	// Else dump ROM contents into mem
	romFile.clear();
	romFile.seekg(0, ios::beg);


	// Read ROM from beginning to romSize, ignoring any EOF
	char *tempMem = new char[romSize];
	romFile.read(tempMem, romSize);

	// Allow RAM object to copy from the array
	if (!ram.copyRangeFromBuffer(tempMem, 0, romSize))
	{
		cout << "ERROR: Copying ROM to mem failed\n";
	}


	romFile.close();
	return true;
}