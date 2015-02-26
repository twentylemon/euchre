

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
    std::string str = "hand: " + getHand().toString() + "\n";
    str += "knownCards: " + getKnownCards().to_string() + "\n";
    return str;
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
    return Card(hand.removeCard(0));
}
