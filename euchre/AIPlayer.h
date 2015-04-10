
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

    virtual void startNewHand() override;
    
    virtual void publicKnowledge(const Card& card, int playerIDX) override;

    virtual std::string toString() const override;

    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const override;
    virtual std::pair<int,bool> pickItUp(const Card& top) const override;
    virtual void replaceCard(const Card& top) override;
    virtual std::pair<int,bool> callTrump(int badSuit) const override;
    virtual std::pair<int,bool> stickTrump(int badSuit) const override;

    virtual Card playCard(const Trick& trick) override;

    std::bitset<Card::NUM_CARDS> getKnownCards() const;

private:
    std::bitset<Card::NUM_CARDS> knownCards;
};
