#ifndef FitResults1DPlotter_h
#define FitResults1DPlotter_h

#include "HistoPlotter.C"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodBase.h"



class FitResults1DPlotter : public HistoPlotter
{
  public:
    FitResults1DPlotter(std::vector<TH1D*> hTemplates_, TH1D* hData_, TFitterMinuit* tfm_, std::string outputFileName_, int status_) :
                        hTemplates(hTemplates_), hData(hData_), tfm(tfm_), outputFileName(outputFileName_), status(status_)
                        {Initialize();};
    ~FitResults1DPlotter();
    void CreateAndSavePlots();
    void SetPlotOrder(std::vector<unsigned int> ordering_) {ordering = ordering_;};
    
  private:
    void Initialize();
    
    std::vector<TH1D*> hTemplates;
    TH1D* hData;
    TFitterMinuit* tfm;
    BinnedLogLikelihoodBase* likelihoodFCN;
    std::string outputFileName;
    int status;
    TFile* outputFile;
    std::vector<double> vPars;
    std::vector<double> vParErrors;
    std::vector<std::string> vParNames;
    std::vector<unsigned int> ordering;
    double nEvents;
    double nDataEvents;
    std::string curDirName;
};

#endif // FitResults1DPlotter_h
