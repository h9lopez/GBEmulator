#include <iostream>
#include "CPUCore.h"
#include "gb_rom.h"
using namespace std;

int main(void)
{
	static std::string bootROM = "/Users/hlopez34/Code/GBEmulator/GBEmulator_Test/ASMTest/simpleLoadTest.gb";
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

	try
	{
		while (true)
		{
			core.cycle();
			//char c;
			//std::cin >> c;
		}
	}
	catch (std::runtime_error e)
	{
		std::cerr << "Caught error and exiting: " << e.what() << std::endl;

		ByteType val = ram.readByte(0xBABF);
		std::cerr << "Content at BABF: " << std::hex << +val << std::endl;
	}

}