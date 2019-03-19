#ifndef MuonBasicDistributionsMC_h
#define MuonBasicDistributionsMC_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"
#include "../utilities/EfficiencyFactorsGetter.C"

// #include "../utilities/MuonMCAssociator.C"

class MuonBasicDistributionsMC : public TreeAnalyzerMC , public EfficiencyFactorsGetter /*, public MuonMCAssociator*/
{
  public:
    MuonBasicDistributionsMC(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerMC("MuonBasicDistributionsMC", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
    double DxyD6TScaling(double dxy);
};

#endif // MuonBasicDistributionsMC_h
