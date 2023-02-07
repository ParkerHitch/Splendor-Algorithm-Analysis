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
    int nChildren();

    explicit node(GameAction ga);
    void addChild(int index, GameAction ga);
    void deleteChildren();
    void print(int indent);
};

struct tree{
    node* base = nullptr;
    node* selected = nullptr;
    void select(int index);
    void back();
    void selectBase();
    void setBase();
    bool atBase();
    void clear();
    static void recursiveDelete(node* n);
    void backpropagate(bool win, node* startingNode);
    void print();
};


#endif //RESEARCH_TREE_H
