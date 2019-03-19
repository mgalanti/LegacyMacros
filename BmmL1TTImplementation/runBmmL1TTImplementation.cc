void runBmmL1TTImplementation(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("BmmL1TTImplementation.C");
  BmmL1TTImplementation m(sampleName, text, maxEvents);
  m.Loop();
}
