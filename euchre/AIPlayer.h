
#pragma once
#include "Card.h"
#include "Hand.h"
#include "Trick.h"
#include "Player.h"
#include <ppl.h>
#include <atomic>
#include <string>
#include <bitset>
#include <utility>


/**
 * an ai player
 * @author Taras Mychaskiw
 */
class AIPlayer : public Player
{
public:
    AIPlayer();
    AIPlayer(std::string name);

    void startNewHand() override;

    void addCard(Card card) override;
    void seenCard(Card card);

    std::string toString() override;

    std::pair<int,bool> orderUp(Card top, bool yourTeam) override;
    std::pair<int,bool> pickItUp(Card top) override;
    void replaceCard(Card top) override;
    std::pair<int,bool> callTrump(int badSuit) override;
    std::pair<int,bool> stickTrump(int badSuit) override;

    Card playCard(Trick &trick) override;

    std::bitset<Card::NUM_CARDS> getKnownCards();

private:
    std::bitset<Card::NUM_CARDS> knownCards;
};
