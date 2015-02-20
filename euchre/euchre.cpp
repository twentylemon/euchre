
#include "euchre.h"

/**
 * only consider games after two tricks have been played?
 *  after only one trick may also be tractable? maybe not after considering playing cards from the hand?
 */


/**
 * entry point to the application
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 */
int main(int argc, char** argv){
    std::srand((unsigned)std::time(nullptr));
    
    Deck deck;
    Hand hand;
    AIPlayer ai;

    std::cout << deck.toString() << std::endl;
    deck.shuffle();
    
    ai.addCard(deck.pop());
    ai.addCard(deck.pop());
    ai.addCard(deck.pop());
    ai.addCard(deck.pop());
    ai.addCard(deck.pop());
    
    ai.seenCard(deck.pop());
    ai.seenCard(deck.pop());
    ai.seenCard(deck.pop());
    ai.seenCard(deck.pop());
    ai.seenCard(deck.pop());
    
    std::cout << deck.toString() << std::endl;
    std::cout << ai.toString() << std::endl;

    hand = ai.getHand();
    std::cout << hand.toString() << std::endl;
    hand.removeCard(0);
    std::cout << hand.toString() << std::endl;
    hand.removeCard(1);
    std::cout << hand.toString() << std::endl;

    

#ifdef EVAL
    ULONGLONG start1 = GetTickCount64();
    eval(1);
    std::cout << "took " << GetTickCount64() - start1 << "ms" << std::endl;
#endif
    /*
    ULONGLONG start = GetTickCount64();
    std::array<std::array<int, 4>, Card::MAX_CARD> wins = evaluateTricks();
    std::cout << "took " << GetTickCount64() - start << "ms" << std::endl;
    for (Card card : Card::ALL_CARDS){
        std::cout << card.toString() << "\t";
        int total = 0;
        for (int w : wins[card.hashCode()]){
            std::cout << w << "\t";
            total += w;
        }
        printf("\t%s: %5d\n", card.toString().c_str(), total);
    }
    */
    system("pause");
}
