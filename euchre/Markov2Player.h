

#pragma once
#include "MarkovPlayer.h"


class Markov2Player : public MarkovPlayer
{
public:
    Markov2Player();
    Markov2Player(double threshold);
    Markov2Player(std::string name);
    Markov2Player(std::string name, double threshold);

    const static double DEFAULT_THRESHOLD;

    /*
    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const override;
    virtual std::pair<int,bool> pickItUp(const Card& top) const override;
    virtual void replaceCard(const Card& top) override;
    virtual std::pair<int,bool> callTrump(int badSuit) const override;
    virtual std::pair<int,bool> stickTrump(int badSuit) const override;
    */
    virtual Card playCard(const Trick& trick) override;
};

