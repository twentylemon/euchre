

#include "HybridPlayer.h"


/**
 * default threshold value if none is provided
 */
const double HybridPlayer::DEFAULT_THRESHOLD = 0.25;

/**
 * constructor
 */
HybridPlayer::HybridPlayer() : threshold(DEFAULT_THRESHOLD), CardCountingPlayer() {
}

/**
 * @param threshold the threshold for this player
 */
HybridPlayer::HybridPlayer(double threshold) : threshold(threshold), CardCountingPlayer() {
}

/**
 * @param name the name of this player
 */
HybridPlayer::HybridPlayer(std::string name) : threshold(DEFAULT_THRESHOLD), CardCountingPlayer(name) {
}

/**
 * @param name the name of this player
 * @param threshold the threshold for this player
 */
HybridPlayer::HybridPlayer(std::string name, double threshold) : threshold(threshold), CardCountingPlayer(name) {
}


/**
 * @return the current threshold for this player
 */
double HybridPlayer::getThreshold() const {
    return threshold;
}

/**
 * @param threshold the new threshold for this player
 */
void HybridPlayer::setThreshold(double threshold) {
    this->threshold = threshold;
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card HybridPlayer::playCard(const Trick& trick) {
    double deckScore = CardScore::sumWinRate(trick.getTrump(), ~getKnownCards());
    double handScore = CardScore::sumWinRate(trick.getTrump(), hand.getBitset());
    if (handScore / deckScore >= getThreshold()) {
        return PartnerHLPlayer::playCard(trick);
    }
    return CardCountingPlayer::playCard(trick);
}
