

#include "HighLowPlayer.h"


/**
 * basic constructor
 */
HighLowPlayer::HighLowPlayer() : Player(){
}

/**
 * @param name the name of this player
 */
HighLowPlayer::HighLowPlayer(std::string name) : Player(name){
}

/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card HighLowPlayer::playCard(const Trick& trick) {
    std::vector<int> cards = hand.getCards();
    std::sort(cards.begin(), cards.end(), CardScore::hashComparator[trick.getTrump()]);
    if (trick.empty()){
        return hand.removeCard(cards.back());   //to lead, play the highest card
    }
    std::vector<int> legal = hand.getLegalCards(trick);
    if (legal.empty()){
        if (hand.getSuitCards(trick.getTrump()).empty()){
            return hand.removeCard(cards.front());  //no trump cards, dump
        }
        legal = cards;  //otherwise, check if our trump can beat the current winning card
    }
    std::sort(legal.begin(), legal.end(), CardScore::hashComparator[trick.getTrump()]);
    Card winning = trick.getWinningCard();
    if (CardScore::cardComparator[trick.getTrump()](winning, Card(legal.back()))){
        return hand.removeCard(legal.back());   //winning < our best legal card
    }
    return hand.removeCard(legal.front());      //otherwise, dump the worst legal card
}
