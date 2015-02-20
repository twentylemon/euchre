
#include "euchre.h"

#ifdef EVAL

const int NUM_PLAYERS = 4;

std::vector<std::vector<int>> bitStrings;
std::vector<std::vector<Hand>> hands;

std::vector<std::array<std::array<ulong, 4>, Card::MAX_CARD>> wins;
std::array<std::array<ulong, 4>, Card::MAX_CARD> cur;
std::array<ulong, Hand::NUM_CARDS+1> totals;

std::vector<std::map<int, Hand>> handScores;

Hand trickHands[NUM_PLAYERS];
int trickCode;

const int MAX_TRICK = 0x00EFAE78 + 0x01;
int trickWinCode[MAX_TRICK];
int trickWinIDX[MAX_TRICK];

/**
 * @param numOn the number of bits that should be on
 * @return the first int with `numOn` bits on
 */
int first(int numOn){
    int ret = 1;
    for (int i = 1; i < numOn; i++){
        ret = (ret << 1) + 1;
    }
    return ret;
}

/**
 * @param current the current n bit set number
 * @return the next int with the same number of set bits as current
 */
int next(int current){
    int t = (current | (current - 1)) + 1;
    return t | ((((t & -t) / (current & -current)) >> 1) - 1);
}

/**
 * @param numOn the number of bits that should be on
 * @return the last 24bit number with `numOn` bits on
 */
int last(int numOn){
    return (0x00FFFFFF >> (Deck::SIZE - numOn)) << (Deck::SIZE - numOn);
}

/**
 * @param n create all 24bit strings with up to and including n bits on
 */
void createBitStrings(int n){
    bitStrings.clear();
    std::vector<int> vals;
    vals.push_back(0);
    bitStrings.push_back(vals);
    for (int size = 1; size <= n; size++){
        vals.clear();
        int start = first(size);
        int finish = last(size);
        for (int bits = start; bits <= finish; bits = next(bits)){
            vals.push_back(bits);
        }
        bitStrings.push_back(vals);
    }
}

/**
 * @param bits the bitset of cards
 * @return the hand using the given set of cards
 */
Hand bitsToHand(int bits){
    Hand hand;
    hand.addSet(bits);
    return hand;
}

/**
 * @param n create all hands including up to n cards
 */
void createHands(int n){
    createBitStrings(n);
    hands.clear();
    std::vector<Hand> nHands;
    for (int i = 0; i <= n; i++){
        nHands.clear();
        for (int bits : bitStrings[i]){
            nHands.push_back(bitsToHand(bits));
            if (nHands.back().getNumCards() != i){
                std::cout << bits << std::endl << nHands.back().toString() << std::endl;
                system("pause");
            }
        }
        hands.push_back(nHands);
    }
}

/**
 * @param newCard updates the global trickCode adding this card to it
 */
void updateTrickCode(int newCard){
    trickCode = (trickCode << Card::HASHBITS) | newCard;
}

/**
 * stores winners of each trick by hashcode
 */
void initTricks(){
    Trick trick(Card::Spades);
    std::fill(trickWinCode, trickWinCode + MAX_TRICK, 0);
    std::fill(trickWinIDX, trickWinIDX + MAX_TRICK, 0);
    Deck deck;

    for (int i = 0; i < Deck::SIZE; i++){
        trick.addCard(deck.getCard(i));
        trickCode = deck.getCard(i).hashCode();

        for (int j = 0; j < Deck::SIZE; j++){
            if (i == j){ continue; }
            int code1 = trickCode;
            trick.addCard(deck.getCard(j));
            updateTrickCode(deck.getCard(j).hashCode());

            for (int k = 0; k < Deck::SIZE; k++){
                if (i == k || j == k){ continue; }
                int code2 = trickCode;
                trick.addCard(deck.getCard(k));
                updateTrickCode(deck.getCard(k).hashCode());

                for (int l = 0; l < Deck::SIZE; l++){
                    if (i == l || j == l || k == l){ continue; }
                    int code3 = trickCode;
                    trick.addCard(deck.getCard(l));
                    updateTrickCode(deck.getCard(l).hashCode());
                    trickWinCode[trickCode] = trick.getWinningCard().hashCode();
                    trickWinIDX[trickCode] = trick.getWinner();
                    trick.removeLastCard();
                    trickCode = code3;
                }
                trick.removeLastCard();
                trickCode = code2;
            }
            trick.removeLastCard();
            trickCode = code1;
        }
        trick.removeLastCard();
    }
}


/**
 * clears the global current trick counter
 */
void clear(){
    for (int card : Card::ALL_CARDS){
        cur[card].fill(0);
    }
}

/**
 * @param size the hand size
 * @param hand1 the first hand
 * @param hand2 the second hand
 * @param hand3 the third hand
 */
void fourth(int size){
    int code = trickCode;
    /**
     * @todo instead of just counting, use the size-1 results to obtain a score
     *  the score can be the percentage chance of winning the trick
     *  score = (this chance) + (score of remaining size-1 hand)
     * @todo the same trick is being evaluated again and again, store results of trick wins somehow
     * @todo permutation avoidance?
     */
    for (Hand hand : hands[size]){
        if (hand.intersects(trickHands[0]) || hand.intersects(trickHands[1]) || hand.intersects(trickHands[2])){ continue; }
        for (int i = 0; i < size; i++){
            updateTrickCode(hand.getCard(i));
            cur[trickWinCode[trickCode]][trickWinIDX[trickCode]]++;
            totals[size]++;
            trickCode = code;
        }
    }
}

/**
 * @param size the hand size
 * @param hand1 the first hand
 * @param hand2 the second hand
 */
void third(int size){
    int code = trickCode;
    for (Hand hand : hands[size]){
        if (hand.intersects(trickHands[0]) || hand.intersects(trickHands[1])){ continue; }
        trickHands[2] = hand;
        for (int i = 0; i < size; i++){
            updateTrickCode(trickHands[2].getCard(i));
            fourth(size);
            trickCode = code;
        }
    }
}

/**
 * @param size the hand size
 * @param hand1 the first hand
 */
void second(int size){
    int code = trickCode;
    for (Hand hand : hands[size]){
        if (hand.intersects(trickHands[0])){ continue; }
        trickHands[1] = hand;
        for (int i = 0; i < size; i++){
            updateTrickCode(trickHands[1].getCard(i));
            third(size);
            trickCode = code;
        }
    }
}


/**
 * @param n eval hands up to including n in size
 */
void eval(const int n){
    clear();
    wins.clear();
    wins.push_back(cur);    //for size 0 hands
    totals.fill(0);
    
    createHands(n);
    initTricks();

    for (int size = 1; size <= n; size++){
        clear();
        for (Hand hand : hands[size]){
            trickHands[0] = hand;
            for (int i = 0; i < size; i++){
                std::cout << trickHands[0].toString() << "  playing  " << Card(trickHands[0].getCard(i)).toString() << std::endl;
                trickCode = trickHands[0].getCard(i);
                second(size);
                //no need to revert trickCode
            }
        }
        wins.push_back(cur);
    }

    std::array<std::array<ulong, 4>, Card::MAX_CARD> win = wins.back();//wins[n];
    for (Card card : Card::ALL_CARDS){
        std::cout << card.toString() << "\t";
        ulong total = 0;
        for (ulong w : win[card.hashCode()]){
            std::cout << w << "\t";
            total += w;
        }
        printf("\t%s: %5f\n", card.toString().c_str(), ((double)total / (double)totals[n]));
    }
}
#endif
