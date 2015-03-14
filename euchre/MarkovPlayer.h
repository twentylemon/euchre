

#pragma once
#include "Player.h"

/**
 * agent that uses the markov desicion process
 * keeps track of cards that have been seen
 * on play: sum win rates of cards in deck (D) and out hand (H)
 * if H/D >= threshold, play high, else low
 * 0 <= theshold <= 1; 0: always high, 1: always low
 */
class MarkovPlayer : public Player
{
public:
    MarkovPlayer();
    MarkovPlayer(double threshold);
    MarkovPlayer(std::string name);
    MarkovPlayer(std::string name, double threshold);

    const static double DEFAULT_THRESHOLD;
    
    virtual void startNewHand() override;
    virtual void publicKnowledge(const Card& card, int playerIDX) override;

    virtual std::bitset<Card::NUM_CARDS> getKnownCards() const;
    virtual void setThreshold(double threshold);
    virtual double getThreshold() const;

    /*
    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const override;
    virtual std::pair<int,bool> pickItUp(const Card& top) const override;
    virtual void replaceCard(const Card& top) override;
    virtual std::pair<int,bool> callTrump(int badSuit) const override;
    virtual std::pair<int,bool> stickTrump(int badSuit) const override;
    */
    virtual Card playCard(const Trick& trick) override;

protected:
    std::bitset<Card::NUM_CARDS> knownCards;
    double threshold;
};
