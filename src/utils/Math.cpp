//
// Created by Parker Hitchcock on 12/14/22.
//

#include "Math.h"
#include <iostream>
#include <random>

int factorial(int a){
    return a==0?1:a * factorial(a-1);
}

int randRange(int lower, int upper){
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(lower, upper); // define the range
    return dstr(gen);
}