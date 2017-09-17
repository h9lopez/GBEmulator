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
}



void CPUCore::initOpcodes()
{
	// Initialize regular opcodes

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


	// Initialize CB-prefix opcodes

}
