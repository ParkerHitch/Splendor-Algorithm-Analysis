//
// Created by Parker Hitchcock on 11/3/22.
//
#ifndef RESEARCH_GAME_H
#define RESEARCH_GAME_H

using namespace std;

#include "gameState.h"
#include "gameAction.h"

void startGame(ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n);
void takeTurn();
void applyAction(GameAction& ga);
bool validAction(GameAction& ga);

bool flipCard(GameState& gs, int dNum, int newPos);
bool flipNoble(GameState& gs, int newPos);

#endif //RESEARCH_GAME_H
