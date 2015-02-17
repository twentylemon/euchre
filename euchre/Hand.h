
#pragma once
#include "Card.h"

/**
 * a player hand
 * @todo remove nth card
 * @todo red hands with the same ranked cards should be considered equal
 *      maybe we can use the trick statistics to check card equivalences?
 */
class Hand
{
public:
    Hand();

    static const int NUM_CARDS = 5;

    void addCard(Card card);

    bool contains(Card card);
    bool contains(int hash);

    int getCard(int n);

    void removeLastCard();

    bool operator==(Hand hand);

private:
    int hand;
};
