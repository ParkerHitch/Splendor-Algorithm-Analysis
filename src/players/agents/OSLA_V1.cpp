//
// Created by Parker Hitchcock on 12/17/22.
//
#include "../Player.h"

float OSLA_V1::evaluate(GameState& gs){

    float score = 0;
    PlayerState& ps = gs.playerStates[id];
    score += (float)ps.discounts[0];
    score += (float)ps.discounts[1];
    score += (float)ps.discounts[2];
    score += (float)ps.discounts[3];
    score += (float)ps.discounts[4];
    return score;
}

GameAction OSLA_V1::takeAction(GameState &gs) {
    vector<GameAction> &gas = gs.possibleActions;
    
}
