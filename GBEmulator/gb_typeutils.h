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

// The normal amount to increment PC by after each instruction
static const WordType PC_INC_NORMAL = 1;

// Enum for a return type that describes whether 
// an opcodes cycle count should be that of its TAKEN or UNTAKEN count
// This changes, for example, in the case of JR/JP
typedef enum {
    CYCLE_TAKEN,
    CYCLE_UNTAKEN
} CYCLE_ACTION;

#endif 