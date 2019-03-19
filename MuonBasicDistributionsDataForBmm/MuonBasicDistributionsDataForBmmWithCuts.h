#ifndef MuonBasicDistributionsDataForBmmWithCuts_h
#define MuonBasicDistributionsDataForBmmWithCuts_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"
#include "../utilities/EfficiencyFactorsGetter.C"



class MuonBasicDistributionsDataForBmmWithCuts : public TreeAnalyzerMC , public EfficiencyFactorsGetter
{
  public:
    MuonBasicDistributionsDataForBmmWithCuts(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerMC("MuonBasicDistributionsDataForBmm", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop(const double LowPtCut_, const double HighPtCut_, const double LowAbsEtaCut_, const double HighAbsEtaCut_, const int SelectChargeCut_);

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // MuonBasicDistributionsDataForBmmWithCuts_h
