
#pragma once
#include "Card.h"
#include <array>
#include <string>
#include <bitset>

/**
 * a player hand
 */
class Hand
{
public:
    Hand();

    static const int NUM_CARDS = 5;

    void addCard(Card card);
    void addCard(int hash);

    int getCard(int idx);

    int removeCard(int idx);
    int removeCard(Card card);
    int removeLastCard();

    int getNumCards();

    void clear();

    std::bitset<Card::NUM_CARDS> getBitset();

    std::string toString();

    void addSet(int bits);
    int getSet();
    int intersects(Hand hand);

private:
    int numCards;
    int set;
    std::bitset<Card::NUM_CARDS> bits;
    std::array<int, NUM_CARDS> hand;
};
