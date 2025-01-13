#include "../Prototype.h"
#include "../Competition.h"
#include "PrototypeTests.hpp"
#include "SimulatorTests.hpp"
#include <iostream>
#include <algorithm>

#ifdef _WIN32
#include <Windows.h>
void setConsoleVTMode() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hConsole, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, dwMode);
}
#endif

int main()
{
#ifdef _WIN32
	setConsoleVTMode();
#endif
	srand(static_cast<unsigned int>(time(nullptr)));
#ifndef ASSIGNMENT_2
	int score = doPrototypeTests(std::cout);
	std::cout << "\n\033[0;1;4mScore\t\t\033[33;1;0m" << score << "\033[0m/70\n\n\n";
#else
	doPrototypeTests(std::cout);
	int score = doSimulatorTests(std::cout);
	std::cout << "\n\033[0;1;4mScore\t\t\033[33;1;0m" << score << "\033[0m/40\n\n\n";
#endif
}