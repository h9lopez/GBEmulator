#ifndef INCLUDED_GB_ROM
#define INCLUDED_GB_ROM

#include <string>
#include "gb_typeutils.h"
#include "gb_ram.h"

class ROMLoader
{
public:
    static bool fromFile(const std::string& filename, RAM &obj);

    template <typename Iter>
    static bool fromRawBytes(Iter beg, Iter end, RAM &obj);

private:
	static const int MAX_ROMSIZE = 0x8000;

    ROMLoader();
};


template <typename Iter>
bool ROMLoader::fromRawBytes(Iter beg, Iter end, RAM &obj)
{
	auto res = obj.copyRangeFromBuffer<Iter>(beg, end, obj.begin());

	if (!res)
	{
		std::cout << "ERROR: Copying ROM from byte list failed\n";
	}

	return res;
}


#endif