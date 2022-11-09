#include <iostream>
#include <string>
#include "game.h"

using namespace std;

const string assetsDir = "../assets/";

int main() {
    ifstream deck1(assetsDir + "default/decks/1.csv");
    ifstream deck2(assetsDir + "default/decks/2.csv");
    ifstream deck3(assetsDir + "default/decks/3.csv");
    ifstream noble(assetsDir + "default/nobles.csv");
    if (deck1.is_open() && deck2.is_open() && deck3.is_open() && noble.is_open()) {
        startGame(deck1, deck2, deck3, noble);
    } else cout << "Unable to open file(s)";
    return 0;
}
