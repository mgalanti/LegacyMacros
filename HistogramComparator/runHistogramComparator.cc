#include <string>
#include <vector>


void runHistogramComparator()
{

  // List of input files
  std::vector<std::string> inputFileNames;
//   inputFileNames.push_back("PtRel__Histograms__Data__Mu__All2010__132440-144114__global.root");
//   inputFileNames.push_back("PtRel__Histograms__Data__Mu__All2010__132440-144114__atLeastTwoTight.root");
//   inputFileNames.push_back("PtRel__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__noGenp__global.root");
//   inputFileNames.push_back("PtRel__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__noGenp__atLeastTwoTight.root");
//   inputFileNames.push_back("/home/mgalanti/Work/Macros/MuonIPResolutions/MuonIPResolutions__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__PVWithBS__muonV0Pt3.root");
//   inputFileNames.push_back("/home/mgalanti/Work/Macros/MuonIPResolutions/MuonIPResolutions__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__muonV0Pt3.root");
//   inputFileNames.push_back("/home/mgalanti/Work/Macros/MuonIPResolutionsData/MuonIPResolutionsData__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__PVWithBS__muonV0Pt3.root");
//   inputFileNames.push_back("/home/mgalanti/Work/Macros/MuonIPResolutionsData/MuonIPResolutionsData__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__muonV0Pt3.root");
  //   inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToMuMu_BsFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v1__NoSelections_2__muonV1BmmPt4.root");
//     inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToMuMu_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7C-v1__NoSelections__muonV1BmmPt4.root");
  //     inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToMuMu_BsFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v1__NoSelections_2__muonV1BmmPt4-withgenacceptance-noptcut.root");
//   inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToMuMu_BsFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v1__NoSelections_2__muonV2BmmPt4.root");
//     inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToMuMu_BsFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v1__NoSelections_2__muonV1TMVABmmPt4.root");
  //   inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BdToPiPi_2PiPtEtaFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v2__NoSelections_2__muonV1BmmPt4.root");
//   inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BdToPiPi_2PiPtEtaFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v2__NoSelections_2__muonV1TMVABmmPt4.root");
//   inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToKK_2KPtEtaFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v1__NoSelections_3__muonV1BmmPt4.root");
    inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToKK_EtaPtFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7C-v1__NoSelections__muonV1BmmPt4.root");
  //   inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToKK_2KPtEtaFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v1__NoSelections_3__muonV1BmmPt4-withgenacceptance-noptcut.root");
  //     inputFileNames.push_back("../MuonBasicDistributionsDataForBmm/MuonBasicDistributionsDataForBmm__Histograms__MC__BsToKK_2KPtEtaFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7A-v1__NoSelections_3__muonV1TMVABmmPt4.root");
//   inputFileNames.push_back("../SimpleTMVAResultsAnalyzer/SimpleTMVAResultsAnalyzer__Histograms__signal__TMVA_Mu_K.130422v1.root");
//   inputFileNames.push_back("../SimpleTMVAResultsAnalyzer/SimpleTMVAResultsAnalyzer__Histograms__background__TMVA_Mu_K.130422v1.root");
//   inputFileNames.push_back("../SimpleTMVAResultsAnalyzer/SimpleTMVAResultsAnalyzer__Histograms__signal__TMVA_Mu_K.130425v1.root");
  inputFileNames.push_back("../SimpleTMVAResultsAnalyzer/SimpleTMVAResultsAnalyzer__Histograms__background__TMVA_Mu_K.130425v1.root");
  
  // Output file
//     const std::string outputFileName("BasicDistributionsData__Histograms__BsToMuMu_Vs_BdToPiPi__muonV1BmmPt4__norm1.root");
//     const std::string outputFileName("BasicDistributionsData__Histograms__BsToMuMu_Vs_BsToKK__muonV1BmmPt4__norm4.root");
//   const std::string outputFileName("BasicDistributionsData__Histograms__BsToMuMu__muonV2BmmPt4_vs_muonV1TMVABmmPt4__norm0.root");
//   const std::string outputFileName("TMVAVariables_myMacro-V7C_vs_TMVATraining-130425v1_signal_muonV1BmmPt4_norm4.root");
  const std::string outputFileName("TMVAVariables_myMacro-V7C_vs_TMVATraining-130425v1_background_muonV1BmmPt4_norm4.root");
  
  // How to normalize
  // Normalization method can be
  // 0 - No normalization
  // 1 - Same area
  // 2 - Use the ratio of entries of a given histogram for all the histograms
  // 3 - Use the ratio of counts (= sum of bin entries times bin x-value) of a
  //     given histogram for all the histograms
  //     (In cases 2 and 3 the name of an histogram must be provided in the constructor)
  // 4 - Normalized to unity
  const unsigned short int normalizationMethod = 4;
  const std::string normalizationHistoName("hMuonDxySimplifiedTemplate_1");

  // Graphics definition
  const unsigned int lineStyles[10] = {0,0,0,0};
  const unsigned int markerStyles[10] = {21,20,22,23};
  const unsigned int colors[10] = {1,2,4,3};
  std::vector<std::string> legendLabels;
//   legendLabels.push_back("B_{s} #rightarrow #mu #mu - tight #mu");
//   legendLabels.push_back("B_{s} #rightarrow #mu #mu - TMVA #mu");
  //   legendLabels.push_back("B_{d} #rightarrow #pi #pi - tight #mu");
//   legendLabels.push_back("B_{d} #rightarrow #pi #pi - TMVA #mu");
//   legendLabels.push_back("B_{s} #rightarrow KK - tight #mu");
//   legendLabels.push_back("B_{s} #rightarrow KK - TMVA #mu");
//   legendLabels.push_back("B_{s} #rightarrow #mu #mu - muonV1BmmPt4");
//   legendLabels.push_back("B_{s} #rightarrow #mu #mu - muonV2BmmPt4");
//   legendLabels.push_back("B_{s} #rightarrow #mu #mu - muonV1TMVABmmPt4");
//   legendLabels.push_back("Real #mu - macro");
//   legendLabels.push_back("Real #mu - TMVA");
  legendLabels.push_back("K #rightarrow #mu - macro");
  legendLabels.push_back("K #rightarrow #mu - TMVA");
  
  const std::string canvasTitle("CMS Simulation");
//   const std::string canvasTitle("CMS Simulation, #sqrt{s} = 8 TeV");
//   const std::string canvasTitle("CMS Simulation, #sqrt{s} = 7 TeV");

//   legendLabels.push_back("Data - global #mu");
//   legendLabels.push_back("Data - tight #mu");
//   legendLabels.push_back("MC - global #mu");
//   legendLabels.push_back("MC - tight #mu");
  
  gSystem->CompileMacro("../utilities/HistogramComparator.C");
  HistogramComparator hc(inputFileNames, outputFileName, normalizationMethod, normalizationHistoName, lineStyles, markerStyles, colors, canvasTitle, legendLabels);
  hc.LoopOverHistograms();
  hc.LoopOverGraphs();
}
