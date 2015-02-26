#include "HumanEuchreGame.h"

/**
 * sets up a euchre game
 */
HumanEuchreGame::HumanEuchreGame() : EuchreGame(&partner, &human, &left, &right){
    dealer = std::rand() % NUM_PLAYERS;
    setTeamName(UP_TEAM, "your team");
    setTeamName(LEFT_TEAM, "ai team");
    human.setName("you");
    partner.setName("partner");
    left.setName("left");
    right.setName("right");
}


/**
 * starts a new hand, updates seen the top card for the ai
 */
void HumanEuchreGame::startNewHand(){
    EuchreGame::startNewHand();
    postCardTrickUpdate(getTopCard());
}


/**
 * displays the game
 */
void HumanEuchreGame::draw(){
    std::cout
        << "         partner" << (dealer == PARTNER ? "*" : "") << std::endl
        << "        " << std::string("X X X X X ", 2*partner.getHand().getNumCards()) << std::endl
        << "      +------------+" << std::endl
        << " l " << (left.getHand().getNumCards() > 0 ? "X" : " ") << "  |   " << (dealer == PARTNER && top.hashCode() ? top.toString() : "  ") << (trickCards[PARTNER].hashCode() ? trickCards[PARTNER].toString() : "  ") << "     |  " << (right.getHand().getNumCards() > 0 ? "X" : " ") << " r" << std::endl
        << " e " << (left.getHand().getNumCards() > 1 ? "X" : " ") << "  |         " << (dealer == RIGHT && top.hashCode() ? top.toString() : "  ") << " |  " << (right.getHand().getNumCards() > 1 ? "X" : " ") << " i" << std::endl
        << " f " << (left.getHand().getNumCards() > 2 ? "X" : " ") << "  | " << (trickCards[LEFT].hashCode() ? trickCards[LEFT].toString() : "  ") << "      " << (trickCards[RIGHT].hashCode() ? trickCards[RIGHT].toString() : "  ") << " |  " << (right.getHand().getNumCards() > 2 ? "X" : " ") << " g" << std::endl
        << " t " << (left.getHand().getNumCards() > 3 ? "X" : " ") << "  | " << (dealer == LEFT && top.hashCode() ? top.toString() : "  ") << "         |  " << (right.getHand().getNumCards() > 3 ? "X" : " ") << " h" << std::endl
        << " " << (dealer == LEFT ? "*" : " ") << " " << (left.getHand().getNumCards() > 4 ? "X" : " ") << "  |     " << (trickCards[HUMAN].hashCode() ? trickCards[HUMAN].toString() : "  ") << (dealer == HUMAN && top.hashCode() ? top.toString() : "  ") << "   |  " << (right.getHand().getNumCards() > 4 ? "X" : " ") << " t" << std::endl
        << "      +------------+    " << (dealer == RIGHT ? "*" : "") << std::endl
        << "      " << human.getHand().toString() << std::endl
        << (trick.getTrump() == -1 ? "        " : "trump: " + Card::SUIT_SYMBOLS[trick.getTrump()]) << "   you" << (dealer == HUMAN ? "*" : "") << "\t\tyou " << getTeamScore(HUMAN_TEAM) << "-" << getTeamScore(AI_TEAM) << " ai" << std::endl
        << (trick.getTrump() == -1 ? "" : "called by " + teamNames[call.first]) << std::endl;
}


/**
 * @param card a card that all the ai should mark as seen
 */
void HumanEuchreGame::postCardTrickUpdate(Card card){
    left.seenCard(card);
    right.seenCard(card);
    partner.seenCard(card);
}
