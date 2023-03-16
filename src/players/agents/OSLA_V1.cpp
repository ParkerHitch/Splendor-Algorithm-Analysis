//
// Created by Parker Hitchcock on 12/17/22.
//
#include "../Player.h"
#include "../../utils/Math.h"

float RLBasic_V1::evaluate(GameState& gs, GameAction& ga){
    GameState* test = new GameState(gs);
    test->applyAction(ga);
    float score = 0;
    for(int i=0; i<4; i++) {
        PlayerState &ps = test->playerStates[(id+i)%4]; // weights[0] is weight for current player
        score += weights[i][0] * (float) ps.discounts[0];
        score += weights[i][1] * (float) ps.discounts[1];
        score += weights[i][2] * (float) ps.discounts[2];
        score += weights[i][3] * (float) ps.discounts[3];
        score += weights[i][4] * (float) ps.discounts[4];
        score += weights[i][5] * (float) ps.balance0;
        score += weights[i][6] * (float) ps.balance1;
        score += weights[i][7] * (float) ps.balance2;
        score += weights[i][8] * (float) ps.balance3;
        score += weights[i][9] * (float) ps.balance4;
        score += weights[i][10] * (float) ps.balanceY;
        score += weights[i][11] * (float) ps.pts;
        int nReserved = (ps.reservedCards[0]==nullptr) + (ps.reservedCards[1]==nullptr) + (ps.reservedCards[2]==nullptr);
        int reservedPts = (ps.reservedCards[0]==nullptr?0:ps.reservedCards[0]->points) +
                          (ps.reservedCards[1]==nullptr?0:ps.reservedCards[1]->points) +
                          (ps.reservedCards[2]==nullptr?0:ps.reservedCards[2]->points);
        score += weights[i][12] * (float) nReserved;
        score += weights[i][13] * (float) reservedPts;
    }
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
