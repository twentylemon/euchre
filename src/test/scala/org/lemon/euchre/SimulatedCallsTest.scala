package org.lemon.euchre

import org.lemon.euchre.Generators.{*, given}
import org.lemon.euchre.strategies.*
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

import scala.util.Random

class SimulatedCallsTest extends UnitTest:

  private val decisions =
    for
      cards <- deck
      seat <- arbitrary[Seat]
      dealer <- arbitrary[Seat]
      round <- Gen.oneOf(1, 2)
    yield Decision(seat, dealer, Cards.from(cards.take(Game.handSize)), cards(Game.handSize), round)

  private val reachable = arbitrary[Long].flatMap(seed =>
    Gen.oneOf(SimulatedCalls.sampleDecisions(1, RuleBidder.tuned, Random(seed)).map(_._1))
  )

  private val decisionsAndBids =
    for
      decision <- decisions
      bid <- Gen.oneOf(decision.options)
      seed <- arbitrary[Long]
    yield (decision, bid, seed)

  private val strongest = Decision(Seat.Left, Seat.Down, cards("J♠ J♣ A♠ K♠ Q♠"), parse("9♠"), 1)

  test("round one offers a pass and the up card's suit, alone only for the dealer's partner") {
    check(forAll(decisions.map(_.copy(round = 1))) { decision =>
      val calls = decision.options.flatten
      decision.options.head.isEmpty && calls.forall(_.trump == decision.upCard.suit) &&
      calls.map(_.alone) == (if decision.seat == decision.dealer.partner then Seq(true) else Seq(false, true))
    })
  }

  test("round two offers each other suit with and without the partner, and a pass unless stuck") {
    check(forAll(decisions.map(_.copy(round = 2))) { decision =>
      val calls = decision.options.flatten
      decision.options.contains(None) == (decision.seat != decision.dealer) &&
      calls.map(_.trump).toSet == Suit.values.toSet - decision.upCard.suit &&
      calls.size == 6 && calls.toSet.size == 6
    })
  }

  test("earlier passes are the seats before this one, and in round two everyone's first") {
    check(forAll(decisions) { decision =>
      val passes = decision.earlierPasses
      val firstRound = if decision.round == 1 then decision.position - 1 else Seat.values.size
      passes.count(_._2 == 1) == firstRound && passes.count(
        _._2 == 2
      ) == (decision.round - 1) * (decision.position - 1) &&
      passes.forall((seat, round) => round < decision.round || seat.position(decision.dealer) < decision.position)
    })
  }

  test("forced holds the earlier seats to their passes and this seat to its bid") {
    check(forAll(decisionsAndBids) { (decision, bid, _) =>
      val forced = decision.forced(bid)
      forced(decision.seat)(decision.round) == bid &&
      decision.earlierPasses.forall((seat, round) =>
        (seat, round) == (decision.seat, decision.round) || forced(seat)(round).isEmpty
      ) &&
      forced.values.map(_.size).sum == decision.earlierPasses.size + 1
    })
  }

  test("a deal keeps the decision's hand and deals the other cards") {
    check(forAll(decisions, arbitrary[Long]) { (decision, seed) =>
      val hands = SimulatedCalls.deal(decision, RuleBidder.tuned, Random(seed))
      hands(decision.seat) == decision.hand && hands.size == Seat.values.size &&
      hands.values.forall(_.size == Game.handSize) && hands.values.reduce(_ ++ _).size == 20 &&
      !hands.values.exists(_.contains(decision.upCard))
    })
  }

  test("a deal of a decision rule bidding reaches is one where the earlier seats would have passed") {
    check(forAll(reachable, arbitrary[Long]) { (decision, seed) =>
      val hands = SimulatedCalls.deal(decision, RuleBidder.tuned, Random(seed))
      SimulatedCalls.passes(decision, hands, RuleBidder.tuned, Random(0))
    })
  }

  /** Calls in `round` from `seats`, and passes otherwise. */
  private final case class CallsAt(seats: Set[Seat], round: Int) extends StuckDealer:
    override def orderUp(view: BidView): Option[Call] =
      Option.when(round == 1 && seats(view.seat))(Call(view.upCard.suit))
    override def callTrump(view: BidView): Option[Call] = Option.when(round == 2 && seats(view.seat))(stickTrump(view))

  test("passes checks each earlier pass but the decision's own, in its round") {
    val callers =
      for
        seats <- Gen.someOf(Seat.values.toSeq)
        round <- Gen.oneOf(1, 2)
      yield CallsAt(seats.toSet, round)
    check(forAll(decisions, callers, arbitrary[Long]) { (decision, others, seed) =>
      val hands = SimulatedCalls.deal(decision, StuckDealer, Random(seed))
      val broken = decision.earlierPasses.exists((seat, round) =>
        seat != decision.seat && others.seats(seat) && round == others.round
      )
      SimulatedCalls.passes(decision, hands, others, Random(0)) == !broken
    })
  }

  test("playing a bid makes it, after the earlier passes") {
    check(forAll(decisionsAndBids) { (decision, bid, seed) =>
      val hands = SimulatedCalls.deal(decision, RuleBidder.tuned, Random(seed))
      val result = SimulatedCalls.play(decision, bid, hands, RuleBidder.tuned, PartnerHighLow, seed)
      val calledHere = result.caller == decision.seat && result.round == decision.round
      result.dealt == hands && !decision.earlierPasses.contains(result.caller -> result.round) &&
      bid.fold(!calledHere)(call => calledHere && result.call == call)
    })
  }

  test("points are the bidder's team's, negative when the other team scores") {
    check(forAll(decisionsAndBids) { (decision, bid, seed) =>
      val hands = SimulatedCalls.deal(decision, RuleBidder.tuned, Random(seed))
      val result = SimulatedCalls.play(decision, bid, hands, RuleBidder.tuned, PartnerHighLow, seed)
      val team = decision.seat.team
      SimulatedCalls.points(result, team) == -SimulatedCalls.points(result, team.other) &&
      (SimulatedCalls.points(result, team) > 0) == (result.scoringTeam == team) &&
      math.abs(SimulatedCalls.points(result, team)) == result.points
    })
  }

  test("evaluate values every option, between a euchre and a lone march") {
    check(
      forAll(decisions, arbitrary[Long]) { (decision, seed) =>
        val values = SimulatedCalls.evaluate(decision, 4, RuleBidder.tuned, PartnerHighLow, Random(seed))
        values.map(_.bid) == decision.options && values.forall(value =>
          value.mean >= -4 && value.mean <= 4 && value.stdErr >= 0
        )
      },
      minSuccessful(200)
    )
  }

  test("evaluate is the mean and standard error of the plays on the same deals for every option") {
    check(
      forAll(decisions, arbitrary[Long]) { (decision, seed) =>
        val values = SimulatedCalls.evaluate(decision, 3, RuleBidder.tuned, PartnerHighLow, Random(seed))
        val rng = Random(seed)
        val deals = Seq.fill(3)(SimulatedCalls.deal(decision, RuleBidder.tuned, rng) -> rng.nextLong())
        values.forall(value =>
          val points = deals.map((hands, seed) =>
            SimulatedCalls.points(
              SimulatedCalls.play(decision, value.bid, hands, RuleBidder.tuned, PartnerHighLow, seed),
              decision.seat.team
            )
          )
          val mean = points.sum / 3.0
          val spread = math.sqrt(points.map(point => (point - mean) * (point - mean)).sum / 2)
          math.abs(value.mean - mean) < 1e-9 && math.abs(value.stdErr - spread / math.sqrt(3)) < 1e-9
        )
      },
      minSuccessful(100)
    )
  }

  test("the top five trumps march alone every time") {
    val values = SimulatedCalls.evaluate(strongest, 50, RuleBidder.tuned, PartnerHighLow, Random(0))
    val alone = values.find(_.bid == Some(Call(Suit.Spades, alone = true))).get
    alone.mean shouldBe 4.0
    alone.stdErr shouldBe 0.0
    values.filter(_ != alone).forall(_.mean < 4) shouldBe true
  }

  test("a weak hand's points vary from deal to deal") {
    val values = SimulatedCalls.evaluate(
      strongest.copy(hand = cards("9♠ T♥ 9♦ T♦ 9♣")),
      50,
      RuleBidder.tuned,
      PartnerHighLow,
      Random(0)
    )
    values.exists(_.stdErr > 0) shouldBe true
  }

  test("the oracle goes alone with the top five trumps") {
    val view = BidView(strongest.seat, strongest.dealer, strongest.hand, strongest.upCard, Random(0))
    OracleBidder(20).orderUp(view) shouldBe Some(Call(Suit.Spades, alone = true))
    val hearts = cards("J♥ J♦ A♥ K♥ Q♥")
    OracleBidder(20).callTrump(view.copy(hand = hearts)) shouldBe Some(Call(Suit.Hearts, alone = true))
    OracleBidder(20).stickTrump(view.copy(seat = Seat.Down, hand = hearts)) shouldBe Call(Suit.Hearts, alone = true)
  }

  test("the oracle bids legally") {
    check(
      forAll(arbitrary[Long], arbitrary[Seat]) { (seed, dealer) =>
        val strategy = Composite("oracle", OracleBidder(2), PartnerHighLow)
        val rng = Random(seed)
        val hand = Game(Seat.values.map(_ -> strategy).toMap, rng).playHand(dealer, rng.shuffle(Card.all))
        hand.tricks.values.sum == Game.handSize
      },
      minSuccessful(30)
    )
  }

  test("decisions run until the first bid, in bidding order") {
    check(forAll(deck, arbitrary[Seat]) { (cards, dealer) =>
      val order = (1 to 4).map(dealer.offset)
      val dealt = order.zip(cards.grouped(5).map(Cards.from)).toMap
      val decisions = SimulatedCalls.decisions(dealer, dealt, cards(20), RuleBidder.tuned)
      val seats = decisions.map(_._1).map(decision => (decision.round, decision.seat))
      val expected = Seq(1, 2).flatMap(round => order.map(round -> _))
      seats == expected.take(seats.size) && decisions.init.forall(_._2.isEmpty) && decisions.last._2.isDefined &&
      decisions.forall((decision, _) => decision.hand == dealt(decision.seat) && decision.upCard == cards(20))
    })
  }

  test("decisions record the bid the bidder made") {
    check(forAll(deck, arbitrary[Seat]) { (cards, dealer) =>
      val order = (1 to 4).map(dealer.offset)
      val dealt = order.zip(cards.grouped(5).map(Cards.from)).toMap
      val (decision, bid) = SimulatedCalls.decisions(dealer, dealt, cards(20), RuleBidder.tuned).last
      val strategy = Composite("rule", RuleBidder.tuned, PartnerHighLow)
      val result = Game(Seat.values.map(_ -> strategy).toMap, Random(0)).playHand(dealer, cards)
      (result.caller, result.round, Some(result.call)) == (decision.seat, decision.round, bid)
    })
  }

  test("compare counts agreement with the best bid, and the points given up") {
    val reference = Seq(
      Seq(BidValue(None, 0.5, 0), BidValue(Some(Call(Suit.Spades)), 1.0, 0)),
      Seq(BidValue(None, 0.2, 0), BidValue(Some(Call(Suit.Hearts)), -1.0, 0)),
    )
    SimulatedCalls.compare(reference, Seq(None, None)) shouldBe (0.5, 0.25)
    SimulatedCalls.compare(reference, Seq(Some(Call(Suit.Spades)), None)) shouldBe (1.0, 0.0)
  }

  test("labels and kinds name each bid") {
    SimulatedCalls.label(None) shouldBe "pass"
    SimulatedCalls.label(Some(Call(Suit.Hearts))) shouldBe "♥"
    SimulatedCalls.label(Some(Call(Suit.Hearts, alone = true))) shouldBe "♥ alone"
    SimulatedCalls.kind(None) shouldBe SimulatedCalls.Kind.Pass
    SimulatedCalls.kind(Some(Call(Suit.Hearts))) shouldBe SimulatedCalls.Kind.Partner
    SimulatedCalls.kind(Some(Call(Suit.Hearts, alone = true))) shouldBe SimulatedCalls.Kind.Alone
  }

  test("Game.deck deals the hands back") {
    check(forAll(decisions, arbitrary[Long]) { (decision, seed) =>
      val hands = SimulatedCalls.deal(decision, StuckDealer, Random(seed))
      val result = Game(Seat.values.map(_ -> PartnerHighLow).toMap, Random(0))
        .playHand(decision.dealer, Game.deck(decision.dealer, hands, decision.upCard))
      result.dealt == hands && result.upCard == decision.upCard
    })
  }
