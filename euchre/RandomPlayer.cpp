
#include "RandomPlayer.h"

/**
 * constructor, just call parent
 */
RandomPlayer::RandomPlayer() : Player(){
}

/**
 * @param name the name of this player
 */
RandomPlayer::RandomPlayer(std::string name) : Player(name){
}


/**
 * @param top the card that would be ordered up
 * @param yourTeam true if you would be ordering up your partner
 * @return the action the player is taking, ordering up or passing
 */
std::pair<int,bool> RandomPlayer::orderUp(Card top, bool yourTeam){
    if (Random::nextBool()){
        return std::make_pair(top.getSuit(), Random::nextBool());
    }
    return std::make_pair(Pass, false);
}


/**
 * @param top the card that would be picked up
 * @return the action the player is taking, picking up or turning down
 */
std::pair<int,bool> RandomPlayer::pickItUp(Card top){
    return orderUp(top, false);
}


/**
 * @param top the card that is coming into your hand, replacing another card
 */
void RandomPlayer::replaceCard(Card top){
    hand.removeCard(Random::nextInt(hand.getNumCards()));
    hand.addCard(top);
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump or passing
 */
std::pair<int,bool> RandomPlayer::callTrump(int badSuit){
    int suit = Random::nextInt(Card::SUITS.size());
    if (Card::SUITS[suit] == badSuit){
        return std::make_pair(Pass, false);
    }
    return std::make_pair(suit, Random::nextBool());
}


/**
 * @param badSuit the suit that was turned down
 * @return the action the player is taking, calling trump must occur
 */
std::pair<int,bool> RandomPlayer::stickTrump(int badSuit){
    std::pair<int,bool> ret;
    while (true){ //meh
        ret = callTrump(badSuit);
        if (ret.first != Pass){
            return ret;
        }
    }
    return std::make_pair(Pass, false);
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 * @todo this
 */
Card RandomPlayer::playCard(Trick &trick){
    return Card(hand.removeCard(0));
}
