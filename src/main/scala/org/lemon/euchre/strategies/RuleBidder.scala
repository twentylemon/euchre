package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** A count of a hand's tricks for a trump suit. Each trump counts its chance of winning a random trick it's in (1 for
  * the right bower, down to 0.38 for the nine). Each off-suit ace counts `offAce`, and each void counts `void` if the
  * hand holds a trump to ruff with.
  */
final case class HandStrength(offAce: Double, void: Double):
  def apply(hand: Cards, trump: Suit): Double =
    val trumps = hand.filter(_.isTrump(trump))
    val offAces = hand.filter(card => card.rank == Rank.Ace && !card.isTrump(trump)).size
    val voids = Suit.values.count(suit => hand.filter(_.effectiveSuit(trump) == suit).isEmpty)
    trumps.iterator.map(HandStrength.winChance(trump, _)).sum + offAce * offAces +
      (if trumps.nonEmpty then void * voids else 0)

object HandStrength:
  private val tricksPerCard = Seat.values.size * (1 until Seat.values.size).map(Card.all.size - _).product

  /** @return the chance `card` wins a random trick it's in */
  def winChance(trump: Suit, card: Card): Double = Ranking.trickWins(trump, card).toDouble / tricksPerCard

/** Bids when `strength` reaches the threshold for the bidder's seat.
  *
  * In round one the up card counts for the dealer's team and against the other. The dealer counts the hand they'd
  * keep after picking up. In round two the bidder considers their strongest legal suit, with `next` added to the
  * strength of the turned down suit's colour. A stuck dealer names their strongest legal suit. Any call goes alone at
  * `alone` strength. The dealer discards to make a void if they can, keeping off-suit aces.
  *
  * @param orderUp the strength to order up from positions 1 to 3, then for the dealer to pick up
  * @param call the strength to call in round two from positions 1 to 3
  */
final case class RuleBidder(
    strength: HandStrength,
    orderUp: Seq[Double],
    call: Seq[Double],
    next: Double,
    alone: Double,
) extends Bidder:
  private def bid(view: BidView, trump: Suit, value: Double, threshold: Double): Option[Call] =
    Option.when(value >= threshold)(Call(trump, alone = value >= alone))

  def orderUp(view: BidView): Option[Call] =
    val trump = view.upCard.suit
    val value =
      if view.isDealer then strength(RuleBidder.pickUp(view.hand, view.upCard), trump)
      else
        val upCard = HandStrength.winChance(trump, view.upCard)
        strength(view.hand, trump) + (if view.dealer.team == view.seat.team then upCard else -upCard)
    bid(view, trump, value, orderUp(view.position - 1))

  private def strongest(view: BidView): (Suit, Double) =
    val turnedDown = view.upCard.suit
    Suit.values
      .filter(_ != turnedDown)
      .map(suit => suit -> (strength(view.hand, suit) + (if suit == turnedDown.sameColour then next else 0)))
      .maxBy(_._2)

  def callTrump(view: BidView): Option[Call] =
    val (trump, value) = strongest(view)
    bid(view, trump, value, call(view.position - 1))

  def stickTrump(view: BidView): Call =
    val (trump, value) = strongest(view)
    Call(trump, alone = value >= alone)

  def discard(view: BidView, trump: Suit): Card = RuleBidder.discard(view.hand, trump)

object RuleBidder:
  /** From `runRuleSweep 4000 2 1 0.01`. */
  val tuned: RuleBidder = RuleBidder(
    HandStrength(offAce = 0.5, void = 0.3),
    orderUp = Seq(2.25, 2.5, 2.25, 2.0),
    call = Seq(2.0, 2.0, 2.0),
    next = 0.25,
    alone = 3.0,
  )

  /** @return the card to discard: the lowest off-suit singleton that isn't an ace, which makes a void, else the
    *   lowest off-suit card that isn't an ace, else the lowest card
    */
  def discard(hand: Cards, trump: Suit): Card =
    val spare = hand.filter(card => !card.isTrump(trump) && card.rank != Rank.Ace)
    val singletons = spare.filter(card => hand.filter(_.effectiveSuit(trump) == card.effectiveSuit(trump)).size == 1)
    Ranking.worst(Seq(singletons, spare, hand).find(_.nonEmpty).get, trump)

  /** @return the dealer's hand after picking up `upCard` and discarding */
  def pickUp(hand: Cards, upCard: Card): Cards =
    val withUpCard = hand + upCard
    withUpCard - discard(withUpCard, upCard.suit)
