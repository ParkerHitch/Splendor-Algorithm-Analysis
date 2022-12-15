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

    int results[K_PNUM] = {0,0,0,0};
    long tNum = 0;
    int stale = 0;

    int barWidth = 50;
    int num = 100000;

    for(int n=0; n<num; n++) {
        Game* cg = Game::fromGS(&baseGS)->usePlayers(players);
        int w = cg->runGame();
        if(cg->hasWinner()){
            results[w]++;
            tNum += cg->getTurn();
        } else
            stale++;
        std::cout << "[";
        int pos = barWidth * n / num;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int((float)n/(float)num * 100.0) << " %\r";
        std::cout.flush();
    }

    cout << results[0] << "," << results[1] << "," << results[2] << "," << results[3] << endl;
    cout << tNum << endl;
    cout << stale;

    for(Player* p : players)
        free(p);

    return 0;
}
