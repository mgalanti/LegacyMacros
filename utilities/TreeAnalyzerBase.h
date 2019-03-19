#ifndef TreeAnalyzerBase_h
#define TreeAnalyzerBase_h

#include <TFile.h>
#include "HistoPlotter.C"
#include "RecoTools.C"
#include "DuplicateEventHandler.h"


class TreeAnalyzerBase : public HistoPlotter , public RecoTools
{
  public:
    TreeAnalyzerBase() { std::cout << "TreeAnalyzerBase: Please call constructor with arguments!\n";};
    TreeAnalyzerBase(const std::string myName_, const std::string sampleName_, const std::string text_, const int maxEvents_ = -1) {Initialize(myName_, sampleName_, text_, maxEvents_);};
    ~TreeAnalyzerBase();

  private:
    void Initialize(const std::string myName_, const std::string sampleName_, const std::string text_, const int maxEvents_);
//     virtual const std::string ClassName();

    std::string myName;

  protected:
    std::string sampleName;
    int maxEvents;
    std::string text;
    TFile* outFile;
};

#endif // TreeAnalyzerBase_h
