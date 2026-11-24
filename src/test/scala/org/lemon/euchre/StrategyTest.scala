package org.lemon.euchre

import org.lemon.euchre.Generators.{*, given}
import org.lemon.euchre.strategies.*
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

class StrategyTest extends UnitTest:

  for player <- allPlayers do
    test(s"${player.getClass.getSimpleName} $player plays a playable card in any hand") {
      check(
        forAll(arbitrary[Long], arbitrary[Seat]) { (seed, dealer) =>
          val (_, views) = spyHand(player, seed, dealer)
          views.forall(view => view.playable.contains(player.play(view)))
        },
        minSuccessful(200)
      )
    }

  test("MonteCarlo plays a playable card in any hand") {
    check(
      forAll(arbitrary[Long], arbitrary[Seat]) { (seed, dealer) =>
        val player = MonteCarlo(samples = 2)
        val (_, views) = spyHand(player, seed, dealer)
        views.forall(view => view.playable.contains(player.play(view)))
      },
      minSuccessful(20)
    )
  }

  test("MonteCarlo deals the unseen cards consistently") {
    check(
      forAll(arbitrary[Long], arbitrary[Seat]) { (seed, dealer) =>
        val (_, views) = spyHand(RandomStrategy, seed, dealer)
        views.filter(_.sittingOut.isEmpty).forall(view =>
          MonteCarlo.deal(view).forall(hands =>
            val others = Seat.values.filter(_ != view.seat)
            hands(view.seat.ordinal) == view.hand &&
            others.forall(seat => hands(seat.ordinal).subsetOf(view.knowledge.possible(seat))) &&
            others.forall(seat =>
              hands(seat.ordinal).size == view.hand.size - (if view.trick.cardOf(seat).isDefined then 1 else 0)
            ) &&
            Cards.from(hands.flatMap(_.toSeq)).size == hands.map(_.size).sum
          )
        )
      },
      minSuccessful(200)
    )
  }

  test("PartnerHighLow plays low when partner is winning") {
    val trick = Trick(Suit.Spades) :+ (Seat.Down -> parse("A♥")) :+ (Seat.Left -> parse("9♥"))
    val hand = cards("K♥ T♥ 9♠")
    val knowledge = Knowledge.start(Seat.Up, hand, parse("9♦"), None, None)
    val view = PlayView(Seat.Up, hand, trick, Seat.Right, false, knowledge, scala.util.Random(0))
    PartnerHighLow.play(view) shouldBe parse("T♥")
    HighLow.play(view.copy(seat = Seat.Right, trick = trick :+ (Seat.Up -> parse("Q♥")))) shouldBe parse("T♥")
  }
