package org.lemon.euchre

import org.lemon.euchre.strategies.*

import scala.collection.parallel.CollectionConverters.*
import scala.util.Random

/** Outcomes of a set of calls, from the calling team's side. */
final case class CallStats(hands: Int, made: Int, marched: Int, points: Int):
  def euchred: Int = hands - made
  def madeRate: Double = made.toDouble / hands
  def marchRate: Double = marched.toDouble / hands
  def euchreRate: Double = euchred.toDouble / hands
  def pointsPerCall: Double = points.toDouble / hands

  def :+(hand: HandResult): CallStats =
    val tricks = hand.callerTricks
    CallStats(
      hands + 1,
      made + (if tricks > Game.handSize / 2 then 1 else 0),
      marched + (if tricks == Game.handSize then 1 else 0),
      points + hand.callerPoints,
    )

object CallStats:
  val empty: CallStats = CallStats(0, 0, 0, 0)
  def of(hands: IterableOnce[HandResult]): CallStats = hands.iterator.foldLeft(empty)(_ :+ _)

object BiddingStats:
  def by[K: Ordering](hands: Seq[HandResult])(key: HandResult => K): Seq[(K, CallStats)] =
    hands.groupBy(key).view.mapValues(CallStats.of).toSeq.sortBy(_._1)

  /** @return the number of trumps in `seat`'s hand as dealt */
  def trumps(hand: HandResult, seat: Seat): Int = hand.dealt(seat).filter(_.isTrump(hand.call.trump)).size

  /** @return a deck that deals `hand` again */
  def deck(hand: HandResult): Seq[Card] =
    val order = (1 to Seat.values.size).map(hand.dealer.offset)
    val dealt = order.flatMap(hand.dealt(_).toSeq) :+ hand.upCard
    dealt ++ Card.all.filterNot(dealt.contains)

  /** Replays a stuck hand with the dealer naming each legal suit without going alone, everyone playing `player`.
    *
    * @return the calling team's points for each suit
    */
  def stuckAlternatives(hand: HandResult, player: Strategy): Map[Suit, Int] =
    Suit.values
      .filter(_ != hand.upCard.suit)
      .map(trump =>
        val game = Game(Seat.values.map(_ -> StuckWith(player, trump)).toMap, Random(0))
        trump -> game.playHand(hand.dealer, deck(hand)).callerPoints
      )
      .toMap

  private def row(cells: Any*): String = cells.mkString("| ", " | ", " |")
  private def percent(rate: Double): String = f"${100 * rate}%.1f%%"

  def table[K](heading: String, rows: Seq[(K, CallStats)]): String =
    val header = Seq(
      row(heading, "calls", "made", "marched", "euchred", "points per call"),
      row("---", "---:", "---:", "---:", "---:", "---:"),
    )
    val body = rows.map((key, stats) =>
      row(
        key,
        stats.hands,
        percent(stats.madeRate),
        percent(stats.marchRate),
        percent(stats.euchreRate),
        f"${stats.pointsPerCall}%+.3f",
      )
    )
    (header ++ body).mkString("\n")

  def report(hands: Seq[HandResult]): String =
    Seq(
      table("all calls", Seq("all" -> CallStats.of(hands))),
      table(
        "round, caller (4 is the dealer)",
        by(hands)(hand => (hand.round, hand.callerPosition)).map((k, v) => s"${k._1}, ${k._2}" -> v)
      ),
      table("alone", by(hands)(_.call.alone)),
      table("caller's trumps", by(hands)(hand => trumps(hand, hand.caller))),
      table("partner's trumps", by(hands)(hand => trumps(hand, hand.caller.partner))),
      table("team's trumps", by(hands)(hand => trumps(hand, hand.caller) + trumps(hand, hand.caller.partner))),
    ).mkString("\n\n")

  /** Points per stuck call for the suit each stuck dealer named, and for the alternatives. */
  final case class StuckSummary(asPlayed: Double, replayed: Double, average: Double, best: Double)

  def stuckSummary(hands: Seq[HandResult], player: Strategy): StuckSummary =
    val stuck = hands.filter(_.stuck)
    val alternatives = stuck.par.map(hand => hand -> stuckAlternatives(hand, player)).seq
    def perCall(points: Iterable[Double]) = points.sum / stuck.size
    StuckSummary(
      asPlayed = perCall(stuck.map(_.callerPoints.toDouble)),
      replayed = perCall(alternatives.map((hand, points) => points(hand.call.trump))),
      average = perCall(alternatives.map((_, points) => points.values.sum.toDouble / points.size)),
      best = perCall(alternatives.map((_, points) => points.values.max.toDouble)),
    )

  def stuckReport(summary: StuckSummary): String =
    def points(perCall: Double) = f"$perCall%+.3f"
    Seq(
      row("stuck dealer names", "points per call"),
      row("---", "---:"),
      row("the named suit, as played", points(summary.asPlayed)),
      row("the named suit, replayed without going alone", points(summary.replayed)),
      row("an average legal suit", points(summary.average)),
      row("the best legal suit in hindsight", points(summary.best)),
    ).mkString("\n")

/** Reports on every call made with `player`'s trick play at all four seats, bidding as the stuck dealer or with
  * `RuleBidder.tuned`: `runBiddingStats [games=10000] [seed=1] [player=PartnerHighLow] [bidder=stuck|rule]`.
  */
@main
def runBiddingStats(args: String*): Unit =
  val games = args.lift(0).fold(10000)(_.toInt)
  val seed = args.lift(1).fold(1L)(_.toLong)
  val trickPlayer = Strategies.named(args.lift(2).getOrElse(PartnerHighLow.name))
  val player = args.lift(3).getOrElse("stuck") match
    case "stuck" => trickPlayer
    case "rule" => Composite(s"${trickPlayer.name} with RuleBidder", RuleBidder.tuned, trickPlayer)
    case other => throw IllegalArgumentException(s"unknown bidder $other, expected stuck or rule")
  val game = Game(Seat.values.map(_ -> player).toMap, Random(seed))
  val hands = Seq.fill(games)(game.playGame()).flatMap(_.hands)
  println(s"${player.name} at every seat, $games games, seed $seed\n")
  println(BiddingStats.report(hands))
  println()
  println(BiddingStats.stuckReport(BiddingStats.stuckSummary(hands, player)))
