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
    std::uniform_int_distribution<> distr(lower, upper-1); // define the range
    return distr(gen);
}

unsigned int rightmostSetBitPos(unsigned int n){
    if (n == 0)
        return 0;
    // Position variable initialize with 1
    // m variable is used to check the set bit
    int position = 0;
    int m = 1;

    while (!(n & m)) {

        // left shift
        m = m << 1;
        position++;
    }
    return position;
}
unsigned int leftmostSetBitPos(unsigned int n) {
    unsigned int v = 0; // 32-bit word to find the log base 2 of
    unsigned int r = 0; // r will be lg(v)

    while (v >>= 1)
    {
        r++;
    }
    return r;
}