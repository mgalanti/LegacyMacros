void runMACRONAMEMC(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MACRONAMEMC.C");
  MACRONAMEMC m(sampleName, text, maxEvents);
  m.Loop();
}
