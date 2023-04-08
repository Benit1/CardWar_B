//
// Created by beni on 3/21/23.
//

#include "player.hpp"

using namespace ariel;

Player::Player(std::string player_name) : player_name(std::move(player_name)),
                                          cards_taken(0), draws(0), drawRate(0), wins(0),
                                          winRate(0), playing(false) {}


int Player::stacksize() const {
    return (int) this->player_deck.size();
}

int Player::cardesTaken() const {
    return this->cards_taken;
}

const string &Player::getPlayerName() const {
    return player_name;
}

bool Player::isPlaying() const {
    return playing;
}

void Player::setPlaying(bool playingState) {
    Player::playing = playingState;
}

void Player::setWins() {
    Player::wins += 1;
}

void Player::setWinRate(int turns) {
    Player::winRate = 100 * (((double) this->wins) / turns);

}

void Player::setDraws() {
    Player::draws += 1;
}

void Player::setDrawRate(int turns) {
    Player::drawRate = 100 * (((double) this->draws) / turns);
}

const vector<Card> &Player::getSideDeck() const {
    return side_deck;
}

std::string Player::toString() const {
    std::string playingString;
    if (playing) {
        playingString = "true";
    } else playingString = "false";
    return "Player Name: " + player_name + " Wins: " + to_string(wins) +
           " Win Rate: %" + to_string(winRate) + " Draws: " + to_string(draws) + " Draw Rate: %" +
           to_string(drawRate) + " cards Taken: " + to_string(cards_taken) + " In game: " + playingString;
}

const vector<Card> &Player::getPlayerDeck() const {
    return player_deck;
}

void Player::setPlayerDeck(const vector<Card> &playerDeck) {
    player_deck = playerDeck;
}

void Player::setCardsTaken(size_t cardsTaken) {
    cards_taken += (int) cardsTaken;
}

const Card &Player::drawCard() {
    const Card &card = this->player_deck.back();
    this->player_deck.pop_back();
    return card;
}

void Player::cleanDeck() {
    this->player_deck.clear();
}
