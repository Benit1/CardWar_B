//
// Created by beni on 3/21/23.
//

#ifndef CARDWAR_A_MAIN_CARD_H
#define CARDWAR_A_MAIN_CARD_H

#include <iostream>
enum winner {
    draw = 0, player1 = 1, player2 = 2, noWinner = 3
};
using namespace std;
namespace ariel {
    class Card {
        int card_number;//(ace(1),2-10,Jack(11),queen(12),king(13))
        string shape;//(Clubs (♣), Diamonds (♦), Hearts (♥), and Spades (♠))

    public:
        Card();

        Card(int card_number, string shape);


        string toString() const;

        enum winner compare(const Card& other) const;

    };
}
#endif //CARDWAR_A_MAIN_CARD_H
