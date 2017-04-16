#include <stdint.h>
#pragma once

typedef bool Flag;

typedef union
{
	uint16_t word;
	struct {
		uint8_t lo;
		uint8_t hi;
	};
} Register;

typedef struct {
	Register	A; // only the lo byte in this register will be paid attention to
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