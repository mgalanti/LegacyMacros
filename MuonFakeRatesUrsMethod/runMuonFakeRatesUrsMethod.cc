void runMuonFakeRatesUrsMethod(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonFakeRatesUrsMethod.C");
  MuonFakeRatesUrsMethod m(sampleName, text, maxEvents);
  m.Loop();
}
