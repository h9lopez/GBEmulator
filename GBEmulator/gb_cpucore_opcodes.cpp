#include <bitset>
#include <iostream>

#include "gb_typeutils.h"
#include "CPUCore.h"
#include "OpcodeResultContext.h"

using namespace Core;

// This namespace will contain generic helper functions that don't modify any
// register/cpu state
namespace helpers {
	ByteType circularRotateLeft(ByteType in)
	{
		std::bitset<BYTETYPE_SIZE> bytes(in);
		bool highbit = bytes[bytes.size() - 1];
		bytes <<= 1;
		bytes[0] = highbit;
		
		return (ByteType)bytes.to_ulong();
	}

	ByteType circularRotateRight(ByteType in)
	{
		std::bitset<BYTETYPE_SIZE> bytes(in);
		bool lowbit = bytes[0];
		bytes >>= 1;
		bytes[bytes.size()-1] = lowbit;

		return (ByteType)bytes.to_ulong();
	}
}

// This namespace will hold common register operations that can be used throughout
// various operations (i.e. abstracted operations like ADD x)
namespace {
	void incrementRegister(RegBank &regs,
		std::function<void(ByteType)> regSet,
		std::function<ByteType(void)> regGet)
	{
		ByteType regVal = regGet();
		bool isHalfCarry = ((regVal & 0xf) + (1 & 0xf)) & 0x10;
		++regVal;
		regSet(regVal);
		if (regVal == 0) {
			regs.flagZero(true);
		}
		regs.flagSubtract(false);
		regs.flagHalfCarry(isHalfCarry);
	}

	void decrementRegister(RegBank &regs,
		std::function<void(ByteType)> regSet,
		std::function<ByteType(void)> regGet)
	{
		ByteType regVal = regGet();
		// TODO: Check accuracy of this line for subtracts
		bool isHalfCarry = ((regVal & 0xf) - (1 & 0xf)) & 0x10;
		--regVal;
		regSet(regVal);
		if (regVal == 0) {
			regs.flagZero(true);
		}
		regs.flagSubtract(true);
		regs.flagHalfCarry(isHalfCarry);
	}

	void xorRegister(RegBank &regs,
		std::function<ByteType(void)> regGet)
	{
		ByteType regVal = regGet();

		regs.A( regVal ^ regs.A() );
		regs.flagZero(regs.A() == 0x0);
	}

	void checkBit(RegBank &regs,
				  std::function<ByteType(void)> regGet,
				  unsigned int bitn)
	{
		bool isOn = (regGet() >> bitn) & 0x1;
		regs.flagZero(!isOn);
		regs.flagSubtract(false);
		regs.flagHalfCarry(true);
	}

	void loadIntoHL(RegBank &regs,
					std::function<WordType(void)> valGet)
	{
		WordType val = valGet();
		bool isHalfCarry = ((regs.HL() & 0xf) + (val & 0xf)) & 0x10;

		unsigned int sum = static_cast<unsigned int>(regs.HL()) +
						   static_cast<unsigned int>(val);
		bool isCarry = sum & 0x100;

		regs.HL((ByteType)sum);
		// Set flags
		regs.flagSubtract(false);
		regs.flagHalfCarry(isHalfCarry);
		regs.flagCarry(isCarry);
		// zero flag is ignored	
	}

	void loadByteIntoRegister(std::function<void(ByteType)> regSet,
							std::function<ByteType(void)> valGet)
	{
		// NOTE: This function may seem useless but may come in handy later
		//		 when we start doing profiling or debugging.
		regSet( valGet() );
	}

	void performByteAdd(RegBank &regs,
						std::function<void(ByteType)> regSetA,
						std::function<ByteType(void)> regGetA,
						std::function<ByteType(void)> regGetB)
	{
		bool isHalfCarry = ((regGetA() & 0xf) + (regGetB() & 0xf)) & 0x10;
		unsigned int sum = static_cast<unsigned int>(regGetA()) +
						   static_cast<unsigned int>(regGetB());
		bool isCarry = sum & 0x100;

		regSetA( (ByteType)sum );
		
		regs.flagZero( (sum & 0xFF) == 0 );
		regs.flagSubtract( false );
		regs.flagHalfCarry(isHalfCarry);
		regs.flagCarry(isCarry);	
	}

	void performByteSub(RegBank &regs,
						std::function<void(ByteType)> regSetA,
						std::function<ByteType(void)> regGetA,
						std::function<ByteType(void)> regGetB)
	{
		// This might be unhealthily expensive
		bool isHalfCarry = !((((regGetA() & 0xf) | 0x10) - (regGetB() & 0xf)) & 0x10);
		// Can we just take the twos complement of a value and do that?

		ByteType a = regGetA();
		ByteType b = regGetB();
		unsigned int result = static_cast<unsigned int>(regGetA()) -
							  ((static_cast<unsigned int>(regGetB())));

		bool isCarry = ((int)result) & 0x100;

		regSetA( static_cast<ByteType>(result) );

		regs.flagZero( (result & 0xFF) == 0 );
		regs.flagSubtract(true);
		regs.flagHalfCarry(isHalfCarry);
		regs.flagCarry(isCarry);
	}

	void performSBC(RegBank &regs,
		std::function<ByteType(void)> secondaryRegGet)
	{
		bool flagCarry = regs.flagCarry();

		performByteSub(regs, 
			[&regs](ByteType t) { regs.A(t); },
			[&regs]() { return regs.A(); },
			secondaryRegGet
		);

		performByteSub(regs, 
			[&regs](ByteType t) { regs.A(t); },
			[&regs]() { return regs.A(); },
			[&regs, flagCarry]() { return flagCarry; }
		);
	}

	void performADC(RegBank &regs,
		std::function<ByteType(void)> secondaryRegGet)
	{
		bool flagCarry = regs.flagCarry();

		performByteAdd(regs,
			[&regs](ByteType t) { regs.A(t); },
			[&regs]() { return regs.A(); },
			secondaryRegGet
		);

		performByteAdd(regs,
			[&regs](ByteType t) {regs.A(t); },
			[&regs]() { return regs.A(); },
			[&regs, flagCarry]() { return flagCarry; }
		);
	}

	void performAND(RegBank &regs,
		std::function<ByteType(void)> rhsGet)
	{
		ByteType result = regs.A() & rhsGet();

		regs.A(result);

		regs.flagZero(result == 0);
		regs.flagSubtract(0);
		regs.flagHalfCarry(1);
		regs.flagCarry(0);
	}

	void performOR(RegBank &regs,
		std::function<ByteType(void)> rhsGet)
	{
		ByteType result = regs.A() | rhsGet();

		regs.A(result);

		regs.flagZero(result == 0x0);
		regs.flagSubtract(0);
		regs.flagHalfCarry(0);
		regs.flagCarry(0);
	}

	void popSP(RegBank &regs, const RAM &ram, std::function<void(WordType)> rhsSet)
	{
		// Construct the word type to set
		WordRegister construct;
		construct.lo = ram.readByte(regs.SP());
		construct.hi = ram.readByte(regs.SP() + 1);
		
		rhsSet(construct.word);

		regs.SP(regs.SP() + 2);
	}

	void pushSP(RegBank &regs, RAM &ram, std::function<WordType()> rhsGet)
	{
		WordRegister construct;
		construct.word = rhsGet();

		ram.writeByte(regs.SP() - 1, construct.hi);
		ram.writeByte(regs.SP() - 2, construct.lo);
		regs.SP( regs.SP() - 2 );
	}

	void registerRst(RegBank &regs, RAM &ram, ByteType f_val)
	{
		// (SP-1) <- PCh, (SP-2) <- PCl, PCh <- 0, PCl <- f, SP <- SP-2
		WordRegister pcReg;
		pcReg.word = regs.PC();

		ram.writeByte(regs.SP() - 1, pcReg.hi);
		ram.writeByte(regs.SP() - 2, pcReg.lo);
		
		// NOTE: An api here that could set the individual high/lo would be way better than reconstructing new thing
		WordRegister newPC;
		newPC.hi = 0;
		newPC.lo = f_val;

		regs.PC(newPC.word);
		regs.SP( regs.SP() - 2 );
	}

	void performCall(RegBank &regs, RAM &ram, WordType address)
	{
		// (SP-1) <- PCh, (SP-2) <- PCl, PC <- nn, SP <- SP-2
		WordRegister pcReg;
		pcReg.word = regs.PC();
		
		ram.writeByte(regs.SP() - 1, pcReg.hi);
		ram.writeByte(regs.SP() - 2, pcReg.lo);

		regs.PC(address);

		regs.SP( regs.SP() - 2 );
	}

	// For use in RL opcodes
	void fullCircularRotateLeft(RegBank &regs, RAM &ram,
								std::function<void(ByteType)> regSet,
								std::function<ByteType(void)> regGet)
	{
		std::bitset<BYTETYPE_SIZE> bytes(regGet());
		bool highbit = bytes[bytes.size() - 1];
		bytes <<= 1;
		bytes[0] = regs.flagCarry();

		regSet( (ByteType)bytes.to_ulong() );
		regs.flagCarry(highbit);
		regs.flagZero( bytes.to_ulong() == 0 );
	}
}


using namespace std;


void CPUCore::initOpcodes()
{
	// Initialize regular opcodes

	// NO OP
	d_opcodes[0x00] = [this]() { 
		return OpcodeResultContext::Builder(0x00).ShortCycle().IncrementPCDefault().Build();
	};

	// RLCA
	d_opcodes[0x07] = [this]() {
		// left bit goes to carry flag and replaces 0 bit
		ByteType target = d_regs->A();
		ByteType highbit = target & 0x80;
		
		d_regs->flagCarry(highbit);
		d_regs->flagZero(false);
		d_regs->flagSubtract(false);
		d_regs->flagHalfCarry(false);
		d_regs->A(helpers::circularRotateLeft(target));
		return OpcodeResultContext::Builder(0x07).ShortCycle().IncrementPCDefault().Build();
	};

	// RLA
	d_opcodes[0x17] = [this]() {
		// current value in carry flag goes to bit 0 of A
		// then bit 7 of A goes to carry flag
		
		std::bitset<BYTETYPE_SIZE> val(d_regs->A());
		bool highbit = val[val.size() - 1];

		val <<= 1;
		val[0] = d_regs->flagCarry();
		d_regs->flagCarry( highbit );
		d_regs->A( (ByteType)val.to_ulong() );

		d_regs->flagZero(false);
		d_regs->flagSubtract(false);
		d_regs->flagHalfCarry(false);
		return OpcodeResultContext::Builder(0x17).ShortCycle().IncrementPCDefault().Build();
	};

	// RRCA
	d_opcodes[0x0F] = [this]() {
		ByteType target = d_regs->A();
		ByteType lowbit = target & 0x01;

		d_regs->flagCarry(lowbit);
		d_regs->flagZero(false);
		d_regs->flagSubtract(false);
		d_regs->flagHalfCarry(false);

		d_regs->A(helpers::circularRotateRight(target));
		return OpcodeResultContext::Builder(0x0F).ShortCycle().IncrementPCDefault().Build();
	};

	// RRA
	d_opcodes[0x1F] = [this]() {
		// high bit of A becomes carry flag, then carry flag becomes low bit,
		// rest of the byte is shifted

		std::bitset<BYTETYPE_SIZE> val(d_regs->A());
		bool lowbit = val[0];

		val >>= 1;
		val[val.size() - 1] = d_regs->flagCarry();
		d_regs->flagCarry(lowbit);
		d_regs->A( (ByteType)val.to_ulong() );
		d_regs->flagZero(false);
		d_regs->flagHalfCarry(false);
		d_regs->flagSubtract(false);

		return OpcodeResultContext::Builder(0x1F).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (a16), SP
	d_opcodes[0x08] = [this]()
	{
		// No flags affected
		d_ram->writeWord( readNextTwoBytes(), d_regs->SP() );
		return OpcodeResultContext::Builder(0x08).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD HL, BC
	d_opcodes[0x09] = [this]()
	{
		loadIntoHL(*d_regs, [this]() { return d_regs->BC(); });

		return OpcodeResultContext::Builder(0x09).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD HL, DE
	d_opcodes[0x19] = [this]()
	{
		loadIntoHL(*d_regs, [this]() { return d_regs->DE(); });
		return OpcodeResultContext::Builder(0x19).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD HL, HL
	d_opcodes[0x29] = [this]()
	{
		loadIntoHL(*d_regs, [this]() { return d_regs->HL(); });
		return OpcodeResultContext::Builder(0x29).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD HL, SP
	d_opcodes[0x39] = [this]()
	{
		loadIntoHL(*d_regs, [this]() { return d_regs->SP(); });
		return OpcodeResultContext::Builder(0x39).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, (BC)
	d_opcodes[0x0A] = [this]() 
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_ram->readByte( d_regs->BC() ); }
		);

		return OpcodeResultContext::Builder(0x0A).ShortCycle().IncrementPCDefault().Build();

	};

	// LD A, (DE)
	d_opcodes[0x1A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_ram->readByte( d_regs->DE() ); }
		);
		return OpcodeResultContext::Builder(0x1A).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, (HL+) aka LDI A, (HL)
	d_opcodes[0x2A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_ram->readByte( d_regs->HL() ); }
		);

		// We don't use the regular register increment helper because this 
		// operation doesn't set any flags related to the increment
		d_regs->HL( d_regs->HL() + 1 );

		return OpcodeResultContext::Builder(0x2A).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, (HL-) aka LDD A, (HL)
	d_opcodes[0x3A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);

		// We don't use the regular register decrement helper because this 
		// operation doesn't set any flags related to the decrement
		d_regs->HL(d_regs->HL() - 1);

		return OpcodeResultContext::Builder(0x3A).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD SP, r8
	d_opcodes[0xE8] = [this]()
	{
		ByteType incVal = readNextByte();

		// Detect carry and half carry
		bool isHalfCarry = ((d_regs->SP() & 0xf) + (incVal & 0xf)) & 0x10;
		unsigned int interimSum = static_cast<unsigned int>(d_regs->SP()) + 
							  static_cast<unsigned int>(incVal);
		bool isCarry = interimSum & 0x100;

		d_regs->SP( d_regs->SP() + incVal );
		d_regs->flagZero(false);
		d_regs->flagSubtract(false);
		return OpcodeResultContext::Builder(0xE8).ShortCycle().IncrementPCDefault().Build();
	};

	// LD HL,SP+r8
	d_opcodes[0xF8] = [this]()
	{
		ByteType incVal = readNextByte();

		// Detect carry and half carry
		bool isHalfCarry = ((d_regs->SP() & 0xf) + (incVal & 0xf)) & 0x10;
		unsigned int interimSum = static_cast<unsigned int>(d_regs->SP()) + 
							  static_cast<unsigned int>(incVal);
		bool isCarry = interimSum & 0x100;

		// NOTE: This downcast should probably be tested
		WordType loadVal = d_ram->readWord(static_cast<ByteType>(interimSum));

		d_regs->HL(loadVal);

		// Set flags
		d_regs->flagZero(false);
		d_regs->flagSubtract(false);
		return OpcodeResultContext::Builder(0xF8).ShortCycle().IncrementPCDefault().Build();
	};

	// LD SP,HL
	d_opcodes[0xF9] = [this]()
	{
		d_regs->SP(d_regs->HL());
		return OpcodeResultContext::Builder(0xF9).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (a16),A
	d_opcodes[0xEA] = [this]()
	{
		WordType memAddr = readNextTwoBytes();
		d_ram->writeByte(memAddr, d_regs->A());
		return OpcodeResultContext::Builder(0xEA).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A,(a16)
	d_opcodes[0xFA] = [this]()
	{
		WordType rval = readNextTwoBytes();
		d_regs->A(rval);
		return OpcodeResultContext::Builder(0xFA).ShortCycle().IncrementPCDefault().Build();
	};

	// JR NZ, r8
	d_opcodes[0x20] = [this]() 
	{
		ByteType incAmt = readNextByte();

		if (!d_regs->flagZero())
		{
			// Do jump
			return OpcodeResultContext::Builder(0x20).LongCycle().IncrementPCBy(incAmt).Build();
		}

		return OpcodeResultContext::Builder(0x20).ShortCycle().IncrementPCDefault().Build();
	};

	// JR NC, r8
	d_opcodes[0x30] = [this]()
	{
		ByteType incAmt = readNextByte();
		if (!d_regs->flagCarry())
		{
			return OpcodeResultContext::Builder(0x30).LongCycle().IncrementPCBy(incAmt).Build();
		}

		return OpcodeResultContext::Builder(0x30).ShortCycle().IncrementPCDefault().Build();
	};

	// JR r8
	d_opcodes[0x18] = [this]()
	{
		ByteType incAmt = readNextByte();
		return OpcodeResultContext::Builder(0x18).LongCycle().IncrementPCBy(incAmt).Build();
	};

	// JR Z, r8
	d_opcodes[0x28] = [this]()
	{
		ByteType incAmt = readNextByte();
		if (d_regs->flagZero())
		{
			return OpcodeResultContext::Builder(0x28).LongCycle().IncrementPCBy(incAmt).Build();
		}
		return OpcodeResultContext::Builder(0x28).ShortCycle().IncrementPCDefault().Build();
	};

	// JR C, r8
	d_opcodes[0x38] = [this]()
	{
		ByteType incAmt = readNextByte();
		if (d_regs->flagCarry())
		{
			return OpcodeResultContext::Builder(0x38).LongCycle().IncrementPCBy(incAmt).Build();
		}
		return OpcodeResultContext::Builder(0x38).ShortCycle().IncrementPCDefault().Build();
	};

	// ======================== 16bit immediate load -> 16bit regs
	d_opcodes[0x01] = [this]() { 
		d_regs->BC(readNextTwoBytes()); 
		return OpcodeResultContext::Builder(0x01).ShortCycle().IncrementPCDefault().Build();
	};
	d_opcodes[0x11] = [this]() { 
		d_regs->DE(readNextTwoBytes()); 
		return OpcodeResultContext::Builder(0x11).ShortCycle().IncrementPCDefault().Build();
	};
	d_opcodes[0x21] = [this]() { 
		d_regs->HL(readNextTwoBytes()); 
		return OpcodeResultContext::Builder(0x21).ShortCycle().IncrementPCDefault().Build();
	};
	d_opcodes[0x31] = [this]() { 
		d_regs->SP(readNextTwoBytes()); 
		return OpcodeResultContext::Builder(0x31).ShortCycle().IncrementPCDefault().Build();
	};

	// INC Increments
	d_opcodes[0x03] = [this]() { 
		d_regs->BC(d_regs->BC() + 1); 
		return OpcodeResultContext::Builder(0x03).ShortCycle().IncrementPCDefault().Build();
	};
	d_opcodes[0x13] = [this]() { 
		d_regs->DE(d_regs->DE() + 1); 
		return OpcodeResultContext::Builder(0x13).ShortCycle().IncrementPCDefault().Build();
	};
	d_opcodes[0x23] = [this]() { 
		d_regs->HL(d_regs->HL() + 1); 
		return OpcodeResultContext::Builder(0x23).ShortCycle().IncrementPCDefault().Build();
	};
	d_opcodes[0x33] = [this]() { 
		d_regs->SP(d_regs->SP() + 1); 
		return OpcodeResultContext::Builder(0x33).ShortCycle().IncrementPCDefault().Build();
	};
	// individual regs

	// INC B
	d_opcodes[0x04] = [this]() 
	{ 
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_regs->B(); });

		return OpcodeResultContext::Builder(0x04).ShortCycle().IncrementPCDefault().Build();
	};
	
	// INC D
	d_opcodes[0x14] = [this]() 
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->D(t); },
			[this]() { return d_regs->D(); });
		return OpcodeResultContext::Builder(0x14).ShortCycle().IncrementPCDefault().Build();
	};

	// INC H
	d_opcodes[0x24] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->H(t); },
			[this]() { return d_regs->H(); });
		return OpcodeResultContext::Builder(0x24).ShortCycle().IncrementPCDefault().Build();
	};
	
	// INC (HL)
	d_opcodes[0x34] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_ram->writeByte(d_regs->HL(), t); },
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return OpcodeResultContext::Builder(0x34).ShortCycle().IncrementPCDefault().Build();
	};

	// INC C
	d_opcodes[0x0C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->C(t); },
			[this]() { return d_regs->C(); });
		return OpcodeResultContext::Builder(0x0C).ShortCycle().IncrementPCDefault().Build();
	};

	// INC E
	d_opcodes[0x1C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->E(t); },
			[this]() { return d_regs->E(); });
		return OpcodeResultContext::Builder(0x1C).ShortCycle().IncrementPCDefault().Build();
	};

	// INC L
	d_opcodes[0x2C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->L(t); },
			[this]() { return d_regs->L(); });
		return OpcodeResultContext::Builder(0x2C).ShortCycle().IncrementPCDefault().Build();
	};

	// INC A
	d_opcodes[0x3C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); });
		return OpcodeResultContext::Builder(0x3C).ShortCycle().IncrementPCDefault().Build();
	};

	// DEC instructions

	// DEC B
	d_opcodes[0x05] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_regs->B(); });
		return OpcodeResultContext::Builder(0x05).ShortCycle().IncrementPCDefault().Build();
	};

	// DEC D
	d_opcodes[0x15] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->D(t); },
			[this]() { return d_regs->D(); });
		return OpcodeResultContext::Builder(0x15).ShortCycle().IncrementPCDefault().Build();
	};
	
	// DEC H
	d_opcodes[0x25] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->H(t); },
			[this]() { return d_regs->H(); });
		return OpcodeResultContext::Builder(0x25).ShortCycle().IncrementPCDefault().Build();
	};

	// DEC (HL)
	d_opcodes[0x35] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_ram->writeByte(d_regs->HL(), t); },
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return OpcodeResultContext::Builder(0x35).ShortCycle().IncrementPCDefault().Build();
	};

	// DEC C
	d_opcodes[0x0D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->C(t); },
			[this]() { return d_regs->C(); });
		return OpcodeResultContext::Builder(0x0D).ShortCycle().IncrementPCDefault().Build();
	};

	// DEC E
	d_opcodes[0x1D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->E(t); },
			[this]() { return d_regs->E(); });
		return OpcodeResultContext::Builder(0x1D).ShortCycle().IncrementPCDefault().Build();
	};

	// DEC L
	d_opcodes[0x2D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->L(t); },
			[this]() { return d_regs->L(); });
		return OpcodeResultContext::Builder(0x2D).ShortCycle().IncrementPCDefault().Build();
	};

	// DEC A
	d_opcodes[0x3D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); });
		return OpcodeResultContext::Builder(0x3D).ShortCycle().IncrementPCDefault().Build();
	};

	// 16-bit decrements
	// DEC BC
	d_opcodes[0x0B] = [this]() { 
		d_regs->BC(d_regs->BC() + 1); 
		return OpcodeResultContext::Builder(0x0B).ShortCycle().IncrementPCDefault().Build();
	};
	// DEC DE
	d_opcodes[0x1B] = [this]() { 
		d_regs->DE(d_regs->DE() + 1); 
		return OpcodeResultContext::Builder(0x1B).ShortCycle().IncrementPCDefault().Build();
	};
	// DEC HL
	d_opcodes[0x2B] = [this]() { 
		d_regs->HL(d_regs->HL() + 1); 
		return OpcodeResultContext::Builder(0x2B).ShortCycle().IncrementPCDefault().Build();
	};
	// DEC SP
	d_opcodes[0x3B] = [this]() { 
		d_regs->SP(d_regs->SP() + 1); 
		return OpcodeResultContext::Builder(0x3B).ShortCycle().IncrementPCDefault().Build();
	};


	// XOR

	// XOR B
	d_opcodes[0xA8] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->B(); });
		return OpcodeResultContext::Builder(0xA8).ShortCycle().IncrementPCDefault().Build();
	};

	// XOR C
	d_opcodes[0xA9] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->C(); });
		return OpcodeResultContext::Builder(0xA9).ShortCycle().IncrementPCDefault().Build();
	};

	// XOR D
	d_opcodes[0xAA] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->D(); });
		return OpcodeResultContext::Builder(0xAA).ShortCycle().IncrementPCDefault().Build();
	};

	// XOR E
	d_opcodes[0xAB] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->E(); });
		return OpcodeResultContext::Builder(0xAB).ShortCycle().IncrementPCDefault().Build();
	};

	// XOR H
	d_opcodes[0xAC] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->H(); });
		return OpcodeResultContext::Builder(0xAC).ShortCycle().IncrementPCDefault().Build();
	};

	// XOR L
	d_opcodes[0xAD] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->L(); });
		return OpcodeResultContext::Builder(0xAD).ShortCycle().IncrementPCDefault().Build();
	};

	// XOR (HL)
	d_opcodes[0xAE] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return OpcodeResultContext::Builder(0xAE).ShortCycle().IncrementPCDefault().Build();
	};

	// XOR A
	d_opcodes[0xAF] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->A(); });
		return OpcodeResultContext::Builder(0xAF).ShortCycle().IncrementPCDefault().Build();
	};

	// XOR d8
	d_opcodes[0xEE] = [this]()
	{
		ByteType val = readNextByte();
		xorRegister(*d_regs,
			[this,val]() { return val; }
		);
		return OpcodeResultContext::Builder(0xEE).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (16b), A

	// LD (BC), A
	d_opcodes[0x02] = [this]()
	{
		d_ram->writeByte(d_regs->BC(), d_regs->A());
		return OpcodeResultContext::Builder(0x02).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (DE), A
	d_opcodes[0x12] = [this]()
	{
		d_ram->writeByte(d_regs->DE(), d_regs->A());
		return OpcodeResultContext::Builder(0x12).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL+), A
	d_opcodes[0x22] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->A());
		d_regs->HL( d_regs->HL() + 1 );
		return OpcodeResultContext::Builder(0x22).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL-), A
	d_opcodes[0x32] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->A());
		d_regs->HL( d_regs->HL() - 1 );
		return OpcodeResultContext::Builder(0x32).ShortCycle().IncrementPCDefault().Build();
	};


	// LD REG, d8 instructions

	// LD B, d8
	d_opcodes[0x06] = [this]()
	{
		d_regs->B(readNextByte());
		return OpcodeResultContext::Builder(0x06).ShortCycle().IncrementPCDefault().Build();
	};

	// LD D, d8
	d_opcodes[0x16] = [this]()
	{
		d_regs->D(readNextByte());
		return OpcodeResultContext::Builder(0x16).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H, d8
	d_opcodes[0x26] = [this]()
	{
		d_regs->H(readNextByte());
		return OpcodeResultContext::Builder(0x26).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL), d8
	d_opcodes[0x36] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), readNextByte());
		return OpcodeResultContext::Builder(0x36).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, d8
	d_opcodes[0x0E] = [this]()
	{
		d_regs->C(readNextByte());
		return OpcodeResultContext::Builder(0x0E).ShortCycle().IncrementPCDefault().Build();
	};

	// LD E, d8
	d_opcodes[0x1E] = [this]()
	{
		d_regs->E(readNextByte());
		return OpcodeResultContext::Builder(0x1E).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, d8
	d_opcodes[0x2E] = [this]()
	{
		d_regs->L(readNextByte());
		return OpcodeResultContext::Builder(0x2E).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, d8
	d_opcodes[0x3E] = [this]()
	{
		d_regs->A(readNextByte());
		return OpcodeResultContext::Builder(0x3E).ShortCycle().IncrementPCDefault().Build();
	};

	// IO Port Read/Write Instructions

	// LD ($FF00+C), A
	d_opcodes[0xE2] = [this]()
	{
		d_ram->writeByte(0xFF00 + d_regs->C(), d_regs->A());
		return OpcodeResultContext::Builder(0xE2).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, ($FF00+C)
	d_opcodes[0xF2] = [this]()
	{
		d_regs->A(d_ram->readByte(0xFF00 + d_regs->C()));
		return OpcodeResultContext::Builder(0xF2).ShortCycle().IncrementPCDefault().Build();
	};

	// HALT
	// TODO: This should wait until the next interrupt before starting again
	d_opcodes[0x76] = [this]()
	{
		// TODO: Decide whether to use exception or return status
		throw std::runtime_error("JOB IS DONE");
		return OpcodeResultContext::Builder(0x76).ShortCycle().IncrementPCDefault().Build();
	};

	// STOP 0, cousin of HALT
	d_opcodes[0x10] = [this]()
	{
		return OpcodeResultContext::Builder(0x10).ShortCycle().IncrementPCDefault().Build();
	};

	// CPL
	d_opcodes[0x2F] = [this]()
	{
		d_regs->A( ~d_regs->A() );
		d_regs->flagSubtract(true);
		d_regs->flagHalfCarry(true);
		return OpcodeResultContext::Builder(0x2F).ShortCycle().IncrementPCDefault().Build();
	};

	// CCF
	d_opcodes[0x3F] = [this]()
	{
		d_regs->flagCarry( !(d_regs->flagCarry()) );
		d_regs->flagSubtract(false);
		d_regs->flagHalfCarry(false);
		return OpcodeResultContext::Builder(0x3F).ShortCycle().IncrementPCDefault().Build();
	};

	// SCF
	d_opcodes[0x37] = [this]()
	{
		d_regs->flagCarry(true);
		d_regs->flagSubtract(false);
		d_regs->flagHalfCarry(false);
		return OpcodeResultContext::Builder(0x37).ShortCycle().IncrementPCDefault().Build();
	};

	// LD B,B
	d_opcodes[0x40] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->B(); }
		);
		return OpcodeResultContext::Builder(0x40).ShortCycle().IncrementPCDefault().Build();
	};

	// LD B,C
	d_opcodes[0x41] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->C(); }
		);
		return OpcodeResultContext::Builder(0x41).ShortCycle().IncrementPCDefault().Build();
	};

	// LD B,D
	d_opcodes[0x42] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->D(); }
		);
		return OpcodeResultContext::Builder(0x42).ShortCycle().IncrementPCDefault().Build();
	};

	// LD B,E
	d_opcodes[0x43] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->E(); }
		);
		return OpcodeResultContext::Builder(0x43).ShortCycle().IncrementPCDefault().Build();
	};

	// LD B,H
	d_opcodes[0x44] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->H(); }
		);
		return OpcodeResultContext::Builder(0x44).ShortCycle().IncrementPCDefault().Build();
	};

	// LD B,L
	d_opcodes[0x45] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->L(); }
		);
		return OpcodeResultContext::Builder(0x45).ShortCycle().IncrementPCDefault().Build();
	};

	// LD B,A
	d_opcodes[0x47] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->A(); }
		);
		return OpcodeResultContext::Builder(0x47).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, B
	d_opcodes[0x48] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->B(); }
		);
		return OpcodeResultContext::Builder(0x48).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, C
	d_opcodes[0x49] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->C(); }
		);
		return OpcodeResultContext::Builder(0x49).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, D
	d_opcodes[0x4A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->D(); }
		);
		return OpcodeResultContext::Builder(0x4A).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, E
	d_opcodes[0x4B] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->E(); }
		);
		return OpcodeResultContext::Builder(0x4B).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, H
	d_opcodes[0x4C] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->H(); }
		);
		return OpcodeResultContext::Builder(0x4C).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, L
	d_opcodes[0x4D] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->L(); }
		);
		return OpcodeResultContext::Builder(0x4D).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, A
	d_opcodes[0x4F] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->A(); }
		);
		return OpcodeResultContext::Builder(0x4F).ShortCycle().IncrementPCDefault().Build();
	};


	// LD D,B
	d_opcodes[0x50] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->B(); }
		);
		return OpcodeResultContext::Builder(0x50).ShortCycle().IncrementPCDefault().Build();
	};

	// LD D,C
	d_opcodes[0x51] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->C(); }
		);
		return OpcodeResultContext::Builder(0x51).ShortCycle().IncrementPCDefault().Build();
	};

	// LD D,D
	d_opcodes[0x52] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->D(); }
		);
		return OpcodeResultContext::Builder(0x52).ShortCycle().IncrementPCDefault().Build();
	};

	// LD D,E
	d_opcodes[0x53] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->E(); }
		);
		return OpcodeResultContext::Builder(0x53).ShortCycle().IncrementPCDefault().Build();
	};

	// LD D,H
	d_opcodes[0x54] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->H(); }
		);
		return OpcodeResultContext::Builder(0x54).ShortCycle().IncrementPCDefault().Build();
	};

	// LD D,L
	d_opcodes[0x55] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->L(); }
		);
		return OpcodeResultContext::Builder(0x55).ShortCycle().IncrementPCDefault().Build();
	};

	// LD D,A
	d_opcodes[0x57] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->A(); }
		);
		return OpcodeResultContext::Builder(0x57).ShortCycle().IncrementPCDefault().Build();
	};

	
	// LD E, B
	d_opcodes[0x58] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->B(); }
		);
		return OpcodeResultContext::Builder(0x58).ShortCycle().IncrementPCDefault().Build();
	};

	// LD E, C
	d_opcodes[0x59] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->C(); }
		);
		return OpcodeResultContext::Builder(0x59).ShortCycle().IncrementPCDefault().Build();
	};

	// LD E, D
	d_opcodes[0x5A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->D(); }
		);
		return OpcodeResultContext::Builder(0x5A).ShortCycle().IncrementPCDefault().Build();
	};

	// LD E, E
	d_opcodes[0x5B] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->E(); }
		);
		return OpcodeResultContext::Builder(0x5B).ShortCycle().IncrementPCDefault().Build();
	};

	// LD E, H
	d_opcodes[0x5C] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->H(); }
		);
		return OpcodeResultContext::Builder(0x5C).ShortCycle().IncrementPCDefault().Build();
	};

	// LD E, L
	d_opcodes[0x5D] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->L(); }
		);
		return OpcodeResultContext::Builder(0x5D).ShortCycle().IncrementPCDefault().Build();
	};

	// LD E, A
	d_opcodes[0x5F] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->A(); }
		);
		return OpcodeResultContext::Builder(0x5F).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H,B
	d_opcodes[0x60] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->B(); }
		);
		return OpcodeResultContext::Builder(0x60).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H,C
	d_opcodes[0x61] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->C(); }
		);
		return OpcodeResultContext::Builder(0x61).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H,D
	d_opcodes[0x62] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->D(); }
		);
		return OpcodeResultContext::Builder(0x62).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H,E
	d_opcodes[0x63] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->E(); }
		);
		return OpcodeResultContext::Builder(0x63).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H,H
	d_opcodes[0x64] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->H(); }
		);
		return OpcodeResultContext::Builder(0x64).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H,L
	d_opcodes[0x65] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->L(); }
		);
		return OpcodeResultContext::Builder(0x65).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H,A
	d_opcodes[0x67] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->A(); }
		);
		return OpcodeResultContext::Builder(0x67).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, B
	d_opcodes[0x68] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->B(); }
		);
		return OpcodeResultContext::Builder(0x68).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, C
	d_opcodes[0x69] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->C(); }
		);
		return OpcodeResultContext::Builder(0x69).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, D
	d_opcodes[0x6A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->D(); }
		);
		return OpcodeResultContext::Builder(0x6A).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, E
	d_opcodes[0x6B] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->E(); }
		);
		return OpcodeResultContext::Builder(0x6B).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, H
	d_opcodes[0x6C] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->H(); }
		);
		return OpcodeResultContext::Builder(0x6C).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, L
	d_opcodes[0x6D] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->L(); }
		);
		return OpcodeResultContext::Builder(0x6D).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, A
	d_opcodes[0x6F] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->A(); }
		);
		return OpcodeResultContext::Builder(0x6F).ShortCycle().IncrementPCDefault().Build();
	};

	
	// LD A, B
	d_opcodes[0x78] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->B(); }
		);
		return OpcodeResultContext::Builder(0x78).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, C
	d_opcodes[0x79] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->C(); }
		);
		return OpcodeResultContext::Builder(0x79).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, D
	d_opcodes[0x7A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->D(); }
		);
		return OpcodeResultContext::Builder(0x7A).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, E
	d_opcodes[0x7B] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->E(); }
		);
		return OpcodeResultContext::Builder(0x7B).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, H
	d_opcodes[0x7C] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->H(); }
		);
		return OpcodeResultContext::Builder(0x7C).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, L
	d_opcodes[0x7D] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->L(); }
		);
		return OpcodeResultContext::Builder(0x7D).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, A
	d_opcodes[0x7F] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->A(); }
		);
		return OpcodeResultContext::Builder(0x7F).ShortCycle().IncrementPCDefault().Build();
	};

	// LD B, (HL)
	d_opcodes[0x46] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_ram->readByte( d_regs->HL() ); }
		);
		return OpcodeResultContext::Builder(0x46).ShortCycle().IncrementPCDefault().Build();
	};

	// LD C, (HL)
	d_opcodes[0x4E] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return OpcodeResultContext::Builder(0x4E).ShortCycle().IncrementPCDefault().Build();
	};

	// LD D, (HL)
	d_opcodes[0x56] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return OpcodeResultContext::Builder(0x56).ShortCycle().IncrementPCDefault().Build();
	};

	// LD E, (HL)
	d_opcodes[0x5E] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return OpcodeResultContext::Builder(0x5E).ShortCycle().IncrementPCDefault().Build();
	};

	// LD H, (HL)
	d_opcodes[0x66] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return OpcodeResultContext::Builder(0x66).ShortCycle().IncrementPCDefault().Build();
	};

	// LD L, (HL)
	d_opcodes[0x6E] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return OpcodeResultContext::Builder(0x6E).ShortCycle().IncrementPCDefault().Build();
	};

	// LD A, (HL)
	d_opcodes[0x7E] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return OpcodeResultContext::Builder(0x7E).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL), A
	d_opcodes[0x77] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->A());
		return OpcodeResultContext::Builder(0x77).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL), B
	d_opcodes[0x70] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->B());
		return OpcodeResultContext::Builder(0x70).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL), C
	d_opcodes[0x71] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->C());
		return OpcodeResultContext::Builder(0x71).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL), D
	d_opcodes[0x72] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->D());
		return OpcodeResultContext::Builder(0x72).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL), E
	d_opcodes[0x73] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->E());
		return OpcodeResultContext::Builder(0x73).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL), H
	d_opcodes[0x74] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->H());
		return OpcodeResultContext::Builder(0x74).ShortCycle().IncrementPCDefault().Build();
	};

	// LD (HL), L
	d_opcodes[0x75] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->L());
		return OpcodeResultContext::Builder(0x75).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD REG, REG SECTION

	// ADD A, B
	d_opcodes[0x80] = [this]()
	{
		performByteAdd( 
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->B(); }
		);

		return OpcodeResultContext::Builder(0x80).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD A, C
	d_opcodes[0x81] = [this]()
	{
		performByteAdd( 
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->C(); }
		);

		return OpcodeResultContext::Builder(0x81).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD A, D
	d_opcodes[0x82] = [this]()
	{
		performByteAdd( 
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->D(); }
		);

		return OpcodeResultContext::Builder(0x82).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD A, E
	d_opcodes[0x83] = [this]()
	{
		performByteAdd( 
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->E(); }
		);

		return OpcodeResultContext::Builder(0x83).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD A, H
	d_opcodes[0x84] = [this]()
	{
		performByteAdd( 
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->H(); }
		);

		return OpcodeResultContext::Builder(0x84).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD A, L
	d_opcodes[0x85] = [this]()
	{
		performByteAdd( 
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->L(); }
		);

		return OpcodeResultContext::Builder(0x85).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD A, (HL)
	d_opcodes[0x86] = [this]()
	{
		performByteAdd( 
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);

		return OpcodeResultContext::Builder(0x86).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD A, A
	d_opcodes[0x87] = [this]()
	{
		performByteAdd( 
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->A(); }
		);

		return OpcodeResultContext::Builder(0x87).ShortCycle().IncrementPCDefault().Build();
	};

	// ADD A, d8
	d_opcodes[0xC6] = [this]()
	{
		ByteType constVal = readNextByte();
		performByteAdd(
			*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this, constVal]() { return constVal; }
		);

		return OpcodeResultContext::Builder(0xC6).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,B
	d_opcodes[0x88] = [this]()
	{
		//// Do regular A + B first
		//performByteAdd(*d_regs,
		//	[this](ByteType t) { d_regs->A(t); },
		//	[this]() { return d_regs->A(); },
		//	[this]() { return d_regs->B(); }
		//);

		//// I guess we're technically adding from right to left, c + B + A
		//performByteAdd(*d_regs,
		//	[this](ByteType t) { d_regs->A(t); },
		//	[this]() { return d_regs->A(); },
		//	[this]() { return d_regs->flagCarry() ? 1 : 0; });

		performADC(*d_regs, [this]() { return d_regs->B(); });
		return OpcodeResultContext::Builder(0x88).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,C
	d_opcodes[0x89] = [this]()
	{
		performADC(*d_regs, [this]() { return d_regs->C(); });

		return OpcodeResultContext::Builder(0x89).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,D
	d_opcodes[0x8A] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->D(); });
		return OpcodeResultContext::Builder(0x8A).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,E
	d_opcodes[0x8B] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->E(); });
		return OpcodeResultContext::Builder(0x8B).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,H
	d_opcodes[0x8C] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->H(); });
		return OpcodeResultContext::Builder(0x8C).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,L
	d_opcodes[0x8D] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->L(); });
		return OpcodeResultContext::Builder(0x8D).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,(HL)
	d_opcodes[0x8E] = [this]()
	{
		performADC(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); });
		return OpcodeResultContext::Builder(0x8E).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,A
	d_opcodes[0x8F] = [this]()
	{
		performADC(*d_regs, [this]() { return d_regs->A(); });
		return OpcodeResultContext::Builder(0x8F).ShortCycle().IncrementPCDefault().Build();
	};

	// ADC A,d8
	d_opcodes[0xCE] = [this]()
	{
		ByteType numLiteral = readNextByte();
		performADC(*d_regs, [this, numLiteral]() { return numLiteral; });
		return OpcodeResultContext::Builder(0xCE).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB B
	d_opcodes[0x90] = [this]()
	{
		performByteSub(*d_regs,	[this](ByteType t) { return d_regs->A(t); },
								[this]() { return d_regs->A(); },
								[this]() { return d_regs->B(); }
			);
		return OpcodeResultContext::Builder(0x90).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB C
	d_opcodes[0x91] = [this]()
	{
		performByteSub(*d_regs, [this](ByteType t) { return d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->C(); }
		);
		return OpcodeResultContext::Builder(0x91).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB D
	d_opcodes[0x92] = [this]()
	{
		performByteSub(*d_regs, [this](ByteType t) { return d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->D(); }
		);
		return OpcodeResultContext::Builder(0x92).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB E
	d_opcodes[0x93] = [this]()
	{
		performByteSub(*d_regs, [this](ByteType t) { return d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->E(); }
		);
		return OpcodeResultContext::Builder(0x93).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB H
	d_opcodes[0x94] = [this]()
	{
		performByteSub(*d_regs, [this](ByteType t) { return d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->H(); }
		);
		return OpcodeResultContext::Builder(0x94).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB L
	d_opcodes[0x95] = [this]()
	{
		performByteSub(*d_regs, [this](ByteType t) { return d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->L(); }
		);
		return OpcodeResultContext::Builder(0x95).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB (HL)
	d_opcodes[0x96] = [this]()
	{
		WordType a = d_regs->HL();
		ByteType read = d_ram->readByte(a);
		performByteSub(*d_regs, [this](ByteType t) { return d_regs->A(t); },
								[this]() { return d_regs->A(); },
								[this]() { return d_ram->readByte( d_regs->HL() ); }
		);
		return OpcodeResultContext::Builder(0x96).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB A
	d_opcodes[0x97] = [this]()
	{
		performByteSub(*d_regs, [this](ByteType t) { return d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->A(); }
		);
		return OpcodeResultContext::Builder(0x97).ShortCycle().IncrementPCDefault().Build();
	};

	// SUB (d8)
	d_opcodes[0xD6] = [this]()
	{
		ByteType numLiteral = readNextByte();

		performByteSub(*d_regs, [this](ByteType t) { return d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this, numLiteral]() { return numLiteral; }
		);
		return OpcodeResultContext::Builder(0xD6).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,B
	d_opcodes[0x98] = [this]()
	{
		performSBC(*d_regs, [this]() { return d_regs->B(); });

		return OpcodeResultContext::Builder(0x98).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,C
	d_opcodes[0x99] = [this]()
	{
		performSBC(*d_regs, [this]() { return d_regs->C(); });

		return OpcodeResultContext::Builder(0x99).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,D
	d_opcodes[0x9A] = [this]()
	{
		performSBC(*d_regs, [this]() { return d_regs->D(); });

		return OpcodeResultContext::Builder(0x9A).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,E
	d_opcodes[0x9B] = [this]()
	{
		performSBC(*d_regs, [this]() { return d_regs->E(); });

		return OpcodeResultContext::Builder(0x9B).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,H
	d_opcodes[0x9C] = [this]()
	{
		performSBC(*d_regs, [this]() { return d_regs->H(); });

		return OpcodeResultContext::Builder(0x9C).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,L
	d_opcodes[0x9D] = [this]()
	{
		performSBC(*d_regs, [this]() { return d_regs->L(); });

		return OpcodeResultContext::Builder(0x9D).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,(HL)
	d_opcodes[0x9E] = [this]()
	{
		performSBC(*d_regs, [this]() { return d_ram->readByte( d_regs->HL() ); });

		return OpcodeResultContext::Builder(0x9E).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,A
	d_opcodes[0x9F] = [this]()
	{
		performSBC(*d_regs, [this]() { return d_regs->A(); });

		return OpcodeResultContext::Builder(0x9F).ShortCycle().IncrementPCDefault().Build();
	};

	// SBC A,d8
	d_opcodes[0xDE] = [this]()
	{
		ByteType numLiteral = readNextByte();
		performSBC(*d_regs, [this, numLiteral]() { return numLiteral; });

		return OpcodeResultContext::Builder(0xDE).ShortCycle().IncrementPCDefault().Build();
	};

	// AND B
	d_opcodes[0xA0] = [this]()
	{
		performAND(*d_regs, [this]() { return d_regs->B(); });

		return OpcodeResultContext::Builder(0xA0).ShortCycle().IncrementPCDefault().Build();
	};

	// AND C
	d_opcodes[0xA1] = [this]()
	{
		performAND(*d_regs, [this]() { return d_regs->C(); });

		return OpcodeResultContext::Builder(0xA1).ShortCycle().IncrementPCDefault().Build();
	};

	// AND D
	d_opcodes[0xA2] = [this]()
	{
		performAND(*d_regs, [this]() { return d_regs->D(); });

		return OpcodeResultContext::Builder(0xA2).ShortCycle().IncrementPCDefault().Build();
	};

	// AND E
	d_opcodes[0xA3] = [this]()
	{
		performAND(*d_regs, [this]() { return d_regs->E(); });

		return OpcodeResultContext::Builder(0xA3).ShortCycle().IncrementPCDefault().Build();
	};

	// AND H
	d_opcodes[0xA4] = [this]()
	{
		performAND(*d_regs, [this]() { return d_regs->H(); });

		return OpcodeResultContext::Builder(0xA4).ShortCycle().IncrementPCDefault().Build();
	};

	// AND L
	d_opcodes[0xA5] = [this]()
	{
		performAND(*d_regs, [this]() { return d_regs->L(); });

		return OpcodeResultContext::Builder(0xA5).ShortCycle().IncrementPCDefault().Build();
	};

	// AND (HL)
	d_opcodes[0xA6] = [this]()
	{
		performAND(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); });

		return OpcodeResultContext::Builder(0xA6).ShortCycle().IncrementPCDefault().Build();
	};

	// AND A
	d_opcodes[0xA7] = [this]()
	{
		performAND(*d_regs, [this]() { return d_regs->A(); });

		return OpcodeResultContext::Builder(0xA7).ShortCycle().IncrementPCDefault().Build();
	};

	// AND d8
	d_opcodes[0xE6] = [this]()
	{
		ByteType numLiteral = readNextByte();
		performAND(*d_regs, [this, numLiteral]() { return numLiteral; });

		return OpcodeResultContext::Builder(0xE6).ShortCycle().IncrementPCDefault().Build();
	};

	// OR B
	d_opcodes[0xB0] = [this]()
	{
		performOR(*d_regs, [this]() {return d_regs->B(); });
		return OpcodeResultContext::Builder(0xB0).ShortCycle().IncrementPCDefault().Build();
	};

	// OR C
	d_opcodes[0xB1] = [this]()
	{
		performOR(*d_regs, [this]() {return d_regs->C(); });
		return OpcodeResultContext::Builder(0xB1).ShortCycle().IncrementPCDefault().Build();
	};

	// OR D
	d_opcodes[0xB2] = [this]()
	{
		performOR(*d_regs, [this]() {return d_regs->D(); });
		return OpcodeResultContext::Builder(0xB2).ShortCycle().IncrementPCDefault().Build();
	};

	// OR E
	d_opcodes[0xB3] = [this]()
	{
		performOR(*d_regs, [this]() {return d_regs->E(); });
		return OpcodeResultContext::Builder(0xB3).ShortCycle().IncrementPCDefault().Build();
	};

	// OR H
	d_opcodes[0xB4] = [this]()
	{
		performOR(*d_regs, [this]() {return d_regs->H(); });
		return OpcodeResultContext::Builder(0xB4).ShortCycle().IncrementPCDefault().Build();
	};

	// OR L
	d_opcodes[0xB5] = [this]()
	{
		performOR(*d_regs, [this]() {return d_regs->L(); });
		return OpcodeResultContext::Builder(0xB5).ShortCycle().IncrementPCDefault().Build();
	};

	// OR (HL)
	d_opcodes[0xB6] = [this]()
	{
		performOR(*d_regs, [this]() {return d_ram->readByte(d_regs->HL()); });
		return OpcodeResultContext::Builder(0xB6).ShortCycle().IncrementPCDefault().Build();
	};

	// OR A
	d_opcodes[0xB7] = [this]()
	{
		performOR(*d_regs, [this]() {return d_regs->A(); });
		return OpcodeResultContext::Builder(0xB7).ShortCycle().IncrementPCDefault().Build();
	};

	// OR d8
	d_opcodes[0xF6] = [this]()
	{
		ByteType numLiteral = readNextByte();
		performOR(*d_regs, [this, numLiteral]() {return numLiteral; });
		return OpcodeResultContext::Builder(0xF6).ShortCycle().IncrementPCDefault().Build();
	};


	// TODO: Maybe these are worth revisiting? It'd be interesting to know performance losses doing it this method.
	// CP B
	d_opcodes[0xB8] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0x90]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xB8).ShortCycle().IncrementPCDefault().Build();
	};

	// CP C
	d_opcodes[0xB9] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0x91]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xB9).ShortCycle().IncrementPCDefault().Build();
	};

	// CP D
	d_opcodes[0xBA] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0x92]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xBA).ShortCycle().IncrementPCDefault().Build();
	};

	// CP E
	d_opcodes[0xBB] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0x93]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xBB).ShortCycle().IncrementPCDefault().Build();
	};

	// CP H
	d_opcodes[0xBC] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0x94]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xBC).ShortCycle().IncrementPCDefault().Build();
	};

	// CP L
	d_opcodes[0xBD] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0x95]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xBD).ShortCycle().IncrementPCDefault().Build();
	};

	// CP (HL)
	d_opcodes[0xBE] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0x96]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xBE).ShortCycle().IncrementPCDefault().Build();
	};

	// CP A
	d_opcodes[0xBF] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0x97]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xBF).ShortCycle().IncrementPCDefault().Build();
	};

	// CP d8
	d_opcodes[0xFE] = [this]()
	{
		// Save current value of A. Do subtraction. Restore A.
		ByteType previousRegVal = d_regs->A();
		d_opcodes[0xD6]();
		d_regs->A(previousRegVal);
		return OpcodeResultContext::Builder(0xFE).ShortCycle().IncrementPCDefault().Build();
	};

	// Stack Pointer Operations

	// POP BC
	d_opcodes[0xC1] = [this]()
	{
		popSP(*d_regs, *d_ram, [this](WordType t) { d_regs->BC(t); });
		return OpcodeResultContext::Builder(0xC1).ShortCycle().IncrementPCDefault().Build();
	};

	// POP DE
	d_opcodes[0xD1] = [this]()
	{
		popSP(*d_regs, *d_ram, [this](WordType t) { d_regs->DE(t); });
		return OpcodeResultContext::Builder(0xD1).ShortCycle().IncrementPCDefault().Build();
	};

	// POP HL
	d_opcodes[0xE1] = [this]()
	{
		popSP(*d_regs, *d_ram, [this](WordType t) { d_regs->HL(t); });
		return OpcodeResultContext::Builder(0xE1).ShortCycle().IncrementPCDefault().Build();
	};

	// POP AF
	d_opcodes[0xF1] = [this]()
	{
		popSP(*d_regs, *d_ram, [this](WordType t) { d_regs->AF(t); });
		return OpcodeResultContext::Builder(0xF1).ShortCycle().IncrementPCDefault().Build();
	};

	// PUSH BC
	d_opcodes[0xC5] = [this]()
	{
		pushSP(*d_regs, *d_ram, [this]() { return d_regs->BC(); });
		return OpcodeResultContext::Builder(0xC5).ShortCycle().IncrementPCDefault().Build();
	};

	// PUSH DE
	d_opcodes[0xD5] = [this]()
	{
		pushSP(*d_regs, *d_ram, [this]() { return d_regs->DE(); });
		return OpcodeResultContext::Builder(0xD5).ShortCycle().IncrementPCDefault().Build();
	};

	// PUSH HL
	d_opcodes[0xE5] = [this]()
	{
		pushSP(*d_regs, *d_ram, [this]() { return d_regs->HL(); });
		return OpcodeResultContext::Builder(0xE5).ShortCycle().IncrementPCDefault().Build();
	};

	// PUSH AF
	d_opcodes[0xF5] = [this]()
	{
		pushSP(*d_regs, *d_ram, [this]() { return d_regs->AF(); });
		return OpcodeResultContext::Builder(0xF5).ShortCycle().IncrementPCDefault().Build();
	};

	// RET Section

	// RET NZ
	d_opcodes[0xC0] = [this]()
	{
		if (!d_regs->flagZero())
		{
			// Will call actual RET opcode
			d_opcodes[0xC9]();
			
			return OpcodeResultContext::Builder(0xC0).LongCycle().FreezePC().Build();
		}
		return OpcodeResultContext::Builder(0xC0).ShortCycle().IncrementPCDefault().Build();
	};

	// RET NC
	d_opcodes[0xD0] = [this]()
	{
		if (!d_regs->flagCarry())
		{
			d_opcodes[0xC9]();
			return OpcodeResultContext::Builder(0xD0).LongCycle().FreezePC().Build();
		}
		return OpcodeResultContext::Builder(0xD0).ShortCycle().IncrementPCDefault().Build();
	};

	// RET Z
	d_opcodes[0xC8] = [this]()
	{
		if (d_regs->flagZero())
		{
			d_opcodes[0xC9]();
			return OpcodeResultContext::Builder(0xC8).LongCycle().FreezePC().Build();
		}
		return OpcodeResultContext::Builder(0xC8).ShortCycle().IncrementPCDefault().Build();
	};

	// RET C
	d_opcodes[0xD8] = [this]()
	{
		if (d_regs->flagCarry())
		{
			d_opcodes[0xC9]();
			return OpcodeResultContext::Builder(0xD8).LongCycle().FreezePC().Build();
		}
		return OpcodeResultContext::Builder(0xD8).ShortCycle().IncrementPCDefault().Build();
	};

	// RET
	d_opcodes[0xC9] = [this]()
	{
		// POP SP onto our PC
		popSP(*d_regs, *d_ram, [this](WordType t) { d_regs->PC(t); });

		return OpcodeResultContext::Builder(0xC9).ShortCycle().FreezePC().Build();
	}; 

	// RETI
	// TODO: Actually re-enable interrupts after return
	d_opcodes[0xD9] = [this]()
	{
		// Call RET opcode and then enable interrupts afterwards
		OpcodeResultContext retContext = d_opcodes[0xC9]();

		// TODO: Enable interrupts
		// Currently unimplemented and this will throw
		d_opcodes[0xFB]();

		return OpcodeResultContext::Builder(0xD9).ShortCycle().FreezePC().Build();
	};

	// JP Section
	// JP NZ,a16
	d_opcodes[0xC2] = [this]()
	{
		WordType jpAddr = readNextTwoBytes();
		if (!d_regs->flagZero())
		{
			// Set PC to address read in
			return OpcodeResultContext::Builder(0xC2).LongCycle().SetPCTo(jpAddr).Build();
		}
		return OpcodeResultContext::Builder(0xC2).ShortCycle().IncrementPCDefault().Build();
	};

	// JP NC,a16
	d_opcodes[0xD2] = [this]()
	{
		WordType jpAddr = readNextTwoBytes();
		if (!d_regs->flagCarry())
		{
			// Set PC to address read in
			return OpcodeResultContext::Builder(0xD2).LongCycle().SetPCTo(jpAddr).Build();
		}
		return OpcodeResultContext::Builder(0xD2).ShortCycle().IncrementPCDefault().Build();
	};

	// JP a16
	d_opcodes[0xC3] = [this]()
	{
		// Set PC to address read in
		WordType jpAddr = readNextTwoBytes();
		return OpcodeResultContext::Builder(0xC3).LongCycle().SetPCTo(jpAddr).Build();
	};

	// JP Z,a16
	d_opcodes[0xCA] = [this]()
	{
		WordType jpAddr = readNextTwoBytes();
		if (d_regs->flagZero())
		{
			// Set PC to address read in
			return OpcodeResultContext::Builder(0xCA).LongCycle().SetPCTo(jpAddr).Build();
		}
		return OpcodeResultContext::Builder(0xCA).ShortCycle().IncrementPCDefault().Build();
	};

	// JP C,a16
	d_opcodes[0xDA] = [this]()
	{
		WordType jpAddr = readNextTwoBytes();
		if (d_regs->flagCarry())
		{
			// Set PC to address read in
			return OpcodeResultContext::Builder(0xDA).LongCycle().SetPCTo(jpAddr).Build();
		}
		return OpcodeResultContext::Builder(0xDA).ShortCycle().IncrementPCDefault().Build();
	};


	// JP (HL)
	d_opcodes[0xE9] = [this]()
	{
		if (!d_regs->flagZero())
		{
			// Set PC to address read in
			WordType jpAddr = d_ram->readWord(d_regs->HL());
			return OpcodeResultContext::Builder(0xE9).LongCycle().SetPCTo(jpAddr).Build();
		}
		return OpcodeResultContext::Builder(0xE9).ShortCycle().IncrementPCDefault().Build();
	};


	// RST Section
	// (SP-1) <- PCh, (SP-2) <- PCl, PCh <- 0, PCl <- f, SP <- SP-2

	// RST 00H
	d_opcodes[0xC7] = [this]()
	{
		registerRst(*d_regs, *d_ram, 0x00);
		return OpcodeResultContext::Builder(0xC7).ShortCycle().FreezePC().Build();
	};

	// RST 10H
	d_opcodes[0xD7] = [this]()
	{
		registerRst(*d_regs, *d_ram, 0x10);
		return OpcodeResultContext::Builder(0xD7).ShortCycle().FreezePC().Build();
	};

	// RST 20H
	d_opcodes[0xE7] = [this]()
	{
		registerRst(*d_regs, *d_ram, 0x20);
		return OpcodeResultContext::Builder(0xE7).ShortCycle().FreezePC().Build();
	};

	// RST 30H
	d_opcodes[0xF7] = [this]()
	{
		registerRst(*d_regs, *d_ram, 0x30);
		return OpcodeResultContext::Builder(0xF7).ShortCycle().FreezePC().Build();
	};

	// RST 08H
	d_opcodes[0xCF] = [this]()
	{
		registerRst(*d_regs, *d_ram, 0x08);
		return OpcodeResultContext::Builder(0xCF).ShortCycle().FreezePC().Build();
	};

	// RST 18H
	d_opcodes[0xDF] = [this]()
	{
		registerRst(*d_regs, *d_ram, 0x18);
		return OpcodeResultContext::Builder(0xDF).ShortCycle().FreezePC().Build();		
	};

	// RST 28H
	d_opcodes[0xEF] = [this]()
	{
		registerRst(*d_regs, *d_ram, 0x28);
		return OpcodeResultContext::Builder(0xEF).ShortCycle().FreezePC().Build();		
	};

	// RST 38H
	d_opcodes[0xFF] = [this]()
	{
		registerRst(*d_regs, *d_ram, 0x38);
		return OpcodeResultContext::Builder(0xFF).ShortCycle().FreezePC().Build();		
	};

	// CALL Section
	
	// CALL NZ,a16
	d_opcodes[0xC4] = [this]()
	{
		WordType targetAddr = readNextTwoBytes();
		if (!d_regs->flagZero())
		{
			performCall(*d_regs, *d_ram, targetAddr);
			return OpcodeResultContext::Builder(0xC4).LongCycle().FreezePC().Build();
		}
		return OpcodeResultContext::Builder(0xC4).ShortCycle().IncrementPCDefault().Build();
	};

	// CALL NC,a16
	d_opcodes[0xD4] = [this]()
	{
		WordType targetAddr = readNextTwoBytes();
		if (!d_regs->flagCarry())
		{
			performCall(*d_regs, *d_ram, targetAddr);
			return OpcodeResultContext::Builder(0xD4).LongCycle().FreezePC().Build();
		}
		return OpcodeResultContext::Builder(0xD4).ShortCycle().IncrementPCDefault().Build();		
	};

	// CALL Z,a16
	d_opcodes[0xCC] = [this]()
	{
		WordType targetAddr = readNextTwoBytes();
		if (d_regs->flagZero())
		{
			performCall(*d_regs, *d_ram, targetAddr);
			return OpcodeResultContext::Builder(0xCC).LongCycle().FreezePC().Build();
		}
		return OpcodeResultContext::Builder(0xCC).ShortCycle().IncrementPCDefault().Build();		
	};

	// CALL C,a16
	d_opcodes[0xDC] = [this]()
	{
		WordType targetAddr = readNextTwoBytes();
		if (d_regs->flagCarry())
		{
			performCall(*d_regs, *d_ram, targetAddr);
			return OpcodeResultContext::Builder(0xDC).LongCycle().FreezePC().Build();
		}
		return OpcodeResultContext::Builder(0xDC).ShortCycle().IncrementPCDefault().Build();		
	};

	// CALL a16
	d_opcodes[0xCD] = [this]()
	{
		WordType targetAddr = readNextTwoBytes();
		performCall(*d_regs, *d_ram, targetAddr);
		return OpcodeResultContext::Builder(0xCD).LongCycle().FreezePC().Build();
	};

	// INTERRUPT Section
	
	// Disable Interrupts
	// DI
	// TODO: Return to this
	d_opcodes[0xF3] = [this]()
	{
		// TODO: Throw custom runtime exception here to differentiate between standard runtime error
		throw std::runtime_error("Opcode 0xF3 is unimplemented");
		return OpcodeResultContext::Builder(0xF3).ShortCycle().FreezePC().Build();
	};

	// EI
	d_opcodes[0xFB] = [this]()
	{
		throw std::runtime_error("Opcode 0xFB is unimplemented");
		return OpcodeResultContext::Builder(0xF3).ShortCycle().FreezePC().Build();	
	};

	// LDH Section
	
	// LDH (a8), A
	d_opcodes[0xE0] = [this]()
	{
		ByteType incByVal = readNextByte();
		WordType memTargetAddr = 0xFF00 + incByVal;

		d_ram->writeByte(memTargetAddr, d_regs->A());
		return OpcodeResultContext::Builder(0xE0).ShortCycle().IncrementPCDefault().Build();
	};

	// LDH A, (a8)
	d_opcodes[0xF0] = [this]()
	{
		ByteType incByVal = readNextByte();
		WordType memTargetAddr = 0xFF00 + incByVal;
		
		d_regs->A( d_ram->readByte(memTargetAddr) );
		return OpcodeResultContext::Builder(0xF0).ShortCycle().IncrementPCDefault().Build();
	};

	// =============== CB Opcode Section
	// Initialize CB-prefix opcodes

	// Placeholder element for the first opcode in the sequence
	d_opcodes[0xCB] = [this]()
	{
		throw std::runtime_error("CB opcode should never be directly called");
		return OpcodeResultContext::Builder(0xCB).ShortCycle().FreezePC().Build();
	};
	
	// BIT 0, B
	d_cbOpcodes[0x40] = [this]() { 
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 0); 
		return OpcodeResultContext::Builder(0x40).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x41 - BIT 0, C
	d_cbOpcodes[0x41] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 0);
		return OpcodeResultContext::Builder(0x41).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x42 - BIT 0, D
	d_cbOpcodes[0x42] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 0);
		return OpcodeResultContext::Builder(0x42).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x43 - BIT 0, E
	d_cbOpcodes[0x43] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 0);
		return OpcodeResultContext::Builder(0x43).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x44 - BIT 0, H
	d_cbOpcodes[0x44] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 0);
		return OpcodeResultContext::Builder(0x44).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x45 - BIT 0, L
	d_cbOpcodes[0x45] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 0);
		return OpcodeResultContext::Builder(0x45).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x46 - BIT 0, (HL)
	d_cbOpcodes[0x46] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 0);
		return OpcodeResultContext::Builder(0x46).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x47 - BIT 0, A
	d_cbOpcodes[0x47] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 0);
		return OpcodeResultContext::Builder(0x47).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x48 - BIT 1, B
	d_cbOpcodes[0x48] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 1);
		return OpcodeResultContext::Builder(0x48).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x49 - BIT 1, C
	d_cbOpcodes[0x49] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 1);
		return OpcodeResultContext::Builder(0x49).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4A - BIT 1, D
	d_cbOpcodes[0x4A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 1);
		return OpcodeResultContext::Builder(0x4A).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4B - BIT 1, E
	d_cbOpcodes[0x4B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 1);
		return OpcodeResultContext::Builder(0x4B).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4C - BIT 1, H
	d_cbOpcodes[0x4C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 1);
		return OpcodeResultContext::Builder(0x4C).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4D - BIT 1, L
	d_cbOpcodes[0x4D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 1);
		return OpcodeResultContext::Builder(0x4D).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4E - BIT 1, (HL)
	d_cbOpcodes[0x4E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 1);
		return OpcodeResultContext::Builder(0x4E).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4F - BIT 1, A
	d_cbOpcodes[0x4F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 1);
		return OpcodeResultContext::Builder(0x4F).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x50 - BIT 2, B
	d_cbOpcodes[0x50] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 2);
		return OpcodeResultContext::Builder(0x50).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x51 - BIT 2, C
	d_cbOpcodes[0x51] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 2);
		return OpcodeResultContext::Builder(0x51).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x52 - BIT 2, D
	d_cbOpcodes[0x52] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 2);
		return OpcodeResultContext::Builder(0x52).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x53 - BIT 2, E
	d_cbOpcodes[0x53] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 2);
		return OpcodeResultContext::Builder(0x53).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x54 - BIT 2, H
	d_cbOpcodes[0x54] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 2);
		return OpcodeResultContext::Builder(0x54).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x55 - BIT 2, L
	d_cbOpcodes[0x55] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 2);
		return OpcodeResultContext::Builder(0x55).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x56 - BIT 2, (HL)
	d_cbOpcodes[0x56] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 2);
		return OpcodeResultContext::Builder(0x56).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x57 - BIT 2, A
	d_cbOpcodes[0x57] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 2);
		return OpcodeResultContext::Builder(0x57).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x58 - BIT 3, B
	d_cbOpcodes[0x58] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 3);
		return OpcodeResultContext::Builder(0x58).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x59 - BIT 3, C
	d_cbOpcodes[0x59] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 3);
		return OpcodeResultContext::Builder(0x59).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5A - BIT 3, D
	d_cbOpcodes[0x5A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 3);
		return OpcodeResultContext::Builder(0x5A).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5B - BIT 3, E
	d_cbOpcodes[0x5B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 3);
		return OpcodeResultContext::Builder(0x5B).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5C - BIT 3, H
	d_cbOpcodes[0x5C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 3);
		return OpcodeResultContext::Builder(0x5C).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5D - BIT 3, L
	d_cbOpcodes[0x5D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 3);
		return OpcodeResultContext::Builder(0x5D).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5E - BIT 3, (HL)
	d_cbOpcodes[0x5E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 3);
		return OpcodeResultContext::Builder(0x5E).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5F - BIT 3, A
	d_cbOpcodes[0x5F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 3);
		return OpcodeResultContext::Builder(0x5F).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x60 - BIT 4, B
	d_cbOpcodes[0x60] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 4);
		return OpcodeResultContext::Builder(0x60).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x61 - BIT 4, C
	d_cbOpcodes[0x61] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 4);
		return OpcodeResultContext::Builder(0x61).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x62 - BIT 4, D
	d_cbOpcodes[0x62] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 4);
		return OpcodeResultContext::Builder(0x62).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x63 - BIT 4, E
	d_cbOpcodes[0x63] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 4);
		return OpcodeResultContext::Builder(0x63).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x64 - BIT 4, H
	d_cbOpcodes[0x64] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 4);
		return OpcodeResultContext::Builder(0x64).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x65 - BIT 4, L
	d_cbOpcodes[0x65] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 4);
		return OpcodeResultContext::Builder(0x65).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x66 - BIT 4, (HL)
	d_cbOpcodes[0x66] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 4);
		return OpcodeResultContext::Builder(0x66).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x67 - BIT 4, A
	d_cbOpcodes[0x67] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 4);
		return OpcodeResultContext::Builder(0x67).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x68 - BIT 5, B
	d_cbOpcodes[0x68] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 5);
		return OpcodeResultContext::Builder(0x68).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x69 - BIT 5, C
	d_cbOpcodes[0x69] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 5);
		return OpcodeResultContext::Builder(0x69).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6A - BIT 5, D
	d_cbOpcodes[0x6A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 5);
		return OpcodeResultContext::Builder(0x6A).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6B - BIT 5, E
	d_cbOpcodes[0x6B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 5);
		return OpcodeResultContext::Builder(0x6B).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6C - BIT 5, H
	d_cbOpcodes[0x6C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 5);
		return OpcodeResultContext::Builder(0x6C).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6D - BIT 5, L
	d_cbOpcodes[0x6D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 5);
		return OpcodeResultContext::Builder(0x6D).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6E - BIT 5, (HL)
	d_cbOpcodes[0x6E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 5);
		return OpcodeResultContext::Builder(0x6E).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6F - BIT 5, A
	d_cbOpcodes[0x6F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 5);
		return OpcodeResultContext::Builder(0x6F).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x70 - BIT 6, B
	d_cbOpcodes[0x70] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 6);
		return OpcodeResultContext::Builder(0x70).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x71 - BIT 6, C
	d_cbOpcodes[0x71] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 6);
		return OpcodeResultContext::Builder(0x71).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x72 - BIT 6, D
	d_cbOpcodes[0x72] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 6);
		return OpcodeResultContext::Builder(0x72).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x73 - BIT 6, E
	d_cbOpcodes[0x73] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 6);
		return OpcodeResultContext::Builder(0x73).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x74 - BIT 6, H
	d_cbOpcodes[0x74] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 6);
		return OpcodeResultContext::Builder(0x74).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x75 - BIT 6, L
	d_cbOpcodes[0x75] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 6);
		return OpcodeResultContext::Builder(0x75).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x76 - BIT 6, (HL)
	d_cbOpcodes[0x76] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 6);
		return OpcodeResultContext::Builder(0x76).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x77 - BIT 6, A
	d_cbOpcodes[0x77] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 6);
		return OpcodeResultContext::Builder(0x77).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x78 - BIT 7, B
	d_cbOpcodes[0x78] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 7);
		return OpcodeResultContext::Builder(0x78).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x79 - BIT 7, C
	d_cbOpcodes[0x79] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 7);
		return OpcodeResultContext::Builder(0x79).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7A - BIT 7, D
	d_cbOpcodes[0x7A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 7);
		return OpcodeResultContext::Builder(0x7A).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7B - BIT 7, E
	d_cbOpcodes[0x7B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 7);
		return OpcodeResultContext::Builder(0x7B).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7C - BIT 7, H
	d_cbOpcodes[0x7C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 7);
		return OpcodeResultContext::Builder(0x7C, true).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7D - BIT 7, L
	d_cbOpcodes[0x7D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 7);
		return OpcodeResultContext::Builder(0x7D).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7E - BIT 7, (HL)
	d_cbOpcodes[0x7E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 7);
		return OpcodeResultContext::Builder(0x7E).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7F - BIT 7, A
	d_cbOpcodes[0x7F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 7);
		return OpcodeResultContext::Builder(0x7F).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x10 - RL B
	d_cbOpcodes[0x10] = [this]() {
		fullCircularRotateLeft(*d_regs, *d_ram,
							[this](ByteType t) { d_regs->B(t); },
							[this]() { return d_regs->B(); });
		return OpcodeResultContext::Builder(0x10, true).ShortCycle().IncrementPCDefault().Build();
	};

	// 0x11 - RL C
	d_cbOpcodes[0x11] = [this]() {
		fullCircularRotateLeft(*d_regs, *d_ram,
							[this](ByteType t) { d_regs->C(t); },
							[this]() { return d_regs->C(); });
		return OpcodeResultContext::Builder(0x11, true).ShortCycle().IncrementPCDefault().Build();
	};


}
