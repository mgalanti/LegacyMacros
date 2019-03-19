#ifndef MuonBasicDistributionsDataForBmm2_cxx
#define MuonBasicDistributionsDataForBmm2_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include <math.h>

#include "MuonBasicDistributionsDataForBmm2.h"
#include "../utilities/constants.h"



void MuonBasicDistributionsDataForBmm2::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  // MC matching parameters
//   Default
  double maxDistanceForMatching = 0.12;
  double maxDPtOverPtForMatching = 0.3;
// Stricter matchings for systematics
// //   Option 1
//   double maxDistanceForMatching = 0.1;
//   double maxDPtOverPtForMatching = 0.1;
//   // Option 2
//   double maxDistanceForMatching = 0.1;
//   double maxDPtOverPtForMatching = 0.05;
// //   Option 3
//   double maxDistanceForMatching = 0.01;
//   double maxDPtOverPtForMatching = 0.1;
  
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
  else if(sampleName.find("JPsiK") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B*ToJPsiK sample: only reco muons matched to generated kaons will be selected.\n";
    trueGenpId = 321;
  }
  else if(sampleName.find("PMuNu") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a *B*ToPMuNu sample: only reco muons matched to generated protons will be selected.\n";
    trueGenpId = 2212;
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
  else if(sampleName.find("BuTo") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a B_u* sample: only gen particles coming from a B_u mom will be selected.\n";
    trueBId = 521;
  }
  else if(sampleName.find("LambdaBTo") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a LambdaB* sample: only gen particles coming from a Lambda_b mom will be selected.\n";
    trueBId = 5122;
  }
  else
  {
    std::cout << "W A R N I N G ! Could not infer the sample type from the name!\n";
    std::cout << " MC matching of reco muons is disabled!\n";
  }
  
  bool needsJPsi=false;
  if(sampleName.find("JPsi") != std::string::npos)
  {
    std::cout << "I N F O : Analyzing a *JPsi* sample: only particles with a JPsi sister will be considered.\n";
    needsJPsi=true;
  }

  std::map<std::string,muon::SelectionType> mSelectionType = s->GetMuonSelectionTypeMap();
  
  std::cout << "Analyzing " << nentries << " events.\n";
  
  // Histogram binning and labels
  int nPtVarBins = 4; // for variable binning
  double ptVarBins[7]={0,4,6,10,30}; // for variable binning
  const unsigned int nPtBins = 150;
  const double ptMin = 0.;
  const double ptMax = 30.;
  const char* genPtLabel = "gen p_{T} [GeV/c]";
  const char* recoPtLabel = "reco p_{T} [GeV/c]";
  int nEtaVarBins = 3; // for variable binning
  double etaVarBins[7]={0.,0.9,1.4,2.4}; // for variable binning
  const unsigned int nEtaBins = 60;
  const double etaMin = -3.;
  const double etaMax = 3.;
  const char* genEtaLabel = "gen #eta";
  const char* recoEtaLabel = "reco #eta";
  
  const char* nMuLabel = "N_{#mu}";
  
  // Book histograms here
  TH1D* hSelMuonDeltaRFromGenp = Create1DHistogram<TH1D>("hSelMuonDeltaRFromGenp","#Delta(R) of gen particle from closest matching selected muon", 100, 0., 1., "#Delta(R)", nMuLabel);
  TH1D* hSelMuonDeltaPtOverPtFromGenp = Create1DHistogram<TH1D>("hSelMuonDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}| of gen particle from closest matching selected muon", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel);
  TH2D* hSelMuonDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hSelMuonDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching selected muon",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH1D* hSelMuonPlusDeltaRFromGenp = Create1DHistogram<TH1D>("hSelMuonPlusDeltaRFromGenp","#Delta(R) of positive gen particle from closest matching selected muon", 100, 0., 1., "#Delta(R)", nMuLabel);
  TH1D* hSelMuonPlusDeltaPtOverPtFromGenp = Create1DHistogram<TH1D>("hSelMuonPlusDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}| of positive gen particle from closest matching selected muon", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel);
  TH2D* hSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of positive gen particle from closest matching selected muon",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH1D* hSelMuonMinusDeltaRFromGenp = Create1DHistogram<TH1D>("hSelMuonMinusDeltaRFromGenp","#Delta(R) of negative gen particle from closest matching selected muon", 100, 0., 1., "#Delta(R)", nMuLabel);
  TH1D* hSelMuonMinusDeltaPtOverPtFromGenp = Create1DHistogram<TH1D>("hSelMuonMinusDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}| of negative gen particle from closest matching selected muon", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel);
  TH2D* hSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of negative gen particle from closest matching selected muon",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH1D* hSelMuonFakeClass = Create1DHistogram<TH1D>("hSelMuonFakeClass","Fake classification of selected muons", 51, -0.5,50.5,"Class",nMuLabel);
  TH1D* hSelMuonPlusFakeClass = Create1DHistogram<TH1D>("hSelMuonPlusFakeClass","Fake classification of positive selected muons", 51, -0.5,50.5,"Class",nMuLabel);
  TH1D* hSelMuonMinusFakeClass = Create1DHistogram<TH1D>("hSelMuonMinusFakeClass","Fake classification of negative selected muons", 51, -0.5,50.5,"Class",nMuLabel);
  
  TH1D* hTrackNearJPsiDauDeltaRFromGenp = Create1DHistogram<TH1D>("hTrackNearJPsiDauDeltaRFromGenp", "#Delta(R) of matching track near J/#psi daughters from gen particle", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hTrackNearJPsiDauDeltaPtOverPtFromGenp = Create1DHistogram<TH1D>("hTrackNearJPsiDauDeltaPtOverPtFromGenp", "|#Delta(p_{T})/p_{T}| of matching track near J/#psi daughters from gen particle", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH2D* hTrackNearJPsiDauDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackNearJPsiDauDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of matching track near J/#psi daughters from gen particle",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  
  TH1D* hTrackDeltaRFromJPsiDau = Create1DHistogram<TH1D>("hTrackDeltaRFromJPsiDau", "#Delta(R) of matching track from J/#psi daughters", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hTrackDeltaPtOverPtFromJPsiDau = Create1DHistogram<TH1D>("hTrackDeltaPtOverPtFromJPsiDau", "|#Delta(p_{T})/p_{T}| of matching track from J/#psi daughters", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH1D* hTrackDeltaRFromGenp = Create1DHistogram<TH1D>("hTrackDeltaRFromGenp","#Delta(R) of gen particle from closest matching track", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hTrackDeltaPtOverPtFromGenp = Create1DHistogram<TH1D>("hTrackDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}-1| of gen particle from closest matching track", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH2D* hTrackDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching track",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH2D* hTrackChi2NDOFVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackChi2NDOFVsDeltaPtOverPtFromGenp","#chi^{2}/NDOF vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching track",100,0.,1.,100,0.,10.,"|#Delta(p_{T})/p_{T}|","#chi^{2}/NDOF");
  TH2D* hTrackNDOFVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackNDOFVsDeltaPtOverPtFromGenp","NDOF vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching track",100,0.,1., 51, -0.5, 50.5,"|#Delta(p_{T})/p_{T}|","NDOF");
  TH1D* hTrackPlusDeltaRFromGenp = Create1DHistogram<TH1D>("hTrackPlusDeltaRFromGenp","#Delta(R) of positive gen particle from closest matching track", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hTrackPlusDeltaPtOverPtFromGenp = Create1DHistogram<TH1D>("hTrackPlusDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}| of positive gen particle from closest matching track", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH2D* hTrackPlusDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackPlusDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of positive gen particle from closest matching track",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH1D* hTrackMinusDeltaRFromGenp = Create1DHistogram<TH1D>("hTrackMinusDeltaRFromGenp","#Delta(R) of negative gen particle from closest matching track", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hTrackMinusDeltaPtOverPtFromGenp = Create1DHistogram<TH1D>("hTrackMinusDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}-1| of negative gen particle from closest matching track", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH2D* hTrackMinusDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackMinusDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of negative gen particle from closest matching track",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  
  TH1D* hSelMuonDeltaRFromAssocGenp = Create1DHistogram<TH1D>("hSelMuonDeltaRFromAssocGenp","#Delta(R) of gen particle from closest matching selected muon for successful associations", 100, 0., 1., "#Delta(R)", nMuLabel);
  TH1D* hSelMuonDeltaPtOverPtFromAssocGenp = Create1DHistogram<TH1D>("hSelMuonDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}| of gen particle from closest matching selected muon for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel);
  TH2D* hSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching selected muon for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH1D* hSelMuonPlusDeltaRFromAssocGenp = Create1DHistogram<TH1D>("hSelMuonPlusDeltaRFromAssocGenp","#Delta(R) of positive gen particle from closest matching selected muon for successful associations", 100, 0., 1., "#Delta(R)", nMuLabel);
  TH1D* hSelMuonPlusDeltaPtOverPtFromAssocGenp = Create1DHistogram<TH1D>("hSelMuonPlusDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of positive gen particle from closest matching selected muon for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel);
  TH2D* hSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of positive gen particle from closest matching selected muon for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH1D* hSelMuonMinusDeltaRFromAssocGenp = Create1DHistogram<TH1D>("hSelMuonMinusDeltaRFromAssocGenp","#Delta(R) of negative gen particle from closest matching selected muon for successful associations", 100, 0., 1., "#Delta(R)", nMuLabel);
  TH1D* hSelMuonMinusDeltaPtOverPtFromAssocGenp = Create1DHistogram<TH1D>("hSelMuonMinusDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of negative gen particle from closest matching selected muon for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel);
  TH2D* hSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of negative gen particle from closest matching selected muon for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  
  TH1D* hAssocTrackDeltaRFromJPsiDau = Create1DHistogram<TH1D>("hAssocTrackDeltaRFromJPsiDau", "#Delta(R) of matching track from J/#psi daughters for successful associations", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hAssocTrackDeltaPtOverPtFromJPsiDau = Create1DHistogram<TH1D>("hAssocTrackDeltaPtOverPtFromJPsiDau", "|#Delta(p_{T})/p_{T}| of matching track from J/#psi daughters for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH1D* hTrackDeltaRFromAssocGenp = Create1DHistogram<TH1D>("hTrackDeltaRFromAssocGenp","#Delta(R) of gen particle from closest matching track for successful associations", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hTrackDeltaPtOverPtFromAssocGenp = Create1DHistogram<TH1D>("hTrackDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}| of gen particle from closest matching track for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH2D* hTrackDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hTrackDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching track for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH2D* hTrackChi2NDOFVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hTrackChi2NDOFVsDeltaPtOverPtFromAssocGenp","#chi^{2}/NDOF vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching track for successful associations",100,0.,1.,100,0.,10.,"|#Delta(p_{T})/p_{T}|","#chi^{2}/NDOF");
  TH2D* hTrackNDOFVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hTrackNDOFVsDeltaPtOverPtFromAssocGenp","NDOF vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching track for successful association",100,0.,1., 51, -0.5, 50.5,"|#Delta(p_{T})/p_{T}|","NDOF");
  TH1D* hTrackPlusDeltaRFromAssocGenp = Create1DHistogram<TH1D>("hTrackPlusDeltaRFromAssocGenp","#Delta(R) of positive gen particle from closest matching track for successful associations", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hTrackPlusDeltaPtOverPtFromAssocGenp = Create1DHistogram<TH1D>("hTrackPlusDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of positive gen particle from closest matching track for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH2D* hTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of positive gen particle from closest matching track for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  TH1D* hTrackMinusDeltaRFromAssocGenp = Create1DHistogram<TH1D>("hTrackMinusDeltaRFromAssocGenp","#Delta(R) of negative gen particle from closest matching track for successful associations", 100, 0., 1., "#Delta(R)", "N_{tracks}");
  TH1D* hTrackMinusDeltaPtOverPtFromAssocGenp = Create1DHistogram<TH1D>("hTrackMinusDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of negative gen particle from closest matching track for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}");
  TH2D* hTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of negative gen particle from closest matching track for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  
  TH1D* hAllGenParticleGenPt = Create1DHistogram<TH1D>("hAllGenParticleGenPt","p_{T} of all gen particles coming from B decays", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocGenParticleGenPt = Create1DHistogram<TH1D>("hAssocGenParticleGenPt","p_{T} of gen particles coming from B decays associated to a muon", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocGenParticleRecoPt = Create1DHistogram<TH1D>("hAssocGenParticleRecoPt","reco p_{T} of gen particles coming from B decays associated to a muon", nPtVarBins, ptVarBins, recoPtLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticleGenPt = Create1DHistogram<TH1D>("hAssocTkGenParticleGenPt","p_{T} of gen particles coming from B decays associated to a track", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticleRecoPt = Create1DHistogram<TH1D>("hAssocTkGenParticleRecoPt","reco p_{T} of gen particles coming from B decays associated to a track", nPtVarBins, ptVarBins, recoPtLabel,"n_{GenP}");
  TH1D* hAllGenParticleGenEta = Create1DHistogram<TH1D>("hAllGenParticleGenEta","#eta of all gen particles coming from B decays", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocGenParticleGenEta = Create1DHistogram<TH1D>("hAssocGenParticleGenEta","#eta of gen particles coming from B decays associated to a muon", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocGenParticleRecoEta = Create1DHistogram<TH1D>("hAssocGenParticleRecoEta","reco #eta of gen particles coming from B decays associated to a muon", nEtaVarBins, etaVarBins, recoEtaLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticleGenEta = Create1DHistogram<TH1D>("hAssocTkGenParticleGenEta","#eta of gen particles coming from B decays associated to a track", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticleRecoEta = Create1DHistogram<TH1D>("hAssocTkGenParticleRecoEta","reco #eta of gen particles coming from B decays associated to a track", nEtaVarBins, etaVarBins, recoEtaLabel,"n_{GenP}");
  TH2D* hAllGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAllGenParticleGenPtVsEta","p_{T} vs. #eta of all gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleGenPtVsEta","p_{T} vs. #eta of gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticleRecoPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleRecoPtVsEta","reco p_{T} vs. #eta of gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAssocTkGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleGenPtVsEta","p_{T} vs. #eta of gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocTkGenParticleRecoPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleRecoPtVsEta","reco p_{T} vs. #eta of gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);

  TH1D* hAllGenParticlePlusGenPt = Create1DHistogram<TH1D>("hAllGenParticlePlusGenPt","p_{T} of all positive gen particles coming from B decays", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocGenParticlePlusGenPt = Create1DHistogram<TH1D>("hAssocGenParticlePlusGenPt","p_{T} of positive gen particles coming from B decays associated to a muon", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocGenParticlePlusRecoPt = Create1DHistogram<TH1D>("hAssocGenParticlePlusRecoPt","reco p_{T} of positive gen particles coming from B decays associated to a muon", nPtVarBins, ptVarBins, recoPtLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticlePlusGenPt = Create1DHistogram<TH1D>("hAssocTkGenParticlePlusGenPt","p_{T} of positive gen particles coming from B decays associated to a track", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticlePlusRecoPt = Create1DHistogram<TH1D>("hAssocTkGenParticlePlusRecoPt","reco p_{T} of positive gen particles coming from B decays associated to a track", nPtVarBins, ptVarBins, recoPtLabel,"n_{GenP}");
  TH1D* hAllGenParticlePlusGenEta = Create1DHistogram<TH1D>("hAllGenParticlePlusGenEta","#eta of all positive gen particles coming from B decays", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocGenParticlePlusGenEta = Create1DHistogram<TH1D>("hAssocGenParticlePlusGenEta","#eta of positive gen particles coming from B decays associated to a muon", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocGenParticlePlusRecoEta = Create1DHistogram<TH1D>("hAssocGenParticlePlusRecoEta","reco #eta of positive gen particles coming from B decays associated to a muon", nEtaVarBins, etaVarBins, recoEtaLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticlePlusGenEta = Create1DHistogram<TH1D>("hAssocTkGenParticlePlusGenEta","#eta of positive gen particles coming from B decays associated to a track", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticlePlusRecoEta = Create1DHistogram<TH1D>("hAssocTkGenParticlePlusRecoEta","reco #eta positive of gen particles coming from B decays associated to a track", nEtaVarBins, etaVarBins, recoEtaLabel,"n_{GenP}");  
  TH2D* hAllGenParticlePlusGenPtVsEta = Create2DHistogram<TH2D>("hAllGenParticlePlusGenPtVsEta","p_{T} vs. #eta of all positive gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticlePlusGenPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticlePlusGenPtVsEta","p_{T} vs. #eta of positive gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticlePlusRecoPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticlePlusRecoPtVsEta","reco p_{T} vs. #eta of positive gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAssocTkGenParticlePlusGenPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticlePlusGenPtVsEta","p_{T} vs. #eta of positive gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocTkGenParticlePlusRecoPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticlePlusRecoPtVsEta","reco p_{T} vs. #eta of positive gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  
  TH1D* hAllGenParticleMinusGenPt = Create1DHistogram<TH1D>("hAllGenParticleMinusGenPt","p_{T} of all negative gen particles coming from B decays", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocGenParticleMinusGenPt = Create1DHistogram<TH1D>("hAssocGenParticleMinusGenPt","p_{T} of negative gen particles coming from B decays associated to a muon", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocGenParticleMinusRecoPt = Create1DHistogram<TH1D>("hAssocGenParticleMinusRecoPt","reco p_{T} of negative gen particles coming from B decays associated to a muon", nPtVarBins, ptVarBins, recoPtLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticleMinusGenPt = Create1DHistogram<TH1D>("hAssocTkGenParticleMinusGenPt","p_{T} of negative gen particles coming from B decays associated to a track", nPtVarBins, ptVarBins, genPtLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticleMinusRecoPt = Create1DHistogram<TH1D>("hAssocTkGenParticleMinusRecoPt","reco p_{T} of negative gen particles coming from B decays associated to a track", nPtVarBins, ptVarBins, recoPtLabel,"n_{GenP}");
  TH1D* hAllGenParticleMinusGenEta = Create1DHistogram<TH1D>("hAllGenParticleMinusGenEta","#eta of all negative gen particles coming from B decays", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocGenParticleMinusGenEta = Create1DHistogram<TH1D>("hAssocGenParticleMinusGenEta","#eta of negative gen particles coming from B decays associated to a muon", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocGenParticleMinusRecoEta = Create1DHistogram<TH1D>("hAssocGenParticleMinusRecoEta","reco #eta of negative gen particles coming from B decays associated to a muon", nEtaVarBins, etaVarBins, recoEtaLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticleMinusGenEta = Create1DHistogram<TH1D>("hAssocTkGenParticleMinusGenEta","#eta of negative gen particles coming from B decays associated to a track", nEtaVarBins, etaVarBins, genEtaLabel,"n_{GenP}");
  TH1D* hAssocTkGenParticleMinusRecoEta = Create1DHistogram<TH1D>("hAssocTkGenParticleMinusRecoEta","reco #eta of negative gen particles coming from B decays associated to a track", nEtaVarBins, etaVarBins, recoEtaLabel,"n_{GenP}");  
  TH2D* hAllGenParticleMinusGenPtVsEta = Create2DHistogram<TH2D>("hAllGenParticleMinusGenPtVsEta","p_{T} vs. #eta of all negative gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticleMinusGenPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleMinusGenPtVsEta","p_{T} vs. #eta of negative gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticleMinusRecoPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleMinusRecoPtVsEta","reco p_{T} vs. #eta of negative gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAssocTkGenParticleMinusGenPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleMinusGenPtVsEta","p_{T} vs. #eta of negative gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocTkGenParticleMinusRecoPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleMinusRecoPtVsEta","reco p_{T} vs. #eta of negative gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  
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
    // Use only default selection
    // Selection string is passed to ParticleSelector
    // since the constructor of TreeAnalyzer*
    if(!s->MuonSelectorIsValid()) 
    {
      std::cout << "E R R O R ! MuonSelector is not valid!\nExiting..." << std::endl;
      exit(1);
    }
//     if(needsJPsi)
//     {
//       if(!s->SelectEventPassingTrigger("HLT_DoubleMu4_Jpsi_Displaced_v9"))
//       {
//         continue;
//       }
//     }
        
    // Loop over gen particles to get the kaons and pions from B decay
    std::vector<unsigned short> vParticlesFromB;
    std::vector<std::pair<short, double> > vTracksWithDistance;
    std::vector<bool> vTrackIsNearJPsiDau;
    std::vector<unsigned short> vParticlesInAccFromB;
    std::vector<unsigned short> vParticlesPlusFromB;
    std::vector<unsigned short> vParticlesPlusInAccFromB;
    std::vector<unsigned short> vParticlesMinusFromB;
    std::vector<unsigned short> vParticlesMinusInAccFromB;
    for(unsigned int iGenParticle = 0; iGenParticle < Genp_Id->size(); iGenParticle++)
    {
      if(Genp_pt->at(iGenParticle) > 0)
      {
        int genpId = Genp_Id->at(iGenParticle);
        if(abs(genpId) == trueGenpId)
        {
          int iMother = Genp_vMom->at(iGenParticle).at(0);
          int motherId = Genp_Id->at(iMother);
          std::vector<short unsigned int> vJPsiDaus;
//           if(abs(motherId) == trueBId)
          if(abs(motherId) == trueBId/* && Genp_pt->at(iMother) > 7.5*/)
          {
            bool hasJPsi = false;
            if(needsJPsi)
            {
              for(unsigned int i = 0; i < Genp_vDau->at(iMother).size(); i++)
              {
                int iSister = Genp_vDau->at(iMother).at(i);
                if(abs(Genp_Id->at(iSister)) == 443)
                {
                  hasJPsi = true;
                  // Save vector of JPsi daughters
//                  RecursivePrintDaughters(iSister);
                  vJPsiDaus = Genp_vDau->at(iSister);
                  break;
                }
              }
            }
            if(hasJPsi || !needsJPsi)
            {
//               std::cout << "Found a kaon or a pion or a proton coming from a B!\n";
//               std::cout << "I = " << iGenParticle << std::endl;
//               std::cout << "ID = " << genpId << std::endl;
//               std::cout << "iMother = " << iMother << std::endl;
//               std::cout << "motherId = " << motherId << std::endl;
//               std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticle(iGenParticle);
              std::vector<std::pair<short int, double> > vTracksWithDistanceJPsiDaus;
              for(unsigned int iDau = 0; iDau < vJPsiDaus.size(); iDau++)
              {
                std::pair<short int, double> iTrackWithDistanceJPsiDau = FindTrackNearestToGenParticleWithPtThreshold(vJPsiDaus[iDau], 0.0, true);
                vTracksWithDistanceJPsiDaus.push_back(iTrackWithDistanceJPsiDau);
              }
              std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticleWithPtThreshold(iGenParticle, 0.0, true);
              short int iTrack = iTrackWithDistance.first;
              double distance = iTrackWithDistance.second;
              double dPtOverPtTrack = 99999;
              if(iTrack != -1)
                dPtOverPtTrack = fabs((Track_pt->at(iTrack) - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle));
//               std::cout << "Track associated to the K/pi/p has i = " << iTrack << ", distance = " << distance << ", and dpt/pt = " << dPtOverPtTrack << std::endl;
              // Reject tracks near to JPsi daughters
              bool trackIsNearJPsiDau = false;
              for(unsigned int iDau = 0; iDau < vJPsiDaus.size(); iDau++)
              {
                short unsigned int iJPsiDau = vJPsiDaus[iDau];
                TLorentzVector pJPsiDau, pTrack;
                pJPsiDau.SetPtEtaPhiM(Genp_pt->at(iJPsiDau), Genp_eta->at(iJPsiDau), Genp_phi->at(iJPsiDau), Genp_mass->at(iJPsiDau));
                pTrack.SetPtEtaPhiM(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack), constants::muonMass);
                double dPtOverPtWrtJPsiDau = fabs((Track_pt->at(iTrack) - Genp_pt->at(vJPsiDaus[iDau])) / Genp_pt->at(vJPsiDaus[iDau]));
//                 std::cout << "     Track " << iTrack << " has DeltaR = " << pTrack.DeltaR(pJPsiDau) << " and dpt/pt = " << dPtOverPtWrtJPsiDau << " from J/psi daughter " << iDau << std::endl; 
                hTrackDeltaRFromJPsiDau->Fill(pTrack.DeltaR(pJPsiDau));
                hTrackDeltaPtOverPtFromJPsiDau->Fill(dPtOverPtWrtJPsiDau);
                if(pTrack.DeltaR(pJPsiDau) <= 0.02 && dPtOverPtWrtJPsiDau < 0.05)
                {
                  trackIsNearJPsiDau = true;
                  hTrackNearJPsiDauDeltaRFromGenp->Fill(distance);
                  hTrackNearJPsiDauDeltaPtOverPtFromGenp->Fill(dPtOverPtTrack);
                  hTrackNearJPsiDauDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPtTrack,distance);
                }
              }
              if(trackIsNearJPsiDau)
              {
                std::cout << "I N F O : Rejecting gen muon because its associated track is near to a JPsi daughter.\n";
//                 continue;
              }
              double dPtOverPt = 999.9;
              if(iTrack!=-1)
              {
                dPtOverPt = fabs((Track_pt->at(iTrack) - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle));
              }
//               else
//               {
//                 if(numberOfTrack!=0)
//                 {
//                   std::cout << "Found iTrack=-1 - numberOfTrack = " << numberOfTrack << "\n";
//                   std::cout << "Genparticle: pt = " << Genp_pt->at(iGenParticle) << ", eta = " << Genp_eta->at(iGenParticle) << std::endl;
//                 }
//               }
              vParticlesFromB.push_back(iGenParticle);
              vTracksWithDistance.push_back(iTrackWithDistance);
              vTrackIsNearJPsiDau.push_back(trackIsNearJPsiDau);
              if(Genp_charge->at(iGenParticle) > 0)
              {
                vParticlesPlusFromB.push_back(iGenParticle);
              }
              else
              {
                vParticlesMinusFromB.push_back(iGenParticle);
              }
              
              if(iTrack!=-1)
              {
                if(Track_pt->at(iTrack) > 4 && fabs(Track_eta->at(iTrack))<2.4)
                {
                  hTrackDeltaRFromGenp->Fill(iTrackWithDistance.second);
                  hTrackDeltaPtOverPtFromGenp->Fill(dPtOverPt);
                  hTrackDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                  hTrackChi2NDOFVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,Track_normalizedChi2->at(iTrack));
                  hTrackNDOFVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,Track_ndof->at(iTrack));
                  if(Genp_charge->at(iGenParticle) > 0)
                  {
                    hTrackPlusDeltaRFromGenp->Fill(iTrackWithDistance.second);
                    hTrackPlusDeltaPtOverPtFromGenp->Fill(dPtOverPt);
                    hTrackPlusDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                  }
                  else
                  {
                    hTrackMinusDeltaRFromGenp->Fill(iTrackWithDistance.second);
                    hTrackMinusDeltaPtOverPtFromGenp->Fill(dPtOverPt);
                    hTrackMinusDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                  }

                  if(iTrackWithDistance.second < maxDistanceForMatching && dPtOverPt < maxDPtOverPtForMatching && !trackIsNearJPsiDau)
                  {
                    for(unsigned int iDau = 0; iDau < vJPsiDaus.size(); iDau++)
                    {
                      short unsigned int iJPsiDau = vJPsiDaus[iDau];
                      TLorentzVector pJPsiDau, pTrack;
                      pJPsiDau.SetPtEtaPhiM(Genp_pt->at(iJPsiDau), Genp_eta->at(iJPsiDau), Genp_phi->at(iJPsiDau), Genp_mass->at(iJPsiDau));
                      pTrack.SetPtEtaPhiM(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack), constants::muonMass);
                      double dPtOverPtWrtJPsiDau = fabs((Track_pt->at(iTrack) - Genp_pt->at(vJPsiDaus[iDau])) / Genp_pt->at(vJPsiDaus[iDau]));
                      //                 std::cout << "This track has DeltaR = " << pTrack.DeltaR(pJPsiDau) << " and dpt/pt = " << dPtOverPtWrtJPsiDau << " from J/psi daughter " << iDau << std::endl; 
                      hAssocTrackDeltaRFromJPsiDau->Fill(pTrack.DeltaR(pJPsiDau));
                      hAssocTrackDeltaPtOverPtFromJPsiDau->Fill(dPtOverPtWrtJPsiDau);
                    }
                    hAssocTkGenParticleRecoPt->Fill(Track_pt->at(iTrack));
                    hAssocTkGenParticleRecoEta->Fill(fabs(Track_eta->at(iTrack)));
                    hTrackDeltaRFromAssocGenp->Fill(iTrackWithDistance.second);
                    hTrackDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt);
                    hTrackDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                    hTrackChi2NDOFVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,Track_normalizedChi2->at(iTrack));
                    hTrackNDOFVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,Track_ndof->at(iTrack));
                    hAssocTkGenParticleRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
                    if(Track_charge->at(iTrack) > 0)
                    {                      
                      hTrackPlusDeltaRFromAssocGenp->Fill(iTrackWithDistance.second);
                      hTrackPlusDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt);
                      hTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                      hAssocTkGenParticlePlusRecoPt->Fill(Track_pt->at(iTrack));
                      hAssocTkGenParticlePlusRecoEta->Fill(fabs(Track_eta->at(iTrack)));
                      hAssocTkGenParticlePlusRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
                    }
                    else
                    {
                      hTrackMinusDeltaRFromAssocGenp->Fill(iTrackWithDistance.second);
                      hTrackMinusDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt);
                      hTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                      hAssocTkGenParticleMinusRecoPt->Fill(Track_pt->at(iTrack));
                      hAssocTkGenParticleMinusRecoEta->Fill(fabs(Track_eta->at(iTrack)));
                      hAssocTkGenParticleMinusRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
                    }
                  }
                }
              }
              
              if(Genp_pt->at(iGenParticle) > 4 && fabs(Genp_eta->at(iGenParticle)) < 2.4)
              {
                vParticlesInAccFromB.push_back(iGenParticle);
                hAllGenParticleGenPt->Fill(Genp_pt->at(iGenParticle));
                hAllGenParticleGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                hAllGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                if(Genp_charge->at(iGenParticle) > 0)
                {
                  vParticlesPlusInAccFromB.push_back(iGenParticle);
                  hAllGenParticlePlusGenPt->Fill(Genp_pt->at(iGenParticle));
                  hAllGenParticlePlusGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                  hAllGenParticlePlusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                }
                else
                {
                  vParticlesMinusInAccFromB.push_back(iGenParticle);
                  hAllGenParticleMinusGenPt->Fill(Genp_pt->at(iGenParticle));
                  hAllGenParticleMinusGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                  hAllGenParticleMinusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                }
                
                if(iTrackWithDistance.second < maxDistanceForMatching && dPtOverPt < maxDPtOverPtForMatching && !trackIsNearJPsiDau)
                {
                  hAssocTkGenParticleGenPt->Fill(Genp_pt->at(iGenParticle));
                  hAssocTkGenParticleGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                  hAssocTkGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                  if(Genp_charge->at(iGenParticle) > 0)
                  {
                    hAssocTkGenParticlePlusGenPt->Fill(Genp_pt->at(iGenParticle));
                    hAssocTkGenParticlePlusGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                    hAssocTkGenParticlePlusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                  }
                  else
                  {
                    hAssocTkGenParticleMinusGenPt->Fill(Genp_pt->at(iGenParticle));
                    hAssocTkGenParticleMinusGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                    hAssocTkGenParticleMinusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                  }
                }
              }
            }
          }
        }
      }
    }
    
//     std::cout << "vParticlesFromB.size() = " << vParticlesFromB.size() << std::endl;
//     if(numberOfMuon)
//       std::cout << "Event " << jentry << ", numberOfMuon = " << numberOfMuon << std::endl;

    std::vector<double> binningVariables;
    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      if(s->SelectMuon(iMuon) && fabs(Muon_eta->at(iMuon))<2.4)
      {
//         std::cout << "muon " << iMuon << " selected!\n";
        short int trackRef = Muon_trackref->at(iMuon);
        for(unsigned int iParticleFromB = 0; iParticleFromB < vParticlesFromB.size(); iParticleFromB++)
        {
          unsigned short iGenParticle = vParticlesFromB[iParticleFromB];
          std::pair<short int, double> iTrackWithDistance = vTracksWithDistance[iParticleFromB];
          short int iTrack = iTrackWithDistance.first;
          bool trackIsNearJPsiDau = vTrackIsNearJPsiDau[iParticleFromB];
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
//               std::cout << "iGenParticle = " << iGenParticle << ", jGenParticle = " << jGenParticle << ", j ID = " << Genp_Id->at(jGenParticle) << std::endl;
//               hasMuonDaughter = (abs(RecursiveLookForDaughterId(jGenParticle, 13, false)) != 65535);
//               if(hasMuonDaughter)
//               {
//                 
//                 std::cout << "The other B daughter has decayed to a muon!\n";
//                 std::cout << "Daughters of kaon under study:\n";
//                 RecursivePrintDaughters(iGenParticle);
//                 std::cout << "Daughters of the other kaon:\n";
//                 RecursivePrintDaughters(jGenParticle);
//               }
            }
//             if(hasMuonDaughter)
//               std::cout << "EVENT: " << evNumber << ":" << lumiSection << ":" << runNumber << std::endl;
            double dPtOverPt = fabs((Track_pt->at(trackRef) - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle));
            hSelMuonDeltaRFromGenp->Fill(iTrackWithDistance.second);
            hSelMuonDeltaPtOverPtFromGenp->Fill(dPtOverPt);
            hSelMuonDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
            if(Genp_charge->at(iGenParticle) > 0)
            {
              hSelMuonPlusDeltaRFromGenp->Fill(iTrackWithDistance.second);
              hSelMuonPlusDeltaPtOverPtFromGenp->Fill(dPtOverPt);
              hSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
            }
            else
            {
              hSelMuonMinusDeltaRFromGenp->Fill(iTrackWithDistance.second);
              hSelMuonMinusDeltaPtOverPtFromGenp->Fill(dPtOverPt);
              hSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
            }
            
            if(iTrackWithDistance.second < maxDistanceForMatching && dPtOverPt < maxDPtOverPtForMatching && !trackIsNearJPsiDau)
            {
              int fakeClass = FindFakeClassification(iGenParticle);
              std::cout << "Class of fake is " << fakeClass << std::endl;
              
              
              vSelectedMuons.push_back(iMuon);
              hSelMuonDeltaRFromAssocGenp->Fill(iTrackWithDistance.second);
              hSelMuonDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt);
              hSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
              hSelMuonFakeClass->Fill(abs(fakeClass));
              if(Genp_charge->at(iGenParticle) > 0)
              {
                hSelMuonPlusDeltaRFromAssocGenp->Fill(iTrackWithDistance.second);
                hSelMuonPlusDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt);
                hSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                hSelMuonPlusFakeClass->Fill(abs(fakeClass));
              }
              else
              {
                hSelMuonMinusDeltaRFromAssocGenp->Fill(iTrackWithDistance.second);
                hSelMuonMinusDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt);
                hSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                hSelMuonMinusFakeClass->Fill(abs(fakeClass));
              }
              if(Genp_pt->at(iGenParticle) > 4 && fabs(Genp_eta->at(iGenParticle)) < 2.4)
              {
                hAssocGenParticleGenPt->Fill(Genp_pt->at(iGenParticle));
                hAssocGenParticleGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                hAssocGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
              
                if(Genp_charge->at(iGenParticle) > 0)
                {
                  hAssocGenParticlePlusGenPt->Fill(Genp_pt->at(iGenParticle));
                  hAssocGenParticlePlusGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                  hAssocGenParticlePlusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                }
                else
                {
                  hAssocGenParticleMinusGenPt->Fill(Genp_pt->at(iGenParticle));
                  hAssocGenParticleMinusGenEta->Fill(fabs(Genp_eta->at(iGenParticle)));
                  hAssocGenParticleMinusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                }
              }
              hAssocGenParticleRecoPt->Fill(Track_pt->at(iTrack));
              hAssocGenParticleRecoEta->Fill(fabs(Track_eta->at(iTrack)));
              hAssocGenParticleRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
              if(dPtOverPt < maxDPtOverPtForMatching)
              {
                if(Track_charge->at(iTrack) > 0)
                {
                  hAssocGenParticlePlusRecoPt->Fill(Track_pt->at(iTrack));
                  hAssocGenParticlePlusRecoEta->Fill(fabs(Track_eta->at(iTrack)));
                  hAssocGenParticlePlusRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
                }
                else
                {
                  hAssocGenParticleMinusRecoPt->Fill(Track_pt->at(iTrack));
                  hAssocGenParticleMinusRecoEta->Fill(fabs(Track_eta->at(iTrack)));
                  hAssocGenParticleMinusRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
                }
              }
              break;
            }
          }
        }
      }
//       else
//       {
//         std::cout << "muon " << iMuon << " not selected!\n";
//       }
    }
  }

  // Keep this line here!
  outFile->cd();
  
  // Writes into the outFile the name of the efficiency file used in this run
  WriteEfficiencyMetadata(outFile);
  
  unsigned int nPoints = 0;
  
  TGraphAsymmErrors* gFakeRateVsGenPt = CreateGraph<TGraphAsymmErrors>("gFakeRateVsGenPt", "Fake rate vs. gen p_{T}","gen p_{T} [GeV]","Fake rate", nPoints);
  TGraphAsymmErrors* gFakeRateVsGenEta = CreateGraph<TGraphAsymmErrors>("gFakeRateVsGenEta", "Fake rate vs. gen #eta","gen #eta","Fake rate", nPoints);
  TGraphAsymmErrors* gFakeRateVsGenPtPlus = CreateGraph<TGraphAsymmErrors>("gFakeRateVsGenPtPlus", "Fake rate of positive particles vs. gen p_{T}","gen p_{T} [GeV]","Fake rate", nPoints);
  TGraphAsymmErrors* gFakeRateVsGenEtaPlus = CreateGraph<TGraphAsymmErrors>("gFakeRateVsGenEtaPlus", "Fake rate of positive particles vs. gen #eta","gen #eta","Fake rate", nPoints);
  TGraphAsymmErrors* gFakeRateVsGenPtMinus = CreateGraph<TGraphAsymmErrors>("gFakeRateVsGenPtMinus", "Fake rate of negative particles vs. gen p_{T}","gen p_{T} [GeV]","Fake rate", nPoints);
  TGraphAsymmErrors* gFakeRateVsGenEtaMinus = CreateGraph<TGraphAsymmErrors>("gFakeRateVsGenEtaMinus", "Fake rate of negative particles vs. gen #eta","gen #eta","Fake rate", nPoints);
  
  TGraphAsymmErrors* gFakeRateTkVsGenPt = CreateGraph<TGraphAsymmErrors>("gFakeRateTkVsGenPt", "Tracking efficiency vs. gen p_{T}","gen p_{T} [GeV]","Tracking efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateTkVsGenEta = CreateGraph<TGraphAsymmErrors>("gFakeRateTkVsGenEta", "Tracking efficiency vs. gen #eta","gen #eta","Tracking efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateTkVsGenPtPlus = CreateGraph<TGraphAsymmErrors>("gFakeRateTkVsGenPtPlus", "Tracking efficiency of positive particles vs. gen p_{T}","gen p_{T} [GeV]","Tracking efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateTkVsGenEtaPlus = CreateGraph<TGraphAsymmErrors>("gFakeRateTkVsGenEtaPlus", "Tracking efficiency of positive particles vs. gen #eta","gen #eta","Tracking efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateTkVsGenPtMinus = CreateGraph<TGraphAsymmErrors>("gFakeRateTkVsGenPtMinus", "Tracking efficiency of negative particles vs. gen p_{T}","gen p_{T} [GeV]","Tracking efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateTkVsGenEtaMinus = CreateGraph<TGraphAsymmErrors>("gFakeRateTkVsGenEtaMinus", "Tracking efficiency of negative particles vs. gen #eta","gen #eta","Tracking efficiency", nPoints);
  
  TGraphAsymmErrors* gFakeRateSelVsRecoPt = CreateGraph<TGraphAsymmErrors>("gFakeRateSelVsRecoPt", "Selection efficiency vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateSelVsRecoEta = CreateGraph<TGraphAsymmErrors>("gFakeRateSelVsRecoEta", "Selection efficiency vs. reco #eta","reco #eta","Selection efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateSelVsRecoPtPlus = CreateGraph<TGraphAsymmErrors>("gFakeRateSelVsRecoPtPlus", "Selection efficiency of positive particles vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateSelVsRecoEtaPlus = CreateGraph<TGraphAsymmErrors>("gFakeRateSelVsRecoEtaPlus", "Selection efficiency of positive particles vs. reco #eta","reco #eta","Selection efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateSelVsRecoPtMinus = CreateGraph<TGraphAsymmErrors>("gFakeRateSelVsRecoPtMinus", "Selection efficiency of negative particles vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency", nPoints);
  TGraphAsymmErrors* gFakeRateSelVsRecoEtaMinus = CreateGraph<TGraphAsymmErrors>("gFakeRateSelVsRecoEtaMinus", "Selection efficiency of negative particles vs. reco #eta","reco #eta","Selection efficiency", nPoints);
  
  BayesDivide(hAssocGenParticleGenPt, hAllGenParticleGenPt, gFakeRateVsGenPt);
  BayesDivide(hAssocGenParticleGenEta, hAllGenParticleGenEta, gFakeRateVsGenEta);
  BayesDivide(hAssocGenParticlePlusGenPt, hAllGenParticlePlusGenPt, gFakeRateVsGenPtPlus);
  BayesDivide(hAssocGenParticlePlusGenEta, hAllGenParticlePlusGenEta, gFakeRateVsGenEtaPlus);
  BayesDivide(hAssocGenParticleMinusGenPt, hAllGenParticleMinusGenPt, gFakeRateVsGenPtMinus);
  BayesDivide(hAssocGenParticleMinusGenEta, hAllGenParticleMinusGenEta, gFakeRateVsGenEtaMinus);
  BayesDivide(hAssocTkGenParticleGenPt, hAllGenParticleGenPt, gFakeRateTkVsGenPt);
  BayesDivide(hAssocTkGenParticleGenEta, hAllGenParticleGenEta, gFakeRateTkVsGenEta);
  BayesDivide(hAssocTkGenParticlePlusGenPt, hAllGenParticlePlusGenPt, gFakeRateTkVsGenPtPlus);
  BayesDivide(hAssocTkGenParticlePlusGenEta, hAllGenParticlePlusGenEta, gFakeRateTkVsGenEtaPlus);
  BayesDivide(hAssocTkGenParticleMinusGenPt, hAllGenParticleMinusGenPt, gFakeRateTkVsGenPtMinus);
  BayesDivide(hAssocTkGenParticleMinusGenEta, hAllGenParticleMinusGenEta, gFakeRateTkVsGenEtaMinus);
  BayesDivide(hAssocGenParticleRecoPt,hAssocTkGenParticleRecoPt, gFakeRateSelVsRecoPt);
  BayesDivide(hAssocGenParticleRecoEta,hAssocTkGenParticleRecoEta, gFakeRateSelVsRecoEta);
  BayesDivide(hAssocGenParticlePlusRecoPt,hAssocTkGenParticlePlusRecoPt, gFakeRateSelVsRecoPtPlus);
  BayesDivide(hAssocGenParticlePlusRecoEta,hAssocTkGenParticlePlusRecoEta, gFakeRateSelVsRecoEtaPlus);
  BayesDivide(hAssocGenParticleMinusRecoPt,hAssocTkGenParticleMinusRecoPt, gFakeRateSelVsRecoPtMinus);
  BayesDivide(hAssocGenParticleMinusRecoEta,hAssocTkGenParticleMinusRecoEta, gFakeRateSelVsRecoEtaMinus);
    
  gFakeRateVsGenPt->Write();
  gFakeRateVsGenEta->Write();
  gFakeRateVsGenPtPlus->Write();
  gFakeRateVsGenEtaPlus->Write();
  gFakeRateVsGenPtMinus->Write();
  gFakeRateVsGenEtaMinus->Write();
  gFakeRateTkVsGenPt->Write();
  gFakeRateTkVsGenEta->Write();
  gFakeRateTkVsGenPtPlus->Write();
  gFakeRateTkVsGenEtaPlus->Write();
  gFakeRateTkVsGenPtMinus->Write();
  gFakeRateTkVsGenEtaMinus->Write();
  gFakeRateSelVsRecoPt->Write();
  gFakeRateSelVsRecoEta->Write();
  gFakeRateSelVsRecoPtPlus->Write();
  gFakeRateSelVsRecoEtaPlus->Write();
  gFakeRateSelVsRecoPtMinus->Write();
  gFakeRateSelVsRecoEtaMinus->Write();
    
  // Create and write canvases here
  CreateAndWriteCanvas("cSelMuonDeltaRFromGenp",0, 21, 1, false, false, hSelMuonDeltaRFromGenp);
  CreateAndWriteCanvas("cSelMuonDeltaPtOverPtFromGenp",0, 21, 1, false, false, hSelMuonDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hSelMuonDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonFakeClass", 0, 21, 1, false, true, hSelMuonFakeClass);
  CreateAndWriteCanvas("cTrackNearJPsiDauDeltaRFromGenp",0, 21, 1, false, true, hTrackNearJPsiDauDeltaRFromGenp);
  CreateAndWriteCanvas("cTrackNearJPsiDauDeltaPtOverPtFromGenp",0, 21, 1, false, true, hTrackNearJPsiDauDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackNearJPsiDauDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackNearJPsiDauDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("chTrackDeltaRFromJPsiDau",0, 21, 1, false, true, hTrackDeltaRFromJPsiDau);
  CreateAndWriteCanvas("cTrackDeltaPtOverPtFromJPsiDau",0, 21, 1, false, true, hTrackDeltaPtOverPtFromJPsiDau);
  CreateAndWriteCanvas("cTrackDeltaRFromGenp",0, 21, 1, false, false, hTrackDeltaRFromGenp);
  CreateAndWriteCanvas("cTrackDeltaPtOverPtFromGenp",0, 21, 1, false, false, hTrackDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackChi2NDOFVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackChi2NDOFVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackNDOFVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackNDOFVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonDeltaRFromAssocGenp",0, 21, 1, false, false, hSelMuonDeltaRFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonDeltaPtOverPtFromAssocGenp",0, 21, 1, false, false, hSelMuonDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cAssocTrackDeltaRFromJPsiDau",0, 21, 1, false, true, hAssocTrackDeltaRFromJPsiDau);
  CreateAndWriteCanvas("cAssocTrackDeltaPtOverPtFromJPsiDau",0, 21, 1, false, true, hAssocTrackDeltaPtOverPtFromJPsiDau);
  CreateAndWriteCanvas("cTrackDeltaRFromAssocGenp",0, 21, 1, false, false, hTrackDeltaRFromAssocGenp);
  CreateAndWriteCanvas("cTrackDeltaPtOverPtFromAssocGenp",0, 21, 1, false, false, hTrackDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hTrackDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackNDOFVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hTrackNDOFVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cAllGenParticleGenPt",0, 21, 1, false, false, hAllGenParticleGenPt);
  CreateAndWriteCanvas("cAssocGenParticleGenPt",0, 21, 1, false, false, hAssocGenParticleGenPt);
  CreateAndWriteCanvas("cAssocGenParticleRecoPt",0, 21, 1, false, false, hAssocGenParticleRecoPt);
  CreateAndWriteCanvas("cAssocTkGenParticleGenPt",0, 21, 1, false, false, hAssocTkGenParticleGenPt);
  CreateAndWriteCanvas("cAssocTkGenParticleRecoPt",0, 21, 1, false, false, hAssocTkGenParticleRecoPt);
  CreateAndWriteCanvas("cAllGenParticleGenEta",0, 21, 1, false, false, hAllGenParticleGenEta);
  CreateAndWriteCanvas("cAssocGenParticleGenEta",0, 21, 1, false, false, hAssocGenParticleGenEta);
  CreateAndWriteCanvas("cAssocGenParticleRecoEta",0, 21, 1, false, false, hAssocGenParticleRecoEta);
  CreateAndWriteCanvas("cAssocTkGenParticleGenEta",0, 21, 1, false, false, hAssocTkGenParticleGenEta);
  CreateAndWriteCanvas("cAssocTkGenParticleRecoEta",0, 21, 1, false, false, hAssocTkGenParticleRecoEta);
  CreateAndWriteCanvas("cAllGenParticleGenPtVsEta", "colz", false, false, true, hAllGenParticleGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticleGenPtVsEta", "colz", false, false, true, hAssocGenParticleGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticleRecoPtVsEta", "colz", false, false, true, hAssocGenParticleRecoPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticleGenPtVsEta", "colz", false, false, true, hAssocTkGenParticleGenPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticleRecoPtVsEta", "colz", false, false, true, hAssocTkGenParticleRecoPtVsEta);  
  CreateAndWriteCanvas("cFakeRateVsGenPt",21, 1, false, false, gFakeRateVsGenPt);
  CreateAndWriteCanvas("cFakeRateVsGenEta",21, 1, false, false, gFakeRateVsGenEta);
  CreateAndWriteCanvas("cFakeRateTkVsGenPt",21, 1, false, false, gFakeRateTkVsGenPt);
  CreateAndWriteCanvas("cFakeRateTkVsGenEta",21, 1, false, false, gFakeRateTkVsGenEta);
  CreateAndWriteCanvas("cFakeRateSelVsRecoPt",21, 1, false, false, gFakeRateSelVsRecoPt);
  CreateAndWriteCanvas("cFakeRateSelVsRecoEta",21, 1, false, false, gFakeRateSelVsRecoEta);
  
  CreateAndWriteCanvas("cSelMuonPlusDeltaRFromGenp",0, 21, 1, false, false, hSelMuonPlusDeltaRFromGenp);
  CreateAndWriteCanvas("cSelMuonPlusDeltaPtOverPtFromGenp",0, 21, 1, false, false, hSelMuonPlusDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonPlusFakeClass", 0, 21, 1, false, true, hSelMuonPlusFakeClass);
  CreateAndWriteCanvas("cTrackPlusDeltaRFromGenp",0, 21, 1, false, false, hTrackPlusDeltaRFromGenp);
  CreateAndWriteCanvas("cTrackPlusDeltaPtOverPtFromGenp",0, 21, 1, false, false, hTrackPlusDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackPlusDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackPlusDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonPlusDeltaRFromAssocGenp",0, 21, 1, false, false, hSelMuonPlusDeltaRFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonPlusDeltaPtOverPtFromAssocGenp",0, 21, 1, false, false, hSelMuonPlusDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackPlusDeltaRFromAssocGenp",0, 21, 1, false, false, hTrackPlusDeltaRFromAssocGenp);
  CreateAndWriteCanvas("cTrackPlusDeltaPtOverPtFromAssocGenp",0, 21, 1, false, false, hTrackPlusDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cAllGenParticlePlusGenPt",0, 21, 1, false, false, hAllGenParticlePlusGenPt);
  CreateAndWriteCanvas("cAssocGenParticlePlusGenPt",0, 21, 1, false, false, hAssocGenParticlePlusGenPt);
  CreateAndWriteCanvas("cAssocGenParticlePlusRecoPt",0, 21, 1, false, false, hAssocGenParticlePlusRecoPt);
  CreateAndWriteCanvas("cAssocTkGenParticlePlusGenPt",0, 21, 1, false, false, hAssocTkGenParticlePlusGenPt);
  CreateAndWriteCanvas("cAssocTkGenParticlePlusRecoPt",0, 21, 1, false, false, hAssocTkGenParticlePlusRecoPt);
  CreateAndWriteCanvas("cAllGenParticlePlusGenEta",0, 21, 1, false, false, hAllGenParticlePlusGenEta);
  CreateAndWriteCanvas("cAssocGenParticlePlusGenEta",0, 21, 1, false, false, hAssocGenParticlePlusGenEta);
  CreateAndWriteCanvas("cAssocGenParticlePlusRecoEta",0, 21, 1, false, false, hAssocGenParticlePlusRecoEta);
  CreateAndWriteCanvas("cAssocTkGenParticlePlusGenEta",0, 21, 1, false, false, hAssocTkGenParticlePlusGenEta);
  CreateAndWriteCanvas("cAssocTkGenParticlePlusRecoEta",0, 21, 1, false, false, hAssocTkGenParticlePlusRecoEta);
  CreateAndWriteCanvas("cAllGenParticlePlusGenPtVsEta", "colz", false, false, true, hAllGenParticlePlusGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticlePlusGenPtVsEta", "colz", false, false, true, hAssocGenParticlePlusGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticlePlusRecoPtVsEta", "colz", false, false, true, hAssocGenParticlePlusRecoPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticlePlusGenPtVsEta", "colz", false, false, true, hAssocTkGenParticlePlusGenPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticlePlusRecoPtVsEta", "colz", false, false, true, hAssocTkGenParticlePlusRecoPtVsEta);
  CreateAndWriteCanvas("cFakeRateVsGenPtPlus",21, 1, false, false, gFakeRateVsGenPtPlus);
  CreateAndWriteCanvas("cFakeRateVsGenEtaPlus",21, 1, false, false, gFakeRateVsGenEtaPlus);
  CreateAndWriteCanvas("cFakeRateTkVsGenPtPlus",21, 1, false, false, gFakeRateTkVsGenPtPlus);
  CreateAndWriteCanvas("cFakeRateTkVsGenEtaPlus",21, 1, false, false, gFakeRateTkVsGenEtaPlus);
  CreateAndWriteCanvas("cFakeRateSelVsRecoPtPlus",21, 1, false, false, gFakeRateSelVsRecoPtPlus);
  CreateAndWriteCanvas("cFakeRateSelVsRecoEtaPlus",21, 1, false, false, gFakeRateSelVsRecoEtaPlus);
  
  CreateAndWriteCanvas("cSelMuonMinusDeltaRFromGenp",0, 21, 1, false, false, hSelMuonMinusDeltaRFromGenp);
  CreateAndWriteCanvas("cSelMuonMinusDeltaPtOverPtFromGenp",0, 21, 1, false, false, hSelMuonMinusDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonMinusFakeClass", 0, 21, 1, false, true, hSelMuonMinusFakeClass);
  CreateAndWriteCanvas("cTrackMinusDeltaRFromGenp",0, 21, 1, false, false, hTrackMinusDeltaRFromGenp);
  CreateAndWriteCanvas("cTrackMinusDeltaPtOverPtFromGenp",0, 21, 1, false, false, hTrackMinusDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackMinusDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackMinusDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonMinusDeltaRFromAssocGenp",0, 21, 1, false, false, hSelMuonMinusDeltaRFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonMinusDeltaPtOverPtFromAssocGenp",0, 21, 1, false, false, hSelMuonMinusDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackMinusDeltaRFromAssocGenp",0, 21, 1, false, false, hTrackMinusDeltaRFromAssocGenp);
  CreateAndWriteCanvas("cTrackMinusDeltaPtOverPtFromAssocGenp",0, 21, 1, false, false, hTrackMinusDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cAllGenParticleMinusGenPt",0, 21, 1, false, false, hAllGenParticleMinusGenPt);
  CreateAndWriteCanvas("cAssocGenParticleMinusGenPt",0, 21, 1, false, false, hAssocGenParticleMinusGenPt);
  CreateAndWriteCanvas("cAssocGenParticleMinusRecoPt",0, 21, 1, false, false, hAssocGenParticleMinusRecoPt);
  CreateAndWriteCanvas("cAssocTkGenParticleMinusGenPt",0, 21, 1, false, false, hAssocTkGenParticleMinusGenPt);
  CreateAndWriteCanvas("cAssocTkGenParticleMinusRecoPt",0, 21, 1, false, false, hAssocTkGenParticleMinusRecoPt);
  CreateAndWriteCanvas("cAllGenParticleMinusGenEta",0, 21, 1, false, false, hAllGenParticleMinusGenEta);
  CreateAndWriteCanvas("cAssocGenParticleMinusGenEta",0, 21, 1, false, false, hAssocGenParticleMinusGenEta);
  CreateAndWriteCanvas("cAssocGenParticleMinusRecoEta",0, 21, 1, false, false, hAssocGenParticleMinusRecoEta);
  CreateAndWriteCanvas("cAssocTkGenParticleMinusGenEta",0, 21, 1, false, false, hAssocTkGenParticleMinusGenEta);
  CreateAndWriteCanvas("cAssocTkGenParticleMinusRecoEta",0, 21, 1, false, false, hAssocTkGenParticleMinusRecoEta);
  CreateAndWriteCanvas("cAllGenParticleMinusGenPtVsEta", "colz", false, false, true, hAllGenParticleMinusGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticleMinusGenPtVsEta", "colz", false, false, true, hAssocGenParticleMinusGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticleMinusRecoPtVsEta", "colz", false, false, true, hAssocGenParticleMinusRecoPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticleMinusGenPtVsEta", "colz", false, false, true, hAssocTkGenParticleMinusGenPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticleMinusRecoPtVsEta", "colz", false, false, true, hAssocTkGenParticleMinusRecoPtVsEta);  
  CreateAndWriteCanvas("cFakeRateVsGenPtMinus",21, 1, false, false, gFakeRateVsGenPtMinus);
  CreateAndWriteCanvas("cFakeRateVsGenEtaMinus",21, 1, false, false, gFakeRateVsGenEtaMinus);
  CreateAndWriteCanvas("cFakeRateTkVsGenPtMinus",21, 1, false, false, gFakeRateTkVsGenPtMinus);
  CreateAndWriteCanvas("cFakeRateTkVsGenEtaMinus",21, 1, false, false, gFakeRateTkVsGenEtaMinus);
  CreateAndWriteCanvas("cFakeRateSelVsRecoPtMinus",21, 1, false, false, gFakeRateSelVsRecoPtMinus);
  CreateAndWriteCanvas("cgFakeRateSelVsRecoEtaMinus",21, 1, false, false, gFakeRateSelVsRecoEtaMinus);
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}



std::string MuonBasicDistributionsDataForBmm2::GetBinningVariableString(const std::string sBinning)
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



std::vector<double> MuonBasicDistributionsDataForBmm2::GetBins(const std::string sBinning)
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



std::vector<double> MuonBasicDistributionsDataForBmm2::ParseBins(const std::string sBins)
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



std::vector<double> MuonBasicDistributionsDataForBmm2::GetBinningVariablesValues(const unsigned int iMuon)
{
  std::vector<double> result;
  result.push_back(1e100);
  result.push_back(Muon_pt->at(iMuon));
  result.push_back(fabs(Muon_eta->at(iMuon)));
  result.push_back(Muon_charge->at(iMuon));
  return result;
}



std::vector<double> MuonBasicDistributionsDataForBmm2::GetGenBinningVariablesValues(const unsigned int iGenParticle)
{
  std::vector<double> result;
  result.push_back(1e100);
  result.push_back(Genp_pt->at(iGenParticle));
  result.push_back(fabs(Genp_eta->at(iGenParticle)));
  result.push_back(Genp_charge->at(iGenParticle));
  return result;
}



#endif // MuonBasicDistributionsDataForBmm2_cxx
