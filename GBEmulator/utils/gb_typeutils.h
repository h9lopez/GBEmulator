#ifndef INCLUDED_GB_TYPEUTILS
#define INCLUDED_GB_TYPEUTILS

#include <cstdint>
#include <tuple>
#include <ostream>

typedef uint16_t WordType;
typedef uint8_t ByteType;
typedef bool Flag;

// For usage mainly within the register bank implementation
typedef union
{
	WordType word;
	struct {
		ByteType lo;
		ByteType hi;
	};
} WordRegister;
typedef ByteType ByteRegister;


static const int BYTETYPE_SIZE = 8;
static const int WORDTYPE_SIZE = BYTETYPE_SIZE * 2;

typedef uint16_t Address;
struct _AddressRange {
	Address start;
	Address end;

	_AddressRange()
		: start(0x0), end(0x0) {}
		
	_AddressRange(Address start, Address end)
		: start(start), end(start) {}

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



// Enum for a return type that describes whether 
// an opcodes cycle count should be that of its TAKEN or UNTAKEN count
// This changes, for example, in the case of JR/JP
typedef enum {
    CYCLE_TAKEN,
    CYCLE_UNTAKEN
} CycleAction;

// The normal amount to increment PC by after each instruction
static const WordType PC_INC_NORMAL = 1;
// Used in instances where we manually set the PC and don't want it overridden when we return
static const WordType PC_INC_BYPASS = 0;



#endif 