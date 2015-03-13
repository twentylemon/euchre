

#include "HighLowPlayer.h"


/**
 * basic constructor
 */
HighLowPlayer::HighLowPlayer() : Player() {
}

/**
 * @param name the name of this player
 */
HighLowPlayer::HighLowPlayer(std::string name) : Player(name) {
}

/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card HighLowPlayer::playCard(const Trick& trick) {
    if (trick.empty()) {
        return hand.removeCard(CardScore::getBestCard(hand.getCards(), trick)); //to lead, play the highest card
    }
    std::vector<int> cards = hand.getPlayableCards(trick);
    int winning = trick.getWinningCard().hashCode();
    if (CardScore::hashComparator[trick.getTrump()](winning, CardScore::getBestCard(cards, trick))) {
        return hand.removeCard(CardScore::getBestCard(cards, trick));   //winning < our best legal card
    }
    return hand.removeCard(CardScore::getWorstCard(cards, trick));      //otherwise, dump the worst legal card
}
