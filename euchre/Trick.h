#pragma once
#include "Card.h"
#include "CardScore.h"
#include <array>
#include <vector>
#include <string>

class CardScore;

/**
 * define this to include trick hashcode stuff
 */
//#define TRICK_INCLUDE_HASH

/**
 * One euchre trick.
 * @author Taras Mychaskiw
 */
class Trick
{
public:

    static const int NUM_CARDS = 4;

    static const std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> EFFECTIVE_SUITS;

    Trick();
    Trick(int);

    int getTrump() const;
    void setTrump(int trump);

    int getLeadSuit() const;
    bool isLegal(const Card& card) const;

    void clear();

    std::vector<Card> getCards() const;
    const Card& getCard(int position) const;
    int getNumCards() const;
    bool empty() const;

    void addCard(const Card& card);
    void removeLastCard();

    int getWinner() const;
    const Card& getWinningCard() const;

#ifdef TRICK_INCLUDE_HASH
    int hashCode() const;
    static int hashCode(int trump, const Card& card1, const Card& card2, const Card& card3, const Card& card4);
    static int hashCode(int trump, int card1, int card2, int card3, int card4);
#endif

    std::string toString() const;

private:
#ifdef TRICK_INCLUDE_HASH
    int hash;
#endif
    int trump;
    int leadSuit;
    std::array<int, NUM_CARDS> winner;
    std::vector<Card> cards;
};
