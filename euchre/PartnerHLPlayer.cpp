

#include "PartnerHLPlayer.h"


/**
 * basic constructor
 */
PartnerHLPlayer::PartnerHLPlayer() : HighLowPlayer() {
}

/**
 * @param name the name of this player
 */
PartnerHLPlayer::PartnerHLPlayer(std::string name) : HighLowPlayer(name) {
}

/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card PartnerHLPlayer::playCard(const Trick& trick) {
    if (trick.getNumCards() <= 1) {
        return HighLowPlayer::playCard(trick);  //partner hasn't played, no strategy change
    }
    std::vector<int> cards = hand.getPlayableCards(trick);
    int partnerCard = trick.getCard(trick.getNumCards() - 2).hashCode();
    int winning = trick.getWinningCard().hashCode();
    if (partnerCard == winning) {
        return hand.removeCard(CardScore::getWorstCard(cards, trick));  //partner is winning, play bad
    }
    return HighLowPlayer::playCard(trick);  //otherwise, play to win if possible
}
