#include <iostream>
#include "CPUCore.h"
#include "display/gb_ascii_screen.h"
#include "gb_rom.h"
#include <boost/log/trivial.hpp>
using namespace std;

int main(void)
{
	//static std::string bootROM = "/Users/hlopez34/Code/GBEmulator/GBEmulator_Test/ASMTest/simpleLoadTest.gb";
	static std::string bootROM = "/Users/hlopez34/Code/GBEmulator/GBEmulator_Test/ASMTest/DMG_ROM.bin";

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
		BOOST_LOG_TRIVIAL(fatal) << "Caught error and exiting " << e.what();
	}

	std::cout << "Ending state of RAM: " << ram << std::endl;

}