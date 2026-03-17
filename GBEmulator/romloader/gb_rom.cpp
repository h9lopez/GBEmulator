#include <string>
#include <vector>
#include <iostream>
#include <cerrno>
#include <fstream>
#include "gb_rom.h"


ROMLoader::ROMLoader() {}

bool ROMLoader::fromFile(const std::string &filename, RAM &ram)
{
	using namespace std;

	// Open file on disk
	ifstream romFile;
	romFile.open(filename, ios::binary);
	if (!romFile)
	{
		cerr << "ERROR: Could not open ROM file, exiting. path=" << filename << ", error=" << std::strerror(errno) << "\n";
		return false;
	}

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
	std::vector<char> tempMem;

	std::copy(std::istreambuf_iterator<char>(romFile), std::istreambuf_iterator<char>(),
			 std::back_inserter(tempMem));

	// Allow RAM object to copy from the array
	auto res = ram.copyRangeFromBuffer<std::vector<char>::iterator>(tempMem.begin(),
																	tempMem.end(),
																	ram.begin());
	if (!res)
	{
		cout << "ERROR: Copying ROM to mem failed\n";
	}

	romFile.close();
	return true;
}



