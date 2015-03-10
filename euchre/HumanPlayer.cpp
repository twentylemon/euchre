

#include "HumanPlayer.h"

/**
 * constructor
 */
HumanPlayer::HumanPlayer() : Player() {
}

/**
 * @param name the name of this player
 */
HumanPlayer::HumanPlayer(std::string name) : Player(name) {
}


/**
 * @param top the card that would be ordered up
 * @param yourTeam true if you would be ordering up your partner
 * @return the action the player is taking, ordering up or passing
 */
std::pair<int,bool> HumanPlayer::orderUp(const Card& top, bool yourTeam) const {
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
std::pair<int,bool> HumanPlayer::pickItUp(const Card& top) const {
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
void HumanPlayer::replaceCard(const Card& top) {
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump or passing
 * @todo this
 */
std::pair<int,bool> HumanPlayer::callTrump(int badSuit) const {
    return Player::callTrump(badSuit);
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump must occur
 * @todo this
 */
std::pair<int,bool> HumanPlayer::stickTrump(int badSuit) const {
    return Player::stickTrump(badSuit);
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 * @todo this
 */
Card HumanPlayer::playCard(const Trick& trick) {
    return Player::playCard(trick);
}
