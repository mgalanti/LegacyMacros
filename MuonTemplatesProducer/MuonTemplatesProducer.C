#ifndef MuonTemplatesProducer_cxx
#define MuonTemplatesProducer_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include <sstream>
#include <map>

#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "MuonTemplatesProducer.h"

#include "../utilities/constants.h"


void MuonTemplatesProducer::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Setup the random number engine for template smearing
  TRandom3* rndm = new TRandom3();
  rndm->SetSeed(12345);

//   // This is used to make 2D ptRel Vs pt plots
//   const double muonPtMin = 0.;
//   const double muonPtMax = 100.;
//   const unsigned int nMuonPtBins = 100*(muonPtMax-muonPtMin);
//
//   const double jetPtMin = 0.;
//   const double jetPtMax = 500.;
//   const unsigned int nJetPtBins = 100*(jetPtMax-jetPtMin);

  // Book histograms here
  // Genp-Muon association
  TH2D* hOutVsInAssocGenpId = Create2DHistogram<TH2D>("hOutVsInAssocGenpId", "Genp id associated to outer vs. inner track", 4999, -2499.5, 2499.5, 4999, -2499.5, 2499.5, "Genp Id(inner)", "Genp Id(outer)");
  // Muon Classes (Muon-vs-Muon and Single Muon)
  TH2D* hMuonClasses     = Create2DHistogram<TH2D>("hMuonClasses",     "Classification of selected muons", 17, -8.5, 8.5, 17, -8.5, 8.5, "#mu_{1} class", "#mu_{2} class");
  TH1D* hSingleMuClasses = Create1DHistogram<TH1D>("hSingleMuClasses", "Classification of selected muons", 17, -8.5, 8.5, "#mu class", "N_{#mu}");
  // Muon MC Provenance (Muon-vs-Muon and Single Muon)
  TH2D* hMuonMCProvenance     = Create2DHistogram<TH2D>("hMuonMCProvenance",     "MC provenance of selected muons", 51, -25.5, 25.5, 51, -25.5, 25.5, "#mu_{1} MC prov.", "#mu_{2} MC prov.");
  TH1D* hSingleMuMCProvenance = Create1DHistogram<TH1D>("hSingleMuMCProvenance", "MC provenance of selected muons", 51, -25.5, 25.5, "#mu MC prov.", "N_{#mu}");
  // Muon Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonMergedCode     = Create2DHistogram<TH2D>("hMuonMergedCode",     "Merged MC code of selected muons", 51, -25.5, 25.5, 51, -25.5, 25.5,"#mu_{1} merged code", "#mu_{2} merged code");
  TH1D* hSingleMuMergedCode = Create1DHistogram<TH1D>("hSingleMuMergedCode", "Merged MC code of selected muons", 51, -25.5, 25.5, "#mu merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedIPCode     = Create2DHistogram<TH2D>("hMuonSimplifiedIPCode",     "Simplified MC code of selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedIPCode = Create1DHistogram<TH1D>("hSingleMuSimplifiedIPCode", "Simplified MC code of selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedIPCode     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedIPCode",     "Simplified and merged MC code of selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedIPCode = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedIPCode", "Simplified and merged MC code of selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");

  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedPtRelCode     = Create2DHistogram<TH2D>("hMuonSimplifiedPtRelCode",     "Simplified MC code of selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedPtRelCode = Create1DHistogram<TH1D>("hSingleMuSimplifiedPtRelCode", "Simplified MC code of selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeV0     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeV0",     "Simplified and merged MC code of selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeV0 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeV0", "Simplified and merged MC code of selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeV1     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeV1",     "Simplified and merged MC code of selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeV1 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeV1", "Simplified and merged MC code of selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeV2     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeV2",     "Simplified and merged MC code of selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeV2 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeV2", "Simplified and merged MC code of selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");

  // Particular sub-cases
  // Muon Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonMergedCode02     = Create2DHistogram<TH2D>("hMuonMergedCode02",     "Merged MC code of selected muons with d_{xy} < 0.2 cm", 51, -25.5, 25.5, 51, -25.5, 25.5,"#mu_{1} merged code", "#mu_{2} merged code");
  TH1D* hSingleMuMergedCode02 = Create1DHistogram<TH1D>("hSingleMuMergedCode02", "Merged MC code of selected muons with d_{xy} < 0.2 cm", 51, -25.5, 25.5, "#mu merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedIPCode02     = Create2DHistogram<TH2D>("hMuonSimplifiedIPCode02",     "Simplified MC code of selected muons with d_{xy} < 0.2 cm", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedIPCode02 = Create1DHistogram<TH1D>("hSingleMuSimplifiedIPCode02", "Simplified MC code of selected muons with d_{xy} < 0.2 cm", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedIPCode02     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedIPCode02",     "Simplified and merged MC code of selected muons with d_{xy} < 0.2 cm", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedIPCode02 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedIPCode02", "Simplified and merged MC code of selected muons with d_{xy} < 0.2 cm", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");

  TH2D* hMuonMergedCode0250     = Create2DHistogram<TH2D>("hMuonMergedCode0250",     "Merged MC code of selected muons with d_{xy} < 0.2 cm and p_T^{Out} < 5.0 GeV", 51, -25.5, 25.5, 51, -25.5, 25.5,"#mu_{1} merged code", "#mu_{2} merged code");
  TH1D* hSingleMuMergedCode0250 = Create1DHistogram<TH1D>("hSingleMuMergedCode0250", "Merged MC code of selected muons with d_{xy} < 0.2 cm and p_T^{Out} < 5.0 GeV", 51, -25.5, 25.5, "#mu merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedIPCode0250     = Create2DHistogram<TH2D>("hMuonSimplifiedIPCode0250",     "Simplified MC code of selected muons with d_{xy} < 0.2 cm and p_T^{Out} < 5.0 GeV", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedIPCode0250 = Create1DHistogram<TH1D>("hSingleMuSimplifiedIPCode0250", "Simplified MC code of selected muons with d_{xy} < 0.2 cm and p_T^{Out} < 5.0 GeV", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedIPCode0250     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedIPCode0250",     "Simplified and merged MC code of selected muons with d_{xy} < 0.2 cm and p_T^{Out} < 5.0 GeV", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedIPCode0250 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedIPCode0250", "Simplified and merged MC code of selected muons with d_{xy} < 0.2 cm and p_T^{Out} < 5.0 GeV", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");

  TH2D* hMuonMergedCode50     = Create2DHistogram<TH2D>("hMuonMergedCode50",     "Merged MC code of selected muons with p_T^{Out} < 5.0 GeV", 51, -25.5, 25.5, 51, -25.5, 25.5,"#mu_{1} merged code", "#mu_{2} merged code");
  TH1D* hSingleMuMergedCode50 = Create1DHistogram<TH1D>("hSingleMuMergedCode50", "Merged MC code of selected muons with p_T^{Out} < 5.0 GeV", 51, -25.5, 25.5, "#mu merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedIPCode50     = Create2DHistogram<TH2D>("hMuonSimplifiedIPCode50",     "Simplified MC code of selected muons with p_T^{Out} < 5.0 GeV", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedIPCode50 = Create1DHistogram<TH1D>("hSingleMuSimplifiedIPCode50", "Simplified MC code of selected muons with p_T^{Out} < 5.0 GeV", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedIPCode50     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedIPCode50",     "Simplified and merged MC code of selected muons with p_T^{Out} < 5.0 GeV", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedIPCode50 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedIPCode50", "Simplified and merged MC code of selected muons with p_T^{Out} < 5.0 GeV", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");

  // MG - Commented temporarily waiting to avoid having "unused variable" warnings
  // Muon Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonMergedCodeOS     = Create2DHistogram<TH2D>("hMuonMergedCodeOS",     "Merged MC code of OS selected muons", 51, -25.5, 25.5, 51, -25.5, 25.5,"#mu_{1} merged code", "#mu_{2} merged code");
  TH1D* hSingleMuMergedCodeOS = Create1DHistogram<TH1D>("hSingleMuMergedCodeOS", "Merged MC code of OS selected muons", 51, -25.5, 25.5, "#mu merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedIPCodeOS     = Create2DHistogram<TH2D>("hMuonSimplifiedIPCodeOS",     "Simplified MC code of OS selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedIPCodeOS = Create1DHistogram<TH1D>("hSingleMuSimplifiedIPCodeOS", "Simplified MC code of OS selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedIPCodeOS     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedIPCodeOS",     "Simplified and merged MC code of OS selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedIPCodeOS = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedIPCodeOS", "Simplified and merged MC code of OS selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonMergedCodeSS     = Create2DHistogram<TH2D>("hMuonMergedCodeSS",     "Merged MC code of SS selected muons", 51, -25.5, 25.5, 51, -25.5, 25.5,"#mu_{1} merged code", "#mu_{2} merged code");
  TH1D* hSingleMuMergedCodeSS = Create1DHistogram<TH1D>("hSingleMuMergedCodeSS", "Merged MC code of SS selected muons", 51, -25.5, 25.5, "#mu merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedIPCodeSS     = Create2DHistogram<TH2D>("hMuonSimplifiedIPCodeSS",     "Simplified MC code of SS selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedIPCodeSS = Create1DHistogram<TH1D>("hSingleMuSimplifiedIPCodeSS", "Simplified MC code of SS selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedIPCodeSS     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedIPCodeSS",     "Simplified and merged MC code of SS selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedIPCodeSS = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedIPCodeSS", "Simplified and merged MC code of SS selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonMergedCodeSSPlus     = Create2DHistogram<TH2D>("hMuonMergedCodeSSPlus",     "Merged MC code of SSPlus selected muons", 51, -25.5, 25.5, 51, -25.5, 25.5,"#mu_{1} merged code", "#mu_{2} merged code");
  TH1D* hSingleMuMergedCodeSSPlus = Create1DHistogram<TH1D>("hSingleMuMergedCodeSSPlus", "Merged MC code of SSPlus selected muons", 51, -25.5, 25.5, "#mu merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedIPCodeSSPlus     = Create2DHistogram<TH2D>("hMuonSimplifiedIPCodeSSPlus",     "Simplified MC code of SSPlus selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedIPCodeSSPlus = Create1DHistogram<TH1D>("hSingleMuSimplifiedIPCodeSSPlus", "Simplified MC code of SSPlus selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedIPCodeSSPlus     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedIPCodeSSPlus",     "Simplified and merged MC code of SSPlus selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedIPCodeSSPlus = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedIPCodeSSPlus", "Simplified and merged MC code of SSPlus selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonMergedCodeSSMinus     = Create2DHistogram<TH2D>("hMuonMergedCodeSSMinus",     "Merged MC code of SSMinus selected muons", 51, -25.5, 25.5, 51, -25.5, 25.5,"#mu_{1} merged code", "#mu_{2} merged code");
  TH1D* hSingleMuMergedCodeSSMinus = Create1DHistogram<TH1D>("hSingleMuMergedCodeSSMinus", "Merged MC code of SSMinus selected muons", 51, -25.5, 25.5, "#mu merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedIPCodeSSMinus     = Create2DHistogram<TH2D>("hMuonSimplifiedIPCodeSSMinus",     "Simplified MC code of SSMinus selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedIPCodeSSMinus = Create1DHistogram<TH1D>("hSingleMuSimplifiedIPCodeSSMinus", "Simplified MC code of SSMinus selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedIPCodeSSMinus     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedIPCodeSSMinus",     "Simplified and merged MC code of SSMinus selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedIPCodeSSMinus = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedIPCodeSSMinus", "Simplified and merged MC code of SSMinus selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");

  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedPtRelCodeOS     = Create2DHistogram<TH2D>("hMuonSimplifiedPtRelCodeOS",     "Simplified MC code of OS selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedPtRelCodeOS = Create1DHistogram<TH1D>("hSingleMuSimplifiedPtRelCodeOS", "Simplified MC code of OS selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeOSV0     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeOSV0",     "Simplified and merged MC code of OS selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeOSV0 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeOSV0", "Simplified and merged MC code of OS selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeOSV1     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeOSV1",     "Simplified and merged MC code of OS selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeOSV1 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeOSV1", "Simplified and merged MC code of OS selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeOSV2     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeOSV2",     "Simplified and merged MC code of OS selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeOSV2 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeOSV2", "Simplified and merged MC code of OS selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedPtRelCodeSS     = Create2DHistogram<TH2D>("hMuonSimplifiedPtRelCodeSS",     "Simplified MC code of SS selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedPtRelCodeSS = Create1DHistogram<TH1D>("hSingleMuSimplifiedPtRelCodeSS", "Simplified MC code of SS selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSV0     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSV0",     "Simplified and merged MC code of SS selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSV0 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSV0", "Simplified and merged MC code of SS selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSV1     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSV1",     "Simplified and merged MC code of SS selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSV1 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSV1", "Simplified and merged MC code of SS selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSV2     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSV2",     "Simplified and merged MC code of SS selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSV2 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSV2", "Simplified and merged MC code of SS selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedPtRelCodeSSPlus     = Create2DHistogram<TH2D>("hMuonSimplifiedPtRelCodeSSPlus",     "Simplified MC code of SSPlus selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedPtRelCodeSSPlus = Create1DHistogram<TH1D>("hSingleMuSimplifiedPtRelCodeSSPlus", "Simplified MC code of SSPlus selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSPlusV0     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSPlusV0",     "Simplified and merged MC code of SSPlus selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSPlusV0 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSPlusV0", "Simplified and merged MC code of SSPlus selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSPlusV1     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSPlusV1",     "Simplified and merged MC code of SSPlus selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSPlusV1 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSPlusV1", "Simplified and merged MC code of SSPlus selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSPlusV2     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSPlusV2",     "Simplified and merged MC code of SSPlus selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSPlusV2 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSPlusV2", "Simplified and merged MC code of SSPlus selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedPtRelCodeSSMinus     = Create2DHistogram<TH2D>("hMuonSimplifiedPtRelCodeSSMinus",     "Simplified MC code of SSMinus selected muons", 7, -0.5, 6.5, 7, -0.5, 6.5, "#mu_{1} simpl. code", "#mu_{2} simpl. code");
  TH1D* hSingleMuSimplifiedPtRelCodeSSMinus = Create1DHistogram<TH1D>("hSingleMuSimplifiedPtRelCodeSSMinus", "Simplified MC code of SSMinus selected muons", 7, -0.5, 6.5, "#mu simpl. code", "N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSMinusV0     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSMinusV0",     "Simplified and merged MC code of SSMinus selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSMinusV0 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSMinusV0", "Simplified and merged MC code of SSMinus selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSMinusV1     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSMinusV1",     "Simplified and merged MC code of SSMinus selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSMinusV1 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSMinusV1", "Simplified and merged MC code of SSMinus selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");
  // Muon Simplified Merged Code (Muon-vs-Muon and Single Muon)
  TH2D* hMuonSimplifiedMergedPtRelCodeSSMinusV2     = Create2DHistogram<TH2D>("hMuonSimplifiedMergedPtRelCodeSSMinusV2",     "Simplified and merged MC code of SSMinus selected muons", 6, -0.5, 5.5, 6, -0.5, 5.5, "#mu_{1} simpl. merged code", "#mu_{2} simpl. merged code");
  TH1D* hSingleMuSimplifiedMergedPtRelCodeSSMinusV2 = Create1DHistogram<TH1D>("hSingleMuSimplifiedMergedPtRelCodeSSMinusV2", "Simplified and merged MC code of SSMinus selected muons", 6, -0.5, 5.5, "#mu simpl. merged code","N_{#mu}");

  // Maps for event counts
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSimplifiedMerged;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMerged;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetOSSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedOS;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedOS;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedOS;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMerged;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedSS;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedSS;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedSS;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMerged;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMerged;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMerged;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMerged;

  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSimplifiedMergedV1;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedV1;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedV1;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMergedV1;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMergedV1;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMergedV1;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMergedV1;
    
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSimplifiedMergedV2;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedV2;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedV2;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMergedV2;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMergedV2;
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMergedV2;  
  std::map<std::pair<short int, short int>,TH2I*> mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMergedV2;

  std::stringstream oss;
  std::string name,title;
  
  for(short int i = 0; i < 7; i++)
  {
    for(short int j = i; j < 7; j++)
    {
      std::pair<short int, short int> indexPair = std::make_pair(i,j);
      // Inclusive sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      TH2I* histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max E_{#mu} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSimplifiedMerged[indexPair] = histo2D;
      
      // Opposite sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetOSSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetOSSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetOSSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max E_{#mu} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetOSSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedOS_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedOS " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedOS[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedOS_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedOS " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedOS[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedOS_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedOS " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedOS[indexPair] = histo2D;
      
      // Same sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max E_{#mu} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedSS_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedSS " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedSS[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedSS_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedSS " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedSS[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedSS_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedSS " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedSS[indexPair] = histo2D;
      
      // Same sign plus
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max E_{#mu} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMerged[indexPair] = histo2D;
      
      // Same sign minus
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMerged[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMerged_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max E_{#mu} for SimplifiedMerged " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMerged[indexPair] = histo2D;
    }
  }

  
  for(short int i = 0; i < 4; i++)
  {
    for(short int j = i; j < 4; j++)
    {
      std::pair<short int, short int> indexPair = std::make_pair(i,j);
      // Inclusive sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      TH2I* histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSimplifiedMergedV1[indexPair] = histo2D;
      
      // Opposite sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedV1[indexPair] = histo2D;
      
      // Same sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedV1[indexPair] = histo2D;
      
      // Same sign plus
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMergedV1[indexPair] = histo2D;
      
      // Same sign minus
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMergedV1[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMergedV1_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV1 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMergedV1[indexPair] = histo2D;
    }
  }
  

  for(short int i = 0; i < 5; i++)
  {
    for(short int j = i; j < 5; j++)
    {
      std::pair<short int, short int> indexPair = std::make_pair(i,j);
      // Inclusive sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      TH2I* histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSimplifiedMergedV2[indexPair] = histo2D;
      
      // Opposite sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected OS events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedV2[indexPair] = histo2D;
      
      // Same sign
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SS events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedV2[indexPair] = histo2D;
      
      // Same sign plus
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSPlus events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMergedV2[indexPair] = histo2D;
      
      // Same sign minus
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max p_{T}^{Rel} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Rel} [GeV/c]");
      mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max p_{T}^{Out} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 50, 0., 5., "d_{xy} [cm]", "p_{T}^{Out} [GeV/c]");
      mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMergedV2[indexPair] = histo2D;
      
      oss.str("");    oss << "hNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMergedV2_" << i << "_" << j;    name = oss.str();
      oss.str("");    oss << "Number of selected SSMinus events vs. max d_{xy} and max E_{#mu} for SimplifiedMergedV2 " << i << " - " << j;    title = oss.str();
      histo2D = Create2DHistogram<TH2I>(name.c_str(),title.c_str(), 50, 0., 0.5, 60, 0., 6., "d_{xy} [cm]", "E_{#mu} [GeV]");
      mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMergedV2[indexPair] = histo2D;
    }
  }
  
  // Maps for IP templates
  std::map<short int,TH1D*> mMuonDxyzTemplates;
  std::map<short int,TH1D*> mMuonDxyTemplates;
  std::map<short int,TH1D*> mMuonDzTemplates;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDzSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedMergedTemplates;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates;
  std::map<short int,TH1D*> mMuonDzSimplifiedMergedTemplates;
  
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedOSTemplates;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedSSTemplates;
  
  std::map<short int,TH2D*> mMuonDxySimplifiedMerged2DTemplates;
  std::map<short int,TH2D*> mMuonDxySimplifiedMerged2DTemplatesOS;
  std::map<short int,TH2D*> mMuonDxySimplifiedMerged2DTemplatesSS;
  std::map<std::pair<short int, short int>,TH1D*> mMuonDxySimplifiedMergedTemplatesByEventCode;
  std::map<std::pair<short int, short int>,TH1D*> mMuonDxySimplifiedMergedTemplatesOSByEventCode;
  std::map<std::pair<short int, short int>,TH1D*> mMuonDxySimplifiedMergedTemplatesSSByEventCode;
  
  // Particular sub-cases
  // Shifts
  std::map<short int,TH1D*> mMuonDxyzTemplates095;
  std::map<short int,TH1D*> mMuonDxyTemplates095;
  std::map<short int,TH1D*> mMuonDzTemplates095;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedTemplates095;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates095;
  std::map<short int,TH1D*> mMuonDzSimplifiedTemplates095;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedMergedTemplates095;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates095;
  std::map<short int,TH1D*> mMuonDzSimplifiedMergedTemplates095;
  std::map<short int,TH1D*> mMuonDxyzTemplates099;
  std::map<short int,TH1D*> mMuonDxyTemplates099;
  std::map<short int,TH1D*> mMuonDzTemplates099;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedTemplates099;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates099;
  std::map<short int,TH1D*> mMuonDzSimplifiedTemplates099;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedMergedTemplates099;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates099;
  std::map<short int,TH1D*> mMuonDzSimplifiedMergedTemplates099;
  std::map<short int,TH1D*> mMuonDxyzTemplates101;
  std::map<short int,TH1D*> mMuonDxyTemplates101;
  std::map<short int,TH1D*> mMuonDzTemplates101;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedTemplates101;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates101;
  std::map<short int,TH1D*> mMuonDzSimplifiedTemplates101;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedMergedTemplates101;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates101;
  std::map<short int,TH1D*> mMuonDzSimplifiedMergedTemplates101;
  std::map<short int,TH1D*> mMuonDxyzTemplates105;
  std::map<short int,TH1D*> mMuonDxyTemplates105;
  std::map<short int,TH1D*> mMuonDzTemplates105;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedTemplates105;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates105;
  std::map<short int,TH1D*> mMuonDzSimplifiedTemplates105;
  std::map<short int,TH1D*> mMuonDxyzSimplifiedMergedTemplates105;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates105;
  std::map<short int,TH1D*> mMuonDzSimplifiedMergedTemplates105;

  // Particular sub-cases for Dxy
  // D6T
  //std::map<short int,TH1D*> mMuonDxySimplifiedTemplates_D6TCor;
  //std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates_D6TCor;
  // IP Smearing
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates_1um;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates_3um;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates_5um;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates_8um;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates_10um;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates_12um;
  std::map<short int,TH1D*> mMuonDxySimplifiedTemplates_20um;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates_1um;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates_3um;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates_5um;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates_8um;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates_10um;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates_12um;
  std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplates_20um;

  // Now create the histograms

  // MC provenance  
  for(short int i = -25; i < 26; i++)
  {
    oss.str("");    oss << "hMuonDxyzTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzTemplates[i] = histoDxyz;

    oss.str("");    oss << "hMuonDxyTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyTemplates[i] = histoDxy;

    oss.str("");    oss << "hMuonDzTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzTemplates[i] = histoDz;

    oss.str("");    oss << "hMuonDxyzTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzTemplates095[i] = histoDxyz095;

    oss.str("");    oss << "hMuonDxyTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDxy095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyTemplates095[i] = histoDxy095;

    oss.str("");    oss << "hMuonDzTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDz095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzTemplates095[i] = histoDz095;

    oss.str("");    oss << "hMuonDxyzTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 0.99 for muon category " << i;
    title = oss.str();
    TH1D* histoDxyz099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzTemplates099[i] = histoDxyz099;

    oss.str("");    oss << "hMuonDxyTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 0.99 for muon category " << i;    title = oss.str();
    TH1D* histoDxy099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyTemplates099[i] = histoDxy099;

    oss.str("");    oss << "hMuonDzTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 0.99 for muon category " << i;    title = oss.str();
    TH1D* histoDz099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzTemplates099[i] = histoDz099;

    oss.str("");    oss << "hMuonDxyzTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzTemplates101[i] = histoDxyz101;

    oss.str("");    oss << "hMuonDxyTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDxy101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyTemplates101[i] = histoDxy101;

    oss.str("");    oss << "hMuonDzTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDz101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzTemplates101[i] = histoDz101;

    oss.str("");    oss << "hMuonDxyzTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzTemplates105[i] = histoDxyz105;

    oss.str("");    oss << "hMuonDxyTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDxy105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyTemplates105[i] = histoDxy105;

    oss.str("");    oss << "hMuonDzTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDz105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzTemplates105[i] = histoDz105;
  }

  // MC simplified code
  for(short int i = 0; i < 7; i++)
  {
    oss.str("");    oss << "hMuonDxyzSimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedTemplates[i] = histoDxyz;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates[i] = histoDxy;

    oss.str("");    oss << "hMuonDzSimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedTemplates[i] = histoDz;

    oss.str("");    oss << "hMuonDxyzSimplifiedTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedTemplates095[i] = histoDxyz095;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDxy095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates095[i] = histoDxy095;

    oss.str("");    oss << "hMuonDzSimplifiedTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDz095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedTemplates095[i] = histoDz095;

    oss.str("");    oss << "hMuonDxyzSimplifiedTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 0.99 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedTemplates099[i] = histoDxyz099;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 0.99 for muon category " << i;    title = oss.str();
    TH1D* histoDxy099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates099[i] = histoDxy099;

    oss.str("");    oss << "hMuonDzSimplifiedTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 0.99 for muon category " << i;    title = oss.str();
    TH1D* histoDz099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedTemplates099[i] = histoDz099;

    oss.str("");    oss << "hMuonDxyzSimplifiedTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedTemplates101[i] = histoDxyz101;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDxy101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates101[i] = histoDxy101;

    oss.str("");    oss << "hMuonDzSimplifiedTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDz101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedTemplates101[i] = histoDz101;

    oss.str("");    oss << "hMuonDxyzSimplifiedTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedTemplates105[i] = histoDxyz105;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDxy105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates105[i] = histoDxy105;

    oss.str("");    oss << "hMuonDzSimplifiedTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDz105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedTemplates105[i] = histoDz105;

//    oss.str("");     oss << "hMuonDxySimplifiedTemplate_D6TCor_" << i;     name = oss.str();
//    oss.str("");     oss << "d_{xy} template (corr. for D6T/Z2) for muon category " << i;     title = oss.str();
//    histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy}","N_{#mu}");
//    mMuonDxySimplifiedTemplates_D6TCor[i] = histoDxy;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_1um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+1 #mu m smearing) for muon category " << i;    title = oss.str();
    histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates_1um[i] = histoDxy;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_3um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+3 #mu m smearing) for muon category " << i;    title = oss.str();
    histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates_3um[i] = histoDxy;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_5um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+5 #mu m smearing) for muon category " << i;    title = oss.str();
    histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates_5um[i] = histoDxy;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_8um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+8 #mu m smearing) for muon category " << i;    title = oss.str();
    histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates_8um[i] = histoDxy;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_10um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+10 #mu m smearing) for muon category " << i;    title = oss.str();
    histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates_10um[i] = histoDxy;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_12um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+12 #mu m smearing) for muon category " << i;    title = oss.str();
    histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates_12um[i] = histoDxy;

    oss.str("");    oss << "hMuonDxySimplifiedTemplate_20um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+20 #mu m smearing) for muon category " << i;    title = oss.str();
    histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedTemplates_20um[i] = histoDxy;
  }

  for(short int i = 0; i < 8; i++)
  {
    oss.str("");    oss << "hMuonDxySimplifiedMergedOSTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} OS template for muon category " << i;    title = oss.str();
    TH1D* histoDxyOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedOSTemplates[i] = histoDxyOS;
    
    oss.str("");    oss << "hMuonDxySimplifiedMergedSSTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} SS template for muon category " << i;    title = oss.str();
    TH1D* histoDxySS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedSSTemplates[i] = histoDxySS;
  }
  
  // MC simplified merged code
  for(short int i = 0; i < 7; i++)
  {
    oss.str("");    oss << "hMuonDxyzSimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedMergedTemplates[i] = histoDxyz;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates[i] = histoDxy;

    oss.str("");    oss << "hMuonDzSimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedMergedTemplates[i] = histoDz;

    for(short int j = i; j < 7; j++)
    {
      short int index = j<=i?10*j+i:10*i+j;

      oss.str("");    oss << "hMuonDxySimplifiedMerged2DTemplate_" << index;    name=oss.str();
      oss.str("");    oss << "2D d_{xy} template for event category " << index;    title=oss.str();
      TH2D* histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
      mMuonDxySimplifiedMerged2DTemplates[index] = histo2D;

      oss.str("");    oss << "hMuonDxySimplifiedMerged2DTemplateOS_" << index;    name=oss.str();
      oss.str("");    oss << "2D d_{xy} OS template for event category " << index;    title=oss.str();
      histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
      mMuonDxySimplifiedMerged2DTemplatesOS[index] = histo2D;
      
      oss.str("");    oss << "hMuonDxySimplifiedMerged2DTemplateSS_" << index;    name=oss.str();
      oss.str("");    oss << "2D d_{xy} SS template for event category " << index;    title=oss.str();
      histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
      mMuonDxySimplifiedMerged2DTemplatesSS[index] = histo2D;

      if(i==j)
      {
        std::pair<short int, short int> indexPair = std::make_pair(i,index);
        
        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateByEventCode_" << i << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} template for muon category " << i << " and event category " << index;    title=oss.str();
        TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesByEventCode[indexPair] = histo;

        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateOSByEventCode_" << i << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} OS template for muon category " << i << " and event category " << index;    title=oss.str();
        histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesOSByEventCode[indexPair] = histo;
        
        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateSSByEventCode_" << i << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} SS template for muon category " << i << " and event category " << index;    title=oss.str();
        histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesSSByEventCode[indexPair] = histo;
      }
      else
      {
        std::pair<short int, short int> indexPair = std::make_pair(i,index);
        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateByEventCode_" << i << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} template for muon category " << i << " and event category " << index;    title=oss.str();
        TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesByEventCode[indexPair] = histo;
        
        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateOSByEventCode_" << i << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} OS template for muon category " << i << " and event category " << index;    title=oss.str();
        histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesOSByEventCode[indexPair] = histo;
        
        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateSSByEventCode_" << i << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} SS template for muon category " << i << " and event category " << index;    title=oss.str();
        histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesSSByEventCode[indexPair] = histo;
        
        indexPair = std::make_pair(j,index);
        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateByEventCode_" << j << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} template for muon category " << j << " and event category " << index;    title=oss.str();
        histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesByEventCode[indexPair] = histo;

        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateOSByEventCode_" << j << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} OS template for muon category " << j << " and event category " << index;    title=oss.str();
        histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesOSByEventCode[indexPair] = histo;
        
        oss.str("");    oss << "hMuonDxySimplifiedMergedTemplateSSByEventCode_" << j << "_" << index;    name=oss.str();
        oss.str("");    oss << "d_{xy} SS template for muon category " << j << " and event category " << index;    title=oss.str();
        histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5,"d_{xy} [cm]", "N_{#mu}");
        mMuonDxySimplifiedMergedTemplatesSSByEventCode[indexPair] = histo;
      }
    }

    oss.str("");    oss << "hMuonDxyzSimplifiedMergedTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedMergedTemplates095[i] = histoDxyz095;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDxy095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates095[i] = histoDxy095;

    oss.str("");    oss << "hMuonDzSimplifiedMergedTemplate_095_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 0.95 for muon category " << i;    title = oss.str();
    TH1D* histoDz095 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedMergedTemplates095[i] = histoDz095;

    oss.str("");    oss << "hMuonDxyzSimplifiedMergedTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 0.99 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedMergedTemplates099[i] = histoDxyz099;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 0.99 for muon category " << i;    title = oss.str();
    TH1D* histoDxy099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates099[i] = histoDxy099;

    oss.str("");    oss << "hMuonDzSimplifiedMergedTemplate_099_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 0.99 for muon category " << i;    title = oss.str();
    TH1D* histoDz099 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedMergedTemplates099[i] = histoDz099;

    oss.str("");    oss << "hMuonDxyzSimplifiedMergedTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedMergedTemplates101[i] = histoDxyz101;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDxy101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates101[i] = histoDxy101;

    oss.str("");    oss << "hMuonDzSimplifiedMergedTemplate_101_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 1.01 for muon category " << i;    title = oss.str();
    TH1D* histoDz101 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedMergedTemplates101[i] = histoDz101;

    oss.str("");    oss << "hMuonDxyzSimplifiedMergedTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDxyz105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzSimplifiedMergedTemplates105[i] = histoDxyz105;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDxy105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates105[i] = histoDxy105;

    oss.str("");    oss << "hMuonDzSimplifiedMergedTemplate_105_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template with a shift 1.05 for muon category " << i;    title = oss.str();
    TH1D* histoDz105 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzSimplifiedMergedTemplates105[i] = histoDz105;

//     oss.str("");     oss << "hMuonDxySimplifiedMergedTemplate_D6TCor_" << i;     name = oss.str();
//     oss.str("");     oss << "d_{xy} template (corr. for D6T/Z2) for muon category " << i;    title = oss.str();
//     histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy}","N_{#mu}");
//     mMuonDxySimplifiedMergedTemplates_D6TCor[i] = histoDxy;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_1um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+1 #mu m smearing) for muon category " << i;    title = oss.str();
    TH1D* histoDxy1um = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates_1um[i] = histoDxy1um;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_3um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+3 #mu m smearing) for muon category " << i;    title = oss.str();
    TH1D* histoDxy3um = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates_3um[i] = histoDxy3um;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_5um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+5 #mu m smearing) for muon category " << i;    title = oss.str();
    TH1D* histoDxy5um = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates_5um[i] = histoDxy5um;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_8um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+8 #mu m smearing) for muon category " << i;    title = oss.str();
    TH1D* histoDxy8um = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates_8um[i] = histoDxy8um;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_10um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+10 #mu m smearing) for muon category " << i;    title = oss.str();
    TH1D* histoDxy10um = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates_10um[i] = histoDxy10um;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_12um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+12 #mu m smearing) for muon category " << i;    title = oss.str();
    TH1D* histoDxy12um = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates_12um[i] = histoDxy12um;

    oss.str("");    oss << "hMuonDxySimplifiedMergedTemplate_20um_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template (+20 #mu m smearing) for muon category " << i;    title = oss.str();
    TH1D* histoDxy20um = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxySimplifiedMergedTemplates_20um[i] = histoDxy20um;
  }

  std::map<short int,TH1D*> mMuonDxyzPlusTemplates;
  std::map<short int,TH1D*> mMuonDxyPlusTemplates;
  std::map<short int,TH1D*> mMuonDzPlusTemplates;
  std::map<short int,TH1D*> mMuonDxyzPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDxyPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDzPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDxyzPlusSimplifiedMergedTemplates;
  std::map<short int,TH1D*> mMuonDxyPlusSimplifiedMergedTemplates;
  std::map<short int,TH1D*> mMuonDzPlusSimplifiedMergedTemplates;

  std::map<short int,TH1D*> mMuonDxyzMinusTemplates;
  std::map<short int,TH1D*> mMuonDxyMinusTemplates;
  std::map<short int,TH1D*> mMuonDzMinusTemplates;
  std::map<short int,TH1D*> mMuonDxyzMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDxyMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDzMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDxyzMinusSimplifiedMergedTemplates;
  std::map<short int,TH1D*> mMuonDxyMinusSimplifiedMergedTemplates;
  std::map<short int,TH1D*> mMuonDzMinusSimplifiedMergedTemplates;

  // MC provenance
  for(short int i = -25; i < 26; i++)
  {
    oss.str("");    oss << "hMuonDxyzPlusTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzPlusTemplates[i] = histoDxyz;

    oss.str("");    oss << "hMuonDxyPlusTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyPlusTemplates[i] = histoDxy;

    oss.str("");    oss << "hMuonDzPlusTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzPlusTemplates[i] = histoDz;

    oss.str("");    oss << "hMuonDxyzMinusTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzMinusTemplates[i] = histoDxyz0;

    oss.str("");    oss << "hMuonDxyMinusTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyMinusTemplates[i] = histoDxy0;

    oss.str("");    oss << "hMuonDzMinusTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzMinusTemplates[i] = histoDz0;
  }

  // MC simplified code
  for(short int i = 0; i < 7; i++)
  {
    oss.str("");    oss << "hMuonDxyzPlusSimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzPlusSimplifiedTemplates[i] = histoDxyz;

    oss.str("");    oss << "hMuonDxyPlusSimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyPlusSimplifiedTemplates[i] = histoDxy;

    oss.str("");    oss << "hMuonDzPlusSimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzPlusSimplifiedTemplates[i] = histoDz;

    oss.str("");    oss << "hMuonDxyzMinusSimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzMinusSimplifiedTemplates[i] = histoDxyz0;

    oss.str("");    oss << "hMuonDxyMinusSimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyMinusSimplifiedTemplates[i] = histoDxy0;

    oss.str("");    oss << "hMuonDzMinusSimplifiedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzMinusSimplifiedTemplates[i] = histoDz0;
  }

  // MC simplified merged code
  for(short int i = 0; i < 7; i++)
  {
    oss.str("");    oss << "hMuonDxyzPlusSimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzPlusSimplifiedMergedTemplates[i] = histoDxyz;

    oss.str("");    oss << "hMuonDxyPlusSimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyPlusSimplifiedMergedTemplates[i] = histoDxy;

    oss.str("");    oss << "hMuonDzPlusSimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzPlusSimplifiedMergedTemplates[i] = histoDz;

    oss.str("");    oss << "hMuonDxyzMinusSimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xyz} template for muon category " << i;    title = oss.str();
    TH1D* histoDxyz0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    mMuonDxyzMinusSimplifiedMergedTemplates[i] = histoDxyz0;

    oss.str("");    oss << "hMuonDxyMinusSimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{xy} template for muon category " << i;    title = oss.str();
    TH1D* histoDxy0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    mMuonDxyMinusSimplifiedMergedTemplates[i] = histoDxy0;

    oss.str("");    oss << "hMuonDzMinusSimplifiedMergedTemplate_" << i;    name = oss.str();
    oss.str("");    oss << "d_{z} template for muon category " << i;    title = oss.str();
    TH1D* histoDz0 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
    mMuonDzMinusSimplifiedMergedTemplates[i] = histoDz0;
  }

  // Vectors of vectors of maps of IP Templates
  std::vector<std::vector<std::map<short int,TH1D*> > > vvmMuonDxyzSimplifiedMergedTemplates;
  std::vector<std::vector<std::map<short int,TH1D*> > > vvmMuonDxySimplifiedMergedTemplates;
  std::vector<std::vector<std::map<short int,TH1D*> > > vvmMuonDzSimplifiedMergedTemplates;

  // IP Templates for DIF categories
  TH1D* hMuonDxyDIFFromHFPionTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFFromHFPionTemplate", "d_{xy} template for DIF from heavy flavour pion", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIFFromLFPionTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFFromLFPionTemplate", "d_{xy} template for DIF from light flavour pion", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIFFromHFKaonTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFFromHFKaonTemplate", "d_{xy} template for DIF from heavy flavour kaon", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIFFromLFKaonTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFFromLFKaonTemplate", "d_{xy} template for DIF from light flavour kaon", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIFFromHFTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFFromHFTemplate", "d_{xy} template for DIF from heavy flavour", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIFFromLFTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFFromLFTemplate", "d_{xy} template for DIF from light flavour", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIFFromPionTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFFromPionTemplate", "d_{xy} template for DIF from pion", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIFFromKaonTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFFromKaonTemplate", "d_{xy} template for DIF from kaon", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  // Modified templates with changed fractions
  TH1D* hMuonDxyDIFHalfHFTemplate  = Create1DHistogram<TH1D>("hMuonDxyDIFHalfHFTemplate",  "d_{xy} template with -50% heavy flavour", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIFTwiceHFTemplate = Create1DHistogram<TH1D>("hMuonDxyDIFTwiceHFTemplate", "d_{xy} template with +100% heavy flavour", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIF30MorePionTemplate = Create1DHistogram<TH1D>("hMuonDxyDIF30MorePionTemplate", "d_{xy} template for DIF with +30% pion", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIF30LessPionTemplate = Create1DHistogram<TH1D>("hMuonDxyDIF30LessPionTemplate", "d_{xy} template for DIF with -30% pion", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIF30MoreKaonTemplate = Create1DHistogram<TH1D>("hMuonDxyDIF30MoreKaonTemplate", "d_{xy} template for DIF with +30% kaon", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyDIF30LessKaonTemplate = Create1DHistogram<TH1D>("hMuonDxyDIF30LessKaonTemplate", "d_{xy} template for DIF with -30% kaon", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");

  // C+DIF+Fake+Prompt+PT ptOut templates with modified mixtures
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesCp25_3= Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesCp25_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with +25% C",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesCl25_3 = Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesCl25_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with -25% C",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesCp30_3= Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesCp30_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with +30% C",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesCl30_3 = Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesCl30_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with -30% C",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesCp50_3= Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesCp50_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with +50% C",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesCl50_3 = Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesCl50_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with -50% C",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesCp75_3= Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesCp75_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with +75% C",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesCl75_3 = Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesCl75_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with -75% C",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");

  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp25_3= Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp25_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with +25% DIF",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl25_3 = Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl25_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with -25% DIF",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp30_3= Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp30_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with +30% DIF",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl30_3 = Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl30_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with -30% DIF",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp50_3= Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp50_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with +50% DIF",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl50_3 = Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl50_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with -50% DIF",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp75_3= Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp75_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with +75% DIF",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");
  TH1D* hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl75_3 = Create1DHistogram<TH1D>("hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl75_3", "p_{T}^{Out} template for C+DIF+Fake+Prompt+PT with -75% DIF",500, 0.,5.,"p_{T}^{Out} [GeV]","N_{#mu}");

  // Pt6
  TH1D* hMuonDxyDIFPt4RescaledToPt6Template = Create1DHistogram<TH1D>("hMuonDxyDIFPt4RescaledToPt6Template", "d_{xy} template for DIF  - Pt 4 rescaled to Pt 6 fractions", 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");

  // Maps for PtRel templates
  std::map<short int,TH1D*> mMuonPtRelJetTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVTemplates;
  std::map<short int,TH1D*> mMuonDRPtRelTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetPlusTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetMinusTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVPlusTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVMinusTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetPlusTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetMinusTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVPlusTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVMinusTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVPlusTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVMinusTemplates;

  std::map<short int,TH1D*> mMuonPtRelGenJetTemplates;
  std::map<short int,TH1D*> mMuonDRPtRelGenTemplates;
  std::map<short int,TH1D*> mMuonPtRelGenJetPlusTemplates;
  std::map<short int,TH1D*> mMuonPtRelGenJetMinusTemplates;

  std::map<short int,TH1D*> mMuonPtRelJetSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDRPtRelSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVMinusSimplifiedTemplates;
  
  std::map<short int,TH1D*> mMuonPtRelGenJetSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDRPtRelGenSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelGenJetPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelGenJetMinusSimplifiedTemplates;
  
  std::map<short int,TH1D*> mMuonPtRelJetSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelSVSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonDRPtRelSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelSVPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelSVMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVMinusSimplifiedMergedV0Templates;
  
  std::map<short int,TH1D*> mMuonPtRelGenJetSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonDRPtRelGenSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetMinusSimplifiedMergedV0Templates;
  
  std::map<short int,TH1D*> mMuonPtRelJetSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelSVSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonDRPtRelSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelSVPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelSVMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVMinusSimplifiedMergedV1Templates;
  
  std::map<short int,TH1D*> mMuonPtRelGenJetSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonDRPtRelGenSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetMinusSimplifiedMergedV1Templates;
  
  std::map<short int,TH1D*> mMuonPtRelJetSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelSVSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonDRPtRelSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetMinusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelSVPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelSVMinusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelSVorJetMinusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetNoSVMinusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetYesSVMinusSimplifiedMergedV2Templates;

  std::map<short int,TH1D*> mMuonPtRelGenJetSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonDRPtRelGenSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetMinusSimplifiedMergedV2Templates;
  
  std::map<short int,TH1D*> mMuonPtRelJetOSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetSSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetSSPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetSSMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelJetOSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelJetOSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelJetOSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelJetSSMinusSimplifiedMergedV2Templates;
  
  std::map<short int,TH1D*> mMuonPtRelGenJetOSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtRelGenJetOSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetOSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetOSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtRelGenJetSSMinusSimplifiedMergedV2Templates;

  std::map<short int,TH1D*> mMuonPtRelSmearedJetTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVSimplifiedTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVSimplifiedMergedV0Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVSimplifiedMergedV1Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVSimplifiedMergedV2Templates_SV;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusSimplifiedTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusSimplifiedMergedV0Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusSimplifiedMergedV1Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusSimplifiedMergedV2Templates_SV;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusSimplifiedTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusSimplifiedMergedV0Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusSimplifiedMergedV1Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusSimplifiedMergedV2Templates_SV;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtRelSmearedJetTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVSimplifiedTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVSimplifiedMergedV0Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVSimplifiedMergedV1Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVSimplifiedMergedV2Templates_NO;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusSimplifiedTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusSimplifiedMergedV0Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusSimplifiedMergedV1Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetPlusSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVPlusSimplifiedMergedV2Templates_NO;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusSimplifiedTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusSimplifiedMergedV0Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusSimplifiedMergedV1Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtRelSmearedJetMinusSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtRelSmearedSVMinusSimplifiedMergedV2Templates_NO;  // Not smeared at the moment

  // Maps for PtOut templates
  std::map<short int,TH1D*> mMuonPtOutJetTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVTemplates;
  std::map<short int,TH1D*> mMuonDRPtOutTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetPlusTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetMinusTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVPlusTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVMinusTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetPlusTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetMinusTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVPlusTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVMinusTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVPlusTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVMinusTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplates;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetTemplates;
  std::map<short int,TH1D*> mMuonDRPtOutGenTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetPlusTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetMinusTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplates;
  
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesM0406;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesM0608;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesM0810;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesM1012;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesM1215;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesM1520;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesM20xx;

  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesJ1015;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesJ1520;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesJ2030;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesJ30xx;

  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesM0406;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesM0608;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesM0810;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesM1012;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesM1215;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesM1520;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesM20xx;

  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesJ1015;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesJ1520;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesJ2030;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0TemplatesJ30xx;

  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesM0406;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesM0608;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesM0810;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesM1012;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesM1215;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesM1520;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesM20xx;

  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesJ1015;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesJ1520;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesJ2030;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesJ30xx;

  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesM0406;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesM0608;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesM0810;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesM1012;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesM1215;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesM1520;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesM20xx;

  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesJ1015;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesJ1520;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesJ2030;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesJ30xx;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesM0406;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesM0608;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesM0810;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesM1012;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesM1215;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesM1520;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesM20xx;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesJ1015;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesJ1520;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesJ2030;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesJ30xx;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0406;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0608;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0810;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1012;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1215;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1520;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesM20xx;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ1015;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ1520;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ2030;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ30xx;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0406;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0608;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0810;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1012;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1215;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1520;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesM20xx;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ1015;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ1520;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ2030;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ30xx;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0406;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0608;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0810;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1012;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1215;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1520;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesM20xx;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ1015;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ1520;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ2030;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ30xx;

  // Unused maps...
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesM0406;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesM0608;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesM0810;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesM1012;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesM1215;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesM1520;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesM20xx;
  //
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesJ1015;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesJ1520;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesJ2030;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedTemplatesJ30xx;
  //
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesM0406;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesM0608;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesM0810;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesM1012;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesM1215;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesM1520;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesM20xx;
  //
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesJ1015;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesJ1520;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesJ2030;
  //   std::map<short int,TH1D*> mMuonDxySimplifiedMergedTemplatesJ30xx;
  //

  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesD0025;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesD2550;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesD5075;
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedTemplatesD7500;

  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesD0025;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesD2550;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesD5075;
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedTemplatesD7500;
  
  std::map<short int,TH1D*> mMuonPtOutSVSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonDRPtOutSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVMinusSimplifiedTemplates;
  
  std::map<short int,TH1D*> mMuonDRPtOutGenSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetMinusSimplifiedTemplates;
  
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutSVSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonDRPtOutSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutSVPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutSVMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVMinusSimplifiedMergedV0Templates;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonDRPtOutGenSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetMinusSimplifiedMergedV0Templates;
  
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutSVSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonDRPtOutSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutSVPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutSVMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVMinusSimplifiedMergedV1Templates;
  
  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonDRPtOutGenSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetMinusSimplifiedMergedV1Templates;
  
  std::map<short int,TH2D*> mMuonPtOutJetSimplifiedMergedV12DTemplates;
  std::map<short int,TH2D*> mMuonPtOutJetSimplifiedMergedV12DTemplatesOS;
  std::map<short int,TH2D*> mMuonPtOutJetSimplifiedMergedV12DTemplatesSS;
  std::map<std::pair<short int, short int>,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesByEventCode;
  std::map<std::pair<short int, short int>,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesOSByEventCode;
  std::map<std::pair<short int, short int>,TH1D*> mMuonPtOutJetSimplifiedMergedV1TemplatesSSByEventCode;
  
  std::map<short int,TH1D*> mMuonPtOutJetSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutSVSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonDRPtOutSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetMinusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutSVPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutSVMinusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutSVorJetMinusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetNoSVMinusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetYesSVMinusSimplifiedMergedV2Templates;

  std::map<short int,TH1D*> mMuonPtOutGenJetSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonDRPtOutGenSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetMinusSimplifiedMergedV2Templates;

  std::map<short int,TH2D*> mMuonPtOutJetSimplifiedMergedV22DTemplates;
  std::map<short int,TH2D*> mMuonPtOutJetSimplifiedMergedV22DTemplatesOS;
  std::map<short int,TH2D*> mMuonPtOutJetSimplifiedMergedV22DTemplatesSS;
  std::map<std::pair<short int, short int>,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesByEventCode;
  std::map<std::pair<short int, short int>,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesOSByEventCode;
  std::map<std::pair<short int, short int>,TH1D*> mMuonPtOutJetSimplifiedMergedV2TemplatesSSByEventCode;
  
  std::map<short int,TH1D*> mMuonPtOutJetOSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetSSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetSSPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetSSMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutJetOSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutJetOSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutJetOSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutJetSSMinusSimplifiedMergedV2Templates;

  std::map<short int,TH1D*> mMuonPtOutGenJetOSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSMinusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonPtOutGenJetOSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSMinusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetOSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSMinusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetOSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonPtOutGenJetSSMinusSimplifiedMergedV2Templates;
  
  std::map<short int,TH1D*> mMuonPtOutSmearedJetTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVSimplifiedTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVSimplifiedMergedV0Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVSimplifiedMergedV1Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVSimplifiedMergedV2Templates_SV;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusSimplifiedTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusSimplifiedMergedV0Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusSimplifiedMergedV1Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusSimplifiedMergedV2Templates_SV;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusSimplifiedTemplates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusSimplifiedMergedV0Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusSimplifiedMergedV1Templates_SV;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV2Templates_SV;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusSimplifiedMergedV2Templates_SV;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtOutSmearedJetTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVSimplifiedTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVSimplifiedMergedV0Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVSimplifiedMergedV1Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVSimplifiedMergedV2Templates_NO;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusSimplifiedTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusSimplifiedMergedV0Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusSimplifiedMergedV1Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetPlusSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVPlusSimplifiedMergedV2Templates_NO;  // Not smeared at the moment

  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusSimplifiedTemplates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusSimplifiedMergedV0Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusSimplifiedMergedV1Templates_NO;  // Not smeared at the moment
  std::map<short int,TH1D*> mMuonPtOutSmearedJetMinusSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV2Templates_NO;
  std::map<short int,TH1D*> mMuonPtOutSmearedSVMinusSimplifiedMergedV2Templates_NO;  // Not smeared at the moment

  // Maps for Emu templates
  std::map<short int,TH1D*> mMuonEmuJetTemplates;
  std::map<short int,TH1D*> mMuonEmuJetPlusTemplates;
  std::map<short int,TH1D*> mMuonEmuJetMinusTemplates;
  
  std::map<short int,TH1D*> mMuonEmuJetSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonEmuJetPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonEmuJetMinusSimplifiedTemplates;
  
  std::map<short int,TH1D*> mMuonEmuJetSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonEmuJetPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonEmuJetMinusSimplifiedMergedV0Templates;
  
  std::map<short int,TH1D*> mMuonEmuJetSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonEmuJetPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonEmuJetMinusSimplifiedMergedV1Templates;
  
  std::map<short int,TH1D*> mMuonEmuJetSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonEmuJetPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonEmuJetMinusSimplifiedMergedV2Templates;
  
  std::map<short int,TH1D*> mMuonEmuJetOSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonEmuJetSSSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonEmuJetSSPlusSimplifiedTemplates;
  std::map<short int,TH1D*> mMuonEmuJetSSMinusSimplifiedTemplates;
  
  std::map<short int,TH1D*> mMuonEmuJetOSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSPlusSimplifiedMergedV0Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSMinusSimplifiedMergedV0Templates;
  
  std::map<short int,TH1D*> mMuonEmuJetOSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSPlusSimplifiedMergedV1Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSMinusSimplifiedMergedV1Templates;
  
  std::map<short int,TH1D*> mMuonEmuJetOSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSPlusSimplifiedMergedV2Templates;
  std::map<short int,TH1D*> mMuonEmuJetSSMinusSimplifiedMergedV2Templates;
  
  std::map<short int,TH1D*> mMuonEmuSmearedJetTemplates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetSimplifiedMergedV2Templates_SV;
  
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusTemplates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusSimplifiedMergedV2Templates_SV;
  
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusTemplates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusSimplifiedTemplates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusSimplifiedMergedV0Templates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusSimplifiedMergedV1Templates_SV;
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusSimplifiedMergedV2Templates_SV;
  
  std::map<short int,TH1D*> mMuonEmuSmearedJetTemplates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetSimplifiedMergedV2Templates_NO;
  
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusTemplates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetPlusSimplifiedMergedV2Templates_NO;
  
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusTemplates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusSimplifiedTemplates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusSimplifiedMergedV0Templates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusSimplifiedMergedV1Templates_NO;
  std::map<short int,TH1D*> mMuonEmuSmearedJetMinusSimplifiedMergedV2Templates_NO;

  if (s->JetSelectorIsValid()) {

    // MC provenance
    for(short int i = -25; i < 26; i++)
    {
      // PtRel

      oss.str("");    oss << "hMuonPtRelJetTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetTemplates[i] = histoPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVTemplates[i] = histoPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet or SV template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetTemplates[i] = histoPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVTemplates[i] = histoPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVTemplates[i] = histoPtRelJetYesSV;

      oss.str("");    oss << "hMuonDRPtRelTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRel = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtRelTemplates[i] = histoDRPtRel;

      oss.str("");    oss << "hMuonPtRelJetPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetPlusTemplates[i] = histoPlusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVPlusTemplates[i] = histoPlusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetPlusTemplates[i] = histoPlusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVPlusTemplates[i] = histoPlusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVPlusTemplates[i] = histoPlusPtRelJetYesSV;

      oss.str("");    oss << "hMuonPtRelJetMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetMinusTemplates[i] = histoMinusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVMinusTemplates[i] = histoMinusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetMinusTemplates[i] = histoMinusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVMinusTemplates[i] = histoMinusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVMinusTemplates[i] = histoMinusPtRelJetYesSV;

      oss.str("");    oss << "hMuonPtRelGenJetTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetTemplates[i] = histoPtRelGenJet;
      
      oss.str("");    oss << "hMuonDRPtRelGenTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} gen template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRelGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtRelGenTemplates[i] = histoDRPtRelGen;
      
      oss.str("");    oss << "hMuonPtRelGenJetPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet plus template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetPlusTemplates[i] = histoPtRelGenJetPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet minus template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetMinusTemplates[i] = histoPtRelGenJetMinus;
      
      oss.str("");    oss << "hMuonPtRelSmearedJetTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetTemplates_SV[i] = histoPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVTemplates_SV[i] = histoPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SV template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetTemplates_SV[i] = histoPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusTemplates_SV[i] = histoPlusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusTemplates_SV[i] = histoPlusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusTemplates_SV[i] = histoPlusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusTemplates_SV[i] = histoMinusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusTemplates_SV[i] = histoMinusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusTemplates_SV[i] = histoMinusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetTemplates_NO[i] = histoPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVTemplates_NO[i] = histoPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetTemplates_NO[i] = histoPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusTemplates_NO[i] = histoPlusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusTemplates_NO[i] = histoPlusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusTemplates_NO[i] = histoPlusPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusTemplates_NO[i] = histoMinusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusTemplates_NO[i] = histoMinusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusTemplates_NO[i] = histoMinusPtRelSmearedSVorJet_NO;

      // PtOut

      oss.str("");    oss << "hMuonPtOutJetTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetTemplates[i] = histoPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVTemplates[i] = histoPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetTemplates[i] = histoPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVTemplates[i] = histoPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVTemplates[i] = histoPtOutJetYesSV;

      oss.str("");    oss << "hMuonDRPtOutTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOut = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtOutTemplates[i] = histoDRPtOut;

      oss.str("");    oss << "hMuonPtOutJetPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetPlusTemplates[i] = histoPlusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVPlusTemplates[i] = histoPlusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetPlusTemplates[i] = histoPlusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVPlusTemplates[i] = histoPlusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVPlusTemplates[i] = histoPlusPtOutJetYesSV;

      oss.str("");    oss << "hMuonPtOutJetMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetMinusTemplates[i] = histoMinusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVMinusTemplates[i] = histoMinusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetMinusTemplates[i] = histoMinusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVMinusTemplates[i] = histoMinusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVMinusTemplates[i] = histoMinusPtOutJetYesSV;
      
      oss.str("");    oss << "hMuonPtOutGenJetTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetTemplates[i] = histoPtOutGenJet;
      
      oss.str("");    oss << "hMuonDRPtOutGenTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} gen template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOutGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtOutGenTemplates[i] = histoDRPtOutGen;
      
      oss.str("");    oss << "hMuonPtOutGenJetPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet plus template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetPlusTemplates[i] = histoPtOutGenJetPlus;
      
      oss.str("");    oss << "hMuonPtOutGenJetMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet minus template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetMinusTemplates[i] = histoPtOutGenJetMinus;
      
      oss.str("");    oss << "hMuonPtOutSmearedJetTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetTemplates_SV[i] = histoPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVTemplates_SV[i] = histoPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetTemplates_SV[i] = histoPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusTemplates_SV[i] = histoPlusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusTemplates_SV[i] = histoPlusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusTemplates_SV[i] = histoPlusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusTemplates_SV[i] = histoMinusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusTemplates_SV[i] = histoMinusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusTemplates_SV[i] = histoMinusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetTemplates_NO[i] = histoPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVTemplates_NO[i] = histoPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetTemplates_NO[i] = histoPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusTemplates_NO[i] = histoPlusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusTemplates_NO[i] = histoPlusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusTemplates_NO[i] = histoPlusPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusTemplates_NO[i] = histoMinusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusTemplates_NO[i] = histoMinusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusTemplates_NO[i] = histoMinusPtOutSmearedSVorJet_NO;
      
      // Emu (muon Energy evaluated in the Jet rest frame)
      oss.str("");    oss << "hMuonEmuJetTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) template for muon category " << i;    title = oss.str();
      TH1D* histoEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetTemplates[i] = histoEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetPlusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetPlusTemplates[i] = histoPlusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetMinusTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetMinusTemplates[i] = histoMinusEmuJet;
      
      oss.str("");    oss << "hMuonEmuSmearedJetTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) template for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetTemplates_SV[i] = histoEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusTemplates_SV[i] = histoPlusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusTemplates_SV[i] = histoMinusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) template for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetTemplates_NO[i] = histoEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusTemplates_NO[i] = histoPlusEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusTemplates_NO[i] = histoMinusEmuSmearedJet_NO;
    }

    // MC simplified code
    for(short int i = 0; i < 7; i++)
    {
      // PtRel

      oss.str("");    oss << "hMuonPtRelJetSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSimplifiedTemplates[i] = histoPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVSimplifiedTemplates[i] = histoPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetSimplifiedTemplates[i] = histoPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVSimplifiedTemplates[i] = histoPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVSimplifiedTemplates[i] = histoPtRelJetYesSV;

      oss.str("");    oss << "hMuonDRPtRelSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRel = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtRelSimplifiedTemplates[i] = histoDRPtRel;

      oss.str("");    oss << "hMuonPtRelJetPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetPlusSimplifiedTemplates[i] = histoPlusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVPlusSimplifiedTemplates[i] = histoPlusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetPlusSimplifiedTemplates[i] = histoPlusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVPlusSimplifiedTemplates[i] = histoPlusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVPlusSimplifiedTemplates[i] = histoPlusPtRelJetYesSV;

      oss.str("");    oss << "hMuonPtRelJetMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetMinusSimplifiedTemplates[i] = histoMinusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVMinusSimplifiedTemplates[i] = histoMinusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetMinusSimplifiedTemplates[i] = histoMinusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVMinusSimplifiedTemplates[i] = histoMinusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVMinusSimplifiedTemplates[i] = histoMinusPtRelJetYesSV;

      oss.str("");    oss << "hMuonPtRelGenJetSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet Simplified template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSimplifiedTemplates[i] = histoPtRelGenJet;
      
      oss.str("");    oss << "hMuonDRPtRelGenSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} gen Simplified template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRelGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtRelGenSimplifiedTemplates[i] = histoDRPtRelGen;
      
      oss.str("");    oss << "hMuonPtRelGenJetPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet plus Simplified template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetPlusSimplifiedTemplates[i] = histoPtRelGenJetPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet minus Simplified template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetMinusSimplifiedTemplates[i] = histoPtRelGenJetMinus;
      
      oss.str("");    oss << "hMuonPtRelJetOSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedTemplate for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetOSSimplifiedTemplates[i] = histoMuonPtRelJetOS;

      oss.str("");    oss << "hMuonPtRelJetSSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedTemplate for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSSimplifiedTemplates[i] = histoMuonPtRelJetSS;

      oss.str("");    oss << "hMuonPtRelJetSSPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedTemplate for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSPlusSimplifiedTemplates[i] = histoMuonPtRelJetSSPlus;

      oss.str("");    oss << "hMuonPtRelJetSSMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedTemplate for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSMinusSimplifiedTemplates[i] = histoMuonPtRelJetSSMinus;
      
      oss.str("");    oss << "hMuonPtRelGenJetOSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedTemplate for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetOSSimplifiedTemplates[i] = histoMuonPtRelGenJetOS;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedTemplate for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSSimplifiedTemplates[i] = histoMuonPtRelGenJetSS;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedTemplate for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSPlusSimplifiedTemplates[i] = histoMuonPtRelGenJetSSPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedTemplate for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSMinusSimplifiedTemplates[i] = histoMuonPtRelGenJetSSMinus;
      
      oss.str("");    oss << "hMuonPtRelSmearedJetSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet SimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetSimplifiedTemplates_SV[i] = histoPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV SimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVSimplifiedTemplates_SV[i] = histoPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetSimplifiedTemplates_SV[i] = histoPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusSimplifiedTemplates_SV[i] = histoPlusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusSimplifiedTemplates_SV[i] = histoPlusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusSimplifiedTemplates_SV[i] = histoPlusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusSimplifiedTemplates_SV[i] = histoMinusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusSimplifiedTemplates_SV[i] = histoMinusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusSimplifiedTemplates_SV[i] = histoMinusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet SimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetSimplifiedTemplates_NO[i] = histoPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV SimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVSimplifiedTemplates_NO[i] = histoPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetSimplifiedTemplates_NO[i] = histoPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusSimplifiedTemplates_NO[i] = histoPlusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusSimplifiedTemplates_NO[i] = histoPlusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusSimplifiedTemplates_NO[i] = histoPlusPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusSimplifiedTemplates_NO[i] = histoMinusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusSimplifiedTemplates_NO[i] = histoMinusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusSimplifiedTemplates_NO[i] = histoMinusPtRelSmearedSVorJet_NO;

      // PtOut

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplates[i] = histoPtOutJet;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateM0406_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate M0406 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesM0406[i] = histoPtOutJetM0406;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateM0608_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate M0608 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesM0608[i] = histoPtOutJetM0608;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateM0810_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate M0810 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesM0810[i] = histoPtOutJetM0810;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateM1012_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate M1012 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesM1012[i] = histoPtOutJetM1012;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateM1215_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate M1215 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesM1215[i] = histoPtOutJetM1215;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateM1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate M1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesM1520[i] = histoPtOutJetM1520;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateM20xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate M20xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesM20xx[i] = histoPtOutJetM20xx;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateJ1015_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplateJ1015 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesJ1015[i] = histoPtOutJetJ1015;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateJ1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplateJ1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesJ1520[i] = histoPtOutJetJ1520;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateJ2030_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplateJ2030 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesJ2030[i] = histoPtOutJetJ2030;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateJ30xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplateJ30xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesJ30xx[i] = histoPtOutJetJ30xx;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateD0025_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate D0025 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetD0025 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesD0025[i] = histoPtOutJetD0025;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateD2550_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate D2550 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetD2550 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesD2550[i] = histoPtOutJetD2550;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateD5075_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate D5075 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetD5075 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesD5075[i] = histoPtOutJetD5075;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedTemplateD7500_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate D7500 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetD7500 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedTemplatesD7500[i] = histoPtOutJetD7500;

      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplates[i] = histoPtOutGenJet;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateM0406_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate M0406 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesM0406[i] = histoPtOutGenJetM0406;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateM0608_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate M0608 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesM0608[i] = histoPtOutGenJetM0608;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateM0810_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate M0810 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesM0810[i] = histoPtOutGenJetM0810;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateM1012_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate M1012 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesM1012[i] = histoPtOutGenJetM1012;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateM1215_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate M1215 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesM1215[i] = histoPtOutGenJetM1215;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateM1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate M1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesM1520[i] = histoPtOutGenJetM1520;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateM20xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate M20xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesM20xx[i] = histoPtOutGenJetM20xx;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateJ1015_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplateJ1015 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesJ1015[i] = histoPtOutGenJetJ1015;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateJ1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplateJ1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesJ1520[i] = histoPtOutGenJetJ1520;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateJ2030_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplateJ2030 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesJ2030[i] = histoPtOutGenJetJ2030;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateJ30xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplateJ30xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesJ30xx[i] = histoPtOutGenJetJ30xx;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateD0025_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate D0025 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetD0025 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesD0025[i] = histoPtOutGenJetD0025;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateD2550_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate D2550 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetD2550 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesD2550[i] = histoPtOutGenJetD2550;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateD5075_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate D5075 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetD5075 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesD5075[i] = histoPtOutGenJetD5075;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedTemplateD7500_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedTemplate D7500 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetD7500 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedTemplatesD7500[i] = histoPtOutGenJetD7500;
      
      oss.str("");    oss << "hMuonPtOutSVSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVSimplifiedTemplates[i] = histoPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetSimplifiedTemplates[i] = histoPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVSimplifiedTemplates[i] = histoPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVSimplifiedTemplates[i] = histoPtOutJetYesSV;

      oss.str("");    oss << "hMuonDRPtOutSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOut = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtOutSimplifiedTemplates[i] = histoDRPtOut;

      oss.str("");    oss << "hMuonPtOutJetPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetPlusSimplifiedTemplates[i] = histoPlusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVPlusSimplifiedTemplates[i] = histoPlusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetPlusSimplifiedTemplates[i] = histoPlusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVPlusSimplifiedTemplates[i] = histoPlusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVPlusSimplifiedTemplates[i] = histoPlusPtOutJetYesSV;

      oss.str("");    oss << "hMuonPtOutJetMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetMinusSimplifiedTemplates[i] = histoMinusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVMinusSimplifiedTemplates[i] = histoMinusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetMinusSimplifiedTemplates[i] = histoMinusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVMinusSimplifiedTemplates[i] = histoMinusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVMinusSimplifiedTemplates[i] = histoMinusPtOutJetYesSV;
      
      oss.str("");    oss << "hMuonDRPtOutGenSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} gen Simplified template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOutGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtOutGenSimplifiedTemplates[i] = histoDRPtOutGen;
      
      oss.str("");    oss << "hMuonPtOutGenJetPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet plus Simplified template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetPlusSimplifiedTemplates[i] = histoPtOutGenJetPlus;
      
      oss.str("");    oss << "hMuonPtOutGenJetMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet minus Simplified template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetMinusSimplifiedTemplates[i] = histoPtOutGenJetMinus;

      oss.str("");    oss << "hMuonPtOutJetOSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetOSSimplifiedTemplates[i] = histoMuonPtOutJetOS;

      oss.str("");    oss << "hMuonPtOutJetSSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSSimplifiedTemplates[i] = histoMuonPtOutJetSS;

      oss.str("");    oss << "hMuonPtOutJetSSPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSPlusSimplifiedTemplates[i] = histoMuonPtOutJetSSPlus;

      oss.str("");    oss << "hMuonPtOutJetSSMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedTemplate for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSMinusSimplifiedTemplates[i] = histoMuonPtOutJetSSMinus;

      oss.str("");    oss << "hMuonPtOutGenJetOSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedTemplate for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetOSSimplifiedTemplates[i] = histoMuonPtOutGenJetOS;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedTemplate for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSSimplifiedTemplates[i] = histoMuonPtOutGenJetSS;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedTemplate for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSPlusSimplifiedTemplates[i] = histoMuonPtOutGenJetSSPlus;
      
      oss.str("");    oss << "hMuonPtOutJetGenSSMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedTemplate for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSMinusSimplifiedTemplates[i] = histoMuonPtOutGenJetSSMinus;
      
      oss.str("");    oss << "hMuonPtOutSmearedJetSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet SimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetSimplifiedTemplates_SV[i] = histoPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV SimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVSimplifiedTemplates_SV[i] = histoPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet SimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetSimplifiedTemplates_SV[i] = histoPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusSimplifiedTemplates_SV[i] = histoPlusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusSimplifiedTemplates_SV[i] = histoPlusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusSimplifiedTemplates_SV[i] = histoPlusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusSimplifiedTemplates_SV[i] = histoMinusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusSimplifiedTemplates_SV[i] = histoMinusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusSimplifiedTemplate SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusSimplifiedTemplates_SV[i] = histoMinusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet SimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetSimplifiedTemplates_NO[i] = histoPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV SimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVSimplifiedTemplates_NO[i] = histoPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet SimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetSimplifiedTemplates_NO[i] = histoPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusSimplifiedTemplates_NO[i] = histoPlusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusSimplifiedTemplates_NO[i] = histoPlusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusSimplifiedTemplates_NO[i] = histoPlusPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusSimplifiedTemplates_NO[i] = histoMinusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusSimplifiedTemplates_NO[i] = histoMinusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusSimplifiedTemplate NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusSimplifiedTemplates_NO[i] = histoMinusPtOutSmearedSVorJet_NO;
      
      // Emu (muon Energy evaluated in the Jet rest frame)
      oss.str("");    oss << "hMuonEmuJetSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSimplifiedTemplates[i] = histoEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetPlusSimplifiedTemplates[i] = histoPlusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetMinusSimplifiedTemplates[i] = histoMinusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetOSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedTemplate for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetOSSimplifiedTemplates[i] = histoMuonEmuJetOS;
      
      oss.str("");    oss << "hMuonEmuJetSSSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedTemplate for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSSimplifiedTemplates[i] = histoMuonEmuJetSS;
      
      oss.str("");    oss << "hMuonEmuJetSSPlusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedTemplate for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSPlusSimplifiedTemplates[i] = histoMuonEmuJetSSPlus;
      
      oss.str("");    oss << "hMuonEmuJetSSMinusSimplifiedTemplate_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedTemplate for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSMinusSimplifiedTemplates[i] = histoMuonEmuJetSSMinus;
      
      oss.str("");    oss << "hMuonEmuSmearedJetSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetSimplifiedTemplates_SV[i] = histoEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusSimplifiedTemplates_SV[i] = histoPlusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusSimplifiedTemplate_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusSimplifiedTemplates_SV[i] = histoMinusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetSimplifiedTemplates_NO[i] = histoEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusSimplifiedTemplates_NO[i] = histoPlusEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusSimplifiedTemplate_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedTemplate for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusSimplifiedTemplates_NO[i] = histoMinusEmuSmearedJet_NO;
    }

    // MC simplified merged code
    for(short int i = 0; i < 6; i++)
    {
      // PtRel

      oss.str("");    oss << "hMuonPtRelJetSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSimplifiedMergedV0Templates[i] = histoPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVSimplifiedMergedV0Templates[i] = histoPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetSimplifiedMergedV0Templates[i] = histoPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVSimplifiedMergedV0Templates[i] = histoPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVSimplifiedMergedV0Templates[i] = histoPtRelJetYesSV;

      oss.str("");    oss << "hMuonDRPtRelSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRel = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtRelSimplifiedMergedV0Templates[i] = histoDRPtRel;

      oss.str("");    oss << "hMuonPtRelJetPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetPlusSimplifiedMergedV0Templates[i] = histoPlusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVPlusSimplifiedMergedV0Templates[i] = histoPlusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetPlusSimplifiedMergedV0Templates[i] = histoPlusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVPlusSimplifiedMergedV0Templates[i] = histoPlusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVPlusSimplifiedMergedV0Templates[i] = histoPlusPtRelJetYesSV;

      oss.str("");    oss << "hMuonPtRelJetMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetMinusSimplifiedMergedV0Templates[i] = histoMinusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVMinusSimplifiedMergedV0Templates[i] = histoMinusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetMinusSimplifiedMergedV0Templates[i] = histoMinusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVMinusSimplifiedMergedV0Templates[i] = histoMinusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVMinusSimplifiedMergedV0Templates[i] = histoMinusPtRelJetYesSV;
      
      oss.str("");    oss << "hMuonPtRelGenJetSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet SimplifiedMergedV0 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSimplifiedMergedV0Templates[i] = histoPtRelGenJet;
      
      oss.str("");    oss << "hMuonDRPtRelGenSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} gen SimplifiedMergedV0 template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRelGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtRelGenSimplifiedMergedV0Templates[i] = histoDRPtRelGen;
      
      oss.str("");    oss << "hMuonPtRelGenJetPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet plus SimplifiedMergedV0 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetPlusSimplifiedMergedV0Templates[i] = histoPtRelGenJetPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet minus SimplifiedMergedV0 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetMinusSimplifiedMergedV0Templates[i] = histoPtRelGenJetMinus;

      oss.str("");    oss << "hMuonPtRelJetOSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV0Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetOSSimplifiedMergedV0Templates[i] = histoMuonPtRelJetOS;

      oss.str("");    oss << "hMuonPtRelJetSSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV0Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSSimplifiedMergedV0Templates[i] = histoMuonPtRelJetSS;

      oss.str("");    oss << "hMuonPtRelJetSSPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV0Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSPlusSimplifiedMergedV0Templates[i] = histoMuonPtRelJetSSPlus;

      oss.str("");    oss << "hMuonPtRelJetSSMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV0Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSMinusSimplifiedMergedV0Templates[i] = histoMuonPtRelJetSSMinus;
      
      oss.str("");    oss << "hMuonPtRelGenJetOSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV0Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetOSSimplifiedMergedV0Templates[i] = histoMuonPtRelGenJetOS;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV0Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSSimplifiedMergedV0Templates[i] = histoMuonPtRelGenJetSS;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV0Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSPlusSimplifiedMergedV0Templates[i] = histoMuonPtRelGenJetSSPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV0Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSMinusSimplifiedMergedV0Templates[i] = histoMuonPtRelGenJetSSMinus;
      
      oss.str("");    oss << "hMuonPtRelSmearedJetSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet SimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetSimplifiedMergedV0Templates_SV[i] = histoPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV SimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVSimplifiedMergedV0Templates_SV[i] = histoPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetSimplifiedMergedV0Templates_SV[i] = histoPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusSimplifiedMergedV0Templates_SV[i] = histoPlusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusSimplifiedMergedV0Templates_SV[i] = histoPlusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV0Templates_SV[i] = histoPlusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusSimplifiedMergedV0Templates_SV[i] = histoMinusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusSimplifiedMergedV0Templates_SV[i] = histoMinusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV0Templates_SV[i] = histoMinusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet SimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetSimplifiedMergedV0Templates_NO[i] = histoPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV SimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVSimplifiedMergedV0Templates_NO[i] = histoPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetSimplifiedMergedV0Templates_NO[i] = histoPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusSimplifiedMergedV0Templates_NO[i] = histoPlusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusSimplifiedMergedV0Templates_NO[i] = histoPlusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV0Templates_NO[i] = histoPlusPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusSimplifiedMergedV0Templates_NO[i] = histoMinusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusSimplifiedMergedV0Templates_NO[i] = histoMinusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV0Templates_NO[i] = histoMinusPtRelSmearedSVorJet_NO;

      // PtOut

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0Templates[i] = histoPtOutJet;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateM0406_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template M0406 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesM0406[i] = histoPtOutJetM0406;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateM0608_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template M0608 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesM0608[i] = histoPtOutJetM0608;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateM0810_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template M0810 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesM0810[i] = histoPtOutJetM0810;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateM1012_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template M1012 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesM1012[i] = histoPtOutJetM1012;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateM1215_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template M1215 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesM1215[i] = histoPtOutJetM1215;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateM1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template M1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesM1520[i] = histoPtOutJetM1520;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateM20xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template M20xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesM20xx[i] = histoPtOutJetM20xx;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateJ1015_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0TemplateJ1015 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesJ1015[i] = histoPtOutJetJ1015;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateJ1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0TemplateJ1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesJ1520[i] = histoPtOutJetJ1520;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateJ2030_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0TemplateJ2030 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesJ2030[i] = histoPtOutJetJ2030;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV0TemplateJ30xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0TemplateJ30xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV0TemplatesJ30xx[i] = histoPtOutJetJ30xx;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateM0406_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0Template M0406 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0406[i] = histoPtOutGenJetM0406;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateM0608_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0Template M0608 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0608[i] = histoPtOutGenJetM0608;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateM0810_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0Template M0810 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0810[i] = histoPtOutGenJetM0810;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateM1012_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0Template M1012 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1012[i] = histoPtOutGenJetM1012;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateM1215_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0Template M1215 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1215[i] = histoPtOutGenJetM1215;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateM1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0Template M1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1520[i] = histoPtOutGenJetM1520;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateM20xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0Template M20xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesM20xx[i] = histoPtOutGenJetM20xx;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateJ1015_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0TemplateJ1015 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ1015[i] = histoPtOutGenJetJ1015;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateJ1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0TemplateJ1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ1520[i] = histoPtOutGenJetJ1520;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateJ2030_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0TemplateJ2030 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ2030[i] = histoPtOutGenJetJ2030;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0TemplateJ30xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV0TemplateJ30xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ30xx[i] = histoPtOutGenJetJ30xx;
      
      oss.str("");    oss << "hMuonPtOutSVSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVSimplifiedMergedV0Templates[i] = histoPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetSimplifiedMergedV0Templates[i] = histoPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVSimplifiedMergedV0Templates[i] = histoPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVSimplifiedMergedV0Templates[i] = histoPtOutJetYesSV;

      oss.str("");    oss << "hMuonDRPtOutSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOut = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtOutSimplifiedMergedV0Templates[i] = histoDRPtOut;

      oss.str("");    oss << "hMuonPtOutJetPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetPlusSimplifiedMergedV0Templates[i] = histoPlusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVPlusSimplifiedMergedV0Templates[i] = histoPlusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetPlusSimplifiedMergedV0Templates[i] = histoPlusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVPlusSimplifiedMergedV0Templates[i] = histoPlusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVPlusSimplifiedMergedV0Templates[i] = histoPlusPtOutJetYesSV;

      oss.str("");    oss << "hMuonPtOutJetMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetMinusSimplifiedMergedV0Templates[i] = histoMinusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVMinusSimplifiedMergedV0Templates[i] = histoMinusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetMinusSimplifiedMergedV0Templates[i] = histoMinusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVMinusSimplifiedMergedV0Templates[i] = histoMinusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVMinusSimplifiedMergedV0Templates[i] = histoMinusPtOutJetYesSV;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV0 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV0Templates[i] = histoPtOutGenJet;
      
      oss.str("");    oss << "hMuonDRPtOutGenSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} gen SimplifiedMergedV0 template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOutGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtOutGenSimplifiedMergedV0Templates[i] = histoDRPtOutGen;
      
      oss.str("");    oss << "hMuonPtOutGenJetPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet plus SimplifiedMergedV0 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetPlusSimplifiedMergedV0Templates[i] = histoPtOutGenJetPlus;
      
      oss.str("");    oss << "hMuonPtOutGenJetMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet minus SimplifiedMergedV0 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetMinusSimplifiedMergedV0Templates[i] = histoPtOutGenJetMinus;

      oss.str("");    oss << "hMuonPtOutJetOSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetOSSimplifiedMergedV0Templates[i] = histoMuonPtOutJetOS;

      oss.str("");    oss << "hMuonPtOutJetSSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSSimplifiedMergedV0Templates[i] = histoMuonPtOutJetSS;

      oss.str("");    oss << "hMuonPtOutJetSSPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSPlusSimplifiedMergedV0Templates[i] = histoMuonPtOutJetSSPlus;

      oss.str("");    oss << "hMuonPtOutJetSSMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV0Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSMinusSimplifiedMergedV0Templates[i] = histoMuonPtOutJetSSMinus;
      
      oss.str("");    oss << "hMuonPtOutGenJetOSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV0Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetOSSimplifiedMergedV0Templates[i] = histoMuonPtOutGenJetOS;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV0Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSSimplifiedMergedV0Templates[i] = histoMuonPtOutGenJetSS;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV0Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSPlusSimplifiedMergedV0Templates[i] = histoMuonPtOutGenJetSSPlus;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV0Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSMinusSimplifiedMergedV0Templates[i] = histoMuonPtOutGenJetSSMinus;
      
      oss.str("");    oss << "hMuonPtOutSmearedJetSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet SimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetSimplifiedMergedV0Templates_SV[i] = histoPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV SimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVSimplifiedMergedV0Templates_SV[i] = histoPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet SimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetSimplifiedMergedV0Templates_SV[i] = histoPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusSimplifiedMergedV0Templates_SV[i] = histoPlusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusSimplifiedMergedV0Templates_SV[i] = histoPlusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV0Templates_SV[i] = histoPlusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusSimplifiedMergedV0Templates_SV[i] = histoMinusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusSimplifiedMergedV0Templates_SV[i] = histoMinusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusSimplifiedMergedV0Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV0Templates_SV[i] = histoMinusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet SimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetSimplifiedMergedV0Templates_NO[i] = histoPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV SimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVSimplifiedMergedV0Templates_NO[i] = histoPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet SimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetSimplifiedMergedV0Templates_NO[i] = histoPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusSimplifiedMergedV0Templates_NO[i] = histoPlusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusSimplifiedMergedV0Templates_NO[i] = histoPlusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV0Templates_NO[i] = histoPlusPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusSimplifiedMergedV0Templates_NO[i] = histoMinusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusSimplifiedMergedV0Templates_NO[i] = histoMinusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusSimplifiedMergedV0Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV0Templates_NO[i] = histoMinusPtOutSmearedSVorJet_NO;
      
      // Emu
      oss.str("");    oss << "hMuonEmuJetSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSimplifiedMergedV0Templates[i] = histoEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetPlusSimplifiedMergedV0Templates[i] = histoPlusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetMinusSimplifiedMergedV0Templates[i] = histoMinusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetOSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV0Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetOSSimplifiedMergedV0Templates[i] = histoMuonEmuJetOS;
      
      oss.str("");    oss << "hMuonEmuJetSSSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV0Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSSimplifiedMergedV0Templates[i] = histoMuonEmuJetSS;
      
      oss.str("");    oss << "hMuonEmuJetSSPlusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV0Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSPlusSimplifiedMergedV0Templates[i] = histoMuonEmuJetSSPlus;
      
      oss.str("");    oss << "hMuonEmuJetSSMinusSimplifiedMergedV0Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV0Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSMinusSimplifiedMergedV0Templates[i] = histoMuonEmuJetSSMinus;
      
      oss.str("");    oss << "hMuonEmuSmearedJetSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetSimplifiedMergedV0Templates_SV[i] = histoEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusSimplifiedMergedV0Templates_SV[i] = histoPlusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusSimplifiedMergedV0Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusSimplifiedMergedV0Templates_SV[i] = histoMinusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetSimplifiedMergedV0Templates_NO[i] = histoEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusSimplifiedMergedV0Templates_NO[i] = histoPlusEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusSimplifiedMergedV0Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV0Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusSimplifiedMergedV0Templates_NO[i] = histoMinusEmuSmearedJet_NO;
    }

    for(short int i = 0; i < 4; i++)
    {
      // PtRel

      oss.str("");    oss << "hMuonPtRelJetSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSimplifiedMergedV1Templates[i] = histoPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVSimplifiedMergedV1Templates[i] = histoPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetSimplifiedMergedV1Templates[i] = histoPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVSimplifiedMergedV1Templates[i] = histoPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVSimplifiedMergedV1Templates[i] = histoPtRelJetYesSV;

      oss.str("");    oss << "hMuonDRPtRelSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRel = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtRelSimplifiedMergedV1Templates[i] = histoDRPtRel;

      oss.str("");    oss << "hMuonPtRelJetPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetPlusSimplifiedMergedV1Templates[i] = histoPlusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVPlusSimplifiedMergedV1Templates[i] = histoPlusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetPlusSimplifiedMergedV1Templates[i] = histoPlusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVPlusSimplifiedMergedV1Templates[i] = histoPlusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVPlusSimplifiedMergedV1Templates[i] = histoPlusPtRelJetYesSV;

      oss.str("");    oss << "hMuonPtRelJetMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetMinusSimplifiedMergedV1Templates[i] = histoMinusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVMinusSimplifiedMergedV1Templates[i] = histoMinusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetMinusSimplifiedMergedV1Templates[i] = histoMinusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVMinusSimplifiedMergedV1Templates[i] = histoMinusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVMinusSimplifiedMergedV1Templates[i] = histoMinusPtRelJetYesSV;
      
      oss.str("");    oss << "hMuonPtRelGenJetSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet SimplifiedMergedV1 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSimplifiedMergedV1Templates[i] = histoPtRelGenJet;
      
      oss.str("");    oss << "hMuonDRPtRelGenSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} gen SimplifiedMergedV1 template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRelGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtRelGenSimplifiedMergedV1Templates[i] = histoDRPtRelGen;
      
      oss.str("");    oss << "hMuonPtRelGenJetPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet plus SimplifiedMergedV1 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetPlusSimplifiedMergedV1Templates[i] = histoPtRelGenJetPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet minus SimplifiedMergedV1 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetMinusSimplifiedMergedV1Templates[i] = histoPtRelGenJetMinus;

      oss.str("");    oss << "hMuonPtRelJetOSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV1Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetOSSimplifiedMergedV1Templates[i] = histoMuonPtRelJetOS;

      oss.str("");    oss << "hMuonPtRelJetSSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV1Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSSimplifiedMergedV1Templates[i] = histoMuonPtRelJetSS;

      oss.str("");    oss << "hMuonPtRelJetSSPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV1Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSPlusSimplifiedMergedV1Templates[i] = histoMuonPtRelJetSSPlus;

      oss.str("");    oss << "hMuonPtRelJetSSMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV1Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSMinusSimplifiedMergedV1Templates[i] = histoMuonPtRelJetSSMinus;

      oss.str("");    oss << "hMuonPtRelGenJetOSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV1Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetOSSimplifiedMergedV1Templates[i] = histoMuonPtRelGenJetOS;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV1Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSSimplifiedMergedV1Templates[i] = histoMuonPtRelGenJetSS;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV1Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSPlusSimplifiedMergedV1Templates[i] = histoMuonPtRelGenJetSSPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV1Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSMinusSimplifiedMergedV1Templates[i] = histoMuonPtRelGenJetSSMinus;
      
      oss.str("");    oss << "hMuonPtRelSmearedJetSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet SimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetSimplifiedMergedV1Templates_SV[i] = histoPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV SimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVSimplifiedMergedV1Templates_SV[i] = histoPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetSimplifiedMergedV1Templates_SV[i] = histoPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusSimplifiedMergedV1Templates_SV[i] = histoPlusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusSimplifiedMergedV1Templates_SV[i] = histoPlusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV1Templates_SV[i] = histoPlusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusSimplifiedMergedV1Templates_SV[i] = histoMinusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusSimplifiedMergedV1Templates_SV[i] = histoMinusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV1Templates_SV[i] = histoMinusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet SimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetSimplifiedMergedV1Templates_NO[i] = histoPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV SimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVSimplifiedMergedV1Templates_NO[i] = histoPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetSimplifiedMergedV1Templates_NO[i] = histoPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusSimplifiedMergedV1Templates_NO[i] = histoPlusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusSimplifiedMergedV1Templates_NO[i] = histoPlusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV1Templates_NO[i] = histoPlusPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusSimplifiedMergedV1Templates_NO[i] = histoMinusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusSimplifiedMergedV1Templates_NO[i] = histoMinusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV1Templates_NO[i] = histoMinusPtRelSmearedSVorJet_NO;

      // PtOut

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1Templates[i] = histoPtOutJet;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateM0406_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template M0406 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesM0406[i] = histoPtOutJetM0406;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateM0608_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template M0608 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesM0608[i] = histoPtOutJetM0608;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateM0810_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template M0810 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesM0810[i] = histoPtOutJetM0810;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateM1012_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template M1012 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesM1012[i] = histoPtOutJetM1012;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateM1215_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template M1215 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesM1215[i] = histoPtOutJetM1215;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateM1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template M1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesM1520[i] = histoPtOutJetM1520;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateM20xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template M20xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesM20xx[i] = histoPtOutJetM20xx;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateJ1015_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1TemplateJ1015 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesJ1015[i] = histoPtOutJetJ1015;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateJ1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1TemplateJ1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesJ1520[i] = histoPtOutJetJ1520;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateJ2030_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1TemplateJ2030 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesJ2030[i] = histoPtOutJetJ2030;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateJ30xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1TemplateJ30xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV1TemplatesJ30xx[i] = histoPtOutJetJ30xx;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateM0406_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1Template M0406 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0406[i] = histoPtOutGenJetM0406;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateM0608_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1Template M0608 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0608[i] = histoPtOutGenJetM0608;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateM0810_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1Template M0810 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0810[i] = histoPtOutGenJetM0810;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateM1012_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1Template M1012 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1012[i] = histoPtOutGenJetM1012;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateM1215_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1Template M1215 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1215[i] = histoPtOutGenJetM1215;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateM1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1Template M1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1520[i] = histoPtOutGenJetM1520;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateM20xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1Template M20xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesM20xx[i] = histoPtOutGenJetM20xx;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateJ1015_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1TemplateJ1015 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ1015[i] = histoPtOutGenJetJ1015;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateJ1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1TemplateJ1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ1520[i] = histoPtOutGenJetJ1520;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateJ2030_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1TemplateJ2030 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ2030[i] = histoPtOutGenJetJ2030;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1TemplateJ30xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV1TemplateJ30xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ30xx[i] = histoPtOutGenJetJ30xx;
      
      oss.str("");    oss << "hMuonPtOutSVSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVSimplifiedMergedV1Templates[i] = histoPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetSimplifiedMergedV1Templates[i] = histoPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVSimplifiedMergedV1Templates[i] = histoPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVSimplifiedMergedV1Templates[i] = histoPtOutJetYesSV;

      oss.str("");    oss << "hMuonDRPtOutSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOut = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtOutSimplifiedMergedV1Templates[i] = histoDRPtOut;

      oss.str("");    oss << "hMuonPtOutJetPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetPlusSimplifiedMergedV1Templates[i] = histoPlusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVPlusSimplifiedMergedV1Templates[i] = histoPlusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetPlusSimplifiedMergedV1Templates[i] = histoPlusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVPlusSimplifiedMergedV1Templates[i] = histoPlusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVPlusSimplifiedMergedV1Templates[i] = histoPlusPtOutJetYesSV;

      oss.str("");    oss << "hMuonPtOutJetMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetMinusSimplifiedMergedV1Templates[i] = histoMinusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVMinusSimplifiedMergedV1Templates[i] = histoMinusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetMinusSimplifiedMergedV1Templates[i] = histoMinusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVMinusSimplifiedMergedV1Templates[i] = histoMinusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVMinusSimplifiedMergedV1Templates[i] = histoMinusPtOutJetYesSV;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV1 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV1Templates[i] = histoPtOutGenJet;
      
      oss.str("");    oss << "hMuonDRPtOutGenSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} gen SimplifiedMergedV1 template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOutGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtOutGenSimplifiedMergedV1Templates[i] = histoDRPtOutGen;
      
      oss.str("");    oss << "hMuonPtOutGenJetPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet plus SimplifiedMergedV1 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetPlusSimplifiedMergedV1Templates[i] = histoPtOutGenJetPlus;
      
      oss.str("");    oss << "hMuonPtOutGenJetMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet minus SimplifiedMergedV1 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetMinusSimplifiedMergedV1Templates[i] = histoPtOutGenJetMinus;

      oss.str("");    oss << "hMuonPtOutJetOSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetOSSimplifiedMergedV1Templates[i] = histoMuonPtOutJetOS;

      oss.str("");    oss << "hMuonPtOutJetSSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSSimplifiedMergedV1Templates[i] = histoMuonPtOutJetSS;

      oss.str("");    oss << "hMuonPtOutJetSSPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSPlusSimplifiedMergedV1Templates[i] = histoMuonPtOutJetSSPlus;

      oss.str("");    oss << "hMuonPtOutJetSSMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSMinusSimplifiedMergedV1Templates[i] = histoMuonPtOutJetSSMinus;
      
      oss.str("");    oss << "hMuonPtOutGenJetOSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV1Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetOSSimplifiedMergedV1Templates[i] = histoMuonPtOutGenJetOS;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV1Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSSimplifiedMergedV1Templates[i] = histoMuonPtOutGenJetSS;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV1Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSPlusSimplifiedMergedV1Templates[i] = histoMuonPtOutGenJetSSPlus;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV1Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSMinusSimplifiedMergedV1Templates[i] = histoMuonPtOutGenJetSSMinus;
      
      oss.str("");    oss << "hMuonPtOutSmearedJetSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet SimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetSimplifiedMergedV1Templates_SV[i] = histoPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV SimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVSimplifiedMergedV1Templates_SV[i] = histoPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet SimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetSimplifiedMergedV1Templates_SV[i] = histoPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusSimplifiedMergedV1Templates_SV[i] = histoPlusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusSimplifiedMergedV1Templates_SV[i] = histoPlusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV1Templates_SV[i] = histoPlusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusSimplifiedMergedV1Templates_SV[i] = histoMinusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusSimplifiedMergedV1Templates_SV[i] = histoMinusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusSimplifiedMergedV1Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV1Templates_SV[i] = histoMinusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet SimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetSimplifiedMergedV1Templates_NO[i] = histoPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV SimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVSimplifiedMergedV1Templates_NO[i] = histoPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet SimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetSimplifiedMergedV1Templates_NO[i] = histoPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusSimplifiedMergedV1Templates_NO[i] = histoPlusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusSimplifiedMergedV1Templates_NO[i] = histoPlusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV1Templates_NO[i] = histoPlusPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusSimplifiedMergedV1Templates_NO[i] = histoMinusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusSimplifiedMergedV1Templates_NO[i] = histoMinusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusSimplifiedMergedV1Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV1Templates_NO[i] = histoMinusPtOutSmearedSVorJet_NO;
      
      for(short int j = i; j < 4; j++)
      {
        short int index = j<=i?10*j+i:10*i+j;
        
        oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV12DTemplate_" << index;    name=oss.str();
        oss.str("");    oss << "2D p_{T}^{Out} jet SimplifiedMergedV1Template for event category " << index;    title=oss.str();
        TH2D* histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 5., 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "p_{T}^{Out} [GeV/c]");
        mMuonPtOutJetSimplifiedMergedV12DTemplates[index] = histo2D;
        
        oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV12DTemplateOS_" << index;    name=oss.str();
        oss.str("");    oss << "2D p_{T}^{Out} jet SimplifiedMergedV1Template OS for event category " << index;    title=oss.str();
        histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 5., 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "p_{T}^{Out} [GeV/c]");
        mMuonPtOutJetSimplifiedMergedV12DTemplatesOS[index] = histo2D;
        
        oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV12DTemplateSS_" << index;    name=oss.str();
        oss.str("");    oss << "2D p_{T}^{Out} jet SimplifiedMergedV1Template SS for event category " << index;    title=oss.str();
        histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 5., 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "p_{T}^{Out} [GeV/c]");
        mMuonPtOutJetSimplifiedMergedV12DTemplatesSS[index] = histo2D;
        
        if(i==j)
        {
          std::pair<short int, short int> indexPair = std::make_pair(i,index);
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template for muon category " << i << " and event category " << index;    title=oss.str();
          TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateOSByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template OS for muon category " << i << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesOSByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateSSByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template SS for muon category " << i << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesSSByEventCode[indexPair] = histo;
        }
        else
        {
          std::pair<short int, short int> indexPair = std::make_pair(i,index);
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template for muon category " << i << " and event category " << index;    title=oss.str();
          TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateOSByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template OS for muon category " << i << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesOSByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateSSByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template SS for muon category " << i << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesSSByEventCode[indexPair] = histo;
          
          indexPair = std::make_pair(j,index);
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateByEventCode_" << j << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template for muon category " << j << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateOSByEventCode_" << j << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template OS for muon category " << j << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesOSByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV1TemplateSSByEventCode_" << j << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV1Template SS for muon category " << j << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV1TemplatesSSByEventCode[indexPair] = histo;
        }
      }
      
      // Emu
      oss.str("");    oss << "hMuonEmuJetSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSimplifiedMergedV1Templates[i] = histoEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetPlusSimplifiedMergedV1Templates[i] = histoPlusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetMinusSimplifiedMergedV1Templates[i] = histoMinusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetOSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV1Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetOSSimplifiedMergedV1Templates[i] = histoMuonEmuJetOS;
      
      oss.str("");    oss << "hMuonEmuJetSSSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV1Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSSimplifiedMergedV1Templates[i] = histoMuonEmuJetSS;
      
      oss.str("");    oss << "hMuonEmuJetSSPlusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV1Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSPlusSimplifiedMergedV1Templates[i] = histoMuonEmuJetSSPlus;
      
      oss.str("");    oss << "hMuonEmuJetSSMinusSimplifiedMergedV1Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV1Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSMinusSimplifiedMergedV1Templates[i] = histoMuonEmuJetSSMinus;
      
      oss.str("");    oss << "hMuonEmuSmearedJetSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetSimplifiedMergedV1Templates_SV[i] = histoEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusSimplifiedMergedV1Templates_SV[i] = histoPlusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusSimplifiedMergedV1Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusSimplifiedMergedV1Templates_SV[i] = histoMinusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetSimplifiedMergedV1Templates_NO[i] = histoEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusSimplifiedMergedV1Templates_NO[i] = histoPlusEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusSimplifiedMergedV1Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV1Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusSimplifiedMergedV1Templates_NO[i] = histoMinusEmuSmearedJet_NO;
    }


    for(short int i = 0; i < 5; i++)
    {
      // PtRel

      oss.str("");    oss << "hMuonPtRelJetSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSimplifiedMergedV2Templates[i] = histoPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVSimplifiedMergedV2Templates[i] = histoPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetSimplifiedMergedV2Templates[i] = histoPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVSimplifiedMergedV2Templates[i] = histoPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVSimplifiedMergedV2Templates[i] = histoPtRelJetYesSV;

      oss.str("");    oss << "hMuonDRPtRelSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRel = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtRelSimplifiedMergedV2Templates[i] = histoDRPtRel;

      oss.str("");    oss << "hMuonPtRelJetPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetPlusSimplifiedMergedV2Templates[i] = histoPlusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVPlusSimplifiedMergedV2Templates[i] = histoPlusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetPlusSimplifiedMergedV2Templates[i] = histoPlusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVPlusSimplifiedMergedV2Templates[i] = histoPlusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVPlusSimplifiedMergedV2Templates[i] = histoPlusPtRelJetYesSV;

      oss.str("");    oss << "hMuonPtRelJetMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetMinusSimplifiedMergedV2Templates[i] = histoMinusPtRelJet;

      oss.str("");    oss << "hMuonPtRelSVMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVMinusSimplifiedMergedV2Templates[i] = histoMinusPtRelSV;

      oss.str("");    oss << "hMuonPtRelSVorJetMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} SV or jet MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSVorJetMinusSimplifiedMergedV2Templates[i] = histoMinusPtRelSVorJet;

      oss.str("");    oss << "hMuonPtRelJetNoSVMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet no SV MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetNoSVMinusSimplifiedMergedV2Templates[i] = histoMinusPtRelJetNoSV;

      oss.str("");    oss << "hMuonPtRelJetYesSVMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet with SV MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetYesSVMinusSimplifiedMergedV2Templates[i] = histoMinusPtRelJetYesSV;
      
      oss.str("");    oss << "hMuonPtRelGenJetSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet SimplifiedMergedV2 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSimplifiedMergedV2Templates[i] = histoPtRelGenJet;
      
      oss.str("");    oss << "hMuonDRPtRelGenSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Rel} gen SimplifiedMergedV2 template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtRelGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtRelGenSimplifiedMergedV2Templates[i] = histoDRPtRelGen;
      
      oss.str("");    oss << "hMuonPtRelGenJetPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet plus SimplifiedMergedV2 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetPlusSimplifiedMergedV2Templates[i] = histoPtRelGenJetPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} gen jet minus SimplifiedMergedV2 template for muon category " << i;    title = oss.str();
      TH1D* histoPtRelGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetMinusSimplifiedMergedV2Templates[i] = histoPtRelGenJetMinus;

      oss.str("");    oss << "hMuonPtRelJetOSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV2Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetOSSimplifiedMergedV2Templates[i] = histoMuonPtRelJetOS;

      oss.str("");    oss << "hMuonPtRelJetSSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV2Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSSimplifiedMergedV2Templates[i] = histoMuonPtRelJetSS;

      oss.str("");    oss << "hMuonPtRelJetSSPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV2Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSPlusSimplifiedMergedV2Templates[i] = histoMuonPtRelJetSSPlus;

      oss.str("");    oss << "hMuonPtRelJetSSMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} jet SimplifiedMergedV2Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelJetSSMinusSimplifiedMergedV2Templates[i] = histoMuonPtRelJetSSMinus;

      oss.str("");    oss << "hMuonPtRelGenJetOSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV2Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetOSSimplifiedMergedV2Templates[i] = histoMuonPtRelGenJetOS;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV2Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSSimplifiedMergedV2Templates[i] = histoMuonPtRelGenJetSS;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV2Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSPlusSimplifiedMergedV2Templates[i] = histoMuonPtRelGenJetSSPlus;
      
      oss.str("");    oss << "hMuonPtRelGenJetSSMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} Gen jet SimplifiedMergedV2Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtRelGenJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelGenJetSSMinusSimplifiedMergedV2Templates[i] = histoMuonPtRelGenJetSSMinus;
      
      oss.str("");    oss << "hMuonPtRelSmearedJetSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet SimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetSimplifiedMergedV2Templates_SV[i] = histoPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV SimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVSimplifiedMergedV2Templates_SV[i] = histoPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetSimplifiedMergedV2Templates_SV[i] = histoPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusSimplifiedMergedV2Templates_SV[i] = histoPlusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusSimplifiedMergedV2Templates_SV[i] = histoPlusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV2Templates_SV[i] = histoPlusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusSimplifiedMergedV2Templates_SV[i] = histoMinusPtRelSmearedJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusSimplifiedMergedV2Templates_SV[i] = histoMinusPtRelSmearedSV_SV;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV2Templates_SV[i] = histoMinusPtRelSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtRelSmearedJetSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet SimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetSimplifiedMergedV2Templates_NO[i] = histoPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV SimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVSimplifiedMergedV2Templates_NO[i] = histoPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet SimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetSimplifiedMergedV2Templates_NO[i] = histoPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetPlusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet PlusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetPlusSimplifiedMergedV2Templates_NO[i] = histoPlusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVPlusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV PlusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVPlusSimplifiedMergedV2Templates_NO[i] = histoPlusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetPlusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet PlusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV2Templates_NO[i] = histoPlusPtRelSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedJetMinusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared jet MinusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedJetMinusSimplifiedMergedV2Templates_NO[i] = histoMinusPtRelSmearedJet_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVMinusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV MinusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVMinusSimplifiedMergedV2Templates_NO[i] = histoMinusPtRelSmearedSV_NO;

      oss.str("");    oss << "hMuonPtRelSmearedSVorJetMinusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Rel} smeared SV or jet MinusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtRelSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
      mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV2Templates_NO[i] = histoMinusPtRelSmearedSVorJet_NO;


      // PtOut

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2Templates[i] = histoPtOutJet;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateM0406_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template M0406 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesM0406[i] = histoPtOutJetM0406;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateM0608_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template M0608 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesM0608[i] = histoPtOutJetM0608;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateM0810_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template M0810 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesM0810[i] = histoPtOutJetM0810;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateM1012_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template M1012 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesM1012[i] = histoPtOutJetM1012;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateM1215_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template M1215 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesM1215[i] = histoPtOutJetM1215;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateM1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template M1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesM1520[i] = histoPtOutJetM1520;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateM20xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template M20xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesM20xx[i] = histoPtOutJetM20xx;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateJ1015_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2TemplateJ1015 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesJ1015[i] = histoPtOutJetJ1015;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateJ1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2TemplateJ1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesJ1520[i] = histoPtOutJetJ1520;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateJ2030_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2TemplateJ2030 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesJ2030[i] = histoPtOutJetJ2030;

      oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateJ30xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2TemplateJ30xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSimplifiedMergedV2TemplatesJ30xx[i] = histoPtOutJetJ30xx;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateM0406_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2Template M0406 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0406[i] = histoPtOutGenJetM0406;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateM0608_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2Template M0608 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0608[i] = histoPtOutGenJetM0608;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateM0810_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2Template M0810 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0810[i] = histoPtOutGenJetM0810;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateM1012_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2Template M1012 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1012[i] = histoPtOutGenJetM1012;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateM1215_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2Template M1215 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1215[i] = histoPtOutGenJetM1215;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateM1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2Template M1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1520[i] = histoPtOutGenJetM1520;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateM20xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2Template M20xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesM20xx[i] = histoPtOutGenJetM20xx;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateJ1015_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2TemplateJ1015 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ1015[i] = histoPtOutGenJetJ1015;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateJ1520_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2TemplateJ1520 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ1520[i] = histoPtOutGenJetJ1520;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateJ2030_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2TemplateJ2030 for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ2030[i] = histoPtOutGenJetJ2030;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2TemplateJ30xx_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} Gen jet SimplifiedMergedV2TemplateJ30xx for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ30xx[i] = histoPtOutGenJetJ30xx;
      
      oss.str("");    oss << "hMuonPtOutSVSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVSimplifiedMergedV2Templates[i] = histoPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetSimplifiedMergedV2Templates[i] = histoPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVSimplifiedMergedV2Templates[i] = histoPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVSimplifiedMergedV2Templates[i] = histoPtOutJetYesSV;

      oss.str("");    oss << "hMuonDRPtOutSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOut = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
      mMuonDRPtOutSimplifiedMergedV2Templates[i] = histoDRPtOut;

      oss.str("");    oss << "hMuonPtOutJetPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetPlusSimplifiedMergedV2Templates[i] = histoPlusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVPlusSimplifiedMergedV2Templates[i] = histoPlusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetPlusSimplifiedMergedV2Templates[i] = histoPlusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVPlusSimplifiedMergedV2Templates[i] = histoPlusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVPlusSimplifiedMergedV2Templates[i] = histoPlusPtOutJetYesSV;

      oss.str("");    oss << "hMuonPtOutJetMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetMinusSimplifiedMergedV2Templates[i] = histoMinusPtOutJet;

      oss.str("");    oss << "hMuonPtOutSVMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVMinusSimplifiedMergedV2Templates[i] = histoMinusPtOutSV;

      oss.str("");    oss << "hMuonPtOutSVorJetMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} SV or jet MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSVorJetMinusSimplifiedMergedV2Templates[i] = histoMinusPtOutSVorJet;

      oss.str("");    oss << "hMuonPtOutJetNoSVMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet no SV MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetNoSVMinusSimplifiedMergedV2Templates[i] = histoMinusPtOutJetNoSV;

      oss.str("");    oss << "hMuonPtOutJetYesSVMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet with SV MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetYesSVMinusSimplifiedMergedV2Templates[i] = histoMinusPtOutJetYesSV;
      
      oss.str("");    oss << "hMuonPtOutGenJetSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV2 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSimplifiedMergedV2Templates[i] = histoPtOutGenJet;
      
      oss.str("");    oss << "hMuonDRPtOutGenSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "#Delta(R) p_{T}^{Out} gen SimplifiedMergedV2 template for muon category " << i;    title = oss.str();
      TH1D* histoDRPtOutGen = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu B", "N_{#mu}");
      mMuonDRPtOutGenSimplifiedMergedV2Templates[i] = histoDRPtOutGen;
      
      oss.str("");    oss << "hMuonPtOutGenJetPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet plus SimplifiedMergedV2 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetPlusSimplifiedMergedV2Templates[i] = histoPtOutGenJetPlus;
      
      oss.str("");    oss << "hMuonPtOutGenJetMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet minus SimplifiedMergedV2 template for muon category " << i;    title = oss.str();
      TH1D* histoPtOutGenJetMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetMinusSimplifiedMergedV2Templates[i] = histoPtOutGenJetMinus;

      oss.str("");    oss << "hMuonPtOutJetOSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetOSSimplifiedMergedV2Templates[i] = histoMuonPtOutJetOS;

      oss.str("");    oss << "hMuonPtOutJetSSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSSimplifiedMergedV2Templates[i] = histoMuonPtOutJetSS;

      oss.str("");    oss << "hMuonPtOutJetSSPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSPlusSimplifiedMergedV2Templates[i] = histoMuonPtOutJetSSPlus;

      oss.str("");    oss << "hMuonPtOutJetSSMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutJetSSMinusSimplifiedMergedV2Templates[i] = histoMuonPtOutJetSSMinus;
      
      oss.str("");    oss << "hMuonPtOutGenJetOSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV2Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetOSSimplifiedMergedV2Templates[i] = histoMuonPtOutGenJetOS;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV2Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSSimplifiedMergedV2Templates[i] = histoMuonPtOutGenJetSS;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV2Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSPlusSimplifiedMergedV2Templates[i] = histoMuonPtOutGenJetSSPlus;
      
      oss.str("");    oss << "hMuonPtOutGenJetSSMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} gen jet SimplifiedMergedV2Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonPtOutGenJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutGenJetSSMinusSimplifiedMergedV2Templates[i] = histoMuonPtOutGenJetSSMinus;
      
      oss.str("");    oss << "hMuonPtOutSmearedJetSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet SimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetSimplifiedMergedV2Templates_SV[i] = histoPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV SimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVSimplifiedMergedV2Templates_SV[i] = histoPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet SimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetSimplifiedMergedV2Templates_SV[i] = histoPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusSimplifiedMergedV2Templates_SV[i] = histoPlusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusSimplifiedMergedV2Templates_SV[i] = histoPlusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV2Templates_SV[i] = histoPlusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusSimplifiedMergedV2Templates_SV[i] = histoMinusPtOutSmearedJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusSimplifiedMergedV2Templates_SV[i] = histoMinusPtOutSmearedSV_SV;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusSimplifiedMergedV2Template SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV2Templates_SV[i] = histoMinusPtOutSmearedSVorJet_SV;

      oss.str("");    oss << "hMuonPtOutSmearedJetSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet SimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetSimplifiedMergedV2Templates_NO[i] = histoPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV SimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVSimplifiedMergedV2Templates_NO[i] = histoPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet SimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetSimplifiedMergedV2Templates_NO[i] = histoPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetPlusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet PlusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetPlusSimplifiedMergedV2Templates_NO[i] = histoPlusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVPlusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV PlusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVPlusSimplifiedMergedV2Templates_NO[i] = histoPlusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetPlusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet PlusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoPlusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV2Templates_NO[i] = histoPlusPtOutSmearedSVorJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedJetMinusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared jet MinusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedJetMinusSimplifiedMergedV2Templates_NO[i] = histoMinusPtOutSmearedJet_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVMinusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV MinusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSV_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVMinusSimplifiedMergedV2Templates_NO[i] = histoMinusPtOutSmearedSV_NO;

      oss.str("");    oss << "hMuonPtOutSmearedSVorJetMinusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "p_{T}^{Out} smeared SV or jet MinusSimplifiedMergedV2Template NO SV for muon category " << i;    title = oss.str();
      TH1D* histoMinusPtOutSmearedSVorJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
      mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV2Templates_NO[i] = histoMinusPtOutSmearedSVorJet_NO;
      
      for(short int j = i; j < 5; j++)
      {
        short int index = j<=i?10*j+i:10*i+j;
        
        oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV22DTemplate_" << index;    name=oss.str();
        oss.str("");    oss << "2D p_{T}^{Out} jet SimplifiedMergedV2Template for event category " << index;    title=oss.str();
        TH2D* histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 5., 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "p_{T}^{Out} [GeV/c]");
        mMuonPtOutJetSimplifiedMergedV22DTemplates[index] = histo2D;
        
        oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV22DTemplateOS_" << index;    name=oss.str();
        oss.str("");    oss << "2D p_{T}^{Out} jet SimplifiedMergedV2Template OS for event category " << index;    title=oss.str();
        histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 5., 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "p_{T}^{Out} [GeV/c]");
        mMuonPtOutJetSimplifiedMergedV22DTemplatesOS[index] = histo2D;
        
        oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV22DTemplateSS_" << index;    name=oss.str();
        oss.str("");    oss << "2D p_{T}^{Out} jet SimplifiedMergedV2Template SS for event category " << index;    title=oss.str();
        histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 500, 0., 5., 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "p_{T}^{Out} [GeV/c]");
        mMuonPtOutJetSimplifiedMergedV22DTemplatesSS[index] = histo2D;
        
        if(i==j)
        {
          std::pair<short int, short int> indexPair = std::make_pair(i,index);
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template for muon category " << i << " and event category " << index;    title=oss.str();
          TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateOSByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template OS for muon category " << i << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesOSByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateSSByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template SS for muon category " << i << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesSSByEventCode[indexPair] = histo;
        }
        else
        {
          std::pair<short int, short int> indexPair = std::make_pair(i,index);
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template for muon category " << i << " and event category " << index;    title=oss.str();
          TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateOSByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template OS for muon category " << i << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesOSByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateSSByEventCode_" << i << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template SS for muon category " << i << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesSSByEventCode[indexPair] = histo;
          
          indexPair = std::make_pair(j,index);
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateByEventCode_" << j << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template for muon category " << j << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateOSByEventCode_" << j << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template OS for muon category " << j << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesOSByEventCode[indexPair] = histo;
          
          oss.str("");    oss << "hMuonPtOutJetSimplifiedMergedV2TemplateSSByEventCode_" << j << "_" << index;    name=oss.str();
          oss.str("");    oss << "p_{T}^{Out} jet SimplifiedMergedV2Template SS for muon category " << j << " and event category " << index;    title=oss.str();
          histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5.,"p_{T}^{Out} [GeV/c]", "N_{#mu}");
          mMuonPtOutJetSimplifiedMergedV2TemplatesSSByEventCode[indexPair] = histo;
        }
      }
      
      // Emu
      oss.str("");    oss << "hMuonEmuJetSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSimplifiedMergedV2Templates[i] = histoEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetPlusSimplifiedMergedV2Templates[i] = histoPlusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetMinusSimplifiedMergedV2Templates[i] = histoMinusEmuJet;
      
      oss.str("");    oss << "hMuonEmuJetOSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV2Template for OS muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetOS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetOSSimplifiedMergedV2Templates[i] = histoMuonEmuJetOS;
      
      oss.str("");    oss << "hMuonEmuJetSSSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV2Template for SS muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSS = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSSimplifiedMergedV2Templates[i] = histoMuonEmuJetSS;
      
      oss.str("");    oss << "hMuonEmuJetSSPlusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV2Template for SSPlus muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSSPlus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSPlusSimplifiedMergedV2Templates[i] = histoMuonEmuJetSSPlus;
      
      oss.str("");    oss << "hMuonEmuJetSSMinusSimplifiedMergedV2Template_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV2Template for SSMinus muon category " << i;    title = oss.str();
      TH1D* histoMuonEmuJetSSMinus = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuJetSSMinusSimplifiedMergedV2Templates[i] = histoMuonEmuJetSSMinus;
      
      oss.str("");    oss << "hMuonEmuSmearedJetSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetSimplifiedMergedV2Templates_SV[i] = histoEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusSimplifiedMergedV2Templates_SV[i] = histoPlusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusSimplifiedMergedV2Template_SV_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_SV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusSimplifiedMergedV2Templates_SV[i] = histoMinusEmuSmearedJet_SV;
      
      oss.str("");    oss << "hMuonEmuSmearedJetSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) SimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetSimplifiedMergedV2Templates_NO[i] = histoEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetPlusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) PlusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoPlusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetPlusSimplifiedMergedV2Templates_NO[i] = histoPlusEmuSmearedJet_NO;
      
      oss.str("");    oss << "hMuonEmuSmearedJetMinusSimplifiedMergedV2Template_NO_" << i;    name = oss.str();
      oss.str("");    oss << "E_{#mu}(jet) MinusSimplifiedMergedV2Template for muon category " << i;    title = oss.str();
      TH1D* histoMinusEmuSmearedJet_NO = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
      mMuonEmuSmearedJetMinusSimplifiedMergedV2Templates_NO[i] = histoMinusEmuSmearedJet_NO;
    }


  } /// JetSelectorIsValid()

  // This is to calculate feed-through
  unsigned int nBBEv = 0;
  unsigned int nBBEv02 = 0;
  unsigned int nBBEvOutsideCuts = 0;
  unsigned int nBBEvOutsideCuts02 = 0;
  unsigned int nBBEvInsideCuts = 0;
  unsigned int nBBEvInsideCuts02 = 0;

  // This is to calculate automatically the event counts to be put in the constraints
  // Overall event counts - not divided by event type
  unsigned int nSelEvents = 0; // all events
  unsigned int nSelEvents02 = 0; // dxy < 0.2
  unsigned int nSelEvents50 = 0; // ptRel < 5.0
  unsigned int nSelEvents0250 = 0; // dxy < 0.2 && ptRel < 5.0
  // These maps distinguish between i.e. event class 12 and event class 21
  std::map<unsigned int, unsigned int> mSelEventsMergedIPCodeOrdered; // all events
  std::map<unsigned int, unsigned int> mSelEventsMergedIPCodeOrdered02; // dxy < 0.2
  std::map<unsigned int, unsigned int> mSelEventsMergedIPCodeOrdered50; // ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedIPCodeOrdered0250; // dxy < 0.2 && ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV1Ordered; // all events
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV1Ordered02; // dxy < 0.2
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV1Ordered50; // ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV1Ordered0250; // dxy < 0.2 && ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV2Ordered; // all events
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV2Ordered02; // dxy < 0.2
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV2Ordered50; // ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV2Ordered0250; // dxy < 0.2 && ptRel < 5.0
  // These maps add together the off-diagonal event classes
  std::map<unsigned int, unsigned int> mSelEventsMergedIPCode; // all events
  std::map<unsigned int, unsigned int> mSelEventsMergedIPCode02; // dxy < 0.2
  std::map<unsigned int, unsigned int> mSelEventsMergedIPCode50; // ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedIPCode0250; // dxy < 0.2 && ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV1; // all events
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV102; // dxy < 0.2
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV150; // ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV10250; // dxy < 0.2 && ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV2; // all events
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV202; // dxy < 0.2
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV250; // ptRel < 5.0
  std::map<unsigned int, unsigned int> mSelEventsMergedPtRelCodeV20250; // dxy < 0.2 && ptRel < 5.0

  // Finds pt cut applied
  double ptCut = s->GetMuonPtThreshold();
  
  std::cout << "Sono qui 0\n";

  // Loop over events
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries; jentry++)
  {

    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
    {
      break;
    }
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if(jentry == 0)
    {

      // Build vectors of vectors of maps of templates for different Pdf's
      for(unsigned int iPdfSet = 0; iPdfSet < Genevt_pdfWeights->size(); iPdfSet++)
      {
        std::vector<std::map<short int,TH1D*> > vmMuonDxyzSimplifiedMergedTemplates;
        std::vector<std::map<short int,TH1D*> > vmMuonDxySimplifiedMergedTemplates;
        std::vector<std::map<short int,TH1D*> > vmMuonDzSimplifiedMergedTemplates;
        for(unsigned int iPdfValue = 0; iPdfValue < Genevt_pdfWeights->at(iPdfSet).size(); iPdfValue++)
        {
          std::map<short int,TH1D*> mDxyzSMT;
          std::map<short int,TH1D*> mDxySMT;
          std::map<short int,TH1D*> mDzSMT;
          for(short int i = 0; i < 7; i++)
          {
            oss.str("");
            oss << "hMuonDxyzSimplifiedMergedTemplate_" << pdfWeightNames->at(iPdfSet) << "-" << iPdfValue << "_" << i;
            name = oss.str();
            oss.str("");
            oss << "d_{xyz} template for muon category " << i << " with " <<  pdfWeightNames->at(iPdfSet) << "-" << iPdfValue;
            title = oss.str();
            TH1D* histoDxyz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
            mDxyzSMT[i] = histoDxyz;
            oss.str("");
            oss << "hMuonDxySimplifiedMergedTemplate_" << pdfWeightNames->at(iPdfSet) << "-" << iPdfValue << "_" << i;
            name = oss.str();
            oss.str("");
            oss << "d_{xy} template for muon category " << i << " with " <<  pdfWeightNames->at(iPdfSet) << "-" << iPdfValue;
            title = oss.str();
            TH1D* histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
            mDxySMT[i] = histoDxy;
            oss.str("");
            oss << "hMuonDzSimplifiedMergedTemplate_" << pdfWeightNames->at(iPdfSet) << "-" << iPdfValue << "_" << i;
            name = oss.str();
            oss.str("");
            oss << "d_{z} template for muon category " << i << " with " <<  pdfWeightNames->at(iPdfSet) << "-" << iPdfValue;
            title = oss.str();
            TH1D* histoDz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
            mDzSMT[i] = histoDz;
          }
          vmMuonDxyzSimplifiedMergedTemplates.push_back(mDxyzSMT);
          vmMuonDxySimplifiedMergedTemplates.push_back(mDxySMT);
          vmMuonDzSimplifiedMergedTemplates.push_back(mDzSMT);
        }
        vvmMuonDxyzSimplifiedMergedTemplates.push_back(vmMuonDxyzSimplifiedMergedTemplates);
        vvmMuonDxySimplifiedMergedTemplates.push_back(vmMuonDxySimplifiedMergedTemplates);
        vvmMuonDzSimplifiedMergedTemplates.push_back(vmMuonDzSimplifiedMergedTemplates);
      }
    }

    // Select Muons and Jets according to requirements!
    // Store here selected Muons, Jets and Vtx
    std::vector<unsigned short> vSelectedMuons, vSelectedJets;
    unsigned short iSelectedVertex;

    // Use only default selection
    // Selection string is passed to ParticleSelector
    // since the constructor of TreeAnalyzer*
    if(!s->MuonSelectorIsValid())
    {
      std::cout << "E R R O R ! MuonSelector is not valid!\nExiting..." << std::endl;
      exit(1);
    }

    // Decide if Muon+Jet combined selection or Muon selection only
    if(s->JetSelectorIsValid())
    {
      if(!s->SelectEventWithDiMuonInJetsV0(vSelectedMuons, iSelectedVertex, vSelectedJets))
      {
        continue;
      }
    }
    else
    {
      if(!s->SelectEventWithDiMuonV0(vSelectedMuons, iSelectedVertex))
      {
        continue;
      }
    }

    // Get also trigger selection
    if(!s->SelectEventPassingTrigger("HLT_DoubleMu3"))
//     if(!s->SelectEventPassingTrigger("HLT_L1SingleMuOpen_v6"))
    {
      continue;
    }

    // Codes
    std::vector<short int> muonClass;
    std::vector<unsigned short> muonInAssocGenp;
    std::vector<std::pair<unsigned short, unsigned short> > iStringAndDaughter;
    std::vector<short int> muonMCProvenance;
    std::vector<short int> muonMergedCode;
    std::vector<short int> muonSimplifiedIPCode;
    std::vector<short int> muonSimplifiedPtRelCode;
    std::vector<short int> muonSimplifiedMergedIPCode;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV0;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV1;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV2;

    // Meaning of simplified IP code
    // 0: Fakes
    // 1: B (NO sequentials)
    // 2: Direct C
    // 3: Prompt
    // 4: Decays in flight
    // 5: B to C to mu (sequentials)
    // 6: Punch-through

    // Meaning of simplified PtRel code
    // 0: Fakes
    // 1: B (INCLUDING sequentials)
    // 2: Direct C
    // 3: Prompt
    // 4: Pion decays in flight
    // 5: Kaon decays in flight
    // 6: Punch-through

    // PtRel/PtOut stuff
    std::vector<double> CosThetaJet_PtOut, PtOutJet, PtOutSV, PtOutSVorJet, DRMuonJet_PtOut, CosThetaSV_PtOut, DRMuonSV_PtOut, DRJetSV_PtOut,
                        CosThetaJet_PtRel, PtRelJet, PtRelSV, PtRelSVorJet, DRMuonJet_PtRel, CosThetaSV_PtRel, DRMuonSV_PtRel, DRJetSV_PtRel,
                        SignedDxy,
                        EmuJet,
    // smearing applying SV-present formula
                        CosThetaJetSmearedSV_PtOut, DRMuonJetSmearedSV_PtOut, CosThetaSVSmearedSV_PtOut, DRMuonSVSmearedSV_PtOut, DRJetSVSmearedSV_PtOut,
                        CosThetaJetSmearedSV_PtRel, DRMuonJetSmearedSV_PtRel, CosThetaSVSmearedSV_PtRel, DRMuonSVSmearedSV_PtRel, DRJetSVSmearedSV_PtRel,
                        PtOutJetSmearedSV, PtOutSVSmearedSV, PtOutSVorJetSmearedSV,
                        PtRelJetSmearedSV, PtRelSVSmearedSV, PtRelSVorJetSmearedSV,
                        SignedDxySmearedSV,
                        EmuJetSmearedSV,
    // smearing applying SV-absent formula
                        CosThetaJetSmearedNO_PtOut, DRMuonJetSmearedNO_PtOut, CosThetaSVSmearedNO_PtOut, DRMuonSVSmearedNO_PtOut, DRJetSVSmearedNO_PtOut,
                        CosThetaJetSmearedNO_PtRel, DRMuonJetSmearedNO_PtRel, CosThetaSVSmearedNO_PtRel, DRMuonSVSmearedNO_PtRel, DRJetSVSmearedNO_PtRel,
                        PtOutJetSmearedNO, PtOutSVSmearedNO, PtOutSVorJetSmearedNO,
                        PtRelJetSmearedNO, PtRelSVSmearedNO, PtRelSVorJetSmearedNO,
                        SignedDxySmearedNO,
                        EmuJetSmearedNO,
    // gen-level
                        CosThetaGenJet_PtOut, PtOutGenJet, PtOutGenSV, PtOutGenSVorJet, DRMuonGenJet_PtOut, CosThetaGenSV_PtOut, DRMuonGenSV_PtOut, DRJetGenSV_PtOut,
                        CosThetaGenJet_PtRel, PtRelGenJet, PtRelGenSV, PtRelGenSVorJet, DRMuonGenJet_PtRel, CosThetaGenSV_PtRel, DRMuonGenSV_PtRel, DRJetGenSV_PtRel,
                        SignedDxyGen;

    // IP Stuff
    std::vector<double> vDxyz, vDxy, vDz;
    std::vector<short int> vCharge;

    // Reject event if Muons are not ordered in Pt
    if(Muon_pt->at(vSelectedMuons[0]) < Muon_pt->at(vSelectedMuons[1]))
    {
      std::cout << "E R R O R : Muons are not pt-ordered!\n";
      exit(1);
    }

    // Loop over selected Muons
    for(unsigned short i = 0; i < vSelectedMuons.size(); i++)
    {
      unsigned short iMuon = vSelectedMuons[i];

      muonClass.push_back(FindMuonClassFromMCAssociation(iMuon));
      muonInAssocGenp.push_back(muonInTrkGenp);
      std::pair<unsigned short, unsigned short> iSAndD;
      if(muonInTrkGenp != 65535)
      {
        iSAndD = LookForMotherString(muonInTrkGenp);
      }
      else
      {
        iSAndD = std::make_pair(65535,65535);
      }
      iStringAndDaughter.push_back(iSAndD);
      muonMCProvenance.push_back(FindMuonMCProvenance(iMuon,muonClass[i]));
      muonMergedCode.push_back(MergeMuonClassAndMCProvenance(muonClass[i],muonMCProvenance[i]));

      // Get simplified codes
      muonSimplifiedIPCode.push_back(FindSimplifiedMuonCode(muonMergedCode[i]));
      muonSimplifiedMergedIPCode.push_back(FindSimplifiedMergedMuonCode(muonSimplifiedIPCode[i]));
      muonSimplifiedPtRelCode.push_back(FindSimplifiedMuonPtRelCode(muonMergedCode[i]));
      muonSimplifiedMergedPtRelCodeV0.push_back(FindSimplifiedMergedMuonPtRelCodeV0(muonSimplifiedPtRelCode[i]));
      muonSimplifiedMergedPtRelCodeV1.push_back(FindSimplifiedMergedMuonPtRelCodeV1(muonSimplifiedPtRelCode[i]));
      muonSimplifiedMergedPtRelCodeV2.push_back(FindSimplifiedMergedMuonPtRelCodeV2(muonSimplifiedPtRelCode[i]));

      hOutVsInAssocGenpId->Fill(muonInTrkGenpId,muonOutTrkGenpId);

      // Get IP information from Ntuples
      vDxyz.push_back(Track_dxyz_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));
      vDxy.push_back(Track_dxy_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));
      vDz.push_back(Track_dz_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));
      vCharge.push_back(Track_charge->at(Muon_trackref->at(iMuon)));

      // Calculate PtRel/PtOut information
      if (s->JetSelectorIsValid())
      {
        unsigned short iJet = vSelectedJets[i];

        // Retrieve PV infos
        // Reco
        TVector3 pVtx( PV_x->at(iSelectedVertex), PV_y->at(iSelectedVertex), PV_z->at(iSelectedVertex) );
        // Gen
        TVector3 pGenVtx( Genp_vx->at(0), Genp_vy->at(0), Genp_vz->at(0) );
        
        // Build TLorentzVectors
        // Reco
        TLorentzVector pMuon, pJet;
        pMuon.SetPtEtaPhiM( Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon), constants::muonMass );
        if(s->GetJetType().compare("Tk") == 0)
          pJet.SetPtEtaPhiM( TrackJet_pt->at(iJet), TrackJet_eta->at(iJet), TrackJet_phi->at(iJet), TrackJet_mass->at(iJet) );
        else
          pJet.SetPtEtaPhiM( Jet_pt->at(iJet), Jet_eta->at(iJet), Jet_phi->at(iJet), Jet_mass->at(iJet) );

        // Gen
        TLorentzVector pGenMuon, pGenJet;
//         std::cout << "muonInTrkGenp = " << muonInTrkGenp << std::endl;
        if(muonInTrkGenp != 65535)
        {
          pGenMuon.SetPtEtaPhiM( Genp_pt->at(muonInTrkGenp), Genp_eta->at(muonInTrkGenp), Genp_phi->at(muonInTrkGenp), constants::muonMass );
          pGenJet.SetPtEtaPhiM( Genp_pt->at(iSAndD.second), Genp_eta->at(iSAndD.second), Genp_phi->at(iSAndD.second), Genp_mass->at(iSAndD.second) );
        }
        else
        {
          pGenMuon.SetPtEtaPhiM( 9999, 10, 0, 0);
          pGenJet.SetPtEtaPhiM( 9999, -10, 0, 0);
        }
        
        // Track reference point (corrected for PV)
        // Reco
        TVector3 pTkRef( Track_referencePoint_x->at(Muon_trackref->at(iMuon)) - pVtx.X(),
                         Track_referencePoint_y->at(Muon_trackref->at(iMuon)) - pVtx.Y(),
                         Track_referencePoint_z->at(Muon_trackref->at(iMuon)) - pVtx.Z() );
        // Gen
        TVector3 pGenTkRef;
        if(muonInTrkGenp != 65535)
        {
          pGenTkRef.SetXYZ( Genp_vx->at(muonInTrkGenp) - pGenVtx.X(),
                            Genp_vy->at(muonInTrkGenp) - pGenVtx.Y(),
                            Genp_vz->at(muonInTrkGenp) - pGenVtx.Z() );
        }
        else
        {
          pGenTkRef.SetXYZ( 9999,9999,9999);
        }
        // Use SecVtx if present inside jet
        // Reco
        int iSecVtx = -1;
//         double jetPt=0;
        for (int j = 0; j < numberOfSecondaryVertex; j++)
        {
          if(SV_jetref->at(j) == iJet)
          {
            iSecVtx = j;
            break;
          }
        }

        TLorentzVector pSVtx;
        double flightLength = -9e9;
        double flightLengthError = -9e9;
        if(iSecVtx > -1)
        {
          flightLength = SV_flightDistance->at(iSecVtx);
          flightLengthError = SV_flightDistanceError->at(iSecVtx);
          pSVtx.SetXYZM( SV_flightDirection_x->at(iSecVtx), SV_flightDirection_y->at(iSecVtx), SV_flightDirection_z->at(iSecVtx), SV_mass->at(iSecVtx) );
          pSVtx.SetRho( pJet.Rho() );
        }
        else
        {
          pSVtx.SetXYZM( 9e9, 9e9, 9e9, 9e9 );
        }
        // Gen
        // Take the production vertex of the muon and the mass of the mother particle
        TLorentzVector pGenSVtx;
        if(muonInTrkGenp != 65535)
        {
          unsigned short iMom = Genp_vMom->at(muonInTrkGenp).at(0);
          pGenSVtx.SetXYZM( Genp_vx->at(muonInTrkGenp) - Genp_vx->at(0),
                            Genp_vy->at(muonInTrkGenp) - Genp_vy->at(0),
                            Genp_vz->at(muonInTrkGenp) - Genp_vz->at(0),
                            Genp_mass->at(iMom) );
          pGenSVtx.SetRho( pGenJet.Rho() );
        }
        else
        {
          pGenSVtx.SetXYZM( 9999, -10, 0, 0);
        }
        // Muon not subtracted from the Jet ---> PtRel
        MuonJetContainer contPtRel = MuonJetContainer( pMuon, pJet, pVtx, pTkRef, pSVtx, false );
        CosThetaJet_PtRel.push_back( contPtRel.getCosThetaJet() );
        CosThetaSV_PtRel.push_back( contPtRel.getCosThetaSV() );
        DRMuonJet_PtRel.push_back( contPtRel.getDRMuonJet() );
        PtRelJet.push_back( contPtRel.getPtRelJet() );
        PtRelSV.push_back( contPtRel.getPtRelSV() );
        PtRelSVorJet.push_back( contPtRel.getPtRelSVorJet() );
        SignedDxy.push_back( contPtRel.getSignedDxy() );
        DRMuonSV_PtRel.push_back( contPtRel.getDRMuonSV() );
        DRJetSV_PtRel.push_back( contPtRel.getDRJetSV() );

        // Muon subtracted from the Jet ---> PtOut
        MuonJetContainer contPtOut = MuonJetContainer( pMuon, pJet, pVtx, pTkRef, pSVtx, true );
        CosThetaJet_PtOut.push_back( contPtOut.getCosThetaJet() );
        CosThetaSV_PtOut.push_back( contPtOut.getCosThetaJet() );
        DRMuonJet_PtOut.push_back( contPtOut.getDRMuonJet() );
        PtOutJet.push_back( contPtOut.getPtRelJet() );
        PtOutSVorJet.push_back( contPtOut.getPtRelSVorJet() );
        PtOutSV.push_back( contPtOut.getPtRelSV() );
        DRMuonSV_PtOut.push_back( contPtOut.getDRMuonSV() );
        DRJetSV_PtOut.push_back( contPtOut.getDRJetSV() );
        
        // Variables not dependent to muon-jet subtraction ---> Emu
        EmuJet.push_back( contPtOut.getEmuJet() );

        // Gen muon not subtracted from the gen Jet ---> PtRel
        MuonJetContainer contPtRelGen = MuonJetContainer( pGenMuon, pGenJet, pGenVtx, pGenTkRef, pGenSVtx, false );
        CosThetaGenJet_PtRel.push_back( contPtRelGen.getCosThetaJet() );
        CosThetaGenSV_PtRel.push_back( contPtRelGen.getCosThetaSV() );
        DRMuonGenJet_PtRel.push_back( contPtRelGen.getDRMuonJet() );
        PtRelGenJet.push_back( contPtRelGen.getPtRelJet() );
        PtRelGenSV.push_back( contPtRelGen.getPtRelSV() );
        PtRelGenSVorJet.push_back( contPtRelGen.getPtRelSVorJet() );
        SignedDxyGen.push_back( contPtRelGen.getSignedDxy() );
        DRMuonGenSV_PtRel.push_back( contPtRelGen.getDRMuonSV() );
        DRJetGenSV_PtRel.push_back( contPtRelGen.getDRJetSV() );
        
        // Gen muon subtracted from the gen Jet ---> PtOut
        MuonJetContainer contPtOutGen = MuonJetContainer( pGenMuon, pGenJet, pGenVtx, pGenTkRef, pGenSVtx, true );
        CosThetaGenJet_PtOut.push_back( contPtOutGen.getCosThetaJet() );
        CosThetaGenSV_PtOut.push_back( contPtOutGen.getCosThetaJet() );
        DRMuonGenJet_PtOut.push_back( contPtOutGen.getDRMuonJet() );
        PtOutGenJet.push_back( contPtOutGen.getPtRelJet() );
        PtOutGenSVorJet.push_back( contPtOutGen.getPtRelSVorJet() );
        PtOutGenSV.push_back( contPtOutGen.getPtRelSV() );
        DRMuonGenSV_PtOut.push_back( contPtOutGen.getDRMuonSV() );
        DRJetGenSV_PtOut.push_back( contPtOutGen.getDRJetSV() );
        
        //FIXME! -> Smearing Parameters Hardcoded!!!
        double etaParSV[4] = {0.027717, 1.1937, -0.31044, 0};
        double etaParNO[4] = {0.016271, 1.4785, -0.28137, 0.0006098};
        double phiParSV[4] = {0.023117, 1.1355, -0.38054, 0};
        double phiParNO[4] = {0.017914, 1.4848, -0.35531, 0.0003836};

        double etaSmearSV = etaParSV[0] + etaParSV[1]*exp( etaParSV[2]*pJet.Pt() ) + etaParSV[3]*pJet.Pt();
        double phiSmearSV = phiParSV[0] + phiParSV[1]*exp( phiParSV[2]*pJet.Pt() ) + phiParSV[3]*pJet.Pt();
        double etaSmearNO = etaParNO[0] + etaParNO[1]*exp( etaParNO[2]*pJet.Pt() ) + etaParNO[3]*pJet.Pt();
        double phiSmearNO = phiParNO[0] + phiParNO[1]*exp( phiParNO[2]*pJet.Pt() ) + phiParNO[3]*pJet.Pt();

        if (pJet.Pt() >= 30)
        {
          etaSmearSV = phiSmearSV = etaSmearNO = phiSmearNO = 0.015;
        }

        double etaCorrSV = pJet.Eta() + rndm->Gaus(0, etaSmearSV);
        double phiCorrSV = pJet.Phi() + rndm->Gaus(0, phiSmearSV);
        double etaCorrNO = pJet.Eta() + rndm->Gaus(0, etaSmearNO);
        double phiCorrNO = pJet.Phi() + rndm->Gaus(0, phiSmearNO);

        // NOTE:
        // Given the 2 approaches (use SV smearing everywhere, and use SV/NO differen
        // smearing as complementary strategy), here we have two sets of containers
        // SV) this is used for the everywhere-SV smearing
        // NO) this is used for the complementary subset different smearing

        pJet.SetPtEtaPhiM( pJet.Pt(), etaCorrSV, phiCorrSV, pJet.M() );

        // JetSmearing + Muon not subtracted from the Jet ---> PtRel
        MuonJetContainer contSmearedSVPtRel = MuonJetContainer( pMuon, pJet, pVtx, pTkRef, pSVtx, false );//, true);
        CosThetaJetSmearedSV_PtRel.push_back( contSmearedSVPtRel.getCosThetaJet() );
        CosThetaSVSmearedSV_PtRel.push_back( contSmearedSVPtRel.getCosThetaSV() );
        DRMuonJetSmearedSV_PtRel.push_back( contSmearedSVPtRel.getDRMuonJet() );
        PtRelJetSmearedSV.push_back( contSmearedSVPtRel.getPtRelJet() );
        PtRelSVSmearedSV.push_back( contSmearedSVPtRel.getPtRelSV() );
        PtRelSVorJetSmearedSV.push_back( contSmearedSVPtRel.getPtRelSVorJet() );
        SignedDxySmearedSV.push_back( contSmearedSVPtRel.getSignedDxy() );
        DRMuonSVSmearedSV_PtRel.push_back( contSmearedSVPtRel.getDRMuonSV() );
        DRJetSVSmearedSV_PtRel.push_back( contSmearedSVPtRel.getDRJetSV() );

        // JetSmearing + Muon subtracted from the Jet ---> PtOut
        MuonJetContainer contSmearedSVPtOut = MuonJetContainer( pMuon, pJet, pVtx, pTkRef, pSVtx, true );//, true);
        CosThetaJetSmearedSV_PtOut.push_back( contSmearedSVPtOut.getCosThetaJet() );
        CosThetaSVSmearedSV_PtOut.push_back( contSmearedSVPtOut.getCosThetaJet() );
        DRMuonJetSmearedSV_PtOut.push_back( contSmearedSVPtOut.getDRMuonJet() );
        PtOutJetSmearedSV.push_back( contSmearedSVPtOut.getPtRelJet() );
        PtOutSVorJetSmearedSV.push_back( contSmearedSVPtOut.getPtRelSVorJet() );
        PtOutSVSmearedSV.push_back( contSmearedSVPtOut.getPtRelSV() );
        DRMuonSVSmearedSV_PtOut.push_back( contSmearedSVPtOut.getDRMuonSV() );
        DRJetSVSmearedSV_PtOut.push_back( contSmearedSVPtOut.getDRJetSV() );
        
        // Variables not dependent to muon-jet subtraction ---> Emu
        EmuJetSmearedSV.push_back( contSmearedSVPtOut.getEmuJet() );

        // If NOT SV, then apply the specific smearing, otherwise use the SV smearing
        if (!(iSecVtx > -1 && flightLength/flightLengthError > 4))
        {
          pJet.SetPtEtaPhiM( pJet.Pt(), etaCorrNO, phiCorrNO, pJet.M() );
        }

        // JetSmearing + Muon not subtracted from the Jet ---> PtRel
        MuonJetContainer contSmearedNOPtRel = MuonJetContainer( pMuon, pJet, pVtx, pTkRef, pSVtx, false );//, true);
        CosThetaJetSmearedNO_PtRel.push_back( contSmearedNOPtRel.getCosThetaJet() );
        CosThetaSVSmearedNO_PtRel.push_back( contSmearedNOPtRel.getCosThetaSV() );
        DRMuonJetSmearedNO_PtRel.push_back( contSmearedNOPtRel.getDRMuonJet() );
        PtRelJetSmearedNO.push_back( contSmearedNOPtRel.getPtRelJet() );
        PtRelSVSmearedNO.push_back( contSmearedNOPtRel.getPtRelSV() );
        PtRelSVorJetSmearedNO.push_back( contSmearedNOPtRel.getPtRelSVorJet() );
        SignedDxySmearedNO.push_back( contSmearedNOPtRel.getSignedDxy() );
        DRMuonSVSmearedNO_PtRel.push_back( contSmearedNOPtRel.getDRMuonSV() );
        DRJetSVSmearedNO_PtRel.push_back( contSmearedNOPtRel.getDRJetSV() );

        // JetSmearing + Muon subtracted from the Jet ---> PtOut
        MuonJetContainer contSmearedNOPtOut = MuonJetContainer( pMuon, pJet, pVtx, pTkRef, pSVtx, true );//, true);
        CosThetaJetSmearedNO_PtOut.push_back( contSmearedNOPtOut.getCosThetaJet() );
        CosThetaSVSmearedNO_PtOut.push_back( contSmearedNOPtOut.getCosThetaJet() );
        DRMuonJetSmearedNO_PtOut.push_back( contSmearedNOPtOut.getDRMuonJet() );
        PtOutJetSmearedNO.push_back( contSmearedNOPtOut.getPtRelJet() );
        PtOutSVorJetSmearedNO.push_back( contSmearedNOPtOut.getPtRelSVorJet() );
        PtOutSVSmearedNO.push_back( contSmearedNOPtOut.getPtRelSV() );
        DRMuonSVSmearedNO_PtOut.push_back( contSmearedNOPtOut.getDRMuonSV() );
        DRJetSVSmearedNO_PtOut.push_back( contSmearedNOPtOut.getDRJetSV() );

        // Variables not dependent to muon-jet subtraction ---> Emu
        EmuJetSmearedNO.push_back( contSmearedNOPtOut.getEmuJet() );
        
      } // PtRel/PtOut

    } // Loop over selected muons

    // Muon classes from MC
    if(muonClass.size()==2)
    {
      hMuonClasses->Fill(muonClass[0],muonClass[1]);
      hSingleMuClasses->Fill(muonClass[0]);
      hSingleMuClasses->Fill(muonClass[1]);
    }
    else
    {
      std::cout << "E R R O R: muonClass has !=2 entries! Exiting!\n";
      exit(1);
    }

    // Muon MC provenance
    if(muonMCProvenance.size() == 2)
    {
      hMuonMCProvenance->Fill(muonMCProvenance[0],muonMCProvenance[1]);
      hSingleMuMCProvenance->Fill(muonMCProvenance[0]);
      hSingleMuMCProvenance->Fill(muonMCProvenance[1]);
    }
    else
    {
      std::cout << "muonMCProvenance.size() = " << muonMCProvenance.size() << std::endl;
      std::cout << "E R R O R: muonMCProvenance has != 2 entries! Exiting!\n";
      exit(1);
    }

    // Muon Merged Code
    if(muonMergedCode.size() == 2)
    {
      hMuonMergedCode->Fill(muonMergedCode[0],muonMergedCode[1]);
      hSingleMuMergedCode->Fill(muonMergedCode[0]);
      hSingleMuMergedCode->Fill(muonMergedCode[1]);

      // Muon Merged Code - OPPOSITE SIGN
      if(vCharge[0]!=vCharge[1])
      {
        hMuonMergedCodeOS->Fill(muonMergedCode[0],muonMergedCode[1]);
        hSingleMuMergedCodeOS->Fill(muonMergedCode[0]);
        hSingleMuMergedCodeOS->Fill(muonMergedCode[1]);
      }
      // Muon Merged Code - SAME SIGN	
      else
      {
        hMuonMergedCodeSS->Fill(muonMergedCode[0],muonMergedCode[1]);
        hSingleMuMergedCodeSS->Fill(muonMergedCode[0]);
        hSingleMuMergedCodeSS->Fill(muonMergedCode[1]);
        // Muon Merged Code - SAME SIGN Plus
        if(vCharge[0]>0)
        {
          hMuonMergedCodeSSPlus->Fill(muonMergedCode[0],muonMergedCode[1]);
          hSingleMuMergedCodeSSPlus->Fill(muonMergedCode[0]);
          hSingleMuMergedCodeSSPlus->Fill(muonMergedCode[1]);
        }
        // Muon Merged Code - SAME SIGN Minus	
        else
        {
          hMuonMergedCodeSSMinus->Fill(muonMergedCode[0],muonMergedCode[1]);
          hSingleMuMergedCodeSSMinus->Fill(muonMergedCode[0]);
          hSingleMuMergedCodeSSMinus->Fill(muonMergedCode[1]);
        }
      }

      // Codes for IP < 0.2
      if(vDxy[0]<0.2 && vDxy[1]<0.2)
      {
        hMuonMergedCode02->Fill(muonMergedCode[0],muonMergedCode[1]);
        hSingleMuMergedCode02->Fill(muonMergedCode[0]);
        hSingleMuMergedCode02->Fill(muonMergedCode[1]);      	
      }

      if(s->JetSelectorIsValid())
      {
        if( (vDxy[0]<0.2 && vDxy[1]<0.2) && (PtOutJet[0]<5.0 && PtOutJet[1]<5.0) )
        {
          hMuonMergedCode0250->Fill(muonMergedCode[0],muonMergedCode[1]);
          hSingleMuMergedCode0250->Fill(muonMergedCode[0]);
          hSingleMuMergedCode0250->Fill(muonMergedCode[1]);
        }
        if(PtOutJet[0]<5.0 && PtOutJet[1]<5.0)
        {
          hMuonMergedCode50->Fill(muonMergedCode[0],muonMergedCode[1]);
          hSingleMuMergedCode50->Fill(muonMergedCode[0]);
          hSingleMuMergedCode50->Fill(muonMergedCode[1]);
        }
      }

      for(unsigned int k = 0; k < muonMergedCode.size(); k++)
      {
        // IP information
        mMuonDxyzTemplates[muonMergedCode[k]]->Fill(vDxyz[k]);
        mMuonDxyTemplates[muonMergedCode[k]]->Fill(vDxy[k]);
        mMuonDzTemplates[muonMergedCode[k]]->Fill(vDz[k]);

        // Separate by muon charge
        if(vCharge[k]>0)
        {
          mMuonDxyzPlusTemplates[muonMergedCode[k]]->Fill(vDxyz[k]);
          mMuonDxyPlusTemplates[muonMergedCode[k]]->Fill(vDxy[k]);
          mMuonDzPlusTemplates[muonMergedCode[k]]->Fill(vDz[k]);
        }
        else
        {
          mMuonDxyzMinusTemplates[muonMergedCode[k]]->Fill(vDxyz[k]);
          mMuonDxyMinusTemplates[muonMergedCode[k]]->Fill(vDxy[k]);
          mMuonDzMinusTemplates[muonMergedCode[k]]->Fill(vDz[k]);
        }

        // Shifted ones
        mMuonDxyzTemplates095[muonMergedCode[k]]->Fill(vDxyz[k]*0.95);
        mMuonDxyTemplates095[muonMergedCode[k]]->Fill(vDxy[k]*0.95);
        mMuonDzTemplates095[muonMergedCode[k]]->Fill(vDz[k]*0.95);
        mMuonDxyzTemplates099[muonMergedCode[k]]->Fill(vDxyz[k]*0.99);
        mMuonDxyTemplates099[muonMergedCode[k]]->Fill(vDxy[k]*0.99);
        mMuonDzTemplates099[muonMergedCode[k]]->Fill(vDz[k]*0.99);
        mMuonDxyzTemplates101[muonMergedCode[k]]->Fill(vDxyz[k]*1.01);
        mMuonDxyTemplates101[muonMergedCode[k]]->Fill(vDxy[k]*1.01);
        mMuonDzTemplates101[muonMergedCode[k]]->Fill(vDz[k]*1.01);
        mMuonDxyzTemplates105[muonMergedCode[k]]->Fill(vDxyz[k]*1.05);
        mMuonDxyTemplates105[muonMergedCode[k]]->Fill(vDxy[k]*1.05);
        mMuonDzTemplates105[muonMergedCode[k]]->Fill(vDz[k]*1.05);
      }

      // PtRel/PtOut
      if (s->JetSelectorIsValid())
      {
        /// PtRel
        for (unsigned int k = 0; k < muonMergedCode.size(); k++)
        {
          mMuonPtRelJetTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);
          mMuonPtRelSVorJetTemplates[muonMergedCode[k]]->Fill(PtRelSVorJet[k]);
          mMuonPtRelSVTemplates[muonMergedCode[k]]->Fill(PtRelSV[k]);

          mMuonPtRelSmearedJetTemplates_SV[muonMergedCode[k]]->Fill(PtRelJetSmearedSV[k]);
          mMuonPtRelSmearedSVorJetTemplates_SV[muonMergedCode[k]]->Fill(PtRelSVorJetSmearedSV[k]);
          mMuonPtRelSmearedSVTemplates_SV[muonMergedCode[k]]->Fill(PtRelSVSmearedSV[k]);

          mMuonPtRelSmearedJetTemplates_NO[muonMergedCode[k]]->Fill(PtRelJetSmearedNO[k]);
          mMuonPtRelSmearedSVorJetTemplates_NO[muonMergedCode[k]]->Fill(PtRelSVorJetSmearedNO[k]);
          mMuonPtRelSmearedSVTemplates_NO[muonMergedCode[k]]->Fill(PtRelSVSmearedNO[k]);

          if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);
          else                mMuonPtRelJetNoSVTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);
          mMuonDRPtRelTemplates[muonMergedCode[k]]->Fill(DRMuonJet_PtRel[k]);

          mMuonPtRelGenJetTemplates[muonMergedCode[k]]->Fill(PtRelGenJet[k]);
          mMuonDRPtRelGenTemplates[muonMergedCode[k]]->Fill(DRMuonGenJet_PtRel[k]);
          
          // Separate by muon charge
          if(vCharge[k]>0)
          {
            mMuonPtRelJetPlusTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetPlusTemplates[muonMergedCode[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVPlusTemplates[muonMergedCode[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetPlusTemplates_SV[muonMergedCode[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetPlusTemplates_SV[muonMergedCode[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVPlusTemplates_SV[muonMergedCode[k]]->Fill(PtRelSVSmearedSV[k]);

            mMuonPtRelSmearedJetPlusTemplates_NO[muonMergedCode[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetPlusTemplates_NO[muonMergedCode[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVPlusTemplates_NO[muonMergedCode[k]]->Fill(PtRelSVSmearedNO[k]);

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVPlusTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVPlusTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);
          
            mMuonPtRelGenJetPlusTemplates[muonMergedCode[k]]->Fill(PtRelGenJet[k]);
          }
          else
          {
            mMuonPtRelJetMinusTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetMinusTemplates[muonMergedCode[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVMinusTemplates[muonMergedCode[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetMinusTemplates_SV[muonMergedCode[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetMinusTemplates_SV[muonMergedCode[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVMinusTemplates_SV[muonMergedCode[k]]->Fill(PtRelSVSmearedSV[k]);

            mMuonPtRelSmearedJetMinusTemplates_NO[muonMergedCode[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetMinusTemplates_NO[muonMergedCode[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVMinusTemplates_NO[muonMergedCode[k]]->Fill(PtRelSVSmearedNO[k]);
            
            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVMinusTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVMinusTemplates[muonMergedCode[k]]->Fill(PtRelJet[k]);

            mMuonPtRelGenJetMinusTemplates[muonMergedCode[k]]->Fill(PtRelGenJet[k]);
          } // Separate by muon charge
        }
        
        /// PtOut      
        for (unsigned int k = 0; k < muonMergedCode.size(); k++)
        {
          mMuonPtOutJetTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);
          mMuonPtOutSVorJetTemplates[muonMergedCode[k]]->Fill(PtOutSVorJet[k]);
          mMuonPtOutSVTemplates[muonMergedCode[k]]->Fill(PtOutSV[k]);

          mMuonPtOutSmearedJetTemplates_SV[muonMergedCode[k]]->Fill(PtOutJetSmearedSV[k]);
          mMuonPtOutSmearedSVorJetTemplates_SV[muonMergedCode[k]]->Fill(PtOutSVorJetSmearedSV[k]);
          mMuonPtOutSmearedSVTemplates_SV[muonMergedCode[k]]->Fill(PtOutSVSmearedSV[k]);	

          mMuonPtOutSmearedJetTemplates_NO[muonMergedCode[k]]->Fill(PtOutJetSmearedNO[k]);
          mMuonPtOutSmearedSVorJetTemplates_NO[muonMergedCode[k]]->Fill(PtOutSVorJetSmearedNO[k]);
          mMuonPtOutSmearedSVTemplates_NO[muonMergedCode[k]]->Fill(PtOutSVSmearedNO[k]);	

          if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);
          else                mMuonPtOutJetNoSVTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);
          mMuonDRPtOutTemplates[muonMergedCode[k]]->Fill(DRMuonJet_PtOut[k]);

          mMuonPtOutGenJetTemplates[muonMergedCode[k]]->Fill(PtOutGenJet[k]);
          mMuonDRPtOutGenTemplates[muonMergedCode[k]]->Fill(DRMuonGenJet_PtOut[k]);
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonPtOutJetPlusTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetPlusTemplates[muonMergedCode[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVPlusTemplates[muonMergedCode[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetPlusTemplates_SV[muonMergedCode[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetPlusTemplates_SV[muonMergedCode[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVPlusTemplates_SV[muonMergedCode[k]]->Fill(PtOutSVSmearedSV[k]);	

            mMuonPtOutSmearedJetPlusTemplates_NO[muonMergedCode[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetPlusTemplates_NO[muonMergedCode[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVPlusTemplates_NO[muonMergedCode[k]]->Fill(PtOutSVSmearedNO[k]);	

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVPlusTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVPlusTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetPlusTemplates[muonMergedCode[k]]->Fill(PtOutGenJet[k]);
          }
          else
          {
            mMuonPtOutJetMinusTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetMinusTemplates[muonMergedCode[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVMinusTemplates[muonMergedCode[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetMinusTemplates_SV[muonMergedCode[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetMinusTemplates_SV[muonMergedCode[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVMinusTemplates_SV[muonMergedCode[k]]->Fill(PtOutSVSmearedSV[k]);	

            mMuonPtOutSmearedJetMinusTemplates_NO[muonMergedCode[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetMinusTemplates_NO[muonMergedCode[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVMinusTemplates_NO[muonMergedCode[k]]->Fill(PtOutSVSmearedNO[k]);	

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVMinusTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVMinusTemplates[muonMergedCode[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetMinusTemplates[muonMergedCode[k]]->Fill(PtOutGenJet[k]);
          } // Separate by charge
        }
        /// Emu
        for (unsigned int k = 0; k < muonMergedCode.size(); k++)
        {
          mMuonEmuJetTemplates[muonMergedCode[k]]->Fill(EmuJet[k]);
          mMuonEmuSmearedJetTemplates_SV[muonMergedCode[k]]->Fill(EmuJetSmearedSV[k]);
          mMuonEmuSmearedJetTemplates_NO[muonMergedCode[k]]->Fill(EmuJetSmearedNO[k]);
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonEmuJetPlusTemplates[muonMergedCode[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetPlusTemplates_SV[muonMergedCode[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetPlusTemplates_NO[muonMergedCode[k]]->Fill(EmuJetSmearedNO[k]);
          }
          else
          {
            mMuonEmuJetMinusTemplates[muonMergedCode[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetMinusTemplates_SV[muonMergedCode[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetMinusTemplates_NO[muonMergedCode[k]]->Fill(EmuJetSmearedNO[k]);
          } // Separate by charge
        }        
      } // End of Jet analysis
    }
    else
    {
      std::cout << "muonMergedCode.size() = " << muonMergedCode.size() << std::endl;
      std::cout << "E R R O R: muonMergedCode has != 2 entries! Exiting!\n";
      exit(1);
    }

    // Muon Simplified IP Code and Simplified Merged IP Code
    if(muonSimplifiedIPCode.size() == 2 && muonSimplifiedMergedIPCode.size() == 2)
    {
      hMuonSimplifiedIPCode->Fill(muonSimplifiedIPCode[0],muonSimplifiedIPCode[1]);
      hSingleMuSimplifiedIPCode->Fill(muonSimplifiedIPCode[0]);
      hSingleMuSimplifiedIPCode->Fill(muonSimplifiedIPCode[1]);
      hMuonSimplifiedMergedIPCode->Fill(muonSimplifiedMergedIPCode[0],muonSimplifiedMergedIPCode[1]);
      hSingleMuSimplifiedMergedIPCode->Fill(muonSimplifiedMergedIPCode[0]);
      hSingleMuSimplifiedMergedIPCode->Fill(muonSimplifiedMergedIPCode[1]);

      // Muon Simplified IP Code and Simplified Merged IP Code - OPPOSITE SIGN
      if(vCharge[0]!=vCharge[1])
      {
        hMuonSimplifiedIPCodeOS->Fill(muonSimplifiedIPCode[0],muonSimplifiedIPCode[1]);
        hSingleMuSimplifiedIPCodeOS->Fill(muonSimplifiedIPCode[0]);
        hSingleMuSimplifiedIPCodeOS->Fill(muonSimplifiedIPCode[1]);
        hMuonSimplifiedMergedIPCodeOS->Fill(muonSimplifiedMergedIPCode[0],muonSimplifiedMergedIPCode[1]);
        hSingleMuSimplifiedMergedIPCodeOS->Fill(muonSimplifiedMergedIPCode[0]);
        hSingleMuSimplifiedMergedIPCodeOS->Fill(muonSimplifiedMergedIPCode[1]);
      }
      // Muon Simplified IP Code and Simplified Merged IP Code - SAME SIGN	
      else
      {
        hMuonSimplifiedIPCodeSS->Fill(muonSimplifiedIPCode[0],muonSimplifiedIPCode[1]);
        hSingleMuSimplifiedIPCodeSS->Fill(muonSimplifiedIPCode[0]);
        hSingleMuSimplifiedIPCodeSS->Fill(muonSimplifiedIPCode[1]);
        hMuonSimplifiedMergedIPCodeSS->Fill(muonSimplifiedMergedIPCode[0],muonSimplifiedMergedIPCode[1]);
        hSingleMuSimplifiedMergedIPCodeSS->Fill(muonSimplifiedMergedIPCode[0]);
        hSingleMuSimplifiedMergedIPCodeSS->Fill(muonSimplifiedMergedIPCode[1]);
        // Muon Simplified IP Code and Simplified Merged IP Code - SAME SIGN Plus
        if(vCharge[0]>0)
        {
          hMuonSimplifiedIPCodeSSPlus->Fill(muonSimplifiedIPCode[0],muonSimplifiedIPCode[1]);
          hSingleMuSimplifiedIPCodeSSPlus->Fill(muonSimplifiedIPCode[0]);
          hSingleMuSimplifiedIPCodeSSPlus->Fill(muonSimplifiedIPCode[1]);
          hMuonSimplifiedMergedIPCodeSSPlus->Fill(muonSimplifiedMergedIPCode[0],muonSimplifiedMergedIPCode[1]);
          hSingleMuSimplifiedMergedIPCodeSSPlus->Fill(muonSimplifiedMergedIPCode[0]);
          hSingleMuSimplifiedMergedIPCodeSSPlus->Fill(muonSimplifiedMergedIPCode[1]);
        }
        // Muon Simplified IP Code and Simplified Merged IP Code - SAME SIGN Minus	
        else
        {
          hMuonSimplifiedIPCodeSSMinus->Fill(muonSimplifiedIPCode[0],muonSimplifiedIPCode[1]);
          hSingleMuSimplifiedIPCodeSSMinus->Fill(muonSimplifiedIPCode[0]);
          hSingleMuSimplifiedIPCodeSSMinus->Fill(muonSimplifiedIPCode[1]);
          hMuonSimplifiedMergedIPCodeSSMinus->Fill(muonSimplifiedMergedIPCode[0],muonSimplifiedMergedIPCode[1]);
          hSingleMuSimplifiedMergedIPCodeSSMinus->Fill(muonSimplifiedMergedIPCode[0]);
          hSingleMuSimplifiedMergedIPCodeSSMinus->Fill(muonSimplifiedMergedIPCode[1]);
        }
      }

      // Codes for IP < 0.2
      if(vDxy[0]<0.2 && vDxy[1]<0.2)
      {
        hMuonSimplifiedIPCode02->Fill(muonSimplifiedIPCode[0],muonSimplifiedIPCode[1]);
        hSingleMuSimplifiedIPCode02->Fill(muonSimplifiedIPCode[0]);
        hSingleMuSimplifiedIPCode02->Fill(muonSimplifiedIPCode[1]);      	
        hMuonSimplifiedMergedIPCode02->Fill(muonSimplifiedMergedIPCode[0],muonSimplifiedMergedIPCode[1]);
        hSingleMuSimplifiedMergedIPCode02->Fill(muonSimplifiedMergedIPCode[0]);
        hSingleMuSimplifiedMergedIPCode02->Fill(muonSimplifiedMergedIPCode[1]);      	
      }

      if(s->JetSelectorIsValid())
      {
        if( (vDxy[0]<0.2 && vDxy[1]<0.2) && (PtOutJet[0]<5.0 && PtOutJet[1]<5.0) )
        {
          hMuonSimplifiedIPCode0250->Fill(muonSimplifiedIPCode[0],muonSimplifiedIPCode[1]);
          hSingleMuSimplifiedIPCode0250->Fill(muonSimplifiedIPCode[0]);
          hSingleMuSimplifiedIPCode0250->Fill(muonSimplifiedIPCode[1]);      	
          hMuonSimplifiedMergedIPCode0250->Fill(muonSimplifiedMergedIPCode[0],muonSimplifiedMergedIPCode[1]);
          hSingleMuSimplifiedMergedIPCode0250->Fill(muonSimplifiedMergedIPCode[0]);
          hSingleMuSimplifiedMergedIPCode0250->Fill(muonSimplifiedMergedIPCode[1]);      	
        }
        if(PtOutJet[0]<5.0 && PtOutJet[1]<5.0)
        {
          hMuonSimplifiedIPCode50->Fill(muonSimplifiedIPCode[0],muonSimplifiedIPCode[1]);
          hSingleMuSimplifiedIPCode50->Fill(muonSimplifiedIPCode[0]);
          hSingleMuSimplifiedIPCode50->Fill(muonSimplifiedIPCode[1]);      	
          hMuonSimplifiedMergedIPCode50->Fill(muonSimplifiedMergedIPCode[0],muonSimplifiedMergedIPCode[1]);
          hSingleMuSimplifiedMergedIPCode50->Fill(muonSimplifiedMergedIPCode[0]);
          hSingleMuSimplifiedMergedIPCode50->Fill(muonSimplifiedMergedIPCode[1]);      	
        }
      }

      // Understand the event type
      unsigned int eventSimplifiedIPCodeOrdered = muonSimplifiedIPCode[0]*10 + muonSimplifiedIPCode[1];
      unsigned int eventSimplifiedMergedIPCodeOrdered = muonSimplifiedMergedIPCode[0]*10 + muonSimplifiedMergedIPCode[1];
//       unsigned int eventSimplifiedIPCode;
//       if(muonSimplifiedIPCode[0] <= muonSimplifiedIPCode[1])
//         eventSimplifiedIPCode = muonSimplifiedIPCode[0]*10 + muonSimplifiedIPCode[1];
//       else
//         eventSimplifiedIPCode = muonSimplifiedIPCode[1]*10 + muonSimplifiedIPCode[0];
      unsigned int eventSimplifiedMergedIPCode;
      if(muonSimplifiedMergedIPCode[0] <= muonSimplifiedMergedIPCode[1])
        eventSimplifiedMergedIPCode = muonSimplifiedMergedIPCode[0]*10 + muonSimplifiedMergedIPCode[1];
      else
        eventSimplifiedMergedIPCode = muonSimplifiedMergedIPCode[1]*10 + muonSimplifiedMergedIPCode[0];
#ifdef mgDEBUG
      std::cout << "This event is a " << eventSimplifiedIPCodeOrdered << std::endl;
      if(eventSimplifiedIPCodeOrdered == 11)
      {
        std::cout << "Printing the genparticle record:\n";
        for(unsigned int iGenp = 0; iGenp < Genp_Id->size(); iGenp++)
        {
          std::cout << iGenp << ": Id = " << Genp_Id->at(iGenp);
          if(Genp_vMom->at(iGenp).size() != 0)

            std::cout << ", iMot = ";
          for(unsigned int iMom = 0; iMom != Genp_vMom->at(iGenp).size(); iMom++)
            std::cout << Genp_vMom->at(iGenp).at(iMom) << " ";
          if(Genp_vDau->at(iGenp).size() != 0)
          {
            std::cout << ", iDau = ";
            for(unsigned int iDau = 0; iDau != Genp_vDau->at(iGenp).size(); iDau++)
              std::cout << Genp_vDau->at(iGenp).at(iDau) << " ";
          }
          std::cout << std::endl;
        }
      }
#endif // mgDEBUG

      // Smear the IP templates
      std::vector<double> vDxy_1um, vDxy_3um, vDxy_5um, vDxy_8um, vDxy_10um, vDxy_12um, vDxy_20um;
      for (unsigned int i = 0; i < vDxy.size(); i++)
      {
        vDxy_1um.push_back( fabs( vDxy[i] + rndm->Gaus(0.,0.0001) ) );
        vDxy_3um.push_back( fabs( vDxy[i] + rndm->Gaus(0.,0.0003) ) );
        vDxy_5um.push_back( fabs( vDxy[i] + rndm->Gaus(0.,0.0005) ) );
        vDxy_8um.push_back( fabs( vDxy[i] + rndm->Gaus(0.,0.0008) ) );
        vDxy_10um.push_back( fabs( vDxy[i] + rndm->Gaus(0.,0.0010) ) );
        vDxy_12um.push_back( fabs( vDxy[i] + rndm->Gaus(0.,0.0012) ) );
        vDxy_20um.push_back( fabs( vDxy[i] + rndm->Gaus(0.,0.0020) ) );
      }

      for(unsigned int k = 0; k < muonSimplifiedIPCode.size(); k++)
      {
        // IP information
        mMuonDxyzSimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDxyz[k]);
        mMuonDxySimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDxy[k]);
        mMuonDzSimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDz[k]);

        // Separate by muon charge
        if(vCharge[k]>0)
        {
          mMuonDxyzPlusSimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDxyz[k]);
          mMuonDxyPlusSimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDxy[k]);
          mMuonDzPlusSimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDz[k]);
        }
        else
        {
          mMuonDxyzMinusSimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDxyz[k]);
          mMuonDxyMinusSimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDxy[k]);
          mMuonDzMinusSimplifiedTemplates[muonSimplifiedIPCode[k]]->Fill(vDz[k]);
        }

        // Shifted ones
        mMuonDxyzSimplifiedTemplates095[muonSimplifiedIPCode[k]]->Fill(vDxyz[k]*0.95);
        mMuonDxySimplifiedTemplates095[muonSimplifiedIPCode[k]]->Fill(vDxy[k]*0.95);
        mMuonDzSimplifiedTemplates095[muonSimplifiedIPCode[k]]->Fill(vDz[k]*0.95);
        mMuonDxyzSimplifiedTemplates099[muonSimplifiedIPCode[k]]->Fill(vDxyz[k]*0.99);
        mMuonDxySimplifiedTemplates099[muonSimplifiedIPCode[k]]->Fill(vDxy[k]*0.99);
        mMuonDzSimplifiedTemplates099[muonSimplifiedIPCode[k]]->Fill(vDz[k]*0.99);
        mMuonDxyzSimplifiedTemplates101[muonSimplifiedIPCode[k]]->Fill(vDxyz[k]*1.01);
        mMuonDxySimplifiedTemplates101[muonSimplifiedIPCode[k]]->Fill(vDxy[k]*1.01);
        mMuonDzSimplifiedTemplates101[muonSimplifiedIPCode[k]]->Fill(vDz[k]*1.01);
        mMuonDxyzSimplifiedTemplates105[muonSimplifiedIPCode[k]]->Fill(vDxyz[k]*1.05);
        mMuonDxySimplifiedTemplates105[muonSimplifiedIPCode[k]]->Fill(vDxy[k]*1.05);
        mMuonDzSimplifiedTemplates105[muonSimplifiedIPCode[k]]->Fill(vDz[k]*1.05);

        // Smeared ones
        mMuonDxySimplifiedTemplates_1um[muonSimplifiedIPCode[k]]->Fill(vDxy_1um[k]);
        mMuonDxySimplifiedTemplates_3um[muonSimplifiedIPCode[k]]->Fill(vDxy_3um[k]);
        mMuonDxySimplifiedTemplates_5um[muonSimplifiedIPCode[k]]->Fill(vDxy_5um[k]);
        mMuonDxySimplifiedTemplates_8um[muonSimplifiedIPCode[k]]->Fill(vDxy_8um[k]);
        mMuonDxySimplifiedTemplates_10um[muonSimplifiedIPCode[k]]->Fill(vDxy_10um[k]);
        mMuonDxySimplifiedTemplates_12um[muonSimplifiedIPCode[k]]->Fill(vDxy_12um[k]);
        mMuonDxySimplifiedTemplates_20um[muonSimplifiedIPCode[k]]->Fill(vDxy_20um[k]);

      }

//         mMuonDxySimplifiedTemplates_D6TCor[muonSimplifiedIPCode[0]]->Fill(dxy0, weight0);
//         mMuonDxySimplifiedTemplates_D6TCor[muonSimplifiedIPCode[0]]->Fill(dxy1, weight1);

      std::vector<std::pair<short int, short int> > muonAndEventSimplifiedMergedIPCode;
      for(unsigned int i = 0; i < muonSimplifiedMergedIPCode.size(); i++)
      {
        std::pair<short int, short int> indexPair = std::make_pair(muonSimplifiedMergedIPCode[i],eventSimplifiedMergedIPCode);
        muonAndEventSimplifiedMergedIPCode.push_back(indexPair);
      }
 
      mMuonDxySimplifiedMerged2DTemplates[eventSimplifiedMergedIPCode]->Fill(vDxy[0],vDxy[1],0.5);
      mMuonDxySimplifiedMerged2DTemplates[eventSimplifiedMergedIPCode]->Fill(vDxy[1],vDxy[0],0.5);
      
      mMuonDxySimplifiedMergedTemplatesByEventCode[muonAndEventSimplifiedMergedIPCode[0]]->Fill(vDxy[0]);
      mMuonDxySimplifiedMergedTemplatesByEventCode[muonAndEventSimplifiedMergedIPCode[1]]->Fill(vDxy[1]);

      if(vCharge[0] == vCharge[1])
      {
        mMuonDxySimplifiedMerged2DTemplatesSS[eventSimplifiedMergedIPCode]->Fill(vDxy[0],vDxy[1],0.5);
        mMuonDxySimplifiedMerged2DTemplatesSS[eventSimplifiedMergedIPCode]->Fill(vDxy[1],vDxy[0],0.5);	

        mMuonDxySimplifiedMergedTemplatesSSByEventCode[muonAndEventSimplifiedMergedIPCode[0]]->Fill(vDxy[0]);
        mMuonDxySimplifiedMergedTemplatesSSByEventCode[muonAndEventSimplifiedMergedIPCode[1]]->Fill(vDxy[1]);
      }
      else
      {
        mMuonDxySimplifiedMerged2DTemplatesOS[eventSimplifiedMergedIPCode]->Fill(vDxy[0],vDxy[1],0.5);
        mMuonDxySimplifiedMerged2DTemplatesOS[eventSimplifiedMergedIPCode]->Fill(vDxy[1],vDxy[0],0.5);	

        mMuonDxySimplifiedMergedTemplatesOSByEventCode[muonAndEventSimplifiedMergedIPCode[0]]->Fill(vDxy[0]);
        mMuonDxySimplifiedMergedTemplatesOSByEventCode[muonAndEventSimplifiedMergedIPCode[1]]->Fill(vDxy[1]);
      }

      // Separate by event charge - just for the diagonal case
      if(muonSimplifiedMergedIPCode[0] == 1 && muonSimplifiedMergedIPCode[1] == 1)
      {
        if(vCharge[0] == vCharge[1])
        {
          mMuonDxySimplifiedMergedSSTemplates[6]->Fill(vDxy[0]);
          mMuonDxySimplifiedMergedSSTemplates[6]->Fill(vDxy[1]);
        } 
        else
        {
          mMuonDxySimplifiedMergedOSTemplates[6]->Fill(vDxy[0]);
          mMuonDxySimplifiedMergedOSTemplates[6]->Fill(vDxy[1]);        
        }
      }
      // The other templates remain non-separated
      // TODO: This reuses the same BB events of the case above. Check that this is statistically sound.
      mMuonDxySimplifiedMergedSSTemplates[muonSimplifiedMergedIPCode[0]]->Fill(vDxy[0]);
      mMuonDxySimplifiedMergedSSTemplates[muonSimplifiedMergedIPCode[1]]->Fill(vDxy[1]);
      
      mMuonDxySimplifiedMergedOSTemplates[muonSimplifiedMergedIPCode[0]]->Fill(vDxy[0]);
      mMuonDxySimplifiedMergedOSTemplates[muonSimplifiedMergedIPCode[1]]->Fill(vDxy[1]);        
      
      for(unsigned int k = 0; k < muonSimplifiedMergedIPCode.size(); k++)
      {
        // IP information
        mMuonDxyzSimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDxyz[k]);
        mMuonDxySimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDxy[k]);
        mMuonDzSimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDz[k]);
        
        // Separate by muon charge
        if(vCharge[k]>0)
        {
          mMuonDxyzPlusSimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDxyz[k]);
          mMuonDxyPlusSimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDxy[k]);
          mMuonDzPlusSimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDz[k]);
        }
        else
        {
          mMuonDxyzMinusSimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDxyz[k]);
          mMuonDxyMinusSimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDxy[k]);
          mMuonDzMinusSimplifiedMergedTemplates[muonSimplifiedMergedIPCode[k]]->Fill(vDz[k]);
        }

        // Shifted ones
        mMuonDxyzSimplifiedMergedTemplates095[muonSimplifiedMergedIPCode[k]]->Fill(vDxyz[k]*0.95);
        mMuonDxySimplifiedMergedTemplates095[muonSimplifiedMergedIPCode[k]]->Fill(vDxy[k]*0.95);
        mMuonDzSimplifiedMergedTemplates095[muonSimplifiedMergedIPCode[k]]->Fill(vDz[k]*0.95);
        mMuonDxyzSimplifiedMergedTemplates099[muonSimplifiedMergedIPCode[k]]->Fill(vDxyz[k]*0.99);
        mMuonDxySimplifiedMergedTemplates099[muonSimplifiedMergedIPCode[k]]->Fill(vDxy[k]*0.99);
        mMuonDzSimplifiedMergedTemplates099[muonSimplifiedMergedIPCode[k]]->Fill(vDz[k]*0.99);
        mMuonDxyzSimplifiedMergedTemplates101[muonSimplifiedMergedIPCode[k]]->Fill(vDxyz[k]*1.01);
        mMuonDxySimplifiedMergedTemplates101[muonSimplifiedMergedIPCode[k]]->Fill(vDxy[k]*1.01);
        mMuonDzSimplifiedMergedTemplates101[muonSimplifiedMergedIPCode[k]]->Fill(vDz[k]*1.01);
        mMuonDxyzSimplifiedMergedTemplates105[muonSimplifiedMergedIPCode[k]]->Fill(vDxyz[k]*1.05);
        mMuonDxySimplifiedMergedTemplates105[muonSimplifiedMergedIPCode[k]]->Fill(vDxy[k]*1.05);
        mMuonDzSimplifiedMergedTemplates105[muonSimplifiedMergedIPCode[k]]->Fill(vDz[k]*1.05);

        // Smeared ones
        mMuonDxySimplifiedMergedTemplates_1um[muonSimplifiedMergedIPCode[k]]->Fill(vDxy_1um[k]);
        mMuonDxySimplifiedMergedTemplates_3um[muonSimplifiedMergedIPCode[k]]->Fill(vDxy_3um[k]);
        mMuonDxySimplifiedMergedTemplates_5um[muonSimplifiedMergedIPCode[k]]->Fill(vDxy_5um[k]);
        mMuonDxySimplifiedMergedTemplates_8um[muonSimplifiedMergedIPCode[k]]->Fill(vDxy_8um[k]);
        mMuonDxySimplifiedMergedTemplates_10um[muonSimplifiedMergedIPCode[k]]->Fill(vDxy_10um[k]);
        mMuonDxySimplifiedMergedTemplates_12um[muonSimplifiedMergedIPCode[k]]->Fill(vDxy_12um[k]);
        mMuonDxySimplifiedMergedTemplates_20um[muonSimplifiedMergedIPCode[k]]->Fill(vDxy_20um[k]);
      }

//         mMuonDxySimplifiedMergedTemplates_D6TCor[muonSimplifiedMergedIPCode[0]]->Fill(dxy0, weight0);
//         mMuonDxySimplifiedMergedTemplates_D6TCor[muonSimplifiedMergedIPCode[0]]->Fill(dxy1, weight1);

      // Vectors of vectors of maps
      // for PDF's
      for(unsigned int iPdfSet = 0; iPdfSet < Genevt_pdfWeights->size(); iPdfSet++)
      {
        for(unsigned int iPdfValue = 0; iPdfValue < Genevt_pdfWeights->at(iPdfSet).size(); iPdfValue++)
        {
          double pdfWeight = Genevt_pdfWeights->at(iPdfSet).at(iPdfValue);
          for(unsigned int k = 0; k < muonSimplifiedMergedIPCode.size(); k++)
          {
            vvmMuonDxyzSimplifiedMergedTemplates[iPdfSet][iPdfValue][muonSimplifiedMergedIPCode[k]]->Fill(vDxyz[k],pdfWeight);
            vvmMuonDxySimplifiedMergedTemplates[iPdfSet][iPdfValue][muonSimplifiedMergedIPCode[k]]->Fill(vDxy[k],pdfWeight);
            vvmMuonDzSimplifiedMergedTemplates[iPdfSet][iPdfValue][muonSimplifiedMergedIPCode[k]]->Fill(vDz[k],pdfWeight);
          }
        }
      }

      // Feed-through
      if(eventSimplifiedIPCodeOrdered == 11)
      {
        int genp0 = muonInAssocGenp[0];
        int genp1 = muonInAssocGenp[1];
        nBBEv++;
        if(vDxy[0]<0.2 && vDxy[1]<0.2)
          nBBEv02++;
        if(genp0 == 65535 || genp1 == 65535)
        {
          nBBEvOutsideCuts++;
          if(vDxy[0]<0.2 && vDxy[1]<0.2)
            nBBEvOutsideCuts02++;
        }
        else
        {
          if(Genp_pt->at(genp0) < ptCut || abs(Genp_eta->at(genp0)) > 2.1 || Genp_pt->at(genp1) < ptCut || abs(Genp_eta->at(genp1)) > 2.1 )
          {
            nBBEvOutsideCuts++;
            if(vDxy[0]<0.2 && vDxy[1]<0.2)
              nBBEvOutsideCuts02++;
          }
          else
          {
            nBBEvInsideCuts++;
            if(vDxy[0]<0.2 && vDxy[1]<0.2)
              nBBEvInsideCuts02++;
          }
        }
      } // eventSimplifiedIPCodeOrdered == 11

      // Count events
      std::pair<short int, short int> eventSimplifiedMergedIPCodeIndex;
      if(abs(muonSimplifiedMergedIPCode[0]) <= abs(muonSimplifiedMergedIPCode[1]))
      {
        eventSimplifiedMergedIPCodeIndex = std::make_pair(abs(muonSimplifiedMergedIPCode[0]), abs(muonSimplifiedMergedIPCode[1]));
      }
      else
      {
        eventSimplifiedMergedIPCodeIndex = std::make_pair(abs(muonSimplifiedMergedIPCode[1]), abs(muonSimplifiedMergedIPCode[0]));
      }
      std::pair<short int, short int> eventSimplifiedMergedPtRelCodeV1Index;
      if(abs(muonSimplifiedMergedPtRelCodeV1[0]) <= abs(muonSimplifiedMergedPtRelCodeV1[1]))
      {
        eventSimplifiedMergedPtRelCodeV1Index = std::make_pair(abs(muonSimplifiedMergedPtRelCodeV1[0]), abs(muonSimplifiedMergedPtRelCodeV1[1]));
      }
      else
      {
        eventSimplifiedMergedPtRelCodeV1Index = std::make_pair(abs(muonSimplifiedMergedPtRelCodeV1[1]), abs(muonSimplifiedMergedPtRelCodeV1[0]));
      }
      std::pair<short int, short int> eventSimplifiedMergedPtRelCodeV2Index;
      if(abs(muonSimplifiedMergedPtRelCodeV2[0]) <= abs(muonSimplifiedMergedPtRelCodeV2[1]))
      {
        eventSimplifiedMergedPtRelCodeV2Index = std::make_pair(abs(muonSimplifiedMergedPtRelCodeV2[0]), abs(muonSimplifiedMergedPtRelCodeV2[1]));
      }
      else
      {
        eventSimplifiedMergedPtRelCodeV2Index = std::make_pair(abs(muonSimplifiedMergedPtRelCodeV2[1]), abs(muonSimplifiedMergedPtRelCodeV2[0]));
      }
      double maxDxy = 0;
      double maxPtRelJet = 0;
      double maxPtOutJet = 0;
      double maxEMuJet = 0;
      if(vDxy[0] > vDxy[1])
      {
        maxDxy = vDxy[0];
      }
      else
      {
        maxDxy = vDxy[1];
      }

      if(s->JetSelectorIsValid())
      {
        if(PtRelJet[0] > PtRelJet[1])
        {
          maxPtRelJet = PtRelJet[0];
        }
        else
        {
          maxPtRelJet = PtRelJet[1];
        }
        
        if(PtOutJet[0] > PtOutJet[1])
        {
          maxPtOutJet = PtOutJet[0];
        }
        else
        {
          maxPtOutJet = PtOutJet[1];
        }
        
        if(EmuJet[0] > EmuJet[1])
        {
          maxEMuJet = EmuJet[0];
        }
        else
        {
          maxEMuJet = EmuJet[1];
        }
      }
      
      mNEventsVsMaxDxyAndPtRelJetSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtRelJet);
      mNEventsVsMaxDxyAndPtOutJetSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtOutJet);
      mNEventsVsMaxDxyAndEMuJetSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxEMuJet);

      mNEventsVsMaxDxyAndPtRelJetSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtRelJet);
      mNEventsVsMaxDxyAndPtOutJetSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtOutJet);
      mNEventsVsMaxDxyAndEMuJetSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxEMuJet);
      
      mNEventsVsMaxDxyAndPtRelJetSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtRelJet);
      mNEventsVsMaxDxyAndPtOutJetSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtOutJet);
      mNEventsVsMaxDxyAndEMuJetSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxEMuJet);
      
      // Mapping for SimplifiedMergedOS and SimplifiedMergedSS classification works in this way:
      // BB -> 1,1
      // BB -> 6,6 (yes, BOTH 1,1 and 6,6: this means that if template 66 and template 11 are used in the same fit, unitarity is not conserved!)
      // BX, XY -> n,m according to standard SimplifiedMerged classification
      if(vCharge[0]!=vCharge[1])
      {
        mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtRelJet);
        mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtOutJet);
        mNEventsVsMaxDxyAndEMuJetOSSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxEMuJet);

        mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedOS[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtRelJet);
        mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedOS[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtOutJet);
        mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedOS[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxEMuJet);
        
        if(eventSimplifiedMergedIPCodeIndex.first == 1 && eventSimplifiedMergedIPCodeIndex.second == 1)
        {
          std::pair<short int, short int> newIndex = std::make_pair(6,6);
          mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedOS[newIndex]->Fill(maxDxy, maxPtRelJet);
          mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedOS[newIndex]->Fill(maxDxy, maxPtOutJet);
          mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedOS[newIndex]->Fill(maxDxy, maxEMuJet);
        }
        
        mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtRelJet);
        mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtOutJet);
        mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxEMuJet);

        mNEventsVsMaxDxyAndPtRelJetOSSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtRelJet);
        mNEventsVsMaxDxyAndPtOutJetOSSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtOutJet);
        mNEventsVsMaxDxyAndEMuJetOSSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxEMuJet);
      }
      else
      {
        mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtRelJet);
        mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtOutJet);
        mNEventsVsMaxDxyAndEMuJetSSSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxEMuJet);

        mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedSS[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtRelJet);
        mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedSS[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtOutJet);
        mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedSS[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxEMuJet);
        
        if(eventSimplifiedMergedIPCodeIndex.first == 1 && eventSimplifiedMergedIPCodeIndex.second == 1)
        {
          std::pair<short int, short int> newIndex = std::make_pair(6,6);
          mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedSS[newIndex]->Fill(maxDxy, maxPtRelJet);
          mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedSS[newIndex]->Fill(maxDxy, maxPtOutJet);
          mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedSS[newIndex]->Fill(maxDxy, maxEMuJet);
        }
        
        mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtRelJet);
        mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtOutJet);
        mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxEMuJet);

        mNEventsVsMaxDxyAndPtRelJetSSSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtRelJet);
        mNEventsVsMaxDxyAndPtOutJetSSSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtOutJet);
        mNEventsVsMaxDxyAndEMuJetSSSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxEMuJet);

        if(vCharge[0] > 0)
        {
          mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtRelJet);
          mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtOutJet);
          mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxEMuJet);
          
          mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtRelJet);
          mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtOutJet);
          mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxEMuJet);
          
          mNEventsVsMaxDxyAndPtRelJetSSPlusSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtRelJet);
          mNEventsVsMaxDxyAndPtOutJetSSPlusSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtOutJet);
          mNEventsVsMaxDxyAndEMuJetSSPlusSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxEMuJet);
        }
        else
        {
          mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtRelJet);
          mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxPtOutJet);
          mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMerged[eventSimplifiedMergedIPCodeIndex]->Fill(maxDxy, maxEMuJet);

          mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtRelJet);
          mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxPtOutJet);
          mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMergedV1[eventSimplifiedMergedPtRelCodeV1Index]->Fill(maxDxy, maxEMuJet);

          mNEventsVsMaxDxyAndPtRelJetSSMinusSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtRelJet);
          mNEventsVsMaxDxyAndPtOutJetSSMinusSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxPtOutJet);
          mNEventsVsMaxDxyAndEMuJetSSMinusSimplifiedMergedV2[eventSimplifiedMergedPtRelCodeV2Index]->Fill(maxDxy, maxEMuJet);
        }
      }
        
      nSelEvents++;
      mSelEventsMergedIPCodeOrdered.find(eventSimplifiedMergedIPCodeOrdered) != mSelEventsMergedIPCodeOrdered.end() ?
        mSelEventsMergedIPCodeOrdered[eventSimplifiedMergedIPCodeOrdered]++ :
        mSelEventsMergedIPCodeOrdered[eventSimplifiedMergedIPCodeOrdered] = 1;
      mSelEventsMergedIPCode.find(eventSimplifiedMergedIPCode) != mSelEventsMergedIPCode.end() ?
        mSelEventsMergedIPCode[eventSimplifiedMergedIPCode]++ :
        mSelEventsMergedIPCode[eventSimplifiedMergedIPCode] = 1;
      if(vDxy[0]<0.2 && vDxy[1]<0.2)
      {
        nSelEvents02++;
        mSelEventsMergedIPCodeOrdered02.find(eventSimplifiedMergedIPCodeOrdered) != mSelEventsMergedIPCodeOrdered02.end() ?
          mSelEventsMergedIPCodeOrdered02[eventSimplifiedMergedIPCodeOrdered]++ :
          mSelEventsMergedIPCodeOrdered02[eventSimplifiedMergedIPCodeOrdered] = 1;
        mSelEventsMergedIPCode02.find(eventSimplifiedMergedIPCode) != mSelEventsMergedIPCode02.end() ?
          mSelEventsMergedIPCode02[eventSimplifiedMergedIPCode]++ :
          mSelEventsMergedIPCode02[eventSimplifiedMergedIPCode] = 1;
      }
      if(s->JetSelectorIsValid())
      {
        if(PtRelJet[0] < 5.0 && PtRelJet[1] < 5.0)
        {
          nSelEvents50++;
          mSelEventsMergedIPCodeOrdered50.find(eventSimplifiedMergedIPCodeOrdered) != mSelEventsMergedIPCodeOrdered50.end() ?
            mSelEventsMergedIPCodeOrdered50[eventSimplifiedMergedIPCodeOrdered]++ :
            mSelEventsMergedIPCodeOrdered50[eventSimplifiedMergedIPCodeOrdered] = 1;
          mSelEventsMergedIPCode50.find(eventSimplifiedMergedIPCode) != mSelEventsMergedIPCode50.end() ?
            mSelEventsMergedIPCode50[eventSimplifiedMergedIPCode]++ :
            mSelEventsMergedIPCode50[eventSimplifiedMergedIPCode] = 1;
        }
        if(vDxy[0]<0.2 && vDxy[1]<0.2 && PtRelJet[0] < 5.0 && PtRelJet[1] < 5.0)
        {
          nSelEvents0250++;
          mSelEventsMergedIPCodeOrdered0250.find(eventSimplifiedMergedIPCodeOrdered) != mSelEventsMergedIPCodeOrdered0250.end() ?
            mSelEventsMergedIPCodeOrdered0250[eventSimplifiedMergedIPCodeOrdered]++ :
            mSelEventsMergedIPCodeOrdered0250[eventSimplifiedMergedIPCodeOrdered] = 1;
          mSelEventsMergedIPCode0250.find(eventSimplifiedMergedIPCode) != mSelEventsMergedIPCode0250.end() ?
            mSelEventsMergedIPCode0250[eventSimplifiedMergedIPCode]++ :
            mSelEventsMergedIPCode0250[eventSimplifiedMergedIPCode] = 1;
        }
      }
    }
    else
    {
      std::cout << "muonSimplifiedIPCode.size() = " << muonSimplifiedIPCode.size() << std::endl;
      std::cout << "muonSimplifiedMergedIPCode.size() = " << muonSimplifiedMergedIPCode.size() << std::endl;
      std::cout << "E R R O R: muonSimplifiedIPCode and/or muonSimplifiedMergedIPCode have != 2 entries! Exiting!\n";
      exit(1);
    }
    
    // PtRel/PtOut
    if (s->JetSelectorIsValid())
    {
      // Muon Simplified PtRel Code and Simplified Merged PtRel Code
      if(muonSimplifiedPtRelCode.size() == 2 && muonSimplifiedMergedPtRelCodeV0.size() == 2 &&
        muonSimplifiedMergedPtRelCodeV1.size() == 2 &&
        muonSimplifiedMergedPtRelCodeV2.size() == 2)
      {
        // Understand the event type
//         unsigned int eventSimplifiedPtRelCodeOrdered = muonSimplifiedPtRelCode[0]*10 + muonSimplifiedPtRelCode[1];
        unsigned int eventSimplifiedMergedPtRelCodeV1Ordered = muonSimplifiedMergedPtRelCodeV1[0]*10 + muonSimplifiedMergedPtRelCodeV1[1];
        unsigned int eventSimplifiedMergedPtRelCodeV2Ordered = muonSimplifiedMergedPtRelCodeV2[0]*10 + muonSimplifiedMergedPtRelCodeV2[1];
//         unsigned int eventSimplifiedPtRelCode;
//         if(muonSimplifiedPtRelCode[0] <= muonSimplifiedPtRelCode[1])
//           eventSimplifiedPtRelCode = muonSimplifiedPtRelCode[0]*10 + muonSimplifiedPtRelCode[1];
//         else
//           eventSimplifiedPtRelCode = muonSimplifiedPtRelCode[1]*10 + muonSimplifiedPtRelCode[0];
        unsigned int eventSimplifiedMergedPtRelCodeV1;
        if(muonSimplifiedMergedPtRelCodeV1[0] <= muonSimplifiedMergedPtRelCodeV1[1])
          eventSimplifiedMergedPtRelCodeV1 = muonSimplifiedMergedPtRelCodeV1[0]*10 + muonSimplifiedMergedPtRelCodeV1[1];
        else
          eventSimplifiedMergedPtRelCodeV1 = muonSimplifiedMergedPtRelCodeV1[1]*10 + muonSimplifiedMergedPtRelCodeV1[0];
        unsigned int eventSimplifiedMergedPtRelCodeV2;
        if(muonSimplifiedMergedPtRelCodeV2[0] <= muonSimplifiedMergedPtRelCodeV2[1])
          eventSimplifiedMergedPtRelCodeV2 = muonSimplifiedMergedPtRelCodeV2[0]*10 + muonSimplifiedMergedPtRelCodeV2[1];
        else
          eventSimplifiedMergedPtRelCodeV2 = muonSimplifiedMergedPtRelCodeV2[1]*10 + muonSimplifiedMergedPtRelCodeV2[0];

        hMuonSimplifiedPtRelCode->Fill(muonSimplifiedPtRelCode[0],muonSimplifiedPtRelCode[1]);
        hSingleMuSimplifiedPtRelCode->Fill(muonSimplifiedPtRelCode[0]);
        hSingleMuSimplifiedPtRelCode->Fill(muonSimplifiedPtRelCode[1]);
        hMuonSimplifiedMergedPtRelCodeV0->Fill(muonSimplifiedMergedPtRelCodeV0[0],muonSimplifiedMergedPtRelCodeV0[1]);
        hSingleMuSimplifiedMergedPtRelCodeV0->Fill(muonSimplifiedMergedPtRelCodeV0[0]);
        hSingleMuSimplifiedMergedPtRelCodeV0->Fill(muonSimplifiedMergedPtRelCodeV0[1]);
        hMuonSimplifiedMergedPtRelCodeV1->Fill(muonSimplifiedMergedPtRelCodeV1[0],muonSimplifiedMergedPtRelCodeV1[1]);
        hSingleMuSimplifiedMergedPtRelCodeV1->Fill(muonSimplifiedMergedPtRelCodeV1[0]);
        hSingleMuSimplifiedMergedPtRelCodeV1->Fill(muonSimplifiedMergedPtRelCodeV1[1]);
        hMuonSimplifiedMergedPtRelCodeV2->Fill(muonSimplifiedMergedPtRelCodeV2[0],muonSimplifiedMergedPtRelCodeV2[1]);
        hSingleMuSimplifiedMergedPtRelCodeV2->Fill(muonSimplifiedMergedPtRelCodeV2[0]);
        hSingleMuSimplifiedMergedPtRelCodeV2->Fill(muonSimplifiedMergedPtRelCodeV2[1]);

        // Muon Simplified PtRel Code and Simplified Merged PtRel Code - OPPOSITE SIGN
        if(vCharge[0]!=vCharge[1])
        {
          hMuonSimplifiedPtRelCodeOS->Fill(muonSimplifiedPtRelCode[0],muonSimplifiedPtRelCode[1]);
          hSingleMuSimplifiedPtRelCodeOS->Fill(muonSimplifiedPtRelCode[0]);
          hSingleMuSimplifiedPtRelCodeOS->Fill(muonSimplifiedPtRelCode[1]);
          hMuonSimplifiedMergedPtRelCodeOSV0->Fill(muonSimplifiedMergedPtRelCodeV0[0],muonSimplifiedMergedPtRelCodeV0[1]);
          hSingleMuSimplifiedMergedPtRelCodeOSV0->Fill(muonSimplifiedMergedPtRelCodeV0[0]);
          hSingleMuSimplifiedMergedPtRelCodeOSV0->Fill(muonSimplifiedMergedPtRelCodeV0[1]);
          hMuonSimplifiedMergedPtRelCodeOSV1->Fill(muonSimplifiedMergedPtRelCodeV1[0],muonSimplifiedMergedPtRelCodeV1[1]);
          hSingleMuSimplifiedMergedPtRelCodeOSV1->Fill(muonSimplifiedMergedPtRelCodeV1[0]);
          hSingleMuSimplifiedMergedPtRelCodeOSV1->Fill(muonSimplifiedMergedPtRelCodeV1[1]);
          hMuonSimplifiedMergedPtRelCodeOSV2->Fill(muonSimplifiedMergedPtRelCodeV2[0],muonSimplifiedMergedPtRelCodeV2[1]);
          hSingleMuSimplifiedMergedPtRelCodeOSV2->Fill(muonSimplifiedMergedPtRelCodeV2[0]);
          hSingleMuSimplifiedMergedPtRelCodeOSV2->Fill(muonSimplifiedMergedPtRelCodeV2[1]);
        }
        // Muon Simplified PtRel Code and Simplified Merged PtRel Code - SAME SIGN	
        else
        {
          hMuonSimplifiedPtRelCodeSS->Fill(muonSimplifiedPtRelCode[0],muonSimplifiedPtRelCode[1]);
          hSingleMuSimplifiedPtRelCodeSS->Fill(muonSimplifiedPtRelCode[0]);
          hSingleMuSimplifiedPtRelCodeSS->Fill(muonSimplifiedPtRelCode[1]);
          hMuonSimplifiedMergedPtRelCodeSSV0->Fill(muonSimplifiedMergedPtRelCodeV0[0],muonSimplifiedMergedPtRelCodeV0[1]);
          hSingleMuSimplifiedMergedPtRelCodeSSV0->Fill(muonSimplifiedMergedPtRelCodeV0[0]);
          hSingleMuSimplifiedMergedPtRelCodeSSV0->Fill(muonSimplifiedMergedPtRelCodeV0[1]);
          hMuonSimplifiedMergedPtRelCodeSSV1->Fill(muonSimplifiedMergedPtRelCodeV1[0],muonSimplifiedMergedPtRelCodeV1[1]);
          hSingleMuSimplifiedMergedPtRelCodeSSV1->Fill(muonSimplifiedMergedPtRelCodeV1[0]);
          hSingleMuSimplifiedMergedPtRelCodeSSV1->Fill(muonSimplifiedMergedPtRelCodeV1[1]);
          hMuonSimplifiedMergedPtRelCodeSSV2->Fill(muonSimplifiedMergedPtRelCodeV2[0],muonSimplifiedMergedPtRelCodeV2[1]);
          hSingleMuSimplifiedMergedPtRelCodeSSV2->Fill(muonSimplifiedMergedPtRelCodeV2[0]);
          hSingleMuSimplifiedMergedPtRelCodeSSV2->Fill(muonSimplifiedMergedPtRelCodeV2[1]);
          // Muon Simplified PtRel Code and Simplified Merged PtRel Code - SAME SIGN Plus
          if(vCharge[0]>0)
          {
            hMuonSimplifiedPtRelCodeSSPlus->Fill(muonSimplifiedPtRelCode[0],muonSimplifiedPtRelCode[1]);
            hSingleMuSimplifiedPtRelCodeSSPlus->Fill(muonSimplifiedPtRelCode[0]);
            hSingleMuSimplifiedPtRelCodeSSPlus->Fill(muonSimplifiedPtRelCode[1]);
            hMuonSimplifiedMergedPtRelCodeSSPlusV0->Fill(muonSimplifiedMergedPtRelCodeV0[0],muonSimplifiedMergedPtRelCodeV0[1]);
            hSingleMuSimplifiedMergedPtRelCodeSSPlusV0->Fill(muonSimplifiedMergedPtRelCodeV0[0]);
            hSingleMuSimplifiedMergedPtRelCodeSSPlusV0->Fill(muonSimplifiedMergedPtRelCodeV0[1]);
            hMuonSimplifiedMergedPtRelCodeSSPlusV1->Fill(muonSimplifiedMergedPtRelCodeV1[0],muonSimplifiedMergedPtRelCodeV1[1]);
            hSingleMuSimplifiedMergedPtRelCodeSSPlusV1->Fill(muonSimplifiedMergedPtRelCodeV1[0]);
            hSingleMuSimplifiedMergedPtRelCodeSSPlusV1->Fill(muonSimplifiedMergedPtRelCodeV1[1]);
            hMuonSimplifiedMergedPtRelCodeSSPlusV2->Fill(muonSimplifiedMergedPtRelCodeV2[0],muonSimplifiedMergedPtRelCodeV2[1]);
            hSingleMuSimplifiedMergedPtRelCodeSSPlusV2->Fill(muonSimplifiedMergedPtRelCodeV2[0]);
            hSingleMuSimplifiedMergedPtRelCodeSSPlusV2->Fill(muonSimplifiedMergedPtRelCodeV2[1]);
          }
          // Muon Simplified PtRel Code and Simplified Merged PtRel Code - SAME SIGN Minus	
          else
          {
            hMuonSimplifiedPtRelCodeSSMinus->Fill(muonSimplifiedPtRelCode[0],muonSimplifiedPtRelCode[1]);
            hSingleMuSimplifiedPtRelCodeSSMinus->Fill(muonSimplifiedPtRelCode[0]);
            hSingleMuSimplifiedPtRelCodeSSMinus->Fill(muonSimplifiedPtRelCode[1]);
            hMuonSimplifiedMergedPtRelCodeSSMinusV0->Fill(muonSimplifiedMergedPtRelCodeV0[0],muonSimplifiedMergedPtRelCodeV0[1]);
            hSingleMuSimplifiedMergedPtRelCodeSSMinusV0->Fill(muonSimplifiedMergedPtRelCodeV0[0]);
            hSingleMuSimplifiedMergedPtRelCodeSSMinusV0->Fill(muonSimplifiedMergedPtRelCodeV0[1]);
            hMuonSimplifiedMergedPtRelCodeSSMinusV1->Fill(muonSimplifiedMergedPtRelCodeV1[0],muonSimplifiedMergedPtRelCodeV1[1]);
            hSingleMuSimplifiedMergedPtRelCodeSSMinusV1->Fill(muonSimplifiedMergedPtRelCodeV1[0]);
            hSingleMuSimplifiedMergedPtRelCodeSSMinusV1->Fill(muonSimplifiedMergedPtRelCodeV1[1]);
            hMuonSimplifiedMergedPtRelCodeSSMinusV2->Fill(muonSimplifiedMergedPtRelCodeV2[0],muonSimplifiedMergedPtRelCodeV2[1]);
            hSingleMuSimplifiedMergedPtRelCodeSSMinusV2->Fill(muonSimplifiedMergedPtRelCodeV2[0]);
            hSingleMuSimplifiedMergedPtRelCodeSSMinusV2->Fill(muonSimplifiedMergedPtRelCodeV2[1]);
          }
        }
        
        /// PtRel OS/SS/SSplus/SSminus (Simplified/V0/V1/V2
        if(vCharge[0]!=vCharge[1])
        {
          mMuonPtRelJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtRelJet[0]);
          mMuonPtRelJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtRelJet[1]);
          mMuonPtRelJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtRelJet[0]);
          mMuonPtRelJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtRelJet[1]);
          mMuonPtRelJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtRelJet[0]);
          mMuonPtRelJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtRelJet[1]);
          mMuonPtRelJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtRelJet[0]);
          mMuonPtRelJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtRelJet[1]);
          
          mMuonPtRelGenJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtRelGenJet[0]);
          mMuonPtRelGenJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtRelGenJet[1]);
          mMuonPtRelGenJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtRelGenJet[0]);
          mMuonPtRelGenJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtRelGenJet[1]);
          mMuonPtRelGenJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtRelGenJet[0]);
          mMuonPtRelGenJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtRelGenJet[1]);
          mMuonPtRelGenJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtRelGenJet[0]);
          mMuonPtRelGenJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtRelGenJet[1]);
        }
        else
        {
          mMuonPtRelJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtRelJet[0]);
          mMuonPtRelJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtRelJet[1]);
          mMuonPtRelJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtRelJet[0]);
          mMuonPtRelJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtRelJet[1]);
          mMuonPtRelJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtRelJet[0]);
          mMuonPtRelJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtRelJet[1]);
          mMuonPtRelJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtRelJet[0]);
          mMuonPtRelJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtRelJet[1]);  	

          mMuonPtRelGenJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtRelGenJet[0]);
          mMuonPtRelGenJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtRelGenJet[1]);
          mMuonPtRelGenJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtRelGenJet[0]);
          mMuonPtRelGenJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtRelGenJet[1]);
          mMuonPtRelGenJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtRelGenJet[0]);
          mMuonPtRelGenJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtRelGenJet[1]);
          mMuonPtRelGenJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtRelGenJet[0]);
          mMuonPtRelGenJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtRelGenJet[1]);    
          if(vCharge[0]>0)
          {
            mMuonPtRelJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtRelJet[0]);
            mMuonPtRelJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtRelJet[1]);  	
            mMuonPtRelJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtRelJet[0]);
            mMuonPtRelJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtRelJet[1]);  	
            mMuonPtRelJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtRelJet[0]);
            mMuonPtRelJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtRelJet[1]);  	
            mMuonPtRelJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtRelJet[0]);
            mMuonPtRelJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtRelJet[1]);  	

            mMuonPtRelGenJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtRelGenJet[0]);
            mMuonPtRelGenJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtRelGenJet[1]);      
            mMuonPtRelGenJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtRelGenJet[0]);
            mMuonPtRelGenJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtRelGenJet[1]);      
            mMuonPtRelGenJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtRelGenJet[0]);
            mMuonPtRelGenJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtRelGenJet[1]);      
            mMuonPtRelGenJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtRelGenJet[0]);
            mMuonPtRelGenJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtRelGenJet[1]);      
          }
          else
          {
            mMuonPtRelJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtRelJet[0]);
            mMuonPtRelJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtRelJet[1]);  	
            mMuonPtRelJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtRelJet[0]);
            mMuonPtRelJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtRelJet[1]);  	
            mMuonPtRelJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtRelJet[0]);
            mMuonPtRelJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtRelJet[1]);  	
            mMuonPtRelJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtRelJet[0]);
            mMuonPtRelJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtRelJet[1]);	

            mMuonPtRelGenJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtRelGenJet[0]);
            mMuonPtRelGenJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtRelGenJet[1]);     
            mMuonPtRelGenJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtRelGenJet[0]);
            mMuonPtRelGenJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtRelGenJet[1]);     
            mMuonPtRelGenJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtRelGenJet[0]);
            mMuonPtRelGenJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtRelGenJet[1]);     
            mMuonPtRelGenJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtRelGenJet[0]);
            mMuonPtRelGenJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtRelGenJet[1]);     
          }
        }
        
        /// PtRel Overall/Plus/Minus (Simplified)
        for (unsigned int k = 0; k < muonSimplifiedPtRelCode.size(); k++)
        {
          mMuonPtRelJetSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);
          mMuonPtRelSVorJetSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJet[k]);
          mMuonPtRelSVSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelSV[k]);

          mMuonPtRelSmearedJetSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelJetSmearedSV[k]);
          mMuonPtRelSmearedSVorJetSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJetSmearedSV[k]);
          mMuonPtRelSmearedSVSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVSmearedSV[k]);	

          mMuonPtRelSmearedJetSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelJetSmearedNO[k]);
          mMuonPtRelSmearedSVorJetSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJetSmearedNO[k]);
          mMuonPtRelSmearedSVSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVSmearedNO[k]);

          if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);
          else                mMuonPtRelJetNoSVSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);
          mMuonDRPtRelSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(DRMuonJet_PtRel[k]);
          
          mMuonPtRelGenJetSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelGenJet[k]);
          mMuonDRPtRelGenSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(DRMuonGenJet_PtRel[k]);
          // Separate by muon charge
          if(vCharge[k]>0)
          {
            mMuonPtRelJetPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetPlusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetPlusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVPlusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVSmearedSV[k]);	

            mMuonPtRelSmearedJetPlusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetPlusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVPlusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVSmearedNO[k]);	

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);

            mMuonPtRelGenJetPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelGenJet[k]);
          }
          else
          {
            mMuonPtRelJetMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetMinusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetMinusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVMinusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVSmearedSV[k]);	

            mMuonPtRelSmearedJetMinusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetMinusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVMinusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtRelSVSmearedNO[k]);	

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelJet[k]);

            mMuonPtRelGenJetMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtRelGenJet[k]);
          } // Separate by muon charge
        }
        
        /// PtRel Overall/Plus/Minus (V0)        
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV0.size(); k++)
        {
          mMuonPtRelJetSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);
          mMuonPtRelSVorJetSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJet[k]);
          mMuonPtRelSVSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSV[k]);

          mMuonPtRelSmearedJetSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJetSmearedSV[k]);
          mMuonPtRelSmearedSVorJetSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJetSmearedSV[k]);
          mMuonPtRelSmearedSVSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVSmearedSV[k]);	

          mMuonPtRelSmearedJetSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJetSmearedNO[k]);
          mMuonPtRelSmearedSVorJetSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJetSmearedNO[k]);
          mMuonPtRelSmearedSVSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVSmearedNO[k]);	

          if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);
          else                mMuonPtRelJetNoSVSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);
          mMuonDRPtRelSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(DRMuonJet_PtRel[k]);

          mMuonPtRelGenJetSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelGenJet[k]);
          mMuonDRPtRelGenSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(DRMuonGenJet_PtRel[k]);
          // Separate by muon charge
          if(vCharge[k]>0)
          {
            mMuonPtRelJetPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetPlusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVPlusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVSmearedSV[k]);	

            mMuonPtRelSmearedJetPlusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVPlusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVSmearedNO[k]);	

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);

            mMuonPtRelGenJetPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelGenJet[k]);
          }
          else
          {
            mMuonPtRelJetMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetMinusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVMinusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVSmearedSV[k]);

            mMuonPtRelSmearedJetMinusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVMinusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelSVSmearedNO[k]);

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelJet[k]);
            
            mMuonPtRelGenJetMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtRelGenJet[k]);
          } // Separate by muon charge
        }
        
        /// PtRel Overall/Plus/Minus (V1)               
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV1.size(); k++)
        {
          mMuonPtRelJetSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);
          mMuonPtRelSVorJetSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJet[k]);
          mMuonPtRelSVSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSV[k]);

          mMuonPtRelSmearedJetSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJetSmearedSV[k]);
          mMuonPtRelSmearedSVorJetSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJetSmearedSV[k]);
          mMuonPtRelSmearedSVSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVSmearedSV[k]);	

          mMuonPtRelSmearedJetSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJetSmearedNO[k]);
          mMuonPtRelSmearedSVorJetSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJetSmearedNO[k]);
          mMuonPtRelSmearedSVSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVSmearedNO[k]);	

          if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);
          else                mMuonPtRelJetNoSVSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);
          mMuonDRPtRelSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(DRMuonJet_PtRel[k]);

          mMuonPtRelGenJetSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelGenJet[k]);
          mMuonDRPtRelGenSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(DRMuonGenJet_PtRel[k]);
          // Separate by muon charge
          if(vCharge[k]>0)
          {
            mMuonPtRelJetPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetPlusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVPlusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVSmearedSV[k]);	

            mMuonPtRelSmearedJetPlusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVPlusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVSmearedNO[k]);	

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);

            mMuonPtRelGenJetPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelGenJet[k]);
          }
          else
          {
            mMuonPtRelJetMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetMinusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVMinusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVSmearedSV[k]);

            mMuonPtRelSmearedJetMinusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVMinusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelSVSmearedNO[k]);

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelJet[k]);
          
            mMuonPtRelGenJetMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtRelGenJet[k]);
          } // Separate by muon charge
        }

        /// PtRel Overall/Plus/Minus (V2)                
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV2.size(); k++)
        {
          mMuonPtRelJetSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);
          mMuonPtRelSVorJetSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJet[k]);
          mMuonPtRelSVSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSV[k]);

          mMuonPtRelSmearedJetSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJetSmearedSV[k]);
          mMuonPtRelSmearedSVorJetSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJetSmearedSV[k]);
          mMuonPtRelSmearedSVSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVSmearedSV[k]);	

          mMuonPtRelSmearedJetSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJetSmearedNO[k]);
          mMuonPtRelSmearedSVorJetSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJetSmearedNO[k]);
          mMuonPtRelSmearedSVSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVSmearedNO[k]);	

          if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);
          else                mMuonPtRelJetNoSVSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);
          mMuonDRPtRelSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(DRMuonJet_PtRel[k]);

          mMuonPtRelGenJetSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelGenJet[k]);
          mMuonDRPtRelGenSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(DRMuonGenJet_PtRel[k]);
          // Separate by muon charge
          if(vCharge[k]>0)
          {
            mMuonPtRelJetPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetPlusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVPlusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVSmearedSV[k]);	

            mMuonPtRelSmearedJetPlusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetPlusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVPlusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVSmearedNO[k]);	

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);

            mMuonPtRelGenJetPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelGenJet[k]);
          }
          else
          {
            mMuonPtRelJetMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);
            mMuonPtRelSVorJetMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJet[k]);
            mMuonPtRelSVMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSV[k]);

            mMuonPtRelSmearedJetMinusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJetSmearedSV[k]);
            mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJetSmearedSV[k]);
            mMuonPtRelSmearedSVMinusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVSmearedSV[k]);

            mMuonPtRelSmearedJetMinusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJetSmearedNO[k]);
            mMuonPtRelSmearedSVorJetMinusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVorJetSmearedNO[k]);
            mMuonPtRelSmearedSVMinusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelSVSmearedNO[k]);

            if (PtRelSV[k]<1e9) mMuonPtRelJetYesSVMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);
            else                mMuonPtRelJetNoSVMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelJet[k]);

            mMuonPtRelGenJetMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtRelGenJet[k]);
          } // Separate by muon charge
        }
        
        /// PtOut OS/SS/SSplus/SSminus (Simplified/V0/V1/V2)
        if(vCharge[0]!=vCharge[1])
        {
          mMuonPtOutJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtOutJet[1]);  	
          mMuonPtOutJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtOutJet[1]);  	
          mMuonPtOutJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutJet[1]);  	
          mMuonPtOutJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutJet[1]);  	    	

          mMuonPtOutGenJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtOutGenJet[0]);
          mMuonPtOutGenJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtOutGenJet[1]);    
          mMuonPtOutGenJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtOutGenJet[0]);
          mMuonPtOutGenJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtOutGenJet[1]);    
          mMuonPtOutGenJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutGenJet[0]);
          mMuonPtOutGenJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutGenJet[1]);    
          mMuonPtOutGenJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutGenJet[0]);
          mMuonPtOutGenJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutGenJet[1]);            
        }
        else
        {
          mMuonPtOutJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtOutJet[1]);  	
          mMuonPtOutJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtOutJet[1]);  	
          mMuonPtOutJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutJet[1]);  	
          mMuonPtOutJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutJet[1]);  	

          mMuonPtOutGenJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtOutGenJet[0]);
          mMuonPtOutGenJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtOutGenJet[1]);    
          mMuonPtOutGenJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtOutGenJet[0]);
          mMuonPtOutGenJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtOutGenJet[1]);    
          mMuonPtOutGenJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutGenJet[0]);
          mMuonPtOutGenJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutGenJet[1]);    
          mMuonPtOutGenJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutGenJet[0]);
          mMuonPtOutGenJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutGenJet[1]);    
          if(vCharge[0]>0)
          {
            mMuonPtOutJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtOutJet[0]);
            mMuonPtOutJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtOutJet[1]);  	
            mMuonPtOutJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtOutJet[0]);
            mMuonPtOutJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtOutJet[1]);  	
            mMuonPtOutJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutJet[0]);
            mMuonPtOutJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutJet[1]);  	
            mMuonPtOutJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutJet[0]);
            mMuonPtOutJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutJet[1]);  	

            mMuonPtOutGenJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtOutGenJet[0]);
            mMuonPtOutGenJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtOutGenJet[1]);      
            mMuonPtOutGenJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtOutGenJet[0]);
            mMuonPtOutGenJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtOutGenJet[1]);      
            mMuonPtOutGenJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutGenJet[0]);
            mMuonPtOutGenJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutGenJet[1]);      
            mMuonPtOutGenJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutGenJet[0]);
            mMuonPtOutGenJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutGenJet[1]);      
          }
          else
          {
            mMuonPtOutJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtOutJet[0]);
            mMuonPtOutJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtOutJet[1]);  	
            mMuonPtOutJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtOutJet[0]);
            mMuonPtOutJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtOutJet[1]);  	
            mMuonPtOutJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutJet[0]);
            mMuonPtOutJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutJet[1]);  	
            mMuonPtOutJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutJet[0]);
            mMuonPtOutJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutJet[1]);	

            mMuonPtOutGenJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(PtOutGenJet[0]);
            mMuonPtOutGenJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(PtOutGenJet[1]);     
            mMuonPtOutGenJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(PtOutGenJet[0]);
            mMuonPtOutGenJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(PtOutGenJet[1]);     
            mMuonPtOutGenJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutGenJet[0]);
            mMuonPtOutGenJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutGenJet[1]);     
            mMuonPtOutGenJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutGenJet[0]);
            mMuonPtOutGenJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutGenJet[1]);     
          }
        }
        
        /// PtOut Overall/Plus/Minus (Simplified)       
        for (unsigned int k = 0; k < muonSimplifiedPtRelCode.size(); k++)
        {
          mMuonPtOutJetSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
          mMuonPtOutSVorJetSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJet[k]);
          mMuonPtOutSVSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutSV[k]);

          mMuonPtOutSmearedJetSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutJetSmearedSV[k]);
          mMuonPtOutSmearedSVorJetSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJetSmearedSV[k]);
          mMuonPtOutSmearedSVSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVSmearedSV[k]);

          mMuonPtOutSmearedJetSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutJetSmearedNO[k]);
          mMuonPtOutSmearedSVorJetSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJetSmearedNO[k]);
          mMuonPtOutSmearedSVSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVSmearedNO[k]);

          if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
          else                mMuonPtOutJetNoSVSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
          mMuonDRPtOutSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(DRMuonJet_PtOut[k]);

          mMuonPtOutGenJetSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
          mMuonDRPtOutGenSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(DRMuonGenJet_PtOut[k]);

          if (Muon_pt->at(vSelectedMuons[k]) >= 4.0)
          {
            if (Muon_pt->at(vSelectedMuons[k]) < 6.0)
            {
              mMuonPtOutJetSimplifiedTemplatesM0406[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesM0406[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 8.0)
            {
              mMuonPtOutJetSimplifiedTemplatesM0608[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesM0608[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 10.0)
            {
              mMuonPtOutJetSimplifiedTemplatesM0810[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesM0810[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 12.0)
            {
              mMuonPtOutJetSimplifiedTemplatesM1012[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesM1012[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 15.0)
            {
              mMuonPtOutJetSimplifiedTemplatesM1215[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesM1215[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 20.0)
            {
              mMuonPtOutJetSimplifiedTemplatesM1520[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesM1520[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else
            {
              mMuonPtOutJetSimplifiedTemplatesM20xx[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesM20xx[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
          }
          
          if (Jet_pt->at(vSelectedJets[k]) >= 10.0)
          {
            if (Jet_pt->at(vSelectedJets[k]) < 15.0)
            {
              mMuonPtOutJetSimplifiedTemplatesJ1015[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesJ1015[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Jet_pt->at(vSelectedJets[k]) < 20.0)
            {
              mMuonPtOutJetSimplifiedTemplatesJ1520[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesJ1520[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Jet_pt->at(vSelectedJets[k]) < 30.0)
            {
              mMuonPtOutJetSimplifiedTemplatesJ2030[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesJ2030[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
            else
            {
              mMuonPtOutJetSimplifiedTemplatesJ30xx[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedTemplatesJ30xx[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
            }
          }
          
          double deltaRelPt = ( Jet_pt->at(vSelectedJets[k]) - Muon_pt->at(vSelectedMuons[k]) ) / Jet_pt->at(vSelectedJets[k]);
          if (deltaRelPt <= 0.25)
          {
            mMuonPtOutJetSimplifiedTemplatesD0025[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
            mMuonPtOutGenJetSimplifiedTemplatesD0025[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
          }
          else if (deltaRelPt <= 0.50)
          {
            mMuonPtOutJetSimplifiedTemplatesD2550[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
            mMuonPtOutGenJetSimplifiedTemplatesD2550[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
          }
          else if (deltaRelPt <= 0.75)
          {
            mMuonPtOutJetSimplifiedTemplatesD5075[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
            mMuonPtOutGenJetSimplifiedTemplatesD5075[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
          }
          else if (deltaRelPt <= 1.00)
          {
            mMuonPtOutJetSimplifiedTemplatesD7500[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
            mMuonPtOutGenJetSimplifiedTemplatesD7500[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
          }
          
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonPtOutJetPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetPlusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetPlusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVPlusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVSmearedSV[k]);

            mMuonPtOutSmearedJetPlusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetPlusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVPlusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVSmearedNO[k]);

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
          }
          else
          {
            mMuonPtOutJetMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetMinusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetMinusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVMinusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVSmearedSV[k]);

            mMuonPtOutSmearedJetMinusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetMinusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVMinusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(PtOutSVSmearedNO[k]);

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(PtOutGenJet[k]);
          } // Separate by charge
        }
        
        /// PtOut Overall/Plus/Minus (V0)        
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV0.size(); k++)
        {
          mMuonPtOutJetSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
          mMuonPtOutSVorJetSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJet[k]);
          mMuonPtOutSVSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSV[k]);

          mMuonPtOutSmearedJetSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJetSmearedSV[k]);
          mMuonPtOutSmearedSVorJetSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJetSmearedSV[k]);
          mMuonPtOutSmearedSVSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVSmearedSV[k]);

          mMuonPtOutSmearedJetSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJetSmearedNO[k]);
          mMuonPtOutSmearedSVorJetSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJetSmearedNO[k]);
          mMuonPtOutSmearedSVSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVSmearedNO[k]);

          if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
          else                mMuonPtOutJetNoSVSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
          mMuonDRPtOutSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(DRMuonJet_PtOut[k]);

          mMuonPtOutGenJetSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
          mMuonDRPtOutGenSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(DRMuonGenJet_PtOut[k]);
          if (Muon_pt->at(vSelectedMuons[k]) >= 4.0)
          {
            if (Muon_pt->at(vSelectedMuons[k]) < 6.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesM0406[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0406[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 8.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesM0608[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0608[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 10.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesM0810[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesM0810[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 12.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesM1012[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1012[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 15.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesM1215[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1215[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 20.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesM1520[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesM1520[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesM20xx[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesM20xx[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
          }
          
          if (Jet_pt->at(vSelectedJets[k]) >= 10.0)
          {
            if (Jet_pt->at(vSelectedJets[k]) < 15.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesJ1015[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ1015[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Jet_pt->at(vSelectedJets[k]) < 20.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesJ1520[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ1520[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Jet_pt->at(vSelectedJets[k]) < 30.0)
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesJ2030[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ2030[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
            else
            {
              mMuonPtOutJetSimplifiedMergedV0TemplatesJ30xx[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV0TemplatesJ30xx[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
            }
          }

          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonPtOutJetPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetPlusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVPlusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVSmearedSV[k]);

            mMuonPtOutSmearedJetPlusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVPlusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVSmearedNO[k]);

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
          }
          else
          {
            mMuonPtOutJetMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetMinusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVMinusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVSmearedSV[k]);

            mMuonPtOutSmearedJetMinusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVMinusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutSVSmearedNO[k]);

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(PtOutGenJet[k]);
          } // Separate by charge
        }
        
        /// PtOut Overall/Plus/Minus (V1)        
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV1.size(); k++)
        {
          mMuonPtOutJetSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
          mMuonPtOutSVorJetSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJet[k]);
          mMuonPtOutSVSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSV[k]);

          mMuonPtOutSmearedJetSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJetSmearedSV[k]);
          mMuonPtOutSmearedSVorJetSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJetSmearedSV[k]);
          mMuonPtOutSmearedSVSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVSmearedSV[k]);

          mMuonPtOutSmearedJetSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJetSmearedNO[k]);
          mMuonPtOutSmearedSVorJetSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJetSmearedNO[k]);
          mMuonPtOutSmearedSVSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVSmearedNO[k]);

          if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
          else                mMuonPtOutJetNoSVSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
          mMuonDRPtOutSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(DRMuonJet_PtOut[k]);

          mMuonPtOutGenJetSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
          mMuonDRPtOutGenSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(DRMuonGenJet_PtOut[k]);
          if (Muon_pt->at(vSelectedMuons[k]) >= 4.0)
          {
            if (Muon_pt->at(vSelectedMuons[k]) < 6.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesM0406[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0406[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 8.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesM0608[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0608[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 10.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesM0810[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesM0810[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 12.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesM1012[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1012[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 15.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesM1215[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1215[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 20.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesM1520[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesM1520[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesM20xx[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesM20xx[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
          }
          
          if (Jet_pt->at(vSelectedJets[k]) >= 10.0)
          {
            if (Jet_pt->at(vSelectedJets[k]) < 15.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesJ1015[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ1015[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Jet_pt->at(vSelectedJets[k]) < 20.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesJ1520[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ1520[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Jet_pt->at(vSelectedJets[k]) < 30.0)
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesJ2030[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ2030[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
            else
            {
              mMuonPtOutJetSimplifiedMergedV1TemplatesJ30xx[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV1TemplatesJ30xx[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
            }
          }
          
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonPtOutJetPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetPlusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVPlusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVSmearedSV[k]);

            mMuonPtOutSmearedJetPlusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVPlusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVSmearedNO[k]);

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
          }
          else
          {
            mMuonPtOutJetMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetMinusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVMinusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVSmearedSV[k]);

            mMuonPtOutSmearedJetMinusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVMinusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutSVSmearedNO[k]);

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(PtOutGenJet[k]);
          } // Separate by charge
        }
        
        std::vector<std::pair<short int, short int> > muonAndEventSimplifiedMergedPtRelCodeV1;
        for(unsigned int i = 0; i < muonSimplifiedMergedPtRelCodeV1.size(); i++)
        {
          std::pair<short int, short int> indexPair = std::make_pair(muonSimplifiedMergedPtRelCodeV1[i],eventSimplifiedMergedPtRelCodeV1);
          muonAndEventSimplifiedMergedPtRelCodeV1.push_back(indexPair);
        }
        
        mMuonPtOutJetSimplifiedMergedV12DTemplates[eventSimplifiedMergedPtRelCodeV1]->Fill(PtOutJet[0],PtOutJet[1],0.5);
        mMuonPtOutJetSimplifiedMergedV12DTemplates[eventSimplifiedMergedPtRelCodeV1]->Fill(PtOutJet[1],PtOutJet[0],0.5);
        
        mMuonPtOutJetSimplifiedMergedV1TemplatesByEventCode[muonAndEventSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutJet[0]);
        mMuonPtOutJetSimplifiedMergedV1TemplatesByEventCode[muonAndEventSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutJet[1]);
        
        if(vCharge[0] == vCharge[1])
        {
          mMuonPtOutJetSimplifiedMergedV12DTemplatesSS[eventSimplifiedMergedPtRelCodeV1]->Fill(PtOutJet[0],PtOutJet[1],0.5);
          mMuonPtOutJetSimplifiedMergedV12DTemplatesSS[eventSimplifiedMergedPtRelCodeV1]->Fill(PtOutJet[1],PtOutJet[0],0.5);
          
          mMuonPtOutJetSimplifiedMergedV1TemplatesSSByEventCode[muonAndEventSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetSimplifiedMergedV1TemplatesSSByEventCode[muonAndEventSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutJet[1]);
        }
        else
        {
          mMuonPtOutJetSimplifiedMergedV12DTemplatesOS[eventSimplifiedMergedPtRelCodeV1]->Fill(PtOutJet[0],PtOutJet[1],0.5);
          mMuonPtOutJetSimplifiedMergedV12DTemplatesOS[eventSimplifiedMergedPtRelCodeV1]->Fill(PtOutJet[1],PtOutJet[0],0.5);

          mMuonPtOutJetSimplifiedMergedV1TemplatesOSByEventCode[muonAndEventSimplifiedMergedPtRelCodeV1[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetSimplifiedMergedV1TemplatesOSByEventCode[muonAndEventSimplifiedMergedPtRelCodeV1[1]]->Fill(PtOutJet[1]);
        }
        
        /// PtOut Overall/Plus/Minus (V2)                
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV2.size(); k++)
        {
          mMuonPtOutJetSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
          mMuonPtOutSVorJetSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJet[k]);
          mMuonPtOutSVSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSV[k]);

          mMuonPtOutSmearedJetSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJetSmearedSV[k]);
          mMuonPtOutSmearedSVorJetSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJetSmearedSV[k]);
          mMuonPtOutSmearedSVSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVSmearedSV[k]);

          mMuonPtOutSmearedJetSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJetSmearedNO[k]);
          mMuonPtOutSmearedSVorJetSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJetSmearedNO[k]);
          mMuonPtOutSmearedSVSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVSmearedNO[k]);

          if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
          else                mMuonPtOutJetNoSVSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
          mMuonDRPtOutSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(DRMuonJet_PtOut[k]);

          mMuonPtOutGenJetSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
          mMuonDRPtOutGenSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(DRMuonGenJet_PtOut[k]);
          if (Muon_pt->at(vSelectedMuons[k]) >= 4.0)
          {
            if (Muon_pt->at(vSelectedMuons[k]) < 6.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesM0406[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0406[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 8.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesM0608[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0608[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 10.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesM0810[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesM0810[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 12.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesM1012[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1012[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 15.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesM1215[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1215[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Muon_pt->at(vSelectedMuons[k]) < 20.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesM1520[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesM1520[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesM20xx[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesM20xx[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
          }
          
          if (Jet_pt->at(vSelectedJets[k]) >= 10.0)
          {
            if (Jet_pt->at(vSelectedJets[k]) < 15.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesJ1015[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ1015[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Jet_pt->at(vSelectedJets[k]) < 20.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesJ1520[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ1520[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else if (Jet_pt->at(vSelectedJets[k]) < 30.0)
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesJ2030[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ2030[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
            else
            {
              mMuonPtOutJetSimplifiedMergedV2TemplatesJ30xx[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
              mMuonPtOutGenJetSimplifiedMergedV2TemplatesJ30xx[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
            }
          }
          
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonPtOutJetPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetPlusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVPlusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVSmearedSV[k]);

            mMuonPtOutSmearedJetPlusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetPlusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVPlusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVSmearedNO[k]);

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
          }
          else
          {
            mMuonPtOutJetMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
            mMuonPtOutSVorJetMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJet[k]);
            mMuonPtOutSVMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSV[k]);

            mMuonPtOutSmearedJetMinusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJetSmearedSV[k]);
            mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJetSmearedSV[k]);
            mMuonPtOutSmearedSVMinusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVSmearedSV[k]);

            mMuonPtOutSmearedJetMinusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJetSmearedNO[k]);
            mMuonPtOutSmearedSVorJetMinusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVorJetSmearedNO[k]);
            mMuonPtOutSmearedSVMinusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutSVSmearedNO[k]);

            if (PtOutSV[k]<1e9) mMuonPtOutJetYesSVMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);
            else                mMuonPtOutJetNoSVMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutJet[k]);

            mMuonPtOutGenJetMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(PtOutGenJet[k]);
          } // Separate by charge
        }
        
        std::vector<std::pair<short int, short int> > muonAndEventSimplifiedMergedPtRelCodeV2;
        for(unsigned int i = 0; i < muonSimplifiedMergedPtRelCodeV2.size(); i++)
        {
          std::pair<short int, short int> indexPair = std::make_pair(muonSimplifiedMergedPtRelCodeV2[i],eventSimplifiedMergedPtRelCodeV2);
          muonAndEventSimplifiedMergedPtRelCodeV2.push_back(indexPair);
        }
        
        mMuonPtOutJetSimplifiedMergedV22DTemplates[eventSimplifiedMergedPtRelCodeV2]->Fill(PtOutJet[0],PtOutJet[1],0.5);
        mMuonPtOutJetSimplifiedMergedV22DTemplates[eventSimplifiedMergedPtRelCodeV2]->Fill(PtOutJet[1],PtOutJet[0],0.5);
        
        mMuonPtOutJetSimplifiedMergedV2TemplatesByEventCode[muonAndEventSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutJet[0]);
        mMuonPtOutJetSimplifiedMergedV2TemplatesByEventCode[muonAndEventSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutJet[1]);
        
        if(vCharge[0] == vCharge[1])
        {
          mMuonPtOutJetSimplifiedMergedV22DTemplatesSS[eventSimplifiedMergedPtRelCodeV2]->Fill(PtOutJet[0],PtOutJet[1],0.5);
          mMuonPtOutJetSimplifiedMergedV22DTemplatesSS[eventSimplifiedMergedPtRelCodeV2]->Fill(PtOutJet[1],PtOutJet[0],0.5);
          
          mMuonPtOutJetSimplifiedMergedV2TemplatesSSByEventCode[muonAndEventSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetSimplifiedMergedV2TemplatesSSByEventCode[muonAndEventSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutJet[1]);
        }
        else
        {
          mMuonPtOutJetSimplifiedMergedV22DTemplatesOS[eventSimplifiedMergedPtRelCodeV2]->Fill(PtOutJet[0],PtOutJet[1],0.5);
          mMuonPtOutJetSimplifiedMergedV22DTemplatesOS[eventSimplifiedMergedPtRelCodeV2]->Fill(PtOutJet[1],PtOutJet[0],0.5);
          
          mMuonPtOutJetSimplifiedMergedV2TemplatesOSByEventCode[muonAndEventSimplifiedMergedPtRelCodeV2[0]]->Fill(PtOutJet[0]);
          mMuonPtOutJetSimplifiedMergedV2TemplatesOSByEventCode[muonAndEventSimplifiedMergedPtRelCodeV2[1]]->Fill(PtOutJet[1]);
        }
        
        /// Emu OS/SS/SSplus/SSminus (Simplified/V0/V1/V2)
        if(vCharge[0]!=vCharge[1])
        {
          mMuonEmuJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(EmuJet[0]);
          mMuonEmuJetOSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(EmuJet[1]);    
          mMuonEmuJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(EmuJet[0]);
          mMuonEmuJetOSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(EmuJet[1]);    
          mMuonEmuJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(EmuJet[0]);
          mMuonEmuJetOSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(EmuJet[1]);    
          mMuonEmuJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(EmuJet[0]);
          mMuonEmuJetOSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(EmuJet[1]);            
        }
        else
        {
          mMuonEmuJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(EmuJet[0]);
          mMuonEmuJetSSSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(EmuJet[1]);    
          mMuonEmuJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(EmuJet[0]);
          mMuonEmuJetSSSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(EmuJet[1]);    
          mMuonEmuJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(EmuJet[0]);
          mMuonEmuJetSSSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(EmuJet[1]);    
          mMuonEmuJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(EmuJet[0]);
          mMuonEmuJetSSSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(EmuJet[1]);    
          if(vCharge[0]>0)
          {
            mMuonEmuJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(EmuJet[0]);
            mMuonEmuJetSSPlusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(EmuJet[1]);      
            mMuonEmuJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(EmuJet[0]);
            mMuonEmuJetSSPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(EmuJet[1]);      
            mMuonEmuJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(EmuJet[0]);
            mMuonEmuJetSSPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(EmuJet[1]);      
            mMuonEmuJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(EmuJet[0]);
            mMuonEmuJetSSPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(EmuJet[1]);      
          }
          else
          {
            mMuonEmuJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[0]]->Fill(EmuJet[0]);
            mMuonEmuJetSSMinusSimplifiedTemplates[muonSimplifiedPtRelCode[1]]->Fill(EmuJet[1]);     
            mMuonEmuJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[0]]->Fill(EmuJet[0]);
            mMuonEmuJetSSMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[1]]->Fill(EmuJet[1]);     
            mMuonEmuJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[0]]->Fill(EmuJet[0]);
            mMuonEmuJetSSMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[1]]->Fill(EmuJet[1]);     
            mMuonEmuJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[0]]->Fill(EmuJet[0]);
            mMuonEmuJetSSMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[1]]->Fill(EmuJet[1]);     
          }
        }   
        
        /// Emu Overall/Plus/Minus (Simplified)        
        for (unsigned int k = 0; k < muonSimplifiedPtRelCode.size(); k++)
        {
          mMuonEmuJetSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(EmuJet[k]);
          mMuonEmuSmearedJetSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(EmuJetSmearedSV[k]);
          mMuonEmuSmearedJetSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(EmuJetSmearedNO[k]);
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonEmuJetPlusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetPlusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetPlusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(EmuJetSmearedNO[k]);
          }
          else
          {
            mMuonEmuJetMinusSimplifiedTemplates[muonSimplifiedPtRelCode[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetMinusSimplifiedTemplates_SV[muonSimplifiedPtRelCode[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetMinusSimplifiedTemplates_NO[muonSimplifiedPtRelCode[k]]->Fill(EmuJetSmearedNO[k]);
          } // Separate by charge
        }
        
        /// Emu Overall/Plus/Minus (V0)        
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV0.size(); k++)
        {
          mMuonEmuJetSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJet[k]);
          mMuonEmuSmearedJetSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJetSmearedSV[k]);
          mMuonEmuSmearedJetSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJetSmearedNO[k]);
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonEmuJetPlusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetPlusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetPlusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJetSmearedNO[k]);
          }
          else
          {
            mMuonEmuJetMinusSimplifiedMergedV0Templates[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetMinusSimplifiedMergedV0Templates_SV[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetMinusSimplifiedMergedV0Templates_NO[muonSimplifiedMergedPtRelCodeV0[k]]->Fill(EmuJetSmearedNO[k]);
          } // Separate by charge
        }
        
        /// Emu Overall/Plus/Minus (V1)        
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV1.size(); k++)
        {
          mMuonEmuJetSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJet[k]);
          mMuonEmuSmearedJetSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJetSmearedSV[k]);
          mMuonEmuSmearedJetSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJetSmearedNO[k]);
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonEmuJetPlusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetPlusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetPlusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJetSmearedNO[k]);
          }
          else
          {
            mMuonEmuJetMinusSimplifiedMergedV1Templates[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetMinusSimplifiedMergedV1Templates_SV[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetMinusSimplifiedMergedV1Templates_NO[muonSimplifiedMergedPtRelCodeV1[k]]->Fill(EmuJetSmearedNO[k]);
          } // Separate by charge
        }
        
        /// Emu Overall/Plus/Minus (V2)                
        for (unsigned int k = 0; k < muonSimplifiedMergedPtRelCodeV2.size(); k++)
        {
          mMuonEmuJetSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJet[k]);
          mMuonEmuSmearedJetSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJetSmearedSV[k]);
          mMuonEmuSmearedJetSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJetSmearedNO[k]);
          // Separate by charge
          if(vCharge[k]>0)
          {
            mMuonEmuJetPlusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetPlusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetPlusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJetSmearedNO[k]);
          }
          else
          {
            mMuonEmuJetMinusSimplifiedMergedV2Templates[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJet[k]);
            mMuonEmuSmearedJetMinusSimplifiedMergedV2Templates_SV[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJetSmearedSV[k]);
            mMuonEmuSmearedJetMinusSimplifiedMergedV2Templates_NO[muonSimplifiedMergedPtRelCodeV2[k]]->Fill(EmuJetSmearedNO[k]);
          } // Separate by charge
        }
        
        // Count events
        mSelEventsMergedPtRelCodeV1Ordered.find(eventSimplifiedMergedPtRelCodeV1Ordered) != mSelEventsMergedPtRelCodeV1Ordered.end() ?
          mSelEventsMergedPtRelCodeV1Ordered[eventSimplifiedMergedPtRelCodeV1Ordered]++ :
          mSelEventsMergedPtRelCodeV1Ordered[eventSimplifiedMergedPtRelCodeV1Ordered] = 1;
        mSelEventsMergedPtRelCodeV1.find(eventSimplifiedMergedPtRelCodeV1) != mSelEventsMergedPtRelCodeV1.end() ?
          mSelEventsMergedPtRelCodeV1[eventSimplifiedMergedPtRelCodeV1]++ :
          mSelEventsMergedPtRelCodeV1[eventSimplifiedMergedPtRelCodeV1] = 1;
        mSelEventsMergedPtRelCodeV2Ordered.find(eventSimplifiedMergedPtRelCodeV2Ordered) != mSelEventsMergedPtRelCodeV2Ordered.end() ?
          mSelEventsMergedPtRelCodeV2Ordered[eventSimplifiedMergedPtRelCodeV2Ordered]++ :
          mSelEventsMergedPtRelCodeV2Ordered[eventSimplifiedMergedPtRelCodeV2Ordered] = 1;
        mSelEventsMergedPtRelCodeV2.find(eventSimplifiedMergedPtRelCodeV2) != mSelEventsMergedPtRelCodeV2.end() ?
          mSelEventsMergedPtRelCodeV2[eventSimplifiedMergedPtRelCodeV2]++ :
          mSelEventsMergedPtRelCodeV2[eventSimplifiedMergedPtRelCodeV2] = 1;
          if(vDxy[0]<0.2 && vDxy[1]<0.2)
        {
          mSelEventsMergedPtRelCodeV1Ordered02.find(eventSimplifiedMergedPtRelCodeV1Ordered) != mSelEventsMergedPtRelCodeV1Ordered02.end() ?
            mSelEventsMergedPtRelCodeV1Ordered02[eventSimplifiedMergedPtRelCodeV1Ordered]++ :
            mSelEventsMergedPtRelCodeV1Ordered02[eventSimplifiedMergedPtRelCodeV1Ordered] = 1;
          mSelEventsMergedPtRelCodeV102.find(eventSimplifiedMergedPtRelCodeV1) != mSelEventsMergedPtRelCodeV102.end() ?
            mSelEventsMergedPtRelCodeV102[eventSimplifiedMergedPtRelCodeV1]++ :
            mSelEventsMergedPtRelCodeV102[eventSimplifiedMergedPtRelCodeV1] = 1;
          mSelEventsMergedPtRelCodeV2Ordered02.find(eventSimplifiedMergedPtRelCodeV2Ordered) != mSelEventsMergedPtRelCodeV2Ordered02.end() ?
            mSelEventsMergedPtRelCodeV2Ordered02[eventSimplifiedMergedPtRelCodeV2Ordered]++ :
            mSelEventsMergedPtRelCodeV2Ordered02[eventSimplifiedMergedPtRelCodeV2Ordered] = 1;
          mSelEventsMergedPtRelCodeV202.find(eventSimplifiedMergedPtRelCodeV2) != mSelEventsMergedPtRelCodeV202.end() ?
            mSelEventsMergedPtRelCodeV202[eventSimplifiedMergedPtRelCodeV2]++ :
            mSelEventsMergedPtRelCodeV202[eventSimplifiedMergedPtRelCodeV2] = 1;
        }
        if(PtRelJet[0] < 5.0 && PtRelJet[1] < 5.0)
        {
          mSelEventsMergedPtRelCodeV1Ordered50.find(eventSimplifiedMergedPtRelCodeV1Ordered) != mSelEventsMergedPtRelCodeV1Ordered50.end() ?
            mSelEventsMergedPtRelCodeV1Ordered50[eventSimplifiedMergedPtRelCodeV1Ordered]++ :
            mSelEventsMergedPtRelCodeV1Ordered50[eventSimplifiedMergedPtRelCodeV1Ordered] = 1;
          mSelEventsMergedPtRelCodeV150.find(eventSimplifiedMergedPtRelCodeV1) != mSelEventsMergedPtRelCodeV150.end() ?
            mSelEventsMergedPtRelCodeV150[eventSimplifiedMergedPtRelCodeV1]++ :
            mSelEventsMergedPtRelCodeV150[eventSimplifiedMergedPtRelCodeV1] = 1;
          mSelEventsMergedPtRelCodeV2Ordered50.find(eventSimplifiedMergedPtRelCodeV2Ordered) != mSelEventsMergedPtRelCodeV2Ordered50.end() ?
            mSelEventsMergedPtRelCodeV2Ordered50[eventSimplifiedMergedPtRelCodeV2Ordered]++ :
            mSelEventsMergedPtRelCodeV2Ordered50[eventSimplifiedMergedPtRelCodeV2Ordered] = 1;
          mSelEventsMergedPtRelCodeV250.find(eventSimplifiedMergedPtRelCodeV2) != mSelEventsMergedPtRelCodeV250.end() ?
            mSelEventsMergedPtRelCodeV250[eventSimplifiedMergedPtRelCodeV2]++ :
            mSelEventsMergedPtRelCodeV250[eventSimplifiedMergedPtRelCodeV2] = 1;
        }
        if(vDxy[0]<0.2 && vDxy[1]<0.2 && PtRelJet[0] < 5.0 && PtRelJet[1] < 5.0)
        {
          mSelEventsMergedPtRelCodeV1Ordered0250.find(eventSimplifiedMergedPtRelCodeV1Ordered) != mSelEventsMergedPtRelCodeV1Ordered0250.end() ?
            mSelEventsMergedPtRelCodeV1Ordered0250[eventSimplifiedMergedPtRelCodeV1Ordered]++ :
            mSelEventsMergedPtRelCodeV1Ordered0250[eventSimplifiedMergedPtRelCodeV1Ordered] = 1;
          mSelEventsMergedPtRelCodeV10250.find(eventSimplifiedMergedPtRelCodeV1) != mSelEventsMergedPtRelCodeV10250.end() ?
            mSelEventsMergedPtRelCodeV10250[eventSimplifiedMergedPtRelCodeV1]++ :
            mSelEventsMergedPtRelCodeV10250[eventSimplifiedMergedPtRelCodeV1] = 1;
          mSelEventsMergedPtRelCodeV2Ordered0250.find(eventSimplifiedMergedPtRelCodeV2Ordered) != mSelEventsMergedPtRelCodeV2Ordered0250.end() ?
            mSelEventsMergedPtRelCodeV2Ordered0250[eventSimplifiedMergedPtRelCodeV2Ordered]++ :
            mSelEventsMergedPtRelCodeV2Ordered0250[eventSimplifiedMergedPtRelCodeV2Ordered] = 1;
          mSelEventsMergedPtRelCodeV20250.find(eventSimplifiedMergedPtRelCodeV2) != mSelEventsMergedPtRelCodeV20250.end() ?
            mSelEventsMergedPtRelCodeV20250[eventSimplifiedMergedPtRelCodeV2]++ :
            mSelEventsMergedPtRelCodeV20250[eventSimplifiedMergedPtRelCodeV2] = 1;
        }
      }
      else
      {
        std::cout << "muonSimplifiedPtRelCode.size() = " << muonSimplifiedPtRelCode.size() << std::endl;
        std::cout << "muonSimplifiedMergedPtRelCodeV0.size() = " << muonSimplifiedMergedPtRelCodeV0.size() << std::endl;
        std::cout << "muonSimplifiedMergedPtRelCodeV1.size() = " << muonSimplifiedMergedPtRelCodeV1.size() << std::endl;
        std::cout << "muonSimplifiedMergedPtRelCodeV2.size() = " << muonSimplifiedMergedPtRelCodeV2.size() << std::endl;
        std::cout << "E R R O R: muonSimplifiedPtRelCode and/or muonSimplifiedMergedPtRelCodeV0/V1/V2 have != 2 entries! Exiting!\n";
        exit(1);
      }
    } // End of Jet analysis
  } // Loop on events
  
  // IP Templates for DIF's
  // HF Pion
  hMuonDxyDIFFromHFPionTemplate->Add(mMuonDxyTemplates[-10],mMuonDxyTemplates[-9],1,1);
  hMuonDxyDIFFromHFPionTemplate->Add(hMuonDxyDIFFromHFPionTemplate,mMuonDxyTemplates[9],1,1);
  hMuonDxyDIFFromHFPionTemplate->Add(hMuonDxyDIFFromHFPionTemplate,mMuonDxyTemplates[10],1,1);
  // LF Pion
  hMuonDxyDIFFromLFPionTemplate->Add(mMuonDxyTemplates[-11],mMuonDxyTemplates[11],1,1);
  // HF Kaon
  hMuonDxyDIFFromHFKaonTemplate->Add(mMuonDxyTemplates[-13],mMuonDxyTemplates[-12],1,1);
  hMuonDxyDIFFromHFKaonTemplate->Add(hMuonDxyDIFFromHFKaonTemplate,mMuonDxyTemplates[12],1,1);
  hMuonDxyDIFFromHFKaonTemplate->Add(hMuonDxyDIFFromHFKaonTemplate,mMuonDxyTemplates[13],1,1);
  // LF Kaon
  hMuonDxyDIFFromLFKaonTemplate->Add(mMuonDxyTemplates[-14],mMuonDxyTemplates[14],1,1);
  // HF
  hMuonDxyDIFFromHFTemplate->Add(mMuonDxyTemplates[-13],mMuonDxyTemplates[-12],1,1);
  hMuonDxyDIFFromHFTemplate->Add(hMuonDxyDIFFromHFTemplate,mMuonDxyTemplates[-10],1,1);
  hMuonDxyDIFFromHFTemplate->Add(hMuonDxyDIFFromHFTemplate,mMuonDxyTemplates[-9],1,1);
  hMuonDxyDIFFromHFTemplate->Add(hMuonDxyDIFFromHFTemplate,mMuonDxyTemplates[9],1,1);
  hMuonDxyDIFFromHFTemplate->Add(hMuonDxyDIFFromHFTemplate,mMuonDxyTemplates[10],1,1);
  hMuonDxyDIFFromHFTemplate->Add(hMuonDxyDIFFromHFTemplate,mMuonDxyTemplates[12],1,1);
  hMuonDxyDIFFromHFTemplate->Add(hMuonDxyDIFFromHFTemplate,mMuonDxyTemplates[13],1,1);
  // LF
  hMuonDxyDIFFromLFTemplate->Add(mMuonDxyTemplates[-14],mMuonDxyTemplates[-11],1,1);
  hMuonDxyDIFFromLFTemplate->Add(hMuonDxyDIFFromLFTemplate,mMuonDxyTemplates[11],1,1);
  hMuonDxyDIFFromLFTemplate->Add(hMuonDxyDIFFromLFTemplate,mMuonDxyTemplates[14],1,1);
  // Pion
  hMuonDxyDIFFromPionTemplate->Add(mMuonDxyTemplates[-11],mMuonDxyTemplates[-10],1,1);
  hMuonDxyDIFFromPionTemplate->Add(hMuonDxyDIFFromPionTemplate,mMuonDxyTemplates[-9],1,1);
  hMuonDxyDIFFromPionTemplate->Add(hMuonDxyDIFFromPionTemplate,mMuonDxyTemplates[9],1,1);
  hMuonDxyDIFFromPionTemplate->Add(hMuonDxyDIFFromPionTemplate,mMuonDxyTemplates[10],1,1);
  hMuonDxyDIFFromPionTemplate->Add(hMuonDxyDIFFromPionTemplate,mMuonDxyTemplates[11],1,1);
  // Kaon
  hMuonDxyDIFFromKaonTemplate->Add(mMuonDxyTemplates[-14],mMuonDxyTemplates[-13],1,1);
  hMuonDxyDIFFromKaonTemplate->Add(hMuonDxyDIFFromKaonTemplate,mMuonDxyTemplates[-12],1,1);
  hMuonDxyDIFFromKaonTemplate->Add(hMuonDxyDIFFromKaonTemplate,mMuonDxyTemplates[12],1,1);
  hMuonDxyDIFFromKaonTemplate->Add(hMuonDxyDIFFromKaonTemplate,mMuonDxyTemplates[13],1,1);
  hMuonDxyDIFFromKaonTemplate->Add(hMuonDxyDIFFromKaonTemplate,mMuonDxyTemplates[14],1,1);

  // Some integrals for re-scaling and re-weighting
  double integralDIFHF = hMuonDxyDIFFromHFTemplate->Integral();
  double integralDIFLF = hMuonDxyDIFFromLFTemplate->Integral();
  double integralDIFPion = hMuonDxyDIFFromPionTemplate->Integral();
  double integralDIFKaon = hMuonDxyDIFFromKaonTemplate->Integral();

  // Rescale HF
  hMuonDxyDIFHalfHFTemplate->Add(hMuonDxyDIFFromHFTemplate,hMuonDxyDIFFromLFTemplate,0.5,1+integralDIFHF/(2*integralDIFLF));
  hMuonDxyDIFTwiceHFTemplate->Add(hMuonDxyDIFFromHFTemplate,hMuonDxyDIFFromLFTemplate,2.,1-integralDIFHF/integralDIFLF);
  // Rescale Pion/Kaon
  hMuonDxyDIF30MorePionTemplate->Add(hMuonDxyDIFFromPionTemplate,hMuonDxyDIFFromKaonTemplate,1.3,(integralDIFKaon-0.3*integralDIFPion)/integralDIFKaon);
  hMuonDxyDIF30LessPionTemplate->Add(hMuonDxyDIFFromPionTemplate,hMuonDxyDIFFromKaonTemplate,0.7,(integralDIFKaon+0.3*integralDIFPion)/integralDIFKaon);
  hMuonDxyDIF30MoreKaonTemplate->Add(hMuonDxyDIFFromKaonTemplate,hMuonDxyDIFFromPionTemplate,1.3,(integralDIFPion-0.3*integralDIFKaon)/integralDIFPion);
  hMuonDxyDIF30LessKaonTemplate->Add(hMuonDxyDIFFromKaonTemplate,hMuonDxyDIFFromPionTemplate,0.7,(integralDIFPion+0.3*integralDIFKaon)/integralDIFPion);


  if (s->JetSelectorIsValid()) {

    // Temporary Prompt+Fake+PT histo
    TH1D * hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt = (TH1D*)mMuonPtOutJetSimplifiedTemplates[0]->Clone("hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt");
    hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt->Add(mMuonPtOutJetSimplifiedTemplates[3]);
    hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt->Add(mMuonPtOutJetSimplifiedTemplates[6]);

    TH1D * hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusC = (TH1D*)hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt->Clone("hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusC");
    hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusC->Add(mMuonPtOutJetSimplifiedTemplates[2]);

    TH1D * hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusDIF = (TH1D*)hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt->Clone("hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusDIF");
    hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusDIF->Add(mMuonPtOutJetSimplifiedTemplates[4]);


    double integralC   = mMuonPtOutJetSimplifiedTemplates[2]->Integral();
    double integralDIF = mMuonPtOutJetSimplifiedTemplates[4]->Integral();
    double integralFakePTPromp = hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt->Integral();
    double integralFakePTPromp_PlusC = integralFakePTPromp+integralC;
    double integralFakePTPromp_PlusDIF = integralFakePTPromp+integralDIF;

    hMuonPtOutJetSimplifiedMergedV1TemplatesCp25_3->Add(mMuonPtOutJetSimplifiedTemplates[2],hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusDIF,1.25,(integralFakePTPromp_PlusDIF-0.25*integralC)/integralFakePTPromp_PlusDIF);
    hMuonPtOutJetSimplifiedMergedV1TemplatesCl25_3->Add(mMuonPtOutJetSimplifiedTemplates[2],mMuonPtOutJetSimplifiedTemplates[4],0.75,(integralFakePTPromp_PlusDIF+0.25*integralC)/integralFakePTPromp_PlusDIF);
    hMuonPtOutJetSimplifiedMergedV1TemplatesCp30_3->Add(mMuonPtOutJetSimplifiedTemplates[2],hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusDIF,1.3,(integralFakePTPromp_PlusDIF-0.3*integralC)/integralFakePTPromp_PlusDIF);
    hMuonPtOutJetSimplifiedMergedV1TemplatesCl30_3->Add(mMuonPtOutJetSimplifiedTemplates[2],mMuonPtOutJetSimplifiedTemplates[4],0.7,(integralFakePTPromp_PlusDIF+0.3*integralC)/integralFakePTPromp_PlusDIF);
    hMuonPtOutJetSimplifiedMergedV1TemplatesCp50_3->Add(mMuonPtOutJetSimplifiedTemplates[2],hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusDIF,1.5,(integralFakePTPromp_PlusDIF-0.5*integralC)/integralFakePTPromp_PlusDIF);
    hMuonPtOutJetSimplifiedMergedV1TemplatesCl50_3->Add(mMuonPtOutJetSimplifiedTemplates[2],mMuonPtOutJetSimplifiedTemplates[4],0.5,(integralFakePTPromp_PlusDIF+0.5*integralC)/integralFakePTPromp_PlusDIF);
    hMuonPtOutJetSimplifiedMergedV1TemplatesCp75_3->Add(mMuonPtOutJetSimplifiedTemplates[2],hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusDIF,1.75,(integralFakePTPromp_PlusDIF-0.75*integralC)/integralFakePTPromp_PlusDIF);
    hMuonPtOutJetSimplifiedMergedV1TemplatesCl75_3->Add(mMuonPtOutJetSimplifiedTemplates[2],mMuonPtOutJetSimplifiedTemplates[4],0.25,(integralFakePTPromp_PlusDIF+0.75*integralC)/integralFakePTPromp_PlusDIF);

    hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp25_3->Add(mMuonPtOutJetSimplifiedTemplates[4],hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusC,1.25,(integralFakePTPromp_PlusC-0.25*integralDIF)/integralFakePTPromp_PlusC);
    hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl25_3->Add(mMuonPtOutJetSimplifiedTemplates[4],mMuonPtOutJetSimplifiedTemplates[4],0.75,(integralFakePTPromp_PlusC+0.25*integralDIF)/integralFakePTPromp_PlusC);
    hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp30_3->Add(mMuonPtOutJetSimplifiedTemplates[4],hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusC,1.3,(integralFakePTPromp_PlusC-0.3*integralDIF)/integralFakePTPromp_PlusC);
    hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl30_3->Add(mMuonPtOutJetSimplifiedTemplates[4],mMuonPtOutJetSimplifiedTemplates[4],0.7,(integralFakePTPromp_PlusC+0.3*integralDIF)/integralFakePTPromp_PlusC);
    hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp50_3->Add(mMuonPtOutJetSimplifiedTemplates[4],hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusC,1.5,(integralFakePTPromp_PlusC-0.5*integralDIF)/integralFakePTPromp_PlusC);
    hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl50_3->Add(mMuonPtOutJetSimplifiedTemplates[4],mMuonPtOutJetSimplifiedTemplates[4],0.5,(integralFakePTPromp_PlusC+0.5*integralDIF)/integralFakePTPromp_PlusC);
    hMuonPtOutJetSimplifiedMergedV1TemplatesDIFp75_3->Add(mMuonPtOutJetSimplifiedTemplates[4],hMuonPtOutJetSimplifiedMergedV1TemplatesFakePTPrompt_PlusC,1.75,(integralFakePTPromp_PlusC-0.75*integralDIF)/integralFakePTPromp_PlusC);
    hMuonPtOutJetSimplifiedMergedV1TemplatesDIFl75_3->Add(mMuonPtOutJetSimplifiedTemplates[4],mMuonPtOutJetSimplifiedTemplates[4],0.25,(integralFakePTPromp_PlusC+0.75*integralDIF)/integralFakePTPromp_PlusC);
  }

  // Taken such that the fractions of the 4 GeV templates will be the same
  // of the fractions of the 6 GeV ones
  double sfHFPion = 1.2034378078;
  double sfLFPion = 0.9317165867;
  double sfHFKaon = 1.2197514744;
  double sfLFKaon = 0.9951059847;
  hMuonDxyDIFPt4RescaledToPt6Template->Add(hMuonDxyDIFFromHFPionTemplate, hMuonDxyDIFFromLFPionTemplate,sfHFPion,sfLFPion);
  hMuonDxyDIFPt4RescaledToPt6Template->Add(hMuonDxyDIFPt4RescaledToPt6Template,hMuonDxyDIFFromHFKaonTemplate,1,sfHFKaon);
  hMuonDxyDIFPt4RescaledToPt6Template->Add(hMuonDxyDIFPt4RescaledToPt6Template,hMuonDxyDIFFromLFKaonTemplate,1,sfLFKaon);

  // Keep this line here!
  outFile->cd();

  std::ofstream outFileText;
  std::string outFileTextName = "MuonTemplatesProducer__EventsCounts__" + sampleName + "__" + text + ".txt";
  outFileText.open(outFileTextName.c_str());

  outFileText << "nBBEv = " << nBBEv << std::endl;
  outFileText << "nBBEv02 = " << nBBEv02 << std::endl;
  outFileText << "nBBEvOutsideCuts = " << nBBEvOutsideCuts << std::endl;
  outFileText << "nBBEvOutsideCuts02 = " << nBBEvOutsideCuts02 << std::endl;
  outFileText << "nBBEvInsideCuts = " << nBBEvInsideCuts << std::endl;
  outFileText << "nBBEvInsideCuts02 = " << nBBEvInsideCuts02 << std::endl;
  outFileText << "fraction of BB events with gen mu outside acceptance = " << (double)nBBEvOutsideCuts/(double)nBBEv << std::endl;
  outFileText << "fraction of BB events with both gen mu inside acceptance = " << (double)nBBEvInsideCuts/(double)nBBEv << std::endl;
  outFileText << "fraction of BB events (dxy < 0.2) with gen mu outside acceptance = " << (double)nBBEvOutsideCuts02/(double)nBBEv02 << std::endl;
  outFileText << "fraction of BB events (dxy < 0.2) with both gen mu inside acceptance = " << (double)nBBEvInsideCuts02/(double)nBBEv02 << std::endl;

  outFileText << std::endl;
  outFileText << "selected events = " << nSelEvents << std::endl;
  outFileText << "selected events (dxy < 0.2) = " << nSelEvents02 << std::endl;
  if(s->JetSelectorIsValid())
  {
    outFileText << "selected events (ptrel < 5.0) = " << nSelEvents50 << std::endl;
    outFileText << "selected events (dxy < 0.2 && ptrel < 5.0) = " << nSelEvents0250 << std::endl;
  }

  outFileText << "\nSelEventsMergedIPCodeOrdered\n";
  for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedIPCodeOrdered.begin(); it != mSelEventsMergedIPCodeOrdered.end(); it++)
  {
    outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents << std::endl;
  }
  outFileText << "\nSelEventsMergedIPCodeOrdered02\n";
  for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedIPCodeOrdered02.begin(); it != mSelEventsMergedIPCodeOrdered02.end(); it++)
  {
    outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents02 << std::endl;
  }
  if(s->JetSelectorIsValid())
  {
    outFileText << "\nSelEventsMergedIPCodeOrdered50\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedIPCodeOrdered50.begin(); it != mSelEventsMergedIPCodeOrdered50.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents50 << std::endl;
    }
    outFileText << "\nSelEventsMergedIPCodeOrdered0250\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedIPCodeOrdered0250.begin(); it != mSelEventsMergedIPCodeOrdered0250.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents0250 << std::endl;
    }
  }

  if(s->JetSelectorIsValid())
  {
    outFileText << "\nSelEventsMergedPtRelCodeV1Ordered\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV1Ordered.begin(); it != mSelEventsMergedPtRelCodeV1Ordered.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV1Ordered02\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV1Ordered02.begin(); it != mSelEventsMergedPtRelCodeV1Ordered02.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents02 << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV1Ordered50\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV1Ordered50.begin(); it != mSelEventsMergedPtRelCodeV1Ordered50.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents50 << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV1Ordered0250\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV1Ordered0250.begin(); it != mSelEventsMergedPtRelCodeV1Ordered0250.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents0250 << std::endl;
    }

    outFileText << "\nSelEventsMergedPtRelCodeV2Ordered\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV2Ordered.begin(); it != mSelEventsMergedPtRelCodeV2Ordered.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV2Ordered02\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV2Ordered02.begin(); it != mSelEventsMergedPtRelCodeV2Ordered02.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents02 << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV2Ordered50\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV2Ordered50.begin(); it != mSelEventsMergedPtRelCodeV2Ordered50.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents50 << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV2Ordered0250\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV2Ordered0250.begin(); it != mSelEventsMergedPtRelCodeV2Ordered0250.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents0250 << std::endl;
    }
  }

  outFileText << "\nSelEventsMergedIPCode\n";
  for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedIPCode.begin(); it != mSelEventsMergedIPCode.end(); it++)
  {
    outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents << std::endl;
  }
  outFileText << "\nSelEventsMergedIPCode02\n";
  for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedIPCode02.begin(); it != mSelEventsMergedIPCode02.end(); it++)
  {
    outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents02 << std::endl;
  }
  if(s->JetSelectorIsValid())
  {
    outFileText << "\nSelEventsMergedIPCode50\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedIPCode50.begin(); it != mSelEventsMergedIPCode50.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents50 << std::endl;
    }
    outFileText << "\nSelEventsMergedIPCode0250\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedIPCode0250.begin(); it != mSelEventsMergedIPCode0250.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents0250 << std::endl;
    }
  }

  if(s->JetSelectorIsValid())
  {
    outFileText << "\nSelEventsMergedPtRelCodeV1\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV1.begin(); it != mSelEventsMergedPtRelCodeV1.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV102\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV102.begin(); it != mSelEventsMergedPtRelCodeV102.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents02 << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV150\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV150.begin(); it != mSelEventsMergedPtRelCodeV150.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents50 << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV10250\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV10250.begin(); it != mSelEventsMergedPtRelCodeV10250.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents0250 << std::endl;
    }

    outFileText << "\nSelEventsMergedPtRelCodeV2\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV2.begin(); it != mSelEventsMergedPtRelCodeV2.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV202\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV202.begin(); it != mSelEventsMergedPtRelCodeV202.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents02 << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV250\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV250.begin(); it != mSelEventsMergedPtRelCodeV250.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents50 << std::endl;
    }
    outFileText << "\nSelEventsMergedPtRelCodeV20250\n";
    for(std::map<unsigned int, unsigned int>::const_iterator it = mSelEventsMergedPtRelCodeV20250.begin(); it != mSelEventsMergedPtRelCodeV20250.end(); it++)
    {
      outFileText << it->first << "\t" << it->second << "\t" << (double)(it->second)/nSelEvents0250 << std::endl;
    }
  }

  outFileText.close();

  // Create and write canvases here
  CreateAndWriteCanvas("cOutVsInAssocGenpId","colz",false,false,false,hOutVsInAssocGenpId);
  CreateAndWriteCanvas("cMuonClasses","colz",false,false,true,hMuonClasses);
  CreateAndWriteCanvas("cMuonMCProvenance","colz",false,false,true,hMuonMCProvenance);
  CreateAndWriteCanvas("cMuonMergedCode","colz",false,false,true,hMuonMergedCode);
  CreateAndWriteCanvas("cMuonSimplifiedIPCode","colz",false,false,true,hMuonSimplifiedIPCode);
  CreateAndWriteCanvas("cMuonSimplifiedMergedIPCode","colz",false,false,true,hMuonSimplifiedMergedIPCode);
  CreateAndWriteCanvas("cMuonSimplifiedMergedIPCodeOS","colz",false,false,true,hMuonSimplifiedMergedIPCodeOS);
  CreateAndWriteCanvas("cMuonSimplifiedMergedIPCodeSS","colz",false,false,true,hMuonSimplifiedMergedIPCodeSS);    
  CreateAndWriteCanvas("cMuonSimplifiedMergedIPCodeSSPlus","colz",false,false,true,hMuonSimplifiedMergedIPCodeSSPlus);    
  CreateAndWriteCanvas("cMuonSimplifiedMergedIPCodeSSMinus","colz",false,false,true,hMuonSimplifiedMergedIPCodeSSMinus);    
  
  if (s->JetSelectorIsValid()) {
    CreateAndWriteCanvas("cMuonSimplifiedPtRelCode","colz",false,false,true,hMuonSimplifiedPtRelCode);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeV0","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeV0);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeV1","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeV1);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeV2","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeV2);
    CreateAndWriteCanvas("cMuonSimplifiedPtRelCodeOS","colz",false,false,true,hMuonSimplifiedPtRelCodeOS);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeOSV0","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeOSV0);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeOSV1","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeOSV1);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeOSV2","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeOSV2);
    CreateAndWriteCanvas("cMuonSimplifiedPtRelCodeSS","colz",false,false,true,hMuonSimplifiedPtRelCodeSS);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSV0","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSV0);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSV1","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSV1);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSV2","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSV2);
    CreateAndWriteCanvas("cMuonSimplifiedPtRelCodeSSPlus","colz",false,false,true,hMuonSimplifiedPtRelCodeSSPlus);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSPlusV0","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSPlusV0);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSPlusV1","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSPlusV1);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSPlusV2","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSPlusV2);
    CreateAndWriteCanvas("cMuonSimplifiedPtRelCodeSSMinus","colz",false,false,true,hMuonSimplifiedPtRelCodeSSMinus);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSMinusV0","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSMinusV0);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSMinusV1","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSMinusV1);
    CreateAndWriteCanvas("cMuonSimplifiedMergedPtRelCodeSSMinusV2","colz",false,false,true,hMuonSimplifiedMergedPtRelCodeSSMinusV2);

    CreateAndWriteCanvas("cSingleMuSimplifiedPtRelCode",1,20,1,false,false,hSingleMuSimplifiedPtRelCode);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeV0",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeV0);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeV1",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeV1);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeV2",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeV2);
    CreateAndWriteCanvas("cSingleMuSimplifiedPtRelCodeOS",1,20,1,false,false,hSingleMuSimplifiedPtRelCodeOS);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeOSV0",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeOSV0);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeOSV1",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeOSV1);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeOSV2",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeOSV2);
    CreateAndWriteCanvas("cSingleMuSimplifiedPtRelCodeSS",1,20,1,false,false,hSingleMuSimplifiedPtRelCodeSS);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSV0",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSV0);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSV1",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSV1);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSV2",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSV2);
    CreateAndWriteCanvas("cSingleMuSimplifiedPtRelCodeSSPlus",1,20,1,false,false,hSingleMuSimplifiedPtRelCodeSSPlus);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSPlusV0",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSPlusV0);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSPlusV1",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSPlusV1);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSPlusV2",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSPlusV2);
    CreateAndWriteCanvas("cSingleMuSimplifiedPtRelCodeSSMinus",1,20,1,false,false,hSingleMuSimplifiedPtRelCodeSSMinus);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSMinusV0",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSMinusV0);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSMinusV1",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSMinusV1);
    CreateAndWriteCanvas("cSingleMuSimplifiedMergedPtRelCodeSSMinusV2",1,20,1,false,false,hSingleMuSimplifiedMergedPtRelCodeSSMinusV2);
  }

  // Uncomment this line to write also the histograms to the file
  outFile->Write();

}

double MuonTemplatesProducer::DxyD6TScaling(double dxy)
{
  return 1./(0.958708+0.76623*pow(dxy,1./1.49011));
}

#endif // MuonTemplatesProducer_cxx






