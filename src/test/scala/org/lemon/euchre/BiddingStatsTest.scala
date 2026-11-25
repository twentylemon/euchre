package org.lemon.euchre

import org.lemon.euchre.Generators.{*, given}
import org.lemon.euchre.strategies.*
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

import scala.util.Random

class BiddingStatsTest extends UnitTest:

  private def randomHands(minCount: Int) =
    for
      seed <- arbitrary[Long]
      dealer <- arbitrary[Seat]
      count <- Gen.choose(minCount, 20)
    yield (0 until count).map(i => spyHand(RandomStrategy, seed + i, dealer.offset(i))._1)

  private val stuckHands = arbitrary[Long].map(seed =>
    Game(Seat.values.map(_ -> PartnerHighLow).toMap, Random(seed)).playGame().hands
  )

  test("every call is made or euchred, and only made calls march") {
    check(forAll(randomHands(0)) { hands =>
      val stats = CallStats.of(hands)
      stats.hands == hands.size && stats.made + stats.euchred == stats.hands && stats.marched <= stats.made
    })
  }

  test("points per call are between a euchre and a lone march") {
    check(forAll(randomHands(1))(hands =>
      val perCall = CallStats.of(hands).pointsPerCall
      -2 <= perCall && perCall <= 4
    ))
  }

  test("stats count made, marched and euchred calls with their points") {
    val base = spyHand(RandomStrategy, 0, Seat.Down)._1
    val team = base.caller.team
    def withTricks(tricks: Int) =
      val points = Game.points(tricks, base.call.alone)
      base.copy(
        tricks = Map(team -> tricks, team.other -> (Game.handSize - tricks)),
        scoringTeam = if points > 0 then team else team.other,
        points = if points > 0 then points else Game.euchrePoints,
      )
    val alone = if base.call.alone then 2 else 0
    CallStats.of(Seq(5, 4, 3, 2, 0).map(withTricks)) shouldBe CallStats(5, 3, 1, 2 + alone + 1 + 1 - 2 - 2)
  }

  test("only the dealer is ever stuck, and only in round two") {
    check(forAll(randomHands(0))(hands =>
      hands.filter(_.stuck).forall(hand => hand.caller == hand.dealer && hand.round == 2) &&
        hands.filter(hand => hand.round == 2 && hand.caller == hand.dealer).forall(_.stuck)
    ))
  }

  test("trumps counts the left bower") {
    val base = spyHand(RandomStrategy, 0, Seat.Down)._1
    val hand = base.copy(dealt = base.dealt.updated(Seat.Up, cards("J♠ J♣ A♠ 9♥ T♦")), call = Call(Suit.Spades))
    BiddingStats.trumps(hand, Seat.Up) shouldBe 3
  }

  test("by partitions the hands into sorted, distinct groups") {
    check(forAll(randomHands(0)) { hands =>
      val groups = BiddingStats.by(hands)(hand => BiddingStats.trumps(hand, hand.caller))
      groups.map(_._2.hands).sum == hands.size && groups.map(_._1) == groups.map(_._1).distinct.sorted
    })
  }

  test("the stuck dealer bidding only ever has the dealer call in round two") {
    check(forAll(stuckHands)(_.forall(hand => hand.stuck && hand.callerPosition == 4)), minSuccessful(100))
  }

  test("deck deals the same hand again") {
    check(
      forAll(stuckHands) { hands =>
        val game = Game(Seat.values.map(_ -> PartnerHighLow).toMap, Random(0))
        hands.forall(hand =>
          game.playHand(hand.dealer, BiddingStats.deck(hand)).copy(playNanos = Map.empty) == hand.copy(playNanos =
            Map.empty
          )
        )
      },
      minSuccessful(100)
    )
  }

  test("stuckAlternatives scores every legal suit, including the one named") {
    check(
      forAll(stuckHands) { hands =>
        hands.forall(hand =>
          val alternatives = BiddingStats.stuckAlternatives(hand, PartnerHighLow)
          alternatives.keySet == Suit.values.toSet - hand.upCard.suit &&
          alternatives(hand.call.trump) == hand.callerPoints
        )
      },
      minSuccessful(100)
    )
  }

  test("a replayed stuck call scores as played, between the average and the best legal suit") {
    check(
      forAll(stuckHands) { hands =>
        val minimum = hands.map(BiddingStats.stuckAlternatives(_, PartnerHighLow).values.min).sum.toDouble / hands.size
        val summary = BiddingStats.stuckSummary(hands, PartnerHighLow)
        math.abs(summary.replayed - summary.asPlayed) < 1e-9 &&
        minimum <= summary.average && summary.average <= summary.best && summary.asPlayed <= summary.best
      },
      minSuccessful(100)
    )
  }

  test("the average legal suit of one stuck hand is between its worst and best") {
    check(
      forAll(stuckHands)(hands =>
        hands.forall(hand =>
          val points = BiddingStats.stuckAlternatives(hand, PartnerHighLow).values
          val summary = BiddingStats.stuckSummary(Seq(hand), PartnerHighLow)
          points.min <= summary.average && summary.average <= points.max && summary.best == points.max
        )
      ),
      minSuccessful(100)
    )
  }

  test("table renders a header and one row per group") {
    BiddingStats.table("trumps", Seq(3 -> CallStats(5, 3, 1, 2))).split("\n").toSeq shouldBe Seq(
      "| trumps | calls | made | marched | euchred | points per call |",
      "| --- | ---: | ---: | ---: | ---: | ---: |",
      "| 3 | 5 | 60.0% | 20.0% | 40.0% | +0.400 |",
    )
  }

  test("a stuck hand that didn't go alone replays as played with rule bidding") {
    val strategy = Composite("rules", RuleBidder.tuned, PartnerHighLow)
    check(
      forAll(arbitrary[Long]) { seed =>
        val hands = Game(Seat.values.map(_ -> strategy).toMap, Random(seed)).playGame().hands
        hands.filter(hand => hand.stuck && !hand.call.alone).forall(hand =>
          BiddingStats.stuckAlternatives(hand, strategy)(hand.call.trump) == hand.callerPoints
        )
      },
      minSuccessful(100)
    )
  }

  test("named finds every strategy by name") {
    (Strategies.all :+ MonteCarlo()).foreach(strategy => Strategies.named(strategy.name) shouldBe strategy)
    an[IllegalArgumentException] should be thrownBy Strategies.named("Clairvoyant")
  }
