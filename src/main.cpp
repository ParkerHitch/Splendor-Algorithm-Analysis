#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <sstream>
#include <iomanip>
#include <math.h>
#include "game/gameDependencies.h"
#include "game/game.h"

using namespace std;

const string assetsDir = "../assets/";
const string outputDir = "../output/";

//#define DEBUG
#define AGENT1 OSLA_V1
#define AGENT2 MonteCarlo
#define GAME_NUM 100


#ifdef DEBUG
#define K_THREADNUM 1
#else
#define K_THREADNUM 10
#endif

void fixedConfig(gameData& gd);

//output [gamesPlayed, a1 win, a2 win, draw, nTurns]
template<typename a1, typename a2> void compareAgents(gameData gd, int*output, int nGames){
    Player* ps[] = {new a1(0), new a1(1), new a2(2), new a2(3)};

    for(int n=0; n<nGames; n++) {
        Game* cg = (new Game())->usePlayers(ps)->randomizePlayerOrder();
        int w =
#ifdef DEBUG
                cg->runGameDebug();
#else
                cg->runGame();
#endif
        output[0]++;
        if(cg->hasWinner()){
            output[cg->playerIs<a1>(w)? 1 : 2]++;
            output[4] += cg->getTurn(); //Nturns
        } else
            output[3]++; //Stale
        delete cg;
    }
}

template<typename a1, typename a2> void compare(gameData& gd, int nGames, ofstream& outputFile){
    //int gPerThread = nGames/K_THREADNUM;
    long tNum = 0;
    int stale = 0;

    thread threads[K_THREADNUM];
    //output [gamesPlayed, a1 win, a2 win, draw, nTurns]
    int output[K_THREADNUM*5] = {};

    for(int t = 0; t < K_THREADNUM; t++){
        threads[t] = thread(compareAgents<a1, a2>, gd, output + t*5, nGames/K_THREADNUM);
    }

    nGames = (nGames/K_THREADNUM)*K_THREADNUM;

    int barWidth = 50;
    int totalProgress = 0;

#ifndef DEBUG
    while (totalProgress<nGames){
        totalProgress = 0;
        for(int t = 0; t < K_THREADNUM; t++)
            totalProgress += output[t*5];

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
#endif

    struct{
        int nGames;
        int a1Wins;
        int a2Wins;
        int draws;
        long totalTurns;
        double avgTurns;
    } results{};
    for(int i=0; auto& th: threads){
        th.join();
        results.nGames += output[i*5];
        results.a1Wins += output[i*5 + 1];
        results.a2Wins += output[i*5 + 2];
        results.draws  += output[i*5 + 3];
        results.totalTurns += output[i*5 + 4];
        i++;
    }
    results.avgTurns = ((double) results.totalTurns) / ((double)nGames);

    cout << "Games: " << results.nGames << endl;
    cout << a1::name() << " - " << results.a1Wins << endl;
    cout << a2::name() << " - " << results.a2Wins << endl;
    cout << "Draws - " << results.draws << endl;
    cout << "TotalTurns - " << results.totalTurns << endl;
    cout << "AvgTurns - " << results.avgTurns << endl;
    outputFile << "Games: " << results.nGames << endl;
    outputFile << a1::name() << " - " << results.a1Wins << endl;
    outputFile << a2::name() << " - " << results.a2Wins << endl;
    outputFile << "Draws - " << results.draws << endl;
    outputFile << "TotalTurns - " << results.totalTurns << endl;
    outputFile << "AvgTurns - " << results.avgTurns << endl;
}

//output [gamesPlayed, MCTS win, OSLA win, draw, nTurns]
void testCarlos(gameData gd, int*output, int nGames, int sim, int exp){
    for(int n=0; n<nGames; n++) {
        Player* ps[] = {new OSLA_V1(0), new OSLA_V1(1), new MonteCarlo(2, exp, sim), new MonteCarlo(3, exp, sim)};
        Game* cg = (new Game())->usePlayers(ps)->randomizePlayerOrder();
        int w =
#ifdef DEBUG
                cg->runGameDebug();
#else
                cg->runGame();
#endif
        output[0]++;
        if(cg->hasWinner()){
            output[cg->playerIs<MonteCarlo>(w)? 1 : 2]++;
            output[4] += cg->getTurn(); //Nturns
        } else
            output[3]++; //Stale
        for(Player* p: ps)
            delete p;
        delete cg;
    }
}

void compareMonteCarlos(gameData& gd, int nGames, ofstream& outputFile){
    int simParams[] = {500, 1000, 10000, 50000, 10000};
    float expParams[] = {0.01, 0.5, 1.0, (float)sqrt(2), 2.0};

    for(int simN: simParams){
        for(float  expP : expParams){
            long tNum = 0;
            int stale = 0;

            thread threads[K_THREADNUM];
            //output [gamesPlayed, a1 win, a2 win, draw, nTurns]
            int output[K_THREADNUM*5] = {};

            for(int t = 0; t < K_THREADNUM; t++){
                threads[t] = thread(testCarlos, gd, output + t*5, nGames/K_THREADNUM, simN, expP);
            }

            nGames = (nGames/K_THREADNUM)*K_THREADNUM;

            int barWidth = 50;
            int totalProgress = 0;

#ifndef DEBUG
            while (totalProgress<nGames){
                totalProgress = 0;
                for(int t = 0; t < K_THREADNUM; t++)
                    totalProgress += output[t*5];

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
#endif

            struct{
                int nGames;
                int carloWins;
                int OSLAWins;
                int draws;
                long totalTurns;
                double avgTurns;
            } results{};
            for(int i=0; auto& th: threads){
                th.join();
                results.nGames += output[i*5];
                results.carloWins += output[i*5 + 1];
                results.OSLAWins += output[i*5 + 2];
                results.draws  += output[i*5 + 3];
                results.totalTurns += output[i*5 + 4];
                i++;
            }
            results.avgTurns = ((double) results.totalTurns) / ((double)nGames);

            cout << "Games: " << results.nGames << endl;
            cout << "MCTS ("<< simN << ", "<< expP << ") - " << results.carloWins << endl;
            cout << "OSLA_V1 - " << results.OSLAWins << endl;
            cout << "Draws - " << results.draws << endl;
            cout << "TotalTurns - " << results.totalTurns << endl;
            cout << "AvgTurns - " << results.avgTurns << endl;
            outputFile << "Games: " << results.nGames << endl;
            outputFile << "MCTS ("<< simN << ", "<< expP << ") - " << results.carloWins << endl;
            outputFile << "OSLA_V1 - " << results.OSLAWins << endl;
            outputFile << "Draws - " << results.draws << endl;
            outputFile << "TotalTurns - " << results.totalTurns << endl;
            outputFile << "AvgTurns - " << results.avgTurns << endl;
        }
    }

}

gameData* GameState::data = nullptr;
int main() {
    ifstream deck1(assetsDir + "default/decks/1.csv");
    ifstream deck2(assetsDir + "default/decks/2.csv");
    ifstream deck3(assetsDir + "default/decks/3.csv");
    ifstream noble(assetsDir + "default/nobles.csv");

    if (!(deck1.is_open() && deck2.is_open() && deck3.is_open() && noble.is_open())) {
        cout << "Unable to open file(s)";
        return 0;
    }

    gameData gd(deck1, deck2, deck3, noble);
    GameState::data = &gd;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "Output_%Y-%m-%d.txt");
    auto fileName = oss.str();

    ofstream outFile(outputDir + fileName);

    //compareMonteCarlos(gd, GAME_NUM, outFile);
    /*{
        Game *cg = (new Game())->usePlayer(0, new OSLA_V1(0))
                ->usePlayer(1, new OSLA_V1(1))
                ->usePlayer(2, new OSLA_V1(2))
                ->usePlayer(3, new OSLA_V1(3));
        auto start = chrono::high_resolution_clock::now();
        cg->runGame();
        auto stop = chrono::high_resolution_clock::now();
        unsigned int duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        double avgDur = ((double)duration) / ((double)cg->getTurn());
        cout << "Time taken by OSLA: " << duration << " microseconds. (" << avgDur << " avg)" << endl;
        delete cg;
    }*/
    while (true) {
        Game *cg = (new Game())->usePlayer(0, new RandomPlayer(0))
                ->usePlayer(1, new RandomPlayer(1))
                ->usePlayer(2, new RandomPlayer(2))
                ->usePlayer(3, new RandomPlayer(3));
        cg->runGameDebug();
        delete cg;
    }/*{
        Game *cg = (new Game())->usePlayer(0, new MiniMax(0))
                ->usePlayer(1, new MiniMax(1))
                ->usePlayer(2, new MiniMax(2))
                ->usePlayer(3, new MiniMax(3));
        auto start = chrono::high_resolution_clock::now();
        cg->runGame();
        auto stop = chrono::high_resolution_clock::now();
        unsigned int duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        double avgDur = ((double)duration) / ((double)cg->getTurn());
        cout << "Time taken by OSLA: " << duration << " microseconds. (" << avgDur << " avg)" << endl;
        delete cg;
    }{
        Game *cg = (new Game())->usePlayer(0, new MonteCarlo(0, sqrt(2), 500))
                ->usePlayer(1, new MonteCarlo(1,sqrt(2), 500))
                ->usePlayer(2, new MonteCarlo(2,sqrt(2), 500))
                ->usePlayer(3, new MonteCarlo(3,sqrt(2), 500));
        auto start = chrono::high_resolution_clock::now();
        cg->runGame();
        auto stop = chrono::high_resolution_clock::now();
        unsigned int duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        double avgDur = ((double)duration) / ((double)cg->getTurn());
        cout << "Time taken by MCTS (500): " << duration << " microseconds. (" << avgDur << " avg)" << endl;
        delete cg;
    }{
        Game *cg = (new Game())->usePlayer(0, new MonteCarlo(0,sqrt(2), 1000))
                ->usePlayer(1, new MonteCarlo(1,sqrt(2), 1000))
                ->usePlayer(2, new MonteCarlo(2,sqrt(2), 1000))
                ->usePlayer(3, new MonteCarlo(3,sqrt(2), 1000));
        auto start = chrono::high_resolution_clock::now();
        cg->runGame();
        auto stop = chrono::high_resolution_clock::now();
        unsigned int duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        double avgDur = ((double)duration) / ((double)cg->getTurn());
        cout << "Time taken by MCTS (1000): " << duration << " microseconds. (" << avgDur << " avg)" << endl;
        delete cg;
    }
        Game *cg = (new Game())->usePlayer(0, new MonteCarlo(0, sqrt(2), 10000))
                ->usePlayer(1, new MonteCarlo(1, sqrt(2), 10000))
                ->usePlayer(2, new MonteCarlo(2, sqrt(2), 10000))
                ->usePlayer(3, new MonteCarlo(3, sqrt(2), 10000));
        auto start = chrono::high_resolution_clock::now();
        cg->runGame();
        auto stop = chrono::high_resolution_clock::now();
        unsigned int duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        double avgDur = ((double)duration) / ((double)cg->getTurn());
        cout << "Time taken by MCTS (10000): " << duration << " microseconds. (" << avgDur << " avg)" << endl;
        delete cg;
    }{
        Game *cg = (new Game())->usePlayer(0, new MonteCarlo(0, sqrt(2), 50000))
                ->usePlayer(1, new MonteCarlo(1, sqrt(2), 50000))
                ->usePlayer(2, new MonteCarlo(2, sqrt(2), 50000))
                ->usePlayer(3, new MonteCarlo(3, sqrt(2), 50000));
        auto start = chrono::high_resolution_clock::now();
        cg->runGame();
        auto stop = chrono::high_resolution_clock::now();
        unsigned int duration = chrono::duration_cast<chrono::microseconds>(stop - start).count();
        double avgDur = ((double)duration) / ((double)cg->getTurn());
        cout << "Time taken by MCTS (50000): " << duration << " microseconds. (" << avgDur << " avg)" << endl;
        delete cg;
    } */
//
//
//    compare<RandomPlayer, RandomPlayer>(gd, GAME_NUM, outFile);
//    compare<RandomPlayer, OSLA_V1>(gd, GAME_NUM, outFile);
//    compare<RandomPlayer, MiniMax>(gd, GAME_NUM, outFile);
//    compare<RandomPlayer, MonteCarlo>(gd, GAME_NUM, outFile);
//
//    compare<OSLA_V1, OSLA_V1>(gd, GAME_NUM, outFile);
//    compare<OSLA_V1, MiniMax>(gd, GAME_NUM, outFile);
//    compare<OSLA_V1, MonteCarlo>(gd, GAME_NUM, outFile);
//
//    compare<MiniMax, MiniMax>(gd, GAME_NUM, outFile);
//    compare<MiniMax, MonteCarlo>(gd, GAME_NUM, outFile);
//
//    compare<MonteCarlo, MonteCarlo>(gd, GAME_NUM, outFile);
//
//    outFile.close();

    return 0;
}
//
//void runGame(gameData gd, Player** players, int* output, int nGames){
//    for(int n=0; n<nGames; n++) {
//        Game* cg = (new Game())->usePlayers(players);
//        int w =
//#ifdef DEBUG
//                cg->runGame();
//#else
//                cg->runGame();
//#endif
//        output[0]++;
//        if(cg->hasWinner()){
//            output[w+1]++;
//            output[5] += cg->getTurn(); //Nturns
//
//        } else
//            output[6]++; //Stale
//        delete cg;
//    }
//}
//
//void fixedConfig(gameData& gd, Player** players){
//
//    int nGames = 1;
//    //int gPerThread = nGames/K_THREADNUM;
//    int results[K_PNUM] = {};
//    long tNum = 0;
//    int stale = 0;
//
//    thread threads[K_THREADNUM];
//    int output[K_THREADNUM*7] = {};
//
//    for(int t = 0; t < K_THREADNUM; t++){
//        threads[t] = thread(runGame, gd, players, output + t*7, nGames/K_THREADNUM);
//    }
//
//    int barWidth = 50;
//    int totalProgress = 0;
//
//#ifndef DEBUG
//    while (totalProgress<nGames){
//        totalProgress = 0;
//        for(int t = 0; t < K_THREADNUM; t++)
//            totalProgress += output[t*7];
//
//        std::cout << "[";
//        int pos = barWidth * totalProgress / nGames;
//        for (int i = 0; i < barWidth; ++i) {
//            if (i < pos) std::cout << "=";
//            else if (i == pos) std::cout << ">";
//            else std::cout << " ";
//        }
//        std::cout << "] " << int((float)totalProgress/(float)nGames * 100.0) << " %\r";
//        std::cout.flush();
//    }
//#endif
//
//    for(int i=0; auto& th: threads){
//        th.join();
//        results[0] += output[i*7 + 1];
//        results[1] += output[i*7 + 2];
//        results[2] += output[i*7 + 3];
//        results[3] += output[i*7 + 4];
//        tNum += output[i*7 + 5];
//        stale += output[i*7 + 6];
//        i++;
//    }
//
//    cout << players[0]->name() << "," << players[1]->name() << "," << players[2]->name() << "," << players[3]->name() << endl;
//    cout << results[0] << "," << results[1] << "," << results[2] << "," << results[3] << endl;
//    cout << tNum << endl;
//    cout << stale;
//}