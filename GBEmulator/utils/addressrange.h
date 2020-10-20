#ifndef INCLUDED_GB_ADDRESSRANGE_H
#define INCLUDED_GB_ADDRESSRANGE_H

#include <gb_typeutils.h>
#include <boost/generator_iterator.hpp>

struct _AddressRange {
	Address start;
	Address end;

	_AddressRange()
		: start(0x0), end(0x0) {}
		
	_AddressRange(Address start, Address end)
		: start(start), end(end) {}

	_AddressRange(const _AddressRange& r) 
		: start(r.start), end(r.end) {}

	bool operator <(const _AddressRange& y) const {
		return std::tie(start, end) < std::tie(y.start, y.end);
	}

	bool operator==(const _AddressRange& y) const {
		return std::tie(start, end) == std::tie(y.start, y.end);
	}
};

typedef _AddressRange AddressRange;


#endif