#pragma once
#include "Card.h"
#include "CardScore.h"
#include <array>
#include <vector>
#include <string>

#include <iostream>

/**
 * One euchre trick.
 * @author Taras Mychaskiw
 */
class Trick
{
public:

    static const int NUM_CARDS = 4;

    Trick();
    Trick(int);

    int getTrump();
    void setTrump(int trump);
    void clear();

    std::vector<Card> getCards();

    void addCard(Card card);
    void removeLastCard();

    int getWinner();
    Card getWinningCard();

    std::string toString();

private:
    int trump;
    std::array<int, NUM_CARDS> winner;
    std::vector<Card> cards;
};
