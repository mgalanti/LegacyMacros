#ifndef BmmL1TTImplementation_h
#define BmmL1TTImplementation_h

#include <iostream>

#include "../utilities/TreeAnalyzerData.C"

class BmmL1TTImplementation : public TreeAnalyzerData
{
  public:
    BmmL1TTImplementation(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : 
      TreeAnalyzerData("BmmL1TTImplementation", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // BmmL1TTImplementation_h
