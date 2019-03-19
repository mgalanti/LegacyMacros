#define SimpleTMVAResultsAnalyzer_cxx
#include "SimpleTMVAResultsAnalyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void SimpleTMVAResultsAnalyzer::Loop(const int muonClassId, const std::string muonClassName)
{
//   In a ROOT session, you can do:
//      Root > .L SimpleTMVAResultsAnalyzer.C
//      Root > SimpleTMVAResultsAnalyzer t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;
   
   // Histogram binning and labels
   const unsigned int nPtBins = 150;
   const double ptMin = 0.;
   const double ptMax = 30.;
   const char* recoPtLabel = "reco p_{T} [GeV/c]";
   const unsigned int nEtaBins = 60;
   const double etaMin = -3.;
   const double etaMax = 3.;
   const char* recoEtaLabel = "reco #eta";
   
   const char* nMuLabel = "N_{#mu}";
   
   std::string outFileName = "SimpleTMVAResultsAnalyzer__Histograms__" + muonClassName + "__" + inputFileName;

   TFile* outFile = new TFile(outFileName.c_str(),"RECREATE");
   
   Long64_t nentries = fChain->GetEntriesFast();
   
   TH1D* hSelMuonTrackValidFraction = Create1DHistogram<TH1D>("hSelMuonTrackValidFraction","Fraction of valid hits in inner track from TMVA file", 100, 0., 1., "N_{valid}/N_{hits}", nMuLabel);
   TH1D* hSelMuonGlobalChi2 = Create1DHistogram<TH1D>("hSelMuonGlobalChi2","#chi^{2}/ndof in global track of selected muons", 100, 0., 10., "#chi^{2}/ndof", nMuLabel);
   TH1D* hSelMuonPt = Create1DHistogram<TH1D>("hSelMuonPt","p_{T} of selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
   TH2D* hSelMuonPtVsEta = Create2DHistogram<TH2D>("hSelMuonPtVsEta", "p_{T} vs. #eta of selected muons", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
   TH1D* hSelMuonEta = Create1DHistogram<TH1D>("hSelMuonEta","#eta of selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
   TH1D* hSelMuonSegmentCompatibility = Create1DHistogram<TH1D>("hSelMuonSegmentCompatibility","Segment compatibility of selected muons", 100, 0., 1., "Segment compatibility", nMuLabel);
   TH1D* hSelMuonGlobalKinkChi2LocalMomentum = Create1DHistogram<TH1D>("hSelMuonGlobalKinkChi2LocalMomentum","#chi^{2} of local momenta of STA-TK match of selected muons", 100, 0., 50., "Local p #chi^{2}", nMuLabel);
   TH1D* hSelMuonGlobalKinkChi2LocalPosition = Create1DHistogram<TH1D>("hSelMuonGlobalKinkChi2LocalPosition","#chi^{2} of local positions of STA-TK match of selected muons", 100, 0., 50., "Local position #chi^{2}", nMuLabel);
   TH1D* hSelMuonGlbTrackProbability = Create1DHistogram<TH1D>("hSelMuonGlbTrackProbability","-ln(Probability) of global track of selected muons", 100, 0., 10., "-ln(Probability)", nMuLabel);
   TH1D* hSelMuonTrackValidHits = Create1DHistogram<TH1D>("hSelMuonTrackValidHits","N. of valid hits in inner track of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
   TH1D* hSelMuonTrackExpOuterHits = Create1DHistogram<TH1D>("hSelMuonTrackExpOuterHits","N. of expected valid outer hits in inner track of selected muons", 11, -0.5, 10.5, "N_{exp hits}", nMuLabel);
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if(classID == muonClassId)
      {
        hSelMuonTrackValidFraction->Fill(trkValidFract,weight);
        hSelMuonGlobalChi2->Fill(glbNChi2,weight);
        hSelMuonPt->Fill(pt,weight);
        hSelMuonPtVsEta->Fill(eta, pt,weight);
        hSelMuonEta->Fill(eta,weight);
        hSelMuonSegmentCompatibility->Fill(segComp,weight);
        hSelMuonGlobalKinkChi2LocalMomentum->Fill(chi2LocMom,weight);
        hSelMuonGlobalKinkChi2LocalPosition->Fill(chi2LocPos,weight);
        hSelMuonGlbTrackProbability->Fill(glbTrackProb,weight);
        hSelMuonTrackValidHits->Fill(NTrkVHits,weight);
        hSelMuonTrackExpOuterHits->Fill(NTrkEHitsOut,weight);
      }
   }
   outFile->cd();
   CreateAndWriteCanvas("cSelMuonTrackValidFraction",0, 21, 1, false, false, hSelMuonTrackValidFraction);
   CreateAndWriteCanvas("cSelMuonGlobalChi2",0, 21, 1, false, false, hSelMuonGlobalChi2);
   CreateAndWriteCanvas("cSelMuonPt", 0, 21, 1, false, true, hSelMuonPt);
   CreateAndWriteCanvas("cSelMuonPtVsEta", "colz", false, false, true, hSelMuonPtVsEta);
   CreateAndWriteCanvas("cSelMuonEta", 0, 21, 1, false, false, hSelMuonEta);
   CreateAndWriteCanvas("cSelMuonSegmentCompatibility", 0, 21, 1, false, false, hSelMuonSegmentCompatibility);
   CreateAndWriteCanvas("cSelMuonGlobalKinkChi2LocalMomentum", 0, 21, 1, false, false, hSelMuonGlobalKinkChi2LocalMomentum);
   CreateAndWriteCanvas("cSelMuonGlobalKinkChi2LocalPosition", 0, 21, 1, false, false, hSelMuonGlobalKinkChi2LocalPosition);
   CreateAndWriteCanvas("cSelMuonGlbTrackProbability", 0, 21, 1, false, false, hSelMuonGlbTrackProbability);
   CreateAndWriteCanvas("cSelMuonTrackValidHits", 0, 21, 1, false, false, hSelMuonTrackValidHits);
   CreateAndWriteCanvas("cSelMuonTrackExpOuterHits", 0, 21, 1, false, false, hSelMuonTrackExpOuterHits);
   
   outFile->Write();
}
