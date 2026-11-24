package org.lemon.euchre

import org.lemon.euchre.Generators.{*, given}
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

class RankingTest extends UnitTest:

  test("trump cards outscore every other card") {
    check((trump: Suit, card: Card) => card.isTrump(trump) == (Ranking.score(trump, card) >= Ranking.minTrumpScore))
  }

  test("cards of the same effective suit have distinct scores") {
    check((trump: Suit, suit: Suit) =>
      val scores = Card.all.filter(_.effectiveSuit(trump) == suit).map(Ranking.score(trump, _))
      scores.distinct.size == scores.size
    )
  }

  test("right bower > left bower > ace > king > queen > ten > nine of trump") {
    check((trump: Suit) =>
      val order = Seq(
        Card(Rank.Jack, trump),
        Card(Rank.Jack, trump.sameColour),
        Card(Rank.Ace, trump),
        Card(Rank.King, trump),
        Card(Rank.Queen, trump),
        Card(Rank.Ten, trump),
        Card(Rank.Nine, trump),
      ).map(Ranking.score(trump, _))
      order == order.sorted.reverse
    )
  }

  test("best and worst are the extreme scores") {
    check(forAll(arbitrary[Suit], nonEmptyCards)((trump, cards) =>
      val scores = cards.iterator.map(Ranking.score(trump, _)).toSeq
      Ranking.score(trump, Ranking.best(cards, trump)) == scores.max &&
      Ranking.score(trump, Ranking.worst(cards, trump)) == scores.min
    ))
  }

  private val numTricks = 24 * 23 * 22 * 21

  test("every trick has one winner") {
    for trump <- Suit.values do Card.all.map(Ranking.trickWins(trump, _)).sum shouldBe numTricks
  }

  test("the right bower wins every trick it's in") {
    check((trump: Suit) => Ranking.trickWins(trump, Card(Rank.Jack, trump)) == 4 * 23 * 22 * 21)
  }

  test("partialWins of the last card is whether it wins") {
    check(forAll(trickAndRest(3, 3)) { (trick, rest) =>
      rest.forall(card =>
        val wins = (trick :+ (trick.plays.last._1.next -> card)).winningCard == card
        Ranking.partialWins(trick, card) == (if wins then 1 else 0)
      )
    })
  }

  test("partialWins of a lead is its wins as the first card") {
    check((trump: Suit, card: Card) =>
      val completions = 23 * 22 * 21
      val led = Ranking.partialWins(Trick(trump), card)
      led <= Ranking.trickWins(trump, card) && led <= completions &&
      (!card.isRightBower(trump) || led == completions)
    )
  }
