#ifndef MuonBasicDistributionsData_h
#define MuonBasicDistributionsData_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"
#include "../utilities/EfficiencyFactorsGetter.C"



class MuonBasicDistributionsData : public TreeAnalyzerMC , public EfficiencyFactorsGetter
{
  public:
    MuonBasicDistributionsData(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerMC("MuonBasicDistributionsData", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // MuonBasicDistributionsData_h
