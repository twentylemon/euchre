
#pragma once
#include "Card.h"
#include "Trick.h"
#include <array>
#include <vector>
#include <iostream>
#include <functional>

class Trick;

/**
 * calculates and retrieves card scorings by trump suit, higher scores are better
 * equal scores need to be checked for lead suit etc, higher means they have a chance to be a better card
 * @author Taras Mychaskiw
 */
const class CardScore
{
public:
    static int get(int trump, const Card& card);
    static int get(int trump, int hash);

    const static int MIN_TRUMP_SCORE = 6;
    const static int RIGHT_BOWER = 12;
    const static int LEFT_BOWER = 11;
    const static std::array<int, Card::NUM_RANKS> TRUMP_SCORE;
    const static std::array<int, Card::NUM_RANKS> OFF_SCORE;
    
    static int getBestCard(std::vector<int> cards, const Trick& trick);
    static int getWorstCard(std::vector<int> cards, const Trick& trick);
    const static Card& getBestCard(std::vector<Card> cards, const Trick& trick);
    const static Card& getWorstCard(std::vector<Card> cards, const Trick& trick);

    static void initRankings();
    static void display();
    
    const static std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> cardComparator;
    const static std::array<std::function<bool(int, int)>, Card::NUM_SUITS> hashComparator;

private:
    static bool calculated;
    static std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> rankings;
    
    CardScore();
};
