TEST_F(CPUOpcodeTest, opcode_test_{test_name})
{{
    SetUp({{ {romData} }});

    cpu.cycle();

    RegBank after;
    regs.copyCurrentState(after);

    ASSERT_EQ(cpu.cycleCount(), {cycleAmount});

{flagAsserts}
{regAsserts}
}}