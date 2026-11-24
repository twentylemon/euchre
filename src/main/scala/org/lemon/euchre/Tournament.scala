package org.lemon.euchre

import org.lemon.euchre.strategies.*

import scala.collection.parallel.CollectionConverters.*
import scala.util.Random

final case class Matchup(upDown: Strategy, leftRight: Strategy, games: Int):
  def strategy(seat: Seat): Strategy = if seat.team == Team.UpDown then upDown else leftRight

final case class MatchupResult(matchup: Matchup, games: Seq[GameResult]):
  def gameWins(team: Team): Int = games.count(_.winner == team)
  def trickWins(team: Team): Int = games.map(_.tricks(team)).sum
  def totalScore(team: Team): Int = games.map(_.score(team)).sum
  def playMillis(seat: Seat): Long = games.map(_.playNanos(seat)).sum / 1_000_000

  override def toString: String =
    val seats = Seq(Seat.Up, Seat.Down, Seat.Left, Seat.Right)
      .map(seat => s"${seat.toString.toLowerCase}\t(${matchup.strategy(seat).name})\t${playMillis(seat)}")
    def teams(f: Team => Int) = s"( UP_TEAM => ${f(Team.UpDown)}, LEFT_TEAM => ${f(Team.LeftRight)})"
    (Seq("overall results", "time taken per player (ms)") ++ seats ++ Seq(
      s"gameWins${teams(gameWins)}",
      s"trickWins${teams(trickWins)}",
      s"totalScores${teams(totalScore)}",
    )).mkString("\n")

object Tournament:
  /** The matchups from the paper, each strategy against every simpler one. */
  def matchups(games: Int, monteCarloGames: Int): Seq[Matchup] =
    val ladder = Strategies.all
    val simple = Seq(
      Matchup(RandomStrategy, Low, games),
      Matchup(RandomStrategy, High, games),
      Matchup(Low, High, games),
    )
    val rest = ladder.drop(3).zipWithIndex.flatMap((strategy, i) => ladder.take(i + 3).map(Matchup(strategy, _, games)))
    simple ++ rest :+ Matchup(MonteCarlo(), PartnerHighLow, monteCarloGames)

  def run(matchup: Matchup, seed: Long): MatchupResult =
    val game = Game(Seat.values.map(seat => seat -> matchup.strategy(seat)).toMap, Random(seed))
    MatchupResult(matchup, Seq.fill(matchup.games)(game.playGame()))

/** Runs the paper's tournament: `runTournament [games=10001] [seed=1] [monteCarloGames=5]`. */
@main
def runTournament(args: String*): Unit =
  val games = args.lift(0).fold(10001)(_.toInt)
  val seed = args.lift(1).fold(1L)(_.toLong)
  val monteCarloGames = args.lift(2).fold(5)(_.toInt)
  Tournament
    .matchups(games, monteCarloGames)
    .zipWithIndex
    .par
    .map((matchup, i) => Tournament.run(matchup, seed + i))
    .seq
    .foreach(result => println(s"$result\n\n"))
