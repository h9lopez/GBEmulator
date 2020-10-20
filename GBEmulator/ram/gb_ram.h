#ifndef INCLUDED_GB_RAM
#define INCLUDED_GB_RAM

#include <utils/gb_typeutils.h>
#include <utils/addressrange.h>
#include <array>
#include <iterator>
#include <ostream>
#include <fstream>
#include <iomanip>

#include <boost/log/trivial.hpp>

#include <boost/signals2.hpp>

class RAM
{
private:
	static const int MEMSIZE = 0xFFFF;
	typedef std::array<uint8_t, MEMSIZE> Container;
	typedef Container::iterator IT;
	typedef Container::const_iterator CIT;


public:
	typedef union {
		WordType word;
		ByteType byte;
	} SegmentUpdateData;

	RAM();

	void addSegmentWatcher(const AddressRange& range, boost::signals2::slot<void (Address, SegmentUpdateData)> watcher);

	// helper function for debugging
	void copyCurrentState(RAM& dest) const;

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
	boost::signals2::signal<void (Address, SegmentUpdateData)> SegmentUpdateSignal;
    Container d_mem;
    int d_size;
	std::map<AddressRange, std::shared_ptr<boost::signals2::signal<void (Address, SegmentUpdateData)> > > d_signalMap;

	friend std::ostream& operator<<(std::ostream& os, const RAM& ram)
	{
		os << "[RAM DUMP, BOUNDS (MIN = 0, MAX = " << RAM::MEMSIZE << ")" << std::endl;

		// This loop is definitely assuming an even boundary for the memory
		for (int i = 0; i < RAM::MEMSIZE; i += 0x10)
		{
			os << std::setfill('0') << std::setw(4) << std::hex << i << ": ";
			for (int j = 0; j != 0x10; j++)
			{
				os << " " << std::setfill('0') << std::setw(2) << std::hex << +ram.d_mem[i + j] << " ";
			}
			os << std::endl;
		}
		os << "]" << std::endl;
		return os;
	}

	friend std::ofstream& operator<<(std::ofstream& os, const RAM& ram)
	{
		for (auto it = ram.cbegin(); it != ram.cend(); ++it)
		{
			unsigned char temp = static_cast<unsigned char>(*it);
			os << temp;
		}

		return os;
	}

	friend std::ifstream& operator>>(std::ifstream& is, RAM& ram)
	{
		Address addr;
		for (addr = 0;  addr < RAM::MEMSIZE; addr++)
		{
			unsigned char temp;
			is >> temp;
			ram.writeByte(addr, static_cast<ByteType>(temp));
		}
		return is;
	}
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
