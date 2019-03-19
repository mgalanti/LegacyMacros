void runMuonPromptTemplateDataProducer(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonPromptTemplateDataProducer.C");
  MuonPromptTemplateDataProducer m(sampleName, text, maxEvents);
  m.Loop();
}
