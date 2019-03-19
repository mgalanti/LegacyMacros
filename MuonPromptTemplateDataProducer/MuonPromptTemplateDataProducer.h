#ifndef MuonPromptTemplateDataProducer_h
#define MuonPromptTemplateDataProducer_h

#include <iostream>

#include "../utilities/TreeAnalyzerData.C"

class MuonPromptTemplateDataProducer : public TreeAnalyzerData
{
  public:
    MuonPromptTemplateDataProducer(const std::string sampleName, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerData("MuonPromptTemplateDataProducer", sampleName, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // MuonPromptTemplateDataProducer_h
