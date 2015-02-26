

#include "EuchreGame.h"

/**
 * sets up a euchre game
 */
EuchreGame::EuchreGame(){
    players.fill(nullptr);
    startNewGame();
}


/**
 * @param up the up player
 * @param down the down player
 * @param left the left player
 * @param right the right player
 */
EuchreGame::EuchreGame(Player* up, Player* down, Player* left, Player* right){
    setPlayer(UP, up);
    setPlayer(DOWN, down);
    setPlayer(LEFT, left);
    setPlayer(RIGHT, right);
    startNewGame();
}


/**
 * resets the scores and randomly picks a new dealer
 */
void EuchreGame::startNewGame(){
    setDealer(DOWN);
    score.fill(0);
    setTeamName(UP_TEAM, "up/down team");
    setTeamName(LEFT_TEAM, "left/right team");
}


/**
 * @param playerIDX the index of the player, UP, DOWN, LEFT or RIGHT
 * @return the player
 */
Player* EuchreGame::getPlayer(int playerIDX){
    return players[playerIDX];
}

/**
 * @param playerIDX the player idx to set
 * @param player the player to set
 */
void EuchreGame::setPlayer(int playerIDX, Player* player){
    players[playerIDX] = player;
}

/**
 * @param team the team to set the name of
 * @param name the name to give the team
 */
void EuchreGame::setTeamName(int team, std::string name){
    teamNames[team] = name;
}

/**
 * @param team the team to get the name of
 * @return the name of the team
 */
std::string EuchreGame::getTeamName(int team){
    return teamNames[team];
}


/**
 * @return the player index of the dealer
 */
int EuchreGame::getDealer(){
    return dealer;
}

/**
 * @param playerIDX the player index to test if they are the dealer
 * @return true if the player is the index given
 */
bool EuchreGame::isDealer(int playerIDX){
    return getDealer() == playerIDX;
}

/**
 * @param dealer the player index to change the dealer to
 */
void EuchreGame::setDealer(int dealer){
    this->dealer = dealer;
}


/**
 * sets up a new hand
 */
void EuchreGame::startNewHand(){
    trickCards.fill(Card(0));           //empty the trick cards
    isPlaying.fill(true);               //everyone is playing
    for (Player* player : players){
        player->startNewHand();         //reset all players
    }
    trick.clear();                      //empty the trick
    setDealer(nextPlayer(getDealer())); //rotate the dealer
    deck.shuffle();                     //shuffle up
    deal();                             //and deal
    top = deck.pop();                   //flip over the top card
    inTop = true;                       //we are in the top card phase
}


/**
 * deals Hand::NUM_CARDS cards to each player, does not check hand sizes/deck cards so may segfault
 */
void EuchreGame::deal(){
    for (int i = 0; i < Hand::NUM_CARDS; i++){
        for (Player* player : players){
            player->addCard(deck.pop());
        }
    }
}


/**
 * @return true if the game is over, any team has MAX_SCORE or more points
 */
bool EuchreGame::isGameOver(){
    return getTeamScore(UP_TEAM) >= MAX_SCORE || getTeamScore(LEFT_TEAM) >= MAX_SCORE;
}

/**
 * @param team the team to get the score of
 * @return the score that the team has
 */
int EuchreGame::getTeamScore(int team){
    return score[team];
}


/**
 * @return the team that is winning, or DRAW if there is a tie
 */
int EuchreGame::getWinningTeam(){
    if (getTeamScore(UP_TEAM) > getTeamScore(LEFT_TEAM)){
        return UP_TEAM;
    }
    else if (getTeamScore(UP_TEAM) < getTeamScore(LEFT_TEAM)){
        return LEFT_TEAM;
    }
    return DRAW;
}


/**
 * @param playerIDX the player index to get the next player of
 * @return the player index after playerIDX, skipping players who are not playing
 */
int EuchreGame::nextPlayer(int playerIDX){
    int next = (playerIDX + 1) % NUM_PLAYERS;
    while (!isPlaying[next]){
        next = (next + 1) % NUM_PLAYERS;
    }
    return next;
}


/**
 * @param playerIDX the player to get the index of the other player on their team
 * @return the player index of the other player on the same team as playerIDX
 */
int EuchreGame::getOtherPlayerOnTeam(int playerIDX){
    return (playerIDX + (NUM_PLAYERS / NUM_TEAMS)) % NUM_PLAYERS;
}


/**
 * @param playerIDX the player index to get the team of
 * @return the team of the player
 */
int EuchreGame::getPlayerTeam(int playerIDX){
    if (playerIDX == UP || playerIDX == DOWN){
        return UP_TEAM;
    }
    return LEFT_TEAM;
}


/**
 * @param player1 a player index
 * @param player2 another player index, not equal to player1
 * @return true if the two players are on the same team (and are different players)
 */
bool EuchreGame::isSameTeam(int player1, int player2){
    return getOtherPlayerOnTeam(player1) == player2;
    //return getPlayerTeam(player1) == getPlayerTeam(player2);  //true when player1==player2
}


/**
 * @param team the team to get the other team
 * @return the other team
 */
int EuchreGame::getOtherTeam(int team){
    return team ^ 0x1;  //0^1 = 1, 1^1 = 0
    /*
    if (team == UP_TEAM){
        return LEFT_TEAM;
    }
    return UP_TEAM;
    */
}


/**
 * @return the top card
 */
Card EuchreGame::getTopCard(){
    return top;
}

/**
 * removes the top card from play, causes inTopCardPhase() to return false, getTopCard() is still working
 */
void EuchreGame::turnDownTopCard(){
    inTop = false;
}

/**
 * @param top the new top card to set
 */
void EuchreGame::setTopCard(Card top){
    this->top = top;
}

/**
 * @return true if the top card is still up
 */
bool EuchreGame::inTopCardPhase(){
    return inTop;
}


/**
 * @return the calling team
 */
int EuchreGame::getCallingTeam(){
    return call.first;
}

/**
 * @return true if the calling team went alone this hand
 */
bool EuchreGame::wentAlone(){
    return call.second;
}

/**
 * @param playerIDX the player index to test if they are playing
 * @return true if the player is in the game this hand
 */
bool EuchreGame::isPlayingThisHand(int playerIDX){
    return isPlaying[playerIDX];
}

/**
 * @param playerIDX the player index to set they are playing or not
 * @param playing if they are playing this hand or not
 */
void EuchreGame::setPlaying(int playerIDX, bool playing){
    isPlaying[playerIDX] = playing;
}


/**
 * @param playerIDX the player index of the card to get
 * @return the card the player played in the current trick
 */
Card EuchreGame::getCard(int playerIDX){
    return trickCards[playerIDX];
}

/**
 * @param playerIDX the player index to check
 * @return true if the player has played a card in the current trick
 */
bool EuchreGame::playedCard(int playerIDX){
    return trickCards[playerIDX].hashCode() != 0;
}


/**
 * @param playerIDX the player index
 * @return the string to display for the top card
 */
std::string EuchreGame::topCard(int playerIDX){
    return isDealer(playerIDX) && inTopCardPhase() ? getTopCard().toString() : "  ";
}

/**
 * @param playerIDX the player index
 * @return the string to display for their trick card
 */
std::string EuchreGame::trkCard(int playerIDX){
    return playedCard(playerIDX) ? getCard(playerIDX).toString() : "  ";
}

/**
 * displays the game
 */
void EuchreGame::draw(){
    std::cout
        << "\t+------------+" << std::endl
        << "\t|   " << topCard(UP) << trkCard(UP) << "     |" << std::endl
        << "\t|         " << topCard(RIGHT) << " |" << std::endl
        << "\t| " << trkCard(LEFT) << "      " << trkCard(RIGHT) << " |" << std::endl
        << "\t| " << topCard(LEFT) << "         |" << std::endl
        << "\t|     " << trkCard(DOWN) << topCard(DOWN) << "   |" << std::endl
        << "\t+------------+" << std::endl;
}


/**
 * @param playerIDX the player making the trump call
 * @param response the response from Player::*
 * @return the trump suit
 */
int EuchreGame::updateCall(int playerIDX, std::pair<int ,bool> response){
    call = std::make_pair(getPlayerTeam(playerIDX), response.second);
    if (wentAlone()){
        setPlaying(getOtherPlayerOnTeam(playerIDX), false); //they went alone
    }
    if (inTopCardPhase() && isPlayingThisHand(getDealer())){
        getPlayer(getDealer())->replaceCard(getTopCard());  //replace dealer card if they are still playing
    }
    return response.first;
}


/**
 * plays the phase of the game where the up card is available
 * @return the suit that was picked up, or Player::Pass if it was turned down
 */
int EuchreGame::topCardPhase(){
    std::pair<int,bool> response;
    for (int i = nextPlayer(getDealer()); i != getDealer(); i = nextPlayer(i)){
        response = getPlayer(i)->orderUp(getTopCard(), isSameTeam(i, getDealer()));
        if (response.first != Player::Pass){
            std::cout << getPlayer(i)->getName() << " ordered up the " << getTopCard().toString() << " to " << getPlayer(getDealer())->getName() << std::endl;
            return updateCall(i, response);
        }
        else {
            std::cout << getPlayer(i)->getName() << " passed" << std::endl;
        }
    }

    response = getPlayer(getDealer())->pickItUp(getTopCard());
    if (response.first != Player::Pass){
        return updateCall(dealer, response);
    }
    return Player::Pass;
}


/**
 * plays the phase of the game where players call trump
 * @return the called trump suit
 */
int EuchreGame::callPhase(int unavailableSuit){
    std::pair<int,bool> response;
    for (int i = nextPlayer(getDealer()); i != getDealer(); i = nextPlayer(i)){
        response = getPlayer(i)->callTrump(unavailableSuit);
        if (response.first != Player::Pass){
            return updateCall(i, response);
        }
        else {
            std::cout << getPlayer(i)->getName() << " passed" << std::endl;
        }
    }
    response = getPlayer(getDealer())->stickTrump(unavailableSuit);
    std::cout << getPlayer(getDealer())->getName() << " called " << Card::SUIT_SYMBOLS[response.first] << std::endl;
    return updateCall(dealer, response);
}


/**
 * for overriding, called after each card is played during a trick
 * @param card the card that was most recently played
 */
void EuchreGame::postCardTrickUpdate(Card card){
}


/**
 * plays the tricks phase of the game
 * @param trump the trump suit
 * @param startPlayer the starting player for the trick
 * @return the winner of the trick, relative to who started the trick
 */
int EuchreGame::trickPhase(int trump, int startPlayerIDX){
    trick.clear();
    trick.setTrump(trump);
    trickCards.fill(Card(0));
    for (int i = startPlayerIDX, first = 1; i != startPlayerIDX || first; i = nextPlayer(i), first = 0){
        trickCards[i] = getPlayer(i)->playCard(trick);
        trick.addCard(trickCards[i]);
        postCardTrickUpdate(trickCards[i]);
        draw();
    }
    return trick.getWinner();
}


/**
 * @param wins the trick wins per team, updates the scores for this hand
 */
void EuchreGame::scorePhase(std::array<int, NUM_TEAMS> trickWins){
    int toScore = 0;
    int scoringTeam = 0;
    if (trickWins[getCallingTeam()] == Hand::NUM_CARDS){
        if (wentAlone()){
            toScore = ALONE_ALL;
            scoringTeam = getCallingTeam();
        }
        else {
            toScore = GOT_THEM_ALL;
            scoringTeam = getCallingTeam();
        }
    }
    else if (trickWins[getCallingTeam()] > Hand::NUM_CARDS / 2){
        toScore = NORMAL_WIN;
        scoringTeam = getCallingTeam();
    }
    else {
        toScore = EUCHRE_POINTS;
        scoringTeam = getOtherTeam(getCallingTeam());
    }
    score[scoringTeam] += toScore;
    std::cout << getTeamName(scoringTeam) << " scores " << toScore << " points" << std::endl << std::endl;
}


/**
 * runs through an entire game
 */
void EuchreGame::play(){
    std::array<int, NUM_TEAMS> wins;
    while (!isGameOver()){
        startNewHand();
        draw();

        int trump = topCardPhase();
        turnDownTopCard();
        if (trump == Player::Pass){
            draw();
            std::cout << getTopCard().toString() << " was turned down" << std::endl;
            trump = callPhase(getTopCard().getSuit());
        }

        wins.fill(0);
        int start = nextPlayer(getDealer());
        for (int round = 0; round < Hand::NUM_CARDS; round++){
            /**
             * @todo this fails when someone is alone
             */
            int winner = trickPhase(trump, start);
            start = (start + winner) % NUM_PLAYERS;
            wins[getPlayerTeam(start)]++;
            std::cout << getPlayer(start)->getName() << " takes the trick " << wins[UP_TEAM] << "-" << wins[LEFT_TEAM] << std::endl;
        }
        scorePhase(wins);
    }
    draw();
    std::cout << getTeamName(getWinningTeam()) << " wins" << std::endl << std::endl;
}
