#ifndef MuonDistributionsProducer_h
#define MuonDistributionsProducer_h

#include "TRandom3.h"

#include <iostream>

#include "../utilities/TreeAnalyzerData.C"
#include "../utilities/MuonJetContainer.h"

class MuonDistributionsProducer : public TreeAnalyzerData
{
  public:
    MuonDistributionsProducer(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerData("MuonDistributionsProducer", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
    TRandom3 rng;
};

#endif // MuonDistributionsProducer_h
