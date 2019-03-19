void runMuon2DPtRelDistributionsBuilderFromPtRelDistributions(std::string inputFileName, std::string outputFileName, std::string inputHistoName2D, const double fitMin=0., const double fitMax=0.2,const int rebinFactor=5, const int removeBin=0)
{
  std::vector<std::string> inputHistoNames1D;
  std::vector<std::string> inputHistoNames2D;
  inputHistoNames2D.push_back(inputHistoName2D);

  std::string outputHistoName1D = "hData";
  std::string outputHistoName2D = "hData2D";
  gSystem->CompileMacro("../utilities/MuonHistogramsBuilderBase.C");
  gSystem->CompileMacro("../utilities/MuonDistributionsBuilder.C");
//   MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder("MuonPtRelDistributionsProducer__Histograms__Data__Mu__Run2010A-PromptReco-v4__141001-144114__muonV0Pt3.root","MuonPtRelDistributions__Histograms__Data__Mu__Run2010A-PromptReco-v4__141001-144114__muonV0Pt3.root",inputHistoNames1D,inputHistoNames2D,outputHistoName1D,outputHistoName2D);
  MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(inputFileName,outputFileName,inputHistoNames1D,inputHistoNames2D,outputHistoName1D,outputHistoName2D);
  ipdb->SetRange(fitMin,fitMax);
  ipdb->SetRebinFactor(rebinFactor);
  ipdb->SetRemoveBin(removeBin);
//   ipdb->Build1DDistribution();
  ipdb->Build2DDistribution();
  ipdb->SaveDistributions();
  ipdb->CreateAndSavePrettyPlots();
  delete ipdb;
}
