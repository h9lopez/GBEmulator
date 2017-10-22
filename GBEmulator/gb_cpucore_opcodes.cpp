#include "CPUCore.h"


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
}



void CPUCore::initOpcodes()
{
	// Initialize regular opcodes

	// NO OP
	d_opcodes[0x00] = [this]() { return 4; };

	// JR NZ, r8
	d_opcodes[0x20] = [this]() 
	{
		if (!d_regs->flagZero())
		{
			// Do jump
			d_regs->IncPCBy( readNextByte() );
			return 12;
		}
		d_regs->IncPC();
		return 8;
	};

	// JR NC, r8
	d_opcodes[0x30] = [this]()
	{
		if (!d_regs->flagCarry())
		{
			d_regs->IncPCBy( readNextByte() );
			return 12;
		}
		d_regs->IncPC();
		return 8;
	};

	// JR r8
	d_opcodes[0x18] = [this]()
	{
		d_regs->IncPCBy( readNextByte() );
		return 12;
	};

	// JR Z, r8
	d_opcodes[0x28] = [this]()
	{
		if (d_regs->flagZero())
		{
			d_regs->IncPCBy( readNextByte() );
			return 12;
		}
		d_regs->IncPC();
		return 8;
	};

	// JR C, r8
	d_opcodes[0x38] = [this]()
	{
		if (d_regs->flagCarry())
		{
			d_regs->IncPCBy( readNextByte() );
			return 12;
		}
		d_regs->IncPC();
		return 8;
	};

	// ======================== 16bit immediate load -> 16bit regs
	d_opcodes[0x01] = [this]() { d_regs->BC(readNextTwoBytes()); return 12; };
	d_opcodes[0x11] = [this]() { d_regs->DE(readNextTwoBytes()); return 12; };
	d_opcodes[0x21] = [this]() { d_regs->HL(readNextTwoBytes()); return 12; };
	d_opcodes[0x31] = [this]() { d_regs->SP(readNextTwoBytes()); return 12; };

	// INC Increments
	d_opcodes[0x03] = [this]() { d_regs->BC(d_regs->BC() + 1); return 8; };
	d_opcodes[0x13] = [this]() { d_regs->DE(d_regs->DE() + 1); return 8; };
	d_opcodes[0x23] = [this]() { d_regs->HL(d_regs->HL() + 1); return 8; };
	d_opcodes[0x33] = [this]() { d_regs->SP(d_regs->SP() + 1); return 8; };
	// individual regs

	// INC B
	d_opcodes[0x04] = [this]() 
	{ 
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_regs->B(); });

		return 4;
	};
	
	// INC D
	d_opcodes[0x14] = [this]() 
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->D(t); },
			[this]() { return d_regs->D(); });
		return 4;
	};

	// INC H
	d_opcodes[0x24] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->H(t); },
			[this]() { return d_regs->H(); });
		return 4;
	};
	
	// INC (HL)
	d_opcodes[0x34] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_ram->writeByte(d_regs->HL(), t); },
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return 12;
	};

	// INC C
	d_opcodes[0x0C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->C(t); },
			[this]() { return d_regs->C(); });
		return 4;
	};

	// INC E
	d_opcodes[0x1C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->E(t); },
			[this]() { return d_regs->E(); });
		return 4;
	};

	// INC L
	d_opcodes[0x2C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->L(t); },
			[this]() { return d_regs->L(); });
		return 4;
	};

	// INC A
	d_opcodes[0x3C] = [this]()
	{
		incrementRegister(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); });
		return 4;
	};

	// DEC instructions

	// DEC B
	d_opcodes[0x05] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_regs->B(); });
		return 4;
	};

	// DEC D
	d_opcodes[0x15] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->D(t); },
			[this]() { return d_regs->D(); });
		return 4;
	};
	
	// DEC H
	d_opcodes[0x25] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->H(t); },
			[this]() { return d_regs->H(); });
		return 4;
	};

	// DEC (HL)
	d_opcodes[0x35] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_ram->writeByte(d_regs->HL(), t); },
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return 12;
	};

	// DEC C
	d_opcodes[0x0D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->C(t); },
			[this]() { return d_regs->C(); });
		return 4;
	};

	// DEC E
	d_opcodes[0x1D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->E(t); },
			[this]() { return d_regs->E(); });
		return 4;
	};

	// DEC L
	d_opcodes[0x2D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->L(t); },
			[this]() { return d_regs->L(); });
		return 4;
	};

	// DEC A
	d_opcodes[0x3D] = [this]()
	{
		decrementRegister(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this]() { return d_regs->A(); });
		return 4;
	};

	// 16-bit decrements
	// DEC BC
	d_opcodes[0x0B] = [this]() { d_regs->BC(d_regs->BC() + 1); return 8; };
	// DEC DE
	d_opcodes[0x1B] = [this]() { d_regs->DE(d_regs->DE() + 1); return 8; };
	// DEC HL
	d_opcodes[0x2B] = [this]() { d_regs->HL(d_regs->HL() + 1); return 8; };
	// DEC SP
	d_opcodes[0x3B] = [this]() { d_regs->SP(d_regs->SP() + 1); return 8; };


	// XOR

	// XOR B
	d_opcodes[0xA8] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->B(); });
		return 4;
	};

	// XOR C
	d_opcodes[0xA9] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->C(); });
		return 4;
	};

	// XOR D
	d_opcodes[0xAA] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->D(); });
		return 4;
	};

	// XOR E
	d_opcodes[0xAB] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->E(); });
		return 4;
	};

	// XOR H
	d_opcodes[0xAC] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->H(); });
		return 4;
	};

	// XOR L
	d_opcodes[0xAD] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->L(); });
		return 4;
	};

	// XOR (HL)
	d_opcodes[0xAE] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_ram->readByte(d_regs->HL()); });
		return 4;
	};

	// XOR A
	d_opcodes[0xAF] = [this]()
	{
		xorRegister(*d_regs,
			[this]() { return d_regs->A(); });
		return 4;
	};

	// LD (16b), A

	// LD (BC), A
	d_opcodes[0x02] = [this]()
	{
		d_ram->writeByte(d_regs->BC(), d_regs->A());
		return 8;
	};

	// LD (DE), A
	d_opcodes[0x12] = [this]()
	{
		d_ram->writeByte(d_regs->DE(), d_regs->A());
		return 8;
	};

	// LD (HL+), A
	d_opcodes[0x22] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->A());
		d_regs->HL( d_regs->HL() + 1 );
		return 8;
	};

	// LD (HL-), A
	d_opcodes[0x32] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), d_regs->A());
		d_regs->HL( d_regs->HL() - 1 );
		return 8;
	};


	// LD REG, d8 instructions

	// LD B, d8
	d_opcodes[0x06] = [this]()
	{
		d_regs->B(readNextByte());
		return 8;
	};

	// LD D, d8
	d_opcodes[0x16] = [this]()
	{
		d_regs->D(readNextByte());
		return 8;
	};

	// LD H, d8
	d_opcodes[0x26] = [this]()
	{
		d_regs->H(readNextByte());
		return 8;
	};

	// LD (HL), d8
	d_opcodes[0x36] = [this]()
	{
		d_ram->writeByte(d_regs->HL(), readNextByte());
		return 12;
	};

	// LD C, d8
	d_opcodes[0x0E] = [this]()
	{
		d_regs->C(readNextByte());
		return 8;
	};

	// LD E, d8
	d_opcodes[0x1E] = [this]()
	{
		d_regs->E(readNextByte());
		return 8;
	};

	// LD L, d8
	d_opcodes[0x2E] = [this]()
	{
		d_regs->L(readNextByte());
		return 8;
	};

	// LD A, d8
	d_opcodes[0x3E] = [this]()
	{
		d_regs->A(readNextByte());
		return 8;
	};

	// IO Port Read/Write Instructions

	// LD ($FF00+C), A
	d_opcodes[0xE2] = [this]()
	{
		d_ram->writeByte(0xFF00 + d_regs->C(), d_regs->A());
		return 8;
	};

	// LD A, ($FF00+C)
	d_opcodes[0xF2] = [this]()
	{
		d_regs->A(d_ram->readByte(0xFF00 + d_regs->C()));
		return 8;
	};

	// HALT
	d_opcodes[0x76] = [this]()
	{
		// TODO: Decide whether to use exception or return status
		return 4;
	};


	// =============== CB Opcode Section
	// Initialize CB-prefix opcodes
	
	// BIT 0, B
	d_cbOpcodes[0x40] = [this]() { 
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 0); 
		return 8; 
	};

	// 0x41 - BIT 0, C
	d_cbOpcodes[0x41] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 0);
		return 8;
	};

	// 0x42 - BIT 0, D
	d_cbOpcodes[0x42] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 0);
		return 8;
	};

	// 0x43 - BIT 0, E
	d_cbOpcodes[0x43] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 0);
		return 8;
	};

	// 0x44 - BIT 0, H
	d_cbOpcodes[0x44] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 0);
		return 8;
	};

	// 0x45 - BIT 0, L
	d_cbOpcodes[0x45] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 0);
		return 8;
	};

	// 0x46 - BIT 0, (HL)
	d_cbOpcodes[0x46] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 0);
		return 8;
	};
	// 0x47 - BIT 0, A
	d_cbOpcodes[0x47] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 0);
		return 8;
	};


	// 0x48 - BIT 1, B
	d_cbOpcodes[0x48] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 1);
		return 8;
	};

	// 0x49 - BIT 1, C
	d_cbOpcodes[0x49] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 1);
		return 8;
	};
	// 0x4A - BIT 1, D
	d_cbOpcodes[0x4A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 1);
		return 8;
	};
	// 0x4B - BIT 1, E
	d_cbOpcodes[0x4B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 1);
		return 8;
	};
	// 0x4C - BIT 1, H
	d_cbOpcodes[0x4C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 1);
		return 8;
	};
	// 0x4D - BIT 1, L
	d_cbOpcodes[0x4D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 1);
		return 8;
	};
	// 0x4E - BIT 1, (HL)
	d_cbOpcodes[0x4E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 1);
		return 8;
	};
	// 0x4F - BIT 1, A
	d_cbOpcodes[0x4F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 1);
		return 8;
	};


	// 0x50 - BIT 2, B
	d_cbOpcodes[0x50] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 2);
		return 8;
	};
	// 0x51 - BIT 2, C
	d_cbOpcodes[0x51] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 2);
		return 8;
	};
	// 0x52 - BIT 2, D
	d_cbOpcodes[0x52] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 2);
		return 8;
	};
	// 0x53 - BIT 2, E
	d_cbOpcodes[0x53] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 2);
		return 8;
	};
	// 0x54 - BIT 2, H
	d_cbOpcodes[0x54] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 2);
		return 8;
	};
	// 0x55 - BIT 2, L
	d_cbOpcodes[0x55] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 2);
		return 8;
	};
	// 0x56 - BIT 2, (HL)
	d_cbOpcodes[0x56] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 2);
		return 8;
	};
	// 0x57 - BIT 2, A
	d_cbOpcodes[0x57] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 2);
		return 8;
	};


	// 0x58 - BIT 3, B
	d_cbOpcodes[0x58] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 3);
		return 8;
	};
	// 0x59 - BIT 3, C
	d_cbOpcodes[0x59] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 3);
		return 8;
	};
	// 0x5A - BIT 3, D
	d_cbOpcodes[0x5A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 3);
		return 8;
	};
	// 0x5B - BIT 3, E
	d_cbOpcodes[0x5B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 3);
		return 8;
	};
	// 0x5C - BIT 3, H
	d_cbOpcodes[0x5C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 3);
		return 8;
	};
	// 0x5D - BIT 3, L
	d_cbOpcodes[0x5D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 3);
		return 8;
	};
	// 0x5E - BIT 3, (HL)
	d_cbOpcodes[0x5E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 3);
		return 8;
	};
	// 0x5F - BIT 3, A
	d_cbOpcodes[0x5F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 3);
		return 8;
	};


	// 0x60 - BIT 4, B
	d_cbOpcodes[0x60] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 4);
		return 8;
	};
	// 0x61 - BIT 4, C
	d_cbOpcodes[0x61] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 4);
		return 8;
	};
	// 0x62 - BIT 4, D
	d_cbOpcodes[0x62] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 4);
		return 8;
	};
	// 0x63 - BIT 4, E
	d_cbOpcodes[0x63] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 4);
		return 8;
	};
	// 0x64 - BIT 4, H
	d_cbOpcodes[0x64] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 4);
		return 8;
	};
	// 0x65 - BIT 4, L
	d_cbOpcodes[0x65] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 4);
		return 8;
	};
	// 0x66 - BIT 4, (HL)
	d_cbOpcodes[0x66] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 4);
		return 8;
	};
	// 0x67 - BIT 4, A
	d_cbOpcodes[0x67] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 4);
		return 8;
	};


	// 0x68 - BIT 5, B
	d_cbOpcodes[0x68] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 5);
		return 8;
	};
	// 0x69 - BIT 5, C
	d_cbOpcodes[0x69] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 5);
		return 8;
	};
	// 0x6A - BIT 5, D
	d_cbOpcodes[0x6A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 5);
		return 8;
	};
	// 0x6B - BIT 5, E
	d_cbOpcodes[0x6B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 5);
		return 8;
	};
	// 0x6C - BIT 5, H
	d_cbOpcodes[0x6C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 5);
		return 8;
	};
	// 0x6D - BIT 5, L
	d_cbOpcodes[0x6D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 5);
		return 8;
	};
	// 0x6E - BIT 5, (HL)
	d_cbOpcodes[0x6E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 5);
		return 8;
	};
	// 0x6F - BIT 5, A
	d_cbOpcodes[0x6F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 5);
		return 8;
	};


	// 0x70 - BIT 6, B
	d_cbOpcodes[0x70] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 6);
		return 8;
	};
	// 0x71 - BIT 6, C
	d_cbOpcodes[0x71] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 6);
		return 8;
	};
	// 0x72 - BIT 6, D
	d_cbOpcodes[0x72] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 6);
		return 8;
	};
	// 0x73 - BIT 6, E
	d_cbOpcodes[0x73] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 6);
		return 8;
	};
	// 0x74 - BIT 6, H
	d_cbOpcodes[0x74] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 6);
		return 8;
	};
	// 0x75 - BIT 6, L
	d_cbOpcodes[0x75] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 6);
		return 8;
	};
	// 0x76 - BIT 6, (HL)
	d_cbOpcodes[0x76] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 6);
		return 8;
	};
	// 0x77 - BIT 6, A
	d_cbOpcodes[0x77] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 6);
		return 8;
	};


	// 0x78 - BIT 7, B
	d_cbOpcodes[0x78] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->B(); }, 7);
		return 8;
	};
	// 0x79 - BIT 7, C
	d_cbOpcodes[0x79] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->C(); }, 7);
		return 8;
	};
	// 0x7A - BIT 7, D
	d_cbOpcodes[0x7A] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->D(); }, 7);
		return 8;
	};
	// 0x7B - BIT 7, E
	d_cbOpcodes[0x7B] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->E(); }, 7);
		return 8;
	};
	// 0x7C - BIT 7, H
	d_cbOpcodes[0x7C] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->H(); }, 7);
		return 8;
	};
	// 0x7D - BIT 7, L
	d_cbOpcodes[0x7D] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->L(); }, 7);
		return 8;
	};
	// 0x7E - BIT 7, (HL)
	d_cbOpcodes[0x7E] = [this]() {
		checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 7);
		return 8;
	};
	// 0x7F - BIT 7, A
	d_cbOpcodes[0x7F] = [this]() {
		checkBit(*d_regs, [this]() { return d_regs->A(); }, 7);
		return 8;
	};



}
