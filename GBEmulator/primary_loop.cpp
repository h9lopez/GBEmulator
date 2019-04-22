#include <iostream>
#include <cpu/CPUCore.h>
#include <display/gb_ascii_screen.h>
#include <romloader/gb_rom.h>


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

	Core::CPUCore core(ram, regs);

	BOOST_LOG_TRIVIAL(info) << "Regular opcode map: ";
	Core::CPUCore::reportOpcodeCoverage(INSTR_META, core.getPrimaryOpcodes());
	BOOST_LOG_TRIVIAL(info) << "CB opcode map: ";
	Core::CPUCore::reportOpcodeCoverage(INSTR_CB_META, core.getSecondaryOpcodes());
	exit(1);

	try
	{
		while (true)
		{
			core.cycle();
		}
	}
	catch (std::runtime_error e)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Caught error and exiting " << e.what();
	}

	std::cout << "Ending state of RAM: " << ram << std::endl;

}