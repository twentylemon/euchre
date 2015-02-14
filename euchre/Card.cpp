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
 * initializes this card using the rank and suit
 * @param rank the rank of this card
 * @param suit the suit of this card
 */
Card::Card(int data){
    this->data = data;
    suit = data & 0x03;         //first 2 bits on, rest off
    rank = (data & 0x3C) >> 2;  //all bits on expect first 2
}

/**
 * initializes this card using the rank and suit
 * @param rank the rank of this card
 * @param suit the suit of this card
 */
Card::Card(int rank, int suit){
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
