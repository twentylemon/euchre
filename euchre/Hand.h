
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
    
    int removeCardIDX(int idx);
    int removeCard(int hash);
    int removeCard(const Card& card);
    int removeLastCard();

    int getNumCards() const;

    std::vector<int> getCards() const;
    std::vector<int> getSuitCards(int suit) const;
    std::vector<int> getLegalCards(const Trick& trick) const;
    std::vector<int> getPlayableCards(const Trick& trick) const;

    void clear();

    std::bitset<Card::NUM_CARDS> getBitset() const;
    std::bitset<Card::NUM_CARDS> getSuitBitset(int suit) const;
    std::bitset<Card::NUM_CARDS> getLegalBitset(const Trick& trick) const;
    std::bitset<Card::NUM_CARDS> getPlayableBitset(const Trick& trick) const;
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
