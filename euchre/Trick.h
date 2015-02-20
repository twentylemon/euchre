#pragma once
#include "Card.h"
#include "CardScore.h"
#include <array>
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
    void removeLastCard();

    int getWinner();
    Card getWinningCard();

    int hashCode();
    static int hashCode(Card card1, Card card2, Card card3, Card card4);
    static int hashCode(int card1, int card2, int card3, int card4);

    std::string toString();

private:
    int hash;
    int trump;
    std::array<int, NUM_CARDS> winner;
    std::vector<Card> cards;
};
