// GBEmulator_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "gtest/gtest.h"
#include "CPU.h"
#include "Opcodes.h"
#include "CPUTest.hpp"
#include "OpcodesTest.hpp"

TEST_F(CPUTest, CPUFetchEmptyROM)
{
	this->gbcpu.fetch_and_decode();
}

TEST_F(OpcodesTest, SetZeroFlagOn)
{
	this->gbcpu.set_zero_flag(true);
	ASSERT_TRUE(this->gbcpu.is_zero_flag_set());
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
    return 0;
}

