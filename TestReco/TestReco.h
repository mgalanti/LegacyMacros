#ifndef TestReco_h
#define TestReco_h

#include <iostream>

#include "../utilities/TreeAnalyzerData.C"

class TestReco : public TreeAnalyzerData
{
  public:
    TestReco(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : 
      TreeAnalyzerData("TestReco", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // TestReco_h
