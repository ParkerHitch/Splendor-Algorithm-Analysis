//
// Created by Parker Hitchcock on 11/3/22.
//
#include <iostream>
#include "game.h"
#include "../display/gameOutput.h"

Game* Game::newFromFiles(ifstream &d1, ifstream &d2, ifstream &d3, ifstream &n) {
    Game* newGame = new Game();
    newGame->gameState = GameState(d1, d2, d3, n);
    return newGame;
}
Game* Game::newFromGS(GameState& gs){
    Game* newGame = new Game();
    newGame->gameState = GameState::newGame(gs);
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
    }
    //cout << "Player" << (gameState.turn-1)%4 << " WINS!!" << endl;
    return (gameState.turn-1)%4;
}

int Game::runGameDebug() {
    printGS(gameState);
    while (!gameState.isTerminal && !gameState.isStale) {
        takeTurn();
        printGS(gameState);
    }
    cout << "Player" << (gameState.turn-1)%4 << " WINS!!" << endl;
    return (gameState.turn-1)%4;
}

void Game::takeTurn() {
    GameAction pa;//PLAYER ACTION
    if(gameState.hasValidAction()) {
        //printActions(possibleActions);
        gameState.staleStreak = 0;
        do {
            pa = players[gameState.turn % K_PNUM]->takeAction(gameState);
        } while (!gameState.isValidAction(pa));
        gameState.applyAction(pa);
    } else
        gameState.staleStreak ++;
    gameState.isTerminal = gameState.playerStates[pa.playerId].checkWin();
    if (gameState.staleStreak>=4)
        gameState.isStale=true;
    gameState.advanceTurn();
    gameState.updatePossibleActions();
}

//EVEN WORSE... GETTERS AND SETTERS 😱
int Game::getTurn() {
    return gameState.turn;
}
bool Game::hasWinner() {
    return gameState.isTerminal;
}