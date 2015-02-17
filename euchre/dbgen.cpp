
#include "main.h"

/**
 * Enumerates all possible tricks and stores the number of times each Card won
 * as played from each position (lead, second, third, fourth) in the database.
 *
 * on second though - this going so fast. no need to store it
 */
std::array<std::array<int, Card::NUM_SUITS>, Card::MAX_CARD> evaluateTricks(){
    Trick trick(Card::Spades);
    Card cards[4];
    Deck deck;
    int numTricks = 0;

    std::array<std::array<int, Card::NUM_SUITS>, Card::MAX_CARD> wins;
    for (int hash : Card::ALL_CARDS){
        std::fill(wins[hash].begin(), wins[hash].end(), 0);
    }

    for (int i = 0; i < Deck::SIZE; i++){
        cards[0] = deck.getCard(i);
        trick.addCard(cards[0]);

        for (int j = 0; j < Deck::SIZE; j++){
            if (i == j){ continue; }
            cards[1] = deck.getCard(j);
            trick.addCard(cards[1]);

            for (int k = 0; k < Deck::SIZE; k++){
                if (i == k || j == k){ continue; }
                cards[2] = deck.getCard(k);
                trick.addCard(cards[2]);

                for (int l = 0; l < Deck::SIZE; l++){
                    if (i == l || j == l || k == l){ continue; }
                    cards[3] = deck.getCard(l);
                    trick.addCard(cards[3]);

                    wins[trick.getWinningCard().hashCode()][trick.getWinner()]++;
                    numTricks++;

                    trick.removeLastCard();
                }
                trick.removeLastCard();
            }
            trick.removeLastCard();
        }
        trick.removeLastCard();
    }
    return wins;
}


/**
 * @return map of all tricks where spades is trump
 */
std::map<int, Trick> getAllTricks(){
    std::map<int, Trick> tricks;
    Card cards[4];
    Deck deck;

    for (int i = 0; i < Deck::SIZE; i++){
        cards[0] = deck.getCard(i);

        for (int j = 0; j < Deck::SIZE; j++){
            if (i == j){ continue; }
            cards[1] = deck.getCard(j);

            for (int k = 0; k < Deck::SIZE; k++){
                if (i == k || j == k){ continue; }
                cards[2] = deck.getCard(k);

                for (int l = 0; l < Deck::SIZE; l++){
                    if (i == l || j == l || k == l){ continue; }
                    cards[3] = deck.getCard(l);
                    Trick trick(Card::Spades);
                    trick.addCard(cards[0]);
                    trick.addCard(cards[1]);
                    trick.addCard(cards[2]);
                    trick.addCard(cards[3]);
                    tricks.insert(tricks.end(), std::pair<int, Trick>(trick.hashCode(), trick));
                }
            }
        }
    }
    return tricks;
}
