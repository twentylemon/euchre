
#include "Hand.h"

#ifdef HAND_ALL_HANDS
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
#endif

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
    bits.reset();
    hand.clear();
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
    hand.push_back(hash);
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
    hand.erase(hand.begin() + idx);
    return card;
}

/**
 * @param hash the card to remove
 * @return the removed card
 */
int Hand::removeCard(int hash) {
    for (int i = 0; i < getNumCards(); i++ ) {
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
    return removeCard(getNumCards() - 1);
}


/**
 * @return the number of cards in this hand
 */
int Hand::getNumCards() const {
    return hand.size();
}


/**
 * @return the bitset of hands in this hand
 */
std::bitset<Card::NUM_CARDS> Hand::getBitset() const {
    return bits;
}

/**
 * @param suit the suit of cards to return from this hand
 * @return list of cards of the given suit from this hand
 */
std::bitset<Card::NUM_CARDS> Hand::getSuitBitset(int suit) const {
    std::bitset<Card::NUM_CARDS> cards;
    int hash = Card::hashCode(9, suit);
    for (int card : hand) {
        if (Card::sameSuit(card, hash)) {
            cards.set(Card::HASH_IDX[card], true);
        }
    }
    return cards;
}

/**
 * @param trick the trick that we are playing into
 * @return list of legal cards from this hand
 */
std::bitset<Card::NUM_CARDS> Hand::getLegalBitset(const Trick &trick) const {
    std::bitset<Card::NUM_CARDS> legalCards;
    for (int card : hand) {
        if (trick.isLegal(card)) {
            legalCards.set(Card::HASH_IDX[card], true);
        }
    }
    return legalCards;
}

/**
 * @param trick the trick that we are playing into
 * @return list of all playable cards from this hand
 */
std::bitset<Card::NUM_CARDS> Hand::getPlayableBitset(const Trick &trick) const {
    std::bitset<Card::NUM_CARDS> cards = getLegalBitset(trick);
    if (!cards.any()) {
        return getBitset();
    }
    return cards;
}


/**
 * @return list of all the cards in this hand
 */
std::vector<int> Hand::getCards() const {
    return hand;
}

/**
 * @param suit the suit of cards to return from this hand
 * @return list of cards of the given suit from this hand
 */
std::vector<int> Hand::getSuitCards(int suit) const {
    std::vector<int> cards;
    int hash = Card::hashCode(9, suit);
    for (int card : hand) {
        if (Card::sameSuit(card, hash)) {
            cards.push_back(card);
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
    for (int card : hand) {
        if (trick.isLegal(card)) {
            legalCards.push_back(card);
        }
    }
    return legalCards;
}

/**
 * @param trick the trick that we are playing into
 * @return list of all playable cards from this hand
 */
std::vector<int> Hand::getPlayableCards(const Trick &trick) const {
    std::vector<int> cards = getLegalCards(trick);
    if (cards.empty()) {
        return getCards();
    }
    return cards;
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
    for (int card : hand) {
        str += Card(card).toString() + " ";
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
bool Hand::intersects(const std::bitset<Card::NUM_CARDS>& bits) const {
    return (getBitset() & bits).any();
}

/**
 * @param bits a bitset of cards to check if this hand is contained in the bitset
 * @return true if this hand intersects the bitset of cards given
 */
bool Hand::subsetOf(const std::bitset<Card::NUM_CARDS>& bits) const {
    return (getBitset() & bits) == getBitset();
}
