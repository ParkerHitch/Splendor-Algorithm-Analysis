//
// Created by Parker Hitchcock on 11/3/22.
//
#include <iostream>
#include <random>
#include <chrono>
#include <unistd.h>
#include "game.h"
#include "../display/gameOutput.h"

Game::Game(GameState* gs) {
    gameState = *gs;
}

Game *Game::continueFromGS(GameState* gs) {
    Game* g = new Game(gs);
    return g;
}
//Game* Game::newFromGS(GameState& gs){
//    Game* newGame = new Game();
//    newGame->gameState = GameState::newGame(gs);
//    return newGame;
//}

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

Game* Game::randomizePlayerOrder(){
    static auto rnd = default_random_engine(default_random_engine(chrono::system_clock::now().time_since_epoch().count()));
    shuffle(players, end(players), rnd);
    for(int i=0; i<K_PNUM; i++){
        players[i]->id = i;
    }
    return this;
}

int Game::runGame() {
    while (!gameState.isTerminal && !gameState.isStale) {
        takeTurn(false);
    }
    //cout<<gameState.possibleActions.size()<<endl;
    //cout << "Player" << (gameState.turn-1)%4 << " WINS!!" << endl;
    return (gameState.turn-1)%4;
}

int Game::runGameWithReplay() {
    replay.baseGS = gameState;
    replay.allActionsTaken = std::vector<GameAction>();
    while (!gameState.isTerminal && !gameState.isStale) {
        takeTurn(true);
    }
    //cout<<gameState.possibleActions.size()<<endl;
    //cout << "Player" << (gameState.turn-1)%4 << " WINS!!" << endl;
    return (gameState.turn-1)%4;
}

int Game::runGameDebug() {
    printGS(&gameState);
    while (!gameState.isTerminal && !gameState.isStale) {
        sleep(1);
        takeTurn(false);
        printGS(&gameState);
    }
    cout << "Player" << (gameState.turn-1)%4 << " WINS!!" << endl;
    sleep(10);
    return (gameState.turn-1)%4;
}

void Game::takeTurn(bool saveToReplay) {
    GameAction pa;//PLAYER ACTION
    if(gameState.hasValidAction()) {
        //printActions(gameState.possibleActions);
        do {
            pa = players[gameState.turn % K_PNUM]->takeAction(gameState);
        } while (!gameState.isValidAction(pa));
        //printAction(pa);
        if(saveToReplay)
            replay.allActionsTaken.push_back(pa);
        gameState.applyAction(pa);
//        for(Player* p: players){
//            p->updateState(pa);
////            if(instanceof<MonteCarlo>(p)){
////                (dynamic_cast<MonteCarlo*>(p))->printTree();
////            }
//        }
    } else {
        if (gameState.lastAction.playerId == gameState.turn % K_PNUM)
            gameState.isStale=true;
    }
    gameState.isTerminal = gameState.playerStates[pa.playerId].checkWin();
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
