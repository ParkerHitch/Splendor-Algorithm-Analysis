//
// Created by Parker Hitchcock on 11/14/22.
//

#ifndef RESEARCH_GAMESETUP_H
#define RESEARCH_GAMESETUP_H

#include <fstream>
#include "gameState.h"

using namespace std;

Card cardFromLine(const string& line, int id);
Noble nobleFromLine(const string& line, int id);

GameState newGameFromFiles(ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n);
GameState newGameFromFilesShuffled(ifstream &d1, ifstream &d2, ifstream &d3, ifstream &n);

#endif //RESEARCH_GAMESETUP_H
