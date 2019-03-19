#ifndef MuonBasicDistributionsDataForBmm_cxx
#define MuonBasicDistributionsDataForBmm_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "MuonBasicDistributionsDataForBmm.h"
#include "../utilities/constants.h"



void MuonBasicDistributionsDataForBmm::Loop()
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
  
  std::map<std::string,muon::SelectionType> mSelectionType = s->GetMuonSelectionTypeMap();
  
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
  
  std::string sBinning = "";
  if(s->GetJetSelector() != "" && !s->JetSelectorIsValid())
  {
    sBinning = s->GetJetSelector();
  }
  
  std::string sBinningVariable = GetBinningVariableString(sBinning);
  std::vector<double> vLowBins = GetBins(sBinning);
  
  std::cout << "Binning on " << sBinningVariable << std::endl;
  for(unsigned int iBin = 0; iBin != vLowBins.size(); iBin++)
  {
    std::cout << "Bin " << iBin << " has lower limit " << vLowBins[iBin] << std::endl;
  }
  
  unsigned short binningVariableIndex = 0;
  if(sBinningVariable == "Pt")
  {
    binningVariableIndex = 1;
  }
  if(sBinningVariable == "Eta")
  {
    binningVariableIndex = 2;
  }
  if(sBinningVariable == "Charge")
  {
    binningVariableIndex = 3;
  }

  // This is the one that makes more sense for data
  const std::string effMethod = "MCTruthBB";
  
  const char* nMuLabel = "N_{#mu}";
  const char* nVtxLabel = "N_{vtx}";
  const char* nEvLabel = "N_{ev}";
  
  // Book histograms here
  std::vector<TH1D*>* hSelMuonDeltaRFromGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonDeltaRFromGenp","#Delta(R) of gen particle from closest matching selected muon", 100, 0., 1., "#Delta(R)", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonPt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPt","p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPt = CreateVectorOf1DHistograms<TH1D>("hSelMuonPt","p_{T} of selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hSelMuonLeadingPt = CreateVectorOf1DHistograms<TH1D>("hSelMuonLeadingPt","p_{T} of leading selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonSecondPt = CreateVectorOf1DHistograms<TH1D>("hSelMuonSecondPt","p_{T} of 2nd leading selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonEta = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEta","#eta of reconstructed muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEta = CreateVectorOf1DHistograms<TH1D>("hSelMuonEta","#eta of selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hSelMuonLeadingEta = CreateVectorOf1DHistograms<TH1D>("hSelMuonLeadingEta","#eta of leading-p_{T} selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonSecondEta = CreateVectorOf1DHistograms<TH1D>("hSelMuonSecondEta","#eta of second leading-p_{T} selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonPhi = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPhi","#phi of reconstructed muons", nPhiBins, phiMin, phiMax, recoPhiLabel, nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPhi = CreateVectorOf1DHistograms<TH1D>("hSelMuonPhi","#phi of selected muons", nPhiBins, phiMin, phiMax, recoPhiLabel, nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonCharge = CreateVectorOf1DHistograms<TH1D>("hRecoMuonCharge","Charge of reconstructed muons", 3, -1.5, 1.5, "Charge", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonCharge = CreateVectorOf1DHistograms<TH1D>("hSelMuonCharge","Charge of selected muons", 3, -1.5, 1.5, "Charge", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonHasTriggered = CreateVectorOf1DHistograms<TH1D>("hRecoMuonHasTriggered","Trigger matching of reconstructed muons", 2, -0.5, 1.5, "Has triggered", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonHasTriggered = CreateVectorOf1DHistograms<TH1D>("hSelMuonHasTriggered","Trigger matching of selected muons", 2, -0.5, 1.5, "Has triggered", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonCaloCompatibility = CreateVectorOf1DHistograms<TH1D>("hRecoMuonCaloCompatibility","Calo compatibility of reconstructed muons", 25, 0., 1., "Calo compatibility", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonCaloCompatibility = CreateVectorOf1DHistograms<TH1D>("hSelMuonCaloCompatibility","Calo compatibility of selected muons", 25, 0., 1., "Calo compatibility", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonSegmentCompatibility = CreateVectorOf1DHistograms<TH1D>("hRecoMuonSegmentCompatibility","Segment compatibility of reconstructed muons", 100, 0., 1., "Segment compatibility", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonSegmentCompatibility = CreateVectorOf1DHistograms<TH1D>("hSelMuonSegmentCompatibility","Segment compatibility of selected muons", 100, 0., 1., "Segment compatibility", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonTrackValidHits = CreateVectorOf1DHistograms<TH1D>("hRecoMuonTrackValidHits","N. of valid hits in inner track of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonTrackValidHits = CreateVectorOf1DHistograms<TH1D>("hSelMuonTrackValidHits","N. of valid hits in inner track of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonTrackChi2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonTrackChi2","#chi^{2}/ndof in inner track of reconstructed muons", 50, 0., 5., "#chi^{2}/ndof", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonTrackChi2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonTrackChi2","#chi^{2}/ndof in inner track of selected muons", 50, 0., 5., "#chi^{2}/ndof", nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hRecoMuonTrackNdof = CreateVectorOf1DHistograms<TH1D>("hRecoMuonTrackNdof","ndof in inner track of reconstructed muons", 51, -0.5, 50.5, "ndof", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonTrackNdof = CreateVectorOf1DHistograms<TH1D>("hSelMuonTrackNdof","ndof in inner track of selected muons", 51, -0.5, 50.5, "ndof", nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hRecoMuonTrackLayersWithMeasurement = CreateVectorOf1DHistograms<TH1D>("hRecoMuonTrackLayersWithMeasurement","Layers with measurement in inner track of reconstructed muons", 21, -0.5, 20.5, "N_{layers}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonTrackLayersWithMeasurement = CreateVectorOf1DHistograms<TH1D>("hSelMuonTrackLayersWithMeasurement","Layers with measurement in inner track of selected muons", 21, -0.5, 20.5, "N_{layers}", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonTrackValidPixelHits = CreateVectorOf1DHistograms<TH1D>("hRecoMuonTrackValidPixelHits","N. of valid pixel hits in inner track of reconstructed muons", 11, -0.5, 10.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonTrackValidPixelHits = CreateVectorOf1DHistograms<TH1D>("hSelMuonTrackValidPixelHits","N. of valid pixel hits in inner track of selected muons", 11, -0.5, 10.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hRecoMuonGlobalChi2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalChi2","#chi^{2}/ndof in global track of reconstructed muons", 100, 0., 10., "#chi^{2}/ndof", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalChi2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalChi2","#chi^{2}/ndof in global track of selected muons", 100, 0., 10., "#chi^{2}/ndof", nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hRecoMuonGlobalValidMuonHits = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalValidMuonHits","N. of valid muon hits in global track of reconstructed muons", 51, -0.5, 50.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalValidMuonHits = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalValidMuonHits","N. of valid muon hits in global track of selected muons", 51, -0.5, 50.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hRecoMuonNumberOfMatches = CreateVectorOf1DHistograms<TH1D>("hRecoMuonNumberOfMatches","N. of matches of reconstructed muons", 11, -0.5, 10.5, "N_{matches}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonNumberOfMatches = CreateVectorOf1DHistograms<TH1D>("hSelMuonNumberOfMatches","N. of matches of selected muons", 11, -0.5, 10.5, "N_{matches}", nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hRecoMuonNumberOfMatchedStations = CreateVectorOf1DHistograms<TH1D>("hRecoMuonNumberOfMatchedStations","Number of matched stations of reconstructed muons", 11, -0.5, 10.5, "N_{stations}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonNumberOfMatchedStations = CreateVectorOf1DHistograms<TH1D>("hSelMuonNumberOfMatchedStations","Number of matched stations of selected muons", 11, -0.5, 10.5, "N_{stations}", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonStationsWithHits = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStationsWithHits","Stations with hits of reconstructed muons", 8, 0.5, 8.5, "Station", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStationsWithHits = CreateVectorOf1DHistograms<TH1D>("hSelMuonStationsWithHits","Stations with hits of selected muons", 8, 0.5, 8.5, "Station", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDTStationsWithHits = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDTStationsWithHits","DT stations with hits of reconstructed muons", 4, 0.5, 4.5, "DT Station", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDTStationsWithHits = CreateVectorOf1DHistograms<TH1D>("hSelMuonDTStationsWithHits","DT stations with hits of selected muons", 4, 0.5, 4.5, "DT Station", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonCSCStationsWithHits = CreateVectorOf1DHistograms<TH1D>("hRecoMuonCSCStationsWithHits","CSC stations with hits of reconstructed muons", 4, 0.5, 4.5, "CSC Station", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonCSCStationsWithHits = CreateVectorOf1DHistograms<TH1D>("hSelMuonCSCStationsWithHits","CSC stations with hits of selected muons", 4, 0.5, 4.5, "CSC Station", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonStationsWithHitsMerged = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStationsWithHitsMerged","Stations with hits of reconstructed muons (DT and CSC merged)", 4, 0.5, 4.5, "Station", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStationsWithHitsMerged = CreateVectorOf1DHistograms<TH1D>("hSelMuonStationsWithHitsMerged","Stations with hits of selected muons (DT and CSC merged)", 4, 0.5, 4.5, "Station", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonKink = CreateVectorOf1DHistograms<TH1D>("hRecoMuonKink","Inner track kink of reconstructed muons", 100, 0., 50., "Kink value", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonKink = CreateVectorOf1DHistograms<TH1D>("hSelMuonKink","Inner track kink of selected muons", 100, 0., 50., "Kink value", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonKinkRho = CreateVectorOf1DHistograms<TH1D>("hRecoMuonKinkRho","#rho of inner track kink of reconstructed muons", 100, 0., 100., "Kink #rho", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonKinkRho = CreateVectorOf1DHistograms<TH1D>("hSelMuonKinkRho","#rho of inner track kink of selected muons", 100, 0., 100., "Kink #rho", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonKinkZ = CreateVectorOf1DHistograms<TH1D>("hRecoMuonKinkZ","Z of inner track kink of reconstructed muons", 100, -300., 300., "Kink Z", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonKinkZ = CreateVectorOf1DHistograms<TH1D>("hSelMuonKinkZ","Z of inner track kink of selected muons", 100, -300., 300., "Kink Z", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonGlobalKink = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKink","Global track kink of reconstructed muons", 100, 0., 100000., "Kink value", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKink = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKink","Global track kink of selected muons", 100, 0., 100000., "Kink value", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonGlobalKinkRho = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkRho","#rho of global track kink of reconstructed muons", 100, 0., 100., "Kink #rho", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkRho = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkRho","#rho of global track kink of selected muons", 100, 0., 100., "Kink #rho", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonGlobalKinkZ = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkZ","Z of global track kink of reconstructed muons", 100, -300., 300., "Kink Z", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkZ = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkZ","Z of global track kink of selected muons", 100, -300., 300., "Kink Z", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonGlobalKinkChi2LocalMomentum = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkChi2LocalMomentum","#chi^{2} of local momenta of STA-TK match of reconstructed muons", 100, 0., 50., "Local p #chi^{2}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkChi2LocalMomentum = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkChi2LocalMomentum","#chi^{2} of local momenta of STA-TK match of selected muons", 100, 0., 50., "Local p #chi^{2}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonGlobalKinkChi2LocalPosition = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkChi2LocalPosition","#chi^{2} of local positions of STA-TK match of reconstructed muons", 100, 0., 50., "Local position #chi^{2}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkChi2LocalPosition = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkChi2LocalPosition","#chi^{2} of local positions of STA-TK match of selected muons", 100, 0., 50., "Local position #chi^{2}", nMuLabel,sBinningVariable,vLowBins);
  
  TH2D* hRecoMuonChi2LocalMomentumVsChi2LocalPosition = Create2DHistogram<TH2D>("hRecoMuonChi2LocalMomentumVsChi2LocalPosition", "#chi^{2} of local momenta vs. #chi^{2} of local positions of STA-TK match of reconstructed muons",  100, 0., 50., 100, 0., 50., "Local position #chi^{2}", "Local p #chi^{2}");
  TH2D* hSelMuonChi2LocalMomentumVsChi2LocalPosition = Create2DHistogram<TH2D>("hSelMuonChi2LocalMomentumVsChi2LocalPosition", "#chi^{2} of local momenta vs. #chi^{2} of local positions of STA-TK match of selected muons",  100, 0., 50., 100, 0., 50., "Local position #chi^{2}", "Local p #chi^{2}");
  

  std::vector<TH1D*>* hRecoMuonGlbTrackProbability = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlbTrackProbability","-ln(Probability) of global track of reconstructed muons", 100, 0., 10., "-ln(Probability)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlbTrackProbability = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlbTrackProbability","-ln(Probability) of global track of selected muons", 100, 0., 10., "-ln(Probability)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonGlobalKinkGlobalDeltaEtaPhi = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkGlobalDeltaEtaPhi","#Delta(#eta,#phi) of STA-TK match of reconstructed muons", 100, 0., 0.3, "#Delta(#eta,#phi)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkGlobalDeltaEtaPhi = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkGlobalDeltaEtaPhi","#Delta(#eta,#phi) of STA-TK match of selected muons", 100, 0., 0.3, "#Delta(#eta,#phi)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonGlobalKinkLocalDistance = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkLocalDistance","Local distance of STA-TK match of reconstructed muons", 100, 0., 50., "Distance [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkLocalDistance = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkLocalDistance","Local distance of STA-TK match of selected muons", 100, 0., 50., "Distance [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonGlobalKinkStaRelChi2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkStaRelChi2","#chi^{2}(STA track) over #chi^{2}(GLB track) of reconstructed muons", 100, 0., 10., "#chi^{2}(STA)/#chi^{2}(GLB)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkStaRelChi2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkStaRelChi2","#chi^{2}(STA track) over #chi^{2}(GLB track) of selected muons", 100, 0., 10., "#chi^{2}(STA)/#chi^{2}(GLB)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonGlobalKinkTightMatch = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkTightMatch","Tight match of STA-TK tracks of reconstructed muons", 2, -0.5, 1.5, "Tight match", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkTightMatch = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkTightMatch","Tight match of STA-TK tracks of selected muons", 2, -0.5, 1.5, "Tight match", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonGlobalKinkTrkRelChi2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonGlobalKinkTrkRelChi2","#chi^{2}(TK track) over #chi^{2}(GLB track) of reconstructed muons", 100, 0., 3., "#chi^{2}(TK)/#chi^{2}(GLB)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonGlobalKinkTrkRelChi2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonGlobalKinkTrkRelChi2","#chi^{2}(TK track) over #chi^{2}(GLB track) of selected muons", 100, 0., 3., "#chi^{2}(TK)/#chi^{2}(GLB)", nMuLabel,sBinningVariable,vLowBins);

  TH2D* hRecoMuonStaRelChi2VsTrkRelChi2 = Create2DHistogram<TH2D>("hRecoMuonStaRelChi2VsTrkRelChi2", "#chi^{2}(STA)/#chi^{2}(GLB) vs. #chi^{2}(TK)/#chi^{2}(GLB)  of reconstructed muons",  100, 0., 3., 100, 0., 10., "#chi^{2}(TK)/#chi^{2}(GLB)", "#chi^{2}(STA)/#chi^{2}(GLB)");
  TH2D* hSelMuonStaRelChi2VsTrkRelChi2 = Create2DHistogram<TH2D>("hSelMuonStaRelChi2VsTrkRelChi2", "#chi^{2}(STA)/#chi^{2}(GLB) vs. #chi^{2}(TK)/#chi^{2}(GLB)  of selected muons",  100, 0., 3., 100, 0., 10., "#chi^{2}(TK)/#chi^{2}(GLB)", "#chi^{2}(STA)/#chi^{2}(GLB)");
    
  std::vector<TH1D*>* hRecoMuonEnergyEcalTime = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyEcalTime","ECAL time of reconstructed muons", 100, -100., 100., "ECAL time", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyEcalTime = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyEcalTime","ECAL time of reconstructed muons", 100, -100., 100., "ECAL time", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyEcalTimeError = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyEcalTimeError","ECAL time error of reconstructed muons", 100, 0., 1., "ECAL time", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyEcalTimeError = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyEcalTimeError","ECAL time error of selected muons", 100, 0., 1., "ECAL time", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyEm = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyEm","ECAL energy in crossed crystals of reconstructed muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyEm = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyEm","ECAL energy in crossed crystals of selected muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyEmMax = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyEmMax","Max ECAL energy in 5x5 crystals around reconstructed muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyEmMax = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyEmMax","Max ECAL energy in 5x5 crystals around selected muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyEmS25 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyEmS25","ECAL energy deposited in 5x5 crystals around reconstructed muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyEmS25 = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyEmS25","ECAL energy deposited in 5x5 crystals around selected muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyEmS9 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyEmS9","ECAL energy deposited in 3x3 crystals around reconstructed muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyEmS9 = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyEmS9","ECAL energy deposited in 3x3 crystals around selected muons", 50, 0., 10., "ECAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyHcalTime = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyHcalTime","HCAL time of reconstructed muons", 100, -100., 100., "HCAL time", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyHcalTime = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyHcalTime","HCAL time of reconstructed muons", 100, -100., 100., "HCAL time", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyHcalTimeError = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyHcalTimeError","HCAL time error of reconstructed muons", 100, 0., 1., "HCAL time", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyHcalTimeError = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyHcalTimeError","HCAL time error of selected muons", 100, 0., 1., "HCAL time", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyHad = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyHad","HCAL energy in crossed towers of reconstructed muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyHad = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyHad","HCAL energy in crossed towers of selected muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyHadMax = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyHadMax","Max HCAL energy in 3x3 towers around reconstructed muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyHadMax = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyHadMax","Max HCAL energy in 3x3 towers around selected muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyHadS9 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyHadS9","HCAL energy in 3x3 towers around reconstructed muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyHadS9 = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyHadS9","HCAL energy in 3x3 towers around selected muons", 50, 0., 20., "HCAL energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyTower = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyTower","Total calo energy in crossed towers of reconstructed muons", 50, 0., 20., "Calo energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyTower = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyTower", "Total calo energy in crossed towers of selected muons", 50, 0., 20., "Calo energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEnergyTowerS9 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEnergyTowerS9","Total calo energy in 3x3 towers around reconstructed muons", 50, 0., 20., "Calo energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEnergyTowerS9 = CreateVectorOf1DHistograms<TH1D>("hSelMuonEnergyTowerS9", "Total calo energy in 3x3 towers around selected muons", 50, 0., 20., "Calo energy [GeV]", nMuLabel,sBinningVariable,vLowBins);
  
  
  std::vector<TH1D*>* hRecoMuonIsolationR03EmEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03EmEt", "Electromagnetic #Sigma(E_{T}) in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03EmEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03EmEt", "Electromagnetic #Sigma(E_{T}) in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03EmVetoEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03EmVetoEt", "Electromagnetic #Sigma(E_{T}) in the veto cone around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03EmVetoEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03EmVetoEt", "Electromagnetic #Sigma(E_{T}) in the veto cone around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03EmMinusVetoEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03EmMinusVetoEt", "Electromagnetic #Sigma(E_{T}) in the cone of R=0.3 minus veto around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03EmMinusVetoEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03EmMinusVetoEt", "Electromagnetic #Sigma(E_{T}) in the cone of R=0.3 minus veto around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03HadEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03HadEt", "Hadronic #Sigma(E_{T}) in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03HadEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03HadEt", "Hadronic #Sigma(E_{T}) in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03HadVetoEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03HadVetoEt", "Hadronic #Sigma(E_{T}) in the veto cone around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03HadVetoEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03HadVetoEt", "Hadronic #Sigma(E_{T}) in the veto cone around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03HadMinusVetoEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03HadMinusVetoEt", "Hadronic #Sigma(E_{T}) in the cone of R=0.3 minus veto around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03HadMinusVetoEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03HadMinusVetoEt", "Hadronic #Sigma(E_{T}) in the cone of R=0.3 minus veto around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03HoEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03HoEt", "HO #Sigma(E_{T}) in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03HoEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03HoEt", "HO #Sigma(E_{T}) in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03HoVetoEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03HoVetoEt", "HO #Sigma(E_{T}) in the veto cone around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03HoVetoEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03HoVetoEt", "HO #Sigma(E_{T}) in the veto cone around selected muons", 100, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03HoMinusVetoEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03HoMinusVetoEt", "HO #Sigma(E_{T}) in the cone of R=0.3 minus veto around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03HoMinusVetoEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03HoMinusVetoEt", "HO #Sigma(E_{T}) in the cone of R=0.3 minus veto around selected muons", 100, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03NJets = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03NJets", "N_{jets} in a cone of R=0.3 around reconstructed muons", 6, -0.5, 5.5, "N_{jets}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03NJets = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03NJets", "N_{jets} in a cone of R=0.3 around selected muons", 6, -0.5, 5.5, "N_{jets}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03NTracks = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03NTracks", "N_{tracks} in a cone of R=0.3 around reconstructed muons", 11, -0.5, 10.5, "N_{tracks}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03NTracks = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03NTracks", "N_{tracks} in a cone of R=0.3 around selected muons", 11, -0.5, 10.5, "N_{tracks}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03SumPt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03SumPt", "#Sigma(p_{T}) of tracks in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03SumPt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03SumPt", "#Sigma(p_{T}) of tracks in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonIsolationR03TrackerVetoPt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonIsolationR03TrackerVetoPt", "#Sigma(p_{T}) of tracks in the veto cone around reconstructed muons", 100, 0., 20., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonIsolationR03TrackerVetoPt = CreateVectorOf1DHistograms<TH1D>("hSelMuonIsolationR03TrackerVetoPt", "#Sigma(p_{T}) of tracks in the veto cone around selected muons", 100, 0., 20., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  
  TH2D* hRecoMuonCaloEtR03VsMuonPt =  Create2DHistogram<TH2D>("hRecoMuonCaloEtR03VsMuonPt", "Calo #Sigma(E_{T}) in a cone of R=0.3 vs. p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hSelMuonCaloEtR03VsMuonPt =  Create2DHistogram<TH2D>("hSelMuonCaloEtR03VsMuonPt", "Calo #Sigma(E_{T}) in a cone of R=0.3 vs. p_{T} of selected muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hRecoMuonCaloVetoEtR03VsMuonPt =  Create2DHistogram<TH2D>("hRecoMuonCaloVetoEtR03VsMuonPt", "Calo #Sigma(E_{T}) in the veto cone vs. p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hSelMuonCaloVetoEtR03VsMuonPt =  Create2DHistogram<TH2D>("hSelMuonCaloVetoEtR03VsMuonPt", "Calo #Sigma(E_{T}) in the veto cone vs. p_{T} of selected muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hRecoMuonCaloMinusVetoEtR03VsMuonPt =  Create2DHistogram<TH2D>("hRecoMuonCaloMinusVetoEtR03VsMuonPt", "Calo #Sigma(E_{T}) in a cone of R=0.3 minus veto vs. p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  TH2D* hSelMuonCaloMinusVetoEtR03VsMuonPt =  Create2DHistogram<TH2D>("hSelMuonCaloMinusVetoEtR03VsMuonPt", "Calo #Sigma(E_{T}) in a cone of R=0.3 minus veto vs. p_{T} of selected muons", nPtBins, ptMin, ptMax, 100, 0., 10., "p_{T} [GeV]", "#Sigma(E_{T}) [GeV]");
  
  std::vector<TH1D*>* hRecoMuonPfIsolationR03SumChargedHadronPt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPfIsolationR03SumChargedHadronPt","#Sigma(p_{T}) of charged hadrons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPfIsolationR03SumChargedHadronPt = CreateVectorOf1DHistograms<TH1D>("hSelMuonPfIsolationR03SumChargedHadronPt","#Sigma(p_{T}) of charged hadrons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPfIsolationR03SumChargedParticlePt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPfIsolationR03SumChargedParticlePt","#Sigma(p_{T}) of charged particles in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPfIsolationR03SumChargedParticlePt = CreateVectorOf1DHistograms<TH1D>("hSelMuonPfIsolationR03SumChargedParticlePt","#Sigma(p_{T}) of charged particles in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonPfIsolationR03SumNeutralHadronEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPfIsolationR03SumNeutralHadronEt","#Sigma(E_{T}) of neutral hadrons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPfIsolationR03SumNeutralHadronEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonPfIsolationR03SumNeutralHadronEt","#Sigma(E_{T}) of neutral hadrons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold","#Sigma(E_{T}) (high threshold) of neutral hadrons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold = CreateVectorOf1DHistograms<TH1D>("hSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold","#Sigma(E_{T}) (high threshold) of neutral hadrons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPfIsolationR03SumPhotonEt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPfIsolationR03SumPhotonEt","#Sigma(E_{T}) of photons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPfIsolationR03SumPhotonEt = CreateVectorOf1DHistograms<TH1D>("hSelMuonPfIsolationR03SumPhotonEt","#Sigma(E_{T}) of photons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPfIsolationR03SumPhotonEtHighThreshold = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPfIsolationR03SumPhotonEtHighThreshold","#Sigma(E_{T}) (high threshold) of photons in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPfIsolationR03SumPhotonEtHighThreshold = CreateVectorOf1DHistograms<TH1D>("hSelMuonPfIsolationR03SumPhotonEtHighThreshold","#Sigma(E_{T}) (high threshold) of photons in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(E_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonPfIsolationR03SumPUPt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPfIsolationR03SumPUPt","#Sigma(p_{T}) of PU in a cone of R=0.3 around reconstructed muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPfIsolationR03SumPUPt = CreateVectorOf1DHistograms<TH1D>("hSelMuonPfIsolationR03SumPUPt","#Sigma(p_{T}) of PU in a cone of R=0.3 around selected muons", 50, 0., 10., "#Sigma(p_{T}) [GeV]", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonTrackIso = CreateVectorOf1DHistograms<TH1D>("hRecoMuonTrackIso","Tracker isolation of reconstructed muons", 50, 0., 10., "Isolation", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonTrackIso = CreateVectorOf1DHistograms<TH1D>("hSelMuonTrackIso","Tracker isolation of selected muons", 50, 0., 10., "Isolation", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonEcalIso = CreateVectorOf1DHistograms<TH1D>("hRecoMuonEcalIso","ECAL isolation of reconstructed muons", 50, 0., 10., "Isolation", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonEcalIso = CreateVectorOf1DHistograms<TH1D>("hSelMuonEcalIso","ECAL isolation of selected muons", 50, 0., 10., "Isolation", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonHcalIso = CreateVectorOf1DHistograms<TH1D>("hRecoMuonHcalIso","HCAL isolation of reconstructed muons", 50, 0., 10., "Isolation", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonHcalIso = CreateVectorOf1DHistograms<TH1D>("hSelMuonHcalIso","HCAL isolation of selected muons", 50, 0., 10., "Isolation", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonCaloIso = CreateVectorOf1DHistograms<TH1D>("hRecoMuonCaloIso","Calo isolation of reconstructed muons", 50, 0., 10., "Isolation", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonCaloIso = CreateVectorOf1DHistograms<TH1D>("hSelMuonCaloIso","Calo isolation of selected muons", 50, 0., 10., "Isolation", nMuLabel,sBinningVariable,vLowBins);
  
  std::vector<TH1D*>* hRecoMuonShowerNCorrelatedHitsInStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonShowerNCorrelatedHitsInStation1","Number of correlated hits in station 1 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonShowerNCorrelatedHitsInStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonShowerNCorrelatedHitsInStation1","Number of correlated hits in station 1 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);  
  std::vector<TH1D*>* hRecoMuonShowerNCorrelatedHitsInStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonShowerNCorrelatedHitsInStation2","Number of correlated hits in station 2 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonShowerNCorrelatedHitsInStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonShowerNCorrelatedHitsInStation2","Number of correlated hits in station 2 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonShowerNCorrelatedHitsInStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonShowerNCorrelatedHitsInStation3","Number of correlated hits in station 3 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonShowerNCorrelatedHitsInStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonShowerNCorrelatedHitsInStation3","Number of correlated hits in station 3 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonShowerNCorrelatedHitsInStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonShowerNCorrelatedHitsInStation4","Number of correlated hits in station 4 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonShowerNCorrelatedHitsInStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonShowerNCorrelatedHitsInStation4","Number of correlated hits in station 4 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hRecoMuonShowerNHitsInStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonShowerNHitsInStation1","Number of hits in station 1 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonShowerNHitsInStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonShowerNHitsInStation1","Number of hits in station 1 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);  
  std::vector<TH1D*>* hRecoMuonShowerNHitsInStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonShowerNHitsInStation2","Number of hits in station 2 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonShowerNHitsInStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonShowerNHitsInStation2","Number of hits in station 2 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonShowerNHitsInStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonShowerNHitsInStation3","Number of hits in station 3 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonShowerNHitsInStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonShowerNHitsInStation3","Number of hits in station 3 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonShowerNHitsInStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonShowerNHitsInStation4","Number of hits in station 4 of reconstructed muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonShowerNHitsInStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonShowerNHitsInStation4","Number of hits in station 4 of selected muons", 31, -0.5, 30.5, "N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  
  /*
  TH1D* hRecoMuon = Create1DHistogram<TH1D>("hRecoMuon","of reconstructed muons", 11, -0.5, 10.5, "", nMuLabel);
  TH1D* hSelMuon = Create1DHistogram<TH1D>("hSelMuon","of selected muons", 11, -0.5, 10.5, "", nMuLabel);
  */
  
  
  std::vector<TH1D*>* hSelMuonDxy = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxy", "d_{xy} of selected muons w.r.t. the selected vertex", 100, 0., 0.5, "d_{xy} [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxyError = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxyError", "#sigma(d_{xy}) of selected muons w.r.t. the selected vertex", 100, 0., 0.02, "#sigma(d_{xy}) [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxyz = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxyz", "d_{xyz} of selected muons w.r.t. the selected vertex", 100, 0., 0.5, "d_{xyz} [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxyzError = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxyzError", "#sigma(d_{xyz}) of selected muons w.r.t. the selected vertex", 100, 0., 0.02, "#sigma(d_{xyz}) [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonDz", "d_{z} of selected muons w.r.t. the selected vertex", 200, -0.5, 0.5, "d_{z} [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsDz", "abs(d_{z}) of selected muons w.r.t. the selected vertex", 100, 0., 0.5, "abs(d_{z}) [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDzError = CreateVectorOf1DHistograms<TH1D>("hSelMuonDzError", "#sigma(d_{z}) of selected muons w.r.t. the selected vertex", 100, 0., 0.01, "#sigma(d_{z}) [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonSignedDxyBS = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxyBS", "Signed d_{xy} of selected muons w.r.t. the beam spot", 200, -0.5, 0.5, "d_{xy} [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxyBSError = CreateVectorOf1DHistograms<TH1D>("hSelMuonSignedDxyBSError", "#sigma(d_{xy}) of selected muons w.r.t. the beam spot", 100, 0., 0.02, "#sigma(d_{xy}) [cm]", nMuLabel,sBinningVariable,vLowBins);
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
    for(unsigned int iGenParticle = 0; iGenParticle < Genp_Id->size(); iGenParticle++)
    {
      int genpId = Genp_Id->at(iGenParticle);
      if(abs(genpId) == trueGenpId)
      {
        int iMother = Genp_vMom->at(iGenParticle).at(0);
        int motherId = Genp_Id->at(iMother);
        if(abs(motherId) > 500 && abs(motherId) < 600 )
        {
//           std::cout << "Found a kaon or a pion coming from a B!\n";
//           std::cout << "I = " << iGenParticle << std::endl;
//           std::cout << "ID = " << genpId << std::endl;
//           std::cout << "iMother = " << iMother << std::endl;
//           std::cout << "motherId = " << motherId << std::endl;
          vParticlesFromB.push_back(iGenParticle);
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

    if(vParticlesFromB.size() < 2)
    {
      std::cout << "E R R O R ! Size of vector of B true daughters is less than 2!\n";
      std::cout << "            Size is " << vParticlesFromB.size() << ".\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
    std::vector<double> binningVariables;
    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      if(s->SelectMuon(iMuon))
      {
        short int trackRef = Muon_trackref->at(iMuon);
        binningVariables = GetBinningVariablesValues(iMuon);
        for(unsigned int iParticleFromB = 0; iParticleFromB < vParticlesFromB.size(); iParticleFromB++)
        {
          unsigned short iGenParticle = vParticlesFromB[iParticleFromB];
          std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticle(iGenParticle);
          
          if(iTrackWithDistance.first == trackRef /*&& iTrackWithDistance.second < 0.01*/)
          {
            Fill(hSelMuonDeltaRFromGenp,iTrackWithDistance.second,binningVariables[binningVariableIndex],vLowBins);
            if(iTrackWithDistance.second < 0.1)
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
    
    std::vector<double> binningValues;

    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      nRecoMuons++;
      double binningVariableValue = GetBinningVariablesValues(iMuon).at(binningVariableIndex);
      
      Fill(hRecoMuonPt,Muon_pt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEta,Muon_eta->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonPhi,Muon_phi->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonCharge,Muon_charge->at(iMuon), binningVariableValue,vLowBins);
      if(Muon_hasTriggered->at(iMuon))
      {
        nTriggeringRecoMuons++;
        Fill(hRecoMuonHasTriggered,1., binningVariableValue,vLowBins);
      }
      else
      {
        Fill(hRecoMuonHasTriggered,0., binningVariableValue,vLowBins);
      }
//       std::cout << "Sono qui 4.0\n";
      
      Fill(hRecoMuonCaloCompatibility,Muon_caloCompatibility->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonSegmentCompatibility,Muon_segmentCompatibility->at(iMuon), binningVariableValue,vLowBins);
      unsigned int iTrack = Muon_trackref->at(iMuon);
      if(iTrack != 65535 && iTrack != 2048)
      {
        Fill(hRecoMuonTrackValidHits,Track_numberOfValidHits->at(iTrack), binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackChi2,Track_normalizedChi2->at(iTrack), binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackNdof,Track_ndof->at(iTrack), binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackLayersWithMeasurement,Track_layersWithMeasurement->at(iTrack), binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackValidPixelHits,Track_validPixelHits->at(iTrack), binningVariableValue,vLowBins);
      }
      else
      {
        Fill(hRecoMuonTrackValidHits,0, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackChi2,9999, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackNdof,0, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackLayersWithMeasurement,0, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackValidPixelHits,0, binningVariableValue,vLowBins);
      }
      Fill(hRecoMuonGlobalChi2,Muon_globalChi2->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalValidMuonHits,Muon_globalValidMuonHits->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonNumberOfMatches,Muon_numberOfMatches->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonNumberOfMatchedStations,Muon_numberOfMatchedStations->at(iMuon), binningVariableValue,vLowBins);
      unsigned short stationMask = Muon_stationMask->at(iMuon);
      for(unsigned short iBit = 0; iBit < 8; iBit++)
      {
//         std::cout << "Eta: " << Muon_eta->at(iMuon) << " Station: " << iBit+1 << " value: " << (stationMask&(1<<iBit)) << std::endl;
        if((stationMask&(1<<iBit)) != 0)
        {
          Fill(hRecoMuonStationsWithHits,iBit+1, binningVariableValue,vLowBins);
          Fill(hRecoMuonStationsWithHitsMerged,(iBit%4)+1, binningVariableValue,vLowBins);
          if(iBit < 4)
          {
            Fill(hRecoMuonDTStationsWithHits,iBit+1, binningVariableValue,vLowBins);
          }
          else
          {
            Fill(hRecoMuonCSCStationsWithHits,(iBit-4)+1, binningVariableValue,vLowBins);
          }
        }
      }
      Fill(hRecoMuonKink,Muon_Kink->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonKinkRho,Muon_Kink_rho->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonKinkZ,Muon_Kink_z->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKink,Muon_GlobalKink->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkRho,Muon_GlobalKink_rho->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkZ,Muon_GlobalKink_z->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkChi2LocalMomentum,Muon_GlobalKink_chi2LocalMomentum->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkChi2LocalPosition,Muon_GlobalKink_chi2LocalPosition->at(iMuon), binningVariableValue,vLowBins);
      
      hRecoMuonChi2LocalMomentumVsChi2LocalPosition->Fill(Muon_GlobalKink_chi2LocalPosition->at(iMuon), Muon_GlobalKink_chi2LocalMomentum->at(iMuon));
      
      Fill(hRecoMuonGlbTrackProbability,Muon_glbTrackProbability->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkGlobalDeltaEtaPhi,Muon_GlobalKink_globalDeltaEtaPhi->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkLocalDistance,Muon_GlobalKink_localDistance->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkStaRelChi2,Muon_GlobalKink_staRelChi2->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkTightMatch,Muon_GlobalKink_tightMatch->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonGlobalKinkTrkRelChi2,Muon_GlobalKink_trkRelChi2->at(iMuon), binningVariableValue,vLowBins);
      
      hRecoMuonStaRelChi2VsTrkRelChi2->Fill(Muon_GlobalKink_trkRelChi2->at(iMuon), Muon_GlobalKink_staRelChi2->at(iMuon));
      Fill(hRecoMuonEnergyEcalTime,Muon_energy_ecalTime->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyEcalTimeError,Muon_energy_ecalTimeError->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyEm,Muon_energy_em->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyEmMax,Muon_energy_emMax->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyEmS25,Muon_energy_emS25->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyEmS9,Muon_energy_emS9->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyHcalTime,Muon_energy_hcalTime->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyHcalTimeError,Muon_energy_hcalTimeError->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyHad,Muon_energy_had->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyHadMax,Muon_energy_hadMax->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyHadS9,Muon_energy_hadS9->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyTower,Muon_energy_tower->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEnergyTowerS9,Muon_energy_towerS9->at(iMuon), binningVariableValue,vLowBins);
      
      Fill(hRecoMuonIsolationR03EmEt,Muon_isolationR03_emEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03EmVetoEt,Muon_isolationR03_emVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03EmMinusVetoEt,Muon_isolationR03_emEt->at(iMuon)-Muon_isolationR03_emVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03HadEt,Muon_isolationR03_hadEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03HadVetoEt,Muon_isolationR03_hadVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03HadMinusVetoEt,Muon_isolationR03_hadEt->at(iMuon)-Muon_isolationR03_hadVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03HoEt,Muon_isolationR03_hoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03HoVetoEt,Muon_isolationR03_hoVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03HoMinusVetoEt,Muon_isolationR03_hoEt->at(iMuon)-Muon_isolationR03_hoVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03NJets,Muon_isolationR03_nJets->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03NTracks,Muon_isolationR03_nTracks->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03SumPt,Muon_isolationR03_sumPt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonIsolationR03TrackerVetoPt,Muon_isolationR03_trackerVetoPt->at(iMuon), binningVariableValue,vLowBins);
      
      hRecoMuonCaloEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emEt->at(iMuon)+Muon_isolationR03_hadEt->at(iMuon));
      hRecoMuonCaloVetoEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emVetoEt->at(iMuon)+Muon_isolationR03_hadVetoEt->at(iMuon));
      hRecoMuonCaloMinusVetoEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emEt->at(iMuon)+Muon_isolationR03_hadEt->at(iMuon)-(Muon_isolationR03_emVetoEt->at(iMuon)+Muon_isolationR03_hadVetoEt->at(iMuon)));
      
      
      Fill(hRecoMuonPfIsolationR03SumChargedHadronPt,Muon_pfIsolationR03_sumChargedHadronPt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonPfIsolationR03SumChargedParticlePt,Muon_pfIsolationR03_sumChargedParticlePt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonPfIsolationR03SumNeutralHadronEt,Muon_pfIsolationR03_sumNeutralHadronEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold,Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonPfIsolationR03SumPhotonEt,Muon_pfIsolationR03_sumPhotonEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonPfIsolationR03SumPhotonEtHighThreshold,Muon_pfIsolationR03_sumPhotonEtHighThreshold->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonPfIsolationR03SumPUPt,Muon_pfIsolationR03_sumPUPt->at(iMuon), binningVariableValue,vLowBins);
      
      Fill(hRecoMuonTrackIso,Muon_trackIso->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonEcalIso,Muon_ecalIso->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonHcalIso,Muon_hcalIso->at(iMuon), binningVariableValue,vLowBins);
      Fill(hRecoMuonCaloIso,Muon_caloIso->at(iMuon), binningVariableValue,vLowBins);
      
      Fill(hRecoMuonShowerNCorrelatedHitsInStation1,Muon_shower_nStationCorrelatedHits->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonShowerNCorrelatedHitsInStation2,Muon_shower_nStationCorrelatedHits->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonShowerNCorrelatedHitsInStation3,Muon_shower_nStationCorrelatedHits->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonShowerNCorrelatedHitsInStation4,Muon_shower_nStationCorrelatedHits->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonShowerNHitsInStation1,Muon_shower_nStationHits->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonShowerNHitsInStation2,Muon_shower_nStationHits->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonShowerNHitsInStation3,Muon_shower_nStationHits->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonShowerNHitsInStation4,Muon_shower_nStationHits->at(iMuon).at(3), binningVariableValue,vLowBins);
      
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
        Fill(hSelMuonLeadingPt,ptSelMuon0,binningVariables[binningVariableIndex],vLowBins);
        Fill(hSelMuonLeadingEta,Muon_eta->at(vSelectedMuons[0]),binningVariables[binningVariableIndex],vLowBins);
        Fill(hSelMuonSecondPt,ptSelMuon1,binningVariables[binningVariableIndex],vLowBins);
        Fill(hSelMuonSecondEta,Muon_eta->at(vSelectedMuons[1]),binningVariables[binningVariableIndex],vLowBins);
      }
      else
      {
        Fill(hSelMuonLeadingPt,ptSelMuon1,binningVariables[binningVariableIndex],vLowBins);
        Fill(hSelMuonLeadingEta,Muon_eta->at(vSelectedMuons[1]),binningVariables[binningVariableIndex],vLowBins);
        Fill(hSelMuonSecondPt,ptSelMuon0,binningVariables[binningVariableIndex],vLowBins);
        Fill(hSelMuonSecondEta,Muon_eta->at(vSelectedMuons[0]),binningVariables[binningVariableIndex],vLowBins);
      }
    }
//     std::cout << "Sono qui 7\n";
    
    for(unsigned int i = 0; i < vSelectedMuons.size(); i++)
    {
      nSelMuons++;
      unsigned short iMuon = vSelectedMuons[i];
      double binningVariableValue = GetBinningVariablesValues(iMuon).at(binningVariableIndex);
//       bool selectorResult = s->SelectMuonPassingSelector(iMuon,muon::TMLastStationTight);
      bool selectorResult = s->SelectMuonPassingSelector(iMuon, "TMLastStationTight");
      bool variableResult = Muon_isTMLastStationTight->at(iMuon);
//       std::cout << "selectorResult = " << selectorResult << ", variableResult = " << variableResult;
      if(selectorResult != variableResult)
      {
        std::cout << " DIFFERENT!!!!!!!!!!!!!!!!!\n";
      }
//       else
//       {
//         std::cout << std::endl;
//       }
      Fill(hSelMuonPt,Muon_pt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEta,Muon_eta->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonPhi,Muon_phi->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonCharge,Muon_charge->at(iMuon), binningVariableValue,vLowBins);
      if(Muon_hasTriggered->at(iMuon))
      {
        nTriggeringSelMuons++;
        Fill(hSelMuonHasTriggered,1., binningVariableValue,vLowBins);
      }
      else
      {
        Fill(hSelMuonHasTriggered,0., binningVariableValue,vLowBins);
      }
      Fill(hSelMuonCaloCompatibility,Muon_caloCompatibility->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonSegmentCompatibility,Muon_segmentCompatibility->at(iMuon), binningVariableValue,vLowBins);
      unsigned int iTrack = Muon_trackref->at(iMuon);
      if(iTrack != 65535 && iTrack != 2048)
      {
        Fill(hSelMuonTrackValidHits,Track_numberOfValidHits->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackChi2,Track_normalizedChi2->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackNdof,Track_ndof->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackLayersWithMeasurement,Track_layersWithMeasurement->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackValidPixelHits,Track_validPixelHits->at(iTrack), binningVariableValue,vLowBins);
        
        Fill(hSelMuonDxy,Track_dxy_PV->at(iTrack).at(iSelectedVertex), binningVariableValue,vLowBins);
        Fill(hSelMuonDxyError,Track_dxyError_PV->at(iTrack).at(iSelectedVertex), binningVariableValue,vLowBins);
        Fill(hSelMuonDxyz,Track_dxyz_PV->at(iTrack).at(iSelectedVertex), binningVariableValue,vLowBins);
        Fill(hSelMuonDxyzError,Track_dxyzError_PV->at(iTrack).at(iSelectedVertex), binningVariableValue,vLowBins);
        Fill(hSelMuonDz,Track_dz_PV->at(iTrack).at(iSelectedVertex), binningVariableValue,vLowBins);
        Fill(hSelMuonAbsDz,fabs(Track_dz_PV->at(iTrack).at(iSelectedVertex)), binningVariableValue,vLowBins);
        Fill(hSelMuonDzError,Track_dzError_PV->at(iTrack).at(iSelectedVertex), binningVariableValue,vLowBins);
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

        Fill(hSelMuonSignedDxyBS,Track_dxy_BS->at(iTrack)*sign, binningVariableValue,vLowBins);
        Fill(hSelMuonDxyBSError,Track_dxyError_BS->at(iTrack), binningVariableValue,vLowBins);
        hSelMuonSignedDxyBSVsPhi->Fill(Track_phi->at(iTrack), Track_dxy_BS->at(iTrack)*sign);
        hSelMuonDxyBSErrorVsPhi->Fill(Track_phi->at(iTrack), Track_dxyError_BS->at(iTrack));
      }
      else
      {
        Fill(hSelMuonTrackValidHits,0, binningVariableValue,vLowBins);
        Fill(hSelMuonTrackChi2,9999, binningVariableValue,vLowBins);
        Fill(hSelMuonTrackNdof,0, binningVariableValue,vLowBins);
        Fill(hSelMuonTrackLayersWithMeasurement,0, binningVariableValue,vLowBins);
        Fill(hSelMuonTrackValidPixelHits,0, binningVariableValue,vLowBins);
        Fill(hSelMuonDxy,9999, binningVariableValue,vLowBins);
        Fill(hSelMuonDxyError,9999, binningVariableValue,vLowBins);
        Fill(hSelMuonDxyz,9999, binningVariableValue,vLowBins);
        Fill(hSelMuonDxyzError,9999, binningVariableValue,vLowBins);
        Fill(hSelMuonDz,9999, binningVariableValue,vLowBins);
        Fill(hSelMuonAbsDz,9999, binningVariableValue,vLowBins);
        Fill(hSelMuonDzError,9999, binningVariableValue,vLowBins);
        hSelMuonDxyVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyErrorVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDxyzErrorVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonAbsDzVsRunNumber->Fill(runNumber, 9999);
        hSelMuonDzErrorVsRunNumber->Fill(runNumber, 9999);
        
        Fill(hSelMuonSignedDxyBS,9999, binningVariableValue,vLowBins);
        Fill(hSelMuonDxyBSError,9999, binningVariableValue,vLowBins);
        hSelMuonSignedDxyBSVsPhi->Fill(9999, 9999);
        hSelMuonDxyBSErrorVsPhi->Fill(9999, 9999);
      }
      Fill(hSelMuonGlobalChi2,Muon_globalChi2->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalValidMuonHits,Muon_globalValidMuonHits->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonNumberOfMatches,Muon_numberOfMatches->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonNumberOfMatchedStations,Muon_numberOfMatchedStations->at(iMuon), binningVariableValue,vLowBins);
      
      unsigned short stationMask = Muon_stationMask->at(iMuon);
      for(unsigned short iBit = 0; iBit < 8; iBit++)
      {
//         std::cout << "Eta: " << Muon_eta->at(iMuon) << " Station: " << iBit+1 << " value: " << (stationMask&(1<<iBit)) << std::endl;
        if((stationMask&(1<<iBit)) != 0)
        {
          Fill(hSelMuonStationsWithHits,iBit+1, binningVariableValue,vLowBins);
          Fill(hSelMuonStationsWithHitsMerged,(iBit%4)+1, binningVariableValue,vLowBins);
          if(iBit < 4)
          {
            Fill(hSelMuonDTStationsWithHits,iBit+1, binningVariableValue,vLowBins);
          }
          else
          {
            Fill(hSelMuonCSCStationsWithHits,(iBit-4)+1, binningVariableValue,vLowBins);
          }
        }
      }
      Fill(hSelMuonKink,Muon_Kink->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonKinkRho,Muon_Kink_rho->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonKinkZ,Muon_Kink_z->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKink,Muon_GlobalKink->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkRho,Muon_GlobalKink_rho->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkZ,Muon_GlobalKink_z->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkChi2LocalMomentum,Muon_GlobalKink_chi2LocalMomentum->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkChi2LocalPosition,Muon_GlobalKink_chi2LocalPosition->at(iMuon), binningVariableValue,vLowBins);
      hSelMuonChi2LocalMomentumVsChi2LocalPosition->Fill(Muon_GlobalKink_chi2LocalPosition->at(iMuon), Muon_GlobalKink_chi2LocalMomentum->at(iMuon));

      Fill(hSelMuonGlbTrackProbability,Muon_glbTrackProbability->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkGlobalDeltaEtaPhi,Muon_GlobalKink_globalDeltaEtaPhi->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkLocalDistance,Muon_GlobalKink_localDistance->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkStaRelChi2,Muon_GlobalKink_staRelChi2->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkTightMatch,Muon_GlobalKink_tightMatch->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonGlobalKinkTrkRelChi2,Muon_GlobalKink_trkRelChi2->at(iMuon), binningVariableValue,vLowBins);
      hSelMuonStaRelChi2VsTrkRelChi2->Fill(Muon_GlobalKink_trkRelChi2->at(iMuon), Muon_GlobalKink_staRelChi2->at(iMuon));
      
      Fill(hSelMuonEnergyEcalTime,Muon_energy_ecalTime->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyEcalTimeError,Muon_energy_ecalTimeError->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyEm,Muon_energy_em->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyEmMax,Muon_energy_emMax->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyEmS25,Muon_energy_emS25->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyEmS9,Muon_energy_emS9->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyHcalTime,Muon_energy_hcalTime->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyHcalTimeError,Muon_energy_hcalTimeError->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyHad,Muon_energy_had->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyHadMax,Muon_energy_hadMax->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyHadS9,Muon_energy_hadS9->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyTower,Muon_energy_tower->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEnergyTowerS9,Muon_energy_towerS9->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03EmEt,Muon_isolationR03_emEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03EmVetoEt,Muon_isolationR03_emVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03EmMinusVetoEt,Muon_isolationR03_emEt->at(iMuon)-Muon_isolationR03_emVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03HadEt,Muon_isolationR03_hadEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03HadVetoEt,Muon_isolationR03_hadVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03HadMinusVetoEt,Muon_isolationR03_hadEt->at(iMuon)-Muon_isolationR03_hadVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03HoEt,Muon_isolationR03_hoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03HoVetoEt,Muon_isolationR03_hoVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03HoMinusVetoEt,Muon_isolationR03_hoEt->at(iMuon)-Muon_isolationR03_hoVetoEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03NJets,Muon_isolationR03_nJets->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03NTracks,Muon_isolationR03_nTracks->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03SumPt,Muon_isolationR03_sumPt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonIsolationR03TrackerVetoPt,Muon_isolationR03_trackerVetoPt->at(iMuon), binningVariableValue,vLowBins);
      
      hSelMuonCaloEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emEt->at(iMuon)+Muon_isolationR03_hadEt->at(iMuon));
      hSelMuonCaloVetoEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emVetoEt->at(iMuon)+Muon_isolationR03_hadVetoEt->at(iMuon));
      hSelMuonCaloMinusVetoEtR03VsMuonPt->Fill(Muon_pt->at(iMuon),Muon_isolationR03_emEt->at(iMuon)+Muon_isolationR03_hadEt->at(iMuon)-(Muon_isolationR03_emVetoEt->at(iMuon)+Muon_isolationR03_hadVetoEt->at(iMuon)));
      
      Fill(hSelMuonPfIsolationR03SumChargedHadronPt,Muon_pfIsolationR03_sumChargedHadronPt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonPfIsolationR03SumChargedParticlePt,Muon_pfIsolationR03_sumChargedParticlePt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonPfIsolationR03SumNeutralHadronEt,Muon_pfIsolationR03_sumNeutralHadronEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold,Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonPfIsolationR03SumPhotonEt,Muon_pfIsolationR03_sumPhotonEt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonPfIsolationR03SumPhotonEtHighThreshold,Muon_pfIsolationR03_sumPhotonEtHighThreshold->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonPfIsolationR03SumPUPt,Muon_pfIsolationR03_sumPUPt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonTrackIso,Muon_trackIso->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEcalIso,Muon_ecalIso->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonHcalIso,Muon_hcalIso->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonCaloIso,Muon_caloIso->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonShowerNCorrelatedHitsInStation1,Muon_shower_nStationCorrelatedHits->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonShowerNCorrelatedHitsInStation2,Muon_shower_nStationCorrelatedHits->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonShowerNCorrelatedHitsInStation3,Muon_shower_nStationCorrelatedHits->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonShowerNCorrelatedHitsInStation4,Muon_shower_nStationCorrelatedHits->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonShowerNHitsInStation1,Muon_shower_nStationHits->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonShowerNHitsInStation2,Muon_shower_nStationHits->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonShowerNHitsInStation3,Muon_shower_nStationHits->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonShowerNHitsInStation4,Muon_shower_nStationHits->at(iMuon).at(3), binningVariableValue,vLowBins);
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
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonDeltaRFromGenp);
  
//   CreateAndWriteCanvas("cRecoMuonPt",0, 21, 1, false, true, hRecoMuonPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonLeadingPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonSecondPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonLeadingEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonSecondEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonPhi);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPhi);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonCharge);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonCharge);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonHasTriggered);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonHasTriggered);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonCaloCompatibility);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonCaloCompatibility);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonSegmentCompatibility);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonSegmentCompatibility);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonTrackValidHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonTrackValidHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonTrackChi2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonTrackChi2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonTrackNdof);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonTrackNdof);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonTrackLayersWithMeasurement);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonTrackLayersWithMeasurement);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonTrackValidPixelHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonTrackValidPixelHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalChi2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalChi2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalValidMuonHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalValidMuonHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonNumberOfMatches);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonNumberOfMatches);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonNumberOfMatchedStations);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonNumberOfMatchedStations);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonStationsWithHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonStationsWithHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonStationsWithHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonDTStationsWithHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonDTStationsWithHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonCSCStationsWithHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonStationsWithHitsMerged);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonStationsWithHitsMerged);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonKink);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonKink);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonKinkRho);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonKinkRho);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonKinkZ);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonKinkZ);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKink);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKink);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkRho);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkRho);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkZ);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkZ);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkChi2LocalMomentum);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkChi2LocalMomentum);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkChi2LocalPosition);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkChi2LocalPosition);
  CreateAndWriteCanvas("cRecoMuonChi2LocalMomentumVsChi2LocalPosition","colz", false, false,false, hRecoMuonChi2LocalMomentumVsChi2LocalPosition);
  CreateAndWriteCanvas("cSelMuonChi2LocalMomentumVsChi2LocalPosition","colz", false, false,false, hSelMuonChi2LocalMomentumVsChi2LocalPosition);  
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlbTrackProbability);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlbTrackProbability);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkGlobalDeltaEtaPhi);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkGlobalDeltaEtaPhi);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkLocalDistance);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkLocalDistance);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkStaRelChi2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkStaRelChi2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkTightMatch);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkTightMatch);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonGlobalKinkTrkRelChi2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonGlobalKinkTrkRelChi2);
  CreateAndWriteCanvas("cRecoMuonStaRelChi2VsTrkRelChi2","colz", false, false,false, hRecoMuonStaRelChi2VsTrkRelChi2);
  CreateAndWriteCanvas("cSelMuonStaRelChi2VsTrkRelChi2","colz", false, false,false, hSelMuonStaRelChi2VsTrkRelChi2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyEcalTime);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyEcalTime);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyEcalTimeError);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyEcalTimeError);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyEm);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyEm);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyEmMax);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyEmMax);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyEmS25);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyEmS25);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyEmS9);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyEmS9);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyHcalTime);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyHcalTime);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyHcalTimeError);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyHcalTimeError);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyHad);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyHad);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyHadMax);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyHadMax);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyHadS9);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyHadS9);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyTower);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyTower);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEnergyTowerS9);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEnergyTowerS9);

  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03EmEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03EmEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03EmVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03EmVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03EmMinusVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03EmMinusVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03HadEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03HadEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03HadVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03HadVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03HadMinusVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03HadMinusVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03HoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03HoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03HoVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03HoVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03HoMinusVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03HoMinusVetoEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03NJets);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03NJets);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03NTracks);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03NTracks);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03SumPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03SumPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonIsolationR03TrackerVetoPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonIsolationR03TrackerVetoPt);

  CreateAndWriteCanvas("cRecoMuonCaloEtR03VsMuonPt","colz", false, false,false, hRecoMuonCaloEtR03VsMuonPt);
  CreateAndWriteCanvas("cSelMuonCaloEtR03VsMuonPt","colz", false, false,false, hSelMuonCaloEtR03VsMuonPt);
  CreateAndWriteCanvas("cRecoMuonCaloVetoEtR03VsMuonPt","colz", false, false,false, hRecoMuonCaloVetoEtR03VsMuonPt);
  CreateAndWriteCanvas("cSelMuonCaloVetoEtR03VsMuonPt","colz", false, false,false, hSelMuonCaloVetoEtR03VsMuonPt);
  
  CreateAndWriteCanvas("cRecoMuonCaloMinusVetoEtR03VsMuonPt","colz", false, false,false, hRecoMuonCaloMinusVetoEtR03VsMuonPt);
  CreateAndWriteCanvas("cSelMuonCaloMinusVetoEtR03VsMuonPt","colz", false, false,false, hSelMuonCaloMinusVetoEtR03VsMuonPt);
  
  
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonPfIsolationR03SumChargedHadronPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPfIsolationR03SumChargedHadronPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonPfIsolationR03SumChargedParticlePt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPfIsolationR03SumChargedParticlePt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonPfIsolationR03SumNeutralHadronEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPfIsolationR03SumNeutralHadronEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonPfIsolationR03SumNeutralHadronEtHighThreshold);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPfIsolationR03SumNeutralHadronEtHighThreshold);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonPfIsolationR03SumPhotonEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPfIsolationR03SumPhotonEt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonPfIsolationR03SumPhotonEtHighThreshold);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPfIsolationR03SumPhotonEtHighThreshold);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonPfIsolationR03SumPUPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPfIsolationR03SumPUPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonTrackIso);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonTrackIso);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonEcalIso);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonEcalIso);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonHcalIso);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonHcalIso);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonCaloIso);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonCaloIso);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonShowerNCorrelatedHitsInStation1);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonShowerNCorrelatedHitsInStation1);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonShowerNCorrelatedHitsInStation2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonShowerNCorrelatedHitsInStation2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonShowerNCorrelatedHitsInStation3);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonShowerNCorrelatedHitsInStation3);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonShowerNCorrelatedHitsInStation4);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonShowerNCorrelatedHitsInStation4);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonShowerNHitsInStation1);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonShowerNHitsInStation1);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonShowerNHitsInStation2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonShowerNHitsInStation2);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonShowerNHitsInStation3);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonShowerNHitsInStation3);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonShowerNHitsInStation4);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonShowerNHitsInStation4);
  
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
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxyError);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxyz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxyzError);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDzError);
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

  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonSignedDxyBS);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxyBSError);
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



std::string MuonBasicDistributionsDataForBmm::GetBinningVariableString(const std::string sBinning)
{
  std::string firstChar = sBinning.substr(0,1);
  if(firstChar=="P")
  {
    return "Pt";
  }
  if(firstChar=="E")
  {
    return "Eta";
  }
  if(firstChar=="C")
  {
    return "Charge";
  }
  return "";
}



std::vector<double> MuonBasicDistributionsDataForBmm::GetBins(const std::string sBinning)
{
  std::vector<double> vBins;
  std::string binningVariable = GetBinningVariableString(sBinning);
  if(binningVariable=="Pt" || binningVariable == "Eta")
  {
    std::string sBins = sBinning.substr(1,sBinning.size());
    vBins = ParseBins(sBins);
  }
  return vBins;
}



std::vector<double> MuonBasicDistributionsDataForBmm::ParseBins(const std::string sBins)
{
  std::vector<std::string> tokens;
  std::vector<double> vBins;
  
  // Skip delimiters at beginning.
  std::string::size_type lastPos = sBins.find_first_not_of("x", 0);
  // Find first "non-delimiter".
  std::string::size_type pos = sBins.find_first_of("x", lastPos);
  
  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(sBins.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = sBins.find_first_not_of("x", pos);
    // Find next "non-delimiter"
    pos = sBins.find_first_of("x", lastPos);
    double result;
    if(from_string<double>(result, *(tokens.end()-1), std::dec))
    {
      vBins.push_back(result);
    }
    else
    {
      exit(1);
    }
  }
  
  return vBins;
}



std::vector<double> MuonBasicDistributionsDataForBmm::GetBinningVariablesValues(const unsigned int iMuon)
{
  std::vector<double> result;
  result.push_back(1e100);
  result.push_back(Muon_pt->at(iMuon));
  result.push_back(fabs(Muon_eta->at(iMuon)));
  result.push_back(Muon_charge->at(iMuon));
  return result;
}


#endif // MuonBasicDistributionsDataForBmm_cxx
