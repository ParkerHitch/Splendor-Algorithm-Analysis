//
// Created by Parker Hitchcock on 1/8/23.
//
#include <fstream>

#include "gameData.h"

gameData::gameData(std::ifstream &d1, std::ifstream &d2, std::ifstream &d3, std::ifstream &n) {
        std::string line;
        getline(d1, line);//Skip comment
        int id = 0;
        while (id<40){
            getline(d1, line);
            deck1[id] = Card::fromLine(line,id);
            id++;
        }

        getline(d2, line);//Skip comment
        while (id<70){
            getline(d2, line);
            deck2[id-40] = Card::fromLine(line,id);
            id++;
        }

        getline(d3, line);//Skip comment
        while (id<90){
            getline(d3, line);
            deck3[id-70] = Card::fromLine(line,id);
            id++;
        }

        id = 0;
        getline(n, line);//Skip comment
        while (id<10){
            getline(n, line);
            nobles[id] = Noble::fromLine(line, id);
            id++;
        }
}