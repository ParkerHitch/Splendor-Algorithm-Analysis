//
// Created by Parker Hitchcock on 1/20/23.
//
#include "../Player.h"
#include "../../utils/Math.h"

float OSLA_V2::evaluate(GameState& gs, GameAction& ga){
    GameState* test = new GameState(gs);
    test->applyAction(ga);

    switch(ga.type){
        case ERROR: return -1;
        case PURCHASE:
            return 3 + 2.0f*(float)GameState::data->deck1[ga.id].points;
        case RESERVE:
            return 1 + 2.0f*(float)GameState::data->deck1[ga.id].points;
        case TAKE1:
        case TAKE3:
            float score = 0;
            PlayerState ps = gs.playerStates[id];
            score += 0.2f * (float)ps.balance0;
            score += 0.2f * (float)ps.balance1;
            score += 0.2f * (float)ps.balance2;
            score += 0.2f * (float)ps.balance3;
            score += 0.2f * (float)ps.balance4;
            score += 0.4f * (float)ps.balanceY;
            return score;
    }
}

GameAction OSLA_V2::takeAction(GameState &gs) {
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

string OSLA_V2::name() {
    return "OSLA_V2";
}
