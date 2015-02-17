#include "main.h"

/**
 * entry point to the application
 * @param argc the number of command line arguments
 * @param argv the command line arguments
 */
int main(int argc, char** argv){    
    ULONGLONG start = GetTickCount64();

    std::array<std::array<int, Card::NUM_SUITS>, Card::MAX_CARD> wins = evaluateTricks();

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

    system("pause");
}
