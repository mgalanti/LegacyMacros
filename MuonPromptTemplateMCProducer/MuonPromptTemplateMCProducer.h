#ifndef MuonPromptTemplateMCProducer_h
#define MuonPromptTemplateMCProducer_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"
// #include "../utilities/MuonMCAssociator.C"

class MuonPromptTemplateMCProducer : public TreeAnalyzerMC/* , public MuonMCAssociator*/
{
  public:
    MuonPromptTemplateMCProducer(const std::string sampleName, const std::string text, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerMC("MuonPromptTemplateMCProducer", sampleName, text, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // MuonPromptTemplateMCProducer_h
