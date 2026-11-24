package org.lemon.euchre

final case class Trick(trump: Suit, plays: Vector[(Seat, Card)] = Vector.empty):
  def isEmpty: Boolean = plays.isEmpty
  def size: Int = plays.size
  def cards: Vector[Card] = plays.map(_._2)
  def cardOf(seat: Seat): Option[Card] = plays.collectFirst { case (`seat`, card) => card }

  def leadSuit: Suit = plays.head._2.effectiveSuit(trump)

  /** @return whether `card` follows suit, which any card does when leading */
  def follows(card: Card): Boolean = isEmpty || card.effectiveSuit(trump) == leadSuit

  /** @return the cards of `hand` that follow suit, or the whole hand if none do */
  def playable(hand: Cards): Cards =
    val following = hand.filter(follows)
    if following.isEmpty then hand else following

  def :+(play: (Seat, Card)): Trick = copy(plays = plays :+ play)

  private def canWin(card: Card): Boolean = card.isTrump(trump) || card.effectiveSuit(trump) == leadSuit

  def winnerIndex: Int = plays.indices.filter(i => canWin(plays(i)._2)).maxBy(i => Ranking.score(trump, plays(i)._2))
  def winner: Seat = plays(winnerIndex)._1
  def winningCard: Card = plays(winnerIndex)._2

  override def toString: String =
    val winning = if isEmpty then "N/A" else winningCard.toString
    s"T: ${trump.symbol}; ${cards.mkString(" ")}  W: $winning"
