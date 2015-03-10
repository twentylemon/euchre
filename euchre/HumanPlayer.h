

#pragma once
#include "Player.h"

/**
 * A human controlled player
 */
class HumanPlayer : public Player
{
public:
    HumanPlayer();
    HumanPlayer(std::string name);
    
    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const;
    virtual std::pair<int,bool> pickItUp(const Card& top) const;
    virtual void replaceCard(const Card& top);
    virtual std::pair<int,bool> callTrump(int badSuit) const;
    virtual std::pair<int,bool> stickTrump(int badSuit) const;
    
    virtual Card playCard(const Trick& trick);
};
