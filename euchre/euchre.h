
#pragma once
#include <map>
#include <array>
#include <vector>
#include <string>
#include <bitset>

#include <new>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <Windows.h>

#include "Random.h"
#include "BitString.h"

#include "Card.h"
#include "CardScore.h"
#include "Deck.h"
#include "Hand.h"
#include "Trick.h"

#include "Player.h"
#include "AIPlayer.h"
#include "LowPlayer.h"
#include "HighPlayer.h"
#include "RandomPlayer.h"
#include "HighLowPlayer.h"
#include "PartnerHLPlayer.h"
#include "MarkovPlayer.h"
#include "Markov2Player.h"
#include "CardCountingPlayer.h"
#include "MonteCarloPlayer.h"
#include "HybridPlayer.h"
#include "Hybrid2Player.h"

#include "EuchreGame.h"
#include "DataGame.h"
#include "HumanEuchreGame.h"

#define ulong unsigned long long

//#define EVAL
#ifdef EVAL
extern std::array<std::array<int, 4>, Card::MAX_CARD> evaluateTricks();
extern std::array<int, Card::MAX_CARD> trickWins();
extern std::map<int, Trick> getAllTricks();

extern void eval(int n);
#endif
