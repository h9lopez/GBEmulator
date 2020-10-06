#include <iostream>
#include <cpu/CPUCore.h>
#include <display/gb_ascii_screen.h>
#include <display/gb_sdl_screen.h>
#include <romloader/gb_rom.h>
#include <fstream>
#include <SDL2/SDL.h>


#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
using namespace std;

// SDL consts
#define DEBUG_SDL_WINDOW_TITLE "[DEBUG] SDL test"
#define DEBUG_SDL_WINDOW_HEIGHT 256
#define DEBUG_SDL_WINDOW_WIDTH 256 

// ROM debug consts
#define ARG_ROM_PATH "romPath"
#define ARG_MEM_DUMP_PATH "memDumpPath"

SDL_Window* localSDLInit()
{
    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        BOOST_LOG_TRIVIAL(error) << "Failed to initialize SDL video";
        return NULL;
    }
    
    auto sdlWindow = SDL_CreateWindow(DEBUG_SDL_WINDOW_TITLE, 
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                    DEBUG_SDL_WINDOW_WIDTH, DEBUG_SDL_WINDOW_HEIGHT, 
                    SDL_WINDOW_SHOWN);
    if (sdlWindow == NULL) {
        BOOST_LOG_TRIVIAL(error) << "Failed to create SDL window";
        return NULL;
    }
    return sdlWindow;
}


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

	// Initialize a default display palette for us to use
	// TODO: Load display palette from config
	DisplayPalette screenPalette(
		SDL_Color{.r = 0, .g = 0, .b = 0},
		SDL_Color{.r = 63, .g = 63, .b = 63},
		SDL_Color{.r = 147, .g = 147, .b = 147},
		SDL_Color{.r = 255, .g = 255, .b = 255}
	);

	auto sdlWindow = localSDLInit();
	RAM ram;
	SDLScreen screen(&ram, sdlWindow, screenPalette);
	RegBank regs;

	SDL_ShowWindow(sdlWindow);

	// Add a watcher for the BTT found at range 9800-9BFF
	AddressRange bttRange;
	bttRange.start = 0x9800;
	bttRange.end = 0x9BFF;
	ram.addSegmentWatcher(bttRange, boost::bind(&SDLScreen::processBTTUpdate, &screen, _1, _2));

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
	//Core::CPUCore::reportOpcodeCoverage(INSTR_META, core.getPrimaryOpcodes());
	BOOST_LOG_TRIVIAL(info) << "CB opcode map: ";
	//Core::CPUCore::reportOpcodeCoverage(INSTR_CB_META, core.getSecondaryOpcodes());

	BOOST_LOG_TRIVIAL(info) << "Starting ROM: ";
	try
	{

		SDL_Event ev;
		bool quit = false;
		bool cpuStall = false;
		while (!quit) {
			try {
				if (!cpuStall) {
					core.cycle();
				}

				screen.drawScreen();
			}
			catch (std::runtime_error ex) {
				cpuStall = true;
			}

			while (SDL_PollEvent(&ev)){
				if (ev.type == SDL_QUIT){
					quit = true;
				}
				if (ev.type == SDL_KEYDOWN){
				}
				if (ev.type == SDL_MOUSEBUTTONDOWN){
				}
			}
		}
	}
	catch (std::runtime_error ex)
	{
		BOOST_LOG_TRIVIAL(fatal) << "Caught error and exiting " << ex.what();
	}



	SDL_DestroyWindow(sdlWindow);

	//BOOST_LOG_TRIVIAL(info) << "Ending state of RAM: " << ram;
	if (!memDumpPath.empty()) {
		std::ofstream fileStream;
		fileStream.open(memDumpPath, std::ios::out | std::ios::binary);

		fileStream << ram;
		fileStream.close();
	}
}