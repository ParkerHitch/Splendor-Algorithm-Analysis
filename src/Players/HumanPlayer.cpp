//
// Created by Parker Hitchcock on 11/14/22.
//
#include <iostream>

#include "Player.h"

using namespace std;

Player::Player(int i) : id(i) {};

HumanPlayer::HumanPlayer(int i) : Player(i) {};

GameAction HumanPlayer::takeAction(GameState &gs) {
    GameAction ga = {.playerId = id};
    cout << "Player" << id << " turn." << endl;
    cout << "(0->P3,1->P1,2->R,3->B,4->EXIT) Enter action type (0-4): ";
    int type;
    cin >> type;
    ga.type = actionType(type);
    bool valid = false;
    switch (ga.type) {
        case TAKE3:
            while (!valid) {
                cout << "(0->w,1->u,2->g,3->r,4->k) Enter 1st coin suit (0-4): ";
                cin >> ga.suit1;
                valid = ga.suit1 > -1 && ga.suit1 < 5;
            }
            valid = false;
            while (!valid) {
                cout << "(0->w,1->u,2->g,3->r,4->k) Enter 2nd coin suit (0-4): ";
                cin >> ga.suit2;
                valid = ga.suit2 > -1 && ga.suit2 < 5 && ga.suit2!=ga.suit1;
            }
            valid = false;
            while (!valid) {
                cout << "(0->w,1->u,2->g,3->r,4->k) Enter 3rd coin suit (0-4): ";
                cin >> ga.suit3;
                valid = ga.suit3 > -1 && ga.suit3 < 5 && ga.suit3!=ga.suit1 && ga.suit3!=ga.suit2;
            }
            break;
        case TAKE1:
            while (!valid) {
                cout << "(0->w,1->u,2->g,3->r,4->k) Enter suit of 2 coins (0-4): ";
                cin >> ga.suit1;
                valid = ga.suit1 > -1 && ga.suit1 < 5;
            }
            break;
        case RESERVE:
            cout << "Enter ID of card to reserve: ";
            goto in;
        case PURCHASE:
            cout << "Enter ID of card to purchase: ";
            in:
            cin >> ga.id;
            break;
        case ERROR:
            break;
    }
    return ga;
}