
#pragma once

#include <map>
#include <array>
#include <vector>
#include <string>
#include <bitset>

#include <new>
#include <iostream>
#include <algorithm>
#include <Windows.h>

#include "Random.h"

#include "Card.h"
#include "CardScore.h"
#include "Deck.h"
#include "Hand.h"
#include "Trick.h"

#include "Player.h"
#include "AIPlayer.h"
#include "RandomPlayer.h"

#include "EuchreGame.h"
#include "HumanEuchreGame.h"

#define ulong unsigned long long

extern std::array<std::array<int, 4>, Card::MAX_CARD> evaluateTricks();
extern std::array<int, Card::MAX_CARD> trickWins();
extern std::map<int, Trick> getAllTricks();

//#define EVAL
#ifdef EVAL
extern void eval(int n);
#endif
