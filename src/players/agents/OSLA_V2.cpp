//
// Created by Parker Hitchcock on 1/20/23.
//
#include "../Player.h"

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
