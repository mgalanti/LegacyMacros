#ifndef MACRONAMEMC_h
#define MACRONAMEMC_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class MACRONAMEMC : public TreeAnalyzerMC
{
  public:
    MACRONAMEMC(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) :
      TreeAnalyzerMC("MACRONAMEMC", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // MACRONAMEMC_h
