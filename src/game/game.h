//
// Created by Parker Hitchcock on 11/3/22.
//
#ifndef RESEARCH_GAME_H
#define RESEARCH_GAME_H

using namespace std;

#include "gameState.h"
#include "gameAction.h"
#include "../players/Player.h"
#include "gameSetup.h"
#include "../utils/config.h"

class Game {
public:
    static Game* fromFiles(ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n);
    static Game* fromGS(GameState* gs);
    Game* usePlayer(int n, Player* player);
    Game* usePlayers(Player* players[K_PNUM]);
    Game* shuffleDecks();

    int runGame();
    int runGameDebug();

    int getTurn();
    bool hasWinner();

private:
    Game() = default;

    GameState gameState = GameState{};
    Player* players[K_PNUM];

    void takeTurn();
    void applyAction(GameAction& ga);
    bool validAction(GameAction& ga);
    static bool canAfford(PlayerState& ps, Card& card);
    static bool checkWin(PlayerState &ps);

    void flipCard(int dNum, int newPos);
    void flipNoble(int newPos);
};

#endif //RESEARCH_GAME_H
