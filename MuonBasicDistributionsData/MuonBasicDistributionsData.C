#ifndef MuonBasicDistributionsData_cxx
#define MuonBasicDistributionsData_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "MuonBasicDistributionsData.h"
#include "../utilities/constants.h"

void MuonBasicDistributionsData::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";
  
  // Histogram binning and labels
  const unsigned int nPtBins = 150;
  const double ptMin = 0.;
  const double ptMax = 30.;
  const char* recoPtLabel = "reco p_{T} [GeV/c]";
  const unsigned int nEtaBins = 60;
  const double etaMin = -3.;
  const double etaMax = 3.;
  const char* recoEtaLabel = "reco #eta";
  const double nPhiBins = 60;
  const double phiMin = -constants::PI;
  const double phiMax = constants::PI;
  const char* recoPhiLabel = "reco #phi";

  const unsigned int nJetPtBins = 200;
  const double jetPtMin = 0.;
  const double jetPtMax = 100.;
  const char* recoJetPtLabel = "reco p_{T} [GeV/c]";
  const unsigned int nJetEtaBins = 100;
  const double jetEtaMin = -5.;
  const double jetEtaMax = 5.;
  const char* recoJetEtaLabel = "reco #eta";
  const double nJetPhiBins = 60;
  const double jetPhiMin = -constants::PI;
  const double jetPhiMax = constants::PI;
  const char* recoJetPhiLabel = "reco #phi";
  
  // This is the one that makes more sense for data
  const std::string effMethod = "MCTruthBB";
  
  const char* nMuLabel = "N_{#mu}";
  const char* nVtxLabel = "N_{vtx}";
  const char* nEvLabel = "N_{ev}";
  const char* nJetLabel = "N_{jet}";
  
  // Book histograms here
  TH1D* hRecoMuonPt = Create1DHistogram<TH1D>("hRecoMuonPt","p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  TH1D* hSelMuonPt = Create1DHistogram<TH1D>("hSelMuonPt","p_{T} of selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  TH1D* hSelEffCorMuonPt = Create1DHistogram<TH1D>("hSelEffCorMuonPt","p_{T} of selected muons (eff. corr.)", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  TH1D* hAllSelMuonPt = Create1DHistogram<TH1D>("hAllSelMuonPt","p_{T} of all selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  
  TH1D* hSelMuonLeadingPt = Create1DHistogram<TH1D>("hSelMuonLeadingPt","p_{T} of leading selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  TH1D* hSelEffCorMuonLeadingPt = Create1DHistogram<TH1D>("hSelEffCorMuonLeadingPt","p_{T} of leading selected muons (eff. corr.)", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  TH1D* hSelMuonSecondPt = Create1DHistogram<TH1D>("hSelMuonSecondPt","p_{T} of 2nd leading selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  TH1D* hSelEffCorMuonSecondPt = Create1DHistogram<TH1D>("hSelEffCorMuonSecondPt","p_{T} of 2nd leading selected muons (eff. corr.)", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  
  TH1D* hRecoMuonEta = Create1DHistogram<TH1D>("hRecoMuonEta","#eta of reconstructed muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hSelMuonEta = Create1DHistogram<TH1D>("hSelMuonEta","#eta of selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hSelEffCorMuonEta = Create1DHistogram<TH1D>("hSelEffCorMuonEta","#eta of selected muons (eff. corr.)", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hAllSelMuonEta = Create1DHistogram<TH1D>("hAllSelMuonEta","#eta of all selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);

  TH1D* hSelMuonLeadingEta = Create1DHistogram<TH1D>("hSelMuonLeadingEta","#eta of leading-p_{T} selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hSelEffCorMuonLeadingEta = Create1DHistogram<TH1D>("hSelEffCorMuonLeadingEta","#eta of leading-p_{T} selected muons (eff. corr.)", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hSelMuonSecondEta = Create1DHistogram<TH1D>("hSelMuonSecondEta","#eta of second leading-p_{T} selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hSelEffCorMuonSecondEta = Create1DHistogram<TH1D>("hSelEffCorMuonSecondEta","#eta of second leading-p_{T} selected muons (eff. corr.)", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  
  TH1D* hRecoMuonPhi = Create1DHistogram<TH1D>("hRecoMuonPhi","#phi of reconstructed muons", nPhiBins, phiMin, phiMax, recoPhiLabel, nMuLabel);
  TH1D* hSelMuonPhi = Create1DHistogram<TH1D>("hSelMuonPhi","#phi of selected muons", nPhiBins, phiMin, phiMax, recoPhiLabel, nMuLabel);
  TH1D* hSelEffCorMuonPhi = Create1DHistogram<TH1D>("hSelEffCorMuonPhi","#phi of selected muons (eff. corr.)", nPhiBins, phiMin, phiMax, recoPhiLabel, nMuLabel);
  TH1D* hAllSelMuonPhi = Create1DHistogram<TH1D>("hAllSelMuonPhi","#phi of all selected muons", nPhiBins, phiMin, phiMax, recoPhiLabel, nMuLabel);
  
  TH1D* hRecoMuonCharge = Create1DHistogram<TH1D>("hRecoMuonCharge","Charge of reconstructed muons", 3, -1.5, 1.5, "Charge", nMuLabel);
  TH1D* hSelMuonCharge = Create1DHistogram<TH1D>("hSelMuonCharge","Charge of selected muons", 3, -1.5, 1.5, "Charge", nMuLabel);
  TH1D* hSelEffCorMuonCharge = Create1DHistogram<TH1D>("hSelEffCorMuonCharge","Charge of selected muons (eff. corr.)", 3, -1.5, 1.5, "Charge", nMuLabel);
  TH1D* hAllSelMuonCharge = Create1DHistogram<TH1D>("hAllSelMuonCharge","Charge of all selected muons", 3, -1.5, 1.5, "Charge", nMuLabel);
  
  TH1D* hRecoMuonHasTriggered = Create1DHistogram<TH1D>("hRecoMuonHasTriggered","Trigger matching of reconstructed muons", 2, -0.5, 1.5, "Has triggered", nMuLabel);
  TH1D* hSelMuonHasTriggered = Create1DHistogram<TH1D>("hSelMuonHasTriggered","Trigger matching of selected muons", 2, -0.5, 1.5, "Has triggered", nMuLabel);
  TH1D* hAllSelMuonHasTriggered = Create1DHistogram<TH1D>("hAllSelMuonHasTriggered","Trigger matching of all selected muons", 2, -0.5, 1.5, "Has triggered", nMuLabel);
  
  TH1D* hRecoMuonCaloCompatibility = Create1DHistogram<TH1D>("hRecoMuonCaloCompatibility","Calo compatibility of reconstructed muons", 100, 0., 1., "Calo compatibility", nMuLabel);
  TH1D* hSelMuonCaloCompatibility = Create1DHistogram<TH1D>("hSelMuonCaloCompatibility","Calo compatibility of selected muons", 100, 0., 1., "Calo compatibility", nMuLabel);
  TH1D* hSelEffCorMuonCaloCompatibility = Create1DHistogram<TH1D>("hSelEffCorMuonCaloCompatibility","Calo compatibility of selected muons (eff. corr.)", 100, 0., 1., "Calo compatibility", nMuLabel);
  TH1D* hAllSelMuonCaloCompatibility = Create1DHistogram<TH1D>("hAllSelMuonCaloCompatibility","Calo compatibility of all selected muons", 100, 0., 1., "Calo compatibility", nMuLabel);
  
  TH1D* hRecoMuonSegmentCompatibility = Create1DHistogram<TH1D>("hRecoMuonSegmentCompatibility","Segment compatibility of reconstructed muons", 100, 0., 1., "Segment compatibility", nMuLabel);
  TH1D* hSelMuonSegmentCompatibility = Create1DHistogram<TH1D>("hSelMuonSegmentCompatibility","Segment compatibility of selected muons", 100, 0., 1., "Segment compatibility", nMuLabel);
  TH1D* hSelEffCorMuonSegmentCompatibility = Create1DHistogram<TH1D>("hSelEffCorMuonSegmentCompatibility","Segment compatibility of selected muons (eff. corr.)", 100, 0., 1., "Segment compatibility", nMuLabel);
  TH1D* hAllSelMuonSegmentCompatibility = Create1DHistogram<TH1D>("hAllSelMuonSegmentCompatibility","Segment compatibility of all selected muons", 100, 0., 1., "Segment compatibility", nMuLabel);
  
  TH1D* hRecoMuonTrackValidHits = Create1DHistogram<TH1D>("hRecoMuonTrackValidHits","N. of valid hits in inner track of reconstructed muons", 41, -0.5, 40.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonTrackValidHits = Create1DHistogram<TH1D>("hSelMuonTrackValidHits","N. of valid hits in inner track of selected muons", 41, -0.5, 40.5, "N_{hits}", nMuLabel);
  TH1D* hSelEffCorMuonTrackValidHits = Create1DHistogram<TH1D>("hSelEffCorMuonTrackValidHits","N. of valid hits in inner track of selected muons (eff. corr.)", 41, -0.5, 40.5, "N_{hits}", nMuLabel);
  TH1D* hAllSelMuonTrackValidHits = Create1DHistogram<TH1D>("hAllSelMuonTrackValidHits","N. of valid hits in inner track of all selected muons", 41, -0.5, 40.5, "N_{hits}", nMuLabel);
  
  TH1D* hRecoMuonTrackChi2 = Create1DHistogram<TH1D>("hRecoMuonTrackChi2","#chi^{2}/ndof in inner track of reconstructed muons", 150, 0., 15., "#chi^{2}/ndof", nMuLabel);
  TH1D* hSelMuonTrackChi2 = Create1DHistogram<TH1D>("hSelMuonTrackChi2","#chi^{2}/ndof in inner track of selected muons", 150, 0., 15., "#chi^{2}/ndof", nMuLabel);
  TH1D* hSelEffCorMuonTrackChi2 = Create1DHistogram<TH1D>("hSelEffCorMuonTrackChi2","#chi^{2}/ndof in inner track of selected muons (eff. corr.)", 150, 0., 15., "#chi^{2}/ndof", nMuLabel);
  TH1D* hAllSelMuonTrackChi2 = Create1DHistogram<TH1D>("hAllSelMuonTrackChi2","#chi^{2}/ndof in inner track of all selected muons", 150, 0., 15., "#chi^{2}/ndof", nMuLabel);

  TH1D* hRecoMuonTrackNdof = Create1DHistogram<TH1D>("hRecoMuonTrackNdof","ndof in inner track of reconstructed muons", 51, -0.5, 50.5, "ndof", nMuLabel);
  TH1D* hSelMuonTrackNdof = Create1DHistogram<TH1D>("hSelMuonTrackNdof","ndof in inner track of selected muons", 51, -0.5, 50.5, "ndof", nMuLabel);
  TH1D* hSelEffCorMuonTrackNdof = Create1DHistogram<TH1D>("hSelEffCorMuonTrackNdof","ndof in inner track of selected muons (eff. corr.)", 51, -0.5, 50.5, "ndof", nMuLabel);
  TH1D* hAllSelMuonTrackNdof = Create1DHistogram<TH1D>("hAllSelMuonTrackNdof","ndof in inner track of all selected muons", 51, -0.5, 50.5, "ndof", nMuLabel);

  TH1D* hRecoMuonTrackValidPixelHits = Create1DHistogram<TH1D>("hRecoMuonTrackValidPixelHits","N. of valid pixel hits in inner track of reconstructed muons", 11, -0.5, 10.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonTrackValidPixelHits = Create1DHistogram<TH1D>("hSelMuonTrackValidPixelHits","N. of valid pixel hits in inner track of selected muons", 11, -0.5, 10.5, "N_{hits}", nMuLabel);
  TH1D* hSelEffCorMuonTrackValidPixelHits = Create1DHistogram<TH1D>("hSelEffCorMuonTrackValidPixelHits","N. of valid pixel hits in inner track of selected muons (eff. corr.)", 11, -0.5, 10.5, "N_{hits}", nMuLabel);
  TH1D* hAllSelMuonTrackValidPixelHits = Create1DHistogram<TH1D>("hAllSelMuonTrackValidPixelHits","N. of valid pixel hits in inner track of all selected muons", 11, -0.5, 10.5, "N_{hits}", nMuLabel);

  TH1D* hRecoMuonGlobalChi2 = Create1DHistogram<TH1D>("hRecoMuonGlobalChi2","#chi^{2}/ndof in global track of reconstructed muons", 150, 0., 15., "#chi^{2}/ndof", nMuLabel);
  TH1D* hSelMuonGlobalChi2 = Create1DHistogram<TH1D>("hSelMuonGlobalChi2","#chi^{2}/ndof in global track of selected muons", 150, 0., 15., "#chi^{2}/ndof", nMuLabel);
  TH1D* hSelEffCorMuonGlobalChi2 = Create1DHistogram<TH1D>("hSelEffCorMuonGlobalChi2","#chi^{2}/ndof in global track of selected muons (eff. corr.)", 150, 0., 15., "#chi^{2}/ndof", nMuLabel);
  TH1D* hAllSelMuonGlobalChi2 = Create1DHistogram<TH1D>("hAllSelMuonGlobalChi2","#chi^{2}/ndof in global track of all selected muons", 150, 0., 15., "#chi^{2}/ndof", nMuLabel);

  TH1D* hRecoMuonGlobalValidMuonHits = Create1DHistogram<TH1D>("hRecoMuonGlobalValidMuonHits","N. of valid muon hits in global track of reconstructed muons", 81, -0.5, 80.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonGlobalValidMuonHits = Create1DHistogram<TH1D>("hSelMuonGlobalValidMuonHits","N. of valid muon hits in global track of selected muons", 81, -0.5, 80.5, "N_{hits}", nMuLabel);
  TH1D* hSelEffCorMuonGlobalValidMuonHits = Create1DHistogram<TH1D>("hSelEffCorMuonGlobalValidMuonHits","N. of valid muon hits in global track of selected muons (eff. corr.)", 81, -0.5, 80.5, "N_{hits}", nMuLabel);
  TH1D* hAllSelMuonGlobalValidMuonHits = Create1DHistogram<TH1D>("hAllSelMuonGlobalValidMuonHits","N. of valid muon hits in global track of all selected muons", 81, -0.5, 80.5, "N_{hits}", nMuLabel);

  TH1D* hRecoMuonNumberOfMatches = Create1DHistogram<TH1D>("hRecoMuonNumberOfMatches","N. of matches of reconstructed muons", 11, -0.5, 10.5, "N_{matches}", nMuLabel);
  TH1D* hSelMuonNumberOfMatches = Create1DHistogram<TH1D>("hSelMuonNumberOfMatches","N. of matches of selected muons", 11, -0.5, 10.5, "N_{matches}", nMuLabel);
  TH1D* hSelEffCorMuonNumberOfMatches = Create1DHistogram<TH1D>("hSelEffCorMuonNumberOfMatches","N. of matches of selected muons (eff. corr.)", 11, -0.5, 10.5, "N_{matches}", nMuLabel);
  TH1D* hAllSelMuonNumberOfMatches = Create1DHistogram<TH1D>("hAllSelMuonNumberOfMatches","N. of matches of all selected muons", 11, -0.5, 10.5, "N_{matches}", nMuLabel);
  
  
  TH1D* hRecoJetPt = 0;
  TH1D* hSelJetPt = 0;
  TH1D* hAllSelJetPt = 0;
  
  TH1D* hSelJetLeadingPt = 0;
  TH1D* hSelJetSecondPt = 0;
  
  TH1D* hRecoJetEta = 0;
  TH1D* hSelJetEta = 0;
  TH1D* hAllSelJetEta = 0;
  
  TH1D* hSelJetLeadingEta = 0;
  TH1D* hSelJetSecondEta = 0;
  
  TH1D* hRecoJetPhi = 0;
  TH1D* hSelJetPhi = 0;
  TH1D* hAllSelJetPhi = 0;
  
  TH1D* hRecoJetEcfr = 0;
  TH1D* hSelJetEcfr = 0;
  TH1D* hAllSelJetEcfr = 0;
  
  TH1D* hRecoJetEnfr = 0;
  TH1D* hSelJetEnfr = 0;
  TH1D* hAllSelJetEnfr = 0;
  
  TH1D* hRecoJetHcfr = 0;
  TH1D* hSelJetHcfr = 0;
  TH1D* hAllSelJetHcfr = 0;
  
  TH1D* hRecoJetHnfr = 0;
  TH1D* hSelJetHnfr = 0;
  TH1D* hAllSelJetHnfr = 0;
  
  TH1D* hRecoJetMass = 0;
  TH1D* hSelJetMass = 0;
  TH1D* hAllSelJetMass = 0;
  
  TH1D* hRecoJetNConstituents = 0;
  TH1D* hSelJetNConstituents = 0;
  TH1D* hAllSelJetNConstituents = 0;
  
  TH1D* hRecoJetChargedMultiplicity = 0;
  TH1D* hSelJetChargedMultiplicity = 0;
  TH1D* hAllSelJetChargedMultiplicity = 0;
  
  TH1D* hRecoJetTrackMultiplicity = 0;
  TH1D* hSelJetTrackMultiplicity = 0;
  TH1D* hAllSelJetTrackMultiplicity = 0;
  
  TH1D* hRecoJetNSecondaryVertex = 0;
  TH1D* hSelJetNSecondaryVertex = 0;
  TH1D* hAllSelJetNSecondaryVertex = 0;
  
  TH1D* hSelJetFractionTracksAssociatedToVertex = 0;
  TH1D* hSelJetWeightedFractionTracksAssociatedToVertex = 0;
  TH1D* hSelJetTrackDzWrtSelectedVertex = 0;
  TH1D* hSelJetWeightedTrackDzWrtSelectedVertex = 0;
  TH1D* hSelJetAverageTrackDzWrtSelectedVertex = 0;
  TH1D* hSelJetWeightedAverageTrackDzWrtSelectedVertex = 0;
  TH1D* hSelJetRMSTrackDzWrtSelectedVertex = 0;
  TH1D* hSelJetWeightedRMSTrackDzWrtSelectedVertex = 0;
  
  if(s->JetSelectorIsValid())
  {
    hRecoJetPt = Create1DHistogram<TH1D>("hRecoJetPt","p_{T} of reconstructed jets", nJetPtBins, jetPtMin, jetPtMax, recoJetPtLabel, nJetLabel);
    hSelJetPt = Create1DHistogram<TH1D>("hSelJetPt","p_{T} of selected jets", nJetPtBins, jetPtMin, jetPtMax, recoJetPtLabel, nJetLabel);
    hAllSelJetPt = Create1DHistogram<TH1D>("hAllSelJetPt","p_{T} of all selected jets", nJetPtBins, jetPtMin, jetPtMax, recoJetPtLabel, nJetLabel);
    
    hSelJetLeadingPt = Create1DHistogram<TH1D>("hSelJetLeadingPt","p_{T} of leading selected jets", nJetPtBins, jetPtMin, jetPtMax, recoJetPtLabel, nJetLabel);
    hSelJetSecondPt = Create1DHistogram<TH1D>("hSelJetSecondPt","p_{T} of 2nd leading selected jets", nJetPtBins, jetPtMin, jetPtMax, recoJetPtLabel, nJetLabel);
    
    hRecoJetEta = Create1DHistogram<TH1D>("hRecoJetEta","#eta of reconstructed jets", nJetEtaBins, jetEtaMin, jetEtaMax, recoJetEtaLabel, nJetLabel);
    hSelJetEta = Create1DHistogram<TH1D>("hSelJetEta","#eta of selected jets", nJetEtaBins, jetEtaMin, jetEtaMax, recoJetEtaLabel, nJetLabel);
    hAllSelJetEta = Create1DHistogram<TH1D>("hAllSelJetEta","#eta of all selected jets", nJetEtaBins, jetEtaMin, jetEtaMax, recoJetEtaLabel, nJetLabel);
    
    hSelJetLeadingEta = Create1DHistogram<TH1D>("hSelJetLeadingEta","#eta of leading-p_{T} selected jets", nJetEtaBins, jetEtaMin, jetEtaMax, recoJetEtaLabel, nJetLabel);
    hSelJetSecondEta = Create1DHistogram<TH1D>("hSelJetSecondEta","#eta of second leading-p_{T} selected jets", nJetEtaBins, jetEtaMin, jetEtaMax, recoJetEtaLabel, nJetLabel);
    
    hRecoJetPhi = Create1DHistogram<TH1D>("hRecoJetPhi","#phi of reconstructed jets", nJetPhiBins, jetPhiMin, jetPhiMax, recoJetPhiLabel, nJetLabel);
    hSelJetPhi = Create1DHistogram<TH1D>("hSelJetPhi","#phi of selected jets", nJetPhiBins, jetPhiMin, jetPhiMax, recoJetPhiLabel, nJetLabel);
    hAllSelJetPhi = Create1DHistogram<TH1D>("hAllSelJetPhi","#phi of all selected jets", nJetPhiBins, jetPhiMin, jetPhiMax, recoJetPhiLabel, nJetLabel);
    
    hRecoJetEcfr = Create1DHistogram<TH1D>("hRecoJetEcfr","Electromagnetic charged energy fraction of reconstructed jets", 500, 0., 1., "EC fraction", nJetLabel);
    hSelJetEcfr = Create1DHistogram<TH1D>("hSelJetEcfr","Electromagnetic charged energy fraction of selected jets", 500, 0., 1., "EC fraction", nJetLabel);
    hAllSelJetEcfr = Create1DHistogram<TH1D>("hAllSelJetEcfr","Electromagnetic charged energy fraction of all selected jets", 500, 0., 1., "EC fraction", nJetLabel);
    
    hRecoJetEnfr = Create1DHistogram<TH1D>("hRecoJetEnfr","Electromagnetic neutral energy fraction of reconstructed jets", 500, 0., 1., "EN fraction", nJetLabel);
    hSelJetEnfr = Create1DHistogram<TH1D>("hSelJetEnfr","Electromagnetic neutral energy fraction of selected jets", 500, 0., 1., "EN fraction", nJetLabel);
    hAllSelJetEnfr = Create1DHistogram<TH1D>("hAllSelJetEnfr","Electromagnetic neutral energy fraction of all selected jets", 500, 0., 1., "EN fraction", nJetLabel);
    
    hRecoJetHcfr = Create1DHistogram<TH1D>("hRecoJetHcfr","Hadronic charged energy fraction of reconstructed jets", 500, 0., 1., "HC fraction", nJetLabel);
    hSelJetHcfr = Create1DHistogram<TH1D>("hSelJetHcfr","Hadronic charged energy fraction of selected jets", 500, 0., 1., "HC fraction", nJetLabel);
    hAllSelJetHcfr = Create1DHistogram<TH1D>("hAllSelJetHcfr","Hadronic charged energy fraction of all selected jets", 500, 0., 1., "HC fraction", nJetLabel);
    
    hRecoJetHnfr = Create1DHistogram<TH1D>("hRecoJetHnfr","Hadronic neutral energy fraction of reconstructed jets", 500, 0., 1., "HN fraction", nJetLabel);
    hSelJetHnfr = Create1DHistogram<TH1D>("hSelJetHnfr","Hadronic neutral energy fraction of selected jets", 500, 0., 1., "HN fraction", nJetLabel);
    hAllSelJetHnfr = Create1DHistogram<TH1D>("hAllSelJetHnfr","Hadronic neutral energy fraction of all selected jets", 500, 0., 1., "HN fraction", nJetLabel);
    
    hRecoJetMass = Create1DHistogram<TH1D>("hRecoJetMass","Invariant mass of reconstructed jets", 500, 0., 50., "Mass (GeV)", nJetLabel);
    hSelJetMass = Create1DHistogram<TH1D>("hSelJetMass","Invariant mass of selected jets", 500, 0., 50., "Mass (GeV)", nJetLabel);
    hAllSelJetMass = Create1DHistogram<TH1D>("hAllSelJetMass","Invariant mass of all selected jets", 500, 0., 50., "Mass (GeV)", nJetLabel);
    
    hRecoJetNConstituents = Create1DHistogram<TH1D>("hRecoJetNConstituents","Number of constituents of reconstructed jets", 100, -0.5, 99.5, "# constituents", nJetLabel);
    hSelJetNConstituents = Create1DHistogram<TH1D>("hSelJetNConstituents","Number of constituents of selected jets", 100, -0.5, 99.5, "# constituents", nJetLabel);
    hAllSelJetNConstituents = Create1DHistogram<TH1D>("hAllSelJetNConstituents","Number of constituents of all selected jets", 100, -0.5, 99.5, "# constituents", nJetLabel);
    
    hRecoJetChargedMultiplicity = Create1DHistogram<TH1D>("hRecoJetChargedMultiplicity","Charged multiplicity of reconstructed jets", 100, -0.5, 99.5, "Charged multiplicity", nJetLabel);
    hSelJetChargedMultiplicity = Create1DHistogram<TH1D>("hSelJetChargedMultiplicity","Charged multiplicity of selected jets", 100, -0.5, 99.5, "Charged multiplicity", nJetLabel);
    hAllSelJetChargedMultiplicity = Create1DHistogram<TH1D>("hAllSelJetChargedMultiplicity","Charged multiplicity of all selected jets", 100, -0.5, 99.5, "Charged multiplicity", nJetLabel);
    
    hRecoJetTrackMultiplicity = Create1DHistogram<TH1D>("hRecoJetTrackMultiplicity","Track multiplicity of reconstructed jets", 100, -0.5, 99.5, "Track multiplicity", nJetLabel);
    hSelJetTrackMultiplicity = Create1DHistogram<TH1D>("hSelJetTrackMultiplicity","Track multiplicity of selected jets", 100, -0.5, 99.5, "Track multiplicity", nJetLabel);
    hAllSelJetTrackMultiplicity = Create1DHistogram<TH1D>("hAllSelJetTrackMultiplicity","Track multiplicity of all selected jets", 100, -0.5, 99.5, "Track multiplicity", nJetLabel);
    
    hRecoJetNSecondaryVertex = Create1DHistogram<TH1D>("hRecoJetNSecondaryVertex","Number of secondary vertices of reconstructed jets", 6, -0.5, 5.5, "# secondary vertices", nJetLabel);
    hSelJetNSecondaryVertex = Create1DHistogram<TH1D>("hSelJetNSecondaryVertex","Number of secondary vertices of selected jets", 6, -0.5, 5.5, "# secondary vertices", nJetLabel);
    hAllSelJetNSecondaryVertex = Create1DHistogram<TH1D>("hAllSelJetNSecondaryVertex","Number of secondary vertices of all selected jets", 6, -0.5, 5.5, "# secondary vertices", nJetLabel);
    
    hSelJetFractionTracksAssociatedToVertex = Create1DHistogram<TH1D>("hSelJetFractionTracksAssociatedToVertex","Fraction of tracks in selected jet associated to selected vertex", 500, 0., 1., "Fraction", nJetLabel);
    hSelJetWeightedFractionTracksAssociatedToVertex = Create1DHistogram<TH1D>("hSelJetWeightedFractionTracksAssociatedToVertex","p_{T}-weighted fraction of tracks in selected jet associated to selected vertex", 500, 0., 1., "Weighted fraction", nJetLabel);
    hSelJetTrackDzWrtSelectedVertex = Create1DHistogram<TH1D>("hSelJetTrackDzWrtSelectedVertex","d_{z} of tracks in selected jets with respect to selected vertex", 500, -10., 10., "d_{z} [cm]", nJetLabel);
    hSelJetWeightedTrackDzWrtSelectedVertex = Create1DHistogram<TH1D>("hSelJetWeightedTrackDzWrtSelectedVertex","p_{T}-weighted d_{z} of tracks in selected jets with respect to selected vertex", 500, -10., 10., "weighted d_{z} [cm]", nJetLabel);
    hSelJetAverageTrackDzWrtSelectedVertex = Create1DHistogram<TH1D>("hSelJetAverageTrackDzWrtSelectedVertex","Average d_{z} of tracks in selected jets with respect to selected vertex", 500, -10., 10., "<d_{z}> [cm]", nJetLabel);
    hSelJetWeightedAverageTrackDzWrtSelectedVertex = Create1DHistogram<TH1D>("hSelJetWeightedAverageTrackDzWrtSelectedVertex","p_{T}-weighted average d_{z} of tracks in selected jets with respect to selected vertex", 500, -10., 10., "weighted <d_{z}> [cm]", nJetLabel);
    hSelJetRMSTrackDzWrtSelectedVertex = Create1DHistogram<TH1D>("hSelJetRMSTrackDzWrtSelectedVertex","d_{z} RMS of tracks in selected jets with respect to selected vertex", 500, 0., 10., "RMS(d_{z}) [cm]", nJetLabel);
    hSelJetWeightedRMSTrackDzWrtSelectedVertex = Create1DHistogram<TH1D>("hSelJetWeightedRMSTrackDzWrtSelectedVertex","p_{T}-weighted d_{z} RMS of tracks in selected jets with respect to selected vertex", 500, 0., 10., "weighted RMS(d_{z}) [cm]", nJetLabel);
  }
  
  TH1D* hSelMuonDxy = Create1DHistogram<TH1D>("hSelMuonDxy", "d_{xy} of selected muons w.r.t. the selected vertex", 500, 0., 0.5, "d_{xy} [cm]", nMuLabel);
  TH1D* hSelEffCorMuonDxy = Create1DHistogram<TH1D>("hSelEffCorMuonDxy", "d_{xy} of selected muons w.r.t. the selected vertex (eff. corr.)", 500, 0., 0.5, "d_{xy} [cm]", nMuLabel);
  
  TH1D* hSelMuonDxyError = Create1DHistogram<TH1D>("hSelMuonDxyError", "#sigma(d_{xy}) of selected muons w.r.t. the selected vertex", 100, 0., 0.02, "#sigma(d_{xy}) [cm]", nMuLabel);
  TH1D* hSelEffCorMuonDxyError = Create1DHistogram<TH1D>("hSelEffCorMuonDxyError", "#sigma(d_{xy}) of selected muons w.r.t. the selected vertex (eff. corr.)", 100, 0., 0.02, "#sigma(d_{xy}) [cm]", nMuLabel);
  
  TH1D* hSelMuonDxyz = Create1DHistogram<TH1D>("hSelMuonDxyz", "d_{xyz} of selected muons w.r.t. the selected vertex", 500, 0., 0.5, "d_{xyz} [cm]", nMuLabel);
  TH1D* hSelEffCorMuonDxyz = Create1DHistogram<TH1D>("hSelEffCorMuonDxyz", "d_{xyz} of selected muons w.r.t. the selected vertex (eff. corr.)", 500, 0., 0.5, "d_{xyz} [cm]", nMuLabel);
  
  TH1D* hSelMuonDxyzError = Create1DHistogram<TH1D>("hSelMuonDxyzError", "#sigma(d_{xyz}) of selected muons w.r.t. the selected vertex", 100, 0., 0.02, "#sigma(d_{xyz}) [cm]", nMuLabel);
  TH1D* hSelEffCorMuonDxyzError = Create1DHistogram<TH1D>("hSelEffCorMuonDxyzError", "#sigma(d_{xyz}) of selected muons w.r.t. the selected vertex (eff. corr.)", 100, 0., 0.02, "#sigma(d_{xyz}) [cm]", nMuLabel);
  
  TH1D* hSelMuonDz = Create1DHistogram<TH1D>("hSelMuonDz", "d_{z} of selected muons w.r.t. the selected vertex", 1000, -0.5, 0.5, "d_{z} [cm]", nMuLabel);
  TH1D* hSelEffCorMuonDz = Create1DHistogram<TH1D>("hSelEffCorMuonDz", "d_{z} of selected muons w.r.t. the selected vertex (eff. corr.)", 1000, -0.5, 0.5, "d_{z} [cm]", nMuLabel);
  
  TH1D* hSelMuonAbsDz = Create1DHistogram<TH1D>("hSelMuonAbsDz", "abs(d_{z}) of selected muons w.r.t. the selected vertex", 500, 0., 0.5, "abs(d_{z}) [cm]", nMuLabel);
  TH1D* hSelEffCorMuonAbsDz = Create1DHistogram<TH1D>("hSelEffCorMuonAbsDz", "abs(d_{z}) of selected muons w.r.t. the selected vertex (eff. corr.)", 500, 0., 0.5, "abs(d_{z}) [cm]", nMuLabel);
  
  TH1D* hSelMuonDzError = Create1DHistogram<TH1D>("hSelMuonDzError", "#sigma(d_{z}) of selected muons w.r.t. the selected vertex", 100, 0., 0.01, "#sigma(d_{z}) [cm]", nMuLabel);
  TH1D* hSelEffCorMuonDzError = Create1DHistogram<TH1D>("hSelEffCorMuonDzError", "#sigma(d_{z}) of selected muons w.r.t. the selected vertex (eff. corr.)", 100, 0., 0.01, "#sigma(d_{z}) [cm]", nMuLabel);
  
  TH1D* hSelMuonSignedDxyBS = Create1DHistogram<TH1D>("hSelMuonDxyBS", "Signed d_{xy} of selected muons w.r.t. the beam spot", 2000, -0.5, 0.5, "d_{xy} [cm]", nMuLabel);
  TH1D* hSelEffCorMuonSignedDxyBS = Create1DHistogram<TH1D>("hSelEffCorMuonDxyBS", "Signed d_{xy} of selected muons w.r.t. the beam spot (eff. corr.)", 2000, -0.5, 0.5, "d_{xy} [cm]", nMuLabel);
  
  TH1D* hSelMuonDxyBSError = Create1DHistogram<TH1D>("hSelMuonSignedDxyBSError", "#sigma(d_{xy}) of selected muons w.r.t. the beam spot", 100, 0., 0.02, "#sigma(d_{xy}) [cm]", nMuLabel);
  TH1D* hSelEffCorMuonDxyBSError = Create1DHistogram<TH1D>("hSelEffCorMuonSignedDxyBSError", "#sigma(d_{xy}) of selected muons w.r.t. the beam spot (eff. corr.)", 100, 0., 0.02, "#sigma(d_{xy}) [cm]", nMuLabel);
  
  TH2D* hSelMuonSignedDxyBSVsPhi = Create2DHistogram<TH2D>("hSelMuonSignedDxyBSVsPhi", "Signed d_{xy} of selected muons w.r.t. the beam spot vs. #varphi", 100, -constants::PI, constants::PI, 2000, -0.5, 0.5, "#varphi_{#mu}", "d_{xy} [cm]");
  TH2D* hSelEffCorMuonSignedDxyBSVsPhi = Create2DHistogram<TH2D>("hSelEffCorMuonSignedDxyBSVsPhi", "Signed d_{xy} of selected muons w.r.t. the beam spot vs. #varphi (eff. corr.)", 100, -constants::PI, constants::PI, 2000, -0.5, 0.5, "#varphi_{#mu}", "d_{xy} [cm]");
  
  TH2D* hSelMuonDxyBSErrorVsPhi = Create2DHistogram<TH2D>("hSelMuonDxyBSErrorVsPhi", "#sigma(d_{xy}) of selected muons w.r.t. the beam spot vs. #varphi", 100, -constants::PI, constants::PI, 100, 0., 0.02, "#varphi_{#mu}", "#sigma(d_{xy}) [cm]");
  TH2D* hSelEffCorMuonDxyBSErrorVsPhi = Create2DHistogram<TH2D>("hSelEffCorMuonDxyBSErrorVsPhi", "#sigma(d_{xy}) of selected muons w.r.t. the beam spot vs. #varphi (eff. corr.)", 100, -constants::PI, constants::PI, 100, 0., 0.02, "#varphi_{#mu}", "#sigma(d_{xy}) [cm]");
  
  TH2D* hSelMuonDxyVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDxyVsRunNumber", "d_{xy} of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "d_{xy} [cm]");
  TH2D* hSelEffCorMuonDxyVsRunNumber = Create2DHistogram<TH2D>("hSelEffCorMuonDxyVsRunNumber", "d_{xy} of selected muons w.r.t. the selected vertex vs. run number (eff. corr.)", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "d_{xy} [cm]");
  
  TH2D* hSelMuonDxyErrorVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDxyErrorVsRunNumber", "#sigma(d_{xy}) of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 100, 0., 0.02, "Run", "#sigma(d_{xy}) [cm]");
  TH2D* hSelEffCorMuonDxyErrorVsRunNumber = Create2DHistogram<TH2D>("hSelEffCorMuonDxyErrorVsRunNumber", "#sigma(d_{xy}) of selected muons w.r.t. the selected vertex vs. run number (eff. corr.)", 260, 147140.5, 149740.5, 100, 0., 0.02, "Run", "#sigma(d_{xy}) [cm]");
  
  TH2D* hSelMuonDxyzVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDxyzVsRunNumber", "d_{xyz} of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "d_{xyz} [cm]");
  TH2D* hSelEffCorMuonDxyzVsRunNumber = Create2DHistogram<TH2D>("hSelEffCorMuonDxyzVsRunNumber", "d_{xyz} of selected muons w.r.t. the selected vertex vs. run number (eff. corr.)", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "d_{xyz} [cm]");
  
  TH2D* hSelMuonDxyzErrorVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDxyzErrorVsRunNumber", "#sigma(d_{xyz}) of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 100, 0., 0.02, "Run", "#sigma(d_{xyz}) [cm]");
  TH2D* hSelEffCorMuonDxyzErrorVsRunNumber = Create2DHistogram<TH2D>("hSelEffCorMuonDxyzErrorVsRunNumber", "#sigma(d_{xyz}) of selected muons w.r.t. the selected vertex vs. run number (eff.corr.)", 260, 147140.5, 149740.5, 100, 0., 0.02, "Run", "#sigma(d_{xyz}) [cm]");
  
  TH2D* hSelMuonDzVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDzVsRunNumber", "d_{z} of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 1000, -0.5, 0.5, "Run", "d_{z} [cm]");
  TH2D* hSelEffCorMuonDzVsRunNumber = Create2DHistogram<TH2D>("hSelEffCorMuonDzVsRunNumber", "d_{z} of selected muons w.r.t. the selected vertex vs. run number (eff. corr.)", 260, 147140.5, 149740.5, 1000, -0.5, 0.5, "Run", "d_{z} [cm]");
  
  TH2D* hSelMuonAbsDzVsRunNumber = Create2DHistogram<TH2D>("hSelMuonAbsDzVsRunNumber", "abs(d_{z}) of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "abs(d_{z}) [cm]");
  TH2D* hSelEffCorMuonAbsDzVsRunNumber = Create2DHistogram<TH2D>("hSelEffCorMuonAbsDzVsRunNumber", "abs(d_{z}) of selected muons w.r.t. the selected vertex vs. run number (eff. corr.)", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "abs(d_{z}) [cm]");
  
  TH2D* hSelMuonDzErrorVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDzErrorVsRunNumber", "#sigma(d_{z}) of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 100, 0., 0.01, "Run", "#sigma(d_{z}) [cm]");
  TH2D* hSelEffCorMuonDzErrorVsRunNumber = Create2DHistogram<TH2D>("hSelEffCorMuonDzErrorVsRunNumber", "#sigma(d_{z}) of selected muons w.r.t. the selected vertex vs. run number (eff. corr.)", 260, 147140.5, 149740.5, 100, 0., 0.01, "Run", "#sigma(d_{z}) [cm]");
  
  TH1D* hRecoDiMuonInvMass = Create1DHistogram<TH1D>("hRecoDiMuonInvMass", "Invariant mass of the reconstructed di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelDiMuonInvMass = Create1DHistogram<TH1D>("hSelDiMuonInvMass", "Invariant mass of the selected di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelEffCorDiMuonInvMass = Create1DHistogram<TH1D>("hSelEffCorDiMuonInvMass", "Invariant mass of the selected di-muons (eff. corr.)", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hAllSelDiMuonInvMass = Create1DHistogram<TH1D>("hAllSelDiMuonInvMass", "Invariant mass of all selected di-muon pairs", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");

  TH1D* hRecoDiMuonDeltaR = Create1DHistogram<TH1D>("hRecoDiMuonDeltaR", "#Delta R between the reconstructed di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelDiMuonDeltaR = Create1DHistogram<TH1D>("hSelDiMuonDeltaR", "#Delta R between the selected di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelEffCorDiMuonDeltaR = Create1DHistogram<TH1D>("hSelEffCorDiMuonDeltaR", "#Delta R between the selected di-muons (eff. corr.)", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hAllSelDiMuonDeltaR = Create1DHistogram<TH1D>("hAllSelDiMuonDeltaR", "#Delta R between all the selected di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");

  TH1D* hRecoDiMuonDeltaEta = Create1DHistogram<TH1D>("hRecoDiMuonDeltaEta", "#Delta #eta between the reconstructed di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelDiMuonDeltaEta", "#Delta #eta between the selected di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelEffCorDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelEffCorDiMuonDeltaEta", "#Delta #eta between the selected di-muons (eff. corr.)", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hAllSelDiMuonDeltaEta = Create1DHistogram<TH1D>("hAllSelDiMuonDeltaEta", "#Delta #eta between all the selected di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  
  TH1D* hRecoDiMuonDeltaPhi = Create1DHistogram<TH1D>("hRecoDiMuonDeltaPhi", "#Delta #phi between the reconstructed di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelDiMuonDeltaPhi", "#Delta #phi between the selected di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelEffCorDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelEffCorDiMuonDeltaPhi", "#Delta #phi between the selected di-muons (eff. corr.)", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hAllSelDiMuonDeltaPhi = Create1DHistogram<TH1D>("hAllSelDiMuonDeltaPhi", "#Delta #phi between all the selected di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  
  TH2D* hRecoDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hRecoDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the reconstructed di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelEffCorDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelEffCorDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected di-muons (eff. corr.)", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hAllSelDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hAllSelDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between all the selected di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");

  TH2D* hRecoDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hRecoDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the reconstructed di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelEffCorDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelEffCorDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected di-muons (eff. corr.)", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hAllSelDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hAllSelDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between all the selected di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  
  TH1D* hRecoDSDiMuonInvMass = Create1DHistogram<TH1D>("hRecoDSDiMuonInvMass", "Invariant mass of the reconstructed DS di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelDSDiMuonInvMass = Create1DHistogram<TH1D>("hSelDSDiMuonInvMass", "Invariant mass of the selected DS di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelEffCorDSDiMuonInvMass = Create1DHistogram<TH1D>("hSelEffCorDSDiMuonInvMass", "Invariant mass of the selected DS di-muons (eff. corr.)", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hAllSelDSDiMuonInvMass = Create1DHistogram<TH1D>("hAllSelDSDiMuonInvMass", "Invariant mass of all selected DS di-muon pairs", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  
  TH1D* hRecoDSDiMuonDeltaR = Create1DHistogram<TH1D>("hRecoDSDiMuonDeltaR", "#Delta R between the reconstructed DS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelDSDiMuonDeltaR = Create1DHistogram<TH1D>("hSelDSDiMuonDeltaR", "#Delta R between the selected DS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelEffCorDSDiMuonDeltaR = Create1DHistogram<TH1D>("hSelEffCorDSDiMuonDeltaR", "#Delta R between the selected DS di-muons (eff. corr.)", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hAllSelDSDiMuonDeltaR = Create1DHistogram<TH1D>("hAllSelDSDiMuonDeltaR", "#Delta R between all the selected DS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  
  TH1D* hRecoDSDiMuonDeltaEta = Create1DHistogram<TH1D>("hRecoDSDiMuonDeltaEta", "#Delta #eta between the reconstructed DS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelDSDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelDSDiMuonDeltaEta", "#Delta #eta between the selected DS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelEffCorDSDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelEffCorDSDiMuonDeltaEta", "#Delta #eta between the selected DS di-muons (eff. corr.)", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hAllSelDSDiMuonDeltaEta = Create1DHistogram<TH1D>("hAllSelDSDiMuonDeltaEta", "#Delta #eta between all the selected DS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  
  TH1D* hRecoDSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hRecoDSDiMuonDeltaPhi", "#Delta #phi between the reconstructed DS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelDSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelDSDiMuonDeltaPhi", "#Delta #phi between the selected DS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelEffCorDSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelEffCorDSDiMuonDeltaPhi", "#Delta #phi between the selected DS di-muons (eff. corr.)", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hAllSelDSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hAllSelDSDiMuonDeltaPhi", "#Delta #phi between all the selected DS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  
  TH2D* hRecoDSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hRecoDSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the reconstructed DS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelDSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelDSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected DS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelEffCorDSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelEffCorDSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected di-muons (eff. corr.)", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hAllSelDSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hAllSelDSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between all the selected DS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  
  TH2D* hRecoDSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hRecoDSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the reconstructed DS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelDSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelDSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected DS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelEffCorDSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelEffCorDSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected DS di-muons (eff. corr.)", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hAllSelDSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hAllSelDSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between all the selected DS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  
  TH1D* hRecoSSDiMuonInvMass = Create1DHistogram<TH1D>("hRecoSSDiMuonInvMass", "Invariant mass of the reconstructed SS di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelSSDiMuonInvMass = Create1DHistogram<TH1D>("hSelSSDiMuonInvMass", "Invariant mass of the selected SS di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelEffCorSSDiMuonInvMass = Create1DHistogram<TH1D>("hSelEffCorSSDiMuonInvMass", "Invariant mass of the selected SS di-muons (eff. corr.)", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hAllSelSSDiMuonInvMass = Create1DHistogram<TH1D>("hAllSelSSDiMuonInvMass", "Invariant mass of all selected SS di-muon pairs", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  
  TH1D* hRecoSSDiMuonDeltaR = Create1DHistogram<TH1D>("hRecoSSDiMuonDeltaR", "#Delta R between the reconstructed SS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelSSDiMuonDeltaR = Create1DHistogram<TH1D>("hSelSSDiMuonDeltaR", "#Delta R between the selected SS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelEffCorSSDiMuonDeltaR = Create1DHistogram<TH1D>("hSelEffCorSSDiMuonDeltaR", "#Delta R between the selected SS di-muons (eff. corr.)", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hAllSelSSDiMuonDeltaR = Create1DHistogram<TH1D>("hAllSelSSDiMuonDeltaR", "#Delta R between all the selected SS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  
  TH1D* hRecoSSDiMuonDeltaEta = Create1DHistogram<TH1D>("hRecoSSDiMuonDeltaEta", "#Delta #eta between the reconstructed SS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelSSDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelSSDiMuonDeltaEta", "#Delta #eta between the selected SS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelEffCorSSDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelEffCorSSDiMuonDeltaEta", "#Delta #eta between the selected SS di-muons (eff. corr.)", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hAllSelSSDiMuonDeltaEta = Create1DHistogram<TH1D>("hAllSelSSDiMuonDeltaEta", "#Delta #eta between all the selected SS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  
  TH1D* hRecoSSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hRecoSSDiMuonDeltaPhi", "#Delta #phi between the reconstructed SS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelSSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelSSDiMuonDeltaPhi", "#Delta #phi between the selected SS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelEffCorSSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelEffCorSSDiMuonDeltaPhi", "#Delta #phi between the selected SS di-muons (eff. corr.)", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hAllSelSSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hAllSelSSDiMuonDeltaPhi", "#Delta #phi between all the selected SS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  
  TH2D* hRecoSSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hRecoSSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the reconstructed SS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelSSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelSSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected SS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelEffCorSSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelEffCorSSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected di-muons (eff. corr.)", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hAllSelSSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hAllSelSSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between all the selected SS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");

  TH2D* hRecoSSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hRecoSSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the reconstructed SS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelSSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelSSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected SS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelEffCorSSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelEffCorSSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected SS di-muons (eff. corr.)", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hAllSelSSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hAllSelSSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between all the selected SS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  
  TH1D* hSelMuonsPhiForHighDz = Create1DHistogram<TH1D>("hSelMuonsPhiForHighDz", "#phi for selected muons having both dz>0.2 cm",100, -0.5*constants::PI,1.5*constants::PI, "#phi", "N_{#mu}");
  TH1D* hSelDSMuonsPhiForHighDz = Create1DHistogram<TH1D>("hSelDSMuonsPhiForHighDz", "#phi for selected DS muons having both dz>0.2 cm",100, -0.5*constants::PI,1.5*constants::PI, "#phi", "N_{#mu}");
  TH1D* hSelSSMuonsPhiForHighDz = Create1DHistogram<TH1D>("hSelSSMuonsPhiForHighDz", "#phi for selected SS muons having both dz>0.2 cm",100, -0.5*constants::PI,1.5*constants::PI, "#phi", "N_{#mu}");
  
  TH1D* hSelMuonsEtaForHighDz =  Create1DHistogram<TH1D>("hSelMuonsEtaForHighDz", "#eta for selected muons having both dz>0.2 cm",250, -2.5,2.5, "#eta", "N_{#mu}");
  TH1D* hSelDSMuonsEtaForHighDz = Create1DHistogram<TH1D>("hSelDSMuonsEtaForHighDz", "#eta for selected DS muons having both dz>0.2 cm",250, -2.5,2.5, "#eta", "N_{#mu}");
  TH1D* hSelSSMuonsEtaForHighDz = Create1DHistogram<TH1D>("hSelSSMuonsEtaForHighDz", "#eta for selected SS muons having both dz>0.2 cm",250, -2.5,2.5, "#eta", "N_{#mu}");
  
  TH1D* hSelMuonsDzForHighDz = Create1DHistogram<TH1D>("hSelMuonsDzForHighDz", "dz for selected muons having both dz>0.2 cm",3000, -1.5,1.5, "d_{z} [cm]", "N_{#mu}");
  TH1D* hSelMuonsDzBSForHighDz = Create1DHistogram<TH1D>("hSelMuonsDzBSForHighDz", "dz wrt BS for selected muons having both dz>0.2 cm",1000, -50.,50., "d_{z} [cm]", "N_{#mu}");
  TH1D* hSelMuonsDxyForHighDz = Create1DHistogram<TH1D>("hSelMuonsDxyForHighDz", "dxy for selected muons having both dz>0.2 cm",500, 0.,0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hSelMuonsDxyzForHighDz = Create1DHistogram<TH1D>("hSelMuonsDxyzForHighDz", "dxyz for selected muons having both dz>0.2 cm",500, 0.,0.5, "d_{xyz} [cm]", "N_{#mu}");
  TH2D* hSelMuonsDz1VsDz2ForHighDz = Create2DHistogram<TH2D>("hSelMuonsDz1VsDz2ForHighDz", "dz1 vs. dz2 for selected muons having both dz>0.2 cm",3000,-1.5,1.5,3000,-1.5,1.5,"d_{z} [cm]", "d_{z} [cm]");
  TH2D* hSelMuonsDxy1VsDxy2ForHighDz = Create2DHistogram<TH2D>("hSelMuonsDxy1VsDxy2ForHighDz", "dxy1 vs. dxy2 for selected muons having both dz>0.2 cm",500,0.,0.5,500,0.,0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hSelMuonsDxyz1VsDxyz2ForHighDz = Create2DHistogram<TH2D>("hSelMuonsDxyz1VsDxyz2ForHighDz", "dxyz1 vs. dxyz2 for selected muons having both dz>0.2 cm",500,0.,0.5,500,0.,0.5,"d_{xyz} [cm]", "d_{xyz} [cm]");
  
  TH1D* hSelDSMuonsDzForHighDz = Create1DHistogram<TH1D>("hSelDSMuonsDzForHighDz", "dz for selected DS muons having both dz>0.2 cm",3000, -1.5,1.5, "d_{z} [cm]", "N_{#mu}");
  TH1D* hSelDSMuonsDzBSForHighDz = Create1DHistogram<TH1D>("hSelDSMuonsDzBSForHighDz", "dz wrt BS for selected DS muons having both dz>0.2 cm",1000, -50.,50., "d_{z} [cm]", "N_{#mu}");
  TH1D* hSelDSMuonsDxyForHighDz = Create1DHistogram<TH1D>("hSelDSMuonsDxyForHighDz", "dxy for selected DS muons having both dz>0.2 cm",500, 0.,0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hSelDSMuonsDxyzForHighDz = Create1DHistogram<TH1D>("hSelDSMuonsDxyzForHighDz", "dxyz for selected DS muons having both dz>0.2 cm",500, 0.,0.5, "d_{xyz} [cm]", "N_{#mu}");
  TH2D* hSelDSMuonsDz1VsDz2ForHighDz = Create2DHistogram<TH2D>("hSelDSMuonsDz1VsDz2ForHighDz", "dz1 vs. dz2 for selected DS muons having both dz>0.2 cm",3000,-1.5,1.5,3000,-1.5,1.5,"d_{z} [cm]", "d_{z} [cm]");
  TH2D* hSelDSMuonsDxy1VsDxy2ForHighDz = Create2DHistogram<TH2D>("hSelDSMuonsDxy1VsDxy2ForHighDz", "dxy1 vs. dxy2 for selected DS muons having both dz>0.2 cm",500,0.,0.5,500,0.,0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hSelDSMuonsDxyz1VsDxyz2ForHighDz = Create2DHistogram<TH2D>("hSelDSMuonsDxyz1VsDxyz2ForHighDz", "dxyz1 vs. dxyz2 for selected DS muons having both dz>0.2 cm",500,0.,0.5,500,0.,0.5,"d_{xyz} [cm]", "d_{xyz} [cm]");
  
  TH1D* hSelSSMuonsDzForHighDz = Create1DHistogram<TH1D>("hSelSSMuonsDzForHighDz", "dz for selected SS muons having both dz>0.2 cm",3000, -1.5,1.5, "d_{z} [cm]", "N_{#mu}");
  TH1D* hSelSSMuonsDzBSForHighDz = Create1DHistogram<TH1D>("hSelSSMuonsDzBSForHighDz", "dz wrt BS for selected SS muons having both dz>0.2 cm",1000, -50.,50., "d_{z} [cm]", "N_{#mu}");
  TH1D* hSelSSMuonsDxyForHighDz = Create1DHistogram<TH1D>("hSelSSMuonsDxyForHighDz", "dxy for selected SS muons having both dz>0.2 cm",500, 0.,0.5, "d_{xy} [cm]", "N_{#mu}");
  TH1D* hSelSSMuonsDxyzForHighDz = Create1DHistogram<TH1D>("hSelSSMuonsDxyzForHighDz", "dxyz for selected SS muons having both dz>0.2 cm",500, 0.,0.5, "d_{xyz} [cm]", "N_{#mu}");
  TH2D* hSelSSMuonsDz1VsDz2ForHighDz = Create2DHistogram<TH2D>("hSelSSMuonsDz1VsDz2ForHighDz", "dz1 vs. dz2 for selected SS muons having both dz>0.2 cm",3000,-1.5,1.5,3000,-1.5,1.5,"d_{z} [cm]", "d_{z} [cm]");
  TH2D* hSelSSMuonsDxy1VsDxy2ForHighDz = Create2DHistogram<TH2D>("hSelSSMuonsDxy1VsDxy2ForHighDz", "dxy1 vs. dxy2 for selected SS muons having both dz>0.2 cm",500,0.,0.5,500,0.,0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hSelSSMuonsDxyz1VsDxyz2ForHighDz = Create2DHistogram<TH2D>("hSelSSMuonsDxyz1VsDxyz2ForHighDz", "dxyz1 vs. dxyz2 for selected SS muons having both dz>0.2 cm",500,0.,0.5,500,0.,0.5,"d_{xyz} [cm]", "d_{xyz} [cm]");
  
  TH1D* hNRecoMuons =  Create1DHistogram<TH1D>("hNRecoMuons", "Number of reconstructed muons", 21,-0.5,20.5,nMuLabel,nEvLabel);
  TH1D* hNSelMuons =  Create1DHistogram<TH1D>("hNSelMuons", "Number of selected muons", 21,-0.5,20.5,nMuLabel,nEvLabel);
  TH1D* hNAllSelMuons =  Create1DHistogram<TH1D>("hNAllSelMuons", "Number of all selected muons", 21,-0.5,20.5,nMuLabel,nEvLabel);
  TH1D* hNTriggeringRecoMuons =  Create1DHistogram<TH1D>("hNTriggeringRecoMuons", "Number of triggering reconstructed muons", 11,-0.5,10.5,nMuLabel,nEvLabel);
  TH1D* hNTriggeringSelMuons =  Create1DHistogram<TH1D>("hNTriggeringSelMuons", "Number of triggering selected muons", 11,-0.5,10.5,nMuLabel,nEvLabel);
  TH1D* hNTriggeringAllSelMuons =  Create1DHistogram<TH1D>("hNTriggeringAllSelMuons", "Number of all triggering selected muons", 11,-0.5,10.5,nMuLabel,nEvLabel);
  TH2D* hNRecoMuonsVsRunNumber =  Create2DHistogram<TH2D>("hNRecoMuonsVsRunNumber", "Number of reconstructed muons vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nMuLabel);
  TH2D* hNSelMuonsVsRunNumber =  Create2DHistogram<TH2D>("hNSelMuonsVsRunNumber", "Number of selected muons vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nMuLabel);
  TH2D* hNAllSelMuonsVsRunNumber =  Create2DHistogram<TH2D>("hNAllSelMuonsVsRunNumber", "Number of all selected muons vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nMuLabel);
  
  TH1D* hRecoVerticesX = Create1DHistogram<TH1D>("hRecoVerticesX", "X position of reconstructed vertices", 100,-2.,2.,"x_{PV} [cm]", nVtxLabel);
  TH1D* hSelVerticesX = Create1DHistogram<TH1D>("hSelVerticesX", "X position of selected vertices", 100,-2.,2.,"x_{PV} [cm]", nVtxLabel);
  TH1D* hAllSelVerticesX = Create1DHistogram<TH1D>("hAllSelVerticesX", "X position of all selected vertices", 100,-2.,2.,"x_{PV} [cm]", nVtxLabel);
  TH1D* hRecoVerticesXError = Create1DHistogram<TH1D>("hRecoVerticesXError", "X position error of reconstructed vertices", 1000,0.,2.,"#sigma(x_{PV}) [cm]", nVtxLabel);
  TH1D* hSelVerticesXError = Create1DHistogram<TH1D>("hSelVerticesXError", "X position error of selected vertices", 50,0.,0.05,"#sigma(x_{PV}) [cm]", nVtxLabel);
  TH1D* hAllSelVerticesXError = Create1DHistogram<TH1D>("hAllSelVerticesXError", "X position error of all selected vertices", 50,0.,0.05,"#sigma(x_{PV}) [cm]", nVtxLabel);
  TH2D* hRecoVerticesXErrorVsRunNumber = Create2DHistogram<TH2D>("hRecoVerticesXErrorVsRunNumber", "X position error of reconstructed vertices vs. run number", 260, 147140.5, 149740.5, 1000,0.,2., "Run","#sigma(x_{PV}) [cm]");
  TH2D* hSelVerticesXErrorVsRunNumber = Create2DHistogram<TH2D>("hSelVerticesXErrorVsRunNumber", "X position error of selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(x_{PV}) [cm]");
  TH2D* hAllSelVerticesXErrorVsRunNumber = Create2DHistogram<TH2D>("hAllSelVerticesXErrorVsRunNumber", "X position error of all selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(x_{PV}) [cm]");
  
  TH1D* hRecoVerticesY = Create1DHistogram<TH1D>("hRecoVerticesY", "Y position of reconstructed vertices", 100,-2.,2.,"y_{PV} [cm]", nVtxLabel);
  TH1D* hSelVerticesY = Create1DHistogram<TH1D>("hSelVerticesY", "Y position of selected vertices", 100,-2.,2.,"y_{PV} [cm]", nVtxLabel);
  TH1D* hAllSelVerticesY = Create1DHistogram<TH1D>("hAllSelVerticesY", "Y position of all selected vertices", 100,-2.,2.,"y_{PV} [cm]", nVtxLabel);
  TH1D* hRecoVerticesYError = Create1DHistogram<TH1D>("hRecoVerticesYError", "Y position error of reconstructed vertices", 1000,0.,2.,"#sigma(y_{PV}) [cm]", nVtxLabel);
  TH1D* hSelVerticesYError = Create1DHistogram<TH1D>("hSelVerticesYError", "Y position error of selected vertices", 50,0.,0.05,"#sigma(y_{PV}) [cm]", nVtxLabel);
  TH1D* hAllSelVerticesYError = Create1DHistogram<TH1D>("hAllSelVerticesYError", "Y position error of all selected vertices", 50,0.,0.05,"#sigma(y_{PV}) [cm]", nVtxLabel);
  TH2D* hRecoVerticesYErrorVsRunNumber = Create2DHistogram<TH2D>("hRecoVerticesYErrorVsRunNumber", "Y position error of reconstructed vertices vs. run number", 260, 147140.5, 149740.5, 1000,0.,2., "Run","#sigma(y_{PV}) [cm]");
  TH2D* hSelVerticesYErrorVsRunNumber = Create2DHistogram<TH2D>("hSelVerticesYErrorVsRunNumber", "Y position error of selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(y_{PV}) [cm]");
  TH2D* hAllSelVerticesYErrorVsRunNumber = Create2DHistogram<TH2D>("hAllSelVerticesYErrorVsRunNumber", "Y position error of all selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(y_{PV}) [cm]");
  TH1D* hRecoVerticesZ = Create1DHistogram<TH1D>("hRecoVerticesZ", "Z position of reconstructed vertices", 500,-25.,25.,"z_{PV} [cm]", nVtxLabel);
  TH1D* hSelVerticesZ = Create1DHistogram<TH1D>("hSelVerticesZ", "Z position of selected vertices", 500,-25.,25.,"z_{PV} [cm]", nVtxLabel);
  TH1D* hAllSelVerticesZ = Create1DHistogram<TH1D>("hAllSelVerticesZ", "Z position of all selected vertices", 500,-25.,25.,"z_{PV} [cm]", nVtxLabel);
  TH1D* hRecoVerticesZError = Create1DHistogram<TH1D>("hRecoVerticesZError", "Z position error of reconstructed vertices", 1000,0.,2.,"#sigma(z_{PV}) [cm]", nVtxLabel);
  TH1D* hSelVerticesZError = Create1DHistogram<TH1D>("hSelVerticesZError", "Z position error of selected vertices", 50,0.,0.05,"#sigma(z_{PV}) [cm]", nVtxLabel);
  TH1D* hAllSelVerticesZError = Create1DHistogram<TH1D>("hAllSelVerticesZError", "Z position error of all selected vertices", 50,0.,0.05,"#sigma(z_{PV}) [cm]", nVtxLabel);
  TH2D* hRecoVerticesZErrorVsRunNumber = Create2DHistogram<TH2D>("hRecoVerticesZErrorVsRunNumber", "Z position error of reconstructed vertices vs. run number", 260, 147140.5, 149740.5, 1000,0.,2., "Run","#sigma(z_{PV}) [cm]");
  TH2D* hSelVerticesZErrorVsRunNumber = Create2DHistogram<TH2D>("hSelVerticesZErrorVsRunNumber", "Z position error of selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(z_{PV}) [cm]");
  TH2D* hAllSelVerticesZErrorVsRunNumber = Create2DHistogram<TH2D>("hAllSelVerticesZErrorVsRunNumber", "Z position error of all selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(z_{PV}) [cm]");
  TH1D* hRecoVerticesNDOF = Create1DHistogram<TH1D>("hRecoVerticesNDOF", "Number of degrees of freedom of reconstructed vertices", 201,-0.5,200.5,"NDOF_{PV}", nVtxLabel);
  TH1D* hSelVerticesNDOF = Create1DHistogram<TH1D>("hSelVerticesNDOF", "Number of degrees of freedom of selected vertices", 201,-0.5,200.5,"NDOF_{PV}", nVtxLabel);
  TH1D* hAllSelVerticesNDOF = Create1DHistogram<TH1D>("hAllSelVerticesNDOF", "Number of degrees of freedom of all selected vertices", 201,-0.5,200.5,"NDOF_{PV}", nVtxLabel);
  TH1D* hRecoVerticesNormalizedChi2 = Create1DHistogram<TH1D>("hRecoVerticesNormalizedChi2", "Normalized #chi^{2} of reconstructed vertices", 100,0.,10.,"#chi^{2}/NDOF", nVtxLabel);
  TH1D* hSelVerticesNormalizedChi2 = Create1DHistogram<TH1D>("hSelVerticesNormalizedChi2", "Normalized #chi^{2} of selected vertices", 100,0.,10.,"#chi^{2}/NDOF", nVtxLabel);
  TH1D* hAllSelVerticesNormalizedChi2 = Create1DHistogram<TH1D>("hAllSelVerticesNormalizedChi2", "Normalized #chi^{2} of all selected vertices", 100,0.,10.,"#chi^{2}/NDOF", nVtxLabel);
  TH1D* hRecoVerticesTrkMultiplicity = Create1DHistogram<TH1D>("hRecoVerticesTrkMultiplicity", "Track multiplicity of reconstructed vertices", 151,-0.5,150.5,"N_{tracks}", nVtxLabel);
  TH1D* hSelVerticesTrkMultiplicity = Create1DHistogram<TH1D>("hSelVerticesTrkMultiplicity", "Track multiplicity of selected vertices", 151,-0.5,150.5,"N_{tracks}", nVtxLabel);
  TH1D* hAllSelVerticesTrkMultiplicity = Create1DHistogram<TH1D>("hAllSelVerticesTrkMultiplicity", "Track multiplicity of all selected vertices", 151,-0.5,150.5,"N_{tracks}", nVtxLabel);
  
  TH1D* hNRecoVertices = Create1DHistogram<TH1D>("hNRecoVertices", "Number of reconstructed vertices", 21,-0.5,20.5,nVtxLabel,nEvLabel);
  TH1D* hNSelVertices = Create1DHistogram<TH1D>("hNSelVertices", "Number of selected vertices", 21,-0.5,20.5,nVtxLabel,nEvLabel);
  TH1D* hNAllSelVertices = Create1DHistogram<TH1D>("hNAllSelVertices", "Number of all selected vertices", 21,-0.5,20.5,nVtxLabel,nEvLabel);
  TH2D* hNRecoVerticesVsRunNumber = Create2DHistogram<TH2D>("hNRecoVerticesVsRunNumber", "Number of reconstructed vertices vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nVtxLabel);
  TH2D* hNSelVerticesVsRunNumber = Create2DHistogram<TH2D>("hNSelVerticesVsRunNumber", "Number of selected vertices vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nVtxLabel);
  TH2D* hNAllSelVerticesVsRunNumber = Create2DHistogram<TH2D>("hNAllSelVerticesVsRunNumber", "Number of all selected vertices vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nVtxLabel);
  
  TLorentzVector* piMuon = new TLorentzVector();
  TLorentzVector* pjMuon = new TLorentzVector();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

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
    if(hasGenp)
    {
      if(!s->SelectEventPassingTrigger("HLT_DoubleMu3"))
      {
        continue;
      }
    }
    else
    {
      if(!s->SelectEventPassingTrigger("HLT_DoubleMu3_v2"))
      {
        continue;
      }
    }
    
    unsigned int nRecoMuons = 0;
    unsigned int nSelMuons = 0;
    unsigned int nAllSelMuons = 0;
    unsigned int nTriggeringRecoMuons = 0;
    unsigned int nTriggeringSelMuons = 0;
    unsigned int nTriggeringAllSelMuons = 0;
    
    unsigned int nRecoVertices = 0;
    unsigned int nSelVertices = 0;
    unsigned int nAllSelVertices = 0;

    unsigned int nRecoJets = 0;
    unsigned int nSelJets = 0;
    unsigned int nAllSelJets = 0;
        
    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      nRecoMuons++;
      hRecoMuonPt->Fill(Muon_pt->at(iMuon));
      hRecoMuonEta->Fill(Muon_eta->at(iMuon));
      hRecoMuonPhi->Fill(Muon_phi->at(iMuon));
      hRecoMuonCharge->Fill(Muon_charge->at(iMuon));
      if(Muon_hasTriggered->at(iMuon))
      {
        nTriggeringRecoMuons++;
        hRecoMuonHasTriggered->Fill(1.);
      }
      else
      {
        hRecoMuonHasTriggered->Fill(0.);
      }
      
      hRecoMuonCaloCompatibility->Fill(Muon_caloCompatibility->at(iMuon));
      hRecoMuonSegmentCompatibility->Fill(Muon_segmentCompatibility->at(iMuon));
      unsigned int iTrack = Muon_trackref->at(iMuon);
      if(iTrack != 65535)
      {
        hRecoMuonTrackValidHits->Fill(Track_numberOfValidHits->at(iTrack));
        hRecoMuonTrackChi2->Fill(Track_normalizedChi2->at(iTrack));
        hRecoMuonTrackNdof->Fill(Track_ndof->at(iTrack));
        hRecoMuonTrackValidPixelHits->Fill(Track_validPixelHits->at(iTrack));
      }
      else
      {
        hRecoMuonTrackValidHits->Fill(0);
        hRecoMuonTrackChi2->Fill(9999);
        hRecoMuonTrackNdof->Fill(0);
        hRecoMuonTrackValidPixelHits->Fill(0);
      }
      hRecoMuonGlobalChi2->Fill(Muon_globalChi2->at(iMuon));
      hRecoMuonGlobalValidMuonHits->Fill(Muon_globalValidMuonHits->at(iMuon));
      hRecoMuonNumberOfMatches->Fill(Muon_numberOfMatches->at(iMuon));
      
      piMuon->SetPtEtaPhiM(Muon_pt->at(iMuon),Muon_eta->at(iMuon),Muon_phi->at(iMuon),constants::muonMass);
      for(int jMuon = iMuon+1; jMuon < numberOfMuon; jMuon++)
      {
        pjMuon->SetPtEtaPhiM(Muon_pt->at(jMuon),Muon_eta->at(jMuon),Muon_phi->at(jMuon),constants::muonMass);
        double mass = (*piMuon + *pjMuon).M();
        double deltaR = piMuon->DeltaR(*pjMuon);
        double deltaEta = piMuon->Eta() - pjMuon->Eta();
        double deltaPhi = piMuon->DeltaPhi(*pjMuon);
        if(deltaPhi<-0.5*constants::PI)
          deltaPhi+=2*constants::PI;
        hRecoDiMuonInvMass->Fill(mass);
        hRecoDiMuonDeltaR->Fill(deltaR);
        hRecoDiMuonDeltaEta->Fill(deltaEta);
        hRecoDiMuonDeltaPhi->Fill(deltaPhi);
        hRecoDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
        hRecoDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
        if(Muon_charge->at(jMuon) != Muon_charge->at(iMuon))
        {
          hRecoDSDiMuonInvMass->Fill(mass);
          hRecoDSDiMuonDeltaR->Fill(deltaR);
          hRecoDSDiMuonDeltaEta->Fill(deltaEta);
          hRecoDSDiMuonDeltaPhi->Fill(deltaPhi);
          hRecoDSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
          hRecoDSDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
        }
        else
        {
          hRecoSSDiMuonInvMass->Fill(mass);
          hRecoSSDiMuonDeltaR->Fill(deltaR);
          hRecoSSDiMuonDeltaEta->Fill(deltaEta);
          hRecoSSDiMuonDeltaPhi->Fill(deltaPhi);
          hRecoSSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
          hRecoSSDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
        }
      }
      
      if(s->SelectMuon(iMuon))
      {
        nAllSelMuons++;
        hAllSelMuonPt->Fill(Muon_pt->at(iMuon));
        hAllSelMuonEta->Fill(Muon_eta->at(iMuon));
        hAllSelMuonPhi->Fill(Muon_phi->at(iMuon));
        hAllSelMuonCharge->Fill(Muon_charge->at(iMuon));
        if(Muon_hasTriggered->at(iMuon))
        {
          nTriggeringAllSelMuons++;
          hAllSelMuonHasTriggered->Fill(1.);
        }
        else
        {
          hAllSelMuonHasTriggered->Fill(0.);
        }
        hAllSelMuonCaloCompatibility->Fill(Muon_caloCompatibility->at(iMuon));
        hAllSelMuonSegmentCompatibility->Fill(Muon_segmentCompatibility->at(iMuon));
        if(iTrack != 65535)
        {
          hAllSelMuonTrackValidHits->Fill(Track_numberOfValidHits->at(iTrack));
          hAllSelMuonTrackChi2->Fill(Track_normalizedChi2->at(iTrack));
          hAllSelMuonTrackNdof->Fill(Track_ndof->at(iTrack));
          hAllSelMuonTrackValidPixelHits->Fill(Track_validPixelHits->at(iTrack));
        }
        else
        {
          hAllSelMuonTrackValidHits->Fill(0);
          hAllSelMuonTrackChi2->Fill(9999);
          hAllSelMuonTrackNdof->Fill(0);
          hAllSelMuonTrackValidPixelHits->Fill(0);
        }
        hAllSelMuonGlobalChi2->Fill(Muon_globalChi2->at(iMuon));
        hAllSelMuonGlobalValidMuonHits->Fill(Muon_globalValidMuonHits->at(iMuon));
        hAllSelMuonNumberOfMatches->Fill(Muon_numberOfMatches->at(iMuon));
        
        for(int jMuon = iMuon+1; jMuon < numberOfMuon; jMuon++)
        {
          if(s->SelectMuon(jMuon))
          {
            pjMuon->SetPtEtaPhiM(Muon_pt->at(jMuon),Muon_eta->at(jMuon),Muon_phi->at(jMuon),constants::muonMass);
            double mass = (*piMuon + *pjMuon).M();
            double deltaR = piMuon->DeltaR(*pjMuon);
            double deltaEta = piMuon->Eta() - pjMuon->Eta();
            double deltaPhi = piMuon->DeltaPhi(*pjMuon);
            if(deltaPhi<-0.5*constants::PI)
              deltaPhi+=2*constants::PI;
            hAllSelDiMuonInvMass->Fill(mass);
            hAllSelDiMuonDeltaR->Fill(deltaR);
            hAllSelDiMuonDeltaEta->Fill(deltaEta);
            hAllSelDiMuonDeltaPhi->Fill(deltaPhi);
            hAllSelDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
            hAllSelDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
            if(Muon_charge->at(jMuon) != Muon_charge->at(iMuon))
            {
              hAllSelDSDiMuonInvMass->Fill(mass);
              hAllSelDSDiMuonDeltaR->Fill(deltaR);
              hAllSelDSDiMuonDeltaEta->Fill(deltaEta);
              hAllSelDSDiMuonDeltaPhi->Fill(deltaPhi);
              hAllSelDSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
              hAllSelDSDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
            }
            else
            {
              hAllSelSSDiMuonInvMass->Fill(mass);
              hAllSelSSDiMuonDeltaR->Fill(deltaR);
              hAllSelSSDiMuonDeltaEta->Fill(deltaEta);
              hAllSelSSDiMuonDeltaPhi->Fill(deltaPhi);
              hAllSelSSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
              hAllSelSSDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
            }
          }
        }
      }
    }
    
    if(s->JetSelectorIsValid())
    {
      for(int iJet = 0; iJet < numberOfJet; iJet++)
      {
        nRecoJets++;
        hRecoJetPt->Fill(Jet_pt->at(iJet));
        hRecoJetEta->Fill(Jet_eta->at(iJet));
        hRecoJetPhi->Fill(Jet_phi->at(iJet));
        hRecoJetEcfr->Fill(Jet_ecfr->at(iJet));
        hRecoJetEnfr->Fill(Jet_enfr->at(iJet));
        hRecoJetHcfr->Fill(Jet_hcfr->at(iJet));
        hRecoJetHnfr->Fill(Jet_hnfr->at(iJet));
        hRecoJetMass->Fill(Jet_mass->at(iJet));
        hRecoJetNConstituents->Fill(Jet_nConstituents->at(iJet));
        hRecoJetChargedMultiplicity->Fill(Jet_chargedMultiplicity->at(iJet));
        hRecoJetTrackMultiplicity->Fill(Jet_vTrackref->at(iJet).size());
        hRecoJetNSecondaryVertex->Fill(Jet_vSecVtxref->at(iJet).size());
        
        if(s->SelectJet(iJet))
        {
          nAllSelJets++;
          hAllSelJetPt->Fill(Jet_pt->at(iJet));
          hAllSelJetEta->Fill(Jet_eta->at(iJet));
          hAllSelJetPhi->Fill(Jet_phi->at(iJet));
          hAllSelJetEcfr->Fill(Jet_ecfr->at(iJet));
          hAllSelJetEnfr->Fill(Jet_enfr->at(iJet));
          hAllSelJetHcfr->Fill(Jet_hcfr->at(iJet));
          hAllSelJetHnfr->Fill(Jet_hnfr->at(iJet));
          hAllSelJetMass->Fill(Jet_mass->at(iJet));
          hAllSelJetNConstituents->Fill(Jet_nConstituents->at(iJet));
          hAllSelJetChargedMultiplicity->Fill(Jet_chargedMultiplicity->at(iJet));
          hAllSelJetTrackMultiplicity->Fill(Jet_vTrackref->at(iJet).size());
          hAllSelJetNSecondaryVertex->Fill(Jet_vSecVtxref->at(iJet).size());
        }
      }
    }
    
    double ptSelMuon0 = Muon_pt->at(vSelectedMuons[0]);
    double ptSelMuon1 = Muon_pt->at(vSelectedMuons[1]);
    double effSelMuon0 = GetGlobalEfficiency(Muon_pt->at(vSelectedMuons[0]), Muon_eta->at(vSelectedMuons[0]), effMethod);
    double effSelMuon1 = GetGlobalEfficiency(Muon_pt->at(vSelectedMuons[1]), Muon_eta->at(vSelectedMuons[1]), effMethod);
    if(effSelMuon0 == 0)
      effSelMuon0 = 1;
    if(effSelMuon1 == 0)
      effSelMuon1 = 1;
    std::vector<double> vEffSelMuon;
    vEffSelMuon.push_back(effSelMuon0);
    vEffSelMuon.push_back(effSelMuon1);
    if(ptSelMuon0>ptSelMuon1)
    {
      hSelMuonLeadingPt->Fill(ptSelMuon0);
      hSelEffCorMuonLeadingPt->Fill(ptSelMuon0, 1./effSelMuon0);
      hSelMuonLeadingEta->Fill(Muon_eta->at(vSelectedMuons[0]));
      hSelEffCorMuonLeadingEta->Fill(Muon_eta->at(vSelectedMuons[0]), 1./effSelMuon0);
      hSelMuonSecondPt->Fill(ptSelMuon1);
      hSelEffCorMuonSecondPt->Fill(ptSelMuon1, 1./effSelMuon1);
      hSelMuonSecondEta->Fill(Muon_eta->at(vSelectedMuons[1]));
      hSelEffCorMuonSecondEta->Fill(Muon_eta->at(vSelectedMuons[1]), 1./effSelMuon1);
    }
    else
    {
      hSelMuonLeadingPt->Fill(ptSelMuon1);
      hSelEffCorMuonLeadingPt->Fill(ptSelMuon1, 1./effSelMuon1);
      hSelMuonLeadingEta->Fill(Muon_eta->at(vSelectedMuons[1]));
      hSelEffCorMuonLeadingEta->Fill(Muon_eta->at(vSelectedMuons[1]), 1./effSelMuon1);
      hSelMuonSecondPt->Fill(ptSelMuon0);
      hSelEffCorMuonSecondPt->Fill(ptSelMuon0, 1./effSelMuon0);
      hSelMuonSecondEta->Fill(Muon_eta->at(vSelectedMuons[0]));
      hSelEffCorMuonSecondEta->Fill(Muon_eta->at(vSelectedMuons[0]), 1./effSelMuon0);
    }
    
    for(unsigned int i = 0; i < vSelectedMuons.size(); i++)
    {
      nSelMuons++;
      unsigned short iMuon = vSelectedMuons[i];
      hSelMuonPt->Fill(Muon_pt->at(iMuon));
      hSelEffCorMuonPt->Fill(Muon_pt->at(iMuon), 1./vEffSelMuon[i]);
      hSelMuonEta->Fill(Muon_eta->at(iMuon));
      hSelEffCorMuonEta->Fill(Muon_eta->at(iMuon), 1./vEffSelMuon[i]);
      hSelMuonPhi->Fill(Muon_phi->at(iMuon));
      hSelEffCorMuonPhi->Fill(Muon_phi->at(iMuon), 1./vEffSelMuon[i]);
      hSelMuonCharge->Fill(Muon_charge->at(iMuon));
      hSelEffCorMuonCharge->Fill(Muon_charge->at(iMuon), 1./vEffSelMuon[i]);
      if(Muon_hasTriggered->at(iMuon))
      {
        nTriggeringSelMuons++;
        hSelMuonHasTriggered->Fill(1.);
      }
      else
      {
        hSelMuonHasTriggered->Fill(0.);
      }
      hSelMuonCaloCompatibility->Fill(Muon_caloCompatibility->at(iMuon));
      hSelEffCorMuonCaloCompatibility->Fill(Muon_caloCompatibility->at(iMuon), 1./vEffSelMuon[i]);
      hSelMuonSegmentCompatibility->Fill(Muon_segmentCompatibility->at(iMuon));
      hSelEffCorMuonSegmentCompatibility->Fill(Muon_segmentCompatibility->at(iMuon), 1./vEffSelMuon[i]);
      unsigned int iTrack = Muon_trackref->at(iMuon);
      if(iTrack != 65535)
      {
        hSelMuonTrackValidHits->Fill(Track_numberOfValidHits->at(iTrack));
        hSelEffCorMuonTrackValidHits->Fill(Track_numberOfValidHits->at(iTrack), 1./vEffSelMuon[i]);
        hSelMuonTrackChi2->Fill(Track_normalizedChi2->at(iTrack));
        hSelEffCorMuonTrackChi2->Fill(Track_normalizedChi2->at(iTrack), 1./vEffSelMuon[i]);
        hSelMuonTrackNdof->Fill(Track_ndof->at(iTrack));
        hSelEffCorMuonTrackNdof->Fill(Track_ndof->at(iTrack), 1./vEffSelMuon[i]);
        hSelMuonTrackValidPixelHits->Fill(Track_validPixelHits->at(iTrack));
        hSelEffCorMuonTrackValidPixelHits->Fill(Track_validPixelHits->at(iTrack), 1./vEffSelMuon[i]);
        
        hSelMuonDxy->Fill(Track_dxy_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDxy->Fill(Track_dxy_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDxyError->Fill(Track_dxyError_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDxyError->Fill(Track_dxyError_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDxyz->Fill(Track_dxyz_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDxyz->Fill(Track_dxyz_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDxyzError->Fill(Track_dxyzError_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDxyzError->Fill(Track_dxyzError_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDz->Fill(Track_dz_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDz->Fill(Track_dz_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonAbsDz->Fill(fabs(Track_dz_PV->at(iTrack).at(iSelectedVertex)));
        hSelEffCorMuonAbsDz->Fill(fabs(Track_dz_PV->at(iTrack).at(iSelectedVertex)), 1./vEffSelMuon[i]);
        hSelMuonDzError->Fill(Track_dzError_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDzError->Fill(Track_dzError_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDxyVsRunNumber->Fill(runNumber, Track_dxy_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDxyVsRunNumber->Fill(runNumber, Track_dxy_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDxyErrorVsRunNumber->Fill(runNumber, Track_dxyError_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDxyErrorVsRunNumber->Fill(runNumber, Track_dxyError_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDxyzVsRunNumber->Fill(runNumber, Track_dxyz_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDxyzVsRunNumber->Fill(runNumber, Track_dxyz_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDxyzErrorVsRunNumber->Fill(runNumber, Track_dxyzError_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDxyzErrorVsRunNumber->Fill(runNumber, Track_dxyzError_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonDzVsRunNumber->Fill(runNumber, Track_dz_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDzVsRunNumber->Fill(runNumber, Track_dz_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        hSelMuonAbsDzVsRunNumber->Fill(runNumber, fabs(Track_dz_PV->at(iTrack).at(iSelectedVertex)));
        hSelEffCorMuonAbsDzVsRunNumber->Fill(runNumber, fabs(Track_dz_PV->at(iTrack).at(iSelectedVertex)), 1./vEffSelMuon[i]);
        hSelMuonDzErrorVsRunNumber->Fill(runNumber, Track_dzError_PV->at(iTrack).at(iSelectedVertex));
        hSelEffCorMuonDzErrorVsRunNumber->Fill(runNumber, Track_dzError_PV->at(iTrack).at(iSelectedVertex), 1./vEffSelMuon[i]);
        
        TVector3 referencePoint;
//         referencePoint.SetXYZ(Track_referencePoint_x->at(iTrack),Track_referencePoint_y->at(iTrack),Track_referencePoint_z->at(iTrack));
        referencePoint.SetXYZ(Track_referencePoint_x->at(iTrack),Track_referencePoint_y->at(iTrack),0.);
        TVector3 beamSpot;
        beamSpot.SetXYZ(beamSpot_x, beamSpot_y, 0.);
        TVector3 relativePosition = referencePoint - beamSpot;
        TVector3 momentum;
        momentum.SetPtEtaPhi(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack));
        momentum.SetZ(0.);
        int sign = (relativePosition * momentum)>0?1:-1;

        hSelMuonSignedDxyBS->Fill(Track_dxy_BS->at(iTrack)*sign);
        hSelEffCorMuonSignedDxyBS->Fill(Track_dxy_BS->at(iTrack)*sign, 1./vEffSelMuon[i]);
        hSelMuonDxyBSError->Fill(Track_dxyError_BS->at(iTrack));
        hSelEffCorMuonDxyBSError->Fill(Track_dxyError_BS->at(iTrack), 1./vEffSelMuon[i]);
        hSelMuonSignedDxyBSVsPhi->Fill(Track_phi->at(iTrack), Track_dxy_BS->at(iTrack)*sign);
        hSelEffCorMuonSignedDxyBSVsPhi->Fill(Track_phi->at(iTrack), Track_dxy_BS->at(iTrack)*sign, 1./vEffSelMuon[i]);
        hSelMuonDxyBSErrorVsPhi->Fill(Track_phi->at(iTrack), Track_dxyError_BS->at(iTrack));
        hSelEffCorMuonDxyBSErrorVsPhi->Fill(Track_phi->at(iTrack), Track_dxyError_BS->at(iTrack), 1./vEffSelMuon[i]);
      }
      else
      {
        hSelMuonTrackValidHits->Fill(0);
        hSelEffCorMuonTrackValidHits->Fill(0);
        hSelMuonTrackChi2->Fill(9999);
        hSelEffCorMuonTrackChi2->Fill(9999);
        hSelMuonTrackNdof->Fill(0);
        hSelEffCorMuonTrackNdof->Fill(0);
        hSelMuonTrackValidPixelHits->Fill(0);
        hSelEffCorMuonTrackValidPixelHits->Fill(0);
        hSelMuonDxy->Fill(9999);
        hSelEffCorMuonDxy->Fill(9999);
        hSelMuonDxyError->Fill(9999);
        hSelEffCorMuonDxyError->Fill(9999);
        hSelMuonDxyz->Fill(9999);
        hSelEffCorMuonDxyz->Fill(9999);
        hSelMuonDxyzError->Fill(9999);
        hSelEffCorMuonDxyzError->Fill(9999);
        hSelMuonDz->Fill(9999);
        hSelEffCorMuonDz->Fill(9999);
        hSelMuonAbsDz->Fill(9999);
        hSelEffCorMuonAbsDz->Fill(9999);
        hSelMuonDzError->Fill(9999);
        hSelEffCorMuonDzError->Fill(9999);
        hSelMuonDxyVsRunNumber->Fill(runNumber, 9999);
        hSelEffCorMuonDxyVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyErrorVsRunNumber->Fill(runNumber, 9999);
        hSelEffCorMuonDxyErrorVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyzVsRunNumber->Fill(runNumber, 9999);
        hSelEffCorMuonDxyzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyzErrorVsRunNumber->Fill(runNumber, 9999);
        hSelEffCorMuonDxyzErrorVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDzVsRunNumber->Fill(runNumber, 9999);
        hSelEffCorMuonDzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonAbsDzVsRunNumber->Fill(runNumber, 9999);
        hSelEffCorMuonAbsDzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDzErrorVsRunNumber->Fill(runNumber, 9999);
        hSelEffCorMuonDzErrorVsRunNumber->Fill(runNumber, 9999);
        
        hSelMuonSignedDxyBS->Fill(9999);
        hSelEffCorMuonSignedDxyBS->Fill(9999);
        hSelMuonDxyBSError->Fill(9999);
        hSelEffCorMuonDxyBSError->Fill(9999);
        hSelMuonSignedDxyBSVsPhi->Fill(9999, 9999);
        hSelEffCorMuonSignedDxyBSVsPhi->Fill(9999, 9999);
        hSelMuonDxyBSErrorVsPhi->Fill(9999, 9999);
        hSelEffCorMuonDxyBSErrorVsPhi->Fill(9999, 9999);
      }
      hSelMuonGlobalChi2->Fill(Muon_globalChi2->at(iMuon));
      hSelEffCorMuonGlobalChi2->Fill(Muon_globalChi2->at(iMuon), 1./vEffSelMuon[i]);
      hSelMuonGlobalValidMuonHits->Fill(Muon_globalValidMuonHits->at(iMuon));
      hSelEffCorMuonGlobalValidMuonHits->Fill(Muon_globalValidMuonHits->at(iMuon), 1./vEffSelMuon[i]);
      hSelMuonNumberOfMatches->Fill(Muon_numberOfMatches->at(iMuon));
      hSelEffCorMuonNumberOfMatches->Fill(Muon_numberOfMatches->at(iMuon), 1./vEffSelMuon[i]);
    }
    
    double ptSelJet0 = Jet_pt->at(vSelectedJets[0]);
    double ptSelJet1 = Jet_pt->at(vSelectedJets[1]);
    if(ptSelJet0>ptSelJet1)
    {
      hSelJetLeadingPt->Fill(ptSelJet0);
      hSelJetLeadingEta->Fill(Jet_eta->at(vSelectedJets[0]));
      hSelJetSecondPt->Fill(ptSelJet1);
      hSelJetSecondEta->Fill(Jet_eta->at(vSelectedJets[1]));
    }
    else
    {
      hSelJetLeadingPt->Fill(ptSelJet1);
      hSelJetLeadingEta->Fill(Jet_eta->at(vSelectedJets[1]));
      hSelJetSecondPt->Fill(ptSelJet0);
      hSelJetSecondEta->Fill(Jet_eta->at(vSelectedJets[0]));
    }
    
    for(unsigned int i = 0; i < vSelectedJets.size(); i++)
    {
      nSelJets++;
      unsigned short iJet = vSelectedJets[i];
      unsigned short nTracks = Jet_vTrackref->at(iJet).size();
      hSelJetPt->Fill(Jet_pt->at(iJet));
      hSelJetEta->Fill(Jet_eta->at(iJet));
      hSelJetPhi->Fill(Jet_phi->at(iJet));
      hSelJetEcfr->Fill(Jet_ecfr->at(iJet));
      hSelJetEnfr->Fill(Jet_enfr->at(iJet));
      hSelJetHcfr->Fill(Jet_hcfr->at(iJet));
      hSelJetHnfr->Fill(Jet_hnfr->at(iJet));
      hSelJetMass->Fill(Jet_mass->at(iJet));
      hSelJetNConstituents->Fill(Jet_nConstituents->at(iJet));
      hSelJetChargedMultiplicity->Fill(Jet_chargedMultiplicity->at(iJet));
      hSelJetTrackMultiplicity->Fill(nTracks);
      hSelJetNSecondaryVertex->Fill(Jet_vSecVtxref->at(iJet).size());
      double sumTrackPt = 0;
      for(unsigned int j = 0; j < nTracks; j++)
      {
        unsigned int iTrack = Jet_vTrackref->at(iJet).at(j);
        sumTrackPt+=Track_pt->at(iTrack);
      }
      double nAssociatedTracks = 0;
      double nAssociatedTracksWeighted = 0;
      double sumDz = 0;
      double weightedSumDz = 0;
      double squaredSumDz = 0;
      double weightedSquaredSumDz = 0;
      for(unsigned int j = 0; j < nTracks; j++)
      {
        unsigned int iTrack = Jet_vTrackref->at(iJet).at(j);
        unsigned short iVtx = s->SelectVertexNearestToTrack(iTrack);
        double trackPt = Track_pt->at(iTrack);
        double trackDz = Track_dz_PV->at(iTrack).at(iSelectedVertex);
        double weight = trackPt/sumTrackPt;
        sumDz+=trackDz;
        weightedSumDz+=trackDz*weight;
        squaredSumDz+=trackDz*trackDz;
        weightedSquaredSumDz+=(trackDz*trackDz*weight);
        if(iVtx == iSelectedVertex)
        {
          nAssociatedTracks++;
          nAssociatedTracksWeighted+=weight;
        }
        hSelJetTrackDzWrtSelectedVertex->Fill(trackDz);
        hSelJetWeightedTrackDzWrtSelectedVertex->Fill(trackDz,weight);
      }
      double fractionAssociatedTracks = (double)nAssociatedTracks/nTracks;
//       std::cout << "fractionAssociatedTracks is " << fractionAssociatedTracks << std::endl;
      double fractionAssociatedTracksWeighted = (double)nAssociatedTracksWeighted;
      double averageDz = sumDz/nTracks;
      double averageDzWeighted = weightedSumDz;
      double rmsDz = sqrt(squaredSumDz/nTracks);
      double rmsDzWeighted = sqrt(weightedSquaredSumDz);
      hSelJetFractionTracksAssociatedToVertex->Fill(fractionAssociatedTracks);
      hSelJetWeightedFractionTracksAssociatedToVertex->Fill(fractionAssociatedTracksWeighted);
      hSelJetAverageTrackDzWrtSelectedVertex->Fill(averageDz);
      hSelJetWeightedAverageTrackDzWrtSelectedVertex->Fill(averageDzWeighted);
      hSelJetRMSTrackDzWrtSelectedVertex->Fill(rmsDz);
      hSelJetWeightedRMSTrackDzWrtSelectedVertex->Fill(rmsDzWeighted);
    }
    
    TLorentzVector* pSelMuon0 = new TLorentzVector();
    pSelMuon0->SetPtEtaPhiM(Muon_pt->at(vSelectedMuons[0]),Muon_eta->at(vSelectedMuons[0]),Muon_phi->at(vSelectedMuons[0]),constants::muonMass);
    TLorentzVector* pSelMuon1 = new TLorentzVector();
    pSelMuon1->SetPtEtaPhiM(Muon_pt->at(vSelectedMuons[1]),Muon_eta->at(vSelectedMuons[1]),Muon_phi->at(vSelectedMuons[1]),constants::muonMass);
    double mass = (*pSelMuon0 + *pSelMuon1).M();
    double deltaR = pSelMuon0->DeltaR(*pSelMuon1);
    double deltaEta = pSelMuon0->Eta() - pSelMuon1->Eta();
    double deltaPhi = pSelMuon0->DeltaPhi(*pSelMuon1);
    if(deltaPhi<-0.5*constants::PI)
      deltaPhi+=2*constants::PI;
    hSelDiMuonInvMass->Fill(mass);
    hSelEffCorDiMuonInvMass->Fill(mass,1./(vEffSelMuon[0]*vEffSelMuon[1]));
    hSelDiMuonDeltaR->Fill(deltaR);
    hSelEffCorDiMuonDeltaR->Fill(deltaR,1./(vEffSelMuon[0]*vEffSelMuon[1]));
    hSelDiMuonDeltaEta->Fill(deltaEta);
    hSelEffCorDiMuonDeltaEta->Fill(deltaEta,1./(vEffSelMuon[0]*vEffSelMuon[1]));
    hSelDiMuonDeltaPhi->Fill(deltaPhi);
    hSelEffCorDiMuonDeltaPhi->Fill(deltaPhi,1./(vEffSelMuon[0]*vEffSelMuon[1]));
    hSelDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
    hSelEffCorDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta, 1./(vEffSelMuon[0]*vEffSelMuon[1]));
    hSelDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
    hSelEffCorDiMuonInvMassVsDeltaR->Fill(deltaR, mass, 1./(vEffSelMuon[0]*vEffSelMuon[1]));
    if(Muon_charge->at(vSelectedMuons[0]) != Muon_charge->at(vSelectedMuons[1]))
    {
      hSelDSDiMuonInvMass->Fill(mass);
      hSelEffCorDSDiMuonInvMass->Fill(mass,1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelDSDiMuonDeltaR->Fill(deltaR);
      hSelEffCorDSDiMuonDeltaR->Fill(deltaR,1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelDSDiMuonDeltaEta->Fill(deltaEta);
      hSelEffCorDSDiMuonDeltaEta->Fill(deltaEta,1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelDSDiMuonDeltaPhi->Fill(deltaPhi);
      hSelEffCorDSDiMuonDeltaPhi->Fill(deltaPhi,1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelDSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
      hSelEffCorDSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta, 1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelDSDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
      hSelEffCorDSDiMuonInvMassVsDeltaR->Fill(deltaR, mass, 1./(vEffSelMuon[0]*vEffSelMuon[1]));
    }
    else
    {
      hSelSSDiMuonInvMass->Fill(mass);
      hSelEffCorSSDiMuonInvMass->Fill(mass,1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelSSDiMuonDeltaR->Fill(deltaR);
      hSelEffCorSSDiMuonDeltaR->Fill(deltaR,1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelSSDiMuonDeltaEta->Fill(deltaEta);
      hSelEffCorSSDiMuonDeltaEta->Fill(deltaEta,1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelSSDiMuonDeltaPhi->Fill(deltaPhi);
      hSelEffCorSSDiMuonDeltaPhi->Fill(deltaPhi,1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelSSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
      hSelEffCorSSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta, 1./(vEffSelMuon[0]*vEffSelMuon[1]));
      hSelSSDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
      hSelEffCorSSDiMuonInvMassVsDeltaR->Fill(deltaR, mass, 1./(vEffSelMuon[0]*vEffSelMuon[1]));
    }
    unsigned int trackMuon0 = Muon_trackref->at(vSelectedMuons[0]);
    unsigned int trackMuon1 = Muon_trackref->at(vSelectedMuons[1]);
    if(trackMuon0 != 65535 && trackMuon1 != 65535)
    {
      double dz0 = Track_dz_PV->at(trackMuon0).at(iSelectedVertex);
      double dz1 = Track_dz_PV->at(trackMuon1).at(iSelectedVertex);
      double dxy0 = Track_dxy_PV->at(trackMuon0).at(iSelectedVertex);
      double dxy1 = Track_dxy_PV->at(trackMuon1).at(iSelectedVertex);
      double dxyz0 = Track_dxyz_PV->at(trackMuon0).at(iSelectedVertex);
      double dxyz1 = Track_dxyz_PV->at(trackMuon1).at(iSelectedVertex);
      double dzBS0 = Track_dz_BS->at(trackMuon0);
      double dzBS1 = Track_dz_BS->at(trackMuon1);
      if((dz0 > 0.2 && dz1 > 0.2 && fabs(dz0-dz1)<0.1) || (dz0 < -0.2 && dz1 < -0.2 && fabs(dz0-dz1)<0.1))
      {
        double phi0 = Muon_phi->at(vSelectedMuons[0]);
        double phi1 = Muon_phi->at(vSelectedMuons[1]);
        double eta0 = Muon_eta->at(vSelectedMuons[0]);
        double eta1 = Muon_eta->at(vSelectedMuons[1]);
        if(phi0<-0.5*constants::PI)
          phi0+=2*constants::PI;
        if(phi1<-0.5*constants::PI)
          phi1+=2*constants::PI;
        hSelMuonsPhiForHighDz->Fill(phi0);
        hSelMuonsPhiForHighDz->Fill(phi1);
        hSelMuonsEtaForHighDz->Fill(phi0);
        hSelMuonsEtaForHighDz->Fill(phi1);
        hSelMuonsDzForHighDz->Fill(dz0);
        hSelMuonsDzForHighDz->Fill(dz1);
        hSelMuonsDxyForHighDz->Fill(dxy0);
        hSelMuonsDxyForHighDz->Fill(dxy1);
        hSelMuonsDxyzForHighDz->Fill(dxyz0);
        hSelMuonsDxyzForHighDz->Fill(dxyz1);
        hSelMuonsDzBSForHighDz->Fill(dzBS0);
        hSelMuonsDzBSForHighDz->Fill(dzBS1);
        hSelMuonsDz1VsDz2ForHighDz->Fill(dz0,dz1);
        hSelMuonsDxy1VsDxy2ForHighDz->Fill(dxy0,dxy1);
        hSelMuonsDxyz1VsDxyz2ForHighDz->Fill(dxyz0,dxyz1);
        if(Muon_charge->at(vSelectedMuons[0]) != Muon_charge->at(vSelectedMuons[1]))
        {
          hSelDSMuonsPhiForHighDz->Fill(phi0);
          hSelDSMuonsPhiForHighDz->Fill(phi1);
          hSelDSMuonsEtaForHighDz->Fill(eta0);
          hSelDSMuonsEtaForHighDz->Fill(eta1);
          hSelDSMuonsDzForHighDz->Fill(dz0);
          hSelDSMuonsDzForHighDz->Fill(dz1);
          hSelDSMuonsDxyForHighDz->Fill(dxy0);
          hSelDSMuonsDxyForHighDz->Fill(dxy1);
          hSelDSMuonsDxyzForHighDz->Fill(dxyz0);
          hSelDSMuonsDxyzForHighDz->Fill(dxyz1);
          hSelDSMuonsDzBSForHighDz->Fill(dzBS0);
          hSelDSMuonsDzBSForHighDz->Fill(dzBS1);
          hSelDSMuonsDz1VsDz2ForHighDz->Fill(dz0,dz1);
          hSelDSMuonsDxy1VsDxy2ForHighDz->Fill(dxy0,dxy1);
          hSelDSMuonsDxyz1VsDxyz2ForHighDz->Fill(dxyz0,dxyz1);
        }
        else
        {
          hSelSSMuonsPhiForHighDz->Fill(phi0);
          hSelSSMuonsPhiForHighDz->Fill(phi1);
          hSelSSMuonsEtaForHighDz->Fill(eta0);
          hSelSSMuonsEtaForHighDz->Fill(eta1);
          hSelSSMuonsDzForHighDz->Fill(dz0);
          hSelSSMuonsDzForHighDz->Fill(dz1);
          hSelSSMuonsDxyForHighDz->Fill(dxy0);
          hSelSSMuonsDxyForHighDz->Fill(dxy1);
          hSelSSMuonsDxyzForHighDz->Fill(dxyz0);
          hSelSSMuonsDxyzForHighDz->Fill(dxyz1);
          hSelSSMuonsDzBSForHighDz->Fill(dzBS0);
          hSelSSMuonsDzBSForHighDz->Fill(dzBS1);
          hSelSSMuonsDz1VsDz2ForHighDz->Fill(dz0,dz1);
          hSelSSMuonsDxy1VsDxy2ForHighDz->Fill(dxy0,dxy1);
          hSelSSMuonsDxyz1VsDxyz2ForHighDz->Fill(dxyz0,dxyz1);
        }
      }
    }
    
    hNRecoMuons->Fill(nRecoMuons);
    hNSelMuons->Fill(nSelMuons);
    hNAllSelMuons->Fill(nAllSelMuons);
    hNTriggeringRecoMuons->Fill(nTriggeringRecoMuons);
    hNTriggeringSelMuons->Fill(nTriggeringSelMuons);
    hNTriggeringAllSelMuons->Fill(nTriggeringAllSelMuons);
    hNRecoMuonsVsRunNumber->Fill(runNumber,nRecoMuons);
    hNSelMuonsVsRunNumber->Fill(runNumber,nSelMuons);
    hNAllSelMuonsVsRunNumber->Fill(runNumber,nAllSelMuons);
    
    for(int iVtx = 0; iVtx < numberOfVertex; iVtx++)
    {
      nRecoVertices++;
      hRecoVerticesX->Fill(PV_x->at(iVtx));
      hRecoVerticesXError->Fill(PV_xError->at(iVtx));
      hRecoVerticesXErrorVsRunNumber->Fill(runNumber, PV_xError->at(iVtx));
      hRecoVerticesY->Fill(PV_y->at(iVtx));
      hRecoVerticesYError->Fill(PV_yError->at(iVtx));
      hRecoVerticesYErrorVsRunNumber->Fill(runNumber, PV_yError->at(iVtx));
      hRecoVerticesZ->Fill(PV_z->at(iVtx));
      hRecoVerticesZError->Fill(PV_zError->at(iVtx));
      hRecoVerticesZErrorVsRunNumber->Fill(runNumber, PV_zError->at(iVtx));
      hRecoVerticesNDOF->Fill(PV_ndof->at(iVtx));
      hRecoVerticesNormalizedChi2->Fill(PV_normalizedChi2->at(iVtx));
      hRecoVerticesTrkMultiplicity->Fill(PV_vTrackref->at(iVtx).size());
      if(s->SelectVertex(iVtx,"V0"))
      {
        nAllSelVertices++;
        hAllSelVerticesX->Fill(PV_x->at(iVtx));
        hAllSelVerticesXError->Fill(PV_xError->at(iVtx));
        hAllSelVerticesXErrorVsRunNumber->Fill(runNumber, PV_xError->at(iVtx));
        hAllSelVerticesY->Fill(PV_y->at(iVtx));
        hAllSelVerticesYError->Fill(PV_yError->at(iVtx));
        hAllSelVerticesYErrorVsRunNumber->Fill(runNumber, PV_yError->at(iVtx));
        hAllSelVerticesZ->Fill(PV_z->at(iVtx));
        hAllSelVerticesZError->Fill(PV_zError->at(iVtx));
        hAllSelVerticesZErrorVsRunNumber->Fill(runNumber, PV_zError->at(iVtx));
        hAllSelVerticesNDOF->Fill(PV_ndof->at(iVtx));
        hAllSelVerticesNormalizedChi2->Fill(PV_normalizedChi2->at(iVtx));
        hAllSelVerticesTrkMultiplicity->Fill(PV_vTrackref->at(iVtx).size());
      }
    }
    
    nSelVertices++; // yeah I know it is not needed
    hSelVerticesX->Fill(PV_x->at(iSelectedVertex));
    hSelVerticesXError->Fill(PV_xError->at(iSelectedVertex));
    hSelVerticesXErrorVsRunNumber->Fill(runNumber, PV_xError->at(iSelectedVertex));
    hSelVerticesY->Fill(PV_y->at(iSelectedVertex));
    hSelVerticesYError->Fill(PV_yError->at(iSelectedVertex));
    hSelVerticesYErrorVsRunNumber->Fill(runNumber, PV_yError->at(iSelectedVertex));
    hSelVerticesZ->Fill(PV_z->at(iSelectedVertex));
    hSelVerticesZError->Fill(PV_zError->at(iSelectedVertex));
    hSelVerticesZErrorVsRunNumber->Fill(runNumber, PV_zError->at(iSelectedVertex));
    hSelVerticesNDOF->Fill(PV_ndof->at(iSelectedVertex));
    hSelVerticesNormalizedChi2->Fill(PV_normalizedChi2->at(iSelectedVertex));
    hSelVerticesTrkMultiplicity->Fill(PV_vTrackref->at(iSelectedVertex).size());
    
    hNRecoVertices->Fill(nRecoVertices);
    hNSelVertices->Fill(nSelVertices);
    hNAllSelVertices->Fill(nAllSelVertices);
    hNRecoVerticesVsRunNumber->Fill(runNumber, nRecoVertices);
    hNSelVerticesVsRunNumber->Fill(runNumber, nSelVertices);
    hNAllSelVerticesVsRunNumber->Fill(runNumber, nAllSelVertices);
  }

  // Keep this line here!
  outFile->cd();
  
  // Writes into the outFile the name of the efficiency file used in this run
  WriteEfficiencyMetadata(outFile);
  
  TProfile* hpxRecoVerticesXErrorVsRunNumber = hRecoVerticesXErrorVsRunNumber->ProfileX();
  TProfile* hpxSelVerticesXErrorVsRunNumber = hSelVerticesXErrorVsRunNumber->ProfileX();
  TProfile* hpxAllSelVerticesXErrorVsRunNumber = hAllSelVerticesXErrorVsRunNumber->ProfileX();
  TProfile* hpxRecoVerticesYErrorVsRunNumber = hRecoVerticesYErrorVsRunNumber->ProfileX();
  TProfile* hpxSelVerticesYErrorVsRunNumber = hSelVerticesYErrorVsRunNumber->ProfileX();
  TProfile* hpxAllSelVerticesYErrorVsRunNumber = hAllSelVerticesYErrorVsRunNumber->ProfileX();
  TProfile* hpxRecoVerticesZErrorVsRunNumber = hRecoVerticesZErrorVsRunNumber->ProfileX();
  TProfile* hpxSelVerticesZErrorVsRunNumber = hSelVerticesZErrorVsRunNumber->ProfileX();
  TProfile* hpxAllSelVerticesZErrorVsRunNumber = hAllSelVerticesZErrorVsRunNumber->ProfileX();
  
  TProfile* hpxNRecoMuonsVsRunNumber = hNRecoMuonsVsRunNumber->ProfileX();
  TProfile* hpxNSelMuonsVsRunNumber = hNSelMuonsVsRunNumber->ProfileX();
  TProfile* hpxNAllSelMuonsVsRunNumber = hNAllSelMuonsVsRunNumber->ProfileX();
  TProfile* hpxNRecoVerticesVsRunNumber = hNRecoVerticesVsRunNumber->ProfileX();
  TProfile* hpxNSelVerticesVsRunNumber = hNSelVerticesVsRunNumber->ProfileX();
  TProfile* hpxNAllSelVerticesVsRunNumber = hNAllSelVerticesVsRunNumber->ProfileX();
  
  TProfile* hpxSelMuonDxyVsRunNumber = hSelMuonDxyVsRunNumber->ProfileX();
  TProfile* hpxSelEffCorMuonDxyVsRunNumber = hSelEffCorMuonDxyVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDxyErrorVsRunNumber = hSelMuonDxyErrorVsRunNumber->ProfileX();
  TProfile* hpxSelEffCorMuonDxyErrorVsRunNumber = hSelEffCorMuonDxyErrorVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDxyzVsRunNumber = hSelMuonDxyzVsRunNumber->ProfileX();
  TProfile* hpxSelEffCorMuonDxyzVsRunNumber = hSelEffCorMuonDxyzVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDxyzErrorVsRunNumber = hSelMuonDxyzErrorVsRunNumber->ProfileX();
  TProfile* hpxSelEffCorMuonDxyzErrorVsRunNumber = hSelEffCorMuonDxyzErrorVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDzVsRunNumber = hSelMuonDzVsRunNumber->ProfileX();
  TProfile* hpxSelEffCorMuonDzVsRunNumber = hSelEffCorMuonDzVsRunNumber->ProfileX();
  TProfile* hpxSelMuonAbsDzVsRunNumber = hSelMuonAbsDzVsRunNumber->ProfileX();
  TProfile* hpxSelEffCorMuonAbsDzVsRunNumber = hSelEffCorMuonAbsDzVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDzErrorVsRunNumber = hSelMuonDzErrorVsRunNumber->ProfileX();
  TProfile* hpxSelEffCorMuonDzErrorVsRunNumber = hSelEffCorMuonDzErrorVsRunNumber->ProfileX();
  
  // Create and write canvases here
  CreateAndWriteCanvas("cRecoMuonPt",0, 21, 1, false, true, hRecoMuonPt);
  CreateAndWriteCanvas("cSelMuonPt",0, 21, 1, false, true, hSelMuonPt);
  CreateAndWriteCanvas("cSelEffCorMuonPt",0, 21, 1, false, true, hSelEffCorMuonPt);
  CreateAndWriteCanvas("cAllSelMuonPt",0, 21, 1, false, true, hAllSelMuonPt);
  CreateAndWriteCanvas("cSelMuonLeadingPt",0, 21, 1, false, true, hSelMuonLeadingPt);
  CreateAndWriteCanvas("cSelEffCorMuonLeadingPt",0, 21, 1, false, true, hSelEffCorMuonLeadingPt);
  CreateAndWriteCanvas("cSelMuonSecondPt",0, 21, 1, false, true, hSelMuonSecondPt);
  CreateAndWriteCanvas("cSelEffCorMuonSecondPt",0, 21, 1, false, true, hSelEffCorMuonSecondPt);
  CreateAndWriteCanvas("cRecoMuonEta",0, 21, 1, false, false, hRecoMuonEta);
  CreateAndWriteCanvas("cSelMuonEta",0, 21, 1, false, false, hSelMuonEta);
  CreateAndWriteCanvas("cSelEffCorMuonEta",0, 21, 1, false, false, hSelEffCorMuonEta);
  CreateAndWriteCanvas("cAllSelMuonEta",0, 21, 1, false, false, hAllSelMuonEta);
  CreateAndWriteCanvas("cSelMuonLeadingEta",0, 21, 1, false, true, hSelMuonLeadingEta);
  CreateAndWriteCanvas("cSelEffCorMuonLeadingEta",0, 21, 1, false, true, hSelEffCorMuonLeadingEta);
  CreateAndWriteCanvas("cSelMuonSecondEta",0, 21, 1, false, true, hSelMuonSecondEta);
  CreateAndWriteCanvas("cSelEffCorMuonSecondEta",0, 21, 1, false, true, hSelEffCorMuonSecondEta);
  CreateAndWriteCanvas("cRecoMuonPhi",0, 21, 1, false, false, hRecoMuonPhi);
  CreateAndWriteCanvas("cSelMuonPhi",0, 21, 1, false, false, hSelMuonPhi);
  CreateAndWriteCanvas("cSelEffCorMuonPhi",0, 21, 1, false, false, hSelEffCorMuonPhi);
  CreateAndWriteCanvas("cAllSelMuonPhi",0, 21, 1, false, false, hAllSelMuonPhi);
  CreateAndWriteCanvas("cRecoMuonCharge",0, 21, 1, false, false, hRecoMuonCharge);
  CreateAndWriteCanvas("cSelMuonCharge",0, 21, 1, false, false, hSelMuonCharge);
  CreateAndWriteCanvas("cSelEffCorMuonCharge",0, 21, 1, false, false, hSelEffCorMuonCharge);
  CreateAndWriteCanvas("cAllSelMuonCharge",0, 21, 1, false, false, hAllSelMuonCharge);
  CreateAndWriteCanvas("cRecoMuonHasTriggered",0, 21, 1, false, true, hRecoMuonHasTriggered);
  CreateAndWriteCanvas("cSelMuonHasTriggered",0, 21, 1, false, true, hSelMuonHasTriggered);
  CreateAndWriteCanvas("cAllSelMuonHasTriggered",0, 21, 1, false, true, hAllSelMuonHasTriggered);
  CreateAndWriteCanvas("cRecoMuonCaloCompatibility",0, 21, 1, false, false, hRecoMuonCaloCompatibility);
  CreateAndWriteCanvas("cSelMuonCaloCompatibility",0, 21, 1, false, false, hSelMuonCaloCompatibility);
  CreateAndWriteCanvas("cSelEffCorMuonCaloCompatibility",0, 21, 1, false, false, hSelEffCorMuonCaloCompatibility);
  CreateAndWriteCanvas("cAllSelMuonCaloCompatibility",0, 21, 1, false, false, hAllSelMuonCaloCompatibility);
  CreateAndWriteCanvas("cRecoMuonSegmentCompatibility",0, 21, 1, false, false, hRecoMuonSegmentCompatibility);
  CreateAndWriteCanvas("cSelMuonSegmentCompatibility",0, 21, 1, false, false, hSelMuonSegmentCompatibility);
  CreateAndWriteCanvas("cSelEffCorMuonSegmentCompatibility",0, 21, 1, false, false, hSelEffCorMuonSegmentCompatibility);
  CreateAndWriteCanvas("cAllSelMuonSegmentCompatibility",0, 21, 1, false, false, hAllSelMuonSegmentCompatibility);
  CreateAndWriteCanvas("cRecoMuonTrackValidHits",0, 21, 1, false, false, hRecoMuonTrackValidHits);
  CreateAndWriteCanvas("cSelMuonTrackValidHits",0, 21, 1, false, false, hSelMuonTrackValidHits);
  CreateAndWriteCanvas("cSelEffCorMuonTrackValidHits",0, 21, 1, false, false, hSelEffCorMuonTrackValidHits);
  CreateAndWriteCanvas("cAllSelMuonTrackValidHits",0, 21, 1, false, false, hAllSelMuonTrackValidHits);
  CreateAndWriteCanvas("cRecoMuonTrackChi2",0, 21, 1, false, false, hRecoMuonTrackChi2);
  CreateAndWriteCanvas("cSelMuonTrackChi2",0, 21, 1, false, false, hSelMuonTrackChi2);
  CreateAndWriteCanvas("cSelEffCorMuonTrackChi2",0, 21, 1, false, false, hSelEffCorMuonTrackChi2);
  CreateAndWriteCanvas("cAllSelMuonTrackChi2",0, 21, 1, false, false, hAllSelMuonTrackChi2);
  CreateAndWriteCanvas("cRecoMuonTrackNdof",0, 21, 1, false, false, hRecoMuonTrackNdof);
  CreateAndWriteCanvas("cSelMuonTrackNdof",0, 21, 1, false, false, hSelMuonTrackNdof);
  CreateAndWriteCanvas("cSelEffCorMuonTrackNdof",0, 21, 1, false, false, hSelEffCorMuonTrackNdof);
  CreateAndWriteCanvas("cAllSelMuonTrackNdof",0, 21, 1, false, false, hAllSelMuonTrackNdof);
  CreateAndWriteCanvas("cRecoMuonTrackValidPixelHits",0, 21, 1, false, false, hRecoMuonTrackValidPixelHits);
  CreateAndWriteCanvas("cSelMuonTrackValidPixelHits",0, 21, 1, false, false, hSelMuonTrackValidPixelHits);
  CreateAndWriteCanvas("cSelEffCorMuonTrackValidPixelHits",0, 21, 1, false, false, hSelEffCorMuonTrackValidPixelHits);
  CreateAndWriteCanvas("cAllSelMuonTrackValidPixelHits",0, 21, 1, false, false, hAllSelMuonTrackValidPixelHits);
  CreateAndWriteCanvas("cRecoMuonGlobalChi2",0, 21, 1, false, false, hRecoMuonGlobalChi2);
  CreateAndWriteCanvas("cSelMuonGlobalChi2",0, 21, 1, false, false, hSelMuonGlobalChi2);
  CreateAndWriteCanvas("cSelEffCorMuonGlobalChi2",0, 21, 1, false, false, hSelEffCorMuonGlobalChi2);
  CreateAndWriteCanvas("cAllSelMuonGlobalChi2",0, 21, 1, false, false, hAllSelMuonGlobalChi2);
  CreateAndWriteCanvas("cRecoMuonGlobalValidMuonHits",0, 21, 1, false, false, hRecoMuonGlobalValidMuonHits);
  CreateAndWriteCanvas("cSelMuonGlobalValidMuonHits",0, 21, 1, false, false, hSelMuonGlobalValidMuonHits);
  CreateAndWriteCanvas("cSelEffCorMuonGlobalValidMuonHits",0, 21, 1, false, false, hSelEffCorMuonGlobalValidMuonHits);
  CreateAndWriteCanvas("cAllSelMuonGlobalValidMuonHits",0, 21, 1, false, false, hAllSelMuonGlobalValidMuonHits);
  CreateAndWriteCanvas("cRecoMuonNumberOfMatches",0, 21, 1, false, false, hRecoMuonNumberOfMatches);
  CreateAndWriteCanvas("cSelMuonNumberOfMatches",0, 21, 1, false, false, hSelMuonNumberOfMatches);
  CreateAndWriteCanvas("cSelEffCorMuonNumberOfMatches",0, 21, 1, false, false, hSelEffCorMuonNumberOfMatches);
  CreateAndWriteCanvas("cAllSelMuonNumberOfMatches",0, 21, 1, false, false, hAllSelMuonNumberOfMatches);
  CreateAndWriteCanvas("cRecoDiMuonInvMass",0, 21, 1, false, false, hRecoDiMuonInvMass);
  CreateAndWriteCanvas("cSelDiMuonInvMass",0, 21, 1, false, false, hSelDiMuonInvMass);
  CreateAndWriteCanvas("cSelEffCorDiMuonInvMass",0, 21, 1, false, false, hSelEffCorDiMuonInvMass);
  CreateAndWriteCanvas("cAllSelDiMuonInvMass",0, 21, 1, false, false, hAllSelDiMuonInvMass);
  CreateAndWriteCanvas("cRecoDiMuonDeltaR",0, 21, 1, false, false, hRecoDiMuonDeltaR);
  CreateAndWriteCanvas("cSelDiMuonDeltaR",0, 21, 1, false, false, hSelDiMuonDeltaR);
  CreateAndWriteCanvas("cSelEffCorDiMuonDeltaR",0, 21, 1, false, false, hSelEffCorDiMuonDeltaR);
  CreateAndWriteCanvas("cAllSelDiMuonDeltaR",0, 21, 1, false, false, hAllSelDiMuonDeltaR);
  CreateAndWriteCanvas("cRecoDiMuonDeltaEta",0, 21, 1, false, false, hRecoDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelDiMuonDeltaEta",0, 21, 1, false, false, hSelDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelEffCorDiMuonDeltaEta",0, 21, 1, false, false, hSelEffCorDiMuonDeltaEta);
  CreateAndWriteCanvas("cAllSelDiMuonDeltaEta",0, 21, 1, false, false, hAllSelDiMuonDeltaEta);
  CreateAndWriteCanvas("cRecoDiMuonDeltaPhi",0, 21, 1, false, false, hRecoDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelDiMuonDeltaPhi",0, 21, 1, false, false, hSelDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelEffCorDiMuonDeltaPhi",0, 21, 1, false, false, hSelEffCorDiMuonDeltaPhi);
  CreateAndWriteCanvas("cAllSelDiMuonDeltaPhi",0, 21, 1, false, false, hAllSelDiMuonDeltaPhi);
  CreateAndWriteCanvas("cRecoDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hRecoDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelEffCorDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelEffCorDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cAllSelDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hAllSelDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cRecoDiMuonInvMassVsDeltaR","colz", false, false,false, hRecoDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelDiMuonInvMassVsDeltaR","colz", false, false,false, hSelDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelEffCorDiMuonInvMassVsDeltaR","colz", false, false,false, hSelEffCorDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cAllSelDiMuonInvMassVsDeltaR","colz", false, false,false, hAllSelDiMuonInvMassVsDeltaR);
  
  CreateAndWriteCanvas("cRecoDSDiMuonInvMass",0, 21, 1, false, false, hRecoDSDiMuonInvMass);
  CreateAndWriteCanvas("cSelDSDiMuonInvMass",0, 21, 1, false, false, hSelDSDiMuonInvMass);
  CreateAndWriteCanvas("cSelEffCorDSDiMuonInvMass",0, 21, 1, false, false, hSelEffCorDSDiMuonInvMass);
  CreateAndWriteCanvas("cAllSelDSDiMuonInvMass",0, 21, 1, false, false, hAllSelDSDiMuonInvMass);
  CreateAndWriteCanvas("cRecoDSDiMuonDeltaR",0, 21, 1, false, false, hRecoDSDiMuonDeltaR);
  CreateAndWriteCanvas("cSelDSDiMuonDeltaR",0, 21, 1, false, false, hSelDSDiMuonDeltaR);
  CreateAndWriteCanvas("cSelEffCorDSDiMuonDeltaR",0, 21, 1, false, false, hSelEffCorDSDiMuonDeltaR);
  CreateAndWriteCanvas("cAllSelDSDiMuonDeltaR",0, 21, 1, false, false, hAllSelDSDiMuonDeltaR);
  CreateAndWriteCanvas("cRecoDSDiMuonDeltaEta",0, 21, 1, false, false, hRecoDSDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelDSDiMuonDeltaEta",0, 21, 1, false, false, hSelDSDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelEffCorDSDiMuonDeltaEta",0, 21, 1, false, false, hSelEffCorDSDiMuonDeltaEta);
  CreateAndWriteCanvas("cAllSelDSDiMuonDeltaEta",0, 21, 1, false, false, hAllSelDSDiMuonDeltaEta);
  CreateAndWriteCanvas("cRecoDSDiMuonDeltaPhi",0, 21, 1, false, false, hRecoDSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelDSDiMuonDeltaPhi",0, 21, 1, false, false, hSelDSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelEffCorDSDiMuonDeltaPhi",0, 21, 1, false, false, hSelEffCorDSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cAllSelDSDiMuonDeltaPhi",0, 21, 1, false, false, hAllSelDSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cRecoDSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hRecoDSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelDSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelDSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelEffCorDSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelEffCorDSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cAllSelDSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hAllSelDSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cRecoDSDiMuonInvMassVsDeltaR","colz", false, false,false, hRecoDSDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelDSDiMuonInvMassVsDeltaR","colz", false, false,false, hSelDSDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelEffCorDSDiMuonInvMassVsDeltaR","colz", false, false,false, hSelEffCorDSDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cAllSelDSDiMuonInvMassVsDeltaR","colz", false, false,false, hAllSelDSDiMuonInvMassVsDeltaR);
  
  CreateAndWriteCanvas("cRecoSSDiMuonInvMass",0, 21, 1, false, false, hRecoSSDiMuonInvMass);
  CreateAndWriteCanvas("cSelSSDiMuonInvMass",0, 21, 1, false, false, hSelSSDiMuonInvMass);
  CreateAndWriteCanvas("cSelEffCorSSDiMuonInvMass",0, 21, 1, false, false, hSelEffCorSSDiMuonInvMass);
  CreateAndWriteCanvas("cAllSelSSDiMuonInvMass",0, 21, 1, false, false, hAllSelSSDiMuonInvMass);
  CreateAndWriteCanvas("cRecoSSDiMuonDeltaR",0, 21, 1, false, false, hRecoSSDiMuonDeltaR);
  CreateAndWriteCanvas("cSelSSDiMuonDeltaR",0, 21, 1, false, false, hSelSSDiMuonDeltaR);
  CreateAndWriteCanvas("cSelEffCorSSDiMuonDeltaR",0, 21, 1, false, false, hSelEffCorSSDiMuonDeltaR);
  CreateAndWriteCanvas("cAllSelSSDiMuonDeltaR",0, 21, 1, false, false, hAllSelSSDiMuonDeltaR);
  CreateAndWriteCanvas("cRecoSSDiMuonDeltaEta",0, 21, 1, false, false, hRecoSSDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelSSDiMuonDeltaEta",0, 21, 1, false, false, hSelSSDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelEffCorSSDiMuonDeltaEta",0, 21, 1, false, false, hSelEffCorSSDiMuonDeltaEta);
  CreateAndWriteCanvas("cAllSelSSDiMuonDeltaEta",0, 21, 1, false, false, hAllSelSSDiMuonDeltaEta);
  CreateAndWriteCanvas("cRecoSSDiMuonDeltaPhi",0, 21, 1, false, false, hRecoSSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelSSDiMuonDeltaPhi",0, 21, 1, false, false, hSelSSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelEffCorSSDiMuonDeltaPhi",0, 21, 1, false, false, hSelEffCorSSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cAllSelSSDiMuonDeltaPhi",0, 21, 1, false, false, hAllSelSSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cRecoSSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hRecoSSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelSSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelSSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelEffCorSSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelEffCorSSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cAllSelSSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hAllSelSSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cRecoSSDiMuonInvMassVsDeltaR","colz", false, false,false, hRecoSSDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelSSDiMuonInvMassVsDeltaR","colz", false, false,false, hSelSSDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelEffCorSSDiMuonInvMassVsDeltaR","colz", false, false,false, hSelEffCorSSDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cAllSelSSDiMuonInvMassVsDeltaR","colz", false, false,false, hAllSelSSDiMuonInvMassVsDeltaR);
  
  CreateAndWriteCanvas("cSelMuonDxy",0, 21, 1, false, true, hSelMuonDxy);
  CreateAndWriteCanvas("cSelEffCorMuonDxy",0, 21, 1, false, true, hSelEffCorMuonDxy);
  CreateAndWriteCanvas("cSelMuonDxyError",0, 21, 1, false, true, hSelMuonDxyError);
  CreateAndWriteCanvas("cSelEffCorMuonDxyError",0, 21, 1, false, true, hSelEffCorMuonDxyError);
  CreateAndWriteCanvas("cSelMuonDxyz",0, 21, 1, false, true, hSelMuonDxyz);
  CreateAndWriteCanvas("cSelEffCorMuonDxyz",0, 21, 1, false, true, hSelEffCorMuonDxyz);
  CreateAndWriteCanvas("cSelMuonDxyzError",0, 21, 1, false, true, hSelMuonDxyzError);
  CreateAndWriteCanvas("cSelEffCorMuonDxyzError",0, 21, 1, false, true, hSelEffCorMuonDxyzError);
  CreateAndWriteCanvas("cSelMuonDz",0, 21, 1, false, true, hSelMuonDz);
  CreateAndWriteCanvas("cSelEffCorMuonDz",0, 21, 1, false, true, hSelEffCorMuonDz);
  CreateAndWriteCanvas("cSelMuonAbsDz",0, 21, 1, false, true, hSelMuonAbsDz);
  CreateAndWriteCanvas("cSelEffCorMuonAbsDz",0, 21, 1, false, true, hSelEffCorMuonAbsDz);
  CreateAndWriteCanvas("cSelMuonDzError",0, 21, 1, false, true, hSelMuonDzError);
  CreateAndWriteCanvas("cSelEffCorMuonDzError",0, 21, 1, false, true, hSelEffCorMuonDzError);
  CreateAndWriteCanvas("cSelMuonDxyVsRunNumber", "colz", false, false, true, hSelMuonDxyVsRunNumber);
  CreateAndWriteCanvas("cSelEffCorMuonDxyVsRunNumber", "colz", false, false, true, hSelEffCorMuonDxyVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDxyErrorVsRunNumber", "colz", false, false, true, hSelMuonDxyErrorVsRunNumber);
  CreateAndWriteCanvas("cSelEffCorMuonDxyErrorVsRunNumber", "colz", false, false, true, hSelEffCorMuonDxyErrorVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDxyzVsRunNumber", "colz", false, false, true, hSelMuonDxyzVsRunNumber);
  CreateAndWriteCanvas("cSelEffCorMuonDxyzVsRunNumber", "colz", false, false, true, hSelEffCorMuonDxyzVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDxyzErrorVsRunNumber", "colz", false, false, true, hSelMuonDxyzErrorVsRunNumber);
  CreateAndWriteCanvas("cSelEffCorMuonDxyzErrorVsRunNumber", "colz", false, false, true, hSelEffCorMuonDxyzErrorVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDzVsRunNumber", "colz", false, false, true, hSelMuonDzVsRunNumber);
  CreateAndWriteCanvas("cSelEffCorMuonDzVsRunNumber", "colz", false, false, true, hSelEffCorMuonDzVsRunNumber);
  CreateAndWriteCanvas("cSelMuonAbsDzVsRunNumber", "colz", false, false, true, hSelMuonAbsDzVsRunNumber);
  CreateAndWriteCanvas("cSelEffCorMuonAbsDzVsRunNumber", "colz", false, false, true, hSelEffCorMuonAbsDzVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDzErrorVsRunNumber", "colz", false, false, true, hSelMuonDzErrorVsRunNumber);
  CreateAndWriteCanvas("cSelEffCorMuonDzErrorVsRunNumber", "colz", false, false, true, hSelEffCorMuonDzErrorVsRunNumber);
  CreateAndWriteCanvas("cpxcSelMuonDxyVsRunNumber",0, 21, 1, false, false, hpxSelMuonDxyVsRunNumber);
  CreateAndWriteCanvas("cpxcSelEffCorMuonDxyVsRunNumber",0, 21, 1, false, false, hpxSelEffCorMuonDxyVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDxyErrorVsRunNumber",0, 21, 1, false, false, hpxSelMuonDxyErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelEffCorMuonDxyErrorVsRunNumber",0, 21, 1, false, false, hpxSelEffCorMuonDxyErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDxyzVsRunNumber",0, 21, 1, false, false, hpxSelMuonDxyzVsRunNumber);
  CreateAndWriteCanvas("cpxSelEffCorMuonDxyzVsRunNumber",0, 21, 1, false, false, hpxSelEffCorMuonDxyzVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDxyzErrorVsRunNumber",0, 21, 1, false, false, hpxSelMuonDxyzErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelEffCorMuonDxyzErrorVsRunNumber",0, 21, 1, false, false, hpxSelEffCorMuonDxyzErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDzVsRunNumber",0, 21, 1, false, false, hpxSelMuonDzVsRunNumber);
  CreateAndWriteCanvas("cpxSelEffCorMuonDzVsRunNumber",0, 21, 1, false, false, hpxSelEffCorMuonDzVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonAbsDzVsRunNumber",0, 21, 1, false, false, hpxSelMuonAbsDzVsRunNumber);
  CreateAndWriteCanvas("cpxSelEffCorMuonAbsDzVsRunNumber",0, 21, 1, false, false, hpxSelEffCorMuonAbsDzVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDzErrorVsRunNumber",0, 21, 1, false, false, hpxSelMuonDzErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelEffCorMuonDzErrorVsRunNumber",0, 21, 1, false, false, hpxSelEffCorMuonDzErrorVsRunNumber);

  CreateAndWriteCanvas("cSelMuonSignedDxyBS",0, 21, 1, false, true, hSelMuonSignedDxyBS);
  CreateAndWriteCanvas("cSelEffCorMuonSignedDxyBS",0, 21, 1, false, true, hSelEffCorMuonSignedDxyBS);
  CreateAndWriteCanvas("cSelMuonDxyBSError",0, 21, 1, false, true, hSelMuonDxyBSError);
  CreateAndWriteCanvas("cSelEffCorMuonDxyBSError",0, 21, 1, false, true, hSelEffCorMuonDxyBSError);
  CreateAndWriteCanvas("cSelMuonSignedDxyBSVsPhi", "colz", false, false, true, hSelMuonSignedDxyBSVsPhi);
  CreateAndWriteCanvas("cSelEffCorMuonSignedDxyBSVsPhi", "colz", false, false, true, hSelEffCorMuonSignedDxyBSVsPhi);
  CreateAndWriteCanvas("cSelMuonDxyBSErrorVsPhi", "colz", false, false, true, hSelMuonDxyBSErrorVsPhi);
  CreateAndWriteCanvas("cSelEffCorMuonDxyBSErrorVsPhi", "colz", false, false, true, hSelEffCorMuonDxyBSErrorVsPhi);
  
  CreateAndWriteCanvas("cSelMuonsPhiForHighDz",0, 21, 1, false, false, hSelMuonsPhiForHighDz);
  CreateAndWriteCanvas("cSelDSMuonsPhiForHighDz",0, 21, 1, false, false, hSelDSMuonsPhiForHighDz);
  CreateAndWriteCanvas("cSelSSMuonsPhiForHighDz",0, 21, 1, false, false, hSelSSMuonsPhiForHighDz);

  CreateAndWriteCanvas("cSelMuonsEtaForHighDz",0, 21, 1, false, false, hSelMuonsEtaForHighDz);
  CreateAndWriteCanvas("cSelDSMuonsEtaForHighDz",0, 21, 1, false, false, hSelDSMuonsEtaForHighDz);
  CreateAndWriteCanvas("cSelSSMuonsEtaForHighDz",0, 21, 1, false, false, hSelSSMuonsEtaForHighDz);
  
  CreateAndWriteCanvas("cSelMuonsDzForHighDz",0, 21, 1, false, true, hSelMuonsDzForHighDz);
  CreateAndWriteCanvas("cSelMuonsDxyForHighDz",0, 21, 1, false, true, hSelMuonsDxyForHighDz);
  CreateAndWriteCanvas("cSelMuonsDxyzForHighDz",0, 21, 1, false, true, hSelMuonsDxyzForHighDz);
  CreateAndWriteCanvas("cSelMuonsDzBSForHighDz",0, 21, 1, false, true, hSelMuonsDzBSForHighDz);
  CreateAndWriteCanvas("cSelMuonsDz1VsDz2ForHighDz", "colz", false, false, true, hSelMuonsDz1VsDz2ForHighDz);
  CreateAndWriteCanvas("cSelMuonsDxy1VsDxy2ForHighDz","colz", false, false, true, hSelMuonsDxy1VsDxy2ForHighDz);
  CreateAndWriteCanvas("cSelMuonsDxyz1VsDxyz2ForHighDz", "colz", false, false, true, hSelMuonsDxyz1VsDxyz2ForHighDz);
  
  CreateAndWriteCanvas("cSelDSMuonsDzForHighDz",0, 21, 1, false, true, hSelDSMuonsDzForHighDz);
  CreateAndWriteCanvas("cSelDSMuonsDxyForHighDz",0, 21, 1, false, true, hSelDSMuonsDxyForHighDz);
  CreateAndWriteCanvas("cSelDSMuonsDxyzForHighDz",0, 21, 1, false, true, hSelDSMuonsDxyzForHighDz);
  CreateAndWriteCanvas("cSelDSMuonsDzBSForHighDz",0, 21, 1, false, true, hSelDSMuonsDzBSForHighDz);
  CreateAndWriteCanvas("cSelDSMuonsDz1VsDz2ForHighDz", "colz", false, false, true, hSelDSMuonsDz1VsDz2ForHighDz);
  CreateAndWriteCanvas("cSelDSMuonsDxy1VsDxy2ForHighDz", "colz", false, false, true, hSelDSMuonsDxy1VsDxy2ForHighDz);
  CreateAndWriteCanvas("cSelDSMuonsDxyz1VsDxyz2ForHighDz", "colz", false, false, true, hSelDSMuonsDxyz1VsDxyz2ForHighDz);
  
  CreateAndWriteCanvas("cSelSSMuonsDzForHighDz",0, 21, 1, false, true, hSelSSMuonsDzForHighDz);
  CreateAndWriteCanvas("cSelSSMuonsDxyForHighDz",0, 21, 1, false, true, hSelSSMuonsDxyForHighDz);
  CreateAndWriteCanvas("cSelSSMuonsDxyzForHighDz",0, 21, 1, false, true, hSelSSMuonsDxyzForHighDz);
  CreateAndWriteCanvas("cSelSSMuonsDzBSForHighDz",0, 21, 1, false, true, hSelSSMuonsDzBSForHighDz);
  CreateAndWriteCanvas("cSelSSMuonsDz1VsDz2ForHighDz", "colz", false, false, true, hSelSSMuonsDz1VsDz2ForHighDz);
  CreateAndWriteCanvas("cSelSSMuonsDxy1VsDxy2ForHighDz", "colz", false, false, true, hSelSSMuonsDxy1VsDxy2ForHighDz);
  CreateAndWriteCanvas("cSelSSMuonsDxyz1VsDxyz2ForHighDz", "colz", false, false, true, hSelSSMuonsDxyz1VsDxyz2ForHighDz);
  
  CreateAndWriteCanvas("cNRecoMuons",0, 21, 1, false, true, hNRecoMuons);
  CreateAndWriteCanvas("cNSelMuons",0, 21, 1, false, true, hNSelMuons);
  CreateAndWriteCanvas("cNAllSelMuons",0, 21, 1, false, true, hNAllSelMuons);
  CreateAndWriteCanvas("cNTriggeringRecoMuons",0, 21, 1, false, true, hNTriggeringRecoMuons);
  CreateAndWriteCanvas("cNTriggeringSelMuons",0, 21, 1, false, true, hNTriggeringSelMuons);
  CreateAndWriteCanvas("cNTriggeringAllSelMuons",0, 21, 1, false, true, hNTriggeringAllSelMuons);
  CreateAndWriteCanvas("cNRecoMuonsVsRunNumber", "colz", false, false, true, hNRecoMuonsVsRunNumber);
  CreateAndWriteCanvas("cNSelMuonsVsRunNumber", "colz", false, false, true, hNSelMuonsVsRunNumber);
  CreateAndWriteCanvas("cNAllSelMuonsVsRunNumber", "colz", false, false, true, hNAllSelMuonsVsRunNumber);
  CreateAndWriteCanvas("cpxNRecoMuonsVsRunNumber", 0, 21, 1, false, false, hpxNRecoMuonsVsRunNumber);
  CreateAndWriteCanvas("cpxNSelMuonsVsRunNumber", 0, 21, 1, false, false, hpxNSelMuonsVsRunNumber);
  CreateAndWriteCanvas("cpxNAllSelMuonsVsRunNumber", 0, 21, 1, false, false, hpxNAllSelMuonsVsRunNumber);
  
  CreateAndWriteCanvas("cRecoVerticesX",0, 21, 1, false, true, hRecoVerticesX);
  CreateAndWriteCanvas("cSelVerticesX",0, 21, 1, false, true, hSelVerticesX);
  CreateAndWriteCanvas("cAllSelVerticesX",0, 21, 1, false, true, hAllSelVerticesX);
  CreateAndWriteCanvas("cRecoVerticesXError",0, 21, 1, false, true, hRecoVerticesXError);
  CreateAndWriteCanvas("cSelVerticesXError",0, 21, 1, false, true, hSelVerticesXError);
  CreateAndWriteCanvas("cAllSelVerticesXError",0, 21, 1, false, true, hAllSelVerticesXError);
  CreateAndWriteCanvas("cRecoVerticesXErrorVsRunNumber","colz", false, false, true, hRecoVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cSelVerticesXErrorVsRunNumber","colz", false, false, true, hSelVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cAllSelVerticesXErrorVsRunNumber","colz", false, false, true, hAllSelVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cpxRecoVerticesXErrorVsRunNumber",0, 21, 1, false, false, hpxRecoVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelVerticesXErrorVsRunNumber",0, 21, 1, false, false, hpxSelVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cpxAllSelVerticesXErrorVsRunNumber",0, 21, 1, false, false, hpxAllSelVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cRecoVerticesY",0, 21, 1, false, true, hRecoVerticesY);
  CreateAndWriteCanvas("cSelVerticesY",0, 21, 1, false, true, hSelVerticesY);
  CreateAndWriteCanvas("cAllSelVerticesY",0, 21, 1, false, true, hAllSelVerticesY);
  CreateAndWriteCanvas("cRecoVerticesYError",0, 21, 1, false, true, hRecoVerticesYError);
  CreateAndWriteCanvas("cSelVerticesYError",0, 21, 1, false, true, hSelVerticesYError);
  CreateAndWriteCanvas("cAllSelVerticesYError",0, 21, 1, false, true, hAllSelVerticesYError);
  CreateAndWriteCanvas("cRecoVerticesYErrorVsRunNumber","colz", false, false, true, hRecoVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cSelVerticesYErrorVsRunNumber","colz", false, false, true, hSelVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cAllSelVerticesYErrorVsRunNumber","colz", false, false, true, hAllSelVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cpxRecoVerticesYErrorVsRunNumber",0, 21, 1, false, false, hpxRecoVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelVerticesYErrorVsRunNumber",0, 21, 1, false, false, hpxSelVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cpxAllSelVerticesYErrorVsRunNumber",0, 21, 1, false, false, hpxAllSelVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cRecoVerticesZ",0, 21, 1, false, true, hRecoVerticesZ);
  CreateAndWriteCanvas("cSelVerticesZ",0, 21, 1, false, true, hSelVerticesZ);
  CreateAndWriteCanvas("cAllSelVerticesZ",0, 21, 1, false, true, hAllSelVerticesZ);
  CreateAndWriteCanvas("cRecoVerticesZError",0, 21, 1, false, true, hRecoVerticesZError);
  CreateAndWriteCanvas("cSelVerticesZError",0, 21, 1, false, true, hSelVerticesZError);
  CreateAndWriteCanvas("cAllSelVerticesZError",0, 21, 1, false, true, hAllSelVerticesZError);
  CreateAndWriteCanvas("cRecoVerticesZErrorVsRunNumber","colz", false, false, true, hRecoVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cSelVerticesZErrorVsRunNumber","colz", false, false, true, hSelVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cAllSelVerticesZErrorVsRunNumber","colz", false, false, true, hAllSelVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cpxRecoVerticesZErrorVsRunNumber",0, 21, 1, false, false, hpxRecoVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelVerticesZErrorVsRunNumber",0, 21, 1, false, false, hpxSelVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cpxAllSelVerticesZErrorVsRunNumber",0, 21, 1, false, false, hpxAllSelVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cRecoVerticesNDOF",0, 21, 1, false, false, hRecoVerticesNDOF);
  CreateAndWriteCanvas("cSelVerticesNDOF",0, 21, 1, false, false, hSelVerticesNDOF);
  CreateAndWriteCanvas("cAllSelVerticesNDOF",0, 21, 1, false, false, hAllSelVerticesNDOF);
  CreateAndWriteCanvas("cRecoVerticesNormalizedChi2",0, 21, 1, false, false, hRecoVerticesNormalizedChi2);
  CreateAndWriteCanvas("cSelVerticesNormalizedChi2",0, 21, 1, false, false, hSelVerticesNormalizedChi2);
  CreateAndWriteCanvas("cAllSelVerticesNormalizedChi2",0, 21, 1, false, false, hAllSelVerticesNormalizedChi2);
  CreateAndWriteCanvas("cRecoVerticesTrkMultiplicity",0, 21, 1, false, false, hRecoVerticesTrkMultiplicity);
  CreateAndWriteCanvas("cSelVerticesTrkMultiplicity",0, 21, 1, false, false, hSelVerticesTrkMultiplicity);
  CreateAndWriteCanvas("cAllSelVerticesTrkMultiplicity",0, 21, 1, false, false, hAllSelVerticesTrkMultiplicity);

  CreateAndWriteCanvas("cNRecoVertices",0, 21, 1, false, true, hNRecoVertices);
  CreateAndWriteCanvas("cNSelVertices",0, 21, 1, false, true, hNSelVertices);
  CreateAndWriteCanvas("cNAllSelVertices",0, 21, 1, false, true, hNAllSelVertices);
  CreateAndWriteCanvas("cNRecoVerticesVsRunNumber", "colz", false, false, true, hNRecoVerticesVsRunNumber);
  CreateAndWriteCanvas("cNSelVerticesVsRunNumber", "colz", false, false, true, hNSelVerticesVsRunNumber);
  CreateAndWriteCanvas("cNAllSelVerticesVsRunNumber", "colz", false, false, true, hNAllSelVerticesVsRunNumber);
  CreateAndWriteCanvas("cpxNRecoVerticesVsRunNumber", 0, 21, 1, false, false, hpxNRecoVerticesVsRunNumber);
  CreateAndWriteCanvas("cpxNSelVerticesVsRunNumber", 0, 21, 1, false, false, hpxNSelVerticesVsRunNumber);
  CreateAndWriteCanvas("cpxNAllSelVerticesVsRunNumber", 0, 21, 1, false, false, hpxNAllSelVerticesVsRunNumber);

  if(s->JetSelectorIsValid())
  {
    CreateAndWriteCanvas("cRecoJetPt",0, 21, 1, false, true, hRecoJetPt);
    CreateAndWriteCanvas("cSelJetPt",0, 21, 1, false, true, hSelJetPt);
    CreateAndWriteCanvas("cAllSelJetPt",0, 21, 1, false, true, hAllSelJetPt);
    CreateAndWriteCanvas("cSelJetLeadingPt",0, 21, 1, false, true, hSelJetLeadingPt);
    CreateAndWriteCanvas("cSelJetSecondPt",0, 21, 1, false, true, hSelJetSecondPt);
    CreateAndWriteCanvas("cRecoJetEta",0, 21, 1, false, true, hRecoJetEta);
    CreateAndWriteCanvas("cSelJetEta",0, 21, 1, false, true, hSelJetEta);
    CreateAndWriteCanvas("cAllSelJetEta",0, 21, 1, false, true, hAllSelJetEta);
    CreateAndWriteCanvas("cSelJetLeadingEta",0, 21, 1, false, true, hSelJetLeadingEta);
    CreateAndWriteCanvas("cSelJetSecondEta",0, 21, 1, false, true, hSelJetSecondEta);
    CreateAndWriteCanvas("cRecoJetPhi",0, 21, 1, false, false, hRecoJetPhi);
    CreateAndWriteCanvas("cSelJetPhi",0, 21, 1, false, false, hSelJetPhi);
    CreateAndWriteCanvas("cAllSelJetPhi",0, 21, 1, false, false, hAllSelJetPhi);
    CreateAndWriteCanvas("cRecoJetEcfr",0, 21, 1, false, true, hRecoJetEcfr);
    CreateAndWriteCanvas("cSelJetEcfr",0, 21, 1, false, true, hSelJetEcfr);
    CreateAndWriteCanvas("cAllSelJetEcfr",0, 21, 1, false, true, hAllSelJetEcfr);
    CreateAndWriteCanvas("cRecoJetEnfr",0, 21, 1, false, true, hRecoJetEnfr);
    CreateAndWriteCanvas("cSelJetEnfr",0, 21, 1, false, true, hSelJetEnfr);
    CreateAndWriteCanvas("cAllSelJetEnfr",0, 21, 1, false, true, hAllSelJetEnfr);
    CreateAndWriteCanvas("cRecoJetHcfr",0, 21, 1, false, true, hRecoJetHcfr);
    CreateAndWriteCanvas("cSelJetHcfr",0, 21, 1, false, true, hSelJetHcfr);
    CreateAndWriteCanvas("cAllSelJetHcfr",0, 21, 1, false, true, hAllSelJetHcfr);
    CreateAndWriteCanvas("cRecoJetHnfr",0, 21, 1, false, true, hRecoJetHnfr);
    CreateAndWriteCanvas("cSelJetHnfr",0, 21, 1, false, true, hSelJetHnfr);
    CreateAndWriteCanvas("cAllSelJetHnfr",0, 21, 1, false, true, hAllSelJetHnfr);
    CreateAndWriteCanvas("cRecoJetMass",0, 21, 1, false, true, hRecoJetMass);
    CreateAndWriteCanvas("cSelJetMass",0, 21, 1, false, true, hSelJetMass);
    CreateAndWriteCanvas("cAllSelJetMass",0, 21, 1, false, true, hAllSelJetMass);
    CreateAndWriteCanvas("cRecoJetNConstituents",0, 21, 1, false, true, hRecoJetNConstituents);
    CreateAndWriteCanvas("cSelJetNConstituents",0, 21, 1, false, true, hSelJetNConstituents);
    CreateAndWriteCanvas("cAllSelJetNConstituents",0, 21, 1, false, true, hAllSelJetNConstituents);
    CreateAndWriteCanvas("cRecoJetChargedMultiplicity",0, 21, 1, false, true, hRecoJetChargedMultiplicity);
    CreateAndWriteCanvas("cSelJetChargedMultiplicity",0, 21, 1, false, true, hSelJetChargedMultiplicity);
    CreateAndWriteCanvas("cAllSelJetChargedMultiplicity",0, 21, 1, false, true, hAllSelJetChargedMultiplicity);
    CreateAndWriteCanvas("cRecoJetTrackMultiplicity",0, 21, 1, false, true, hRecoJetTrackMultiplicity);
    CreateAndWriteCanvas("cSelJetTrackMultiplicity",0, 21, 1, false, true, hSelJetTrackMultiplicity);
    CreateAndWriteCanvas("cAllSelJetTrackMultiplicity",0, 21, 1, false, true, hAllSelJetTrackMultiplicity);
    CreateAndWriteCanvas("cRecoJetNSecondaryVertex",0, 21, 1, false, true, hRecoJetNSecondaryVertex);
    CreateAndWriteCanvas("cSelJetNSecondaryVertex",0, 21, 1, false, true, hSelJetNSecondaryVertex);
    CreateAndWriteCanvas("cAllSelJetNSecondaryVertex",0, 21, 1, false, true, hAllSelJetNSecondaryVertex);
    CreateAndWriteCanvas("cSelJetFractionTracksAssociatedToVertex",0, 21, 1, false, false, hSelJetFractionTracksAssociatedToVertex);
    CreateAndWriteCanvas("cSelJetWeightedFractionTracksAssociatedToVertex",0, 21, 1, false, false, hSelJetWeightedFractionTracksAssociatedToVertex);
    CreateAndWriteCanvas("cSelJetTrackDzWrtSelectedVertex",0, 21, 1, false, true, hSelJetTrackDzWrtSelectedVertex);
    CreateAndWriteCanvas("cSelJetWeightedTrackDzWrtSelectedVertex",0, 21, 1, false, true, hSelJetWeightedTrackDzWrtSelectedVertex);
    CreateAndWriteCanvas("cSelJetAverageTrackDzWrtSelectedVertex",0, 21, 1, false, true, hSelJetAverageTrackDzWrtSelectedVertex);
    CreateAndWriteCanvas("cSelJetWeightedAverageTrackDzWrtSelectedVertex",0, 21, 1, false, true, hSelJetWeightedAverageTrackDzWrtSelectedVertex);
    CreateAndWriteCanvas("cSelJetRMSTrackDzWrtSelectedVertex",0, 21, 1, false, true, hSelJetRMSTrackDzWrtSelectedVertex);
    CreateAndWriteCanvas("cSelJetWeightedRMSTrackDzWrtSelectedVertex",0, 21, 1, false, true, hSelJetWeightedRMSTrackDzWrtSelectedVertex);
  }
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // MuonBasicDistributionsData_cxx
