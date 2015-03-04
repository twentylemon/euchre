#include "HumanEuchreGame.h"

/**
 * sets up a euchre game
 */
HumanEuchreGame::HumanEuchreGame() : EuchreGame(&partner, &human, &left, &right) {
    dealer = std::rand() % NUM_PLAYERS;
    setTeamName(UP_TEAM, "your team");
    setTeamName(LEFT_TEAM, "ai team");
    human.setName("you");
    partner.setName("partner");
    left.setName("left");
    right.setName("right");
    setPublicKnowledgeCallback([=](const Card& card, int playerIDX) {
        left.seenCard(card);
        right.seenCard(card);
        partner.seenCard(card);
    });
}


/**
 * displays the game
 */
void HumanEuchreGame::draw() const {
    std::cout
        << "         partner" << (isDealer(PARTNER) ? "*" : "") << std::endl
        << "        " << std::string("X X X X X ", 2*partner.getNumCards()) << std::endl
        << "      +------------+" << std::endl
        << " l " << (left.getNumCards() > 0 ? "X" : " ") << "  |   " << topCard(PARTNER) << trkCard(PARTNER) << "     |  " << (right.getNumCards() > 0 ? "X" : " ") << " r" << std::endl
        << " e " << (left.getNumCards() > 1 ? "X" : " ") << "  |         " << topCard(RIGHT) << " |  " << (right.getNumCards() > 1 ? "X" : " ") << " i" << std::endl
        << " f " << (left.getNumCards() > 2 ? "X" : " ") << "  | " << trkCard(LEFT) << "      " << trkCard(RIGHT) << " |  " << (right.getNumCards() > 2 ? "X" : " ") << " g" << std::endl
        << " t " << (left.getNumCards() > 3 ? "X" : " ") << "  | " << topCard(LEFT) << "         |  " << (right.getNumCards() > 3 ? "X" : " ") << " h" << std::endl
        << " " << (isDealer(LEFT) ? "*" : " ") << " " << (left.getNumCards() > 4 ? "X" : " ") << "  |     " << trkCard(HUMAN) << topCard(HUMAN) << "   |  " << (right.getNumCards() > 4 ? "X" : " ") << " t" << std::endl
        << "      +------------+    " << (isDealer(RIGHT) ? "*" : "") << std::endl
        << "      " << human.getHand().toString() << std::endl
        << (trick.getTrump() == -1 ? "        " : "trump: " + Card::SUIT_SYMBOLS[trick.getTrump()]) << "   you" << (isDealer(HUMAN) ? "*" : "") << "\t\tyou " << getTeamScore(HUMAN_TEAM) << "-" << getTeamScore(AI_TEAM) << " ai" << std::endl
        << (trick.getTrump() == -1 ? "" : "called by " + getTeamName(getCallingTeam())) << std::endl;
}
