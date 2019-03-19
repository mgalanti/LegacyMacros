#include <string>
#include <vector>


void runHistogram2DComparator()
{

  // List of input files
  std::pair<std::string,std::string> inputFileNames;
//   inputFileNames.first = "./output_MuGLBplusQuality_efficiencies.root";
  inputFileNames.first = "../MuonBFractionsAnalysis/IPDistributions__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711_SKIM__muonV6Pt4.root";
  inputFileNames.second = "../MuonBFractionsAnalysis/IPDistributions__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711_SKIM__muonV9Pt4.root";
  
  // Output file
  const std::string outputFileName("IPDistributions__Data__Mu__Run2010B-muonV6Pt4_vs_muonV9Pt4.root");


  // Graphics definition
  std::vector<std::string> legendLabels;
  legendLabels.push_back("Muon V6 selector");
  legendLabels.push_back("Muon V9 selector");

  const std::string canvasTitle("CMS 2010 Data");
//   const std::string canvasTitle("CMS Simulation, #sqrt{s} = 7 TeV");

//   legendLabels.push_back("Data - global #mu");
//   legendLabels.push_back("Data - tight #mu");
//   legendLabels.push_back("MC - global #mu");
//   legendLabels.push_back("MC - tight #mu");

  gSystem->CompileMacro("../utilities/Histogram2DComparator.C");
  Histogram2DComparator h2dc(inputFileNames, outputFileName, canvasTitle, legendLabels);
  
  h2dc.CompareHistograms("hMuonDxy2D","hMuonDxy2D", "hDiffMuonDxy2D", "Difference between V6 and V9 selector");
  h2dc.CompareHistogramsWithPull("hMuonDxy2D","hMuonDxy2D", "hPullMuonDxy2D", "Pull between V6 and V9 selector");
  h2dc.CompareHistogramsWithRatio("hMuonDxy2D","hMuonDxy2D", "hRatioMuonDxy2D", "Ratio between V6 and V9 selector");
  h2dc.CompareHistograms("hData2D","hData2D", "hDiffData2D", "Difference between V6 and V9 selector");
  h2dc.CompareHistogramsWithPull("hData2D","hData2D", "hPullData2D", "Pull between V6 and V9 selector");
  h2dc.CompareHistogramsWithRatio("hData2D","hData2D", "hRatioData2D", "Ratio between V6 and V9 selector");
  
}
