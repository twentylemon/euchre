

#include "MarkovPlayer.h"

/**
 * default threshold value if none is provided
 */
const double MarkovPlayer::DEFAULT_THRESHOLD = 0.20;//0.25;

/**
 * constructor
 */
MarkovPlayer::MarkovPlayer() : threshold(DEFAULT_THRESHOLD), Player() {
}

/**
 * @param threshold the threshold for this player
 */
MarkovPlayer::MarkovPlayer(double threshold) : threshold(threshold), Player() {
}

/**
 * @param name the name of this player
 */
MarkovPlayer::MarkovPlayer(std::string name) : threshold(DEFAULT_THRESHOLD), Player(name) {
}

/**
 * @param name the name of this player
 * @param threshold the threshold for this player
 */
MarkovPlayer::MarkovPlayer(std::string name, double threshold) : threshold(threshold), Player(name) {
}


/**
 * preps the ai player for a new hand.
 */
void MarkovPlayer::startNewHand() {
    knownCards.reset();
    Player::startNewHand();
}


/**
 * @param card the card to set as seen for this player
 * @param playerIDX the player that played the card
 */
void MarkovPlayer::publicKnowledge(const Card& card, int playerIDX) {
    knownCards.set(Card::HASH_IDX[card.hashCode()], true);
}


/**
 * @return the bitset of all cards known by this player
 */
std::bitset<Card::NUM_CARDS> MarkovPlayer::getKnownCards() const {
    return knownCards;
}

/**
 * @return the current threshold for this player
 */
double MarkovPlayer::getThreshold() const {
    return threshold;
}

/**
 * @param threshold the new threshold for this player
 */
void MarkovPlayer::setThreshold(double threshold) {
    this->threshold = threshold;
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card MarkovPlayer::playCard(const Trick& trick) {
    int deckScore = CardScore::sumWinRate(trick.getTrump(), ~getKnownCards());
    int handScore = CardScore::sumWinRate(trick.getTrump(), hand.getPlayableBitset(trick));
    if ((double)handScore / (double)deckScore >= getThreshold()) {
        return hand.removeCard(CardScore::getBestCard(hand.getPlayableCards(trick), trick));
    }
    return hand.removeCard(CardScore::getWorstCard(hand.getPlayableCards(trick), trick));
}
