package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Compares the trick wins of the hand with those of every unseen card. If the hand's share is at least `threshold`,
  * plays the highest playable card, otherwise the lowest. `threshold` 0 always plays high and 1 always plays low.
  *
  * @param partial whether to count trick wins given the trick so far (Markov2) instead of over all tricks
  */
final case class Markov(threshold: Double = Markov.defaultThreshold, partial: Boolean = false)
    extends Strategy, StuckDealer:
  val name: String = if partial then "Markov2" else "Markov"

  def play(view: PlayView): Card =
    if Markov.handShare(view, partial) >= threshold then Ranking.best(view.playable, view.trump)
    else Ranking.worst(view.playable, view.trump)

object Markov:
  val defaultThreshold = 0.25

  /** @return the trick wins of the hand over the trick wins of every unseen card, NaN or infinite if none are unseen */
  def handShare(view: PlayView, partial: Boolean): Double =
    def wins(card: Card) =
      if partial then Ranking.partialWins(view.trick, card) else Ranking.trickWins(view.trump, card)
    view.hand.sumBy(wins).toDouble / view.knowledge.unseen.sumBy(wins)
