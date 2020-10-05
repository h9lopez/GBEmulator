#include "gb_ram.h"
#include <algorithm>
#include <iterator>

#include <boost/signals2.hpp>


RAM::RAM()
{
}

void RAM::addSegmentWatcher(const AddressRange& range, boost::signals2::slot<void (Address, SegmentUpdateData)> watcher)
{
	auto it = std::find_if(d_signalMap.begin(), d_signalMap.end(), [range](const std::pair<AddressRange, std::shared_ptr<boost::signals2::signal<void (Address, SegmentUpdateData)> > >& item) {
		return range == item.first;
	});

	typedef boost::signals2::signal<void (Address, SegmentUpdateData)> SegmentSignalType;

	if (it == d_signalMap.end()) {
		BOOST_LOG_TRIVIAL(debug) << "No exisiting watcher for Address Range, adding new";
		std::shared_ptr<SegmentSignalType> newSignal = std::make_shared<SegmentSignalType>();
		d_signalMap.insert(std::pair<AddressRange, std::shared_ptr<SegmentSignalType> >(range, newSignal));
	}
	
	d_signalMap[range]->connect(watcher);
	BOOST_LOG_TRIVIAL(debug) << "Connected watcher to range "  << "[AddressRange " << std::hex << range.start << " - " << std::hex << range.end;
}


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
	RAM::SegmentUpdateData busData;
	busData.byte = val;
	SegmentUpdateSignal(addr, busData);

	// Search for specific watchers for this memory region
	auto res = std::find_if(d_signalMap.begin(), d_signalMap.end(), [addr](const std::pair<AddressRange, std::shared_ptr<boost::signals2::signal<void (Address, SegmentUpdateData)> > >& item) {
		return (addr >= item.first.start && addr < item.first.end);
	});

	if (res != d_signalMap.end()) {
		// Alert
		auto sig = res->second;
		(*sig)(addr, busData);
	}
}

void RAM::writeWord(Address addr, WordType val)
{
	((WordType*)d_mem[addr])[0] = val;
	RAM::SegmentUpdateData busData;
	busData.byte = val;
	SegmentUpdateSignal(addr, busData);

	// Search for specific watchers for this memory region
	auto res = std::find_if(d_signalMap.begin(), d_signalMap.end(), [addr](const std::pair<AddressRange, std::shared_ptr<boost::signals2::signal<void (Address, SegmentUpdateData)> > >& item) {
		return (addr >= item.first.start && addr < item.first.end);
	});

	if (res != d_signalMap.end()) {
		// Alert
		auto sig = res->second;
		(*sig)(addr, busData);
	}
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