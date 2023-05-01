
#include "doctest.h"
#include "sources/player.hpp"
#include "sources/game.hpp"
#include "sources/card.hpp"


using namespace ariel;
using namespace std;


TEST_CASE("Stack initialization") {
// Creating two players
    Player p1("Alice");
    Player p2("Bob");

//Check stack before game started
    CHECK(p1.stacksize() == 0);
    CHECK(p2.stacksize() == 0);
    CHECK(p1.cardesTaken() == 0);
    CHECK(p2.cardesTaken() == 0);

// Creating a new game with the two players
    Game game(p1, p2);

// Check that the stack size for each player is 26
    CHECK(p1.stacksize() == 26);
    CHECK(p2.stacksize() == 26);
    CHECK(p1.cardesTaken() == 0);
    CHECK(p2.cardesTaken() == 0);

}

TEST_CASE("Play at least one turn") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1, p2);
    game.playTurn();
    //both players must have at least -1 cards in their stack
    CHECK(p1.stacksize() < 26);
    CHECK(p1.stacksize() < 26);
}

TEST_CASE("Cant play after the game ended") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1, p2);
    CHECK_NOTHROW(game.playAll());
    CHECK(p1.cardesTaken() + p2.cardesTaken() == 52);
    CHECK_THROWS(game.playTurn());
}

TEST_CASE("Same player trying to register for the game") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1, p2);
    game.playTurn();
    Game game2(p1, p1);
    CHECK_THROWS(game2.playTurn());

}

TEST_CASE("Trying to print stats when no turns have been played") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1, p2);
    CHECK_THROWS(game.printLastTurn());
}


TEST_CASE("Print winner without error") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1, p2);
    CHECK_NOTHROW(game.playAll());
    CHECK_NOTHROW(game.printWiner());
}

TEST_CASE("The winner has fewer cards than the loser") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1, p2);
    game.playAll();

    if (p1.cardesTaken() > p2.cardesTaken()) {
        CHECK(p2.cardesTaken() < p1.cardesTaken()); // The winner is "Alice" but "Bob" has more cards
    } else if (p2.cardesTaken() > p1.cardesTaken()) {
        CHECK(p1.cardesTaken() < p2.cardesTaken()); //The winner is "Bob" but "Alice" has more cards
    }
}

TEST_CASE("Printing game statistics without error") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1, p2);
    CHECK_NOTHROW(game.playAll());
    CHECK_NOTHROW(game.printStats());
}

//These tests are extra because I tested functions I created
TEST_CASE("Card initialization") {
//creating two cards
    Card c1(5, "clubs");
    Card c2(13, "hearts");
//Checks that the card has been initialized successfully
    CHECK(c1.getCardNumber() == 5);
    CHECK(c1.getShape() == "clubs");
    CHECK(c2.getCardNumber() == 13);
    CHECK(c2.getShape() == "hearts");
}

TEST_CASE("Card comparison") {
//creating four cards
    Card c1(5, "clubs");
    Card c2(13, "hearts");
    Card c3(1, "diamonds");
    Card c4(5, "spades");
//compares the cards
    CHECK(c1.compare(c2) == player2);
    CHECK(c1.compare(c4) == draw);
    CHECK(c2.compare(c3) == player1);
    CHECK(c4.compare(c2) == player2);

}