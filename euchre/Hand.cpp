

#include "Hand.h"

/**
 * constructor sets up an empty hand
 */
Hand::Hand(){
    hand = 0;
}


/**
 * @param card the card to add to this hand
 * @todo make it so the order of cards is not important to the hash
 */
void Hand::addCard(Card card){
    hand = (hand << Card::HASHBITS) | card.hashCode();
}


/**
 * remvoes the last card added to this hand
 */
void Hand::removeLastCard(){
    hand = hand >> Card::HASHBITS;
}


/**
 * @param hash the hashcode of the card to check if this hand contains
 * @return true if this hand contains the card
 */
bool Hand::contains(int hash){
    return hash == getCard(0) || hash == getCard(1) || hash == getCard(2) || hash == getCard(3) || hash == getCard(4);
}

/**
 * @param card the card to check if this hand contains
 * @return true if this hand contains the card
 */
bool Hand::contains(Card card){
    return contains(card.hashCode());
}


/**
 * @param n the index of the card to get
 * @return the hashcode of the nth card
 */
int Hand::getCard(int n){
    return (hand >> (n * Card::HASHBITS)) & 0x3F;
}


/**
 * @param card another Hand to test for equivalence
 * @return true if the two hands are the same
 * @todo [9H 10H JH QH KH] should be equal to [9D 10D JD QD KD]
 */
bool Hand::operator==(Hand hand){
    return this->hand == hand.hand;
}