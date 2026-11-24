package org.lemon.euchre.strategies

import org.lemon.euchre.*

object Strategies:
  /** The paper's strategies from simplest to most complex, without the slow `MonteCarlo`. */
  val all: Seq[Strategy] = Seq(
    RandomStrategy,
    Low,
    High,
    HighLow,
    PartnerHighLow,
    Markov(),
    Markov(partial = true),
    CardCounting,
    Hybrid(),
    Hybrid(partial = true),
  )

  def named(name: String): Strategy =
    (all :+ MonteCarlo()).find(_.name == name).getOrElse(
      throw IllegalArgumentException(s"unknown strategy $name, expected one of ${all.map(_.name).mkString(", ")}")
    )

/** Plays like `player`, passes, and names `trump` when stuck. */
final case class StuckWith(player: Strategy, trump: Suit) extends Strategy, StuckDealer:
  val name: String = player.name
  override def stickTrump(view: BidView): Call = Call(trump)
  def play(view: PlayView): Card = player.play(view)
