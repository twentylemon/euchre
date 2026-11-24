package org.lemon.euchre

import org.lemon.euchre.Generators.{*, given}
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

class TrickTest extends UnitTest:

  private val fullTrick = trick(1, 4)

  test("the winning card follows suit or is trump") {
    check(forAll(fullTrick)(trick =>
      val winning = trick.winningCard
      winning.isTrump(trick.trump) || winning.effectiveSuit(trick.trump) == trick.leadSuit
    ))
  }

  test("if any trump is played, the winning card is trump") {
    check(forAll(fullTrick)(trick =>
      !trick.cards.exists(_.isTrump(trick.trump)) || trick.winningCard.isTrump(trick.trump)
    ))
  }

  test("the winning card outscores every card that follows suit") {
    check(forAll(fullTrick)(trick =>
      val winningScore = Ranking.score(trick.trump, trick.winningCard)
      trick.cards.filter(trick.follows).forall(Ranking.score(trick.trump, _) <= winningScore)
    ))
  }

  test("winner is the seat that played the winning card") {
    check(forAll(fullTrick)(trick => trick.cardOf(trick.winner).contains(trick.winningCard)))
  }

  test("playable cards are the following cards, or the whole hand") {
    check(forAll(trick(0, 3), arbitrary[Cards]) { (trick, hand) =>
      val playable = trick.playable(hand)
      val following = hand.filter(trick.follows)
      playable.subsetOf(hand) && (if following.isEmpty then playable == hand else playable == following)
    })
  }

  test("every card is playable on a lead") {
    check((trump: Suit, hand: Cards) => Trick(trump).playable(hand) == hand)
  }

  test("the left bower follows trump, not its printed suit") {
    val trick = Trick(Suit.Hearts) :+ (Seat.Down -> parse("9♦"))
    trick.follows(parse("J♦")) shouldBe false
    (Trick(Suit.Hearts) :+ (Seat.Down -> parse("9♥"))).follows(parse("J♦")) shouldBe true
  }

  test("an off-suit ace loses to the lead suit") {
    val trick = Trick(Suit.Spades) :+ (Seat.Down -> parse("9♥")) :+ (Seat.Left -> parse("A♦"))
    trick.winner shouldBe Seat.Down
  }

  test("the left bower beats the ace of trump") {
    val trick = Trick(Suit.Spades) :+ (Seat.Down -> parse("A♠")) :+ (Seat.Left -> parse("J♣"))
    trick.winner shouldBe Seat.Left
  }
