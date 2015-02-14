#pragma once
#include <string>

/**
 * An individual euchre card, with suit and rank.
 * @author Taras Mychaskiw
 */
class Card
{
public:

    static enum Suit { Spades = 0, Hearts = 1, Clubs = 2, Diamonds = 3 };   //other suit: (s+2)%4
    static enum Rank { Jack = 11, Queen = 12, King = 13, Ace = 14 };

    static const int FIRST_SUIT = Spades;
    static const int LAST_SUIT = Diamonds;

    static const std::string RANK_SYMBOLS[];
    static const std::string SUIT_SYMBOLS[];
    
    Card(int data);
    Card(int rank, int suit);
    ~Card(){}

    int getSuit();
    int getRank();
    int hashCode();

    std::string toString();

    bool operator==(Card card);

private:
    int data;
    int suit;
    int rank;
};
