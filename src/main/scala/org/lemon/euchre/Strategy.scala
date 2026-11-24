package org.lemon.euchre

import scala.util.Random

final case class Call(trump: Suit, alone: Boolean = false)

final case class BidView(seat: Seat, dealer: Seat, hand: Cards, upCard: Card, rng: Random):
  def isDealer: Boolean = seat == dealer

final case class PlayView(
    seat: Seat,
    hand: Cards,
    trick: Trick,
    caller: Seat,
    alone: Boolean,
    knowledge: Knowledge,
    rng: Random,
):
  def trump: Suit = trick.trump
  def partner: Seat = seat.partner
  def playable: Cards = trick.playable(hand)
  def sittingOut: Option[Seat] = Option.when(alone)(caller.partner)

trait Bidder:
  /** Round one: order up the up card, or pick it up as the dealer. The call must be in the up card's suit. */
  def orderUp(view: BidView): Option[Call]

  /** Round two: name any suit except the turned down `view.upCard`'s. */
  def callTrump(view: BidView): Option[Call]

  /** Round two, when everyone else passed: the dealer must name a suit other than the turned down one. */
  def stickTrump(view: BidView): Call

  /** @param view the dealer's view, with the up card already in `view.hand` */
  def discard(view: BidView, trump: Suit): Card

trait Player:
  /** @return a card from `view.playable` */
  def play(view: PlayView): Card

trait Strategy extends Bidder, Player:
  def name: String

/** Bidding as in the paper: everyone passes, and the stuck dealer names the first legal suit. */
trait StuckDealer extends Bidder:
  def orderUp(view: BidView): Option[Call] = None
  def callTrump(view: BidView): Option[Call] = None
  def stickTrump(view: BidView): Call = Call(Suit.values.find(_ != view.upCard.suit).get)
  def discard(view: BidView, trump: Suit): Card = Ranking.worst(view.hand, trump)
