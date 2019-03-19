void runBmmL1TTStudies(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("BmmL1TTStudies.C");
  BmmL1TTStudies m(sampleName, text, maxEvents);
  m.Loop();
}
