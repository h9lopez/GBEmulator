#include "gb_cpucore_opcode_utils.h"


// This namespace will contain generic helper functions that don't modify any
// register/cpu state
namespace Core {
namespace OpcodeUtils {
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

	void fullRotateLeftCarry(RegBank &regs,
								std::function<void(ByteType)> regSet,
								std::function<ByteType(void)> regGet)
	{
		const ByteType target = regGet();
		ByteType highbit = (target & 0x80) >> 7;
		ByteType result = circularRotateLeft(target);
		
		regs.flagZero((result == 0));
		regs.flagCarry(highbit);
		regs.flagSubtract(false);
		regs.flagHalfCarry(false);
		regSet(result);
	}
} }