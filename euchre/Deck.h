#pragma once
#include "Card.h"
#include <array>
#include <algorithm>

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

    int indexOf(Card card);

    bool isUsed(Card card);
    bool isUsed(int idx);
    void setUsed(Card card, bool used);
    void setUsed(int idx, bool used);

    int getNextIdx(int idx);

    Card getCard(int idx);

private:
    std::array<Card, SIZE> cards;
    std::array<bool, SIZE> used;
    std::array<int, Card::MAX_CARD> positions;

    void updatePositions();
};
