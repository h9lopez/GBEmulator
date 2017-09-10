#include <iostream>
#include "CPUCore.h"
#include "gb_rom.h"
using namespace std;

int main(void)
{
	static std::string bootROM = "C:\\Users\\hlope\\Downloads\\DMG_ROM.bin";
	// Initialize datamem and registers

	// Load ROM into memspace


	RAM ram;
	RegBank regs;
	
	cout << "Attempting to load boot ROM" << '\n';
	if (ROMLoader::fromFile(bootROM, ram))
	{
		cout << "ROM loaded successfully\n";
	}
	else {
		cout << "FAILED ROM LOADING\n";
	}

	CPUCore core(ram, regs);
	core.cycle();

}