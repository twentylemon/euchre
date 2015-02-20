#pragma once
#include "Card.h"
#include <array>
#include <algorithm>
#include <bitset>
#include <string>

/**
 * The euchre deck.
 * @author Taras Mychaskiw
 */
class Deck
{
public:
    static const int SIZE = Card::NUM_RANKS * Card::NUM_SUITS;

    Deck();

    void shuffle();
    Card pop();

    int indexOf(Card card);
    int indexOf(int hash);

    bool isUsed(Card card);
    bool isUsed(int idx);
    void setUsed(Card card, bool used);
    void setUsed(int idx, bool used);
    std::bitset<Card::NUM_CARDS> getBitset();

    Card getCard(int idx);

    std::string toString();

private:
    std::array<Card, SIZE> cards;
    std::bitset<Card::NUM_CARDS> used;
    int top;
    std::array<int, Card::MAX_CARD> positions;

    void updatePositions();
};
