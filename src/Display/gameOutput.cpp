//
// Created by Parker Hitchcock on 11/9/22.
//
#include <iostream>
#include "gameOutput.h"
#include "consoleColors.h"

using namespace std;

void printGS(GameState& gs){
    system("clear");
    cout << BLD UDL "GAME STATE" RST << endl <<
        "Round " << gs.turn/4 <<  " - Player" << gs.turn%4 << " turn" << endl;
    printBank(gs);
    cout << BLD UDL "Deck1 - " << 40-gs.iD1 << " left" RST << endl;
    for(Card*& c : gs.D1Showing)
        printC(*c);
    cout << BLD UDL "Deck2 - " << 30-gs.iD2 << " left" RST << endl;
    for(Card*& c : gs.D2Showing)
        printC(*c);
    cout << BLD UDL "Deck3 - " << 20-gs.iD3 << " left" RST << endl;
    for(Card*& c : gs.D3Showing)
        printC(*c);
    cout << BLD UDL "Nobles - " << 10-gs.iN << " left" RST << endl;
    for(Noble*& n : gs.NoblesShowing)
        printN(*n);
    cout << endl;
    for(PlayerState& ps : gs.playerStates)
        printPS(ps);
}

void printBank(GameState& gs){
    cout << BLD "Bank: " RST <<
        gs.bankAmt0 <<
        " " FG_BLU << gs.bankAmt1 <<
        " " FG_GRN << gs.bankAmt2 <<
        " " FG_RED << gs.bankAmt3 <<
        " " FG_BLK << gs.bankAmt4 <<
        " " FG_YEL << gs.bankAmtY << RST << endl;
}

void printC(Card& c){
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

void printN(Noble& n){
    cout << "[" BLD FG_WHT << n.id
         //NO SUIT 0 BC IT LOOKS BETTER ON MY MACHINE
         << RST "-" BLD <<  n.points
         << RST ": "
         << n.cost0 <<
         FG_BLU << n.cost1 <<
         FG_GRN << n.cost2 <<
         FG_RED << n.cost3 <<
         FG_BLK << n.cost4 << RST "]" << endl;
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
        printC(*ps.ownedCards[i]);
    }
    if(ps.reservedCards[0]!= nullptr)
        cout << "  " UDL "reserved" RST << endl;
    for(int i=0; ps.reservedCards[i]!=nullptr; i++) {
        cout << "    ";
        printC(*ps.reservedCards[i]);
    }
}