//
// Created by Parker Hitchcock on 11/23/22.
//
#include <random>
#include "gameAction.h"
#include "../utils/Math.h"
#include "game.h"
#include <iostream>

using namespace std;

bool start = true;

GameAction randomAction() {
    GameAction ga = {};

    if(start) {
        srand(time(nullptr));
        start = false;
    }
    ga.type  = actionType(rand() % 4);
    ga.id    = rand() % 90;
    ga.suit1 = rand() % 5;
    ga.suit2 = rand() % 5;
    ga.suit3 = rand() % 5;
    return ga;
}

GameAction randomValidAction(GameState& gs) {
    if(start) {
        srand(time(nullptr));
        start = false;
    }
    vector<GameAction> gas = validActions(gs);
    return gas[rand() % gas.size()];
}

void combinations(GameAction ga, int offset, int k, vector<int>& suits, vector<GameAction>& v) {
    if (k == 0) {
        v.push_back(ga);
        return;
    }
    for (int i = offset; i <= suits.size() - k; ++i) {
        (&ga.suit1)[ga.id++] = suits[i];
        combinations(ga, i+1, k-1, suits, v);
        ga.id--;
    }
}

void addValidTake3s(GameState& gs, vector<GameAction>& gas) {

    vector<int> suits;
    for(int s=0; s<5; s++)
        if((&gs.bankAmt0)[s] > 0)
            suits.push_back(s);

    if(suits.empty())
        return;

    int n = (int)suits.size();
    int r = min(n, 3);

    gas.reserve(gas.capacity() + factorial(n)/(factorial(r)* factorial(n-r)));

    //One possible action
    GameAction ga = {.type=TAKE3, .playerId=gs.turn%4, .suit1 =-1, .suit2=-1, .suit3=-1, .id = 0};
    if(n<=3){
        for(int s: suits)
            (&ga.suit1)[ga.id++] = s;
        gas.push_back(ga);
    }
    //More than 3 to pick from. Need to generate all possible combinations
    combinations(ga, 0, r, suits, gas);
}

void addValidTake2s(GameState& gs, vector<GameAction>& gas) {

    for(int s=0; s<5; s++)
        if((&gs.bankAmt0)[s] >= 4)
            gas.push_back(GameAction{.type=TAKE1, .playerId=gs.turn%4, .suit1=s});

}

void addValidBuysAndReserves(GameState& gs, vector<GameAction>& gas){
    PlayerState& ps = gs.playerStates[gs.turn%4];
    bool canReserve = ps.reservedCards[2] == nullptr;
    if (canReserve) {
        gas.reserve(gas.capacity() + 12);
        for(int c=0; c<12; c++){
            Card* card = gs.D1Showing[c];
            if(card == nullptr)
                continue;
            gas.push_back(GameAction{.type=RESERVE, .playerId=ps.playerNum, .id=card->id});
            if(Game::canAfford(ps, *card))
                gas.push_back(GameAction{.type=PURCHASE, .playerId=ps.playerNum, .id=card->id});
        }
    } else {
        for(int c=0; c<12; c++){
            Card* card = gs.D1Showing[c];
            if (card== nullptr)
                continue;
            if(Game::canAfford(ps, *card))
                gas.push_back(GameAction{.type=PURCHASE, .playerId=ps.playerNum, .id=card->id});
        }
    }
    for(int c=0; c<3 && ps.reservedCards[c]!=nullptr; c++){
        Card* card = ps.reservedCards[c];
        if(Game::canAfford(ps, *card))
            gas.push_back(GameAction{.type=PURCHASE, .playerId=ps.playerNum, .id=card->id});
    }
}

vector<GameAction> validActions(GameState& gs) {
    vector<GameAction> gas;
    addValidTake3s(gs, gas);
    addValidTake2s(gs, gas);
    addValidBuysAndReserves(gs, gas);
    return gas;
}