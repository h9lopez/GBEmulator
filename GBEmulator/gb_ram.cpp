#include "gb_ram.h"

RAM::RAM()
{}

ByteType RAM::readByte(Address addr) const
{
	return ByteType(d_mem[addr]);
}

WordType RAM::readWord(Address addr) const
{
	return WordType(((WordType*)(d_mem[addr]))[0]);
}

void RAM::writeByte(Address addr, ByteType val)
{
	d_mem[addr] = val;
}

void RAM::writeWord(Address addr, WordType val)
{
	((WordType*)d_mem[addr])[0] = val;
}

bool RAM::copyRangeFromBuffer(const char * arr, size_t begin, size_t len)
{
	if (!arr || begin < 0) {
		return false;
	}
	if ((begin + len) >= MEMSIZE) {
		return false;
	}

	for (size_t i = begin; i < len && i < MEMSIZE; ++i)
	{
		d_mem[i] = arr[i];
	}
	return true;
}
