#ifndef MACRONAMERECO_h
#define MACRONAMERECO_h

#include <iostream>

#include "../utilities/TreeAnalyzerData.C"

class MACRONAMERECO : public TreeAnalyzerData
{
  public:
    MACRONAMERECO(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : 
      TreeAnalyzerData("MACRONAMERECO", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // MACRONAMERECO_h
