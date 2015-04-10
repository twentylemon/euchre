#include "DataGame.h"

DataGame::DataGame() : EuchreGame() {
}

/**
 * @param up the up player
 * @param down the down player
 * @param left the left player
 * @param right the right player
 */
DataGame::DataGame(Player* up, Player* down, Player* left, Player* right) : EuchreGame(up, down, left, right) {
}

/**
 * sets default values for team names etc
 */
void DataGame::init() {
    EuchreGame::init();
    setDisplay(false);
}


#ifndef MONTE_CARLO
/**
 * skips displaying the game
 */
void DataGame::draw() const {
}
#endif

/**
 * @param numGames the number of games to play
 */
void DataGame::play(int numGames) {
    for (int i = 1; i <= numGames; i++){
        startNewGame();
        EuchreGame::play();
    }
}
