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
    Card(int hash);
    Card(int rank, int suit);

    static enum Suit { Spades = 0, Hearts = 1, Diamonds = 2, Clubs = 3 };   //other suit: xor 0x3
    static enum Rank { Jack = 11, Queen = 12, King = 13, Ace = 14 };

    static const int NUM_SUITS = 4;
    static const int NUM_RANKS = 6;
    static const int NUM_CARDS = NUM_SUITS * NUM_RANKS;
    static const int MIN_CARD = 36; //min hashcode
    static const int MAX_CARD = 60; //max hashcode
    static const int HASHBITS = 6;  //number of bits in each hashcode

    static const std::array<int, NUM_SUITS> SUITS;
    static const std::array<int, NUM_RANKS> RANKS;
    static const std::array<int, NUM_CARDS> ALL_CARDS;
    static const std::array<int, MAX_CARD> HASH_IDX;

    static const std::string RANK_SYMBOLS[];
    static const std::string SUIT_SYMBOLS[];

    static int otherSuit(int suit);

    int getSuit();
    int getRank();
    int hashCode();

    std::string toString();

    bool operator==(Card card);

private:
    int hash;
    int suit;
    int rank;

    void setHashCode(int hash);
    void setRankSuit(int rank, int suit);
};
