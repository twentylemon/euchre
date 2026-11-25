package org.lemon.euchre

import org.lemon.euchre.Generators.{*, given}
import org.lemon.euchre.strategies.*
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

import scala.util.Random

class RuleBidderTest extends UnitTest:

  private val weights = HandStrength(offAce = 0.5, void = 0.2)

  private val bidders =
    for
      offAce <- Gen.choose(0.0, 1.0)
      void <- Gen.choose(0.0, 0.5)
      orderUp <- Gen.listOfN(4, Gen.choose(0.0, 4.0))
      call <- Gen.listOfN(3, Gen.choose(0.0, 4.0))
      next <- Gen.choose(-1.0, 1.0)
      alone <- Gen.choose(2.0, 6.0)
    yield RuleBidder(HandStrength(offAce, void), orderUp, call, next, alone)

  private def bidder(orderUp: Double = 0, call: Double = 0, next: Double = 0, alone: Double = Double.PositiveInfinity) =
    RuleBidder(weights, Seq.fill(4)(orderUp), Seq.fill(3)(call), next, alone)

  private def view(seat: Seat, dealer: Seat, hand: String, upCard: String) =
    BidView(seat, dealer, cards(hand), parse(upCard), Random(0))

  test("the win chances of every card sum to the trick's one winner per card played") {
    check((trump: Suit) => math.abs(Card.all.map(HandStrength.winChance(trump, _)).sum - 6) < 1e-9)
  }

  test("the right bower always wins") {
    check((trump: Suit) => HandStrength.winChance(trump, Card(Rank.Jack, trump)) == 1)
  }

  test("strength counts trumps, off-suit aces and voids, with the left bower as trump") {
    val trump = Suit.Spades
    val chances = Seq("J♠", "J♣", "A♠").map(card => HandStrength.winChance(trump, parse(card))).sum
    weights(cards("J♠ J♣ A♠ A♥ 9♦"), trump) shouldBe (chances + 0.5 + 0.2 +- 1e-9)
  }

  test("a hand without trumps doesn't count its voids") {
    weights(cards("A♥ K♥ Q♥ J♥ T♥"), Suit.Clubs) shouldBe (0.5 +- 1e-9)
  }

  test("adding a trump to a hand with trump adds its win chance") {
    check(forAll(arbitrary[Suit], deck) { (trump, cards) =>
      val (trumps, others) = cards.partition(_.isTrump(trump))
      val hand = Cards.from(others.take(3)) + trumps(1)
      math.abs(weights(hand + trumps(0), trump) - weights(hand, trump) - HandStrength.winChance(
        trump,
        trumps(0)
      )) < 1e-9
    })
  }

  test("the discard comes from the hand, and is an off-suit non-ace when there is one") {
    check(forAll(arbitrary[Suit], deck)((trump, cards) =>
      val hand = Cards.from(cards.take(6))
      val discard = RuleBidder.discard(hand, trump)
      val spare = hand.filter(card => !card.isTrump(trump) && card.rank != Rank.Ace)
      hand.contains(discard) && (spare.isEmpty || spare.contains(discard))
    ))
  }

  test("the discard makes a void when it can") {
    check(forAll(arbitrary[Suit], deck)((trump, cards) =>
      val hand = Cards.from(cards.take(6))
      def voids(hand: Cards) = Suit.values.count(suit => hand.filter(_.effectiveSuit(trump) == suit).isEmpty)
      val canVoid = hand.iterator.exists(card =>
        !card.isTrump(trump) && card.rank != Rank.Ace && voids(hand - card) > voids(hand)
      )
      !canVoid || voids(hand - RuleBidder.discard(hand, trump)) > voids(hand)
    ))
  }

  test("the discard keeps off-suit aces and makes a void") {
    RuleBidder.discard(cards("9♥ K♦ Q♦ J♠ A♣ T♠"), Suit.Spades) shouldBe parse("9♥")
    RuleBidder.discard(cards("A♥ A♦ J♠ A♣ T♠ 9♠"), Suit.Spades) shouldBe parse("A♥")
  }

  test("pickUp keeps five cards from the hand and the up card") {
    check(forAll(deck)(cards =>
      val hand = Cards.from(cards.take(5))
      val kept = RuleBidder.pickUp(hand, cards(5))
      kept.size == 5 && kept.subsetOf(hand + cards(5))
    ))
  }

  test("pickUp keeps every trump when there's an off-suit card to discard") {
    check(forAll(deck)(cards =>
      val (hand, upCard) = (Cards.from(cards.take(5)), cards(5))
      val withUpCard = hand + upCard
      val trumps = withUpCard.filter(_.isTrump(upCard.suit))
      trumps.size == withUpCard.size || trumps.subsetOf(RuleBidder.pickUp(hand, upCard))
    ))
  }

  test("the up card counts for the dealer's team and against the other") {
    val hand = "A♠ K♠ A♥ 9♦ T♣"
    val strength = weights(cards(hand), Suit.Spades)
    val rules = bidder(orderUp = strength + 0.5)
    rules.orderUp(view(Seat.Up, Seat.Down, hand, "J♠")) shouldBe Some(Call(Suit.Spades))
    rules.orderUp(view(Seat.Left, Seat.Down, hand, "J♠")) shouldBe None
  }

  test("the dealer counts the hand they'd keep") {
    val hand = "A♠ K♠ A♥ 9♦ T♣"
    val kept = RuleBidder.pickUp(cards(hand), parse("J♠"))
    val rules = bidder(orderUp = weights(kept, Suit.Spades))
    rules.orderUp(view(Seat.Down, Seat.Down, hand, "J♠")) shouldBe Some(Call(Suit.Spades))
    bidder(orderUp = weights(kept, Suit.Spades) + 0.01).orderUp(view(Seat.Down, Seat.Down, hand, "J♠")) shouldBe None
  }

  test("each seat uses its own threshold") {
    val hand = "A♠ K♠ Q♥ 9♦ T♣"
    val rules = RuleBidder(weights, Seq(9, 9, 0, 9), Seq(9, 0, 9), 0, Double.PositiveInfinity)
    Seat.values.map(seat => seat -> rules.orderUp(view(seat, Seat.Down, hand, "9♠")).isDefined).toMap shouldBe
      Map(Seat.Left -> false, Seat.Up -> false, Seat.Right -> true, Seat.Down -> false)
    Seat.values.filter(_ != Seat.Down).map(seat => seat -> rules.callTrump(view(seat, Seat.Down, hand, "9♥")).isDefined)
      .toMap shouldBe Map(Seat.Left -> false, Seat.Up -> true, Seat.Right -> false)
  }

  test("round two calls the strongest legal suit, with next's bonus") {
    val hand = "J♣ A♣ K♣ A♦ 9♦"
    bidder().callTrump(view(Seat.Left, Seat.Down, hand, "Q♥")).map(_.trump) shouldBe Some(Suit.Clubs)
    bidder(next = 2).callTrump(view(Seat.Left, Seat.Down, hand, "Q♥")).map(_.trump) shouldBe Some(Suit.Diamonds)
    bidder(next = 2).stickTrump(view(Seat.Down, Seat.Down, hand, "Q♥")).trump shouldBe Suit.Diamonds
    bidder().stickTrump(view(Seat.Down, Seat.Down, "A♦ K♦ Q♦ 9♣ T♣", "Q♣")).trump shouldBe Suit.Diamonds
  }

  test("calls go alone at the alone threshold") {
    val hand = "J♠ J♣ A♠ K♠ Q♠"
    val strength = weights(cards(hand), Suit.Spades)
    bidder(alone = strength).stickTrump(view(Seat.Down, Seat.Down, hand, "9♥")) shouldBe Call(Suit.Spades, alone = true)
    bidder(alone = strength + 0.01).stickTrump(view(Seat.Down, Seat.Down, hand, "9♥")) shouldBe Call(Suit.Spades)
    bidder(alone = strength).callTrump(view(Seat.Left, Seat.Down, hand, "9♥")) shouldBe Some(Call(Suit.Spades, true))
    bidder(alone = strength - 1).orderUp(view(Seat.Up, Seat.Down, hand, "9♠")) shouldBe Some(Call(Suit.Spades, true))
  }

  test("rule bidders always bid legally") {
    check(forAll(bidders, arbitrary[Long], arbitrary[Seat]) { (rules, seed, dealer) =>
      val strategy = Composite("rules", rules, RandomStrategy)
      val rng = Random(seed)
      val hand = Game(Seat.values.map(_ -> strategy).toMap, rng).playHand(dealer, rng.shuffle(Card.all))
      hand.tricks.values.sum == Game.handSize
    })
  }

  test("position counts seats from the dealer's left") {
    check(forAll(arbitrary[Seat], Gen.choose(1, 4))((dealer, n) => dealer.offset(n).position(dealer) == n))
  }

  test("Composite bids with its bidder") {
    check(forAll(bidders, deck, arbitrary[Seat], Gen.choose(1, 3)) { (rules, cards, dealer, n) =>
      val strategy = Composite("rules", rules, Low)
      val bidView = BidView(dealer.offset(n), dealer, Cards.from(cards.take(5)), cards(5), Random(0))
      val dealerView = bidView.copy(seat = dealer, hand = bidView.hand + cards(5))
      strategy.orderUp(bidView) == rules.orderUp(bidView) && strategy.callTrump(bidView) == rules.callTrump(bidView) &&
      strategy.orderUp(dealerView) == rules.orderUp(dealerView) &&
      strategy.stickTrump(dealerView) == rules.stickTrump(dealerView) &&
      strategy.discard(dealerView, cards(5).suit) == rules.discard(dealerView, cards(5).suit)
    })
  }
