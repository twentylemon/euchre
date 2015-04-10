

#pragma once
#include "PartnerHLPlayer.h"
#include <array>

/**
 * agent that card counts to help make decisions
 * keeps track of the odds that each player has every card
 * different strategy depending on position
 * 1) if partner out of a suit and has trump, lead that suit low hoping for trump
 *    else, check if partner has a good chance of winning for each suit lead, lead if "good"
 *    otherwise, play high
 * 2) if partner is strong in the lead suit, play low
 *    if partner doesn't have the suit: if trump, play highlow; if partner has trump, play low
 *    otherwise, play HighLow
 * 3) compare partner card strength to last player
 *    if last player is strong and we can possibly win, play high; otherwise play low
 *    otherwise, play PartnerHighLow
 * 4) play PartnerHighLow
 */
class CardCountingPlayer : public PartnerHLPlayer
{
public:
    CardCountingPlayer();
    CardCountingPlayer(std::string name);
    
    virtual void startNewHand() override;

    virtual void publicKnowledge(const Card& card, int playerIDX) override;
    virtual void publicKnowledge(const Card& card, int playerIDX, const Trick& trick) override;
    virtual std::bitset<Card::NUM_CARDS> getKnownCards() const;
    
    virtual bool playerHasSuit(int playerIDX, int suit, int trump) const;
    virtual int playerCountSuit(int playerIDX, int suit, int trump) const;
    virtual int playerStrengthSuit(int playerIDX, int suit, int trump) const;
    virtual int playerStrengthCard(int playerIDX, const Card& card, int trump) const;
    
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

protected:
    std::array<std::bitset<Card::NUM_CARDS>, Trick::NUM_CARDS> table;
    std::bitset<Card::NUM_CARDS> knownCards;
};
