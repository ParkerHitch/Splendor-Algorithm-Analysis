//
// Created by Parker Hitchcock on 11/3/22.
//
#include <iostream>
#include "game.h"
#include "gameSetup.h"
#include "Display/gameOutput.h"
#include "Players/Player.h"

GameState gameState = GameState{};
Player* players[K_PNUM];

void startGame(ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n){
    loadAndShuffleDecks(gameState, d1, d2, d3, n);
    for(int d=0; d<3; d++)
        for(int p=0; p<4; p++)
            flipCard(gameState, d, p);
    for(int i=0; i<5; i++)
        flipNoble(gameState, i);

    //TODO: ADD CODE FOR REAL SETUP CONFIG STUFF
    for(int i=0; i < K_PNUM; i++)
        players[i] = new RandomPlayer(i);

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
        pa = players[gameState.turn % K_PNUM]->takeAction(gameState);
    } while(!validAction(pa));
    printAction(pa);
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
            return  ga.suit1!=ga.suit2 && ga.suit2!=ga.suit3 && ga.suit1!=ga.suit3;// &&
//                    (&gameState.bankAmt0)[ga.suit1] > 0 &&
//                    (&gameState.bankAmt0)[ga.suit2] > 0 &&
//                    (&gameState.bankAmt0)[ga.suit3] > 0;
        case TAKE1:
            return (&gameState.bankAmt0)[ga.suit1] >= 4;//Minimum for taking two (game rule)
        case RESERVE:
            if(gameState.playerStates[ga.playerId].reservedCards[2] != nullptr)
                return false;
        case PURCHASE:
            for(int c=0; c < 12; c++)
                if(gameState.D1Showing[c]->id == ga.id){
                    if(ga.type==PURCHASE)
                        return canBuy(gameState.playerStates[ga.playerId], *gameState.D1Showing[c]);
                    return true;//Found in flipped and reserving
                }
            //Card is not flipped on table. If we are buying it could be in our reserved
            if(ga.type==PURCHASE)
                for(Card* card : gameState.playerStates[ga.playerId].reservedCards){
                    if(card==nullptr)
                        return false;
                    if(card->id==ga.id){
                        return canBuy(gameState.playerStates[ga.playerId], *card);
                    }
                }
            return false;
    }
}

bool canBuy(PlayerState &ps, Card &card) {
    int diff = 0; //# of coins missing
    for (int suit=0; suit < 5; suit++)
        diff += max(0, (&card.cost0)[suit] - ps.discounts[suit] - //HAVE TO PAY
                       (&ps.balance0)[suit]); //HAVE IN BANK
    return diff <= ps.balanceY;
}

void applyAction(GameAction &ga) {
    PlayerState& ps = gameState.playerStates[ga.playerId];
    switch (ga.type) {
        case ERROR:
            gameState.isTerminal = true;
            break;
        case TAKE3: { // FOR VARIABLE SCOPE
            int d1 = min(1, (&gameState.bankAmt0)[ga.suit1]); // if bank amt = 0 d1 = 0 else d1 = 1
            int d2 = min(1, (&gameState.bankAmt0)[ga.suit2]);
            int d3 = min(1, (&gameState.bankAmt0)[ga.suit3]);
            (&gameState.bankAmt0)[ga.suit1] -= d1;
            (&gameState.bankAmt0)[ga.suit2] -= d2;
            (&gameState.bankAmt0)[ga.suit3] -= d3;
            (&ps.balance0)[ga.suit1] += d1;
            (&ps.balance0)[ga.suit2] += d2;
            (&ps.balance0)[ga.suit3] += d3;
            break;
        }
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
                            //SUBTRACT AMOUNT YOU NEED TO PAY
                            int amtToPay = max(0,(&(gameState.D1Showing[i]->cost0))[s] - ps.discounts[s]);
                            (&ps.balance0)[s] -= amtToPay;
                            //If you need to pay more than you have, your balance will be negative
                            //Missing = how many wilds you need to pay
                            int missing = -1*min(0,(&ps.balance0)[s]);
                            //Add amt you need to pay minus how many wilds you pay
                            (&(gameState.bankAmt0))[s] += amtToPay - missing;
                            gameState.bankAmtY += missing;
                            //Correct for wilds
                            ps.balanceY -= missing;
                            (&ps.balance0)[s] = max(0,(&ps.balance0)[s]);
                        }
                        //Add to discount
                        ps.discounts[gameState.D1Showing[i]->suit]++;
                    }
                    flipCard(gameState, i/4, i%4);
                    goto nobleCheck;
                }
            //Card not in flipped cards. Must be purchasing from reserves bc move is valid
            for(int c=0; c<3; c++)
                if(ps.reservedCards[c]->id==ga.id){
                    for (int s=0; s<5; s++) {
                        //SUBTRACT AMOUNT YOU NEED TO PAY
                        int amtToPay = max(0,(&(ps.reservedCards[c]->cost0))[s] - ps.discounts[s]);
                        (&ps.balance0)[s] -= amtToPay;
                        //If you need to pay more than you have, your balance will be negative
                        //Missing = how many wilds you need to pay
                        int missing = -1*min(0,(&ps.balance0)[s]);
                        //Add amt you need to pay minus how many wilds you pay
                        (&(gameState.bankAmt0))[s] += amtToPay - missing;
                        gameState.bankAmtY += missing;
                        //Correct for wilds
                        ps.balanceY -= missing;
                        (&ps.balance0)[s] = max(0,(&ps.balance0)[s]);
                    }
                    int b = 0;
                    while(end[b]!=nullptr)
                        b++;
                    ps.ownedCards[b] = ps.reservedCards[c];
                    //Add to discount
                    ps.discounts[ps.reservedCards[c]->suit]++;
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
void flipCard(GameState& gs, int dNum, int newPos) {
    //POINTS TO START OF DECK
    Card* deck = dNum==0? gs.deck1 : dNum==1? gs.deck2 : gs.deck3;
    Card** row = dNum==0? gs.D1Showing : dNum==1? gs.D2Showing : gs.D3Showing;
    int* index = dNum==0? &gs.iD1 : dNum==1? &gs.iD2 : &gs.iD3;
    if(*index>=40-10*dNum){
        row[newPos] = nullptr;
        return;
    }
    row[newPos] = &(deck[*index]);
    (*index)++;
}
void flipNoble(GameState &gs, int newPos) { //Only used in setup.
    gs.noblesShowing[newPos] = &(gs.nobles[gs.iN]);
    gs.iN++;
}