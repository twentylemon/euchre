# euchre

Euchre strategy simulations in Scala 3 (sbt 2). The 2015 paper is in `paper/` and its original C++ in `euchre/`. The C++ is kept for reference only and isn't built. The current effort is trump calling, tracked in #8. Each work item there has its own issue.

## Layout

- `src/main/scala/org/lemon/euchre/` holds the model:
  - `Card`, and `Cards`, an opaque 24-bit set
  - `Suit`/`Rank`, `Seat`/`Team`, `Trick`
  - `Ranking`, the card scores and the trick-win tables
  - `Knowledge`, what a seat can infer from public play
  - `Strategy`, which is a `Bidder` and a `Player`. `StuckDealer` is the paper's bidding. `Composite` pairs any bidder with any trick player.
  - `Game`, the engine, which validates every call and play
  - `Tournament`, the paper's matchups
  - `BiddingStats`, which reports on how calls turn out and replays stuck deals with each legal suit
  - `RuleSweep`, self-play tuning of `RuleBidder`'s thresholds
- `strategies/` holds the paper's agents, plus `RuleBidder` (rule-based bidding on `HandStrength`). `Strategies.all` lists the paper's agents. Bidding is mixed in (`extends Strategy, StuckDealer`) and trick play composes other strategies (`PartnerHighLow.play(view)`), not inheritance.
- Tests in `src/test/scala/...` extend `UnitTest` (`AnyFunSuite` with `Matchers` and `Checkers`, 1000 successful property cases). `Generators` has the `Arbitrary` instances, `parse("A♥")`, and `spyHand`, which plays a hand with random bidding and records every `PlayView`.

## Commands

- Test: `sbt test`. sbt 2's `test` is incremental, and `testFull` forces a full run. One class: `sbt "testOnly org.lemon.euchre.GameTest"`.
- `sbt a b` only runs `a`, so write `sbt "a; b"`.
- Format: `sbt "scalafmtAll; scalafmtSbt"`. CI runs `scalafmtCheckAll; scalafmtSbtCheck`.
- Tournament: `sbt "runMain org.lemon.euchre.runTournament 10001 1 5"`. MonteCarlo takes about 30 s a game.
- Bidding report: `sbt "runMain org.lemon.euchre.runBiddingStats 10000 1 PartnerHighLow rule"` (the last argument is `stuck` or `rule`). Rule bidding against the stuck dealer: `runRuleCallers 10000 1`.
- Sweep: `runRuleSweep 4000 2 1 0.01` takes over an hour. Run long jobs outside sbt, with `sbt package` and then `java -cp <the coursier jars>:<the package jar> org.lemon.euchre.runRuleSweep ...`, so the sbt server stays free.
- A running `runMain` holds the sbt server, so other sbt commands queue behind it.
- Keep long-running output out of `/tmp`, which gets purged.

## Workflow

- One PR per work item, each on its own `twentylemon/<slug>` branch off the current `main`. Keep the tracking issue's work items, decisions and status current.
- Commit messages use gitmoji and a lowercase imperative summary, with identifiers in backticks: ``:sparkles: add `RuleBidder` ``. In use: `:sparkles:`, `:bug:`, `:zap:`, `:recycle:`, `:art:`, `:white_check_mark:`.
- Explanations go in commit messages and PR bodies, not in the source.
- Games are seeded. Any randomness a strategy needs comes from `view.rng`, so runs are reproducible.

## Code style

- No comments that restate the code. Use clear names instead. Scaladoc describes a strategy's rule, or anything the signature doesn't say.
- Keep descriptive names even where `-Wunused` flags them.
- scalafmt, `maxColumn = 120`.

## Tests

- Properties state real properties (legality, soundness, invariants). They don't re-implement the function.
- Use the most generic generator that fits. Avoid `==>` and `suchThat`, and generate the shape you need instead (`trickAndRest`).
- Rule details (scoring, the left bower) also get example tests.
- Mutation-check every change. Inject several targeted mutants into each changed source file (off-by-one, a flipped condition, a dropped branch, a wrong constant), run the tests, and restore the code. Every mutant must fail a test, unless it's equivalent. Add tests for any that survive. The mutants are a testing tool only, so leave them out of commits and PR bodies. Full stryker isn't needed.
- Bug fixes: write the tests first. They must fail on the unfixed code.
