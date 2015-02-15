#include "Card.h"

/**
 * the symbols for each rank of card
 */
const std::string Card::RANK_SYMBOLS[] = { "?", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

/**
 * the symbols for each suit of card
 */
const std::string Card::SUIT_SYMBOLS[] = { "S", "H", "C", "D" };

/**
 * list of all suits, in order so that (suit+2)%4 = other suit of same colour
 */
const std::array<int, Card::NUM_SUITS> Card::SUITS = { Card::Spades, Card::Hearts, Card::Clubs, Card::Diamonds };

/**
 * list of all ranks in order
 */
const std::array<int, Card::NUM_RANKS> Card::RANKS = { 9, 10, Card::Jack, Card::Queen, Card::King, Card::Ace };

/**
 * list of all cards by hash code
 */
const std::array<int, Card::NUM_SUITS*Card::NUM_RANKS> Card::ALL_CARDS = { 36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59 };


/**
 * initializes this card using the rank and suit
 * @param rank the rank of this card
 * @param suit the suit of this card
 */
Card::Card(int data){
    setData(data);
}

/**
 * initializes this card using the rank and suit
 * @param rank the rank of this card
 * @param suit the suit of this card
 */
Card::Card(int rank, int suit){
    setRankSuit(rank, suit);
}

/**
 * @param data the data code to set this card to
 */
void Card::setData(int data){
    this->data = data;
    suit = data & 0x03;         //first 2 bits on, rest off
    rank = (data & 0x3C) >> 2;  //all bits on expect first 2
}

/**
 * @param rank the rank to set this card to
 * @param suit the suit to set this card to
 */
void Card::setRankSuit(int rank, int suit){
    data = (rank << 2) | suit;
    this->suit = suit;
    this->rank = rank;
}



/**
 * @return the suit of this card
 */
int Card::getSuit(){
    return suit;
}


/**
 * @return the rank of this card, with jack=11,...,ace=14
 */
int Card::getRank(){
    return rank;
}

/**
 * @return a hashcode for this card
 */
int Card::hashCode(){
    return data;
}


/**
 * @return  string version of this card, ie ace of hearts gives "AH"
 */
std::string Card::toString(){
    return RANK_SYMBOLS[rank] + SUIT_SYMBOLS[suit];
}


/**
 * @param card another Card to test for equivalence
 * @return true if the Card sent has the same rank and suit as this card
 */
bool Card::operator==(Card card){
    //return data == card.data;
    return suit == card.suit && rank == card.rank && data == card.data;
}
