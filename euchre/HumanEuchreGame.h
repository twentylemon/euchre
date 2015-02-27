
#pragma once
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "AIPlayer.h"
#include "EuchreGame.h"

#include <array>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>


/**
 * the euchre game master
 */
class HumanEuchreGame : public EuchreGame
{
public:
    static const int HUMAN = EuchreGame::DOWN;
    static const int PARTNER = EuchreGame::UP;
    static const int HUMAN_TEAM = EuchreGame::UP_TEAM;
    static const int AI_TEAM = EuchreGame::LEFT_TEAM;

    HumanEuchreGame();

    virtual void draw() override;

private:
    Player human;
    AIPlayer partner;
    AIPlayer left, right;
};
