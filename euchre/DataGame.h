

#pragma once
#include "Player.h"
#include "EuchreGame.h"
#include <array>

/**
 * game for data collection
 */
class DataGame : public EuchreGame
{
public:
    DataGame();
    DataGame(Player* up, Player* down, Player* left, Player* right);

    virtual void init() override;
#ifndef MONTE_CARLO
    virtual void draw() const override;
#endif

    virtual void play(int numGames);
};
