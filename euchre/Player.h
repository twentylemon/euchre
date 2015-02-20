
#pragma once
#include "Card.h"
#include "Hand.h"
#include "Trick.h"
#include <iostream>

class Player
{
public:
    Player();

    static enum Action { Pass = -1, OrderUp = 0, Alone = 4 };
 
    virtual void startNewHand();

    virtual void addCard(Card card);

    virtual Hand getHand();

    virtual std::string toString();

    /**
     * playing the game functions
     * each return one of:
     *  Player::Action::Pass        passes calling, or denies picking up the card
     *  one of Card::Suit           calls that suit
     *  (Card::Suit)+Player::Alone  calls that suit and is going alone
     */
    virtual int orderUp(Card top);
    virtual int pickItUp(Card top);
    virtual int callTrump(int badSuit);
    virtual int stickTrump(int badSuit);

    virtual Card playCard(Trick &trick);

protected:
    Hand hand;

    int responseToAction(int suit, char action);
};

