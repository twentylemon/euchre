
#pragma once
#include "Card.h"
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
    
    void addSet(unsigned int bits);
    void addSet(std::bitset<Card::NUM_CARDS> bits);
    bool intersects(Hand hand);
    bool intersects(std::bitset<Card::NUM_CARDS> bits);

    static const std::array<std::vector<Hand>, NUM_CARDS+1> ALL_HANDS;

private:
    int numCards;
    std::bitset<Card::NUM_CARDS> bits;
    std::array<int, NUM_CARDS> hand;
};
