#ifndef JetSelectionTests_h
#define JetSelectionTests_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class JetSelectionTests : public TreeAnalyzerMC
{
  public:
    JetSelectionTests(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) :
      TreeAnalyzerMC("JetSelectionTests", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // JetSelectionTests_h
