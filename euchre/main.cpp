#include "main.h"

/**
 * entry point to the application
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 */
int main(int argc, char** argv){
    Trick trick(Card::Spades);
    
    /*trick.addCard(Card(9, Card::Clubs));
    std::cout << trick.toString()<< std::endl;

    trick.addCard(Card(Card::King, Card::Clubs));
    std::cout << trick.toString()<< std::endl;

    trick.addCard(Card(Card::Jack, Card::Clubs));
    std::cout << trick.toString()<< std::endl;

    trick.addCard(Card(Card::Jack, Card::Spades));
    std::cout << trick.toString()<< std::endl;

    trick.removeLastCard();
    std::cout << trick.toString()<< std::endl;

    trick.addCard(Card(Card::King, Card::Hearts));
    std::cout << trick.toString()<< std::endl;*/

    evaluateTricks();

    system("pause");
}
