

#include "AIPlayer.h"


/**
 * constructor
 */
AIPlayer::AIPlayer() : Player() {
}

/**
 * @param name the name of this player
 */
AIPlayer::AIPlayer(std::string name) : Player(name) {
}


/**
 * preps the ai player for a new hand.
 */
void AIPlayer::startNewHand() {
    knownCards.reset();
    Player::startNewHand();
}


/**
 * @param card the card to set as seen for this player
 * @param playerIDX the player that played the card
 */
void AIPlayer::publicKnowledge(const Card& card, int playerIDX) {
    knownCards.set(Card::HASH_IDX[card.hashCode()], true);
}


/**
 * @return the bitset of hands in this hand
 */
std::bitset<Card::NUM_CARDS> AIPlayer::getKnownCards() const {
    return knownCards;
}


/**
 * @return a string representation of this ai player
 */
std::string AIPlayer::toString() const {
    return Player::toString() + ", " + getKnownCards().to_string();
}


/**
 * @param top the card that would be ordered up
 * @param yourTeam true if you would be ordering up your partner
 * @return the action the player is taking, ordering up or passing
 * @todo this
 */
std::pair<int,bool> AIPlayer::orderUp(const Card& top, bool yourTeam) const {
    return Player::orderUp(top, yourTeam);
}


/**
 * @param top the card that would be picked up
 * @return the action the player is taking, picking up or turning down
 * @todo this
 */
std::pair<int,bool> AIPlayer::pickItUp(const Card& top) const {
    return Player::pickItUp(top);
}


/**
 * @param top the card that is coming into your hand, replacing another card
 * @todo this
 */
void AIPlayer::replaceCard(const Card& top) {
    Player::replaceCard(top);
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump or passing
 * @todo this
 */
std::pair<int,bool> AIPlayer::callTrump(int badSuit) const {
    return Player::callTrump(badSuit);
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump must occur
 * @todo this
 */
std::pair<int,bool> AIPlayer::stickTrump(int badSuit) const {
    return Player::stickTrump(badSuit);
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 * @todo this
 */
Card AIPlayer::playCard(const Trick& trick) {
    std::atomic<int> total = 0;
    int numCards = getNumCards();
#ifdef HAND_ALL_HANDS
    if (knownCards.count() >= 11) {
        concurrency::parallel_for_each(Hand::ALL_HANDS[numCards].begin(), Hand::ALL_HANDS[numCards].end(), [&](const Hand& left) {
            if (left.intersects(knownCards)) { return; }
            for (const Hand& across : Hand::ALL_HANDS[numCards]) {
                if (across.intersects(knownCards)|| across.intersects(left)) { continue; }
                for (const Hand& right : Hand::ALL_HANDS[numCards]) {
                    if (right.intersects(knownCards)|| right.intersects(left)|| right.intersects(across)) { continue; }
                    total++;
                }
            }
        });
    }
#endif
    std::cout << "know about " << knownCards.count() << " cards, counted " << total << " hands[size=" << numCards << "]" << std::endl;
    return hand.removeCard(0);
}
/**
 know about 6  cards, counted 617,512,896 hands[size=5]
 know about 7  cards, counted 102,918,816 hands[size=5]
 know about 8  cards, counted  12,108,096 hands[size=5]
 know about 9  cards, counted     756,756 hands[size=5]
 
 know about 9  cards, counted  15,765,750 hands[size=4]
 know about 10 cards, counted   3,153,150 hands[size=4]
 know about 11 cards, counted     450,450 hands[size=4]
 know about 12 cards, counted      34,650 hands[size=4]
 
 know about 12 cards, counted     369,600 hands[size=3]
 know about 13 cards, counted      92,400 hands[size=3]
 know about 14 cards, counted      16,800 hands[size=3]
 know about 15 cards, counted       1,680 hands[size=3]
 
 know about 15 cards, counted       7,560 hands[size=2]
 know about 16 cards, counted       2,520 hands[size=2]
 know about 17 cards, counted         630 hands[size=2]
 know about 18 cards, counted          90 hands[size=2]
 
 know about 18 cards, counted         120 hands[size=1]
 know about 19 cards, counted          60 hands[size=1]
 know about 20 cards, counted          24 hands[size=1]
 know about 21 cards, counted           6 hands[size=1]
**/

