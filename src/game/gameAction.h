//
// Created by Parker Hitchcock on 11/14/22.
//

#ifndef RESEARCH_GAMEACTION_H
#define RESEARCH_GAMEACTION_H

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
};

GameAction randomAction();
GameAction randomValidAction();

#endif //RESEARCH_GAMEACTION_H
