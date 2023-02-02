//
// Created by Parker Hitchcock on 1/19/23.
//

#ifndef RESEARCH_TREE_H
#define RESEARCH_TREE_H

#include "../../../game/gameDependencies.h"
#include <vector>

using namespace std;

struct node{
    node* parent;
    GameAction actionTaken;//to get to this node

    int wins;
    int sims;

    vector<node*> children;
    int nChildren;

    explicit node(GameAction ga);
    void addChild(int index, GameAction ga);
};

struct tree{
    node* base;
    node* selected;
    void select(int index);
    void back();
    void selectBase();
    void setBase();
    bool atBase();
    void recursiveDelete(node* n);
    void backpropagate(bool win, node* startingNode);
};


#endif //RESEARCH_TREE_H
