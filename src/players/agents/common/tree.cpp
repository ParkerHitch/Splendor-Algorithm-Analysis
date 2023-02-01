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