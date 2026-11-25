package org.lemon.euchre

import org.lemon.euchre.strategies.*

import scala.collection.parallel.CollectionConverters.*

object RuleSweep:
  /** A starting point from the bidding baseline, where three trumps (about 1.9 strength) were worth calling. The
    * dealer's partner can only order up alone, so they start at the alone threshold.
    */
  val initial: RuleBidder = RuleBidder(
    HandStrength(offAce = 0.5, void = 0.2),
    orderUp = Seq(2.25, 4.0, 2.25, 2.0),
    call = Seq(2.0, 2.0, 2.0),
    next = 0.25,
    alone = 4.0,
  )

  final case class Param(
      name: String,
      get: RuleBidder => Double,
      set: (RuleBidder, Double) => RuleBidder,
      grid: Seq[Double]
  )

  private def steps(from: Double, to: Double, step: Double) = BigDecimal(from).to(to, step).map(_.toDouble)
  private val thresholds = steps(1.0, 3.5, 0.25)
  private val aloneThresholds = steps(2.5, 5.0, 0.25) :+ Double.PositiveInfinity

  val params: Seq[Param] =
    (0 until 4).map(i =>
      val grid = if i == 1 then aloneThresholds else thresholds
      Param(s"orderUp(${i + 1})", _.orderUp(i), (b, v) => b.copy(orderUp = b.orderUp.updated(i, v)), grid)
    ) ++ (0 until 3).map(i =>
      Param(s"call(${i + 1})", _.call(i), (b, v) => b.copy(call = b.call.updated(i, v)), thresholds)
    ) ++ Seq(
      Param("next", _.next, (b, v) => b.copy(next = v), steps(-0.25, 0.75, 0.25)),
      Param("alone", _.alone, (b, v) => b.copy(alone = v), aloneThresholds),
      Param("offAce", _.strength.offAce, (b, v) => b.copy(strength = b.strength.copy(offAce = v)), steps(0, 0.8, 0.1)),
      Param("void", _.strength.void, (b, v) => b.copy(strength = b.strength.copy(void = v)), steps(0, 0.5, 0.1)),
    )

  /** @return `candidate`'s game win rate against `incumbent`, with each seeded deal played from both sides */
  def winRate(candidate: Bidder, incumbent: Bidder, player: Player, games: Int, seed: Long): Double =
    val challenger = Composite("candidate", candidate, player)
    val defender = Composite("incumbent", incumbent, player)
    val asUpDown = Tournament.run(Matchup(challenger, defender, games), seed).gameWins(Team.UpDown)
    val asLeftRight = Tournament.run(Matchup(defender, challenger, games), seed).gameWins(Team.LeftRight)
    (asUpDown + asLeftRight).toDouble / (2 * games)

  /** Coordinate ascent: each step moves one parameter to the grid value that beats the current bidder most, if its win
    * rate is over one half by more than `margin`. The margin keeps the sweep from chasing noise.
    */
  def sweep(
      start: RuleBidder,
      player: Player,
      games: Int,
      passes: Int,
      seed: Long,
      margin: Double = 0.01,
      params: Seq[Param] = params,
  )(
      log: String => Unit
  ): RuleBidder =
    (1 to passes).foldLeft(start)((incumbent, pass) =>
      params.foldLeft(incumbent)((best, param) =>
        val results =
          param.grid.par.map(value => value -> winRate(param.set(best, value), best, player, games, seed + pass)).seq
        val (value, rate) = results.maxBy(_._2)
        val moved = if rate > 0.5 + margin then param.set(best, value) else best
        log(f"pass $pass ${param.name}%-10s ${param.get(best)}%5.2f -> ${param.get(moved)}%5.2f ($rate%.4f)")
        moved
      )
    )

/** Tunes `RuleBidder` in self-play with `player`:
  * `runRuleSweep [games=4000] [passes=2] [seed=1] [margin=0.01] [player=PartnerHighLow]`.
  */
@main
def runRuleSweep(args: String*): Unit =
  val games = args.lift(0).fold(4000)(_.toInt)
  val passes = args.lift(1).fold(2)(_.toInt)
  val seed = args.lift(2).fold(1L)(_.toLong)
  val margin = args.lift(3).fold(0.01)(_.toDouble)
  val player = Strategies.named(args.lift(4).getOrElse(PartnerHighLow.name))
  val tuned = RuleSweep.sweep(RuleSweep.initial, player, games, passes, seed, margin)(println)
  println(tuned)

/** Plays `RuleBidder.tuned` against the stuck dealer with each trick-play strategy: `runRuleCallers [games=10000] [seed=1]`. */
@main
def runRuleCallers(args: String*): Unit =
  val games = args.lift(0).fold(10000)(_.toInt)
  val seed = args.lift(1).fold(1L)(_.toLong)
  val improvement = RuleSweep.winRate(RuleBidder.tuned, RuleSweep.initial, PartnerHighLow, games, seed)
  println(f"tuned vs initial with PartnerHighLow: ${100 * improvement}%.1f%% of games\n")
  println("| trick play | rule bidder's game win % vs stuck dealer |")
  println("| --- | ---: |")
  Strategies.all.par
    .map(player => player -> RuleSweep.winRate(RuleBidder.tuned, StuckDealer, player, games, seed))
    .seq
    .foreach((player, rate) => println(f"| ${player.name} | ${100 * rate}%.1f%% |"))
