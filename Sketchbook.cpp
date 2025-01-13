#include "Prototype.h"
#include <iostream>

int main() {
    // Test case 1: Create a Prototype instance from a file (example: cities.txt)
    Prototype game("cities.txt");

    // Test case 2: Get a city that starts with a specific letter
    std::string city = game.getCity('p');  // For example, a city starting with 'p'
    std::cout << "City starting with 'p': " << city << std::endl;

    // Test case 3: Check if a city is valid and unused
    bool isValid = game.checkCity("Paris");
    std::cout << "Is Paris a valid and unused city? " << (isValid ? "Yes" : "No") << std::endl;

    // Test case 4: Mark a city as used and try to get it again
    game.markUsed("Paris");
    isValid = game.checkCity("Paris");
    std::cout << "Is Paris a valid and unused city after marking it as used? " 
              << (isValid ? "Yes" : "No") << std::endl;

    // Test case 5: Try to get another city starting with the same letter
    city = game.getCity('p');
    std::cout << "Another city starting with 'p': " << city << std::endl;

    // Test case 6: Restart the game (mark all cities as unused)
    game.restart();
    isValid = game.checkCity("Paris");
    std::cout << "Is Paris a valid city after restarting? " << (isValid ? "Yes" : "No") << std::endl;

    return 0;
}