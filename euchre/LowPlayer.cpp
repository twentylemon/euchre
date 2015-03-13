

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
    std::vector<int> legal = hand.getLegalCards(trick);
    if (legal.empty()) {
        legal = hand.getCards();    //if no legal cards, play the best card in the hand
    }
    std::sort(legal.begin(), legal.end(), CardScore::hashComparator[trick.getTrump()]);
    return hand.removeCard(legal.front());
}
