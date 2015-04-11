

#include "Hybrid2Player.h"


/**
 * default threshold value if none is provided
 */
const double Hybrid2Player::DEFAULT_THRESHOLD = 0.25;

/**
 * constructor
 */
Hybrid2Player::Hybrid2Player() : HybridPlayer(DEFAULT_THRESHOLD) {
}

/**
 * @param threshold the threshold for this player
 */
Hybrid2Player::Hybrid2Player(double threshold) : HybridPlayer(threshold) {
}

/**
 * @param name the name of this player
 */
Hybrid2Player::Hybrid2Player(std::string name) : HybridPlayer(name, DEFAULT_THRESHOLD) {
}

/**
 * @param name the name of this player
 * @param threshold the threshold for this player
 */
Hybrid2Player::Hybrid2Player(std::string name, double threshold) : HybridPlayer(name, threshold) {
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card Hybrid2Player::playCard(const Trick& trick) {
    double deckScore = CardScore::sumPartialWinRate(trick, ~getKnownCards());
    double handScore = CardScore::sumPartialWinRate(trick, hand.getBitset());
    if (handScore / deckScore >= getThreshold()) {
        return PartnerHLPlayer::playCard(trick);
    }
    return CardCountingPlayer::playCard(trick);
}
