#include <bitset>
#include "gb_typeutils.h"
#include "CPUCore.h"
#include "ReverseOpcodeMap.h"

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
		
		regs.flagZero( (sum & 0x11) == 0 );
		regs.flagSubtract( false );
		regs.flagHalfCarry(isHalfCarry);
		regs.flagCarry(isCarry);	
	}

	void performADC(RegBank &regs,
		std::function<ByteType(void)> secondaryRegGet)
	{
		performByteAdd(regs,
			[&regs](ByteType t) { regs.A(t); },
			[&regs]() { return regs.A(); },
			secondaryRegGet
		);

		performByteAdd(regs,
			[&regs](ByteType t) {regs.A(t); },
			[&regs]() { return regs.A(); },
			[&regs]() { return regs.flagCarry() ? 1 : 0; }
		);
	}
}


using namespace std;


void CPUCore::initOpcodes()
{
	// Initialize regular opcodes

	// NO OP
	d_opcodes[0x00] = [this]() { 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (a16), SP
	d_opcodes[0x08] = [this]()
	{
		// No flags affected
		d_ram->writeWord( readNextTwoBytes(), d_regs->SP() );
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADD HL, BC
	d_opcodes[0x09] = [this]()
	{
		loadIntoHL(*d_regs, [this]() { return d_regs->BC(); });

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADD HL, DE
	d_opcodes[0x19] = [this]()
	{
		loadIntoHL(*d_regs, [this]() { return d_regs->DE(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADD HL, HL
	d_opcodes[0x29] = [this]()
	{
		loadIntoHL(*d_regs, [this]() { return d_regs->HL(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADD HL, SP
	d_opcodes[0x39] = [this]()
	{
		loadIntoHL(*d_regs, [this]() { return d_regs->SP(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, (BC)
	d_opcodes[0x0A] = [this]() 
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_ram->readByte( d_regs->BC() ); }
		);

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, (DE)
	d_opcodes[0x1A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_ram->readByte( d_regs->DE() ); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// JR NZ, r8
	d_opcodes[0x20] = [this]() 
	{
		if (!d_regs->flagZero())
		{
			// Do jump
			WordType incAmt = readNextByte();
			return make_tuple(incAmt, CYCLE_TAKEN);
		}

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// JR NC, r8
	d_opcodes[0x30] = [this]()
	{
		if (!d_regs->flagCarry())
		{
			WordType incAmt = readNextByte();
			return make_tuple(incAmt, CYCLE_TAKEN);
		}

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// JR r8
	d_opcodes[0x18] = [this]()
	{
		WordType incAmt = readNextByte();
		return make_tuple(incAmt, CYCLE_TAKEN);
	};

	// JR Z, r8
	d_opcodes[0x28] = [this]()
	{
		if (d_regs->flagZero())
		{
			WordType incAmt = readNextByte();
			return make_tuple(incAmt, CYCLE_TAKEN);
		}
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// JR C, r8
	d_opcodes[0x38] = [this]()
	{
		if (d_regs->flagCarry())
		{
			WordType incAmt = readNextByte();
			return make_tuple(incAmt, CYCLE_TAKEN);
		}
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ======================== 16bit immediate load -> 16bit regs
	d_opcodes[0x01] = [this]() { 
		d_regs->BC(readNextTwoBytes()); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	d_opcodes[0x11] = [this]() { 
		d_regs->DE(readNextTwoBytes()); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	d_opcodes[0x21] = [this]() { 
		d_regs->HL(readNextTwoBytes()); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	d_opcodes[0x31] = [this]() { 
		d_regs->SP(readNextTwoBytes()); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};

	// INC Increments
	d_opcodes[0x03] = [this]() { 
		d_regs->BC(d_regs->BC() + 1); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	d_opcodes[0x13] = [this]() { 
		d_regs->DE(d_regs->DE() + 1); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	d_opcodes[0x23] = [this]() { 
		d_regs->HL(d_regs->HL() + 1); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	d_opcodes[0x33] = [this]() { 
		d_regs->SP(d_regs->SP() + 1); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	// individual regs

	// INC B
	d_opcodes[0x04] = [this]() 
	{ 
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_regs->B(); });

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	
	// INC D
	d_opcodes[0x14] = [this]() 
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->D(t); },
			[this]() { return d_regs->D(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// INC H
	d_opcodes[0x24] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->H(t); },
			[this]() { return d_regs->H(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	
	// INC (HL)
	d_opcodes[0x34] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_ram->writeByte(d_regs->HL(), t); },
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// INC C
	d_opcodes[0x0C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->C(t); },
			[this]() { return d_regs->C(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// INC E
	d_opcodes[0x1C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->E(t); },
			[this]() { return d_regs->E(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// INC L
	d_opcodes[0x2C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->L(t); },
			[this]() { return d_regs->L(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// INC A
	d_opcodes[0x3C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// DEC instructions

	// DEC B
	d_opcodes[0x05] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_regs->B(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// DEC D
	d_opcodes[0x15] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->D(t); },
			[this]() { return d_regs->D(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	
	// DEC H
	d_opcodes[0x25] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->H(t); },
			[this]() { return d_regs->H(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// DEC (HL)
	d_opcodes[0x35] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_ram->writeByte(d_regs->HL(), t); },
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// DEC C
	d_opcodes[0x0D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->C(t); },
			[this]() { return d_regs->C(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// DEC E
	d_opcodes[0x1D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->E(t); },
			[this]() { return d_regs->E(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// DEC L
	d_opcodes[0x2D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->L(t); },
			[this]() { return d_regs->L(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// DEC A
	d_opcodes[0x3D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// 16-bit decrements
	// DEC BC
	d_opcodes[0x0B] = [this]() { 
		d_regs->BC(d_regs->BC() + 1); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	// DEC DE
	d_opcodes[0x1B] = [this]() { 
		d_regs->DE(d_regs->DE() + 1); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	// DEC HL
	d_opcodes[0x2B] = [this]() { 
		d_regs->HL(d_regs->HL() + 1); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};
	// DEC SP
	d_opcodes[0x3B] = [this]() { 
		d_regs->SP(d_regs->SP() + 1); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};


	// XOR

	// XOR B
	d_opcodes[0xA8] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->B(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// XOR C
	d_opcodes[0xA9] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->C(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// XOR D
	d_opcodes[0xAA] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->D(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// XOR E
	d_opcodes[0xAB] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->E(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// XOR H
	d_opcodes[0xAC] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->H(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// XOR L
	d_opcodes[0xAD] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->L(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// XOR (HL)
	d_opcodes[0xAE] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// XOR A
	d_opcodes[0xAF] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->A(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (16b), A

	// LD (BC), A
	d_opcodes[0x02] = [this]()
	{
		d_ram->writeByte(d_regs->BC(), d_regs->A());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (DE), A
	d_opcodes[0x12] = [this]()
	{
		d_ram->writeByte(d_regs->DE(), d_regs->A());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL+), A
	d_opcodes[0x22] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->A());
		d_regs->HL( d_regs->HL() + 1 );
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL-), A
	d_opcodes[0x32] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->A());
		d_regs->HL( d_regs->HL() - 1 );
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// LD REG, d8 instructions

	// LD B, d8
	d_opcodes[0x06] = [this]()
	{
		d_regs->B(readNextByte());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD D, d8
	d_opcodes[0x16] = [this]()
	{
		d_regs->D(readNextByte());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H, d8
	d_opcodes[0x26] = [this]()
	{
		d_regs->H(readNextByte());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL), d8
	d_opcodes[0x36] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), readNextByte());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, d8
	d_opcodes[0x0E] = [this]()
	{
		d_regs->C(readNextByte());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD E, d8
	d_opcodes[0x1E] = [this]()
	{
		d_regs->E(readNextByte());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, d8
	d_opcodes[0x2E] = [this]()
	{
		d_regs->L(readNextByte());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, d8
	d_opcodes[0x3E] = [this]()
	{
		d_regs->A(readNextByte());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// IO Port Read/Write Instructions

	// LD ($FF00+C), A
	d_opcodes[0xE2] = [this]()
	{
		d_ram->writeByte(0xFF00 + d_regs->C(), d_regs->A());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, ($FF00+C)
	d_opcodes[0xF2] = [this]()
	{
		d_regs->A(d_ram->readByte(0xFF00 + d_regs->C()));
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// HALT
	d_opcodes[0x76] = [this]()
	{
		// TODO: Decide whether to use exception or return status
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// CPL
	d_opcodes[0x2F] = [this]()
	{
		d_regs->A( ~d_regs->A() );
		d_regs->flagSubtract(true);
		d_regs->flagHalfCarry(true);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// CCF
	d_opcodes[0x3F] = [this]()
	{
		d_regs->flagCarry( !(d_regs->flagCarry()) );
		d_regs->flagSubtract(false);
		d_regs->flagHalfCarry(false);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// SCF
	d_opcodes[0x37] = [this]()
	{
		d_regs->flagCarry(true);
		d_regs->flagSubtract(false);
		d_regs->flagHalfCarry(false);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD B,B
	d_opcodes[0x40] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->B(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD B,C
	d_opcodes[0x41] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->C(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD B,D
	d_opcodes[0x42] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->D(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD B,E
	d_opcodes[0x43] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->E(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD B,H
	d_opcodes[0x44] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->H(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD B,L
	d_opcodes[0x45] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->L(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD B,A
	d_opcodes[0x47] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t);  },
			[this]() { return d_regs->A(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, B
	d_opcodes[0x48] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->B(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, C
	d_opcodes[0x49] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->C(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, D
	d_opcodes[0x4A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->D(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, E
	d_opcodes[0x4B] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->E(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, H
	d_opcodes[0x4C] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->H(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, L
	d_opcodes[0x4D] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->L(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, A
	d_opcodes[0x4F] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t);  },
			[this]() { return d_regs->A(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// LD D,B
	d_opcodes[0x50] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->B(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD D,C
	d_opcodes[0x51] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->C(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD D,D
	d_opcodes[0x52] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->D(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD D,E
	d_opcodes[0x53] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->E(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD D,H
	d_opcodes[0x54] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->H(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD D,L
	d_opcodes[0x55] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->L(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD D,A
	d_opcodes[0x57] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t);  },
			[this]() { return d_regs->A(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	
	// LD E, B
	d_opcodes[0x58] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->B(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD E, C
	d_opcodes[0x59] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->C(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD E, D
	d_opcodes[0x5A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->D(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD E, E
	d_opcodes[0x5B] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->E(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD E, H
	d_opcodes[0x5C] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->H(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD E, L
	d_opcodes[0x5D] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->L(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD E, A
	d_opcodes[0x5F] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t);  },
			[this]() { return d_regs->A(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H,B
	d_opcodes[0x60] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->B(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H,C
	d_opcodes[0x61] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->C(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H,D
	d_opcodes[0x62] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->D(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H,E
	d_opcodes[0x63] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->E(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H,H
	d_opcodes[0x64] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->H(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H,L
	d_opcodes[0x65] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->L(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H,A
	d_opcodes[0x67] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t);  },
			[this]() { return d_regs->A(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, B
	d_opcodes[0x68] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->B(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, C
	d_opcodes[0x69] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->C(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, D
	d_opcodes[0x6A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->D(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, E
	d_opcodes[0x6B] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->E(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, H
	d_opcodes[0x6C] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->H(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, L
	d_opcodes[0x6D] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->L(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, A
	d_opcodes[0x6F] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t);  },
			[this]() { return d_regs->A(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	
	// LD A, B
	d_opcodes[0x78] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->B(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, C
	d_opcodes[0x79] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->C(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, D
	d_opcodes[0x7A] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->D(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, E
	d_opcodes[0x7B] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->E(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, H
	d_opcodes[0x7C] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->H(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, L
	d_opcodes[0x7D] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->L(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, A
	d_opcodes[0x7F] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t);  },
			[this]() { return d_regs->A(); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD B, (HL)
	d_opcodes[0x46] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_ram->readByte( d_regs->HL() ); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD C, (HL)
	d_opcodes[0x4E] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->C(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD D, (HL)
	d_opcodes[0x56] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->D(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD E, (HL)
	d_opcodes[0x5E] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->E(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD H, (HL)
	d_opcodes[0x66] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->H(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD L, (HL)
	d_opcodes[0x6E] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->L(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD A, (HL)
	d_opcodes[0x7E] = [this]()
	{
		loadByteIntoRegister(
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_ram->readByte(d_regs->HL()); }
		);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL), B
	d_opcodes[0x70] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->B());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL), C
	d_opcodes[0x71] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->C());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL), D
	d_opcodes[0x72] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->D());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL), E
	d_opcodes[0x73] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->E());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL), H
	d_opcodes[0x74] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->H());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// LD (HL), L
	d_opcodes[0x75] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->L());
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
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

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADC A,B
	d_opcodes[0x88] = [this]()
	{
		// Do regular A + B first
		performByteAdd(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->B(); }
		);

		// I guess we're technically adding from right to left, c + B + A
		performByteAdd(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); },
			[this]() { return d_regs->flagCarry() ? 1 : 0; });

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADC A,C
	d_opcodes[0x89] = [this]()
	{
		performADC(*d_regs, [this]() { return d_regs->C(); });

		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADC A,D
	d_opcodes[0x8A] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->D(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADC A,E
	d_opcodes[0x8B] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->E(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADC A,H
	d_opcodes[0x8C] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->H(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADC A,L
	d_opcodes[0x8D] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->L(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADC A,(HL) TODO: DO THIS
	d_opcodes[0x8D] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->L(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// ADC A,A TODO: DO THIS
	d_opcodes[0x8D] = [this]()
	{
		performADC(*d_regs, [this]() {return d_regs->L(); });
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};



	// =============== CB Opcode Section
	// Initialize CB-prefix opcodes
	
	// BIT 0, B
	d_cbOpcodes[0x40] = [this]() { 
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 0); 
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN); 
	};

	// 0x41 - BIT 0, C
	d_cbOpcodes[0x41] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 0);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// 0x42 - BIT 0, D
	d_cbOpcodes[0x42] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 0);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// 0x43 - BIT 0, E
	d_cbOpcodes[0x43] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 0);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// 0x44 - BIT 0, H
	d_cbOpcodes[0x44] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 0);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// 0x45 - BIT 0, L
	d_cbOpcodes[0x45] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 0);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// 0x46 - BIT 0, (HL)
	d_cbOpcodes[0x46] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 0);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x47 - BIT 0, A
	d_cbOpcodes[0x47] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 0);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// 0x48 - BIT 1, B
	d_cbOpcodes[0x48] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 1);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};

	// 0x49 - BIT 1, C
	d_cbOpcodes[0x49] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 1);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x4A - BIT 1, D
	d_cbOpcodes[0x4A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 1);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x4B - BIT 1, E
	d_cbOpcodes[0x4B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 1);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x4C - BIT 1, H
	d_cbOpcodes[0x4C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 1);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x4D - BIT 1, L
	d_cbOpcodes[0x4D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 1);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x4E - BIT 1, (HL)
	d_cbOpcodes[0x4E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 1);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x4F - BIT 1, A
	d_cbOpcodes[0x4F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 1);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// 0x50 - BIT 2, B
	d_cbOpcodes[0x50] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 2);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x51 - BIT 2, C
	d_cbOpcodes[0x51] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 2);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x52 - BIT 2, D
	d_cbOpcodes[0x52] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 2);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x53 - BIT 2, E
	d_cbOpcodes[0x53] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 2);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x54 - BIT 2, H
	d_cbOpcodes[0x54] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 2);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x55 - BIT 2, L
	d_cbOpcodes[0x55] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 2);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x56 - BIT 2, (HL)
	d_cbOpcodes[0x56] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 2);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x57 - BIT 2, A
	d_cbOpcodes[0x57] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 2);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// 0x58 - BIT 3, B
	d_cbOpcodes[0x58] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 3);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x59 - BIT 3, C
	d_cbOpcodes[0x59] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 3);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x5A - BIT 3, D
	d_cbOpcodes[0x5A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 3);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x5B - BIT 3, E
	d_cbOpcodes[0x5B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 3);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x5C - BIT 3, H
	d_cbOpcodes[0x5C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 3);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x5D - BIT 3, L
	d_cbOpcodes[0x5D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 3);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x5E - BIT 3, (HL)
	d_cbOpcodes[0x5E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 3);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x5F - BIT 3, A
	d_cbOpcodes[0x5F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 3);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// 0x60 - BIT 4, B
	d_cbOpcodes[0x60] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 4);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x61 - BIT 4, C
	d_cbOpcodes[0x61] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 4);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x62 - BIT 4, D
	d_cbOpcodes[0x62] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 4);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x63 - BIT 4, E
	d_cbOpcodes[0x63] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 4);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x64 - BIT 4, H
	d_cbOpcodes[0x64] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 4);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x65 - BIT 4, L
	d_cbOpcodes[0x65] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 4);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x66 - BIT 4, (HL)
	d_cbOpcodes[0x66] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 4);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x67 - BIT 4, A
	d_cbOpcodes[0x67] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 4);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// 0x68 - BIT 5, B
	d_cbOpcodes[0x68] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 5);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x69 - BIT 5, C
	d_cbOpcodes[0x69] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 5);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x6A - BIT 5, D
	d_cbOpcodes[0x6A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 5);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x6B - BIT 5, E
	d_cbOpcodes[0x6B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 5);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x6C - BIT 5, H
	d_cbOpcodes[0x6C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 5);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x6D - BIT 5, L
	d_cbOpcodes[0x6D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 5);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x6E - BIT 5, (HL)
	d_cbOpcodes[0x6E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 5);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x6F - BIT 5, A
	d_cbOpcodes[0x6F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 5);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// 0x70 - BIT 6, B
	d_cbOpcodes[0x70] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 6);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x71 - BIT 6, C
	d_cbOpcodes[0x71] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 6);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x72 - BIT 6, D
	d_cbOpcodes[0x72] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 6);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x73 - BIT 6, E
	d_cbOpcodes[0x73] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 6);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x74 - BIT 6, H
	d_cbOpcodes[0x74] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 6);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x75 - BIT 6, L
	d_cbOpcodes[0x75] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 6);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x76 - BIT 6, (HL)
	d_cbOpcodes[0x76] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 6);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x77 - BIT 6, A
	d_cbOpcodes[0x77] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 6);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};


	// 0x78 - BIT 7, B
	d_cbOpcodes[0x78] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 7);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x79 - BIT 7, C
	d_cbOpcodes[0x79] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 7);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x7A - BIT 7, D
	d_cbOpcodes[0x7A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 7);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x7B - BIT 7, E
	d_cbOpcodes[0x7B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 7);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x7C - BIT 7, H
	d_cbOpcodes[0x7C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 7);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x7D - BIT 7, L
	d_cbOpcodes[0x7D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 7);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x7E - BIT 7, (HL)
	d_cbOpcodes[0x7E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 7);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};
	// 0x7F - BIT 7, A
	d_cbOpcodes[0x7F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 7);
		return make_tuple(PC_INC_NORMAL, CYCLE_UNTAKEN);
	};



}
