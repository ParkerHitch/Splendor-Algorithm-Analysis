//
// Created by Parker Hitchcock on 11/14/22.
//

#ifndef RESEARCH_PLAYER_H
#define RESEARCH_PLAYER_H

#include "../game/gameDependencies.h"
#include "agents/common/tree.h"

class Player {
public:
    int id;
    explicit Player(int i);
    virtual GameAction takeAction(GameState& gs)=0;

    virtual void updateState(GameAction& ga){};
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

class OSLA_V1: public Player {
public:
    using Player::Player;
    GameAction takeAction(GameState& gs) override;

    float evaluate(GameState &gs, GameAction& ga);
};

class OSLA_V2: public Player {
public:
    using Player::Player;
    GameAction takeAction(GameState& gs) override;

    float evaluate(GameState &gs, GameAction& ga);
};

class MiniMax: public Player {
public:
    using Player::Player;

    GameAction takeAction(GameState& gs) override;
    float minimax(GameState* gs, int depth);
private:
    int d=0;
};

class MonteCarlo: public Player {
public:
    using Player::Player;

    GameAction takeAction(GameState& gs) override;
    void updateState(GameAction& ga) override;
    void printTree();
private:
    static Player* simulatedPlayers[4];
    float UCB(node*n);
    int d=0;
    tree t{};
};

#endif //RESEARCH_PLAYER_H
