// #include "../utilities/ImagesSaver.C"

void runImagesSaver(void)
{
  gSystem->CompileMacro("../utilities/ImagesSaver.C");

  // std::string inputFileName("MuonIPResolutions__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__muonV0Pt3_saved.root");
  // std::string inputFileName("MuonBasicDistributionsData__Histograms__Data_vs_MC_muonV2NOTMOSTPt4_norm1.root");
  // std::string inputFileName("/home/perrozzi/raid5/Work/Macros_PtRel/MuonBasicDistributionsMC/MuonBasicDistributionsMC__Histograms__MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__DoubleMu3_V3_skimmed__muonV2NOTMOSTPt4jetPFJetPt5.root");
  // std::string inputFileName("MuonBasicDistributionsData__Histograms__Data_2010_vs_2011_Aug5_muonV2NOTMOSTPt4_norm1.root");
  // std::string inputFileName("MuonBasicDistributionsData__Histograms__Data_2010_vs_2011_May10_muonV2NOTMOSTPt4_norm1.root");
//   std::string inputFileName("MuonBasicDistributionsData__Histograms__Data_2011_Aug5_vs_May10_muonV2NOTMOSTPt4_norm1.root");
//   std::string inputFileName("../HistogramComparator/BasicDistributionsData__Histograms__BdToPiPi__muonV1BmmPt4_vs_muonV1TMVABmmPt4__norm0.root");
  
  std::string inputFileName("../JetEnergyScaleTests/JetEnergyScaleTests__Histograms__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5__muonV6Pt4_jetPFV1Pt10.root");
  ImagesSaver is(inputFileName);
  // is.SaveAllHistograms("png");
  is.SaveAllCanvases("png");
  is.SaveAllCanvases("root");
  // is.SaveAllHistograms("png");
}
