void runTestReco(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("TestReco.C");
  TestReco m(sampleName, text, maxEvents);
  m.Loop();
}
