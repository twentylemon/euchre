package org.lemon.euchre.strategies

import org.lemon.euchre.*

import scala.collection.parallel.CollectionConverters.*

/** Deals the unseen cards `samples` times, consistently with what this seat knows, and plays out every way the rest of
  * the hand could go for each playable card. Plays the card with the highest total score.
  *
  * The score of a playout tree, as in the paper, is the sum over its completed tricks after the current one of +1 for a
  * trick our team wins and -1 for one they lose. Falls back to `PartnerHighLow` early in the hand, when only one card
  * is playable, when someone is sitting out, or when no consistent deal is found.
  */
final case class MonteCarlo(samples: Int = MonteCarlo.defaultSamples) extends Strategy, StuckDealer:
  val name = "MonteCarlo"

  def play(view: PlayView): Card =
    val candidates = view.playable.toSeq
    val deals =
      if candidates.size <= 1 || view.knowledge.seen.size <= 7 || view.sittingOut.isDefined then Seq.empty
      else Seq.fill(samples)(MonteCarlo.deal(view)).flatten
    if deals.isEmpty then PartnerHighLow.play(view)
    else
      val scores = deals.par
        .map(hands => candidates.map(card => MonteCarlo.score(view, hands, card)))
        .seq
        .transpose
        .map(_.sum)
      candidates(scores.indexOf(scores.max))

object MonteCarlo:
  val defaultSamples = 30
  private val maxAttempts = 100
  private val numSeats = Seat.values.size

  /** Cards a seat is publicly known to hold (the dealer's up card) are dealt to it first. The dealer may have
    * discarded the up card, so they're only dealt while the seat has room for them.
    *
    * @return a hand for each seat consistent with `view.knowledge`, with this seat's actual hand, if one is found
    */
  def deal(view: PlayView): Option[Vector[Cards]] =
    val knowledge = view.knowledge
    val others = Seat.values.filter(_ != view.seat).sortBy(knowledge.possible(_).size)
    def handSize(seat: Seat) = view.hand.size - (if view.trick.cardOf(seat).isDefined then 1 else 0)
    def attempt(): Option[Vector[Cards]] =
      val hands = Vector.fill(numSeats)(Cards.empty).updated(view.seat.ordinal, view.hand)
      val start = Option(view.rng.shuffle(knowledge.unseen.toSeq) -> hands)
      others.foldLeft(start)((state, seat) =>
        state.flatMap((pool, hands) =>
          val possible = knowledge.possible(seat)
          val known = Cards.from((possible & knowledge.seen).iterator.take(handSize(seat)))
          val (taken, rest) = pool.partition(possible.contains)
          val need = handSize(seat) - known.size
          Option.when(taken.size >= need)(
            (taken.drop(need) ++ rest, hands.updated(seat.ordinal, known ++ Cards.from(taken.take(need))))
          )
        )
      ).map(_._2)
    Iterator.continually(attempt()).take(maxAttempts).collectFirst { case Some(hands) => hands }

  def score(view: PlayView, hands: Vector[Cards], card: Card): Int =
    val me = view.seat
    def without(hands: Vector[Cards], seat: Seat, card: Card) = hands.updated(seat.ordinal, hands(seat.ordinal) - card)

    def nextTricks(hands: Vector[Cards], leader: Seat): Int =
      if hands(me.ordinal).isEmpty then 0 else playOut(Trick(view.trump), leader, hands, scored = true)

    def playOut(trick: Trick, seat: Seat, hands: Vector[Cards], scored: Boolean): Int =
      if trick.size == numSeats then
        val winner = trick.winner
        val won = if !scored then 0 else if winner.team == me.team then 1 else -1
        won + nextTricks(hands, winner)
      else
        trick.playable(hands(seat.ordinal)).iterator
          .map(card => playOut(trick :+ (seat -> card), seat.next, without(hands, seat, card), scored))
          .sum

    playOut(view.trick :+ (me -> card), me.next, without(hands, me, card), scored = false)
