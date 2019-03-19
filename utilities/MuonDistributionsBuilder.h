#ifndef MuonDistributionsBuilder_h
#define MuonDistributionsBuilder_h

#include "MuonHistogramsBuilderBase.h"
#include "HistoPlotter.C"

#include "TH2D.h"
#include "TH1D.h"
#include "TFile.h"
#include "TRandom3.h"

#include <iostream>
#include <sstream>



class MuonDistributionsBuilder : public virtual MuonHistogramsBuilderBase
{
 public:
   MuonDistributionsBuilder() { std::cout << "MuonDistributionsBuilder: Please call constructor with arguments!\n";};
   MuonDistributionsBuilder(const std::string inputFileName_, const std::string outputFileName_, std::vector<std::string> inputHistoNames1D_, std::vector<std::string> inputHistoNames2D_, std::string outputHistoName1D_, std::string outputHistoName2D_) :
     MuonHistogramsBuilderBase(outputFileName_),
     inputFileName(inputFileName_),
     inputHistoNames1D(inputHistoNames1D_),
     inputHistoNames2D(inputHistoNames2D_),
     outputHistoName1D(outputHistoName1D_),
     outputHistoName2D(outputHistoName2D_)
   {
     Initialize();
   }
   ~MuonDistributionsBuilder();
   void Build1DDistribution(TH1D*& result);
   void Build1DDistribution();
   void Build2DDistribution(TH2D*& result);
   void Build2DDistribution();
   void SaveDistributions();
//    void SetRange(const double axisMin, const double axisMax);
   void SetScaleFactors(const std::vector<double> factors) {scaleFactors = factors;}
   void CreateAndSavePrettyPlots();

 private:
   void Initialize();
   std::vector<TH1D*> PrepareHistograms(std::vector<TH1D*> histos);
   std::vector<TH2D*> PrepareHistograms(std::vector<TH2D*> histos);
   std::string inputFileName;
   std::vector<std::string> inputHistoNames1D;
   std::vector<std::string> inputHistoNames2D;
   std::string outputHistoName1D;
   std::string outputHistoName2D;
   std::vector<TH1D*> inputHistos1D;
   std::vector<TH2D*> inputHistos2D;
   TFile* inputFile;
   TH1D* distribution1D;
   TH2D* distribution2D;
   std::vector<double> scaleFactors;
};



#endif // MuonDistributionsBuilder_h


