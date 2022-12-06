//
// Created by Parker Hitchcock on 11/9/22.
//

#ifndef RESEARCH_GAMEOUTPUT_H
#define RESEARCH_GAMEOUTPUT_H

#include "../game/gameState.h"
#include "../game/gameAction.h"

void printC(Card* c);
void printN(Noble& n);
void printBank(GameState& gs);
void printPS(PlayerState& ps);
void printGS(GameState& gs);

void printAction(GameAction& pa);

#endif //RESEARCH_GAMEOUTPUT_H
