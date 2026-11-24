package org.lemon.euchre

import org.lemon.euchre.strategies.*
import org.scalacheck.*
import org.scalacheck.Arbitrary.*

import scala.collection.mutable
import scala.util.Random

object Generators:
  given Arbitrary[Suit] = Arbitrary(Gen.oneOf(Suit.values.toSeq))
  given Arbitrary[Rank] = Arbitrary(Gen.oneOf(Rank.values.toSeq))
  given Arbitrary[Seat] = Arbitrary(Gen.oneOf(Seat.values.toSeq))
  given Arbitrary[Card] = Arbitrary(Gen.oneOf(Card.all))
  given Arbitrary[Cards] = Arbitrary(Gen.choose(0, Cards.all.bits).map(Cards.fromBits))

  val deck: Gen[Seq[Card]] = arbitrary[Long].map(seed => Random(seed).shuffle(Card.all))

  /** @return a trick with between `minSize` and `maxSize` cards, and the cards not in it */
  def trickAndRest(minSize: Int, maxSize: Int): Gen[(Trick, Seq[Card])] =
    for
      trump <- arbitrary[Suit]
      leader <- arbitrary[Seat]
      size <- Gen.choose(minSize, maxSize)
      cards <- deck
    yield
      val trick = cards.take(size).zipWithIndex.foldLeft(Trick(trump))((trick, play) =>
        trick :+ (leader.offset(play._2) -> play._1)
      )
      (trick, cards.drop(size))

  def trick(minSize: Int, maxSize: Int): Gen[Trick] = trickAndRest(minSize, maxSize).map(_._1)

  val nonEmptyCards: Gen[Cards] = Gen.choose(1, Cards.all.bits).map(Cards.fromBits)

  def parse(card: String): Card = Card.all.find(_.toString == card).get
  def cards(cards: String): Cards = Cards.from(cards.split(" ").map(parse))

  /** Orders up, calls and goes alone at random, so games reach every bidding branch. */
  trait RandomBidder extends Bidder:
    def orderUp(view: BidView): Option[Call] =
      Option.when(view.rng.nextInt(4) == 0)(Call(view.upCard.suit, alone = view.rng.nextBoolean()))
    def callTrump(view: BidView): Option[Call] =
      Option.when(view.rng.nextInt(3) == 0)(stickTrump(view))
    def stickTrump(view: BidView): Call =
      val suits = Suit.values.filter(_ != view.upCard.suit)
      Call(suits(view.rng.nextInt(suits.size)), alone = view.rng.nextInt(4) == 0)
    def discard(view: BidView, trump: Suit): Card =
      val cards = view.hand.toSeq
      cards(view.rng.nextInt(cards.size))

  /** Plays like `player`, bids at random and records every `PlayView` it sees. */
  final class Spy(player: Player, views: mutable.Buffer[PlayView]) extends Strategy, RandomBidder:
    val name = "Spy"
    def play(view: PlayView): Card =
      views += view
      player.play(view)

  /** Plays one hand with every seat spying on `player`.
    * @return the result and every `PlayView`, in play order
    */
  def spyHand(player: Player, seed: Long, dealer: Seat): (HandResult, Seq[PlayView]) =
    val views = mutable.Buffer.empty[PlayView]
    val spy = Spy(player, views)
    val result = Game(Seat.values.map(_ -> spy).toMap, Random(seed)).playHand(dealer, spyDeck(seed))
    (result, views.toSeq)

  def spyDeck(seed: Long): Seq[Card] = Random(~seed).shuffle(Card.all)

  val allPlayers: Seq[Player] = Seq(
    RandomStrategy,
    Low,
    High,
    HighLow,
    PartnerHighLow,
    Markov(),
    Markov(partial = true),
    CardCounting,
    Hybrid(),
    Hybrid(partial = true),
  )
