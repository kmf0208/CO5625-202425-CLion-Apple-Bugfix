#include <fstream>
#include <filesystem>
#include <iostream>
#include <list>
#include <string>
#include <regex>

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

namespace fs = std::filesystem;

struct stringDistance {
	std::string reference;
	stringDistance(std::string ref) : reference(ref) {}
	int distance(const std::string& a, const std::string& b) {
		int result = std::labs(a.size() - b.size()) * 1;
		auto i = a.begin();
		auto j = b.begin();
		for (; i != a.end() && j != b.end(); ++i, ++j)
			result += labs(*i - *j);
		return result;
	}
	bool operator () (const std::string& lhs, const std::string& rhs) { return distance(lhs, reference) < distance(rhs, reference); }
};

int main() {
#ifdef _WIN32
	setConsoleVTMode();
#endif
	std::list<std::string> files;
	for (const auto& entry : fs::directory_iterator("../")) {
		if(entry.path().extension() == ".h" || entry.path().extension() == ".cpp" || entry.path().extension() == ".hpp")
			files.push_back(entry.path().filename().string());
	}
	files.sort(stringDistance("Prototype.h"));
	std::regex re("[JKjk][0-9]{5}[0-9]?");
	std::string Jnum = "  ";
	while (!std::regex_match(Jnum, re)) {
		std::cout << "Enter your J number (eg J12345): ";
		std::cin >> Jnum;
	}
	std::ofstream ofs(std::string("") + Jnum + ".txt");
	std::string temp;
	for (auto i : files) {
		std::ifstream ifs(std::string("../") + i);
		ofs << "\n--------------------------------------------";
		ofs << i;
		ofs << "--------------------------------------------\n\n";
		while (getline(ifs, temp, '\n')) {
			ofs << temp << '\n';
		}
		ifs.close();
	}
	ofs.close();

	std::cout << "\033[2J\033[H\n\nYour file is named " << Jnum << ".txt and can be found here (paste into explorer): \n\n";
	std::cout << "\t\033[33m" << std::filesystem::current_path().string() << "\033[0m\n";
	std::cout << "\nYou should submit this file to Turnitin \033[31mexactly\033[0m as written by this program\n\n";
	std::cout << "\033[90m\t(press enter to quit)\n";

	std::cin.ignore(1000, '\n');
	std::cin.get();
	return 0;
}