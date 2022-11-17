//
// Created by Parker Hitchcock on 11/14/22.
//

#include "gameSetup.h"

#include <iostream>
#include <algorithm>
#include <random>

using namespace std;

Card cardFromLine(const string &line, int id) {
    return Card{
            id,
            line[0]-'0',
            line[2]-'0',
            line[4]-'0',
            line[6]-'0',
            line[8]-'0',
            line[10]-'0',
            line[12]-'0',
    };
}

Noble nobleFromLine(const string &line, int id) {
    return Noble{
            id,
            line[0]-'0',
            line[2]-'0',
            line[4]-'0',
            line[6]-'0',
            line[8]-'0',
            line[10]-'0',
    };
}

void loadAndShuffleDecks(GameState& gs, ifstream &d1, ifstream &d2, ifstream &d3, ifstream &n) {
    string line;
    getline(d1, line);//Skip comment
    int id = 0;
    while (id<40){
        getline(d1, line);
        gs.deck1[id] = cardFromLine(line,id);
        id++;
    }
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle((gs.deck1), end(gs.deck1), default_random_engine(seed));

    getline(d2, line);//Skip comment
    while (id<70){
        getline(d2, line);
        gs.deck2[id-40] = cardFromLine(line,id);
        id++;
    }
    seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle((gs.deck2), end(gs.deck2), default_random_engine(seed));

    getline(d3, line);//Skip comment
    while (id<90){
        getline(d3, line);
        gs.deck3[id-70] = cardFromLine(line,id);
        id++;
    }
    seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle((gs.deck3), end(gs.deck3), default_random_engine(seed));

    id = 0;
    getline(n, line);//Skip comment
    while (id<10){
        getline(n, line);
        gs.nobles[id] = nobleFromLine(line, id);
        id++;
    }
    seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle((gs.nobles), end(gs.nobles), default_random_engine(seed));

    for(int i=0; i<4; i++){
        gs.playerStates[i].playerNum = i;
    }
}
