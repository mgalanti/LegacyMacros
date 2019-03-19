void runJetEnergyScaleTests(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("JetEnergyScaleTests.C");
  JetEnergyScaleTests m(sampleName, text, maxEvents);
  m.Loop();
}
