
#pragma once
#include "Card.h"
#include "Hand.h"
#include "Trick.h"
#include <string>
#include <utility>
#include <iostream>

class Player
{
public:
    Player();
    Player(std::string name);

    static enum Action { Pass = -1, OrderUp = 0, Alone = 4 };

    std::string getName();
    void setName(std::string name);

    Hand getHand();
    int getNumCards();

    int getPosition();
    void setPosition(int position);
 
    virtual void startNewHand();

    virtual void addCard(Card card);

    virtual std::string toString();

    virtual std::pair<int,bool> orderUp(Card top, bool yourTeam);
    virtual std::pair<int,bool> pickItUp(Card top);
    virtual void replaceCard(Card top);
    virtual std::pair<int,bool> callTrump(int badSuit);
    virtual std::pair<int,bool> stickTrump(int badSuit);

    virtual Card playCard(Trick &trick);

protected:
    int position;
    Hand hand;
    std::string name;
};
