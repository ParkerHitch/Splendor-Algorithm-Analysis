#include <iostream>
#include <string>
#include "game/game.h"
using namespace std;

const string assetsDir = "../assets/";

int main() {
    ifstream deck1(assetsDir + "default/decks/1.csv");
    ifstream deck2(assetsDir + "default/decks/2.csv");
    ifstream deck3(assetsDir + "default/decks/3.csv");
    ifstream noble(assetsDir + "default/nobles.csv");

    GameState baseGS;

    if (deck1.is_open() && deck2.is_open() && deck3.is_open() && noble.is_open()) {
        baseGS = newGameFromFilesShuffled(deck1, deck2, deck3, noble);
    } else {
        cout << "Unable to open file(s)";
        return 0;
    }

    Player* players[K_PNUM];
    for(int i=0; i < K_PNUM; i++)
        players[i] = new RandomPlayer(i);

    Game::fromGS(&baseGS)->usePlayers(players)->runGame();

    for(Player* p : players)
        free(p);

    return 0;
}
