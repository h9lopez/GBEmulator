TEST_F(CPUOpcodeTest, opcode_test_{test_name})
{{
    SetUp({{ {romData} }});

{preRegSets}
{preMemSets}
{preFlagSets}

{preRegAsserts}
{preMemAsserts}

    cpu.cycle();

    RegBank after;
    RAM memAfter;
    regs.copyCurrentState(after);
    mem.copyCurrentState(memAfter);

    ASSERT_EQ(cpu.cycleCount(), {cycleAmount});

{flagAsserts}
{regAsserts}
{memAsserts}
}}