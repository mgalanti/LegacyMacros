void runMuonEfficienciesComputerFromMCTruth(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonEfficienciesComputerFromMCTruth.C");
  MuonEfficienciesComputerFromMCTruth m(sampleName, text, maxEvents);
  m.Loop();
}
