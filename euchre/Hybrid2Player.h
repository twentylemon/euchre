

#pragma once
#include "HybridPlayer.h"


class Hybrid2Player : public HybridPlayer
{
public:
    Hybrid2Player();
    Hybrid2Player(double threshold);
    Hybrid2Player(std::string name);
    Hybrid2Player(std::string name, double threshold);

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

