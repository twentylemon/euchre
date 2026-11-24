package org.lemon.euchre

import org.lemon.euchre.Generators.given
import org.scalacheck.*

class SeatTest extends UnitTest:

  test("offset by a full table is the same seat") {
    check((seat: Seat, n: Int) => seat.offset(n).offset(-n) == seat && seat.offset(4) == seat)
  }

  test("partners share a team, neighbours don't") {
    check((seat: Seat) => seat.partner.team == seat.team && seat.next.team == seat.team.other)
  }

  test("partner is an involution") {
    check((seat: Seat) => seat.partner != seat && seat.partner.partner == seat)
  }

  test("next goes clockwise from down") {
    Iterator.iterate(Seat.Down)(_.next).take(4).toSeq shouldBe Seq(Seat.Down, Seat.Left, Seat.Up, Seat.Right)
  }
