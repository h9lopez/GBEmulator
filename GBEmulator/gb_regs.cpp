#include "gb_regs.h"

RegBank::RegBank()
	: d_fzero(0), d_fsub(0), d_fhc(0), d_regA(0)
{
	d_regBC.word = 0;
	d_regDE.word = 0;
	d_regHL.word = 0;
	d_regSP.word = 0;
	d_regPC.word = 0;
}


void RegBank::copyCurrentState(RegBank& bank)
{
	// hard copying pretty much all of our state 
	bank.d_fc = 	this->d_fc;
	bank.d_fhc = 	this->d_fhc;
	bank.d_fsub = 	this->d_fsub;
	bank.d_fzero =	this->d_fzero;
	bank.d_regA = 	this->d_regA;
	bank.d_regBC = 	this->d_regBC;
	bank.d_regDE = 	this->d_regDE;
	bank.d_regHL = 	this->d_regHL;
	bank.d_regPC = 	this->d_regPC;
	bank.d_regSP = 	this->d_regSP;
	return;
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

void RegBank::IncPCBy(WordType val)
{
	d_regPC.word += val;
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
