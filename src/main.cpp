#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include "game/game.h"

using namespace std;

const string assetsDir = "../assets/";

#define K_THREADNUM 5

void runGame(GameState baseGS, Player** players, int* output, int nGames){
    for(int n=0; n<nGames; n++) {
        Game* cg = Game::newFromGS(baseGS)->usePlayers(players);
        int w = cg->runGame();
        output[0]++;
        if(cg->hasWinner()){
            output[w+1]++;
            output[5] += cg->getTurn(); //Nturns
        } else
            output[6]++; //Stale
        free(cg);
    }
}

int main() {
    ifstream deck1(assetsDir + "default/decks/1.csv");
    ifstream deck2(assetsDir + "default/decks/2.csv");
    ifstream deck3(assetsDir + "default/decks/3.csv");
    ifstream noble(assetsDir + "default/nobles.csv");

    GameState baseGS;

    if (deck1.is_open() && deck2.is_open() && deck3.is_open() && noble.is_open()) {
        baseGS = GameState(deck1, deck2, deck3, noble);
    } else {
        cout << "Unable to open file(s)";
        return 0;
    }

    Player* players[K_PNUM];
    players[0] = new OSLA_V1(0);
    players[1] = new OSLA_V1(1);
    players[2] = new OSLA_V1(2);
    players[3] = new OSLA_V1(3);

    int nGames = 1000;
    //int gPerThread = nGames/K_THREADNUM;
    int results[K_PNUM] = {};
    long tNum = 0;
    int stale = 0;

    thread threads[K_THREADNUM];
    int output[K_THREADNUM*7] = {};

    for(int t = 0; t < K_THREADNUM; t++){
        threads[t] = thread(runGame, baseGS, players, output + t*7, nGames/K_THREADNUM);
    }

    int barWidth = 50;
    int totalProgress = 0;

    while (totalProgress<nGames){
        totalProgress = 0;
        for(int t = 0; t < K_THREADNUM; t++)
            totalProgress += output[t*7];

        std::cout << "[";
        int pos = barWidth * totalProgress / nGames;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int((float)totalProgress/(float)nGames * 100.0) << " %\r";
        std::cout.flush();
    }

    for(int i=0; auto& th: threads){
        th.join();
        results[0] += output[i*7 + 1];
        results[1] += output[i*7 + 2];
        results[2] += output[i*7 + 3];
        results[3] += output[i*7 + 4];
        tNum += output[i*7 + 5];
        stale += output[i*7 + 6];
        i++;
    }

    cout << results[0] << "," << results[1] << "," << results[2] << "," << results[3] << endl;
    cout << tNum << endl;
    cout << stale;

    for(Player* p : players)
        free(p);

    return 0;
}