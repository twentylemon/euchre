package org.lemon.euchre

final case class Card(rank: Rank, suit: Suit):
  val ordinal: Int = rank.ordinal * Suit.values.size + suit.ordinal

  def isRightBower(trump: Suit): Boolean = rank == Rank.Jack && suit == trump
  def isLeftBower(trump: Suit): Boolean = rank == Rank.Jack && suit == trump.sameColour

  /** @return the suit this card counts as, which differs from `suit` only for the left bower */
  def effectiveSuit(trump: Suit): Suit = if isLeftBower(trump) then trump else suit
  def isTrump(trump: Suit): Boolean = effectiveSuit(trump) == trump

  override def toString: String = s"${rank.symbol}${suit.symbol}"

object Card:
  val all: IndexedSeq[Card] =
    for
      rank <- Rank.values.toIndexedSeq
      suit <- Suit.values
    yield Card(rank, suit)

  def fromOrdinal(ordinal: Int): Card = all(ordinal)
