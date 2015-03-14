

#pragma once
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Random.h"

#include <ctime>
#include <array>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <functional>

/**
 * base class for euchre games
 */
class EuchreGame
{
public:
    static const int NUM_PLAYERS = 4;
    static const int NUM_TEAMS = 2;

    static const int DOWN = 0;
    static const int LEFT = 1;
    static const int UP = 2;
    static const int RIGHT = 3;
    
    static const int UP_TEAM = 0;
    static const int DOWN_TEAM = 0;
    static const int LEFT_TEAM = 1;
    static const int RIGHT_TEAM = 1;
    static const int DRAW = -1;

    static const int MAX_SCORE = 10;
    static const int NORMAL_WIN = 1;
    static const int GOT_THEM_ALL = 2;
    static const int EUCHRE_POINTS = 2;
    static const int ALONE_ALL = 4;


    EuchreGame();
    EuchreGame(Player* up, Player* down, Player* left, Player* right);

    void init();
    virtual void startNewGame();

    virtual Player* getPlayer(int playerIDX) const;
    virtual void setPlayer(int playerIDX, Player* player);
    virtual void setTeamName(int team, std::string name);
    virtual std::string getTeamName(int team) const;

    virtual int getDealer() const;
    virtual bool isDealer(int playerIDX) const;
    virtual void setDealer(int dealer);

    virtual void startNewHand();
    virtual void deal();
    virtual void setScore(int upScore, int leftScore);

    virtual bool isGameOver() const;
    virtual int getTeamScore(int team) const;
    virtual int getWinningTeam() const;

    virtual int nextPlayer(int playerIDX) const;
    virtual int getOtherPlayerOnTeam(int playerIDX) const;
    virtual int getPlayerTeam(int playerIDX) const;
    virtual bool isSameTeam(int player1, int player2) const;
    virtual int getOtherTeam(int team) const;
    
    virtual const Card& getTopCard() const;
    virtual void turnDownTopCard();
    virtual void setTopCard(const Card& top);
    virtual bool inTopCardPhase() const;

    virtual int getCallingTeam() const;
    virtual bool wentAlone() const;
    virtual bool isPlayingThisHand(int playerIDX) const;
    virtual void setPlaying(int playerIDX, bool playing);
    virtual void publicKnowledgeCallback(const Card& card, int playerIDX);
    
    virtual void clearTrick();
    virtual const Card& getCard(int playerIDX) const;
    virtual bool playedCard(int playerIDX) const;
    virtual std::string topCard(int playerIDX) const;
    virtual std::string trkCard(int playerIDX) const;
    virtual void draw() const;
    virtual void write(std::string str) const;
    virtual void setDisplay(bool displayStuff);
    
    virtual std::array<int, NUM_TEAMS> getGameWins() const;
    virtual std::array<int, NUM_TEAMS> getTrickWins() const;
    virtual std::array<int, NUM_TEAMS> getTotalScores() const;
    virtual void displayStats() const;

    virtual void play();
    virtual int updateCall(int playerIDX, std::pair<int,bool> response);
    virtual int topCardPhase();
    virtual int callPhase(int unavailableSuit);
    virtual int trickPhase(int trump, int startPlayerIDX);
    virtual void scorePhase(std::array<int, NUM_TEAMS> trickWins);

protected:
    std::array<Player*, NUM_PLAYERS> players;
    std::array<int, NUM_TEAMS> score;
    std::array<bool, NUM_PLAYERS> isPlaying;
    
    std::array<std::string, NUM_TEAMS> teamNames;
    
    Deck deck;
    Trick trick;
    std::array<Card, NUM_PLAYERS> trickCards;
    
    Card top;
    bool inTop;
    int dealer;
    std::pair<int,bool> call;

    bool displayStuff;
    std::array<int, NUM_TEAMS> gameWins;
    std::array<int, NUM_TEAMS> trickWins;
    std::array<int, NUM_TEAMS> totalScores;
    std::array<std::clock_t, NUM_PLAYERS> timeTaken;
};
