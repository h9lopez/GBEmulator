#ifndef INCLUDED_GB_RAM
#define INCLUDED_GB_RAM

#include "gb_typeutils.h"

class RAM
{
public:
    RAM();

    ByteType readByte(Address addr) const;
    WordType readWord(Address addr) const;

    void writeByte(Address addr, ByteType val);
    void writeWord(Address addr, WordType val);

	bool copyRangeFromBuffer(const char *arr, size_t begin, size_t len);

private:
    static const int MEMSIZE = 0xFFF;
    
    typedef uint8_t* Container;

    Container d_mem;
    int d_size;
};

#endif