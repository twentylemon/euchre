package org.lemon.euchre

import org.lemon.euchre.Generators.given
import org.scalacheck.*

class CardTest extends UnitTest:

  test("fromOrdinal inverts ordinal") {
    check((card: Card) => Card.fromOrdinal(card.ordinal) == card)
  }

  test("all holds 24 distinct ordinals") {
    Card.all.map(_.ordinal) shouldBe (0 until 24)
  }

  test("only the left bower changes suit") {
    check((card: Card, trump: Suit) => (card.effectiveSuit(trump) != card.suit) == card.isLeftBower(trump))
  }

  test("the left bower is trump") {
    check((trump: Suit) => Card(Rank.Jack, trump.sameColour).effectiveSuit(trump) == trump)
  }

  test("sameColour is a different suit and an involution") {
    check((suit: Suit) => suit.sameColour != suit && suit.sameColour.sameColour == suit)
  }

  test("toString is rank then suit") {
    Card(Rank.Ace, Suit.Hearts).toString shouldBe "A♥"
    Card(Rank.Ten, Suit.Clubs).toString shouldBe "T♣"
  }
