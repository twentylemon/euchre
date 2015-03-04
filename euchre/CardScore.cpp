#include "CardScore.h"

/**
 * prevents recalculating the scores
 */
bool CardScore::calculated = false;

/**
 * the scores of each card by trump suit
 */
int CardScore::rankings[Card::NUM_SUITS][Card::MAX_CARD];

/**
 * scores of trump cards and non trump cards, left bower is separate from trump
 */
const int CardScore::TRUMP_SCORE[] = { 6, 7, CardScore::RIGHT_BOWER, 8, 9, 10 };   //6=trump 9, ... , 10=trump ace
const int CardScore::OFF_SCORE[]   = { 0, 1, 2, 3, 4, 5 };     //0=off 9 etc

/**
 * the scores of each card by trump suit, post calculation
 */
const int CardScore::RANKINGS[Card::NUM_SUITS][Card::MAX_CARD] = {
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        6,0,0,0, 7,1,1,1, 12,2,2,11, 8,3,3,3, 9,4,4,4, 10,5,5,5 },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,6,0,0, 1,7,1,1, 2,12,11,2, 3,8,3,3, 4,9,4,4, 5,10,5,5 },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,6,0, 1,1,7,1, 2,11,12,2, 3,3,8,3, 4,4,9,4, 5,5,10,5 },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,6, 1,1,1,7, 11,2,2,12, 3,3,3,8, 4,4,4,9, 5,5,5,10 }
};

/**
 * initializes the rankings
 */
CardScore::CardScore() {
    initRankings();
}

/**
 * @param trump the suit that is trump
 * @param card the card to get the score of
 * @return the score of the card given the trump suit
 */
int CardScore::get(int trump, const Card& card) {
    return get(trump, card.hashCode());
}

/**
 * @param trump the suit that is trump
 * @param hash the has of the card to get the score of
 * @return the score of the card given the trump suit
 */
int CardScore::get(int trump, int hash) {
    return RANKINGS[trump][hash];
}


/**
 * initializes all of the cards' rankings for each trump value
 */
void CardScore::initRankings() {
    if (calculated) {
        return;
    }
    
    for (int trump : Card::SUITS) {
        for (const Card& card : Card::ALL_CARDS) {
            if (card.getRank() == Card::Jack && card.getSuit() == Card::otherSuit(trump)) {
                rankings[trump][card.hashCode()] = LEFT_BOWER;
            }
            else if (card.getSuit() == trump) {
                rankings[trump][card.hashCode()] = TRUMP_SCORE[card.getRank() - 9]; //offset by 9
            }
            else {  //not trump
                rankings[trump][card.hashCode()] = OFF_SCORE[card.getRank() - 9];
            }
        }
    }
    calculated = true;
}

/**
 * displays the calculated scores
 */
void CardScore::display() {
    initRankings();
    for (int suit : Card::SUITS) {
        std::cout << Card::SUIT_SYMBOLS[suit] << std::endl;
        for (Card card : Card::ALL_CARDS) {
            std::cout << card.toString() << "\t" << rankings[suit][card.hashCode()] << std::endl;
        }
        system("pause");
    }
}
