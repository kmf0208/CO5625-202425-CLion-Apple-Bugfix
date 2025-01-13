#pragma once
#include "../Prototype.h"
#include "Requirements.h"
#include <iostream>
#include <sstream>
#include <algorithm>

template<class T>
class Game
{
	T c;
	bool won;
	std::string last;
	static std::string capitalise(std::string s);
public:
	Game(std::string file) : c(file), won(false), last("") { last.push_back((rand() % 23) + 'a'); }
	bool round(std::ostream& o, std::istream& i);
	bool didIWin() { return won; }
};

//--------------------------------------implementation code---------------------------------------
template<class T>
bool Game<T>::round(std::ostream& o, std::istream& i) {
	o << "\033[2J\033[H\n\n\033[20C\033[33mMachine's choice is: " << capitalise(last) << "\n";
	o << "\n\n\033[20C\033[32mEnter a capital city" << " beginning with " << last.back();
	o << "\n\033[20C";
	std::string city;
	getline(i, city);
	std::transform(city.begin(), city.end(), city.begin(), [](unsigned char c) {return std::tolower(c); });
	if (city.empty() || city.front() != last.back() || !c.checkCity(city))
		return false;
	c.markUsed(city);
	city = c.getCity(city.back());	
	if (city.empty()) {
		won = true;
		return false;
	}
	last = city;
	c.markUsed(city);
	return true;
}

template <class T>
std::string Game<T>::capitalise(std::string s){
	std::stringstream ss(s);
	std::string temp;
	std::list<std::string> words;
	while (std::getline(ss, temp, ' ')) {
		if (temp[0] < 'A' || temp[0] > 'Z')
			temp[0] += ('A' - 'a');
		words.push_back(temp);
	}
	std::stringstream result;
	for (auto i : words)
		result << i << " ";
	return result.str();
}
