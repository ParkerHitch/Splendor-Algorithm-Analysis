//
// Created by Parker Hitchcock on 11/9/22.
//

#ifndef RESEARCH_GAMEOUTPUT_H
#define RESEARCH_GAMEOUTPUT_H

#include <vector>
#include "../game/gameDependencies.h"

using namespace std;

void printC(Card* c);
void printN(Noble* n);
void printBank(GameState* gs);
void printPS(PlayerState& ps);
void printGS(GameState* gs);

void printAction(GameAction& pa);
void printActions(vector<GameAction>& actions);
void printTake3(GameAction& T3);
void printTake2(GameAction& T2);

#endif //RESEARCH_GAMEOUTPUT_H
