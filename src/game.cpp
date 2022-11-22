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
        gameState.turn++;
        printGS(gameState);
    }

    for(Player*& player : players)
        free(player);
}

void takeTurn(){
    GameAction pa;//PLAYER ACTION
    do {
        pa = players[gameState.turn % 4]->takeAction(gameState);
    } while(!validAction(pa));
    applyAction(pa);
}

bool checkWin(PlayerState& ps){
    int b = 0;
    int pts = 0;
    while(ps.ownedCards[b]!=nullptr)
        pts+=ps.ownedCards[b++]->points;//Add points & increment id
    b=0;
    while(ps.nobles[b]!=nullptr)
        pts+=ps.nobles[b++]->points;//Add points & increment id
    return pts>=15;
}

bool validAction(GameAction &ga) {
    switch (ga.type) {
        case ERROR:
            return true;
        case TAKE3:
            return  (&gameState.bankAmt0)[ga.suit1] > 0 &&
                    (&gameState.bankAmt0)[ga.suit2] > 0 &&
                    (&gameState.bankAmt0)[ga.suit3] > 0;
        case TAKE1:
            return (&gameState.bankAmt0)[ga.suit1] >= 4;//Minimum for taking two (game rule)
        case RESERVE:
            if(gameState.playerStates[ga.playerId].reservedCards[2] != nullptr)
                return false;
        case PURCHASE:
            for(int i = 0; i<12; i++)
                if(gameState.D1Showing[i]->id==ga.id){
                    if(ga.type==PURCHASE) {
                        int diff = 0; //# of coins missing
                        for (int i = 0; i < 5; i++) {
                            diff += max(0, (&(gameState.D1Showing[i]->cost0))[i] -
                                           (&gameState.playerStates[ga.playerId].balance0)[i]);
                        }
                        return diff <= gameState.playerStates[ga.playerId].balanceY;
                    }
                    return true;//Found in flipped and reserving
                }
            //Card is not flipped on table. If we are buying it could be in our reserved
            if(ga.type==PURCHASE)
                for(Card* card : gameState.playerStates[ga.playerId].reservedCards)
                    if(card->id==ga.id){
                        int diff = 0; //# of coins missing
                        for (int i = 0; i < 5; i++)
                            diff += max(0, (&(gameState.D1Showing[i]->cost0))[i] -
                                           (&gameState.playerStates[ga.playerId].balance0)[i]);
                        return diff <= gameState.playerStates[ga.playerId].balanceY;
                    }
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
            (&gameState.bankAmt0)[ga.suit1] -= 1;
            (&gameState.bankAmt0)[ga.suit2] -= 1;
            (&gameState.bankAmt0)[ga.suit3] -= 1;
            (&ps.balance0)[ga.suit1] += 1;
            (&ps.balance0)[ga.suit2] += 1;
            (&ps.balance0)[ga.suit3] += 1;
            break;
        case TAKE1:
            (&gameState.bankAmt0)[ga.suit1] -= 2;
            (&ps.balance0)[ga.suit1] += 2;
            break;
        case RESERVE:
            if(gameState.bankAmtY>0){
                ps.balanceY+=1;
                gameState.bankAmtY-=1;
            }
        case PURCHASE:
            Card** end = ga.type==PURCHASE?ps.ownedCards:ps.reservedCards;//Where the card ends up
            for(int i = 0; i<12; i++)
                if(gameState.D1Showing[i]->id==ga.id){
                    int b = 0;
                    while(end[b]!=nullptr)
                        b++;
                    //Give player card
                    end[b] = gameState.D1Showing[i];
                    if(ga.type==PURCHASE){
                        //Subtract cost from player. Assumed that player wants to conserve wilds.
                        for (int s=0; s<5; s++) {
                            (&ps.balance0)[s] -= (&(gameState.D1Showing[i]->cost0))[s];
                            ps.balanceY += min(0,(&ps.balance0)[s]);
                            (&ps.balance0)[s] = max(0,(&ps.balance0)[s]);
                        }
                    }
                    flipCard(gameState, i/4, i%4);
                    goto nobleCheck;
                }
            //Card not in flipped cards. Must be purchasing from reserves bc move is valid
            for(int c=0; c<3; c++)
                if(ps.reservedCards[c]->id==ga.id){
                    for (int s=0; s<5; s++) {
                        (&ps.balance0)[s] -= (&(ps.reservedCards[c]->cost0))[s];
                        ps.balanceY += min(0,(&ps.balance0)[s]);
                        (&ps.balance0)[s] = max(0,(&ps.balance0)[s]);
                    }
                    int b = 0;
                    while(end[b]!=nullptr)
                        b++;
                    ps.ownedCards[b] = ps.reservedCards[c];
                    //Remove & shift left
                    ps.reservedCards[c] = nullptr;
                    while(c<2) {
                        ps.reservedCards[c] = ps.reservedCards[c + 1];
                        c++;
                    }
                    ps.reservedCards[2] = nullptr;
                }
            break;
    }
    //Check for Noble visits
    nobleCheck:
    int a[5]={};
    for(int b=0; ps.ownedCards[b]!=nullptr; b++)
        a[ps.ownedCards[b]->suit]++;
    //cout << a[0] << a[1] << a[2] << a[3] << a[4];
    for(int c=0; c<5; c++) {
        if (gameState.noblesShowing[c] == nullptr)
            break;

        for(int s=0; s<5; s++)
             if(a[s]<(&(gameState.noblesShowing[c]->cost0))[s])
                 goto nextN;
        //Noble attracted
        for(Noble*& pn : ps.nobles)
            if(pn==nullptr){
                pn = gameState.noblesShowing[c];
                break;
            }
        //Remove & shift left
        gameState.noblesShowing[c] = nullptr;
        while (c < 4){
            gameState.noblesShowing[c] = gameState.noblesShowing[c+1];
            c++;
        }
        gameState.noblesShowing[4] = nullptr;
        nextN: ;
    }

    //Check win. Not in purchase because you could buy one turn and be able to be visited by 2 nobles. Next turn could be visited by second without purchasing.
    gameState.isTerminal = checkWin(ps);
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
    gs.noblesShowing[newPos] = &(gs.nobles[gs.iN]);
    gs.iN++;
    return true;
}