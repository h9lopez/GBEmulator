#ifndef INCLUDED_GB_TYPEUTILS
#define INCLUDED_GB_TYPEUTILS

#include <stdint.h>
#include <tuple>

typedef uint16_t WordType;
typedef uint8_t ByteType;
typedef bool Flag;

static const int BYTETYPE_SIZE = 8;

typedef uint16_t Address;

// Typedef for the return type of the opcode functions.
// A tuple of the value of PC increment that should occur after
// and the second value the number of cycles taken by the opcode
typedef std::tuple<WordType, ByteType> OpResult;

#endif 