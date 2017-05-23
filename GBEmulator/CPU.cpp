#include "CPU.h"
#include "ReverseOpcodeMap.h"
#include <iostream>
using namespace std;

#define READ_MEM_BYTE(REG) ((mem[REG.word]))

CPU::CPU()
{
	this->regs.PC.word = 0;
}

CPU::CPU(uint8_t *memLoc, int memSize)
{
	if (memLoc == nullptr)
	{
		cout << "ERROR: ROM MEMORY POINTER NULL" << endl;
	}
	this->mem = memLoc;
	this->mem_size = memSize;
	this->regs.PC.word = 0;
}

void CPU::setMemoryLocation(uint8_t * memLoc, int memSize)
{
	if (memLoc == nullptr)
	{
		cout << "ERROR: ROM MEMORY POINTER NULL" << endl;
	}
	this->mem = memLoc;
	this->mem_size = memSize;
}

inline uint8_t CPU::READ_ROM_BYTE()
{
	return mem[regs.PC.word++];
}

inline uint16_t CPU::READ_ROM_2BYTE()
{
	uint16_t res = ((uint16_t*)(mem + regs.PC.word))[0];
	regs.PC.word += 2;
	return res;
}

inline void CPU::WRITE_MEM_BYTE(Register addr, uint8_t val)
{
	mem[addr.word] = val;
}

inline void CPU::INC_CYCLE_COUNT(unsigned int num)
{
	this->cycle_count += num;
}


// ============================================================================
// ============================================================================
// ============================================================================
// ===========================OPCODE HANDLERS==================================
// ============================================================================
// ============================================================================
// ============================================================================
inline void CPU::reg_store_immediate(Register *reg, unsigned int data)
{
	reg->lo = FIRST8_MASK(data);
	reg->hi = SECOND8_MASK(data);
}

inline void CPU::check_bit(uint8_t val, unsigned int bitn)
{
	// function will check bits and set appropriate flags
	// bitn should be between 0-7
	// val is the value held inside an 8-bit register or memory location
	bool is_on = (val >> bitn) & 0x1;
	this->set_zero_flag(!is_on);
	this->set_subtract_flag(false);
	this->set_half_carry_flag(true);
}

inline void CPU::opcode_handle_xor(uint8_t regval)
{
	this->regs.A.lo = (regval) ^ this->regs.A.lo;
	this->set_zero_flag(this->regs.A.lo == 0x0);
}


// ============================================================================
// =======================END HANDLERS=========================================
// ============================================================================


void CPU::cpu_cycle()
{
	// Grab an opcode first
	uint8_t opcode = 0x76;
	
	opcode = this->mem[this->regs.PC.word++];
	cout << hex << "OPCODE: " << static_cast<int>(opcode) << endl;
	cout << "\tINSTR: " << GENERATED_MAIN_INSTRUCTION_NAMES[opcode] << endl;
	// Switch statement into the proper instruction handler
	switch (opcode)
	{
		// JR Jump Relative
		case 0x20: // JR NZ, r8
			if (!is_zero_flag_set()) {
				// Jump
				regs.PC.word += static_cast<int8_t>(READ_ROM_BYTE());
				INC_CYCLE_COUNT(12);
			}
			else {
				INC_CYCLE_COUNT(8);
				regs.PC.word++; // Skip reading the PC offset since we're not taking the branch
			}
			break;
		case 0x30: // JR NC, r8
			if (!is_carry_flag_set()) {
				regs.PC.word += READ_ROM_BYTE();
				INC_CYCLE_COUNT(12);
			}
			else {
				INC_CYCLE_COUNT(8);
				regs.PC.word++;
			}
			break;
		case 0x18: // JR r8	   
			regs.PC.word += READ_ROM_BYTE();
			INC_CYCLE_COUNT(12);
			break;
		case 0x28: // JR Z, r8 
			if (is_zero_flag_set()) {
				regs.PC.word += READ_ROM_BYTE();
				INC_CYCLE_COUNT(12);
			}
			else {
				INC_CYCLE_COUNT(8);
				regs.PC.word++;
			}
			break;
		case 0x38: // JR C, r8 
			if (is_carry_flag_set()) {
				regs.PC.word += READ_ROM_BYTE();
				INC_CYCLE_COUNT(12);
			}
			else {
				INC_CYCLE_COUNT(8);
				regs.PC.word++;
			}
			break;



		// 16-bit immediate load into 16-bit regs
		case 0x01:
			reg_store_immediate(&this->regs.BC, READ_ROM_2BYTE());
			INC_CYCLE_COUNT(12);
			break;
		case 0x11:
			reg_store_immediate(&this->regs.DE, READ_ROM_2BYTE());
			INC_CYCLE_COUNT(12);
			break;
		case 0x21:
			reg_store_immediate(&this->regs.HL, READ_ROM_2BYTE());
			INC_CYCLE_COUNT(12);
			break;
		case 0x31:
			reg_store_immediate(&this->regs.SP, READ_ROM_2BYTE());
			INC_CYCLE_COUNT(12);
			break;

		// XOR instructions
		case 0xA8:
			opcode_handle_xor(this->regs.BC.hi);
			INC_CYCLE_COUNT(4);
			break;
		case 0xA9:
			opcode_handle_xor(this->regs.BC.lo);
			INC_CYCLE_COUNT(4);
			break;
		case 0xAA:
			opcode_handle_xor(this->regs.DE.hi);
			INC_CYCLE_COUNT(4);
			break;
		case 0xAB:
			opcode_handle_xor(this->regs.DE.lo);
			INC_CYCLE_COUNT(4);
			break;
		case 0xAC:
			opcode_handle_xor(this->regs.HL.hi);
			INC_CYCLE_COUNT(4);
			break;
		case 0xAD:
			opcode_handle_xor(this->regs.HL.lo);
			INC_CYCLE_COUNT(4);
			break;
		case 0xAE:
			opcode_handle_xor(READ_MEM_BYTE(regs.HL));
			INC_CYCLE_COUNT(8);
			break;
		case 0xAF:
			opcode_handle_xor(this->regs.A.lo);
			INC_CYCLE_COUNT(4);
			break;

		// LD (16REG), A
		case 0x02:
			WRITE_MEM_BYTE(regs.BC, regs.A.lo);
			INC_CYCLE_COUNT(8);
			break;
		case 0x12:
			WRITE_MEM_BYTE(regs.DE, regs.A.lo);
			INC_CYCLE_COUNT(8);
			break;
		case 0x22:
			WRITE_MEM_BYTE(regs.HL, regs.A.lo);
			regs.HL.word++;
			INC_CYCLE_COUNT(8);
			break;
		case 0x32:
			WRITE_MEM_BYTE(regs.HL, regs.A.lo);
			regs.HL.word--;
			INC_CYCLE_COUNT(8);
			break;

		// LD REG, d8 instructions 
		case 0x06:
			regs.BC.hi = READ_ROM_BYTE();
			INC_CYCLE_COUNT(8);
			break;
		case 0x16:
			regs.DE.hi = READ_ROM_BYTE();
			INC_CYCLE_COUNT(8);
			break;
		case 0x26:
			regs.HL.hi = READ_ROM_BYTE();
			INC_CYCLE_COUNT(8);
			break;
		case 0x36:
			WRITE_MEM_BYTE(regs.HL, READ_ROM_BYTE());
			INC_CYCLE_COUNT(12);
			break;
		case 0x0E:
			regs.BC.lo = READ_ROM_BYTE();
			INC_CYCLE_COUNT(8);
			break;
		case 0x1E:
			regs.DE.lo = READ_ROM_BYTE();
			INC_CYCLE_COUNT(8);
			break;
		case 0x2E:
			regs.HL.lo = READ_ROM_BYTE();
			INC_CYCLE_COUNT(8);
			break;
		case 0x3E:
			regs.A.lo = READ_ROM_BYTE();
			INC_CYCLE_COUNT(8);
			break;

		// Pretty big section. All the CB-prefixed instructions
		case 0xCB:
			handle_cb_prefix();
			break;



		// HALT Instruction
		case 0x76:
			INC_CYCLE_COUNT(4);
			cout << "CPU Power Down" << endl;
			exit(0);

		default:
			cout << "UNKNOWN OPCODE, exiting" << endl;
			cout << "===OPCODE IS: " << GENERATED_MAIN_INSTRUCTION_NAMES[opcode] << endl;
			cout << dec << "===PC IS: " << regs.PC.word << endl;
			cout << hex << "===Real Opcode Hex: " << static_cast<int>(opcode) << endl;
			getchar();
			exit(1);
			break;
	}

	
}


void CPU::handle_cb_prefix()
{
	uint8_t next_op = mem[regs.PC.word++];

	cout << "\tREAL INSTR: " << GENERATED_CB_INSTRUCTION_NAMES[next_op] << endl;

	// ALL CB-prefixed instructions take at LEAST 8 cycles, add 8 more if it's 16 
	INC_CYCLE_COUNT(8);

	switch (next_op)
	{

		// BIT check instructions
		case 0x40:
			check_bit(regs.BC.hi, 0);
			break;
		case 0x41:
			check_bit(regs.BC.lo, 0);
			break;
		case 0x42:
			check_bit(regs.DE.hi, 0);
			break;
		case 0x43:
			check_bit(regs.DE.lo, 0);
			break;
		case 0x44:
			check_bit(regs.HL.hi, 0);
			break;
		case 0x45:
			check_bit(regs.HL.lo, 0);
			break;
		case 0x46:
			check_bit(READ_MEM_BYTE(regs.HL), 0);
			INC_CYCLE_COUNT(8);
			break;
		case 0x47:
			check_bit(regs.A.lo, 0);
			break;
		case 0x48:
			check_bit(regs.BC.hi, 1);
			break;
		case 0x49:
			check_bit(regs.BC.lo, 1);
			break;
		case 0x4A:
			check_bit(regs.DE.hi, 1);
			break;
		case 0x4B:
			check_bit(regs.DE.lo, 1);
			break;
		case 0x4C:
			check_bit(regs.HL.hi, 1);
			break;
		case 0x4D:
			check_bit(regs.HL.lo, 1);
			break;
		case 0x4E:
			check_bit(READ_MEM_BYTE(regs.HL), 1);
			INC_CYCLE_COUNT(8);
			break;
		case 0x4F:
			check_bit(regs.A.lo, 1);
			break;

		case 0x50:
			check_bit(regs.BC.hi, 2);
			break;
		case 0x51:
			check_bit(regs.BC.lo, 2);
			break;
		case 0x52:
			check_bit(regs.DE.hi, 2);
			break;
		case 0x53:
			check_bit(regs.DE.lo, 2);
			break;
		case 0x54:
			check_bit(regs.HL.hi, 2);
			break;
		case 0x55:
			check_bit(regs.HL.lo, 2);
			break;
		case 0x56:
			check_bit(READ_MEM_BYTE(regs.HL), 2);
			INC_CYCLE_COUNT(8);
			break;
		case 0x57:
			check_bit(regs.A.lo, 2);
			break;
		case 0x58:
			check_bit(regs.BC.hi, 3);
			break;
		case 0x59:
			check_bit(regs.BC.lo, 3);
			break;
		case 0x5A:
			check_bit(regs.DE.hi, 3);
			break;
		case 0x5B:
			check_bit(regs.DE.lo, 3);
			break;
		case 0x5C:
			check_bit(regs.HL.hi, 3);
			break;
		case 0x5D:
			check_bit(regs.HL.lo, 3);
			break;
		case 0x5E:
			check_bit(READ_MEM_BYTE(regs.HL), 3);
			INC_CYCLE_COUNT(8);
			break;
		case 0x5F:
			check_bit(regs.A.lo, 3);
			break;

		case 0x60:
			check_bit(regs.BC.hi, 4);
			break;
		case 0x61:
			check_bit(regs.BC.lo, 4);
			break;
		case 0x62:
			check_bit(regs.DE.hi, 4);
			break;
		case 0x63:
			check_bit(regs.DE.lo, 4);
			break;
		case 0x64:
			check_bit(regs.HL.hi, 4);
			break;
		case 0x65:
			check_bit(regs.HL.lo, 4);
			break;
		case 0x66:
			check_bit(READ_MEM_BYTE(regs.HL), 4);
			INC_CYCLE_COUNT(8);
			break;
		case 0x67:
			check_bit(regs.A.lo, 4);
			break;
		case 0x68:
			check_bit(regs.BC.hi, 5);
			break;
		case 0x69:
			check_bit(regs.BC.lo, 5);
			break;
		case 0x6A:
			check_bit(regs.DE.hi, 5);
			break;
		case 0x6B:
			check_bit(regs.DE.lo, 5);
			break;
		case 0x6C:
			check_bit(regs.HL.hi, 5);
			break;
		case 0x6D:
			check_bit(regs.HL.lo, 5);
			break;
		case 0x6E:
			check_bit(READ_MEM_BYTE(regs.HL), 5);
			INC_CYCLE_COUNT(8);
			break;
		case 0x6F:
			check_bit(regs.A.lo, 5);
			break;

		case 0x70:
			check_bit(regs.BC.hi, 6);
			break;
		case 0x71:
			check_bit(regs.BC.lo, 6);
			break;
		case 0x72:
			check_bit(regs.DE.hi, 6);
			break;
		case 0x73:
			check_bit(regs.DE.lo, 6);
			break;
		case 0x74:
			check_bit(regs.HL.hi, 6);
			break;
		case 0x75:
			check_bit(regs.HL.lo, 6);
			break;
		case 0x76:
			check_bit(READ_MEM_BYTE(regs.HL), 6);
			INC_CYCLE_COUNT(8);
			break;
		case 0x77:
			check_bit(regs.A.lo, 6);
			break;
		case 0x78:
			check_bit(regs.BC.hi, 7);
			break;
		case 0x79:
			check_bit(regs.BC.lo, 7);
			break;
		case 0x7A:
			check_bit(regs.DE.hi, 7);
			break;
		case 0x7B:
			check_bit(regs.DE.lo, 7);
			break;
		case 0x7C:
			check_bit(regs.HL.hi, 7);
			break;
		case 0x7D:
			check_bit(regs.HL.lo, 7);
			break;
		case 0x7E:
			check_bit(READ_MEM_BYTE(regs.HL), 7);
			INC_CYCLE_COUNT(8);
			break;
		case 0x7F:
			check_bit(regs.A.lo, 7);
			break;


	}
}

//// NOTE: fully_opcode parameter can be multi-byte depending on the length of the instruction
//void CPU::decode_and_exec(uint8_t leading_opcode, unsigned int full_opcode)
//{
//	int instr_len;
//	opcode_handle_func handler;
//
//	// Grab the opcode handler associated with the leading opcode
//	try
//	{
//		std::tie(instr_len, handler) = global_opcode_vtable.at(leading_opcode);
//	}
//	catch (std::out_of_range &) {
//		cout << hex << "UNSUPPORTED OPCODE 0x" << static_cast<int>(leading_opcode) << endl;
//		// TODO: Throw global exception to stop CPU?
//		return;
//	}
//
//
//	// Call the handler
//	handler(full_opcode);
//
//}
//
//unsigned int CPU::fetch_next()
//{
//	int instr_len;
//	opcode_handle_func handler;
//	unsigned int encoded_args = 0;
//
//	// TODO: Use map to decide which instructions are 1/2/3 bytes long and read the proper amount
//	uint8_t opcode = this->mem[this->regs.PC.word++];
//	
//	cout << hex << "Read opcode 0x" << static_cast<int>(opcode) << endl;
//	cout << "\tLooking for opcode handler in vtable" << endl;
//	// Vector into the table to get function handler and size of full instruction (in case we need to fetch 1 or 2 more bytes before exec)
//	try
//	{
//		std::tie(instr_len, handler) = global_opcode_vtable.at(opcode);
//	} catch (std::out_of_range &) {
//		cout << hex << "UNSUPPORTED OPCODE 0x" << static_cast<int>(opcode) << endl;
//		// TODO: Throw global exception to stop CPU?
//		return 0x0;
//	}
//
//	// Read the full amount of bytes necessary for instruction and encode them as arguments in single integer
//	// TODO: Decouple instruction lengths from the global opcode vtable? Double access in here and
//	//		 then again in decode_and_execute()
//	encoded_args = opcode;
//	instr_len--;
//	while (instr_len > 0)
//	{
//		encoded_args <<= 8;
//		uint8_t next_byte = this->mem[this->regs.PC.word++];
//		encoded_args |= next_byte;
//		instr_len--;
//	}
//
//	cout << hex << "\tEncoded args: " << encoded_args << endl;
//	return encoded_args;
//}

