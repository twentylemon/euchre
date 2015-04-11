

#include "Markov2Player.h"


/**
 * default threshold value if none is provided
 */
const double Markov2Player::DEFAULT_THRESHOLD = 0.25;

/**
 * constructor
 */
Markov2Player::Markov2Player() : MarkovPlayer(DEFAULT_THRESHOLD) {
}

/**
 * @param threshold the threshold for this player
 */
Markov2Player::Markov2Player(double threshold) : MarkovPlayer(threshold) {
}

/**
 * @param name the name of this player
 */
Markov2Player::Markov2Player(std::string name) : MarkovPlayer(name, DEFAULT_THRESHOLD) {
}

/**
 * @param name the name of this player
 * @param threshold the threshold for this player
 */
Markov2Player::Markov2Player(std::string name, double threshold) : MarkovPlayer(name, threshold) {
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card Markov2Player::playCard(const Trick& trick) {
    double deckScore = CardScore::sumPartialWinRate(trick, ~getKnownCards());
    double handScore = CardScore::sumPartialWinRate(trick, hand.getBitset());
    if (handScore / deckScore >= getThreshold()) {
        return hand.removeCard(CardScore::getBestCard(hand.getPlayableCards(trick), trick));
    }
    return hand.removeCard(CardScore::getWorstCard(hand.getPlayableCards(trick), trick));
}
