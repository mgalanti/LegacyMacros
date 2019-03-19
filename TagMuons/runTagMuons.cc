void runTagMuons(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("TagMuons.C");
  TagMuons m(sampleName, text, maxEvents);
  m.Loop();
}
