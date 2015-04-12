

#include "CardCountingPlayer.h"


/**
 * constructor
 */
CardCountingPlayer::CardCountingPlayer() : PartnerHLPlayer() {
}

/**
 * @param name the name of this player
 */
CardCountingPlayer::CardCountingPlayer(std::string name) : PartnerHLPlayer(name) {
}


/**
 * preps the ai player for a new hand.
 */
void CardCountingPlayer::startNewHand() {
    knownCards.reset();
    for (auto &row : table) {
        row.set();
    }
    Player::startNewHand();
}


/**
 * @return the bitset of all cards known by this player
 */
std::bitset<Card::NUM_CARDS> CardCountingPlayer::getKnownCards() const {
    return knownCards;
}

/**
 * @param card the card to set as seen for this player
 * @param playerIDX the player that played the card
 */
void CardCountingPlayer::publicKnowledge(const Card& card, int playerIDX) {
    knownCards.set(Card::HASH_IDX[card.hashCode()], true);
    for (int i = 0; i < (int)table.size(); i++) {
        table[i].set(Card::HASH_IDX[card.hashCode()], false);
    }
}

/**
 * @param card the card to set as seen for this player
 * @param playerIDX the player that played the card
 * @param trick the trick the card was played into
 */
void CardCountingPlayer::publicKnowledge(const Card& card, int playerIDX, const Trick& trick) {
    PartnerHLPlayer::publicKnowledge(card, playerIDX, trick);
    if (playerIDX != getPosition()) { //ignore our plays
        //if they played an illegal card, they are out of the lead suit
        if (!trick.isLegal(card)) {
            for (int hash : Trick::EFF_SUIT_CARDS[trick.getTrump()][trick.getLeadSuit()]) {
                table[playerIDX].set(Card::HASH_IDX[hash], false);
            }
        }
    }
}

/**
 * @param playerIDX the player id
 * @param suit the suit to check
 * @param trump the trump suit
 * @return true if the player has any of the suit remaining
 */
bool CardCountingPlayer::playerHasSuit(int playerIDX, int suit, int trump) const {
    return playerCountSuit(playerIDX, suit, trump) > 0;
}

/**
 * @param playerIDX the player id
 * @param suit the suit to check
 * @param trump the trump suit
 * @return number of cards in that suit they *could* have
 */
int CardCountingPlayer::playerCountSuit(int playerIDX, int suit, int trump) const {
    int count = 0;
    for (int hash : Trick::EFF_SUIT_CARDS[trump][suit]) {
        if (table[playerIDX][Card::HASH_IDX[hash]]) {
            count++;
        }
    }
    return count;
}

/**
 * @param playerIDX the player id
 * @param suit the suit to check
 * @param trump the trump suit
 * @return sum of CardScore's of the cards that the player could have
 */
int CardCountingPlayer::playerStrengthSuit(int playerIDX, int suit, int trump) const {
    int strength = 0;
    for (int hash : Trick::EFF_SUIT_CARDS[trump][suit]) {
        if (table[playerIDX][Card::HASH_IDX[hash]]) {
            strength += CardScore::get(trump, hash);
        }
    }
    return strength;
}

/**
 * @param playerIDX the player id
 * @param card the card to beat
 * @param trump the trump suit
 * @return sum of CardScore's of the cards that the player could have
 */
int CardCountingPlayer::playerStrengthCard(int playerIDX, const Card& card, int trump) const {
    int strength = 0;
    for (int hash : Trick::EFF_SUIT_CARDS[trump][Trick::EFFECTIVE_SUITS[trump][card.hashCode()]]) {
        if (table[playerIDX][Card::HASH_IDX[hash]] && CardScore::hashComparator[trump](card.hashCode(), hash)) {
            strength += CardScore::get(trump, hash);    //they could have the card, and hash is better than it
        }
    }
    return strength;
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card CardCountingPlayer::playCard(const Trick& trick) {
    if (trick.getNumCards() == 0) {
        return playFirst(trick);
    }
    else if (trick.getNumCards() == 1 ) {
        return playSecond(trick);
    }
    else if (trick.getNumCards() == 2) {
        return playThird(trick);
    }
    return playFourth(trick);   //on failure, just play partner high low
}


/**
 * @param trick the trick to play the card in
 * @return the card to lead
 */
Card CardCountingPlayer::playFirst(const Trick& trick) {
    std::vector<int> cards = getHand().getCards();
    int suitToPlay = -1;
    //if partner is out of a suit and has trump, lead low in the suit
    if (playerHasSuit(partnerIDX(), trick.getTrump(), trick.getTrump())) {
        for (Card card : cards) {
            if (!playerHasSuit(partnerIDX(), card.getSuit(), trick.getTrump())) {
                suitToPlay = card.getSuit();
                break;  //just use the first suit partner is out of
            }
        }
        if (suitToPlay != -1) { //they are out of a suit, lead it low
            return hand.removeCard(CardScore::getWorstCard(hand.getSuitCards(suitToPlay), trick));
        }
    }

    //check if partner is "strong" in any suit, lead low that suit
    const int noncalc = -10000;
    std::array<int, Card::NUM_SUITS> difs = {{ noncalc, noncalc, noncalc, noncalc }};
    for (Card card : cards) {
        if (difs[card.getSuit()] == noncalc) {
            int partner = playerStrengthSuit(partnerIDX(), card.getSuit(), trick.getTrump());
            int left = playerStrengthSuit(relativeIDX(1), card.getSuit(), trick.getTrump());
            int right = playerStrengthSuit(relativeIDX(-1), card.getSuit(), trick.getTrump());
            if (partner > 0) { //if they actually have the suit
                difs[card.getSuit()] = std::min(partner - left, partner - right);   //only consider the worst difference
            }
            else {
                difs[card.getSuit()] = noncalc + 1; //to avoid recalculation
            }
        }
    }
    int best = 0;   //use 0 as a threshold, partner must have a better hand than either of the others
    for (int suit : Card::SUITS) {
        if (difs[suit] > best) {
            best = difs[suit];  //should never be able to select a suit that we don't have
            suitToPlay = suit;  //since we loop over the cards in our hand
        }
    }
    if (suitToPlay != -1) { //they are strong in a suit, lead it
        return hand.removeCard(CardScore::getWorstCard(hand.getSuitCards(suitToPlay), trick));
    }
    return PartnerHLPlayer::playCard(trick);    //otherwise, play high
}

/**
 * @param trick the trick to play the card in
 * @return the card to play second
 */
Card CardCountingPlayer::playSecond(const Trick& trick) {
    //if partner is stronger than the opposition, play low
    int partner = playerStrengthCard(partnerIDX(), trick.getWinningCard(), trick.getTrump());
    if (partner > 0 && partner - playerStrengthSuit(relativeIDX(1), trick.getLeadSuit(), trick.getTrump()) > 0) {
        return hand.removeCard(CardScore::getWorstCard(hand.getPlayableCards(trick), trick));
    }
    //if partner doesn't have the suit
    if (!playerHasSuit(partnerIDX(), trick.getLeadSuit(), trick.getTrump())) {
        //if trump wasn't led and my partner has trump, play low - partner is able to trump
        if (trick.getLeadSuit() != trick.getTrump() && playerHasSuit(partnerIDX(), trick.getTrump(), trick.getTrump())) {
            return hand.removeCard(CardScore::getWorstCard(hand.getPlayableCards(trick), trick));
        }
    }
    return PartnerHLPlayer::playCard(trick);    //otherwise, play high low
}

/**
 * @param trick the trick to play the card in
 * @return the card to play third
 */
Card CardCountingPlayer::playThird(const Trick& trick) {
    //if the partner is winning but the last player can potentially beat them
    if (trick.getWinner() == 0 && playerStrengthCard(relativeIDX(1), trick.getWinningCard(), trick.getTrump())) {
        return HighLowPlayer::playCard(trick);  //play high low - beat partner if possible
    }
    return PartnerHLPlayer::playCard(trick);    //otherwise, play partner high low
}

/**
 * @param trick the trick to play the card in
 * @return the card to play last
 */
Card CardCountingPlayer::playFourth(const Trick& trick) {
    return PartnerHLPlayer::playCard(trick);
}
