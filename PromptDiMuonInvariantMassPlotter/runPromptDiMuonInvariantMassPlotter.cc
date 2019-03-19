void runPromptDiMuonInvariantMassPlotter(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("PromptDiMuonInvariantMassPlotter.C");
  PromptDiMuonInvariantMassPlotter m(sampleName, text, maxEvents);
  m.Loop();
}
