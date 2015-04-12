
#include "euchre.h"

#ifdef _DEBUG
    const int NUM_GAMES = 255;
#else
    const int NUM_GAMES = 10001;
#endif

DataGame game;


std::ofstream out("results.txt");
std::streambuf* coutbuf = std::cout.rdbuf();

/**
 * redirects std::cout to the file "results.txt"
 */
void cout2file() {
    std::cout.rdbuf(out.rdbuf());
}

/**
 * restores std::cout to the terminal
 */
void cout2term() {
    std::cout.rdbuf(coutbuf);
}


/**
 * plays out a DataGame
 * @param up the up player
 * @param down the down player
 * @param left the left player
 * @param right the right player
 */
void play(Player* up, Player* down, Player* left, Player* right) {
    game.init();
    game.setPlayers(up, down, left, right);
#ifdef MONTE_CARLO
    game.setDisplay(true);
    game.play(31);
#else
    game.play(NUM_GAMES);
#endif
    game.displayStats();
    std::cout << std::endl << std::endl;
    cout2file();
    game.displayStats();
    std::cout << std::endl << std::endl;
    cout2term();
    delete up, down, left, right;
}

/**
 * runs data collection
 */
void run() {
    play(new RandomPlayer("up"), new RandomPlayer("down"), new LowPlayer("left"), new LowPlayer("right"));
    play(new RandomPlayer("up"), new RandomPlayer("down"), new HighPlayer("left"), new HighPlayer("right"));
    play(new LowPlayer("up"), new LowPlayer("down"), new HighPlayer("left"), new HighPlayer("right"));
    
    play(new HighLowPlayer("up"), new HighLowPlayer("down"), new RandomPlayer("left"), new RandomPlayer("right"));
    play(new HighLowPlayer("up"), new HighLowPlayer("down"), new LowPlayer("left"), new LowPlayer("right"));
    play(new HighLowPlayer("up"), new HighLowPlayer("down"), new HighPlayer("left"), new HighPlayer("right"));
    
    play(new PartnerHLPlayer("up"), new PartnerHLPlayer("down"), new RandomPlayer("left"), new RandomPlayer("right"));
    play(new PartnerHLPlayer("up"), new PartnerHLPlayer("down"), new LowPlayer("left"), new LowPlayer("right"));
    play(new PartnerHLPlayer("up"), new PartnerHLPlayer("down"), new HighPlayer("left"), new HighPlayer("right"));
    play(new PartnerHLPlayer("up"), new PartnerHLPlayer("down"), new HighLowPlayer("left"), new HighLowPlayer("right"));
    
    play(new MarkovPlayer("up"), new MarkovPlayer("down"), new RandomPlayer("left"), new RandomPlayer("right"));
    play(new MarkovPlayer("up"), new MarkovPlayer("down"), new LowPlayer("left"), new LowPlayer("right"));
    play(new MarkovPlayer("up"), new MarkovPlayer("down"), new HighPlayer("left"), new HighPlayer("right"));
    play(new MarkovPlayer("up"), new MarkovPlayer("down"), new HighLowPlayer("left"), new HighLowPlayer("right"));
    play(new MarkovPlayer("up"), new MarkovPlayer("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));
    
    play(new Markov2Player("up"), new Markov2Player("down"), new RandomPlayer("left"), new RandomPlayer("right"));
    play(new Markov2Player("up"), new Markov2Player("down"), new LowPlayer("left"), new LowPlayer("right"));
    play(new Markov2Player("up"), new Markov2Player("down"), new HighPlayer("left"), new HighPlayer("right"));
    play(new Markov2Player("up"), new Markov2Player("down"), new HighLowPlayer("left"), new HighLowPlayer("right"));
    play(new Markov2Player("up"), new Markov2Player("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));
    play(new Markov2Player("up"), new Markov2Player("down"), new MarkovPlayer("left"), new MarkovPlayer("right"));
    
    play(new CardCountingPlayer("up"), new CardCountingPlayer("down"), new RandomPlayer("left"), new RandomPlayer("right"));
    play(new CardCountingPlayer("up"), new CardCountingPlayer("down"), new LowPlayer("left"), new LowPlayer("right"));
    play(new CardCountingPlayer("up"), new CardCountingPlayer("down"), new HighPlayer("left"), new HighPlayer("right"));
    play(new CardCountingPlayer("up"), new CardCountingPlayer("down"), new HighLowPlayer("left"), new HighLowPlayer("right"));
    play(new CardCountingPlayer("up"), new CardCountingPlayer("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));
    play(new CardCountingPlayer("up"), new CardCountingPlayer("down"), new MarkovPlayer("left"), new MarkovPlayer("right"));
    play(new CardCountingPlayer("up"), new CardCountingPlayer("down"), new Markov2Player("left"), new Markov2Player("right"));
    
    play(new HybridPlayer("up"), new HybridPlayer("down"), new RandomPlayer("left"), new RandomPlayer("right"));
    play(new HybridPlayer("up"), new HybridPlayer("down"), new LowPlayer("left"), new LowPlayer("right"));
    play(new HybridPlayer("up"), new HybridPlayer("down"), new HighPlayer("left"), new HighPlayer("right"));
    play(new HybridPlayer("up"), new HybridPlayer("down"), new HighLowPlayer("left"), new HighLowPlayer("right"));
    play(new HybridPlayer("up"), new HybridPlayer("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));
    play(new HybridPlayer("up"), new HybridPlayer("down"), new MarkovPlayer("left"), new MarkovPlayer("right"));
    play(new HybridPlayer("up"), new HybridPlayer("down"), new Markov2Player("left"), new Markov2Player("right"));
    play(new HybridPlayer("up"), new HybridPlayer("down"), new CardCountingPlayer("left"), new CardCountingPlayer("right"));
    
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new RandomPlayer("left"), new RandomPlayer("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new LowPlayer("left"), new LowPlayer("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new HighPlayer("left"), new HighPlayer("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new HighLowPlayer("left"), new HighLowPlayer("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new MarkovPlayer("left"), new MarkovPlayer("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new Markov2Player("left"), new Markov2Player("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new CardCountingPlayer("left"), new CardCountingPlayer("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new HybridPlayer("left"), new HybridPlayer("right"));
}


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

    /*EuchreGame game(new MonteCarloPlayer("up"), new MonteCarloPlayer("down"), new MonteCarloPlayer("left"), new MonteCarloPlayer("right"));
    game.setScore(9, 9);
    ULONGLONG start = GetTickCount64();
    game.play();
    game.displayStats();
    std::cout << "took " << GetTickCount64() - start << "ms" << std::endl;
    */
    play(new CardCountingPlayer("up"), new CardCountingPlayer("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));
    play(new HybridPlayer("up"), new HybridPlayer("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));
    play(new Hybrid2Player("up"), new Hybrid2Player("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));

#ifdef MONTE_CARLO
    play(new MonteCarloPlayer("up"), new MonteCarloPlayer("down"), new PartnerHLPlayer("left"), new PartnerHLPlayer("right"));
#else
    //run();
#endif

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
