void runMACRONAMERECO(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MACRONAMERECO.C");
  MACRONAMERECO m(sampleName, text, maxEvents);
  m.Loop();
}
