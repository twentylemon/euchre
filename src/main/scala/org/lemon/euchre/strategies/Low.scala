package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Plays the lowest playable card. */
object Low extends Strategy, StuckDealer:
  val name = "Low"

  def play(view: PlayView): Card = Ranking.worst(view.playable, view.trump)
