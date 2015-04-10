
#pragma once
#include <ppl.h>
#include <ctime>
#include <atomic>
#include "Random.h"
#include "CardCountingPlayer.h"

/**
 * uses a monte carlo tree search to make the best decision, maximizing the number of tricks won
 *
 * requires #define HAND_ALL_HANDS to compile
 */
class MonteCarloPlayer : public CardCountingPlayer
{
public:
    MonteCarloPlayer();
    MonteCarloPlayer(int timeout);
    MonteCarloPlayer(std::string name);
    MonteCarloPlayer(int timeout, std::string name);

    static const int DEFAULT_TIMEOUT = 10000;

    std::vector<Hand> getPossibleHands(int playerIDX, int numCards) const;
    void setTimeout(int timeout);
    int getTimeout() const;

    /*
    virtual std::pair<int,bool> orderUp(const Card& top, bool yourTeam) const override;
    virtual std::pair<int,bool> pickItUp(const Card& top) const override;
    virtual void replaceCard(const Card& top) override;
    virtual std::pair<int,bool> callTrump(int badSuit) const override;
    virtual std::pair<int,bool> stickTrump(int badSuit) const override;
    */
    virtual Card playCard(const Trick& trick) override;
    virtual Card playFirst(const Trick& trick);
    virtual Card playSecond(const Trick& trick);
    virtual Card playThird(const Trick& trick);
    virtual Card playFourth(const Trick& trick);

    virtual std::array<Hand, Trick::NUM_CARDS> makeHands();

    virtual Card playOutGame(const Trick& actualTrick);
    virtual void addTrickCard(Trick& trick, int card, std::array<Hand, Trick::NUM_CARDS>& hands, int playerIDX);
    virtual void replaceCard(Trick& trick, int card, std::array<Hand, Trick::NUM_CARDS>& hands, int playerIDX);
    virtual int nextTrick(std::array<Hand, Trick::NUM_CARDS>& hands, int toLead, int trump);

protected:
    std::vector<Hand> left, partner, right;
    int timeout;
};
