
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <Windows.h>

#include "Card.h"
#include "CardScore.h"
#include "Deck.h"
#include "Trick.h"
#include "Hand.h"

extern std::array<std::array<int, 4>, Card::MAX_CARD> evaluateTricks();
extern std::map<int, Trick> getAllTricks();
