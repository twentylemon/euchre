

#include "CardScore.h"

/**
 * @return the comparators given the hashcode of the cards
 */
std::array<std::function<bool(int, int)>, Card::NUM_SUITS> mapHash() {
    std::array<std::function<bool(int, int)>, Card::NUM_SUITS> comp;
    std::transform(Card::SUITS.begin(), Card::SUITS.end(), comp.begin(), [](int suit) {
        return std::function<bool(int, int)>(
            [suit](int card1, int card2) { return CardScore::get(suit, card1) < CardScore::get(suit, card2); }
        );
    });
    return comp;
}

/**
 * @return the comparators given the cards
 */
std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> mapCard() {
    std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> comp;
    std::transform(Card::SUITS.begin(), Card::SUITS.end(), comp.begin(), [](int suit) {
        return std::function<bool(const Card&, const Card&)>(
            [suit](const Card& card1, const Card& card2) { return CardScore::get(suit, card1) < CardScore::get(suit, card2); }
        );
    });
    return comp;
}

/**
 * @return the comparators given the hashcode of the cards
 */
std::array<std::function<bool(int, int)>, Card::NUM_SUITS> mapWinHash() {
    std::array<std::function<bool(int, int)>, Card::NUM_SUITS> comp;
    std::transform(Card::SUITS.begin(), Card::SUITS.end(), comp.begin(), [](int suit) {
        return std::function<bool(int, int)>(
            [suit](int card1, int card2) { return CardScore::winRate(suit, card1) < CardScore::winRate(suit, card2); }
        );
    });
    return comp;
}

/**
 * @return the comparators given the cards
 */
std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> mapWinCard() {
    std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> comp;
    std::transform(Card::SUITS.begin(), Card::SUITS.end(), comp.begin(), [](int suit) {
        return std::function<bool(const Card&, const Card&)>(
            [suit](const Card& card1, const Card& card2) { return CardScore::winRate(suit, card1) < CardScore::winRate(suit, card2); }
        );
    });
    return comp;
}

/**
 * the scores of each card by trump suit
 */
std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> CardScore::rankings;

/**
 * how many tricks each card wins
 */
std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> CardScore::trickWins;

/**
 * scores of trump cards and non trump cards, left bower is separate from trump
 */
const std::array<int, Card::NUM_RANKS> CardScore::TRUMP_SCORE = {{ 6, 7, CardScore::RIGHT_BOWER, 8, 9, 10 }};   //6=trump 9, ... , 10=trump ace
const std::array<int, Card::NUM_RANKS> CardScore::OFF_SCORE   = {{ 0, 1, 2, 3, 4, 5 }};     //0=off 9 etc

/**
 * comparators for sortings cards according to their score
 */
const std::array<std::function<bool(int, int)>, Card::NUM_SUITS> CardScore::hashComparator = mapHash();
const std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> CardScore::cardComparator = mapCard();

/**
 * comparators for sortings cards according to their trick win rates
 */
const std::array<std::function<bool(int, int)>, Card::NUM_SUITS> CardScore::hashWinComparator = mapWinHash();
const std::array<std::function<bool(const Card&, const Card&)>, Card::NUM_SUITS> CardScore::cardWinComparator = mapWinCard();

CardScore::CardScore() {
}


/**
 * @param trump the suit that is trump
 * @param card the card to get the score of
 * @return the score of the card given the trump suit
 */
int CardScore::get(int trump, const Card& card) {
    return get(trump, card.hashCode());
}

/**
 * @param trump the suit that is trump
 * @param hash the has of the card to get the score of
 * @return the score of the card given the trump suit
 */
int CardScore::get(int trump, int hash) {
    return rankings[trump][hash];
}


/**
 * @param trump the suit that is trump
 * @param card the card to get the score of
 * @return the number of tricks that the card wins
 */
int CardScore::winRate(int trump, const Card& card) {
    return winRate(trump, card.hashCode());
}

/**
 * @param trump the suit that is trump
 * @param hash the has of the card to get the score of
 * @return the number of tricks that the card wins
 */
int CardScore::winRate(int trump, int hash) {
    return trickWins[trump][hash];
}


/**
 * @param trump the trump suit
 * @param cards bitset of cards to get the win rate of
 * @return the total win rate of all the cards in the bitset
 */
int CardScore::sumWinRate(int trump, std::bitset<Card::NUM_CARDS> cards){
    int total = 0;
    for (int i = 0; i < Card::NUM_CARDS; i++) {
        if (cards[i]) {
            total += winRate(trump, Card::ALL_CARDS[i]);
        }
    }
    return total;
}


/**
 * @param cards the list of cards to play from
 * @param trick the trick
 * @return the best card from the list
 */
int CardScore::getBestCard(std::vector<int> cards, const Trick& trick) {
    return *std::max_element(cards.begin(), cards.end(), hashComparator[trick.getTrump()]);
}

/**
 * @param cards the list of cards to play from
 * @param trick the trick
 * @return the worst card from the list
 */
int CardScore::getWorstCard(std::vector<int> cards, const Trick& trick) {
    return *std::min_element(cards.begin(), cards.end(), hashComparator[trick.getTrump()]);
}

/**
 * @param cards the list of cards to play from
 * @param trick the trick
 * @return the best card from the list
 */
const Card& CardScore::getBestCard(std::vector<Card> cards, const Trick& trick) {
    return *std::max_element(cards.begin(), cards.end(), cardComparator[trick.getTrump()]);
}

/**
 * @param cards the list of cards to play from
 * @param trick the trick
 * @return the worst card from the list
 */
const Card& CardScore::getWorstCard(std::vector<Card> cards, const Trick& trick) {
    return *std::min_element(cards.begin(), cards.end(), cardComparator[trick.getTrump()]);
}


/**
 * @param cards the list of cards to play from
 * @param trick the trick
 * @return the card with the highest trick win rate from the list
 */
int CardScore::getBestWinCard(std::vector<int> cards, const Trick& trick) {
    return *std::max_element(cards.begin(), cards.end(), hashWinComparator[trick.getTrump()]);
}

/**
 * @param cards the list of cards to play from
 * @param trick the trick
 * @return the card with the lowest trick win rate from the list
 */
int CardScore::getWorstWinCard(std::vector<int> cards, const Trick& trick) {
    return *std::min_element(cards.begin(), cards.end(), hashWinComparator[trick.getTrump()]);
}

/**
 * @param cards the list of cards to play from
 * @param trick the trick
 * @return the card with the highest trick win rate from the list
 */
const Card& CardScore::getBestWinCard(std::vector<Card> cards, const Trick& trick) {
    return *std::max_element(cards.begin(), cards.end(), cardWinComparator[trick.getTrump()]);
}

/**
 * @param cards the list of cards to play from
 * @param trick the trick
 * @return the card with the lowest trick win rate from the list
 */
const Card& CardScore::getWorstWinCard(std::vector<Card> cards, const Trick& trick) {
    return *std::min_element(cards.begin(), cards.end(), cardWinComparator[trick.getTrump()]);
}


/**
 * initializes class constants
 */
void CardScore::init() {
    rankings = calcRankings();
    trickWins = calcTrickWins();
}


/**
 * initializes all of the cards' rankings for each trump value
 */
std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> CardScore::calcRankings() {
    std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> r;
    for (int trump : Card::SUITS) {
        r[trump].fill(-1);  //for non-existant cards, rate them below the worst
        for (const Card& card : Card::ALL_CARDS) {
            if (card.getRank() == Card::Jack && card.getSuit() == Card::otherSuit(trump)) {
                r[trump][card.hashCode()] = LEFT_BOWER;
            }
            else if (card.getSuit() == trump) {
                r[trump][card.hashCode()] = TRUMP_SCORE[card.getRank() - 9];    //offset by 9
            }
            else {  //not trump
                r[trump][card.hashCode()] = OFF_SCORE[card.getRank() - 9];
            }
        }
    }
    return r;
}

/**
 * @return the number of tricks each card wins
 */
std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> CardScore::calcTrickWins() {
    std::array<std::array<int, Card::MAX_CARD>, Card::NUM_SUITS> wins;
    for (int trump : Card::SUITS) {
        Trick trick(trump);
        wins[trump].fill(0);
        for (int card1 : Card::ALL_CARDS) {
            trick.addCard(card1);
            for (int card2 : Card::ALL_CARDS) {
                if (card1 == card2){ continue; }
                trick.addCard(card2);
                for (int card3 : Card::ALL_CARDS) {
                    if (card1 == card3 || card2 == card3){ continue; }
                    trick.addCard(card3);
                    for (int card4 : Card::ALL_CARDS) {
                        if (card1 == card4 || card2 == card4 || card3 == card4){ continue; }
                        trick.addCard(card4);
                        wins[trump][trick.getWinningCard().hashCode()]++;
                        trick.removeLastCard();
                    }
                    trick.removeLastCard();
                }
                trick.removeLastCard();
            }
            trick.removeLastCard();
        }
    }
    return wins;
}



/**
 * displays the calculated scores
 */
void CardScore::display() {
    for (int suit : Card::SUITS) {
        std::cout << Card::SUIT_SYMBOLS[suit] << std::endl;
        for (Card card : Card::ALL_CARDS) {
            std::cout << card.toString() << "\t" << rankings[suit][card.hashCode()] << std::endl;
        }
        system("pause");
    }
    for (Card card : Card::ALL_CARDS) {
        std::cout << card.toString() << "\t";
        for (int suit : Card::SUITS) {
            std::cout << Card::SUIT_SYMBOLS[suit];
            printf(": %6d\t", trickWins[suit][card.hashCode()]);
        }
        std::cout << std::endl;
    }
    system("pause");
}
