package org.lemon.euchre

import org.lemon.euchre.Generators.{*, given}
import org.lemon.euchre.strategies.*
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

class KnowledgeTest extends UnitTest:

  /** @return each view with the hand that every other seat held at that moment */
  private def withHands(views: Seq[PlayView]): Seq[(PlayView, Map[Seat, Cards])] =
    views.zipWithIndex.map((view, i) =>
      view -> views.drop(i + 1).filter(_.seat != view.seat).groupBy(_.seat).view.mapValues(_.head.hand).toMap
    )

  test("knowledge never rules out a card a seat holds") {
    check(forAll(arbitrary[Long], arbitrary[Seat]) { (seed, dealer) =>
      val (_, views) = spyHand(RandomStrategy, seed, dealer)
      withHands(views).forall((view, hands) =>
        hands.forall((seat, hand) => hand.subsetOf(view.knowledge.possible(seat)))
      )
    })
  }

  test("seen holds the own hand and every card played") {
    check(forAll(arbitrary[Long], arbitrary[Seat]) { (seed, dealer) =>
      val (_, views) = spyHand(RandomStrategy, seed, dealer)
      val dealt = Cards.from(views.groupBy(_.seat).values.flatMap(_.head.hand.toSeq))
      withHands(views).forall((view, hands) =>
        val held = hands.values.foldLeft(view.hand)(_ ++ _)
        (dealt -- held).subsetOf(view.knowledge.seen) && view.hand.subsetOf(view.knowledge.seen)
      )
    })
  }

  test("possible cards are unseen, apart from the dealer's up card") {
    check(forAll(arbitrary[Long], arbitrary[Seat]) { (seed, dealer) =>
      val (_, views) = spyHand(RandomStrategy, seed, dealer)
      views.forall(view =>
        Seat.values.filter(_ != view.seat).forall(seat =>
          (view.knowledge.possible(seat) & view.knowledge.seen).size <= 1
        )
      )
    })
  }

  test("failing to follow suit rules out the lead suit") {
    val trick = Trick(Suit.Spades) :+ (Seat.Down -> parse("9♥"))
    val start = Knowledge.start(Seat.Up, cards("9♠ T♠ Q♠ K♠ A♠"), parse("9♦"), None, None)
    val after = start.observe(Seat.Left, parse("9♣"), trick)
    after.possible(Seat.Left).iterator.exists(_.suit == Suit.Hearts) shouldBe false
    after.possible(Seat.Right).contains(parse("A♥")) shouldBe true
    after.seen.contains(parse("9♣")) shouldBe true
  }
