

#include "Deck.h"

/**
 * constructs a new unshuffled deck
 */
Deck::Deck(){
    std::copy(Card::ALL_CARDS.begin(), Card::ALL_CARDS.end(), cards.begin());
    used.reset();
    top = 0;
    updatePositions();
}


/**
 * updates the array that keeps track of where every card is in the deck
 */
void Deck::updatePositions(){
    for (int i = 0; i < SIZE; i++){
        positions[getCard(i).hashCode()] = i;
    }
}


/**
 * shuffles the deck, sets all cards to not used
 */
void Deck::shuffle(){
    used.reset();
    top = 0;
    std::random_shuffle(cards.begin(), cards.end());
    updatePositions();
}


/**
 * @return the top card of the deck, which is also set to be `used`
 */
Card Deck::pop(){
    setUsed(top, true);
    return getCard(top++);
}

/**
 * @param card the card to get the index of
 * @return the index of the Card in the deck
 */
int Deck::indexOf(Card card){
    return indexOf(card.hashCode());
}

/**
 * @param hash the hashcode of the card to get the index of
 * @return the index of the card in the deck
 */
int Deck::indexOf(int hash){
    return positions[hash];
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
    return used[Card::HASH_IDX[getCard(idx).hashCode()]];
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
    this->used.set(Card::HASH_IDX[getCard(idx).hashCode()], used);
}


/**
 * @return the bitset of hands in this hand
 */
std::bitset<Card::NUM_CARDS> Deck::getBitset(){
    return used;
}


/**
 * @param idx the index of the card to return
 * @return the card at idx
 */
Card Deck::getCard(int idx){
    return cards[idx];
}


/**
 * @return a string representation of this deck
 */
std::string Deck::toString(){
    std::string str = "";
    for (int i = 0; i < SIZE; i++){
        str += getCard(i).toString();
    }
    str += "\n";
    for (int i = 0; i < SIZE; i++){
        if (isUsed(i)){
            str += "--";
        }
        else {
            str += "  ";
        }
    }
    return str + "\n" + used.to_string();
}
