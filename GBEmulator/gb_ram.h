#ifndef INCLUDED_GB_RAM
#define INCLUDED_GB_RAM

#include "gb_typeutils.h"
#include <array>
#include <iterator>

class RAM
{
private:
	static const int MEMSIZE = 0xFFF;
	typedef std::array<uint8_t, MEMSIZE> Container;
	typedef Container::iterator IT;
	typedef Container::const_iterator CIT;

public:
    RAM();

    ByteType readByte(Address addr) const;
    WordType readWord(Address addr) const;

    void writeByte(Address addr, ByteType val);
    void writeWord(Address addr, WordType val);

	template<typename Iter>
	bool copyRangeFromBuffer(Iter begin, Iter end, IT memTarget);

	// Getters 
	IT begin();
	CIT cbegin() const;
	IT end();
	CIT cend() const;

private:
    Container d_mem;
    int d_size;
};


template<typename Iter>
bool RAM::copyRangeFromBuffer(Iter fBegin, Iter fEnd, RAM::IT memTarget)
{
	if (fBegin == fEnd || memTarget == this->end())
	{
		return false;
	}

	std::copy(fBegin, fEnd, memTarget);
	return true;
}


#endif
