
#include "main.h"

/**
 * Enumerates all possible tricks and stores the number of times each Card won
 * as played from each position (lead, second, third, fourth) in the database.
 *
 * on second though - this going so fast. no need to store it
 */
void evaluateTricks(){
    Trick trick(Card::Spades);
    Card cards[4];
    Deck deck;
    int numTricks = 0;
    std::array<int, Card::MAX_CARD> wins;
    std::fill(wins.begin(), wins.end(), 0);

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

                    wins[trick.getWinningCard().hashCode()]++;
                    numTricks++;

                    trick.removeLastCard();
                }

                trick.removeLastCard();
            }

            trick.removeLastCard();
        }

        trick.removeLastCard();
    }

    int highest = *std::max_element(wins.begin(), wins.end());

    for (int i = Card::MIN_CARD; i < Card::MAX_CARD; i++){
        std::cout << Card(i).toString() << "\t" << wins[i] << "\t" <<
            100*((float)wins[i] / (float)highest) << "%" << std::endl;
    }
}
