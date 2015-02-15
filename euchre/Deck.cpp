#include "Deck.h"

/**
 * constructs a new unshuffled deck
 */
Deck::Deck(){
    //copy() just somehow knows to call the Card(int) constructor?
    std::copy(Card::ALL_CARDS.begin(), Card::ALL_CARDS.end(), cards.begin());
    /*for (int hash : Card::ALL_CARDS){
        cards[hash - Card::MIN_CARD] = Card(hash);
    }*/
    /*for (int i = 0; i < SIZE; i++){
        cards[i] = Card(i + Card::MIN_CARD);
    }*/
    std::fill(used.begin(), used.end(), false);
    updatePositions();
}


/**
 * updates the array that keeps track of where every card is in the deck
 */
void Deck::updatePositions(){
    std::fill(positions.begin(), positions.end(), -1);
    for (int i = 0; i < SIZE; i++){
        positions[cards[i].hashCode()] = i;
    }
}


/**
 * shuffles the deck
 */
void Deck::shuffle(){
    std::random_shuffle(cards.begin(), cards.end());
    updatePositions();
}


/**
 * @param card the card to get the index of
 * @return the index of the Card in the deck
 */
int Deck::indexOf(Card card){
    return positions[card.hashCode()];
}


/**
 * @param card the card to check if it is used, ie in play
 * @return true if the card is used, ie in play
 */
bool Deck::isUsed(Card card){
    return isUsed(indexOf(card));
}

/**
 * @param idx the card index to check if it is used, ie in play
 * @return true if the card is used, ie in play
 */
bool Deck::isUsed(int idx){
    return used[idx];
}

/**
 * @param card the card to set the used flag of, ie in play or not
 * @param used what to set the flag to
 */
void Deck::setUsed(Card card, bool used){
    setUsed(indexOf(card), used);
}

/**
 * @param idx the card index to set the used flag of, ie in play or not
 * @param used what to set the flag to
 */
void Deck::setUsed(int idx, bool used){
    this->used[idx] = used;
}


/**
 * @param idx the index of the card to return
 * @return the card at idx
 */
Card Deck::getCard(int idx){
    return cards[idx];
}


/**
 * @param idx the idx to return the next unplayed card after
 * @return the index of the next unplayed card in the deck after idx
 * @todo fix this
 */
int Deck::getNextIdx(int idx){
    while (used[++idx]);
    return idx;
}
