#include "gb_regs.h"

RegBank::RegBank()
{
}

ByteType RegBank::A() const
{
	return ByteType(d_regA);
}

ByteType RegBank::B() const
{
	return ByteType(d_regBC.hi);
}

ByteType RegBank::C() const
{
	return ByteType(d_regBC.lo);
}

ByteType RegBank::D() const
{
	return ByteType(d_regDE.hi);
}

ByteType RegBank::E() const
{
	return ByteType(d_regDE.lo);
}

ByteType RegBank::H() const
{
	return ByteType(d_regHL.hi);
}

ByteType RegBank::L() const
{
	return ByteType(d_regHL.lo);
}

WordType RegBank::BC() const
{
	return WordType(d_regBC.word);
}

WordType RegBank::DE() const
{
	return WordType(d_regDE.word);
}

WordType RegBank::HL() const
{
	return WordType(d_regHL.word);
}

WordType RegBank::SP() const
{
	return WordType(d_regSP.word);
}

WordType RegBank::PC() const
{
	return WordType(d_regPC.word);
}

void RegBank::A(ByteType val)
{
	d_regA = val;
}

void RegBank::B(ByteType val)
{
	d_regBC.hi = val;
}

void RegBank::C(ByteType val)
{
	d_regBC.lo = val;
}

void RegBank::D(ByteType val)
{
	d_regDE.hi = val;
}

void RegBank::E(ByteType val)
{
	d_regDE.lo = val;
}

void RegBank::H(ByteType val)
{
	d_regHL.hi = val;
}

void RegBank::L(ByteType val)
{
	d_regHL.lo = val;
}

void RegBank::BC(WordType val)
{
	d_regBC.word = val;
}

void RegBank::DE(WordType val)
{
	d_regDE.word = val;
}

void RegBank::HL(WordType val)
{
	d_regHL.word = val;
}

void RegBank::SP(WordType val)
{
	d_regSP.word = val;
}

void RegBank::PC(WordType val)
{
	d_regPC.word = val;
}

WordType RegBank::IncPC()
{
	return (d_regPC.word++);
}

Flag RegBank::flagZero() const
{
	return d_fzero;
}

Flag RegBank::flagSubtract() const
{
	return d_fsub;
}

Flag RegBank::flagHalfCarry() const
{
	return d_fhc;
}

Flag RegBank::flagCarry() const
{
	return d_fc;
}

void RegBank::flagZero(bool on)
{
	d_fzero = on;
}

void RegBank::flagSubtract(bool on)
{
	d_fsub = on;
}

void RegBank::flagHalfCarry(bool on)
{
	d_fhc = on;
}

void RegBank::flagCarry(bool on)
{
	d_fc = on;
}
