package org.lemon.euchre

/** An immutable set of cards, one bit per `Card.ordinal`. */
opaque type Cards = Int

object Cards:
  val empty: Cards = 0
  val all: Cards = (1 << Card.all.size) - 1

  def apply(cards: Card*): Cards = from(cards)
  def from(cards: IterableOnce[Card]): Cards = cards.iterator.foldLeft(empty)((set, card) => set | bit(card))
  def fromBits(bits: Int): Cards = bits & all

  private def bit(card: Card): Int = 1 << card.ordinal

  extension (cards: Cards)
    def bits: Int = cards
    def size: Int = Integer.bitCount(cards)
    def isEmpty: Boolean = cards == 0
    def nonEmpty: Boolean = cards != 0
    def contains(card: Card): Boolean = (cards & bit(card)) != 0

    def +(card: Card): Cards = cards | bit(card)
    def -(card: Card): Cards = cards & ~bit(card)
    def ++(other: Cards): Cards = cards | other
    def --(other: Cards): Cards = cards & ~other
    def &(other: Cards): Cards = cards & other
    def complement: Cards = all & ~cards
    def subsetOf(other: Cards): Boolean = (cards & ~other) == 0

    def iterator: Iterator[Card] =
      Iterator.unfold(cards)(rest =>
        Option.when(rest != 0)(Card.fromOrdinal(Integer.numberOfTrailingZeros(rest)) -> (rest & (rest - 1)))
      )
    def toSeq: Seq[Card] = iterator.toSeq
    def filter(p: Card => Boolean): Cards = iterator.filter(p).foldLeft(empty)((set, card) => set | bit(card))
    def sumBy(f: Card => Int): Int = iterator.map(f).sum
    def mkString(sep: String): String = iterator.mkString(sep)
