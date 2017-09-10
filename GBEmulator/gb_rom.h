#ifndef INCLUDED_GB_ROM
#define INCLUDED_GB_ROM

#include <string>
#include "gb_typeutils.h"
#include "gb_ram.h"

class ROMLoader
{
public:
    static bool fromFile(const std::string& filename, RAM &obj);

private:
	static const int MAX_ROMSIZE = 0x8000;

    ROMLoader();
};


#endif