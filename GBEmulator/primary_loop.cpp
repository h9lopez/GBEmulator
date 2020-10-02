#include <iostream>
#include <cpu/CPUCore.h>
#include <display/gb_ascii_screen.h>
#include <romloader/gb_rom.h>
#include <fstream>


#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
using namespace std;


const string ARG_ROM_PATH = "romPath";
const string ARG_MEM_DUMP_PATH = "memDumpPath";

int main(int argc, char *argv[])
{
	static std::string bootROM = "/Users/hlopez34/Code/GBEmulator/GBEmulator_Test/ASMTest/simpleLoadTest.gb";
	//static std::string bootROM = "/Users/hlopez34/Code/GBEmulator/GBEmulator_Test/ASMTest/DMG_ROM.bin";
	static std::string memDumpPath = "";

	// Load command line args
	boost::program_options::options_description description("Allowed options");
	description.add_options()
				("romPath", boost::program_options::value<string>(), "Set startup ROM path")
				("memDumpPath", boost::program_options::value<string>(), "If set, will dump memory state post-execution of ROM completion.")
	;

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc,argv,description), vm);
	boost::program_options::notify(vm);

	if (vm.count(ARG_ROM_PATH)) {
		bootROM = vm[ARG_ROM_PATH].as< string >();
	}
	else
	{
		BOOST_LOG_TRIVIAL(error) << "No boot rom path provided";
		return -1;
	}
	if (vm.count(ARG_MEM_DUMP_PATH)) {
		memDumpPath = vm[ARG_MEM_DUMP_PATH].as<string>();
		BOOST_LOG_TRIVIAL(error) << "Post-execution RAM state will be stored at: " << memDumpPath;
	}

	// Initialize datamem and registers

	// Load ROM into memspace

	RAM ram;
	RegBank regs;
	
	BOOST_LOG_TRIVIAL(info) << "Attempting to load boot ROM";
	if (ROMLoader::fromFile(bootROM, ram))
	{
		BOOST_LOG_TRIVIAL(info) << "ROM loaded successfully\n";
	}
	else {
		BOOST_LOG_TRIVIAL(info) << "FAILED ROM LOADING\n";
	}

	Core::CPUCore core(ram, regs);

	BOOST_LOG_TRIVIAL(info) << "Regular opcode map: ";
	Core::CPUCore::reportOpcodeCoverage(INSTR_META, core.getPrimaryOpcodes());
	BOOST_LOG_TRIVIAL(info) << "CB opcode map: ";
	Core::CPUCore::reportOpcodeCoverage(INSTR_CB_META, core.getSecondaryOpcodes());

	BOOST_LOG_TRIVIAL(info) << "Starting ROM: ";
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

	BOOST_LOG_TRIVIAL(info) << "Ending state of RAM: " << ram;
	if (!memDumpPath.empty()) {
		std::ofstream fileStream;
		fileStream.open(memDumpPath, std::ios::out | std::ios::binary);

		fileStream << ram;
		fileStream.close();
	}
}