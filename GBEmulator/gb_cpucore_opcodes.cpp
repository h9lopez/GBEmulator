#include "CPUCore.h"


CPUCore::OpcodeContainer CPUCore::d_opcodes({
	{0xAB, []() { 1 + 2; return 1; } },
	{0xBC, []() { 3 + 4; return 1; } }
});


CPUCore::OpcodeContainer CPUCore::d_cbOpcodes({
	{0xCD, []() {4 + 5; return 1; } }
});