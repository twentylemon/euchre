package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Plays `PartnerHighLow` when `Markov`'s hand share is at least `threshold`, otherwise `CardCounting`.
  *
  * @param partial whether the hand share uses trick wins given the trick so far (Hybrid2)
  */
final case class Hybrid(threshold: Double = Markov.defaultThreshold, partial: Boolean = false)
    extends Strategy, StuckDealer:
  val name: String = if partial then "Hybrid2" else "Hybrid"

  def play(view: PlayView): Card =
    if Markov.handShare(view, partial) >= threshold then PartnerHighLow.play(view)
    else CardCounting.play(view)
