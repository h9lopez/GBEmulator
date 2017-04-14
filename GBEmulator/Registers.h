#include <stdint.h>
#pragma once

typedef bool Flag;

typedef union
{
	uint16_t word;
	struct {
		uint8_t hi;
		uint8_t lo;
	};
} Register;

typedef struct {
	uint8_t	A;
	Flag zero;
	Flag subtract;
	Flag half_carry;
	Flag carry;
	Register	BC;
	Register	DE;
	Register	HL;
	Register	SP;
	Register	PC;
} RegisterBank;