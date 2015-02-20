
#pragma once
#include "Card.h"
#include "Hand.h"
#include "Trick.h"
#include "Player.h"
#include <string>
#include <bitset>

/**
 * an ai player
 * @author Taras Mychaskiw
 */
class AIPlayer : public Player
{
public:
    AIPlayer();

    void startNewHand() override;

    void addCard(Card card) override;
    void seenCard(Card card);

    std::string toString() override;

    int orderUp(Card top) override;
    int pickItUp(Card top) override;
    int callTrump(int badSuit) override;
    int stickTrump(int badSuit) override;

    Card playCard(Trick &trick) override;

    std::bitset<Card::NUM_CARDS> getKnownCards();

private:
    std::bitset<Card::NUM_CARDS> knownCards;
};
