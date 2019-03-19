#ifndef PseudoExperiments_h
#define PseudoExperiments_h

#include "TH1D.h"
#include "TH2D.h"

#include "../BinnedLogLikelihood/BinnedLogLikelihood1D.h"
#include "../BinnedLogLikelihood/BinnedLogLikelihood2D.h"
#include "../BinnedLogLikelihood/BinnedLogLikelihood2DConstrained.h"
#include "../utilities/FitResults1DPlotter.C"
#include "../utilities/FitResults2DPlotter.C"
#include "MuonHistogramsBuilderBase.C"
#include "MuonDistributionsBuilder.C"
#include "MuonTemplatesBuilder.C"



template<typename T>
struct BinnedLikelihood
{
  typedef void type;
};



template<>
struct BinnedLikelihood<TH1D>
{
  typedef BinnedLogLikelihood1D type;
};


template<>
struct BinnedLikelihood<TH2D>
{
  typedef BinnedLogLikelihood2D type;
};



class PseudoExperiments : public MuonTemplatesBuilder, public MuonDistributionsBuilder
{
  public:
    PseudoExperiments() { std::cout << "PseudoExperiments: Please call constructor with arguments!\n";};
    PseudoExperiments(const std::string inputTemplateFileName_,
                      std::vector<std::string> inputTemplateHistoNames_,
                      std::vector<std::string> outputTemplateHistoNames_,
                      const std::string inputPromptTemplateFileName_,
                      const std::string inputPromptTemplateHistoName_,
                      const std::string inputDistributionFileName_,
                      const std::vector<std::string> inputDistributionHistoNames1D_,
                      const std::vector<std::string> inputDistributionHistoNames2D_,
                      const std::string outputDistributionHistoName1D_,
                      const std::string outputDistributionHistoName2D_,
                      const std::string outputFileName_) :
      MuonHistogramsBuilderBase(outputFileName_),
      MuonTemplatesBuilder(inputTemplateFileName_,
                           outputFileName_,
                           inputTemplateHistoNames_,
                           outputTemplateHistoNames_,
                           inputPromptTemplateFileName_,
                           inputPromptTemplateHistoName_) ,
      MuonDistributionsBuilder(inputDistributionFileName_,
                               outputFileName_,
                               inputDistributionHistoNames1D_,
                               inputDistributionHistoNames2D_,
                               outputDistributionHistoName1D_,
                               outputDistributionHistoName2D_)
    {
      InitializePseudoExperiments();
    }
    void InitializePseudoExperiments();
    virtual ~PseudoExperiments() {};
    TH1D* CreateTH1FromOscillatedTF1(const TF1* func, const std::string histoName, const unsigned int nBins, const double xMin, const double xMax, const std::string xTitle, const std::string yTitle);
    TH1D* OscillateTH1(const TH1D* histo);
    TH2D* OscillateTH2(const TH2D* histo);
    void BuildFitted1DIPHistos0thIter();
    void BuildPseudoDataIPHisto0thIter();
    void BuildIP1DTemplates(std::vector<TH1D*>& vTemplates);
    void BuildIP1DTemplatesFromFit(std::vector<TH1D*>& vTemplates);
    void BuildIP2DTemplates(std::vector<TH2D*>& vTemplates);
    void BuildIP2DTemplatesFromFit(std::vector<TH2D*>& vTemplates);
    void BuildReferences();
    TFitterMinuit* DoDefaultFit(std::string templateSuffix = "", std::string outputReferenceFileName = "");
    void DoPseudoExperiments(const std::string options = "1IF");
    void Do1DValidationPlotsPerIteration(std::vector<TH1D*> vTemplates, TH1D* pseudoData, TFitterMinuit* tfm, const unsigned int iPseudoExp);
    void Do2DValidationPlotsPerIteration(std::vector<TH2D*> vTemplates, TH2D* pseudoData, TFitterMinuit* tfm, const unsigned int iPseudoExp);
    template<typename T>
    void DoPseudoExperimentsValidationPlots(std::vector<T*> vTemplates, T* pseudoData, TFitterMinuit* tfm);
    template<typename T>
    TFitterMinuit* DoBinnedLikelihoodFit(std::vector<T*> vTemplates, T* pseudoData);
    TFitterMinuit* DoConstrainedBinnedLikelihoodFit(std::vector<TH2D*> vTemplates, TH2D* pseudoData);
    template<typename T>
    void CreateTemplatesForFit(const std::vector<T*>& vTemplates, std::vector<T*>& result, const std::string appended = "");
    template<typename T>
    void PlotFitResults(std::vector<T*> vTemplates, T* pseudoData, TFitterMinuit* tfm, const std::string fileName);
    void SavePseudoExperimentsValidationPlots();
    inline void UseTemplateForFit(const std::string name) {vTemplateNamesForFit.push_back(name);};
    inline void UseTemplatesForFit(const std::vector<std::string> names) {vTemplateNamesForFit = names;};
    inline void SetConstraintToComponent(const std::vector<int> vConstraints) {vConstraintToComponent = vConstraints;};
    inline void SetConstraintValues(const std::vector<double> vConstraints) {vConstraintValues = vConstraints;};
    inline void SetConstraintErrors(const std::vector<double> vConstraints) {vConstraintErrors = vConstraints;};
    inline void SetNPseudoExp(const unsigned int number) {nPseudoExp = number;};
    inline unsigned int GetNPseudoExp() {return nPseudoExp;};
    inline void SetNEventsrIPTemplates(const std::vector<unsigned int> integrals) {nEventsIPTemplates = integrals;};
    inline std::vector<unsigned int> GetNEventsIPTemplates() {return nEventsIPTemplates;};
    inline void SetNEventsIPTemplate(const unsigned int integral, const unsigned int i) {nEventsIPTemplates[i] = integral;};
    inline unsigned int GetNEventsIPTemplate(const unsigned int i) {return nEventsIPTemplates[i];};
    inline void SetRNGSeed(const unsigned int seed) {rng.SetSeed(seed);};
    inline unsigned int GetRNGSeed() {return rng.GetSeed();};
//     inline void SetRebinFactor(unsigned short int factor) {MuonHistogramsBuilderBase::SetRebinFactor(factor);};
  private:
    template<typename T>
    void CleanVectorOfHistograms(std::vector<T*> histograms)
    {
      for(unsigned int iHisto = 0; iHisto < histograms.size(); iHisto++)
      {
        histograms[iHisto]->Clear();
        delete histograms[iHisto];
      }
      histograms.clear();
    };
    void ParseOptions(const std::string options);
    void AppendToTemplateNames(const std::string suffix);
    void AppendToDistributionNames(const std::string suffix);
     
    unsigned int nPseudoExp;
    std::vector<TH1D*> fitted1DIPHistos0thIter;
    std::vector<unsigned int> nEventsIPTemplates;
    std::vector<std::string> vTemplateNamesForFit;
    
    std::vector<int> vConstraintToComponent; 
    std::vector<double> vConstraintValues; 
    std::vector<double> vConstraintErrors; 
    
    TH1D* pseudoData1DIPHisto0thIter;
    TH2D* pseudoData2DIPHisto0thIter;

    std::vector<double> defaultFractions;
    std::vector<double> defaultFractionErrors;
    std::vector<std::string> parameterNames;
    std::vector<TH1D*> fractionsPseudoExp;
    std::vector<std::vector<TH2D*> > fractionsVsFractionsPseudoExp;
    std::vector<TH1D*> fractionErrorsPseudoExp;
    std::vector<TH2D*> fractionsVsErrorsPseudoExp;
    std::vector<TH1D*> fractionsPseudoExpDifferences;
    std::vector<TH1D*> fractionsPseudoExpPulls;
    
    bool use1D;
    bool use2D;
    bool useIP;
    bool usePtRel;
    bool useFittedTemplates;
    bool useSmoothed0thIterHistograms;
    bool useConstrainedLikelihood;
    bool saveValidationPlotsPerIteration;
    
    unsigned int linestyles[2];
    unsigned int markerstyles[2];
    unsigned int colors[2];
};



template<typename T>
void PseudoExperiments::DoPseudoExperimentsValidationPlots(std::vector<T*> vTemplates, T* pseudoData, TFitterMinuit* tfm)
{
  unsigned int nPars = tfm->GetNumberTotalParameters();
  double integral = pseudoData->Integral();
  // If histograms have not been booked yet, do it
  if(fractionsPseudoExp.size() == 0)
  {
    for(unsigned int iPar = 0; iPar < nPars; iPar++)
    {
      std::string histoName = "h" + parameterNames[iPar] + "FractionsPseudoExp";
      std::string histoTitle = "Fractions of parameter " + parameterNames[iPar] + " in the pseudoexperiments";
      std::string xTitle = parameterNames[iPar] + " fraction";
      fractionsPseudoExp.push_back(Create1DHistogram<TH1D>(histoName.c_str(), histoTitle.c_str(), 1000, 0., 1.,xTitle.c_str(), "N. of times"));
    }
  }
  
  if(fractionsVsFractionsPseudoExp.size() == 0)
  {
    for(unsigned int iPar = 0; iPar < nPars; iPar++)
    {
      std::vector<TH2D*> temp;
      for(unsigned int jPar = 0; jPar < nPars; jPar++)
      {
        std::string histoName = "h" + parameterNames[iPar] + "FractionVs" + parameterNames[jPar] + "FractionPseudoExp";
        std::string histoTitle = "Fractions of parameter " + parameterNames[iPar] + " vs " + parameterNames[jPar] + " in the pseudoexperiments";
        std::string xTitle = parameterNames[jPar] + " fraction";
        std::string yTitle = parameterNames[iPar] + " fraction";
        temp.push_back(Create2DHistogram<TH2D>(histoName.c_str(), histoTitle.c_str(), 1000, 0., 1., 1000, 0., 1., xTitle.c_str(), yTitle.c_str(), "N. of times"));
      }
      fractionsVsFractionsPseudoExp.push_back(temp);
    }
  }
  
  if(fractionErrorsPseudoExp.size() == 0)
  {
    for(unsigned int iPar = 0; iPar < nPars; iPar++)
    {
      std::string histoName = "h" + parameterNames[iPar] + "FractionErrorsPseudoExp";
      std::string histoTitle = "Fraction errors of parameter " + parameterNames[iPar] + " in the pseudoexperiments";
      std::string xTitle = parameterNames[iPar] + " error";
      fractionErrorsPseudoExp.push_back(Create1DHistogram<TH1D>(histoName.c_str(), histoTitle.c_str(), 1000, 0., 0.1, xTitle.c_str(), "N. of times"));
    }
  }
  
  if(fractionsVsErrorsPseudoExp.size() == 0)
  {
    for(unsigned int iPar = 0; iPar < nPars; iPar++)
    {
      std::string histoName = "h" + parameterNames[iPar] + "FractionsVsErrorsPseudoExp";
      std::string histoTitle = "Fractions vs. errors of parameter " + parameterNames[iPar] + " in the pseudoexperiments";
      std::string xTitle = parameterNames[iPar] + " error";
      std::string yTitle = parameterNames[iPar] + " fraction";
      fractionsVsErrorsPseudoExp.push_back(Create2DHistogram<TH2D>(histoName.c_str(), histoTitle.c_str(), 1000, 0., 0.1, 1000, 0., 1., xTitle.c_str(), yTitle.c_str(), "N. of times"));
    }
  }
  
  if(fractionsPseudoExpDifferences.size() == 0)
  {
    for(unsigned int iPar = 0; iPar < nPars; iPar++)
    {
      std::string histoName = "h" + parameterNames[iPar] + "FractionPseudoExpDifference";
      std::string histoTitle = "Difference between fraction of " + parameterNames[iPar] + " in the pseudoexperiments and in standard fit";
      std::string xTitle = parameterNames[iPar] + " difference";
      fractionsPseudoExpDifferences.push_back(Create1DHistogram<TH1D>(histoName.c_str(), histoTitle.c_str(), 1000, -1., 1., xTitle.c_str(), "N. of times"));
    }
  }
  
  if(fractionsPseudoExpPulls.size() == 0)
  {
    for(unsigned int iPar = 0; iPar < nPars; iPar++)
    {
      std::string histoName = "h" + parameterNames[iPar] + "FractionPseudoExpPull";
      std::string histoTitle = "Pull between fraction of " + parameterNames[iPar] + " in the pseudoexperiments and in standard fit";
      std::string xTitle = parameterNames[iPar] + " pull";
      fractionsPseudoExpPulls.push_back(Create1DHistogram<TH1D>(histoName.c_str(), histoTitle.c_str(), 1000, -5., 5., xTitle.c_str(), "N. of times"));
    }
  }
  
  for(unsigned int iPar = 0; iPar < nPars; iPar++)
  {
    double fraction = tfm->GetParameter(iPar) / integral;
    double error = tfm->GetParError(iPar) / integral;
    fractionsPseudoExp[iPar]->Fill(fraction);
    fractionErrorsPseudoExp[iPar]->Fill(error);
    fractionsVsErrorsPseudoExp[iPar]->Fill(error, fraction);
    fractionsPseudoExpDifferences[iPar]->Fill(fraction - defaultFractions[iPar]);
    fractionsPseudoExpPulls[iPar]->Fill((fraction - defaultFractions[iPar])/(sqrt(error*error + defaultFractionErrors[iPar]*defaultFractionErrors[iPar])));
    for(unsigned int jPar = 0; jPar < nPars; jPar++)
    {
      double fraction2 = tfm->GetParameter(jPar) / integral;
      double error2 = tfm->GetParError(jPar) / integral;
      fractionsVsFractionsPseudoExp[iPar][jPar]->Fill(fraction2, fraction);
    }
  }
}



template<typename T>
TFitterMinuit* PseudoExperiments::DoBinnedLikelihoodFit(std::vector<T*> vTemplates, T* pseudoData)
{
  typedef typename BinnedLikelihood<T>::type LikelihoodType;
  double nEvents = pseudoData->Integral();
  
  LikelihoodType* likelihood = new LikelihoodType(vTemplates, pseudoData);
  TFitterMinuit *minFit = new TFitterMinuit;
  minFit->Clear();
  minFit->SetPrintLevel(-1);
  minFit->CreateMinimizer();
  minFit->SetMinuitFCN(likelihood);
  
  std::vector<double> par;
  std::vector<double> parError;
  
  for(unsigned int iTemplate = 0; iTemplate < vTemplates.size(); iTemplate++)
  {
    par.push_back(nEvents/vTemplates.size());
    parError.push_back(sqrt(nEvents)/vTemplates.size());
    
    std::string parName = "";
    std::string templateName = vTemplates[iTemplate]->GetName();
    parName+=templateName.erase(0,1);
    minFit->SetParameter(iTemplate,parName.c_str(),par[iTemplate],parError[iTemplate],0.,nEvents);
  }
  minFit->Minimize(1000,0.0000001);
  
  std::cout << "---- MINIMIZATION RESULTS ----" << std::endl;
  minFit->PrintResults(4,0.);
  return minFit;
}



template<typename T>
void PseudoExperiments::CreateTemplatesForFit(const std::vector<T*>& vTemplates, std::vector<T*>& result, const std::string appended)
{
  result.clear();
  for(unsigned int iName = 0; iName < vTemplateNamesForFit.size(); iName++)
  {
    for(unsigned int iTemplate = 0; iTemplate < vTemplates.size(); iTemplate++)
    {
      if((vTemplateNamesForFit[iName]+appended).compare(vTemplates[iTemplate]->GetName()) == 0)
      {
        result.push_back(vTemplates[iTemplate]);
        break;
      }
    }
  }
}



template<>
void PseudoExperiments::PlotFitResults(std::vector<TH1D*> vTemplates, TH1D* data, TFitterMinuit* tfm, const std::string fileName)
{
  std::vector<unsigned int> ordering;
  
  if(vTemplates.size() < 3)
  {
    for(unsigned int i = 0; i < vTemplates.size(); i++)
      ordering.push_back(i);
  }
  else if(vTemplates.size() == 3)
  {
    ordering.push_back(2);
    ordering.push_back(1);
    ordering.push_back(0);
  }
  else if(vTemplates.size() > 3)
  {
    ordering.push_back(2);
    ordering.push_back(3);
    ordering.push_back(1);
    ordering.push_back(0);
    for(unsigned int i = ordering.size(); i < vTemplates.size(); i++)
      ordering.push_back(i);
  }
  
  FitResults1DPlotter* frp = new FitResults1DPlotter(vTemplates, data, tfm, fileName);
  
  frp->SetPlotOrder(ordering);
  frp->CreateAndSavePlots();
  delete frp;
}



template<>
void PseudoExperiments::PlotFitResults(std::vector<TH2D*> vTemplates, TH2D* data, TFitterMinuit* tfm, const std::string fileName)
{
  std::vector<unsigned int> ordering;
  if(vTemplates.size() > 6)
  {
    ordering.push_back(3);
    ordering.push_back(6);
    ordering.push_back(2);
    ordering.push_back(5);
    ordering.push_back(4);
    ordering.push_back(1);
    ordering.push_back(0);
    for(unsigned int i = ordering.size(); i < vTemplates.size(); i++)
      ordering.push_back(i);
  }
  else
  {
    for(unsigned int i = 0; i < vTemplates.size(); i++)
      ordering.push_back(i);
  }
  FitResults2DPlotter* frp = new FitResults2DPlotter(vTemplates, data, tfm, fileName);
  
  frp->SetPlotOrder(ordering);
  frp->CreateAndSavePlots();
  delete frp;
}

#endif // PseudoExperiments_h
