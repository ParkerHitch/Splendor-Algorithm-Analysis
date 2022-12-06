//
// Created by Parker Hitchcock on 11/22/22.
//
#include "../Player.h"

GameAction RandomPlayer::takeAction(GameState &gs) {
    GameAction ga = randomAction();
    ga.playerId = this->id;
    return ga;
}
