//
// Created by beni on 4/6/23.
//

#include "logger.hpp"

using namespace ariel;

void Logger::printWinner(const std::string &pl1Name, const std::string &pl2Name) {
    switch (gameWinner) {
        case draw:
            std::cout << "No gameWinner the game ended in a draw." << std::endl;
        case player1:
            std::cout << "The Winner is: " << pl1Name << std::endl;
            break;
        case player2:
            std::cout << "The Winner is: " << pl2Name << std::endl;
            break;
        case noWinner:
            std::cout << "No Winner" << std::endl;
            break;
        default:
            break;
    }
}

void Logger::printLog() {
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

void Logger::printStats(const ariel::Player &pl1, const ariel::Player &pl2) {
    std::cout << pl1.toString() << std::endl;
    std::cout << pl2.toString() << std::endl;
}

void Logger::printLastTurn() {
    std::cout << logs.back() << std::endl;
}

void Logger::addLog(enum winner turnWinner, const ariel::Player &pl1, const ariel::Player &pl2, const Card &card1,
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

winner Logger::getWinner() const {
    return gameWinner;
}

void Logger::setWinner(enum winner winner) {
    Logger::gameWinner = winner;
}

Logger::Logger() : gameWinner(static_cast<enum winner>(3)) {

}


