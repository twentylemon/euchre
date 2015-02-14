#pragma once
#include "Card.h"
#include <vector>
#include <string>

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
    int getWinner();
    Card getWinningCard();

    std::string toString();

private:
    int trump;
    int winner;
    std::vector<Card> cards;

    const static int MIN_TRUMP_SCORE = 6;
    int rankings[4][60];    //max hash value is 59
    void initRankings();
};

