#ifndef TagMuons_h
#define TagMuons_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class TagMuons : public TreeAnalyzerMC
{
  public:
    TagMuons(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) :
      TreeAnalyzerMC("TagMuons", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();
    virtual void BeginJob() ;
    virtual void EndJob( int ) ;
    virtual void AnalizeEvent( int ) ;

  private:
    // Nothing by default
    // Add your private variables/methods here
};

#endif // TagMuons_h
