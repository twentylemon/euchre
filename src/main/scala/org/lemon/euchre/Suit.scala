package org.lemon.euchre

enum Suit(val symbol: Char):
  case Spades extends Suit('♠')
  case Hearts extends Suit('♥')
  case Diamonds extends Suit('♦')
  case Clubs extends Suit('♣')

  /** @return the other suit of the same colour, the suit of the left bower when this suit is trump */
  def sameColour: Suit = Suit.fromOrdinal(ordinal ^ 3)

enum Rank(val symbol: Char):
  case Nine extends Rank('9')
  case Ten extends Rank('T')
  case Jack extends Rank('J')
  case Queen extends Rank('Q')
  case King extends Rank('K')
  case Ace extends Rank('A')
