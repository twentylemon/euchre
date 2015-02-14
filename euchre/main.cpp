#include "main.h"

/**
 * entry point to the application
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 */
int main(int argc, char** argv){
    Trick trick(Card::Spades);
    Card card1(9, Card::Clubs);
    Card card2(10, Card::Clubs);
    Card card3(Card::King, Card::Hearts);
    Card card4(Card::King, Card::Clubs);
    
    trick.addCard(card1);
    std::cout << trick.toString()<< std::endl;
    trick.addCard(card2);
    std::cout << trick.toString()<< std::endl;
    trick.addCard(card3);
    std::cout << trick.toString()<< std::endl;
    trick.addCard(card4);
    std::cout << trick.toString()<< std::endl;

    system("pause");
}
