#ifndef MuonBasicDistributionsDataForBmmWithCuts_cxx
#define MuonBasicDistributionsDataForBmmWithCuts_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "MuonBasicDistributionsDataForBmmWithCuts.h"
#include "../utilities/constants.h"



void MuonBasicDistributionsDataForBmmWithCuts::Loop(const double LowPtCut_, const double HighPtCut_, const double LowAbsEtaCut_, const double HighAbsEtaCut_, const int SelectChargeCut_)
{
  if (fChain == 0) return;
  
  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

   // Genparticle to look at for the muon truth matching
  int trueGenpId = 0;
  if(sampleName.find("KK") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B*ToKK sample: only reco muons matched to generated kaons will be selected.\n";
    trueGenpId = 321;    
  }
  else if(sampleName.find("PiPi") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B*ToPiPi sample: only reco muons matched to generated pions will be selected.\n";
    trueGenpId = 211;
  }
  else if(sampleName.find("MuMu") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B*ToMuMu sample: only reco muons matched to generated muons will be selected.\n";
    trueGenpId = 13;
  }
  else
  {
    std::cout << "W A R N I N G ! Could not infer the sample type from the name!\n";
    std::cout << "                MC matching of reco muons is disabled!\n";
  }

  int trueBId = 0;
  if(sampleName.find("BsTo") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B_s* sample: only gen particles coming from a B_s mom will be selected.\n";
    trueBId = 531;
    
  }
  else if(sampleName.find("BdTo") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B_d* sample: only gen particles coming from a B_d mom will be selected.\n";
    trueBId = 511;
  }
  else
  {
    std::cout << "W A R N I N G ! Could not infer the sample type from the name!\n";
    std::cout << "                MC matching of reco muons is disabled!\n";
  }
  
  std::cout << "I N F O : Using the following cuts.\n";
  std::cout << "- MuonPt > " << LowPtCut_ << std::endl
            << "- MuonPt < " << HighPtCut_ << std::endl
            << "- |MuonEta| > "  << LowAbsEtaCut_ << std::endl
            << "- |MuonEta| < "  << HighAbsEtaCut_ << std::endl
            << "- MuonCharge = " << SelectChargeCut_ << " (0 means no specific charge selected)"<< std::endl;
            
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

  // This is the one that makes more sense for data
  const std::string effMethod = "MCTruthBB";
  
  const char* nMuLabel = "N_{#mu}";
  const char* nVtxLabel = "N_{vtx}";
  const char* nEvLabel = "N_{ev}";
  
  // Book histograms here
  TH1D* hSelMuonDeltaRFromGenp = Create1DHistogram<TH1D>("hSelMuonDeltaRFromGenp","#Delta(R) of gen particle from closest matching selected muon", 100, 0., 1., "#Delta(R)", nMuLabel);
  
  TH1D* hRecoMuonPt = Create1DHistogram<TH1D>("hRecoMuonPt","p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  TH1D* hSelMuonPt = Create1DHistogram<TH1D>("hSelMuonPt","p_{T} of selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  
  TH1D* hSelMuonLeadingPt = Create1DHistogram<TH1D>("hSelMuonLeadingPt","p_{T} of leading selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  TH1D* hSelMuonSecondPt = Create1DHistogram<TH1D>("hSelMuonSecondPt","p_{T} of 2nd leading selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel);
  
  TH1D* hRecoMuonEta = Create1DHistogram<TH1D>("hRecoMuonEta","#eta of reconstructed muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hSelMuonEta = Create1DHistogram<TH1D>("hSelMuonEta","#eta of selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);

  TH1D* hSelMuonLeadingEta = Create1DHistogram<TH1D>("hSelMuonLeadingEta","#eta of leading-p_{T} selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hSelMuonSecondEta = Create1DHistogram<TH1D>("hSelMuonSecondEta","#eta of second leading-p_{T} selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  
  TH1D* hRecoMuonPhi = Create1DHistogram<TH1D>("hRecoMuonPhi","#phi of reconstructed muons", nPhiBins, phiMin, phiMax, recoPhiLabel, nMuLabel);
  TH1D* hSelMuonPhi = Create1DHistogram<TH1D>("hSelMuonPhi","#phi of selected muons", nPhiBins, phiMin, phiMax, recoPhiLabel, nMuLabel);
  
  TH1D* hRecoMuonCharge = Create1DHistogram<TH1D>("hRecoMuonCharge","Charge of reconstructed muons", 3, -1.5, 1.5, "Charge", nMuLabel);
  TH1D* hSelMuonCharge = Create1DHistogram<TH1D>("hSelMuonCharge","Charge of selected muons", 3, -1.5, 1.5, "Charge", nMuLabel);
  
  TH1D* hRecoMuonHasTriggered = Create1DHistogram<TH1D>("hRecoMuonHasTriggered","Trigger matching of reconstructed muons", 2, -0.5, 1.5, "Has triggered", nMuLabel);
  TH1D* hSelMuonHasTriggered = Create1DHistogram<TH1D>("hSelMuonHasTriggered","Trigger matching of selected muons", 2, -0.5, 1.5, "Has triggered", nMuLabel);
  
  TH1D* hRecoMuonCaloCompatibility = Create1DHistogram<TH1D>("hRecoMuonCaloCompatibility","Calo compatibility of reconstructed muons", 25, 0., 1., "Calo compatibility", nMuLabel);
  TH1D* hSelMuonCaloCompatibility = Create1DHistogram<TH1D>("hSelMuonCaloCompatibility","Calo compatibility of selected muons", 25, 0., 1., "Calo compatibility", nMuLabel);
  
  TH1D* hRecoMuonSegmentCompatibility = Create1DHistogram<TH1D>("hRecoMuonSegmentCompatibility","Segment compatibility of reconstructed muons", 100, 0., 1., "Segment compatibility", nMuLabel);
  TH1D* hSelMuonSegmentCompatibility = Create1DHistogram<TH1D>("hSelMuonSegmentCompatibility","Segment compatibility of selected muons", 100, 0., 1., "Segment compatibility", nMuLabel);
  
  TH1D* hRecoMuonTrackValidHits = Create1DHistogram<TH1D>("hRecoMuonTrackValidHits","N. of valid hits in inner track of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonTrackValidHits = Create1DHistogram<TH1D>("hSelMuonTrackValidHits","N. of valid hits in inner track of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  
  TH1D* hRecoMuonTrackChi2 = Create1DHistogram<TH1D>("hRecoMuonTrackChi2","#chi^{2}/ndof in inner track of reconstructed muons", 50, 0., 5., "#chi^{2}/ndof", nMuLabel);
  TH1D* hSelMuonTrackChi2 = Create1DHistogram<TH1D>("hSelMuonTrackChi2","#chi^{2}/ndof in inner track of selected muons", 50, 0., 5., "#chi^{2}/ndof", nMuLabel);

  TH1D* hRecoMuonTrackNdof = Create1DHistogram<TH1D>("hRecoMuonTrackNdof","ndof in inner track of reconstructed muons", 51, -0.5, 50.5, "ndof", nMuLabel);
  TH1D* hSelMuonTrackNdof = Create1DHistogram<TH1D>("hSelMuonTrackNdof","ndof in inner track of selected muons", 51, -0.5, 50.5, "ndof", nMuLabel);

  TH1D* hRecoMuonTrackLayersWithMeasurement = Create1DHistogram<TH1D>("hRecoMuonTrackLayersWithMeasurement","Layers with measurement in inner track of reconstructed muons", 21, -0.5, 20.5, "N_{layers}", nMuLabel);
  TH1D* hSelMuonTrackLayersWithMeasurement = Create1DHistogram<TH1D>("hSelMuonTrackLayersWithMeasurement","Layers with measurement in inner track of selected muons", 21, -0.5, 20.5, "N_{layers}", nMuLabel);
  
  TH1D* hRecoMuonTrackValidPixelHits = Create1DHistogram<TH1D>("hRecoMuonTrackValidPixelHits","N. of valid pixel hits in inner track of reconstructed muons", 11, -0.5, 10.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonTrackValidPixelHits = Create1DHistogram<TH1D>("hSelMuonTrackValidPixelHits","N. of valid pixel hits in inner track of selected muons", 11, -0.5, 10.5, "N_{hits}", nMuLabel);

  TH1D* hRecoMuonGlobalChi2 = Create1DHistogram<TH1D>("hRecoMuonGlobalChi2","#chi^{2}/ndof in global track of reconstructed muons", 100, 0., 10., "#chi^{2}/ndof", nMuLabel);
  TH1D* hSelMuonGlobalChi2 = Create1DHistogram<TH1D>("hSelMuonGlobalChi2","#chi^{2}/ndof in global track of selected muons", 100, 0., 10., "#chi^{2}/ndof", nMuLabel);

  TH1D* hRecoMuonGlobalValidMuonHits = Create1DHistogram<TH1D>("hRecoMuonGlobalValidMuonHits","N. of valid muon hits in global track of reconstructed muons", 51, -0.5, 50.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonGlobalValidMuonHits = Create1DHistogram<TH1D>("hSelMuonGlobalValidMuonHits","N. of valid muon hits in global track of selected muons", 51, -0.5, 50.5, "N_{hits}", nMuLabel);

  TH1D* hRecoMuonNumberOfMatches = Create1DHistogram<TH1D>("hRecoMuonNumberOfMatches","N. of matches of reconstructed muons", 11, -0.5, 10.5, "N_{matches}", nMuLabel);
  TH1D* hSelMuonNumberOfMatches = Create1DHistogram<TH1D>("hSelMuonNumberOfMatches","N. of matches of selected muons", 11, -0.5, 10.5, "N_{matches}", nMuLabel);

  TH1D* hRecoMuonNumberOfMatchedStations = Create1DHistogram<TH1D>("hRecoMuonNumberOfMatchedStations","Number of matched stations of reconstructed muons", 11, -0.5, 10.5, "N_{stations}", nMuLabel);
  TH1D* hSelMuonNumberOfMatchedStations = Create1DHistogram<TH1D>("hSelMuonNumberOfMatchedStations","Number of matched stations of selected muons", 11, -0.5, 10.5, "N_{stations}", nMuLabel);
  
  TH1D* hRecoMuonStationsWithHits = Create1DHistogram<TH1D>("hRecoMuonStationsWithHits","Stations with hits of reconstructed muons", 8, 0.5, 8.5, "Station", nMuLabel);
  TH1D* hSelMuonStationsWithHits = Create1DHistogram<TH1D>("hSelMuonStationsWithHits","Stations with hits of selected muons", 8, 0.5, 8.5, "Station", nMuLabel);
  TH1D* hRecoMuonDTStationsWithHits = Create1DHistogram<TH1D>("hRecoMuonDTStationsWithHits","DT stations with hits of reconstructed muons", 4, 0.5, 4.5, "DT Station", nMuLabel);
  TH1D* hSelMuonDTStationsWithHits = Create1DHistogram<TH1D>("hSelMuonDTStationsWithHits","DT stations with hits of selected muons", 4, 0.5, 4.5, "DT Station", nMuLabel);
  TH1D* hRecoMuonCSCStationsWithHits = Create1DHistogram<TH1D>("hRecoMuonCSCStationsWithHits","CSC stations with hits of reconstructed muons", 4, 0.5, 4.5, "CSC Station", nMuLabel);
  TH1D* hSelMuonCSCStationsWithHits = Create1DHistogram<TH1D>("hSelMuonCSCStationsWithHits","CSC stations with hits of selected muons", 4, 0.5, 4.5, "CSC Station", nMuLabel);
  
  TH1D* hRecoMuonStationsWithHitsMerged = Create1DHistogram<TH1D>("hRecoMuonStationsWithHitsMerged","Stations with hits of reconstructed muons (DT and CSC merged)", 4, 0.5, 4.5, "Station", nMuLabel);
  TH1D* hSelMuonStationsWithHitsMerged = Create1DHistogram<TH1D>("hSelMuonStationsWithHitsMerged","Stations with hits of selected muons (DT and CSC merged)", 4, 0.5, 4.5, "Station", nMuLabel);
  
  TH1D* hRecoMuonKink = Create1DHistogram<TH1D>("hRecoMuonKink","Inner track kink of reconstructed muons", 100, 0., 50., "Kink value", nMuLabel);
  TH1D* hSelMuonKink = Create1DHistogram<TH1D>("hSelMuonKink","Inner track kink of selected muons", 100, 0., 50., "Kink value", nMuLabel);
  TH1D* hRecoMuonKinkRho = Create1DHistogram<TH1D>("hRecoMuonKinkRho","#rho of inner track kink of reconstructed muons", 100, 0., 100., "Kink #rho", nMuLabel);
  TH1D* hSelMuonKinkRho = Create1DHistogram<TH1D>("hSelMuonKinkRho","#rho of inner track kink of selected muons", 100, 0., 100., "Kink #rho", nMuLabel);
  TH1D* hRecoMuonKinkZ = Create1DHistogram<TH1D>("hRecoMuonKinkZ","Z of inner track kink of reconstructed muons", 100, -300., 300., "Kink Z", nMuLabel);
  TH1D* hSelMuonKinkZ = Create1DHistogram<TH1D>("hSelMuonKinkZ","Z of inner track kink of selected muons", 100, -300., 300., "Kink Z", nMuLabel);
  
  TH1D* hRecoMuonGlobalKink = Create1DHistogram<TH1D>("hRecoMuonGlobalKink","Global track kink of reconstructed muons", 100, 0., 100000., "Kink value", nMuLabel);
  TH1D* hSelMuonGlobalKink = Create1DHistogram<TH1D>("hSelMuonGlobalKink","Global track kink of selected muons", 100, 0., 100000., "Kink value", nMuLabel);
  TH1D* hRecoMuonGlobalKinkRho = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkRho","#rho of global track kink of reconstructed muons", 100, 0., 100., "Kink #rho", nMuLabel);
  TH1D* hSelMuonGlobalKinkRho = Create1DHistogram<TH1D>("hSelMuonGlobalKinkRho","#rho of global track kink of selected muons", 100, 0., 100., "Kink #rho", nMuLabel);
  TH1D* hRecoMuonGlobalKinkZ = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkZ","Z of global track kink of reconstructed muons", 100, -300., 300., "Kink Z", nMuLabel);
  TH1D* hSelMuonGlobalKinkZ = Create1DHistogram<TH1D>("hSelMuonGlobalKinkZ","Z of global track kink of selected muons", 100, -300., 300., "Kink Z", nMuLabel);
  
  TH1D* hRecoMuonGlobalKinkChi2LocalMomentum = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkChi2LocalMomentum","#chi^{2} of local momenta of STA-TK match of reconstructed muons", 100, 0., 50., "Local p #chi^{2}", nMuLabel);
  TH1D* hSelMuonGlobalKinkChi2LocalMomentum = Create1DHistogram<TH1D>("hSelMuonGlobalKinkChi2LocalMomentum","#chi^{2} of local momenta of STA-TK match of selected muons", 100, 0., 50., "Local p #chi^{2}", nMuLabel);
  TH1D* hRecoMuonGlobalKinkChi2LocalPosition = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkChi2LocalPosition","#chi^{2} of local positions of STA-TK match of reconstructed muons", 100, 0., 50., "Local position #chi^{2}", nMuLabel);
  TH1D* hSelMuonGlobalKinkChi2LocalPosition = Create1DHistogram<TH1D>("hSelMuonGlobalKinkChi2LocalPosition","#chi^{2} of local positions of STA-TK match of selected muons", 100, 0., 50., "Local position #chi^{2}", nMuLabel);
  
  TH2D* hRecoMuonChi2LocalMomentumVsChi2LocalPosition = Create2DHistogram<TH2D>("hRecoMuonChi2LocalMomentumVsChi2LocalPosition", "#chi^{2} of local momenta vs. #chi^{2} of local positions of STA-TK match of reconstructed muons",  100, 0., 50., 100, 0., 50., "Local position #chi^{2}", "Local p #chi^{2}");
  TH2D* hSelMuonChi2LocalMomentumVsChi2LocalPosition = Create2DHistogram<TH2D>("hSelMuonChi2LocalMomentumVsChi2LocalPosition", "#chi^{2} of local momenta vs. #chi^{2} of local positions of STA-TK match of selected muons",  100, 0., 50., 100, 0., 50., "Local position #chi^{2}", "Local p #chi^{2}");
  

  TH1D* hRecoMuonGlbTrackProbability = Create1DHistogram<TH1D>("hRecoMuonGlbTrackProbability","-ln(Probability) of global track of reconstructed muons", 100, 0., 10., "-ln(Probability)", nMuLabel);
  TH1D* hSelMuonGlbTrackProbability = Create1DHistogram<TH1D>("hSelMuonGlbTrackProbability","-ln(Probability) of global track of selected muons", 100, 0., 10., "-ln(Probability)", nMuLabel);
  TH1D* hRecoMuonGlobalKinkGlobalDeltaEtaPhi = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkGlobalDeltaEtaPhi","#Delta(#eta,#phi) of STA-TK match of reconstructed muons", 100, 0., 0.3, "#Delta(#eta,#phi)", nMuLabel);
  TH1D* hSelMuonGlobalKinkGlobalDeltaEtaPhi = Create1DHistogram<TH1D>("hSelMuonGlobalKinkGlobalDeltaEtaPhi","#Delta(#eta,#phi) of STA-TK match of selected muons", 100, 0., 0.3, "#Delta(#eta,#phi)", nMuLabel);
  TH1D* hRecoMuonGlobalKinkLocalDistance = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkLocalDistance","Local distance of STA-TK match of reconstructed muons", 100, 0., 50., "Distance [cm]", nMuLabel);
  TH1D* hSelMuonGlobalKinkLocalDistance = Create1DHistogram<TH1D>("hSelMuonGlobalKinkLocalDistance","Local distance of STA-TK match of selected muons", 100, 0., 50., "Distance [cm]", nMuLabel);
  TH1D* hRecoMuonGlobalKinkStaRelChi2 = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkStaRelChi2","#chi^{2}(STA track) over #chi^{2}(GLB track) of reconstructed muons", 100, 0., 10., "#chi^{2}(STA)/#chi^{2}(GLB)", nMuLabel);
  TH1D* hSelMuonGlobalKinkStaRelChi2 = Create1DHistogram<TH1D>("hSelMuonGlobalKinkStaRelChi2","#chi^{2}(STA track) over #chi^{2}(GLB track) of selected muons", 100, 0., 10., "#chi^{2}(STA)/#chi^{2}(GLB)", nMuLabel);
  TH1D* hRecoMuonGlobalKinkTightMatch = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkTightMatch","Tight match of STA-TK tracks of reconstructed muons", 2, -0.5, 1.5, "Tight match", nMuLabel);
  TH1D* hSelMuonGlobalKinkTightMatch = Create1DHistogram<TH1D>("hSelMuonGlobalKinkTightMatch","Tight match of STA-TK tracks of selected muons", 2, -0.5, 1.5, "Tight match", nMuLabel);
  TH1D* hRecoMuonGlobalKinkTrkRelChi2 = Create1DHistogram<TH1D>("hRecoMuonGlobalKinkTrkRelChi2","#chi^{2}(TK track) over #chi^{2}(GLB track) of reconstructed muons", 100, 0., 3., "#chi^{2}(TK)/#chi^{2}(GLB)", nMuLabel);
  TH1D* hSelMuonGlobalKinkTrkRelChi2 = Create1DHistogram<TH1D>("hSelMuonGlobalKinkTrkRelChi2","#chi^{2}(TK track) over #chi^{2}(GLB track) of selected muons", 100, 0., 3., "#chi^{2}(TK)/#chi^{2}(GLB)", nMuLabel);

  TH2D* hRecoMuonStaRelChi2VsTrkRelChi2 = Create2DHistogram<TH2D>("hRecoMuonStaRelChi2VsTrkRelChi2", "#chi^{2}(STA)/#chi^{2}(GLB) vs. #chi^{2}(TK)/#chi^{2}(GLB)  of reconstructed muons",  100, 0., 3., 100, 0., 10., "#chi^{2}(TK)/#chi^{2}(GLB)", "#chi^{2}(STA)/#chi^{2}(GLB)");
  TH2D* hSelMuonStaRelChi2VsTrkRelChi2 = Create2DHistogram<TH2D>("hSelMuonStaRelChi2VsTrkRelChi2", "#chi^{2}(STA)/#chi^{2}(GLB) vs. #chi^{2}(TK)/#chi^{2}(GLB)  of selected muons",  100, 0., 3., 100, 0., 10., "#chi^{2}(TK)/#chi^{2}(GLB)", "#chi^{2}(STA)/#chi^{2}(GLB)");
    
  TH1D* hRecoMuonEnergyEcalTime = Create1DHistogram<TH1D>("hRecoMuonEnergyEcalTime","ECAL time of reconstructed muons", 100, -100., 100., "ECAL time", nMuLabel);
  TH1D* hSelMuonEnergyEcalTime = Create1DHistogram<TH1D>("hSelMuonEnergyEcalTime","ECAL time of reconstructed muons", 100, -100., 100., "ECAL time", nMuLabel);
  TH1D* hRecoMuonEnergyEcalTimeError = Create1DHistogram<TH1D>("hRecoMuonEnergyEcalTimeError","ECAL time error of reconstructed muons", 100, 0., 1., "ECAL time", nMuLabel);
  TH1D* hSelMuonEnergyEcalTimeError = Create1DHistogram<TH1D>("hSelMuonEnergyEcalTimeError","ECAL time error of selected muons", 100, 0., 1., "ECAL time", nMuLabel);
  TH1D* hRecoMuonEnergyEm = Create1DHistogram<TH1D>("hRecoMuonEnergyEm","ECAL energy in crossed crystals of reconstructed muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyEm = Create1DHistogram<TH1D>("hSelMuonEnergyEm","ECAL energy in crossed crystals of selected muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel);
  TH1D* hRecoMuonEnergyEmMax = Create1DHistogram<TH1D>("hRecoMuonEnergyEmMax","Max ECAL energy in 5x5 crystals around reconstructed muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyEmMax = Create1DHistogram<TH1D>("hSelMuonEnergyEmMax","Max ECAL energy in 5x5 crystals around selected muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel);
  TH1D* hRecoMuonEnergyEmS25 = Create1DHistogram<TH1D>("hRecoMuonEnergyEmS25","ECAL energy deposited in 5x5 crystals around reconstructed muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyEmS25 = Create1DHistogram<TH1D>("hSelMuonEnergyEmS25","ECAL energy deposited in 5x5 crystals around selected muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel);
  TH1D* hRecoMuonEnergyEmS9 = Create1DHistogram<TH1D>("hRecoMuonEnergyEmS9","ECAL energy deposited in 3x3 crystals around reconstructed muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyEmS9 = Create1DHistogram<TH1D>("hSelMuonEnergyEmS9","ECAL energy deposited in 3x3 crystals around selected muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel);
  TH1D* hRecoMuonEnergyHcalTime = Create1DHistogram<TH1D>("hRecoMuonEnergyHcalTime","HCAL time of reconstructed muons", 100, -100., 100., "HCAL time", nMuLabel);
  TH1D* hSelMuonEnergyHcalTime = Create1DHistogram<TH1D>("hSelMuonEnergyHcalTime","HCAL time of reconstructed muons", 100, -100., 100., "HCAL time", nMuLabel);
  TH1D* hRecoMuonEnergyHcalTimeError = Create1DHistogram<TH1D>("hRecoMuonEnergyHcalTimeError","HCAL time error of reconstructed muons", 100, 0., 1., "HCAL time", nMuLabel);
  TH1D* hSelMuonEnergyHcalTimeError = Create1DHistogram<TH1D>("hSelMuonEnergyHcalTimeError","HCAL time error of selected muons", 100, 0., 1., "HCAL time", nMuLabel);
  TH1D* hRecoMuonEnergyHad = Create1DHistogram<TH1D>("hRecoMuonEnergyHad","HCAL energy in crossed towers of reconstructed muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyHad = Create1DHistogram<TH1D>("hSelMuonEnergyHad","HCAL energy in crossed towers of selected muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel);
  TH1D* hRecoMuonEnergyHadMax = Create1DHistogram<TH1D>("hRecoMuonEnergyHadMax","Max HCAL energy in 3x3 towers around reconstructed muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyHadMax = Create1DHistogram<TH1D>("hSelMuonEnergyHadMax","Max HCAL energy in 3x3 towers around selected muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel);
  TH1D* hRecoMuonEnergyHadS9 = Create1DHistogram<TH1D>("hRecoMuonEnergyHadS9","HCAL energy in 3x3 towers around reconstructed muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyHadS9 = Create1DHistogram<TH1D>("hSelMuonEnergyHadS9","HCAL energy in 3x3 towers around selected muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel);
  TH1D* hRecoMuonEnergyTower = Create1DHistogram<TH1D>("hRecoMuonEnergyTower","Total calo energy in crossed towers of reconstructed muons", 50, 0., 20., "Calo energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyTower = Create1DHistogram<TH1D>("hSelMuonEnergyTower", "Total calo energy in crossed towers of selected muons", 50, 0., 20., "Calo energy [GeV]", nMuLabel);
  TH1D* hRecoMuonEnergyTowerS9 = Create1DHistogram<TH1D>("hRecoMuonEnergyTowerS9","Total calo energy in 3x3 towers around reconstructed muons", 50, 0., 20., "Calo energy [GeV]", nMuLabel);
  TH1D* hSelMuonEnergyTowerS9 = Create1DHistogram<TH1D>("hSelMuonEnergyTowerS9", "Total calo energy in 3x3 towers around selected muons", 50, 0., 20., "Calo energy [GeV]", nMuLabel);
  
  
  TH1D* hRecoMuonIsolationR03EmEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03EmEt", "Electromagnetic #Sigma(E_{T}) in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03EmEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03EmEt", "Electromagnetic #Sigma(E_{T}) in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03EmVetoEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03EmVetoEt", "Electromagnetic #Sigma(E_{T}) in the veto cone around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03EmVetoEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03EmVetoEt", "Electromagnetic #Sigma(E_{T}) in the veto cone around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03EmMinusVetoEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03EmMinusVetoEt", "Electromagnetic #Sigma(E_{T}) in the cone of R=0.3 minus veto around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03EmMinusVetoEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03EmMinusVetoEt", "Electromagnetic #Sigma(E_{T}) in the cone of R=0.3 minus veto around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03HadEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03HadEt", "Hadronic #Sigma(E_{T}) in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03HadEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03HadEt", "Hadronic #Sigma(E_{T}) in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03HadVetoEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03HadVetoEt", "Hadronic #Sigma(E_{T}) in the veto cone around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03HadVetoEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03HadVetoEt", "Hadronic #Sigma(E_{T}) in the veto cone around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03HadMinusVetoEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03HadMinusVetoEt", "Hadronic #Sigma(E_{T}) in the cone of R=0.3 minus veto around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03HadMinusVetoEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03HadMinusVetoEt", "Hadronic #Sigma(E_{T}) in the cone of R=0.3 minus veto around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03HoEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03HoEt", "HO #Sigma(E_{T}) in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03HoEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03HoEt", "HO #Sigma(E_{T}) in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03HoVetoEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03HoVetoEt", "HO #Sigma(E_{T}) in the veto cone around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03HoVetoEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03HoVetoEt", "HO #Sigma(E_{T}) in the veto cone around selected muons", 100, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03HoMinusVetoEt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03HoMinusVetoEt", "HO #Sigma(E_{T}) in the cone of R=0.3 minus veto around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03HoMinusVetoEt = Create1DHistogram<TH1D>("hSelMuonIsolationR03HoMinusVetoEt", "HO #Sigma(E_{T}) in the cone of R=0.3 minus veto around selected muons", 100, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03NJets = Create1DHistogram<TH1D>("hRecoMuonIsolationR03NJets", "N_{jets} in a cone of R=0.3 around reconstructed muons", 6, -0.5, 5.5, "N_{jets}", nMuLabel);
  TH1D* hSelMuonIsolationR03NJets = Create1DHistogram<TH1D>("hSelMuonIsolationR03NJets", "N_{jets} in a cone of R=0.3 around selected muons", 6, -0.5, 5.5, "N_{jets}", nMuLabel);
  TH1D* hRecoMuonIsolationR03NTracks = Create1DHistogram<TH1D>("hRecoMuonIsolationR03NTracks", "N_{tracks} in a cone of R=0.3 around reconstructed muons", 11, -0.5, 10.5, "N_{tracks}", nMuLabel);
  TH1D* hSelMuonIsolationR03NTracks = Create1DHistogram<TH1D>("hSelMuonIsolationR03NTracks", "N_{tracks} in a cone of R=0.3 around selected muons", 11, -0.5, 10.5, "N_{tracks}", nMuLabel);
  TH1D* hRecoMuonIsolationR03SumPt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03SumPt", "#Sigma(p_{T}) of tracks in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03SumPt = Create1DHistogram<TH1D>("hSelMuonIsolationR03SumPt", "#Sigma(p_{T}) of tracks in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonIsolationR03TrackerVetoPt = Create1DHistogram<TH1D>("hRecoMuonIsolationR03TrackerVetoPt", "#Sigma(p_{T}) of tracks in the veto cone around reconstructed muons", 100, 0., 20., "#Sigma(p_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonIsolationR03TrackerVetoPt = Create1DHistogram<TH1D>("hSelMuonIsolationR03TrackerVetoPt", "#Sigma(p_{T}) of tracks in the veto cone around selected muons", 100, 0., 20., "#Sigma(p_{T}) [GeV]", nMuLabel);
  
  TH2D* hRecoMuonCaloEtR03VsMuonPt =  Create2DHistogram<TH2D>("hRecoMuonCaloEtR03VsMuonPt", "Calo #Sigma(E_{T}) in a cone of R=0.3 vs. p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hSelMuonCaloEtR03VsMuonPt =  Create2DHistogram<TH2D>("hSelMuonCaloEtR03VsMuonPt", "Calo #Sigma(E_{T}) in a cone of R=0.3 vs. p_{T} of selected muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hRecoMuonCaloVetoEtR03VsMuonPt =  Create2DHistogram<TH2D>("hRecoMuonCaloVetoEtR03VsMuonPt", "Calo #Sigma(E_{T}) in the veto cone vs. p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hSelMuonCaloVetoEtR03VsMuonPt =  Create2DHistogram<TH2D>("hSelMuonCaloVetoEtR03VsMuonPt", "Calo #Sigma(E_{T}) in the veto cone vs. p_{T} of selected muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hRecoMuonCaloMinusVetoEtR03VsMuonPt =  Create2DHistogram<TH2D>("hRecoMuonCaloMinusVetoEtR03VsMuonPt", "Calo #Sigma(E_{T}) in a cone of R=0.3 minus veto vs. p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hSelMuonCaloMinusVetoEtR03VsMuonPt =  Create2DHistogram<TH2D>("hSelMuonCaloMinusVetoEtR03VsMuonPt", "Calo #Sigma(E_{T}) in a cone of R=0.3 minus veto vs. p_{T} of selected muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  
  TH1D* hRecoMuonPfIsolationR03SumChargedHadronPt = Create1DHistogram<TH1D>("hRecoMuonPfIsolationR03SumChargedHadronPt","#Sigma(p_{T}) of charged hadrons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonPfIsolationR03SumChargedHadronPt = Create1DHistogram<TH1D>("hSelMuonPfIsolationR03SumChargedHadronPt","#Sigma(p_{T}) of charged hadrons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonPfIsolationR03SumChargedParticlePt = Create1DHistogram<TH1D>("hRecoMuonPfIsolationR03SumChargedParticlePt","#Sigma(p_{T}) of charged particles in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonPfIsolationR03SumChargedParticlePt = Create1DHistogram<TH1D>("hSelMuonPfIsolationR03SumChargedParticlePt","#Sigma(p_{T}) of charged particles in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel);
  
  TH1D* hRecoMuonPfIsolationR03SumNeutralHadronEt = Create1DHistogram<TH1D>("hRecoMuonPfIsolationR03SumNeutralHadronEt","#Sigma(E_{T}) of neutral hadrons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonPfIsolationR03SumNeutralHadronEt = Create1DHistogram<TH1D>("hSelMuonPfIsolationR03SumNeutralHadronEt","#Sigma(E_{T}) of neutral hadrons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold = Create1DHistogram<TH1D>("hRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold","#Sigma(E_{T}) (high threshold) of neutral hadrons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold = Create1DHistogram<TH1D>("hSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold","#Sigma(E_{T}) (high threshold) of neutral hadrons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonPfIsolationR03SumPhotonEt = Create1DHistogram<TH1D>("hRecoMuonPfIsolationR03SumPhotonEt","#Sigma(E_{T}) of photons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonPfIsolationR03SumPhotonEt = Create1DHistogram<TH1D>("hSelMuonPfIsolationR03SumPhotonEt","#Sigma(E_{T}) of photons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hRecoMuonPfIsolationR03SumPhotonEtHighThreshold = Create1DHistogram<TH1D>("hRecoMuonPfIsolationR03SumPhotonEtHighThreshold","#Sigma(E_{T}) (high threshold) of photons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonPfIsolationR03SumPhotonEtHighThreshold = Create1DHistogram<TH1D>("hSelMuonPfIsolationR03SumPhotonEtHighThreshold","#Sigma(E_{T}) (high threshold) of photons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel);
  
  TH1D* hRecoMuonPfIsolationR03SumPUPt = Create1DHistogram<TH1D>("hRecoMuonPfIsolationR03SumPUPt","#Sigma(p_{T}) of PU in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel);
  TH1D* hSelMuonPfIsolationR03SumPUPt = Create1DHistogram<TH1D>("hSelMuonPfIsolationR03SumPUPt","#Sigma(p_{T}) of PU in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel);
  
  TH1D* hRecoMuonTrackIso = Create1DHistogram<TH1D>("hRecoMuonTrackIso","Tracker isolation of reconstructed muons", 50, 0., 10., "Isolation", nMuLabel);
  TH1D* hSelMuonTrackIso = Create1DHistogram<TH1D>("hSelMuonTrackIso","Tracker isolation of selected muons", 50, 0., 10., "Isolation", nMuLabel);
  TH1D* hRecoMuonEcalIso = Create1DHistogram<TH1D>("hRecoMuonEcalIso","ECAL isolation of reconstructed muons", 50, 0., 10., "Isolation", nMuLabel);
  TH1D* hSelMuonEcalIso = Create1DHistogram<TH1D>("hSelMuonEcalIso","ECAL isolation of selected muons", 50, 0., 10., "Isolation", nMuLabel);
  TH1D* hRecoMuonHcalIso = Create1DHistogram<TH1D>("hRecoMuonHcalIso","HCAL isolation of reconstructed muons", 50, 0., 10., "Isolation", nMuLabel);
  TH1D* hSelMuonHcalIso = Create1DHistogram<TH1D>("hSelMuonHcalIso","HCAL isolation of selected muons", 50, 0., 10., "Isolation", nMuLabel);
  TH1D* hRecoMuonCaloIso = Create1DHistogram<TH1D>("hRecoMuonCaloIso","Calo isolation of reconstructed muons", 50, 0., 10., "Isolation", nMuLabel);
  TH1D* hSelMuonCaloIso = Create1DHistogram<TH1D>("hSelMuonCaloIso","Calo isolation of selected muons", 50, 0., 10., "Isolation", nMuLabel);
  
  TH1D* hRecoMuonShowerNCorrelatedHitsInStation1 = Create1DHistogram<TH1D>("hRecoMuonShowerNCorrelatedHitsInStation1","Number of correlated hits in station 1 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonShowerNCorrelatedHitsInStation1 = Create1DHistogram<TH1D>("hSelMuonShowerNCorrelatedHitsInStation1","Number of correlated hits in station 1 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);  
  TH1D* hRecoMuonShowerNCorrelatedHitsInStation2 = Create1DHistogram<TH1D>("hRecoMuonShowerNCorrelatedHitsInStation2","Number of correlated hits in station 2 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonShowerNCorrelatedHitsInStation2 = Create1DHistogram<TH1D>("hSelMuonShowerNCorrelatedHitsInStation2","Number of correlated hits in station 2 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hRecoMuonShowerNCorrelatedHitsInStation3 = Create1DHistogram<TH1D>("hRecoMuonShowerNCorrelatedHitsInStation3","Number of correlated hits in station 3 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonShowerNCorrelatedHitsInStation3 = Create1DHistogram<TH1D>("hSelMuonShowerNCorrelatedHitsInStation3","Number of correlated hits in station 3 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hRecoMuonShowerNCorrelatedHitsInStation4 = Create1DHistogram<TH1D>("hRecoMuonShowerNCorrelatedHitsInStation4","Number of correlated hits in station 4 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonShowerNCorrelatedHitsInStation4 = Create1DHistogram<TH1D>("hSelMuonShowerNCorrelatedHitsInStation4","Number of correlated hits in station 4 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);

  TH1D* hRecoMuonShowerNHitsInStation1 = Create1DHistogram<TH1D>("hRecoMuonShowerNHitsInStation1","Number of hits in station 1 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonShowerNHitsInStation1 = Create1DHistogram<TH1D>("hSelMuonShowerNHitsInStation1","Number of hits in station 1 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);  
  TH1D* hRecoMuonShowerNHitsInStation2 = Create1DHistogram<TH1D>("hRecoMuonShowerNHitsInStation2","Number of hits in station 2 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonShowerNHitsInStation2 = Create1DHistogram<TH1D>("hSelMuonShowerNHitsInStation2","Number of hits in station 2 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hRecoMuonShowerNHitsInStation3 = Create1DHistogram<TH1D>("hRecoMuonShowerNHitsInStation3","Number of hits in station 3 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonShowerNHitsInStation3 = Create1DHistogram<TH1D>("hSelMuonShowerNHitsInStation3","Number of hits in station 3 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hRecoMuonShowerNHitsInStation4 = Create1DHistogram<TH1D>("hRecoMuonShowerNHitsInStation4","Number of hits in station 4 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  TH1D* hSelMuonShowerNHitsInStation4 = Create1DHistogram<TH1D>("hSelMuonShowerNHitsInStation4","Number of hits in station 4 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel);
  
  /*
  TH1D* hRecoMuon = Create1DHistogram<TH1D>("hRecoMuon","of reconstructed muons", 11, -0.5, 10.5, "", nMuLabel);
  TH1D* hSelMuon = Create1DHistogram<TH1D>("hSelMuon","of selected muons", 11, -0.5, 10.5, "", nMuLabel);
  */
  
  
  TH1D* hSelMuonDxy = Create1DHistogram<TH1D>("hSelMuonDxy", "d_{xy} of selected muons w.r.t. the selected vertex", 100, 0., 0.5, "d_{xy} [cm]", nMuLabel);
  TH1D* hSelMuonDxyError = Create1DHistogram<TH1D>("hSelMuonDxyError", "#sigma(d_{xy}) of selected muons w.r.t. the selected vertex", 100, 0., 0.02, "#sigma(d_{xy}) [cm]", nMuLabel);
  TH1D* hSelMuonDxyz = Create1DHistogram<TH1D>("hSelMuonDxyz", "d_{xyz} of selected muons w.r.t. the selected vertex", 100, 0., 0.5, "d_{xyz} [cm]", nMuLabel);
  TH1D* hSelMuonDxyzError = Create1DHistogram<TH1D>("hSelMuonDxyzError", "#sigma(d_{xyz}) of selected muons w.r.t. the selected vertex", 100, 0., 0.02, "#sigma(d_{xyz}) [cm]", nMuLabel);
  TH1D* hSelMuonDz = Create1DHistogram<TH1D>("hSelMuonDz", "d_{z} of selected muons w.r.t. the selected vertex", 200, -0.5, 0.5, "d_{z} [cm]", nMuLabel);
  TH1D* hSelMuonAbsDz = Create1DHistogram<TH1D>("hSelMuonAbsDz", "abs(d_{z}) of selected muons w.r.t. the selected vertex", 100, 0., 0.5, "abs(d_{z}) [cm]", nMuLabel);
  TH1D* hSelMuonDzError = Create1DHistogram<TH1D>("hSelMuonDzError", "#sigma(d_{z}) of selected muons w.r.t. the selected vertex", 100, 0., 0.01, "#sigma(d_{z}) [cm]", nMuLabel);
  TH1D* hSelMuonSignedDxyBS = Create1DHistogram<TH1D>("hSelMuonDxyBS", "Signed d_{xy} of selected muons w.r.t. the beam spot", 200, -0.5, 0.5, "d_{xy} [cm]", nMuLabel);
  TH1D* hSelMuonDxyBSError = Create1DHistogram<TH1D>("hSelMuonSignedDxyBSError", "#sigma(d_{xy}) of selected muons w.r.t. the beam spot", 100, 0., 0.02, "#sigma(d_{xy}) [cm]", nMuLabel);
  TH2D* hSelMuonSignedDxyBSVsPhi = Create2DHistogram<TH2D>("hSelMuonSignedDxyBSVsPhi", "Signed d_{xy} of selected muons w.r.t. the beam spot vs. #varphi", 100, -constants::PI, constants::PI, 2000, -0.5, 0.5, "#varphi_{#mu}", "d_{xy} [cm]");
  TH2D* hSelMuonDxyBSErrorVsPhi = Create2DHistogram<TH2D>("hSelMuonDxyBSErrorVsPhi", "#sigma(d_{xy}) of selected muons w.r.t. the beam spot vs. #varphi", 100, -constants::PI, constants::PI, 100, 0., 0.02, "#varphi_{#mu}", "#sigma(d_{xy}) [cm]");
  TH2D* hSelMuonDxyVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDxyVsRunNumber", "d_{xy} of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "d_{xy} [cm]");
  TH2D* hSelMuonDxyErrorVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDxyErrorVsRunNumber", "#sigma(d_{xy}) of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 100, 0., 0.02, "Run", "#sigma(d_{xy}) [cm]");
  TH2D* hSelMuonDxyzVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDxyzVsRunNumber", "d_{xyz} of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "d_{xyz} [cm]");
  TH2D* hSelMuonDxyzErrorVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDxyzErrorVsRunNumber", "#sigma(d_{xyz}) of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 100, 0., 0.02, "Run", "#sigma(d_{xyz}) [cm]");
  TH2D* hSelMuonDzVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDzVsRunNumber", "d_{z} of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 1000, -0.5, 0.5, "Run", "d_{z} [cm]");
  TH2D* hSelMuonAbsDzVsRunNumber = Create2DHistogram<TH2D>("hSelMuonAbsDzVsRunNumber", "abs(d_{z}) of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 500, 0., 0.5, "Run", "abs(d_{z}) [cm]");
  TH2D* hSelMuonDzErrorVsRunNumber = Create2DHistogram<TH2D>("hSelMuonDzErrorVsRunNumber", "#sigma(d_{z}) of selected muons w.r.t. the selected vertex vs. run number", 260, 147140.5, 149740.5, 100, 0., 0.01, "Run", "#sigma(d_{z}) [cm]");
  TH1D* hRecoDiMuonInvMass = Create1DHistogram<TH1D>("hRecoDiMuonInvMass", "Invariant mass of the reconstructed di-muons", 100,3.,8.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelDiMuonInvMass = Create1DHistogram<TH1D>("hSelDiMuonInvMass", "Invariant mass of the selected di-muons", 100,3.,8.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");

  TH1D* hRecoDiMuonDeltaR = Create1DHistogram<TH1D>("hRecoDiMuonDeltaR", "#Delta R between the reconstructed di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelDiMuonDeltaR = Create1DHistogram<TH1D>("hSelDiMuonDeltaR", "#Delta R between the selected di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hRecoDiMuonDeltaEta = Create1DHistogram<TH1D>("hRecoDiMuonDeltaEta", "#Delta #eta between the reconstructed di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelDiMuonDeltaEta", "#Delta #eta between the selected di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hRecoDiMuonDeltaPhi = Create1DHistogram<TH1D>("hRecoDiMuonDeltaPhi", "#Delta #phi between the reconstructed di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelDiMuonDeltaPhi", "#Delta #phi between the selected di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH2D* hRecoDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hRecoDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the reconstructed di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hRecoDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hRecoDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the reconstructed di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  
  TH1D* hRecoDSDiMuonInvMass = Create1DHistogram<TH1D>("hRecoDSDiMuonInvMass", "Invariant mass of the reconstructed DS di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelDSDiMuonInvMass = Create1DHistogram<TH1D>("hSelDSDiMuonInvMass", "Invariant mass of the selected DS di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hRecoDSDiMuonDeltaR = Create1DHistogram<TH1D>("hRecoDSDiMuonDeltaR", "#Delta R between the reconstructed DS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelDSDiMuonDeltaR = Create1DHistogram<TH1D>("hSelDSDiMuonDeltaR", "#Delta R between the selected DS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hRecoDSDiMuonDeltaEta = Create1DHistogram<TH1D>("hRecoDSDiMuonDeltaEta", "#Delta #eta between the reconstructed DS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelDSDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelDSDiMuonDeltaEta", "#Delta #eta between the selected DS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hRecoDSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hRecoDSDiMuonDeltaPhi", "#Delta #phi between the reconstructed DS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelDSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelDSDiMuonDeltaPhi", "#Delta #phi between the selected DS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH2D* hRecoDSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hRecoDSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the reconstructed DS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelDSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelDSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected DS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hRecoDSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hRecoDSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the reconstructed DS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelDSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelDSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected DS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  
  TH1D* hRecoSSDiMuonInvMass = Create1DHistogram<TH1D>("hRecoSSDiMuonInvMass", "Invariant mass of the reconstructed SS di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  TH1D* hSelSSDiMuonInvMass = Create1DHistogram<TH1D>("hSelSSDiMuonInvMass", "Invariant mass of the selected SS di-muons", 500,0.,100.,"M_{#mu #mu} [GeV/c^{2}]","N_{#mu #mu}");
  
  TH1D* hRecoSSDiMuonDeltaR = Create1DHistogram<TH1D>("hRecoSSDiMuonDeltaR", "#Delta R between the reconstructed SS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hSelSSDiMuonDeltaR = Create1DHistogram<TH1D>("hSelSSDiMuonDeltaR", "#Delta R between the selected SS di-muons", 500,0.,10.,"#Delta R","N_{#mu #mu}");
  TH1D* hRecoSSDiMuonDeltaEta = Create1DHistogram<TH1D>("hRecoSSDiMuonDeltaEta", "#Delta #eta between the reconstructed SS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hSelSSDiMuonDeltaEta = Create1DHistogram<TH1D>("hSelSSDiMuonDeltaEta", "#Delta #eta between the selected SS di-muons", 500,-5.,5.,"#Delta #eta","N_{#mu #mu}");
  TH1D* hRecoSSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hRecoSSDiMuonDeltaPhi", "#Delta #phi between the reconstructed SS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH1D* hSelSSDiMuonDeltaPhi = Create1DHistogram<TH1D>("hSelSSDiMuonDeltaPhi", "#Delta #phi between the selected SS di-muons", 500,-0.5*constants::PI,1.5*constants::PI,"#Delta #phi","N_{#mu #mu}");
  TH2D* hRecoSSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hRecoSSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the reconstructed SS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hSelSSDiMuonDeltaEtaVsDeltaPhi =  Create2DHistogram<TH2D>("hSelSSDiMuonDeltaEtaVsDeltaPhi", "#Delta #eta vs. #Delta #phi between the selected SS di-muons", 500, -0.5*constants::PI,1.5*constants::PI, 500,-5.,5., "#Delta #phi", "#Delta #eta");
  TH2D* hRecoSSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hRecoSSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the reconstructed SS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  TH2D* hSelSSDiMuonInvMassVsDeltaR =  Create2DHistogram<TH2D>("hSelSSDiMuonInvMassVsDeltaR", "Invariant mass vs. #Delta R between the selected SS di-muons", 500, 0.,10., 500,0.,100., "#Delta R", "M_{#mu #mu} [GeV/c^{2}]");
  
  TH1D* hNRecoMuons =  Create1DHistogram<TH1D>("hNRecoMuons", "Number of reconstructed muons", 21,-0.5,20.5,nMuLabel,nEvLabel);
  TH1D* hNSelMuons =  Create1DHistogram<TH1D>("hNSelMuons", "Number of selected muons", 21,-0.5,20.5,nMuLabel,nEvLabel);
  TH1D* hNTriggeringRecoMuons =  Create1DHistogram<TH1D>("hNTriggeringRecoMuons", "Number of triggering reconstructed muons", 11,-0.5,10.5,nMuLabel,nEvLabel);
  TH1D* hNTriggeringSelMuons =  Create1DHistogram<TH1D>("hNTriggeringSelMuons", "Number of triggering selected muons", 11,-0.5,10.5,nMuLabel,nEvLabel);
  TH2D* hNRecoMuonsVsRunNumber =  Create2DHistogram<TH2D>("hNRecoMuonsVsRunNumber", "Number of reconstructed muons vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nMuLabel);
  TH2D* hNSelMuonsVsRunNumber =  Create2DHistogram<TH2D>("hNSelMuonsVsRunNumber", "Number of selected muons vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nMuLabel);
  
  TH1D* hRecoVerticesX = Create1DHistogram<TH1D>("hRecoVerticesX", "X position of reconstructed vertices", 100,-2.,2.,"x_{PV} [cm]", nVtxLabel);
  TH1D* hSelVerticesX = Create1DHistogram<TH1D>("hSelVerticesX", "X position of selected vertices", 100,-2.,2.,"x_{PV} [cm]", nVtxLabel);
  TH1D* hRecoVerticesXError = Create1DHistogram<TH1D>("hRecoVerticesXError", "X position error of reconstructed vertices", 1000,0.,2.,"#sigma(x_{PV}) [cm]", nVtxLabel);
  TH1D* hSelVerticesXError = Create1DHistogram<TH1D>("hSelVerticesXError", "X position error of selected vertices", 50,0.,0.05,"#sigma(x_{PV}) [cm]", nVtxLabel);
  TH2D* hRecoVerticesXErrorVsRunNumber = Create2DHistogram<TH2D>("hRecoVerticesXErrorVsRunNumber", "X position error of reconstructed vertices vs. run number", 260, 147140.5, 149740.5, 1000,0.,2., "Run","#sigma(x_{PV}) [cm]");
  TH2D* hSelVerticesXErrorVsRunNumber = Create2DHistogram<TH2D>("hSelVerticesXErrorVsRunNumber", "X position error of selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(x_{PV}) [cm]");
  
  TH1D* hRecoVerticesY = Create1DHistogram<TH1D>("hRecoVerticesY", "Y position of reconstructed vertices", 100,-2.,2.,"y_{PV} [cm]", nVtxLabel);
  TH1D* hSelVerticesY = Create1DHistogram<TH1D>("hSelVerticesY", "Y position of selected vertices", 100,-2.,2.,"y_{PV} [cm]", nVtxLabel);
  TH1D* hRecoVerticesYError = Create1DHistogram<TH1D>("hRecoVerticesYError", "Y position error of reconstructed vertices", 1000,0.,2.,"#sigma(y_{PV}) [cm]", nVtxLabel);
  TH1D* hSelVerticesYError = Create1DHistogram<TH1D>("hSelVerticesYError", "Y position error of selected vertices", 50,0.,0.05,"#sigma(y_{PV}) [cm]", nVtxLabel);
  TH2D* hRecoVerticesYErrorVsRunNumber = Create2DHistogram<TH2D>("hRecoVerticesYErrorVsRunNumber", "Y position error of reconstructed vertices vs. run number", 260, 147140.5, 149740.5, 1000,0.,2., "Run","#sigma(y_{PV}) [cm]");
  TH2D* hSelVerticesYErrorVsRunNumber = Create2DHistogram<TH2D>("hSelVerticesYErrorVsRunNumber", "Y position error of selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(y_{PV}) [cm]");
  TH1D* hRecoVerticesZ = Create1DHistogram<TH1D>("hRecoVerticesZ", "Z position of reconstructed vertices", 500,-25.,25.,"z_{PV} [cm]", nVtxLabel);
  TH1D* hSelVerticesZ = Create1DHistogram<TH1D>("hSelVerticesZ", "Z position of selected vertices", 500,-25.,25.,"z_{PV} [cm]", nVtxLabel);
  TH1D* hRecoVerticesZError = Create1DHistogram<TH1D>("hRecoVerticesZError", "Z position error of reconstructed vertices", 1000,0.,2.,"#sigma(z_{PV}) [cm]", nVtxLabel);
  TH1D* hSelVerticesZError = Create1DHistogram<TH1D>("hSelVerticesZError", "Z position error of selected vertices", 50,0.,0.05,"#sigma(z_{PV}) [cm]", nVtxLabel);
  TH2D* hRecoVerticesZErrorVsRunNumber = Create2DHistogram<TH2D>("hRecoVerticesZErrorVsRunNumber", "Z position error of reconstructed vertices vs. run number", 260, 147140.5, 149740.5, 1000,0.,2., "Run","#sigma(z_{PV}) [cm]");
  TH2D* hSelVerticesZErrorVsRunNumber = Create2DHistogram<TH2D>("hSelVerticesZErrorVsRunNumber", "Z position error of selected vertices vs. run number", 260, 147140.5, 149740.5, 50,0.,0.05, "Run","#sigma(z_{PV}) [cm]");
  TH1D* hRecoVerticesNDOF = Create1DHistogram<TH1D>("hRecoVerticesNDOF", "Number of degrees of freedom of reconstructed vertices", 201,-0.5,200.5,"NDOF_{PV}", nVtxLabel);
  TH1D* hSelVerticesNDOF = Create1DHistogram<TH1D>("hSelVerticesNDOF", "Number of degrees of freedom of selected vertices", 201,-0.5,200.5,"NDOF_{PV}", nVtxLabel);
  TH1D* hRecoVerticesNormalizedChi2 = Create1DHistogram<TH1D>("hRecoVerticesNormalizedChi2", "Normalized #chi^{2} of reconstructed vertices", 100,0.,10.,"#chi^{2}/NDOF", nVtxLabel);
  TH1D* hSelVerticesNormalizedChi2 = Create1DHistogram<TH1D>("hSelVerticesNormalizedChi2", "Normalized #chi^{2} of selected vertices", 100,0.,10.,"#chi^{2}/NDOF", nVtxLabel);
  TH1D* hRecoVerticesTrkMultiplicity = Create1DHistogram<TH1D>("hRecoVerticesTrkMultiplicity", "Track multiplicity of reconstructed vertices", 151,-0.5,150.5,"N_{tracks}", nVtxLabel);
  TH1D* hSelVerticesTrkMultiplicity = Create1DHistogram<TH1D>("hSelVerticesTrkMultiplicity", "Track multiplicity of selected vertices", 151,-0.5,150.5,"N_{tracks}", nVtxLabel);
  
  TH1D* hNRecoVertices = Create1DHistogram<TH1D>("hNRecoVertices", "Number of reconstructed vertices", 21,-0.5,20.5,nVtxLabel,nEvLabel);
  TH1D* hNSelVertices = Create1DHistogram<TH1D>("hNSelVertices", "Number of selected vertices", 21,-0.5,20.5,nVtxLabel,nEvLabel);
  TH2D* hNRecoVerticesVsRunNumber = Create2DHistogram<TH2D>("hNRecoVerticesVsRunNumber", "Number of reconstructed vertices vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nVtxLabel);
  TH2D* hNSelVerticesVsRunNumber = Create2DHistogram<TH2D>("hNSelVerticesVsRunNumber", "Number of selected vertices vs. run number", 260, 147140.5, 149740.5, 21,-0.5,20.5, "Run", nVtxLabel);
  
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

//     cout << "############\n";
    
    std::vector<unsigned short> vSelectedMuons, vSelectedJets;
    unsigned short iSelectedVertex = 65535;
    // Use only default selection
    // Selection string is passed to ParticleSelector
    // since the constructor of TreeAnalyzer*
    if(!s->MuonSelectorIsValid()) 
    {
      std::cout << "E R R O R ! MuonSelector is not valid!\nExiting..." << std::endl;
      exit(1);
    }
    
//     std::cout << "Number of muons in event is " << numberOfMuon << std::endl;
    if(numberOfMuon == 0)
    {
//       std::cout << "Event rejected because it does not have reco muons!\n";
      continue;
    }
    
    // Loop over gen particles to get the kaons and pions from B decay
    std::vector<unsigned short> vParticlesFromB;
    std::map<unsigned short, std::vector<unsigned short> > mParticlesFromB;
    for(unsigned int iGenParticle = 0; iGenParticle < Genp_Id->size(); iGenParticle++)
    {
      int genpId = Genp_Id->at(iGenParticle);
      if(abs(genpId) == trueGenpId)
      {
        int iMother = Genp_vMom->at(iGenParticle).at(0);
        int motherId = Genp_Id->at(iMother);
        
/////// GEN PART MOTHER pdgId BETWEEN 500 AND 600
//         if(abs(motherId) > 500 && abs(motherId) < 600 )
//         {
//           std::cout << "Found a kaon or a pion coming from a B!\n";
//           std::cout << "I = " << iGenParticle << std::endl;
//           std::cout << "ID = " << genpId << std::endl;
//           std::cout << "iMother = " << iMother << std::endl;
//           std::cout << "motherId = " << motherId << std::endl;
//           vParticlesFromB.push_back(iGenParticle);
//         }
///////       
        
        if( abs(motherId) == trueBId )
        {
//           std::cout << "Found a kaon or a pion coming from a B_s (531) or a B_d (511)!\n";
//           std::cout << "I = " << iGenParticle << std::endl;
//           std::cout << "ID = " << genpId << std::endl;
//           std::cout << "iMother = " << iMother << std::endl;
//           std::cout << "motherId = " << motherId << std::endl;
                    
             vParticlesFromB.push_back(iGenParticle);                                        
//           mParticlesFromB[iMother].push_back(iGenParticle); --> THIS IS TO MATCH EVERY GENP WITH ITS MOM
        }
      }
      
//       if(abs(genpId) == 531)
//       {
//         std::cout << "Found a B_s!\n";
//         std::cout << "Printing daughters:\n";
//         std::vector<unsigned short> vDaus = Genp_vDau->at(iGenParticle);
//         for(unsigned int iDau = 0; iDau < vDaus.size(); iDau++)
//         {
//           unsigned int index = vDaus[iDau];
//           std::cout << "index = " << index << ", id = " << Genp_Id->at(index) << std::endl;  
//         }
//       }
    }        
 
///////// SELECT ONLY EVENTS <=> THERE ARE ONLY 2 GEN PARTICLES ASSOCIATED TO THE SAME B-MOM
//    unsigned short nPartPairs = 0;
//    for(std::map< unsigned short, std::vector<unsigned short> >::const_iterator it = mParticlesFromB.begin(); it != mParticlesFromB.end(); ++it)
//    {
//      if ( it->second.size() == 2 ) 
//      {
//        nPartPairs++;
//        vParticlesFromB = it->second;
//      }
//    }
//    
//    if(nPartPairs != 1) continue;
// 
//     cout << "*** number of GenP coming from a B and having the same mom: " << vParticlesFromB.size() << endl;
//     for(int ii= 0; ii < vParticlesFromB.size(); ii++) 
//     {
//       cout << "* GenPid: " << vParticlesFromB[ii] << endl; 
//     }
// 
/////////   
   

    if(vParticlesFromB.size() < 2)
    {
      std::cout << "E R R O R ! Size of vector of B true daughters is less than 2!\n";
      std::cout << "            Size is " << vParticlesFromB.size() << ".\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
    
    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    { 
      // Applying selection cuts for each muon... this is not the smartest way to do it...
//       std::cout << "Pt-Eta-Charge (BEFORE CUTS): " << Muon_pt->at(iMuon) << "-" << Muon_eta->at(iMuon) << "-" << Muon_charge->at(iMuon) << std::endl;
      if( Muon_pt->at(iMuon) < LowPtCut_) continue;
      if( Muon_pt->at(iMuon) > HighPtCut_) continue;
      if( fabs(Muon_eta->at(iMuon)) < LowAbsEtaCut_) continue;
      if( fabs(Muon_eta->at(iMuon)) > HighAbsEtaCut_) continue;
      if( SelectChargeCut_ && Muon_charge->at(iMuon) != SelectChargeCut_) continue;
//       std::cout << "Pt-Eta-Charge (AFTER CUTS): " << Muon_pt->at(iMuon) << "-" << Muon_eta->at(iMuon) << "-" << Muon_charge->at(iMuon) << std::endl;
      
      if( s->SelectMuon(iMuon) )
      {
        short int trackRef = Muon_trackref->at(iMuon);
        for(unsigned int iParticleFromB = 0; iParticleFromB < vParticlesFromB.size(); iParticleFromB++)
        {
          unsigned short iGenParticle = vParticlesFromB[iParticleFromB];
          std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticle(iGenParticle);
          
          if(iTrackWithDistance.first == trackRef /*&& iTrackWithDistance.second < 0.01*/)
          {
            hSelMuonDeltaRFromGenp->Fill(iTrackWithDistance.second);
            if(iTrackWithDistance.second < 0.01) // FINE DELTA R CUT
//             if(iTrackWithDistance.second < 0.1) // BROADER DELTA R CUT
            {
//               std::cout << "Inner muon track is associated to a gen particle of Id = " << Genp_Id->at(iGenParticle) << "!\n";
              vSelectedMuons.push_back(iMuon);
              break;
            }
          }
        }
      }
    }

//     std::cout << "Sono qui 1\n";
    
    
    if(numberOfMuon != 0)
    {
      if(vSelectedMuons.size() != 0)
      {
        for(unsigned int iMuon = 0; iMuon < vSelectedMuons.size(); iMuon++)
        {
          unsigned int iTrack = Muon_trackref->at(vSelectedMuons[iMuon]);
          if(iTrack != 65535 && iTrack != 2048)
          {
            iSelectedVertex = s->SelectVertexNearestToTrack(iTrack, "V1");
            break;
          }
        }
      }
      else
      {
        for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
        {
          unsigned int iTrack = Muon_trackref->at(iMuon);
          if(iTrack != 65535 && iTrack != 2048)
          {
            iSelectedVertex = s->SelectVertexNearestToTrack(iTrack, "V1");
            break;
          }
        }
      }        
    }
//     std::cout << "Sono qui 2\n";
    
    if(iSelectedVertex == 65535)
    {
//       std::cout << "Event rejected because it does not have a vertex associated to muons!\n";
      continue;
    }
//     std::cout << "Sono qui 3\n";
    
    
    unsigned int nRecoMuons = 0;
    unsigned int nSelMuons = 0;
    unsigned int nTriggeringRecoMuons = 0;
    unsigned int nTriggeringSelMuons = 0;
    
    unsigned int nRecoVertices = 0;
    unsigned int nSelVertices = 0;

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
//       std::cout << "Sono qui 4.0\n";
      
      hRecoMuonCaloCompatibility->Fill(Muon_caloCompatibility->at(iMuon));
      hRecoMuonSegmentCompatibility->Fill(Muon_segmentCompatibility->at(iMuon));
      unsigned int iTrack = Muon_trackref->at(iMuon);
      if(iTrack != 65535 && iTrack != 2048)
      {
        hRecoMuonTrackValidHits->Fill(Track_numberOfValidHits->at(iTrack));
        hRecoMuonTrackChi2->Fill(Track_normalizedChi2->at(iTrack));
        hRecoMuonTrackNdof->Fill(Track_ndof->at(iTrack));
        hRecoMuonTrackLayersWithMeasurement->Fill(Track_layersWithMeasurement->at(iTrack));
        hRecoMuonTrackValidPixelHits->Fill(Track_validPixelHits->at(iTrack));
      }
      else
      {
        hRecoMuonTrackValidHits->Fill(0);
        hRecoMuonTrackChi2->Fill(9999);
        hRecoMuonTrackNdof->Fill(0);
        hRecoMuonTrackLayersWithMeasurement->Fill(0);
        hRecoMuonTrackValidPixelHits->Fill(0);
      }
      hRecoMuonGlobalChi2->Fill(Muon_globalChi2->at(iMuon));
      hRecoMuonGlobalValidMuonHits->Fill(Muon_globalValidMuonHits->at(iMuon));
      hRecoMuonNumberOfMatches->Fill(Muon_numberOfMatches->at(iMuon));
      hRecoMuonNumberOfMatchedStations->Fill(Muon_numberOfMatchedStations->at(iMuon));
      unsigned short stationMask = Muon_stationMask->at(iMuon);
      for(unsigned short iBit = 0; iBit < 8; iBit++)
      {
//         std::cout << "Eta: " << Muon_eta->at(iMuon) << " Station: " << iBit+1 << " value: " << (stationMask&(1<<iBit)) << std::endl;
        if((stationMask&(1<<iBit)) != 0)
        {
          hRecoMuonStationsWithHits->Fill(iBit+1);
          hRecoMuonStationsWithHitsMerged->Fill((iBit%4)+1);
          if(iBit < 4)
          {
            hRecoMuonDTStationsWithHits->Fill(iBit+1);
          }
          else
          {
            hRecoMuonCSCStationsWithHits->Fill((iBit-4)+1);
          }
        }
      }
      hRecoMuonKink->Fill(Muon_Kink->at(iMuon));
      hRecoMuonKinkRho->Fill(Muon_Kink_rho->at(iMuon));
      hRecoMuonKinkZ->Fill(Muon_Kink_z->at(iMuon));
      hRecoMuonGlobalKink->Fill(Muon_GlobalKink->at(iMuon));
      hRecoMuonGlobalKinkRho->Fill(Muon_GlobalKink_rho->at(iMuon));
      hRecoMuonGlobalKinkZ->Fill(Muon_GlobalKink_z->at(iMuon));
      hRecoMuonGlobalKinkChi2LocalMomentum->Fill(Muon_GlobalKink_chi2LocalMomentum->at(iMuon));
      hRecoMuonGlobalKinkChi2LocalPosition->Fill(Muon_GlobalKink_chi2LocalPosition->at(iMuon));
      
      hRecoMuonChi2LocalMomentumVsChi2LocalPosition->Fill(Muon_GlobalKink_chi2LocalPosition->at(iMuon), Muon_GlobalKink_chi2LocalMomentum->at(iMuon));
      
      hRecoMuonGlbTrackProbability->Fill(Muon_glbTrackProbability->at(iMuon));
      hRecoMuonGlobalKinkGlobalDeltaEtaPhi->Fill(Muon_GlobalKink_globalDeltaEtaPhi->at(iMuon));
      hRecoMuonGlobalKinkLocalDistance->Fill(Muon_GlobalKink_localDistance->at(iMuon));
      hRecoMuonGlobalKinkLocalDistance->Fill(Muon_GlobalKink_localDistance->at(iMuon));
      hRecoMuonGlobalKinkStaRelChi2->Fill(Muon_GlobalKink_staRelChi2->at(iMuon));
      hRecoMuonGlobalKinkTightMatch->Fill(Muon_GlobalKink_tightMatch->at(iMuon));
      hRecoMuonGlobalKinkTrkRelChi2->Fill(Muon_GlobalKink_trkRelChi2->at(iMuon));
      
      hRecoMuonStaRelChi2VsTrkRelChi2->Fill(Muon_GlobalKink_trkRelChi2->at(iMuon), Muon_GlobalKink_staRelChi2->at(iMuon));
      
      hRecoMuonEnergyEcalTime->Fill(Muon_energy_ecalTime->at(iMuon));
      hRecoMuonEnergyEcalTimeError->Fill(Muon_energy_ecalTimeError->at(iMuon));
      hRecoMuonEnergyEm->Fill(Muon_energy_em->at(iMuon));
      hRecoMuonEnergyEmMax->Fill(Muon_energy_emMax->at(iMuon));
      hRecoMuonEnergyEmS25->Fill(Muon_energy_emS25->at(iMuon));
      hRecoMuonEnergyEmS9->Fill(Muon_energy_emS9->at(iMuon));
      hRecoMuonEnergyHcalTime->Fill(Muon_energy_hcalTime->at(iMuon));
      hRecoMuonEnergyHcalTimeError->Fill(Muon_energy_hcalTimeError->at(iMuon));
      hRecoMuonEnergyHad->Fill(Muon_energy_had->at(iMuon));
      hRecoMuonEnergyHadMax->Fill(Muon_energy_hadMax->at(iMuon));
      hRecoMuonEnergyHadS9->Fill(Muon_energy_hadS9->at(iMuon));
      hRecoMuonEnergyTower->Fill(Muon_energy_tower->at(iMuon));
      hRecoMuonEnergyTowerS9->Fill(Muon_energy_towerS9->at(iMuon));
      
      hRecoMuonIsolationR03EmEt->Fill(Muon_isolationR03_emEt->at(iMuon));
      hRecoMuonIsolationR03EmVetoEt->Fill(Muon_isolationR03_emVetoEt->at(iMuon));
      hRecoMuonIsolationR03EmMinusVetoEt->Fill(Muon_isolationR03_emEt->at(iMuon)-Muon_isolationR03_emVetoEt->at(iMuon));
      hRecoMuonIsolationR03HadEt->Fill(Muon_isolationR03_hadEt->at(iMuon));
      hRecoMuonIsolationR03HadVetoEt->Fill(Muon_isolationR03_hadVetoEt->at(iMuon));
      hRecoMuonIsolationR03HadMinusVetoEt->Fill(Muon_isolationR03_hadEt->at(iMuon)-Muon_isolationR03_hadVetoEt->at(iMuon));
      hRecoMuonIsolationR03HoEt->Fill(Muon_isolationR03_hoEt->at(iMuon));
      hRecoMuonIsolationR03HoVetoEt->Fill(Muon_isolationR03_hoVetoEt->at(iMuon));
      hRecoMuonIsolationR03HoMinusVetoEt->Fill(Muon_isolationR03_hoEt->at(iMuon)-Muon_isolationR03_hoVetoEt->at(iMuon));
      hRecoMuonIsolationR03NJets->Fill(Muon_isolationR03_nJets->at(iMuon));
      hRecoMuonIsolationR03NTracks->Fill(Muon_isolationR03_nTracks->at(iMuon));
      hRecoMuonIsolationR03SumPt->Fill(Muon_isolationR03_sumPt->at(iMuon));
      hRecoMuonIsolationR03TrackerVetoPt->Fill(Muon_isolationR03_sumPt->at(iMuon)-Muon_isolationR03_trackerVetoPt->at(iMuon));
      
      hRecoMuonCaloEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emEt->at(iMuon)+Muon_isolationR03_hadEt->at(iMuon));
      hRecoMuonCaloVetoEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emVetoEt->at(iMuon)+Muon_isolationR03_hadVetoEt->at(iMuon));
      hRecoMuonCaloMinusVetoEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emEt->at(iMuon)+Muon_isolationR03_hadEt->at(iMuon)-(Muon_isolationR03_emVetoEt->at(iMuon)+Muon_isolationR03_hadVetoEt->at(iMuon)));
      
      
      hRecoMuonPfIsolationR03SumChargedHadronPt->Fill(Muon_pfIsolationR03_sumChargedHadronPt->at(iMuon));
      hRecoMuonPfIsolationR03SumChargedParticlePt->Fill(Muon_pfIsolationR03_sumChargedParticlePt->at(iMuon));
      hRecoMuonPfIsolationR03SumNeutralHadronEt->Fill(Muon_pfIsolationR03_sumNeutralHadronEt->at(iMuon));
      hRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold->Fill(Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold->at(iMuon));
      hRecoMuonPfIsolationR03SumPhotonEt->Fill(Muon_pfIsolationR03_sumPhotonEt->at(iMuon));
      hRecoMuonPfIsolationR03SumPhotonEtHighThreshold->Fill(Muon_pfIsolationR03_sumPhotonEtHighThreshold->at(iMuon));
      hRecoMuonPfIsolationR03SumPUPt->Fill(Muon_pfIsolationR03_sumPUPt->at(iMuon));

      hRecoMuonTrackIso->Fill(Muon_trackIso->at(iMuon));
      hRecoMuonEcalIso->Fill(Muon_ecalIso->at(iMuon));
      hRecoMuonHcalIso->Fill(Muon_hcalIso->at(iMuon));
      hRecoMuonCaloIso->Fill(Muon_caloIso->at(iMuon));
      
      hRecoMuonShowerNCorrelatedHitsInStation1->Fill(Muon_shower_nStationCorrelatedHits->at(iMuon).at(0));
      hRecoMuonShowerNCorrelatedHitsInStation2->Fill(Muon_shower_nStationCorrelatedHits->at(iMuon).at(1));
      hRecoMuonShowerNCorrelatedHitsInStation3->Fill(Muon_shower_nStationCorrelatedHits->at(iMuon).at(2));
      hRecoMuonShowerNCorrelatedHitsInStation4->Fill(Muon_shower_nStationCorrelatedHits->at(iMuon).at(3));
      hRecoMuonShowerNHitsInStation1->Fill(Muon_shower_nStationHits->at(iMuon).at(0));
      hRecoMuonShowerNHitsInStation2->Fill(Muon_shower_nStationHits->at(iMuon).at(1));
      hRecoMuonShowerNHitsInStation3->Fill(Muon_shower_nStationHits->at(iMuon).at(2));
      hRecoMuonShowerNHitsInStation4->Fill(Muon_shower_nStationHits->at(iMuon).at(3));
      
//       std::cout << "Sono qui 4.1\n";
      
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
    }

//     std::cout << "Sono qui 5\n";
    
//     std::cout << "Sono qui 6\n";
    std::vector<double> vEffSelMuon;
    for(unsigned int iSelectedMuon = 0; iSelectedMuon != vSelectedMuons.size(); iSelectedMuon++)
    {
      double effSelMuon = GetGlobalEfficiency(Muon_pt->at(vSelectedMuons[iSelectedMuon]), Muon_eta->at(vSelectedMuons[iSelectedMuon]), effMethod);
      if(effSelMuon == 0)
        effSelMuon = 1;
      vEffSelMuon.push_back(effSelMuon);
    }
    if(vSelectedMuons.size() > 1)
    {
      double ptSelMuon0 = Muon_pt->at(vSelectedMuons[0]);
      double ptSelMuon1 = Muon_pt->at(vSelectedMuons[1]);
      double effSelMuon0 = GetGlobalEfficiency(Muon_pt->at(vSelectedMuons[0]), Muon_eta->at(vSelectedMuons[0]), effMethod);
      double effSelMuon1 = GetGlobalEfficiency(Muon_pt->at(vSelectedMuons[1]), Muon_eta->at(vSelectedMuons[1]), effMethod);
      if(effSelMuon0 == 0)
        effSelMuon0 = 1;
      if(effSelMuon1 == 0)
        effSelMuon1 = 1;
      if(ptSelMuon0>ptSelMuon1)
      {
        hSelMuonLeadingPt->Fill(ptSelMuon0);
        hSelMuonLeadingEta->Fill(Muon_eta->at(vSelectedMuons[0]));
        hSelMuonSecondPt->Fill(ptSelMuon1);
        hSelMuonSecondEta->Fill(Muon_eta->at(vSelectedMuons[1]));
      }
      else
      {
        hSelMuonLeadingPt->Fill(ptSelMuon1);
        hSelMuonLeadingEta->Fill(Muon_eta->at(vSelectedMuons[1]));
        hSelMuonSecondPt->Fill(ptSelMuon0);
        hSelMuonSecondEta->Fill(Muon_eta->at(vSelectedMuons[0]));
      }
    }
//     std::cout << "Sono qui 7\n";
    
    for(unsigned int i = 0; i < vSelectedMuons.size(); i++)
    {
      nSelMuons++;
      unsigned short iMuon = vSelectedMuons[i];
      hSelMuonPt->Fill(Muon_pt->at(iMuon));
      hSelMuonEta->Fill(Muon_eta->at(iMuon));
      hSelMuonPhi->Fill(Muon_phi->at(iMuon));
      hSelMuonCharge->Fill(Muon_charge->at(iMuon));
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
      hSelMuonSegmentCompatibility->Fill(Muon_segmentCompatibility->at(iMuon));
      unsigned int iTrack = Muon_trackref->at(iMuon);
      if(iTrack != 65535 && iTrack != 2048)
      {
        hSelMuonTrackValidHits->Fill(Track_numberOfValidHits->at(iTrack));
        hSelMuonTrackChi2->Fill(Track_normalizedChi2->at(iTrack));
        hSelMuonTrackNdof->Fill(Track_ndof->at(iTrack));
        hSelMuonTrackLayersWithMeasurement->Fill(Track_layersWithMeasurement->at(iTrack));
        hSelMuonTrackValidPixelHits->Fill(Track_validPixelHits->at(iTrack));
        
        hSelMuonDxy->Fill(Track_dxy_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDxyError->Fill(Track_dxyError_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDxyz->Fill(Track_dxyz_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDxyzError->Fill(Track_dxyzError_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDz->Fill(Track_dz_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonAbsDz->Fill(fabs(Track_dz_PV->at(iTrack).at(iSelectedVertex)));
        hSelMuonDzError->Fill(Track_dzError_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDxyVsRunNumber->Fill(runNumber, Track_dxy_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDxyErrorVsRunNumber->Fill(runNumber, Track_dxyError_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDxyzVsRunNumber->Fill(runNumber, Track_dxyz_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDxyzErrorVsRunNumber->Fill(runNumber, Track_dxyzError_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonDzVsRunNumber->Fill(runNumber, Track_dz_PV->at(iTrack).at(iSelectedVertex));
        hSelMuonAbsDzVsRunNumber->Fill(runNumber, fabs(Track_dz_PV->at(iTrack).at(iSelectedVertex)));
        hSelMuonDzErrorVsRunNumber->Fill(runNumber, Track_dzError_PV->at(iTrack).at(iSelectedVertex));
        
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
        hSelMuonDxyBSError->Fill(Track_dxyError_BS->at(iTrack));
        hSelMuonSignedDxyBSVsPhi->Fill(Track_phi->at(iTrack), Track_dxy_BS->at(iTrack)*sign);
        hSelMuonDxyBSErrorVsPhi->Fill(Track_phi->at(iTrack), Track_dxyError_BS->at(iTrack));
      }
      else
      {
        hSelMuonTrackValidHits->Fill(0);
        hSelMuonTrackChi2->Fill(9999);
        hSelMuonTrackNdof->Fill(0);
        hSelMuonTrackLayersWithMeasurement->Fill(0);
        hSelMuonTrackValidPixelHits->Fill(0);
        hSelMuonDxy->Fill(9999);
        hSelMuonDxyError->Fill(9999);
        hSelMuonDxyz->Fill(9999);
        hSelMuonDxyzError->Fill(9999);
        hSelMuonDz->Fill(9999);
        hSelMuonAbsDz->Fill(9999);
        hSelMuonDzError->Fill(9999);
        hSelMuonDxyVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyErrorVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyzErrorVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonAbsDzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDzErrorVsRunNumber->Fill(runNumber, 9999);
        
        hSelMuonSignedDxyBS->Fill(9999);
        hSelMuonDxyBSError->Fill(9999);
        hSelMuonSignedDxyBSVsPhi->Fill(9999, 9999);
        hSelMuonDxyBSErrorVsPhi->Fill(9999, 9999);
      }
      hSelMuonGlobalChi2->Fill(Muon_globalChi2->at(iMuon));
      hSelMuonGlobalValidMuonHits->Fill(Muon_globalValidMuonHits->at(iMuon));
      hSelMuonNumberOfMatches->Fill(Muon_numberOfMatches->at(iMuon));
    
      hSelMuonNumberOfMatchedStations->Fill(Muon_numberOfMatchedStations->at(iMuon));
      unsigned short stationMask = Muon_stationMask->at(iMuon);
      for(unsigned short iBit = 0; iBit < 8; iBit++)
      {
//         std::cout << "Eta: " << Muon_eta->at(iMuon) << " Station: " << iBit+1 << " value: " << (stationMask&(1<<iBit)) << std::endl;
        if((stationMask&(1<<iBit)) != 0)
        {
          hSelMuonStationsWithHits->Fill(iBit+1);
          hSelMuonStationsWithHitsMerged->Fill((iBit%4)+1);
          if(iBit < 4)
          {
            hSelMuonDTStationsWithHits->Fill(iBit+1);
          }
          else
          {
            hSelMuonCSCStationsWithHits->Fill((iBit-4)+1);
          }
        }
      }
      hSelMuonKink->Fill(Muon_Kink->at(iMuon));
      hSelMuonKinkRho->Fill(Muon_Kink_rho->at(iMuon));
      hSelMuonKinkZ->Fill(Muon_Kink_z->at(iMuon));
      hSelMuonGlobalKink->Fill(Muon_GlobalKink->at(iMuon));
      hSelMuonGlobalKinkRho->Fill(Muon_GlobalKink_rho->at(iMuon));
      hSelMuonGlobalKinkZ->Fill(Muon_GlobalKink_z->at(iMuon));
      hSelMuonGlobalKinkChi2LocalMomentum->Fill(Muon_GlobalKink_chi2LocalMomentum->at(iMuon));
      hSelMuonGlobalKinkChi2LocalPosition->Fill(Muon_GlobalKink_chi2LocalPosition->at(iMuon));
      hSelMuonChi2LocalMomentumVsChi2LocalPosition->Fill(Muon_GlobalKink_chi2LocalPosition->at(iMuon), Muon_GlobalKink_chi2LocalMomentum->at(iMuon));
      hSelMuonGlbTrackProbability->Fill(Muon_glbTrackProbability->at(iMuon));
      hSelMuonGlobalKinkGlobalDeltaEtaPhi->Fill(Muon_GlobalKink_globalDeltaEtaPhi->at(iMuon));
      hSelMuonGlobalKinkLocalDistance->Fill(Muon_GlobalKink_localDistance->at(iMuon));
      hSelMuonGlobalKinkStaRelChi2->Fill(Muon_GlobalKink_staRelChi2->at(iMuon));
      hSelMuonGlobalKinkTightMatch->Fill(Muon_GlobalKink_tightMatch->at(iMuon));
      hSelMuonGlobalKinkTrkRelChi2->Fill(Muon_GlobalKink_trkRelChi2->at(iMuon));
      hSelMuonStaRelChi2VsTrkRelChi2->Fill(Muon_GlobalKink_trkRelChi2->at(iMuon), Muon_GlobalKink_staRelChi2->at(iMuon));
      hSelMuonEnergyEcalTime->Fill(Muon_energy_ecalTime->at(iMuon));
      hSelMuonEnergyEcalTimeError->Fill(Muon_energy_ecalTimeError->at(iMuon));
      hSelMuonEnergyEm->Fill(Muon_energy_em->at(iMuon));
      hSelMuonEnergyEmMax->Fill(Muon_energy_emMax->at(iMuon));
      hSelMuonEnergyEmS25->Fill(Muon_energy_emS25->at(iMuon));
      hSelMuonEnergyEmS9->Fill(Muon_energy_emS9->at(iMuon));
      hSelMuonEnergyHcalTime->Fill(Muon_energy_hcalTime->at(iMuon));
      hSelMuonEnergyHcalTimeError->Fill(Muon_energy_hcalTimeError->at(iMuon));
      hSelMuonEnergyHad->Fill(Muon_energy_had->at(iMuon));
      hSelMuonEnergyHadMax->Fill(Muon_energy_hadMax->at(iMuon));
      hSelMuonEnergyHadS9->Fill(Muon_energy_hadS9->at(iMuon));
      hSelMuonEnergyTower->Fill(Muon_energy_tower->at(iMuon));
      hSelMuonEnergyTowerS9->Fill(Muon_energy_towerS9->at(iMuon));
      hSelMuonIsolationR03EmEt->Fill(Muon_isolationR03_emEt->at(iMuon));
      hSelMuonIsolationR03EmVetoEt->Fill(Muon_isolationR03_emVetoEt->at(iMuon));
      hSelMuonIsolationR03EmMinusVetoEt->Fill(Muon_isolationR03_emEt->at(iMuon)-Muon_isolationR03_emVetoEt->at(iMuon));
      hSelMuonIsolationR03HadEt->Fill(Muon_isolationR03_hadEt->at(iMuon));
      hSelMuonIsolationR03HadVetoEt->Fill(Muon_isolationR03_hadVetoEt->at(iMuon));
      hSelMuonIsolationR03HadMinusVetoEt->Fill(Muon_isolationR03_hadEt->at(iMuon)-Muon_isolationR03_hadVetoEt->at(iMuon));
      hSelMuonIsolationR03HoEt->Fill(Muon_isolationR03_hoEt->at(iMuon));
      hSelMuonIsolationR03HoVetoEt->Fill(Muon_isolationR03_hoVetoEt->at(iMuon));
      hSelMuonIsolationR03HoMinusVetoEt->Fill(Muon_isolationR03_hoEt->at(iMuon)-Muon_isolationR03_hoVetoEt->at(iMuon));
      hSelMuonIsolationR03NJets->Fill(Muon_isolationR03_nJets->at(iMuon));
      hSelMuonIsolationR03NTracks->Fill(Muon_isolationR03_nTracks->at(iMuon));
      hSelMuonIsolationR03SumPt->Fill(Muon_isolationR03_sumPt->at(iMuon));
      hSelMuonIsolationR03TrackerVetoPt->Fill(Muon_isolationR03_sumPt->at(iMuon)-Muon_isolationR03_trackerVetoPt->at(iMuon));
     
      hSelMuonCaloEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emEt->at(iMuon)+Muon_isolationR03_hadEt->at(iMuon));
      hSelMuonCaloVetoEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emVetoEt->at(iMuon)+Muon_isolationR03_hadVetoEt->at(iMuon));
      hSelMuonCaloMinusVetoEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emEt->at(iMuon)+Muon_isolationR03_hadEt->at(iMuon)-(Muon_isolationR03_emVetoEt->at(iMuon)+Muon_isolationR03_hadVetoEt->at(iMuon)));
      
      hSelMuonPfIsolationR03SumChargedHadronPt->Fill(Muon_pfIsolationR03_sumChargedHadronPt->at(iMuon));
      hSelMuonPfIsolationR03SumChargedParticlePt->Fill(Muon_pfIsolationR03_sumChargedParticlePt->at(iMuon));
      hSelMuonPfIsolationR03SumNeutralHadronEt->Fill(Muon_pfIsolationR03_sumNeutralHadronEt->at(iMuon));
      hSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold->Fill(Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold->at(iMuon));
      hSelMuonPfIsolationR03SumPhotonEt->Fill(Muon_pfIsolationR03_sumPhotonEt->at(iMuon));
      hSelMuonPfIsolationR03SumPhotonEtHighThreshold->Fill(Muon_pfIsolationR03_sumPhotonEtHighThreshold->at(iMuon));
      hSelMuonPfIsolationR03SumPUPt->Fill(Muon_pfIsolationR03_sumPUPt->at(iMuon));
      hSelMuonTrackIso->Fill(Muon_trackIso->at(iMuon));
      hSelMuonEcalIso->Fill(Muon_ecalIso->at(iMuon));
      hSelMuonHcalIso->Fill(Muon_hcalIso->at(iMuon));
      hSelMuonCaloIso->Fill(Muon_caloIso->at(iMuon));
      hSelMuonShowerNCorrelatedHitsInStation1->Fill(Muon_shower_nStationCorrelatedHits->at(iMuon).at(0));
      hSelMuonShowerNCorrelatedHitsInStation2->Fill(Muon_shower_nStationCorrelatedHits->at(iMuon).at(1));
      hSelMuonShowerNCorrelatedHitsInStation3->Fill(Muon_shower_nStationCorrelatedHits->at(iMuon).at(2));
      hSelMuonShowerNCorrelatedHitsInStation4->Fill(Muon_shower_nStationCorrelatedHits->at(iMuon).at(3));
      hSelMuonShowerNHitsInStation1->Fill(Muon_shower_nStationHits->at(iMuon).at(0));
      hSelMuonShowerNHitsInStation2->Fill(Muon_shower_nStationHits->at(iMuon).at(1));
      hSelMuonShowerNHitsInStation3->Fill(Muon_shower_nStationHits->at(iMuon).at(2));
      hSelMuonShowerNHitsInStation4->Fill(Muon_shower_nStationHits->at(iMuon).at(3));
    }
//     std::cout << "Sono qui 8\n";

//     std::cout << "Sono qui 9\n";
    
//     std::cout << "Sono qui 10\n";
    
    if(vSelectedMuons.size() > 1)
    {
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
      hSelDiMuonDeltaR->Fill(deltaR);
      hSelDiMuonDeltaEta->Fill(deltaEta);
      hSelDiMuonDeltaPhi->Fill(deltaPhi);
      hSelDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
      hSelDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
//       std::cout << "Sono qui 11\n";
      if(Muon_charge->at(vSelectedMuons[0]) != Muon_charge->at(vSelectedMuons[1]))
      {
        hSelDSDiMuonInvMass->Fill(mass);
        hSelDSDiMuonDeltaR->Fill(deltaR);
        hSelDSDiMuonDeltaEta->Fill(deltaEta);
        hSelDSDiMuonDeltaPhi->Fill(deltaPhi);
        hSelDSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
        hSelDSDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
      }
      else
      {
        hSelSSDiMuonInvMass->Fill(mass);
        hSelSSDiMuonDeltaR->Fill(deltaR);
        hSelSSDiMuonDeltaEta->Fill(deltaEta);
        hSelSSDiMuonDeltaPhi->Fill(deltaPhi);
        hSelSSDiMuonDeltaEtaVsDeltaPhi->Fill(deltaPhi, deltaEta);
        hSelSSDiMuonInvMassVsDeltaR->Fill(deltaR, mass);
      }
    }
//     std::cout << "Sono qui 12\n";
    
    hNRecoMuons->Fill(nRecoMuons);
    hNSelMuons->Fill(nSelMuons);
    hNTriggeringRecoMuons->Fill(nTriggeringRecoMuons);
    hNTriggeringSelMuons->Fill(nTriggeringSelMuons);
    hNRecoMuonsVsRunNumber->Fill(runNumber,nRecoMuons);
    hNSelMuonsVsRunNumber->Fill(runNumber,nSelMuons);
//     std::cout << "Sono qui 13\n";
    
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
    }
//     std::cout << "Sono qui 14\n";
    
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
    hNRecoVerticesVsRunNumber->Fill(runNumber, nRecoVertices);
    hNSelVerticesVsRunNumber->Fill(runNumber, nSelVertices);
//     std::cout << "Sono qui 15\n";
  }

  // Keep this line here!
  outFile->cd();
  
  // Writes into the outFile the name of the efficiency file used in this run
  WriteEfficiencyMetadata(outFile);
  
  TProfile* hpxRecoVerticesXErrorVsRunNumber = hRecoVerticesXErrorVsRunNumber->ProfileX();
  TProfile* hpxSelVerticesXErrorVsRunNumber = hSelVerticesXErrorVsRunNumber->ProfileX();
  TProfile* hpxRecoVerticesYErrorVsRunNumber = hRecoVerticesYErrorVsRunNumber->ProfileX();
  TProfile* hpxSelVerticesYErrorVsRunNumber = hSelVerticesYErrorVsRunNumber->ProfileX();
  TProfile* hpxRecoVerticesZErrorVsRunNumber = hRecoVerticesZErrorVsRunNumber->ProfileX();
  TProfile* hpxSelVerticesZErrorVsRunNumber = hSelVerticesZErrorVsRunNumber->ProfileX();
  
  TProfile* hpxNRecoMuonsVsRunNumber = hNRecoMuonsVsRunNumber->ProfileX();
  TProfile* hpxNSelMuonsVsRunNumber = hNSelMuonsVsRunNumber->ProfileX();
  TProfile* hpxNRecoVerticesVsRunNumber = hNRecoVerticesVsRunNumber->ProfileX();
  TProfile* hpxNSelVerticesVsRunNumber = hNSelVerticesVsRunNumber->ProfileX();
  
  TProfile* hpxSelMuonDxyVsRunNumber = hSelMuonDxyVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDxyErrorVsRunNumber = hSelMuonDxyErrorVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDxyzVsRunNumber = hSelMuonDxyzVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDxyzErrorVsRunNumber = hSelMuonDxyzErrorVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDzVsRunNumber = hSelMuonDzVsRunNumber->ProfileX();
  TProfile* hpxSelMuonAbsDzVsRunNumber = hSelMuonAbsDzVsRunNumber->ProfileX();
  TProfile* hpxSelMuonDzErrorVsRunNumber = hSelMuonDzErrorVsRunNumber->ProfileX();
  
  // Create and write canvases here
  CreateAndWriteCanvas("cSelMuonDeltaRFromGenp", 0, 21, 1, false, false, hSelMuonDeltaRFromGenp);
  
  CreateAndWriteCanvas("cRecoMuonPt",0, 21, 1, false, true, hRecoMuonPt);
  CreateAndWriteCanvas("cSelMuonPt",0, 21, 1, false, true, hSelMuonPt);
  CreateAndWriteCanvas("cSelMuonLeadingPt",0, 21, 1, false, true, hSelMuonLeadingPt);
  CreateAndWriteCanvas("cSelMuonSecondPt",0, 21, 1, false, true, hSelMuonSecondPt);
  CreateAndWriteCanvas("cRecoMuonEta",0, 21, 1, false, false, hRecoMuonEta);
  CreateAndWriteCanvas("cSelMuonEta",0, 21, 1, false, false, hSelMuonEta);
  CreateAndWriteCanvas("cSelMuonLeadingEta",0, 21, 1, false, true, hSelMuonLeadingEta);
  CreateAndWriteCanvas("cSelMuonSecondEta",0, 21, 1, false, true, hSelMuonSecondEta);
  CreateAndWriteCanvas("cRecoMuonPhi",0, 21, 1, false, false, hRecoMuonPhi);
  CreateAndWriteCanvas("cSelMuonPhi",0, 21, 1, false, false, hSelMuonPhi);
  CreateAndWriteCanvas("cRecoMuonCharge",0, 21, 1, false, false, hRecoMuonCharge);
  CreateAndWriteCanvas("cSelMuonCharge",0, 21, 1, false, false, hSelMuonCharge);
  CreateAndWriteCanvas("cRecoMuonHasTriggered",0, 21, 1, false, true, hRecoMuonHasTriggered);
  CreateAndWriteCanvas("cSelMuonHasTriggered",0, 21, 1, false, true, hSelMuonHasTriggered);
  CreateAndWriteCanvas("cRecoMuonCaloCompatibility",0, 21, 1, false, false, hRecoMuonCaloCompatibility);
  CreateAndWriteCanvas("cSelMuonCaloCompatibility",0, 21, 1, false, false, hSelMuonCaloCompatibility);
  CreateAndWriteCanvas("cRecoMuonSegmentCompatibility",0, 21, 1, false, false, hRecoMuonSegmentCompatibility);
  CreateAndWriteCanvas("cSelMuonSegmentCompatibility",0, 21, 1, false, false, hSelMuonSegmentCompatibility);
  CreateAndWriteCanvas("cRecoMuonTrackValidHits",0, 21, 1, false, false, hRecoMuonTrackValidHits);
  CreateAndWriteCanvas("cSelMuonTrackValidHits",0, 21, 1, false, false, hSelMuonTrackValidHits);
  CreateAndWriteCanvas("cRecoMuonTrackChi2",0, 21, 1, false, false, hRecoMuonTrackChi2);
  CreateAndWriteCanvas("cSelMuonTrackChi2",0, 21, 1, false, false, hSelMuonTrackChi2);
  CreateAndWriteCanvas("cRecoMuonTrackNdof",0, 21, 1, false, false, hRecoMuonTrackNdof);
  CreateAndWriteCanvas("cSelMuonTrackNdof",0, 21, 1, false, false, hSelMuonTrackNdof);
  CreateAndWriteCanvas("cRecoMuonTrackLayersWithMeasurement",0, 21, 1, false, false, hRecoMuonTrackLayersWithMeasurement);
  CreateAndWriteCanvas("cSelMuonTrackLayersWithMeasurement",0, 21, 1, false, false, hSelMuonTrackLayersWithMeasurement);
  CreateAndWriteCanvas("cRecoMuonTrackValidPixelHits",0, 21, 1, false, false, hRecoMuonTrackValidPixelHits);
  CreateAndWriteCanvas("cSelMuonTrackValidPixelHits",0, 21, 1, false, false, hSelMuonTrackValidPixelHits);
  CreateAndWriteCanvas("cRecoMuonGlobalChi2",0, 21, 1, false, false, hRecoMuonGlobalChi2);
  CreateAndWriteCanvas("cSelMuonGlobalChi2",0, 21, 1, false, false, hSelMuonGlobalChi2);
  CreateAndWriteCanvas("cRecoMuonGlobalValidMuonHits",0, 21, 1, false, false, hRecoMuonGlobalValidMuonHits);
  CreateAndWriteCanvas("cSelMuonGlobalValidMuonHits",0, 21, 1, false, false, hSelMuonGlobalValidMuonHits);
  CreateAndWriteCanvas("cRecoMuonNumberOfMatches",0, 21, 1, false, false, hRecoMuonNumberOfMatches);
  CreateAndWriteCanvas("cSelMuonNumberOfMatches",0, 21, 1, false, false, hSelMuonNumberOfMatches);
  CreateAndWriteCanvas("cRecoMuonNumberOfMatchedStations",0, 21, 1, false, false, hRecoMuonNumberOfMatchedStations);
  CreateAndWriteCanvas("cSelMuonNumberOfMatchedStations",0, 21, 1, false, false, hSelMuonNumberOfMatchedStations);
  CreateAndWriteCanvas("cRecoMuonStationsWithHits",0, 21, 1, false, false, hRecoMuonStationsWithHits);
  CreateAndWriteCanvas("cSelMuonStationsWithHits",0, 21, 1, false, false, hSelMuonStationsWithHits);
  CreateAndWriteCanvas("cRecoMuonDTStationsWithHits",0, 21, 1, false, false, hRecoMuonStationsWithHits);
  CreateAndWriteCanvas("cSelMuonDTStationsWithHits",0, 21, 1, false, false, hSelMuonDTStationsWithHits);
  CreateAndWriteCanvas("cRecoMuonStationsWithHits",0, 21, 1, false, false, hRecoMuonDTStationsWithHits);
  CreateAndWriteCanvas("cSelMuonCSCStationsWithHits",0, 21, 1, false, false, hSelMuonCSCStationsWithHits);
  CreateAndWriteCanvas("cRecoMuonCSCStationsWithHitsMerged",0, 21, 1, false, false, hRecoMuonStationsWithHitsMerged);
  CreateAndWriteCanvas("cSelMuonStationsWithHitsMerged",0, 21, 1, false, false, hSelMuonStationsWithHitsMerged);
  CreateAndWriteCanvas("cRecoMuonKink",0, 21, 1, false, false, hRecoMuonKink);
  CreateAndWriteCanvas("cSelMuonKink",0, 21, 1, false, false, hSelMuonKink);
  CreateAndWriteCanvas("cRecoMuonKinkRho",0, 21, 1, false, false, hRecoMuonKinkRho);
  CreateAndWriteCanvas("cSelMuonKinkRho",0, 21, 1, false, false, hSelMuonKinkRho);
  CreateAndWriteCanvas("cRecoMuonKinkZ",0, 21, 1, false, false, hRecoMuonKinkZ);
  CreateAndWriteCanvas("cSelMuonKinkZ",0, 21, 1, false, false, hSelMuonKinkZ);
  CreateAndWriteCanvas("cRecoMuonGlobalKink",0, 21, 1, false, false, hRecoMuonGlobalKink);
  CreateAndWriteCanvas("cSelMuonGlobalKink",0, 21, 1, false, false, hSelMuonGlobalKink);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkRho",0, 21, 1, false, false, hRecoMuonGlobalKinkRho);
  CreateAndWriteCanvas("cSelMuonGlobalKinkRho",0, 21, 1, false, false, hSelMuonGlobalKinkRho);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkZ",0, 21, 1, false, false, hRecoMuonGlobalKinkZ);
  CreateAndWriteCanvas("cSelMuonGlobalKinkZ",0, 21, 1, false, false, hSelMuonGlobalKinkZ);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkChi2LocalMomentum", 0, 21, 1, false, false, hRecoMuonGlobalKinkChi2LocalMomentum);
  CreateAndWriteCanvas("cSelMuonGlobalKinkChi2LocalMomentum", 0, 21, 1, false, false, hSelMuonGlobalKinkChi2LocalMomentum);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkChi2LocalPosition", 0, 21, 1, false, false, hRecoMuonGlobalKinkChi2LocalPosition);
  CreateAndWriteCanvas("cSelMuonGlobalKinkChi2LocalPosition", 0, 21, 1, false, false, hSelMuonGlobalKinkChi2LocalPosition);
  CreateAndWriteCanvas("cRecoMuonChi2LocalMomentumVsChi2LocalPosition","colz", false, false,false, hRecoMuonChi2LocalMomentumVsChi2LocalPosition);
  CreateAndWriteCanvas("cSelMuonChi2LocalMomentumVsChi2LocalPosition","colz", false, false,false, hSelMuonChi2LocalMomentumVsChi2LocalPosition);  
  CreateAndWriteCanvas("cRecoMuonGlbTrackProbability", 0, 21, 1, false, false, hRecoMuonGlbTrackProbability);
  CreateAndWriteCanvas("cSelMuonGlbTrackProbability", 0, 21, 1, false, false, hSelMuonGlbTrackProbability);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkGlobalDeltaEtaPhi", 0, 21, 1, false, false, hRecoMuonGlobalKinkGlobalDeltaEtaPhi);
  CreateAndWriteCanvas("cSelMuonGlobalKinkGlobalDeltaEtaPhi", 0, 21, 1, false, false, hSelMuonGlobalKinkGlobalDeltaEtaPhi);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkLocalDistance", 0, 21, 1, false, false, hRecoMuonGlobalKinkLocalDistance);
  CreateAndWriteCanvas("cSelMuonGlobalKinkLocalDistance", 0, 21, 1, false, false, hSelMuonGlobalKinkLocalDistance);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkStaRelChi2", 0, 21, 1, false, false, hRecoMuonGlobalKinkStaRelChi2);
  CreateAndWriteCanvas("cSelMuonGlobalKinkStaRelChi2", 0, 21, 1, false, false, hSelMuonGlobalKinkStaRelChi2);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkTightMatch", 0, 21, 1, false, false, hRecoMuonGlobalKinkTightMatch);
  CreateAndWriteCanvas("cSelMuonGlobalKinkTightMatch", 0, 21, 1, false, false, hSelMuonGlobalKinkTightMatch);
  CreateAndWriteCanvas("cRecoMuonGlobalKinkTrkRelChi2", 0, 21, 1, false, false, hRecoMuonGlobalKinkTrkRelChi2);
  CreateAndWriteCanvas("cSelMuonGlobalKinkTrkRelChi2", 0, 21, 1, false, false, hSelMuonGlobalKinkTrkRelChi2);
  CreateAndWriteCanvas("cRecoMuonStaRelChi2VsTrkRelChi2","colz", false, false,false, hRecoMuonStaRelChi2VsTrkRelChi2);
  CreateAndWriteCanvas("cSelMuonStaRelChi2VsTrkRelChi2","colz", false, false,false, hSelMuonStaRelChi2VsTrkRelChi2);
  CreateAndWriteCanvas("cRecoMuonEnergyEcalTime", 0, 21, 1, false, false, hRecoMuonEnergyEcalTime);
  CreateAndWriteCanvas("cSelMuonEnergyEcalTime", 0, 21, 1, false, false, hSelMuonEnergyEcalTime);
  CreateAndWriteCanvas("cRecoMuonEnergyEcalTimeError", 0, 21, 1, false, false, hRecoMuonEnergyEcalTimeError);
  CreateAndWriteCanvas("cSelMuonEnergyEcalTimeError", 0, 21, 1, false, false, hSelMuonEnergyEcalTimeError);
  CreateAndWriteCanvas("cRecoMuonEnergyEm", 0, 21, 1, false, false, hRecoMuonEnergyEm);
  CreateAndWriteCanvas("cSelMuonEnergyEm", 0, 21, 1, false, false, hSelMuonEnergyEm);
  CreateAndWriteCanvas("cRecoMuonEnergyEmMax", 0, 21, 1, false, false, hRecoMuonEnergyEmMax);
  CreateAndWriteCanvas("cSelMuonEnergyEmMax", 0, 21, 1, false, false, hSelMuonEnergyEmMax);
  CreateAndWriteCanvas("cRecoMuonEnergyEmS25", 0, 21, 1, false, false, hRecoMuonEnergyEmS25);
  CreateAndWriteCanvas("cSelMuonEnergyEmS25", 0, 21, 1, false, false, hSelMuonEnergyEmS25);
  CreateAndWriteCanvas("cRecoMuonEnergyEmS9", 0, 21, 1, false, false, hRecoMuonEnergyEmS9);
  CreateAndWriteCanvas("cSelMuonEnergyEmS9", 0, 21, 1, false, false, hSelMuonEnergyEmS9);
  CreateAndWriteCanvas("cRecoMuonEnergyHcalTime", 0, 21, 1, false, false, hRecoMuonEnergyHcalTime);
  CreateAndWriteCanvas("cSelMuonEnergyHcalTime", 0, 21, 1, false, false, hSelMuonEnergyHcalTime);
  CreateAndWriteCanvas("cRecoMuonEnergyHcalTimeError", 0, 21, 1, false, false, hRecoMuonEnergyHcalTimeError);
  CreateAndWriteCanvas("cSelMuonEnergyHcalTimeError", 0, 21, 1, false, false, hSelMuonEnergyHcalTimeError);
  CreateAndWriteCanvas("cRecoMuonEnergyHad", 0, 21, 1, false, false, hRecoMuonEnergyHad);
  CreateAndWriteCanvas("cSelMuonEnergyHad", 0, 21, 1, false, false, hSelMuonEnergyHad);
  CreateAndWriteCanvas("cRecoMuonEnergyHadMax", 0, 21, 1, false, false, hRecoMuonEnergyHadMax);
  CreateAndWriteCanvas("cSelMuonEnergyHadMax", 0, 21, 1, false, false, hSelMuonEnergyHadMax);
  CreateAndWriteCanvas("cRecoMuonEnergyHadS9", 0, 21, 1, false, false, hRecoMuonEnergyHadS9);
  CreateAndWriteCanvas("cSelMuonEnergyHadS9", 0, 21, 1, false, false, hSelMuonEnergyHadS9);
  CreateAndWriteCanvas("cRecoMuonEnergyTower", 0, 21, 1, false, false, hRecoMuonEnergyTower);
  CreateAndWriteCanvas("cSelMuonEnergyTower", 0, 21, 1, false, false, hSelMuonEnergyTower);
  CreateAndWriteCanvas("cRecoMuonEnergyTowerS9", 0, 21, 1, false, false, hRecoMuonEnergyTowerS9);
  CreateAndWriteCanvas("cSelMuonEnergyTowerS9", 0, 21, 1, false, false, hSelMuonEnergyTowerS9);
 
  CreateAndWriteCanvas("cRecoMuonIsolationR03EmEt", 0, 21, 1, false, false, hRecoMuonIsolationR03EmEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03EmEt", 0, 21, 1, false, false, hSelMuonIsolationR03EmEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03EmVetoEt", 0, 21, 1, false, false, hRecoMuonIsolationR03EmVetoEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03EmVetoEt", 0, 21, 1, false, false, hSelMuonIsolationR03EmVetoEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03EmMinusVetoEt", 0, 21, 1, false, false, hRecoMuonIsolationR03EmMinusVetoEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03EmMinusVetoEt", 0, 21, 1, false, false, hSelMuonIsolationR03EmMinusVetoEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03HadEt", 0, 21, 1, false, false, hRecoMuonIsolationR03HadEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03HadEt", 0, 21, 1, false, false, hSelMuonIsolationR03HadEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03HadVetoEt", 0, 21, 1, false, false, hRecoMuonIsolationR03HadVetoEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03HadVetoEt", 0, 21, 1, false, false, hSelMuonIsolationR03HadVetoEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03HadMinusVetoEt", 0, 21, 1, false, false, hRecoMuonIsolationR03HadMinusVetoEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03HadMinusVetoEt", 0, 21, 1, false, false, hSelMuonIsolationR03HadMinusVetoEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03HoEt", 0, 21, 1, false, false, hRecoMuonIsolationR03HoEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03HoEt", 0, 21, 1, false, false, hSelMuonIsolationR03HoEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03HoVetoEt", 0, 21, 1, false, false, hRecoMuonIsolationR03HoVetoEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03HoVetoEt", 0, 21, 1, false, false, hSelMuonIsolationR03HoVetoEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03HoMinusVetoEt", 0, 21, 1, false, false, hRecoMuonIsolationR03HoMinusVetoEt);
  CreateAndWriteCanvas("cSelMuonIsolationR03HoMinusVetoEt", 0, 21, 1, false, false, hSelMuonIsolationR03HoMinusVetoEt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03NJets", 0, 21, 1, false, false, hRecoMuonIsolationR03NJets);
  CreateAndWriteCanvas("cSelMuonIsolationR03NJets", 0, 21, 1, false, false, hSelMuonIsolationR03NJets);
  CreateAndWriteCanvas("cRecoMuonIsolationR03NTracks", 0, 21, 1, false, false, hRecoMuonIsolationR03NTracks);
  CreateAndWriteCanvas("cSelMuonIsolationR03NTracks", 0, 21, 1, false, false, hSelMuonIsolationR03NTracks);
  CreateAndWriteCanvas("cRecoMuonIsolationR03SumPt", 0, 21, 1, false, false, hRecoMuonIsolationR03SumPt);
  CreateAndWriteCanvas("cSelMuonIsolationR03SumPt", 0, 21, 1, false, false, hSelMuonIsolationR03SumPt);
  CreateAndWriteCanvas("cRecoMuonIsolationR03TrackerVetoPt", 0, 21, 1, false, false, hRecoMuonIsolationR03TrackerVetoPt);
  CreateAndWriteCanvas("cSelMuonIsolationR03TrackerVetoPt", 0, 21, 1, false, false, hSelMuonIsolationR03TrackerVetoPt);

  CreateAndWriteCanvas("cRecoMuonCaloEtR03VsMuonPt","colz", false, false,false, hRecoMuonCaloEtR03VsMuonPt);
  CreateAndWriteCanvas("cSelMuonCaloEtR03VsMuonPt","colz", false, false,false, hSelMuonCaloEtR03VsMuonPt);
  CreateAndWriteCanvas("cRecoMuonCaloVetoEtR03VsMuonPt","colz", false, false,false, hRecoMuonCaloVetoEtR03VsMuonPt);
  CreateAndWriteCanvas("cSelMuonCaloVetoEtR03VsMuonPt","colz", false, false,false, hSelMuonCaloVetoEtR03VsMuonPt);
  
  CreateAndWriteCanvas("cRecoMuonCaloMinusVetoEtR03VsMuonPt","colz", false, false,false, hRecoMuonCaloMinusVetoEtR03VsMuonPt);
  CreateAndWriteCanvas("cSelMuonCaloMinusVetoEtR03VsMuonPt","colz", false, false,false, hSelMuonCaloMinusVetoEtR03VsMuonPt);
  
  
  CreateAndWriteCanvas("cRecoMuonPfIsolationR03SumChargedHadronPt", 0, 21, 1, false, false, hRecoMuonPfIsolationR03SumChargedHadronPt);
  CreateAndWriteCanvas("cSelMuonPfIsolationR03SumChargedHadronPt", 0, 21, 1, false, false, hSelMuonPfIsolationR03SumChargedHadronPt);
  CreateAndWriteCanvas("cRecoMuonPfIsolationR03SumChargedParticlePt", 0, 21, 1, false, false, hRecoMuonPfIsolationR03SumChargedParticlePt);
  CreateAndWriteCanvas("cSelMuonPfIsolationR03SumChargedParticlePt", 0, 21, 1, false, false, hSelMuonPfIsolationR03SumChargedParticlePt);
  CreateAndWriteCanvas("cRecoMuonPfIsolationR03SumNeutralHadronEt", 0, 21, 1, false, false, hRecoMuonPfIsolationR03SumNeutralHadronEt);
  CreateAndWriteCanvas("cSelMuonPfIsolationR03SumNeutralHadronEt", 0, 21, 1, false, false, hSelMuonPfIsolationR03SumNeutralHadronEt);
  CreateAndWriteCanvas("cRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold", 0, 21, 1, false, false, hRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold);
  CreateAndWriteCanvas("cSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold", 0, 21, 1, false, false, hSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold);
  CreateAndWriteCanvas("cRecoMuonPfIsolationR03SumPhotonEt", 0, 21, 1, false, false, hRecoMuonPfIsolationR03SumPhotonEt);
  CreateAndWriteCanvas("cSelMuonPfIsolationR03SumPhotonEt", 0, 21, 1, false, false, hSelMuonPfIsolationR03SumPhotonEt);
  CreateAndWriteCanvas("cRecoMuonPfIsolationR03SumPhotonEtHighThreshold", 0, 21, 1, false, false, hRecoMuonPfIsolationR03SumPhotonEtHighThreshold);
  CreateAndWriteCanvas("cSelMuonPfIsolationR03SumPhotonEtHighThreshold", 0, 21, 1, false, false, hSelMuonPfIsolationR03SumPhotonEtHighThreshold);
  CreateAndWriteCanvas("cRecoMuonPfIsolationR03SumPUPt", 0, 21, 1, false, false, hRecoMuonPfIsolationR03SumPUPt);
  CreateAndWriteCanvas("cSelMuonPfIsolationR03SumPUPt", 0, 21, 1, false, false, hSelMuonPfIsolationR03SumPUPt);
  CreateAndWriteCanvas("cRecoMuonTrackIso", 0, 21, 1, false, false, hRecoMuonTrackIso);
  CreateAndWriteCanvas("cSelMuonTrackIso", 0, 21, 1, false, false, hSelMuonTrackIso);
  CreateAndWriteCanvas("cRecoMuonEcalIso", 0, 21, 1, false, false, hRecoMuonEcalIso);
  CreateAndWriteCanvas("cSelMuonEcalIso", 0, 21, 1, false, false, hSelMuonEcalIso);
  CreateAndWriteCanvas("cRecoMuonHcalIso", 0, 21, 1, false, false, hRecoMuonHcalIso);
  CreateAndWriteCanvas("cSelMuonHcalIso", 0, 21, 1, false, false, hSelMuonHcalIso);
  CreateAndWriteCanvas("cRecoMuonCaloIso", 0, 21, 1, false, false, hRecoMuonCaloIso);
  CreateAndWriteCanvas("cSelMuonCaloIso", 0, 21, 1, false, false, hSelMuonCaloIso);
  CreateAndWriteCanvas("cRecoMuonShowerNCorrelatedHitsInStation1", 0, 21, 1, false, false, hRecoMuonShowerNCorrelatedHitsInStation1);
  CreateAndWriteCanvas("cSelMuonShowerNCorrelatedHitsInStation1", 0, 21, 1, false, false, hSelMuonShowerNCorrelatedHitsInStation1);
  CreateAndWriteCanvas("cRecoMuonShowerNCorrelatedHitsInStation2", 0, 21, 1, false, false, hRecoMuonShowerNCorrelatedHitsInStation2);
  CreateAndWriteCanvas("cSelMuonShowerNCorrelatedHitsInStation2", 0, 21, 1, false, false, hSelMuonShowerNCorrelatedHitsInStation2);
  CreateAndWriteCanvas("cRecoMuonShowerNCorrelatedHitsInStation3", 0, 21, 1, false, false, hRecoMuonShowerNCorrelatedHitsInStation3);
  CreateAndWriteCanvas("cSelMuonShowerNCorrelatedHitsInStation3", 0, 21, 1, false, false, hSelMuonShowerNCorrelatedHitsInStation3);
  CreateAndWriteCanvas("cRecoMuonShowerNCorrelatedHitsInStation4", 0, 21, 1, false, false, hRecoMuonShowerNCorrelatedHitsInStation4);
  CreateAndWriteCanvas("cSelMuonShowerNCorrelatedHitsInStation4", 0, 21, 1, false, false, hSelMuonShowerNCorrelatedHitsInStation4);
  CreateAndWriteCanvas("cRecoMuonShowerNHitsInStation1", 0, 21, 1, false, false, hRecoMuonShowerNHitsInStation1);
  CreateAndWriteCanvas("cSelMuonShowerNHitsInStation1", 0, 21, 1, false, false, hSelMuonShowerNHitsInStation1);
  CreateAndWriteCanvas("cRecoMuonShowerNHitsInStation2", 0, 21, 1, false, false, hRecoMuonShowerNHitsInStation2);
  CreateAndWriteCanvas("cSelMuonShowerNHitsInStation2", 0, 21, 1, false, false, hSelMuonShowerNHitsInStation2);
  CreateAndWriteCanvas("cRecoMuonShowerNHitsInStation3", 0, 21, 1, false, false, hRecoMuonShowerNHitsInStation3);
  CreateAndWriteCanvas("cSelMuonShowerNHitsInStation3", 0, 21, 1, false, false, hSelMuonShowerNHitsInStation3);
  CreateAndWriteCanvas("cRecoMuonShowerNHitsInStation4", 0, 21, 1, false, false, hRecoMuonShowerNHitsInStation4);
  CreateAndWriteCanvas("cSelMuonShowerNHitsInStation4", 0, 21, 1, false, false, hSelMuonShowerNHitsInStation4);
  
  /*
  CreateAndWriteCanvas("cRecoMuon", 0, 21, 1, false, false, hRecoMuon);
  CreateAndWriteCanvas("cSelMuon", 0, 21, 1, false, false, hSelMuon);
  */
  
  CreateAndWriteCanvas("cRecoDiMuonInvMass",0, 21, 1, false, false, hRecoDiMuonInvMass);
  CreateAndWriteCanvas("cSelDiMuonInvMass",0, 21, 1, false, false, hSelDiMuonInvMass);
  CreateAndWriteCanvas("cRecoDiMuonDeltaR",0, 21, 1, false, false, hRecoDiMuonDeltaR);
  CreateAndWriteCanvas("cSelDiMuonDeltaR",0, 21, 1, false, false, hSelDiMuonDeltaR);
  CreateAndWriteCanvas("cRecoDiMuonDeltaEta",0, 21, 1, false, false, hRecoDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelDiMuonDeltaEta",0, 21, 1, false, false, hSelDiMuonDeltaEta);
  CreateAndWriteCanvas("cRecoDiMuonDeltaPhi",0, 21, 1, false, false, hRecoDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelDiMuonDeltaPhi",0, 21, 1, false, false, hSelDiMuonDeltaPhi);
  CreateAndWriteCanvas("cRecoDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hRecoDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cRecoDiMuonInvMassVsDeltaR","colz", false, false,false, hRecoDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelDiMuonInvMassVsDeltaR","colz", false, false,false, hSelDiMuonInvMassVsDeltaR);
  
  CreateAndWriteCanvas("cRecoDSDiMuonInvMass",0, 21, 1, false, false, hRecoDSDiMuonInvMass);
  CreateAndWriteCanvas("cSelDSDiMuonInvMass",0, 21, 1, false, false, hSelDSDiMuonInvMass);
  CreateAndWriteCanvas("cRecoDSDiMuonDeltaR",0, 21, 1, false, false, hRecoDSDiMuonDeltaR);
  CreateAndWriteCanvas("cSelDSDiMuonDeltaR",0, 21, 1, false, false, hSelDSDiMuonDeltaR);
  CreateAndWriteCanvas("cRecoDSDiMuonDeltaEta",0, 21, 1, false, false, hRecoDSDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelDSDiMuonDeltaEta",0, 21, 1, false, false, hSelDSDiMuonDeltaEta);
  CreateAndWriteCanvas("cRecoDSDiMuonDeltaPhi",0, 21, 1, false, false, hRecoDSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelDSDiMuonDeltaPhi",0, 21, 1, false, false, hSelDSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cRecoDSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hRecoDSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelDSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelDSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cRecoDSDiMuonInvMassVsDeltaR","colz", false, false,false, hRecoDSDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelDSDiMuonInvMassVsDeltaR","colz", false, false,false, hSelDSDiMuonInvMassVsDeltaR);
  
  CreateAndWriteCanvas("cRecoSSDiMuonInvMass",0, 21, 1, false, false, hRecoSSDiMuonInvMass);
  CreateAndWriteCanvas("cSelSSDiMuonInvMass",0, 21, 1, false, false, hSelSSDiMuonInvMass);
  CreateAndWriteCanvas("cRecoSSDiMuonDeltaR",0, 21, 1, false, false, hRecoSSDiMuonDeltaR);
  CreateAndWriteCanvas("cSelSSDiMuonDeltaR",0, 21, 1, false, false, hSelSSDiMuonDeltaR);
  CreateAndWriteCanvas("cRecoSSDiMuonDeltaEta",0, 21, 1, false, false, hRecoSSDiMuonDeltaEta);
  CreateAndWriteCanvas("cSelSSDiMuonDeltaEta",0, 21, 1, false, false, hSelSSDiMuonDeltaEta);
  CreateAndWriteCanvas("cRecoSSDiMuonDeltaPhi",0, 21, 1, false, false, hRecoSSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cSelSSDiMuonDeltaPhi",0, 21, 1, false, false, hSelSSDiMuonDeltaPhi);
  CreateAndWriteCanvas("cRecoSSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hRecoSSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cSelSSDiMuonDeltaEtaVsDeltaPhi","colz", false, false,false, hSelSSDiMuonDeltaEtaVsDeltaPhi);
  CreateAndWriteCanvas("cRecoSSDiMuonInvMassVsDeltaR","colz", false, false,false, hRecoSSDiMuonInvMassVsDeltaR);
  CreateAndWriteCanvas("cSelSSDiMuonInvMassVsDeltaR","colz", false, false,false, hSelSSDiMuonInvMassVsDeltaR);
  
  CreateAndWriteCanvas("cSelMuonDxy",0, 21, 1, false, true, hSelMuonDxy);
  CreateAndWriteCanvas("cSelMuonDxyError",0, 21, 1, false, true, hSelMuonDxyError);
  CreateAndWriteCanvas("cSelMuonDxyz",0, 21, 1, false, true, hSelMuonDxyz);
  CreateAndWriteCanvas("cSelMuonDxyzError",0, 21, 1, false, true, hSelMuonDxyzError);
  CreateAndWriteCanvas("cSelMuonDz",0, 21, 1, false, true, hSelMuonDz);
  CreateAndWriteCanvas("cSelMuonAbsDz",0, 21, 1, false, true, hSelMuonAbsDz);
  CreateAndWriteCanvas("cSelMuonDzError",0, 21, 1, false, true, hSelMuonDzError);
  CreateAndWriteCanvas("cSelMuonDxyVsRunNumber", "colz", false, false, true, hSelMuonDxyVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDxyErrorVsRunNumber", "colz", false, false, true, hSelMuonDxyErrorVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDxyzVsRunNumber", "colz", false, false, true, hSelMuonDxyzVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDxyzErrorVsRunNumber", "colz", false, false, true, hSelMuonDxyzErrorVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDzVsRunNumber", "colz", false, false, true, hSelMuonDzVsRunNumber);
  CreateAndWriteCanvas("cSelMuonAbsDzVsRunNumber", "colz", false, false, true, hSelMuonAbsDzVsRunNumber);
  CreateAndWriteCanvas("cSelMuonDzErrorVsRunNumber", "colz", false, false, true, hSelMuonDzErrorVsRunNumber);
  CreateAndWriteCanvas("cpxcSelMuonDxyVsRunNumber",0, 21, 1, false, false, hpxSelMuonDxyVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDxyErrorVsRunNumber",0, 21, 1, false, false, hpxSelMuonDxyErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDxyzVsRunNumber",0, 21, 1, false, false, hpxSelMuonDxyzVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDxyzErrorVsRunNumber",0, 21, 1, false, false, hpxSelMuonDxyzErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDzVsRunNumber",0, 21, 1, false, false, hpxSelMuonDzVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonAbsDzVsRunNumber",0, 21, 1, false, false, hpxSelMuonAbsDzVsRunNumber);
  CreateAndWriteCanvas("cpxSelMuonDzErrorVsRunNumber",0, 21, 1, false, false, hpxSelMuonDzErrorVsRunNumber);

  CreateAndWriteCanvas("cSelMuonSignedDxyBS",0, 21, 1, false, true, hSelMuonSignedDxyBS);
  CreateAndWriteCanvas("cSelMuonDxyBSError",0, 21, 1, false, true, hSelMuonDxyBSError);
  CreateAndWriteCanvas("cSelMuonSignedDxyBSVsPhi", "colz", false, false, true, hSelMuonSignedDxyBSVsPhi);
  CreateAndWriteCanvas("cSelMuonDxyBSErrorVsPhi", "colz", false, false, true, hSelMuonDxyBSErrorVsPhi);
  
  CreateAndWriteCanvas("cNRecoMuons",0, 21, 1, false, true, hNRecoMuons);
  CreateAndWriteCanvas("cNSelMuons",0, 21, 1, false, true, hNSelMuons);
  CreateAndWriteCanvas("cNTriggeringRecoMuons",0, 21, 1, false, true, hNTriggeringRecoMuons);
  CreateAndWriteCanvas("cNTriggeringSelMuons",0, 21, 1, false, true, hNTriggeringSelMuons);
  CreateAndWriteCanvas("cNRecoMuonsVsRunNumber", "colz", false, false, true, hNRecoMuonsVsRunNumber);
  CreateAndWriteCanvas("cNSelMuonsVsRunNumber", "colz", false, false, true, hNSelMuonsVsRunNumber);
  CreateAndWriteCanvas("cpxNRecoMuonsVsRunNumber", 0, 21, 1, false, false, hpxNRecoMuonsVsRunNumber);
  CreateAndWriteCanvas("cpxNSelMuonsVsRunNumber", 0, 21, 1, false, false, hpxNSelMuonsVsRunNumber);
  
  CreateAndWriteCanvas("cRecoVerticesX",0, 21, 1, false, true, hRecoVerticesX);
  CreateAndWriteCanvas("cSelVerticesX",0, 21, 1, false, true, hSelVerticesX);
  CreateAndWriteCanvas("cRecoVerticesXError",0, 21, 1, false, true, hRecoVerticesXError);
  CreateAndWriteCanvas("cSelVerticesXError",0, 21, 1, false, true, hSelVerticesXError);
  CreateAndWriteCanvas("cRecoVerticesXErrorVsRunNumber","colz", false, false, true, hRecoVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cSelVerticesXErrorVsRunNumber","colz", false, false, true, hSelVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cpxRecoVerticesXErrorVsRunNumber",0, 21, 1, false, false, hpxRecoVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelVerticesXErrorVsRunNumber",0, 21, 1, false, false, hpxSelVerticesXErrorVsRunNumber);
  CreateAndWriteCanvas("cRecoVerticesY",0, 21, 1, false, true, hRecoVerticesY);
  CreateAndWriteCanvas("cSelVerticesY",0, 21, 1, false, true, hSelVerticesY);
  CreateAndWriteCanvas("cRecoVerticesYError",0, 21, 1, false, true, hRecoVerticesYError);
  CreateAndWriteCanvas("cSelVerticesYError",0, 21, 1, false, true, hSelVerticesYError);
  CreateAndWriteCanvas("cRecoVerticesYErrorVsRunNumber","colz", false, false, true, hRecoVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cSelVerticesYErrorVsRunNumber","colz", false, false, true, hSelVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cpxRecoVerticesYErrorVsRunNumber",0, 21, 1, false, false, hpxRecoVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelVerticesYErrorVsRunNumber",0, 21, 1, false, false, hpxSelVerticesYErrorVsRunNumber);
  CreateAndWriteCanvas("cRecoVerticesZ",0, 21, 1, false, true, hRecoVerticesZ);
  CreateAndWriteCanvas("cSelVerticesZ",0, 21, 1, false, true, hSelVerticesZ);
  CreateAndWriteCanvas("cRecoVerticesZError",0, 21, 1, false, true, hRecoVerticesZError);
  CreateAndWriteCanvas("cSelVerticesZError",0, 21, 1, false, true, hSelVerticesZError);
  CreateAndWriteCanvas("cRecoVerticesZErrorVsRunNumber","colz", false, false, true, hRecoVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cSelVerticesZErrorVsRunNumber","colz", false, false, true, hSelVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cpxRecoVerticesZErrorVsRunNumber",0, 21, 1, false, false, hpxRecoVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cpxSelVerticesZErrorVsRunNumber",0, 21, 1, false, false, hpxSelVerticesZErrorVsRunNumber);
  CreateAndWriteCanvas("cRecoVerticesNDOF",0, 21, 1, false, false, hRecoVerticesNDOF);
  CreateAndWriteCanvas("cSelVerticesNDOF",0, 21, 1, false, false, hSelVerticesNDOF);
  CreateAndWriteCanvas("cRecoVerticesNormalizedChi2",0, 21, 1, false, false, hRecoVerticesNormalizedChi2);
  CreateAndWriteCanvas("cSelVerticesNormalizedChi2",0, 21, 1, false, false, hSelVerticesNormalizedChi2);
  CreateAndWriteCanvas("cRecoVerticesTrkMultiplicity",0, 21, 1, false, false, hRecoVerticesTrkMultiplicity);
  CreateAndWriteCanvas("cSelVerticesTrkMultiplicity",0, 21, 1, false, false, hSelVerticesTrkMultiplicity);

  CreateAndWriteCanvas("cNRecoVertices",0, 21, 1, false, true, hNRecoVertices);
  CreateAndWriteCanvas("cNSelVertices",0, 21, 1, false, true, hNSelVertices);
  CreateAndWriteCanvas("cNRecoVerticesVsRunNumber", "colz", false, false, true, hNRecoVerticesVsRunNumber);
  CreateAndWriteCanvas("cNSelVerticesVsRunNumber", "colz", false, false, true, hNSelVerticesVsRunNumber);
  CreateAndWriteCanvas("cpxNRecoVerticesVsRunNumber", 0, 21, 1, false, false, hpxNRecoVerticesVsRunNumber);
  CreateAndWriteCanvas("cpxNSelVerticesVsRunNumber", 0, 21, 1, false, false, hpxNSelVerticesVsRunNumber);

  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // MuonBasicDistributionsDataForBmmWithCuts_cxx
