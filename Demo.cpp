/**
 * Demo file for the exercise on binary tree
 *
 * @author Evgeny Hershkovitch Neiterman
 * @since 2023-03
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace std;

#include "sources/player.hpp"
#include "sources/game.hpp"
#include "sources/card.hpp"

using namespace ariel;


int main() {
    // Create two players with their names
    Player p1("Alice");
    Player p2("Bob");

    Game game(p1, p2);
    for (int i = 0; i < 5; i++) {
        game.playTurn();
    }
    //Check if it's the same player
    if (&p1 == &game.getPl1()) {
        cout << "the same" << endl;
    }
    if (&p2 == &game.getPl2()) {
        cout << "the same" << endl;
    }
    Game game2(p1, p1);
    try {
        game2.playTurn();
        //"catch(...)" is used to catch any exception thrown by the try block.
    } catch (...) {
        cerr << "Got an exception" << endl;
    }
    try {
        game2.playAll();
    } catch (...) {
        cerr << "Got an exception" << endl;
    }
    game.printLastTurn();
    cout << p1.stacksize()
         << endl;
    cout << p2.cardesTaken() << endl;
    game.playAll();
    game.printWiner();
    game.printLog();
    game.printStats();

}
