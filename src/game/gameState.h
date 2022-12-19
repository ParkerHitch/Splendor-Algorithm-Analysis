//
// Created by Parker Hitchcock on 11/3/22.
//

#ifndef RESEARCH_GAMESTATE_H
#define RESEARCH_GAMESTATE_H

using namespace std;

#include <vector>
#include "../utils/config.h"
#include "gameAction.h"

struct GameAction;

struct Card {
    int id;
    int suit;
    int points;
    int cost0;
    int cost1;
    int cost2;
    int cost3;
    int cost4;
    static Card fromLine(const string& line, int id);
};

struct Noble {
    int id;
    int points;
    int cost0;
    int cost1;
    int cost2;
    int cost3;
    int cost4;
    static Noble fromLine(const string& line, int id);
};

struct PlayerState {
    int playerNum;
    Card* ownedCards[50];//IDs of cards. Max 50 before u get 15 pts
    int discounts[5]={};
    Card* reservedCards[3];
    Noble* nobles[5];//Max
    int balance0 = 0;
    int balance1 = 0;
    int balance2 = 0;
    int balance3 = 0;
    int balance4 = 0;
    int balanceY = 0;

    bool canAfford(Card& card);
    bool checkWin();
};

struct GameState {
    bool isTerminal = false;//WIN
    bool isStale = false;

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
    Noble* noblesShowing[5]{};
    int     bankAmt0 = 7,
            bankAmt1 = 7,
            bankAmt2 = 7,
            bankAmt3 = 7,
            bankAmt4 = 7,
            bankAmtY = 5;

    PlayerState playerStates[K_PNUM]{};
    int turn = 0;

    vector<GameAction> possibleActions;
    int staleStreak = 0;

    //METHODS:
    void updatePossibleActions();
    bool hasValidAction() const;
    bool isValidAction(GameAction& ga);
    void applyAction(GameAction& ga);
    void advanceTurn();

    static GameState newGame(GameState& gs);
    //Implicit Copy Constructor: GameState(GameState& gs);
    GameState(ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n);
    GameState()=default;

private:


    void shuffleAndFlip();

    void flipCard(int dNum, int newPos);
    void flipNoble(int newPos);
};

#endif //RESEARCH_GAMESTATE_H