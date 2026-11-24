package org.lemon.euchre

/** What `seat` can infer from public information.
  *
  * @param seen the cards `seat` has seen: its own hand, the up card, its discard and every card played
  * @param possible per seat, the cards that seat could still hold
  */
final case class Knowledge(seat: Seat, seen: Cards, possible: Vector[Cards]):
  def unseen: Cards = seen.complement
  def possible(other: Seat): Cards = possible(other.ordinal)

  /** @param trick the trick before `card` was played into it */
  def observe(player: Seat, card: Card, trick: Trick): Knowledge =
    val withoutCard = possible.map(_ - card)
    val updated =
      if player == seat || trick.follows(card) then withoutCard
      else
        val lead = trick.leadSuit
        withoutCard.updated(player.ordinal, withoutCard(player.ordinal).filter(_.effectiveSuit(trick.trump) != lead))
    Knowledge(seat, seen + card, updated)

object Knowledge:
  /** @param hand the hand of `seat` once bidding is over
    * @param pickedUpBy the dealer, if they picked up the up card
    * @param discarded the dealer's discard, when `seat` is the dealer
    */
  def start(
      seat: Seat,
      hand: Cards,
      upCard: Card,
      pickedUpBy: Option[Seat],
      sittingOut: Option[Seat],
      discarded: Cards = Cards.empty,
  ): Knowledge =
    val seen = hand + upCard ++ discarded
    val possible = Seat.values.toVector.map(other =>
      if other == seat then hand
      else if sittingOut.contains(other) then Cards.empty
      else if pickedUpBy.contains(other) then seen.complement + upCard
      else seen.complement
    )
    Knowledge(seat, seen, possible)
