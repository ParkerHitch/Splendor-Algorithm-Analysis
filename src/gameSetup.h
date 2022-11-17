//
// Created by Parker Hitchcock on 11/14/22.
//

#ifndef RESEARCH_GAMESETUP_H
#define RESEARCH_GAMESETUP_H

#include "gameState.h"

using namespace std;

Card cardFromLine(const string& line, int id);

Noble nobleFromLine(const string& line, int id);

void loadAndShuffleDecks(GameState& gs, ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n);

#endif //RESEARCH_GAMESETUP_H
