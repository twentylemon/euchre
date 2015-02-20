
#include "Hand.h"

/**
 * constructor sets up an empty hand
 */
Hand::Hand(){
    clear();
}

/**
 * clears all cards from this hand
 */
void Hand::clear(){
    numCards = 0;
    set = 0;
    bits.reset();
}


/**
 * @param card the card to add to this hand
 */
void Hand::addCard(Card card){
    addCard(card.hashCode());
}


/**
 * @param hash the hashcode of the card to add to this hand
 */
void Hand::addCard(int hash){
    hand[numCards++] = hash;
    bits.set(Card::HASH_IDX[hash], true);
}


/**
 * @param idx the index of the card to return
 * @return the card at the specified index
 */
int Hand::getCard(int idx){
    return hand[idx];
}


/**
 * @param idx the index of the card to remove
 * @return the removed card
 */
int Hand::removeCard(int idx){
    int card = getCard(idx);
    numCards--;
    for (int i = idx; i < numCards; i++){
        hand[i] = hand[i+1];    //shift the other cards down
    }
    return card;
}


/**
 * @param card the card to remove from this card
 * @return the removed card, or -1 if it was not found
 */
int Hand::removeCard(Card card){
    for (int i = 0; i < numCards; i++){
        if (card.hashCode() == getCard(i)){
            return removeCard(i);
        }
    }
    return -1;
}

/**
 * @return the last card added to this card, which is also removed from this hand
 */
int Hand::removeLastCard(){
    return hand[--numCards];
}


/**
 * @return the number of cards in this hand
 */
int Hand::getNumCards(){
    return numCards;
}


/**
 * @return the bitset of hands in this hand
 */
std::bitset<Card::NUM_CARDS> Hand::getBitset(){
    return bits;
}


/**
 * @return a string representation of this hand
 */
std::string Hand::toString(){
    std::string str = "";
    for (int i = 0; i < numCards; i++){
        str += Card(getCard(i)).toString() + " ";
    }
    return str;
}


/**
 * @param bits bitstring of cards to add to this hand
 */
void Hand::addSet(int bits){
    int idx = 0;
    set = bits;
    while (bits > 0){
        if (bits & 1){
            addCard(Card::ALL_CARDS[idx]);
        }
        idx++;
        bits = bits >> 1;
    }
}

/**
 * @return the bitset of cards in this hand
 */
int Hand::getSet(){
    return set;
}

/**
 * @param hand bitset hand to test for an intersection of cards
 * @return true if the two hands have common cards
 */
int Hand::intersects(Hand hand){
    return set & hand.getSet();
}
