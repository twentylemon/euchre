
#pragma once
#include "Random.h"
#include "Card.h"
#include "Hand.h"
#include "Trick.h"
#include "Player.h"
#include <utility>

/**
 * a euchre agent. this agent just plays a random legal card from their hand
 * @author Taras Mychaskiw
 */
class RandomPlayer : public Player
{
public:
    RandomPlayer();
    RandomPlayer(std::string name);

    std::pair<int,bool> orderUp(Card top, bool yourTeam) override;
    std::pair<int,bool> pickItUp(Card top) override;
    void replaceCard(Card top) override;
    std::pair<int,bool> callTrump(int badSuit) override;
    std::pair<int,bool> stickTrump(int badSuit) override;

    Card playCard(Trick &trick) override;
};
