//
// Created by beni on 3/21/23.
//

#ifndef CARDWAR_A_MAIN_PLAYER_H
#define CARDWAR_A_MAIN_PLAYER_H

#include <iostream>
#include "card.hpp"
#include "vector"
#include "array"

using namespace std;
namespace ariel {
    class Player {
    private:
        int cards_taken;
        string player_name;
        std::vector<ariel::Card> player_deck;
        std::vector<ariel::Card> side_deck;
        bool playing;
        int wins;
        double winRate;
        int draws;
        double drawRate;
    public:

        explicit Player(std::string player_name);

        int stacksize() const;

        int cardesTaken() const;

        void setCardsTaken(size_t cardsTaken);

        const string &getPlayerName() const;

        bool isPlaying() const;

        void setPlaying(bool playingState);

        void setWins();

        void setWinRate(int turns);

        void setDraws();

        void setDrawRate(int turns);

        const vector<Card> &getPlayerDeck() const;

        void setPlayerDeck(const vector<Card> &playerDeck);

        const vector<Card> &getSideDeck() const;

        std::string toString() const;

        const Card &drawCard();

        void cleanDeck();
    };
}

#endif //CARDWAR_A_MAIN_PLAYER_H
