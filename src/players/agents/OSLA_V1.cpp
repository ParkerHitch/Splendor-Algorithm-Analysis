//
// Created by Parker Hitchcock on 12/17/22.
//
#include "../Player.h"

float OSLA_V1::evaluate(GameState& gs, GameAction& ga){
    GameState* test = new GameState(gs);
    test->applyAction(ga);
    PlayerState& ps = test->playerStates[id];

    float score = 0;
    score += (float)ps.discounts[0];
    score += (float)ps.discounts[1];
    score += (float)ps.discounts[2];
    score += (float)ps.discounts[3];
    score += (float)ps.discounts[4];
    score += 0.2f * (float)ps.balance0;
    score += 0.2f * (float)ps.balance1;
    score += 0.2f * (float)ps.balance2;
    score += 0.2f * (float)ps.balance3;
    score += 0.2f * (float)ps.balance4;
    score += 0.4f * (float)ps.balanceY;
    free(test);
    return score;
}

GameAction OSLA_V1::takeAction(GameState &gs) {
    vector<GameAction> &gas = gs.possibleActions;
    float max = 0;
    int maxI = 0;
    for(int i=0; GameAction ga: gas){
        float score = evaluate(gs, ga);
        if(score>max){
            max = score;
            maxI = i;
        }
        i++;
    }
    return gas[maxI];
}
