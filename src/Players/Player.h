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
    Player(int i);
    virtual GameAction takeAction(GameState& gs)=0;
};

class HumanPlayer: public Player {
public:
    HumanPlayer(int i);
    GameAction takeAction(GameState& gs) override;
};

#endif //RESEARCH_PLAYER_H
