void runSimpleMuonEtaPlotter(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("SimpleMuonEtaPlotter.C");
  SimpleMuonEtaPlotter m(sampleName, text, maxEvents);
  m.Loop();
}
