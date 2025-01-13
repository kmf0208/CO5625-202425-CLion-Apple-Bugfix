#include "Game.hpp"
#include <iostream>

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

int main() {
	#ifdef _WIN32
		setConsoleVTMode();
	#endif
	srand(static_cast<unsigned int>(time(nullptr)));
	std::cout << "\n\n\n\033[30C\033[38;5;220mWelcome to Capital City Dominoes\033[0m\n\n\n";
	std::cout << "\033[15C\033[38;5;223m\033[14mIn each round you'll be asked to enter the name of a capital city\t\t\t\n";
	std::cout << "\033[15CThe computer will respond with a capital city ending with the last letter of yours\n";
	std::cout << "\033[15CYou must then enter a capital city beginning with the last letter of the computer's city\n";
	std::cout << "\033[15CThe game continues until one of you runs out of ideas. Repeat cities are not permitted\n\033[0m";
	std::cout << "\n\n\033[30C\033[38;5;220mPress any key to begin...\033[0m";
	std::cin.get();
	
	Game<Prototype> theGame("Cities.txt");
	while (theGame.round(std::cout, std::cin));

	std::cout << "\033[2J\033[H\n\n\n\033[20C" << (theGame.didIWin() ? "\033[38;5;120mCongratulations. You have won." : "\033[38;5;210mSadly, you have lost the game.");
	std::cout << "\033[0m";
	std::cin.get();
	return 0;
}