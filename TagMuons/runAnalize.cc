void runAnalize(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("TagMuons.C");
  gSystem->CompileMacro("Analize.C");
  Analize m(sampleName, text, maxEvents);
  m.Loop();
}
