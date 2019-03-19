void runMuonBasicDistributionsMC(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonBasicDistributionsMC.C");
  MuonBasicDistributionsMC m(sampleName, text, maxEvents);
  m.Loop();
}
