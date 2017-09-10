#include "gb_ram.h"
#include <algorithm>
#include <iterator>

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

RAM::IT RAM::begin()
{
	return std::begin(d_mem);
}

RAM::IT RAM::end()
{
	return std::end(d_mem);
}