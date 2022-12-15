//
// Created by Parker Hitchcock on 12/14/22.
//

#include "Math.h"

int factorial(int a){
    return a==0?1:a * factorial(a-1);
}