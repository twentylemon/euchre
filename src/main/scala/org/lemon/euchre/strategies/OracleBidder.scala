package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Bids the option with the most expected points, from `SimulatedCalls.evaluate` over `samples` deals of the unseen
  * cards. The other seats, and this seat's own later bids, are modelled as bidding like `others`, and every seat plays
  * tricks like `player`. Discards like `others`. Slow: it plays every option out on every sampled deal.
  */
final case class OracleBidder(
    samples: Int = 100,
    others: Bidder = RuleBidder.tuned,
    player: Player = PartnerHighLow,
) extends Bidder:
  private def best(view: BidView, round: Int): Option[Call] =
    SimulatedCalls.best(SimulatedCalls.evaluate(Decision.of(view, round), samples, others, player, view.rng))

  def orderUp(view: BidView): Option[Call] = best(view, 1)
  def callTrump(view: BidView): Option[Call] = best(view, 2)
  def stickTrump(view: BidView): Call = best(view, 2).get
  def discard(view: BidView, trump: Suit): Card = others.discard(view, trump)
