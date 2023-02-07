//
// Created by Parker Hitchcock on 1/19/23.
//
#include "../Player.h"
#include "../../utils/Math.h"
#include "../../game/game.h"
#include <cmath>
#include <iostream>
#define UCB_CONST sqrt(2)

float MonteCarlo::UCB(node* n){
    return (((float)n->wins)/((float)n->sims)) + UCB_CONST * sqrt(log(t.base->sims)/(float)n->sims);
}

Player* MonteCarlo::simulatedPlayers[] = {new RandomPlayer(0),
                                                new RandomPlayer(1),
                                                new RandomPlayer(2),
                                                new RandomPlayer(3)};

GameAction MonteCarlo::takeAction(GameState &gs) {
    t.base = new node(gs.lastAction);
    t.base->children = vector<node*>(gs.possibleActions.size(), nullptr);
    /*else if(t.base->nChildren() != gs.possibleActions.size()){
        t.base->deleteChildren();
        t.base->children = vector<node*>(gs.possibleActions.size(), nullptr);
    }*/
    int nsims = 0;
    while(nsims < 500) {
        GameState *test = new GameState(gs); //TODO: Test if this or undoing is more efficient
        //Selection
        t.selectBase();
        while (t.selected->nChildren() == test->possibleActions.size()) { //While node is fully expanded
            if(t.selected->nChildren() == 0){
                goto sim;
            }
            //Select child of fully expanded node that meets ucb
            //All children have at least one sim
            unsigned int maxI = 0;
            float maxUCB = -100000;
            int i = 0;
            int n=0;
            for(node* c: t.selected->children){
                float cUCB = UCB(c);
                if(cUCB>maxUCB){
                    maxI = 1<<i; //i-th bit is 1
                    maxUCB = cUCB;
                    n=1;
                } else if(cUCB==maxUCB){
                    maxI = maxI | (1<<i); //flip i-th bit to 1
                    n++;
                }
                i++;
            }
            int selected;
            if(n==1){
                selected = rightmostSetBitPos(maxI);
            } else {
                selected = randRange(0, n);
                //Select the n-th set bit
                for (int j=0; j<selected; j++) {
                    maxI &= maxI-1; // remove the least significant bit (the furthest right)
                }
                selected = rightmostSetBitPos(maxI);
            }
            t.select(selected);
            test->applyAction(t.selected->actionTaken);
            test->advanceTurn();
            test->updatePossibleActions();
        }
        //Selected node is now not fully expanded i.e. missing some children. It's expansion time.
        //Select node to expand to
        int newNode;
        do {
            newNode = randRange(0,test->possibleActions.size());
        } while (t.selected->children[newNode]!=nullptr);
        //Create node
        t.selected->addChild(newNode, test->possibleActions[newNode]);
        t.select(newNode);
        test->applyAction(t.selected->actionTaken);
        test->advanceTurn();
        test->updatePossibleActions();
        t.selected->children = vector<node*>(test->possibleActions.size(), nullptr);
        //Simulate game
        sim:
        Game* sim = Game::continueFromGS(test)->usePlayers(MonteCarlo::simulatedPlayers);
        bool getWin = sim->runGame() == id;
        t.backpropagate(getWin, t.selected);
        free(sim);
        free(test);
        nsims++;
    }
    unsigned int maxI = 0;
    float maxVal = -1;
    int i = 0;
    int n=0;
    for(node* c: t.base->children){
        float cVal = ((float)c->wins/(float)c->sims);
        if(cVal>maxVal){
            maxI = 1<<i; //i-th bit is 1
            maxVal = cVal;
            n=1;
        } else if(cVal==maxVal){
            maxI = maxI | (1<<i); //flip i-th bit to 1
            n++;
        }
        i++;
    }
    if(n==1){
        maxI = rightmostSetBitPos(maxI);
    } else {
        int h = randRange(0, n);
        //Select the n-th set bit
        for (int j=0; j<h; j++) {
            maxI &= maxI-1; // remove the least significant bit (the furthest right)
        }
        maxI = rightmostSetBitPos(maxI);
    }
    t.clear();
    //t.print();
    return gs.possibleActions[maxI];
}

void MonteCarlo::updateState(GameAction &ga) {
    t.selectBase();
    if(t.base == nullptr)
        return;
    int i=0;
    for(node*c:t.base->children){
        if(c != nullptr)
            if(c->actionTaken == ga){
                t.select(i);
                t.setBase();
                return;
            }
        i++;
    }
    //No valid moves
    t.recursiveDelete(t.base);
    t.base = nullptr;
    t.selected = nullptr;
}

void MonteCarlo::printTree() {
    t.print();
}
