
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
    CardScore::initRankings();

    EuchreGame game(new HighLowPlayer("1"), new HighLowPlayer("2"), new HighLowPlayer("3"), new HighLowPlayer("4"));
    game.setScore(9, 9);
    /*game.setPublicKnowledgeCallback([&game](const Card& card, int playerIDX) {
        ((AIPlayer*)game.getPlayer(EuchreGame::UP))->seenCard(card);
        ((AIPlayer*)game.getPlayer(EuchreGame::DOWN))->seenCard(card);
        ((AIPlayer*)game.getPlayer(EuchreGame::LEFT))->seenCard(card);
        ((AIPlayer*)game.getPlayer(EuchreGame::RIGHT))->seenCard(card);
    });*/
    ULONGLONG start = GetTickCount64();
    game.play();
    std::cout << "took " << GetTickCount64() - start << "ms" << std::endl;

#ifdef EVAL
    ULONGLONG start1 = GetTickCount64();
    eval(1);
    std::cout << "took " << GetTickCount64() - start1 << "ms" << std::endl;
#endif
    system("pause");
}
