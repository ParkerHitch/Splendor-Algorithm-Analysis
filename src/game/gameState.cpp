//
// Created by Parker Hitchcock on 12/16/22.
//
#include <fstream>
#include <random>
#include "gameState.h"
#include "gameAction.h"

using namespace std;

GameState::GameState(std::ifstream &d1, std::ifstream &d2, std::ifstream &d3, std::ifstream &n) {
    string line;
    getline(d1, line);//Skip comment
    int id = 0;
    while (id<40){
        getline(d1, line);
        deck1[id] = Card::fromLine(line,id);
        id++;
    }

    getline(d2, line);//Skip comment
    while (id<70){
        getline(d2, line);
        deck2[id-40] = Card::fromLine(line,id);
        id++;
    }

    getline(d3, line);//Skip comment
    while (id<90){
        getline(d3, line);
        deck3[id-70] = Card::fromLine(line,id);
        id++;
    }

    id = 0;
    getline(n, line);//Skip comment
    while (id<10){
        getline(n, line);
        nobles[id] = Noble::fromLine(line, id);
        id++;
    }
    for(int i=0; i<4; i++){
        playerStates[i].playerNum = i;
    }
    shuffleAndFlip();
    updatePossibleActions();
}
GameState GameState::newGame(GameState &gs) {
    GameState ng = GameState();
    memcpy(ng.deck1, gs.deck1, sizeof(deck1));
    memcpy(ng.deck2, gs.deck2, sizeof(deck2));
    memcpy(ng.deck3, gs.deck3, sizeof(deck3));
    for(int i=0; i<4; i++){
        ng.playerStates[i].playerNum = i;
    }

    ng.shuffleAndFlip();
    ng.updatePossibleActions();
    return ng;
}
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
    PlayerState &ps = playerStates[ga.playerId];
    switch (ga.type) {
        case ERROR:
            isTerminal = true;
            break;
        case TAKE3:
            if (ga.suit1!=-1) {
                int d1 = min(1, (&bankAmt0)[ga.suit1]); // if bank amt = 0 d1 = 0 else d1 = 1
                (&bankAmt0)[ga.suit1] -= d1;
                (&ps.balance0)[ga.suit1] += d1;
            }
            if(ga.suit2!=-1) {
                int d2 = min(1, (&bankAmt0)[ga.suit2]);
                (&bankAmt0)[ga.suit2] -= d2;
                (&ps.balance0)[ga.suit2] += d2;
            }
            if(ga.suit3!=-1) {
                int d3 = min(1, (&bankAmt0)[ga.suit3]);
                (&bankAmt0)[ga.suit3] -= d3;
                (&ps.balance0)[ga.suit3] += d3;
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
                if (D1Showing[i] == nullptr)
                    continue;
                if (D1Showing[i]->id == ga.id) {
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
                    }
                    flipCard(i / 4, i % 4);
                    goto nobleCheck;
                }
            }
            //Card not in flipped cards. Must be purchasing from reserves bc move is valid
            for (int c = 0; c < 3; c++)
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
                    //Remove & shift left
                    ps.reservedCards[c] = nullptr;
                    while (c < 2) {
                        ps.reservedCards[c] = ps.reservedCards[c + 1];
                        c++;
                    }
                    ps.reservedCards[2] = nullptr;
                }
            break;
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
                break;
            }
        //Remove & shift left
        noblesShowing[c] = nullptr;
        while (c < 4) {
            noblesShowing[c] = noblesShowing[c + 1];
            c++;
        }
        noblesShowing[4] = nullptr;
        break; //ONE PER TURN
        nextN:;
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
    Card *deck = dNum == 0 ? deck1 : dNum == 1 ? deck2 : deck3;
    Card **row = dNum == 0 ? D1Showing : dNum == 1 ? D2Showing : D3Showing;
    int *index = dNum == 0 ? &iD1 : dNum == 1 ? &iD2 : &iD3;
    if (*index >= 40 - 10 * dNum) {
        row[newPos] = nullptr;
        return;
    }
    row[newPos] = &(deck[*index]);
    (*index)++;
}
void GameState::flipNoble(int newPos) {
    noblesShowing[newPos] = &(nobles[iN]);
    iN++;
}

void GameState::advanceTurn() {
    turn++;
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
    int b = 0;
    int pts = 0;
    while (ownedCards[b] != nullptr)
        pts += ownedCards[b++]->points;//Add points & increment id
    b = 0;
    while (b < 5 && nobles[b] != nullptr)
        pts += nobles[b++]->points;//Add points & increment id
    return pts >= 15;
}
bool PlayerState::canAfford(Card &card) {
    int diff = 0; //# of coins missing
    for (int suit = 0; suit < 5; suit++)
        diff += max(0, (&card.cost0)[suit] - discounts[suit] - //HAVE TO PAY
                       (&balance0)[suit]); //HAVE IN BANK
    return diff <= balanceY;
}