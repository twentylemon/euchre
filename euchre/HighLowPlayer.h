

#pragma once
#include "Player.h"
#include "LowPlayer.h"
#include "HighPlayer.h"

/**
 * plays the best card from their hand if they can win the trick so far
 * otherwise plays the worst card
 * to lead, play the best card
 */
class HighLowPlayer : public Player
{
public:
    HighLowPlayer();
    HighLowPlayer(std::string name);

    /*
    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const override;
    virtual std::pair<int,bool> pickItUp(const Card& top) const override;
    virtual void replaceCard(const Card& top) override;
    virtual std::pair<int,bool> callTrump(int badSuit) const override;
    virtual std::pair<int,bool> stickTrump(int badSuit) const override;
    */
    virtual Card playCard(const Trick& trick) override;
};
