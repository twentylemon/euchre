package org.lemon.euchre

import scala.annotation.tailrec
import scala.util.Random

/** @param dealt each seat's hand as dealt, before the dealer picks up
  * @param round 1 if the up card's suit was ordered up, 2 if trump was called after it was turned down
  */
final case class HandResult(
    dealer: Seat,
    dealt: Map[Seat, Cards],
    upCard: Card,
    caller: Seat,
    call: Call,
    round: Int,
    tricks: Map[Team, Int],
    scoringTeam: Team,
    points: Int,
    playNanos: Map[Seat, Long],
):
  def callerPosition: Int = caller.position(dealer)
  def stuck: Boolean = round == 2 && caller == dealer
  def callerTricks: Int = tricks(caller.team)

  /** @return the points the calling team won, negative if they were euchred */
  def callerPoints: Int = if scoringTeam == caller.team then points else -points

final case class GameResult(hands: Seq[HandResult], score: Map[Team, Int]):
  def winner: Team = score.maxBy(_._2)._1
  def tricks(team: Team): Int = hands.map(_.tricks(team)).sum
  def playNanos(seat: Seat): Long = hands.map(_.playNanos(seat)).sum

object Game:
  val handSize = 5
  val winningScore = 10

  def points(tricks: Int, alone: Boolean): Int =
    if tricks == handSize then if alone then 4 else 2
    else if tricks > handSize / 2 then 1
    else 0

  val euchrePoints = 2

/** Plays euchre with the given strategies, checking that every call and play is legal. */
final class Game(strategies: Map[Seat, Strategy], rng: Random):
  import Game.*

  private def biddingOrder(dealer: Seat): Seq[Seat] = (1 to Seat.values.size).map(dealer.offset)

  def playGame(): GameResult =
    @tailrec
    def loop(dealer: Seat, hands: Vector[HandResult], score: Map[Team, Int]): GameResult =
      if score.values.exists(_ >= winningScore) then GameResult(hands, score)
      else
        val hand = playHand(dealer, rng.shuffle(Card.all))
        loop(dealer.next, hands :+ hand, score.updated(hand.scoringTeam, score(hand.scoringTeam) + hand.points))
    loop(Seat.fromOrdinal(rng.nextInt(Seat.values.size)), Vector.empty, Team.values.map(_ -> 0).toMap)

  /** @return the caller, their call and the round it was made in */
  private def bid(dealer: Seat, dealt: Map[Seat, Cards], upCard: Card): (Seat, Call, Int) =
    val order = biddingOrder(dealer)
    def view(seat: Seat) = BidView(seat, dealer, dealt(seat), upCard, rng)
    def firstCall(seats: Seq[Seat], bid: (Strategy, BidView) => Option[Call]) =
      seats.iterator
        .map(seat => seat -> bid(strategies(seat), view(seat)))
        .collectFirst { case (seat, Some(call)) => (seat, call) }

    firstCall(order, _.orderUp(_)) match
      case Some((seat, call)) =>
        require(call.trump == upCard.suit, s"$seat ordered up ${call.trump} but the up card is $upCard")
        (seat, if seat == dealer.partner then call.copy(alone = true) else call, 1)
      case None =>
        val (seat, call) = firstCall(order.init, _.callTrump(_))
          .getOrElse(dealer -> strategies(dealer).stickTrump(view(dealer)))
        require(call.trump != upCard.suit, s"$seat called ${call.trump} after $upCard was turned down")
        (seat, call, 2)

  def playHand(dealer: Seat, deck: Seq[Card]): HandResult =
    val dealt = biddingOrder(dealer).zip(deck.grouped(handSize).map(Cards.from)).toMap
    val upCard = deck(Seat.values.size * handSize)
    val (caller, call, round) = bid(dealer, dealt, upCard)

    val sittingOut = Option.when(call.alone)(caller.partner)
    val pickedUp = round == 1 && !sittingOut.contains(dealer)
    val discard = Option.when(pickedUp) {
      val withUpCard = dealt(dealer) + upCard
      val card = strategies(dealer).discard(BidView(dealer, dealer, withUpCard, upCard, rng), call.trump)
      require(withUpCard.contains(card), s"$dealer discarded $card, which isn't in ${withUpCard.mkString(" ")}")
      card
    }
    val hands = discard.fold(dealt)(card => dealt.updated(dealer, dealt(dealer) + upCard - card))
    val active = Seat.values.filterNot(sittingOut.contains)
    val knowledge = active.map(seat =>
      val discarded = if seat == dealer then Cards.from(discard) else Cards.empty
      seat -> Knowledge.start(seat, hands(seat), upCard, Option.when(pickedUp)(dealer), sittingOut, discarded)
    ).toMap

    val playNanos = collection.mutable.Map.from(Seat.values.map(_ -> 0L))
    def playTrick(state: (Map[Seat, Cards], Map[Seat, Knowledge], Seat))
        : (Map[Seat, Cards], Map[Seat, Knowledge], Seat) =
      val (hands, knowledge, leader) = state
      val seats = (0 until Seat.values.size).map(leader.offset).filter(active.contains)
      val (trick, finalHands, finalKnowledge) =
        seats.foldLeft((Trick(call.trump), hands, knowledge)) { case ((trick, hands, knowledge), seat) =>
          val view = PlayView(seat, hands(seat), trick, caller, call.alone, knowledge(seat), rng)
          val start = System.nanoTime()
          val card = strategies(seat).play(view)
          playNanos(seat) += System.nanoTime() - start
          require(
            view.playable.contains(card),
            s"$seat played $card, which isn't playable into $trick from ${view.hand.mkString(" ")}"
          )
          (
            trick :+ (seat -> card),
            hands.updated(seat, hands(seat) - card),
            knowledge.view.mapValues(_.observe(seat, card, trick)).toMap
          )
        }
      (finalHands, finalKnowledge, trick.winner)

    val leader = Iterator.iterate(dealer.next)(_.next).find(active.contains).get
    val winners = Iterator
      .iterate((hands, knowledge, leader))(playTrick)
      .drop(1)
      .take(handSize)
      .map(_._3)
      .toSeq
    val tricks = Team.values.map(team => team -> winners.count(_.team == team)).toMap

    val callingTeam = caller.team
    val callerPoints = points(tricks(callingTeam), call.alone)
    val (scoringTeam, scored) =
      if callerPoints > 0 then (callingTeam, callerPoints) else (callingTeam.other, euchrePoints)
    HandResult(dealer, dealt, upCard, caller, call, round, tricks, scoringTeam, scored, playNanos.toMap)
