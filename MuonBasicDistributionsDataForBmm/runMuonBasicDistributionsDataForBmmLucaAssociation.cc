#include <exception>

void runMuonBasicDistributionsDataForBmmLucaAssociation(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonBasicDistributionsDataForBmmLucaAssociation.C");
  MuonBasicDistributionsDataForBmmLucaAssociation m(sampleName, text, maxEvents);
  m.Loop();
}
