#include <exception>

void runMuonBasicDistributionsDataForBmmUrsAssociation(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonBasicDistributionsDataForBmmUrsAssociation.C");
  MuonBasicDistributionsDataForBmmUrsAssociation m(sampleName, text, maxEvents);
  m.Loop();
}
