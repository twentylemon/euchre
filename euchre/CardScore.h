
#pragma once
#include "Card.h"
#include <iostream>

/**
 * calculates and retrieves card scorings by trump suit, higher scores are better
 * equal scores need to be checked for lead suit etc, higher means they have a chance to be a better card
 * @author Taras Mychaskiw
 */
const class CardScore
{
public:
    CardScore();

    static int get(int trump, Card card);
    static int get(int trump, int hash);

    const static int MIN_TRUMP_SCORE = 6;
    const static int RIGHT_BOWER = 12;
    const static int LEFT_BOWER = 11;
    const static int TRUMP_SCORE[6];
    const static int OFF_SCORE[6];

    const static int RANKINGS[Card::NUM_SUITS][Card::MAX_CARD];

    static void initRankings();
    static void display();

private:
    static bool calculated;
    static int rankings[Card::NUM_SUITS][Card::MAX_CARD];
};
