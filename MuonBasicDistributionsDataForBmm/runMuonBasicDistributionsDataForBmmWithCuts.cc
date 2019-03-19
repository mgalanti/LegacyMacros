#include <exception>

void runMuonBasicDistributionsDataForBmmWithCuts(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1, const double LowPtCut = 4., const double HighPtCut = 30., const double LowAbsEtaCut = 0., const double HighAbsEtaCut = 2.4,  const int SelectChargeCut = 0)
{
  gSystem->CompileMacro("MuonBasicDistributionsDataForBmmWithCuts.C");
  MuonBasicDistributionsDataForBmmWithCuts m(sampleName, text, maxEvents);
  m.Loop(LowPtCut, HighPtCut, LowAbsEtaCut, HighAbsEtaCut, SelectChargeCut);
}
