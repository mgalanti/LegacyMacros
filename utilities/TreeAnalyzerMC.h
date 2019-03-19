#ifndef TreeAnalyzerMC_h
#define TreeAnalyzerMC_h

#include "TreeAnalyzerBase.C"
#include "TreeReaderMC.C"
#include "HistoPlotter.C"
#include "MCTools.C"
#include "MuonMCAssociator.C"



class PARTICLESELECTOR;

class TreeAnalyzerMC : public TreeAnalyzerBase , public TreeReaderMC, public virtual MCTools, public virtual MuonMCAssociator
{
  public:
    TreeAnalyzerMC() { std::cout << "TreeAnalyzerMC: Please call constructor with arguments!\n";};
    TreeAnalyzerMC(const std::string myName_, const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerBase(myName_, sampleName_, text_, maxEvents_), TreeReaderMC(sampleName_, tree) {InitSelector();};
//     ~TreeAnalyzerMC();

    friend class PARTICLESELECTOR;

  private:
    void InitSelector();

  protected:
    PARTICLESELECTOR* s;
};

#endif // TreeAnalyzerMC_h
