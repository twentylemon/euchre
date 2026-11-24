lazy val euchre = (project in file("."))
  .settings(
    scalaVersion := "3.10.1",
    libraryDependencies += "org.scala-lang.modules" %% "scala-parallel-collections" % "1.3.0",
    libraryDependencies += "org.scalacheck" %% "scalacheck" % "1.20.0" % Test,
    libraryDependencies += "org.scalatestplus" %% "scalacheck-1-19" % "3.2.20.0" % Test,
    libraryDependencies += "org.scalatest" %% "scalatest" % "3.2.20" % Test,
    Test / testOptions += Tests.Argument(TestFrameworks.ScalaTest, "-oDF"),
    Test / logBuffered := false,
  )
