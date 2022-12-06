//
// Created by Parker Hitchcock on 11/14/22.
//

#include "gameSetup.h"

#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

Card cardFromLine(const string &line, int id) {
    return Card{
            id,
            line[0]-'0',
            line[2]-'0',
            line[4]-'0',
            line[6]-'0',
            line[8]-'0',
            line[10]-'0',
            line[12]-'0',
    };
}

Noble nobleFromLine(const string &line, int id) {
    return Noble{
            id,
            line[0]-'0',
            line[2]-'0',
            line[4]-'0',
            line[6]-'0',
            line[8]-'0',
            line[10]-'0',
    };
}

void flipCard(GameState& gameState, int dNum, int newPos) {
    //POINTS TO START OF DECK
    Card *deck = dNum == 0 ? gameState.deck1 : dNum == 1 ? gameState.deck2 : gameState.deck3;
    Card **row = dNum == 0 ? gameState.D1Showing : dNum == 1 ? gameState.D2Showing : gameState.D3Showing;
    int *index = dNum == 0 ? &gameState.iD1 : dNum == 1 ? &gameState.iD2 : &gameState.iD3;
    if (*index >= 40 - 10 * dNum) {
        row[newPos] = nullptr;
        return;
    }
    row[newPos] = &(deck[*index]);
    (*index)++;
}

void flipNoble(GameState& gameState, int newPos) { //Only used in setup.
    gameState.noblesShowing[newPos] = &(gameState.nobles[gameState.iN]);
    gameState.iN++;
}

GameState load(ifstream &d1, ifstream &d2, ifstream &d3, ifstream &n) {
    GameState gs = {};
    string line;
    getline(d1, line);//Skip comment
    int id = 0;
    while (id<40){
        getline(d1, line);
        gs.deck1[id] = cardFromLine(line,id);
        id++;
    }

    getline(d2, line);//Skip comment
    while (id<70){
        getline(d2, line);
        gs.deck2[id-40] = cardFromLine(line,id);
        id++;
    }

    getline(d3, line);//Skip comment
    while (id<90){
        getline(d3, line);
        gs.deck3[id-70] = cardFromLine(line,id);
        id++;
    }

    id = 0;
    getline(n, line);//Skip comment
    while (id<10){
        getline(n, line);
        gs.nobles[id] = nobleFromLine(line, id);
        id++;
    }
    for(int i=0; i<4; i++){
        gs.playerStates[i].playerNum = i;
    }
    return gs;
}

GameState newGameFromFiles(ifstream &d1, ifstream &d2, ifstream &d3, ifstream &n) {
    GameState gs = load(d1,d2,d3,n);
    for (int d = 0; d < 3; d++)
        for (int p = 0; p < 4; p++)
            flipCard(gs, d, p);
    for (int i = 0; i < 5; i++)
        flipNoble(gs, i);
    return gs;
}

GameState newGameFromFilesShuffled(ifstream &d1, ifstream &d2, ifstream &d3, ifstream &n) {
    GameState gs = load(d1, d2, d3, n);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    auto rnd = default_random_engine(default_random_engine(seed));
    shuffle((gs.deck1), end(gs.deck1), rnd);
    shuffle((gs.deck2), end(gs.deck2), rnd);
    shuffle((gs.deck3), end(gs.deck3), rnd);
    shuffle((gs.nobles), end(gs.nobles), rnd);
    for (int d = 0; d < 3; d++)
        for (int p = 0; p < 4; p++)
            flipCard(gs, d, p);
    for (int i = 0; i < 5; i++)
        flipNoble(gs, i);
    return gs;
}