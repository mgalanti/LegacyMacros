void runJetSelectionTests(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("JetSelectionTests.C");
  JetSelectionTests m(sampleName, text, maxEvents);
  m.Loop();
}
