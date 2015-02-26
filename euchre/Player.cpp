

#include "Player.h"

/**
 * base constructor
 */
Player::Player(){
    startNewHand();
    setName("default");
}

/**
 * @param name the name of this player
 */
Player::Player(std::string name){
    startNewHand();
    setName(name);
}


/**
 * @return the name of this player
 */
std::string Player::getName(){
    return name;
}

/**
 * @param name the name of this player
 */
void Player::setName(std::string name){
    this->name = name;
}


/**
 * preps this player for a new hand
 */
void Player::startNewHand(){
    hand.clear();
}


/**
 * @param card the card to add to this players hand
 */
void Player::addCard(Card card){
    hand.addCard(card);
}


/**
 * @return this players hand
 */
Hand Player::getHand(){
    return hand;
}


/**
 * @return string representation of this player
 */
std::string Player::toString(){
    return hand.toString();
}


/**
 * @param top the card that would be ordered up
 * @param yourTeam true if you would be ordering up your partner
 * @return the action the player is taking, ordering up or passing
 */
std::pair<int,bool> Player::orderUp(Card top, bool yourTeam){
    char response = '\0';
    std::cout << "do you want to order up " << top.toString() << " ([y]es/[n]o/[a]lone)? ";
    do {
        std::cin >> response;
    } while (response != 'y' && response != 'a' && response != 'n');
    return std::make_pair(response == 'n' ? Pass : top.getSuit(), response == 'a');
}


/**
 * @param top the card that would be picked up
 * @return the action the player is taking, picking up or turning down
 */
std::pair<int,bool> Player::pickItUp(Card top){
    char response = '\0';
    std::cout << "do you want to pick up " << top.toString() << " ([y]es/[n]o/[a]lone)? ";
    do {
        std::cin >> response;
    } while (response != 'y' && response != 'a' && response != 'n');
    return std::make_pair(response == 'n' ? Pass : top.getSuit(), response == 'a');
}


/**
 * @param top the card that is coming into your hand, replacing another card
 * @todo this
 */
void Player::replaceCard(Card top){
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump or passing
 * @todo this
 */
std::pair<int,bool> Player::callTrump(int badSuit){
    return std::make_pair(Pass, false);
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump must occur
 * @todo this
 */
std::pair<int,bool> Player::stickTrump(int badSuit){
    return std::make_pair(Card::Spades, false);
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 * @todo this
 */
Card Player::playCard(Trick &trick){
    return Card(hand.removeCard(0));
}
