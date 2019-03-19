void runMuonDistributionsBuilder(std::string inputFileName, std::string outputFileName)
{
  std::vector<std::string> inputHistoNames1D;
  std::vector<std::string> inputHistoNames2D;
  inputHistoNames1D.push_back("hMuonDxySimplifiedTemplate_1");
  inputHistoNames1D.push_back("hMuonDxySimplifiedTemplate_2");
  inputHistoNames1D.push_back("hMuonDxySimplifiedTemplate_3");
  inputHistoNames1D.push_back("hMuonDxySimplifiedTemplate_4");

//   inputHistoNames2D.push_back("hMuonDxy2D");
  std::string outputHistoName1D = "hData";
  std::string outputHistoName2D = "hData2D";
  gSystem->CompileMacro("../utilities/MuonHistogramsBuilderBase.C");
  gSystem->CompileMacro("../utilities/MuonDistributionsBuilder.C");
//   MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder("MuonIPDistributionsProducer__Histograms__Data__Mu__Run2010A-PromptReco-v4__141001-144114__muonV0Pt3.root","MuonIPDistributions__Histograms__Data__Mu__Run2010A-PromptReco-v4__141001-144114__muonV0Pt3.root",inputHistoNames1D,inputHistoNames2D,outputHistoName1D,outputHistoName2D);
  MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(inputFileName,outputFileName,inputHistoNames1D,inputHistoNames2D,outputHistoName1D,outputHistoName2D);
  ipdb->Build1DDistribution();
//   ipdb->Build2DDistribution();
  ipdb->SaveDistributions();
  ipdb->CreateAndSavePrettyPlots();
  delete ipdb;
}
