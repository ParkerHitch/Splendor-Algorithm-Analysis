//
// Created by Parker Hitchcock on 11/14/22.
//

#ifndef RESEARCH_GAMEACTION_H
#define RESEARCH_GAMEACTION_H

#include <vector>

using namespace std;

struct GameState;

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
               type==TAKE1 ? suit1==other.suit1 :
               type==TAKE3 ? suit1==other.suit1&&suit2==other.suit2&&suit3==other.suit3 :
               id == other.id;
    }
};

GameAction randomAction();
GameAction randomValidAction(GameState& gs);
GameAction randomValidAction(vector<GameAction>& gas);

vector<GameAction> validActions(GameState& gs);

#endif //RESEARCH_GAMEACTION_H
