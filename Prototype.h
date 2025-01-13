#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Prototype {
private:
    std::unordered_map<char, std::vector<std::string>> cityMap;
    std::unordered_set<std::string> usedCities;

public:
    // Constructor
    Prototype(const std::string& filename);

    // Get a valid city starting with the provided letter
    std::string getCity(char firstLetter);

    // Check if a city is valid and unused
    bool checkCity(const std::string& city);

    // Mark a city as used
    void markUsed(const std::string& city);

    // Reset the used cities
    void restart();
};