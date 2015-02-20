

#include "Player.h"

/**
 * base constructor
 */
Player::Player(){
    startNewHand();
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
 * @param suit the suit to maybe call
 * @param action 'y' for call, 'a' for alone, otherwise for pass
 * @return the action to take
 */
int Player::responseToAction(int suit, char action){
    if (action == 'y'){
        return suit;
    }
    else if (action == 'a'){
        return suit + Alone;
    }
    else {
        return Pass;
    }
}


/**
 * @param top the card that would be ordered up
 * @return the action the player is taking, ordering up or passing
 */
int Player::orderUp(Card top){
    char response = '\0';
    std::cout << "do you want to order up " << top.toString() << " ([y]es/[n]o/[a]lone)? ";
    do {
        std::cin >> response;
    } while (response != 'y' && response != 'a' && response != 'n');
    return responseToAction(top.getSuit(), response);
}


/**
 * @param top the card that would be picked up
 * @return the action the player is taking, picking up or turning down
 * @todo place card in hand, discard another card
 */
int Player::pickItUp(Card top){
    char response = '\0';
    std::cout << "do you want to pick up " << top.toString() << " ([y]es/[n]o/[a]lone)? ";
    do {
        std::cin >> response;
    } while (response != 'y' && response != 'a' && response != 'n');
    return responseToAction(top.getSuit(), response);
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump or passing
 * @todo this
 */
int Player::callTrump(int badSuit){
    return Pass;
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump must occur
 * @todo this
 */
int Player::stickTrump(int badSuit){
    return Pass;
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 * @todo this
 */
Card Player::playCard(Trick &trick){
    return hand.getCard(0);
}
