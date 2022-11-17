//
// Created by Parker Hitchcock on 11/3/22.
//
#include <fstream>

#ifndef RESEARCH_GAMESTATE_H
#define RESEARCH_GAMESTATE_H

struct Card {
    int id;
    int suit;
    int points;
    int cost0;
    int cost1;
    int cost2;
    int cost3;
    int cost4;
};

struct Noble {
    int id;
    int points;
    int cost0;
    int cost1;
    int cost2;
    int cost3;
    int cost4;
};
const Noble* findNoble(int id);

struct PlayerState {
    int playerNum;
    Card* ownedCards[50];//IDs of cards. Max 50 before u get 15 pts
    Card* reservedCards[3];
    int balance0;
    int balance1;
    int balance2;
    int balance3;
    int balance4;
    int balanceY;
    int* balance = &balance0;
};

struct GameState {
    bool isTerminal = false;//WIN

    Card deck1[40]{};
    int iD1 = 0; //Which card to draw next. NOT ID. LIST INDEX
    Card deck2[30]{};
    int iD2 = 0; //Which card to draw next. NOT ID. LIST INDEX
    Card deck3[20]{};
    int iD3 = 0; //Which card to draw next. NOT ID. LIST INDEX

    Card* D1Showing[4]{};
    Card* D2Showing[4]{};
    Card* D3Showing[4]{};

    Noble nobles[10]{};
    int iN = 0;
    Noble* NoblesShowing[5]{};
    int     bankAmt0 = 7,
            bankAmt1 = 7,
            bankAmt2 = 7,
            bankAmt3 = 7,
            bankAmt4 = 7,
            bankAmtY = 5;
    int* bank = &bankAmt0;

    PlayerState playerStates[4]{};

    int turn = 0;
};

#endif //RESEARCH_GAMESTATE_H