//
// Created by Parker Hitchcock on 11/14/22.
//

#ifndef RESEARCH_GAMEACTION_H
#define RESEARCH_GAMEACTION_H

#include <vector>
#include "gameState.h"

using namespace std;

enum actionType{
    TAKE3,
    TAKE1,
    RESERVE,
    PURCHASE,
    ERROR
};

struct GameAction{
    actionType type;
    int playerId;
    //TAKE3 and TAKE1 (only type1)
    int suit1, suit2, suit3;
    //RESERVE & PURCHASE
    int id;
    bool operator==( const GameAction& other ) const {
        return type     == other.type &&
               playerId == other.playerId &&
               suit1    == other.suit1 &&
               suit2    == other.suit2 &&
               suit3    == other.suit3 &&
               id       == other.id;
    }
};

GameAction randomAction();
GameAction randomValidAction(GameState& gs);

vector<GameAction> validActions(GameState& gs);

#endif //RESEARCH_GAMEACTION_H
