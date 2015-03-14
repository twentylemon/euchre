
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
int main(int argc, char** argv) {
#ifndef _DEBUG
    Random::initSeed();
#endif
    CardScore::init();
    const int NUM_GAMES = 255;

    /*
    EuchreGame game(new MarkovPlayer("up"), new MarkovPlayer("down"), new MarkovPlayer("left"), new MarkovPlayer("right"));
    game.setScore(9, 9);
    ULONGLONG start = GetTickCount64();
    game.play();
    game.displayStats();
    std::cout << "took " << GetTickCount64() - start << "ms" << std::endl;
    */
    DataGame game(new PartnerHLPlayer("up"), new PartnerHLPlayer("down"), new HighPlayer("left"), new HighPlayer("right"));
    ULONGLONG start = GetTickCount64();
    game.play(NUM_GAMES);
    game.displayStats();
    std::cout << "took " << GetTickCount64() - start << "ms" << std::endl;

#ifdef EVAL
    ULONGLONG start1 = GetTickCount64();
    std::array<std::array<int, 4>, Card::MAX_CARD> win = evaluateTricks();
    for (Card card : Card::ALL_CARDS) {
        std::cout << card.toString() << "\t";
        int total = 0;
        for (int w : win[card.hashCode()]) {
            std::cout << w << "\t";
            total += w;
        }
        std::cout << total << std::endl;
    }
    std::cout << "took " << GetTickCount64() - start1 << "ms" << std::endl;
#endif
    system("pause");
}
