#ifndef TreeAnalyzerData_h
#define TreeAnalyzerData_h

#include "TreeAnalyzerBase.C"
#include "TreeReaderData.C"
#include "HistoPlotter.C"



class PARTICLESELECTOR;

class TreeAnalyzerData : public TreeAnalyzerBase , public TreeReaderData
{
  public:
    TreeAnalyzerData() { std::cout << "TreeAnalyzerData: Please call constructor with arguments!\n";};
    TreeAnalyzerData(const std::string myName_, const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) : TreeAnalyzerBase(myName_, sampleName_, text_, maxEvents_), TreeReaderData(sampleName_, tree) {/*CheckRecoTreeStructure();*/InitSelector();};

    friend class PARTICLESELECTOR;

  private:
    // void CheckRecoTreeStructure();
    void InitSelector();

  protected:
    PARTICLESELECTOR* s;
};

#endif // TreeAnalyzerData_h
