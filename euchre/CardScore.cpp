#include "CardScore.h"

/**
 * prevents recalculating the scores
 */
bool CardScore::calculated = false;

/**
 * the scores of each card by trump suit
 */
int CardScore::rankings[4][60];

/**
 * scores of trump cards and non trump cards, left bower is separate from trump
 */
const int CardScore::TRUMP_SCORE[] = { 6, 7, CardScore::RIGHT_BOWER, 8, 9, 10 };   //6=trump 9, ... , 10=trump ace
const int CardScore::OFF_SCORE[]   = { 0, 1, 2, 3, 4, 5 };     //0=off 9 etc

/**
 * initializes the rankings
 */
CardScore::CardScore(){
    initRankings();
}

/**
 * @param trump the suit that is trump
 * @param card the card to get the score of
 * @return the score of the card given the trump suit
 */
int CardScore::get(int trump, Card card){
    return get(trump, card.hashCode());
}

/**
 * @param trump the suit that is trump
 * @param hash the has of the card to get the score of
 * @return the score of the card given the trump suit
 */
int CardScore::get(int trump, int hash){
    return rankings[trump][hash];
}


/**
 * initializes all of the cards' rankings for each trump value
 */
void CardScore::initRankings(){
    if (calculated){
        return;
    }
    
    for (int trump : Card::SUITS){
        for (Card card : Card::ALL_CARDS){
            if (card.getRank() == Card::Rank::Jack && card.getSuit() == Card::otherSuit(trump)){
                rankings[trump][card.hashCode()] = LEFT_BOWER;
            }
            else if (card.getSuit() == trump){
                rankings[trump][card.hashCode()] = TRUMP_SCORE[card.getRank() - 9]; //offset by 9
            }
            else {  //not trump
                rankings[trump][card.hashCode()] = OFF_SCORE[card.getRank() - 9];
            }
        }
    }
    calculated = true;
}
