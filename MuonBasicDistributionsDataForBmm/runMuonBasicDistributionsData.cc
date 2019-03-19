void runMuonBasicDistributionsData(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonBasicDistributionsData.C");
  MuonBasicDistributionsData m(sampleName, text, maxEvents);
  m.Loop();
}
