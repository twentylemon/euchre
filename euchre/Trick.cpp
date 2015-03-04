
#include "Trick.h"

/**
 * @return the effectives suits of every card per trump value
 */
std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> effSuits() {
    std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> suits;
    for (int trump : Card::SUITS) {
        for (const Card& card : Card::ALL_CARDS) {
            suits[trump][card.hashCode()] = card.getEffectiveSuit(trump);
        }
    }
    return suits;
}

/**
 * effective suits per trump
 */
const std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> Trick::EFF_SUITS = effSuits();

/**
 * initializes this trick to be empty.
 * @param trump the suit that is trump
 */
Trick::Trick() {
    clear();
    setTrump(-1);
}


/**
 * initializes this trick to be empty.
 * @param trump the suit that is trump
 */
Trick::Trick(int trump) {
    clear();
    setTrump(trump);
}


/**
 * clears the list of cards in this trick
 */
void Trick::clear() {
    cards.clear();
    winner.fill(0);
}


/**
 * @return the suit that is trump
 */
int Trick::getTrump() const {
    return trump;
}


/**
 * @param trump the new suit to set to be trump
 */
void Trick::setTrump(int trump) {
    this->trump = trump;
#ifdef TRICK_INCLUDE_HASH
    hash = trump;
#endif
}


/**
 * @return the suit of the card that led, and will crash if no cards have been played so far
 */
int Trick::getLeadSuit() const {
    return leadSuit;
}

/**
 * @param card a card to see if it is legal to play that card in this trick
 * @return true if card is a legal card to play
 */
bool Trick::isLegal(const Card& card) const {
    return cards.empty() || getLeadSuit() == EFF_SUITS[getTrump()][card.hashCode()];
}


/**
 * @return the cards currently in the trick
 */
std::vector<Card> Trick::getCards() const {
    return cards;
}

/**
 * @return the number of cards in the trick
 */
int Trick::getNumCards() const {
    return cards.size();
}


/**
 * @param card the card to add to this trick
 */
void Trick::addCard(const Card& card) {
    if (!cards.empty()) {
        int curBest = CardScore::get(trump, getWinningCard().hashCode());
        int thisScore = CardScore::get(trump, card.hashCode());
        
        winner[cards.size()] = winner[cards.size() - 1];    //winner is previous winning card unless
        if (thisScore > curBest) {                          //this card is possibly better
            if (thisScore >= CardScore::MIN_TRUMP_SCORE ||  //they played a better trump card, or trumped the trick
                card.getSuit() == getLeadSuit()) {          //they followed lead suit and played a better card
                    winner[cards.size()] = cards.size();    //this card is winning the trick
            }
        }
    }
    else {
        leadSuit = EFF_SUITS[getTrump()][card.hashCode()];
    }
    cards.push_back(card);
#ifdef TRICK_INCLUDE_HASH
    hash = (hash << Card::HASHBITS) | card.hashCode();  //this hash states order is important
#endif
}


/**
 * remvoes the last played card from this trick
 */
void Trick::removeLastCard() {
    cards.pop_back();
#ifdef TRICK_INCLUDE_HASH
    hash = hash >> Card::HASHBITS;
#endif
}


/**
 * @return the id of the card that is winning this trick
 */
int Trick::getWinner() const {
    return winner[cards.size() - 1];
}


/**
 * @return the Card that is winning this trick
 */
const Card& Trick::getWinningCard() {
    return cards[getWinner()];
}

/**
 * @return the Card that is winning this trick
 */
const Card& Trick::getWinningCard() const {
    return cards[getWinner()];
}


#ifdef TRICK_INCLUDE_HASH
/**
 * @return a hash code for this trick
 */
int Trick::hashCode() const {
    return hash;
}

/**
 * @return a hashcode for the given cards if they were a trick
 */
int Trick::hashCode(int trump, int card1, int card2, int card3, int card4) {
    return (trump << (4*Card::HASHBITS+2)) | (card1 << (4*Card::HASHBITS)) | (card2 << (3*Card::HASHBITS)) | (card3 << (2*Card::HASHBITS)) | card4;
}

/**
 * @return a hashcode for the given cards if they were a trick
 */
int Trick::hashCode(int trump, const Card& card1, const Card& card2, const Card& card3, const Card& card4) {
    return hashCode(trump, card1.hashCode(), card2.hashCode(), card3.hashCode(), card4.hashCode());
}
#endif


/**
 * @return  string version of this trick
 */
std::string Trick::toString() const {
    std::string str = "T: " + Card::SUIT_SYMBOLS[trump] + ";";
    for (auto it = cards.begin(), end = cards.end(); it != end; it++) {
        str += " " + it->toString();
    }
    return str + "  W: " + (cards.empty() ? "N/A" : getWinningCard().toString());
}
