#include <exception>

void runMuonBasicDistributionsDataForBmm(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonBasicDistributionsDataForBmm.C");
  MuonBasicDistributionsDataForBmm m(sampleName, text, maxEvents);
  m.Loop();
}
