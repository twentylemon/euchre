

#pragma once
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "Player.h"
#include "Random.h"

#include <array>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
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

    virtual Player* getPlayer(int playerIDX);
    virtual void setPlayer(int playerIDX, Player* player);
    virtual void setTeamName(int team, std::string name);
    virtual std::string getTeamName(int team);

    virtual int getDealer();
    virtual bool isDealer(int playerIDX);
    virtual void setDealer(int dealer);

    virtual void startNewHand();
    virtual void deal();
    virtual void setScore(int upScore, int leftScore);

    virtual bool isGameOver();
    virtual int getTeamScore(int team);
    virtual int getWinningTeam();

    virtual int nextPlayer(int playerIDX);
    virtual int getOtherPlayerOnTeam(int playerIDX);
    virtual int getPlayerTeam(int playerIDX);
    virtual bool isSameTeam(int player1, int player2);
    virtual int getOtherTeam(int team);
    
    virtual Card getTopCard();
    virtual void turnDownTopCard();
    virtual void setTopCard(Card top);
    virtual bool inTopCardPhase();

    virtual int getCallingTeam();
    virtual bool wentAlone();
    virtual bool isPlayingThisHand(int playerIDX);
    virtual void setPlaying(int playerIDX, bool playing);
    
    virtual void clearTrick();
    virtual Card getCard(int playerIDX);
    virtual bool playedCard(int playerIDX);
    virtual std::string topCard(int playerIDX);
    virtual std::string trkCard(int playerIDX);
    virtual void draw();

    void setPublicKnowledgeCallback(std::function<void(Card,int)> fn);

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

private:
    std::function<void(Card,int)> publicKnowledgeCallback;
};
