package org.lemon.euchre

import org.lemon.euchre.strategies.*

import java.io.PrintWriter
import scala.collection.parallel.CollectionConverters.*
import scala.util.Random

/** `seat`'s bid in `round`, holding `hand` as dealt, after every earlier bid of the deal was a pass. */
final case class Decision(seat: Seat, dealer: Seat, hand: Cards, upCard: Card, round: Int):
  def position: Int = seat.position(dealer)

  /** @return the bids open to `seat`, a pass (`None`) first if it may pass */
  def options: Seq[Option[Call]] =
    def withAndAlone(trump: Suit) = Seq(Call(trump), Call(trump, alone = true))
    val turnedUp = upCard.suit
    if round == 1 then
      val orders = if seat == dealer.partner then Seq(Call(turnedUp, alone = true)) else withAndAlone(turnedUp)
      None +: orders.map(Some(_))
    else
      val calls = Suit.values.toSeq.filter(_ != turnedUp).flatMap(withAndAlone).map(Some(_))
      if seat == dealer then calls else None +: calls

  /** @return each seat that passed before this decision, with the round it passed in */
  def earlierPasses: Seq[(Seat, Int)] =
    val order = (1 to Seat.values.size).map(dealer.offset)
    val before = order.takeWhile(_ != seat)
    if round == 1 then before.map(_ -> 1) else order.map(_ -> 1) ++ before.map(_ -> 2)

  /** @return the bids each seat is held to: the earlier passes, and `bid` for `seat` */
  def forced(bid: Option[Call]): Map[Seat, Map[Int, Option[Call]]] =
    val passes = earlierPasses.groupMap(_._1)((_, round) => round -> Option.empty[Call]).view.mapValues(_.toMap).toMap
    passes.updated(seat, passes.getOrElse(seat, Map.empty).updated(round, bid))

object Decision:
  def of(view: BidView, round: Int): Decision = Decision(view.seat, view.dealer, view.hand, view.upCard, round)

/** Bids `bids(round)` in the rounds it holds, and like `others` otherwise. */
final case class Forced(bids: Map[Int, Option[Call]], others: Bidder) extends Bidder:
  def orderUp(view: BidView): Option[Call] = bids.getOrElse(1, others.orderUp(view))
  def callTrump(view: BidView): Option[Call] = bids.getOrElse(2, others.callTrump(view))
  def stickTrump(view: BidView): Call = bids.get(2).flatten.getOrElse(others.stickTrump(view))
  def discard(view: BidView, trump: Suit): Card = others.discard(view, trump)

/** The expected points to the bidder's team of bidding `bid`, over the sampled deals. */
final case class BidValue(bid: Option[Call], mean: Double, stdErr: Double)

object SimulatedCalls:
  private val maxAttempts = 10_000

  /** @return whether every seat but the decision's own would have passed as it did, bidding like `others` */
  def passes(decision: Decision, hands: Map[Seat, Cards], others: Bidder, rng: Random): Boolean =
    decision.earlierPasses.forall((seat, round) =>
      seat == decision.seat || {
        val view = BidView(seat, decision.dealer, hands(seat), decision.upCard, rng)
        if round == 1 then others.orderUp(view).isEmpty else others.callTrump(view).isEmpty
      }
    )

  /** Deals the unseen cards at random until the earlier passes are what `others` would have bid. If no such deal turns
    * up in `maxAttempts`, any deal is used: the earlier seats are still held to their passes in play.
    *
    * @return each seat's hand, with the decision's own
    */
  def deal(decision: Decision, others: Bidder, rng: Random): Map[Seat, Cards] =
    val unseen = Card.all.filterNot(card => decision.hand.contains(card) || card == decision.upCard)
    val seats = Seat.values.filter(_ != decision.seat)
    def attempt() = seats.zip(rng.shuffle(unseen).grouped(Game.handSize).map(Cards.from)).toMap +
      (decision.seat -> decision.hand)
    Iterator.continually(attempt()).take(maxAttempts).find(passes(decision, _, others, rng)).getOrElse(attempt())

  /** Plays out the deal with `seat` bidding `bid`, the earlier seats passing, and every other bid like `others`. */
  def play(
      decision: Decision,
      bid: Option[Call],
      hands: Map[Seat, Cards],
      others: Bidder,
      player: Player,
      seed: Long
  ): HandResult =
    val forced = decision.forced(bid)
    val strategies = Seat.values
      .map(seat => seat -> Composite(s"$seat", Forced(forced.getOrElse(seat, Map.empty), others), player))
      .toMap
    Game(strategies, Random(seed)).playHand(decision.dealer, Game.deck(decision.dealer, hands, decision.upCard))

  def points(hand: HandResult, team: Team): Int = if hand.scoringTeam == team then hand.points else -hand.points

  /** Plays every option on the same `samples` deals, so the differences between options have less noise. */
  def evaluate(decision: Decision, samples: Int, others: Bidder, player: Player, rng: Random): Seq[BidValue] =
    val deals = Seq.fill(samples)(deal(decision, others, rng) -> rng.nextLong())
    decision.options.par
      .map(bid =>
        val scores = deals.map((hands, seed) =>
          points(play(decision, bid, hands, others, player, seed), decision.seat.team).toDouble
        )
        val mean = scores.sum / samples
        val variance =
          if samples < 2 then 0 else scores.map(score => (score - mean) * (score - mean)).sum / (samples - 1)
        BidValue(bid, mean, math.sqrt(variance / samples))
      )
      .seq

  /** @return the decisions of a deal bid with `bidder`, in order, each with the bid `bidder` made */
  def decisions(dealer: Seat, dealt: Map[Seat, Cards], upCard: Card, bidder: Bidder): Seq[(Decision, Option[Call])] =
    val order = (1 to Seat.values.size).map(dealer.offset)
    val all = Seq(1, 2).flatMap(round => order.map(seat => Decision(seat, dealer, dealt(seat), upCard, round)))
    val bids = all.map(decision =>
      val view = BidView(decision.seat, dealer, decision.hand, upCard, Random(0))
      decision -> (
        if decision.round == 1 then bidder.orderUp(view)
        else if decision.seat == dealer then Some(bidder.stickTrump(view))
        else bidder.callTrump(view)
      )
    )
    val (passed, rest) = bids.span(_._2.isEmpty)
    passed ++ rest.take(1)

  /** @return `deals` random deals' decisions, bid with `bidder` */
  def sampleDecisions(deals: Int, bidder: Bidder, rng: Random): Seq[(Decision, Option[Call])] =
    Seq.fill(deals) {
      val dealer = Seat.fromOrdinal(rng.nextInt(Seat.values.size))
      val deck = rng.shuffle(Card.all)
      val order = (1 to Seat.values.size).map(dealer.offset)
      val dealt = order.zip(deck.grouped(Game.handSize).map(Cards.from)).toMap
      decisions(dealer, dealt, deck(Seat.values.size * Game.handSize), bidder)
    }.flatten

  def label(bid: Option[Call]): String =
    bid.fold("pass")(call => s"${call.trump.symbol}${if call.alone then " alone" else ""}")

  enum Kind:
    case Pass, Partner, Alone

  def kind(bid: Option[Call]): Kind = bid.fold(Kind.Pass)(call => if call.alone then Kind.Alone else Kind.Partner)

  /** @return how often `chosen` picks the reference's best bid, and its mean regret in the reference's points */
  def compare(reference: Seq[Seq[BidValue]], chosen: Seq[Option[Call]]): (Double, Double) =
    val regrets =
      reference.zip(chosen).map((values, bid) => values.map(_.mean).max - values.find(_.bid == bid).get.mean)
    val agree = reference.zip(chosen).count((values, bid) => values.maxBy(_.mean).bid == bid)
    (agree.toDouble / chosen.size, regrets.sum / chosen.size)

  def best(values: Seq[BidValue]): Option[Call] = values.maxBy(_.mean).bid

/** Writes the simulated value of every bid open at the decisions of `deals` deals bid with `RuleBidder.tuned`, as CSV:
  * `runOracleData [deals=1000] [samples=200] [seed=1] [out=oracle.csv]`.
  */
@main
def runOracleData(args: String*): Unit =
  val deals = args.lift(0).fold(1000)(_.toInt)
  val samples = args.lift(1).fold(200)(_.toInt)
  val seed = args.lift(2).fold(1L)(_.toLong)
  val out = PrintWriter(args.lift(3).getOrElse("oracle.csv"), "UTF-8")
  val rng = Random(seed)
  val decisions = SimulatedCalls.sampleDecisions(deals, RuleBidder.tuned, rng)
  out.println("decision,round,position,hand,hand_bits,up_card,bid,mean,std_err,rule_bid")
  decisions.zipWithIndex.foreach { case ((decision, rule), i) =>
    val values = SimulatedCalls.evaluate(decision, samples, RuleBidder.tuned, PartnerHighLow, rng)
    values.foreach(value =>
      out.println(
        Seq(
          i,
          decision.round,
          decision.position,
          decision.hand.mkString(" "),
          decision.hand.bits,
          decision.upCard,
          SimulatedCalls.label(value.bid),
          f"${value.mean}%.4f",
          f"${value.stdErr}%.4f",
          value.bid == rule,
        ).mkString(",")
      )
    )
    out.flush()
  }
  out.close()

/** Reports how many samples the oracle needs, how much its bids depend on the later bidders, and how the rule bidder's
  * bids compare: `runSimulatedCalls [deals=200] [reference=2000] [seed=1]`.
  */
@main
def runSimulatedCalls(args: String*): Unit =
  import SimulatedCalls.*
  val deals = args.lift(0).fold(200)(_.toInt)
  val referenceSamples = args.lift(1).fold(2000)(_.toInt)
  val seed = args.lift(2).fold(1L)(_.toLong)
  val decisions = sampleDecisions(deals, RuleBidder.tuned, Random(seed))
  def evaluateAll(samples: Int, others: Bidder, seed: Long) =
    val rng = Random(seed)
    decisions.map((decision, _) => evaluate(decision, samples, others, PartnerHighLow, rng))
  val reference = evaluateAll(referenceSamples, RuleBidder.tuned, seed + 1)
  def row(cells: Any*) = println(cells.mkString("| ", " | ", " |"))
  def percent(rate: Double) = f"${100 * rate}%.1f%%"

  println(s"${decisions.size} decisions from $deals deals, against $referenceSamples samples\n")
  row("samples", "picks the reference's best bid", "regret per decision")
  row("---:", "---:", "---:")
  Seq(25, 50, 100, 200, 400).foreach(samples =>
    val (agree, regret) = compare(reference, evaluateAll(samples, RuleBidder.tuned, seed + 2).map(best))
    row(samples, percent(agree), f"$regret%.4f")
  )

  println()
  row("bidder", "picks the reference's best bid", "regret per decision")
  row("---", "---:", "---:")
  val stuckLater = evaluateAll(400, StuckDealer, seed + 3).map(best)
  val (stuckAgree, stuckRegret) = compare(reference, stuckLater)
  row("oracle, if the later bidders only pass", percent(stuckAgree), f"$stuckRegret%.4f")
  val (ruleAgree, ruleRegret) = compare(reference, decisions.map(_._2))
  row("RuleBidder.tuned", percent(ruleAgree), f"$ruleRegret%.4f")

  println()
  row("RuleBidder bids \\ oracle's best" +: Kind.values.map(_.toString)*)
  row("---" +: Kind.values.map(_ => "---:")*)
  Kind.values.foreach(ruleKind =>
    val oracleKinds = decisions.zip(reference).collect {
      case ((_, rule), values) if kind(rule) == ruleKind => kind(best(values))
    }
    row(ruleKind +: Kind.values.map(oracleKind => oracleKinds.count(_ == oracleKind))*)
  )

/** Plays `OracleBidder` against `RuleBidder.tuned`, both with PartnerHighLow:
  * `runOracleMatch [games=200] [samples=100] [seed=1]`.
  */
@main
def runOracleMatch(args: String*): Unit =
  val games = args.lift(0).fold(200)(_.toInt)
  val samples = args.lift(1).fold(100)(_.toInt)
  val seed = args.lift(2).fold(1L)(_.toLong)
  val rate = RuleSweep.winRate(OracleBidder(samples), RuleBidder.tuned, PartnerHighLow, games, seed)
  println(f"OracleBidder($samples) vs RuleBidder.tuned: ${100 * rate}%.1f%% of ${2 * games} games")
