#ifndef INCLUDE_GB_CPUCODE_OPCODE_UTILS_H
#define INCLUDE_GB_CPUCODE_OPCODE_UTILS_H

#include <bitset>

#include <registers/gb_regs.h>
#include <ram/gb_ram.h>
#include <utils/gb_typeutils.h>


// This namespace will contain generic helper functions that don't modify any
// register/cpu state
namespace Core {
namespace OpcodeUtils {
	ByteType circularRotateLeft(ByteType in);

	ByteType circularRotateRight(ByteType in);

	void incrementRegister(RegBank &regs,
		std::function<void(ByteType)> regSet,
		std::function<ByteType(void)> regGet);

	void decrementRegister(RegBank &regs,
		std::function<void(ByteType)> regSet,
		std::function<ByteType(void)> regGet);

	void xorRegister(RegBank &regs,
		std::function<ByteType(void)> regGet);

	void checkBit(RegBank &regs,
				  std::function<ByteType(void)> regGet,
				  unsigned int bitn);

	void loadIntoHL(RegBank &regs,
					std::function<WordType(void)> valGet);

	void loadByteIntoRegister(std::function<void(ByteType)> regSet,
							std::function<ByteType(void)> valGet);

	void performByteAdd(RegBank &regs,
						std::function<void(ByteType)> regSetA,
						std::function<ByteType(void)> regGetA,
						std::function<ByteType(void)> regGetB);

	void performByteSub(RegBank &regs,
						std::function<void(ByteType)> regSetA,
						std::function<ByteType(void)> regGetA,
						std::function<ByteType(void)> regGetB);

	void performSBC(RegBank &regs,
		std::function<ByteType(void)> secondaryRegGet);

	void performADC(RegBank &regs,
		std::function<ByteType(void)> secondaryRegGet);

	void performAND(RegBank &regs,
		std::function<ByteType(void)> rhsGet);

	void performOR(RegBank &regs,
		std::function<ByteType(void)> rhsGet);

	void popSP(RegBank &regs, const RAM &ram, std::function<void(WordType)> rhsSet);

	void pushSP(RegBank &regs, RAM &ram, std::function<WordType()> rhsGet);

	void registerRst(RegBank &regs, RAM &ram, ByteType f_val);

	void performCall(RegBank &regs, RAM &ram, WordType address);

	// For use in RL opcodes
	void fullCircularRotateLeft(RegBank &regs, RAM &ram,
								std::function<void(ByteType)> regSet,
								std::function<ByteType(void)> regGet);

	void fullRotateLeftCarry(RegBank &regs,
								std::function<void(ByteType)> regSet,
								std::function<ByteType(void)> regGet);
} }

#endif