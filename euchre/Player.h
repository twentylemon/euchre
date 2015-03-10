
#pragma once
#include "Card.h"
#include "Hand.h"
#include "Trick.h"
#include <string>
#include <utility>
#include <iostream>

/**
 * base class for players. provides basic "skip turn" implementations for each action
 */
class Player
{
public:
    Player();
    Player(std::string name);

    static enum Action { Pass = -1, OrderUp = 0, Alone = 4 };

    std::string getName() const;
    void setName(std::string name);

    const Hand& getHand() const;
    int getNumCards() const;

    int getPosition() const;
    void setPosition(int position);
 
    virtual void startNewHand();

    virtual void addCard(const Card& card);

    virtual std::string toString() const;

    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const;
    virtual std::pair<int,bool> pickItUp(const Card& top) const;
    virtual void replaceCard(const Card& top);
    virtual std::pair<int,bool> callTrump(int badSuit) const;
    virtual std::pair<int,bool> stickTrump(int badSuit) const;

    virtual Card playCard(const Trick& trick);

protected:
    int position;
    Hand hand;
    std::string name;
};
