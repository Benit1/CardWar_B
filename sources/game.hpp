//
// Created by beni on 3/21/23.
//

#ifndef CARDWAR_A_MAIN_GAME_H
#define CARDWAR_A_MAIN_GAME_H

#include <array>
#include <iostream>
#include "player.hpp"
#include "card.hpp"
#include "logger.hpp"
#include "algorithm"
#include "random"

static constexpr int CARD_DECK_SIZE = 52;

namespace ariel {
    class Game {
    private:
        Player &pl1;
        Player &pl2;
        std::array<Card, CARD_DECK_SIZE> card_deck;
        Logger logger;
        int turns;


    public:
        Game(Player &pl1, Player &pl2);

        void playAll();

        void printWiner();

        void printLog();

        void printStats() const;

        void playTurn();

        void printLastTurn();

        Player &getPl1() const;

        Player &getPl2() const;

        void setCardDeck();

        static string &numberToCardShape(int index, string &cardShape);

        void shuffleGameDeckAndDeal(std::array<ariel::Card, CARD_DECK_SIZE> deck);

        void
        player2WonTurn(const vector<Card> &pl1ThrownCards, const vector<Card> &pl2ThrownCards, winner &currentWinner,
                       const Card &pl1Card, const Card &pl2Card);

        void
        player1WonTurn(const vector<Card> &pl1ThrownCards, const vector<Card> &pl2ThrownCards, winner &currentWinner,
                       const Card &pl1Card, const Card &pl2Card);


        void reshuffle(vector<Card> &pl1ThrownCards, vector<Card> &pl2ThrownCards);

        void updateDraws();

        void draw2Cards(vector<Card> &pl1ThrownCards, vector<Card> &pl2ThrownCards, Card &pl1Card, Card &pl2Card);

        void setWinner();

        void drawCardEach(vector<Card> &pl1ThrownCards, vector<Card> &pl2ThrownCards, Card &pl1Card, Card &pl2Card);

        void setRates();

        vector<Card> insertIntoDeck(vector<Card> &plThrownCards, int playerNumber) const;
    };
}

#endif //CARDWAR_A_MAIN_GAME_H
