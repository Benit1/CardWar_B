//
// Created by beni on 3/21/23.
//

#include "player.hpp"

#include <utility>

using namespace std;


ariel::Player::Player(std::string player_name) : player_name(std::move(player_name)),
                                                 cards_taken(0), draws(0), drawRate(0), wins(0),
                                                 winRate(0), playing(false), id(increaseID()) {}


int ariel::Player::stacksize() const {
    return (int) this->player_deck.size();
}

int ariel::Player::cardesTaken() const {
    return this->cards_taken;
}

const string &ariel::Player::getPlayerName() const {
    return player_name;
}

void ariel::Player::setPlayerName(const string &playerName) {
    player_name = playerName;
}

bool ariel::Player::isPlaying() const {
    return playing;
}

void ariel::Player::setPlaying(bool playing) {
    Player::playing = playing;
}

int ariel::Player::getWins() const {
    return wins;
}

void ariel::Player::setWins() {
    Player::wins += 1;
}

double ariel::Player::getWinRate() const {
    return winRate;
}

void ariel::Player::setWinRate(int turns) {
    Player::winRate = 100 * (((double) this->wins) / turns);

}

int ariel::Player::getDraws() const {
    return draws;
}

void ariel::Player::setDraws() {
    Player::draws += 1;
}

double ariel::Player::getDrawRate() const {
    return drawRate;
}

void ariel::Player::setDrawRate(int turns) {
    Player::drawRate = 100 * (((double) this->draws) / turns);
}

const vector<ariel::Card> &ariel::Player::getSideDeck() const {
    return side_deck;
}

void ariel::Player::setSideDeck(const vector<Card> &sideDeck) {
    side_deck = sideDeck;
}

std::string ariel::Player::toString() const {
    std::string playingString;
    if (playing) {
        playingString = "true";
    } else playingString = "false";
    return "Player Name: " + player_name + " Wins: " + to_string(wins) +
           " Win Rate: %" + to_string(winRate) + " Draws: " + to_string(draws) + " Draw Rate: %" +
           to_string(drawRate) + " cards Taken: " + to_string(cards_taken) + " In game: " + playingString;
}

const vector<ariel::Card> &ariel::Player::getPlayerDeck() const {
    return player_deck;
}

void ariel::Player::setPlayerDeck(const vector<Card> &playerDeck) {
    player_deck = playerDeck;
}

void ariel::Player::setCardsTaken(size_t cardsTaken) {
    cards_taken += cardsTaken;
}

const ariel::Card &ariel::Player::drawCard() {
    const Card &card = this->player_deck.back();
    this->player_deck.pop_back();
    return card;
}


unsigned long ariel::Player::getID() const {
    return id;
}

unsigned long ariel::Player::increaseID() {
    static unsigned long idStatic = 1;
    return idStatic++;
}

