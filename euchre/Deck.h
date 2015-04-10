#pragma once
#include "Card.h"
#include "Random.h"
#include <array>
#include <bitset>
#include <string>
#include <algorithm>

/**
 * The euchre deck.
 * @author Taras Mychaskiw
 */
class Deck
{
public:
    static const int SIZE = Card::NUM_CARDS;

    Deck();

    void shuffle();
    const Card& pop();

    int indexOf(const Card& card) const;
    int indexOf(int hash) const;

    bool isUsed(const Card& card) const;
    bool isUsed(int idx) const;
    void setUsed(const Card& card, bool used);
    void setUsed(int idx, bool used);
    std::bitset<Card::NUM_CARDS> getBitset() const;
    
    const Card& getCard(int idx);
    const Card& getCard(int idx) const;

    std::string toString() const;

private:
    std::array<Card, SIZE> cards;
    std::bitset<Card::NUM_CARDS> used;
    int top;
    std::array<int, Card::MAX_CARD> positions;

    void updatePositions();
};
