//
// Created by beni on 3/21/23.
//
#include "card.hpp"

using namespace std;
using namespace ariel;

Card::Card(int card_number, string shape) {
    this->card_number = card_number;
    this->shape = std::move(shape);
}

Card::Card() {
    this->card_number = 1;
    this->shape = "Clubs";
}

string Card::toString() const {
    //(ace(1),2-10,Jack(11),queen(12),king(13))
    std::string numberString;
    switch (this->card_number) {
        case 1:
            numberString = "Ace";
            break;
        case 11:
            numberString = "Jack";
            break;
        case 12:
            numberString = "Queen";
            break;
        case 13:
            numberString = "King";
            break;
        default:
            break;
    }
    std::string cardString;
    if (numberString.empty()) {
        cardString = to_string(this->card_number) + " of " + shape;
    } else {
        cardString = numberString + " of " + shape;
    }
    return cardString;
}

// 0 for draw
// 1 for this wins
// 2 for other wins
enum winner Card::compare(const Card &other) const {
    if (this->card_number == 1 && other.card_number == 2) {
        return player2;
    } else if (this->card_number == 2 && other.card_number == 1) {
        return player1;
    } else if (this->card_number == other.card_number) {
        return draw;
    } else if (this->card_number > other.card_number) {
        return player1;
    } else {
        return player2;
    }
}

int Card::getCardNumber() const {
    return card_number;
}

void Card::setCardNumber(int cardNumber) {
    card_number = cardNumber;
}

const string &Card::getShape() const {
    return shape;
}

void Card::setShape(const string &shape) {
    Card::shape = shape;
}

