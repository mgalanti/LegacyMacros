// To translate the various efficiency results into a standard format digestable by the macros
// Also "massages" some bins to have smoother values... // Maybe no

#include "TH2D.h"
#include "TFile.h"
#include <iostream>
#include "../utilities/HistoPlotter.C"



void MultiplyEfficiencies(const TH2D* hEff1, const TH2D* hEff1Hi, const TH2D* hEff1Lo,
                          const TH2D* hEff2, const TH2D* hEff2Hi, const TH2D* hEff2Lo,
                          TH2D*& hEff, TH2D*& hEffHi, TH2D*& hEffLo,
                          std::string name, std::string title, HistoPlotter* hp);

void runEfficienciesFileBuilder()
{
  HistoPlotter* hp = new HistoPlotter();
  
  std::string name = "";
  std::string title = "";
  
  std::string ptLabel = "p_{T} [GeV/c]";
  std::string etaLabel = "#eta";

  std::string genPtLabel = "gen p_{T} [GeV/c]";
  std::string genEtaLabel = "gen #eta";
  
  TFile* fTnpTrackingEff = new TFile("output_tracking_efficiencies_failingbins_20110513.root", "READ");
  TFile* fTnpRecoSelEff = new TFile("output_MuGLBplusQuality_efficiencies_failing_bins_20110513.root", "READ");
  TFile* fTnpTrgL1L2Eff = new TFile("output_Mu0L1L2_efficiencies_newDist120MB1_20110608.root", "READ");
  TFile* fTnpTrgHLTEff = new TFile("output_DoubleMu3L3_efficiencies_failingbins_20110513.root", "READ");
  TFile* fMCEff = new TFile("MuonEfficienciesComputerFromMCTruth__Histograms__MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_2__muonV2Pt4Eff.root");
  TFile* fOutput = new TFile("efficiencies_11_06_08_1.root","RECREATE");
  
  TH2D* hTkEffData0 = (TH2D*)fTnpTrackingEff->Get("hEff_DATA_central");
  TH2D* hTkEffData = hp->SymmetrizeHistogramInX(hTkEffData0);
  TH2D* hTkEffData_Hi0 = (TH2D*)fTnpTrackingEff->Get("hEff_DATA_upper");
  TH2D* hTkEffData_Hi = hp->SymmetrizeHistogramInX(hTkEffData_Hi0);
  TH2D* hTkEffData_Lo0 = (TH2D*)fTnpTrackingEff->Get("hEff_DATA_lower");
  TH2D* hTkEffData_Lo = hp->SymmetrizeHistogramInX(hTkEffData_Lo0);
  hTkEffData->SetName("hTkEffData");
  hTkEffData->SetTitle("Tracking efficiency from T&P in data");
  hTkEffData->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffData->GetYaxis()->SetTitle(ptLabel.c_str());
  hTkEffData_Hi->SetName("hTkEffData_Hi");
  hTkEffData_Hi->SetTitle("Tracking efficiency from T&P in data - Hi err");
  hTkEffData_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffData_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTkEffData_Lo->SetName("hTkEffData_Lo");
  hTkEffData_Lo->SetTitle("Tracking efficiency from T&P in data - Lo err");
  hTkEffData_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffData_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTkEffMC0 = (TH2D*)fTnpTrackingEff->Get("hEff_MC_central");
  TH2D* hTkEffMC = hp->SymmetrizeHistogramInX(hTkEffMC0);
  TH2D* hTkEffMC_Hi0 = (TH2D*)fTnpTrackingEff->Get("hEff_MC_upper");
  TH2D* hTkEffMC_Hi = hp->SymmetrizeHistogramInX(hTkEffMC_Hi0);
  TH2D* hTkEffMC_Lo0 = (TH2D*)fTnpTrackingEff->Get("hEff_MC_lower");
  TH2D* hTkEffMC_Lo = hp->SymmetrizeHistogramInX(hTkEffMC_Lo0);
  hTkEffMC->SetName("hTkEffMC");
  hTkEffMC->SetTitle("Tracking efficiency from T&P in MC");
  hTkEffMC->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMC->GetYaxis()->SetTitle(ptLabel.c_str());
  hTkEffMC_Hi->SetName("hTkEffMC_Hi");
  hTkEffMC_Hi->SetTitle("Tracking efficiency from T&P in MC - Hi err");
  hTkEffMC_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMC_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTkEffMC_Lo->SetName("hTkEffMC_Lo");
  hTkEffMC_Lo->SetTitle("Tracking efficiency from T&P in MC - Lo err");
  hTkEffMC_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMC_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTkEffMCTruth0 = (TH2D*)fTnpTrackingEff->Get("hEff_mctruth_central");
  TH2D* hTkEffMCTruth = hp->SymmetrizeHistogramInX(hTkEffMCTruth0);
  TH2D* hTkEffMCTruth_Hi0 = (TH2D*)fTnpTrackingEff->Get("hEff_mctruth_upper");
  TH2D* hTkEffMCTruth_Hi = hp->SymmetrizeHistogramInX(hTkEffMCTruth_Hi0);
  TH2D* hTkEffMCTruth_Lo0 = (TH2D*)fTnpTrackingEff->Get("hEff_mctruth_lower");
  TH2D* hTkEffMCTruth_Lo = hp->SymmetrizeHistogramInX(hTkEffMCTruth_Lo0);
  hTkEffMCTruth->SetName("hTkEffMCTruth");
  hTkEffMCTruth->SetTitle("Tracking efficiency from T&P MC truth");
  hTkEffMCTruth->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMCTruth->GetYaxis()->SetTitle(ptLabel.c_str());
  hTkEffMCTruth_Hi->SetName("hTkEffMCTruth_Hi");
  hTkEffMCTruth_Hi->SetTitle("Tracking efficiency from T&P MC truth - Hi err");
  hTkEffMCTruth_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMCTruth_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTkEffMCTruth_Lo->SetName("hTkEffMCTruth_Lo");
  hTkEffMCTruth_Lo->SetTitle("Tracking efficiency from T&P MC truth - Lo err");
  hTkEffMCTruth_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMCTruth_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTkEffMCTruthBB = (TH2D*)fMCEff->Get("hGoodTrackOverGenMuVsGenPtEtaSC_1");
  TH2D* hTkEffMCTruthBB_Hi = (TH2D*)fMCEff->Get("hGoodTrackOverGenMuVsGenPtEtaHiSC_1");
  TH2D* hTkEffMCTruthBB_Lo = (TH2D*)fMCEff->Get("hGoodTrackOverGenMuVsGenPtEtaLoSC_1");
  hTkEffMCTruthBB->SetName("hTkEffMCTruthBB");
  hTkEffMCTruthBB->SetTitle("Tracking efficiency from b #bar{b} MC truth");
  hTkEffMCTruthBB->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMCTruthBB->GetYaxis()->SetTitle(ptLabel.c_str());
  hTkEffMCTruthBB_Hi->SetName("hTkEffMCTruthBB_Hi");
  hTkEffMCTruthBB_Hi->SetTitle("Tracking efficiency from b #bar{b} MC truth - Hi err");
  hTkEffMCTruthBB_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMCTruthBB_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTkEffMCTruthBB_Lo->SetName("hTkEffMCTruthBB_Lo");
  hTkEffMCTruthBB_Lo->SetTitle("Tracking efficiency from b #bar{b} MC truth - Lo err");
  hTkEffMCTruthBB_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTkEffMCTruthBB_Lo->GetYaxis()->SetTitle(ptLabel.c_str());

  TH2D* hRecoSelEffData0 = (TH2D*)fTnpRecoSelEff->Get("hEff_DATA_central");
  TH2D* hRecoSelEffData = hp->SymmetrizeHistogramInX(hRecoSelEffData0);
  TH2D* hRecoSelEffData_Hi0 = (TH2D*)fTnpRecoSelEff->Get("hEff_DATA_upper");
  TH2D* hRecoSelEffData_Hi = hp->SymmetrizeHistogramInX(hRecoSelEffData_Hi0);
  TH2D* hRecoSelEffData_Lo0 = (TH2D*)fTnpRecoSelEff->Get("hEff_DATA_lower");
  TH2D* hRecoSelEffData_Lo = hp->SymmetrizeHistogramInX(hRecoSelEffData_Lo0);
  hRecoSelEffData->SetName("hRecoSelEffData");
  hRecoSelEffData->SetTitle("#mu reco + sel efficiency from T&P in data");
  hRecoSelEffData->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffData->GetYaxis()->SetTitle(ptLabel.c_str());
  hRecoSelEffData_Hi->SetName("hRecoSelEffData_Hi");
  hRecoSelEffData_Hi->SetTitle("#mu reco + sel efficiency from T&P in data - Hi err");
  hRecoSelEffData_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffData_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hRecoSelEffData_Lo->SetName("hRecoSelEffData_Lo");
  hRecoSelEffData_Lo->SetTitle("#mu reco + sel efficiency from T&P in data - Lo err");
  hRecoSelEffData_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffData_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hRecoSelEffMC0 = (TH2D*)fTnpRecoSelEff->Get("hEff_MC_central");
  TH2D* hRecoSelEffMC = hp->SymmetrizeHistogramInX(hRecoSelEffMC0);
  TH2D* hRecoSelEffMC_Hi0 = (TH2D*)fTnpRecoSelEff->Get("hEff_MC_upper");
  TH2D* hRecoSelEffMC_Hi = hp->SymmetrizeHistogramInX(hRecoSelEffMC_Hi0);
  TH2D* hRecoSelEffMC_Lo0 = (TH2D*)fTnpRecoSelEff->Get("hEff_MC_lower");
  TH2D* hRecoSelEffMC_Lo = hp->SymmetrizeHistogramInX(hRecoSelEffMC_Lo0);
  hRecoSelEffMC->SetName("hRecoSelEffMC");
  hRecoSelEffMC->SetTitle("#mu reco + sel efficiency from T&P in MC");
  hRecoSelEffMC->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMC->GetYaxis()->SetTitle(ptLabel.c_str());
  hRecoSelEffMC_Hi->SetName("hRecoSelEffMC_Hi");
  hRecoSelEffMC_Hi->SetTitle("#mu reco + sel efficiency from T&P in MC - Hi err");
  hRecoSelEffMC_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMC_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hRecoSelEffMC_Lo->SetName("hRecoSelEffMC_Lo");
  hRecoSelEffMC_Lo->SetTitle("#mu reco + sel efficiency from T&P in MC - Lo err");
  hRecoSelEffMC_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMC_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hRecoSelEffMCTruth0 = (TH2D*)fTnpRecoSelEff->Get("hEff_mctruth_central");
  TH2D* hRecoSelEffMCTruth = hp->SymmetrizeHistogramInX(hRecoSelEffMCTruth0);
  TH2D* hRecoSelEffMCTruth_Hi0 = (TH2D*)fTnpRecoSelEff->Get("hEff_mctruth_upper");
  TH2D* hRecoSelEffMCTruth_Hi = hp->SymmetrizeHistogramInX(hRecoSelEffMCTruth_Hi0);
  TH2D* hRecoSelEffMCTruth_Lo0 = (TH2D*)fTnpRecoSelEff->Get("hEff_mctruth_lower");
  TH2D* hRecoSelEffMCTruth_Lo = hp->SymmetrizeHistogramInX(hRecoSelEffMCTruth_Lo0);
  hRecoSelEffMCTruth->SetName("hRecoSelEffMCTruth");
  hRecoSelEffMCTruth->SetTitle("#mu reco + sel efficiency from T&P MC truth");
  hRecoSelEffMCTruth->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMCTruth->GetYaxis()->SetTitle(ptLabel.c_str());
  hRecoSelEffMCTruth_Hi->SetName("hRecoSelEffMCTruth_Hi");
  hRecoSelEffMCTruth_Hi->SetTitle("#mu reco + sel efficiency from T&P MC truth - Hi err");
  hRecoSelEffMCTruth_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMCTruth_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hRecoSelEffMCTruth_Lo->SetName("hRecoSelEffMCTruth_Lo");
  hRecoSelEffMCTruth_Lo->SetTitle("#mu reco + sel efficiency from T&P MC truth - Lo err");
  hRecoSelEffMCTruth_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMCTruth_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hRecoSelEffMCTruthBB = (TH2D*)fMCEff->Get("hGoodSelOverGoodTrackVsRecoPtEtaSC_1");
  TH2D* hRecoSelEffMCTruthBB_Hi = (TH2D*)fMCEff->Get("hGoodSelOverGoodTrackVsRecoPtEtaHiSC_1");
  TH2D* hRecoSelEffMCTruthBB_Lo = (TH2D*)fMCEff->Get("hGoodSelOverGoodTrackVsRecoPtEtaLoSC_1");
  hRecoSelEffMCTruthBB->SetName("hRecoSelEffMCTruthBB");
  hRecoSelEffMCTruthBB->SetTitle("#mu reco + sel efficiency from b #bar{b} MC truth");
  hRecoSelEffMCTruthBB->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMCTruthBB->GetYaxis()->SetTitle(ptLabel.c_str());
  hRecoSelEffMCTruthBB_Hi->SetName("hRecoSelEffMCTruthBB_Hi");
  hRecoSelEffMCTruthBB_Hi->SetTitle("#mu reco + sel efficiency from b #bar{b} MC truth - Hi err");
  hRecoSelEffMCTruthBB_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMCTruthBB_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hRecoSelEffMCTruthBB_Lo->SetName("hRecoSelEffMCTruthBB_Lo");
  hRecoSelEffMCTruthBB_Lo->SetTitle("#mu reco + sel efficiency from b #bar{b} MC truth - Lo err");
  hRecoSelEffMCTruthBB_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hRecoSelEffMCTruthBB_Lo->GetYaxis()->SetTitle(ptLabel.c_str());

  TH2D* hRecoSelEffMCTruthBBGen = (TH2D*)fMCEff->Get("hGoodSelOverGoodTrackVsGenPtEtaSC_1");
  TH2D* hRecoSelEffMCTruthBBGen_Hi = (TH2D*)fMCEff->Get("hGoodSelOverGoodTrackVsGenPtEtaHiSC_1");
  TH2D* hRecoSelEffMCTruthBBGen_Lo = (TH2D*)fMCEff->Get("hGoodSelOverGoodTrackVsGenPtEtaLoSC_1");
  hRecoSelEffMCTruthBBGen->SetName("hRecoSelEffMCTruthBBGen");
  hRecoSelEffMCTruthBBGen->SetTitle("#mu reco + sel efficiency from b #bar{b} MC truth - gen vars");
  hRecoSelEffMCTruthBBGen->GetXaxis()->SetTitle(genEtaLabel.c_str());
  hRecoSelEffMCTruthBBGen->GetYaxis()->SetTitle(genPtLabel.c_str());
  hRecoSelEffMCTruthBBGen_Hi->SetName("hRecoSelEffMCTruthBBGen_Hi");
  hRecoSelEffMCTruthBBGen_Hi->SetTitle("#mu reco + sel efficiency from b #bar{b} MC truth - gen vars - Hi err");
  hRecoSelEffMCTruthBBGen_Hi->GetXaxis()->SetTitle(genEtaLabel.c_str());
  hRecoSelEffMCTruthBBGen_Hi->GetYaxis()->SetTitle(genPtLabel.c_str());
  hRecoSelEffMCTruthBBGen_Lo->SetName("hRecoSelEffMCTruthBBGen_Lo");
  hRecoSelEffMCTruthBBGen_Lo->SetTitle("#mu reco + sel efficiency from b #bar{b} MC truth - gen vars - Lo err");
  hRecoSelEffMCTruthBBGen_Lo->GetXaxis()->SetTitle(genEtaLabel.c_str());
  hRecoSelEffMCTruthBBGen_Lo->GetYaxis()->SetTitle(genPtLabel.c_str());
  
  TH2D* hTrgL1L2EffData0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_DATA_central");
  TH2D* hTrgL1L2EffData = hp->SymmetrizeHistogramInX(hTrgL1L2EffData0);
  TH2D* hTrgL1L2EffData_Hi0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_DATA_upper");
  TH2D* hTrgL1L2EffData_Hi = hp->SymmetrizeHistogramInX(hTrgL1L2EffData_Hi0);
  TH2D* hTrgL1L2EffData_Lo0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_DATA_lower");
  TH2D* hTrgL1L2EffData_Lo = hp->SymmetrizeHistogramInX(hTrgL1L2EffData_Lo0);
  hTrgL1L2EffData->SetName("hTrgL1L2EffData");
  hTrgL1L2EffData->SetTitle("#mu trg L1 + L2 efficiency from T&P in data");
  hTrgL1L2EffData->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffData->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgL1L2EffData_Hi->SetName("hTrgL1L2EffData_Hi");
  hTrgL1L2EffData_Hi->SetTitle("#mu trg L1 + L2 efficiency from T&P in data - Hi err");
  hTrgL1L2EffData_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffData_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgL1L2EffData_Lo->SetName("hTrgL1L2EffData_Lo");
  hTrgL1L2EffData_Lo->SetTitle("#mu trg L1 + L2 efficiency from T&P in data - Lo err");
  hTrgL1L2EffData_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffData_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgL1L2EffMC0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_MC_central");
  TH2D* hTrgL1L2EffMC = hp->SymmetrizeHistogramInX(hTrgL1L2EffMC0);
  TH2D* hTrgL1L2EffMC_Hi0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_MC_upper");
  TH2D* hTrgL1L2EffMC_Hi = hp->SymmetrizeHistogramInX(hTrgL1L2EffMC_Hi0);
  TH2D* hTrgL1L2EffMC_Lo0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_MC_lower");
  TH2D* hTrgL1L2EffMC_Lo = hp->SymmetrizeHistogramInX(hTrgL1L2EffMC_Lo0);
  hTrgL1L2EffMC->SetName("hTrgL1L2EffMC");
  hTrgL1L2EffMC->SetTitle("#mu trg L1 + L2 efficiency from T&P in MC");
  hTrgL1L2EffMC->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffMC->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgL1L2EffMC_Hi->SetName("hTrgL1L2EffMC_Hi");
  hTrgL1L2EffMC_Hi->SetTitle("#mu trg L1 + L2 efficiency from T&P in MC - Hi err");
  hTrgL1L2EffMC_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffMC_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgL1L2EffMC_Lo->SetName("hTrgL1L2EffMC_Lo");
  hTrgL1L2EffMC_Lo->SetTitle("#mu trg L1 + L2 efficiency from T&P in MC - Lo err");
  hTrgL1L2EffMC_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffMC_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgL1L2EffMCTruth0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_mctruth_central");
  TH2D* hTrgL1L2EffMCTruth = hp->SymmetrizeHistogramInX(hTrgL1L2EffMCTruth0);
  TH2D* hTrgL1L2EffMCTruth_Hi0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_mctruth_upper");
  TH2D* hTrgL1L2EffMCTruth_Hi = hp->SymmetrizeHistogramInX(hTrgL1L2EffMCTruth_Hi0);
  TH2D* hTrgL1L2EffMCTruth_Lo0 = (TH2D*)fTnpTrgL1L2Eff->Get("hEff_mctruth_lower");
  TH2D* hTrgL1L2EffMCTruth_Lo = hp->SymmetrizeHistogramInX(hTrgL1L2EffMCTruth_Lo0);
  hTrgL1L2EffMCTruth->SetName("hTrgL1L2EffMCTruth");
  hTrgL1L2EffMCTruth->SetTitle("#mu trg L1 + L2 efficiency from T&P in MC truth");
  hTrgL1L2EffMCTruth->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffMCTruth->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgL1L2EffMCTruth_Hi->SetName("hTrgL1L2EffMCTruth_Hi");
  hTrgL1L2EffMCTruth_Hi->SetTitle("#mu trg L1 + L2 efficiency from T&P in MC truth - Hi err");
  hTrgL1L2EffMCTruth_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffMCTruth_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgL1L2EffMCTruth_Lo->SetName("hTrgL1L2EffMCTruth_Lo");
  hTrgL1L2EffMCTruth_Lo->SetTitle("#mu trg L1 + L2 efficiency from T&P in MC truth - Lo err");
  hTrgL1L2EffMCTruth_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgL1L2EffMCTruth_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgHLTEffData0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_DATA_central");
  TH2D* hTrgHLTEffData = hp->SymmetrizeHistogramInX(hTrgHLTEffData0);
  TH2D* hTrgHLTEffData_Hi0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_DATA_upper");
  TH2D* hTrgHLTEffData_Hi = hp->SymmetrizeHistogramInX(hTrgHLTEffData_Hi0);
  TH2D* hTrgHLTEffData_Lo0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_DATA_lower");
  TH2D* hTrgHLTEffData_Lo = hp->SymmetrizeHistogramInX(hTrgHLTEffData_Lo0);
  hTrgHLTEffData->SetName("hTrgHLTEffData");
  hTrgHLTEffData->SetTitle("#mu trg HLT efficiency from T&P in data");
  hTrgHLTEffData->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffData->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgHLTEffData_Hi->SetName("hTrgHLTEffData_Hi");
  hTrgHLTEffData_Hi->SetTitle("#mu trg HLT efficiency from T&P in data - Hi err");
  hTrgHLTEffData_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffData_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgHLTEffData_Lo->SetName("hTrgHLTEffData_Lo");
  hTrgHLTEffData_Lo->SetTitle("#mu trg HLT efficiency from T&P in data - Lo err");
  hTrgHLTEffData_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffData_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgHLTEffMC0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_MC_central");
  TH2D* hTrgHLTEffMC = hp->SymmetrizeHistogramInX(hTrgHLTEffMC0);
  TH2D* hTrgHLTEffMC_Hi0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_MC_upper");
  TH2D* hTrgHLTEffMC_Hi = hp->SymmetrizeHistogramInX(hTrgHLTEffMC_Hi0);
  TH2D* hTrgHLTEffMC_Lo0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_MC_lower");
  TH2D* hTrgHLTEffMC_Lo = hp->SymmetrizeHistogramInX(hTrgHLTEffMC_Lo0);
  hTrgHLTEffMC->SetName("hTrgHLTEffMC");
  hTrgHLTEffMC->SetTitle("#mu trg HLT efficiency from T&P in MC");
  hTrgHLTEffMC->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffMC->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgHLTEffMC_Hi->SetName("hTrgHLTEffMC_Hi");
  hTrgHLTEffMC_Hi->SetTitle("#mu trg HLT efficiency from T&P in MC - Hi err");
  hTrgHLTEffMC_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffMC_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgHLTEffMC_Lo->SetName("hTrgHLTEffMC_Lo");
  hTrgHLTEffMC_Lo->SetTitle("#mu trg HLT efficiency from T&P in MC - Lo err");
  hTrgHLTEffMC_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffMC_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgHLTEffMCTruth0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_mctruth_central");
  TH2D* hTrgHLTEffMCTruth = hp->SymmetrizeHistogramInX(hTrgHLTEffMCTruth0);
  TH2D* hTrgHLTEffMCTruth_Hi0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_mctruth_upper");
  TH2D* hTrgHLTEffMCTruth_Hi = hp->SymmetrizeHistogramInX(hTrgHLTEffMCTruth_Hi0);
  TH2D* hTrgHLTEffMCTruth_Lo0 = (TH2D*)fTnpTrgHLTEff->Get("hEff_mctruth_lower");
  TH2D* hTrgHLTEffMCTruth_Lo = hp->SymmetrizeHistogramInX(hTrgHLTEffMCTruth_Lo0);
  hTrgHLTEffMCTruth->SetName("hTrgHLTEffMCTruth");
  hTrgHLTEffMCTruth->SetTitle("#mu trg HLT efficiency from T&P in MC truth");
  hTrgHLTEffMCTruth->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffMCTruth->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgHLTEffMCTruth_Hi->SetName("hTrgHLTEffMCTruth_Hi");
  hTrgHLTEffMCTruth_Hi->SetTitle("#mu trg HLT efficiency from T&P in MC truth - Hi err");
  hTrgHLTEffMCTruth_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffMCTruth_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgHLTEffMCTruth_Lo->SetName("hTrgHLTEffMCTruth_Lo");
  hTrgHLTEffMCTruth_Lo->SetTitle("#mu trg HLT efficiency from T&P in MC truth - Lo err");
  hTrgHLTEffMCTruth_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgHLTEffMCTruth_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgEffData = 0;
  TH2D* hTrgEffData_Hi = 0;
  TH2D* hTrgEffData_Lo = 0;
  name = "hTrgEffData";
  title = "Trigger efficiency from T&P in data";
  MultiplyEfficiencies(hTrgL1L2EffData, hTrgL1L2EffData_Hi, hTrgL1L2EffData_Lo,
                       hTrgHLTEffData, hTrgHLTEffData_Hi, hTrgHLTEffData_Lo,
                       hTrgEffData, hTrgEffData_Hi, hTrgEffData_Lo, name, title, hp);
  hTrgEffData->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffData->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgEffData_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffData_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgEffData_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffData_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgEffMC = 0;
  TH2D* hTrgEffMC_Hi = 0;
  TH2D* hTrgEffMC_Lo = 0;
  name = "hTrgEffMC";
  title = "Trigger efficiencies from T&P in MC";
  MultiplyEfficiencies(hTrgL1L2EffMC, hTrgL1L2EffMC_Hi, hTrgL1L2EffMC_Lo,
                       hTrgHLTEffMC, hTrgHLTEffMC_Hi, hTrgHLTEffMC_Lo,
                       hTrgEffMC, hTrgEffMC_Hi, hTrgEffMC_Lo, name, title, hp);
  hTrgEffMC->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMC->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgEffMC_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMC_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgEffMC_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMC_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgEffMCTruth = 0;
  TH2D* hTrgEffMCTruth_Hi = 0;
  TH2D* hTrgEffMCTruth_Lo = 0;
  name = "hTrgEffMCTruth";
  title = "Trigger efficiencies from T&P MC truth";
  MultiplyEfficiencies(hTrgL1L2EffMCTruth, hTrgL1L2EffMCTruth_Hi, hTrgL1L2EffMCTruth_Lo,
                       hTrgHLTEffMCTruth, hTrgHLTEffMCTruth_Hi, hTrgHLTEffMCTruth_Lo,
                       hTrgEffMCTruth, hTrgEffMCTruth_Hi, hTrgEffMCTruth_Lo, name, title, hp);
  hTrgEffMCTruth->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMCTruth->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgEffMCTruth_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMCTruth_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgEffMCTruth_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMCTruth_Lo->GetYaxis()->SetTitle(ptLabel.c_str());
  
  TH2D* hTrgEffMCTruthBB = (TH2D*)fMCEff->Get("hGoodTrgOverGoodSelMuVsRecoPtEtaSC_1");
  TH2D* hTrgEffMCTruthBB_Hi = (TH2D*)fMCEff->Get("hGoodTrgOverGoodSelMuVsRecoPtEtaHiSC_1");
  TH2D* hTrgEffMCTruthBB_Lo = (TH2D*)fMCEff->Get("hGoodTrgOverGoodSelMuVsRecoPtEtaLoSC_1");
  hTrgEffMCTruthBB->SetName("hTrgEffMCTruthBB");
  hTrgEffMCTruthBB->SetTitle("Trigger efficiency from b #bar{b} MC truth");
  hTrgEffMCTruthBB->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMCTruthBB->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgEffMCTruthBB_Hi->SetName("hTrgEffMCTruthBB_Hi");
  hTrgEffMCTruthBB_Hi->SetTitle("Trigger efficiency from b #bar{b} MC truth - Hi err");
  hTrgEffMCTruthBB_Hi->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMCTruthBB_Hi->GetYaxis()->SetTitle(ptLabel.c_str());
  hTrgEffMCTruthBB_Lo->SetName("hTrgEffMCTruthBB_Lo");
  hTrgEffMCTruthBB_Lo->SetTitle("Trigger efficiency from b #bar{b} MC truth - Lo err");
  hTrgEffMCTruthBB_Lo->GetXaxis()->SetTitle(etaLabel.c_str());
  hTrgEffMCTruthBB_Lo->GetYaxis()->SetTitle(ptLabel.c_str());

  TH2D* hTrgEffMCTruthBBGen = (TH2D*)fMCEff->Get("hGoodTrgOverGoodSelMuVsGenPtEtaSC_1");
  TH2D* hTrgEffMCTruthBBGen_Hi = (TH2D*)fMCEff->Get("hGoodTrgOverGoodSelMuVsGenPtEtaHiSC_1");
  TH2D* hTrgEffMCTruthBBGen_Lo = (TH2D*)fMCEff->Get("hGoodTrgOverGoodSelMuVsGenPtEtaLoSC_1");
  hTrgEffMCTruthBBGen->SetName("hTrgEffMCTruthBBGen");
  hTrgEffMCTruthBBGen->SetTitle("Trigger efficiency from b #bar{b} MC truth - gen vars");
  hTrgEffMCTruthBBGen->GetXaxis()->SetTitle(genEtaLabel.c_str());
  hTrgEffMCTruthBBGen->GetYaxis()->SetTitle(genPtLabel.c_str());
  hTrgEffMCTruthBBGen_Hi->SetName("hTrgEffMCTruthBBGen_Hi");
  hTrgEffMCTruthBBGen_Hi->SetTitle("Trigger efficiency from b #bar{b} MC truth - gen vars - Hi err");
  hTrgEffMCTruthBBGen_Hi->GetXaxis()->SetTitle(genEtaLabel.c_str());
  hTrgEffMCTruthBBGen_Hi->GetYaxis()->SetTitle(genPtLabel.c_str());
  hTrgEffMCTruthBBGen_Lo->SetName("hTrgEffMCTruthBBGen_Lo");
  hTrgEffMCTruthBBGen_Lo->SetTitle("Trigger efficiency from b #bar{b} MC truth - gen vars - Lo err");
  hTrgEffMCTruthBBGen_Lo->GetXaxis()->SetTitle(genEtaLabel.c_str());
  hTrgEffMCTruthBBGen_Lo->GetYaxis()->SetTitle(genPtLabel.c_str());
  
  fOutput->cd();

  hTkEffData->Write();
  hTkEffData_Hi->Write();
  hTkEffData_Lo->Write();
  hp->CreateAndWriteCanvas("cTkEffData","COLZ",false,false,false,hTkEffData);
  hp->CreateAndWriteCanvas("cTkEffData_Hi","COLZ",false,false,false,hTkEffData_Hi);
  hp->CreateAndWriteCanvas("cTkEffData_Lo","COLZ",false,false,false,hTkEffData_Lo);
  
  hTkEffMC->Write();
  hTkEffMC_Hi->Write();
  hTkEffMC_Lo->Write();
  hp->CreateAndWriteCanvas("cTkEffMC","COLZ",false,false,false,hTkEffMC);
  hp->CreateAndWriteCanvas("cTkEffMC_Hi","COLZ",false,false,false,hTkEffMC_Hi);
  hp->CreateAndWriteCanvas("cTkEffMC_Lo","COLZ",false,false,false,hTkEffMC_Lo);
  
  hTkEffMCTruth->Write();
  hTkEffMCTruth_Hi->Write();
  hTkEffMCTruth_Lo->Write();
  hp->CreateAndWriteCanvas("cTkEffMCTruth","COLZ",false,false,false,hTkEffMCTruth);
  hp->CreateAndWriteCanvas("cTkEffMCTruth_Hi","COLZ",false,false,false,hTkEffMCTruth_Hi);
  hp->CreateAndWriteCanvas("cTkEffMCTruth_Lo","COLZ",false,false,false,hTkEffMCTruth_Lo);
  
  hTkEffMCTruthBB->Write();
  hTkEffMCTruthBB_Hi->Write();
  hTkEffMCTruthBB_Lo->Write();
  hp->CreateAndWriteCanvas("cTkEffMCTruthBB","COLZ",false,false,false,hTkEffMCTruthBB);
  hp->CreateAndWriteCanvas("cTkEffMCTruthBB_Hi","COLZ",false,false,false,hTkEffMCTruthBB_Hi);
  hp->CreateAndWriteCanvas("cTkEffMCTruthBB_Lo","COLZ",false,false,false,hTkEffMCTruthBB_Lo);
  
  hRecoSelEffData->Write();
  hRecoSelEffData_Hi->Write();
  hRecoSelEffData_Lo->Write();
  hp->CreateAndWriteCanvas("cRecoSelEffData","COLZ",false,false,false,hRecoSelEffData);
  hp->CreateAndWriteCanvas("cRecoSelEffData_Hi","COLZ",false,false,false,hRecoSelEffData_Hi);
  hp->CreateAndWriteCanvas("cRecoSelEffData_Lo","COLZ",false,false,false,hRecoSelEffData_Lo);
  
  hRecoSelEffMC->Write();
  hRecoSelEffMC_Hi->Write();
  hRecoSelEffMC_Lo->Write();
  hp->CreateAndWriteCanvas("cRecoSelEffMC","COLZ",false,false,false,hRecoSelEffMC);
  hp->CreateAndWriteCanvas("cRecoSelEffMC_Hi","COLZ",false,false,false,hRecoSelEffMC_Hi);
  hp->CreateAndWriteCanvas("cRecoSelEffMC_Lo","COLZ",false,false,false,hRecoSelEffMC_Lo);
  
  hRecoSelEffMCTruth->Write();
  hRecoSelEffMCTruth_Hi->Write();
  hRecoSelEffMCTruth_Lo->Write();
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruth","COLZ",false,false,false,hRecoSelEffMCTruth);
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruth_Hi","COLZ",false,false,false,hRecoSelEffMCTruth_Hi);
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruth_Lo","COLZ",false,false,false,hRecoSelEffMCTruth_Lo);
  
  hRecoSelEffMCTruthBB->Write();
  hRecoSelEffMCTruthBB_Hi->Write();
  hRecoSelEffMCTruthBB_Lo->Write();
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruthBB","COLZ",false,false,false,hRecoSelEffMCTruthBB);
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruthBB_Hi","COLZ",false,false,false,hRecoSelEffMCTruthBB_Hi);
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruthBB_Lo","COLZ",false,false,false,hRecoSelEffMCTruthBB_Lo);
  
  hRecoSelEffMCTruthBBGen->Write();
  hRecoSelEffMCTruthBBGen_Hi->Write();
  hRecoSelEffMCTruthBBGen_Lo->Write();
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruthBBGen","COLZ",false,false,false,hRecoSelEffMCTruthBBGen);
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruthBBGen_Hi","COLZ",false,false,false,hRecoSelEffMCTruthBBGen_Hi);
  hp->CreateAndWriteCanvas("cRecoSelEffMCTruthBBGen_Lo","COLZ",false,false,false,hRecoSelEffMCTruthBBGen_Lo);
  
  hTrgL1L2EffData->Write();
  hTrgL1L2EffData_Hi->Write();
  hTrgL1L2EffData_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgL1L2EffData","COLZ",false,false,false,hTrgL1L2EffData);
  hp->CreateAndWriteCanvas("cTrgL1L2EffData_Hi","COLZ",false,false,false,hTrgL1L2EffData_Hi);
  hp->CreateAndWriteCanvas("cTrgL1L2EffData_Lo","COLZ",false,false,false,hTrgL1L2EffData_Lo);
  
  hTrgL1L2EffMC->Write();
  hTrgL1L2EffMC_Hi->Write();
  hTrgL1L2EffMC_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgL1L2EffMC","COLZ",false,false,false,hTrgL1L2EffMC);
  hp->CreateAndWriteCanvas("cTrgL1L2EffMC_Hi","COLZ",false,false,false,hTrgL1L2EffMC_Hi);
  hp->CreateAndWriteCanvas("cTrgL1L2EffMC_Lo","COLZ",false,false,false,hTrgL1L2EffMC_Lo);
  
  hTrgL1L2EffMCTruth->Write();
  hTrgL1L2EffMCTruth_Hi->Write();
  hTrgL1L2EffMCTruth_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgL1L2EffMCTruth","COLZ",false,false,false,hTrgL1L2EffMCTruth);
  hp->CreateAndWriteCanvas("cTrgL1L2EffMCTruth_Hi","COLZ",false,false,false,hTrgL1L2EffMCTruth_Hi);
  hp->CreateAndWriteCanvas("cTrgL1L2EffMCTruth_Lo","COLZ",false,false,false,hTrgL1L2EffMCTruth_Lo);
  
  hTrgHLTEffData->Write();
  hTrgHLTEffData_Hi->Write();
  hTrgHLTEffData_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgHLTEffData","COLZ",false,false,false,hTrgHLTEffData);
  hp->CreateAndWriteCanvas("cTrgHLTEffData_Hi","COLZ",false,false,false,hTrgHLTEffData_Hi);
  hp->CreateAndWriteCanvas("cTrgHLTEffData_Lo","COLZ",false,false,false,hTrgHLTEffData_Lo);
  
  hTrgHLTEffMC->Write();
  hTrgHLTEffMC_Hi->Write();
  hTrgHLTEffMC_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgHLTEffMC","COLZ",false,false,false,hTrgHLTEffMC);
  hp->CreateAndWriteCanvas("cTrgHLTEffMC_Hi","COLZ",false,false,false,hTrgHLTEffMC_Hi);
  hp->CreateAndWriteCanvas("cTrgHLTEffMC_Lo","COLZ",false,false,false,hTrgHLTEffMC_Lo);
  
  hTrgHLTEffMCTruth->Write();
  hTrgHLTEffMCTruth_Hi->Write();
  hTrgHLTEffMCTruth_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgHLTEffMCTruth","COLZ",false,false,false,hTrgHLTEffMCTruth);
  hp->CreateAndWriteCanvas("cTrgHLTEffMCTruth_Hi","COLZ",false,false,false,hTrgHLTEffMCTruth_Hi);
  hp->CreateAndWriteCanvas("cTrgHLTEffMCTruth_Lo","COLZ",false,false,false,hTrgHLTEffMCTruth_Lo);
  
  hTrgEffData->Write();
  hTrgEffData_Hi->Write();
  hTrgEffData_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgEffData","COLZ",false,false,false,hTrgEffData);
  hp->CreateAndWriteCanvas("cTrgEffData_Hi","COLZ",false,false,false,hTrgEffData_Hi);
  hp->CreateAndWriteCanvas("cTrgEffData_Lo","COLZ",false,false,false,hTrgEffData_Lo);
  
  hTrgEffMC->Write();
  hTrgEffMC_Hi->Write();
  hTrgEffMC_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgEffMC","COLZ",false,false,false,hTrgEffMC);
  hp->CreateAndWriteCanvas("cTrgEffMC_Hi","COLZ",false,false,false,hTrgEffMC_Hi);
  hp->CreateAndWriteCanvas("cTrgEffMC_Lo","COLZ",false,false,false,hTrgEffMC_Lo);
  
  hTrgEffMCTruth->Write();
  hTrgEffMCTruth_Hi->Write();
  hTrgEffMCTruth_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgEffMCTruth","COLZ",false,false,false,hTrgEffMCTruth);
  hp->CreateAndWriteCanvas("cTrgEffMCTruth_Hi","COLZ",false,false,false,hTrgEffMCTruth_Hi);
  hp->CreateAndWriteCanvas("cTrgEffMCTruth_Lo","COLZ",false,false,false,hTrgEffMCTruth_Lo);
  
  hTrgEffMCTruthBB->Write();
  hTrgEffMCTruthBB_Hi->Write();
  hTrgEffMCTruthBB_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgEffMCTruthBB","COLZ",false,false,false,hTrgEffMCTruthBB);
  hp->CreateAndWriteCanvas("cTrgEffMCTruthBB_Hi","COLZ",false,false,false,hTrgEffMCTruthBB_Hi);
  hp->CreateAndWriteCanvas("cTrgEffMCTruthBB_Lo","COLZ",false,false,false,hTrgEffMCTruthBB_Lo);
  
  hTrgEffMCTruthBBGen->Write();
  hTrgEffMCTruthBBGen_Hi->Write();
  hTrgEffMCTruthBBGen_Lo->Write();
  hp->CreateAndWriteCanvas("cTrgEffMCTruthBBGen","COLZ",false,false,false,hTrgEffMCTruthBBGen);
  hp->CreateAndWriteCanvas("cTrgEffMCTruthBBGen_Hi","COLZ",false,false,false,hTrgEffMCTruthBBGen_Hi);
  hp->CreateAndWriteCanvas("cTrgEffMCTruthBBGen_Lo","COLZ",false,false,false,hTrgEffMCTruthBBGen_Lo);
  
  fOutput->Close();
  fTnpTrackingEff->Close();
  fTnpRecoSelEff->Close();
  fTnpTrgL1L2Eff->Close();
  fTnpTrgHLTEff->Close();
  fMCEff->Close();
}



void MultiplyEfficiencies(const TH2D* hEff1, const TH2D* hEff1Hi, const TH2D* hEff1Lo,
                          const TH2D* hEff2, const TH2D* hEff2Hi, const TH2D* hEff2Lo,
                          TH2D*& hEff, TH2D*& hEffHi, TH2D*& hEffLo,
                          std::string name, std::string title, HistoPlotter* hp)
{
  // First check that efficiency and error histograms have the same binning
  if(!hp->SameBins(hEff1, hEff1Hi) || !hp->SameBins(hEff1, hEff1Lo))
  {
    std::cout << "E R R O R ! First efficiency and error histograms have different binning!\n";
  }
  if(!hp->SameBins(hEff2, hEff2Hi) || !hp->SameBins(hEff2, hEff2Lo))
  {
    std::cout << "E R R O R ! Second efficiency and error histograms have different binning!\n";
  }
  
  // Then find the binning of the combined histograms
  std::pair<std::vector<double>,std::vector<double> > pBinning = hp->FindDifferenceBinDivision(hEff1,hEff2);
  
  // Now create the resulting histograms
  unsigned int nXBins = pBinning.first.size()-1;
  unsigned int nYBins = pBinning.second.size()-1;
  
  hEff = hp->Create2DHistogram<TH2D>(name.c_str(), title.c_str(), nXBins, pBinning.first, nYBins, pBinning.second, hEff1->GetXaxis()->GetTitle(), hEff1->GetYaxis()->GetTitle());

  std::string nameHi = name + "_Hi";
  std::string titleHi = title + " - Hi err";
  hEffHi = hp->Create2DHistogram<TH2D>(nameHi.c_str(), titleHi.c_str(), nXBins, pBinning.first, nYBins, pBinning.second, hEff1Hi->GetXaxis()->GetTitle(), hEff1Hi->GetYaxis()->GetTitle());

  std::string nameLo = name + "_Lo";
  std::string titleLo = title + " - Lo err";
  hEffLo = hp->Create2DHistogram<TH2D>(nameLo.c_str(), titleLo.c_str(), nXBins, pBinning.first, nYBins, pBinning.second, hEff1Lo->GetXaxis()->GetTitle(), hEff1Lo->GetYaxis()->GetTitle());
  
  for(unsigned int xBin = 1; xBin <= nXBins; xBin++)
  {
    for(unsigned int yBin = 1; yBin <= nYBins; yBin++)
    {
      double x = hEff->GetXaxis()->GetBinLowEdge(xBin);
      double y = hEff->GetYaxis()->GetBinLowEdge(yBin);
      int binFirst = hEff1->FindFixBin(x,y);
      int binSecond = hEff2->FindFixBin(x,y);
      double eff1 = hEff1->GetBinContent(binFirst);
      double eff2 = hEff2->GetBinContent(binSecond);
      double err1Hi = hEff1Hi->GetBinContent(binFirst);
      double err2Hi = hEff2Hi->GetBinContent(binSecond);
      double err1Lo = hEff1Lo->GetBinContent(binFirst);
      double err2Lo = hEff2Lo->GetBinContent(binSecond);
      
      double eff = eff1*eff2;
      double effHi = eff * sqrt((err1Hi*err1Hi)/(eff1*eff1) + (err2Hi*err2Hi)/(eff2*eff2));
      double effLo = eff * sqrt((err1Lo*err1Lo)/(eff1*eff1) + (err2Lo*err2Lo)/(eff2*eff2));
      hEff->SetBinContent(xBin, yBin, eff);
      hEffHi->SetBinContent(xBin, yBin, effHi);
      hEffLo->SetBinContent(xBin, yBin, effLo);
    }
  }
}
