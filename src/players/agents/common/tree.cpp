//
// Created by Parker Hitchcock on 1/19/23.
//

#include "tree.h"

void tree::select(int index) {
    selected = selected->children[index];
}
void tree::selectBase() {
    selected = base;
}
void tree::back() {
    selected = selected->parent;
}
void tree::setBase() {
    selected->parent = nullptr;
    recursiveDelete(base);
    base = selected;
}
bool tree::atBase() {
    return selected == base;
}
void tree::recursiveDelete(node* n){
    if(!n->children.empty())
        for(node* c:n->children){
            if(c->parent!=nullptr)
                recursiveDelete(c);
        }
    free(n);
}

void tree::backpropagate(bool win, node* n) {
    n->sims++;
    n->wins+=win;
    if(n->parent==nullptr)
        return;
    backpropagate(win, n->parent);
}

node::node(GameAction ga) {
    actionTaken = ga;
    wins = 0; sims = 0;
    parent = nullptr;
    nChildren = 0;
    children = std::vector<node*>(0);
}

void node::addChild(int index, GameAction ga) {
    node* child = new node(ga);
    child->parent = this;
    this->children[index]=child;
}
