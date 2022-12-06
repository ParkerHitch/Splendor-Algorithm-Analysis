//
// Created by Parker Hitchcock on 11/23/22.
//
#include <random>
#include "gameAction.h"
#include "gameState.h"

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
    GameAction ga = {};

    if(start) {
        srand(time(nullptr));
        start = false;
    }

    ga.type  = actionType(rand() % 4);
    ga.id    = (gs.D1Showing[rand()%12])->id;
    ga.suit1 = rand() % 5;
    ga.suit2 = rand() % 5;
    ga.suit3 = rand() % 5;
    return ga;
}