package org.lemon.euchre

import org.lemon.euchre.Generators.given
import org.scalacheck.*

class CardsTest extends UnitTest:

  test("from inverts toSeq") {
    check((cards: Cards) => Cards.from(cards.toSeq) == cards)
  }

  test("size counts the iterated cards") {
    check((cards: Cards) => cards.size == cards.iterator.size)
  }

  test("iterator is in ordinal order without duplicates") {
    check((cards: Cards) => cards.toSeq.map(_.ordinal) == cards.toSeq.map(_.ordinal).distinct.sorted)
  }

  test("+ adds the card and - removes it") {
    check((cards: Cards, card: Card) => (cards + card).contains(card) && !(cards - card).contains(card))
  }

  test("+ and - leave every other card alone") {
    check((cards: Cards, card: Card) =>
      Card.all.filter(_ != card).forall(other =>
        (cards + card).contains(other) == cards.contains(other) &&
          (cards - card).contains(other) == cards.contains(other)
      )
    )
  }

  test("complement partitions all cards") {
    check((cards: Cards) => (cards & cards.complement).isEmpty && (cards ++ cards.complement) == Cards.all)
  }

  test("filter agrees with the iterator") {
    check((cards: Cards, suit: Suit) => cards.filter(_.suit == suit).toSeq == cards.toSeq.filter(_.suit == suit))
  }

  test("subsetOf agrees with contains") {
    check((a: Cards, b: Cards) => a.subsetOf(b) == a.iterator.forall(b.contains))
  }

  test("-- removes exactly the other set") {
    check((a: Cards, b: Cards) => (a -- b).toSeq == a.toSeq.filterNot(b.contains))
  }
