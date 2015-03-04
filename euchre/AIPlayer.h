
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

    std::string toString() const override;

    std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const override;
    std::pair<int,bool> pickItUp(const Card& top) const override;
    void replaceCard(const Card& top) override;
    std::pair<int,bool> callTrump(int badSuit) const override;
    std::pair<int,bool> stickTrump(int badSuit) const override;

    Card playCard(const Trick& trick) override;

    std::bitset<Card::NUM_CARDS> getKnownCards() const;

private:
    std::bitset<Card::NUM_CARDS> knownCards;
};
