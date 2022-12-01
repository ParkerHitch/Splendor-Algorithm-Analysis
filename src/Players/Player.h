//
// Created by Parker Hitchcock on 11/14/22.
//

#ifndef RESEARCH_PLAYER_H
#define RESEARCH_PLAYER_H

#include "../gameAction.h"
#include "../gameState.h"

class Player {
public:
    const int id;
    explicit Player(int i);
    virtual GameAction takeAction(GameState& gs)=0;
};

class HumanPlayer: public Player {
public:
    using Player::Player;
    GameAction takeAction(GameState& gs) override;
};

class RandomPlayer: public Player {
public:
    using Player::Player;
    GameAction takeAction(GameState& gs) override;
};

#endif //RESEARCH_PLAYER_H
