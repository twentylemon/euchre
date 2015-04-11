

#pragma once
#include "CardCountingPlayer.h"


/**
 * combines the strategies of CardCounting and PartnerHL using the threshold from Markov
 */
class HybridPlayer : public CardCountingPlayer
{
public:
    HybridPlayer();
    HybridPlayer(double threshold);
    HybridPlayer(std::string name);
    HybridPlayer(std::string name, double threshold);

    const static double DEFAULT_THRESHOLD;

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
    double threshold;
};
