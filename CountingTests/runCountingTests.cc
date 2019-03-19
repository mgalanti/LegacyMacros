void runCountingTests()
{
  gSystem->CompileMacro("CountingTests.C");
  CountingTests t;
  t.Loop();
}
