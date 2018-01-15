#include "gb_ram.h"
#include <algorithm>
#include <iterator>

RAM::RAM()
{}

void RAM::copyCurrentState(RAM & dest) const
{
	// hard copy all variables to new destination
	// TODO: Has possibility of failure
	dest.copyRangeFromBuffer(this->d_mem.begin(), this->d_mem.end(), dest.begin());
}

ByteType RAM::readByte(Address addr) const
{
	return ByteType(d_mem[addr]);
}

WordType RAM::readWord(Address addr) const
{
	return ((WordType*)(&d_mem[addr]))[0];
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

RAM::CIT RAM::cbegin() const
{
	return std::cbegin(d_mem);
}

RAM::CIT RAM::cend() const
{
	return std::cend(d_mem);
}