//
// Created by Parker Hitchcock on 11/22/22.
//
#include "../Player.h"

GameAction RandomPlayer::takeAction(GameState &gs, vector<GameAction>& gas) {
    GameAction ga = randomValidAction(gas);
    return ga;
}
