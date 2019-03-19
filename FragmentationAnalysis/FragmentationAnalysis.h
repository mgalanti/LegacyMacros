#ifndef FragmentationAnalysis_h
#define FragmentationAnalysis_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class FragmentationAnalysis : public TreeAnalyzerMC
{
  public:
    FragmentationAnalysis(const std::string sampleName, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerMC("FragmentationAnalysis", sampleName, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // FragmentationAnalysis_h
