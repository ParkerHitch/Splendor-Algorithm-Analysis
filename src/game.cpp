//
// Created by Parker Hitchcock on 11/3/22.
//
#include <iostream>
#include "game.h"

GameState gameState;

void startGame(ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n){
    gameState = GameState{};
    loadAndShuffleDecks(gameState, d1, d2, d3, n);
}

void takeTurn(){

}