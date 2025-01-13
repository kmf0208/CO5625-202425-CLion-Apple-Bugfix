#pragma once
#include "Prototype.h"

#define Competition_class Prototype  //change to Competition once you've implemented your Competition class, defaults to Prototype

class Competition : public Competition_class {
public:
    // Constructor declaration with noexcept to match the definition in the cpp file
    Competition() noexcept;
    
    // You can add competition-specific methods and functionality here
};