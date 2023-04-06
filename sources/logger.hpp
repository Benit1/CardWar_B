//
// Created by beni on 4/6/23.
//

#ifndef BENIT_SYSTEMS2_EX2B_LOGGER_HPP
#define BENIT_SYSTEMS2_EX2B_LOGGER_HPP

#include <iostream>
#include "vector"
#include "player.hpp"


class Logger {

private:
    std::vector<std::string> logs;
    winner winner;
public:

    Logger();
    void printWinner(const std::string &p1Name, const std::string &p2Name);

    void printLog();

    static void printStats(const ariel::Player &pl1, const ariel::Player &pl2);

    void printLastTurn();

    void
    addLog(enum winner turnWinner, const ariel::Player &pl1, const ariel::Player &pl2, const ariel::Card &card1,
           const ariel::Card &card2);

    enum winner getWinner() const;

    void setWinner(enum winner winner);
};


#endif //BENIT_SYSTEMS2_EX2B_LOGGER_HPP
