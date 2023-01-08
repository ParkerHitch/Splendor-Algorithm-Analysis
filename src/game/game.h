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
    static Game* newFromFiles(ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n);
    static Game* newFromGS(GameState& gs);
    Game* usePlayer(int n, Player* player);
    Game* usePlayers(Player* players[K_PNUM]);

    int runGame();
    int runGameDebug();

    int getTurn();
    bool hasWinner();

private:
    GameState gameState;
    Player* players[K_PNUM];

    void takeTurn();
};

#endif //RESEARCH_GAME_H
