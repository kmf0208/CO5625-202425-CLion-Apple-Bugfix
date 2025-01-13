#include "Prototype.h"

// Constructor: Reads the city names from the given file and stores them in the cityMap
Prototype::Prototype(const std::string& filename) {
    std::ifstream file(filename);
    std::string city;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Read and populate the city map
    while (std::getline(file, city)) {
        char firstLetter = std::tolower(city[0]);
        cityMap[firstLetter].push_back(city);
    }

    file.close();
}

// getCity: Returns a valid, unused city that starts with the given letter
std::string Prototype::getCity(char firstLetter) {
    firstLetter = std::tolower(firstLetter);

    if (cityMap.find(firstLetter) != cityMap.end()) {
        for (auto& city : cityMap[firstLetter]) {
            if (usedCities.find(city) == usedCities.end()) {
                return city;
            }
        }
    }

    return "";  // If no unused city is found, return an empty string
}

// checkCity: Verifies if the city exists and is unused
bool Prototype::checkCity(const std::string& city) {
    char firstLetter = std::tolower(city[0]);

    return cityMap.find(firstLetter) != cityMap.end() &&
           std::find(cityMap[firstLetter].begin(), cityMap[firstLetter].end(), city) != cityMap[firstLetter].end() &&
           usedCities.find(city) == usedCities.end();
}

// markUsed: Marks the city as used by inserting it into the usedCities set
void Prototype::markUsed(const std::string& city) {
    usedCities.insert(city);
}

// restart: Resets all cities to unused by clearing the usedCities set
void Prototype::restart() {
    usedCities.clear();
}