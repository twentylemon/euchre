
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
#ifndef _DEBUG
    Random::initSeed();
#endif

    //HumanEuchreGame game;
    EuchreGame game(new AIPlayer(), new AIPlayer(), new AIPlayer(), new AIPlayer());
    //EuchreGame game(new RandomPlayer(), new RandomPlayer(), new RandomPlayer(), new RandomPlayer());
    game.setScore(9, 9);
   game.setPublicKnowledgeCallback([&](Card card, int playerIDX){
        ((AIPlayer*)game.getPlayer(EuchreGame::UP))->seenCard(card);
        ((AIPlayer*)game.getPlayer(EuchreGame::DOWN))->seenCard(card);
        ((AIPlayer*)game.getPlayer(EuchreGame::LEFT))->seenCard(card);
        ((AIPlayer*)game.getPlayer(EuchreGame::RIGHT))->seenCard(card);
    });
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
