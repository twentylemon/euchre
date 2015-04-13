

#include "MonteCarloPlayer.h"

/**
 * constructor
 */
MonteCarloPlayer::MonteCarloPlayer() : timeout(DEFAULT_TIMEOUT), CardCountingPlayer() {
}

/**
 * @param name the name of this player
 */
MonteCarloPlayer::MonteCarloPlayer(int timeout) : timeout(timeout), CardCountingPlayer() {
}

/**
 * @param name the name of this player
 */
MonteCarloPlayer::MonteCarloPlayer(std::string name) : timeout(DEFAULT_TIMEOUT), CardCountingPlayer(name) {
}

/**
 * @param name the name of this player
 */
MonteCarloPlayer::MonteCarloPlayer(int timeout, std::string name) : timeout(timeout), CardCountingPlayer(name) {
}

/**
 * @param timeout the new timeout to set
 */
void MonteCarloPlayer::setTimeout(int timeout) {
    this->timeout = timeout;
}

/**
 * @return the timeout this player is using
 */
int MonteCarloPlayer::getTimeout() const {
    return timeout;
}


/**
 * @param playerIDX the player to get the list of possible hands for
 * @param numCards the number of cards they should have in their hand
 */
std::vector<Hand> MonteCarloPlayer::getPossibleHands(int playerIDX, int numCards) const {
    std::vector<Hand> hands;
#ifdef HAND_ALL_HANDS
    for (const Hand& hand : Hand::ALL_HANDS[numCards]) {
        if (hand.subsetOf(table[playerIDX])) {
            hands.push_back(hand);
        }
    }
#endif
    return hands;
}


/**
 * @param trick the trick to play the card in
 * @return the card to play
 */
Card MonteCarloPlayer::playCard(const Trick& trick) {
    std::vector<int> cards = getHand().getPlayableCards(trick);
    if (cards.size() > 1 && getKnownCards().count() > 7) {
        return CardCountingPlayer::playCard(trick); //fork to playFirst etc
    }
    return PartnerHLPlayer::playCard(trick);
}


/**
 * @param trick the trick to play the card in
 * @return the card to lead
 */
Card MonteCarloPlayer::playFirst(const Trick& trick) {
    left = getPossibleHands(relativeIDX(1), getNumCards());
    partner = getPossibleHands(partnerIDX(), getNumCards());
    right = getPossibleHands(relativeIDX(-1), getNumCards());
    return playOutGame(trick);
}

/**
 * @param trick the trick to play the card in
 * @return the card to play second
 */
Card MonteCarloPlayer::playSecond(const Trick& trick) {
    left = getPossibleHands(relativeIDX(1), getNumCards() - 1);
    partner = getPossibleHands(partnerIDX(), getNumCards());
    right = getPossibleHands(relativeIDX(-1), getNumCards());
    return playOutGame(trick);
}

/**
 * @param trick the trick to play the card in
 * @return the card to play third
 */
Card MonteCarloPlayer::playThird(const Trick& trick) {
    left = getPossibleHands(relativeIDX(1), getNumCards() - 1);
    partner = getPossibleHands(partnerIDX(), getNumCards() - 1);
    right = getPossibleHands(relativeIDX(-1), getNumCards());
    return playOutGame(trick);
}

/**
 * @param trick the trick to play the card in
 * @return the card to play last
 */
Card MonteCarloPlayer::playFourth(const Trick& trick) {
    left = getPossibleHands(relativeIDX(1), getNumCards() - 1);
    partner = getPossibleHands(partnerIDX(), getNumCards() - 1);
    right = getPossibleHands(relativeIDX(-1), getNumCards() - 1);
    return playOutGame(trick);
}


/**
 * @return possible hands for each player
 */
std::array<Hand, Trick::NUM_CARDS> MonteCarloPlayer::makeHands() {
    int l, p, r;
    do {
        l = Random::nextInt(left.size());
        p = Random::nextInt(partner.size());
        r = Random::nextInt(right.size());
    } while (left[l].intersects(partner[p].getBitset() | right[r].getBitset()));
    std::array<Hand, Trick::NUM_CARDS - 1> otherHands = {{ left[l], partner[p], right[r] }};
    std::array<Hand, Trick::NUM_CARDS> hands;
    hands[getPosition()] = Hand(getHand()); //my position, copy my hand
    for (int idx = 1; idx <= 3; idx++) {
        hands[relativeIDX(idx)] = otherHands[idx - 1]; 
    }
    return hands;
}

/**
 * @param actualTrick the starting trick
 * @return list of pairs of our card hashes and the sum of the number of points achieved in their playout path
 */
Card MonteCarloPlayer::playOutGame(const Trick& actualTrick) {
    if (left.empty() || partner.empty() || right.empty()) {
        return PartnerHLPlayer::playCard(actualTrick);  //shouldn't trigger, but make the quick check
    }
    std::vector<int> cards = getHand().getPlayableCards(actualTrick);
    std::array<std::atomic<int>, Hand::NUM_CARDS> scores;
    for (int i = 0; i < Hand::NUM_CARDS; i++) {
        scores[i] = 0;
    }
    int runs = 0;
    std::clock_t time = std::clock();
    while (std::clock() - time < getTimeout()) {
        runs++;
        //do about timeout/2000=30default iterations per run
        concurrency::parallel_for(0, getTimeout() / 2000, [&](int useless) {
    std::array<Hand, Trick::NUM_CARDS> hands = makeHands();

    //finish this trick first
    Trick trick(actualTrick);
    int lPos = relativeIDX(1), pPos = partnerIDX(), rPos = relativeIDX(-1), mPos = getPosition();
    for (int i = 0; i < (int)cards.size(); i++) {
        addTrickCard(trick, cards[i], hands, mPos);
        //get all players playable cards now to avoid repeated calls
        std::vector<int> lCards = hands[lPos].getPlayableCards(trick);
        std::vector<int> pCards = hands[pPos].getPlayableCards(trick);
        std::vector<int> rCards = hands[rPos].getPlayableCards(trick);
        if (trick.getNumCards() < Trick::NUM_CARDS) { //if more cards are needed in the trick
            for (int lCard : lCards) {
                addTrickCard(trick, lCard, hands, lPos);    //add left's cards
                if (trick.getNumCards() < Trick::NUM_CARDS) {
                    for (int pCard : pCards) {
                        addTrickCard(trick, pCard, hands, pPos);
                        if (trick.getNumCards() < Trick::NUM_CARDS) {
                            for (int rCard : rCards) {
                                addTrickCard(trick, rCard, hands, rPos);
                                scores[i] += nextTrick(hands, relativeIDX(trick.getWinner() - actualTrick.getNumCards()), trick.getTrump());
                                replaceCard(trick, rCard, hands, rPos); //deepest possible, this is when we led the trick
                            }
                        }
                        else {
                            scores[i] += nextTrick(hands, relativeIDX(trick.getWinner() - actualTrick.getNumCards()), trick.getTrump());
                        }
                        replaceCard(trick, pCard, hands, pPos);
                    }
                }
                else {
                    scores[i] += nextTrick(hands, relativeIDX(trick.getWinner() - actualTrick.getNumCards()), trick.getTrump());
                }
                replaceCard(trick, lCard, hands, lPos);
            }
        }
        else {
            scores[i] += nextTrick(hands, relativeIDX(trick.getWinner() - actualTrick.getNumCards()), trick.getTrump());
        }
        replaceCard(trick, cards[i], hands, mPos);
    }

        }); //parallel_for
    }   //while
    int best = 0;
    for (int i = 0; i < (int)cards.size(); i++) {
        if (scores[i] > scores[best]) {
            best = i;
        }
    }
    return hand.removeCard(cards[best]);
}

/**
 * adds the card to the trick, removes the card from the player's hand
 */
void MonteCarloPlayer::addTrickCard(Trick& trick, int hash, std::array<Hand, Trick::NUM_CARDS>& hands, int playerIDX) {
    trick.addCard(Card( hands[playerIDX].removeCard(hash) ));
}

/**
 * replaces the card into the player's hand
 */
void MonteCarloPlayer::replaceCard(Trick& trick, int hash, std::array<Hand, Trick::NUM_CARDS>& hands, int playerIDX) {
    trick.removeLastCard();
    hands[playerIDX].addCard(hash);
}

/**
 * recursively plays out the rest of the game
 * @return scoring
 */
int MonteCarloPlayer::nextTrick(std::array<Hand, Trick::NUM_CARDS>& hands, int toLead, int trump) {
    if (hands[getPosition()].getNumCards() <= 0) {
        return 0;
    }
    Trick trick(trump);
    std::vector<int> leadCards = hands[toLead].getCards();
    int mPos = toLead, lPos = cyclicIDX(toLead+1), pPos = cyclicIDX(toLead+2), rPos = cyclicIDX(toLead+3);
    int result = 0;
    for (int leadCard : leadCards) {
        addTrickCard(trick, leadCard, hands, mPos);
        std::vector<int> lCards = hands[lPos].getPlayableCards(trick);
        std::vector<int> pCards = hands[pPos].getPlayableCards(trick);
        std::vector<int> rCards = hands[rPos].getPlayableCards(trick);
        for (int lCard : lCards) {
            addTrickCard(trick, lCard, hands, lPos);
            for (int pCard : pCards) {
                addTrickCard(trick, pCard, hands, pPos);
                for (int rCard : rCards) {
                    addTrickCard(trick, rCard, hands, rPos);
                    int winnerPos = cyclicIDX(trick.getWinner() + toLead);
                    if (winnerPos == getPosition() || winnerPos == partnerIDX()) {
                        result += 1 + nextTrick(hands, winnerPos, trick.getTrump());
                    }
                    else {
                        result += -1 + nextTrick(hands, winnerPos, trick.getTrump());
                    }
                    replaceCard(trick, rCard, hands, rPos);
                }
                replaceCard(trick, pCard, hands, pPos);
            }
            replaceCard(trick, lCard, hands, lPos);
        }
        replaceCard(trick, leadCard, hands, mPos);
    }
    return result;
}
