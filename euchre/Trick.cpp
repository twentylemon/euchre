#include "Trick.h"

/**
 * initializes this trick to be empty.
 * @param trump the suit that is trump
 */
Trick::Trick(){
    clear();
}


/**
 * initializes this trick to be empty.
 * @param trump the suit that is trump
 */
Trick::Trick(int trump){
    clear();
    setTrump(trump);
}


/**
 * clears the list of cards in this trick
 */
void Trick::clear(){
    trump = -1;
    cards.clear();
    winner.fill(0);
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
    hash = trump;
}


/**
 * @return the suit of the card that led, or -1 if no cards have been played so far
 */
int Trick::getLeadSuit(){
    if (cards.empty()){
        return -1;
    }
    return cards[0].getSuit();
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
    if (!cards.empty()){
        int curBest = CardScore::get(trump, getWinningCard().hashCode());
        int thisScore = CardScore::get(trump, card.hashCode());
        
        winner[cards.size()] = winner[cards.size() - 1];    //winner is previous winning card unless
        if (thisScore > curBest){                           //this card is possibly better
            if (thisScore >= CardScore::MIN_TRUMP_SCORE ||  //they played a better trump card, or trumped the trick
                card.getSuit() == cards[0].getSuit()){      //they followed lead suit and played a better card
                    winner[cards.size()] = cards.size();    //this card is winning the trick
            }
        }
    }
    cards.push_back(card);
    hash = (hash << Card::HASHBITS) | card.hashCode();  //this hash states order is important
}


/**
 * remvoes the last played card from this trick
 */
void Trick::removeLastCard(){
    cards.pop_back();
    hash = hash >> Card::HASHBITS;
    //winner update will be handled by addCard(Card)
}


/**
 * @return the id of the card that is winning this trick
 */
int Trick::getWinner(){
    return winner[cards.size() - 1];
}


/**
 * @return the Card that is winning this trick
 */
Card Trick::getWinningCard(){
    return cards[getWinner()];
}


/**
 * @return a hash code for this trick
 */
int Trick::hashCode(){
    return hash;
}


/**
 * @return a hashcode for the given cards if they were a trick
 */
int Trick::hashCode(int card1, int card2, int card3, int card4){
    return (card1 << 4*Card::HASHBITS) | (card2 << 3*Card::HASHBITS) | (card3 << 2*Card::HASHBITS) | card4;
}

/**
 * @return a hashcode for the given cards if they were a trick
 */
int Trick::hashCode(Card card1, Card card2, Card card3, Card card4){
    return hashCode(card1.hashCode(), card2.hashCode(), card3.hashCode(), card4.hashCode());
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
