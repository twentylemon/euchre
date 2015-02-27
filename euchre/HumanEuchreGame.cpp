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
    setPublicKnowledgeCallback([=](Card card){
        left.seenCard(card);
        right.seenCard(card);
        partner.seenCard(card);
    });
}


/**
 * displays the game
 */
void HumanEuchreGame::draw(){
    std::cout
        << "         partner" << (isDealer(PARTNER) ? "*" : "") << std::endl
        << "        " << std::string("X X X X X ", 2*partner.getHand().getNumCards()) << std::endl
        << "      +------------+" << std::endl
        << " l " << (left.getHand().getNumCards() > 0 ? "X" : " ") << "  |   " << topCard(PARTNER) << trkCard(PARTNER) << "     |  " << (right.getHand().getNumCards() > 0 ? "X" : " ") << " r" << std::endl
        << " e " << (left.getHand().getNumCards() > 1 ? "X" : " ") << "  |         " << topCard(RIGHT) << " |  " << (right.getHand().getNumCards() > 1 ? "X" : " ") << " i" << std::endl
        << " f " << (left.getHand().getNumCards() > 2 ? "X" : " ") << "  | " << trkCard(LEFT) << "      " << trkCard(RIGHT) << " |  " << (right.getHand().getNumCards() > 2 ? "X" : " ") << " g" << std::endl
        << " t " << (left.getHand().getNumCards() > 3 ? "X" : " ") << "  | " << topCard(LEFT) << "         |  " << (right.getHand().getNumCards() > 3 ? "X" : " ") << " h" << std::endl
        << " " << (isDealer(LEFT) ? "*" : " ") << " " << (left.getHand().getNumCards() > 4 ? "X" : " ") << "  |     " << trkCard(HUMAN) << topCard(HUMAN) << "   |  " << (right.getHand().getNumCards() > 4 ? "X" : " ") << " t" << std::endl
        << "      +------------+    " << (isDealer(RIGHT) ? "*" : "") << std::endl
        << "      " << human.getHand().toString() << std::endl
        << (trick.getTrump() == -1 ? "        " : "trump: " + Card::SUIT_SYMBOLS[trick.getTrump()]) << "   you" << (isDealer(HUMAN) ? "*" : "") << "\t\tyou " << getTeamScore(HUMAN_TEAM) << "-" << getTeamScore(AI_TEAM) << " ai" << std::endl
        << (trick.getTrump() == -1 ? "" : "called by " + getTeamName(getCallingTeam())) << std::endl;
}
