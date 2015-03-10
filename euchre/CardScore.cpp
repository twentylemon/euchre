#include "CardScore.h"

/**
 * @return the comparators given the hashcode of the cards
 */
std::array<std::function<bool(int, int)>, Card::NUM_SUITS> mapHash(){
    std::array<std::function<bool(int, int)>, Card::NUM_SUITS> comp;
    std::transform(Card::SUITS.begin(), Card::SUITS.end(), comp.begin(), [](int suit){
        return std::function<bool(int, int)>(
            [suit](int card1, int card2){ return CardScore::get(suit, card1) < CardScore::get(suit, card2); }
        );
    });
    return comp;
}

/**
 * @return the comparators given the cards
 */
std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> mapCard(){
    std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> comp;
    std::transform(Card::SUITS.begin(), Card::SUITS.end(), comp.begin(), [](int suit){
        return std::function<bool(const Card&, const Card&)>(
            [suit](const Card& card1, const Card& card2){ return CardScore::get(suit, card1) < CardScore::get(suit, card2); }
        );
    });
    return comp;
}

/**
 * prevents recalculating the scores
 */
bool CardScore::calculated = false;

/**
 * the scores of each card by trump suit
 */
std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> CardScore::rankings;

/**
 * scores of trump cards and non trump cards, left bower is separate from trump
 */
const std::array<int, Card::NUM_RANKS> CardScore::TRUMP_SCORE = {{ 6, 7, CardScore::RIGHT_BOWER, 8, 9, 10 }};   //6=trump 9, ... , 10=trump ace
const std::array<int, Card::NUM_RANKS> CardScore::OFF_SCORE   = {{ 0, 1, 2, 3, 4, 5 }};     //0=off 9 etc

/**
 * comparators for sortings cards according to their score
 */
const std::array<std::function<bool(int, int)>, Card::NUM_SUITS> CardScore::hashComparator = mapHash();
const std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> CardScore::cardComparator = mapCard();

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
    return rankings[trump][hash];
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
