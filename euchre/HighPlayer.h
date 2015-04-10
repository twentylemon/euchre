
#pragma once
#include "Player.h"
#include <algorithm>

/**
 * an agent that plays the highest card in his hand
 */
class HighPlayer : public Player
{
public:
    HighPlayer();
    HighPlayer(std::string name);

    /*
    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const override;
    virtual std::pair<int,bool> pickItUp(const Card& top) const override;
    virtual void replaceCard(const Card& top) override;
    virtual std::pair<int,bool> callTrump(int badSuit) const override;
    virtual std::pair<int,bool> stickTrump(int badSuit) const override;
    */
    Card playCard(const Trick& trick) override;
};
