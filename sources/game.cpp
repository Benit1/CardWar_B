//
// Created by beni on 3/25/23.
//
#include "game.hpp"
#include <random>

using namespace ariel;

Game::Game(Player &pl1, Player &pl2) : pl1(pl1), pl2(pl2), logger(), turns(0) {
    pl1.setPlaying(true);
    pl2.setPlaying(true);
    this->setCardDeck();
    shuffleGameDeckAndDeal(this->card_deck);

}

void Game::playAll() {
    while (logger.getWinner() == 3) {
        this->playTurn();
    }
}

void Game::playTurn() {
    if (&pl1 == &pl2) {
        throw std::invalid_argument("Both players are the same");
    }
    if (logger.getWinner() != 3) {
        throw logic_error("Game has ended can't play another turn");
    }
    if (!pl1.isPlaying() && !pl2.isPlaying()) {
        throw std::logic_error("The game has ended");
    }
    string turnLog;
    turns++;
    vector<Card> pl1ThrownCards = pl1.getSideDeck();
    vector<Card> pl2ThrownCards = pl1.getSideDeck();
    bool lowAmountOfCards = false;
    enum winner currentWinner;
    int round = 0;
    if (pl1.stacksize() > 0 || pl2.stacksize() > 0) {
        if (pl1.stacksize() <= 2 || pl2.stacksize() <= 2) {
            lowAmountOfCards = true;
        }
        Card pl1Card;
        Card pl2Card;
        drawCardEach(pl1ThrownCards, pl2ThrownCards, pl1Card, pl2Card);
        currentWinner = pl1Card.compare(pl2Card);
        while (true) {
            if (currentWinner == player1) {
                player1WonTurn(pl1ThrownCards, pl2ThrownCards, currentWinner, pl1Card, pl2Card);
                break;
            } else if (currentWinner == player2) {
                player2WonTurn(pl1ThrownCards, pl2ThrownCards, currentWinner, pl1Card, pl2Card);
                break;
            } else if (currentWinner == draw) {
                if (lowAmountOfCards && round == 0) {
                    if (pl1.stacksize() == 0 || pl2.stacksize() == 0) {
                        pl1.setCardsTaken(1);
                        pl2.setCardsTaken(1);
                    } else {
                        pl1.setCardsTaken(2);
                        pl2.setCardsTaken(2);
                    }
                    setWinner();
                    return;
                }
                round += 1;
                updateDraws();
                logger.addLog(currentWinner, pl1, pl2, pl1Card, pl2Card);
                if (lowAmountOfCards) {
                    reshuffle(pl1ThrownCards, pl2ThrownCards);
                    drawCardEach(pl1ThrownCards, pl2ThrownCards, pl1Card, pl2Card);
                    currentWinner = pl1Card.compare(pl2Card);
                    break;
                } else if (pl1.stacksize() >= 2 || pl2.stacksize() >= 2) {
                    draw2Cards(pl1ThrownCards, pl2ThrownCards, pl1Card, pl2Card);
                    currentWinner = pl1Card.compare(pl2Card);
                } else {
                    break;
                }
            }
            if (pl1.stacksize() == 0 || pl2.stacksize() == 0) {
                if (!pl1ThrownCards.empty() || !pl2ThrownCards.empty()) {
                    if (currentWinner == player1) {
                        player1WonTurn(pl1ThrownCards, pl2ThrownCards, currentWinner, pl1Card, pl2Card);
                    } else if (currentWinner == player2) {
                        player2WonTurn(pl1ThrownCards, pl2ThrownCards, currentWinner, pl1Card, pl2Card);
                    } else {
                        pl1.setCardsTaken(pl1ThrownCards.size());
                        pl2.setCardsTaken(pl2ThrownCards.size());
                    }
                }
                setWinner();
                break;
            }
        }
        if (pl1.stacksize() == 0 || pl2.stacksize() == 0) {
            setWinner();
        }
    }
}

void Game::drawCardEach(vector<Card> &pl1ThrownCards, vector<Card> &pl2ThrownCards, Card &pl1Card,
                        Card &pl2Card) {
    pl1Card = pl1.drawCard();
    pl2Card = pl2.drawCard();
    pl1ThrownCards.push_back(pl1Card);
    pl2ThrownCards.push_back(pl2Card);
}

void Game::setWinner() {
    int pl1Cards = pl1.cardesTaken();
    int pl2Cards = pl2.cardesTaken();
    pl1.cleanDeck();
    pl2.cleanDeck();
    pl1.setPlaying(false);
    pl2.setPlaying(false);
    if (pl1Cards > pl2Cards) {
        logger.setWinner(player1);
    } else if (pl1Cards < pl2Cards) {
        logger.setWinner(player2);
    } else {
        logger.setWinner(draw);
    }
}

void Game::draw2Cards(vector<Card> &pl1ThrownCards, vector<Card> &pl2ThrownCards, Card &pl1Card,
                      Card &pl2Card) {
    pl1ThrownCards.push_back(pl1.drawCard());
    pl2ThrownCards.push_back(pl2.drawCard());
    pl1Card = pl1.drawCard();
    pl2Card = pl2.drawCard();
    pl1ThrownCards.push_back(pl1Card);
    pl2ThrownCards.push_back(pl2Card);

}

void Game::updateDraws() {
    pl1.setDrawRate(turns);
    pl2.setDrawRate(turns);
    pl1.setDraws();
    pl2.setDraws();
}

void Game::reshuffle(vector<Card> &pl1ThrownCards, vector<Card> &pl2ThrownCards) {
    vector<Card> pl1Deck = insertIntoDeck(pl1ThrownCards, 1);
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(pl1Deck.begin(), pl1Deck.end(), generator);
    pl1.setPlayerDeck(pl1Deck);
    pl1ThrownCards.clear();
    vector<Card> pl2Deck = insertIntoDeck(pl2ThrownCards, 2);
    std::shuffle(pl2Deck.begin(), pl2Deck.end(), generator);
    pl2.setPlayerDeck(pl2Deck);
    pl2ThrownCards.clear();
}

vector<Card> Game::insertIntoDeck(vector<Card> &plThrownCards, int playerNumber) const {
    vector<Card> plDeck;
    if (playerNumber == 1) {
        plDeck = pl1.getPlayerDeck();
    } else {
        plDeck = pl2.getPlayerDeck();
    }
    plDeck.insert(plThrownCards.end(), plDeck.begin(), plDeck.end());
    return plDeck;
}

void
Game::player1WonTurn(const vector<Card> &pl1ThrownCards, const vector<Card> &pl2ThrownCards,
                     winner &currentWinner,
                     const Card &pl1Card, const Card &pl2Card) {
    pl1.setCardsTaken(pl1ThrownCards.size() + pl2ThrownCards.size());
    pl1.setWins();
    setRates();
    logger.addLog(currentWinner, pl1, pl2, pl1Card, pl2Card);
}

void Game::setRates() {
    pl1.setWinRate(turns);
    pl2.setWinRate(turns);
    pl1.setDrawRate(turns);
    pl2.setDrawRate(turns);
}

void
Game::player2WonTurn(const vector<Card> &pl1ThrownCards, const vector<Card> &pl2ThrownCards,
                     winner &currentWinner,
                     const Card &pl1Card, const Card &pl2Card) {
    pl2.setCardsTaken(pl1ThrownCards.size() + pl2ThrownCards.size());
    pl2.setWins();
    setRates();
    logger.addLog(currentWinner, pl1, pl2, pl1Card, pl2Card);
}

void Game::printLastTurn() {
    logger.printLastTurn();
}

void Game::printLog() {
    logger.printLog();
}

void Game::printStats() const {
    Logger::printStats(this->getPl1(), this->getPl2());
}

void Game::printWiner() {
    logger.printWinner(pl1.getPlayerName(), pl2.getPlayerName());
}

Player &Game::getPl1() const {
    return pl1;
}


Player &Game::getPl2() const {
    return pl2;
}

void Game::setCardDeck() {
    array<Card, CARD_DECK_SIZE> newDeck;
    for (size_t index = 0; index < 13; ++index) {
        for (size_t jndex = 0; jndex < 4; ++jndex) {
            std::string cardShape;
            cardShape = numberToCardShape((int) jndex, cardShape);
            Card currentCard((int) (index + 1), cardShape);
            newDeck[(index * 4) + jndex] = currentCard;
        }
    }
    this->card_deck = newDeck;
}

string &Game::numberToCardShape(int index, string &cardShape) {
    switch (index) {
        case 0:
            cardShape = "Clubs (♣)";
            break;
        case 1:
            cardShape = "Diamonds (♦)";
            break;
        case 2:
            cardShape = "Hearts (♥)";
            break;
        case 3:
            cardShape = "Spades (♠)";
            break;
        default:
            break;
    }
    return cardShape;
}

void Game::shuffleGameDeckAndDeal(std::array<Card, CARD_DECK_SIZE> deck) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(deck.begin(), deck.end(), generator);
    vector<Card> pl1Deck;
    vector<Card> pl2Deck;
    for (size_t i = 0; i < deck.size(); i++) {
        if (i < 26) {
            pl1Deck.push_back(deck[i]);
        } else {
            pl2Deck.push_back(deck[i]);
        }
    }
    pl1.setPlayerDeck(pl1Deck);
    pl2.setPlayerDeck(pl2Deck);
}
