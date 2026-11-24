package org.lemon.euchre.strategies

import org.lemon.euchre.*

/** Plays low if partner already played and is winning the trick, otherwise plays `HighLow`. The paper's CoopHighLow. */
object PartnerHighLow extends Strategy, StuckDealer:
  val name = "PartnerHighLow"

  def play(view: PlayView): Card =
    val partnerWinning = view.trick.cardOf(view.partner).isDefined && view.trick.winner == view.partner
    if partnerWinning then Ranking.worst(view.playable, view.trump)
    else HighLow.play(view)
