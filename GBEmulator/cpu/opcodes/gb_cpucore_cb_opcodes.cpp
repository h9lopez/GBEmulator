#include <cpu/CPUCore.h>

#include <cpu/opcodes/gb_cpucore_opcode_utils.h>
#include <cpu/opcodes/OpcodeResultContext.h>


namespace Core {

void CPUCore::initCBOpcodes()
{
    // =============== CB Opcode Section
	// Initialize CB-prefix opcodes

	// Placeholder element for the first opcode in the sequence
	d_opcodes[0xCB] = [this]()
	{
		throw std::runtime_error("CB opcode should never be directly called");
		return OpcodeResultContext::Builder(0xCB).ShortCycle().FreezePC().Build();
	};

	// RLC B
	d_cbOpcodes[0x00] = [this]() {
		OpcodeUtils::fullRotateLeftCarry(*d_regs,
			[this](ByteType t) { d_regs->B(t); },
			[this]() { return d_regs->B(); });
		return OpcodeResultContext::Builder(0x00, true).ShortCycle().IncrementPCDefault().Build();
	};

	// RLC C
	d_cbOpcodes[0x01] = [this]() {
		OpcodeUtils::fullRotateLeftCarry(*d_regs,
			[this](ByteType t) { d_regs->C(t); },
			[this](){ return d_regs->C(); });
		return OpcodeResultContext::Builder(0x01, true).ShortCycle().IncrementPCDefault().Build();
	};

	// RLC D
	d_cbOpcodes[0x02] = [this]() {
		OpcodeUtils::fullRotateLeftCarry(*d_regs,
			[this](ByteType t) { d_regs->D(t); },
			[this](){ return d_regs->D(); });
		return OpcodeResultContext::Builder(0x02, true).ShortCycle().IncrementPCDefault().Build();
	};

	// RLC E
	d_cbOpcodes[0x03] = [this]() {
		OpcodeUtils::fullRotateLeftCarry(*d_regs,
			[this](ByteType t) { d_regs->E(t); },
			[this](){ return d_regs->E(); });
		return OpcodeResultContext::Builder(0x03, true).ShortCycle().IncrementPCDefault().Build();
	};

	// RLC H
	d_cbOpcodes[0x04] = [this]() {
		OpcodeUtils::fullRotateLeftCarry(*d_regs,
			[this](ByteType t) { d_regs->H(t); },
			[this](){ return d_regs->H(); });
		return OpcodeResultContext::Builder(0x04, true).ShortCycle().IncrementPCDefault().Build();
	};

	// RLC L
	d_cbOpcodes[0x05] = [this]() {
		OpcodeUtils::fullRotateLeftCarry(*d_regs,
			[this](ByteType t) { d_regs->L(t); },
			[this](){ return d_regs->L(); });
		return OpcodeResultContext::Builder(0x05, true).ShortCycle().IncrementPCDefault().Build();
	};

	// RLC (HL)
	d_cbOpcodes[0x06] = [this]() {
		OpcodeUtils::fullRotateLeftCarry(*d_regs,
			[this](ByteType t) { d_regs->HL(t); },
			[this](){ return d_regs->HL(); });
		return OpcodeResultContext::Builder(0x06, true).ShortCycle().IncrementPCDefault().Build();
	};

	// RLC A
	d_cbOpcodes[0x07] = [this]() {
		OpcodeUtils::fullRotateLeftCarry(*d_regs,
			[this](ByteType t) { d_regs->A(t); },
			[this](){ return d_regs->A(); });
		return OpcodeResultContext::Builder(0x07, true).ShortCycle().IncrementPCDefault().Build();
	};
	
	// BIT 0, B
	d_cbOpcodes[0x40] = [this]() { 
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->B(); }, 0); 
		return OpcodeResultContext::Builder(0x40).ShortCycle().IncrementPCDefault().Build();
	};

	// 0x41 - BIT 0, C
	d_cbOpcodes[0x41] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->C(); }, 0);
		return OpcodeResultContext::Builder(0x41).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x42 - BIT 0, D
	d_cbOpcodes[0x42] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->D(); }, 0);
		return OpcodeResultContext::Builder(0x42).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x43 - BIT 0, E
	d_cbOpcodes[0x43] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->E(); }, 0);
		return OpcodeResultContext::Builder(0x43).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x44 - BIT 0, H
	d_cbOpcodes[0x44] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->H(); }, 0);
		return OpcodeResultContext::Builder(0x44).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x45 - BIT 0, L
	d_cbOpcodes[0x45] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->L(); }, 0);
		return OpcodeResultContext::Builder(0x45).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x46 - BIT 0, (HL)
	d_cbOpcodes[0x46] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 0);
		return OpcodeResultContext::Builder(0x46).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x47 - BIT 0, A
	d_cbOpcodes[0x47] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->A(); }, 0);
		return OpcodeResultContext::Builder(0x47).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x48 - BIT 1, B
	d_cbOpcodes[0x48] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->B(); }, 1);
		return OpcodeResultContext::Builder(0x48).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x49 - BIT 1, C
	d_cbOpcodes[0x49] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->C(); }, 1);
		return OpcodeResultContext::Builder(0x49).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4A - BIT 1, D
	d_cbOpcodes[0x4A] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->D(); }, 1);
		return OpcodeResultContext::Builder(0x4A).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4B - BIT 1, E
	d_cbOpcodes[0x4B] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->E(); }, 1);
		return OpcodeResultContext::Builder(0x4B).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4C - BIT 1, H
	d_cbOpcodes[0x4C] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->H(); }, 1);
		return OpcodeResultContext::Builder(0x4C).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4D - BIT 1, L
	d_cbOpcodes[0x4D] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->L(); }, 1);
		return OpcodeResultContext::Builder(0x4D).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4E - BIT 1, (HL)
	d_cbOpcodes[0x4E] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 1);
		return OpcodeResultContext::Builder(0x4E).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x4F - BIT 1, A
	d_cbOpcodes[0x4F] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->A(); }, 1);
		return OpcodeResultContext::Builder(0x4F).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x50 - BIT 2, B
	d_cbOpcodes[0x50] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->B(); }, 2);
		return OpcodeResultContext::Builder(0x50).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x51 - BIT 2, C
	d_cbOpcodes[0x51] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->C(); }, 2);
		return OpcodeResultContext::Builder(0x51).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x52 - BIT 2, D
	d_cbOpcodes[0x52] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->D(); }, 2);
		return OpcodeResultContext::Builder(0x52).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x53 - BIT 2, E
	d_cbOpcodes[0x53] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->E(); }, 2);
		return OpcodeResultContext::Builder(0x53).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x54 - BIT 2, H
	d_cbOpcodes[0x54] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->H(); }, 2);
		return OpcodeResultContext::Builder(0x54).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x55 - BIT 2, L
	d_cbOpcodes[0x55] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->L(); }, 2);
		return OpcodeResultContext::Builder(0x55).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x56 - BIT 2, (HL)
	d_cbOpcodes[0x56] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 2);
		return OpcodeResultContext::Builder(0x56).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x57 - BIT 2, A
	d_cbOpcodes[0x57] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->A(); }, 2);
		return OpcodeResultContext::Builder(0x57).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x58 - BIT 3, B
	d_cbOpcodes[0x58] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->B(); }, 3);
		return OpcodeResultContext::Builder(0x58).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x59 - BIT 3, C
	d_cbOpcodes[0x59] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->C(); }, 3);
		return OpcodeResultContext::Builder(0x59).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5A - BIT 3, D
	d_cbOpcodes[0x5A] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->D(); }, 3);
		return OpcodeResultContext::Builder(0x5A).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5B - BIT 3, E
	d_cbOpcodes[0x5B] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->E(); }, 3);
		return OpcodeResultContext::Builder(0x5B).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5C - BIT 3, H
	d_cbOpcodes[0x5C] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->H(); }, 3);
		return OpcodeResultContext::Builder(0x5C).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5D - BIT 3, L
	d_cbOpcodes[0x5D] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->L(); }, 3);
		return OpcodeResultContext::Builder(0x5D).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5E - BIT 3, (HL)
	d_cbOpcodes[0x5E] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 3);
		return OpcodeResultContext::Builder(0x5E).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x5F - BIT 3, A
	d_cbOpcodes[0x5F] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->A(); }, 3);
		return OpcodeResultContext::Builder(0x5F).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x60 - BIT 4, B
	d_cbOpcodes[0x60] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->B(); }, 4);
		return OpcodeResultContext::Builder(0x60).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x61 - BIT 4, C
	d_cbOpcodes[0x61] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->C(); }, 4);
		return OpcodeResultContext::Builder(0x61).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x62 - BIT 4, D
	d_cbOpcodes[0x62] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->D(); }, 4);
		return OpcodeResultContext::Builder(0x62).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x63 - BIT 4, E
	d_cbOpcodes[0x63] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->E(); }, 4);
		return OpcodeResultContext::Builder(0x63).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x64 - BIT 4, H
	d_cbOpcodes[0x64] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->H(); }, 4);
		return OpcodeResultContext::Builder(0x64).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x65 - BIT 4, L
	d_cbOpcodes[0x65] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->L(); }, 4);
		return OpcodeResultContext::Builder(0x65).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x66 - BIT 4, (HL)
	d_cbOpcodes[0x66] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 4);
		return OpcodeResultContext::Builder(0x66).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x67 - BIT 4, A
	d_cbOpcodes[0x67] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->A(); }, 4);
		return OpcodeResultContext::Builder(0x67).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x68 - BIT 5, B
	d_cbOpcodes[0x68] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->B(); }, 5);
		return OpcodeResultContext::Builder(0x68).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x69 - BIT 5, C
	d_cbOpcodes[0x69] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->C(); }, 5);
		return OpcodeResultContext::Builder(0x69).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6A - BIT 5, D
	d_cbOpcodes[0x6A] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->D(); }, 5);
		return OpcodeResultContext::Builder(0x6A).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6B - BIT 5, E
	d_cbOpcodes[0x6B] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->E(); }, 5);
		return OpcodeResultContext::Builder(0x6B).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6C - BIT 5, H
	d_cbOpcodes[0x6C] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->H(); }, 5);
		return OpcodeResultContext::Builder(0x6C).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6D - BIT 5, L
	d_cbOpcodes[0x6D] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->L(); }, 5);
		return OpcodeResultContext::Builder(0x6D).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6E - BIT 5, (HL)
	d_cbOpcodes[0x6E] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 5);
		return OpcodeResultContext::Builder(0x6E).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x6F - BIT 5, A
	d_cbOpcodes[0x6F] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->A(); }, 5);
		return OpcodeResultContext::Builder(0x6F).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x70 - BIT 6, B
	d_cbOpcodes[0x70] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->B(); }, 6);
		return OpcodeResultContext::Builder(0x70).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x71 - BIT 6, C
	d_cbOpcodes[0x71] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->C(); }, 6);
		return OpcodeResultContext::Builder(0x71).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x72 - BIT 6, D
	d_cbOpcodes[0x72] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->D(); }, 6);
		return OpcodeResultContext::Builder(0x72).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x73 - BIT 6, E
	d_cbOpcodes[0x73] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->E(); }, 6);
		return OpcodeResultContext::Builder(0x73).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x74 - BIT 6, H
	d_cbOpcodes[0x74] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->H(); }, 6);
		return OpcodeResultContext::Builder(0x74).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x75 - BIT 6, L
	d_cbOpcodes[0x75] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->L(); }, 6);
		return OpcodeResultContext::Builder(0x75).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x76 - BIT 6, (HL)
	d_cbOpcodes[0x76] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 6);
		return OpcodeResultContext::Builder(0x76).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x77 - BIT 6, A
	d_cbOpcodes[0x77] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->A(); }, 6);
		return OpcodeResultContext::Builder(0x77).ShortCycle().IncrementPCDefault().Build();;
	};


	// 0x78 - BIT 7, B
	d_cbOpcodes[0x78] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->B(); }, 7);
		return OpcodeResultContext::Builder(0x78).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x79 - BIT 7, C
	d_cbOpcodes[0x79] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->C(); }, 7);
		return OpcodeResultContext::Builder(0x79).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7A - BIT 7, D
	d_cbOpcodes[0x7A] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->D(); }, 7);
		return OpcodeResultContext::Builder(0x7A).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7B - BIT 7, E
	d_cbOpcodes[0x7B] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->E(); }, 7);
		return OpcodeResultContext::Builder(0x7B).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7C - BIT 7, H
	d_cbOpcodes[0x7C] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->H(); }, 7);
		return OpcodeResultContext::Builder(0x7C, true).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7D - BIT 7, L
	d_cbOpcodes[0x7D] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->L(); }, 7);
		return OpcodeResultContext::Builder(0x7D).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7E - BIT 7, (HL)
	d_cbOpcodes[0x7E] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_ram->readByte(d_regs->HL()); }, 7);
		return OpcodeResultContext::Builder(0x7E).ShortCycle().IncrementPCDefault().Build();;
	};
	// 0x7F - BIT 7, A
	d_cbOpcodes[0x7F] = [this]() {
		OpcodeUtils::checkBit(*d_regs, [this]() { return d_regs->A(); }, 7);
		return OpcodeResultContext::Builder(0x7F).ShortCycle().IncrementPCDefault().Build();;
	};

	// 0x10 - RL B
	d_cbOpcodes[0x10] = [this]() {
		OpcodeUtils::fullCircularRotateLeft(*d_regs, *d_ram,
							[this](ByteType t) { d_regs->B(t); },
							[this]() { return d_regs->B(); });
		return OpcodeResultContext::Builder(0x10, true).ShortCycle().IncrementPCDefault().Build();
	};

	// 0x11 - RL C
	d_cbOpcodes[0x11] = [this]() {
		OpcodeUtils::fullCircularRotateLeft(*d_regs, *d_ram,
							[this](ByteType t) { d_regs->C(t); },
							[this]() { return d_regs->C(); });
		return OpcodeResultContext::Builder(0x11, true).ShortCycle().IncrementPCDefault().Build();
	};
}


}