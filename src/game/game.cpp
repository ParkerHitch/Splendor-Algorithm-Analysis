//
// Created by Parker Hitchcock on 11/3/22.
//
#include <iostream>
#include <algorithm>
#include <random>
#include "game.h"
#include "../display/gameOutput.h"

Game* Game::fromFiles(ifstream &d1, ifstream &d2, ifstream &d3, ifstream &n) {
    Game* newGame = new Game();
    newGame->gameState = newGameFromFilesShuffled(d1, d2, d3, n);
    return newGame;
}

Game* Game::fromGS(GameState* gs){
    Game* newGame = new Game();
    newGame->gameState = *gs;
    return newGame;
}

Game* Game::usePlayer(int n, Player *player) {
    players[n] = player;
    return this;
}

Game* Game::usePlayers(Player* pls[K_PNUM]) {
    players[0] = pls[0];
    players[1] = pls[1];
    players[2] = pls[2];
    players[3] = pls[3];
    return this;
}

int Game::runGame() {
    while (!gameState.isTerminal && !gameState.isStale) {
        takeTurn();
        gameState.turn++;
    }
    //cout << "Player" << (gameState.turn-1)%4 << " WINS!!" << endl;
    return (gameState.turn-1)%4;
}

int Game::runGameDebug() {
    printGS(gameState);
    while (!gameState.isTerminal && !gameState.isStale) {
        takeTurn();
        gameState.turn++;
        printGS(gameState);
    }
    cout << "Player" << (gameState.turn-1)%4 << " WINS!!" << endl;
    return (gameState.turn-1)%4;
}

void Game::takeTurn() {
    GameAction pa;//PLAYER ACTION
    possibleActions = validActions(gameState);
    if(!possibleActions.empty()) {
        //printActions(possibleActions);
        staleStreak = 0;
        do {
            pa = players[gameState.turn % K_PNUM]->takeAction(gameState);
        } while (!validAction(pa));
        applyAction(pa);
    } else
        staleStreak ++;
    gameState.isTerminal = checkWin(gameState.playerStates[pa.playerId]);
    if (staleStreak>=4)
        gameState.isStale=true;
}

bool Game::checkWin(PlayerState &ps) {
    int b = 0;
    int pts = 0;
    while (ps.ownedCards[b] != nullptr)
        pts += ps.ownedCards[b++]->points;//Add points & increment id
    b = 0;
    while (b < 5 && ps.nobles[b] != nullptr)
        pts += ps.nobles[b++]->points;//Add points & increment id
    return pts >= 15;
}

bool Game::validAction(GameAction &ga) {
    return any_of(possibleActions.begin(), possibleActions.end(), [&ga](GameAction a){return a==ga;});
//    switch (ga.type) {
//        case ERROR:
//            return true;
//        case TAKE3:
//            return ga.suit1 != ga.suit2 && ga.suit2 != ga.suit3 && ga.suit1 != ga.suit3;// &&
////                    (&gameState.bankAmt0)[ga.suit1] > 0 &&
////                    (&gameState.bankAmt0)[ga.suit2] > 0 &&
////                    (&gameState.bankAmt0)[ga.suit3] > 0;
//        case TAKE1:
//            return (&gameState.bankAmt0)[ga.suit1] >= 4;//Minimum for taking two (game rule)
//        case RESERVE:
//            if (gameState.playerStates[ga.playerId].reservedCards[2] != nullptr)
//                return false;
//        case PURCHASE:
//            for (int c = 0; c < 12; c++) {
//                if (gameState.D1Showing[c] == nullptr)
//                    continue;
//                if (gameState.D1Showing[c]->id == ga.id) {
//                    if (ga.type == PURCHASE)
//                        return canAfford(gameState.playerStates[ga.playerId], *gameState.D1Showing[c]);
//                    return true;//Found in flipped and reserving
//                }
//            }
//            //Card is not flipped on table. If we are buying it could be in our reserved
//            if (ga.type == PURCHASE)
//                for (Card *card: gameState.playerStates[ga.playerId].reservedCards) {
//                    if (card == nullptr)
//                        return false;
//                    if (card->id == ga.id) {
//                        return canAfford(gameState.playerStates[ga.playerId], *card);
//                    }
//                }
//            return false;
//    }
}

bool Game::canAfford(PlayerState &ps, Card &card) {
    int diff = 0; //# of coins missing
    for (int suit = 0; suit < 5; suit++)
        diff += max(0, (&card.cost0)[suit] - ps.discounts[suit] - //HAVE TO PAY
                       (&ps.balance0)[suit]); //HAVE IN BANK
    return diff <= ps.balanceY;
}

void Game::applyAction(GameAction &ga) {
    PlayerState &ps = gameState.playerStates[ga.playerId];
    switch (ga.type) {
        case ERROR:
            gameState.isTerminal = true;
            break;
        case TAKE3:
            if (ga.suit1!=-1) {
                int d1 = min(1, (&gameState.bankAmt0)[ga.suit1]); // if bank amt = 0 d1 = 0 else d1 = 1
                (&gameState.bankAmt0)[ga.suit1] -= d1;
                (&ps.balance0)[ga.suit1] += d1;
            }
            if(ga.suit2!=-1) {
                int d2 = min(1, (&gameState.bankAmt0)[ga.suit2]);
                (&gameState.bankAmt0)[ga.suit2] -= d2;
                (&ps.balance0)[ga.suit2] += d2;
            }
            if(ga.suit3!=-1) {
                int d3 = min(1, (&gameState.bankAmt0)[ga.suit3]);
                (&gameState.bankAmt0)[ga.suit3] -= d3;
                (&ps.balance0)[ga.suit3] += d3;
            }
            break;
        case TAKE1:
            (&gameState.bankAmt0)[ga.suit1] -= 2;
            (&ps.balance0)[ga.suit1] += 2;
            break;
        case RESERVE:
            if (gameState.bankAmtY > 0) {
                ps.balanceY += 1;
                gameState.bankAmtY -= 1;
            }
        case PURCHASE:
            Card **end = ga.type == PURCHASE ? ps.ownedCards : ps.reservedCards;//Where the card ends up
            for (int i = 0; i < 12; i++) {
                if (gameState.D1Showing[i] == nullptr)
                    continue;
                if (gameState.D1Showing[i]->id == ga.id) {
                    int b = 0;
                    while (end[b] != nullptr)
                        b++;
                    //Give player card
                    end[b] = gameState.D1Showing[i];
                    if (ga.type == PURCHASE) {
                        //Subtract cost from player. Assumed that player wants to conserve wilds.
                        for (int s = 0; s < 5; s++) {
                            //SUBTRACT AMOUNT YOU NEED TO PAY
                            int amtToPay = max(0, (&(gameState.D1Showing[i]->cost0))[s] - ps.discounts[s]);
                            (&ps.balance0)[s] -= amtToPay;
                            //If you need to pay more than you have, your balance will be negative
                            //Missing = how many wilds you need to pay
                            int missing = -1 * min(0, (&ps.balance0)[s]);
                            //Add amt you need to pay minus how many wilds you pay
                            (&(gameState.bankAmt0))[s] += amtToPay - missing;
                            gameState.bankAmtY += missing;
                            //Correct for wilds
                            ps.balanceY -= missing;
                            (&ps.balance0)[s] = max(0, (&ps.balance0)[s]);
                        }
                        //Add to discount
                        ps.discounts[gameState.D1Showing[i]->suit]++;
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
                        (&(gameState.bankAmt0))[s] += amtToPay - missing;
                        gameState.bankAmtY += missing;
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
        if (gameState.noblesShowing[c] == nullptr)
            break;

        for (int s = 0; s < 5; s++)
            if (ps.discounts[s] < (&(gameState.noblesShowing[c]->cost0))[s])
                goto nextN;
        //Noble attracted
        for (Noble *&pn: ps.nobles)
            if (pn == nullptr) {
                pn = gameState.noblesShowing[c];
                break;
            }
        //Remove & shift left
        gameState.noblesShowing[c] = nullptr;
        while (c < 4) {
            gameState.noblesShowing[c] = gameState.noblesShowing[c + 1];
            c++;
        }
        gameState.noblesShowing[4] = nullptr;
        break; //ONE PER TURN
        nextN:;
    }
}

//HELPER UNIMPORTANT BAD FUNCTIONS BELOW THIS LINE. WE DISCRIMINATE!

Game* Game::shuffleDecks(){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    auto rnd = default_random_engine(default_random_engine(seed));
    shuffle((gameState.deck1), end(gameState.deck1), rnd);
    shuffle((gameState.deck2), end(gameState.deck2), rnd);
    shuffle((gameState.deck3), end(gameState.deck3), rnd);
    shuffle((gameState.nobles), end(gameState.nobles), rnd);
    return this;
}

void Game::flipCard(int dNum, int newPos) {
    //POINTS TO START OF DECK
    Card *deck = dNum == 0 ? gameState.deck1 : dNum == 1 ? gameState.deck2 : gameState.deck3;
    Card **row = dNum == 0 ? gameState.D1Showing : dNum == 1 ? gameState.D2Showing : gameState.D3Showing;
    int *index = dNum == 0 ? &gameState.iD1 : dNum == 1 ? &gameState.iD2 : &gameState.iD3;
    if (*index >= 40 - 10 * dNum) {
        row[newPos] = nullptr;
        return;
    }
    row[newPos] = &(deck[*index]);
    (*index)++;
}

void Game::flipNoble(int newPos) { //Only used in setup.
    gameState.noblesShowing[newPos] = &(gameState.nobles[gameState.iN]);
    gameState.iN++;
}

//EVEN WORSE... GETTERS AND SETTERS 😱
int Game::getTurn() {
    return gameState.turn;
}
bool Game::hasWinner() {
    return gameState.isTerminal;
}