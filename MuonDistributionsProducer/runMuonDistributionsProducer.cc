void runMuonDistributionsProducer(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonDistributionsProducer.C");
  MuonDistributionsProducer m(sampleName, text, maxEvents);
  m.Loop();
}
