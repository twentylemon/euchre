
#include "RandomPlayer.h"

/**
 * constructor, just call parent
 */
RandomPlayer::RandomPlayer() : Player() {
}

/**
 * @param name the name of this player
 */
RandomPlayer::RandomPlayer(std::string name) : Player(name) {
}


/**
 * @param top the card that would be ordered up
 * @param yourTeam true if you would be ordering up your partner
 * @return the action the player is taking, ordering up or passing
 */
/*
std::pair<int,bool> RandomPlayer::orderUp(const Card& top, bool yourTeam) const {
    if (Random::nextBool()) {
        return std::make_pair(top.getSuit(), Random::nextBool());
    }
    return std::make_pair(Pass, false);
}
*/

/**
 * @param top the card that would be picked up
 * @return the action the player is taking, picking up or turning down
 */
/*
std::pair<int,bool> RandomPlayer::pickItUp(const Card& top) const {
    return orderUp(top, false);
}
*/

/**
 * @param top the card that is coming into your hand, replacing another card
 */
/*
void RandomPlayer::replaceCard(const Card& top) {
    hand.removeCard(Random::nextInt(hand.getNumCards()));
    hand.addCard(top);
}
*/

/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump or passing
 */
/*
std::pair<int,bool> RandomPlayer::callTrump(int badSuit) const {
    int suit = Random::nextInt(Card::SUITS.size());
    if (Card::SUITS[suit] == badSuit) {
        return std::make_pair(Pass, false);
    }
    return std::make_pair(suit, Random::nextBool());
}
*/

/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump must occur
 */
/*
std::pair<int,bool> RandomPlayer::stickTrump(int badSuit) const {
    std::pair<int,bool> ret;
    while (true) { //meh
        ret = callTrump(badSuit);
        if (ret.first != Pass) {
            return ret;
        }
    }
    return std::make_pair(Pass, false);
}
*/

/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card RandomPlayer::playCard(const Trick& trick) {
    std::vector<int> cards = getHand().getPlayableCards(trick);
    return hand.removeCard(cards[Random::nextInt(cards.size())]);
    /*
    std::vector<int> legalCards = getHand().getLegalCards(trick);
    if (legalCards.empty()) {
        return hand.removeCard(Random::nextInt(hand.getNumCards()));
    }
    return hand.removeCard(Card(legalCards[Random::nextInt(legalCards.size())]));
    */
}
