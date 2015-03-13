
#pragma once
#include "HighLowPlayer.h"

/**
 * checks their partners card before considering
 * if their partner has played a card and is winning, play low
 * otherwise, use HighLow strategy
 */
class PartnerHLPlayer : public HighLowPlayer
{
public:
    PartnerHLPlayer();
    PartnerHLPlayer(std::string name);

    /*
    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const override;
    virtual std::pair<int,bool> pickItUp(const Card& top) const override;
    virtual void replaceCard(const Card& top) override;
    virtual std::pair<int,bool> callTrump(int badSuit) const override;
    virtual std::pair<int,bool> stickTrump(int badSuit) const override;
    */
    virtual Card playCard(const Trick& trick) override;
};
