#include "Trick.h"

/**
 * initializes this trick to be empty.
 * @param trump the suit that is trump
 */
Trick::Trick(){
    clear();
    initRankings();
}

/**
 * initializes this trick to be empty.
 * @param trump the suit that is trump
 */
Trick::Trick(int trump){
    clear();
    initRankings();
    setTrump(trump);
}

/**
 * clears the list of cards in this trick
 */
void Trick::clear(){
    cards.clear();
    winner = 0;
}

/**
 * @return the suit that is trump
 */
int Trick::getTrump(){
    return trump;
}

/**
 * @param trump the new suit to set to be trump
 */
void Trick::setTrump(int trump){
    this->trump = trump;
}

/**
 * @return the cards currently in the trick
 */
std::vector<Card> Trick::getCards(){
    return cards;
}

/**
 * @param card the card to add to this trick
 */
void Trick::addCard(Card card){
    cards.push_back(card);
    if (cards.size() > 1){
        int curBest = rankings[trump][getWinningCard().hashCode()];
        int thisScore = rankings[trump][card.hashCode()];

        if (thisScore > curBest){ //their card is possibly better
            if (thisScore >= MIN_TRUMP_SCORE || //they played a better trump card, or trumped the trick
                card.getSuit() == cards[0].getSuit()){ //they followed lead suit and played a better card
                    winner = cards.size() - 1;
            }
        }
    }
}

/**
 * note: this function assumes that all the cards are in the trick
 * @return the id of the card that is winning this trick
 */
int Trick::getWinner(){
    return winner;
}

/**
 * @return the Card that is winning this trick
 */
Card Trick::getWinningCard(){
    return cards[winner];
}

/**
 * @return  string version of this trick
 */
std::string Trick::toString(){
    std::string str = "T: " + Card::SUIT_SYMBOLS[trump] + ";";
    for (std::vector<Card>::iterator it = cards.begin(), end = cards.end(); it != end; it++){
        str += " " + it->toString();
    }
    return str + "  W: " + getWinningCard().toString();
}

/**
 * initializes all of the cards' rankings for each trump value
 */
void Trick::initRankings(){
    const int MIN_HASH = 36;    //minimum Card.hashCode() value
    const int MAX_HASH = 59;    //maximum Card.hashCode() value

    const int RIGHT_BOWER = 12;
    const int LEFT_BOWER  = 11;
    const int TRUMP[] = { 6, 7, RIGHT_BOWER, 8, 9, 10 };    //6=trump 9, ... , 10 = trump ace
    const int OFF[]   = { 0, 1, 2, 3, 4, 5 };   //0=off 9 etc

    for (int trump = Card::FIRST_SUIT; trump <= Card::LAST_SUIT; trump++){
        for (int hash = MIN_HASH; hash <= MAX_HASH; hash++){
            Card card(hash);
            if (card.getRank() == Card::Rank::Jack && card.getSuit() == (trump+2)%4){
                rankings[trump][hash] = LEFT_BOWER;
            }
            else if (card.getSuit() == trump){
                rankings[trump][hash] = TRUMP[card.getRank() - 9];  //offset by 9
            }
            else {  //not trump
                rankings[trump][hash] = OFF[card.getRank() - 9];
            }
        }
    }
}
