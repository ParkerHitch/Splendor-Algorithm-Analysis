//
// Created by Parker Hitchcock on 1/3/23.
//
#include "../Player.h"
#include <iostream>
#include "../../display/gameOutput.h"
#include "../../utils/Math.h"

//#define MINIMAX_DEBUG

float evaluate(GameState* gs, int playerID){
    PlayerState& ps = gs->playerStates[playerID];

//    float score = (float)ps.pts*3;
//    score += 2.0f * (float)ps.discounts[0];
//    score += 2.0f * (float)ps.discounts[1];
//    score += 2.0f * (float)ps.discounts[2];
//    score += 2.0f * (float)ps.discounts[3];
//    score += 2.0f * (float)ps.discounts[4];
//    score += 0.2f * (float)ps.balance0;
//    score += 0.2f * (float)ps.balance1;
//    score += 0.2f * (float)ps.balance2;
//    score += 0.2f * (float)ps.balance3;
//    score += 0.2f * (float)ps.balance4;
//    score += 0.4f * (float)ps.balanceY;
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

float evalAction(GameState* gs, GameAction ga){
    //Assuming action has been applied therefor evaluating for previous player
    switch(ga.type){
        case ERROR: return -1;
        case PURCHASE:
            return 3 + 2.0f*(float)GameState::data->deck1[ga.id].points;
        case RESERVE:
            return 1 + 2.0f*(float)GameState::data->deck1[ga.id].points;
        case TAKE1:
        case TAKE3:
            float score = 0;
            PlayerState ps = gs->playerStates[(gs->turn-1)%4];
            score += 0.2f * (float)ps.balance0;
            score += 0.2f * (float)ps.balance1;
            score += 0.2f * (float)ps.balance2;
            score += 0.2f * (float)ps.balance3;
            score += 0.2f * (float)ps.balance4;
            score += 0.4f * (float)ps.balanceY;
            return score;
    }
    //Assuming gs is result of applying ga
}

float MiniMax::minimax(GameState* gs, int depth){
    if(depth == 0){
        return evaluate(gs, id); //Only evaluate/decrease depth when it's our turn
        //Aka assuming opponents working purely against us, even if it means losing out on their own gains
        //Aka assuming worst-case-scenario
    } else {
        d++;
        bool ourTurn = gs->turn%4 == id;
        std::vector<GameAction> gas(gs->possibleActions);
        if(ourTurn){
            //Max
            float max = 0;
            for(GameAction ga: gas){


            #ifdef MINIMAX_DEBUG
                std::cout << std::string(d, ' ') << "Apply - ";
                printAction(ga);
            #endif

                gs->applyAction(ga);
                gs->advanceTurn();
                gs->updatePossibleActions();

                float score;
//                if(depth==1) {
//                    score = evalAction(gs, ga);
//                } else {
                    score = minimax(gs, depth - 1);
//                }
            #ifdef MINIMAX_DEBUG
                std::cout << std::string(d, ' ') << "Undo - ";
                printAction(ga);
            #endif
                gs->undo(ga);

                //gs->updatePossibleActions();
                if(score>max)
                    max = score;
            }
            d--;
            return max;
        } else {
            //Min
            float min = 0;
            for(GameAction ga: gas){

            #ifdef MINIMAX_DEBUG
                std::cout << std::string(d, ' ') << "Apply - ";
                printAction(ga);
            #endif
                gs->applyAction(ga);

                gs->advanceTurn();
                gs->updatePossibleActions();

                float score = minimax(gs, depth);

            #ifdef MINIMAX_DEBUG
                std::cout << std::string(d, ' ') << "Undo - ";
                printAction(ga);
            #endif
                gs->undo(ga);

                //gs->updatePossibleActions();
                if(score<min)
                    min = score;
            }
            d--;
            return min;
        }
    }
}

GameAction MiniMax::takeAction(GameState &gs) {
    vector<GameAction> &gas = gs.possibleActions;
    float max = 0;
    int maxI = 0;
    int n = 0;
    for(int i=0; GameAction ga: gas){
        GameState* test = new GameState(gs);
        test->applyAction(gas[i]);
        test->advanceTurn();
        test->updatePossibleActions();
        float score = minimax(test, 1);
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

string MiniMax::name() {
    return "MiniMax";
}