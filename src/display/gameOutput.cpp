//
// Created by Parker Hitchcock on 11/9/22.
//
#include <iostream>
#include "gameOutput.h"
#include "consoleColors.h"
using namespace std;

void printGS(GameState* gs){
    //system("clear");
    cout << BLD UDL "GAME STATE" RST << endl <<
         "Round " << gs->turn / K_PNUM << " - Player" << gs->turn % K_PNUM << " turn" << endl;
    printBank(gs);
    cout << BLD UDL "Deck1 - " << 40-gs->iD1 << " left" RST << endl;
    for(Card* c : gs->D1Showing)
        printC(c);
    cout << BLD UDL "Deck2 - " << 30-gs->iD2 << " left" RST << endl;
    for(Card* c : gs->D2Showing)
        printC(c);
    cout << BLD UDL "Deck3 - " << 20-gs->iD3 << " left" RST << endl;
    for(Card* c : gs->D3Showing)
        printC(c);
    cout << BLD UDL "Nobles" RST << endl;// - " << 10-gs->iN << " left" RST << endl;
    for(int n=0; n<5&&gs->noblesShowing[n]!=nullptr; n++)
        printN(gs->noblesShowing[n]);
    cout << endl;
    for(PlayerState& ps : gs->playerStates)
        printPS(ps);
}

void printBank(GameState* gs){
    cout << BLD "Bank: " RST <<
        gs->bankAmt0 <<
        " " FG_BLU << gs->bankAmt1 <<
        " " FG_GRN << gs->bankAmt2 <<
        " " FG_RED << gs->bankAmt3 <<
        " " FG_BLK << gs->bankAmt4 <<
        " " FG_YEL << gs->bankAmtY << RST << endl;
}

void printC(Card* card){
    if(card == nullptr)
        return;
    Card& c = *card;
    cout << "[" BLD FG_WHT << c.id
                            //NO SUIT 0 BC IT LOOKS BETTER ON MY MACHINE
        << RST "-" BLD << (c.suit==0?"":c.suit==1?FG_BLU:c.suit==2?FG_GRN:c.suit==3?FG_RED:FG_BLK) << c.points
        << RST ": "
            << c.cost0 <<
            FG_BLU << c.cost1 <<
            FG_GRN << c.cost2 <<
            FG_RED << c.cost3 <<
            FG_BLK << c.cost4 << RST "]" << endl;
}

void printN(Noble* n){
    cout << "[" BLD FG_WHT << n->id
         //NO SUIT 0 BC IT LOOKS BETTER ON MY MACHINE
         << RST "-" BLD <<  n->points
         << RST ": "
         << n->cost0 <<
         FG_BLU << n->cost1 <<
         FG_GRN << n->cost2 <<
         FG_RED << n->cost3 <<
         FG_BLK << n->cost4 << RST "]" << endl;
}

void printPS(PlayerState& ps){
    cout << BLD "Player" << ps.playerNum << ": " RST <<
                                      ps.balance0 <<
         " " FG_BLU << ps.balance1 <<
         " " FG_GRN << ps.balance2 <<
         " " FG_RED << ps.balance3 <<
         " " FG_BLK << ps.balance4 <<
         " " FG_YEL << ps.balanceY << RST << endl;
    for(int i=0; ps.ownedCards[i]!=nullptr; i++) {
        cout << "  ";
        printC(ps.ownedCards[i]);
    }
    if(ps.reservedCards[0]!= nullptr)
        cout << "  " UDL "reserved" RST << endl;
    for(int i=0; ps.reservedCards[i]!=nullptr; i++) {
        cout << "    ";
        printC(ps.reservedCards[i]);
    }
    if(ps.nobles[0]!= nullptr)
        cout << "  " UDL "nobles" RST << endl;
    for(int i=0; ps.nobles[i]!=nullptr; i++) {
        cout << "    ";
        printN(ps.nobles[i]);
    }
}

inline const char* ToString(actionType v) {
    switch (v) {
        case TAKE3: return "TAKE3";
        case TAKE1: return "TAKE1";
        case RESERVE: return "RESERVE";
        case PURCHASE: return "PURCHASE";
        case ERROR: return "ERROR";
    }
}
void printAction(GameAction &pa) {
    cout << ToString(pa.type) << " ";
    switch (pa.type) {
        case TAKE3:
            printTake3(pa);
            return;
        case TAKE1:
            printTake2(pa);
            return;
        case RESERVE:
        case PURCHASE:
            cout << pa.id;
            break;
        case ERROR:
            break;
    }
    cout << endl;
}

void printActions(vector<GameAction>& actions){
    for(GameAction& action : actions)
        printAction(action);
}

void printTake3(GameAction& T3){
    cout << "[ "
         << (T3.suit1==0?"":T3.suit1==1?FG_BLU:T3.suit1==2?FG_GRN:T3.suit1==3?FG_RED:FG_BLK) << T3.suit1 << " " RST
         << (T3.suit2==0?"":T3.suit2==1?FG_BLU:T3.suit2==2?FG_GRN:T3.suit2==3?FG_RED:FG_BLK) << T3.suit2 << " " RST
         << (T3.suit3==0?"":T3.suit3==1?FG_BLU:T3.suit3==2?FG_GRN:T3.suit3==3?FG_RED:FG_BLK) << T3.suit3
         << " " RST "] " << endl;
}

void printTake2(GameAction& T2){
    cout << "[ "
         << (T2.suit1==0?"":T2.suit1==1?FG_BLU:T2.suit1==2?FG_GRN:T2.suit1==3?FG_RED:FG_BLK) << T2.suit1 << RST " ] " << endl;
}