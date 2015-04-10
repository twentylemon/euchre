

#include "HighPlayer.h"


/**
 * basic constructor
 */
HighPlayer::HighPlayer() : Player() {
}

/**
 * @param name the name of this player
 */
HighPlayer::HighPlayer(std::string name) : Player(name) {
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card HighPlayer::playCard(const Trick& trick) {
    std::vector<int> cards = getHand().getPlayableCards(trick);
    std::sort(cards.begin(), cards.end(), CardScore::hashComparator[trick.getTrump()]);
    return hand.removeCard(cards.back());
}
