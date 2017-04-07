#include <stdint.h>
#pragma once


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
	uint8_t	F;
	Register	BC;
	Register	DE;
	Register	HL;
	Register	SP;
	Register	PC;
} RegisterBank;