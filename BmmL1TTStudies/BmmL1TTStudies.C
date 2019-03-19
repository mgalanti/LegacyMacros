#ifndef BmmL1TTStudies_cxx
#define BmmL1TTStudies_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "BmmL1TTStudies.h"
#include "../utilities/constants.h"

void BmmL1TTStudies::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  //   Default
  double maxDistanceForMatching = 0.05;
  double maxDPtOverPtForMatching = 0.2;
  int trueGenpId = 13;
  int trueBId = 0;
  if(sampleName.find("BsTo") != std::string::npos || sampleName.find("Bs2") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B_s* sample: only gen particles coming from a B_s mom will be selected.\n";
    trueBId = 531;
  }
  else if(sampleName.find("BdTo") != std::string::npos || sampleName.find("Bd2") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B_d* sample: only gen particles coming from a B_d mom will be selected.\n";
    trueBId = 511;
  }
  else
  {
    std::cout << "W A R N I N G ! Could not infer the sample type from the name!\n";
    std::cout << " MC matching of L1 Tk muons is disabled!\n";
  }
  std::cout << "Analyzing " << nentries << " events.\n";

  // Algorithm parameters
  // FIXME: MAKE THEM CONFIGURABLE???
  bool use5ParFit = true;
  double ptThreshold = 4.0;
  double etaThreshold = 5.0;
  double l1SingleTrackPtThreshold = 0.;
  double l1SingleTrackEtaThreshold = 5.;
  
  // Histogram definition parameters
//   int nPtVarBins = 6; // for variable binning
//   double ptVarBins[9]={0,4,6,10,30,50,100}; // for variable binning
  const unsigned int nPtBins = 15;
  const double ptMin = 1.;
  const double ptMax = 31.;
  const char* genPtLabel = "gen #mu p_{T} [GeV/c]";
  const char* l1PtLabel = "L1 Tk #mu p_{T} [GeV/c]";
  const char* ttPtLabel = "TTTrack p_{T} [GeV/c]";
  //   int nEtaVarBins = 4; // for variable binning
//   double etaVarBins[8]={0.,0.9,1.4,2.4,4.5}; // for variable binning
  const unsigned int nEtaBins = 27;
  const double etaMin = -2.7;
  const double etaMax = 2.7;
  const char* genEtaLabel = "gen #mu #eta";
  const char* l1EtaLabel = "L1 Tk #mu #eta";
  const char* ttEtaLabel = "TTTrack #eta";
  
  const char* nMuLabel = "N_{#mu}";

  std::string sBinningVariable = "|#eta|";
  std::vector<double> vLowBins;
  vLowBins.push_back(0.);
  vLowBins.push_back(1.4);
  vLowBins.push_back(2.5);
  vLowBins.push_back(5.);
  
  // Book histograms here
  
  std::vector<TH1D*>* hNAllGenBCandPerEvent = CreateVectorOf1DHistograms<TH1D>("hNAllGenBCandPerEvent", "Number of generated B candidates per event", 6, -0.5, 5.5, "n_{B cands}", "n_{evts}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hNAllGenMuonsPerEvent = CreateVectorOf1DHistograms<TH1D>("hNAllGenMuonsPerEvent", "Number of generated muons from B candidates per event", 6, -0.5, 5.5, "n_{B cands}", "n_{evts}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hAllGenParticleGenPt = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleGenPt","p_{T} of all gen particles coming from B decays", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}", sBinningVariable, vLowBins);
  TH1D* hAllGenParticleGenEta = Create1DHistogram<TH1D>("hAllGenParticleGenEta","#eta of all gen particles coming from B decays", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}");
  TH2D* hAllGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAllGenParticleGenPtVsEta","p_{T} vs. #eta of all gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  std::vector<TH1D*>* hAllGenParticleGenDxy = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleGenDxy","gen d_{xy} of all gen particles coming from B decays", 50, 0, 0.5, "gen #mu d_{xy} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAllGenParticleGenDz = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleGenDz","gen d_{z} of all gen particles coming from B decays", 50, -25., 25., "gen #mu d_{z} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hAssocTkGenParticleGenPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleGenPt","p_{T} of gen particles coming from B decays associated to a TTTrack", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}", sBinningVariable, vLowBins);
  TH1D* hAssocTkGenParticleGenEta = Create1DHistogram<TH1D>("hAssocTkGenParticleGenEta","#eta of gen particles coming from B decays associated to a TTTrack", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}");
  TH2D* hAssocTkGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleGenPtVsEta","p_{T} vs. #eta of gen particles coming from B decays associated to a TTTrack", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  std::vector<TH1D*>* hAssocTkGenParticleRecoPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleRecoPt","p_{T} of TTTracks associated to gen particles coming from B decays", nPtBins, ptMin, ptMax, ttPtLabel,"n_{TTTracks}", sBinningVariable, vLowBins);
  TH1D* hAssocTkGenParticleRecoEta = Create1DHistogram<TH1D>("hAssocTkGenParticleRecoEta","#eta of TTTracks associated to gen particles coming from B decays", nEtaBins, etaMin, etaMax, ttEtaLabel,"n_{TTTracks}");
  TH2D* hAssocTkGenParticleRecoPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleRecoPtVsEta","p_{T} vs. #eta of TTTracks associated to gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, ttEtaLabel, ttPtLabel);
  std::vector<TH1D*>* hAssocTkGenParticleDeltaPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleDeltaPt","reco p_{T} - gen p_{T} of gen particles coming from B decays associated to a TTTrack", 50, -2., 2., "#Delta(p_{T}) [GeV]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleDeltaEta = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleDeltaEta","reco #eta - gen #eta of gen particles coming from B decays associated to a TTTrack", 50, -0.1, 0.1, "#Delta(#eta)","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH2D*>* hAssocTkGenParticleDeltaPtVsDeltaEta = CreateVectorOf2DHistograms<TH2D>("hAssocTkGenParticleDeltaPtVsDeltaEta","#Delta(p_{T}) vs. #Delta(#eta) of TTTracks associated to gen particles coming from B decays", 50, -0.1, 0.1, 50, -2., 2., "#Delta(#eta)", "#Delta(p_{T}) [GeV]", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hAssocTkGenParticleGenDxy = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleGenDxy","gen d_{xy} of gen particles coming from B decays associated to a TTTrack", 50, 0, 0.5, "gen #mu d_{xy} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleRecoDxy = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleRecoDxy","reco d_{xy} of gen particles coming from B decays associated to a TTTrack", 50, 0, 0.5, "reco #mu d_{xy} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleDeltaDxy = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleDeltaDxy","(reco d_{xy} - gen d_{xy}) of gen particles coming from B decays associated to a TTTrack", 100, -0.2, 0.2, "#Delta(d_{xy}) [cm]","n_{GenP}", sBinningVariable, vLowBins);

  std::vector<TH2D*>* hAssocTkGenParticleDeltaDxyVsRecoPt = CreateVectorOf2DHistograms<TH2D>("hAssocTkGenParticleDeltaDxyVsRecoPt","(reco d_{xy} - gen d_{xy}) vs. reco p_{T} of gen particles coming from B decays associated to a TTTrack", nPtBins, ptMin, ptMax, 100, -0.2, 0.2, "reco p_{T} [GeV]", "#Delta(d_{xy})\ [cm]", sBinningVariable, vLowBins);
  
  
  std::vector<TH1D*>* hAssocTkGenParticleGenDz = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleGenDz","gen d_{z} of gen particles coming from B decays associated to a TTTrack", 50, -25., 25., "gen #mu d_{z} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleRecoDz = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleRecoDz","reco d_{z} of gen particles coming from B decays associated to a TTTrack", 50, -25., 25., "reco #mu d_{z} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleDeltaDz = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleDeltaDz","(reco d_{z} - gen d_{z}) of gen particles coming from B decays associated to a TTTrack", 50, -1., 1., "#Delta(d_{z}) [cm]","n_{GenP}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hAssocGenParticleGenPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleGenPt","p_{T} of gen particles coming from B decays associated to a L1 muon", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}", sBinningVariable, vLowBins);
  TH1D* hAssocGenParticleGenEta = Create1DHistogram<TH1D>("hAssocGenParticleGenEta","#eta of gen particles coming from B decays associated to a L1 muon", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}");
  TH2D* hAssocGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleGenPtVsEta","p_{T} vs. #eta of gen particles coming from B decays associated to a L1 muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  std::vector<TH1D*>* hAssocGenParticleRecoPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleRecoPt","p_{T} of L1 muons associated to gen particles coming from B decays", nPtBins, ptMin, ptMax, l1PtLabel,"n_{L1 muons}", sBinningVariable, vLowBins);
  TH1D* hAssocGenParticleRecoEta = Create1DHistogram<TH1D>("hAssocGenParticleRecoEta","#eta of L1 muons associated to gen particles coming from B decays", nEtaBins, etaMin, etaMax, l1EtaLabel,"n_{L1 muons}");
  TH2D* hAssocGenParticleRecoPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleRecoPtVsEta","p_{T} vs. #eta of L1 muons associated to gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, l1EtaLabel, l1PtLabel);
  std::vector<TH1D*>* hAssocGenParticleDeltaPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleDeltaPt","reco p_{T} - gen p_{T} of gen particles coming from B decays associated to a L1 muon", 50, -2., 2., "#Delta(p_{T}) [GeV]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocGenParticleDeltaEta = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleDeltaEta","reco #eta - gen #eta of gen particles coming from B decays associated to a L1 muon", 50, -0.1, 0.1, "#Delta(#eta)","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH2D*>* hAssocGenParticleDeltaPtVsDeltaEta = CreateVectorOf2DHistograms<TH2D>("hAssocGenParticleDeltaPtVsDeltaEta","#Delta(p_{T}) vs. #Delta(#eta) of L1 muons associated to gen particles coming from B decays", 50, -0.1, 0.1, 50, -2., 2., "#Delta(#eta)", "#Delta(p_{T}) [GeV]", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hAssocGenParticleGenDxy = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleGenDxy","gen d_{xy} of gen particles coming from B decays associated to a L1 muon", 50, 0, 0.5, "gen #mu d_{xy} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocGenParticleRecoDxy = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleRecoDxy","reco d_{xy} of gen particles coming from B decays associated to a L1 muon", 50, 0, 0.5, "reco #mu d_{xy} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocGenParticleDeltaDxy = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleDeltaDxy","|reco d_{xy} - gen d_{xy}| of gen particles coming from B decays associated to a L1 muon", 50, 0., 0.2, "|#Delta(d_{xy})| [cm]","n_{GenP}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hAssocGenParticleGenDz = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleGenDz","gen d_{z} of gen particles coming from B decays associated to a L1 muon", 50, -25., 25., "gen #mu d_{xy} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocGenParticleRecoDz = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleRecoDz","reco d_{z} of gen particles coming from B decays associated to a L1 muon", 50, -25., 25., "reco #mu d_{z} [cm]","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocGenParticleDeltaDz = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleDeltaDz","(reco d_{z} - gen d_{z}) of gen particles coming from B decays associated to a L1 muon", 50, -1., 1., "#Delta(d_{z}) [cm]","n_{GenP}", sBinningVariable, vLowBins);

  std::vector<TH1D*>* hAssocTkGenParticleDeltaR = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleDeltaR","|#Delta(R)| of gen particles coming from B decays from associated TTTrack", 50, 0, 0.2, "|#Delta(R)|","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleDeltaPtOverPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleDeltaPtOverPt","|#Delta(p_{T})|/p_{T} of gen particles coming from B decays from associated TTTrack", 50, 0, 0.5, "|#Delta(p_{T})|/p_{T}","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH2D*>* hAssocTkGenParticleDeltaPtOverPtVsDeltaR = CreateVectorOf2DHistograms<TH2D>("hAssocTkGenParticleDeltaPtOverPtVsDeltaR","|#Delta(p_{T})|/p_{T} vs. |#Delta(R)| of gen particles coming from B decays from associated TTTrack", 50, 0, 0.2, 50, 0, 0.5, "|#Delta(R)|", "|#Delta(p_{T})|/p_{T}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hAssocGenParticleDeltaR = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleDeltaR","|#Delta(R)| of gen particles coming from B decays from associated L1 Tk muon", 50, 0, 0.2, "|#Delta(R)|","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAssocGenParticleDeltaPtOverPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleDeltaPtOverPt","|#Delta(p_{T})|/p_{T} of gen particles coming from B decays from associated L1 Tk muon", 50, 0, 0.5, "|#Delta(p_{T})|/p_{T}","n_{GenP}", sBinningVariable, vLowBins);
  std::vector<TH2D*>* hAssocGenParticleDeltaPtOverPtVsDeltaR = CreateVectorOf2DHistograms<TH2D>("hAssocGenParticleDeltaPtOverPtVsDeltaR","|#Delta(p_{T})|/p_{T} vs. |#Delta(R)| of gen particles coming from B decays from associated L1 Tk muon", 50, 0, 0.2, 50, 0, 0.5, "|#Delta(R)|", "|#Delta(p_{T})|/p_{T}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hAllDiGenMuonInvariantMass = CreateVectorOf1DHistograms<TH1D>("hAllDiGenMuonInvariantMass", "Invariant mass of generated di-#mu from B", 100, 4.9, 5.9, "M(#mu#mu) [GeV]", "N_{#mu#mu}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAllDiGenMuonPt = CreateVectorOf1DHistograms<TH1D>("hAllDiGenMuonPt", "p_{T} of generated di-#mu from B", 100, 0., 50., "p_{T}(#mu#mu) [GeV]", "N_{#mu#mu}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hAllDiGenMuonEta = CreateVectorOf1DHistograms<TH1D>("hAllDiGenMuonEta", "#eta of generated di-#mu from B", 60, -3.0, 3.0, "#eta(#mu#mu)", "N_{#mu#mu}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hDiTTTrackInvariantMass = CreateVectorOf1DHistograms<TH1D>("hDiTTTrackInvariantMass", "Invariant mass of reconstructed TTTrack pairs associated to #mu from B", 100, 4.9, 5.9, "M(Tk Tk) [GeV]", "N_{Tk Tk}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiTTTrackPt = CreateVectorOf1DHistograms<TH1D>("hDiTTTrackPt", "p_{T} of reconstructed TTTrack pairs associated to #mu from B", 100, 0., 50., "p_{T}(Tk TK) [GeV]", "N_{Tk Tk}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiTTTrackEta = CreateVectorOf1DHistograms<TH1D>("hDiTTTrackEta", "#eta of reconstructed TTTrack pairs associated to #mu from B", 60, -3.0, 3.0, "#eta(Tk Tk)", "N_{Tk Tk}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiTTTrackDxyDiff = CreateVectorOf1DHistograms<TH1D>("hDiTTTrackDxyDiff", "#Delta(d_{xy}) of reconstructed TTTrack pairs associated to #mu from B", 100, -3.0, 3.0, "d_{xy}(Tk1) - d_{xy}(Tk2) [cm]", "N_{Tk Tk}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiTTTrackDzDiff = CreateVectorOf1DHistograms<TH1D>("hDiTTTrackDzDiff", "#Delta(d_{z}) of reconstructed TTTrack pairs associated to #mu from B", 100, -3.0, 3.0, "d_{z}(Tk1) - d_{z}(Tk2) [cm]", "N_{Tk Tk}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hDiTTTrackDeltaInvariantMass = CreateVectorOf1DHistograms<TH1D>("hDiTTTrackDeltaInvariantMass", "[M(TkTk)-M(#mu#mu)]/M(#mu#mu) between reconstructed TTTrack pairs and di-#mu from B", 100, -0.5, 0.5, "#Delta(M)/M", "N_{Tk Tk}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiTTTrackDeltaPt = CreateVectorOf1DHistograms<TH1D>("hDiTTTrackDeltaPt", "[p_{T}(TkTk)-p_{T}(#mu#mu)]/p_{T}(#mu#mu) between reconstructed TTTrack pairs and di-#mu from B", 100, -0.5, 0.5, "#Delta(p_{T})/p_{T}", "N_{Tk Tk}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiTTTrackDeltaEta = CreateVectorOf1DHistograms<TH1D>("hDiTTTrackDeltaEta", "[#eta(TkTk)-#eta(#mu#mu)]/#eta(#mu#mu) between reconstructed TTTrack pairs and di-#mu from B", 100, -0.5, 0.5, "#Delta(#eta)/#eta", "N_{Tk Tk}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hDiL1TkMuonsInvariantMass = CreateVectorOf1DHistograms<TH1D>("hDiL1TkMuonsInvariantMass", "Invariant mass of reconstructed L1TkMuon pairs associated to #mu from B", 100, 4.9, 5.9, "M(L1#mu L1#mu) [GeV]", "N_{L1#mu L1#mu}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiL1TkMuonsPt = CreateVectorOf1DHistograms<TH1D>("hDiL1TkMuonsPt", "p_{T} of reconstructed L1TkMuon pairs associated to #mu from B", 100, 0., 50., "p_{T}(L1#mu L1#mu) [GeV]", "N_{L1#mu L1#mu}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiL1TkMuonsEta = CreateVectorOf1DHistograms<TH1D>("hDiL1TkMuonsEta", "#eta of reconstructed L1TkMuon pairs associated to #mu from B", 60, -3.0, 3.0, "#eta(L1#mu L1#mu)", "N_{L1#mu L1#mu}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiL1TkMuonsDxyDiff = CreateVectorOf1DHistograms<TH1D>("hDiL1TkMuonsDxyDiff", "#Delta(d_{xy}) of reconstructed L1TkMuon pairs associated to #mu from B", 100, -3.0, 3.0, "d_{xy}(L1#mu1) - d_{xy}(L1#mu2) [cm]", "N_{L1#mu L1#mu}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiL1TkMuonsDzDiff = CreateVectorOf1DHistograms<TH1D>("hDiL1TkMuonsDzDiff", "#Delta(d_{z}) of reconstructed L1TkMuon pairs associated to #mu from B", 100, -3.0, 3.0, "d_{z}(L1#mu1) - d_{z}(L1#mu2) [cm]", "N_{L1#mu L1#mu}", sBinningVariable, vLowBins);
  
  std::vector<TH1D*>* hDiL1TkMuonsDeltaInvariantMass = CreateVectorOf1DHistograms<TH1D>("hDiL1TkMuonsDeltaInvariantMass", "[M(L1#muL1#mu)-M(#mu#mu)]/M(#mu#mu) between reconstructed L1TkMuon pairs and di-#mu from B", 100, -0.5, 0.5, "#Delta(M)/M", "N_{L1#mu L1#mu}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiL1TkMuonsDeltaPt = CreateVectorOf1DHistograms<TH1D>("hDiL1TkMuonsDeltaPt", "[p_{T}(L1#muL1#mu)-p_{T}(#mu#mu)]/p_{T}(#mu#mu) between reconstructed L1TkMuon pairs and di-#mu from B", 100, -0.5, 0.5, "#Delta(p_{T})/p_{T}", "N_{L1#mu L1#mu}", sBinningVariable, vLowBins);
  std::vector<TH1D*>* hDiL1TkMuonsDeltaEta = CreateVectorOf1DHistograms<TH1D>("hDiL1TkMuonsDeltaEta", "[#eta(L1#muL1#mu)-#eta(#mu#mu)]/#eta(#mu#mu) between reconstructed L1TkMuon pairs and di-#mu from B", 100, -0.5, 0.5, "#Delta(#eta)/#eta", "N_{L1#mu L1#mu}", sBinningVariable, vLowBins);
  Long64_t nbytes = 0, nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    std::vector<unsigned short> vSelectedMuons;
    std::vector<unsigned short> vParticlesFromB;
    std::vector<std::pair<short, double> > vTracksWithDistance;
    std::vector<unsigned short> vParticlesInAccFromB;

    double genEtaBinningVariable = -1.;
    double recoEtaBinningVariable = -1.;
    double genEtaSingleMuBinningVariable = -1.;
    double recoEtaSingleMuBinningVariable = -1.;
    
    TVector3 pL1TTTrack;
    TVector3 vL1TTTrack;
    
    std::map<unsigned int, unsigned int> mBCands;
    unsigned int nGenMuons = 0;
    
//     std::cout << "Sono qui 1 \n";
    
    for(unsigned int iGenParticle = 0; iGenParticle < Genp_Id->size(); iGenParticle++)
    {
      if(Genp_pt->at(iGenParticle) > 0)
      {
        int genpId = Genp_Id->at(iGenParticle);
        if(abs(genpId) == trueGenpId)
        {
          int iMother = Genp_vMom->at(iGenParticle).at(0);
          int motherId = Genp_Id->at(iMother);
          //           if(abs(motherId) == trueBId)
          if(abs(motherId) == trueBId/* && Genp_pt->at(iMother) > 7.5*/)
          {
            bool hasDifferentDaughters = false;
            for(unsigned int i = 0; i < Genp_vDau->at(iMother).size(); i++)
            {
              unsigned int iDau = Genp_vDau->at(iMother).at(i);
              // mu mu gamma decays are allowed!
              if(abs(Genp_Id->at(iDau)) != trueGenpId && abs(Genp_Id->at(iDau)) != 22)
              {
                hasDifferentDaughters = true;
                break;
              }
            }
//             if(hasDifferentDaughters)
//             {
//               std::cout << "Found a B hadron with different daughters.\n";
//               std::cout << "Printing the daughters record.\n";
//               RecursivePrintDaughters(iMother);
//             }
            if(!hasDifferentDaughters)
            {
//               std::cout << "Found a muon coming from a B!\n";
//               std::cout << "I = " << iGenParticle << std::endl;
//               std::cout << "ID = " << genpId << std::endl;
//               std::cout << "iMother = " << iMother << std::endl;
//               std::cout << "motherId = " << motherId << std::endl;
              vParticlesFromB.push_back(iGenParticle);
            }
          }
        }
      }
    }
    
//     std::cout << "Sono qui 2 \n";
    for(unsigned int i = 0; i < vParticlesFromB.size(); i++)
    {
      unsigned int iGenParticle = vParticlesFromB[i];
      if(fabs(Genp_eta->at(iGenParticle)) > genEtaBinningVariable)
      {
        genEtaBinningVariable = fabs(Genp_eta->at(iGenParticle));
      }
    }
    
//     std::cout << "Sono qui 3 \n";
    for(unsigned int i = 0; i < vParticlesFromB.size(); i++)
    {
      unsigned int iGenParticle = vParticlesFromB[i];
      int iMother = Genp_vMom->at(iGenParticle).at(0);
//       int motherId = Genp_Id->at(iMother);
      genEtaSingleMuBinningVariable = fabs(Genp_eta->at(iGenParticle));
      std::pair<short int, double> iTrackWithDistance = FindL1TTTrackNearestToGenParticle(iGenParticle, use5ParFit);
      //               std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticleWithPtThreshold(iGenParticle, 0.0, true);
      short int iTrack = iTrackWithDistance.first;
//       double distance = iTrackWithDistance.second;
      double dPtOverPtTrack = 99999;
      if(iTrack != -1)
      {
        if(use5ParFit)
        {
          pL1TTTrack.SetXYZ(L1TTTracks_theMomentum5Par_x->at(iTrack), L1TTTracks_theMomentum5Par_y->at(iTrack), L1TTTracks_theMomentum5Par_z->at(iTrack));
          vL1TTTrack.SetXYZ(L1TTTracks_thePOCA5Par_x->at(iTrack), L1TTTracks_thePOCA5Par_y->at(iTrack), L1TTTracks_thePOCA5Par_z->at(iTrack));
        }
        else
        {
          pL1TTTrack.SetXYZ(L1TTTracks_theMomentum4Par_x->at(iTrack), L1TTTracks_theMomentum4Par_y->at(iTrack), L1TTTracks_theMomentum4Par_z->at(iTrack));
          vL1TTTrack.SetXYZ(L1TTTracks_thePOCA4Par_x->at(iTrack), L1TTTracks_thePOCA4Par_y->at(iTrack), L1TTTracks_thePOCA4Par_z->at(iTrack));
        }
      }
      if(iTrack != -1)
      {
        dPtOverPtTrack = fabs((pL1TTTrack.Perp() - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle));
        if(iTrackWithDistance.second < maxDistanceForMatching && dPtOverPtTrack < maxDPtOverPtForMatching)
        {
          Fill(hAssocTkGenParticleGenPt, Genp_pt->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
          hAssocTkGenParticleGenEta->Fill(Genp_eta->at(iGenParticle));
          hAssocTkGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle), Genp_pt->at(iGenParticle));
          Fill(hAssocTkGenParticleRecoPt, pL1TTTrack.Perp(), genEtaSingleMuBinningVariable, vLowBins);
          hAssocTkGenParticleRecoEta->Fill(pL1TTTrack.Eta());
          hAssocTkGenParticleRecoPtVsEta->Fill(pL1TTTrack.Eta(), pL1TTTrack.Perp());
          Fill(hAssocTkGenParticleDeltaPt, pL1TTTrack.Perp() - Genp_pt->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
          Fill(hAssocTkGenParticleDeltaEta, pL1TTTrack.Eta() - Genp_eta->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
          Fill(hAssocTkGenParticleDeltaPtVsDeltaEta, pL1TTTrack.Eta() - Genp_eta->at(iGenParticle), pL1TTTrack.Perp() - Genp_pt->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
          Fill(hAssocTkGenParticleDeltaR, iTrackWithDistance.second, genEtaSingleMuBinningVariable, vLowBins);
          Fill(hAssocTkGenParticleDeltaPtOverPt, dPtOverPtTrack, genEtaSingleMuBinningVariable, vLowBins);
          Fill(hAssocTkGenParticleDeltaPtOverPtVsDeltaR, iTrackWithDistance.second, dPtOverPtTrack, genEtaSingleMuBinningVariable, vLowBins);
          
          if(Genp_pt->at(iGenParticle) > ptThreshold && fabs(Genp_eta->at(iGenParticle)) < etaThreshold)
          {
            Fill(hAssocTkGenParticleGenDxy, Genp_dxy->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocTkGenParticleRecoDxy, vL1TTTrack.Perp(), genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocTkGenParticleDeltaDxy, Genp_dxy->at(iGenParticle)-vL1TTTrack.Perp(), genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocTkGenParticleDeltaDxyVsRecoPt, pL1TTTrack.Perp(), Genp_dxy->at(iGenParticle)-vL1TTTrack.Perp(), genEtaSingleMuBinningVariable, vLowBins);
            
            Fill(hAssocTkGenParticleGenDz, Genp_dz->at(iGenParticle)+Genp_vz->at(2), genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocTkGenParticleRecoDz, vL1TTTrack.Z(), genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocTkGenParticleDeltaDz, Genp_dz->at(iGenParticle)+Genp_vz->at(2)-vL1TTTrack.Z(), genEtaSingleMuBinningVariable, vLowBins);
            if(vL1TTTrack.Perp() > 0.29 && vL1TTTrack.Perp() < 0.3)
            {
              std::cout << "Ev. " << evNumber << ", iTrack = " << iTrack << ", Tk matched, vL1TTTrack.Perp() = " << vL1TTTrack.Perp() << std::endl;
            }
          }
        }
      }
      //                 dPtOverPtTrack = fabs((Track_pt->at(iTrack) - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle));
      //               std::cout << "L1 TT Track associated to the gen mu has i = " << iTrack << ", distance = " << distance << ", and dpt/pt = " << dPtOverPtTrack << std::endl;
      //               double dPtOverPt = 999.9;
      
      vTracksWithDistance.push_back(iTrackWithDistance);
      
      if(mBCands.find(iMother)==mBCands.end())
      {
        mBCands[iMother] = 1;
      }
      else
      {
        mBCands[iMother]++;
      }
      
      nGenMuons++;
      Fill(hAllGenParticleGenPt, Genp_pt->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
      hAllGenParticleGenEta->Fill(Genp_eta->at(iGenParticle));
      hAllGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
      if(Genp_pt->at(iGenParticle) > ptThreshold && fabs(Genp_eta->at(iGenParticle)) < etaThreshold)
      {
        Fill(hAllGenParticleGenDxy, Genp_dxy->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
        Fill(hAllGenParticleGenDz, Genp_dz->at(iGenParticle) + Genp_vz->at(2), genEtaSingleMuBinningVariable, vLowBins);
      }
      if(Genp_pt->at(iGenParticle) > 4 && fabs(Genp_eta->at(iGenParticle)) < 2.4)
      {
        vParticlesInAccFromB.push_back(iGenParticle);
        //                 hAllGenParticleGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
        //                 hAllGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
        
      }
    }
//     std::cout << "Sono qui 4 \n";
    
    for(unsigned int iL1TkMuon = 0 ; iL1TkMuon < L1TkMuons_l1TTTrackRef->size(); iL1TkMuon++)
    {
      short int trackRef = L1TkMuons_l1TTTrackRef->at(iL1TkMuon);
      bool sameMuon = false;
      for(unsigned int jL1TkMuon = 0 ; jL1TkMuon < iL1TkMuon; jL1TkMuon++)
      {
        short int jTrackRef = L1TkMuons_l1TTTrackRef->at(jL1TkMuon);
        if(L1TTTracks_theMomentum5Par_x->at(trackRef) == L1TTTracks_theMomentum5Par_x->at(jTrackRef) &&
           L1TTTracks_theMomentum5Par_y->at(trackRef) == L1TTTracks_theMomentum5Par_y->at(jTrackRef) &&
           L1TTTracks_theMomentum5Par_z->at(trackRef) == L1TTTracks_theMomentum5Par_z->at(jTrackRef))
        {
          std::cout << "Found a doubly-counted L1TkMuon!\n";
          sameMuon = true;
        }
      }
      if(sameMuon)
        continue;
      for(unsigned int iParticleFromB = 0; iParticleFromB < vParticlesFromB.size(); iParticleFromB++)
      {
        unsigned short iGenParticle = vParticlesFromB[iParticleFromB];
        genEtaSingleMuBinningVariable = fabs(Genp_eta->at(iGenParticle));
        std::pair<short int, double> iTrackWithDistance = vTracksWithDistance[iParticleFromB];
        short int iTrack = iTrackWithDistance.first;
        if(iTrack == trackRef /*&& iTrackWithDistance.second < 0.1*/)
        {
          //             std::cout << "Found a gen particle associated to the muon!\n";
          unsigned int iMom = Genp_vMom->at(iGenParticle).at(0);
          //             bool hasMuonDaughter = false;
          for (unsigned int jParticleFromB = 0; jParticleFromB < Genp_vDau->at(iMom).size(); jParticleFromB++)
          {
            unsigned int jGenParticle = Genp_vDau->at(iMom).at(jParticleFromB);
            if (jGenParticle == iGenParticle)
              continue;
          }
          if(use5ParFit)
          {
            pL1TTTrack.SetXYZ(L1TTTracks_theMomentum5Par_x->at(iTrack), L1TTTracks_theMomentum5Par_y->at(iTrack), L1TTTracks_theMomentum5Par_z->at(iTrack));
            vL1TTTrack.SetXYZ(L1TTTracks_thePOCA5Par_x->at(iTrack), L1TTTracks_thePOCA5Par_y->at(iTrack), L1TTTracks_thePOCA5Par_z->at(iTrack));
          }
          else
          {
            pL1TTTrack.SetXYZ(L1TTTracks_theMomentum4Par_x->at(iTrack), L1TTTracks_theMomentum4Par_y->at(iTrack), L1TTTracks_theMomentum4Par_z->at(iTrack));
            vL1TTTrack.SetXYZ(L1TTTracks_thePOCA4Par_x->at(iTrack), L1TTTracks_thePOCA4Par_y->at(iTrack), L1TTTracks_thePOCA4Par_z->at(iTrack));
          }
          
          double dPtOverPt = fabs((pL1TTTrack.Perp() - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle));
          if(iTrackWithDistance.second < maxDistanceForMatching && dPtOverPt < maxDPtOverPtForMatching)
          {
            vSelectedMuons.push_back(iL1TkMuon);
            Fill(hAssocGenParticleGenPt, Genp_pt->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
            hAssocGenParticleGenEta->Fill(Genp_eta->at(iGenParticle));
            hAssocGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
            Fill(hAssocGenParticleRecoPt, pL1TTTrack.Perp(), genEtaSingleMuBinningVariable, vLowBins);
            hAssocGenParticleRecoEta->Fill(pL1TTTrack.Eta());
            hAssocGenParticleRecoPtVsEta->Fill(pL1TTTrack.Eta(), pL1TTTrack.Perp());
            Fill(hAssocGenParticleDeltaPt, pL1TTTrack.Perp() - Genp_pt->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocGenParticleDeltaEta, pL1TTTrack.Eta() - Genp_eta->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocGenParticleDeltaPtVsDeltaEta, pL1TTTrack.Eta() - Genp_eta->at(iGenParticle), pL1TTTrack.Perp() - Genp_pt->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocGenParticleDeltaR, iTrackWithDistance.second, genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocGenParticleDeltaPtOverPt, dPtOverPt, genEtaSingleMuBinningVariable, vLowBins);
            Fill(hAssocGenParticleDeltaPtOverPtVsDeltaR, iTrackWithDistance.second, dPtOverPt, genEtaSingleMuBinningVariable, vLowBins);

            if(Genp_pt->at(iGenParticle) > ptThreshold && fabs(Genp_eta->at(iGenParticle)) < etaThreshold)
            {
              Fill(hAssocGenParticleGenDxy, Genp_dxy->at(iGenParticle), genEtaSingleMuBinningVariable, vLowBins);
              Fill(hAssocGenParticleRecoDxy, vL1TTTrack.Perp(), genEtaSingleMuBinningVariable, vLowBins);
              Fill(hAssocGenParticleDeltaDxy, fabs(Genp_dxy->at(iGenParticle)-vL1TTTrack.Perp()), genEtaSingleMuBinningVariable, vLowBins);
              Fill(hAssocGenParticleGenDz, Genp_dz->at(iGenParticle)+ Genp_vz->at(2), genEtaSingleMuBinningVariable, vLowBins);
              Fill(hAssocGenParticleRecoDz, vL1TTTrack.Z(), genEtaSingleMuBinningVariable, vLowBins);
              Fill(hAssocGenParticleDeltaDz, Genp_dz->at(iGenParticle)+Genp_vz->at(2)-vL1TTTrack.Z(), genEtaSingleMuBinningVariable, vLowBins);
              if(vL1TTTrack.Perp() >= 0.29 && vL1TTTrack.Perp() < 0.3)
              {
                std::cout << "Ev. " << evNumber << ", iL1TkMuon = " << iL1TkMuon << " L1 mu matched, vL1TTTrack.Perp() = " << vL1TTTrack.Perp() << std::endl;
              }
            }
          }
        }
      }
    }
//     std::cout << "Sono qui 5 \n";
    
    // B candidates - gen level
    if(vParticlesFromB.size() == 2)
    {
      int iGenMuon1 = vParticlesFromB[0];
      int iGenMuon2 = vParticlesFromB[1];
      TLorentzVector pGenMuon1, pGenMuon2;
      pGenMuon1.SetPtEtaPhiM(Genp_pt->at(iGenMuon1), Genp_eta->at(iGenMuon1), Genp_phi->at(iGenMuon1), Genp_mass->at(iGenMuon1));
      pGenMuon2.SetPtEtaPhiM(Genp_pt->at(iGenMuon2), Genp_eta->at(iGenMuon2), Genp_phi->at(iGenMuon2), Genp_mass->at(iGenMuon2));
      TLorentzVector pDiGenMuon = pGenMuon1 + pGenMuon2;
      double massDiGenMuon = pDiGenMuon.M();
      double ptDiGenMuon = pDiGenMuon.Perp();
      double etaDiGenMuon = pDiGenMuon.Eta();
      Fill(hAllDiGenMuonInvariantMass, massDiGenMuon, genEtaBinningVariable, vLowBins);
      Fill(hAllDiGenMuonPt, ptDiGenMuon, genEtaBinningVariable, vLowBins);
      Fill(hAllDiGenMuonEta, etaDiGenMuon, genEtaBinningVariable, vLowBins);
      std::cout << "This B candidate has " << vTracksWithDistance.size() << " muons matched to reco tracks\n";
      if(vTracksWithDistance.size() > 2)
      {
        std::cout << "E R R O R ! We should never have more than 2 matched tracks!!!\n";
        exit(1);
      }
      if(vTracksWithDistance.size() == 2)
      {
        int iTTTrack1 = vTracksWithDistance[0].first;
        int iTTTrack2 = vTracksWithDistance[1].first;
        TVector3 pocaTTTrack1, pocaTTTrack2;
        pocaTTTrack1.SetXYZ(L1TTTracks_thePOCA5Par_x->at(iTTTrack1),L1TTTracks_thePOCA5Par_y->at(iTTTrack1),L1TTTracks_thePOCA5Par_z->at(iTTTrack1));
        pocaTTTrack2.SetXYZ(L1TTTracks_thePOCA5Par_x->at(iTTTrack2),L1TTTracks_thePOCA5Par_y->at(iTTTrack2),L1TTTracks_thePOCA5Par_z->at(iTTTrack2));
        TLorentzVector pTTTrack1, pTTTrack2;
        pTTTrack1.SetXYZM(L1TTTracks_theMomentum5Par_x->at(iTTTrack1),L1TTTracks_theMomentum5Par_y->at(iTTTrack1),L1TTTracks_theMomentum5Par_z->at(iTTTrack1),constants::muonMass);
        pTTTrack2.SetXYZM(L1TTTracks_theMomentum5Par_x->at(iTTTrack2),L1TTTracks_theMomentum5Par_y->at(iTTTrack2),L1TTTracks_theMomentum5Par_z->at(iTTTrack2),constants::muonMass);
        if(fabs(pTTTrack2.Eta()) > fabs(pTTTrack1.Eta()))
        {
          recoEtaBinningVariable = fabs(pTTTrack2.Eta());
        }
        else
        {
          recoEtaBinningVariable = fabs(pTTTrack1.Eta());
        }
        if(pTTTrack1.Perp() > l1SingleTrackPtThreshold && pTTTrack2.Perp() > l1SingleTrackPtThreshold && fabs(pTTTrack1.Eta()) < l1SingleTrackEtaThreshold && fabs(pTTTrack2.Eta()) < l1SingleTrackEtaThreshold)
        {
          TLorentzVector pDiTTTrack = pTTTrack1 + pTTTrack2;
          double massDiTTTrack = pDiTTTrack.M();
          double ptDiTTTrack = pDiTTTrack.Perp();
          double etaDiTTTrack = pDiTTTrack.Eta();
          double dxyDiffDiTTTrack = pocaTTTrack1.Perp() - pocaTTTrack2.Perp();
          double dzDiffDiTTTrack = pocaTTTrack1.Z() - pocaTTTrack2.Z();
          Fill(hDiTTTrackInvariantMass, massDiTTTrack, recoEtaBinningVariable, vLowBins);
          Fill(hDiTTTrackPt, ptDiTTTrack, recoEtaBinningVariable, vLowBins);
          Fill(hDiTTTrackEta, etaDiTTTrack, recoEtaBinningVariable, vLowBins);
          Fill(hDiTTTrackDxyDiff, dxyDiffDiTTTrack, recoEtaBinningVariable, vLowBins);
          Fill(hDiTTTrackDzDiff, dzDiffDiTTTrack, recoEtaBinningVariable, vLowBins);
          
          double deltaMassDiTTTrack = (massDiTTTrack - massDiGenMuon) / massDiGenMuon;
          double deltaPtDiTTTrack = (ptDiTTTrack - ptDiGenMuon) / ptDiGenMuon;
          double deltaEtaDiTTTrack = (etaDiTTTrack - etaDiGenMuon) / etaDiGenMuon;
          Fill(hDiTTTrackDeltaInvariantMass, deltaMassDiTTTrack, recoEtaBinningVariable, vLowBins);
          Fill(hDiTTTrackDeltaPt, deltaPtDiTTTrack, recoEtaBinningVariable, vLowBins);
          Fill(hDiTTTrackDeltaEta, deltaEtaDiTTTrack, recoEtaBinningVariable, vLowBins);
          
          if(vSelectedMuons.size() == 2)
          {
            Fill(hDiL1TkMuonsInvariantMass, massDiTTTrack, recoEtaBinningVariable, vLowBins);
            Fill(hDiL1TkMuonsPt, ptDiTTTrack, recoEtaBinningVariable, vLowBins);
            Fill(hDiL1TkMuonsEta, etaDiTTTrack, recoEtaBinningVariable, vLowBins);
            Fill(hDiL1TkMuonsDxyDiff, dxyDiffDiTTTrack, recoEtaBinningVariable, vLowBins);
            Fill(hDiL1TkMuonsDzDiff, dzDiffDiTTTrack, recoEtaBinningVariable, vLowBins);
            
            Fill(hDiL1TkMuonsDeltaInvariantMass, deltaMassDiTTTrack, recoEtaBinningVariable, vLowBins);
            Fill(hDiL1TkMuonsDeltaPt, deltaPtDiTTTrack, recoEtaBinningVariable, vLowBins);
            Fill(hDiL1TkMuonsDeltaEta, deltaEtaDiTTTrack, recoEtaBinningVariable, vLowBins);
          }
        }
      }
    }
//     for(std::map<unsigned int, unsigned int>::const_iterator it = mBCands.begin(); it != mBCands.end(); it++)
//     {
//       unsigned iGenBCand = it->first;
//       
//     }

    Fill(hNAllGenBCandPerEvent,mBCands.size(),genEtaBinningVariable, vLowBins);
    Fill(hNAllGenMuonsPerEvent,nGenMuons,genEtaSingleMuBinningVariable, vLowBins);
    // Just for debugging - can leave commented
//     bool bmmCandExists = false;
//     std::cout << "\n";
//     for(std::map<unsigned int, unsigned int>::const_iterator it = mBCands.begin(); it != mBCands.end(); it++)
//     {
//       std::cout << "it->first = " << it->first << ", it->second = " << it->second << std::endl;
//       if(it->second == 2)
//       {
//         bmmCandExists = true;
//       }
//       if(it->second != 2)
//       {
//         RecursivePrintDaughters(it->first);
//       }
//     }
//     if(!bmmCandExists)
//     {
//       std::cout << "Found an event with no B->mumu candidate!\n";
//       std::cout << "Printing event record:\n";
//       RecursivePrintDaughters(0);
//     }
  }
  
  // Keep this line here!
  outFile->cd();
  
  // Create and write canvases here
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hNAllGenBCandPerEvent);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hNAllGenMuonsPerEvent);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAllGenParticleGenPt);
  CreateAndWriteCanvas("cAllGenParticleGenEta", 0, 21, 1, false, false, hAllGenParticleGenEta);
  CreateAndWriteCanvas("cAllGenParticleGenPtVsEta", "colz", false, false, true, hAllGenParticleGenPtVsEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAllGenParticleGenDxy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAllGenParticleGenDz);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleGenPt);
  CreateAndWriteCanvas("cAssocTkGenParticleGenEta", 0, 21, 1, false, false, hAssocTkGenParticleGenEta);
  CreateAndWriteCanvas("cAssocTkGenParticleGenPtVsEta", "colz", false, false, true, hAssocTkGenParticleGenPtVsEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleRecoPt);
  CreateAndWriteCanvas("cAssocTkGenParticleRecoEta", 0, 21, 1, false, false, hAssocTkGenParticleRecoEta);
  CreateAndWriteCanvas("cAssocTkGenParticleRecoPtVsEta", "colz", false, false, true, hAssocTkGenParticleRecoPtVsEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleDeltaPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleDeltaEta);
  CreateAndWriteCanvases("colz", false, false, true, *hAssocTkGenParticleDeltaPtVsDeltaEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleGenDxy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleRecoDxy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleDeltaDxy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleGenDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleRecoDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleDeltaDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleDeltaR);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocTkGenParticleDeltaPtOverPt);
  CreateAndWriteCanvases("colz", false, false, true, *hAssocTkGenParticleDeltaPtOverPtVsDeltaR);
  CreateAndWriteCanvases("colz", false, false, true, *hAssocTkGenParticleDeltaDxyVsRecoPt);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleGenPt);
  CreateAndWriteCanvas("cAssocGenParticleGenEta", 0, 21, 1, false, false, hAssocGenParticleGenEta);
  CreateAndWriteCanvas("cAssocGenParticleGenPtVsEta", "colz", false, false, true, hAssocGenParticleGenPtVsEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleRecoPt);
  CreateAndWriteCanvas("cAssocGenParticleRecoEta", 0, 21, 1, false, false, hAssocGenParticleRecoEta);
  CreateAndWriteCanvas("cAssocGenParticleRecoPtVsEta", "colz", false, false, true, hAssocGenParticleRecoPtVsEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleDeltaPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleDeltaEta);
  CreateAndWriteCanvases("colz", false, false, true, *hAssocGenParticleDeltaPtVsDeltaEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleGenDxy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleRecoDxy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleDeltaDxy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleGenDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleRecoDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleDeltaDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleDeltaR);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAssocGenParticleDeltaPtOverPt);
  CreateAndWriteCanvases("colz", false, false, true, *hAssocGenParticleDeltaPtOverPtVsDeltaR);
  
  CreateAndWriteCanvasWith2DRatio("cTTTrackEfficiencyVsGenPtAndEta","Efficiency of TTTrack reconstruction vs. p_{T} and #eta", "colz", false, false, false, hAssocTkGenParticleGenPtVsEta, hAllGenParticleGenPtVsEta);
  CreateAndWriteCanvasWith2DRatio("cL1TkMuonTotalEfficiencyVsGenPtAndEta","Total efficiency of L1Tk muon reconstruction vs. p_{T} and #eta", "colz", false, false, false, hAssocGenParticleGenPtVsEta, hAllGenParticleGenPtVsEta);
  CreateAndWriteCanvasWith2DRatio("cL1TkMuonStepEfficiencyVsGenPtAndEta","Step efficiency of L1Tk muon reconstruction vs. p_{T} and #eta", "colz", false, false, false, hAssocGenParticleGenPtVsEta, hAssocTkGenParticleGenPtVsEta);
  CreateAndWriteCanvasWith2DRatio("cL1TkMuonStepEfficiencyVsRecoPtAndEta","Step efficiency of L1Tk muon reconstruction vs. reco p_{T} and #eta", "colz", false, false, false, hAssocGenParticleRecoPtVsEta, hAssocTkGenParticleRecoPtVsEta);
  
  std::vector<TGraphAsymmErrors*>* gTTTrackEfficiencyVsGenPt = CreateVectorOfGraphs<TGraphAsymmErrors>("gTTTrackEfficiencyVsGenPt", "Efficiency of TTTrack reconstruction vs. gen p_{T", "p_{T} [GeV]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonTotalEfficiencyVsGenPt = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonTotalEfficiencyVsGenPt", "Total efficiency of L1 Tk muon reconstruction vs. gen p_{T}", "p_{T} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonStepEfficiencyVsGenPt = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonStepEfficiencyVsGenPt", "Step efficiency of L1 Tk muon reconstruction vs. gen p_{T}", "p_{T} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonStepEfficiencyVsRecoPt = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonStepEfficiencyVsRecoPt", "Step efficiency of L1 Tk muon reconstruction vs. reco p_{T}", "p_{T} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  
  std::vector<TGraphAsymmErrors*>* gTTTrackEfficiencyVsGenDxy = CreateVectorOfGraphs<TGraphAsymmErrors>("gTTTrackEfficiencyVsGenDxy", "Efficiency of TTTrack reconstruction vs. gen d_{xy}", "d_{xy} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonTotalEfficiencyVsGenDxy = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonTotalEfficiencyVsGenDxy", "Total efficiency of L1 Tk muon reconstruction vs. gen d_{xy}", "d_{xy} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonStepEfficiencyVsGenDxy = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonStepEfficiencyVsGenDxy", "Step efficiency of L1 Tk muon reconstruction vs. gen d_{xy}", "d_{xy} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonStepEfficiencyVsRecoDxy = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonStepEfficiencyVsRecoDxy", "Step efficiency of L1 Tk muon reconstruction vs. reco d_{xy}", "d_{xy} [cm]", "#varepsilon", sBinningVariable, vLowBins);

  std::vector<TGraphAsymmErrors*>* gTTTrackEfficiencyVsGenDz = CreateVectorOfGraphs<TGraphAsymmErrors>("gTTTrackEfficiencyVsGenDz", "Efficiency of TTTrack reconstruction vs. gen d_{z}", "d_{z} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonTotalEfficiencyVsGenDz = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonTotalEfficiencyVsGenDz", "Total efficiency of L1 Tk muon reconstruction vs. gen d_{z}", "d_{z} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonStepEfficiencyVsGenDz = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonStepEfficiencyVsGenDz", "Step efficiency of L1 Tk muon reconstruction vs. gen d_{z}", "d_{z} [cm]", "#varepsilon", sBinningVariable, vLowBins);
  std::vector<TGraphAsymmErrors*>* gL1TkMuonStepEfficiencyVsRecoDz = CreateVectorOfGraphs<TGraphAsymmErrors>("gL1TkMuonStepEfficiencyVsRecoDz", "Step efficiency of L1 Tk muon reconstruction vs. reco d_{z}", "d_{z} [cm]", "#varepsilon", sBinningVariable, vLowBins);

  for(unsigned int iBin = 0; iBin < vLowBins.size(); iBin++)
  {
    ClopperPearsonDivide(hAssocTkGenParticleGenPt->at(iBin),hAllGenParticleGenPt->at(iBin), gTTTrackEfficiencyVsGenPt->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleGenPt->at(iBin), hAllGenParticleGenPt->at(iBin), gL1TkMuonTotalEfficiencyVsGenPt->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleGenPt->at(iBin), hAssocTkGenParticleGenPt->at(iBin), gL1TkMuonStepEfficiencyVsGenPt->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleRecoPt->at(iBin), hAssocTkGenParticleRecoPt->at(iBin), gL1TkMuonStepEfficiencyVsRecoPt->at(iBin));

    ClopperPearsonDivide(hAssocTkGenParticleGenDxy->at(iBin),hAllGenParticleGenDxy->at(iBin), gTTTrackEfficiencyVsGenDxy->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleGenDxy->at(iBin), hAllGenParticleGenDxy->at(iBin), gL1TkMuonTotalEfficiencyVsGenDxy->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleGenDxy->at(iBin), hAssocTkGenParticleGenDxy->at(iBin), gL1TkMuonStepEfficiencyVsGenDxy->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleRecoDxy->at(iBin), hAssocTkGenParticleRecoDxy->at(iBin), gL1TkMuonStepEfficiencyVsRecoDxy->at(iBin));
    ClopperPearsonDivide(hAssocTkGenParticleGenDz->at(iBin),hAllGenParticleGenDz->at(iBin), gTTTrackEfficiencyVsGenDz->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleGenDz->at(iBin), hAllGenParticleGenDz->at(iBin), gL1TkMuonTotalEfficiencyVsGenDz->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleGenDz->at(iBin), hAssocTkGenParticleGenDz->at(iBin), gL1TkMuonStepEfficiencyVsGenDz->at(iBin));
    ClopperPearsonDivide(hAssocGenParticleRecoDz->at(iBin), hAssocTkGenParticleRecoDz->at(iBin), gL1TkMuonStepEfficiencyVsRecoDz->at(iBin));
  }
  CreateAndWriteCanvases(21, 1, false, false, *gTTTrackEfficiencyVsGenPt);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonTotalEfficiencyVsGenPt);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonStepEfficiencyVsGenPt);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonStepEfficiencyVsRecoPt);

  CreateAndWriteCanvases(21, 1, false, false, *gTTTrackEfficiencyVsGenDxy);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonTotalEfficiencyVsGenDxy);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonStepEfficiencyVsGenDxy);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonStepEfficiencyVsRecoDxy);
  CreateAndWriteCanvases(21, 1, false, false, *gTTTrackEfficiencyVsGenDz);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonTotalEfficiencyVsGenDz);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonStepEfficiencyVsGenDz);
  CreateAndWriteCanvases(21, 1, false, false, *gL1TkMuonStepEfficiencyVsRecoDz);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAllDiGenMuonInvariantMass);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAllDiGenMuonPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hAllDiGenMuonEta);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiTTTrackInvariantMass);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiTTTrackPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiTTTrackEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiTTTrackDxyDiff);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiTTTrackDzDiff);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiTTTrackDeltaInvariantMass);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiTTTrackDeltaPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiTTTrackDeltaEta);

  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiL1TkMuonsInvariantMass);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiL1TkMuonsPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiL1TkMuonsEta);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiL1TkMuonsDxyDiff);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiL1TkMuonsDzDiff);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiL1TkMuonsDeltaInvariantMass);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiL1TkMuonsDeltaPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hDiL1TkMuonsDeltaEta);
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // BmmL1TTStudies_cxx
