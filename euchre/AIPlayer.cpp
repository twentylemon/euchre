

#include "AIPlayer.h"


/**
 * constructor
 */
AIPlayer::AIPlayer() : Player(){
}

/**
 * @param name the name of this player
 */
AIPlayer::AIPlayer(std::string name) : Player(name){
}


/**
 * preps the ai player for a new hand.
 */
void AIPlayer::startNewHand(){
    knownCards.reset();
    Player::startNewHand();
}


/**
 * @param card the card to add to this players hand
 */
void AIPlayer::addCard(Card card){
    seenCard(card);
    Player::addCard(card);
}


/**
 * @param card the card to set as seen for this player
 */
void AIPlayer::seenCard(Card card){
    knownCards.set(Card::HASH_IDX[card.hashCode()], true);
}


/**
 * @return the bitset of hands in this hand
 */
std::bitset<Card::NUM_CARDS> AIPlayer::getKnownCards(){
    return knownCards;
}


/**
 * @return a string representation of this ai player
 */
std::string AIPlayer::toString(){
    return Player::toString() + ", " + getKnownCards().to_string();
}



/**
 * @param top the card that would be ordered up
 * @param yourTeam true if you would be ordering up your partner
 * @return the action the player is taking, ordering up or passing
 * @todo this
 */
std::pair<int,bool> AIPlayer::orderUp(Card top, bool yourTeam){
    return std::make_pair(Pass, false);
}


/**
 * @param top the card that would be picked up
 * @return the action the player is taking, picking up or turning down
 * @todo this
 */
std::pair<int,bool> AIPlayer::pickItUp(Card top){
    return std::make_pair(Pass, false);
}


/**
 * @param top the card that is coming into your hand, replacing another card
 * @todo this
 */
void AIPlayer::replaceCard(Card top){
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump or passing
 * @todo this
 */
std::pair<int,bool> AIPlayer::callTrump(int badSuit){
    return std::make_pair(Pass, false);
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump must occur
 * @todo this
 */
std::pair<int,bool> AIPlayer::stickTrump(int badSuit){
    return std::make_pair(Card::Spades, false);
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 * @todo this
 */
Card AIPlayer::playCard(Trick &trick){
    std::atomic<int> total = 0;
    int numCards = getNumCards();
    if (knownCards.count() >= 13){
        concurrency::parallel_for_each(Hand::ALL_HANDS[numCards].begin(), Hand::ALL_HANDS[numCards].end(), [&](Hand left){
            if (left.intersects(knownCards)){ return; }
            for (Hand across : Hand::ALL_HANDS[numCards]){
                if (across.intersects(knownCards)|| across.intersects(left)){ continue; }
                for (Hand right : Hand::ALL_HANDS[numCards]){
                    if (right.intersects(knownCards)|| right.intersects(left)|| right.intersects(across)){ continue; }
                    total++;
                }
            }
        });
    }
    std::cout << "know about " << knownCards.count() << " cards, counted " << total << " hands[size=" << numCards << "]" << std::endl;
    return Card(hand.removeCard(0));
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

