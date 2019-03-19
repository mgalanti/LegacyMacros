#ifndef MuonEfficienciesComputerFromMCTruth_h
#define MuonEfficienciesComputerFromMCTruth_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class MuonEfficienciesComputerFromMCTruth : public TreeAnalyzerMC
{
  public:
    MuonEfficienciesComputerFromMCTruth(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerMC("MuonEfficienciesComputerFromMCTruth", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
    typedef std::map<unsigned int, std::pair<unsigned int, double> > OneToOneMapWithQuality;
};

#endif // MuonEfficienciesComputerFromMCTruth_h
