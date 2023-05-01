//
// Created by beni on 3/25/23.
//
#include "game.hpp"
#include "algorithm"
#include "random"

using namespace ariel;

//normal constructor that init a new game
Game::Game(Player &pl1, Player &pl2) : pl1(pl1), pl2(pl2), turns(0), gameWinner(noWinner) {

    if (!this->pl1.isPlaying()) {
        this->pl1.resetPlayer();
    }
    if (!this->pl2.isPlaying()) {
        this->pl2.resetPlayer();
    }
    this->pl1.setPlaying(true);
    this->pl2.setPlaying(true);
    this->setCardDeck();
    shuffleGameDeckAndDeal(this->card_deck);

}

//Play the game until there is a winner && throws logic_error if trying to play again after the game ended
void Game::playAll() {
    if (getWinner() != noWinner) throw std::logic_error("game has ended");
    while (getWinner() == 3) {
        this->playTurn();
    }
}

//play a turn
//  remove the top card from both decks and compare them
//  save the log of that turn
//  if there is a tie between the 2 cards the turn will continue until there is a winner
//  if there is a tie the 2 players will put 1 card on the "back" and one faced up
//  that will be compared against the other player's card
//  that will happen as long as both players have enough cards to play the "war"
//  throws logic_error happens with 3 cases
//  1) if both players are the same
//  2) the game ended
//  3)Players already registered to another game
void Game::playTurn() {
    if (&pl1 == &pl2) {
        throw std::invalid_argument("Both players are the same");
    }
    if (getWinner() != 3) {
        throw logic_error("Game has ended can't play another turn");
    }
    if (!pl1.isPlaying() && !pl2.isPlaying()) {
        throw std::logic_error("The game has ended");
    }
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
                addLog(currentWinner, pl1, pl2, pl1Card, pl2Card);
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

//draw a card for each player and add it to the thrown cards vector
void Game::drawCardEach(vector<Card> &pl1ThrownCards, vector<Card> &pl2ThrownCards, Card &pl1Card,
                        Card &pl2Card) {
    pl1Card = pl1.drawCard();
    pl2Card = pl2.drawCard();
    pl1ThrownCards.push_back(pl1Card);
    pl2ThrownCards.push_back(pl2Card);
}

//Check which player have more cards taken and set the winner according to the result
void Game::setWinner() {
    int pl1Cards = pl1.cardesTaken();
    int pl2Cards = pl2.cardesTaken();
    pl1.cleanDeck();
    pl2.cleanDeck();
    pl1.setPlaying(false);
    pl2.setPlaying(false);
    if (pl1Cards > pl2Cards) {
        setWinner(player1);
    } else if (pl1Cards < pl2Cards) {
        setWinner(player2);
    } else {
        setWinner(draw);
    }
}

//draw 2 cards for each player and add it to the thrown cards vector
void Game::draw2Cards(vector<Card> &pl1ThrownCards, vector<Card> &pl2ThrownCards, Card &pl1Card,
                      Card &pl2Card) {
    pl1ThrownCards.push_back(pl1.drawCard());
    pl2ThrownCards.push_back(pl2.drawCard());
    pl1Card = pl1.drawCard();
    pl2Card = pl2.drawCard();
    pl1ThrownCards.push_back(pl1Card);
    pl2ThrownCards.push_back(pl2Card);

}

//update the draw rate for both players
void Game::updateDraws() {
    pl1.setDrawRate(turns);
    pl2.setDrawRate(turns);
    pl1.setDraws();
    pl2.setDraws();
}

//shuffle both players deck randomly
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

//add all the thrown cards back to the deck
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

//player 1 won this turn
//  increase the amount of cards taken
//  set both player win rate and draw rate
//  and add the turn to the turn logs
void
Game::player1WonTurn(const vector<Card> &pl1ThrownCards, const vector<Card> &pl2ThrownCards,
                     winner &currentWinner,
                     const Card &pl1Card, const Card &pl2Card) {
    pl1.setCardsTaken(pl1ThrownCards.size() + pl2ThrownCards.size());
    pl1.setWins();
    setRates();
    addLog(currentWinner, pl1, pl2, pl1Card, pl2Card);
}

//update the win rate for both players
void Game::setRates() {
    pl1.setWinRate(turns);
    pl2.setWinRate(turns);
    pl1.setDrawRate(turns);
    pl2.setDrawRate(turns);
}

//player 2 won this turn
//  increase the amount of cards taken
//  set both player win rate and draw rate
//  and add the turn to the logs
void
Game::player2WonTurn(const vector<Card> &pl1ThrownCards, const vector<Card> &pl2ThrownCards,
                     winner &currentWinner,
                     const Card &pl1Card, const Card &pl2Card) {
    pl2.setCardsTaken(pl1ThrownCards.size() + pl2ThrownCards.size());
    pl2.setWins();
    setRates();
    addLog(currentWinner, pl1, pl2, pl1Card, pl2Card);
}

void Game::printLastTurn() {
    if (logs.empty()) throw logic_error("no logs yet");
    string str = logs.back();
    for (int i = logs.size() - 2; i >= 0; i--) {
        if (logs.at((size_t) i).find("draw") != string::npos) {
            str.insert(0, logs.at((size_t) i));
        } else {
            break;
        }
    }
    std::cout << str << std::endl;
}

//print the entire game log
void Game::printLog() {
    string str;
    for (const string &currentTurn: logs) {
        if (currentTurn.find("draw") != string::npos) {
            str += currentTurn;
        } else {
            str += currentTurn;
            std::cout << str << std::endl;
            str = "";
        }
    }
}

void Game::printStats() const {
    std::cout << pl1.toString() << std::endl;
    std::cout << pl2.toString() << std::endl;
}

void Game::printWiner() {
    switch (gameWinner) {
        case draw:
            std::cout << "No gameWinner the game ended in a draw." << std::endl;
        case player1:
            std::cout << "The Winner is: " << pl1.getPlayerName() << std::endl;
            break;
        case player2:
            std::cout << "The Winner is: " << pl1.getPlayerName() << std::endl;
            break;
        case noWinner:
            std::cout << "No Winner" << std::endl;
            break;
        default:
            break;
    }
}

Player &Game::getPl1() const {
    return pl1;
}


Player &Game::getPl2() const {
    return pl2;
}

//generate the deck with 52 cards 4 of each kind(ace-king)
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

//shuffle the deck and give 26 cards for each player
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

// build what happened in this turn and add it to the logs
void Game::addLog(enum winner turnWinner, const ariel::Player &pl1, const ariel::Player &pl2, const Card &card1,
                  const Card &card2) {
    std::string logRow = pl1.getPlayerName() + " played " + card1.toString() + ". "
                         + pl2.getPlayerName() + "played " + card2.toString() + ". ";
    if (turnWinner == player1) {
        logRow += pl1.getPlayerName() + " wins.";
    } else if (turnWinner == player2) {
        logRow += pl2.getPlayerName() + " wins.";
    } else if (turnWinner == draw) {
        logRow += "draw.";
    }
    logs.push_back(logRow);
}

winner Game::getWinner() const {
    return gameWinner;
}

void Game::setWinner(enum winner winner) {
    Game::gameWinner = winner;
}
