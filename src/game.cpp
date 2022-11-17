//
// Created by Parker Hitchcock on 11/3/22.
//
#include <iostream>
#include "game.h"
#include "gameSetup.h"
#include "Display/gameOutput.h"
#include "Players/Player.h"

GameState gameState = GameState{};
Player* players[4];

void startGame(ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n){
    loadAndShuffleDecks(gameState, d1, d2, d3, n);
    for(int d=0; d<3; d++)
        for(int p=0; p<4; p++)
            flipCard(gameState, d, p);
    for(int i=0; i<5; i++)
        flipNoble(gameState, i);

    //TODO: ADD CODE FOR REAL SETUP CONFIG STUFF
    for(int i=0; i<4; i++)
        players[i] = new HumanPlayer(i);

    printGS(gameState);
    while(!gameState.isTerminal) {
        takeTurn();
        printGS(gameState);
    }

    for(Player*& player : players)
        free(player);

    printGS(gameState);
}

void takeTurn(){
    GameAction pa;//PLAYER ACTION
    do {
        pa = players[gameState.turn % 4]->takeAction(gameState);
    } while(!validAction(pa));
    applyAction(pa);
}

bool validAction(GameAction &ga) {
    switch (ga.type) {
        case ERROR:
            return true;
        case TAKE3:
            return  gameState.bank[ga.suit1] > 0 &&
                    gameState.bank[ga.suit2] > 0 &&
                    gameState.bank[ga.suit3] > 0;
        case TAKE1:
            return gameState.bank[ga.suit1] > 4;//MINIMUM FOR TAKING TWO (RULE)
        case RESERVE:
            if(gameState.playerStates[ga.playerId].reservedCards[2] != nullptr)
                return false;
        case PURCHASE:
            Card** row = ga.id<40? gameState.D1Showing : ga.id<70? gameState.D2Showing : gameState.D3Showing;
            for(int i = 0; i<4; i++)
                if(row[i]->id==ga.id)
                    return true;
            return false;
    }
}

void applyAction(GameAction &ga) {
    PlayerState& ps = gameState.playerStates[ga.playerId];
    switch (ga.type) {
        case ERROR:
            gameState.isTerminal = true;
            break;
        case TAKE3:
            gameState.bank[ga.suit1] -= 1;
            gameState.bank[ga.suit2] -= 1;
            gameState.bank[ga.suit3] -= 1;
            ps.balance[ga.suit1] += 1;
            ps.balance[ga.suit2] += 1;
            ps.balance[ga.suit3] += 1;
            break;
        case TAKE1:
            gameState.bank[ga.suit1] -= 2;
            gameState.playerStates[ga.playerId].balance[ga.suit1] += 2;
            break;
        case RESERVE:
        case PURCHASE:
            Card** end = ga.type==PURCHASE?ps.ownedCards:ps.reservedCards;
            int dNum = ga.id<40? 0 : ga.id<70? 1 : 2;
            for(int i = 0; i<12; i++)
                if(gameState.D1Showing[i]->id==ga.id){
                    int b = 0;
                    while(end[b]!=nullptr)
                        b++;
                    end[b] = gameState.D1Showing[i];
                    flipCard(gameState, dNum, i%4);
                    return;
                }
            break;

    }
    gameState.turn++;
}


//HELPER UNIMPORTANT BAD FUNCTIONS BELOW THIS LINE. WE DISCRIMINATE!
bool flipCard(GameState& gs, int dNum, int newPos) {
    //POINTS TO START OF DECK
    Card* deck = dNum==0? gs.deck1 : dNum==1? gs.deck2 : gs.deck3;
    Card** row = dNum==0? gs.D1Showing : dNum==1? gs.D2Showing : gs.D3Showing;
    int* index = dNum==0? &gs.iD1 : dNum==1? &gs.iD2 : &gs.iD3;
    if(*index>=20+10*dNum)
        return false;
    row[newPos] = &(deck[*index]);
    (*index)++;
    return true;
}
bool flipNoble(GameState &gs, int newPos) {
    if(gs.iN>=10)
        return false;
    gs.NoblesShowing[newPos] = &(gs.nobles[gs.iN]);
    gs.iN++;
    return true;
}