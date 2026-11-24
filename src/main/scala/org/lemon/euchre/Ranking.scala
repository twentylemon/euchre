package org.lemon.euchre

/** Card scores per trump suit. Higher scores are better, but an off-suit card only wins if it follows the lead suit. */
object Ranking:
  val minTrumpScore = 7
  val leftBower = 12
  val rightBower = 13
  private val trumpScores = IArray(7, 8, rightBower, 9, 10, 11)
  private val offScores = IArray(1, 2, 3, 4, 5, 6)

  private val scores: IArray[IArray[Int]] = IArray.from(Suit.values.map(trump =>
    IArray.from(Card.all.map(card =>
      if card.isLeftBower(trump) then leftBower
      else if card.suit == trump then trumpScores(card.rank.ordinal)
      else offScores(card.rank.ordinal)
    ))
  ))

  def score(trump: Suit, card: Card): Int = scores(trump.ordinal)(card.ordinal)

  /** @return the highest scoring card, the first in `Cards` order on a tie */
  def best(cards: Cards, trump: Suit): Card = cards.iterator.maxBy(score(trump, _))

  /** @return the lowest scoring card, the first in `Cards` order on a tie */
  def worst(cards: Cards, trump: Suit): Card = cards.iterator.minBy(score(trump, _))

  private val numCards = Card.all.size

  private def allTricks(trump: Suit): Iterator[Trick] =
    for
      first <- Card.all.iterator
      second <- Card.all.iterator if second != first
      third <- Card.all.iterator if third != first && third != second
      fourth <- Card.all.iterator if fourth != first && fourth != second && fourth != third
    yield Seat.values.zip(Seq(first, second, third, fourth)).foldLeft(Trick(trump))(_ :+ _)

  private lazy val trickWinTable: IArray[IArray[Int]] = IArray.from(Suit.values.map(trump =>
    val wins = Array.fill(numCards)(0)
    allTricks(trump).foreach(trick => wins(trick.winningCard.ordinal) += 1)
    IArray.from(wins)
  ))

  /** @return the number of tricks, out of every ordering of 4 distinct cards, that `card` wins */
  def trickWins(trump: Suit, card: Card): Int = trickWinTable(trump.ordinal)(card.ordinal)

  private def prefixIndex(cards: Iterable[Card]): Int = cards.foldLeft(0)((idx, card) => idx * numCards + card.ordinal)

  /** Indexed by trump, then by prefix length - 1, then by `prefixIndex`. */
  private lazy val partialWinTable: IArray[IArray[IArray[Int]]] = IArray.from(Suit.values.map(trump =>
    val wins = (1 to Seat.values.size).map(length => Array.fill(math.pow(numCards, length).toInt)(0))
    allTricks(trump).foreach(trick =>
      val winners = trick.cards.take(trick.winnerIndex + 1)
      wins(winners.size - 1)(prefixIndex(winners)) += 1
    )
    IArray.from(wins.map(IArray.from))
  ))

  /** @return the number of completions of `trick` in which `card`, played next, wins the trick */
  def partialWins(trick: Trick, card: Card): Int =
    val cards = trick.cards :+ card
    partialWinTable(trick.trump.ordinal)(cards.size - 1)(prefixIndex(cards))
