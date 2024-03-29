//
// Created by Parker Hitchcock on 11/3/22.
//
#ifndef RESEARCH_GAME_H
#define RESEARCH_GAME_H

using namespace std;

#include "gameDependencies.h"
#include "../players/Player.h"
#include "../utils/config.h"

class Game {
public:
    static Game* continueFromGS(GameState* gs);
    Game* usePlayer(int n, Player* player);
    Game* usePlayers(Player* players[K_PNUM]);
    Game* randomizePlayerOrder();

    int runGame();
    int runGameDebug();
    int runGameWithReplay();

    int getTurn();
    bool hasWinner();

    template<typename type>
    bool playerIs(int id){
        return (dynamic_cast<type*>(players[id]))!=nullptr;
    }

    Game() = default;

    gameReplay replay;

private:
    Game(GameState* gs);

    GameState gameState;
    Player* players[K_PNUM];

    void takeTurn(bool saveToReplay);
};

#endif //RESEARCH_GAME_H
