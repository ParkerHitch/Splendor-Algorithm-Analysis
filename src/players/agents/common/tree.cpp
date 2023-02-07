//
// Created by Parker Hitchcock on 1/19/23.
//

#include "tree.h"
#include <iostream>
#include "../../../display/gameOutput.h"
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
    for(node* c:n->children){
        if(c==nullptr || c->parent==nullptr)
            continue;
        recursiveDelete(c);
    }
    free(n);
}

void tree::clear() {
    recursiveDelete(base);
    base = nullptr;
    selected = nullptr;
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
    children = std::vector<node*>(0);
}

void node::addChild(int index, GameAction ga) {
    node* child = new node(ga);
    child->parent = this;
    this->children[index]=child;
}

void node::deleteChildren() {
    for(node* c:children){
        if(c==nullptr)
            continue;
        tree::recursiveDelete(c);
    }
}

void tree::print() {
    if(base!= nullptr)
        base->print(0);
    else
        std::cout<<"No Tree"<<std::endl;
}

void node::print(int indent) {
    std::cout<< std::string(indent,' ') << wins << '/' << sims;
    printAction(this->actionTaken);
    for(node* c:children){
        if(c==nullptr)
            continue;
        c->print(indent+1);
    }
}

int node::nChildren() {
    return std::count_if(children.begin(), children.end(), [](node* c)->bool{return c!=nullptr;});
}
