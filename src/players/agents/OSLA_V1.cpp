//
// Created by Parker Hitchcock on 12/17/22.
//
#include "../Player.h"
#include "../../utils/Math.h"

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
    delete test;
    return score;
}

GameAction OSLA_V1::takeAction(GameState &gs) {
    vector<GameAction> &gas = gs.possibleActions;
    float max = 0;
    int maxI = 0;
    int n = 0;
    for(int i=0; GameAction ga: gas){
        float score = evaluate(gs, ga);
        if(score>max){
            maxI = 1<<i; //i-th bit is 1
            max = score;
            n=1;
        } else if(score==max){
            maxI = maxI | (1<<i); //flip i-th bit to 1
            n++;
        }
        i++;
    }
    if(n==1){
        maxI = rightmostSetBitPos(maxI);
    } else {
        int h = randRange(0, n);
        //Select the n-th set bit
        for (int j=0; j<h; j++) {
            maxI &= maxI-1; // remove the least significant bit (the furthest right)
        }
        maxI = rightmostSetBitPos(maxI);
    }
    return gas[maxI];
}

string OSLA_V1::name() {
    return "OSLA_V1";
}
