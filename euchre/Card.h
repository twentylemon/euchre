#pragma once
#include <string>
#include <array>

/**
 * An individual euchre card, with suit and rank.
 * @author Taras Mychaskiw
 */
class Card
{
public:
    Card(){}
    Card(int data);
    Card(int rank, int suit);

    static enum Suit { Spades = 0, Hearts = 1, Clubs = 2, Diamonds = 3 };   //other suit: (s+2)%4
    static enum Rank { Jack = 11, Queen = 12, King = 13, Ace = 14 };

    static const int NUM_SUITS = 4;
    static const int NUM_RANKS = 6;
    static const std::array<int, NUM_SUITS> SUITS;
    static const std::array<int, NUM_RANKS> RANKS;
    static const std::array<int, NUM_SUITS*NUM_RANKS> ALL_CARDS;

    static const int MIN_CARD = 36;
    static const int MAX_CARD = 60;

    static const std::string RANK_SYMBOLS[];
    static const std::string SUIT_SYMBOLS[];

    int getSuit();
    int getRank();
    int hashCode();

    std::string toString();

    bool operator==(Card card);

private:
    int data;
    int suit;
    int rank;

    void setData(int data);
    void setRankSuit(int rank, int suit);
};
