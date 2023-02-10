//
// Created by Parker Hitchcock on 12/16/22.
//
#include <random>
#include <iostream>
#include <chrono>
#include "gameState.h"
#include "gameAction.h"
#include "../display/gameOutput.h"

using namespace std;

GameState::GameState() {
    for(int i=0; i<90; i++){
        deck1[i] = &(GameState::data->deck1[i]);
    }
    for(int i=0; i<10; i++){
        nobles[i] = &(GameState::data->nobles[i]);
    }
    for(int i=0; i<4; i++){
        playerStates[i].playerNum = i;
    }
    shuffleAndFlip();
    updatePossibleActions();
}

//GameState GameState::newGame(GameState &gs) {
//    GameState ng = GameState();
//    memcpy(ng.deck1, gs.deck1, sizeof(deck1));
//    memcpy(ng.deck2, gs.deck2, sizeof(deck2));
//    memcpy(ng.deck3, gs.deck3, sizeof(deck3));
//    memcpy(ng.nobles, gs.nobles, sizeof(deck3));
//    for(int i=0; i<4; i++){
//        ng.playerStates[i].playerNum = i;
//    }
//
//    ng.shuffleAndFlip();
//    ng.updatePossibleActions();
//    return ng;
//}
void GameState::shuffleAndFlip(){
    static auto rnd = default_random_engine(default_random_engine(chrono::system_clock::now().time_since_epoch().count()));
    shuffle((deck1), end(deck1), rnd);
    shuffle((deck2), end(deck2), rnd);
    shuffle((deck3), end(deck3), rnd);
    shuffle((nobles), end(nobles), rnd);
    for (int d = 0; d < 3; d++)
        for (int p = 0; p < 4; p++)
            flipCard(d, p);
    for (int i = 0; i < 5; i++)
        flipNoble(i);
}

void GameState::applyAction(GameAction &ga) {
    lastAction = ga;
    PlayerState &ps = playerStates[ga.playerId];
    switch (ga.type) {
        case ERROR:
            isTerminal = true;
            break;
        case TAKE3:
            if (ga.suit1!=-1) {
                (&bankAmt0)[ga.suit1] -= 1;
                (&ps.balance0)[ga.suit1] += 1;
            }
            if(ga.suit2!=-1) {
                (&bankAmt0)[ga.suit2] -= 1;
                (&ps.balance0)[ga.suit2] += 1;
            }
            if(ga.suit3!=-1) {
                (&bankAmt0)[ga.suit3] -= 1;
                (&ps.balance0)[ga.suit3] += 1;
            }
            break;
        case TAKE1:
            (&bankAmt0)[ga.suit1] -= 2;
            (&ps.balance0)[ga.suit1] += 2;
            break;
        case RESERVE:
            if (bankAmtY > 0) {
                ps.balanceY += 1;
                bankAmtY -= 1;
            }
        case PURCHASE:
            Card **end = ga.type == PURCHASE ? ps.ownedCards : ps.reservedCards;//Where the card ends up
            for (int i = 0; i < 12; i++) {
                if (D1Showing[i] != nullptr && D1Showing[i]->id == ga.id) {
                    int b = 0;
                    while (end[b] != nullptr)
                        b++;
                    //Give player card
                    end[b] = D1Showing[i];
                    if (ga.type == PURCHASE) {
                        //Subtract cost from player. Assumed that player wants to conserve wilds.
                        for (int s = 0; s < 5; s++) {
                            //SUBTRACT AMOUNT YOU NEED TO PAY
                            int amtToPay = max(0, (&(D1Showing[i]->cost0))[s] - ps.discounts[s]);
                            (&ps.balance0)[s] -= amtToPay;
                            //If you need to pay more than you have, your balance will be negative
                            //Missing = how many wilds you need to pay
                            int missing = -1 * min(0, (&ps.balance0)[s]);
                            //Add amt you need to pay minus how many wilds you pay
                            (&(bankAmt0))[s] += amtToPay - missing;
                            bankAmtY += missing;
                            //Correct for wilds
                            ps.balanceY -= missing;
                            (&ps.balance0)[s] = max(0, (&ps.balance0)[s]);
                        }
                        //Add to discount
                        ps.discounts[D1Showing[i]->suit]++;
                        ps.pts++;
                        flipCard(i / 4, i % 4);
                        goto nobleCheck;
                    } else {
                        flipCard(i / 4, i % 4);
                        return;
                    }
                }
            }
            //Card not in flipped cards. Must be purchasing from reserves bc move is valid
            for (int c = 0; c < 3; c++) {
                if (ps.reservedCards[c]->id == ga.id) {
                    for (int s = 0; s < 5; s++) {
                        //SUBTRACT AMOUNT YOU NEED TO PAY
                        int amtToPay = max(0, (&(ps.reservedCards[c]->cost0))[s] - ps.discounts[s]);
                        (&ps.balance0)[s] -= amtToPay;
                        //If you need to pay more than you have, your balance will be negative
                        //Missing = how many wilds you need to pay
                        int missing = -1 * min(0, (&ps.balance0)[s]);
                        //Add amt you need to pay minus how many wilds you pay
                        (&(bankAmt0))[s] += amtToPay - missing;
                        bankAmtY += missing;
                        //Correct for wilds
                        ps.balanceY -= missing;
                        (&ps.balance0)[s] = max(0, (&ps.balance0)[s]);
                    }
                    int b = 0;
                    while (end[b] != nullptr)
                        b++;
                    ps.ownedCards[b] = ps.reservedCards[c];
                    //Add to discount
                    ps.discounts[ps.reservedCards[c]->suit]++;
                    ps.pts++;
                    //Remove & shift left
                    ps.reservedCards[c] = nullptr;
                    while (c < 2) {
                        ps.reservedCards[c] = ps.reservedCards[c + 1];
                        c++;
                    }
                    ps.reservedCards[2] = nullptr;
                }
            }
            //Check for Noble visits
            nobleCheck:
            for (int c = 0; c < 5; c++) {
                if (noblesShowing[c] == nullptr)
                    break;

                for (int s = 0; s < 5; s++)
                    if (ps.discounts[s] < (&(noblesShowing[c]->cost0))[s])
                        goto nextN;
                //Noble attracted
                for (Noble *&pn: ps.nobles)
                    if (pn == nullptr) {
                        pn = noblesShowing[c];
                        ps.pts += pn->points;
                        break;
                    }
                //Remove & shift left
                noblesShowing[c] = nullptr;
                while (c < 4) {
                    noblesShowing[c] = noblesShowing[c + 1];
                    c++;
                }
                noblesShowing[4] = nullptr;
                //I am removing this rule for coding's sake break; //ONE PER TURN
                nextN:;
            }
            break;
    }
}

void GameState::undo(GameAction &ga) {
    reduceTurn(); // Now the turn of the player who took the action to undo
    PlayerState &ps = playerStates[ga.playerId];
    switch (ga.type) {
        case ERROR:
            //WTF?!?!
            isTerminal = true;
            break;
        case TAKE3:
            if (ga.suit1!=-1) {
                (&ps.balance0)[ga.suit1] -= 1;
                (&bankAmt0)[ga.suit1] += 1;
            }
            if(ga.suit2!=-1) {
                (&ps.balance0)[ga.suit2] -= 1;
                (&bankAmt0)[ga.suit2] += 1;
            }
            if(ga.suit3!=-1) {
                (&ps.balance0)[ga.suit3] -= 1;
                (&bankAmt0)[ga.suit3] += 1;
            }
            break;
        case TAKE1:
            (&ps.balance0)[ga.suit1] -= 2;
            (&bankAmt0)[ga.suit1] += 2;
            break;
        case RESERVE:
            bankAmtY += ga.suit2;
            ps.balanceY -= ga.suit2;
            //Remove from player's reserved cards
            //(Assuming that we are undoing the most recent action)
            for (int i = 2; i >= 0; i--) {
                if (ps.reservedCards[i] != nullptr) {
                    Card* card = ps.reservedCards[i];
                    ps.reservedCards[i] = nullptr;
                    D1Showing[ga.suit1] = card; // Put card back onto table
                    ((&iD1)[ga.suit1/4])--; // Decrease index
                    return;
                }
            }
            break;
        case PURCHASE:
            //Remove from owned cards
            int nCards = 0;
            for(int c : ps.discounts){
                nCards += c;
            }
            Card* card = ps.ownedCards[nCards-1];
            ps.discounts[card->suit]--;
            ps.pts--;
            ps.ownedCards[nCards-1] = nullptr;
            for (int s = 0; s < 6; s++) {
                int amtPaid = (&bankAmt0)[s] - (ga.suit2>>((5-s)*3))&7;
                (&ps.balance0)[s] += amtPaid;
                (&bankAmt0)[s] -= amtPaid;
            }
            //Remove nobles
            for(int i=0; ps.nobles[i]!=nullptr;){
                if (ps.discounts[card->suit] < (&(ps.nobles[i]->cost0))[card->suit]) {
                    //Put noble back
                    for(auto & n : noblesShowing){
                        if(n == nullptr) {
                            n = ps.nobles[i];
                            break;
                        }
                    }
                    int c = i;
                    ps.pts -= ps.nobles[c]->points;
                    ps.nobles[c] = nullptr;
                    while (c < 4) {
                        ps.nobles[c] = ps.nobles[c + 1];
                        c++;
                    }
                    ps.nobles[4] = nullptr;
                } else {
                    i++;
                }
            }
            if(ga.suit1 > 11){
                for(int p=2; p>ga.suit1-12; p--)
                    ps.reservedCards[p] = ps.reservedCards[p-1];
                ps.reservedCards[ga.suit1-12] = card;
            } else {
                D1Showing[ga.suit1] = card; // Put card back onto table
                ((&iD1)[ga.suit1/4])--; // Decrease index
            }
    }
}

void GameState::updatePossibleActions() {
    possibleActions = validActions(*this);
}
bool GameState::isValidAction(GameAction &ga) {
    return any_of(possibleActions.begin(), possibleActions.end(), [&ga](GameAction a) { return a == ga; });
}
bool GameState::hasValidAction() const {
    return !possibleActions.empty();
}

void GameState::flipCard(int dNum, int newPos) {
    Card **deck = dNum == 0 ? deck1 : dNum == 1 ? deck2 : deck3;
    Card **row = dNum == 0 ? D1Showing : dNum == 1 ? D2Showing : D3Showing;
    int *index = dNum == 0 ? &iD1 : dNum == 1 ? &iD2 : &iD3;
//    if (*index >= 40 - 10 * dNum) {
//        row[newPos] = nullptr;
//        return;
//    }
    row[newPos] = *index >= 40 - 10 * dNum ? nullptr: deck[*index];
    (*index)++;
}
void GameState::flipNoble(int newPos) {
    noblesShowing[newPos] = nobles[iN];
    iN++;
}

void GameState::advanceTurn() {
    turn++;
}
void GameState::reduceTurn() {
    turn--;
}

Card Card::fromLine(const string &line, int id) {
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
Noble Noble::fromLine(const string &line, int id) {
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

//######### PLAYER #########

bool PlayerState::checkWin() {
    return pts >= 15;
}
bool PlayerState::canAfford(Card* card) {
    int diff = 0; //# of coins missing
    for (int suit = 0; suit < 5; suit++)
        diff += max(0, (&(card->cost0))[suit] - discounts[suit] - //HAVE TO PAY
                       (&balance0)[suit]); //HAVE IN BANK
    return diff <= balanceY;
}