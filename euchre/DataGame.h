

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

    virtual void draw() const override;

    virtual void play(int numGames);
};
