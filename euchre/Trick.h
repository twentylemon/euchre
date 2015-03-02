#pragma once
#include "Card.h"
#include "CardScore.h"
#include <array>
#include <vector>
#include <string>

/**
 * define this to include trick hashcode stuff
 */
#define TRICK_INCLUDE_HASH

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

    int getLeadSuit();
    bool isLegal(Card card);

    void clear();

    std::vector<Card> getCards();
    int getNumCards();

    void addCard(Card card);
    void removeLastCard();

    int getWinner();
    Card getWinningCard();

#ifdef TRICK_INCLUDE_HASH
    int hashCode();
    static int hashCode(int trump, Card card1, Card card2, Card card3, Card card4);
    static int hashCode(int trump, int card1, int card2, int card3, int card4);
#endif

    std::string toString();

    static const std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> EFF_SUITS;

private:
#ifdef TRICK_INCLUDE_HASH
    int hash;
#endif
    int trump;
    int leadSuit;
    std::array<int, NUM_CARDS> winner;
    std::vector<Card> cards;
};
