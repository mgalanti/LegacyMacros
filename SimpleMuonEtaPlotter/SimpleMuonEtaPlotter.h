#ifndef SimpleMuonEtaPlotter_h
#define SimpleMuonEtaPlotter_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class SimpleMuonEtaPlotter : public TreeAnalyzerMC
{
  public:
    SimpleMuonEtaPlotter(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : 
      TreeAnalyzerMC("SimpleMuonEtaPlotter", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // SimpleMuonEtaPlotter_h
