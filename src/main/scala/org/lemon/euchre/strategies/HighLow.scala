package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Leads the highest card. Otherwise plays the highest playable card if it outscores the winning card, else the lowest.
  *
  * The comparison is on raw scores, so an off-suit card can outscore a lead-suit winner without beating it.
  */
object HighLow extends Strategy, StuckDealer:
  val name = "HighLow"

  def play(view: PlayView): Card =
    val trump = view.trump
    val best = Ranking.best(view.playable, trump)
    if view.trick.isEmpty || Ranking.score(trump, view.trick.winningCard) < Ranking.score(trump, best) then best
    else Ranking.worst(view.playable, trump)
