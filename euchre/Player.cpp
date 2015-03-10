

#include "Player.h"

/**
 * base constructor
 */
Player::Player() {
    startNewHand();
    setName("default");
}

/**
 * @param name the name of this player
 */
Player::Player(std::string name) {
    startNewHand();
    setName(name);
}


/**
 * @return the name of this player
 */
std::string Player::getName() const {
    return name;
}

/**
 * @param name the name of this player
 */
void Player::setName(std::string name) {
    this->name = name;
}


/**
 * @return this players hand
 */
const Hand& Player::getHand() const {
    return hand;
}

/**
 * @return the number of cards this player has in their hand
 */
int Player::getNumCards() const {
    return hand.getNumCards();
}


/**
 * @return the position of this player in the game, ie one of EuchreGame::UP/DOWN/LEFT/RIGHT
 */
int Player::getPosition() const {
    return position;
}

/**
 * @param position the position to give this player, ie one of EuchreGame::UP/DOWN/LEFT/RIGHT
 */
void Player::setPosition(int position) {
    this->position = position;
}


/**
 * preps this player for a new hand
 */
void Player::startNewHand() {
    hand.clear();
}


/**
 * @param card the card to add to this players hand
 */
void Player::addCard(const Card& card) {
    hand.addCard(card);
}


/**
 * @return string representation of this player
 */
std::string Player::toString() const {
    return name + ": " + hand.toString();
}


/**
 * @param top the card that would be ordered up
 * @param yourTeam true if you would be ordering up your partner
 * @return the action the player is taking, ordering up or passing
 */
std::pair<int,bool> Player::orderUp(const Card& top, bool yourTeam) const {
    return std::make_pair(Pass, false);
}


/**
 * @param top the card that would be picked up
 * @return the action the player is taking, picking up or turning down
 */
std::pair<int,bool> Player::pickItUp(const Card& top) const {
    return std::make_pair(Pass, false);
}


/**
 * @param top the card that is coming into your hand, replacing another card
 * @todo this
 */
void Player::replaceCard(const Card& top) {
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump or passing
 * @todo this
 */
std::pair<int,bool> Player::callTrump(int badSuit) const {
    return std::make_pair(Pass, false);
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump must occur
 */
std::pair<int,bool> Player::stickTrump(int badSuit) const {
    return std::make_pair(Card::Spades, false);
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 * @todo this
 */
Card Player::playCard(const Trick& trick) {
    return hand.removeCardIDX(0);
}
