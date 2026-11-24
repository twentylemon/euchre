package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Tracks which cards each player could still hold, and plays by position in the trick.
  *
  *  1. Leading: if partner could have trump, lead low in the first suit of our hand that partner is out of.
  *     Otherwise lead low in the suit where partner is strongest relative to both opponents, if partner is stronger
  *     than either of them. Otherwise play `PartnerHighLow`.
  *  1. Second: play low if partner could beat the winning card and is stronger than the last player in the lead
  *     suit, or if partner is out of a non-trump lead suit but could have trump. Otherwise play `PartnerHighLow`.
  *  1. Third: if partner is winning but the last player could beat them, play `HighLow`, otherwise `PartnerHighLow`.
  *  1. Fourth: play `PartnerHighLow`.
  *
  * Suits taken from our own cards are their printed suits, not their effective suits, as in the paper.
  */
object CardCounting extends Strategy, StuckDealer:
  val name = "CardCounting"

  private def suitCards(knowledge: Knowledge, seat: Seat, suit: Suit, trump: Suit): Cards =
    knowledge.possible(seat).filter(_.effectiveSuit(trump) == suit)

  private def hasSuit(knowledge: Knowledge, seat: Seat, suit: Suit, trump: Suit): Boolean =
    suitCards(knowledge, seat, suit, trump).nonEmpty

  /** @return the sum of the scores of the cards in `suit` that `seat` could hold */
  private def suitStrength(knowledge: Knowledge, seat: Seat, suit: Suit, trump: Suit): Int =
    suitCards(knowledge, seat, suit, trump).sumBy(Ranking.score(trump, _))

  /** @return the sum of the scores of the cards that `seat` could hold that follow and outscore `card` */
  private def strengthAbove(knowledge: Knowledge, seat: Seat, card: Card, trump: Suit): Int =
    val score = Ranking.score(trump, card)
    suitCards(knowledge, seat, card.effectiveSuit(trump), trump)
      .filter(Ranking.score(trump, _) > score)
      .sumBy(Ranking.score(trump, _))

  def play(view: PlayView): Card =
    view.trick.size match
      case 0 => playFirst(view)
      case 1 => playSecond(view)
      case 2 => playThird(view)
      case _ => PartnerHighLow.play(view)

  private def lowIn(view: PlayView, suit: Suit): Card = Ranking.worst(view.hand.filter(_.suit == suit), view.trump)

  private def playFirst(view: PlayView): Card =
    val (seat, hand, trump, knowledge) = (view.seat, view.hand, view.trump, view.knowledge)
    val partnerVoid =
      if !hasSuit(knowledge, seat.partner, trump, trump) then None
      else hand.iterator.map(_.suit).find(suit => !hasSuit(knowledge, seat.partner, suit, trump))
    partnerVoid match
      case Some(suit) => lowIn(view, suit)
      case None =>
        val advantages = hand.iterator.map(_.suit).distinct.flatMap(suit =>
          val partner = suitStrength(knowledge, seat.partner, suit, trump)
          Option.when(partner > 0) {
            val left = suitStrength(knowledge, seat.offset(1), suit, trump)
            val right = suitStrength(knowledge, seat.offset(-1), suit, trump)
            suit -> math.min(partner - left, partner - right)
          }
        ).toMap
        Suit.values.filter(advantages.get(_).exists(_ > 0)).maxByOption(advantages) match
          case Some(suit) => lowIn(view, suit)
          case None => PartnerHighLow.play(view)

  private def playSecond(view: PlayView): Card =
    val (seat, trick, trump, knowledge) = (view.seat, view.trick, view.trump, view.knowledge)
    val partner = strengthAbove(knowledge, seat.partner, trick.winningCard, trump)
    val partnerStronger = partner > 0 && partner - suitStrength(knowledge, seat.offset(1), trick.leadSuit, trump) > 0
    val partnerCanTrump = !hasSuit(knowledge, seat.partner, trick.leadSuit, trump) &&
      trick.leadSuit != trump && hasSuit(knowledge, seat.partner, trump, trump)
    if partnerStronger || partnerCanTrump then Ranking.worst(view.playable, trump)
    else PartnerHighLow.play(view)

  private def playThird(view: PlayView): Card =
    val (seat, trick, trump, knowledge) = (view.seat, view.trick, view.trump, view.knowledge)
    if trick.winner == seat.partner && strengthAbove(knowledge, seat.offset(1), trick.winningCard, trump) > 0 then
      HighLow.play(view)
    else PartnerHighLow.play(view)
