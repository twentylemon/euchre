
#include "Hand.h"

/**
 * @return all possible hands per size
 */
std::array<std::vector<Hand>, Hand::NUM_CARDS+1> allHands() {
    std::array<std::vector<Hand>, Hand::NUM_CARDS+1> hands;
    for (int size = 1; size <= Hand::NUM_CARDS; size++) {
        int start = BitString::first(size);
        int finish = BitString::last(size, Card::NUM_CARDS);
        for (int bits = start; bits <= finish; bits = BitString::next(bits)) {
            hands[size].push_back(Hand(std::bitset<Card::NUM_CARDS>(bits)));
        }
        hands[size].shrink_to_fit();
    }
    return hands;
}

/**
 * all possible euchre hands
 */
const std::array<std::vector<Hand>, Hand::NUM_CARDS+1> Hand::ALL_HANDS = allHands();


/**
 * constructor sets up an empty hand
 */
Hand::Hand() {
    clear();
}

/**
 * @param bits the cards to add to this hand
 */
Hand::Hand(std::bitset<Card::NUM_CARDS> bits) {
    clear();
    addSet(bits);
}

/**
 * clears all cards from this hand
 */
void Hand::clear() {
    numCards = 0;
    bits.reset();
}


/**
 * @param card the card to add to this hand
 */
void Hand::addCard(const Card& card) {
    addCard(card.hashCode());
}


/**
 * @param hash the hashcode of the card to add to this hand
 */
void Hand::addCard(int hash) {
    hand[numCards++] = hash;
    bits.set(Card::HASH_IDX[hash], true);
}


/**
 * @param bits bitstring of cards to add to this hand
 */
void Hand::addSet(unsigned int bits) {
    addSet(std::bitset<Card::NUM_CARDS>(bits));
}

/**
 * @param bits the bitset of cards to add to this hand
 */
void Hand::addSet(std::bitset<Card::NUM_CARDS> bits) {
    for (int i = 0; bits.any(); i++) {
        if (bits[i]) {
            bits.set(i, false);
            addCard(Card::ALL_CARDS[i]);
        }
    }
}


/**
 * @param idx the index of the card to return
 * @return the card at the specified index
 */
int Hand::getCard(int idx) const {
    return hand[idx];
}


/**
 * @param idx the index of the card to remove
 * @return the removed card
 */
int Hand::removeCardIDX(int idx) {
    int card = getCard(idx);
    bits.set(Card::HASH_IDX[card], false);
    numCards--;
    for (int i = idx; i < numCards; i++) {
        hand[i] = hand[i+1];    //shift the other cards down
    }
    return card;
}

/**
 * @param hash the card to remove
 * @return the removed card
 */
int Hand::removeCard(int hash) {
    for (int i = 0; i < numCards; i++ ) {
        if (hash == getCard(i)) {
            return removeCardIDX(i);
        }
    }
    return -1;
}


/**
 * @param card the card to remove from this card
 * @return the removed card, or -1 if it was not found
 */
int Hand::removeCard(const Card& card) {
    return removeCard(card.hashCode());
}

/**
 * @return the last card added to this card, which is also removed from this hand
 */
int Hand::removeLastCard() {
    return removeCard(numCards - 1);
}


/**
 * @return the number of cards in this hand
 */
int Hand::getNumCards() const {
    return numCards;
}


/**
 * @return the bitset of hands in this hand
 */
std::bitset<Card::NUM_CARDS> Hand::getBitset() const {
    return bits;
}


/**
 * @return list of all the cards in this hand
 */
std::vector<int> Hand::getCards() const {
    std::vector<int> cards;
    for (int i = 0; i < getNumCards(); i++){
        cards.push_back(getCard(i));
    }
    return cards;
}

/**
 * @param suit the suit of cards to return from this hand
 * @return list of cards of the given suit from this hand
 */
std::vector<int> Hand::getSuitCards(int suit) const {
    std::vector<int> cards;
    int hash = Card(9, suit).hashCode();
    for (int i = 0; i < getNumCards(); i++) {
        if (Card::sameSuit(getCard(i), hash)) {
            cards.push_back(getCard(i));
        }
    }
    return cards;
}

/**
 * @param trick the trick that we are playing into
 * @return list of legal cards from this hand
 */
std::vector<int> Hand::getLegalCards(const Trick &trick) const {
    std::vector<int> legalCards;
    for (int i = 0; i < getNumCards(); i++) {
        if (trick.isLegal(getCard(i))) {
            legalCards.push_back(getCard(i));
        }
    }
    return legalCards;
}


/**
 * @return a hashcode of this hand
 */
int Hand::hashCode() const {
    return bits.to_ulong();
}


/**
 * @return a string representation of this hand
 */
std::string Hand::toString() const {
    std::string str = "";
    for (int i = 0; i < numCards; i++) {
        str += Card(getCard(i)).toString() + " ";
    }
    return str;
}


/**
 * @param hand the hand to test for an intersection of cards
 * @return true if the two hands have common cards
 */
bool Hand::intersects(const Hand& hand) const {
    return intersects(hand.getBitset());
}

/**
 * @param bits a bitset of cards to check if this hand intersects it
 * @return true if the two hands have common cards
 */
bool Hand::intersects(unsigned int bits) const {
    return (getBitset().to_ulong() & bits) != 0;
}

/**
 * @param bits a bitset of cards to check if this hand intersects it
 * @return true if this hand intersects the bitset of cards given
 */
bool Hand::intersects(std::bitset<Card::NUM_CARDS> bits) const {
    return (getBitset() & bits).any();
}
