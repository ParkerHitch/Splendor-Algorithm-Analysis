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

    int data;

    vector<node*> children;
};

struct tree{
    node* base;
    node* selected;
    void select(int index);
    void back();
    void selectBase();
    void setBase();
    boolean atBase();
    void recursiveDelete(node* n);
};


#endif //RESEARCH_TREE_H
