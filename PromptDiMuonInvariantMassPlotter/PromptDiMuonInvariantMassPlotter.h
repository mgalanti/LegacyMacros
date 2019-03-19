#ifndef PromptDiMuonInvariantMassPlotter_h
#define PromptDiMuonInvariantMassPlotter_h

#include <iostream>

#include "../utilities/TreeAnalyzerData.C"

class PromptDiMuonInvariantMassPlotter : public TreeAnalyzerData
{
  public:
    PromptDiMuonInvariantMassPlotter(const std::string sampleName, const std::string text, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerData("PromptDiMuonInvariantMassPlotter", sampleName, text, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // PromptDiMuonInvariantMassPlotter_h
