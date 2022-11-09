//
// Created by Parker Hitchcock on 11/3/22.
//
#include <iostream>
#include "gameState.h"
#include <algorithm>
#include <random>

#define makeDeck(deck, state)

using namespace std;

Card cardFromLine(const string& line, int id){
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
void print(Card c){
    cout << "Id:" << c.id
         << " Suit:" << c.suit
         << " cW:" << c.cost0
         << " cU:" << c.cost1
         << " cG:" << c.cost2
         << " cR:" << c.cost3
         << " cK:" << c.cost4 << endl;
}

Noble nobleFromLine(const string& line, int id){
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

void loadAndShuffleDecks(GameState& gs, ifstream& d1, ifstream& d2, ifstream& d3, ifstream& n) {
    string line;
    getline(d1, line);//Skip comment
    int id = 0;
    while (d1){
        getline(d1, line);
        gs.deck1[id] = cardFromLine(line,id);
        id++;
    }
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle((gs.deck1), end(gs.deck1), default_random_engine(seed));

    getline(d2, line);//Skip comment
    while (d2){
        getline(d2, line);
        gs.deck2[id-40] = cardFromLine(line,id);
        id++;
    }
    seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle((gs.deck2), end(gs.deck2), default_random_engine(seed));

    getline(d3, line);//Skip comment
    while (d3){
        getline(d3, line);
        gs.deck3[id-70] = cardFromLine(line,id);
        id++;
    }
    seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle((gs.deck3), end(gs.deck3), default_random_engine(seed));

    id = 0;
    getline(n, line);//Skip comment
    while (n){
        gs.nobles[id] = nobleFromLine(line, id);
        id++;
    }
    seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle((gs.nobles), end(gs.nobles), default_random_engine(seed));
}
