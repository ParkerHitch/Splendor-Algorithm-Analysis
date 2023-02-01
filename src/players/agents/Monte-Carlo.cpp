//
// Created by Parker Hitchcock on 1/19/23.
//
#include "../Player.h"
#include "../../utils/Math.h"
GameAction MonteCarlo::takeAction(GameState &gs) {

    if (t.base == nullptr){
        t.base = new node{.parent=nullptr, .data=0};
    }
    t.selectBase();
    //selection
    while(!t.selected->children.empty()){
        t.select(randRange(0,t.selected->children.size()));
    }

    vector<GameAction> &gas = gs.possibleActions;
    float max = 0;
    int maxI = 0;
    for(int i=0; GameAction ga: gas){
        GameState* test = new GameState(gs);
        test->applyAction(gas[i]);
        test->advanceTurn();
        test->updatePossibleActions();
        free(test);
        i++;
    }
    return gas[maxI];
}