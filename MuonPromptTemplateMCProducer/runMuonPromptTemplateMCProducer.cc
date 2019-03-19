void runMuonPromptTemplateMCProducer(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonPromptTemplateMCProducer.C");
  MuonPromptTemplateMCProducer m(sampleName, text, maxEvents);
  m.Loop();
}
