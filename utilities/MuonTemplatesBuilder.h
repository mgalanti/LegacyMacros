#ifndef MuonTemplatesBuilder_h
#define MuonTemplatesBuilder_h

#include "MuonHistogramsBuilderBase.C"
#include "HistoPlotter.C"

#include "TF1.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TFitResultPtr.h"

#include <map>
#include <iostream>
#include <sstream>



class MuonTemplatesBuilder : public virtual MuonHistogramsBuilderBase, public virtual HistoPlotter
{
 public:
   MuonTemplatesBuilder() { std::cout << "MuonTemplatesBuilder: Please call constructor with arguments!\n";};
   MuonTemplatesBuilder(const std::string inputFileName_, const std::string outputFileName_, std::vector<std::string> inputHistoNames_, std::vector<std::string> outputHistoNames_, const std::string inputFileNamePrompt_ = "", const std::string inputHistoNamePrompt_ = "") :
     MuonHistogramsBuilderBase(outputFileName_),
     inputFileName(inputFileName_),
     inputHistoNames(inputHistoNames_),
     outputHistoNames(outputHistoNames_),
     inputFileNamePrompt(inputFileNamePrompt_),
     inputHistoNamePrompt(inputHistoNamePrompt_)
   {
     Initialize();
   }
   ~MuonTemplatesBuilder();
   void Build1DTemplates();
   void Build1DTemplatesFromFit();
   void Build1DIPDistributions();
   void Build1DDistributions();
   void Build2DTemplates();
   void Build2DTemplatesFromFit();
   void Build2DIPDistributions();
   void Build2DDistributions();
   void SaveTemplates();
   void CreateAndSavePrettyPlots();
   
   std::map<std::string,std::pair<double,double> > FindIntegralOfRemovedBinsFor1DTemplates();
   std::map<std::string,std::pair<double,double> > FindIntegralOfRemovedBinsFor2DTemplates();

   void Initialize();
//    TFitResultPtr FitDistribution(TH1D* histo, const std::string name, const bool useGauss, unsigned short nExpos,const double funcMin, const double funcMax);
   TFitResultPtr FitIPDistribution(TH1D* histo, const std::string name, unsigned short nGauss, unsigned short nExpos,const double funcMin, const double funcMax);
   TFitResultPtr FitIPDistribution(TH1D* histo, const std::string name, unsigned short nGauss, unsigned short nExpos,const double funcMin, const double funcMax, TH1D*& histoResult, TF1*& fitFunction, bool produceValidationPlots, bool doRebin = true);
   
   TH1D* Build1DTemplate(TH1D* histo, std::string name, const bool doRebin = true);
   TH1D* Build1DTemplateFromFit(TH1D* histo, std::string name);
   TH1D* Build1DIPDistribution(TH1D* histo, std::string name, const bool doRebin = true);
   TH1D* Build1DDistribution(TH1D* histo, std::string name, const bool doRebin = true);
   TH2D* Build2DTemplate(TH1D* xHisto, TH1D* yHisto, std::string name, const bool doRebin = true);
   TH2D* Build2DTemplateFromFit(TH1D* xHisto, TH1D* yHisto, std::string name);
   TH2D* Build2DIPDistribution(TH1D* xHisto, TH1D* yHisto, std::string name, const bool doRebin = true);
   TH2D* Build2DDistribution(TH1D* xHisto, TH1D* yHisto, std::string name, const bool doRebin = true);
   TH1D* PrepareHistogram(TH1D* histo, const bool doRebin = true);
   TH1D* PrepareHistogramForFit(TH1D* histo, const bool doRebin = true);
   TH1D* NormalizeHistogram(TH1D* histo);
   TH1D* PrepareAndNormalizeHistogram(TH1D* histo, const bool doRebin = true);
   TH1D* CreateTH1FromTF1(const TF1* func, const std::string histoName, const unsigned int nBins, const double xMin, const double xMax, const std::string xTitle, const std::string yTitle);
   void FindIPFitParameters(const TH1D* histo, const std::string histoName, double& funcMin, double& funcMax, unsigned short& nGauss, unsigned short& nExp);
private:
   std::string inputFileName;
   std::vector<std::string> inputHistoNames;
   std::vector<TH1D*> inputHistos;
   TFile* inputFile;
   TFile* inputFilePrompt;
   std::vector<TH1D*> templates1D;
   std::vector<TH1D*> distributions1D;
   std::vector<TF1*> fitFunctions;
   std::vector<TH1D*> templates1DFromFit;
   std::vector<TH2D*> templates2D;
   std::vector<TH2D*> distributions2D;
   std::vector<std::string> outputHistoNames;
   std::string inputFileNamePrompt;
   std::string inputHistoNamePrompt;
   bool useSeparatePrompt;
};



#endif // MuonTemplatesBuilder_h

