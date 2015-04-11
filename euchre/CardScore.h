
#pragma once
#include "Card.h"
#include "Trick.h"
#include <array>
#include <vector>
#include <bitset>
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

    static int winRate(int trump, const Card& card);
    static int winRate(int trump, int hash);
    static int sumWinRate(int trump, std::bitset<Card::NUM_CARDS> cards);

    const static int MIN_TRUMP_SCORE = 7;
    const static int RIGHT_BOWER = 13;
    const static int LEFT_BOWER = 12;
    const static std::array<int, Card::NUM_RANKS> TRUMP_SCORE;
    const static std::array<int, Card::NUM_RANKS> OFF_SCORE;

    static void init();

    static std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> calcRankings();
    const static std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> cardComparator;
    const static std::array<std::function<bool(int, int)>, Card::NUM_SUITS> hashComparator;
    static int getBestCard(std::vector<int> cards, const Trick& trick);
    static int getWorstCard(std::vector<int> cards, const Trick& trick);
    const static Card& getBestCard(std::vector<Card> cards, const Trick& trick);
    const static Card& getWorstCard(std::vector<Card> cards, const Trick& trick);

    static std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> calcTrickWins();
    const static std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> cardWinComparator;
    const static std::array<std::function<bool(int, int)>, Card::NUM_SUITS> hashWinComparator;
    static int getBestWinCard(std::vector<int> cards, const Trick& trick);
    static int getWorstWinCard(std::vector<int> cards, const Trick& trick);
    const static Card& getBestWinCard(std::vector<Card> cards, const Trick& trick);
    const static Card& getWorstWinCard(std::vector<Card> cards, const Trick& trick);

    static std::array<std::array<int, Card::MAX_CARD*Card::MAX_CARD*Card::MAX_CARD*Card::MAX_CARD>, Card::NUM_SUITS> calcPartialWins();
    static int getPartialWinRate(int card, const Trick& trick);
    static int getPartialWinRate(const Card& card, const Trick& trick);
    static int sumPartialWinRate(const Trick& trick, std::bitset<Card::NUM_CARDS> cards);

    static void display();

private:
    static std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> rankings;
    static std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> trickWins;
    static std::array<std::array<int, Card::MAX_CARD*Card::MAX_CARD*Card::MAX_CARD*Card::MAX_CARD>, Card::NUM_SUITS> partialWins;
    
    CardScore();
};
