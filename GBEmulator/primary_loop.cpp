#include <iostream>
#include "Gameboy.h"
using namespace std;

int main(void)
{
	// Initialize datamem and registers

	// Load ROM into memspace

	Gameboy gb;
	auto res = gb.loadROM("C:\\Users\\hlope\\Downloads\\dummy.bin");
	if (res)
	{
		cout << "ROM loaded successfully\n";
	}
	else {
		cout << "FAILED ROM LOADING\n";
	}

	gb.beginExec();

	getchar();
}