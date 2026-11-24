package org.lemon.euchre

import org.lemon.euchre.Generators.{*, given}
import org.lemon.euchre.strategies.*
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

import scala.util.Random

class GameTest extends UnitTest:

  private val seeds =
    for
      seed <- arbitrary[Long]
      dealer <- arbitrary[Seat]
    yield (seed, dealer)

  private val hands = seeds.map(spyHand(RandomStrategy, _, _))

  test("five tricks are played") {
    check(forAll(hands)((result, _) => result.tricks.values.sum == Game.handSize))
  }

  test("every active seat plays every card, and no card is played twice") {
    check(forAll(hands) { (result, views) =>
      val active = Seat.values.count(seat => !(result.call.alone && seat == result.caller.partner))
      val firstHands = views.groupBy(_.seat).values.map(_.head.hand)
      views.size == active * Game.handSize && firstHands.forall(_.size == Game.handSize) &&
      Cards.from(firstHands.flatMap(_.toSeq)).size == active * Game.handSize
    })
  }

  test("the partner of a lone caller sits out") {
    check(forAll(hands)((result, views) => !result.call.alone || !views.exists(_.seat == result.caller.partner)))
  }

  test("round one calls the up card's suit, round two never does") {
    check(forAll(hands)((result, _) => (result.call.trump == result.upCard.suit) == (result.round == 1)))
  }

  test("dealt holds four disjoint hands of five, without the up card") {
    check(forAll(hands) { (result, _) =>
      val dealt = result.dealt.values
      dealt.size == 4 && dealt.forall(_.size == Game.handSize) &&
      dealt.reduce(_ ++ _).size == 4 * Game.handSize && !dealt.exists(_.contains(result.upCard))
    })
  }

  test("the dealer picks up the up card and discards, unless sitting out") {
    check(forAll(hands) { (result, views) =>
      val dealt = result.dealt(result.dealer)
      val pickedUp = result.round == 1 && !(result.call.alone && result.caller == result.dealer.partner)
      views.find(_.seat == result.dealer).map(_.hand).forall(hand =>
        if pickedUp then hand.size == 5 && hand.subsetOf(dealt + result.upCard) else hand == dealt
      )
    })
  }

  test("callerPosition counts seats from the dealer's left") {
    check(forAll(hands, Gen.choose(1, 4)) { case ((result, _), n) =>
      result.copy(caller = result.dealer.offset(n)).callerPosition == n
    })
  }

  test("the calling team's points are positive exactly when they take three tricks") {
    check(forAll(hands)((result, _) => (result.callerPoints > 0) == (result.callerTricks >= 3)))
  }

  test("the calling team scores by tricks taken, or is euchred") {
    check(forAll(hands) { (result, _) =>
      val team = result.caller.team
      val tricks = result.tricks(team)
      val expected =
        if tricks == 5 && result.call.alone then (team, 4)
        else if tricks == 5 then (team, 2)
        else if tricks >= 3 then (team, 1)
        else (team.other, 2)
      (result.scoringTeam, result.points) == expected
    })
  }

  test("points by tricks taken") {
    Game.points(5, alone = true) shouldBe 4
    Game.points(5, alone = false) shouldBe 2
    Game.points(4, alone = true) shouldBe 1
    Game.points(3, alone = false) shouldBe 1
    Game.points(2, alone = false) shouldBe 0
    Game.points(0, alone = true) shouldBe 0
  }

  test("a stuck dealer is euchred by the top five trumps") {
    val down = "J♠ J♣ A♠ K♠ Q♠"
    val left = "9♥ T♥ Q♥ K♥ A♥"
    val up = "9♦ T♦ Q♦ K♦ A♦"
    val right = "9♣ T♣ Q♣ K♣ A♣"
    val deck = Seq(down, left, up, right, "J♥ J♦ T♠ 9♠").flatMap(_.split(" ")).map(parse)
    val game = Game(Seat.values.map(_ -> PartnerHighLow).toMap, Random(0))
    val result = game.playHand(Seat.Right, deck)
    result.caller shouldBe Seat.Right
    result.call shouldBe Call(Suit.Spades)
    result.round shouldBe 2
    (result.scoringTeam, result.points) shouldBe (Team.UpDown, 2)
  }

  test("the stuck dealer can't name the turned down suit") {
    val rest = Card.all.filterNot(_ == parse("9♠"))
    val deck = rest.take(20) ++ Seq(parse("9♠")) ++ rest.drop(20)
    val result = Game(Seat.values.map(_ -> Low).toMap, Random(0)).playHand(Seat.Down, deck)
    result.upCard shouldBe parse("9♠")
    result.call shouldBe Call(Suit.Hearts)
  }

  test("a game ends when one team reaches 10") {
    check(
      forAll(arbitrary[Long]) { seed =>
        val result = Game(Seat.values.map(_ -> PartnerHighLow).toMap, Random(seed)).playGame()
        val (winner, loser) = (result.score(result.winner), result.score(result.winner.other))
        val totals = Team.values.map(team => result.hands.filter(_.scoringTeam == team).map(_.points).sum)
        winner >= Game.winningScore && loser < Game.winningScore &&
        totals.toSeq == Team.values.map(result.score).toSeq &&
        result.hands.sliding(2).forall {
          case Seq(a, b) => b.dealer == a.dealer.next
          case _ => true
        }
      },
      minSuccessful(100)
    )
  }

  test("the same seed plays the same game") {
    check(
      forAll(arbitrary[Long]) { seed =>
        def play() =
          val strategies =
            Seat.values.map(seat => seat -> (if seat.team == Team.UpDown then CardCounting else RandomStrategy))
          Game(strategies.toMap, Random(seed)).playGame().hands.map(_.copy(playNanos = Map.empty))
        play() == play()
      },
      minSuccessful(100)
    )
  }
