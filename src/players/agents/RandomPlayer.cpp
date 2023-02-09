//
// Created by Parker Hitchcock on 11/22/22.
//
#include "../Player.h"

GameAction RandomPlayer::takeAction(GameState &gs) {
    GameAction ga = randomValidAction(gs.possibleActions);
    return ga;
}

string RandomPlayer::name() {
    return "RandomPlayer";
}
