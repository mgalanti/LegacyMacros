#ifndef JetEnergyScaleTests_h
#define JetEnergyScaleTests_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class JetEnergyScaleTests : public TreeAnalyzerMC
{
  public:
    JetEnergyScaleTests(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) :
      TreeAnalyzerMC("JetEnergyScaleTests", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // JetEnergyScaleTests_h
