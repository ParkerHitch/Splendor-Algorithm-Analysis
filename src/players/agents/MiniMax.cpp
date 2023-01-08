//
// Created by Parker Hitchcock on 1/3/23.
//
#include "../Player.h"
float evaluate(GameState* gs, int playerID){
    PlayerState& ps = gs->playerStates[playerID];

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
    return score;
}
float MiniMax::minimax(GameState* gs, int depth){
    if(depth == 0){
        return evaluate(gs, id); //Only evaluate/decrease depth when it's our turn
        //Aka assuming opponents working purely against us, even if it means losing out on their own gains
        //Aka assuming worst-case-scenario
    } else {
        bool ourTurn = gs->turn%4 == id;
        std::vector<GameAction> gas = gs->possibleActions;
        if(ourTurn){
            //Max
            float max = 0;
            for(GameAction ga: gas){
                gs->applyAction(ga);
                float score = minimax(gs, depth-1);
                gs->undo(ga);
                if(score>max)
                    max = score;
            }
            return max;
        } else {
            //Min
            float min = 0;
            for(GameAction ga: gas){
                gs->applyAction(ga);
                float score = minimax(gs, depth);
                gs->undo(ga);
                if(score<min)
                    min = score;
            }
            return min;
        }
    }
}

GameAction MiniMax::takeAction(GameState &gs) {
    vector<GameAction> &gas = gs.possibleActions;
    float max = 0;
    int maxI = 0;
    for(int i=0; GameAction ga: gas){
        GameState* test = new GameState(gs);
        test->applyAction(gas[i]);
        float score = minimax(test, 4);
        free(test);
        if(score>max){
            max = score;
            maxI = i;
        }
        i++;
    }
    return gas[maxI];
}
