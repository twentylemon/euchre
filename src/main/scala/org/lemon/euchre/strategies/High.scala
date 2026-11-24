package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Plays the highest playable card. */
object High extends Strategy, StuckDealer:
  val name = "High"

  def play(view: PlayView): Card = Ranking.best(view.playable, view.trump)
