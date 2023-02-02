//
// Created by Parker Hitchcock on 1/8/23.
//

#ifndef RESEARCH_GAMEDATA_H
#define RESEARCH_GAMEDATA_H

#include <fstream>
#include "gameDependencies.h"

struct Card;
struct Noble;

struct gameData{
    gameData(std::ifstream &d1, std::ifstream &d2, std::ifstream &d3, std::ifstream &n);

    Card deck1[40]={};
    Card deck2[30]={};
    Card deck3[20]={};
    Noble nobles[10]={};
};

#endif //RESEARCH_GAMEDATA_H
