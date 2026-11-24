package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Plays a random playable card. */
object RandomStrategy extends Strategy, StuckDealer:
  val name = "Random"

  def play(view: PlayView): Card =
    val cards = view.playable.toSeq
    cards(view.rng.nextInt(cards.size))
