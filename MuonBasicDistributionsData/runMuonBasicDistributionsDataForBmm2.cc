#include <exception>

void runMuonBasicDistributionsDataForBmm2(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonBasicDistributionsDataForBmm2.C");
  MuonBasicDistributionsDataForBmm2 m(sampleName, text, maxEvents);
  m.Loop();
}
