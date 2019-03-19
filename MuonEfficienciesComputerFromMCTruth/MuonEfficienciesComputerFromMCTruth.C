#ifndef MuonEfficienciesComputerFromMCTruth_cxx
#define MuonEfficienciesComputerFromMCTruth_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include <sstream>

#include "MuonEfficienciesComputerFromMCTruth.h"
#include "../utilities/constants.h"

void MuonEfficienciesComputerFromMCTruth::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Kinematic cuts
  const double etaOut = 2.1;
  const double etaIn = 2.1;
  const double ptOut = 4.;
  const double ptIn = 4.;
  // Histogram binning and labels
  const unsigned int nPtBins = 150;
  const double ptMin = 0.;
  const double ptMax = 30.;
  const char* genPtLabel = "gen p_{T} [GeV/c]";
  const char* recoPtLabel = "reco p_{T} [GeV/c]";
  const unsigned int nEtaBins = 60;
  const double etaMin = -3.;
  const double etaMax = 3.;
  const char* genEtaLabel = "gen #eta";
  const char* recoEtaLabel = "reco #eta";
  const char* nGenMuLabel = "N_{gen #mu}";
  const char* nRecoMuLabel = "N_{reco #mu}";
  const char* nSelMuLabel = "N_{sel #mu}";
  const char* nTrgMuLabel = "N_{trg #mu}";
  const char* nGoodTrackLabel = "N_{good track}";
  const char* nGoodRecoMuLabel = "N_{good reco #mu}";
  const char* nGoodSelMuLabel = "N_{good sel #mu}";
  const char* nGoodTrgMuLabel = "N_{good trg #mu}";
  const char* goodTrackOverGenLabel = "N_{good track} / N_{gen #mu}";
  const char* goodRecoOverGenLabel = "N_{good reco #mu} / N_{gen #mu}";
  const char* goodRecoOverGoodTrackLabel = "N_{good reco #mu} / N_{good track}";
  const char* goodSelOverGenLabel = "N_{good sel #mu} / N_{gen #mu}";
  const char* goodSelOverGoodTrackLabel = "N_{good sel #mu} / N_{good track}";
  const char* goodSelOverGoodRecoLabel = "N_{good sel #mu} / N_{good reco #mu}";
  const char* goodTrgOverGenLabel = "N_{good trg #mu} / N_{gen #mu}";
  const char* goodTrgOverGoodTrackLabel = "N_{good trg #mu} / N_{good track}";
  const char* goodTrgOverGoodRecoLabel = "N_{good trg #mu} / N_{good reco #mu}";
  const char* goodTrgOverGoodSelLabel = "N_{good trg #mu} / N_{good sel #mu}";
  const char* goodRecoOverAllRecoLabel = "N_{good reco #mu} / N_{reco #mu}";
  const char* goodSelOverAllSelLabel = "N_{good sel #mu} / N_{sel #mu}";
  const char* goodTrgOverAllTrgLabel = "N_{good trg #mu} / N_{trg #mu}";

  // Book histograms here

  std::stringstream oss;
  std::string name,title;
  TH2D* hAllGenMuVsGenPtEta = Create2DHistogram<TH2D>("hAllGenMuVsGenPtEta", "All generated muons vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAllGenMuOutsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hAllGenMuOutsideCutsVsGenPtEta", "All generated muons outside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAllGenMuInAcceptanceVsGenPtEta = Create2DHistogram<TH2D>("hAllGenMuInAcceptanceVsGenPtEta", "All generated muons in acceptance vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH1D* hAllGenMuVsGenPt = Create1DHistogram<TH1D>("hAllGenMuVsGenPt", "All generated muons vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGenMuLabel);
  TH1D* hAllGenMuOutsideCutsVsGenPt = Create1DHistogram<TH1D>("hAllGenMuOutsideCutsVsGenPt", "All generated muons outside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGenMuLabel);
  TH1D* hAllGenMuInAcceptanceVsGenPt = Create1DHistogram<TH1D>("hAllGenMuInAcceptanceVsGenPt", "All generated muons in acceptance vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGenMuLabel);
  TH1D* hAllGenMuVsGenEta = Create1DHistogram<TH1D>("hAllGenMuVsGenEta", "All generated muons vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGenMuLabel);
  TH1D* hAllGenMuOutsideCutsVsGenEta = Create1DHistogram<TH1D>("hAllGenMuOutsideCutsVsGenEta", "All generated muons outside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGenMuLabel);
  TH1D* hAllGenMuInAcceptanceVsGenEta = Create1DHistogram<TH1D>("hAllGenMuInAcceptanceVsGenEta", "All generated muons in acceptance vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGenMuLabel);

  TH2D* hAllRecoMuVsRecoPtEta = Create2DHistogram<TH2D>("hAllRecoMuVsRecoPtEta", "All reconstructed muons vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAllRecoMuInsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hAllRecoMuInsideCutsVsRecoPtEta", "Reconstructed muons inside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH1D* hAllRecoMuVsRecoPt = Create1DHistogram<TH1D>("hAllRecoMuVsRecoPt", "All reconstructed muons vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nRecoMuLabel);
  TH1D* hAllRecoMuInsideCutsVsRecoPt = Create1DHistogram<TH1D>("hAllRecoMuInsideCutsVsRecoPt", "Reconstructed muons inside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nRecoMuLabel);
  TH1D* hAllRecoMuVsRecoEta = Create1DHistogram<TH1D>("hAllRecoMuVsRecoEta", "All reconstructed muons vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nRecoMuLabel);
  TH1D* hAllRecoMuInsideCutsVsRecoEta = Create1DHistogram<TH1D>("hAllRecoMuInsideCutsVsRecoEta", "Reconstructed muons inside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nRecoMuLabel);

  TH2D* hAllSelMuVsRecoPtEta = Create2DHistogram<TH2D>("hAllSelMuVsRecoPtEta", "All selected muons vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAllSelMuInsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hAllSelMuInsideCutsVsRecoPtEta", "All selected muons inside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH1D* hAllSelMuVsRecoPt = Create1DHistogram<TH1D>("hAllSelMuVsRecoPta", "All selected muons vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nSelMuLabel);
  TH1D* hAllSelMuInsideCutsVsRecoPt = Create1DHistogram<TH1D>("hAllSelMuInsideCutsVsRecoPt", "All selected muons inside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nSelMuLabel);
  TH1D* hAllSelMuVsRecoEta = Create1DHistogram<TH1D>("hAllSelMuVsRecoEta", "All selected muons vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nSelMuLabel);
  TH1D* hAllSelMuInsideCutsVsRecoEta = Create1DHistogram<TH1D>("hAllSelMuInsideCutsVsRecoEta", "All selected muons inside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nSelMuLabel);

  TH2D* hAllTrgMuVsRecoPtEta = Create2DHistogram<TH2D>("hAllTrgMuVsRecoPtEta", "All trigger muons vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAllTrgMuInsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hAllTrgMuInsideCutsVsRecoPtEta", "All trigger muons inside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH1D* hAllTrgMuVsRecoPt = Create1DHistogram<TH1D>("hAllTrgMuVsRecoPta", "All trigger muons vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nTrgMuLabel);
  TH1D* hAllTrgMuInsideCutsVsRecoPt = Create1DHistogram<TH1D>("hAllTrgMuInsideCutsVsRecoPt", "All trigger muons inside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nTrgMuLabel);
  TH1D* hAllTrgMuVsRecoEta = Create1DHistogram<TH1D>("hAllTrgMuVsRecoEta", "All trigger muons vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nTrgMuLabel);
  TH1D* hAllTrgMuInsideCutsVsRecoEta = Create1DHistogram<TH1D>("hAllTrgMuInsideCutsVsRecoEta", "All trigger muons inside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nTrgMuLabel);
  
  TH2D* hGoodTrackVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrackVsGenPtEta", "Generated muons associated to a track vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrackInsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrackInsideCutsVsGenPtEta", "Generated muons associated to a track inside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrackOutsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrackOutsideCutsVsGenPtEta", "Generated muons associated to a track outside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrackVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrackVsRecoPtEta", "Generated muons associated to a track vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrackInsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrackInsideCutsVsRecoPtEta", "Generated muons associated to a track inside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrackOutsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrackOutsideCutsVsRecoPtEta", "Generated muons associated to a track outside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH1D* hGoodTrackVsGenPt = Create1DHistogram<TH1D>("hGoodTrackVsGenPt", "Generated muons associated to a track vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodTrackLabel);
  TH1D* hGoodTrackInsideCutsVsGenPt = Create1DHistogram<TH1D>("hGoodTrackInsideCutsVsGenPt", "Generated muons associated to a track inside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodTrackLabel);
  TH1D* hGoodTrackOutsideCutsVsGenPt = Create1DHistogram<TH1D>("hGoodTrackOutsideCutsVsGenPt", "Generated muons associated to a track outside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodTrackLabel);
  TH1D* hGoodTrackVsRecoPt = Create1DHistogram<TH1D>("hGoodTrackVsRecoPt", "Generated muons associated to a track vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrackLabel);
  TH1D* hGoodTrackInsideCutsVsRecoPt = Create1DHistogram<TH1D>("hGoodTrackInsideCutsVsRecoPt", "Generated muons associated to a track inside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrackLabel);
  TH1D* hGoodTrackOutsideCutsVsRecoPt = Create1DHistogram<TH1D>("hGoodTrackOutsideCutsVsRecoPt", "Generated muons associated to a track outside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrackLabel);
  TH1D* hGoodTrackVsGenEta = Create1DHistogram<TH1D>("hGoodTrackVsGenEta", "Generated muons associated to a track vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrackLabel);
  TH1D* hGoodTrackInsideCutsVsGenEta = Create1DHistogram<TH1D>("hGoodTrackInsideCutsVsGenEta", "Generated muons associated to a track inside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrackLabel);
  TH1D* hGoodTrackOutsideCutsVsGenEta = Create1DHistogram<TH1D>("hGoodTrackOutsideCutsVsGenEta", "Generated muons associated to a track outside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrackLabel);
  TH1D* hGoodTrackVsRecoEta = Create1DHistogram<TH1D>("hGoodTrackVsRecoEta", "Generated muons associated to a track vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrackLabel);
  TH1D* hGoodTrackInsideCutsVsRecoEta = Create1DHistogram<TH1D>("hGoodTrackInsideCutsVsRecoEta", "Generated muons associated to a track inside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrackLabel);
  TH1D* hGoodTrackOutsideCutsVsRecoEta = Create1DHistogram<TH1D>("hGoodTrackOutsideCutsVsRecoEta", "Generated muons associated to a track outside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrackLabel);

  TH2D* hGoodRecoMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodRecoMuVsGenPtEta", "Generated muons associated to a reco muon vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodRecoMuInsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hGoodRecoMuInsideCutsVsGenPtEta", "Generated muons associated to a reco muon inside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodRecoMuOutsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hGoodRecoMuOutsideCutsVsGenPtEta", "Generated muons associated to a reco muon outside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodRecoMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodRecoMuVsRecoPtEta", "Generated muons associated to a reco muon vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodRecoMuInsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hGoodRecoMuInsideCutsVsRecoPtEta", "Generated muons associated to a reco muon inside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodRecoMuOutsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hGoodRecoMuOutsideCutsVsRecoPtEta", "Generated muons associated to a reco muon outside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH1D* hGoodRecoMuVsGenPt = Create1DHistogram<TH1D>("hGoodRecoMuVsGenPt", "Generated muons associated to a reco muon vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuInsideCutsVsGenPt = Create1DHistogram<TH1D>("hGoodRecoMuInsideCutsVsGenPt", "Generated muons associated to a reco muon inside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuOutsideCutsVsGenPt = Create1DHistogram<TH1D>("hGoodRecoMuOutsideCutsVsGenPt", "Generated muons associated to a reco muon outside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuVsRecoPt = Create1DHistogram<TH1D>("hGoodRecoMuVsRecoPt", "Generated muons associated to a reco muon vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuInsideCutsVsRecoPt = Create1DHistogram<TH1D>("hGoodRecoMuInsideCutsVsRecoPt", "Generated muons associated to a reco muon inside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuOutsideCutsVsRecoPt = Create1DHistogram<TH1D>("hGoodRecoMuOutsideCutsVsRecoPt", "Generated muons associated to a reco muon outside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuVsGenEta = Create1DHistogram<TH1D>("hGoodRecoMuVsGenEta", "Generated muons associated to a reco muon vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuInsideCutsVsGenEta = Create1DHistogram<TH1D>("hGoodRecoMuInsideCutsVsGenEta", "Generated muons associated to a reco muon inside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuOutsideCutsVsGenEta = Create1DHistogram<TH1D>("hGoodRecoMuOutsideCutsVsGenEta", "Generated muons associated to a reco muon outside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuVsRecoEta = Create1DHistogram<TH1D>("hGoodRecoMuVsRecoEta", "Generated muons associated to a reco muon vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuInsideCutsVsRecoEta = Create1DHistogram<TH1D>("hGoodRecoMuInsideCutsVsRecoEta", "Generated muons associated to a reco muon inside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodRecoMuLabel);
  TH1D* hGoodRecoMuOutsideCutsVsRecoEta = Create1DHistogram<TH1D>("hGoodRecoMuOutsideCutsVsRecoEta", "Generated muons associated to a reco muon outside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodRecoMuLabel);

  TH2D* hGoodSelMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodSelMuVsGenPtEta", "Generated muons associated to a selected muon vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelMuInsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hGoodSelMuInsideCutsVsGenPtEta", "Generated muons associated to a selected muon inside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelMuOutsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hGoodSelMuOutsideCutsVsGenPtEta", "Generated muons associated to a selected muon outside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodSelMuVsRecoPtEta", "Generated muons associated to a selected muon vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodSelMuInsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hGoodSelMuInsideCutsVsRecoPtEta", "Generated muons associated to a selected muon inside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodSelMuOutsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hGoodSelMuOutsideCutsVsRecoPtEta", "Generated muons associated to a selected muon outside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH1D* hGoodSelMuVsGenPt = Create1DHistogram<TH1D>("hGoodSelMuVsGenPt", "Generated muons associated to a selected muon vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuInsideCutsVsGenPt = Create1DHistogram<TH1D>("hGoodSelMuInsideCutsVsGenPt", "Generated muons associated to a selected muon inside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuOutsideCutsVsGenPt = Create1DHistogram<TH1D>("hGoodSelMuOutsideCutsVsGenPt", "Generated muons associated to a selected muon outside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuVsRecoPt = Create1DHistogram<TH1D>("hGoodSelMuVsRecoPt", "Generated muons associated to a selected muon vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuInsideCutsVsRecoPt = Create1DHistogram<TH1D>("hGoodSelMuInsideCutsVsRecoPt", "Generated muons associated to a selected muon inside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuOutsideCutsVsRecoPt = Create1DHistogram<TH1D>("hGoodSelMuOutsideCutsVsRecoPt", "Generated muons associated to a selected muon outside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuVsGenEta = Create1DHistogram<TH1D>("hGoodSelMuVsGenEta", "Generated muons associated to a selected muon vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuInsideCutsVsGenEta = Create1DHistogram<TH1D>("hGoodSelMuInsideCutsVsGenEta", "Generated muons associated to a selected muon inside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuOutsideCutsVsGenEta = Create1DHistogram<TH1D>("hGoodSelMuOutsideCutsVsGenEta", "Generated muons associated to a selected muon outside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuVsRecoEta = Create1DHistogram<TH1D>("hGoodSelMuVsRecoEta", "Generated muons associated to a selected muon vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuInsideCutsVsRecoEta = Create1DHistogram<TH1D>("hGoodSelMuInsideCutsVsRecoEta", "Generated muons associated to a selected muon inside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodSelMuLabel);
  TH1D* hGoodSelMuOutsideCutsVsRecoEta = Create1DHistogram<TH1D>("hGoodSelMuOutsideCutsVsRecoEta", "Generated muons associated to a selected muon outside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodSelMuLabel);

  TH2D* hGoodTrgMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrgMuVsGenPtEta", "Generated muons associated to a trigger muon vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgMuInsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrgMuInsideCutsVsGenPtEta", "Generated muons associated to a trigger muon inside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgMuOutsideCutsVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrgMuOutsideCutsVsGenPtEta", "Generated muons associated to a trigger muon outside cuts vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrgMuVsRecoPtEta", "Generated muons associated to a trigger muon vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgMuInsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrgMuInsideCutsVsRecoPtEta", "Generated muons associated to a trigger muon inside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgMuOutsideCutsVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrgMuOutsideCutsVsRecoPtEta", "Generated muons associated to a trigger muon outside cuts vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH1D* hGoodTrgMuVsGenPt = Create1DHistogram<TH1D>("hGoodTrgMuVsGenPt", "Generated muons associated to a trigger muon vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuInsideCutsVsGenPt = Create1DHistogram<TH1D>("hGoodTrgMuInsideCutsVsGenPt", "Generated muons associated to a trigger muon inside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuOutsideCutsVsGenPt = Create1DHistogram<TH1D>("hGoodTrgMuOutsideCutsVsGenPt", "Generated muons associated to a trigger muon outside cuts vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuVsRecoPt = Create1DHistogram<TH1D>("hGoodTrgMuVsRecoPt", "Generated muons associated to a trigger muon vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuInsideCutsVsRecoPt = Create1DHistogram<TH1D>("hGoodTrgMuInsideCutsVsRecoPt", "Generated muons associated to a trigger muon inside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuOutsideCutsVsRecoPt = Create1DHistogram<TH1D>("hGoodTrgMuOutsideCutsVsRecoPt", "Generated muons associated to a trigger muon outside cuts vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuVsGenEta = Create1DHistogram<TH1D>("hGoodTrgMuVsGenEta", "Generated muons associated to a trigger muon vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuInsideCutsVsGenEta = Create1DHistogram<TH1D>("hGoodTrgMuInsideCutsVsGenEta", "Generated muons associated to a trigger muon inside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuOutsideCutsVsGenEta = Create1DHistogram<TH1D>("hGoodTrgMuOutsideCutsVsGenEta", "Generated muons associated to a trigger muon outside cuts vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuVsRecoEta = Create1DHistogram<TH1D>("hGoodTrgMuVsRecoEta", "Generated muons associated to a trigger muon vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuInsideCutsVsRecoEta = Create1DHistogram<TH1D>("hGoodTrgMuInsideCutsVsRecoEta", "Generated muons associated to a trigger muon inside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrgMuLabel);
  TH1D* hGoodTrgMuOutsideCutsVsRecoEta = Create1DHistogram<TH1D>("hGoodTrgMuOutsideCutsVsRecoEta", "Generated muons associated to a trigger muon outside cuts vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrgMuLabel);
  
  std::map<short int, TH2D*> mAllGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mAllGenMuOutsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mAllGenMuInAcceptanceVsGenPtEtaSC;
  std::map<short int, TH1D*> mAllGenMuVsGenPtSC;
  std::map<short int, TH1D*> mAllGenMuOutsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mAllGenMuInAcceptanceVsGenPtSC;
  std::map<short int, TH1D*> mAllGenMuVsGenEtaSC;
  std::map<short int, TH1D*> mAllGenMuOutsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mAllGenMuInAcceptanceVsGenEtaSC;

  std::map<short int, TH2D*> mGoodTrackVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrackInsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrackOutsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrackVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrackInsideCutsVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrackOutsideCutsVsRecoPtEtaSC;
  std::map<short int, TH1D*> mGoodTrackVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrackInsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrackOutsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrackVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrackInsideCutsVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrackOutsideCutsVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrackVsGenEtaSC;
  std::map<short int, TH1D*> mGoodTrackInsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mGoodTrackOutsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mGoodTrackVsRecoEtaSC;
  std::map<short int, TH1D*> mGoodTrackInsideCutsVsRecoEtaSC;
  std::map<short int, TH1D*> mGoodTrackOutsideCutsVsRecoEtaSC;

  std::map<short int, TH2D*> mGoodRecoMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoMuInsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoMuOutsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoMuInsideCutsVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoMuOutsideCutsVsRecoPtEtaSC;
  std::map<short int, TH1D*> mGoodRecoMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodRecoMuInsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mGoodRecoMuOutsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mGoodRecoMuVsRecoPtSC;
  std::map<short int, TH1D*> mGoodRecoMuInsideCutsVsRecoPtSC;
  std::map<short int, TH1D*> mGoodRecoMuOutsideCutsVsRecoPtSC;
  std::map<short int, TH1D*> mGoodRecoMuVsGenEtaSC;
  std::map<short int, TH1D*> mGoodRecoMuInsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mGoodRecoMuOutsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mGoodRecoMuVsRecoEtaSC;
  std::map<short int, TH1D*> mGoodRecoMuInsideCutsVsRecoEtaSC;
  std::map<short int, TH1D*> mGoodRecoMuOutsideCutsVsRecoEtaSC;

  std::map<short int, TH2D*> mGoodSelMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelMuInsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelMuOutsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodSelMuInsideCutsVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodSelMuOutsideCutsVsRecoPtEtaSC;
  std::map<short int, TH1D*> mGoodSelMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodSelMuInsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mGoodSelMuOutsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mGoodSelMuVsRecoPtSC;
  std::map<short int, TH1D*> mGoodSelMuInsideCutsVsRecoPtSC;
  std::map<short int, TH1D*> mGoodSelMuOutsideCutsVsRecoPtSC;
  std::map<short int, TH1D*> mGoodSelMuVsGenEtaSC;
  std::map<short int, TH1D*> mGoodSelMuInsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mGoodSelMuOutsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mGoodSelMuVsRecoEtaSC;
  std::map<short int, TH1D*> mGoodSelMuInsideCutsVsRecoEtaSC;
  std::map<short int, TH1D*> mGoodSelMuOutsideCutsVsRecoEtaSC;

  std::map<short int, TH2D*> mGoodTrgMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgMuInsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgMuOutsideCutsVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgMuInsideCutsVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgMuOutsideCutsVsRecoPtEtaSC;
  std::map<short int, TH1D*> mGoodTrgMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrgMuInsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrgMuOutsideCutsVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrgMuVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrgMuInsideCutsVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrgMuOutsideCutsVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrgMuVsGenEtaSC;
  std::map<short int, TH1D*> mGoodTrgMuInsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mGoodTrgMuOutsideCutsVsGenEtaSC;
  std::map<short int, TH1D*> mGoodTrgMuVsRecoEtaSC;
  std::map<short int, TH1D*> mGoodTrgMuInsideCutsVsRecoEtaSC;
  std::map<short int, TH1D*> mGoodTrgMuOutsideCutsVsRecoEtaSC;

  
  for(unsigned int i = 0; i < 7; i++)
  {
    oss.str("");
    oss << "hAllGenMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    TH2D* histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mAllGenMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hAllGenMuOutsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons outside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mAllGenMuOutsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hAllGenMuInAcceptanceVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons in acceptance vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mAllGenMuInAcceptanceVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hAllGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons vs gen p_{T} for muon category " << i;
    title = oss.str();
    TH1D* histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGenMuLabel);
    mAllGenMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hAllGenMuOutsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons outside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGenMuLabel);
    mAllGenMuOutsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hAllGenMuInAcceptanceVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons in acceptance vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGenMuLabel);
    mAllGenMuInAcceptanceVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hAllGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGenMuLabel);
    mAllGenMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hAllGenMuOutsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons outside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGenMuLabel);
    mAllGenMuOutsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hAllGenMuInAcceptanceVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "All generated muons in acceptance vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGenMuLabel);
    mAllGenMuInAcceptanceVsGenEtaSC[i] = histo1d;

    oss.str("");
    oss << "hGoodTrackVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrackVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrackInsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks inside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrackInsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrackOutsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks outside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrackOutsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrackVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrackVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrackInsideCutsVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks inside cuts vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrackInsideCutsVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrackOutsideCutsVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks outside cuts vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrackOutsideCutsVsRecoPtEtaSC[i] = histo;

    oss.str("");
    oss << "hGoodTrackVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodTrackLabel);
    mGoodTrackVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackInsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks inside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodTrackLabel);
    mGoodTrackInsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackOutsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks outside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodTrackLabel);
    mGoodTrackOutsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrackLabel);
    mGoodTrackVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackInsideCutsVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks inside cuts vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrackLabel);
    mGoodTrackInsideCutsVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackOutsideCutsVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks outside cuts vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrackLabel);
    mGoodTrackOutsideCutsVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrackLabel);
    mGoodTrackVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackInsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks inside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrackLabel);
    mGoodTrackInsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackOutsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks outside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrackLabel);
    mGoodTrackOutsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrackLabel);
    mGoodTrackVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackInsideCutsVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks inside cuts vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrackLabel);
    mGoodTrackInsideCutsVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrackOutsideCutsVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good tracks outside cuts vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrackLabel);
    mGoodTrackOutsideCutsVsRecoEtaSC[i] = histo1d;

    oss.str("");
    oss << "hGoodRecoMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoMuInsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons inside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoMuInsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoMuOutsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons outside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoMuOutsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoMuVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodRecoMuVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoMuInsideCutsVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons inside cuts vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodRecoMuInsideCutsVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoMuOutsideCutsVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons outside cuts vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodRecoMuOutsideCutsVsRecoPtEtaSC[i] = histo;

    oss.str("");
    oss << "hGoodRecoMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodRecoMuLabel);
    mGoodRecoMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuInsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons inside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodRecoMuLabel);
    mGoodRecoMuInsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuOutsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons outside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodRecoMuLabel);
    mGoodRecoMuOutsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodRecoMuLabel);
    mGoodRecoMuVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuInsideCutsVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons inside cuts vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodRecoMuLabel);
    mGoodRecoMuInsideCutsVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuOutsideCutsVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons outside cuts vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodRecoMuLabel);
    mGoodRecoMuOutsideCutsVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodRecoMuLabel);
    mGoodRecoMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuInsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons inside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodRecoMuLabel);
    mGoodRecoMuInsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuOutsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons outside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodRecoMuLabel);
    mGoodRecoMuOutsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodRecoMuLabel);
    mGoodRecoMuVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuInsideCutsVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons inside cuts vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodRecoMuLabel);
    mGoodRecoMuInsideCutsVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodRecoMuOutsideCutsVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good reco muons outside cuts vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodRecoMuLabel);
    mGoodRecoMuOutsideCutsVsRecoEtaSC[i] = histo1d;

    oss.str("");
    oss << "hGoodSelMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelMuInsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons inside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelMuInsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelMuOutsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons outside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelMuOutsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelMuVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelMuVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelMuInsideCutsVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons inside cuts vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelMuInsideCutsVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelMuOutsideCutsVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons outside cuts vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelMuOutsideCutsVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodSelMuLabel);
    mGoodSelMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuInsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons inside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodSelMuLabel);
    mGoodSelMuInsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuOutsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons outside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodSelMuLabel);
    mGoodSelMuOutsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodSelMuLabel);
    mGoodSelMuVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuInsideCutsVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons inside cuts vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodSelMuLabel);
    mGoodSelMuInsideCutsVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuOutsideCutsVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons outside cuts vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodSelMuLabel);
    mGoodSelMuOutsideCutsVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodSelMuLabel);
    mGoodSelMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuInsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons inside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodSelMuLabel);
    mGoodSelMuInsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuOutsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons outside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodSelMuLabel);
    mGoodSelMuOutsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodSelMuLabel);
    mGoodSelMuVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuInsideCutsVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons inside cuts vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodSelMuLabel);
    mGoodSelMuInsideCutsVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodSelMuOutsideCutsVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good sel muons outside cuts vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodSelMuLabel);
    mGoodSelMuOutsideCutsVsRecoEtaSC[i] = histo1d;

    oss.str("");
    oss << "hGoodTrgMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgMuInsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons inside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgMuInsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgMuOutsideCutsVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons outside cuts vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgMuOutsideCutsVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgMuVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgMuVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgMuInsideCutsVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons inside cuts vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgMuInsideCutsVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgMuOutsideCutsVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons outside cuts vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgMuOutsideCutsVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodTrgMuLabel);
    mGoodTrgMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuInsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons inside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodTrgMuLabel);
    mGoodTrgMuInsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuOutsideCutsVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons outside cuts vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, nGoodTrgMuLabel);
    mGoodTrgMuOutsideCutsVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrgMuLabel);
    mGoodTrgMuVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuInsideCutsVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons inside cuts vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrgMuLabel);
    mGoodTrgMuInsideCutsVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuOutsideCutsVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons outside cuts vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, nGoodTrgMuLabel);
    mGoodTrgMuOutsideCutsVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrgMuLabel);
    mGoodTrgMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuInsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons inside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrgMuLabel);
    mGoodTrgMuInsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuOutsideCutsVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons outside cuts vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, nGoodTrgMuLabel);
    mGoodTrgMuOutsideCutsVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrgMuLabel);
    mGoodTrgMuVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuInsideCutsVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons inside cuts vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrgMuLabel);
    mGoodTrgMuInsideCutsVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hGoodTrgMuOutsideCutsVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Good trg muons outside cuts vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, nGoodTrgMuLabel);
    mGoodTrgMuOutsideCutsVsRecoEtaSC[i] = histo1d;
  }

  std::vector<TGraphAsymmErrors*> vgGoodTrackOverGenMuVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrackOverGenMuVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good tracks)/(gen muons) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrackOverGenLabel, nPtBins);
    vgGoodTrackOverGenMuVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodTrackOverGenMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrackOverGenMuVsGenPtEta", "(good tracks)/(gen muons) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrackOverGenMuVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodTrackOverGenMuVsGenPtEtaHi", "(good tracks)/(gen muons) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrackOverGenMuVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodTrackOverGenMuVsGenPtEtaLo", "(good tracks)/(gen muons) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodTrackOverGenMuVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodTrackOverGenMuVsGenPt", "(good tracks)/(gen muons) vs gen p_{T}", genPtLabel, goodTrackOverGenLabel, nPtBins);
  TH1D* hGoodTrackOverGenMuVsGenPt = Create1DHistogram<TH1D>("hGoodTrackOverGenMuVsGenPt", "(good tracks)/(gen muons) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodTrackOverGenLabel);
  TGraphAsymmErrors* gGoodTrackOverGenMuVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodTrackOverGenMuVsGenEta", "(good tracks)/(gen muons) vs gen #eta", genEtaLabel, goodTrackOverGenLabel, nEtaBins);
  TH1D* hGoodTrackOverGenMuVsGenEta = Create1DHistogram<TH1D>("hGoodTrackOverGenMuVsGenEta", "(good tracks)/(gen muons) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodTrackOverGenLabel);

  std::vector<TGraphAsymmErrors*> vgGoodRecoOverGenMuVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodRecoOverGenMuVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(gen muons) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodRecoOverGenLabel, nPtBins);
    vgGoodRecoOverGenMuVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodRecoOverGenMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodRecoOverGenMuVsGenPtEta", "(good reco muons)/(gen muons) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodRecoOverGenMuVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodRecoOverGenMuVsGenPtEtaHi", "(good reco muons)/(gen muons) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodRecoOverGenMuVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodRecoOverGenMuVsGenPtEtaLo", "(good reco muons)/(gen muons) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodRecoOverGenMuVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodRecoOverGenMuVsGenPt", "(good reco muons)/(gen muons) vs gen p_{T}", genPtLabel, goodRecoOverGenLabel, nPtBins);
  TH1D* hGoodRecoOverGenMuVsGenPt = Create1DHistogram<TH1D>("hGoodRecoOverGenMuVsGenPt", "(good reco muons)/(gen muons) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodRecoOverGenLabel);
  TGraphAsymmErrors* gGoodRecoOverGenMuVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodRecoOverGenMuVsGenEta", "(good reco muons)/(gen muons) vs gen #eta", genEtaLabel, goodRecoOverGenLabel, nEtaBins);
  TH1D* hGoodRecoOverGenMuVsGenEta = Create1DHistogram<TH1D>("hGoodRecoOverGenMuVsGenEta", "(good reco muons)/(gen muons) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodRecoOverGenLabel);

  std::vector<TGraphAsymmErrors*> vgGoodRecoOverGoodTrackVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodRecoOverGoodTrackVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodRecoOverGoodTrackLabel, nPtBins);
    vgGoodRecoOverGoodTrackVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodRecoOverGoodTrackVsGenPtEta = Create2DHistogram<TH2D>("hGoodRecoOverGoodTrackVsGenPtEta", "(good reco muons)/(good tracks) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodRecoOverGoodTrackVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodRecoOverGoodTrackVsGenPtEtaHi", "(good reco muons)/(good tracks) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodRecoOverGoodTrackVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodRecoOverGoodTrackVsGenPtEtaLo", "(good reco muons)/(good tracks) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodRecoOverGoodTrackVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodRecoOverGoodTrackVsGenPt", "(good reco muons)/(good tracks) vs gen p_{T}", genPtLabel, goodRecoOverGoodTrackLabel, nPtBins);
  TH1D* hGoodRecoOverGoodTrackVsGenPt = Create1DHistogram<TH1D>("hGoodRecoOverGoodTrackVsGenPt", "(good reco muons)/(good tracks) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodRecoOverGoodTrackLabel);
  TGraphAsymmErrors* gGoodRecoOverGoodTrackVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodRecoOverGoodTrackVsGenEta", "(good reco muons)/(good tracks) vs gen #eta", genEtaLabel, goodRecoOverGoodTrackLabel, nEtaBins);
  TH1D* hGoodRecoOverGoodTrackVsGenEta = Create1DHistogram<TH1D>("hGoodRecoOverGoodTrackVsGenEta", "(good reco muons)/(good tracks) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodRecoOverGoodTrackLabel);

  std::vector<TGraphAsymmErrors*> vgGoodRecoOverGoodTrackVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodRecoOverGoodTrackVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodRecoOverGoodTrackLabel, nPtBins);
    vgGoodRecoOverGoodTrackVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodRecoOverGoodTrackVsRecoPtEta = Create2DHistogram<TH2D>("hGoodRecoOverGoodTrackVsRecoPtEta", "(good reco muons)/(good tracks) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodRecoOverGoodTrackVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodRecoOverGoodTrackVsRecoPtEtaHi", "(good reco muons)/(good tracks) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodRecoOverGoodTrackVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodRecoOverGoodTrackVsRecoPtEtaLo", "(good reco muons)/(good tracks) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodRecoOverGoodTrackVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodRecoOverGoodTrackVsRecoPt", "(good reco muons)/(good tracks) vs reco p_{T}", recoPtLabel, goodRecoOverGoodTrackLabel, nPtBins);
  TH1D* hGoodRecoOverGoodTrackVsRecoPt = Create1DHistogram<TH1D>("hGoodRecoOverGoodTrackVsRecoPt", "(good reco muons)/(good tracks) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodRecoOverGoodTrackLabel);
  TGraphAsymmErrors* gGoodRecoOverGoodTrackVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodRecoOverGoodTrackVsRecoEta", "(good reco muons)/(good tracks) vs reco #eta", recoEtaLabel, goodRecoOverGoodTrackLabel, nEtaBins);
  TH1D* hGoodRecoOverGoodTrackVsRecoEta = Create1DHistogram<TH1D>("hGoodRecoOverGoodTrackVsRecoEta", "(good reco muons)/(good tracks) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodRecoOverGoodTrackLabel);

  std::vector<TGraphAsymmErrors*> vgGoodSelOverGenMuVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodSelOverGenMuVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(gen muons) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodSelOverGenLabel, nPtBins);
    vgGoodSelOverGenMuVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodSelOverGenMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodSelOverGenMuVsGenPtEta", "(good sel muons)/(gen muons) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelOverGenMuVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodSelOverGenMuVsGenPtEtaHi", "(good sel muons)/(gen muons) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelOverGenMuVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodSelOverGenMuVsGenPtEtaLo", "(good sel muons)/(gen muons) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodSelOverGenMuVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGenMuVsGenPt", "(good sel muons)/(gen muons) vs gen p_{T}", genPtLabel, goodSelOverGenLabel, nPtBins);
  TH1D* hGoodSelOverGenMuVsGenPt = Create1DHistogram<TH1D>("hGoodSelOverGenMuVsGenPt", "(good sel muons)/(gen muons) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodSelOverGenLabel);
  TGraphAsymmErrors* gGoodSelOverGenMuVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGenMuVsGenEta", "(good sel muons)/(gen muons) vs gen #eta", genEtaLabel, goodSelOverGenLabel, nEtaBins);
  TH1D* hGoodSelOverGenMuVsGenEta = Create1DHistogram<TH1D>("hGoodSelOverGenMuVsGenEta", "(good sel muons)/(gen muons) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodSelOverGenLabel);
  
  std::vector<TGraphAsymmErrors*> vgGoodSelOverGoodTrackVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodSelOverGoodTrackVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodSelOverGoodTrackLabel, nPtBins);
    vgGoodSelOverGoodTrackVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodSelOverGoodTrackVsGenPtEta = Create2DHistogram<TH2D>("hGoodSelOverGoodTrackVsGenPtEta", "(good Sel muons)/(good tracks) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelOverGoodTrackVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodSelOverGoodTrackVsGenPtEtaHi", "(good sel muons)/(good tracks) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelOverGoodTrackVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodRecoSelGoodTrackVsGenPtEtaLo", "(good sel muons)/(good tracks) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodSelOverGoodTrackVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGoodTrackVsGenPt", "(good sel muons)/(good tracks) vs gen p_{T}", genPtLabel, goodSelOverGoodTrackLabel, nPtBins);
  TH1D* hGoodSelOverGoodTrackVsGenPt = Create1DHistogram<TH1D>("hGoodSelOverGoodTrackVsGenPt", "(good sel muons)/(good tracks) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodSelOverGoodTrackLabel);
  TGraphAsymmErrors* gGoodSelOverGoodTrackVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGoodTrackVsGenEta", "(good sel muons)/(good tracks) vs gen #eta", genEtaLabel, goodSelOverGoodTrackLabel, nEtaBins);
  TH1D* hGoodSelOverGoodTrackVsGenEta = Create1DHistogram<TH1D>("hGoodSelOverGoodTrackVsGenEta", "(good sel muons)/(good tracks) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodSelOverGoodTrackLabel);

  std::vector<TGraphAsymmErrors*> vgGoodSelOverGoodTrackVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodSelOverGoodTrackVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodSelOverGoodTrackLabel, nPtBins);
    vgGoodSelOverGoodTrackVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodSelOverGoodTrackVsRecoPtEta = Create2DHistogram<TH2D>("hGoodSelOverGoodTrackVsRecoPtEta", "(good sel muons)/(good tracks) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodSelOverGoodTrackVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodSelOverGoodTrackVsRecoPtEtaHi", "(good sel muons)/(good tracks) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodSelOverGoodTrackVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodSelOverGoodTrackVsRecoPtEtaLo", "(good sel muons)/(good tracks) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodSelOverGoodTrackVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGoodTrackVsRecoPt", "(good sel muons)/(good tracks) vs reco p_{T}", recoPtLabel, goodSelOverGoodTrackLabel, nPtBins);
  TH1D* hGoodSelOverGoodTrackVsRecoPt = Create1DHistogram<TH1D>("hGoodSelOverGoodTrackVsRecoPt", "(good sel muons)/(good tracks) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodSelOverGoodTrackLabel);
  TGraphAsymmErrors* gGoodSelOverGoodTrackVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGoodTrackVsRecoEta", "(good sel muons)/(good tracks) vs reco #eta", recoEtaLabel, goodSelOverGoodTrackLabel, nEtaBins);
  TH1D* hGoodSelOverGoodTrackVsRecoEta = Create1DHistogram<TH1D>("hGoodSelOverGoodTrackVsRecoEta", "(good sel muons)/(good tracks) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodSelOverGoodTrackLabel);
  
  std::vector<TGraphAsymmErrors*> vgGoodSelOverGoodRecoMuVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodSelOverGoodRecoMuVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodSelOverGoodRecoLabel, nPtBins);
    vgGoodSelOverGoodRecoMuVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodSelOverGoodRecoMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodSelOverGoodRecoMuVsGenPtEta", "(good sel muons)/(good reco muons) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelOverGoodRecoMuVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodSelOverGoodRecoMuVsGenPtEtaHi", "(good sel muons)/(good reco muons) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodSelOverGoodRecoMuVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodSelOverGoodRecoMuVsGenPtEtaLo", "(good sel muons)/(good reco muons) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodSelOverGoodRecoMuVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGoodRecoMuVsGenPt", "(good sel muons)/(good reco muons) vs gen p_{T}", genPtLabel, goodSelOverGoodRecoLabel, nPtBins);
  TH1D* hGoodSelOverGoodRecoMuVsGenPt = Create1DHistogram<TH1D>("hGoodSelOverGoodRecoMuVsGenPt", "(good sel muons)/(good reco muons) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodSelOverGoodRecoLabel);
  TGraphAsymmErrors* gGoodSelOverGoodRecoMuVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGoodRecoMuVsGenEta", "(good sel muons)/(good reco muons) vs gen #eta", genEtaLabel, goodSelOverGoodRecoLabel, nEtaBins);
  TH1D* hGoodSelOverGoodRecoMuVsGenEta = Create1DHistogram<TH1D>("hGoodSelOverGoodRecoMuVsGenEta", "(good sel muons)/(good reco muons) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodSelOverGoodRecoLabel);

  std::vector<TGraphAsymmErrors*> vgGoodSelOverGoodRecoMuVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodSelOverGoodRecoMuVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodSelOverGoodRecoLabel, nPtBins);
    vgGoodSelOverGoodRecoMuVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodSelOverGoodRecoMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodSelOverGoodRecoMuVsRecoPtEta", "(good sel muons)/(good reco muons) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodSelOverGoodRecoMuVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodSelOverGoodRecoMuVsRecoPtEtaHi", "(good sel muons)/(good reco muons) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodSelOverGoodRecoMuVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodSelOverGoodRecoMuVsRecoPtEtaLo", "(good sel muons)/(good reco muons) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodSelOverGoodRecoMuVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGoodRecoMuVsRecoPt", "(good sel muons)/(good reco muons) vs reco p_{T}", recoPtLabel, goodSelOverGoodRecoLabel, nPtBins);
  TH1D* hGoodSelOverGoodRecoMuVsRecoPt = Create1DHistogram<TH1D>("hGoodSelOverGoodRecoMuVsRecoPt", "(good sel muons)/(good reco muons) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodSelOverGoodRecoLabel);
  TGraphAsymmErrors* gGoodSelOverGoodRecoMuVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodSelOverGoodRecoMuVsRecoEta", "(good sel muons)/(good reco muons) vs reco #eta", recoEtaLabel, goodSelOverGoodRecoLabel, nEtaBins);
  TH1D* hGoodSelOverGoodRecoMuVsRecoEta = Create1DHistogram<TH1D>("hGoodSelOverGoodRecoMuVsRecoEta", "(good sel muons)/(good reco muons) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodSelOverGoodRecoLabel);

  std::vector<TGraphAsymmErrors*> vgGoodTrgOverGenMuVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrgOverGenMuVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(gen muons) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrgOverGenLabel, nPtBins);
    vgGoodTrgOverGenMuVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodTrgOverGenMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrgOverGenMuVsGenPtEta", "(good trg muons)/(gen muons) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgOverGenMuVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodTrgOverGenMuVsGenPtEtaHi", "(good trg muons)/(gen muons) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgOverGenMuVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodTrgOverGenMuVsGenPtEtaLo", "(good trg muons)/(gen muons) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodTrgOverGenMuVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGenMuVsGenPt", "(good trg muons)/(gen muons) vs gen p_{T}", genPtLabel, goodTrgOverGenLabel, nPtBins);
  TH1D* hGoodTrgOverGenMuVsGenPt = Create1DHistogram<TH1D>("hGoodTrgOverGenMuVsGenPt", "(good trg muons)/(gen muons) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodTrgOverGenLabel);
  TGraphAsymmErrors* gGoodTrgOverGenMuVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGenMuVsGenEta", "(good trg muons)/(gen muons) vs gen #eta", genEtaLabel, goodTrgOverGenLabel, nEtaBins);
  TH1D* hGoodTrgOverGenMuVsGenEta = Create1DHistogram<TH1D>("hGoodTrgOverGenMuVsGenEta", "(good trg muons)/(gen muons) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodTrgOverGenLabel);
  
  std::vector<TGraphAsymmErrors*> vgGoodTrgOverGoodTrackVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrgOverGoodTrackVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrgOverGoodTrackLabel, nPtBins);
    vgGoodTrgOverGoodTrackVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodTrgOverGoodTrackVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrgOverGoodTrackVsGenPtEta", "(good Trg muons)/(good tracks) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgOverGoodTrackVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodTrgOverGoodTrackVsGenPtEtaHi", "(good trg muons)/(good tracks) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgOverGoodTrackVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodRecoTrgGoodTrackVsGenPtEtaLo", "(good trg muons)/(good tracks) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodTrackVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodTrackVsGenPt", "(good trg muons)/(good tracks) vs gen p_{T}", genPtLabel, goodTrgOverGoodTrackLabel, nPtBins);
  TH1D* hGoodTrgOverGoodTrackVsGenPt = Create1DHistogram<TH1D>("hGoodTrgOverGoodTrackVsGenPt", "(good trg muons)/(good tracks) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodTrgOverGoodTrackLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodTrackVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodTrackVsGenEta", "(good trg muons)/(good tracks) vs gen #eta", genEtaLabel, goodTrgOverGoodTrackLabel, nEtaBins);
  TH1D* hGoodTrgOverGoodTrackVsGenEta = Create1DHistogram<TH1D>("hGoodTrgOverGoodTrackVsGenEta", "(good trg muons)/(good tracks) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodTrgOverGoodTrackLabel);

  std::vector<TGraphAsymmErrors*> vgGoodTrgOverGoodTrackVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrgOverGoodTrackVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodTrgOverGoodTrackLabel, nPtBins);
    vgGoodTrgOverGoodTrackVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodTrgOverGoodTrackVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrgOverGoodTrackVsRecoPtEta", "(good trg muons)/(good tracks) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgOverGoodTrackVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodTrgOverGoodTrackVsRecoPtEtaHi", "(good trg muons)/(good tracks) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgOverGoodTrackVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodTrgOverGoodTrackVsRecoPtEtaLo", "(good trg muons)/(good tracks) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodTrackVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodTrackVsRecoPt", "(good trg muons)/(good tracks) vs reco p_{T}", recoPtLabel, goodTrgOverGoodTrackLabel, nPtBins);
  TH1D* hGoodTrgOverGoodTrackVsRecoPt = Create1DHistogram<TH1D>("hGoodTrgOverGoodTrackVsRecoPt", "(good trg muons)/(good tracks) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodTrgOverGoodTrackLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodTrackVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodTrackVsRecoEta", "(good trg muons)/(good tracks) vs reco #eta", recoEtaLabel, goodTrgOverGoodTrackLabel, nEtaBins);
  TH1D* hGoodTrgOverGoodTrackVsRecoEta = Create1DHistogram<TH1D>("hGoodTrgOverGoodTrackVsRecoEta", "(good trg muons)/(good tracks) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodTrgOverGoodTrackLabel);
  
  std::vector<TGraphAsymmErrors*> vgGoodTrgOverGoodRecoMuVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrgOverGoodRecoMuVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrgOverGoodRecoLabel, nPtBins);
    vgGoodTrgOverGoodRecoMuVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodTrgOverGoodRecoMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrgOverGoodRecoMuVsGenPtEta", "(good trg muons)/(good reco muons) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgOverGoodRecoMuVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodTrgOverGoodRecoMuVsGenPtEtaHi", "(good trg muons)/(good reco muons) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgOverGoodRecoMuVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodTrgOverGoodRecoMuVsGenPtEtaLo", "(good trg muons)/(good reco muons) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodRecoMuVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodRecoMuVsGenPt", "(good trg muons)/(good reco muons) vs gen p_{T}", genPtLabel, goodTrgOverGoodRecoLabel, nPtBins);
  TH1D* hGoodTrgOverGoodRecoMuVsGenPt = Create1DHistogram<TH1D>("hGoodTrgOverGoodRecoMuVsGenPt", "(good trg muons)/(good reco muons) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodTrgOverGoodRecoLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodRecoMuVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodRecoMuVsGenEta", "(good trg muons)/(good reco muons) vs gen #eta", genEtaLabel, goodTrgOverGoodRecoLabel, nEtaBins);
  TH1D* hGoodTrgOverGoodRecoMuVsGenEta = Create1DHistogram<TH1D>("hGoodTrgOverGoodRecoMuVsGenEta", "(good trg muons)/(good reco muons) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodTrgOverGoodRecoLabel);

  std::vector<TGraphAsymmErrors*> vgGoodTrgOverGoodRecoMuVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrgOverGoodRecoMuVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodTrgOverGoodRecoLabel, nPtBins);
    vgGoodTrgOverGoodRecoMuVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodTrgOverGoodRecoMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrgOverGoodRecoMuVsRecoPtEta", "(good trg muons)/(good reco muons) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgOverGoodRecoMuVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodTrgOverGoodRecoMuVsRecoPtEtaHi", "(good trg muons)/(good reco muons) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgOverGoodRecoMuVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodTrgOverGoodRecoMuVsRecoPtEtaLo", "(good trg muons)/(good reco muons) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodRecoMuVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodRecoMuVsRecoPt", "(good trg muons)/(good reco muons) vs reco p_{T}", recoPtLabel, goodTrgOverGoodRecoLabel, nPtBins);
  TH1D* hGoodTrgOverGoodRecoMuVsRecoPt = Create1DHistogram<TH1D>("hGoodTrgOverGoodRecoMuVsRecoPt", "(good trg muons)/(good reco muons) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodTrgOverGoodRecoLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodRecoMuVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodRecoMuVsRecoEta", "(good trg muons)/(good reco muons) vs reco #eta", recoEtaLabel, goodTrgOverGoodRecoLabel, nEtaBins);
  TH1D* hGoodTrgOverGoodRecoMuVsRecoEta = Create1DHistogram<TH1D>("hGoodTrgOverGoodRecoMuVsRecoEta", "(good trg muons)/(good reco muons) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodTrgOverGoodRecoLabel);

  std::vector<TGraphAsymmErrors*> vgGoodTrgOverGoodSelMuVsGenPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrgOverGoodSelMuVsGenPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs gen p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrgOverGoodSelLabel, nPtBins);
    vgGoodTrgOverGoodSelMuVsGenPtEta.push_back(tg);
  }
  TH2D* hGoodTrgOverGoodSelMuVsGenPtEta = Create2DHistogram<TH2D>("hGoodTrgOverGoodSelMuVsGenPtEta", "(good trg muons)/(good sel muons) vs gen p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgOverGoodSelMuVsGenPtEtaHi = Create2DHistogram<TH2D>("hGoodTrgOverGoodSelMuVsGenPtEtaHi", "(good trg muons)/(good sel muons) vs gen p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hGoodTrgOverGoodSelMuVsGenPtEtaLo = Create2DHistogram<TH2D>("hGoodTrgOverGoodSelMuVsGenPtEtaLo", "(good trg muons)/(good sel muons) vs gen p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodSelMuVsGenPt = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodSelMuVsGenPt", "(good trg muons)/(good sel muons) vs gen p_{T}", genPtLabel, goodTrgOverGoodSelLabel, nPtBins);
  TH1D* hGoodTrgOverGoodSelMuVsGenPt = Create1DHistogram<TH1D>("hGoodTrgOverGoodSelMuVsGenPt", "(good trg muons)/(good sel muons) vs gen p_{T}", nPtBins, ptMin, ptMax, genPtLabel, goodTrgOverGoodSelLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodSelMuVsGenEta = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodSelMuVsGenEta", "(good trg muons)/(good sel muons) vs gen #eta", genEtaLabel, goodTrgOverGoodSelLabel, nEtaBins);
  TH1D* hGoodTrgOverGoodSelMuVsGenEta = Create1DHistogram<TH1D>("hGoodTrgOverGoodSelMuVsGenEta", "(good trg muons)/(good sel muons) vs gen #eta", nEtaBins, etaMin, etaMax, genEtaLabel, goodTrgOverGoodSelLabel);

  std::vector<TGraphAsymmErrors*> vgGoodTrgOverGoodSelMuVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrgOverGoodSelMuVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodTrgOverGoodSelLabel, nPtBins);
    vgGoodTrgOverGoodSelMuVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodTrgOverGoodSelMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrgOverGoodSelMuVsRecoPtEta", "(good trg muons)/(good sel muons) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgOverGoodSelMuVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodTrgOverGoodSelMuVsRecoPtEtaHi", "(good trg muons)/(good sel muons) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgOverGoodSelMuVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodTrgOverGoodSelMuVsRecoPtEtaLo", "(good trg muons)/(good sel muons) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodSelMuVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodSelMuVsRecoPt", "(good trg muons)/(good sel muons) vs reco p_{T}", recoPtLabel, goodTrgOverGoodSelLabel, nPtBins);
  TH1D* hGoodTrgOverGoodSelMuVsRecoPt = Create1DHistogram<TH1D>("hGoodTrgOverGoodSelMuVsRecoPt", "(good trg muons)/(good sel muons) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodTrgOverGoodSelLabel);
  TGraphAsymmErrors* gGoodTrgOverGoodSelMuVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverGoodSelMuVsRecoEta", "(good trg muons)/(good sel muons) vs reco #eta", recoEtaLabel, goodTrgOverGoodSelLabel, nEtaBins);
  TH1D* hGoodTrgOverGoodSelMuVsRecoEta = Create1DHistogram<TH1D>("hGoodTrgOverGoodSelMuVsRecoEta", "(good trg muons)/(good sel muons) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodTrgOverGoodSelLabel);

  std::vector<TGraphAsymmErrors*> vgGoodRecoOverAllRecoMuVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodRecoOverAllRecoMuVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(all reco muons) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodRecoOverAllRecoLabel, nPtBins);
    vgGoodRecoOverAllRecoMuVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodRecoOverAllRecoMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodRecoOverAllRecoMuVsRecoPtEta", "(good reco muons)/(all reco muons) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodRecoOverAllRecoMuVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodRecoOverAllRecoMuVsRecoPtEtaHi", "(good reco muons)/(all reco muons) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodRecoOverAllRecoMuVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodRecoOverAllRecoMuVsRecoPtEtaLo", "(good reco muons)/(all reco muons) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodRecoOverAllRecoMuVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodRecoOverAllRecoMuVsRecoPt", "(good reco muons)/(all reco muons) vs reco p_{T}", recoPtLabel, goodRecoOverAllRecoLabel, nPtBins);
  TH1D* hGoodRecoOverAllRecoMuVsRecoPt = Create1DHistogram<TH1D>("hGoodRecoOverAllRecoMuVsRecoPt", "(good reco muons)/(all reco muons) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodRecoOverAllRecoLabel);
  TGraphAsymmErrors* gGoodRecoOverAllRecoMuVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodRecoOverAllRecoMuVsRecoEta", "(good reco muons)/(all reco muons) vs reco #eta", recoEtaLabel, goodRecoOverAllRecoLabel, nEtaBins);
  TH1D* hGoodRecoOverAllRecoMuVsRecoEta = Create1DHistogram<TH1D>("hGoodRecoOverAllRecoMuVsRecoEta", "(good reco muons)/(all reco muons) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodRecoOverAllRecoLabel);

  std::vector<TGraphAsymmErrors*> vgGoodSelOverAllSelMuVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodSelOverAllSelMuVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(all sel muons) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodSelOverAllSelLabel, nPtBins);
    vgGoodSelOverAllSelMuVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodSelOverAllSelMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodSelOverAllSelMuVsRecoPtEta", "(good sel muons)/(all sel muons) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodSelOverAllSelMuVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodSelOverAllSelMuVsRecoPtEtaHi", "(good sel muons)/(all sel muons) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodSelOverAllSelMuVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodSelOverAllSelMuVsRecoPtEtaLo", "(good sel muons)/(all sel muons) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodSelOverAllSelMuVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodSelOverAllSelMuVsRecoPt", "(good sel muons)/(all sel muons) vs reco p_{T}", recoPtLabel, goodSelOverAllSelLabel, nPtBins);
  TH1D* hGoodSelOverAllSelMuVsRecoPt = Create1DHistogram<TH1D>("hGoodSelOverAllSelMuVsRecoPt", "(good sel muons)/(all sel muons) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodSelOverAllSelLabel);
  TGraphAsymmErrors* gGoodSelOverAllSelMuVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodSelOverAllSelMuVsRecoEta", "(good sel muons)/(all sel muons) vs reco #eta", recoEtaLabel, goodSelOverAllSelLabel, nEtaBins);
  TH1D* hGoodSelOverAllSelMuVsRecoEta = Create1DHistogram<TH1D>("hGoodSelOverAllSelMuVsRecoEta", "(good sel muons)/(all sel muons) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodSelOverAllSelLabel);
  
  std::vector<TGraphAsymmErrors*> vgGoodTrgOverAllTrgMuVsRecoPtEta;
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    oss.str("");
    oss << "gGoodTrgOverAllTrgMuVsRecoPt_EtaBin" << iGraph+1;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(all trg muons) vs reco p_{T} in #eta bin " << iGraph+1;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodTrgOverAllTrgLabel, nPtBins);
    vgGoodTrgOverAllTrgMuVsRecoPtEta.push_back(tg);
  }
  TH2D* hGoodTrgOverAllTrgMuVsRecoPtEta = Create2DHistogram<TH2D>("hGoodTrgOverAllTrgMuVsRecoPtEta", "(good trg muons)/(all trg muons) vs reco p_{T} and #eta", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgOverAllTrgMuVsRecoPtEtaHi = Create2DHistogram<TH2D>("hGoodTrgOverAllTrgMuVsRecoPtEtaHi", "(good trg muons)/(all trg muons) vs reco p_{T} and #eta - Hi err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hGoodTrgOverAllTrgMuVsRecoPtEtaLo = Create2DHistogram<TH2D>("hGoodTrgOverAllTrgMuVsRecoPtEtaLo", "(good trg muons)/(all trg muons) vs reco p_{T} and #eta - Lo err", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TGraphAsymmErrors* gGoodTrgOverAllTrgMuVsRecoPt = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverAllTrgMuVsRecoPt", "(good trg muons)/(all trg muons) vs reco p_{T}", recoPtLabel, goodTrgOverAllTrgLabel, nPtBins);
  TH1D* hGoodTrgOverAllTrgMuVsRecoPt = Create1DHistogram<TH1D>("hGoodTrgOverAllTrgMuVsRecoPt", "(good trg muons)/(all trg muons) vs reco p_{T}", nPtBins, ptMin, ptMax, recoPtLabel, goodTrgOverAllTrgLabel);
  TGraphAsymmErrors* gGoodTrgOverAllTrgMuVsRecoEta = CreateGraph<TGraphAsymmErrors>("gGoodTrgOverAllTrgMuVsRecoEta", "(good trg muons)/(all trg muons) vs reco #eta", recoEtaLabel, goodTrgOverAllTrgLabel, nEtaBins);
  TH1D* hGoodTrgOverAllTrgMuVsRecoEta = Create1DHistogram<TH1D>("hGoodTrgOverAllTrgMuVsRecoEta", "(good trg muons)/(all trg muons) vs reco #eta", nEtaBins, etaMin, etaMax, recoEtaLabel, goodTrgOverAllTrgLabel);
  
  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodTrackOverGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrackOverGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrackOverGenMuVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodTrackOverGenMuVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodTrackOverGenMuVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrackOverGenMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrackOverGenMuVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrackOverGenMuVsGenEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodRecoOverGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoOverGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoOverGenMuVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodRecoOverGenMuVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodRecoOverGenMuVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodRecoOverGenMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodRecoOverGenMuVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodRecoOverGenMuVsGenEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodRecoOverGoodTrackVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoOverGoodTrackVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoOverGoodTrackVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodRecoOverGoodTrackVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodRecoOverGoodTrackVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodRecoOverGoodTrackVsGenPtSC;
  std::map<short int, TH1D*> mGoodRecoOverGoodTrackVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodRecoOverGoodTrackVsGenEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodRecoOverGoodTrackVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoOverGoodTrackVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodRecoOverGoodTrackVsRecoPtEtaHiSC;
  std::map<short int, TH2D*> mGoodRecoOverGoodTrackVsRecoPtEtaLoSC;
  std::map<short int, TH1D*> mGoodRecoOverGoodTrackVsRecoPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodRecoOverGoodTrackVsRecoPtSC;
  std::map<short int, TH1D*> mGoodRecoOverGoodTrackVsRecoEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodRecoOverGoodTrackVsRecoEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodSelOverGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGenMuVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodSelOverGenMuVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodSelOverGenMuVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGenMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodSelOverGenMuVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGenMuVsGenEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodSelOverGoodTrackVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGoodTrackVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGoodTrackVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodSelOverGoodTrackVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodSelOverGoodTrackVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGoodTrackVsGenPtSC;
  std::map<short int, TH1D*> mGoodSelOverGoodTrackVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGoodTrackVsGenEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodSelOverGoodTrackVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGoodTrackVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGoodTrackVsRecoPtEtaHiSC;
  std::map<short int, TH2D*> mGoodSelOverGoodTrackVsRecoPtEtaLoSC;
  std::map<short int, TH1D*> mGoodSelOverGoodTrackVsRecoPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGoodTrackVsRecoPtSC;
  std::map<short int, TH1D*> mGoodSelOverGoodTrackVsRecoEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGoodTrackVsRecoEtaSC;
  
  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodSelOverGoodRecoMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGoodRecoMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGoodRecoMuVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodSelOverGoodRecoMuVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodSelOverGoodRecoMuVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGoodRecoMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodSelOverGoodRecoMuVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGoodRecoMuVsGenEtaSC;
  
  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGoodRecoMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodSelOverGoodRecoMuVsRecoPtEtaHiSC;
  std::map<short int, TH2D*> mGoodSelOverGoodRecoMuVsRecoPtEtaLoSC;
  std::map<short int, TH1D*> mGoodSelOverGoodRecoMuVsRecoPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGoodRecoMuVsRecoPtSC;
  std::map<short int, TH1D*> mGoodSelOverGoodRecoMuVsRecoEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodSelOverGoodRecoMuVsRecoEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodTrgOverGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGenMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGenMuVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodTrgOverGenMuVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodTrgOverGenMuVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGenMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrgOverGenMuVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGenMuVsGenEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodTrgOverGoodTrackVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodTrackVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodTrackVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodTrackVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodTrackVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodTrackVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodTrackVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodTrackVsGenEtaSC;

  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodTrgOverGoodTrackVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodTrackVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodTrackVsRecoPtEtaHiSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodTrackVsRecoPtEtaLoSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodTrackVsRecoPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodTrackVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodTrackVsRecoEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodTrackVsRecoEtaSC;
  
  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodRecoMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodRecoMuVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodRecoMuVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodRecoMuVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodRecoMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodRecoMuVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodRecoMuVsGenEtaSC;
  
  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodRecoMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodRecoMuVsRecoPtEtaHiSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodRecoMuVsRecoPtEtaLoSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodRecoMuVsRecoPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodRecoMuVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodRecoMuVsRecoEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodRecoMuVsRecoEtaSC;
  
  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodTrgOverGoodSelMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodSelMuVsGenPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodSelMuVsGenPtEtaHiSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodSelMuVsGenPtEtaLoSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodSelMuVsGenPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodSelMuVsGenPtSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodSelMuVsGenEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodSelMuVsGenEtaSC;
  
  std::map<short int, std::vector<TGraphAsymmErrors*> > mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodSelMuVsRecoPtEtaSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodSelMuVsRecoPtEtaHiSC;
  std::map<short int, TH2D*> mGoodTrgOverGoodSelMuVsRecoPtEtaLoSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodSelMuVsRecoPtSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodSelMuVsRecoPtSC;
  std::map<short int, TH1D*> mGoodTrgOverGoodSelMuVsRecoEtaSC;
  std::map<short int, TGraphAsymmErrors*> mgGoodTrgOverGoodSelMuVsRecoEtaSC;
  
  for(unsigned int i = 0; i < 7; i++)
  {
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodTrackOverGenMuVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good tracks)/(gen muons) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrackOverGenLabel, nPtBins);
      mvgGoodTrackOverGenMuVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodTrackOverGenMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good tracks)/(gen muons) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    TH2D* histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrackOverGenMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrackOverGenMuVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good tracks)/(gen muons) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrackOverGenMuVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodTrackOverGenMuVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good tracks)/(gen muons) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrackOverGenMuVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodTrackOverGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good tracks)/(gen muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    TH1D* histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodTrackOverGenLabel);
    mGoodTrackOverGenMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrackOverGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good tracks)/(gen muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    TGraphAsymmErrors* tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodTrackOverGenLabel, nPtBins);
    mgGoodTrackOverGenMuVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodTrackOverGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good tracks)/(gen muons) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodTrackOverGenLabel);
    mGoodTrackOverGenMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrackOverGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good tracks)/(gen muons) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodTrackOverGenLabel, nEtaBins);
    mgGoodTrackOverGenMuVsGenEtaSC[i] = tg;

    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodRecoOverGenMuVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good reco muons)/(gen muons) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodRecoOverGenLabel, nPtBins);
      mvgGoodRecoOverGenMuVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodRecoOverGenMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(gen muons) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoOverGenMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGenMuVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(gen muons) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoOverGenMuVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGenMuVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(gen muons) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoOverGenMuVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(gen muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodRecoOverGenLabel);
    mGoodRecoOverGenMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodRecoOverGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(gen muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodRecoOverGenLabel, nPtBins);
    mgGoodRecoOverGenMuVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodRecoOverGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(gen muons) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodRecoOverGenLabel);
    mGoodRecoOverGenMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodRecoOverGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(gen muons) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodRecoOverGenLabel, nEtaBins);
    mgGoodRecoOverGenMuVsGenEtaSC[i] = tg;

    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodRecoOverGoodTrackVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good reco muons)/(good tracks) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodRecoOverGoodTrackLabel, nPtBins);
      mvgGoodRecoOverGoodTrackVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoOverGoodTrackVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoOverGoodTrackVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodRecoOverGoodTrackVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodRecoOverGoodTrackLabel);
    mGoodRecoOverGoodTrackVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodRecoOverGoodTrackVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodRecoOverGoodTrackLabel, nPtBins);
    mgGoodRecoOverGoodTrackVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodRecoOverGoodTrackLabel);
    mGoodRecoOverGoodTrackVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodRecoOverGoodTrackVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodRecoOverGoodTrackLabel, nEtaBins);
    mgGoodRecoOverGoodTrackVsGenEtaSC[i] = tg;

    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodRecoOverGoodTrackVsRecoPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good reco muons)/(good tracks) vs reco p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodRecoOverGoodTrackLabel, nPtBins);
      mvgGoodRecoOverGoodTrackVsRecoPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodRecoOverGoodTrackVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsRecoPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs reco p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodRecoOverGoodTrackVsRecoPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsRecoPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs reco p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodRecoOverGoodTrackVsRecoPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, goodRecoOverGoodTrackLabel);
    mGoodRecoOverGoodTrackVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodRecoOverGoodTrackVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs reco p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoPtLabel, goodRecoOverGoodTrackLabel, nPtBins);
    mgGoodRecoOverGoodTrackVsRecoPtSC[i] = tg;
    oss.str("");
    oss << "hGoodRecoOverGoodTrackVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, goodRecoOverGoodTrackLabel);
    mGoodRecoOverGoodTrackVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodRecoOverGoodTrackVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good reco muons)/(good tracks) vs reco #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoEtaLabel, goodRecoOverGoodTrackLabel, nEtaBins);
    mgGoodRecoOverGoodTrackVsRecoEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodSelOverGenMuVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good sel muons)/(gen muons) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodSelOverGenLabel, nPtBins);
      mvgGoodSelOverGenMuVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodSelOverGenMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(gen muons) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGenMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGenMuVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(gen muons) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGenMuVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGenMuVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(gen muons) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGenMuVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(gen muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodSelOverGenLabel);
    mGoodSelOverGenMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(gen muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodSelOverGenLabel, nPtBins);
    mgGoodSelOverGenMuVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodSelOverGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(gen muons) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodSelOverGenLabel);
    mGoodSelOverGenMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(gen muons) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodSelOverGenLabel, nEtaBins);
    mgGoodSelOverGenMuVsGenEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodSelOverGoodTrackVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good sel muons)/(good tracks) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodSelOverGoodTrackLabel, nPtBins);
      mvgGoodSelOverGoodTrackVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGoodTrackVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGoodTrackVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGoodTrackVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodSelOverGoodTrackLabel);
    mGoodSelOverGoodTrackVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGoodTrackVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodSelOverGoodTrackLabel, nPtBins);
    mgGoodSelOverGoodTrackVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodSelOverGoodTrackLabel);
    mGoodSelOverGoodTrackVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGoodTrackVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodSelOverGoodTrackLabel, nEtaBins);
    mgGoodSelOverGoodTrackVsGenEtaSC[i] = tg;

    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodSelOverGoodTrackVsRecoPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good sel muons)/(good tracks) vs reco p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodSelOverGoodTrackLabel, nPtBins);
      mvgGoodSelOverGoodTrackVsRecoPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelOverGoodTrackVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsRecoPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs reco p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelOverGoodTrackVsRecoPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsRecoPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs reco p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelOverGoodTrackVsRecoPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, goodSelOverGoodTrackLabel);
    mGoodSelOverGoodTrackVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGoodTrackVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs reco p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoPtLabel, goodSelOverGoodTrackLabel, nPtBins);
    mgGoodSelOverGoodTrackVsRecoPtSC[i] = tg;
    oss.str("");
    oss << "hGoodSelOverGoodTrackVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, goodSelOverGoodTrackLabel);
    mGoodSelOverGoodTrackVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGoodTrackVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good tracks) vs reco #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoEtaLabel, goodSelOverGoodTrackLabel, nEtaBins);
    mgGoodSelOverGoodTrackVsRecoEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodSelOverGoodRecoMuVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good sel muons)/(good reco muons) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodSelOverGoodRecoLabel, nPtBins);
      mvgGoodSelOverGoodRecoMuVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGoodRecoMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGoodRecoMuVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodSelOverGoodRecoMuVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodSelOverGoodRecoLabel);
    mGoodSelOverGoodRecoMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGoodRecoMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodSelOverGoodRecoLabel, nPtBins);
    mgGoodSelOverGoodRecoMuVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodSelOverGoodRecoLabel);
    mGoodSelOverGoodRecoMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGoodRecoMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodSelOverGoodRecoLabel, nEtaBins);
    mgGoodSelOverGoodRecoMuVsGenEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodSelOverGoodRecoMuVsRecoPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good sel muons)/(good reco muons) vs reco p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodSelOverGoodRecoLabel, nPtBins);
      mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelOverGoodRecoMuVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsRecoPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs reco p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelOverGoodRecoMuVsRecoPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsRecoPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs reco p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodSelOverGoodRecoMuVsRecoPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, goodSelOverGoodRecoLabel);
    mGoodSelOverGoodRecoMuVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGoodRecoMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs reco p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoPtLabel, goodSelOverGoodRecoLabel, nPtBins);
    mgGoodSelOverGoodRecoMuVsRecoPtSC[i] = tg;
    oss.str("");
    oss << "hGoodSelOverGoodRecoMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, goodSelOverGoodRecoLabel);
    mGoodSelOverGoodRecoMuVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodSelOverGoodRecoMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good sel muons)/(good reco muons) vs reco #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoEtaLabel, goodSelOverGoodRecoLabel, nEtaBins);
    mgGoodSelOverGoodRecoMuVsRecoEtaSC[i] = tg;

    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodTrgOverGenMuVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good trg muons)/(gen muons) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrgOverGenLabel, nPtBins);
      mvgGoodTrgOverGenMuVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodTrgOverGenMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(gen muons) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGenMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGenMuVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(gen muons) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGenMuVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGenMuVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(gen muons) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGenMuVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(gen muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodTrgOverGenLabel);
    mGoodTrgOverGenMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGenMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(gen muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodTrgOverGenLabel, nPtBins);
    mgGoodTrgOverGenMuVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodTrgOverGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(gen muons) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodTrgOverGenLabel);
    mGoodTrgOverGenMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGenMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(gen muons) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodTrgOverGenLabel, nEtaBins);
    mgGoodTrgOverGenMuVsGenEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodTrgOverGoodTrackVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good trg muons)/(good tracks) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrgOverGoodTrackLabel, nPtBins);
      mvgGoodTrgOverGoodTrackVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodTrackVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodTrackVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodTrackVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodTrgOverGoodTrackLabel);
    mGoodTrgOverGoodTrackVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodTrackVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodTrgOverGoodTrackLabel, nPtBins);
    mgGoodTrgOverGoodTrackVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodTrgOverGoodTrackLabel);
    mGoodTrgOverGoodTrackVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodTrackVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodTrgOverGoodTrackLabel, nEtaBins);
    mgGoodTrgOverGoodTrackVsGenEtaSC[i] = tg;

    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodTrgOverGoodTrackVsRecoPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good trg muons)/(good tracks) vs reco p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodTrgOverGoodTrackLabel, nPtBins);
      mvgGoodTrgOverGoodTrackVsRecoPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodTrackVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsRecoPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs reco p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodTrackVsRecoPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsRecoPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs reco p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodTrackVsRecoPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, goodTrgOverGoodTrackLabel);
    mGoodTrgOverGoodTrackVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodTrackVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs reco p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoPtLabel, goodTrgOverGoodTrackLabel, nPtBins);
    mgGoodTrgOverGoodTrackVsRecoPtSC[i] = tg;
    oss.str("");
    oss << "hGoodTrgOverGoodTrackVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, goodTrgOverGoodTrackLabel);
    mGoodTrgOverGoodTrackVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodTrackVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good tracks) vs reco #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoEtaLabel, goodTrgOverGoodTrackLabel, nEtaBins);
    mgGoodTrgOverGoodTrackVsRecoEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodTrgOverGoodRecoMuVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good trg muons)/(good reco muons) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrgOverGoodRecoLabel, nPtBins);
      mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodRecoMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodRecoMuVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodRecoMuVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodTrgOverGoodRecoLabel);
    mGoodTrgOverGoodRecoMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodRecoMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodTrgOverGoodRecoLabel, nPtBins);
    mgGoodTrgOverGoodRecoMuVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodTrgOverGoodRecoLabel);
    mGoodTrgOverGoodRecoMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodRecoMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodTrgOverGoodRecoLabel, nEtaBins);
    mgGoodTrgOverGoodRecoMuVsGenEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodTrgOverGoodRecoMuVsRecoPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good trg muons)/(good reco muons) vs reco p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodTrgOverGoodRecoLabel, nPtBins);
      mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsRecoPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs reco p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodRecoMuVsRecoPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsRecoPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs reco p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodRecoMuVsRecoPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, goodTrgOverGoodRecoLabel);
    mGoodTrgOverGoodRecoMuVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodRecoMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs reco p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoPtLabel, goodTrgOverGoodRecoLabel, nPtBins);
    mgGoodTrgOverGoodRecoMuVsRecoPtSC[i] = tg;
    oss.str("");
    oss << "hGoodTrgOverGoodRecoMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, goodTrgOverGoodRecoLabel);
    mGoodTrgOverGoodRecoMuVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodRecoMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good reco muons) vs reco #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoEtaLabel, goodTrgOverGoodRecoLabel, nEtaBins);
    mgGoodTrgOverGoodRecoMuVsRecoEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodTrgOverGoodSelMuVsGenPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good trg muons)/(good sel muons) vs gen p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), genPtLabel, goodTrgOverGoodSelLabel, nPtBins);
      mvgGoodTrgOverGoodSelMuVsGenPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsGenPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs gen p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodSelMuVsGenPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsGenPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs gen p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodSelMuVsGenPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsGenPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs gen p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
    mGoodTrgOverGoodSelMuVsGenPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, genPtLabel, goodTrgOverGoodSelLabel);
    mGoodTrgOverGoodSelMuVsGenPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodSelMuVsGenPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs gen p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genPtLabel, goodTrgOverGoodSelLabel, nPtBins);
    mgGoodTrgOverGoodSelMuVsGenPtSC[i] = tg;
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs gen #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, genEtaLabel, goodTrgOverGoodSelLabel);
    mGoodTrgOverGoodSelMuVsGenEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodSelMuVsGenEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs gen #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), genEtaLabel, goodTrgOverGoodSelLabel, nEtaBins);
    mgGoodTrgOverGoodSelMuVsGenEtaSC[i] = tg;
    
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      oss.str("");
      oss << "gGoodTrgOverGoodSelMuVsRecoPtSC" << i << "_EtaBin" << iGraph+1;
      name = oss.str();
      oss.str("");
      oss << "(good trg muons)/(good sel muons) vs reco p_{T} in #eta bin " << iGraph+1 << " for muon category " << i;
      title = oss.str();
      tg = CreateGraph<TGraphAsymmErrors>(name.c_str(), title.c_str(), recoPtLabel, goodTrgOverGoodSelLabel, nPtBins);
      mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC[i].push_back(tg);
    }
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsRecoPtEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs reco p_{T} and #eta for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodSelMuVsRecoPtEtaSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsRecoPtEtaHiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs reco p_{T} and #eta - Hi err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodSelMuVsRecoPtEtaHiSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsRecoPtEtaLoSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs reco p_{T} and #eta - Lo err - for muon category " << i;
    title = oss.str();
    histo = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
    mGoodTrgOverGoodSelMuVsRecoPtEtaLoSC[i] = histo;
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs reco p_{T} for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nPtBins, ptMin, ptMax, recoPtLabel, goodTrgOverGoodSelLabel);
    mGoodTrgOverGoodSelMuVsRecoPtSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodSelMuVsRecoPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs reco p_{T} for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoPtLabel, goodTrgOverGoodSelLabel, nPtBins);
    mgGoodTrgOverGoodSelMuVsRecoPtSC[i] = tg;
    oss.str("");
    oss << "hGoodTrgOverGoodSelMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs reco #eta for muon category " << i;
    title = oss.str();
    histo1d = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), nEtaBins, etaMin, etaMax, recoEtaLabel, goodTrgOverGoodSelLabel);
    mGoodTrgOverGoodSelMuVsRecoEtaSC[i] = histo1d;
    oss.str("");
    oss << "hgGoodTrgOverGoodSelMuVsRecoEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "(good trg muons)/(good sel muons) vs reco #eta for muon category " << i;
    title = oss.str();
    tg = CreateGraph<TGraphAsymmErrors>(name.c_str(),title.c_str(), recoEtaLabel, goodTrgOverGoodSelLabel, nEtaBins);
    mgGoodTrgOverGoodSelMuVsRecoEtaSC[i] = tg;
  }

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    // How to find efficiencies

    // Find all gen muons
    std::vector<unsigned short> vGenMuons;
    std::vector<short> vGenMuonsProvenance;
    std::vector<unsigned short> vGenMuonsSimplifiedCode;
    for(unsigned int iGenp = 0; iGenp < Genp_Id->size(); iGenp++)
    {
      if(abs(Genp_Id->at(iGenp)) == 13)
      {
        vGenMuons.push_back(iGenp);
        vGenMuonsProvenance.push_back(FindGenMuonMCProvenance(iGenp));
        vGenMuonsSimplifiedCode.push_back(FindSimplifiedMuonCode(vGenMuonsProvenance[vGenMuonsProvenance.size()-1]));
      }
    }

    // Associate all tracks
    std::vector<unsigned int> vTrackGenp;
    std::vector<int> vTrackGenpId;
    std::vector<double> vTrackAssocQuality;
    for(int iTrack = 0; iTrack < numberOfTrack; iTrack++)
    {
      if(Track_vGenprefByHits->at(iTrack).size() > 0)
      {
        unsigned short iGenp = Track_vGenprefByHits->at(iTrack).at(0);
        int idGenp = Genp_Id->at(iGenp);
        vTrackGenp.push_back(iGenp);
        vTrackGenpId.push_back(idGenp);
        vTrackAssocQuality.push_back(Track_vGenpQualityByHits->at(iTrack).at(0));
      }
      else
      {
        vTrackGenp.push_back(65535);
        vTrackGenpId.push_back(0);
        vTrackAssocQuality.push_back(0);
      }
    }

    // Associate all reco muons (inner, outer and global track)
    std::vector<unsigned int> vMuonInTrkGenp;
    std::vector<int> vMuonInTrkGenpId;
    std::vector<double> vMuonInTrkAssocQuality;
    std::vector<unsigned int> vMuonOutTrkGenp;
    std::vector<int> vMuonOutTrkGenpId;
    std::vector<double> vMuonOutTrkAssocQuality;
    std::vector<unsigned int> vMuonGlbTrkGenp;
    std::vector<int> vMuonGlbTrkGenpId;
    std::vector<double> vMuonGlbTrkAssocQuality;
    std::vector<short int> muonClass;
    std::vector<short int> muonMCProvenance;
    std::vector<short int> muonMergedCode;
    std::vector<short int> muonSimplifiedCode;
    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      AssociateInnerMuonTrack(iMuon);
      vMuonInTrkGenp.push_back(muonInTrkGenp);
      vMuonInTrkGenpId.push_back(muonInTrkGenpId);
      vMuonInTrkAssocQuality.push_back(muonInTrkAssocQuality);
      AssociateOuterMuonTrack(iMuon);
      vMuonOutTrkGenp.push_back(muonOutTrkGenp);
      vMuonOutTrkGenpId.push_back(muonOutTrkGenpId);
      vMuonOutTrkAssocQuality.push_back(muonOutTrkAssocQuality);
      AssociateGlobalMuonTrack(iMuon);
      vMuonGlbTrkGenp.push_back(muonGlbTrkGenp);
      vMuonGlbTrkGenpId.push_back(muonGlbTrkGenpId);
      vMuonGlbTrkAssocQuality.push_back(muonGlbTrkAssocQuality);
      muonClass.push_back(FindMuonClassFromMCAssociation(iMuon));
      muonMCProvenance.push_back(FindMuonMCProvenance(iMuon,muonClass[iMuon]));
      muonMergedCode.push_back(MergeMuonClassAndMCProvenance(muonClass[iMuon],muonMCProvenance[iMuon]));
      muonSimplifiedCode.push_back(FindSimplifiedMuonCode(muonMergedCode[iMuon]));
    }

    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
//       if(!((Muon_selmask->at(iMuon)&1<<4) && (Muon_selmask->at(iMuon)&1<<12)))
//         continue;
//       std::cout << "Muon " << iMuon << std::endl;
      hAllRecoMuVsRecoPtEta->Fill(Muon_eta->at(iMuon), Muon_pt->at(iMuon));
      hAllRecoMuVsRecoPt->Fill(Muon_pt->at(iMuon));
      hAllRecoMuVsRecoEta->Fill(Muon_eta->at(iMuon));
      if(fabs(Muon_eta->at(iMuon)) < etaIn && Muon_pt->at(iMuon) > ptIn)
      {
        hAllRecoMuInsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
        hAllRecoMuInsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
        hAllRecoMuInsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
      }

//       short int difCode = FindDIFMuonProvenance(iMuon);
//       std::cout << "\n";
//       std::cout << "muonMergedCode = " << muonMergedCode[iMuon] << std::endl;
//       std::cout << "difCode = " << difCode << std::endl;

      if(s->SelectMuon(iMuon,text.c_str()))
      {
        hAllSelMuVsRecoPtEta->Fill(Muon_eta->at(iMuon), Muon_pt->at(iMuon));
        hAllSelMuVsRecoPt->Fill(Muon_pt->at(iMuon));
        hAllSelMuVsRecoEta->Fill(Muon_eta->at(iMuon));
        if(fabs(Muon_eta->at(iMuon)) < etaIn && Muon_pt->at(iMuon) > ptIn)
        {
          hAllSelMuInsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
          hAllSelMuInsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
          hAllSelMuInsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
        }
        
        if(Muon_hasTriggered->at(iMuon))
        {
          hAllTrgMuVsRecoPtEta->Fill(Muon_eta->at(iMuon), Muon_pt->at(iMuon));
          hAllTrgMuVsRecoPt->Fill(Muon_pt->at(iMuon));
          hAllTrgMuVsRecoEta->Fill(Muon_eta->at(iMuon));
          if(fabs(Muon_eta->at(iMuon)) < etaIn && Muon_pt->at(iMuon) > ptIn)
          {
            hAllTrgMuInsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
            hAllTrgMuInsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
            hAllTrgMuInsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
          }
        }          
      }
    }

    OneToOneMapWithQuality mTrackGenMuonAssociationMap;
    for(unsigned int i = 0; i < vGenMuons.size(); i++)
    {
      unsigned short iGenMuon = vGenMuons[i];
      hAllGenMuVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
      hAllGenMuVsGenPt->Fill(Genp_pt->at(iGenMuon));
      hAllGenMuVsGenEta->Fill(Genp_eta->at(iGenMuon));
      mAllGenMuVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
      mAllGenMuVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
      mAllGenMuVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
      if(fabs(Genp_eta->at(iGenMuon)) > etaOut || Genp_pt->at(iGenMuon) < ptOut)
      {
        hAllGenMuOutsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        hAllGenMuOutsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
        hAllGenMuOutsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
        mAllGenMuOutsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        mAllGenMuOutsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
        mAllGenMuOutsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
      }
      if(fabs(Genp_eta->at(iGenMuon)) < etaOut && Genp_pt->at(iGenMuon) > ptOut)
      {
        hAllGenMuInAcceptanceVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        hAllGenMuInAcceptanceVsGenPt->Fill(Genp_pt->at(iGenMuon));
        hAllGenMuInAcceptanceVsGenEta->Fill(Genp_eta->at(iGenMuon));
        mAllGenMuInAcceptanceVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        mAllGenMuInAcceptanceVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
        mAllGenMuInAcceptanceVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
      }

      // Fill the association map between gen muons and tracks
      unsigned int iAssociatedTrack = 65535;
      double iAssociatedQuality = 0;

      // First find the best track associated to this gen muon
      for(int iTrack = 0; iTrack < numberOfTrack; iTrack++)
      {
        // Track is associated to a muon
        if(abs(vTrackGenpId[iTrack]) == 13)
        {
          if(iGenMuon == vTrackGenp[iTrack])
          {
            if(vTrackAssocQuality[iTrack] > iAssociatedQuality)
            {
              iAssociatedTrack = iTrack;
              iAssociatedQuality = vTrackAssocQuality[iTrack];
            }
          }
        }
      }
      if(iAssociatedTrack == 65535)
      {
        continue;
      }

      // Then arbitrate the association on the other way, to find if this track-gen association is the best one
      // No other gen muons associated to the same track found up to now
      if(mTrackGenMuonAssociationMap.find(iAssociatedTrack) == mTrackGenMuonAssociationMap.end())
      {
        mTrackGenMuonAssociationMap[iAssociatedTrack] = std::make_pair(i, iAssociatedQuality);
      }
      else
      {
        // Otherwise save only the association with the best quality
        double quality = mTrackGenMuonAssociationMap[iAssociatedTrack].second;
        unsigned int iGenMuonInMap = vGenMuons[mTrackGenMuonAssociationMap[iAssociatedTrack].first];
        if(quality < vTrackAssocQuality[iAssociatedTrack])
        {
          mTrackGenMuonAssociationMap[iAssociatedTrack].first = i;
          mTrackGenMuonAssociationMap[iAssociatedTrack].second = iAssociatedQuality;
        }
        else if(quality == iAssociatedQuality)
        {
          // If quality is the same, then check best Delta(R)
          TVector3 vTrack, vGenMuon, vGenMuonInMap;
          vTrack.SetPtEtaPhi(Track_pt->at(iAssociatedTrack), Track_eta->at(iAssociatedTrack), Track_phi->at(iAssociatedTrack));
          vGenMuon.SetPtEtaPhi(Genp_pt->at(iGenMuon), Genp_eta->at(iGenMuon), Genp_phi->at(iGenMuon));
          vGenMuonInMap.SetPtEtaPhi(Genp_pt->at(iGenMuonInMap), Genp_eta->at(iGenMuonInMap), Genp_phi->at(iGenMuonInMap));
          double deltaR = vTrack.DeltaR(vGenMuon);
          double deltaRInMap = vTrack.DeltaR(vGenMuonInMap);
          if(deltaR < deltaRInMap)
          {
            mTrackGenMuonAssociationMap[iAssociatedTrack].first = i;
            mTrackGenMuonAssociationMap[iAssociatedTrack].second = iAssociatedQuality;
          }
        }
      }
    }

    if(mTrackGenMuonAssociationMap.size() > vGenMuons.size())
    {
      std::cout << "W A R N I N G ! More good tracks than gen muons!\n";
    }

// LEGACY CODE - KEPT FOR REFERENCE ONLY
//     OneToOneMapWithQuality mRecoGenMuonAssociationMap;
//     for(unsigned int i = 0; i < vGenMuons.size(); i++)
//     {
//       unsigned short iGenMuon = vGenMuons[i];
//       hAllGenMuVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
//       hAllGenMuVsGenPt->Fill(Genp_pt->at(iGenMuon));
//       hAllGenMuVsGenEta->Fill(Genp_eta->at(iGenMuon));
//       mAllGenMuVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
//       mAllGenMuVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
//       mAllGenMuVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
//       if(fabs(Genp_eta->at(iGenMuon)) > etaOut || Genp_pt->at(iGenMuon) < ptOut)
//       {
//         hAllGenMuOutsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
//         hAllGenMuOutsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
//         hAllGenMuOutsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
//         mAllGenMuOutsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
//         mAllGenMuOutsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
//         mAllGenMuOutsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
//       }
//       if(fabs(Genp_eta->at(iGenMuon)) < etaOut && Genp_pt->at(iGenMuon) > ptOut)
//       {
//         hAllGenMuInAcceptanceVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
//         hAllGenMuInAcceptanceVsGenPt->Fill(Genp_pt->at(iGenMuon));
//         hAllGenMuInAcceptanceVsGenEta->Fill(Genp_eta->at(iGenMuon));
//         mAllGenMuInAcceptanceVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
//         mAllGenMuInAcceptanceVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
//         mAllGenMuInAcceptanceVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
//       }
//
//       // Fill the association map between gen and reco muons
//       unsigned int iAssociatedMuon = 65535;
//       double iAssociatedQuality = 0;
//
//       // First find the best reco muon associated to this gen muon
//       for(unsigned int iMuon = 0; iMuon < numberOfMuon; iMuon++)
//       {
//         // Codes <= 8 are for fake muons
//         // Gen muon is associated to reco muon
//         if(abs(muonMergedCode[iMuon]) > 8)
//         {
//           bool inAssoc = false;
//           bool outAssoc = false;
//           double quality = 0;
//           if(iGenMuon == vMuonInTrkGenp[iMuon])
//             inAssoc = true;
//           if(iGenMuon == vMuonOutTrkGenp[iMuon])
//             outAssoc = true;
//           if(inAssoc && outAssoc)
//           {
//             quality = std::max(vMuonInTrkAssocQuality[iMuon], vMuonOutTrkAssocQuality[iMuon]);
//           }
//           else if(inAssoc)
//             quality = vMuonInTrkAssocQuality[iMuon];
//           else if(outAssoc)
//             quality = vMuonOutTrkAssocQuality[iMuon];
//           if(inAssoc || outAssoc)
//           {
//             if(quality > iAssociatedQuality)
//             {
//               iAssociatedMuon = iMuon;
//               iAssociatedQuality = quality;
//             }
//           }
//         }
//       }
//       if(iAssociatedMuon == 65535)
//         continue;
//
//       // Then arbitrate the association on the other way: find if this reco-gen association is the best one
//       // No other gen muons associated to the same reco muon found up to now
//       if(mRecoGenMuonAssociationMap.find(iAssociatedMuon) == mRecoGenMuonAssociationMap.end())
//       {
//         mRecoGenMuonAssociationMap[iAssociatedMuon] = std::make_pair(iGenMuon, iAssociatedQuality);
//       }
//       else
//       {
//         // Otherwise just save the association with the best quality
//         double quality = mRecoGenMuonAssociationMap[iAssociatedMuon].second;
//         unsigned int iGenMuonInMap = mRecoGenMuonAssociationMap[iAssociatedMuon].first;
//         if(quality < std::max(vMuonInTrkAssocQuality[iAssociatedMuon], vMuonOutTrkAssocQuality[iAssociatedMuon]))
//         {
//           mRecoGenMuonAssociationMap[iAssociatedMuon].first = iGenMuon;
//           mRecoGenMuonAssociationMap[iAssociatedMuon].second = iAssociatedQuality;
//         }
//         else if(quality == iAssociatedQuality)
//         {
//           // if quality is the same, then check best Delta(R)
//           TVector3 vMuon, vGenMuon, vGenMuonInMap;
//           vMuon.SetPtEtaPhi(Muon_pt->at(iAssociatedMuon), Muon_eta->at(iAssociatedMuon), Muon_phi->at(iAssociatedMuon));
//           vGenMuon.SetPtEtaPhi(Genp_pt->at(iGenMuon), Genp_eta->at(iGenMuon), Genp_phi->at(iGenMuon));
//           vGenMuonInMap.SetPtEtaPhi(Genp_pt->at(iGenMuonInMap), Genp_eta->at(iGenMuonInMap), Genp_phi->at(iGenMuonInMap));
//           double deltaR = vMuon.DeltaR(vGenMuon);
//           double deltaRInMap = vMuon.DeltaR(vGenMuonInMap);
//           if(deltaR < deltaRInMap)
//           {
//             mRecoGenMuonAssociationMap[iAssociatedMuon].first = iGenMuon;
//             mRecoGenMuonAssociationMap[iAssociatedMuon].second = iAssociatedQuality;
//           }
//         }
//       }
//     }
//
//    if(mRecoGenMuonAssociationMap.size() > vGenMuons.size())
//    {
//      std::cout << "W A R N I N G ! More good reco muons than gen muons!\n";
//    }
// END OF LEGACY CODE

    // Then loop over the association map to fill the histograms
    for(OneToOneMapWithQuality::const_iterator it = mTrackGenMuonAssociationMap.begin(); it != mTrackGenMuonAssociationMap.end(); it++)
    {
      unsigned int iTrack = it->first;
      unsigned int i = it->second.first;
      unsigned int iGenMuon = vGenMuons[i];

      hGoodTrackVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
      hGoodTrackVsGenPt->Fill(Genp_pt->at(iGenMuon));
      hGoodTrackVsGenEta->Fill(Genp_eta->at(iGenMuon));
      mGoodTrackVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
      mGoodTrackVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
      mGoodTrackVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
      hGoodTrackVsRecoPtEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
      hGoodTrackVsRecoPt->Fill(Track_pt->at(iTrack));
      hGoodTrackVsRecoEta->Fill(Track_eta->at(iTrack));
      mGoodTrackVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
      mGoodTrackVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_pt->at(iTrack));
      mGoodTrackVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_eta->at(iTrack));
      if(fabs(Track_eta->at(iTrack)) < etaIn && Track_pt->at(iTrack) > ptIn)
      {
        hGoodTrackInsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        hGoodTrackInsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
        hGoodTrackInsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
        mGoodTrackInsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        mGoodTrackInsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
        mGoodTrackInsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
        hGoodTrackInsideCutsVsRecoPtEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
        hGoodTrackInsideCutsVsRecoPt->Fill(Track_pt->at(iTrack));
        hGoodTrackInsideCutsVsRecoEta->Fill(Track_eta->at(iTrack));
        mGoodTrackInsideCutsVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
        mGoodTrackInsideCutsVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_pt->at(iTrack));
        mGoodTrackInsideCutsVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_eta->at(iTrack));
      }
      if(fabs(Genp_eta->at(iGenMuon)) > etaOut || Genp_pt->at(iGenMuon) < ptOut)
      {
        hGoodTrackOutsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        hGoodTrackOutsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
        hGoodTrackOutsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
        mGoodTrackOutsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        mGoodTrackOutsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
        mGoodTrackOutsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
        hGoodTrackOutsideCutsVsRecoPtEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
        hGoodTrackOutsideCutsVsRecoPt->Fill(Track_pt->at(iTrack));
        hGoodTrackOutsideCutsVsRecoEta->Fill(Track_eta->at(iTrack));
        mGoodTrackOutsideCutsVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
        mGoodTrackOutsideCutsVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_pt->at(iTrack));
        mGoodTrackOutsideCutsVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Track_eta->at(iTrack));
      }

      unsigned short iMuon = 65535;
      for(unsigned short iMu = 0; iMu < numberOfMuon; iMu++)
      {
        if(iTrack == Muon_trackref->at(iMu))
        {
          iMuon = iMu;
          break;
        }
      }
      if(iMuon == 65535)
        continue;

      hGoodRecoMuVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
      hGoodRecoMuVsGenPt->Fill(Genp_pt->at(iGenMuon));
      hGoodRecoMuVsGenEta->Fill(Genp_eta->at(iGenMuon));
      mGoodRecoMuVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
      mGoodRecoMuVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
      mGoodRecoMuVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
      hGoodRecoMuVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
      hGoodRecoMuVsRecoPt->Fill(Muon_pt->at(iMuon));
      hGoodRecoMuVsRecoEta->Fill(Muon_eta->at(iMuon));
      mGoodRecoMuVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
      mGoodRecoMuVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
      mGoodRecoMuVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
      if(fabs(Muon_eta->at(iMuon)) < etaIn && Muon_pt->at(iMuon) > ptIn)
      {
        hGoodRecoMuInsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        hGoodRecoMuInsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
        hGoodRecoMuInsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
        mGoodRecoMuInsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        mGoodRecoMuInsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
        mGoodRecoMuInsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
        hGoodRecoMuInsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
        hGoodRecoMuInsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
        hGoodRecoMuInsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
        mGoodRecoMuInsideCutsVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
        mGoodRecoMuInsideCutsVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
        mGoodRecoMuInsideCutsVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
      }
      if(fabs(Genp_eta->at(iGenMuon)) > etaOut || Genp_pt->at(iGenMuon) < ptOut)
      {
        hGoodRecoMuOutsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        hGoodRecoMuOutsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
        hGoodRecoMuOutsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
        mGoodRecoMuOutsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        mGoodRecoMuOutsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
        mGoodRecoMuOutsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
        hGoodRecoMuOutsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
        hGoodRecoMuOutsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
        hGoodRecoMuOutsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
        mGoodRecoMuOutsideCutsVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
        mGoodRecoMuOutsideCutsVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
        mGoodRecoMuOutsideCutsVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
      }

      if(s->SelectMuon(iMuon,text.c_str()))
      {
        hGoodSelMuVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        hGoodSelMuVsGenPt->Fill(Genp_pt->at(iGenMuon));
        hGoodSelMuVsGenEta->Fill(Genp_eta->at(iGenMuon));
        mGoodSelMuVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
        mGoodSelMuVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
        mGoodSelMuVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
        hGoodSelMuVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
        hGoodSelMuVsRecoPt->Fill(Muon_pt->at(iMuon));
        hGoodSelMuVsRecoEta->Fill(Muon_eta->at(iMuon));
        mGoodSelMuVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
        mGoodSelMuVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
        mGoodSelMuVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
        if(fabs(Muon_eta->at(iMuon)) < etaIn && Muon_pt->at(iMuon) > ptIn)
        {
          hGoodSelMuInsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
          hGoodSelMuInsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
          hGoodSelMuInsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
          mGoodSelMuInsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
          mGoodSelMuInsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
          mGoodSelMuInsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
          hGoodSelMuInsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
          hGoodSelMuInsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
          hGoodSelMuInsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
          mGoodSelMuInsideCutsVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
          mGoodSelMuInsideCutsVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
          mGoodSelMuInsideCutsVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
        }
        if(fabs(Genp_eta->at(iGenMuon)) > etaOut || Genp_pt->at(iGenMuon) < ptOut)
        {
          hGoodSelMuOutsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
          hGoodSelMuOutsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
          hGoodSelMuOutsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
          mGoodSelMuOutsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
          mGoodSelMuOutsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
          mGoodSelMuOutsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
          hGoodSelMuOutsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
          hGoodSelMuOutsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
          hGoodSelMuOutsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
          mGoodSelMuOutsideCutsVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
          mGoodSelMuOutsideCutsVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
          mGoodSelMuOutsideCutsVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
        }
        
//         if(Muon_hasTriggered->at(iMuon))
        if(s->SelectEventPassingTrigger("HLT_Mu3"))
        {
          hGoodTrgMuVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
          hGoodTrgMuVsGenPt->Fill(Genp_pt->at(iGenMuon));
          hGoodTrgMuVsGenEta->Fill(Genp_eta->at(iGenMuon));
          mGoodTrgMuVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
          mGoodTrgMuVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
          mGoodTrgMuVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
          hGoodTrgMuVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
          hGoodTrgMuVsRecoPt->Fill(Muon_pt->at(iMuon));
          hGoodTrgMuVsRecoEta->Fill(Muon_eta->at(iMuon));
          mGoodTrgMuVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
          mGoodTrgMuVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
          mGoodTrgMuVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
          if(fabs(Muon_eta->at(iMuon)) < etaIn && Muon_pt->at(iMuon) > ptIn)
          {
            hGoodTrgMuInsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
            hGoodTrgMuInsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
            hGoodTrgMuInsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
            mGoodTrgMuInsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
            mGoodTrgMuInsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
            mGoodTrgMuInsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
            hGoodTrgMuInsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
            hGoodTrgMuInsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
            hGoodTrgMuInsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
            mGoodTrgMuInsideCutsVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
            mGoodTrgMuInsideCutsVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
            mGoodTrgMuInsideCutsVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
          }
          if(fabs(Genp_eta->at(iGenMuon)) > etaOut || Genp_pt->at(iGenMuon) < ptOut)
          {
            hGoodTrgMuOutsideCutsVsGenPtEta->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
            hGoodTrgMuOutsideCutsVsGenPt->Fill(Genp_pt->at(iGenMuon));
            hGoodTrgMuOutsideCutsVsGenEta->Fill(Genp_eta->at(iGenMuon));
            mGoodTrgMuOutsideCutsVsGenPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon),Genp_pt->at(iGenMuon));
            mGoodTrgMuOutsideCutsVsGenPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_pt->at(iGenMuon));
            mGoodTrgMuOutsideCutsVsGenEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Genp_eta->at(iGenMuon));
            hGoodTrgMuOutsideCutsVsRecoPtEta->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
            hGoodTrgMuOutsideCutsVsRecoPt->Fill(Muon_pt->at(iMuon));
            hGoodTrgMuOutsideCutsVsRecoEta->Fill(Muon_eta->at(iMuon));
            mGoodTrgMuOutsideCutsVsRecoPtEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon),Muon_pt->at(iMuon));
            mGoodTrgMuOutsideCutsVsRecoPtSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
            mGoodTrgMuOutsideCutsVsRecoEtaSC[vGenMuonsSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
          }
        }
      }
    }
  }

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here
  double xGraphValue, yGraphValue = 0;
  double yGraphErrorHi, yGraphErrorLo = 0;

  std::cout << "vgGoodTrackOverGenMuVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrackVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hAllGenMuVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrackOverGenMuVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrackOverGenMuVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrackOverGenMuVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrackOverGenMuVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrackOverGenMuVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrackOverGenMuVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrackOverGenMuVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrackOverGenMuVsGenPtEta->SetBinContent(hGoodTrackOverGenMuVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrackOverGenMuVsGenPtEtaHi->SetBinContent(hGoodTrackOverGenMuVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrackOverGenMuVsGenPtEtaLo->SetBinContent(hGoodTrackOverGenMuVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrackVsGenPt, hAllGenMuVsGenPt, gGoodTrackOverGenMuVsGenPt);
  ClopperPearsonDivide(hGoodTrackVsGenPt, hAllGenMuVsGenPt, gGoodTrackOverGenMuVsGenPt);
  hGoodTrackOverGenMuVsGenPt->Divide(hGoodTrackVsGenPt,hAllGenMuVsGenPt);
//   BayesDivide(hGoodTrackVsGenEta, hAllGenMuVsGenEta, gGoodTrackOverGenMuVsGenEta);
  ClopperPearsonDivide(hGoodTrackVsGenEta, hAllGenMuVsGenEta, gGoodTrackOverGenMuVsGenEta);
  hGoodTrackOverGenMuVsGenEta->Divide(hGoodTrackVsGenEta,hAllGenMuVsGenEta);

  std::cout << "vgGoodRecoOverGenMuVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodRecoMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hAllGenMuVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodRecoOverGenMuVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodRecoOverGenMuVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodRecoOverGenMuVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodRecoOverGenMuVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodRecoOverGenMuVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodRecoOverGenMuVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodRecoOverGenMuVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodRecoOverGenMuVsGenPtEta->SetBinContent(hGoodRecoOverGenMuVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodRecoOverGenMuVsGenPtEtaHi->SetBinContent(hGoodRecoOverGenMuVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodRecoOverGenMuVsGenPtEtaLo->SetBinContent(hGoodRecoOverGenMuVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodRecoMuVsGenPt, hAllGenMuVsGenPt, gGoodRecoOverGenMuVsGenPt);
  ClopperPearsonDivide(hGoodRecoMuVsGenPt, hAllGenMuVsGenPt, gGoodRecoOverGenMuVsGenPt);
  hGoodRecoOverGenMuVsGenPt->Divide(hGoodRecoMuVsGenPt,hAllGenMuVsGenPt);
//   BayesDivide(hGoodRecoMuVsGenEta, hAllGenMuVsGenEta, gGoodRecoOverGenMuVsGenEta);
  ClopperPearsonDivide(hGoodRecoMuVsGenEta, hAllGenMuVsGenEta, gGoodRecoOverGenMuVsGenEta);
  hGoodRecoOverGenMuVsGenEta->Divide(hGoodRecoMuVsGenEta,hAllGenMuVsGenEta);

  std::cout << "vgGoodRecoOverGoodTrackVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodRecoMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodTrackVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodRecoOverGoodTrackVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodRecoOverGoodTrackVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodRecoOverGoodTrackVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodRecoOverGoodTrackVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodRecoOverGoodTrackVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodRecoOverGoodTrackVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodRecoOverGoodTrackVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodRecoOverGoodTrackVsGenPtEta->SetBinContent(hGoodRecoOverGoodTrackVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodRecoOverGoodTrackVsGenPtEtaHi->SetBinContent(hGoodRecoOverGoodTrackVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodRecoOverGoodTrackVsGenPtEtaLo->SetBinContent(hGoodRecoOverGoodTrackVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodRecoMuVsGenPt, hGoodTrackVsGenPt, gGoodRecoOverGoodTrackVsGenPt);
  ClopperPearsonDivide(hGoodRecoMuVsGenPt, hGoodTrackVsGenPt, gGoodRecoOverGoodTrackVsGenPt);
  hGoodRecoOverGoodTrackVsGenPt->Divide(hGoodRecoMuVsGenPt,hGoodTrackVsGenPt);
//   BayesDivide(hGoodRecoMuVsGenEta, hGoodTrackVsGenEta, gGoodRecoOverGoodTrackVsGenEta);
  ClopperPearsonDivide(hGoodRecoMuVsGenEta, hGoodTrackVsGenEta, gGoodRecoOverGoodTrackVsGenEta);
  hGoodRecoOverGoodTrackVsGenEta->Divide(hGoodRecoMuVsGenEta,hGoodTrackVsGenEta);

  std::cout << "vgGoodRecoOverGoodTrackVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodRecoMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodTrackVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodRecoOverGoodTrackVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodRecoOverGoodTrackVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodRecoOverGoodTrackVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodRecoOverGoodTrackVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodRecoOverGoodTrackVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodRecoOverGoodTrackVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodRecoOverGoodTrackVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodRecoOverGoodTrackVsRecoPtEta->SetBinContent(hGoodRecoOverGoodTrackVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodRecoOverGoodTrackVsRecoPtEtaHi->SetBinContent(hGoodRecoOverGoodTrackVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodRecoOverGoodTrackVsRecoPtEtaLo->SetBinContent(hGoodRecoOverGoodTrackVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodRecoMuVsRecoPt, hGoodTrackVsRecoPt, gGoodRecoOverGoodTrackVsRecoPt);
  ClopperPearsonDivide(hGoodRecoMuVsRecoPt, hGoodTrackVsRecoPt, gGoodRecoOverGoodTrackVsRecoPt);
  hGoodRecoOverGoodTrackVsRecoPt->Divide(hGoodRecoMuVsRecoPt,hGoodTrackVsRecoPt);
//   BayesDivide(hGoodRecoMuVsRecoEta, hGoodTrackVsRecoEta, gGoodRecoOverGoodTrackVsRecoEta);
  ClopperPearsonDivide(hGoodRecoMuVsRecoEta, hGoodTrackVsRecoEta, gGoodRecoOverGoodTrackVsRecoEta);
  hGoodRecoOverGoodTrackVsRecoEta->Divide(hGoodRecoMuVsRecoEta,hGoodTrackVsRecoEta);

  std::cout << "vgGoodSelOverGenMuVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodSelMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hAllGenMuVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodSelOverGenMuVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodSelOverGenMuVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodSelOverGenMuVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodSelOverGenMuVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodSelOverGenMuVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodSelOverGenMuVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodSelOverGenMuVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodSelOverGenMuVsGenPtEta->SetBinContent(hGoodSelOverGenMuVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodSelOverGenMuVsGenPtEtaHi->SetBinContent(hGoodSelOverGenMuVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodSelOverGenMuVsGenPtEtaLo->SetBinContent(hGoodSelOverGenMuVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodSelMuVsGenPt, hAllGenMuVsGenPt, gGoodSelOverGenMuVsGenPt);
  ClopperPearsonDivide(hGoodSelMuVsGenPt, hAllGenMuVsGenPt, gGoodSelOverGenMuVsGenPt);
  hGoodSelOverGenMuVsGenPt->Divide(hGoodSelMuVsGenPt,hAllGenMuVsGenPt);
//   BayesDivide(hGoodSelMuVsGenEta, hAllGenMuVsGenEta, gGoodSelOverGenMuVsGenEta);
  ClopperPearsonDivide(hGoodSelMuVsGenEta, hAllGenMuVsGenEta, gGoodSelOverGenMuVsGenEta);
  hGoodSelOverGenMuVsGenEta->Divide(hGoodSelMuVsGenEta,hAllGenMuVsGenEta);
  
  std::cout << "vgGoodSelOverGoodTrackVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodSelMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodTrackVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodSelOverGoodTrackVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodSelOverGoodTrackVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodSelOverGoodTrackVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodSelOverGoodTrackVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodSelOverGoodTrackVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodSelOverGoodTrackVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodSelOverGoodTrackVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodSelOverGoodTrackVsGenPtEta->SetBinContent(hGoodSelOverGoodTrackVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodSelOverGoodTrackVsGenPtEtaHi->SetBinContent(hGoodSelOverGoodTrackVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodSelOverGoodTrackVsGenPtEtaLo->SetBinContent(hGoodSelOverGoodTrackVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodSelMuVsGenPt, hGoodTrackVsGenPt, gGoodSelOverGoodTrackVsGenPt);
  ClopperPearsonDivide(hGoodSelMuVsGenPt, hGoodTrackVsGenPt, gGoodSelOverGoodTrackVsGenPt);
  hGoodSelOverGoodTrackVsGenPt->Divide(hGoodSelMuVsGenPt,hGoodTrackVsGenPt);
//   BayesDivide(hGoodSelMuVsGenEta, hGoodTrackVsGenEta, gGoodSelOverGoodTrackVsGenEta);
  ClopperPearsonDivide(hGoodSelMuVsGenEta, hGoodTrackVsGenEta, gGoodSelOverGoodTrackVsGenEta);
  hGoodSelOverGoodTrackVsGenEta->Divide(hGoodSelMuVsGenEta,hGoodTrackVsGenEta);

  std::cout << "vgGoodSelOverGoodTrackVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodSelMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodTrackVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodSelOverGoodTrackVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodSelOverGoodTrackVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodSelOverGoodTrackVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodSelOverGoodTrackVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodSelOverGoodTrackVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodSelOverGoodTrackVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodSelOverGoodTrackVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodSelOverGoodTrackVsRecoPtEta->SetBinContent(hGoodSelOverGoodTrackVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodSelOverGoodTrackVsRecoPtEtaHi->SetBinContent(hGoodSelOverGoodTrackVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodSelOverGoodTrackVsRecoPtEtaLo->SetBinContent(hGoodSelOverGoodTrackVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodSelMuVsRecoPt, hGoodTrackVsRecoPt, gGoodSelOverGoodTrackVsRecoPt);
  ClopperPearsonDivide(hGoodSelMuVsRecoPt, hGoodTrackVsRecoPt, gGoodSelOverGoodTrackVsRecoPt);
  hGoodSelOverGoodTrackVsRecoPt->Divide(hGoodSelMuVsRecoPt,hGoodTrackVsRecoPt);
//   BayesDivide(hGoodSelMuVsRecoEta, hGoodTrackVsRecoEta, gGoodSelOverGoodTrackVsRecoEta);
  ClopperPearsonDivide(hGoodSelMuVsRecoEta, hGoodTrackVsRecoEta, gGoodSelOverGoodTrackVsRecoEta);
  hGoodSelOverGoodTrackVsRecoEta->Divide(hGoodSelMuVsRecoEta,hGoodTrackVsRecoEta);
  
  std::cout << "vgGoodSelOverGoodRecoMuVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodSelMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodRecoMuVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodSelOverGoodRecoMuVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodSelOverGoodRecoMuVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodSelOverGoodRecoMuVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodSelOverGoodRecoMuVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodSelOverGoodRecoMuVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodSelOverGoodRecoMuVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodSelOverGoodRecoMuVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodSelOverGoodRecoMuVsGenPtEta->SetBinContent(hGoodSelOverGoodRecoMuVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodSelOverGoodRecoMuVsGenPtEtaHi->SetBinContent(hGoodSelOverGoodRecoMuVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodSelOverGoodRecoMuVsGenPtEtaLo->SetBinContent(hGoodSelOverGoodRecoMuVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodSelMuVsGenPt, hGoodRecoMuVsGenPt, gGoodSelOverGoodRecoMuVsGenPt);
  ClopperPearsonDivide(hGoodSelMuVsGenPt, hGoodRecoMuVsGenPt, gGoodSelOverGoodRecoMuVsGenPt);
  hGoodSelOverGoodRecoMuVsGenPt->Divide(hGoodSelMuVsGenPt,hGoodRecoMuVsGenPt);
//   BayesDivide(hGoodSelMuVsGenEta, hGoodRecoMuVsGenEta, gGoodSelOverGoodRecoMuVsGenEta);
  ClopperPearsonDivide(hGoodSelMuVsGenEta, hGoodRecoMuVsGenEta, gGoodSelOverGoodRecoMuVsGenEta);
  hGoodSelOverGoodRecoMuVsGenEta->Divide(hGoodSelMuVsGenEta,hGoodRecoMuVsGenEta);

  std::cout << "vgGoodSelOverGoodRecoMuVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodSelMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodRecoMuVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodSelOverGoodRecoMuVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodSelOverGoodRecoMuVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodSelOverGoodRecoMuVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodSelOverGoodRecoMuVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodSelOverGoodRecoMuVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodSelOverGoodRecoMuVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodSelOverGoodRecoMuVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodSelOverGoodRecoMuVsRecoPtEta->SetBinContent(hGoodSelOverGoodRecoMuVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodSelOverGoodRecoMuVsRecoPtEtaHi->SetBinContent(hGoodSelOverGoodRecoMuVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodSelOverGoodRecoMuVsRecoPtEtaLo->SetBinContent(hGoodSelOverGoodRecoMuVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodSelMuVsRecoPt, hGoodRecoMuVsRecoPt, gGoodSelOverGoodRecoMuVsRecoPt);
  ClopperPearsonDivide(hGoodSelMuVsRecoPt, hGoodRecoMuVsRecoPt, gGoodSelOverGoodRecoMuVsRecoPt);
  hGoodSelOverGoodRecoMuVsRecoPt->Divide(hGoodSelMuVsRecoPt,hGoodRecoMuVsRecoPt);
//   BayesDivide(hGoodSelMuVsRecoEta, hGoodRecoMuVsRecoEta, gGoodSelOverGoodRecoMuVsRecoEta);
  ClopperPearsonDivide(hGoodSelMuVsRecoEta, hGoodRecoMuVsRecoEta, gGoodSelOverGoodRecoMuVsRecoEta);
  hGoodSelOverGoodRecoMuVsRecoEta->Divide(hGoodSelMuVsRecoEta,hGoodRecoMuVsRecoEta);
  
  std::cout << "vgGoodTrgOverGenMuVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrgMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hAllGenMuVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrgOverGenMuVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrgOverGenMuVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrgOverGenMuVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrgOverGenMuVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrgOverGenMuVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrgOverGenMuVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrgOverGenMuVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrgOverGenMuVsGenPtEta->SetBinContent(hGoodTrgOverGenMuVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrgOverGenMuVsGenPtEtaHi->SetBinContent(hGoodTrgOverGenMuVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrgOverGenMuVsGenPtEtaLo->SetBinContent(hGoodTrgOverGenMuVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrgMuVsGenPt, hAllGenMuVsGenPt, gGoodTrgOverGenMuVsGenPt);
  ClopperPearsonDivide(hGoodTrgMuVsGenPt, hAllGenMuVsGenPt, gGoodTrgOverGenMuVsGenPt);
  hGoodTrgOverGenMuVsGenPt->Divide(hGoodTrgMuVsGenPt,hAllGenMuVsGenPt);
//   BayesDivide(hGoodTrgMuVsGenEta, hAllGenMuVsGenEta, gGoodTrgOverGenMuVsGenEta);
  ClopperPearsonDivide(hGoodTrgMuVsGenEta, hAllGenMuVsGenEta, gGoodTrgOverGenMuVsGenEta);
  hGoodTrgOverGenMuVsGenEta->Divide(hGoodTrgMuVsGenEta,hAllGenMuVsGenEta);
  
  std::cout << "vgGoodTrgOverGoodTrackVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrgMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodTrackVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrgOverGoodTrackVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrgOverGoodTrackVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrgOverGoodTrackVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrgOverGoodTrackVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrgOverGoodTrackVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrgOverGoodTrackVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrgOverGoodTrackVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrgOverGoodTrackVsGenPtEta->SetBinContent(hGoodTrgOverGoodTrackVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrgOverGoodTrackVsGenPtEtaHi->SetBinContent(hGoodTrgOverGoodTrackVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrgOverGoodTrackVsGenPtEtaLo->SetBinContent(hGoodTrgOverGoodTrackVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrgMuVsGenPt, hGoodTrackVsGenPt, gGoodTrgOverGoodTrackVsGenPt);
  ClopperPearsonDivide(hGoodTrgMuVsGenPt, hGoodTrackVsGenPt, gGoodTrgOverGoodTrackVsGenPt);
  hGoodTrgOverGoodTrackVsGenPt->Divide(hGoodTrgMuVsGenPt,hGoodTrackVsGenPt);
//   BayesDivide(hGoodTrgMuVsGenEta, hGoodTrackVsGenEta, gGoodTrgOverGoodTrackVsGenEta);
  ClopperPearsonDivide(hGoodTrgMuVsGenEta, hGoodTrackVsGenEta, gGoodTrgOverGoodTrackVsGenEta);
  hGoodTrgOverGoodTrackVsGenEta->Divide(hGoodTrgMuVsGenEta,hGoodTrackVsGenEta);

  std::cout << "vgGoodTrgOverGoodTrackVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrgMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodTrackVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrgOverGoodTrackVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrgOverGoodTrackVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrgOverGoodTrackVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrgOverGoodTrackVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrgOverGoodTrackVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrgOverGoodTrackVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrgOverGoodTrackVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrgOverGoodTrackVsRecoPtEta->SetBinContent(hGoodTrgOverGoodTrackVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrgOverGoodTrackVsRecoPtEtaHi->SetBinContent(hGoodTrgOverGoodTrackVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrgOverGoodTrackVsRecoPtEtaLo->SetBinContent(hGoodTrgOverGoodTrackVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrgMuVsRecoPt, hGoodTrackVsRecoPt, gGoodTrgOverGoodTrackVsRecoPt);
  ClopperPearsonDivide(hGoodTrgMuVsRecoPt, hGoodTrackVsRecoPt, gGoodTrgOverGoodTrackVsRecoPt);
  hGoodTrgOverGoodTrackVsRecoPt->Divide(hGoodTrgMuVsRecoPt,hGoodTrackVsRecoPt);
//   BayesDivide(hGoodTrgMuVsRecoEta, hGoodTrackVsRecoEta, gGoodTrgOverGoodTrackVsRecoEta);
  ClopperPearsonDivide(hGoodTrgMuVsRecoEta, hGoodTrackVsRecoEta, gGoodTrgOverGoodTrackVsRecoEta);
  hGoodTrgOverGoodTrackVsRecoEta->Divide(hGoodTrgMuVsRecoEta,hGoodTrackVsRecoEta);
  
  std::cout << "vgGoodTrgOverGoodRecoMuVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrgMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodRecoMuVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrgOverGoodRecoMuVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrgOverGoodRecoMuVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrgOverGoodRecoMuVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrgOverGoodRecoMuVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrgOverGoodRecoMuVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrgOverGoodRecoMuVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrgOverGoodRecoMuVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrgOverGoodRecoMuVsGenPtEta->SetBinContent(hGoodTrgOverGoodRecoMuVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrgOverGoodRecoMuVsGenPtEtaHi->SetBinContent(hGoodTrgOverGoodRecoMuVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrgOverGoodRecoMuVsGenPtEtaLo->SetBinContent(hGoodTrgOverGoodRecoMuVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrgMuVsGenPt, hGoodRecoMuVsGenPt, gGoodTrgOverGoodRecoMuVsGenPt);
  ClopperPearsonDivide(hGoodTrgMuVsGenPt, hGoodRecoMuVsGenPt, gGoodTrgOverGoodRecoMuVsGenPt);
  hGoodTrgOverGoodRecoMuVsGenPt->Divide(hGoodTrgMuVsGenPt,hGoodRecoMuVsGenPt);
//   BayesDivide(hGoodTrgMuVsGenEta, hGoodRecoMuVsGenEta, gGoodTrgOverGoodRecoMuVsGenEta);
  ClopperPearsonDivide(hGoodTrgMuVsGenEta, hGoodRecoMuVsGenEta, gGoodTrgOverGoodRecoMuVsGenEta);
  hGoodTrgOverGoodRecoMuVsGenEta->Divide(hGoodTrgMuVsGenEta,hGoodRecoMuVsGenEta);

  std::cout << "vgGoodTrgOverGoodRecoMuVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrgMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodRecoMuVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrgOverGoodRecoMuVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrgOverGoodRecoMuVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrgOverGoodRecoMuVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrgOverGoodRecoMuVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrgOverGoodRecoMuVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrgOverGoodRecoMuVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrgOverGoodRecoMuVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrgOverGoodRecoMuVsRecoPtEta->SetBinContent(hGoodTrgOverGoodRecoMuVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrgOverGoodRecoMuVsRecoPtEtaHi->SetBinContent(hGoodTrgOverGoodRecoMuVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrgOverGoodRecoMuVsRecoPtEtaLo->SetBinContent(hGoodTrgOverGoodRecoMuVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrgMuVsRecoPt, hGoodRecoMuVsRecoPt, gGoodTrgOverGoodRecoMuVsRecoPt);
  ClopperPearsonDivide(hGoodTrgMuVsRecoPt, hGoodRecoMuVsRecoPt, gGoodTrgOverGoodRecoMuVsRecoPt);
  hGoodTrgOverGoodRecoMuVsRecoPt->Divide(hGoodTrgMuVsRecoPt,hGoodRecoMuVsRecoPt);
//   BayesDivide(hGoodTrgMuVsRecoEta, hGoodRecoMuVsRecoEta, gGoodTrgOverGoodRecoMuVsRecoEta);
  ClopperPearsonDivide(hGoodTrgMuVsRecoEta, hGoodRecoMuVsRecoEta, gGoodTrgOverGoodRecoMuVsRecoEta);
  hGoodTrgOverGoodRecoMuVsRecoEta->Divide(hGoodTrgMuVsRecoEta,hGoodRecoMuVsRecoEta);

  std::cout << "vgGoodTrgOverGoodSelMuVsGenPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrgMuVsGenPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodSelMuVsGenPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrgOverGoodSelMuVsGenPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrgOverGoodSelMuVsGenPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrgOverGoodSelMuVsGenPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrgOverGoodSelMuVsGenPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrgOverGoodSelMuVsGenPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrgOverGoodSelMuVsGenPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrgOverGoodSelMuVsGenPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrgOverGoodSelMuVsGenPtEta->SetBinContent(hGoodTrgOverGoodSelMuVsGenPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrgOverGoodSelMuVsGenPtEtaHi->SetBinContent(hGoodTrgOverGoodSelMuVsGenPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrgOverGoodSelMuVsGenPtEtaLo->SetBinContent(hGoodTrgOverGoodSelMuVsGenPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrgMuVsGenPt, hGoodSelMuVsGenPt, gGoodTrgOverGoodSelMuVsGenPt);
  ClopperPearsonDivide(hGoodTrgMuVsGenPt, hGoodSelMuVsGenPt, gGoodTrgOverGoodSelMuVsGenPt);
  hGoodTrgOverGoodSelMuVsGenPt->Divide(hGoodTrgMuVsGenPt,hGoodSelMuVsGenPt);
//   BayesDivide(hGoodTrgMuVsGenEta, hGoodSelMuVsGenEta, gGoodTrgOverGoodSelMuVsGenEta);
  ClopperPearsonDivide(hGoodTrgMuVsGenEta, hGoodSelMuVsGenEta, gGoodTrgOverGoodSelMuVsGenEta);
  hGoodTrgOverGoodSelMuVsGenEta->Divide(hGoodTrgMuVsGenEta,hGoodSelMuVsGenEta);

  std::cout << "vgGoodTrgOverGoodSelMuVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrgMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hGoodSelMuVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrgOverGoodSelMuVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrgOverGoodSelMuVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrgOverGoodSelMuVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrgOverGoodSelMuVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrgOverGoodSelMuVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrgOverGoodSelMuVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrgOverGoodSelMuVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrgOverGoodSelMuVsRecoPtEta->SetBinContent(hGoodTrgOverGoodSelMuVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrgOverGoodSelMuVsRecoPtEtaHi->SetBinContent(hGoodTrgOverGoodSelMuVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrgOverGoodSelMuVsRecoPtEtaLo->SetBinContent(hGoodTrgOverGoodSelMuVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrgMuVsRecoPt, hGoodSelMuVsRecoPt, gGoodTrgOverGoodSelMuVsRecoPt);
  ClopperPearsonDivide(hGoodTrgMuVsRecoPt, hGoodSelMuVsRecoPt, gGoodTrgOverGoodSelMuVsRecoPt);
  hGoodTrgOverGoodSelMuVsRecoPt->Divide(hGoodTrgMuVsRecoPt,hGoodSelMuVsRecoPt);
//   BayesDivide(hGoodTrgMuVsRecoEta, hGoodSelMuVsRecoEta, gGoodTrgOverGoodSelMuVsRecoEta);
  ClopperPearsonDivide(hGoodTrgMuVsRecoEta, hGoodSelMuVsRecoEta, gGoodTrgOverGoodSelMuVsRecoEta);
  hGoodTrgOverGoodSelMuVsRecoEta->Divide(hGoodTrgMuVsRecoEta,hGoodSelMuVsRecoEta);
  
  std::cout << "vgGoodRecoOverAllRecoMuVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodRecoMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hAllRecoMuVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodRecoOverAllRecoMuVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodRecoOverAllRecoMuVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodRecoOverAllRecoMuVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodRecoOverAllRecoMuVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodRecoOverAllRecoMuVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodRecoOverAllRecoMuVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodRecoOverAllRecoMuVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodRecoOverAllRecoMuVsRecoPtEta->SetBinContent(hGoodRecoOverAllRecoMuVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodRecoOverAllRecoMuVsRecoPtEtaHi->SetBinContent(hGoodRecoOverAllRecoMuVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodRecoOverAllRecoMuVsRecoPtEtaLo->SetBinContent(hGoodRecoOverAllRecoMuVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodRecoMuVsRecoPt, hAllRecoMuVsRecoPt, gGoodRecoOverAllRecoMuVsRecoPt);
  ClopperPearsonDivide(hGoodRecoMuVsRecoPt, hAllRecoMuVsRecoPt, gGoodRecoOverAllRecoMuVsRecoPt);
  hGoodRecoOverAllRecoMuVsRecoPt->Divide(hGoodRecoMuVsRecoPt,hAllRecoMuVsRecoPt);
//   BayesDivide(hGoodRecoMuVsRecoEta, hAllRecoMuVsRecoEta, gGoodRecoOverAllRecoMuVsRecoEta);
  ClopperPearsonDivide(hGoodRecoMuVsRecoEta, hAllRecoMuVsRecoEta, gGoodRecoOverAllRecoMuVsRecoEta);
  hGoodRecoOverAllRecoMuVsRecoEta->Divide(hGoodRecoMuVsRecoEta,hAllRecoMuVsRecoEta);

  std::cout << "vgGoodSelOverAllSelMuVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodSelMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hAllSelMuVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodSelOverAllSelMuVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodSelOverAllSelMuVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodSelOverAllSelMuVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodSelOverAllSelMuVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodSelOverAllSelMuVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodSelOverAllSelMuVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodSelOverAllSelMuVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodSelOverAllSelMuVsRecoPtEta->SetBinContent(hGoodSelOverAllSelMuVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodSelOverAllSelMuVsRecoPtEtaHi->SetBinContent(hGoodSelOverAllSelMuVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodSelOverAllSelMuVsRecoPtEtaLo->SetBinContent(hGoodSelOverAllSelMuVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodSelMuVsRecoPt, hAllSelMuVsRecoPt, gGoodSelOverAllSelMuVsRecoPt);
  ClopperPearsonDivide(hGoodSelMuVsRecoPt, hAllSelMuVsRecoPt, gGoodSelOverAllSelMuVsRecoPt);
  hGoodSelOverAllSelMuVsRecoPt->Divide(hGoodSelMuVsRecoPt,hAllSelMuVsRecoPt);
//   BayesDivide(hGoodSelMuVsRecoEta, hAllSelMuVsRecoEta, gGoodSelOverAllSelMuVsRecoEta);
  ClopperPearsonDivide(hGoodSelMuVsRecoEta, hAllSelMuVsRecoEta, gGoodSelOverAllSelMuVsRecoEta);
  hGoodSelOverAllSelMuVsRecoEta->Divide(hGoodSelMuVsRecoEta,hAllSelMuVsRecoEta);

  std::cout << "vgGoodTrgOverAllTrgMuVsRecoPtEta\n";
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    TH1D* hPass = hGoodTrgMuVsRecoPtEta->ProjectionY("hPass", iGraph+1, iGraph+1);
    TH1D* hTotal = hAllTrgMuVsRecoPtEta->ProjectionY("hTotal", iGraph+1, iGraph+1);
//     BayesDivide(hPass, hTotal, vgGoodTrgOverAllTrgMuVsRecoPtEta[iGraph]);
    ClopperPearsonDivide(hPass, hTotal, vgGoodTrgOverAllTrgMuVsRecoPtEta[iGraph]);
  }
  for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
  {
    unsigned int nPoints = vgGoodTrgOverAllTrgMuVsRecoPtEta[iGraph]->GetN();
    for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
    {
      vgGoodTrgOverAllTrgMuVsRecoPtEta[iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
      unsigned int yBin = hGoodTrgOverAllTrgMuVsRecoPtEta->GetYaxis()->FindBin(xGraphValue);
      yGraphErrorHi = vgGoodTrgOverAllTrgMuVsRecoPtEta[iGraph]->GetErrorYhigh(iPoint);
      yGraphErrorLo = vgGoodTrgOverAllTrgMuVsRecoPtEta[iGraph]->GetErrorYlow(iPoint);
      if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
        yGraphErrorHi = 0;
      if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
        yGraphErrorLo = 0;
      if(yGraphValue+yGraphErrorHi > 1)
        yGraphErrorHi = 0;
      if(yGraphValue-yGraphErrorLo < 0)
        yGraphErrorLo = 0;
      hGoodTrgOverAllTrgMuVsRecoPtEta->SetBinContent(hGoodTrgOverAllTrgMuVsRecoPtEta->GetBin(iGraph+1,yBin),yGraphValue);
      hGoodTrgOverAllTrgMuVsRecoPtEtaHi->SetBinContent(hGoodTrgOverAllTrgMuVsRecoPtEtaHi->GetBin(iGraph+1,yBin),yGraphErrorHi);
      hGoodTrgOverAllTrgMuVsRecoPtEtaLo->SetBinContent(hGoodTrgOverAllTrgMuVsRecoPtEtaLo->GetBin(iGraph+1,yBin),yGraphErrorLo);
    }
  }
//   BayesDivide(hGoodTrgMuVsRecoPt, hAllTrgMuVsRecoPt, gGoodTrgOverAllTrgMuVsRecoPt);
  ClopperPearsonDivide(hGoodTrgMuVsRecoPt, hAllTrgMuVsRecoPt, gGoodTrgOverAllTrgMuVsRecoPt);
  hGoodTrgOverAllTrgMuVsRecoPt->Divide(hGoodTrgMuVsRecoPt,hAllTrgMuVsRecoPt);
//   BayesDivide(hGoodTrgMuVsRecoEta, hAllTrgMuVsRecoEta, gGoodTrgOverAllTrgMuVsRecoEta);
  ClopperPearsonDivide(hGoodTrgMuVsRecoEta, hAllTrgMuVsRecoEta, gGoodTrgOverAllTrgMuVsRecoEta);
  hGoodTrgOverAllTrgMuVsRecoEta->Divide(hGoodTrgMuVsRecoEta,hAllTrgMuVsRecoEta);

  for(unsigned int i = 0; i < 7; i++)
  {

    std::cout << "mvgGoodTrackOverGenMuVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodTrackVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mAllGenMuVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodTrackOverGenMuVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodTrackOverGenMuVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodTrackOverGenMuVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodTrackOverGenMuVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodTrackOverGenMuVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodTrackOverGenMuVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodTrackOverGenMuVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodTrackOverGenMuVsGenPtEtaSC[i]->SetBinContent(mGoodTrackOverGenMuVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodTrackOverGenMuVsGenPtEtaHiSC[i]->SetBinContent(mGoodTrackOverGenMuVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodTrackOverGenMuVsGenPtEtaLoSC[i]->SetBinContent(mGoodTrackOverGenMuVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodTrackVsGenPtSC[i], mAllGenMuVsGenPtSC[i], mgGoodTrackOverGenMuVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodTrackVsGenPtSC[i], mAllGenMuVsGenPtSC[i], mgGoodTrackOverGenMuVsGenPtSC[i]);
    mGoodTrackOverGenMuVsGenPtSC[i]->Divide(mGoodTrackVsGenPtSC[i],mAllGenMuVsGenPtSC[i]);
//     BayesDivide(mGoodTrackVsGenEtaSC[i], mAllGenMuVsGenEtaSC[i], mgGoodTrackOverGenMuVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodTrackVsGenEtaSC[i], mAllGenMuVsGenEtaSC[i], mgGoodTrackOverGenMuVsGenEtaSC[i]);
    mGoodTrackOverGenMuVsGenEtaSC[i]->Divide(mGoodTrackVsGenEtaSC[i],mAllGenMuVsGenEtaSC[i]);

    std::cout << "mvgGoodRecoOverGenMuVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodRecoMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mAllGenMuVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodRecoOverGenMuVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodRecoOverGenMuVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodRecoOverGenMuVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodRecoOverGenMuVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodRecoOverGenMuVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodRecoOverGenMuVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodRecoOverGenMuVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodRecoOverGenMuVsGenPtEtaSC[i]->SetBinContent(mGoodRecoOverGenMuVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodRecoOverGenMuVsGenPtEtaHiSC[i]->SetBinContent(mGoodRecoOverGenMuVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodRecoOverGenMuVsGenPtEtaLoSC[i]->SetBinContent(mGoodRecoOverGenMuVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodRecoMuVsGenPtSC[i], mAllGenMuVsGenPtSC[i], mgGoodRecoOverGenMuVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodRecoMuVsGenPtSC[i], mAllGenMuVsGenPtSC[i], mgGoodRecoOverGenMuVsGenPtSC[i]);
    mGoodRecoOverGenMuVsGenPtSC[i]->Divide(mGoodRecoMuVsGenPtSC[i],mAllGenMuVsGenPtSC[i]);
//     BayesDivide(mGoodRecoMuVsGenEtaSC[i], mAllGenMuVsGenEtaSC[i], mgGoodRecoOverGenMuVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodRecoMuVsGenEtaSC[i], mAllGenMuVsGenEtaSC[i], mgGoodRecoOverGenMuVsGenEtaSC[i]);
    mGoodRecoOverGenMuVsGenEtaSC[i]->Divide(mGoodRecoMuVsGenEtaSC[i],mAllGenMuVsGenEtaSC[i]);

    std::cout << "mvgGoodRecoOverGoodTrackVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodRecoMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodTrackVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodRecoOverGoodTrackVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodRecoOverGoodTrackVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodRecoOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodRecoOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodRecoOverGoodTrackVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodRecoOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodRecoOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodRecoOverGoodTrackVsGenPtEtaSC[i]->SetBinContent(mGoodRecoOverGoodTrackVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodRecoOverGoodTrackVsGenPtEtaHiSC[i]->SetBinContent(mGoodRecoOverGoodTrackVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodRecoOverGoodTrackVsGenPtEtaLoSC[i]->SetBinContent(mGoodRecoOverGoodTrackVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodRecoMuVsGenPtSC[i], mGoodTrackVsGenPtSC[i], mgGoodRecoOverGoodTrackVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodRecoMuVsGenPtSC[i], mGoodTrackVsGenPtSC[i], mgGoodRecoOverGoodTrackVsGenPtSC[i]);
    mGoodRecoOverGoodTrackVsGenPtSC[i]->Divide(mGoodRecoMuVsGenPtSC[i],mGoodTrackVsGenPtSC[i]);
//     BayesDivide(mGoodRecoMuVsGenEtaSC[i], mGoodTrackVsGenEtaSC[i], mgGoodRecoOverGoodTrackVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodRecoMuVsGenEtaSC[i], mGoodTrackVsGenEtaSC[i], mgGoodRecoOverGoodTrackVsGenEtaSC[i]);
    mGoodRecoOverGoodTrackVsGenEtaSC[i]->Divide(mGoodRecoMuVsGenEtaSC[i],mGoodTrackVsGenEtaSC[i]);

    std::cout << "mvgGoodRecoOverGoodTrackVsRecoPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodRecoMuVsRecoPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodTrackVsRecoPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodRecoOverGoodTrackVsRecoPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodRecoOverGoodTrackVsRecoPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodRecoOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodRecoOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodRecoOverGoodTrackVsRecoPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodRecoOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodRecoOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodRecoOverGoodTrackVsRecoPtEtaSC[i]->SetBinContent(mGoodRecoOverGoodTrackVsRecoPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodRecoOverGoodTrackVsRecoPtEtaHiSC[i]->SetBinContent(mGoodRecoOverGoodTrackVsRecoPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodRecoOverGoodTrackVsRecoPtEtaLoSC[i]->SetBinContent(mGoodRecoOverGoodTrackVsRecoPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodRecoMuVsRecoPtSC[i], mGoodTrackVsRecoPtSC[i], mgGoodRecoOverGoodTrackVsRecoPtSC[i]);
    ClopperPearsonDivide(mGoodRecoMuVsRecoPtSC[i], mGoodTrackVsRecoPtSC[i], mgGoodRecoOverGoodTrackVsRecoPtSC[i]);
    mGoodRecoOverGoodTrackVsRecoPtSC[i]->Divide(mGoodRecoMuVsRecoPtSC[i],mGoodTrackVsRecoPtSC[i]);
//     BayesDivide(mGoodRecoMuVsRecoEtaSC[i], mGoodTrackVsRecoEtaSC[i], mgGoodRecoOverGoodTrackVsRecoEtaSC[i]);
    ClopperPearsonDivide(mGoodRecoMuVsRecoEtaSC[i], mGoodTrackVsRecoEtaSC[i], mgGoodRecoOverGoodTrackVsRecoEtaSC[i]);
    mGoodRecoOverGoodTrackVsRecoEtaSC[i]->Divide(mGoodRecoMuVsRecoEtaSC[i],mGoodTrackVsRecoEtaSC[i]);

    std::cout << "mvgGoodSelOverGenMuVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodSelMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mAllGenMuVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodSelOverGenMuVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodSelOverGenMuVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodSelOverGenMuVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodSelOverGenMuVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodSelOverGenMuVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodSelOverGenMuVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodSelOverGenMuVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodSelOverGenMuVsGenPtEtaSC[i]->SetBinContent(mGoodSelOverGenMuVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodSelOverGenMuVsGenPtEtaHiSC[i]->SetBinContent(mGoodSelOverGenMuVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodSelOverGenMuVsGenPtEtaLoSC[i]->SetBinContent(mGoodSelOverGenMuVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodSelMuVsGenPtSC[i], mAllGenMuVsGenPtSC[i], mgGoodSelOverGenMuVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsGenPtSC[i], mAllGenMuVsGenPtSC[i], mgGoodSelOverGenMuVsGenPtSC[i]);
    mGoodSelOverGenMuVsGenPtSC[i]->Divide(mGoodSelMuVsGenPtSC[i],mAllGenMuVsGenPtSC[i]);
//     BayesDivide(mGoodSelMuVsGenEtaSC[i], mAllGenMuVsGenEtaSC[i], mgGoodSelOverGenMuVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsGenEtaSC[i], mAllGenMuVsGenEtaSC[i], mgGoodSelOverGenMuVsGenEtaSC[i]);
    mGoodSelOverGenMuVsGenEtaSC[i]->Divide(mGoodSelMuVsGenEtaSC[i],mAllGenMuVsGenEtaSC[i]);

    std::cout << "mvgGoodSelOverGoodTrackVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodSelMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodTrackVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodSelOverGoodTrackVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodSelOverGoodTrackVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodSelOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodSelOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodSelOverGoodTrackVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodSelOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodSelOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodSelOverGoodTrackVsGenPtEtaSC[i]->SetBinContent(mGoodSelOverGoodTrackVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodSelOverGoodTrackVsGenPtEtaHiSC[i]->SetBinContent(mGoodSelOverGoodTrackVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodSelOverGoodTrackVsGenPtEtaLoSC[i]->SetBinContent(mGoodSelOverGoodTrackVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodSelMuVsGenPtSC[i], mGoodTrackVsGenPtSC[i], mgGoodSelOverGoodTrackVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsGenPtSC[i], mGoodTrackVsGenPtSC[i], mgGoodSelOverGoodTrackVsGenPtSC[i]);
    mGoodSelOverGoodTrackVsGenPtSC[i]->Divide(mGoodSelMuVsGenPtSC[i],mGoodTrackVsGenPtSC[i]);
//     BayesDivide(mGoodSelMuVsGenEtaSC[i], mGoodTrackVsGenEtaSC[i], mgGoodSelOverGoodTrackVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsGenEtaSC[i], mGoodTrackVsGenEtaSC[i], mgGoodSelOverGoodTrackVsGenEtaSC[i]);
    mGoodSelOverGoodTrackVsGenEtaSC[i]->Divide(mGoodSelMuVsGenEtaSC[i],mGoodTrackVsGenEtaSC[i]);

    std::cout << "mvgGoodSelOverGoodTrackVsRecoPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodSelMuVsRecoPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodTrackVsRecoPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodSelOverGoodTrackVsRecoPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodSelOverGoodTrackVsRecoPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodSelOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodSelOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodSelOverGoodTrackVsRecoPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodSelOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodSelOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodSelOverGoodTrackVsRecoPtEtaSC[i]->SetBinContent(mGoodSelOverGoodTrackVsRecoPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodSelOverGoodTrackVsRecoPtEtaHiSC[i]->SetBinContent(mGoodSelOverGoodTrackVsRecoPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodSelOverGoodTrackVsRecoPtEtaLoSC[i]->SetBinContent(mGoodSelOverGoodTrackVsRecoPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodSelMuVsRecoPtSC[i], mGoodTrackVsRecoPtSC[i], mgGoodSelOverGoodTrackVsRecoPtSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsRecoPtSC[i], mGoodTrackVsRecoPtSC[i], mgGoodSelOverGoodTrackVsRecoPtSC[i]);
    mGoodSelOverGoodTrackVsRecoPtSC[i]->Divide(mGoodSelMuVsRecoPtSC[i],mGoodTrackVsRecoPtSC[i]);
//     BayesDivide(mGoodSelMuVsRecoEtaSC[i], mGoodTrackVsRecoEtaSC[i], mgGoodSelOverGoodTrackVsRecoEtaSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsRecoEtaSC[i], mGoodTrackVsRecoEtaSC[i], mgGoodSelOverGoodTrackVsRecoEtaSC[i]);
    mGoodSelOverGoodTrackVsRecoEtaSC[i]->Divide(mGoodSelMuVsRecoEtaSC[i],mGoodTrackVsRecoEtaSC[i]);

    std::cout << "mvgGoodSelOverGoodRecoMuVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodSelMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodRecoMuVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodSelOverGoodRecoMuVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodSelOverGoodRecoMuVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodSelOverGoodRecoMuVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodSelOverGoodRecoMuVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodSelOverGoodRecoMuVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodSelOverGoodRecoMuVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodSelOverGoodRecoMuVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodSelOverGoodRecoMuVsGenPtEtaSC[i]->SetBinContent(mGoodSelOverGoodRecoMuVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodSelOverGoodRecoMuVsGenPtEtaHiSC[i]->SetBinContent(mGoodSelOverGoodRecoMuVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodSelOverGoodRecoMuVsGenPtEtaLoSC[i]->SetBinContent(mGoodSelOverGoodRecoMuVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodSelMuVsGenPtSC[i], mGoodRecoMuVsGenPtSC[i], mgGoodSelOverGoodRecoMuVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsGenPtSC[i], mGoodRecoMuVsGenPtSC[i], mgGoodSelOverGoodRecoMuVsGenPtSC[i]);
    mGoodSelOverGoodRecoMuVsGenPtSC[i]->Divide(mGoodSelMuVsGenPtSC[i],mGoodRecoMuVsGenPtSC[i]);
//     BayesDivide(mGoodSelMuVsGenEtaSC[i], mGoodRecoMuVsGenEtaSC[i], mgGoodSelOverGoodRecoMuVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsGenEtaSC[i], mGoodRecoMuVsGenEtaSC[i], mgGoodSelOverGoodRecoMuVsGenEtaSC[i]);
    mGoodSelOverGoodRecoMuVsGenEtaSC[i]->Divide(mGoodSelMuVsGenEtaSC[i],mGoodRecoMuVsGenEtaSC[i]);
    
    std::cout << "mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodSelMuVsRecoPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodRecoMuVsRecoPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodSelOverGoodRecoMuVsRecoPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodSelOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodSelOverGoodRecoMuVsRecoPtEtaSC[i]->SetBinContent(mGoodSelOverGoodRecoMuVsRecoPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodSelOverGoodRecoMuVsRecoPtEtaHiSC[i]->SetBinContent(mGoodSelOverGoodRecoMuVsRecoPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodSelOverGoodRecoMuVsRecoPtEtaLoSC[i]->SetBinContent(mGoodSelOverGoodRecoMuVsRecoPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodSelMuVsRecoPtSC[i], mGoodRecoMuVsRecoPtSC[i], mgGoodSelOverGoodRecoMuVsRecoPtSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsRecoPtSC[i], mGoodRecoMuVsRecoPtSC[i], mgGoodSelOverGoodRecoMuVsRecoPtSC[i]);
    mGoodSelOverGoodRecoMuVsRecoPtSC[i]->Divide(mGoodSelMuVsRecoPtSC[i],mGoodRecoMuVsRecoPtSC[i]);
//     BayesDivide(mGoodSelMuVsRecoEtaSC[i], mGoodRecoMuVsRecoEtaSC[i], mgGoodSelOverGoodRecoMuVsRecoEtaSC[i]);
    ClopperPearsonDivide(mGoodSelMuVsRecoEtaSC[i], mGoodRecoMuVsRecoEtaSC[i], mgGoodSelOverGoodRecoMuVsRecoEtaSC[i]);
    mGoodSelOverGoodRecoMuVsRecoEtaSC[i]->Divide(mGoodSelMuVsRecoEtaSC[i],mGoodRecoMuVsRecoEtaSC[i]);

    std::cout << "mvgGoodTrgOverGenMuVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodTrgMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mAllGenMuVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodTrgOverGenMuVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodTrgOverGenMuVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodTrgOverGenMuVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodTrgOverGenMuVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodTrgOverGenMuVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodTrgOverGenMuVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodTrgOverGenMuVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodTrgOverGenMuVsGenPtEtaSC[i]->SetBinContent(mGoodTrgOverGenMuVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodTrgOverGenMuVsGenPtEtaHiSC[i]->SetBinContent(mGoodTrgOverGenMuVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodTrgOverGenMuVsGenPtEtaLoSC[i]->SetBinContent(mGoodTrgOverGenMuVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodTrgMuVsGenPtSC[i], mAllGenMuVsGenPtSC[i], mgGoodTrgOverGenMuVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsGenPtSC[i], mAllGenMuVsGenPtSC[i], mgGoodTrgOverGenMuVsGenPtSC[i]);
    mGoodTrgOverGenMuVsGenPtSC[i]->Divide(mGoodTrgMuVsGenPtSC[i],mAllGenMuVsGenPtSC[i]);
//     BayesDivide(mGoodTrgMuVsGenEtaSC[i], mAllGenMuVsGenEtaSC[i], mgGoodTrgOverGenMuVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsGenEtaSC[i], mAllGenMuVsGenEtaSC[i], mgGoodTrgOverGenMuVsGenEtaSC[i]);
    mGoodTrgOverGenMuVsGenEtaSC[i]->Divide(mGoodTrgMuVsGenEtaSC[i],mAllGenMuVsGenEtaSC[i]);

    std::cout << "mvgGoodTrgOverGoodTrackVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodTrgMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodTrackVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodTrgOverGoodTrackVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodTrgOverGoodTrackVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodTrgOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodTrgOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodTrgOverGoodTrackVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodTrgOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodTrgOverGoodTrackVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodTrgOverGoodTrackVsGenPtEtaSC[i]->SetBinContent(mGoodTrgOverGoodTrackVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodTrgOverGoodTrackVsGenPtEtaHiSC[i]->SetBinContent(mGoodTrgOverGoodTrackVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodTrgOverGoodTrackVsGenPtEtaLoSC[i]->SetBinContent(mGoodTrgOverGoodTrackVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodTrgMuVsGenPtSC[i], mGoodTrackVsGenPtSC[i], mgGoodTrgOverGoodTrackVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsGenPtSC[i], mGoodTrackVsGenPtSC[i], mgGoodTrgOverGoodTrackVsGenPtSC[i]);
    mGoodTrgOverGoodTrackVsGenPtSC[i]->Divide(mGoodTrgMuVsGenPtSC[i],mGoodTrackVsGenPtSC[i]);
//     BayesDivide(mGoodTrgMuVsGenEtaSC[i], mGoodTrackVsGenEtaSC[i], mgGoodTrgOverGoodTrackVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsGenEtaSC[i], mGoodTrackVsGenEtaSC[i], mgGoodTrgOverGoodTrackVsGenEtaSC[i]);
    mGoodTrgOverGoodTrackVsGenEtaSC[i]->Divide(mGoodTrgMuVsGenEtaSC[i],mGoodTrackVsGenEtaSC[i]);

    std::cout << "mvgGoodTrgOverGoodTrackVsRecoPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodTrgMuVsRecoPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodTrackVsRecoPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodTrgOverGoodTrackVsRecoPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodTrgOverGoodTrackVsRecoPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodTrgOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodTrgOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodTrgOverGoodTrackVsRecoPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodTrgOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodTrgOverGoodTrackVsRecoPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodTrgOverGoodTrackVsRecoPtEtaSC[i]->SetBinContent(mGoodTrgOverGoodTrackVsRecoPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodTrgOverGoodTrackVsRecoPtEtaHiSC[i]->SetBinContent(mGoodTrgOverGoodTrackVsRecoPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodTrgOverGoodTrackVsRecoPtEtaLoSC[i]->SetBinContent(mGoodTrgOverGoodTrackVsRecoPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodTrgMuVsRecoPtSC[i], mGoodTrackVsRecoPtSC[i], mgGoodTrgOverGoodTrackVsRecoPtSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsRecoPtSC[i], mGoodTrackVsRecoPtSC[i], mgGoodTrgOverGoodTrackVsRecoPtSC[i]);
    mGoodTrgOverGoodTrackVsRecoPtSC[i]->Divide(mGoodTrgMuVsRecoPtSC[i],mGoodTrackVsRecoPtSC[i]);
//     BayesDivide(mGoodTrgMuVsRecoEtaSC[i], mGoodTrackVsRecoEtaSC[i], mgGoodTrgOverGoodTrackVsRecoEtaSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsRecoEtaSC[i], mGoodTrackVsRecoEtaSC[i], mgGoodTrgOverGoodTrackVsRecoEtaSC[i]);
    mGoodTrgOverGoodTrackVsRecoEtaSC[i]->Divide(mGoodTrgMuVsRecoEtaSC[i],mGoodTrackVsRecoEtaSC[i]);

    std::cout << "mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodTrgMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodRecoMuVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodTrgOverGoodRecoMuVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodTrgOverGoodRecoMuVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodTrgOverGoodRecoMuVsGenPtEtaSC[i]->SetBinContent(mGoodTrgOverGoodRecoMuVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodTrgOverGoodRecoMuVsGenPtEtaHiSC[i]->SetBinContent(mGoodTrgOverGoodRecoMuVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodTrgOverGoodRecoMuVsGenPtEtaLoSC[i]->SetBinContent(mGoodTrgOverGoodRecoMuVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodTrgMuVsGenPtSC[i], mGoodRecoMuVsGenPtSC[i], mgGoodTrgOverGoodRecoMuVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsGenPtSC[i], mGoodRecoMuVsGenPtSC[i], mgGoodTrgOverGoodRecoMuVsGenPtSC[i]);
    mGoodTrgOverGoodRecoMuVsGenPtSC[i]->Divide(mGoodTrgMuVsGenPtSC[i],mGoodRecoMuVsGenPtSC[i]);
//     BayesDivide(mGoodTrgMuVsGenEtaSC[i], mGoodRecoMuVsGenEtaSC[i], mgGoodTrgOverGoodRecoMuVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsGenEtaSC[i], mGoodRecoMuVsGenEtaSC[i], mgGoodTrgOverGoodRecoMuVsGenEtaSC[i]);
    mGoodTrgOverGoodRecoMuVsGenEtaSC[i]->Divide(mGoodTrgMuVsGenEtaSC[i],mGoodRecoMuVsGenEtaSC[i]);
    
    std::cout << "mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodTrgMuVsRecoPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodRecoMuVsRecoPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i]->SetBinContent(mGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodTrgOverGoodRecoMuVsRecoPtEtaHiSC[i]->SetBinContent(mGoodTrgOverGoodRecoMuVsRecoPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodTrgOverGoodRecoMuVsRecoPtEtaLoSC[i]->SetBinContent(mGoodTrgOverGoodRecoMuVsRecoPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodTrgMuVsRecoPtSC[i], mGoodRecoMuVsRecoPtSC[i], mgGoodTrgOverGoodRecoMuVsRecoPtSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsRecoPtSC[i], mGoodRecoMuVsRecoPtSC[i], mgGoodTrgOverGoodRecoMuVsRecoPtSC[i]);
    mGoodTrgOverGoodRecoMuVsRecoPtSC[i]->Divide(mGoodTrgMuVsRecoPtSC[i],mGoodRecoMuVsRecoPtSC[i]);
//     BayesDivide(mGoodTrgMuVsRecoEtaSC[i], mGoodRecoMuVsRecoEtaSC[i], mgGoodTrgOverGoodRecoMuVsRecoEtaSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsRecoEtaSC[i], mGoodRecoMuVsRecoEtaSC[i], mgGoodTrgOverGoodRecoMuVsRecoEtaSC[i]);
    mGoodTrgOverGoodRecoMuVsRecoEtaSC[i]->Divide(mGoodTrgMuVsRecoEtaSC[i],mGoodRecoMuVsRecoEtaSC[i]);

    std::cout << "mvgGoodTrgOverGoodSelMuVsGenPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodTrgMuVsGenPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodSelMuVsGenPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodTrgOverGoodSelMuVsGenPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodTrgOverGoodSelMuVsGenPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodTrgOverGoodSelMuVsGenPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodTrgOverGoodSelMuVsGenPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodTrgOverGoodSelMuVsGenPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodTrgOverGoodSelMuVsGenPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodTrgOverGoodSelMuVsGenPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodTrgOverGoodSelMuVsGenPtEtaSC[i]->SetBinContent(mGoodTrgOverGoodSelMuVsGenPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodTrgOverGoodSelMuVsGenPtEtaHiSC[i]->SetBinContent(mGoodTrgOverGoodSelMuVsGenPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodTrgOverGoodSelMuVsGenPtEtaLoSC[i]->SetBinContent(mGoodTrgOverGoodSelMuVsGenPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodTrgMuVsGenPtSC[i], mGoodSelMuVsGenPtSC[i], mgGoodTrgOverGoodSelMuVsGenPtSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsGenPtSC[i], mGoodSelMuVsGenPtSC[i], mgGoodTrgOverGoodSelMuVsGenPtSC[i]);
    mGoodTrgOverGoodSelMuVsGenPtSC[i]->Divide(mGoodTrgMuVsGenPtSC[i],mGoodSelMuVsGenPtSC[i]);
//     BayesDivide(mGoodTrgMuVsGenEtaSC[i], mGoodSelMuVsGenEtaSC[i], mgGoodTrgOverGoodSelMuVsGenEtaSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsGenEtaSC[i], mGoodSelMuVsGenEtaSC[i], mgGoodTrgOverGoodSelMuVsGenEtaSC[i]);
    mGoodTrgOverGoodSelMuVsGenEtaSC[i]->Divide(mGoodTrgMuVsGenEtaSC[i],mGoodSelMuVsGenEtaSC[i]);
    
    std::cout << "mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC\n";
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      TH1D* hPass = mGoodTrgMuVsRecoPtEtaSC[i]->ProjectionY("hPass", iGraph+1, iGraph+1);
      TH1D* hTotal = mGoodSelMuVsRecoPtEtaSC[i]->ProjectionY("hTotal", iGraph+1, iGraph+1);
//       BayesDivide(hPass, hTotal, mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC[i][iGraph]);
      ClopperPearsonDivide(hPass, hTotal, mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC[i][iGraph]);
    }
    for(unsigned int iGraph = 0; iGraph < nEtaBins; iGraph++)
    {
      unsigned int nPoints = mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC[i][iGraph]->GetN();
      for(unsigned int iPoint = 0; iPoint < nPoints; iPoint++)
      {
        mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC[i][iGraph]->GetPoint(iPoint,xGraphValue,yGraphValue);
        unsigned int yBin = mGoodTrgOverGoodSelMuVsRecoPtEtaSC[i]->GetYaxis()->FindBin(xGraphValue);
        yGraphErrorHi = mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC[i][iGraph]->GetErrorYhigh(iPoint);
        yGraphErrorLo = mvgGoodTrgOverGoodSelMuVsRecoPtEtaSC[i][iGraph]->GetErrorYlow(iPoint);
        if(yGraphErrorHi < 1e-300 || yGraphErrorHi > 65534)
          yGraphErrorHi = 0;
        if(yGraphErrorLo < 1e-300 || yGraphErrorLo > 65534)
          yGraphErrorLo = 0;
        if(yGraphValue+yGraphErrorHi > 1)
          yGraphErrorHi = 0;
        if(yGraphValue-yGraphErrorLo < 0)
          yGraphErrorLo = 0;
        mGoodTrgOverGoodSelMuVsRecoPtEtaSC[i]->SetBinContent(mGoodTrgOverGoodSelMuVsRecoPtEtaSC[i]->GetBin(iGraph+1,yBin),yGraphValue);
        mGoodTrgOverGoodSelMuVsRecoPtEtaHiSC[i]->SetBinContent(mGoodTrgOverGoodSelMuVsRecoPtEtaHiSC[i]->GetBin(iGraph+1,yBin),yGraphErrorHi);
        mGoodTrgOverGoodSelMuVsRecoPtEtaLoSC[i]->SetBinContent(mGoodTrgOverGoodSelMuVsRecoPtEtaLoSC[i]->GetBin(iGraph+1,yBin),yGraphErrorLo);
      }
    }
//     BayesDivide(mGoodTrgMuVsRecoPtSC[i], mGoodSelMuVsRecoPtSC[i], mgGoodTrgOverGoodSelMuVsRecoPtSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsRecoPtSC[i], mGoodSelMuVsRecoPtSC[i], mgGoodTrgOverGoodSelMuVsRecoPtSC[i]);
    mGoodTrgOverGoodSelMuVsRecoPtSC[i]->Divide(mGoodTrgMuVsRecoPtSC[i],mGoodSelMuVsRecoPtSC[i]);
//     BayesDivide(mGoodTrgMuVsRecoEtaSC[i], mGoodSelMuVsRecoEtaSC[i], mgGoodTrgOverGoodSelMuVsRecoEtaSC[i]);
    ClopperPearsonDivide(mGoodTrgMuVsRecoEtaSC[i], mGoodSelMuVsRecoEtaSC[i], mgGoodTrgOverGoodSelMuVsRecoEtaSC[i]);
    mGoodTrgOverGoodSelMuVsRecoEtaSC[i]->Divide(mGoodTrgMuVsRecoEtaSC[i],mGoodSelMuVsRecoEtaSC[i]);
  }

  for(unsigned int iCanvas = 0; iCanvas < nEtaBins; iCanvas++)
  {
    oss.str("");
    oss << "cGoodTrackOverGenMuVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodTrackOverGenMuVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodTrackOverGenMuVsGenPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodRecoOverGenMuVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodRecoOverGenMuVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodRecoOverGenMuVsGenPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodRecoOverGoodTrackVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodRecoOverGoodTrackVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodRecoOverGoodTrackVsGenPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodRecoOverGoodTrackVsRecoPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodRecoOverGoodTrackVsRecoPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodRecoOverGoodTrackVsRecoPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodSelOverGenMuVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodSelOverGenMuVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodSelOverGenMuVsGenPtEta[iCanvas]);
    
    oss.str("");
    oss << "cGoodSelOverGoodTrackVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodSelOverGoodTrackVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodSelOverGoodTrackVsGenPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodSelOverGoodTrackVsRecoPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodSelOverGoodTrackVsRecoPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodSelOverGoodTrackVsRecoPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodSelOverGoodRecoMuVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodSelOverGoodRecoMuVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodSelOverGoodRecoMuVsGenPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodSelOverGoodRecoMuVsRecoPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodSelOverGoodRecoMuVsRecoPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodSelOverGoodRecoMuVsRecoPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodTrgOverGenMuVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodTrgOverGenMuVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodTrgOverGenMuVsGenPtEta[iCanvas]);
    
    oss.str("");
    oss << "cGoodTrgOverGoodTrackVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodTrgOverGoodTrackVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodTrgOverGoodTrackVsGenPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodTrgOverGoodTrackVsRecoPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodTrgOverGoodTrackVsRecoPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodTrgOverGoodTrackVsRecoPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodTrgOverGoodRecoMuVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodTrgOverGoodRecoMuVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodTrgOverGoodRecoMuVsGenPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodTrgOverGoodRecoMuVsRecoPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodTrgOverGoodRecoMuVsRecoPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodTrgOverGoodRecoMuVsRecoPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodTrgOverGoodSelMuVsGenPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodTrgOverGoodSelMuVsGenPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodTrgOverGoodSelMuVsGenPtEta[iCanvas]);

    oss.str("");
    oss << "cGoodTrgOverGoodSelMuVsRecoPt_EtaBin" << iCanvas+1;
    name = oss.str();
    std::cout << name << std::endl;
    if(vgGoodTrgOverGoodSelMuVsRecoPtEta[iCanvas]->GetN() != 0)
      CreateAndWriteCanvas(name.c_str(), 21, 1, false, false, vgGoodTrgOverGoodSelMuVsRecoPtEta[iCanvas]);
  }

  CreateAndWriteCanvas("cAllGenMuVsGenPtEta", "colz", false, false, false, hAllGenMuVsGenPtEta);
  CreateAndWriteCanvas("cAllGenOutsideCutsMuVsGenPtEta", "colz", false, false, false, hAllGenMuOutsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cAllGenMuInAcceptanceVsGenPtEta", "colz", false, false, false, hAllGenMuInAcceptanceVsGenPtEta);
  CreateAndWriteCanvas("cAllRecoMuVsRecoPtEta", "colz", false, false, false, hAllRecoMuVsRecoPtEta);
  CreateAndWriteCanvas("cAllRecoMuInsideCutsVsRecoPtEta", "colz", false, false, false, hAllRecoMuInsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cAllSelMuVsRecoPtEta", "colz", false, false, false, hAllSelMuVsRecoPtEta);
  CreateAndWriteCanvas("cAllSelMuInsideCutsVsRecoPtEta", "colz", false, false, false, hAllSelMuInsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cAllTrgMuVsRecoPtEta", "colz", false, false, false, hAllTrgMuVsRecoPtEta);
  CreateAndWriteCanvas("cAllTrgMuInsideCutsVsRecoPtEta", "colz", false, false, false, hAllTrgMuInsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrackVsGenPtEta", "colz", false, false, false, hGoodTrackVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrackInsideCutsVsGenPtEta", "colz", false, false, false, hGoodTrackInsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrackOutsideCutsVsGenPtEta", "colz", false, false, false, hGoodTrackOutsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrackVsRecoPtEta", "colz", false, false, false, hGoodTrackVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrackInsideCutsVsRecoPtEta", "colz", false, false, false, hGoodTrackInsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrackOutsideCutsVsRecoPtEta", "colz", false, false, false, hGoodTrackOutsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cGoodRecoMuVsGenPtEta", "colz", false, false, false, hGoodRecoMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodRecoMuInsideCutsVsGenPtEta", "colz", false, false, false, hGoodRecoMuInsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cGoodRecoMuOutsideCutsVsGenPtEta", "colz", false, false, false, hGoodRecoMuOutsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cGoodRecoMuVsRecoPtEta", "colz", false, false, false, hGoodRecoMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodRecoMuInsideCutsVsRecoPtEta", "colz", false, false, false, hGoodRecoMuInsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cGoodRecoMuOutsideCutsVsRecoPtEta", "colz", false, false, false, hGoodRecoMuOutsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cGoodSelMuVsGenPtEta", "colz", false, false, false, hGoodSelMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodSelMuInsideCutsVsGenPtEta", "colz", false, false, false, hGoodSelMuInsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cGoodSelMuOutsideCutsVsGenPtEta", "colz", false, false, false, hGoodSelMuOutsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cGoodSelMuVsRecoPtEta", "colz", false, false, false, hGoodSelMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodSelMuInsideCutsVsRecoPtEta", "colz", false, false, false, hGoodSelMuInsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cGoodSelMuOutsideCutsVsRecoPtEta", "colz", false, false, false, hGoodSelMuOutsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrgMuVsGenPtEta", "colz", false, false, false, hGoodTrgMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrgMuInsideCutsVsGenPtEta", "colz", false, false, false, hGoodTrgMuInsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrgMuOutsideCutsVsGenPtEta", "colz", false, false, false, hGoodTrgMuOutsideCutsVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrgMuVsRecoPtEta", "colz", false, false, false, hGoodTrgMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrgMuInsideCutsVsRecoPtEta", "colz", false, false, false, hGoodTrgMuInsideCutsVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrgMuOutsideCutsVsRecoPtEta", "colz", false, false, false, hGoodTrgMuOutsideCutsVsRecoPtEta);
  
  CreateAndWriteCanvas("cAllGenMuVsGenPt", 0, 21, 1, false, false, hAllGenMuVsGenPt);
  CreateAndWriteCanvas("cAllGenOutsideCutsMuVsGenPt", 0, 21, 1, false, false, hAllGenMuOutsideCutsVsGenPt);
  CreateAndWriteCanvas("cAllGenMuInAcceptanceVsGenPt", 0, 21, 1, false, false, hAllGenMuInAcceptanceVsGenPt);
  CreateAndWriteCanvas("cAllGenMuVsGenEta", 0, 21, 1, false, false, hAllGenMuVsGenEta);
  CreateAndWriteCanvas("cAllGenOutsideCutsMuVsGenEta", 0, 21, 1, false, false, hAllGenMuOutsideCutsVsGenEta);
  CreateAndWriteCanvas("cAllGenMuInAcceptanceVsGenEta", 0, 21, 1, false, false, hAllGenMuInAcceptanceVsGenEta);
  CreateAndWriteCanvas("cAllRecoMuVsRecoPt", 0, 21, 1, false, false, hAllRecoMuVsRecoPt);
  CreateAndWriteCanvas("cAllRecoMuInsideCutsVsRecoPt", 0, 21, 1, false, false, hAllRecoMuInsideCutsVsRecoPt);
  CreateAndWriteCanvas("cAllRecoMuVsRecoEta", 0, 21, 1, false, false, hAllRecoMuVsRecoEta);
  CreateAndWriteCanvas("cAllRecoMuInsideCutsVsRecoEta", 0, 21, 1, false, false, hAllRecoMuInsideCutsVsRecoEta);
  CreateAndWriteCanvas("cAllSelMuVsRecoPt", 0, 21, 1, false, false, hAllSelMuVsRecoPt);
  CreateAndWriteCanvas("cAllSelMuInsideCutsVsRecoPt", 0, 21, 1, false, false, hAllSelMuInsideCutsVsRecoPt);
  CreateAndWriteCanvas("cAllSelMuVsRecoEta", 0, 21, 1, false, false, hAllSelMuVsRecoEta);
  CreateAndWriteCanvas("cAllSelMuInsideCutsVsRecoEta", 0, 21, 1, false, false, hAllSelMuInsideCutsVsRecoEta);
  CreateAndWriteCanvas("cAllTrgMuVsRecoPt", 0, 21, 1, false, false, hAllTrgMuVsRecoPt);
  CreateAndWriteCanvas("cAllTrgMuInsideCutsVsRecoPt", 0, 21, 1, false, false, hAllTrgMuInsideCutsVsRecoPt);
  CreateAndWriteCanvas("cAllTrgMuVsRecoEta", 0, 21, 1, false, false, hAllTrgMuVsRecoEta);
  CreateAndWriteCanvas("cAllTrgMuInsideCutsVsRecoEta", 0, 21, 1, false, false, hAllTrgMuInsideCutsVsRecoEta);
  CreateAndWriteCanvas("cGoodTrackVsGenPt", 0, 21, 1, false, false, hGoodTrackVsGenPt);
  CreateAndWriteCanvas("cGoodTrackInsideCutsVsGenPt", 0, 21, 1, false, false, hGoodTrackInsideCutsVsGenPt);
  CreateAndWriteCanvas("cGoodTrackOutsideCutsVsGenPt", 0, 21, 1, false, false, hGoodTrackOutsideCutsVsGenPt);
  CreateAndWriteCanvas("cGoodTrackVsRecoPt", 0, 21, 1, false, false, hGoodTrackVsRecoPt);
  CreateAndWriteCanvas("cGoodTrackInsideCutsVsRecoPt", 0, 21, 1, false, false, hGoodTrackInsideCutsVsRecoPt);
  CreateAndWriteCanvas("cGoodTrackOutsideCutsVsRecoPt", 0, 21, 1, false, false, hGoodTrackOutsideCutsVsRecoPt);
  CreateAndWriteCanvas("cGoodTrackVsGenEta", 0, 21, 1, false, false, hGoodTrackVsGenEta);
  CreateAndWriteCanvas("cGoodTrackInsideCutsVsGenEta", 0, 21, 1, false, false, hGoodTrackInsideCutsVsGenEta);
  CreateAndWriteCanvas("cGoodTrackOutsideCutsVsGenEta", 0, 21, 1, false, false, hGoodTrackOutsideCutsVsGenEta);
  CreateAndWriteCanvas("cGoodTrackVsRecoEta", 0, 21, 1, false, false, hGoodTrackVsRecoEta);
  CreateAndWriteCanvas("cGoodTrackInsideCutsVsRecoEta", 0, 21, 1, false, false, hGoodTrackInsideCutsVsRecoEta);
  CreateAndWriteCanvas("cGoodTrackOutsideCutsVsRecoEta", 0, 21, 1, false, false, hGoodTrackOutsideCutsVsRecoEta);
  CreateAndWriteCanvas("cGoodRecoMuVsGenPt", 0, 21, 1, false, false, hGoodRecoMuVsGenPt);
  CreateAndWriteCanvas("cGoodRecoMuInsideCutsVsGenPt", 0, 21, 1, false, false, hGoodRecoMuInsideCutsVsGenPt);
  CreateAndWriteCanvas("cGoodRecoMuOutsideCutsVsGenPt", 0, 21, 1, false, false, hGoodRecoMuOutsideCutsVsGenPt);
  CreateAndWriteCanvas("cGoodRecoMuVsRecoPt", 0, 21, 1, false, false, hGoodRecoMuVsRecoPt);
  CreateAndWriteCanvas("cGoodRecoMuInsideCutsVsRecoPt", 0, 21, 1, false, false, hGoodRecoMuInsideCutsVsRecoPt);
  CreateAndWriteCanvas("cGoodRecoMuOutsideCutsVsRecoPt", 0, 21, 1, false, false, hGoodRecoMuOutsideCutsVsRecoPt);
  CreateAndWriteCanvas("cGoodRecoMuVsGenEta", 0, 21, 1, false, false, hGoodRecoMuVsGenEta);
  CreateAndWriteCanvas("cGoodRecoMuInsideCutsVsGenEta", 0, 21, 1, false, false, hGoodRecoMuInsideCutsVsGenEta);
  CreateAndWriteCanvas("cGoodRecoMuOutsideCutsVsGenEta", 0, 21, 1, false, false, hGoodRecoMuOutsideCutsVsGenEta);
  CreateAndWriteCanvas("cGoodRecoMuVsRecoEta", 0, 21, 1, false, false, hGoodRecoMuVsRecoEta);
  CreateAndWriteCanvas("cGoodRecoMuInsideCutsVsRecoEta", 0, 21, 1, false, false, hGoodRecoMuInsideCutsVsRecoEta);
  CreateAndWriteCanvas("cGoodRecoMuOutsideCutsVsRecoEta", 0, 21, 1, false, false, hGoodRecoMuOutsideCutsVsRecoEta);
  CreateAndWriteCanvas("cGoodSelMuVsGenPt", 0, 21, 1, false, false, hGoodSelMuVsGenPt);
  CreateAndWriteCanvas("cGoodSelMuInsideCutsVsGenPt", 0, 21, 1, false, false, hGoodSelMuInsideCutsVsGenPt);
  CreateAndWriteCanvas("cGoodSelMuOutsideCutsVsGenPt", 0, 21, 1, false, false, hGoodSelMuOutsideCutsVsGenPt);
  CreateAndWriteCanvas("cGoodSelMuVsRecoPt", 0, 21, 1, false, false, hGoodSelMuVsRecoPt);
  CreateAndWriteCanvas("cGoodSelMuInsideCutsVsRecoPt", 0, 21, 1, false, false, hGoodSelMuInsideCutsVsRecoPt);
  CreateAndWriteCanvas("cGoodSelMuOutsideCutsVsRecoPt", 0, 21, 1, false, false, hGoodSelMuOutsideCutsVsRecoPt);
  CreateAndWriteCanvas("cGoodSelMuVsGenEta", 0, 21, 1, false, false, hGoodSelMuVsGenEta);
  CreateAndWriteCanvas("cGoodSelMuInsideCutsVsGenEta", 0, 21, 1, false, false, hGoodSelMuInsideCutsVsGenEta);
  CreateAndWriteCanvas("cGoodSelMuOutsideCutsVsGenEta", 0, 21, 1, false, false, hGoodSelMuOutsideCutsVsGenEta);
  CreateAndWriteCanvas("cGoodSelMuVsRecoEta", 0, 21, 1, false, false, hGoodSelMuVsRecoEta);
  CreateAndWriteCanvas("cGoodSelMuInsideCutsVsRecoEta", 0, 21, 1, false, false, hGoodSelMuInsideCutsVsRecoEta);
  CreateAndWriteCanvas("cGoodSelMuOutsideCutsVsRecoEta", 0, 21, 1, false, false, hGoodSelMuOutsideCutsVsRecoEta);
  CreateAndWriteCanvas("cGoodTrgMuVsGenPt", 0, 21, 1, false, false, hGoodTrgMuVsGenPt);
  CreateAndWriteCanvas("cGoodTrgMuInsideCutsVsGenPt", 0, 21, 1, false, false, hGoodTrgMuInsideCutsVsGenPt);
  CreateAndWriteCanvas("cGoodTrgMuOutsideCutsVsGenPt", 0, 21, 1, false, false, hGoodTrgMuOutsideCutsVsGenPt);
  CreateAndWriteCanvas("cGoodTrgMuVsRecoPt", 0, 21, 1, false, false, hGoodTrgMuVsRecoPt);
  CreateAndWriteCanvas("cGoodTrgMuInsideCutsVsRecoPt", 0, 21, 1, false, false, hGoodTrgMuInsideCutsVsRecoPt);
  CreateAndWriteCanvas("cGoodTrgMuOutsideCutsVsRecoPt", 0, 21, 1, false, false, hGoodTrgMuOutsideCutsVsRecoPt);
  CreateAndWriteCanvas("cGoodTrgMuVsGenEta", 0, 21, 1, false, false, hGoodTrgMuVsGenEta);
  CreateAndWriteCanvas("cGoodTrgMuInsideCutsVsGenEta", 0, 21, 1, false, false, hGoodTrgMuInsideCutsVsGenEta);
  CreateAndWriteCanvas("cGoodTrgMuOutsideCutsVsGenEta", 0, 21, 1, false, false, hGoodTrgMuOutsideCutsVsGenEta);
  CreateAndWriteCanvas("cGoodTrgMuVsRecoEta", 0, 21, 1, false, false, hGoodTrgMuVsRecoEta);
  CreateAndWriteCanvas("cGoodTrgMuInsideCutsVsRecoEta", 0, 21, 1, false, false, hGoodTrgMuInsideCutsVsRecoEta);
  CreateAndWriteCanvas("cGoodTrgMuOutsideCutsVsRecoEta", 0, 21, 1, false, false, hGoodTrgMuOutsideCutsVsRecoEta);

  CreateAndWriteCanvas("cGoodTrackOverGenMuVsGenPtEta", "colz", false, false, false, hGoodTrackOverGenMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrackOverGenMuVsGenPtEtaHi", "colz", false, false, false, hGoodTrackOverGenMuVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodTrackOverGenMuVsGenPtEtaLo", "colz", false, false, false, hGoodTrackOverGenMuVsGenPtEtaLo);
  if(gGoodTrackOverGenMuVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrackOverGenMuVsGenPt", 21, 1, false, false, gGoodTrackOverGenMuVsGenPt);
  if(gGoodTrackOverGenMuVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrackOverGenMuVsGenEta", 21, 1, false, false, gGoodTrackOverGenMuVsGenEta);
  std::cout << "Sono qui1\n";
  
  CreateAndWriteCanvas("cGoodRecoOverGenMuVsGenPtEta", "colz", false, false, false, hGoodRecoOverGenMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodRecoOverGenMuVsGenPtEtaHi", "colz", false, false, false, hGoodRecoOverGenMuVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodRecoOverGenMuVsGenPtEtaLo", "colz", false, false, false, hGoodRecoOverGenMuVsGenPtEtaLo);
  if(gGoodRecoOverGenMuVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodRecoOverGenMuVsGenPt", 21, 1, false, false, gGoodRecoOverGenMuVsGenPt);
  if(gGoodRecoOverGenMuVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodRecoOverGenMuVsGenEta", 21, 1, false, false, gGoodRecoOverGenMuVsGenEta);
  std::cout << "Sono qui2\n";
  
  CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsGenPtEta", "colz", false, false, false, hGoodRecoOverGoodTrackVsGenPtEta);
  CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsGenPtEtaHi", "colz", false, false, false, hGoodRecoOverGoodTrackVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsGenPtEtaLo", "colz", false, false, false, hGoodRecoOverGoodTrackVsGenPtEtaLo);
  if(gGoodRecoOverGoodTrackVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsGenPt", 21, 1, false, false, gGoodRecoOverGoodTrackVsGenPt);
  if(gGoodRecoOverGoodTrackVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsGenEta", 21, 1, false, false, gGoodRecoOverGoodTrackVsGenEta);
  std::cout << "Sono qui3\n";
  
  CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsRecoPtEta", "colz", false, false, false, hGoodRecoOverGoodTrackVsRecoPtEta);
  CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsRecoPtEtaHi", "colz", false, false, false, hGoodRecoOverGoodTrackVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsRecoPtEtaLo", "colz", false, false, false, hGoodRecoOverGoodTrackVsRecoPtEtaLo);
  if(gGoodRecoOverGoodTrackVsRecoPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsRecoPt", 21, 1, false, false, gGoodRecoOverGoodTrackVsRecoPt);
  if(gGoodRecoOverGoodTrackVsRecoEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodRecoOverGoodTrackVsRecoEta", 21, 1, false, false, gGoodRecoOverGoodTrackVsRecoEta);
  std::cout << "Sono qui4\n";
  
  CreateAndWriteCanvas("cGoodSelOverGenMuVsGenPtEta", "colz", false, false, false, hGoodSelOverGenMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodSelOverGenMuVsGenPtEtaHi", "colz", false, false, false, hGoodSelOverGenMuVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodSelOverGenMuVsGenPtEtaLo", "colz", false, false, false, hGoodSelOverGenMuVsGenPtEtaLo);
  if(gGoodSelOverGenMuVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGenMuVsGenPt", 21, 1, false, false, gGoodSelOverGenMuVsGenPt);
  if(gGoodSelOverGenMuVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGenMuVsGenEta", 21, 1, false, false, gGoodSelOverGenMuVsGenEta);
  std::cout << "Sono qui5\n";
  
  CreateAndWriteCanvas("cGoodSelOverGoodTrackVsGenPtEta", "colz", false, false, false, hGoodSelOverGoodTrackVsGenPtEta);
  CreateAndWriteCanvas("cGoodSelOverGoodTrackVsGenPtEtaHi", "colz", false, false, false, hGoodSelOverGoodTrackVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodSelOverGoodTrackVsGenPtEtaLo", "colz", false, false, false, hGoodSelOverGoodTrackVsGenPtEtaLo);
  if(gGoodSelOverGoodTrackVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGoodTrackVsGenPt", 21, 1, false, false, gGoodSelOverGoodTrackVsGenPt);
  if(gGoodSelOverGoodTrackVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGoodTrackVsGenEta", 21, 1, false, false, gGoodSelOverGoodTrackVsGenEta);
  std::cout << "Sono qui6\n";
  
  CreateAndWriteCanvas("cGoodSelOverGoodTrackVsRecoPtEta", "colz", false, false, false, hGoodSelOverGoodTrackVsRecoPtEta);
  CreateAndWriteCanvas("cGoodSelOverGoodTrackVsRecoPtEtaHi", "colz", false, false, false, hGoodSelOverGoodTrackVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodSelOverGoodTrackVsRecoPtEtaLo", "colz", false, false, false, hGoodSelOverGoodTrackVsRecoPtEtaLo);
  if(gGoodSelOverGoodTrackVsRecoPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGoodTrackVsRecoPt", 21, 1, false, false, gGoodSelOverGoodTrackVsRecoPt);
  if(gGoodSelOverGoodTrackVsRecoEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGoodTrackVsRecoEta", 21, 1, false, false, gGoodSelOverGoodTrackVsRecoEta);
  std::cout << "Sono qui7\n";
  
  CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsGenPtEta", "colz", false, false, false, hGoodSelOverGoodRecoMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsGenPtEtaHi", "colz", false, false, false, hGoodSelOverGoodRecoMuVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsGenPtEtaLo", "colz", false, false, false, hGoodSelOverGoodRecoMuVsGenPtEtaLo);
  if(gGoodSelOverGoodRecoMuVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsGenPt", 21, 1, false, false, gGoodSelOverGoodRecoMuVsGenPt);
  if(gGoodSelOverGoodRecoMuVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsGenEta", 21, 1, false, false, gGoodSelOverGoodRecoMuVsGenEta);
  std::cout << "Sono qui8\n";
  
  CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsRecoPtEta", "colz", false, false, false, hGoodSelOverGoodRecoMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsRecoPtEtaHi", "colz", false, false, false, hGoodSelOverGoodRecoMuVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsRecoPtEtaLo", "colz", false, false, false, hGoodSelOverGoodRecoMuVsRecoPtEtaLo);
  if(gGoodSelOverGoodRecoMuVsRecoPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsRecoPt", 21, 1, false, false, gGoodSelOverGoodRecoMuVsRecoPt);
  if(gGoodSelOverGoodRecoMuVsRecoEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverGoodRecoMuVsRecoEta", 21, 1, false, false, gGoodSelOverGoodRecoMuVsRecoEta);
  std::cout << "Sono qui9\n";
  
  CreateAndWriteCanvas("cGoodTrgOverGenMuVsGenPtEta", "colz", false, false, false, hGoodTrgOverGenMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrgOverGenMuVsGenPtEtaHi", "colz", false, false, false, hGoodTrgOverGenMuVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodTrgOverGenMuVsGenPtEtaLo", "colz", false, false, false, hGoodTrgOverGenMuVsGenPtEtaLo);
  if(gGoodTrgOverGenMuVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGenMuVsGenPt", 21, 1, false, false, gGoodTrgOverGenMuVsGenPt);
  if(gGoodTrgOverGenMuVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGenMuVsGenEta", 21, 1, false, false, gGoodTrgOverGenMuVsGenEta);
  std::cout << "Sono qui10\n";
  
  CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsGenPtEta", "colz", false, false, false, hGoodTrgOverGoodTrackVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsGenPtEtaHi", "colz", false, false, false, hGoodTrgOverGoodTrackVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsGenPtEtaLo", "colz", false, false, false, hGoodTrgOverGoodTrackVsGenPtEtaLo);
  if(gGoodTrgOverGoodTrackVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsGenPt", 21, 1, false, false, gGoodTrgOverGoodTrackVsGenPt);
  if(gGoodTrgOverGoodTrackVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsGenEta", 21, 1, false, false, gGoodTrgOverGoodTrackVsGenEta);
  std::cout << "Sono qui11\n";
  
  CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsRecoPtEta", "colz", false, false, false, hGoodTrgOverGoodTrackVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsRecoPtEtaHi", "colz", false, false, false, hGoodTrgOverGoodTrackVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsRecoPtEtaLo", "colz", false, false, false, hGoodTrgOverGoodTrackVsRecoPtEtaLo);
  if(gGoodTrgOverGoodTrackVsRecoPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsRecoPt", 21, 1, false, false, gGoodTrgOverGoodTrackVsRecoPt);
  if(gGoodTrgOverGoodTrackVsRecoEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodTrackVsRecoEta", 21, 1, false, false, gGoodTrgOverGoodTrackVsRecoEta);
  std::cout << "Sono qui12\n";
  
  CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsGenPtEta", "colz", false, false, false, hGoodTrgOverGoodRecoMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsGenPtEtaHi", "colz", false, false, false, hGoodTrgOverGoodRecoMuVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsGenPtEtaLo", "colz", false, false, false, hGoodTrgOverGoodRecoMuVsGenPtEtaLo);
  if(gGoodTrgOverGoodRecoMuVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsGenPt", 21, 1, false, false, gGoodTrgOverGoodRecoMuVsGenPt);
  if(gGoodTrgOverGoodRecoMuVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsGenEta", 21, 1, false, false, gGoodTrgOverGoodRecoMuVsGenEta);
  std::cout << "Sono qui13\n";
  
  CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsRecoPtEta", "colz", false, false, false, hGoodTrgOverGoodRecoMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsRecoPtEtaHi", "colz", false, false, false, hGoodTrgOverGoodRecoMuVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsRecoPtEtaLo", "colz", false, false, false, hGoodTrgOverGoodRecoMuVsRecoPtEtaLo);
  if(gGoodTrgOverGoodRecoMuVsRecoPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsRecoPt", 21, 1, false, false, gGoodTrgOverGoodRecoMuVsRecoPt);
  if(gGoodTrgOverGoodRecoMuVsRecoEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodRecoMuVsRecoEta", 21, 1, false, false, gGoodTrgOverGoodRecoMuVsRecoEta);
  std::cout << "Sono qui14\n";
  
  CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsGenPtEta", "colz", false, false, false, hGoodTrgOverGoodSelMuVsGenPtEta);
  CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsGenPtEtaHi", "colz", false, false, false, hGoodTrgOverGoodSelMuVsGenPtEtaHi);
  CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsGenPtEtaLo", "colz", false, false, false, hGoodTrgOverGoodSelMuVsGenPtEtaLo);
  if(gGoodTrgOverGoodSelMuVsGenPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsGenPt", 21, 1, false, false, gGoodTrgOverGoodSelMuVsGenPt);
  if(gGoodTrgOverGoodSelMuVsGenEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsGenEta", 21, 1, false, false, gGoodTrgOverGoodSelMuVsGenEta);
  std::cout << "Sono qui15\n";
  
  CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsRecoPtEta", "colz", false, false, false, hGoodTrgOverGoodSelMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsRecoPtEtaHi", "colz", false, false, false, hGoodTrgOverGoodSelMuVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsRecoPtEtaLo", "colz", false, false, false, hGoodTrgOverGoodSelMuVsRecoPtEtaLo);
  if(gGoodTrgOverGoodSelMuVsRecoPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsRecoPt", 21, 1, false, false, gGoodTrgOverGoodSelMuVsRecoPt);
  if(gGoodTrgOverGoodSelMuVsRecoEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodTrgOverGoodSelMuVsRecoEta", 21, 1, false, false, gGoodTrgOverGoodSelMuVsRecoEta);
  std::cout << "Sono qui16\n";
  
  CreateAndWriteCanvas("cGoodRecoOverAllRecoMuVsRecoPtEta", "colz", false, false, false, hGoodRecoOverAllRecoMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodRecoOverAllRecoMuVsRecoPtEtaHi", "colz", false, false, false, hGoodRecoOverAllRecoMuVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodRecoOverAllRecoMuVsRecoPtEtaLo", "colz", false, false, false, hGoodRecoOverAllRecoMuVsRecoPtEtaLo);
  if(gGoodRecoOverAllRecoMuVsRecoPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodRecoOverAllRecoMuVsRecoPt", 21, 1, false, false, gGoodRecoOverAllRecoMuVsRecoPt);
  if(gGoodRecoOverAllRecoMuVsRecoEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodRecoOverAllRecoMuVsRecoEta", 21, 1, false, false, gGoodRecoOverAllRecoMuVsRecoEta);
  std::cout << "Sono qui17\n";
  
  CreateAndWriteCanvas("cGoodSelOverAllSelMuVsRecoPtEta", "colz", false, false, false, hGoodSelOverAllSelMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodSelOverAllSelMuVsRecoPtEtaHi", "colz", false, false, false, hGoodSelOverAllSelMuVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodSelOverAllSelMuVsRecoPtEtaLo", "colz", false, false, false, hGoodSelOverAllSelMuVsRecoPtEtaLo);
  if(gGoodSelOverAllSelMuVsRecoPt->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverAllSelMuVsRecoPt", 21, 1, false, false, gGoodSelOverAllSelMuVsRecoPt);
  if(gGoodSelOverAllSelMuVsRecoEta->GetN() != 0)
    CreateAndWriteCanvas("cGoodSelOverAllSelMuVsRecoEta", 21, 1, false, false, gGoodSelOverAllSelMuVsRecoEta);
  std::cout << "Sono qui18\n";
  
  CreateAndWriteCanvas("cGoodTrgOverAllTrgMuVsRecoPtEta", "colz", false, false, false, hGoodTrgOverAllTrgMuVsRecoPtEta);
  CreateAndWriteCanvas("cGoodTrgOverAllTrgMuVsRecoPtEtaHi", "colz", false, false, false, hGoodTrgOverAllTrgMuVsRecoPtEtaHi);
  CreateAndWriteCanvas("cGoodTrgOverAllTrgMuVsRecoPtEtaLo", "colz", false, false, false, hGoodTrgOverAllTrgMuVsRecoPtEtaLo);
  if(gGoodTrgOverAllTrgMuVsRecoPt->GetN()>0 && gGoodTrgOverAllTrgMuVsRecoPt->GetMaximum() != gGoodTrgOverAllTrgMuVsRecoPt->GetMinimum())
    CreateAndWriteCanvas("cGoodTrgOverAllTrgMuVsRecoPt", 21, 1, false, false, gGoodTrgOverAllTrgMuVsRecoPt);
  if(gGoodTrgOverAllTrgMuVsRecoEta->GetN()>0 && gGoodTrgOverAllTrgMuVsRecoEta->GetMaximum() != gGoodTrgOverAllTrgMuVsRecoEta->GetMinimum())
    CreateAndWriteCanvas("cGoodTrgOverAllTrgMuVsRecoEta", 21, 1, false, false, gGoodTrgOverAllTrgMuVsRecoEta);
  std::cout << "Sono qui19\n";
  
  std::cout << "Sono qui\n";
  
  for(unsigned int i = 0; i < 7 ; i++)
  {
    oss.str("");
    oss << "cAllGenMuVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mAllGenMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cAllGenMuOutsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mAllGenMuOutsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cAllGenMuInAcceptanceVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mAllGenMuInAcceptanceVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cAllGenMuVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mAllGenMuVsGenPtSC[i]);
    oss.str("");
    oss << "cAllGenMuOutsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mAllGenMuOutsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cAllGenMuInAcceptanceVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mAllGenMuInAcceptanceVsGenPtSC[i]);
    oss.str("");
    oss << "cAllGenMuVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mAllGenMuVsGenEtaSC[i]);
    oss.str("");
    oss << "cAllGenMuOutsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mAllGenMuOutsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cAllGenMuInAcceptanceVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mAllGenMuInAcceptanceVsGenEtaSC[i]);

    oss.str("");
    oss << "cGoodTrackVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackInsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackInsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackOutsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackOutsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackInsideCutsVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackInsideCutsVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackOutsideCutsVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackOutsideCutsVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodTrackInsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackInsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodTrackOutsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackOutsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodTrackVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodTrackInsideCutsVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21 ,1 , false, false, mGoodTrackInsideCutsVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodTrackOutsideCutsVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackOutsideCutsVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodTrackVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackInsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackInsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackOutsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackOutsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackVsRecoEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackInsideCutsVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21 ,1 , false, false, mGoodTrackInsideCutsVsRecoEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackOutsideCutsVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrackOutsideCutsVsRecoEtaSC[i]);

    oss.str("");
    oss << "cGoodRecoMuVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuInsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoMuInsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuOutsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoMuOutsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoMuVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuInsideCutsVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoMuInsideCutsVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuOutsideCutsVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoMuOutsideCutsVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodRecoMuInsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuInsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodRecoMuOutsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuOutsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodRecoMuVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodRecoMuInsideCutsVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21 ,1 , false, false, mGoodRecoMuInsideCutsVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodRecoMuOutsideCutsVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuOutsideCutsVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodRecoMuVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuInsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuInsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuOutsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuOutsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuVsRecoEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuInsideCutsVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21 ,1 , false, false, mGoodRecoMuInsideCutsVsRecoEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoMuOutsideCutsVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodRecoMuOutsideCutsVsRecoEtaSC[i]);

    oss.str("");
    oss << "cGoodSelMuVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuInsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelMuInsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuOutsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelMuOutsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuInsideCutsVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelMuInsideCutsVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuOutsideCutsVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelMuOutsideCutsVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodSelMuVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodSelMuInsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodSelMuInsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodSelMuOutsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21 ,1 , false, false, mGoodSelMuOutsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodSelMuInsideCutsVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodSelMuInsideCutsVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodSelMuOutsideCutsVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodSelMuOutsideCutsVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodSelMuVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodSelMuVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuInsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodSelMuInsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuOutsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21 ,1 , false, false, mGoodSelMuOutsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuInsideCutsVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodSelMuInsideCutsVsRecoEtaSC[i]);
    oss.str("");
    oss << "cGoodSelMuOutsideCutsVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodSelMuOutsideCutsVsRecoEtaSC[i]);
    
    oss.str("");
    oss << "cGoodTrgMuVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuInsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgMuInsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuOutsideCutsVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgMuOutsideCutsVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuInsideCutsVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgMuInsideCutsVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuOutsideCutsVsRecoPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgMuOutsideCutsVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrgMuVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodTrgMuInsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrgMuInsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodTrgMuOutsideCutsVsGenPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21 ,1 , false, false, mGoodTrgMuOutsideCutsVsGenPtSC[i]);
    oss.str("");
    oss << "cGoodTrgMuInsideCutsVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrgMuInsideCutsVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodTrgMuOutsideCutsVsRecoPtSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrgMuOutsideCutsVsRecoPtSC[i]);
    oss.str("");
    oss << "cGoodTrgMuVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrgMuVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuInsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrgMuInsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuOutsideCutsVsGenEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21 ,1 , false, false, mGoodTrgMuOutsideCutsVsGenEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuInsideCutsVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrgMuInsideCutsVsRecoEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgMuOutsideCutsVsRecoEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), 0, 21, 1, false, false, mGoodTrgMuOutsideCutsVsRecoEtaSC[i]);
    
    oss.str("");
    oss << "cGoodTrackOverGenMuVsGenPtEtaSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackOverGenMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrackOverGenMuVsGenPtEtaHiSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackOverGenMuVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodTrackOverGenMuVsGenPtEtaLoSC_" << i;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrackOverGenMuVsGenPtEtaLoSC[i]);
    if(mgGoodTrackOverGenMuVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrackOverGenMuVsGenPtSC_" << i;
      std::cout << oss.str() << std::endl;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrackOverGenMuVsGenPtSC[i]);
    }
    std::cout << "mgGoodTrackOverGenMuVsGenEtaSC\n";
    if(mgGoodTrackOverGenMuVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrackOverGenMuVsGenEtaSC_" << i;
      std::cout << oss.str() << std::endl;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrackOverGenMuVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodRecoOverGenMuVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGenMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoOverGenMuVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGenMuVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodRecoOverGenMuVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGenMuVsGenPtEtaLoSC[i]);
    std::cout << "mgGoodRecoOverGenMuVsGenPtSC\n";
    if(mgGoodRecoOverGenMuVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodRecoOverGenMuVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodRecoOverGenMuVsGenPtSC[i]);
    }
    std::cout << "mgGoodRecoOverGenMuVsGenEtaSC\n";
    if(mgGoodRecoOverGenMuVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodRecoOverGenMuVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodRecoOverGenMuVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodRecoOverGoodTrackVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGoodTrackVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoOverGoodTrackVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGoodTrackVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodRecoOverGoodTrackVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGoodTrackVsGenPtEtaLoSC[i]);
    if(mgGoodRecoOverGoodTrackVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodRecoOverGoodTrackVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodRecoOverGoodTrackVsGenPtSC[i]);
    }
    std::cout << "mgGoodRecoOverGoodTrackVsGenEtaSC\n";
    if(mgGoodRecoOverGoodTrackVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodRecoOverGoodTrackVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodRecoOverGoodTrackVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodRecoOverGoodTrackVsRecoPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGoodTrackVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodRecoOverGoodTrackVsRecoPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGoodTrackVsRecoPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodRecoOverGoodTrackVsRecoPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodRecoOverGoodTrackVsRecoPtEtaLoSC[i]);
    if(mgGoodRecoOverGoodTrackVsRecoPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodRecoOverGoodTrackVsRecoPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodRecoOverGoodTrackVsRecoPtSC[i]);
    }
    std::cout << "mgGoodRecoOverGoodTrackVsRecoEtaSC\n";
    if(mgGoodRecoOverGoodTrackVsRecoEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodRecoOverGoodTrackVsRecoEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodRecoOverGoodTrackVsRecoEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodSelOverGenMuVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGenMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelOverGenMuVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGenMuVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodSelOverGenMuVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGenMuVsGenPtEtaLoSC[i]);
    std::cout << "mgGoodSelOverGenMuVsGenPtSC\n";
    if(mgGoodSelOverGenMuVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGenMuVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGenMuVsGenPtSC[i]);
    }
    std::cout << "mgGoodSelOverGenMuVsGenEtaSC\n";
    if(mgGoodSelOverGenMuVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGenMuVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGenMuVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodSelOverGoodTrackVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodTrackVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelOverGoodTrackVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodTrackVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodSelOverGoodTrackVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodTrackVsGenPtEtaLoSC[i]);
    if(mgGoodSelOverGoodTrackVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGoodTrackVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGoodTrackVsGenPtSC[i]);
    }
    std::cout << "mgGoodSelOverGoodTrackVsGenEtaSC\n";
    if(mgGoodSelOverGoodTrackVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGoodTrackVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGoodTrackVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodSelOverGoodTrackVsRecoPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodTrackVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelOverGoodTrackVsRecoPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodTrackVsRecoPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodSelOverGoodTrackVsRecoPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodTrackVsRecoPtEtaLoSC[i]);
    if(mgGoodSelOverGoodTrackVsRecoPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGoodTrackVsRecoPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGoodTrackVsRecoPtSC[i]);
    }
    std::cout << "mgGoodSelOverGoodTrackVsRecoEtaSC\n";
    if(mgGoodSelOverGoodTrackVsRecoEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGoodTrackVsRecoEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGoodTrackVsRecoEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodSelOverGoodRecoMuVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodRecoMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelOverGoodRecoMuVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodRecoMuVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodSelOverGoodRecoMuVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodRecoMuVsGenPtEtaLoSC[i]);
    std::cout << "mgGoodSelOverGoodRecoMuVsGenPtSC\n";
    if(mgGoodSelOverGoodRecoMuVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGoodRecoMuVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGoodRecoMuVsGenPtSC[i]);
    }
    std::cout << "mgGoodSelOverGoodRecoMuVsGenEtaSC\n";
    if(mgGoodSelOverGoodRecoMuVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGoodRecoMuVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGoodRecoMuVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodSelOverGoodRecoMuVsRecoPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodRecoMuVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodSelOverGoodRecoMuVsRecoPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodRecoMuVsRecoPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodSelOverGoodRecoMuVsRecoPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodSelOverGoodRecoMuVsRecoPtEtaLoSC[i]);
    std::cout << "mgGoodSelOverGoodRecoMuVsRecoPtSC\n";
    if(mgGoodSelOverGoodRecoMuVsRecoPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGoodRecoMuVsRecoPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGoodRecoMuVsRecoPtSC[i]);
    }
    std::cout << "mgGoodSelOverGoodRecoMuVsRecoEtaSC\n";
    if(mgGoodSelOverGoodRecoMuVsRecoEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodSelOverGoodRecoMuVsRecoEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodSelOverGoodRecoMuVsRecoEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodTrgOverGenMuVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGenMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGenMuVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGenMuVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGenMuVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGenMuVsGenPtEtaLoSC[i]);
    std::cout << "mgGoodTrgOverGenMuVsGenPtSC\n";
    if(mgGoodTrgOverGenMuVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGenMuVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGenMuVsGenPtSC[i]);
    }
    std::cout << "mgGoodTrgOverGenMuVsGenEtaSC\n";
    if(mgGoodTrgOverGenMuVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGenMuVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGenMuVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodTrgOverGoodTrackVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodTrackVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodTrackVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodTrackVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodTrackVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodTrackVsGenPtEtaLoSC[i]);
    if(mgGoodTrgOverGoodTrackVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodTrackVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodTrackVsGenPtSC[i]);
    }
    std::cout << "mgGoodTrgOverGoodTrackVsGenEtaSC\n";
    if(mgGoodTrgOverGoodTrackVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodTrackVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodTrackVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodTrgOverGoodTrackVsRecoPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodTrackVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodTrackVsRecoPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodTrackVsRecoPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodTrackVsRecoPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodTrackVsRecoPtEtaLoSC[i]);
    if(mgGoodTrgOverGoodTrackVsRecoPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodTrackVsRecoPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodTrackVsRecoPtSC[i]);
    }
    std::cout << "mgGoodTrgOverGoodTrackVsRecoEtaSC\n";
    if(mgGoodTrgOverGoodTrackVsRecoEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodTrackVsRecoEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodTrackVsRecoEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodTrgOverGoodRecoMuVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodRecoMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodRecoMuVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodRecoMuVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodRecoMuVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodRecoMuVsGenPtEtaLoSC[i]);
    std::cout << "mgGoodTrgOverGoodRecoMuVsGenPtSC\n";
    if(mgGoodTrgOverGoodRecoMuVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodRecoMuVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodRecoMuVsGenPtSC[i]);
    }
    std::cout << "mgGoodTrgOverGoodRecoMuVsGenEtaSC\n";
    if(mgGoodTrgOverGoodRecoMuVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodRecoMuVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodRecoMuVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodTrgOverGoodRecoMuVsRecoPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodRecoMuVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodRecoMuVsRecoPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodRecoMuVsRecoPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodRecoMuVsRecoPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodRecoMuVsRecoPtEtaLoSC[i]);
    std::cout << "mgGoodTrgOverGoodRecoMuVsRecoPtSC\n";
    if(mgGoodTrgOverGoodRecoMuVsRecoPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodRecoMuVsRecoPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodRecoMuVsRecoPtSC[i]);
    }
    std::cout << "mgGoodTrgOverGoodRecoMuVsRecoEtaSC\n";
    if(mgGoodTrgOverGoodRecoMuVsRecoEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodRecoMuVsRecoEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodRecoMuVsRecoEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodTrgOverGoodSelMuVsGenPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodSelMuVsGenPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodSelMuVsGenPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodSelMuVsGenPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodSelMuVsGenPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodSelMuVsGenPtEtaLoSC[i]);
    std::cout << "mgGoodTrgOverGoodSelMuVsGenPtSC\n";
    if(mgGoodTrgOverGoodSelMuVsGenPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodSelMuVsGenPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodSelMuVsGenPtSC[i]);
    }
    std::cout << "mgGoodTrgOverGoodSelMuVsGenEtaSC\n";
    if(mgGoodTrgOverGoodSelMuVsGenEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodSelMuVsGenEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodSelMuVsGenEtaSC[i]);
    }

    oss.str("");
    oss << "cGoodTrgOverGoodSelMuVsRecoPtEtaSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodSelMuVsRecoPtEtaSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodSelMuVsRecoPtEtaHiSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodSelMuVsRecoPtEtaHiSC[i]);
    oss.str("");
    oss << "cGoodTrgOverGoodSelMuVsRecoPtEtaLoSC_" << i;
    std::cout << oss.str() << std::endl;
    CreateAndWriteCanvas(oss.str().c_str(), "colz", false, false, false, mGoodTrgOverGoodSelMuVsRecoPtEtaLoSC[i]);
    std::cout << "mgGoodTrgOverGoodSelMuVsRecoPtSC\n";
    if(mgGoodTrgOverGoodSelMuVsRecoPtSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodSelMuVsRecoPtSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodSelMuVsRecoPtSC[i]);
    }
    std::cout << "mgGoodTrgOverGoodSelMuVsRecoEtaSC\n";
    if(mgGoodTrgOverGoodSelMuVsRecoEtaSC[i]->GetN() != 0)
    {
      oss.str("");
      oss << "cGoodTrgOverGoodSelMuVsRecoEtaSC_" << i;
      CreateAndWriteCanvas(oss.str().c_str(), 21, 1, false, false, mgGoodTrgOverGoodSelMuVsRecoEtaSC[i]);
    }
  }

  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // MuonEfficienciesComputerFromMCTruth_cxx
