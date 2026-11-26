package org.lemon.euchre

import org.lemon.euchre.Generators.given
import org.lemon.euchre.strategies.*
import org.scalacheck.*
import org.scalacheck.Arbitrary.*
import org.scalacheck.Prop.*

class RuleSweepTest extends UnitTest:

  private val start = RuleSweep.initial
  private val values = Gen.oneOf(RuleSweep.params).flatMap(param => Gen.oneOf(param.grid).map(param -> _))

  test("a bidder wins exactly half its games against itself") {
    check(
      forAll(arbitrary[Long])(seed => RuleSweep.winRate(start, start, PartnerHighLow, 10, seed) == 0.5),
      minSuccessful(20)
    )
  }

  test("win rates are complementary") {
    check(
      forAll(arbitrary[Long]) { seed =>
        RuleSweep.winRate(start, StuckDealer, Low, 10, seed) + RuleSweep.winRate(StuckDealer, start, Low, 10, seed) == 1
      },
      minSuccessful(20)
    )
  }

  test("each parameter sets exactly its own value") {
    check(forAll(values) { case (param, value) =>
      val moved = param.set(start, value)
      param.get(moved) == value && RuleSweep.params.filter(_ != param).forall(other =>
        other.get(moved) == other.get(start)
      )
    })
  }

  test("the sweep moves to a value only when it beats the current bidder by the margin") {
    check(
      forAll(values, arbitrary[Long], Gen.oneOf(0.0, 0.05)) { case ((param, value), seed, margin) =>
        val rate = RuleSweep.winRate(param.set(start, value), start, PartnerHighLow, 20, seed + 1)
        val grid = Seq(param.copy(grid = Seq(value)))
        val swept = RuleSweep.sweep(start, PartnerHighLow, 20, 1, seed, margin, grid)(_ => ())
        swept == (if rate > 0.5 + margin then param.set(start, value) else start)
      },
      minSuccessful(30)
    )
  }
