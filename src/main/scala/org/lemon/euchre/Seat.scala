package org.lemon.euchre

/** The seats in play order, so the next player is the next ordinal. */
enum Seat:
  case Down, Left, Up, Right

  def next: Seat = offset(1)
  def partner: Seat = offset(2)
  def offset(n: Int): Seat = Seat.fromOrdinal(Math.floorMod(ordinal + n, Seat.values.size))
  def team: Team = if ordinal % 2 == 0 then Team.UpDown else Team.LeftRight

enum Team:
  case UpDown, LeftRight

  def other: Team = Team.fromOrdinal(ordinal ^ 1)
