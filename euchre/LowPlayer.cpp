

#include "LowPlayer.h"


/**
 * basic constructor
 */
LowPlayer::LowPlayer() : Player() {
}

/**
 * @param name the name of this player
 */
LowPlayer::LowPlayer(std::string name) : Player(name) {
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card LowPlayer::playCard(const Trick& trick) {
    std::vector<int> cards = getHand().getPlayableCards(trick);
    std::sort(cards.begin(), cards.end(), CardScore::hashComparator[trick.getTrump()]);
    return hand.removeCard(cards.front());
}
