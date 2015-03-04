
#pragma once
#include "Card.h"
#include "Trick.h"
#include "BitString.h"
#include <array>
#include <string>
#include <bitset>
#include <vector>

/**
 * a player hand
 */
class Hand
{
public:
    Hand();
    Hand(std::bitset<Card::NUM_CARDS> bits);

    static const int NUM_CARDS = 5;
    static const std::array<std::vector<Hand>, NUM_CARDS+1> ALL_HANDS;

    void addCard(const Card& card);
    void addCard(int hash);
    
    void addSet(unsigned int bits);
    void addSet(std::bitset<Card::NUM_CARDS> bits);

    int getCard(int idx) const;

    int removeCard(int idx);
    int removeCard(const Card& card);
    int removeLastCard();

    int getNumCards() const;

    std::vector<int> getLegalCards(const Trick &trick) const;

    void clear();

    std::bitset<Card::NUM_CARDS> getBitset() const;
    int hashCode() const;

    std::string toString() const;

    bool intersects(const Hand& hand) const;
    bool intersects(unsigned int bits) const;
    bool intersects(std::bitset<Card::NUM_CARDS> bits) const;

private:
    int numCards;
    std::bitset<Card::NUM_CARDS> bits;
    std::array<int, NUM_CARDS> hand;
};
