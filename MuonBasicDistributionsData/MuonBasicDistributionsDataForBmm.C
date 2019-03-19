#ifndef MuonBasicDistributionsDataForBmm_cxx
#define MuonBasicDistributionsDataForBmm_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include <math.h>

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
    std::cout << " MC matching of reco muons is disabled!\n";
  }

  std::map<std::string,muon::SelectionType> mSelectionType = s->GetMuonSelectionTypeMap();
  
  std::cout << "Analyzing " << nentries << " events.\n";
  
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
  const double nPhiBins = 60;
  const double phiMin = -constants::PI;
  const double phiMax = constants::PI;
//   const char* genPhiLabel = "gen #phi";
  const char* recoPhiLabel = "reco #phi";
  
  std::string sBinning = "";
  if(s->GetJetSelector() != "" && !s->JetSelectorIsValid())
  {
    sBinning = s->GetJetSelector();
  }
  
  std::string sBinningVariable = GetBinningVariableString(sBinning);
  std::vector<double> vLowBins = GetBins(sBinning);
  
  if(vLowBins.size() != 0)
  {
    std::cout << "Binning on " << sBinningVariable << std::endl;
  }
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
  std::vector<TH1D*>* hSelMuonDeltaPtOverPtFromGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}| of gen particle from closest matching selected muon", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hSelMuonDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hSelMuonDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching selected muon",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  std::vector<TH1D*>* hSelMuonPlusDeltaRFromGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonPlusDeltaRFromGenp","#Delta(R) of positive gen particle from closest matching selected muon", 100, 0., 1., "#Delta(R)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPlusDeltaPtOverPtFromGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonPlusDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}-1| of positive gen particle from closest matching selected muon", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of positive gen particle from closest matching selected muon",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  std::vector<TH1D*>* hSelMuonMinusDeltaRFromGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinusDeltaRFromGenp","#Delta(R) of negative gen particle from closest matching selected muon", 100, 0., 1., "#Delta(R)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinusDeltaPtOverPtFromGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinusDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}-1| of negative gen particle from closest matching selected muon", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of negative gen particle from closest matching selected muon",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  
  std::vector<TH1D*>* hTrackDeltaRFromGenp = CreateVectorOf1DHistograms<TH1D>("hTrackDeltaRFromGenp","#Delta(R) of gen particle from closest matching track", 100, 0., 1., "#Delta(R)", "N_{tracks}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hTrackDeltaPtOverPtFromGenp = CreateVectorOf1DHistograms<TH1D>("hTrackDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}-1| of gen particle from closest matching track", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}",sBinningVariable,vLowBins);
  TH2D* hTrackDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching track",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  std::vector<TH1D*>* hTrackPlusDeltaRFromGenp = CreateVectorOf1DHistograms<TH1D>("hTrackPlusDeltaRFromGenp","#Delta(R) of positive gen particle from closest matching track", 100, 0., 1., "#Delta(R)", "N_{tracks}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hTrackPlusDeltaPtOverPtFromGenp = CreateVectorOf1DHistograms<TH1D>("hTrackPlusDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}| of positive gen particle from closest matching track", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}",sBinningVariable,vLowBins);
  TH2D* hTrackPlusDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackPlusDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of positive gen particle from closest matching track",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  std::vector<TH1D*>* hTrackMinusDeltaRFromGenp = CreateVectorOf1DHistograms<TH1D>("hTrackMinusDeltaRFromGenp","#Delta(R) of negative gen particle from closest matching track", 100, 0., 1., "#Delta(R)", "N_{tracks}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hTrackMinusDeltaPtOverPtFromGenp = CreateVectorOf1DHistograms<TH1D>("hTrackMinusDeltaPtOverPtFromGenp","|#Delta(p_{T})/p_{T}-1| of negative gen particle from closest matching track", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}",sBinningVariable,vLowBins);
  TH2D* hTrackMinusDeltaRVsDeltaPtOverPtFromGenp = Create2DHistogram<TH2D>("hTrackMinusDeltaRVsDeltaPtOverPtFromGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of negative gen particle from closest matching track",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  
  std::vector<TH1D*>* hSelMuonDeltaRFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonDeltaRFromAssocGenp","#Delta(R) of gen particle from closest matching selected muon for successful associations", 100, 0., 1., "#Delta(R)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDeltaPtOverPtFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}| of gen particle from closest matching selected muon for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching selected muon for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  std::vector<TH1D*>* hSelMuonPlusDeltaRFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonPlusDeltaRFromAssocGenp","#Delta(R) of positive gen particle from closest matching selected muon for successful associations", 100, 0., 1., "#Delta(R)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPlusDeltaPtOverPtFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonPlusDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of positive gen particle from closest matching selected muon for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of positive gen particle from closest matching selected muon for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  std::vector<TH1D*>* hSelMuonMinusDeltaRFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinusDeltaRFromAssocGenp","#Delta(R) of negative gen particle from closest matching selected muon for successful associations", 100, 0., 1., "#Delta(R)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinusDeltaPtOverPtFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinusDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of negative gen particle from closest matching selected muon for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of negative gen particle from closest matching selected muon for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  
  std::vector<TH1D*>* hTrackDeltaRFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hTrackDeltaRFromAssocGenp","#Delta(R) of gen particle from closest matching track for successful associations", 100, 0., 1., "#Delta(R)", "N_{tracks}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hTrackDeltaPtOverPtFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hTrackDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of gen particle from closest matching track for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}",sBinningVariable,vLowBins);
  TH2D* hTrackDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hTrackDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of gen particle from closest matching track for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  std::vector<TH1D*>* hTrackPlusDeltaRFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hTrackPlusDeltaRFromAssocGenp","#Delta(R) of positive gen particle from closest matching track for successful associations", 100, 0., 1., "#Delta(R)", "N_{tracks}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hTrackPlusDeltaPtOverPtFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hTrackPlusDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of positive gen particle from closest matching track for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}",sBinningVariable,vLowBins);
  TH2D* hTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of positive gen particle from closest matching track for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  std::vector<TH1D*>* hTrackMinusDeltaRFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hTrackMinusDeltaRFromAssocGenp","#Delta(R) of negative gen particle from closest matching track for successful associations", 100, 0., 1., "#Delta(R)", "N_{tracks}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hTrackMinusDeltaPtOverPtFromAssocGenp = CreateVectorOf1DHistograms<TH1D>("hTrackMinusDeltaPtOverPtFromAssocGenp","|#Delta(p_{T})/p_{T}-1| of negative gen particle from closest matching track for successful associations", 100, 0., 1., "|#Delta(p_{T})/p_{T}|", "N_{tracks}",sBinningVariable,vLowBins);
  TH2D* hTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp = Create2DHistogram<TH2D>("hTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp","#Delta(R) vs. |#Delta(p_{T})/p_{T}| of negative gen particle from closest matching track for successful associations",100,0.,1.,100,0.,1.,"|#Delta(p_{T})/p_{T}|","#Delta(R)");
  
  std::vector<TH1D*>* hRecoMuonPt = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPt","p_{T} of reconstructed muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPt = CreateVectorOf1DHistograms<TH1D>("hSelMuonPt","p_{T} of selected muons", nPtBins, ptMin, ptMax, recoPtLabel, nMuLabel,sBinningVariable,vLowBins);
  TH2D* hSelMuonPtVsEta = Create2DHistogram<TH2D>("hSelMuonPtVsEta", "p_{T} vs. #eta of selected muons", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  std::vector<TH1D*>* hAllGenParticleGenPt = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleGenPt","p_{T} of all gen particles coming from B decays", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleGenPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleGenPt","p_{T} of gen particles coming from B decays associated to a muon", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleRecoPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleRecoPt","reco p_{T} of gen particles coming from B decays associated to a muon", nPtBins, ptMin, ptMax, recoPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleGenPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleGenPt","p_{T} of gen particles coming from B decays associated to a track", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleRecoPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleRecoPt","reco p_{T} of gen particles coming from B decays associated to a track", nPtBins, ptMin, ptMax, recoPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAllGenParticleGenEta = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleGenEta","#eta of all gen particles coming from B decays", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleGenEta = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleGenEta","#eta of gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleRecoEta = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleRecoEta","reco #eta of gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, recoEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleGenEta = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleGenEta","#eta of gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleRecoEta = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleRecoEta","reco #eta of gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, recoEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  TH2D* hAllGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAllGenParticleGenPtVsEta","p_{T} vs. #eta of all gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleGenPtVsEta","p_{T} vs. #eta of gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticleRecoPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleRecoPtVsEta","reco p_{T} vs. #eta of gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAssocTkGenParticleGenPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleGenPtVsEta","p_{T} vs. #eta of gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocTkGenParticleRecoPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleRecoPtVsEta","reco p_{T} vs. #eta of gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  std::vector<TH1D*>* hAllGenParticleNDaughters = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleNDaughters","Number of daughters of all gen particles coming from B decays", 10, -0.5, 9.5, "n_{daughters}","n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleNDaughters = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleNDaughters","Number of daughters of gen particles coming from B decays associated to a muon", 10, -0.5, 9.5, "n_{daughters}","n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAllGenParticleDecayLength = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleDecayLength", "Decay length of all gen particles coming from B decays", 150, 0., 1500., "L [cm]", "n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleDecayLength = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleDecayLength", "Decay length of gen particles coming from B decays associated to a muon", 150, 0., 1500., "L [cm]", "n_{GenP}",sBinningVariable,vLowBins);
  TH2D* hAllGenParticleDecayVertexPosition = Create2DHistogram<TH2D>("hAllGenParticleDecayVertexPosition", "Position of decay vertex of all gen particles coming from B decays", 1500, 0., 1500., 1500, 0., 1500.,  "z [cm]", "r [cm]");
  TH2D* hAssocGenParticleDecayVertexPosition = Create2DHistogram<TH2D>("hAssocGenParticleDecayVertexPosition", "Position of decay vertex of gen particles coming from B decays associated to a muon", 1500, 0., 1500., 1500, 0., 1500.,  "z [cm]", "r [cm]");

  std::vector<TH1D*>* hAllGenParticlePlusGenPt = CreateVectorOf1DHistograms<TH1D>("hAllGenParticlePlusGenPt","p_{T} of all positive gen particles coming from B decays", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticlePlusGenPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticlePlusGenPt","p_{T} of positive gen particles coming from B decays associated to a muon", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticlePlusRecoPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticlePlusRecoPt","reco p_{T} of positive gen particles coming from B decays associated to a muon", nPtBins, ptMin, ptMax, recoPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticlePlusGenPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticlePlusGenPt","p_{T} of positive gen particles coming from B decays associated to a track", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticlePlusRecoPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticlePlusRecoPt","reco p_{T} of positive gen particles coming from B decays associated to a track", nPtBins, ptMin, ptMax, recoPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAllGenParticlePlusGenEta = CreateVectorOf1DHistograms<TH1D>("hAllGenParticlePlusGenEta","#eta of all positive gen particles coming from B decays", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticlePlusGenEta = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticlePlusGenEta","#eta of positive gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticlePlusRecoEta = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticlePlusRecoEta","reco #eta of positive gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, recoEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticlePlusGenEta = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticlePlusGenEta","#eta of positive gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticlePlusRecoEta = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticlePlusRecoEta","reco #eta positive of gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, recoEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  TH2D* hAllGenParticlePlusGenPtVsEta = Create2DHistogram<TH2D>("hAllGenParticlePlusGenPtVsEta","p_{T} vs. #eta of all positive gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticlePlusGenPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticlePlusGenPtVsEta","p_{T} vs. #eta of positive gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticlePlusRecoPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticlePlusRecoPtVsEta","reco p_{T} vs. #eta of positive gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAssocTkGenParticlePlusGenPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticlePlusGenPtVsEta","p_{T} vs. #eta of positive gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocTkGenParticlePlusRecoPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticlePlusRecoPtVsEta","reco p_{T} vs. #eta of positive gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  std::vector<TH1D*>* hAllGenParticlePlusNDaughters = CreateVectorOf1DHistograms<TH1D>("hAllGenParticlePlusNDaughters","Number of daughters of all positive gen particles coming from B decays", 10, -0.5, 9.5, "n_{daughters}","n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticlePlusNDaughters = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticlePlusNDaughters","Number of daughters of positive gen particles coming from B decays associated to a muon", 10, -0.5, 9.5, "n_{daughters}","n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAllGenParticlePlusDecayLength = CreateVectorOf1DHistograms<TH1D>("hAllGenParticlePlusDecayLength", "Decay length of all positive gen particles coming from B decays", 150, 0., 1500., "L [cm]", "n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticlePlusDecayLength = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticlePlusDecayLength", "Decay length of positive gen particles coming from B decays associated to a muon", 150, 0., 1500., "L [cm]", "n_{GenP}",sBinningVariable,vLowBins);
  TH2D* hAllGenParticlePlusDecayVertexPosition = Create2DHistogram<TH2D>("hAllGenParticlePlusDecayVertexPosition", "Position of decay vertex of all positive gen particles coming from B decays", 1500, 0., 1500., 1500, 0., 1500.,  "z [cm]", "r [cm]");
  TH2D* hAssocGenParticlePlusDecayVertexPosition = Create2DHistogram<TH2D>("hAssocGenParticlePlusDecayVertexPosition", "Position of decay vertex of positive gen particles coming from B decays associated to a muon", 1500, 0., 1500., 1500, 0., 1500.,  "z [cm]", "r [cm]");
  
  std::vector<TH1D*>* hAllGenParticleMinusGenPt = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleMinusGenPt","p_{T} of all negative gen particles coming from B decays", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleMinusGenPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleMinusGenPt","p_{T} of negative gen particles coming from B decays associated to a muon", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleMinusRecoPt = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleMinusRecoPt","reco p_{T} of negative gen particles coming from B decays associated to a muon", nPtBins, ptMin, ptMax, recoPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleMinusGenPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleMinusGenPt","p_{T} of negative gen particles coming from B decays associated to a track", nPtBins, ptMin, ptMax, genPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleMinusRecoPt = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleMinusRecoPt","reco p_{T} of negative gen particles coming from B decays associated to a track", nPtBins, ptMin, ptMax, recoPtLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAllGenParticleMinusGenEta = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleMinusGenEta","#eta of all negative gen particles coming from B decays", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleMinusGenEta = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleMinusGenEta","#eta of negative gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleMinusRecoEta = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleMinusRecoEta","reco #eta of negative gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, recoEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleMinusGenEta = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleMinusGenEta","#eta of negative gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, genEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocTkGenParticleMinusRecoEta = CreateVectorOf1DHistograms<TH1D>("hAssocTkGenParticleMinusRecoEta","reco #eta of negative gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, recoEtaLabel,"n_{GenP}",sBinningVariable,vLowBins);
  TH2D* hAllGenParticleMinusGenPtVsEta = Create2DHistogram<TH2D>("hAllGenParticleMinusGenPtVsEta","p_{T} vs. #eta of all negative gen particles coming from B decays", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticleMinusGenPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleMinusGenPtVsEta","p_{T} vs. #eta of negative gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocGenParticleMinusRecoPtVsEta = Create2DHistogram<TH2D>("hAssocGenParticleMinusRecoPtVsEta","reco p_{T} vs. #eta of negative gen particles coming from B decays associated to a muon", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  TH2D* hAssocTkGenParticleMinusGenPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleMinusGenPtVsEta","p_{T} vs. #eta of negative gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, genEtaLabel, genPtLabel);
  TH2D* hAssocTkGenParticleMinusRecoPtVsEta = Create2DHistogram<TH2D>("hAssocTkGenParticleMinusRecoPtVsEta","reco p_{T} vs. #eta of negative gen particles coming from B decays associated to a track", nEtaBins, etaMin, etaMax, nPtBins, ptMin, ptMax, recoEtaLabel, recoPtLabel);
  std::vector<TH1D*>* hAllGenParticleMinusNDaughters = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleMinusNDaughters","Number of daughters of all negative gen particles coming from B decays", 10, -0.5, 9.5, "n_{daughters}","n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleMinusNDaughters = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleMinusNDaughters","Number of daughters of negative gen particles coming from B decays associated to a muon", 10, -0.5, 9.5, "n_{daughters}","n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAllGenParticleMinusDecayLength = CreateVectorOf1DHistograms<TH1D>("hAllGenParticleMinusDecayLength", "Decay length of all negative gen particles coming from B decays", 150, 0., 1500., "L [cm]", "n_{GenP}",sBinningVariable,vLowBins);
  std::vector<TH1D*>* hAssocGenParticleMinusDecayLength = CreateVectorOf1DHistograms<TH1D>("hAssocGenParticleMinusDecayLength", "Decay length of negative gen particles coming from B decays associated to a muon", 150, 0., 1500., "L [cm]", "n_{GenP}",sBinningVariable,vLowBins);
  TH2D* hAllGenParticleMinusDecayVertexPosition = Create2DHistogram<TH2D>("hAllGenParticleMinusDecayVertexPosition", "Position of decay vertex of all negative gen particles coming from B decays", 1500, 0., 1500., 1500, 0., 1500.,  "z [cm]", "r [cm]");
  TH2D* hAssocGenParticleMinusDecayVertexPosition = Create2DHistogram<TH2D>("hAssocGenParticleMinusDecayVertexPosition", "Position of decay vertex of negative gen particles coming from B decays associated to a muon", 1500, 0., 1500., 1500, 0., 1500.,  "z [cm]", "r [cm]");
  
  
  
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
  
  std::vector<TH1D*>* hRecoMuonTrackValidFraction = CreateVectorOf1DHistograms<TH1D>("hRecoMuonTrackValidFraction","Fraction of valid hits in inner track of reconstructed muons", 100, 0., 1., "N_{valid}/N_{hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonTrackValidFraction = CreateVectorOf1DHistograms<TH1D>("hSelMuonTrackValidFraction","Fraction of valid hits in inner track of selected muons", 100, 0., 1., "N_{valid}/N_{hits}", nMuLabel,sBinningVariable,vLowBins);

  std::vector<TH1D*>* hRecoMuonTrackExpOuterHits = CreateVectorOf1DHistograms<TH1D>("hRecoMuonTrackExpOuterHits","N. of expected valid outer hits in inner track of reconstructed muons", 11, -0.5, 10.5, "N_{exp hits}", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonTrackExpOuterHits = CreateVectorOf1DHistograms<TH1D>("hSelMuonTrackExpOuterHits","N. of expected valid outer hits in inner track of selected muons", 11, -0.5, 10.5, "N_{exp hits}", nMuLabel,sBinningVariable,vLowBins);
  
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
  
  std::vector<TH1D*>* hRecoMuonDDxDzInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDzInDTStation1","#Delta(dx/dz) in DT station 1 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDzInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDzInDTStation1","#Delta(dx/dz) in DT station 1 of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDxDzInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDzInDTStation2","#Delta(dx/dz) in DT station 2 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDzInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDzInDTStation2","#Delta(dx/dz) in DT station 2 of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDxDzInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDzInDTStation3","#Delta(dx/dz) in DT station 3 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDzInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDzInDTStation3","#Delta(dx/dz) in DT station 3 of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDxDzInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDzInDTStation4","#Delta(dx/dz) in DT station 4 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDzInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDzInDTStation4","#Delta(dx/dz) in DT station 4 of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDxDzInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDzInCSCStation1","#Delta(dx/dz) in CSC station 1 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDzInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDzInCSCStation1","#Delta(dx/dz) in CSC station 1 of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDxDzInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDzInCSCStation2","#Delta(dx/dz) in CSC station 2 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDzInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDzInCSCStation2","#Delta(dx/dz) in CSC station 2 of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDxDzInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDzInCSCStation3","#Delta(dx/dz) in CSC station 3 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDzInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDzInCSCStation3","#Delta(dx/dz) in CSC station 3 of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDxDzInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDzInCSCStation4","#Delta(dx/dz) in CSC station 4 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDzInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDzInCSCStation4","#Delta(dx/dz) in CSC station 4 of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDxDz","#Delta(dx/dz) of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDxDz","#Delta(dx/dz) of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinDDxDz","Minimum #Delta(dx/dz) of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinDDxDz","Minimum #Delta(dx/dz) of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxDDxDz","Maximum #Delta(dx/dz) of reconstructed muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxDDxDz","Maximum #Delta(dx/dz) of selected muons", 100, -0.5, 0.5, "#Delta(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgDDxDz","Average #Delta(dx/dz) of reconstructed muons", 100, -0.5, 0.5, "<#Delta(dx/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgDDxDz","Average #Delta(dx/dz) of selected muons", 100, -0.5, 0.5, "<#Delta(dx/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianDDxDz","Median #Delta(dx/dz) of reconstructed muons", 100, -0.5, 0.5, "<#Delta(dx/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianDDxDz","Median #Delta(dx/dz) of selected muons", 100, -0.5, 0.5, "<#Delta(dx/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevDDxDz","#sigma(#Delta(dx/dz)) of reconstructed muons", 100, 0., 0.5, "#sigma(#Delta(dx/dz))", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevDDxDz","#sigma(#Delta(dx/dz)) of selected muons", 100, 0., 0.5, "#sigma(#Delta(dx/dz))", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevDDxDz = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevDDxDz", "Average #Delta(dx/dz) vs. #sigma(#Delta(dx/dz)) of reconstructed muons", 100, 0., 0.5, 100, -0.5, 0.5, "#sigma(#Delta(dx/dz))", "<#Delta(dx/dz)>");
  TH2D* hSelMuonAvgVsStddevDDxDz = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevDDxDz", "Average #Delta(dx/dz) vs. #sigma(#Delta(dx/dz)) of selected muons", 100, 0., 0.5, 100, -0.5, 0.5, "#sigma(#Delta(dx/dz))", "<#Delta(dx/dz)>");
  TH2D* hRecoMuonMedianVsStddevDDxDz = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevDDxDz", "Median #Delta(dx/dz) vs. #sigma(#Delta(dx/dz)) of reconstructed muons", 100, 0., 0.5, 100, -0.5, 0.5, "#sigma(#Delta(dx/dz))", "<#Delta(dx/dz)>");
  TH2D* hSelMuonMedianVsStddevDDxDz = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevDDxDz", "Median #Delta(dx/dz) vs. #sigma(#Delta(dx/dz)) of selected muons", 100, 0., 0.5, 100, -0.5, 0.5, "#sigma(#Delta(dx/dz))", "<#Delta(dx/dz)>");
  std::vector<TH1D*>* hRecoMuonAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsDDxDz","|#Delta(dx/dz)| of reconstructed muons", 100, 0., 0.5, "|#Delta(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsDDxDz","|#Delta(dx/dz)| of selected muons", 100, 0., 0.5, "|#Delta(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsDDxDz","Minimum |#Delta(dx/dz)| of reconstructed muons", 100, 0., 0.5, "|#Delta(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsDDxDz","Minimum |#Delta(dx/dz)| of selected muons", 100, 0., 0.5, "|#Delta(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsDDxDz","Maximum |#Delta(dx/dz)| of reconstructed muons", 100, 0., 0.5, "|#Delta(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsDDxDz","Maximum |#Delta(dx/dz)| of selected muons", 100, 0., 0.5, "|#Delta(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsDDxDz","Average |#Delta(dx/dz)| of reconstructed muons", 100, 0., 0.5, "<|#Delta(dx/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsDDxDz","Average |#Delta(dx/dz)| of selected muons", 100, 0., 0.5, "<|#Delta(dx/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsDDxDz","Median |#Delta(dx/dz)| of reconstructed muons", 100, 0., 0.5, "<|#Delta(dx/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsDDxDz","Median |#Delta(dx/dz)| of selected muons", 100, 0., 0.5, "<|#Delta(dx/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsDDxDz","#sigma(|#Delta(dx/dz)|) of reconstructed muons", 100, 0., 0.5, "#sigma(|#Delta(dx/dz)|)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsDDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsDDxDz","#sigma(|#Delta(dx/dz)|) of selected muons", 100, 0., 0.5, "#sigma(|#Delta(dx/dz)|)", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsDDxDz = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsDDxDz", "Average |#Delta(dx/dz)| vs. #sigma(|#Delta(dx/dz)|) of reconstructed muons", 100, 0., 0.5, 100, 0., 0.5, "#sigma(|#Delta(dx/dz)|)", "<|#Delta(dx/dz)|>");
  TH2D* hSelMuonAvgVsStddevAbsDDxDz = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsDDxDz", "Average |#Delta(dx/dz)| vs. #sigma(|#Delta(dx/dz)|) of selected muons", 100, 0., 0.5, 100, 0., 0.5, "#sigma(|#Delta(dx/dz)|)", "<|#Delta(dx/dz)|>");
  TH2D* hRecoMuonMedianVsStddevAbsDDxDz = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsDDxDz", "Median |#Delta(dx/dz)| vs. #sigma(|#Delta(dx/dz)|) of reconstructed muons", 100, 0., 0.5, 100, 0., 0.5, "#sigma(|#Delta(dx/dz)|)", "<|#Delta(dx/dz)|>");
  TH2D* hSelMuonMedianVsStddevAbsDDxDz = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsDDxDz", "Median |#Delta(dx/dz)| vs. #sigma(|#Delta(dx/dz)|) of selected muons", 100, 0., 0.5, 100, 0., 0.5, "#sigma(|#Delta(dx/dz)|)", "<|#Delta(dx/dz)|>");
  
  std::vector<TH1D*>* hRecoMuonDDyDzInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDzInDTStation1","#Delta(dy/dz) in DT station 1 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDzInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDzInDTStation1","#Delta(dy/dz) in DT station 1 of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDyDzInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDzInDTStation2","#Delta(dy/dz) in DT station 2 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDzInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDzInDTStation2","#Delta(dy/dz) in DT station 2 of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDyDzInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDzInDTStation3","#Delta(dy/dz) in DT station 3 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDzInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDzInDTStation3","#Delta(dy/dz) in DT station 3 of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDyDzInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDzInDTStation4","#Delta(dy/dz) in DT station 4 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDzInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDzInDTStation4","#Delta(dy/dz) in DT station 4 of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDyDzInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDzInCSCStation1","#Delta(dy/dz) in CSC station 1 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDzInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDzInCSCStation1","#Delta(dy/dz) in CSC station 1 of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDyDzInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDzInCSCStation2","#Delta(dy/dz) in CSC station 2 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDzInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDzInCSCStation2","#Delta(dy/dz) in CSC station 2 of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDyDzInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDzInCSCStation3","#Delta(dy/dz) in CSC station 3 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDzInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDzInCSCStation3","#Delta(dy/dz) in CSC station 3 of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDyDzInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDzInCSCStation4","#Delta(dy/dz) in CSC station 4 of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDzInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDzInCSCStation4","#Delta(dy/dz) in CSC station 4 of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDDyDz","#Delta(dy/dz) of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonDDyDz","#Delta(dy/dz) of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinDDyDz","Minimum #Delta(dy/dz) of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinDDyDz","Minimum #Delta(dy/dz) of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxDDyDz","Maximum #Delta(dy/dz) of reconstructed muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxDDyDz","Maximum #Delta(dy/dz) of selected muons", 100, -0.5, 0.5, "#Delta(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgDDyDz","Average #Delta(dy/dz) of reconstructed muons", 100, -0.5, 0.5, "<#Delta(dy/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgDDyDz","Average #Delta(dy/dz) of selected muons", 100, -0.5, 0.5, "<#Delta(dy/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianDDyDz","Median #Delta(dy/dz) of reconstructed muons", 100, -0.5, 0.5, "<#Delta(dy/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianDDyDz","Median #Delta(dy/dz) of selected muons", 100, -0.5, 0.5, "<#Delta(dy/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevDDyDz","#sigma(#Delta(dy/dz)) of reconstructed muons", 100, 0., 0.5, "#sigma(#Delta(dy/dz))", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevDDyDz","#sigma(#Delta(dy/dz)) of selected muons", 100, 0., 0.5, "#sigma(#Delta(dy/dz))", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevDDyDz = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevDDyDz", "Average #Delta(dy/dz) vs. #sigma(#Delta(dy/dz)) of reconstructed muons", 100, 0., 0.5, 100, -0.5, 0.5, "#sigma(#Delta(dy/dz))", "<#Delta(dy/dz)>");
  TH2D* hSelMuonAvgVsStddevDDyDz = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevDDyDz", "Average #Delta(dy/dz) vs. #sigma(#Delta(dy/dz)) of selected muons", 100, 0., 0.5, 100, -0.5, 0.5, "#sigma(#Delta(dy/dz))", "<#Delta(dy/dz)>");
  TH2D* hRecoMuonMedianVsStddevDDyDz = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevDDyDz", "Median #Delta(dy/dz) vs. #sigma(#Delta(dy/dz)) of reconstructed muons", 100, 0., 0.5, 100, -0.5, 0.5, "#sigma(#Delta(dy/dz))", "<#Delta(dy/dz)>");
  TH2D* hSelMuonMedianVsStddevDDyDz = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevDDyDz", "Median #Delta(dy/dz) vs. #sigma(#Delta(dy/dz)) of selected muons", 100, 0., 0.5, 100, -0.5, 0.5, "#sigma(#Delta(dy/dz))", "<#Delta(dy/dz)>");
  std::vector<TH1D*>* hRecoMuonAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsDDyDz","|#Delta(dy/dz)| of reconstructed muons", 100, 0., 0.5, "|#Delta(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsDDyDz","|#Delta(dy/dz)| of selected muons", 100, 0., 0.5, "|#Delta(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsDDyDz","Minimum |#Delta(dy/dz)| of reconstructed muons", 100, 0., 0.5, "|#Delta(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsDDyDz","Minimum |#Delta(dy/dz)| of selected muons", 100, 0., 0.5, "|#Delta(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsDDyDz","Maximum |#Delta(dy/dz)| of reconstructed muons", 100, 0., 0.5, "|#Delta(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsDDyDz","Maximum |#Delta(dy/dz)| of selected muons", 100, 0., 0.5, "|#Delta(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsDDyDz","Average |#Delta(dy/dz)| of reconstructed muons", 100, 0., 0.5, "<|#Delta(dy/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsDDyDz","Average |#Delta(dy/dz)| of selected muons", 100, 0., 0.5, "<|#Delta(dy/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsDDyDz","Median |#Delta(dy/dz)| of reconstructed muons", 100, 0., 0.5, "<|#Delta(dy/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsDDyDz","Median |#Delta(dy/dz)| of selected muons", 100, 0., 0.5, "<|#Delta(dy/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsDDyDz","#sigma(|#Delta(dy/dz)|) of reconstructed muons", 100, 0., 0.5, "#sigma(|#Delta(dy/dz)|)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsDDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsDDyDz","#sigma(|#Delta(dy/dz)|) of selected muons", 100, 0., 0.5, "#sigma(|#Delta(dy/dz)|)", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsDDyDz = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsDDyDz", "Average |#Delta(dy/dz)| vs. #sigma(|#Delta(dy/dz)|) of reconstructed muons", 100, 0., 0.5, 100, 0., 0.5, "#sigma(|#Delta(dy/dz)|)", "<|#Delta(dy/dz)|>");
  TH2D* hSelMuonAvgVsStddevAbsDDyDz = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsDDyDz", "Average |#Delta(dy/dz)| vs. #sigma(|#Delta(dy/dz)|) of selected muons", 100, 0., 0.5, 100, 0., 0.5, "#sigma(|#Delta(dy/dz)|)", "<|#Delta(dy/dz)|>");
  TH2D* hRecoMuonMedianVsStddevAbsDDyDz = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsDDyDz", "Median |#Delta(dy/dz)| vs. #sigma(|#Delta(dy/dz)|) of reconstructed muons", 100, 0., 0.5, 100, 0., 0.5, "#sigma(|#Delta(dy/dz)|)", "<|#Delta(dy/dz)|>");
  TH2D* hSelMuonMedianVsStddevAbsDDyDz = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsDDyDz", "Median |#Delta(dy/dz)| vs. #sigma(|#Delta(dy/dz)|) of selected muons", 100, 0., 0.5, 100, 0., 0.5, "#sigma(|#Delta(dy/dz)|)", "<|#Delta(dy/dz)|>");
  
  std::vector<TH1D*>* hRecoMuonDxInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDxInDTStation1","#Delta x in DT station 1 of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxInDTStation1","#Delta x in DT station 1 of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDxInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDxInDTStation2","#Delta x in DT station 2 of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxInDTStation2","#Delta x in DT station 2 of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDxInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDxInDTStation3","#Delta x in DT station 3 of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxInDTStation3","#Delta x in DT station 3 of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDxInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDxInDTStation4","#Delta x in DT station 4 of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxInDTStation4","#Delta x in DT station 4 of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDxInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDxInCSCStation1","#Delta x in CSC station 1 of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxInCSCStation1","#Delta x in CSC station 1 of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDxInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDxInCSCStation2","#Delta x in CSC station 2 of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxInCSCStation2","#Delta x in CSC station 2 of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDxInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDxInCSCStation3","#Delta x in CSC station 3 of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxInCSCStation3","#Delta x in CSC station 3 of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDxInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDxInCSCStation4","#Delta x in CSC station 4 of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDxInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDxInCSCStation4","#Delta x in CSC station 4 of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDx","#Delta x of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonDx","#Delta x of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinDx","Minimum #Delta x of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinDx","Minimum #Delta x of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxDx","Maximum #Delta x of reconstructed muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxDx","Maximum #Delta x of selected muons", 100, -100., 100., "#Delta x [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgDx","Average #Delta x of reconstructed muons", 100, -100., 100., "<#Delta x> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgDx","Average #Delta x of selected muons", 100, -100., 100., "<#Delta x> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianDx","Median #Delta x of reconstructed muons", 100, -100., 100., "<#Delta x> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianDx","Median #Delta x of selected muons", 100, -100., 100., "<#Delta x> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevDx","#sigma(#Delta x) of reconstructed muons", 100, 0., 100., "#sigma(#Delta x) [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevDx","#sigma(#Delta x) of selected muons", 100, 0., 100., "#sigma(#Delta x) [cm]", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevDx = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevDx", "Average #Delta x vs. #sigma(#Delta x) of reconstructed muons", 100, 0., 100., 100, -100., 100., "#sigma(#Delta x) [cm]", "<#Delta x> [cm]");
  TH2D* hSelMuonAvgVsStddevDx = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevDx", "Average #Delta x vs. #sigma(#Delta x) of selected muons", 100, 0., 100., 100, -100., 100., "#sigma(#Delta x) [cm]", "<#Delta x> [cm]");
  TH2D* hRecoMuonMedianVsStddevDx = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevDx", "Median #Delta x vs. #sigma(#Delta x) of reconstructed muons", 100, 0., 100., 100, -100., 100., "#sigma(#Delta x) [cm]", "<#Delta x> [cm]");
  TH2D* hSelMuonMedianVsStddevDx = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevDx", "Median #Delta x vs. #sigma(#Delta x) of selected muons", 100, 0., 100., 100, -100., 100., "#sigma(#Delta x) [cm]", "<#Delta x> [cm]");
  std::vector<TH1D*>* hRecoMuonAbsDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsDx","|#Delta x| of reconstructed muons", 100, 0., 100., "|#Delta x| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsDx","|#Delta x| of selected muons", 100, 0., 100., "|#Delta x| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsDx","Minimum |#Delta x| of reconstructed muons", 100, 0., 100., "|#Delta x| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsDx","Minimum |#Delta x| of selected muons", 100, 0., 100., "|#Delta x| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsDx","Maximum |#Delta x| of reconstructed muons", 100, 0., 100., "|#Delta x| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsDx","Maximum |#Delta x| of selected muons", 100, 0., 100., "|#Delta x| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsDx","Average |#Delta x| of reconstructed muons", 100, 0., 100., "<|#Delta x|> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsDx","Average |#Delta x| of selected muons", 100, 0., 100., "<|#Delta x|> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsDx","Median |#Delta x| of reconstructed muons", 100, 0., 100., "<|#Delta x|> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsDx","Median |#Delta x| of selected muons", 100, 0., 100., "<|#Delta x|> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsDx = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsDx","#sigma(|#Delta x|) of reconstructed muons", 100, 0., 100., "#sigma(|#Delta x|) [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsDx = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsDx","#sigma(|#Delta x|) of selected muons", 100, 0., 100., "#sigma(|#Delta x|) [cm]", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsDx = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsDx", "Average |#Delta x| vs. #sigma(|#Delta x|) of reconstructed muons", 100, 0., 100., 100, 0., 100., "#sigma(|#Delta x|) [cm]", "<|#Delta x|> [cm]");
  TH2D* hSelMuonAvgVsStddevAbsDx = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsDx", "Average |#Delta x| vs. #sigma(|#Delta x|) of selected muons", 100, 0., 100., 100, 0., 100., "#sigma(|#Delta x|) [cm]", "<|#Delta x|> [cm]");
  TH2D* hRecoMuonMedianVsStddevAbsDx = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsDx", "Median |#Delta x| vs. #sigma(|#Delta x|) of reconstructed muons", 100, 0., 100., 100, 0., 100., "#sigma(|#Delta x|) [cm]", "<|#Delta x|> [cm]");
  TH2D* hSelMuonMedianVsStddevAbsDx = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsDx", "Median |#Delta x| vs. #sigma(|#Delta x|) of selected muons", 100, 0., 100., 100, 0., 100., "#sigma(|#Delta x|) [cm]", "<|#Delta x|> [cm]");
  
  std::vector<TH1D*>* hRecoMuonDyInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDyInDTStation1","#Delta y in DT station 1 of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDyInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDyInDTStation1","#Delta y in DT station 1 of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDyInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDyInDTStation2","#Delta y in DT station 2 of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDyInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDyInDTStation2","#Delta y in DT station 2 of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDyInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDyInDTStation3","#Delta y in DT station 3 of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDyInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDyInDTStation3","#Delta y in DT station 3 of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDyInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDyInDTStation4","#Delta y in DT station 4 of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDyInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDyInDTStation4","#Delta y in DT station 4 of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDyInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDyInCSCStation1","#Delta y in CSC station 1 of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDyInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDyInCSCStation1","#Delta y in CSC station 1 of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDyInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDyInCSCStation2","#Delta y in CSC station 2 of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDyInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDyInCSCStation2","#Delta y in CSC station 2 of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDyInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDyInCSCStation3","#Delta y in CSC station 3 of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDyInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDyInCSCStation3","#Delta y in CSC station 3 of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDyInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDyInCSCStation4","#Delta y in CSC station 4 of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDyInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDyInCSCStation4","#Delta y in CSC station 4 of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDy","#Delta y of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonDy","#Delta y of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinDy","Minimum #Delta y of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinDy","Minimum #Delta y of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxDy","Maximum #Delta y of reconstructed muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxDy","Maximum #Delta y of selected muons", 100, -100., 100., "#Delta y [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgDy","Average #Delta y of reconstructed muons", 100, -100., 100., "<#Delta y> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgDy","Average #Delta y of selected muons", 100, -100., 100., "<#Delta y> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianDy","Median #Delta y of reconstructed muons", 100, -100., 100., "<#Delta y> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianDy","Median #Delta y of selected muons", 100, -100., 100., "<#Delta y> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevDy","#sigma(#Delta y) of reconstructed muons", 100, 0., 100., "#sigma(#Delta y) [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevDy","#sigma(#Delta y) of selected muons", 100, 0., 100., "#sigma(#Delta y) [cm]", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevDy = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevDy", "Average #Delta y vs. #sigma(#Delta y) of reconstructed muons", 100, 0., 100., 100, -100., 100., "#sigma(#Delta y) [cm]", "<#Delta y> [cm]");
  TH2D* hSelMuonAvgVsStddevDy = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevDy", "Average #Delta y vs. #sigma(#Delta y) of selected muons", 100, 0., 100., 100, -100., 100., "#sigma(#Delta y) [cm]", "<#Delta y> [cm]");
  TH2D* hRecoMuonMedianVsStddevDy = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevDy", "Median #Delta y vs. #sigma(#Delta y) of reconstructed muons", 100, 0., 100., 100, -100., 100., "#sigma(#Delta y) [cm]", "<#Delta y> [cm]");
  TH2D* hSelMuonMedianVsStddevDy = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevDy", "Median #Delta y vs. #sigma(#Delta y) of selected muons", 100, 0., 100., 100, -100., 100., "#sigma(#Delta y) [cm]", "<#Delta y> [cm]");
  std::vector<TH1D*>* hRecoMuonAbsDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsDy","|#Delta y| of reconstructed muons", 100, 0., 100., "|#Delta y| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsDy","|#Delta y| of selected muons", 100, 0., 100., "|#Delta y| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsDy","Minimum |#Delta y| of reconstructed muons", 100, 0., 100., "|#Delta y| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsDy","Minimum |#Delta y| of selected muons", 100, 0., 100., "|#Delta y| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsDy","Maximum |#Delta y| of reconstructed muons", 100, 0., 100., "|#Delta y| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsDy","Maximum |#Delta y| of selected muons", 100, 0., 100., "|#Delta y| [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsDy","Average |#Delta y| of reconstructed muons", 100, 0., 100., "<|#Delta y|> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsDy","Average |#Delta y| of selected muons", 100, 0., 100., "<|#Delta y|> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsDy","Median |#Delta y| of reconstructed muons", 100, 0., 100., "<|#Delta y|> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsDy","Median |#Delta y| of selected muons", 100, 0., 100., "<|#Delta y|> [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsDy = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsDy","#sigma(|#Delta y|) of reconstructed muons", 100, 0., 100., "#sigma(|#Delta y|) [cm]", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsDy = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsDy","#sigma(|#Delta y|) of selected muons", 100, 0., 100., "#sigma(|#Delta y|) [cm]", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsDy = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsDy", "Average |#Delta y| vs. #sigma(|#Delta y|) of reconstructed muons", 100, 0., 100., 100, 0., 100., "#sigma(|#Delta y|) [cm]", "<|#Delta y|> [cm]");
  TH2D* hSelMuonAvgVsStddevAbsDy = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsDy", "Average |#Delta y| vs. #sigma(|#Delta y|) of selected muons", 100, 0., 100., 100, 0., 100., "#sigma(|#Delta y|) [cm]", "<|#Delta y|> [cm]");
  TH2D* hRecoMuonMedianVsStddevAbsDy = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsDy", "Median |#Delta y| vs. #sigma(|#Delta y|) of reconstructed muons", 100, 0., 100., 100, 0., 100., "#sigma(|#Delta y|) [cm]", "<|#Delta y|> [cm]");
  TH2D* hSelMuonMedianVsStddevAbsDy = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsDy", "Median |#Delta y| vs. #sigma(|#Delta y|) of selected muons", 100, 0., 100., 100, 0., 100., "#sigma(|#Delta y|) [cm]", "<|#Delta y|> [cm]");
  
  std::vector<TH1D*>* hRecoMuonPullDxDzInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDzInDTStation1","Pull(dx/dz) in DT station 1 of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDzInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDzInDTStation1","Pull(dx/dz) in DT station 1 of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDxDzInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDzInDTStation2","Pull(dx/dz) in DT station 2 of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDzInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDzInDTStation2","Pull(dx/dz) in DT station 2 of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDxDzInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDzInDTStation3","Pull(dx/dz) in DT station 3 of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDzInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDzInDTStation3","Pull(dx/dz) in DT station 3 of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDxDzInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDzInDTStation4","Pull(dx/dz) in DT station 4 of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDzInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDzInDTStation4","Pull(dx/dz) in DT station 4 of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDxDzInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDzInCSCStation1","Pull(dx/dz) in CSC station 1 of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDzInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDzInCSCStation1","Pull(dx/dz) in CSC station 1 of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDxDzInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDzInCSCStation2","Pull(dx/dz) in CSC station 2 of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDzInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDzInCSCStation2","Pull(dx/dz) in CSC station 2 of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDxDzInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDzInCSCStation3","Pull(dx/dz) in CSC station 3 of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDzInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDzInCSCStation3","Pull(dx/dz) in CSC station 3 of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDxDzInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDzInCSCStation4","Pull(dx/dz) in CSC station 4 of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDzInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDzInCSCStation4","Pull(dx/dz) in CSC station 4 of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDxDz","Pull(dx/dz) of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDxDz","Pull(dx/dz) of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinPullDxDz","Minimum Pull(dx/dz) of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinPullDxDz","Minimum Pull(dx/dz) of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxPullDxDz","Maximum Pull(dx/dz) of reconstructed muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxPullDxDz","Maximum Pull(dx/dz) of selected muons", 100, -5.0, 5.0, "Pull(dx/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgPullDxDz","Average Pull(dx/dz) of reconstructed muons", 100, -5.0, 5.0, "<Pull(dx/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgPullDxDz","Average Pull(dx/dz) of selected muons", 100, -5.0, 5.0, "<Pull(dx/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianPullDxDz","Median Pull(dx/dz) of reconstructed muons", 100, -5.0, 5.0, "<Pull(dx/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianPullDxDz","Median Pull(dx/dz) of selected muons", 100, -5.0, 5.0, "<Pull(dx/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevPullDxDz","#sigma(Pull(dx/dz)) of reconstructed muons", 100, 0., 5.0, "#sigma(Pull(dx/dz))", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevPullDxDz","#sigma(Pull(dx/dz)) of selected muons", 100, 0., 5.0, "#sigma(Pull(dx/dz))", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevPullDxDz = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevPullDxDz", "Average Pull(dx/dz) vs. #sigma(Pull(dx/dz)) of reconstructed muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(dx/dz))", "<Pull(dx/dz)>");
  TH2D* hSelMuonAvgVsStddevPullDxDz = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevPullDxDz", "Average Pull(dx/dz) vs. #sigma(Pull(dx/dz)) of selected muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(dx/dz))", "<Pull(dx/dz)>");
  TH2D* hRecoMuonMedianVsStddevPullDxDz = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevPullDxDz", "Median Pull(dx/dz) vs. #sigma(Pull(dx/dz)) of reconstructed muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(dx/dz))", "<Pull(dx/dz)>");
  TH2D* hSelMuonMedianVsStddevPullDxDz = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevPullDxDz", "Median Pull(dx/dz) vs. #sigma(Pull(dx/dz)) of selected muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(dx/dz))", "<Pull(dx/dz)>");
  std::vector<TH1D*>* hRecoMuonAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsPullDxDz","|Pull(dx/dz)| of reconstructed muons", 100, 0., 5.0, "|Pull(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsPullDxDz","|Pull(dx/dz)| of selected muons", 100, 0., 5.0, "|Pull(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsPullDxDz","Minimum |Pull(dx/dz)| of reconstructed muons", 100, 0., 5.0, "|Pull(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsPullDxDz","Minimum |Pull(dx/dz)| of selected muons", 100, 0., 5.0, "|Pull(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsPullDxDz","Maximum |Pull(dx/dz)| of reconstructed muons", 100, 0., 5.0, "|Pull(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsPullDxDz","Maximum |Pull(dx/dz)| of selected muons", 100, 0., 5.0, "|Pull(dx/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsPullDxDz","Average |Pull(dx/dz)| of reconstructed muons", 100, 0., 5.0, "<|Pull(dx/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsPullDxDz","Average |Pull(dx/dz)| of selected muons", 100, 0., 5.0, "<|Pull(dx/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsPullDxDz","Median |Pull(dx/dz)| of reconstructed muons", 100, 0., 5.0, "<|Pull(dx/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsPullDxDz","Median |Pull(dx/dz)| of selected muons", 100, 0., 5.0, "<|Pull(dx/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsPullDxDz","#sigma(|Pull(dx/dz)|) of reconstructed muons", 100, 0., 5.0, "#sigma(|Pull(dx/dz)|)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsPullDxDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsPullDxDz","#sigma(|Pull(dx/dz)|) of selected muons", 100, 0., 5.0, "#sigma(|Pull(dx/dz)|)", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsPullDxDz = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsPullDxDz", "Average |Pull(dx/dz)| vs. #sigma(|Pull(dx/dz)|) of reconstructed muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(dx/dz)|)", "<|Pull(dx/dz)|>");
  TH2D* hSelMuonAvgVsStddevAbsPullDxDz = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsPullDxDz", "Average |Pull(dx/dz)| vs. #sigma(|Pull(dx/dz)|) of selected muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(dx/dz)|)", "<|Pull(dx/dz)|>");
  TH2D* hRecoMuonMedianVsStddevAbsPullDxDz = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsPullDxDz", "Median |Pull(dx/dz)| vs. #sigma(|Pull(dx/dz)|) of reconstructed muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(dx/dz)|)", "<|Pull(dx/dz)|>");
  TH2D* hSelMuonMedianVsStddevAbsPullDxDz = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsPullDxDz", "Median |Pull(dx/dz)| vs. #sigma(|Pull(dx/dz)|) of selected muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(dx/dz)|)", "<|Pull(dx/dz)|>");
  
  std::vector<TH1D*>* hRecoMuonPullDyDzInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDzInDTStation1","Pull(dy/dz) in DT station 1 of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDzInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDzInDTStation1","Pull(dy/dz) in DT station 1 of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDyDzInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDzInDTStation2","Pull(dy/dz) in DT station 2 of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDzInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDzInDTStation2","Pull(dy/dz) in DT station 2 of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDyDzInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDzInDTStation3","Pull(dy/dz) in DT station 3 of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDzInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDzInDTStation3","Pull(dy/dz) in DT station 3 of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDyDzInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDzInDTStation4","Pull(dy/dz) in DT station 4 of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDzInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDzInDTStation4","Pull(dy/dz) in DT station 4 of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDyDzInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDzInCSCStation1","Pull(dy/dz) in CSC station 1 of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDzInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDzInCSCStation1","Pull(dy/dz) in CSC station 1 of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDyDzInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDzInCSCStation2","Pull(dy/dz) in CSC station 2 of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDzInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDzInCSCStation2","Pull(dy/dz) in CSC station 2 of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDyDzInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDzInCSCStation3","Pull(dy/dz) in CSC station 3 of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDzInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDzInCSCStation3","Pull(dy/dz) in CSC station 3 of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDyDzInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDzInCSCStation4","Pull(dy/dz) in CSC station 4 of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDzInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDzInCSCStation4","Pull(dy/dz) in CSC station 4 of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullDyDz","Pull(dy/dz) of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullDyDz","Pull(dy/dz) of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinPullDyDz","Minimum Pull(dy/dz) of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinPullDyDz","Minimum Pull(dy/dz) of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxPullDyDz","Maximum Pull(dy/dz) of reconstructed muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxPullDyDz","Maximum Pull(dy/dz) of selected muons", 100, -5.0, 5.0, "Pull(dy/dz)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgPullDyDz","Average Pull(dy/dz) of reconstructed muons", 100, -5.0, 5.0, "<Pull(dy/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgPullDyDz","Average Pull(dy/dz) of selected muons", 100, -5.0, 5.0, "<Pull(dy/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianPullDyDz","Median Pull(dy/dz) of reconstructed muons", 100, -5.0, 5.0, "<Pull(dy/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianPullDyDz","Median Pull(dy/dz) of selected muons", 100, -5.0, 5.0, "<Pull(dy/dz)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevPullDyDz","#sigma(Pull(dy/dz)) of reconstructed muons", 100, 0., 5.0, "#sigma(Pull(dy/dz))", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevPullDyDz","#sigma(Pull(dy/dz)) of selected muons", 100, 0., 5.0, "#sigma(Pull(dy/dz))", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevPullDyDz = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevPullDyDz", "Average Pull(dy/dz) vs. #sigma(Pull(dy/dz)) of reconstructed muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(dy/dz))", "<Pull(dy/dz)>");
  TH2D* hSelMuonAvgVsStddevPullDyDz = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevPullDyDz", "Average Pull(dy/dz) vs. #sigma(Pull(dy/dz)) of selected muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(dy/dz))", "<Pull(dy/dz)>");
  TH2D* hRecoMuonMedianVsStddevPullDyDz = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevPullDyDz", "Median Pull(dy/dz) vs. #sigma(Pull(dy/dz)) of reconstructed muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(dy/dz))", "<Pull(dy/dz)>");
  TH2D* hSelMuonMedianVsStddevPullDyDz = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevPullDyDz", "Median Pull(dy/dz) vs. #sigma(Pull(dy/dz)) of selected muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(dy/dz))", "<Pull(dy/dz)>");
  std::vector<TH1D*>* hRecoMuonAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsPullDyDz","|Pull(dy/dz)| of reconstructed muons", 100, 0., 5.0, "|Pull(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsPullDyDz","|Pull(dy/dz)| of selected muons", 100, 0., 5.0, "|Pull(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsPullDyDz","Minimum |Pull(dy/dz)| of reconstructed muons", 100, 0., 5.0, "|Pull(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsPullDyDz","Minimum |Pull(dy/dz)| of selected muons", 100, 0., 5.0, "|Pull(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsPullDyDz","Maximum |Pull(dy/dz)| of reconstructed muons", 100, 0., 5.0, "|Pull(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsPullDyDz","Maximum |Pull(dy/dz)| of selected muons", 100, 0., 5.0, "|Pull(dy/dz)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsPullDyDz","Average |Pull(dy/dz)| of reconstructed muons", 100, 0., 5.0, "<|Pull(dy/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsPullDyDz","Average |Pull(dy/dz)| of selected muons", 100, 0., 5.0, "<|Pull(dy/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsPullDyDz","Median |Pull(dy/dz)| of reconstructed muons", 100, 0., 5.0, "<|Pull(dy/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsPullDyDz","Median |Pull(dy/dz)| of selected muons", 100, 0., 5.0, "<|Pull(dy/dz)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsPullDyDz","#sigma(|Pull(dy/dz)|) of reconstructed muons", 100, 0., 5.0, "#sigma(|Pull(dy/dz)|)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsPullDyDz = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsPullDyDz","#sigma(|Pull(dy/dz)|) of selected muons", 100, 0., 5.0, "#sigma(|Pull(dy/dz)|)", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsPullDyDz = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsPullDyDz", "Average |Pull(dy/dz)| vs. #sigma(|Pull(dy/dz)|) of reconstructed muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(dy/dz)|)", "<|Pull(dy/dz)|>");
  TH2D* hSelMuonAvgVsStddevAbsPullDyDz = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsPullDyDz", "Average |Pull(dy/dz)| vs. #sigma(|Pull(dy/dz)|) of selected muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(dy/dz)|)", "<|Pull(dy/dz)|>");
  TH2D* hRecoMuonMedianVsStddevAbsPullDyDz = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsPullDyDz", "Median |Pull(dy/dz)| vs. #sigma(|Pull(dy/dz)|) of reconstructed muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(dy/dz)|)", "<|Pull(dy/dz)|>");
  TH2D* hSelMuonMedianVsStddevAbsPullDyDz = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsPullDyDz", "Median |Pull(dy/dz)| vs. #sigma(|Pull(dy/dz)|) of selected muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(dy/dz)|)", "<|Pull(dy/dz)|>");
  
  std::vector<TH1D*>* hRecoMuonPullXInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullXInDTStation1","Pull(x) in DT station 1 of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullXInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullXInDTStation1","Pull(x) in DT station 1 of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullXInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullXInDTStation2","Pull(x) in DT station 2 of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullXInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullXInDTStation2","Pull(x) in DT station 2 of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullXInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullXInDTStation3","Pull(x) in DT station 3 of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullXInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullXInDTStation3","Pull(x) in DT station 3 of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullXInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullXInDTStation4","Pull(x) in DT station 4 of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullXInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullXInDTStation4","Pull(x) in DT station 4 of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullXInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullXInCSCStation1","Pull(x) in CSC station 1 of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullXInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullXInCSCStation1","Pull(x) in CSC station 1 of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullXInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullXInCSCStation2","Pull(x) in CSC station 2 of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullXInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullXInCSCStation2","Pull(x) in CSC station 2 of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullXInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullXInCSCStation3","Pull(x) in CSC station 3 of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullXInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullXInCSCStation3","Pull(x) in CSC station 3 of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullXInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullXInCSCStation4","Pull(x) in CSC station 4 of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullXInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullXInCSCStation4","Pull(x) in CSC station 4 of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullX","Pull(x) of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullX","Pull(x) of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinPullX","Minimum Pull(x) of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinPullX","Minimum Pull(x) of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxPullX","Maximum Pull(x) of reconstructed muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxPullX","Maximum Pull(x) of selected muons", 100, -5.0, 5.0, "Pull(x)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgPullX","Average Pull(x) of reconstructed muons", 100, -5.0, 5.0, "<Pull(x)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgPullX","Average Pull(x) of selected muons", 100, -5.0, 5.0, "<Pull(x)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianPullX","Median Pull(x) of reconstructed muons", 100, -5.0, 5.0, "<Pull(x)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianPullX","Median Pull(x) of selected muons", 100, -5.0, 5.0, "<Pull(x)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevPullX","#sigma(Pull(x)) of reconstructed muons", 100, 0., 5.0, "#sigma(Pull(x))", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevPullX","#sigma(Pull(x)) of selected muons", 100, 0., 5.0, "#sigma(Pull(x))", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevPullX = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevPullX", "Average Pull(x) vs. #sigma(Pull(x)) of reconstructed muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(x))", "<Pull(x)>");
  TH2D* hSelMuonAvgVsStddevPullX = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevPullX", "Average Pull(x) vs. #sigma(Pull(x)) of selected muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(x))", "<Pull(x)>");
  TH2D* hRecoMuonMedianVsStddevPullX = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevPullX", "Median Pull(x) vs. #sigma(Pull(x)) of reconstructed muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(x))", "<Pull(x)>");
  TH2D* hSelMuonMedianVsStddevPullX = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevPullX", "Median Pull(x) vs. #sigma(Pull(x)) of selected muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(x))", "<Pull(x)>");
  std::vector<TH1D*>* hRecoMuonAbsPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsPullX","|Pull(x)| of reconstructed muons", 100, 0., 5.0, "|Pull(x)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsPullX","|Pull(x)| of selected muons", 100, 0., 5.0, "|Pull(x)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsPullX","Minimum |Pull(x)| of reconstructed muons", 100, 0., 5.0, "|Pull(x)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsPullX","Minimum |Pull(x)| of selected muons", 100, 0., 5.0, "|Pull(x)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsPullX","Maximum |Pull(x)| of reconstructed muons", 100, 0., 5.0, "|Pull(x)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsPullX","Maximum |Pull(x)| of selected muons", 100, 0., 5.0, "|Pull(x)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsPullX","Average |Pull(x)| of reconstructed muons", 100, 0., 5.0, "<|Pull(x)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsPullX","Average |Pull(x)| of selected muons", 100, 0., 5.0, "<|Pull(x)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsPullX","Median |Pull(x)| of reconstructed muons", 100, 0., 5.0, "<|Pull(x)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsPullX","Median |Pull(x)| of selected muons", 100, 0., 5.0, "<|Pull(x)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsPullX = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsPullX","#sigma(|Pull(x)|) of reconstructed muons", 100, 0., 5.0, "#sigma(|Pull(x)|)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsPullX = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsPullX","#sigma(|Pull(x)|) of selected muons", 100, 0., 5.0, "#sigma(|Pull(x)|)", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsPullX = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsPullX", "Average |Pull(x)| vs. #sigma(|Pull(x)|) of reconstructed muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(x)|)", "<|Pull(x)|>");
  TH2D* hSelMuonAvgVsStddevAbsPullX = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsPullX", "Average |Pull(x)| vs. #sigma(|Pull(x)|) of selected muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(x)|)", "<|Pull(x)|>");
  TH2D* hRecoMuonMedianVsStddevAbsPullX = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsPullX", "Median |Pull(x)| vs. #sigma(|Pull(x)|) of reconstructed muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(x)|)", "<|Pull(x)|>");
  TH2D* hSelMuonMedianVsStddevAbsPullX = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsPullX", "Median |Pull(x)| vs. #sigma(|Pull(x)|) of selected muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(x)|)", "<|Pull(x)|>");
  
  std::vector<TH1D*>* hRecoMuonPullYInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullYInDTStation1","Pull(y) in DT station 1 of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullYInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullYInDTStation1","Pull(y) in DT station 1 of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullYInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullYInDTStation2","Pull(y) in DT station 2 of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullYInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullYInDTStation2","Pull(y) in DT station 2 of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullYInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullYInDTStation3","Pull(y) in DT station 3 of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullYInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullYInDTStation3","Pull(y) in DT station 3 of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullYInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullYInDTStation4","Pull(y) in DT station 4 of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullYInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullYInDTStation4","Pull(y) in DT station 4 of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullYInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullYInCSCStation1","Pull(y) in CSC station 1 of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullYInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullYInCSCStation1","Pull(y) in CSC station 1 of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullYInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullYInCSCStation2","Pull(y) in CSC station 2 of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullYInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullYInCSCStation2","Pull(y) in CSC station 2 of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullYInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullYInCSCStation3","Pull(y) in CSC station 3 of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullYInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullYInCSCStation3","Pull(y) in CSC station 3 of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullYInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullYInCSCStation4","Pull(y) in CSC station 4 of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullYInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullYInCSCStation4","Pull(y) in CSC station 4 of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonPullY","Pull(y) of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonPullY","Pull(y) of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinPullY","Minimum Pull(y) of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinPullY","Minimum Pull(y) of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxPullY","Maximum Pull(y) of reconstructed muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxPullY","Maximum Pull(y) of selected muons", 100, -5.0, 5.0, "Pull(y)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgPullY","Average Pull(y) of reconstructed muons", 100, -5.0, 5.0, "<Pull(y)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgPullY","Average Pull(y) of selected muons", 100, -5.0, 5.0, "<Pull(y)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianPullY","Median Pull(y) of reconstructed muons", 100, -5.0, 5.0, "<Pull(y)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianPullY","Median Pull(y) of selected muons", 100, -5.0, 5.0, "<Pull(y)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevPullY","#sigma(Pull(y)) of reconstructed muons", 100, 0., 5.0, "#sigma(Pull(y))", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevPullY","#sigma(Pull(y)) of selected muons", 100, 0., 5.0, "#sigma(Pull(y))", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevPullY = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevPullY", "Average Pull(y) vs. #sigma(Pull(y)) of reconstructed muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(y))", "<Pull(y)>");
  TH2D* hSelMuonAvgVsStddevPullY = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevPullY", "Average Pull(y) vs. #sigma(Pull(y)) of selected muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(y))", "<Pull(y)>");
  TH2D* hRecoMuonMedianVsStddevPullY = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevPullY", "Median Pull(y) vs. #sigma(Pull(y)) of reconstructed muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(y))", "<Pull(y)>");
  TH2D* hSelMuonMedianVsStddevPullY = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevPullY", "Median Pull(y) vs. #sigma(Pull(y)) of selected muons", 100, 0., 5.0, 100, -5.0, 5.0, "#sigma(Pull(y))", "<Pull(y)>");
  std::vector<TH1D*>* hRecoMuonAbsPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsPullY","|Pull(y)| of reconstructed muons", 100, 0., 5.0, "|Pull(y)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsPullY","|Pull(y)| of selected muons", 100, 0., 5.0, "|Pull(y)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsPullY","Minimum |Pull(y)| of reconstructed muons", 100, 0., 5.0, "|Pull(y)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsPullY","Minimum |Pull(y)| of selected muons", 100, 0., 5.0, "|Pull(y)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsPullY","Maximum |Pull(y)| of reconstructed muons", 100, 0., 5.0, "|Pull(y)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsPullY","Maximum |Pull(y)| of selected muons", 100, 0., 5.0, "|Pull(y)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsPullY","Average |Pull(y)| of reconstructed muons", 100, 0., 5.0, "<|Pull(y)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsPullY","Average |Pull(y)| of selected muons", 100, 0., 5.0, "<|Pull(y)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsPullY","Median |Pull(y)| of reconstructed muons", 100, 0., 5.0, "<|Pull(y)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsPullY","Median |Pull(y)| of selected muons", 100, 0., 5.0, "<|Pull(y)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsPullY = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsPullY","#sigma(|Pull(y)|) of reconstructed muons", 100, 0., 5.0, "#sigma(|Pull(y)|)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsPullY = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsPullY","#sigma(|Pull(y)|) of selected muons", 100, 0., 5.0, "#sigma(|Pull(y)|)", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsPullY = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsPullY", "Average |Pull(y)| vs. #sigma(|Pull(y)|) of reconstructed muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(y)|)", "<|Pull(y)|>");
  TH2D* hSelMuonAvgVsStddevAbsPullY = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsPullY", "Average |Pull(y)| vs. #sigma(|Pull(y)|) of selected muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(y)|)", "<|Pull(y)|>");
  TH2D* hRecoMuonMedianVsStddevAbsPullY = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsPullY", "Median |Pull(y)| vs. #sigma(|Pull(y)|) of reconstructed muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(y)|)", "<|Pull(y)|>");
  TH2D* hSelMuonMedianVsStddevAbsPullY = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsPullY", "Median |Pull(y)| vs. #sigma(|Pull(y)|) of selected muons", 100, 0., 5.0, 100, 0., 5.0, "#sigma(|Pull(y)|)", "<|Pull(y)|>");
  
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraightInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraightInDTStation1","#Delta(#theta) w.r.t. straight line in DT station 1 of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraightInDTStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraightInDTStation1","#Delta(#theta) w.r.t. straight line in DT station 1 of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraightInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraightInDTStation2","#Delta(#theta) w.r.t. straight line in DT station 2 of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraightInDTStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraightInDTStation2","#Delta(#theta) w.r.t. straight line in DT station 2 of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraightInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraightInDTStation3","#Delta(#theta) w.r.t. straight line in DT station 3 of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraightInDTStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraightInDTStation3","#Delta(#theta) w.r.t. straight line in DT station 3 of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraightInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraightInDTStation4","#Delta(#theta) w.r.t. straight line in DT station 4 of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraightInDTStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraightInDTStation4","#Delta(#theta) w.r.t. straight line in DT station 4 of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraightInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraightInCSCStation1","#Delta(#theta) w.r.t. straight line in CSC station 1 of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraightInCSCStation1 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraightInCSCStation1","#Delta(#theta) w.r.t. straight line in CSC station 1 of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraightInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraightInCSCStation2","#Delta(#theta) w.r.t. straight line in CSC station 2 of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraightInCSCStation2 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraightInCSCStation2","#Delta(#theta) w.r.t. straight line in CSC station 2 of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraightInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraightInCSCStation3","#Delta(#theta) w.r.t. straight line in CSC station 3 of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraightInCSCStation3 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraightInCSCStation3","#Delta(#theta) w.r.t. straight line in CSC station 3 of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraightInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraightInCSCStation4","#Delta(#theta) w.r.t. straight line in CSC station 4 of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraightInCSCStation4 = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraightInCSCStation4","#Delta(#theta) w.r.t. straight line in CSC station 4 of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonDThetaWrtStraight","#Delta(#theta) w.r.t. straight line of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonDThetaWrtStraight","#Delta(#theta) w.r.t. straight line of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinDThetaWrtStraight","Minimum #Delta(#theta) w.r.t. straight line of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinDThetaWrtStraight","Minimum #Delta(#theta) w.r.t. straight line of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxDThetaWrtStraight","Maximum #Delta(#theta) w.r.t. straight line of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxDThetaWrtStraight","Maximum #Delta(#theta) w.r.t. straight line of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgDThetaWrtStraight","Average #Delta(#theta) w.r.t. straight line of reconstructed muons", 100, -0.05, 0.05, "<#Delta(#theta)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgDThetaWrtStraight","Average #Delta(#theta) w.r.t. straight line of selected muons", 100, -0.05, 0.05, "<#Delta(#theta)>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianDThetaWrtStraight","Median #Delta(#theta) w.r.t. straight line of reconstructed muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianDThetaWrtStraight","Median #Delta(#theta) w.r.t. straight line of selected muons", 100, -0.05, 0.05, "#Delta(#theta)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevDThetaWrtStraight","#sigma(#Delta(#theta)) w.r.t. straight line of reconstructed muons", 100, 0., 0.05, "#sigma(#Delta(#theta))", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevDThetaWrtStraight","#sigma(#Delta(#theta)) w.r.t. straight line of selected muons", 100, 0., 0.05, "#sigma(#Delta(#theta))", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevDThetaWrtStraight = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevDThetaWrtStraight", "Average #Delta(#theta) vs. #sigma(#Delta(#theta)) w.r.t. straight line of reconstructed muons", 100, 0., 0.05, 100, -0.05, 0.05, "#sigma(#Delta(#theta))", "<#Delta(#theta)>");
  TH2D* hSelMuonAvgVsStddevDThetaWrtStraight = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevDThetaWrtStraight", "Average #Delta(#theta) vs. #sigma(#Delta(#theta)) w.r.t. straight line of selected muons", 100, 0., 0.05, 100, -0.05, 0.05, "#sigma(#Delta(#theta))", "<#Delta(#theta)>");
  TH2D* hRecoMuonMedianVsStddevDThetaWrtStraight = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevDThetaWrtStraight", "Median #Delta(#theta) vs. #sigma(#Delta(#theta)) w.r.t. straight line of reconstructed muons", 100, 0., 0.05, 100, -0.05, 0.05, "#sigma(#Delta(#theta))", "<#Delta(#theta)>");
  TH2D* hSelMuonMedianVsStddevDThetaWrtStraight = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevDThetaWrtStraight", "Median #Delta(#theta) vs. #sigma(#Delta(#theta)) w.r.t. straight line of selected muons", 100, 0., 0.05, 100, -0.05, 0.05, "#sigma(#Delta(#theta))", "<#Delta(#theta)>");
  std::vector<TH1D*>* hRecoMuonAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAbsDThetaWrtStraight","|#Delta(#theta)| w.r.t. straight line of reconstructed muons", 100, 0., 0.05, "|#Delta(#theta)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonAbsDThetaWrtStraight","|#Delta(#theta)| w.r.t. straight line of selected muons", 100, 0., 0.05, "|#Delta(#theta)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMinAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMinAbsDThetaWrtStraight","Minimum |#Delta(#theta)| w.r.t. straight line of reconstructed muons", 100, 0., 0.05, "|#Delta(#theta)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMinAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonMinAbsDThetaWrtStraight","Minimum |#Delta(#theta)| w.r.t. straight line of selected muons", 100, 0., 0.05, "|#Delta(#theta)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMaxAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMaxAbsDThetaWrtStraight","Maximum |#Delta(#theta)| w.r.t. straight line of reconstructed muons", 100, 0., 0.05, "|#Delta(#theta)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMaxAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonMaxAbsDThetaWrtStraight","Maximum |#Delta(#theta)| w.r.t. straight line of selected muons", 100, 0., 0.05, "|#Delta(#theta)|", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonAvgAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonAvgAbsDThetaWrtStraight","Average |#Delta(#theta)| w.r.t. straight line of reconstructed muons", 100, 0., 0.05, "<|#Delta(#theta)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonAvgAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonAvgAbsDThetaWrtStraight","Average |#Delta(#theta)| w.r.t. straight line of selected muons", 100, 0., 0.05, "<|#Delta(#theta)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonMedianAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonMedianAbsDThetaWrtStraight","Median |#Delta(#theta)| w.r.t. straight line of reconstructed muons", 100, 0., 0.05, "<|#Delta(#theta)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonMedianAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonMedianAbsDThetaWrtStraight","Median |#Delta(#theta)| w.r.t. straight line of selected muons", 100, 0., 0.05, "<|#Delta(#theta)|>", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hRecoMuonStddevAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hRecoMuonStddevAbsDThetaWrtStraight","#sigma(|#Delta(#theta)|) w.r.t. straight line of reconstructed muons", 100, 0., 0.05, "#sigma(|#Delta(#theta)|)", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuonStddevAbsDThetaWrtStraight = CreateVectorOf1DHistograms<TH1D>("hSelMuonStddevAbsDThetaWrtStraight","#sigma(|#Delta(#theta)|) w.r.t. straight line of selected muons", 100, 0., 0.05, "#sigma(|#Delta(#theta)|)", nMuLabel,sBinningVariable,vLowBins);
  TH2D* hRecoMuonAvgVsStddevAbsDThetaWrtStraight = Create2DHistogram<TH2D>("hRecoMuonAvgVsStddevAbsDThetaWrtStraight", "Average |#Delta(#theta)| vs. #sigma(|#Delta(#theta)|) w.r.t. straight line of reconstructed muons", 100, 0., 0.05, 100, 0., 0.05, "#sigma(|#Delta(#theta)|)", "<|#Delta(#theta)|>");
  TH2D* hSelMuonAvgVsStddevAbsDThetaWrtStraight = Create2DHistogram<TH2D>("hSelMuonAvgVsStddevAbsDThetaWrtStraight", "Average |#Delta(#theta)| vs. #sigma(|#Delta(#theta)|) w.r.t. straight line of selected muons", 100, 0., 0.05, 100, 0., 0.05, "#sigma(|#Delta(#theta)|)", "<|#Delta(#theta)|>");
  TH2D* hRecoMuonMedianVsStddevAbsDThetaWrtStraight = Create2DHistogram<TH2D>("hRecoMuonMedianVsStddevAbsDThetaWrtStraight", "Median |#Delta(#theta)| vs. #sigma(|#Delta(#theta)|) w.r.t. straight line of reconstructed muons", 100, 0., 0.05, 100, 0., 0.05, "#sigma(|#Delta(#theta)|)", "<|#Delta(#theta)|>");
  TH2D* hSelMuonMedianVsStddevAbsDThetaWrtStraight = Create2DHistogram<TH2D>("hSelMuonMedianVsStddevAbsDThetaWrtStraight", "Median |#Delta(#theta)| vs. #sigma(|#Delta(#theta)|) w.r.t. straight line of selected muons", 100, 0., 0.05, 100, 0., 0.05, "#sigma(|#Delta(#theta)|)", "<|#Delta(#theta)|>");
  
  TH2D* hRecoMuonDThetaWrtStraightVsP = Create2DHistogram<TH2D>("hRecoMuonDThetaWrtStraightVsP", "#Delta(#theta) w.r.t. straight line of reconstructed muons vs. momentum", 10, 0., 50., 100, -0.5, 0.5, "p [GeV]", "#Delta(#theta)");
  TH2D* hSelMuonDThetaWrtStraightVsP = Create2DHistogram<TH2D>("hSelMuonDThetaWrtStraightVsP", "#Delta(#theta) w.r.t. straight line of selected muons vs. momentum", 10, 0., 50., 100, -0.5, 0.5, "p [GeV]", "#Delta(#theta)");
  
  /*
  TH1D* hRecoMuon = Create1DHistogram<TH1D>("hRecoMuon","of reconstructed muons", 11, -0.5, 10.5, "", nMuLabel);
  TH1D* hSelMuon = Create1DHistogram<TH1D>("hSelMuon","of selected muons", 11, -0.5, 10.5, "", nMuLabel);

  std::vector<TH1D*>* hRecoMuon = CreateVectorOf1DHistograms<TH1D>("hRecoMuon"," of reconstructed muons", 100, -0.5, 0.5, "", nMuLabel,sBinningVariable,vLowBins);
  std::vector<TH1D*>* hSelMuon = CreateVectorOf1DHistograms<TH1D>("hSelMuon"," of selected muons", 100, -0.5, 0.5, "", nMuLabel,sBinningVariable,vLowBins);  
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
//     if(numberOfMuon == 0)
//     {
// //       std::cout << "Event rejected because it does not have reco muons!\n";
//       continue;
//     }
    
    // Loop over gen particles to get the kaons and pions from B decay
    std::vector<unsigned short> vParticlesFromB;
    std::vector<unsigned short> vParticlesInAccFromB;
    std::vector<double> vDecayLengths;
    std::vector<double> vDecayZ;
    std::vector<double> vDecayR;
    std::vector<unsigned short> vParticlesPlusFromB;
    std::vector<unsigned short> vParticlesPlusInAccFromB;
    std::vector<double> vDecayLengthsPlus;
    std::vector<double> vDecayZPlus;
    std::vector<double> vDecayRPlus;
    std::vector<unsigned short> vParticlesMinusFromB;
    std::vector<unsigned short> vParticlesMinusInAccFromB;
    std::vector<double> vDecayLengthsMinus;
    std::vector<double> vDecayZMinus;
    std::vector<double> vDecayRMinus;
    for(unsigned int iGenParticle = 0; iGenParticle < Genp_Id->size(); iGenParticle++)
    {
      if(Genp_pt->at(iGenParticle) > 3)
      {
        int genpId = Genp_Id->at(iGenParticle);
        if(abs(genpId) == trueGenpId)
        {
//           if(Genp_vMom->at(iGenParticle).size() > 1)
//           {
//             bool hasTrueMom = false;
//             for(unsigned int iMom = 1; iMom < Genp_vMom->at(iGenParticle).size(); iMom++)
//             {
//               if(abs(Genp_vMom->at(iGenParticle).at(iMom)) == trueBId)
//               {
//                 hasTrueMom = true;
//                 break;
//               }
//             }
//             if(hasTrueMom)
//             {
//               std::cout << "W A R N I N G ! GenParticle has true mom but it is not the first one!!!\n";
//               std::cout << "                gen eta = " << Genp_eta->at(iGenParticle) << ", gen pt = " << Genp_pt->at(iGenParticle) << std::endl;
//             }
//           }
          int iMother = Genp_vMom->at(iGenParticle).at(0);
          int motherId = Genp_Id->at(iMother);
          if(abs(motherId) == trueBId)
          {
//             std::cout << "Found a kaon or a pion coming from a B!\n";
//             std::cout << "I = " << iGenParticle << std::endl;
//             std::cout << "ID = " << genpId << std::endl;
//             std::cout << "iMother = " << iMother << std::endl;
//             std::cout << "motherId = " << motherId << std::endl;
            double binningVariableValue = GetGenBinningVariablesValues(iGenParticle).at(binningVariableIndex);
//             std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticle(iGenParticle);
            std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticleWithPtThreshold(iGenParticle, 0.0);
            short int iTrack = iTrackWithDistance.first;
            // Just for cross-check: try matching also in the other direction and print warning if it is different
//             std::pair<short int, double> iGenParticleWithDistance = FindGenParticleNearestToTrack(iTrack);
            if(iTrack!=-1)
            {
              std::pair<short int, double> iGenParticleWithDistance = FindGenParticleNearestToTrackWithPtThreshold(iTrack, 0.0);
              if(iGenParticleWithDistance.first != abs(iGenParticle))
              {
                if(/*fabs((Track_pt->at(iTrack) - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle)) < 0.05 &&
                   fabs((Track_pt->at(iTrack) - Genp_pt->at(iGenParticleWithDistance.first)) / Genp_pt->at(iGenParticleWithDistance.first)) < 0.05 &&*/
                   iTrackWithDistance.second < 0.1 &&
                   iGenParticleWithDistance.second < 0.1/* &&
                   Genp_pt->at(iGenParticle) > 3 &&
                   Genp_pt->at(iGenParticleWithDistance.first) > 3*/)
                {
                  bool isADaughter = false;
                  isADaughter = RecursiveLookForDaughter(iGenParticle,iGenParticleWithDistance.first);
                  if(!isADaughter)
                  {
                    isADaughter = RecursiveLookForDaughter(iGenParticleWithDistance.first,iGenParticle);
                  }
                  if(!isADaughter)
                  {
//                     std::cout << "W A R N I N G ! Ev. " << jentry << ", Gen->Reco and Reco->Gen associations give different results!!!\n";
//                     std::cout << "                Gen->Reco:\n";
//                     std::cout << "                   iGenParticle = " << iGenParticle << " (ID = " << Genp_Id->at(iGenParticle) << ")\n";
//                     std::cout << "                                  pt = " << Genp_pt->at(iGenParticle) << ", eta = " << Genp_eta->at(iGenParticle) << ", phi = " << Genp_phi->at(iGenParticle) << std::endl;
//                     std::cout << "                   iTrack       = " << iTrackWithDistance.first << std::endl;
//                     std::cout << "                                  pt = " << Track_pt->at(iTrackWithDistance.first) << ", eta = " << Track_eta->at(iTrackWithDistance.first) << ", phi = " << Track_phi->at(iTrackWithDistance.first) << std::endl; 
//                     std::cout << "                   DeltaR       = " << iTrackWithDistance.second << std::endl;
//                     std::cout << "                   DeltaPt/Pt   = " << fabs((Track_pt->at(iTrack) - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle)) << std::endl;
//                     std::cout << "                Reco->Gen:\n";
//                     std::cout << "                   iGenParticle = " << iGenParticleWithDistance.first  << " (ID = " << Genp_Id->at(iGenParticleWithDistance.first) << ")\n";
//                     std::cout << "                                  pt = " << Genp_pt->at(iGenParticleWithDistance.first) << ", eta = " << Genp_eta->at(iGenParticleWithDistance.first) << ", phi = " << Genp_phi->at(iGenParticleWithDistance.first) << std::endl;
//                     std::cout << "                   iTrack       = " << iTrackWithDistance.first << std::endl;
//                     std::cout << "                                  pt = " << Track_pt->at(iTrackWithDistance.first) << ", eta = " << Track_eta->at(iTrackWithDistance.first) << ", phi = " << Track_phi->at(iTrackWithDistance.first) << std::endl; 
//                     std::cout << "                   DeltaR       = " << iGenParticleWithDistance.second << std::endl;
//                     std::cout << "                   DeltaPt/Pt   = " << fabs((Track_pt->at(iTrack) - Genp_pt->at(iGenParticleWithDistance.first)) / Genp_pt->at(iGenParticleWithDistance.first)) << std::endl;
                  }
                  else
                  {
                    std::cout << "I N F O : Ev. " << jentry << ", Gen->Reco and Reco->Gen associations give different results, but they are in a mother-daughter relationship.\n";
                  }
                }
              }
            }
            double dPtOverPt = 999.9;
            if(iTrack!=-1)
            {
              dPtOverPt = fabs((Track_pt->at(iTrack) - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle));
            }
            else
            {
              if(numberOfTrack!=0)
              {
                std::cout << "Found iTrack=-1 - numberOfTrack = " << numberOfTrack << "\n";
                std::cout << "Genparticle: pt = " << Genp_pt->at(iGenParticle) << ", eta = " << Genp_eta->at(iGenParticle) << std::endl;
              }
            }
            vParticlesFromB.push_back(iGenParticle);
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
                Fill(hTrackDeltaRFromGenp,iTrackWithDistance.second,binningVariableValue,vLowBins);
                Fill(hTrackDeltaPtOverPtFromGenp,dPtOverPt,binningVariableValue,vLowBins);
                hTrackDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                if(Genp_charge->at(iGenParticle) > 0)
                {
                  Fill(hTrackPlusDeltaRFromGenp,iTrackWithDistance.second,binningVariableValue,vLowBins);
                  Fill(hTrackPlusDeltaPtOverPtFromGenp,dPtOverPt,binningVariableValue,vLowBins);
                  hTrackPlusDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                }
                else
                {
                  Fill(hTrackMinusDeltaRFromGenp,iTrackWithDistance.second,binningVariableValue,vLowBins);
                  Fill(hTrackMinusDeltaPtOverPtFromGenp,dPtOverPt,binningVariableValue,vLowBins);
                  hTrackMinusDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                }
                
//                 if(iTrackWithDistance.second < 0.1 && dPtOverPt < 0.05)
                if(iTrackWithDistance.second < 0.1)
                {
                  Fill(hAssocTkGenParticleRecoPt,Track_pt->at(iTrack),binningVariableValue,vLowBins);
                  Fill(hAssocTkGenParticleRecoEta,Track_eta->at(iTrack),binningVariableValue,vLowBins);
                  Fill(hTrackDeltaRFromAssocGenp, iTrackWithDistance.second,binningVariableValue,vLowBins);
                  Fill(hTrackDeltaPtOverPtFromAssocGenp,dPtOverPt,binningVariableValue,vLowBins);
                  hTrackDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                  hAssocTkGenParticleRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
                  if(Genp_charge->at(iGenParticle) > 0)
                  {
                    Fill(hTrackPlusDeltaRFromAssocGenp, iTrackWithDistance.second,binningVariableValue,vLowBins);
                    Fill(hTrackPlusDeltaPtOverPtFromAssocGenp,dPtOverPt,binningVariableValue,vLowBins);
                    hTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                    Fill(hAssocTkGenParticlePlusRecoPt,Track_pt->at(iTrack),binningVariableValue,vLowBins);
                    Fill(hAssocTkGenParticlePlusRecoEta,Track_eta->at(iTrack),binningVariableValue,vLowBins);
                    hAssocTkGenParticlePlusRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
                  }
                  else
                  {
                    Fill(hTrackMinusDeltaRFromAssocGenp, iTrackWithDistance.second,binningVariableValue,vLowBins);
                    Fill(hTrackMinusDeltaPtOverPtFromAssocGenp,dPtOverPt,binningVariableValue,vLowBins);
                    hTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
                    Fill(hAssocTkGenParticleMinusRecoPt,Track_pt->at(iTrack),binningVariableValue,vLowBins);
                    Fill(hAssocTkGenParticleMinusRecoEta,Track_eta->at(iTrack),binningVariableValue,vLowBins);
                    hAssocTkGenParticleMinusRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
                  }
                }
              }
            }
            
            double decayLength = -1;
            int iDaughter = -1;
            
            if(Genp_vDau->at(iGenParticle).size()>0)
            {
              TVector3 productionPoint(Genp_vx->at(iGenParticle),Genp_vy->at(iGenParticle),Genp_vz->at(iGenParticle));
              iDaughter = Genp_vDau->at(iGenParticle).at(0);
              TVector3 daughterProductionPoint(Genp_vx->at(iDaughter),Genp_vy->at(iDaughter),Genp_vz->at(iDaughter));
              decayLength = (daughterProductionPoint - productionPoint).Mag();
              vDecayLengths.push_back(decayLength);
              vDecayZ.push_back(Genp_vz->at(iDaughter));
              vDecayR.push_back(sqrt(Genp_vx->at(iDaughter)*Genp_vx->at(iDaughter) + Genp_vy->at(iDaughter)*Genp_vy->at(iDaughter)));
              if(Genp_charge->at(iGenParticle) > 0)
              {
                vDecayLengthsPlus.push_back(decayLength);
                vDecayZPlus.push_back(Genp_vz->at(iDaughter));
                vDecayRPlus.push_back(sqrt(Genp_vx->at(iDaughter)*Genp_vx->at(iDaughter) + Genp_vy->at(iDaughter)*Genp_vy->at(iDaughter)));
              }
              else
              {
                vDecayLengthsMinus.push_back(decayLength);
                vDecayZMinus.push_back(Genp_vz->at(iDaughter));
                vDecayRMinus.push_back(sqrt(Genp_vx->at(iDaughter)*Genp_vx->at(iDaughter) + Genp_vy->at(iDaughter)*Genp_vy->at(iDaughter)));
              }
            }
            else
            {
              vDecayLengths.push_back(9999);
              vDecayZ.push_back(9999);
              vDecayR.push_back(9999);
              if(Genp_charge->at(iGenParticle) > 0)
              {
                vDecayLengthsPlus.push_back(9999);
                vDecayZPlus.push_back(9999);
                vDecayRPlus.push_back(9999);
              }
              else
            {
              vDecayLengthsMinus.push_back(9999);
              vDecayZMinus.push_back(9999);
              vDecayRMinus.push_back(9999);
            }
            }
            
            
            if(Genp_pt->at(iGenParticle) > 4 && fabs(Genp_eta->at(iGenParticle)) < 2.4)
            {
              vParticlesInAccFromB.push_back(iGenParticle);
              Fill(hAllGenParticleGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
              Fill(hAllGenParticleGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
              hAllGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
              Fill(hAllGenParticleNDaughters,Genp_vDau->at(iGenParticle).size(),binningVariableValue,vLowBins);
              if(Genp_charge->at(iGenParticle) > 0)
              {
                vParticlesPlusInAccFromB.push_back(iGenParticle);
                Fill(hAllGenParticlePlusGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
                Fill(hAllGenParticlePlusGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
                hAllGenParticlePlusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                Fill(hAllGenParticlePlusNDaughters,Genp_vDau->at(iGenParticle).size(),binningVariableValue,vLowBins);
              }
              else
              {
                vParticlesMinusInAccFromB.push_back(iGenParticle);
                Fill(hAllGenParticleMinusGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
                Fill(hAllGenParticleMinusGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
                hAllGenParticleMinusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                Fill(hAllGenParticleMinusNDaughters,Genp_vDau->at(iGenParticle).size(),binningVariableValue,vLowBins);
              }
              
//               if(iTrackWithDistance.second < 0.1 && dPtOverPt < 0.05)
              if(iTrackWithDistance.second < 0.1)
              {
                Fill(hAssocTkGenParticleGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
                Fill(hAssocTkGenParticleGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
                hAssocTkGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                if(Genp_charge->at(iGenParticle) > 0)
                {
                  Fill(hAssocTkGenParticlePlusGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
                  Fill(hAssocTkGenParticlePlusGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
                  hAssocTkGenParticlePlusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                }
                else
                {
                  Fill(hAssocTkGenParticleMinusGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
                  Fill(hAssocTkGenParticleMinusGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
                  hAssocTkGenParticleMinusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                }
              }
              
              if(Genp_vDau->at(iGenParticle).size()>0)
              {
                Fill(hAllGenParticleDecayLength,decayLength,binningVariableValue,vLowBins);
                hAllGenParticleDecayVertexPosition->Fill(Genp_vz->at(iDaughter),sqrt(Genp_vx->at(iDaughter)*Genp_vx->at(iDaughter) + Genp_vy->at(iDaughter)*Genp_vy->at(iDaughter)));
                if(Genp_charge->at(iGenParticle) > 0)
                {
                  Fill(hAllGenParticlePlusDecayLength,decayLength,binningVariableValue,vLowBins);
                  hAllGenParticlePlusDecayVertexPosition->Fill(Genp_vz->at(iDaughter),sqrt(Genp_vx->at(iDaughter)*Genp_vx->at(iDaughter) + Genp_vy->at(iDaughter)*Genp_vy->at(iDaughter)));
                }
                else
                {
                  Fill(hAllGenParticleMinusDecayLength,decayLength,binningVariableValue,vLowBins);
                  hAllGenParticleMinusDecayVertexPosition->Fill(Genp_vz->at(iDaughter),sqrt(Genp_vx->at(iDaughter)*Genp_vx->at(iDaughter) + Genp_vy->at(iDaughter)*Genp_vy->at(iDaughter)));
                }
              }
            }
          }
        }
//         if(abs(genpId) == 531)
//         {
//           std::cout << "Found a B_s!\n";
//           std::cout << "Printing daughters:\n";
//           std::vector<unsigned short> vDaus = Genp_vDau->at(iGenParticle);
//           for(unsigned int iDau = 0; iDau < vDaus.size(); iDau++)
//           {
//             unsigned int index = vDaus[iDau];
//             std::cout << "index = " << index << ", id = " << Genp_Id->at(index) << std::endl;  
//           }
//         }
      }
    }

//     if(vParticlesFromB.size() < 2)
//     {
//       std::cout << "E R R O R ! Size of vector of B true daughters is less than 2!\n";
//       std::cout << "            Size is " << vParticlesFromB.size() << ".\n";
//       std::cout << "            Exiting...\n";
//       exit(1);
//     }
    std::vector<double> binningVariables;
    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      if(s->SelectMuon(iMuon) && fabs(Muon_eta->at(iMuon))<2.4)
      {
        short int trackRef = Muon_trackref->at(iMuon);
        binningVariables = GetBinningVariablesValues(iMuon);
        for(unsigned int iParticleFromB = 0; iParticleFromB < vParticlesFromB.size(); iParticleFromB++)
//         for(unsigned int iParticleFromB = 0; iParticleFromB < vParticlesInAccFromB.size(); iParticleFromB++)
        {
          unsigned short iGenParticle = vParticlesFromB[iParticleFromB];
//           unsigned short iGenParticle = vParticlesInAccFromB[iParticleFromB];
//           std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticle(iGenParticle);
          std::pair<short int, double> iTrackWithDistance = FindTrackNearestToGenParticleWithPtThreshold(iGenParticle, 0.0);
          short int iTrack = iTrackWithDistance.first;
          
          if(iTrack == trackRef /*&& iTrackWithDistance.second < 0.1*/)
          {
            double dPtOverPt = fabs((Track_pt->at(trackRef) - Genp_pt->at(iGenParticle)) / Genp_pt->at(iGenParticle));
            Fill(hSelMuonDeltaRFromGenp,iTrackWithDistance.second,binningVariables[binningVariableIndex],vLowBins);
            Fill(hSelMuonDeltaPtOverPtFromGenp,dPtOverPt,binningVariables[binningVariableIndex],vLowBins);
            hSelMuonDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
            if(Genp_charge->at(iGenParticle) > 0)
            {
              Fill(hSelMuonPlusDeltaRFromGenp,iTrackWithDistance.second,binningVariables[binningVariableIndex],vLowBins);
              Fill(hSelMuonPlusDeltaPtOverPtFromGenp,dPtOverPt,binningVariables[binningVariableIndex],vLowBins);
              hSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
            }
            else
            {
              Fill(hSelMuonMinusDeltaRFromGenp,iTrackWithDistance.second,binningVariables[binningVariableIndex],vLowBins);
              Fill(hSelMuonMinusDeltaPtOverPtFromGenp,dPtOverPt,binningVariables[binningVariableIndex],vLowBins);
              hSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp->Fill(dPtOverPt,iTrackWithDistance.second);
            }
//             if(iTrackWithDistance.second < 0.1 && dPtOverPt < 0.05)
            if(iTrackWithDistance.second < 0.1)
            {
//               std::cout << "Inner muon track is associated to a gen particle of Id = " << Genp_Id->at(iGenParticle) << "!\n";
              double binningVariableValue = GetGenBinningVariablesValues(iGenParticle).at(binningVariableIndex);
              vSelectedMuons.push_back(iMuon);
              Fill(hSelMuonDeltaRFromAssocGenp,iTrackWithDistance.second,binningVariableValue,vLowBins);
              Fill(hSelMuonDeltaPtOverPtFromAssocGenp,dPtOverPt,binningVariableValue,vLowBins);
              hSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
              if(Genp_charge->at(iGenParticle) > 0)
              {
                Fill(hSelMuonPlusDeltaRFromAssocGenp,iTrackWithDistance.second,binningVariableValue,vLowBins);
                Fill(hSelMuonPlusDeltaPtOverPtFromAssocGenp,dPtOverPt,binningVariableValue,vLowBins);
                hSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
              }
              else
              {
                Fill(hSelMuonMinusDeltaRFromAssocGenp,iTrackWithDistance.second,binningVariableValue,vLowBins);
                Fill(hSelMuonMinusDeltaPtOverPtFromAssocGenp,dPtOverPt,binningVariableValue,vLowBins);
                hSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp->Fill(dPtOverPt,iTrackWithDistance.second);
              }
              if(Genp_pt->at(iGenParticle) > 4 && fabs(Genp_eta->at(iGenParticle)) < 2.4)
              {
                Fill(hAssocGenParticleGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
                Fill(hAssocGenParticleGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
                hAssocGenParticleGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                Fill(hAssocGenParticleNDaughters,Genp_vDau->at(iGenParticle).size(),binningVariableValue,vLowBins);
                Fill(hAssocGenParticleDecayLength,vDecayLengths[iParticleFromB],binningVariableValue,vLowBins);
                hAssocGenParticleDecayVertexPosition->Fill(vDecayZ[iParticleFromB],vDecayR[iParticleFromB]);
              
                if(Genp_charge->at(iGenParticle) > 0)
                {
                  Fill(hAssocGenParticlePlusGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
                  Fill(hAssocGenParticlePlusGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
                  hAssocGenParticlePlusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                  Fill(hAssocGenParticlePlusNDaughters,Genp_vDau->at(iGenParticle).size(),binningVariableValue,vLowBins);
                  Fill(hAssocGenParticlePlusDecayLength,vDecayLengths[iParticleFromB],binningVariableValue,vLowBins);
                  hAssocGenParticlePlusDecayVertexPosition->Fill(vDecayZ[iParticleFromB],vDecayR[iParticleFromB]);
                }
                else
                {
                  Fill(hAssocGenParticleMinusGenPt,Genp_pt->at(iGenParticle),binningVariableValue,vLowBins);
                  Fill(hAssocGenParticleMinusGenEta,Genp_eta->at(iGenParticle),binningVariableValue,vLowBins);
                  hAssocGenParticleMinusGenPtVsEta->Fill(Genp_eta->at(iGenParticle),Genp_pt->at(iGenParticle));
                  Fill(hAssocGenParticleMinusNDaughters,Genp_vDau->at(iGenParticle).size(),binningVariableValue,vLowBins);
                  Fill(hAssocGenParticleMinusDecayLength,vDecayLengths[iParticleFromB],binningVariableValue,vLowBins);
                  hAssocGenParticleMinusDecayVertexPosition->Fill(vDecayZ[iParticleFromB],vDecayR[iParticleFromB]);
                }
              }
              Fill(hAssocGenParticleRecoPt,Track_pt->at(iTrack),binningVariableValue,vLowBins);
              Fill(hAssocGenParticleRecoEta,Track_eta->at(iTrack),binningVariableValue,vLowBins);
              hAssocGenParticleRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
              if(Genp_charge->at(iGenParticle) > 0)
              {
                Fill(hAssocGenParticlePlusRecoPt,Track_pt->at(iTrack),binningVariableValue,vLowBins);
                Fill(hAssocGenParticlePlusRecoEta,Track_eta->at(iTrack),binningVariableValue,vLowBins);
                hAssocGenParticlePlusRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
              }
              else
              {
                Fill(hAssocGenParticleMinusRecoPt,Track_pt->at(iTrack),binningVariableValue,vLowBins);
                Fill(hAssocGenParticleMinusRecoEta,Track_eta->at(iTrack),binningVariableValue,vLowBins);
                hAssocGenParticleMinusRecoPtVsEta->Fill(Track_eta->at(iTrack),Track_pt->at(iTrack));
              }
//               std::cout << "Genparticle " << iGenParticle << " has " << Genp_vDau->at(iGenParticle).size() << " daughters.\n";    
//               if(Genp_vDau->at(iGenParticle).size() > 2)
//                 RecursivePrintDaughters(iGenParticle);
              break;
            }
          }
        }
      }
    }
    
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
        Fill(hRecoMuonTrackValidFraction,GetTrackValidFraction(iTrack), binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackExpOuterHits,GetNumberOfLostTrackerOuterHits(iTrack), binningVariableValue,vLowBins);
      }
      else
      {
        Fill(hRecoMuonTrackValidHits,0, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackChi2,9999, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackNdof,0, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackLayersWithMeasurement,0, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackValidPixelHits,0, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackValidFraction,0, binningVariableValue,vLowBins);
        Fill(hRecoMuonTrackExpOuterHits,0, binningVariableValue,vLowBins);
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
      
//       int charge = Muon_charge->at(iMuon);
      
      TVector3 muonP;
      muonP.SetPtEtaPhi(Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon));
      double muonPMag = muonP.Mag();
      muonP.SetMag(1);
      muonP.RotateZ(atan(muonP.X()/muonP.Y()));
      double muonPDyDz = muonP.Y()/muonP.Z();
      
      TVector3 muonMu1State, muonMu2State;
      muonMu1State.SetXYZ(Muon_Mu1State_x->at(iMuon),Muon_Mu1State_y->at(iMuon),Muon_Mu1State_z->at(iMuon));
      muonMu2State.SetXYZ(Muon_Mu2State_x->at(iMuon),Muon_Mu2State_y->at(iMuon),Muon_Mu2State_z->at(iMuon));
      muonMu1State.SetMag(1);
      muonMu2State.SetMag(1);
      muonMu1State.RotateZ(atan(muonMu1State.X()/muonMu1State.Y()));
      muonMu2State.RotateZ(atan(muonMu2State.X()/muonMu2State.Y()));
//       double muonMu1StateDyDz = muonMu1State.Y()/muonMu1State.Z();
//       double muonMu2StateDyDz = muonMu2State.Y()/muonMu2State.Z();
      
      double minDDxDz = 9999999;
      double minDDyDz = 9999999;
      double minDx = 9999999;
      double minDy = 9999999;
      double minPullDxDz = 9999999;
      double minPullDyDz = 9999999;
      double minPullX = 9999999;
      double minPullY = 9999999;
      double minAngleWrtP = 9999999;
      double maxDDxDz = 9999999;
      double maxDDyDz = 9999999;
      double maxDx = 9999999;
      double maxDy = 9999999;
      double maxPullDxDz = 9999999;
      double maxPullDyDz = 9999999;
      double maxPullX = 9999999;
      double maxPullY = 9999999;
      double maxAngleWrtP = 9999999;
      double avgDDxDz = 0;
      double avgDDyDz = 0;
      double avgDx = 0;
      double avgDy = 0;
      double avgPullDxDz = 0;
      double avgPullDyDz = 0;
      double avgPullX = 0;
      double avgPullY = 0;
      double avgAngleWrtP = 0;
      double medianDDxDz = 9999999;
      double medianDDyDz = 9999999;
      double medianDx = 9999999;
      double medianDy = 9999999;
      double medianPullDxDz = 9999999;
      double medianPullDyDz = 9999999;
      double medianPullX = 9999999;
      double medianPullY = 9999999;
      double medianAngleWrtP = 9999999;
      double stddevDDxDz = 0;
      double stddevDDyDz = 0;
      double stddevDx = 0;
      double stddevDy = 0;
      double stddevPullDxDz = 0;
      double stddevPullDyDz = 0;
      double stddevPullX = 0;
      double stddevPullY = 0;
      double stddevAngleWrtP = 0;
      double minAbsDDxDz = 9999999;
      double minAbsDDyDz = 9999999;
      double minAbsDx = 9999999;
      double minAbsDy = 9999999;
      double minAbsPullDxDz = 9999999;
      double minAbsPullDyDz = 9999999;
      double minAbsPullX = 9999999;
      double minAbsPullY = 9999999;
      double minAbsAngleWrtP = 9999999;
      double maxAbsDDxDz = 9999999;
      double maxAbsDDyDz = 9999999;
      double maxAbsDx = 9999999;
      double maxAbsDy = 9999999;
      double maxAbsPullDxDz = 9999999;
      double maxAbsPullDyDz = 9999999;
      double maxAbsPullX = 9999999;
      double maxAbsPullY = 9999999;
      double maxAbsAngleWrtP = 9999999;
      double avgAbsDDxDz = 0;
      double avgAbsDDyDz = 0;
      double avgAbsDx = 0;
      double avgAbsDy = 0;
      double avgAbsPullDxDz = 0;
      double avgAbsPullDyDz = 0;
      double avgAbsPullX = 0;
      double avgAbsPullY = 0;
      double avgAbsAngleWrtP = 0;
      double medianAbsDDxDz = 9999999;
      double medianAbsDDyDz = 9999999;
      double medianAbsDx = 9999999;
      double medianAbsDy = 9999999;
      double medianAbsPullDxDz = 9999999;
      double medianAbsPullDyDz = 9999999;
      double medianAbsPullX = 9999999;
      double medianAbsPullY = 9999999;
      double medianAbsAngleWrtP = 9999999;
      double stddevAbsDDxDz = 0;
      double stddevAbsDDyDz = 0;
      double stddevAbsDx = 0;
      double stddevAbsDy = 0;
      double stddevAbsPullDxDz = 0;
      double stddevAbsPullDyDz = 0;
      double stddevAbsPullX = 0;
      double stddevAbsPullY = 0;
      double stddevAbsAngleWrtP = 0;
      std::vector<double> vDDxDz;
      std::vector<double> vDDyDz;
      std::vector<double> vDx;
      std::vector<double> vDy;
      std::vector<double> vPullDxDz;
      std::vector<double> vPullDyDz;
      std::vector<double> vPullX;
      std::vector<double> vPullY;
      std::vector<double> vAngleWrtP;
      std::vector<double> vAngleWrtPAllStations;
      // Hardcoded function: tries to factor out the dependence from momentum of the DTheta distribution
      double factor = exp(-3.15511-1.21642e-1+muonPMag)+4.40649e-2;
      
      for(unsigned int iStation = 0; iStation < 8; iStation++)
      {
        if(Muon_dDxDz->at(iMuon).at(iStation) < 999998)
        {
          vDDxDz.push_back(Muon_dDxDz->at(iMuon).at(iStation));
          Fill(hRecoMuonDDxDz, Muon_dDxDz->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsDDxDz, fabs(Muon_dDxDz->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_dDyDz->at(iMuon).at(iStation) < 999998)
        {
          vDDyDz.push_back(Muon_dDyDz->at(iMuon).at(iStation));
          Fill(hRecoMuonDDyDz, Muon_dDyDz->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsDDyDz, fabs(Muon_dDyDz->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_Dx->at(iMuon).at(iStation) < 999998)
        {
          vDx.push_back(Muon_Dx->at(iMuon).at(iStation));
          Fill(hRecoMuonDx, Muon_Dx->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsDx, fabs(Muon_Dx->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_Dy->at(iMuon).at(iStation) < 999998)
        {
          vDy.push_back(Muon_Dy->at(iMuon).at(iStation));
          Fill(hRecoMuonDy, Muon_Dy->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsDy, fabs(Muon_Dy->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_pullDxDz->at(iMuon).at(iStation) < 999998)
        {
          vPullDxDz.push_back(Muon_pullDxDz->at(iMuon).at(iStation));
          Fill(hRecoMuonPullDxDz, Muon_pullDxDz->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsPullDxDz, fabs(Muon_pullDxDz->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_pullDyDz->at(iMuon).at(iStation) < 999998)
        {
          vPullDyDz.push_back(Muon_pullDyDz->at(iMuon).at(iStation));
          Fill(hRecoMuonPullDyDz, Muon_pullDyDz->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsPullDyDz, fabs(Muon_pullDyDz->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_pullX->at(iMuon).at(iStation) < 999998)
        {
          vPullX.push_back(Muon_pullX->at(iMuon).at(iStation));
          Fill(hRecoMuonPullX, Muon_pullX->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsPullX, fabs(Muon_pullX->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_pullY->at(iMuon).at(iStation) < 999998)
        {
          vPullY.push_back(Muon_pullY->at(iMuon).at(iStation));
          Fill(hRecoMuonPullY, Muon_pullY->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsPullY, fabs(Muon_pullY->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_segmentDxDz->at(iMuon).at(iStation) < 999998 && Muon_segmentDyDz->at(iMuon).at(iStation) < 999998)
        {
          TVector3 segmentXYZ;
          if(iStation < 4)
            segmentXYZ.SetXYZ(Muon_segmentDxDz->at(iMuon).at(iStation),1,-Muon_segmentDyDz->at(iMuon).at(iStation));
          else
            segmentXYZ.SetXYZ(Muon_segmentDxDz->at(iMuon).at(iStation),-Muon_segmentDyDz->at(iMuon).at(iStation),1);
          segmentXYZ.SetMag(1);
          segmentXYZ.RotateZ(atan(segmentXYZ.X()/segmentXYZ.Y()));
          double segmentXYZDyDz = segmentXYZ.Y()/segmentXYZ.Z();
          double angleWrtP = fabs(atan(segmentXYZDyDz)) - fabs(atan(muonPDyDz));
          vAngleWrtP.push_back(angleWrtP/factor);
          vAngleWrtPAllStations.push_back(angleWrtP/factor);
          Fill(hRecoMuonDThetaWrtStraight,angleWrtP/factor,binningVariableValue,vLowBins);
          Fill(hRecoMuonAbsDThetaWrtStraight,fabs(angleWrtP/factor),binningVariableValue,vLowBins);
          hRecoMuonDThetaWrtStraightVsP->Fill(muonPMag,angleWrtP);
        }
        else
        {
          vAngleWrtPAllStations.push_back(999999);
        }
      }
      if(vAngleWrtP.size() == 0)
      {
//         std::cout << "W A R N I N G ! No segments with full angular info found for this muon!\n";
//         std::cout << "                Filling the DeltaAngle vector with zero.\n";
        vAngleWrtP.push_back(0);
      }
      FindMinMaxAvgMedianStddev(&vDDxDz, minDDxDz, maxDDxDz, avgDDxDz, medianDDxDz, stddevDDxDz);
      FindMinMaxAvgMedianStddev(&vDDyDz, minDDyDz, maxDDyDz, avgDDyDz, medianDDyDz, stddevDDyDz);
      FindMinMaxAvgMedianStddev(&vDx, minDx, maxDx, avgDx, medianDx, stddevDx);
      FindMinMaxAvgMedianStddev(&vDy, minDy, maxDy, avgDy, medianDy, stddevDy);
      FindMinMaxAvgMedianStddev(&vPullDxDz, minPullDxDz, maxPullDxDz, avgPullDxDz, medianPullDxDz, stddevPullDxDz);
      FindMinMaxAvgMedianStddev(&vPullDyDz, minPullDyDz, maxPullDyDz, avgPullDyDz, medianPullDyDz, stddevPullDyDz);
      FindMinMaxAvgMedianStddev(&vPullX, minPullX, maxPullX, avgPullX, medianPullX, stddevPullX);
      FindMinMaxAvgMedianStddev(&vPullY, minPullY, maxPullY, avgPullY, medianPullY, stddevPullY);
      FindMinMaxAvgMedianStddev(&vAngleWrtP, minAngleWrtP, maxAngleWrtP, avgAngleWrtP, medianAngleWrtP, stddevAngleWrtP);
      
      FindAbsMinMaxAvgMedianStddev(&vDDxDz, minAbsDDxDz, maxAbsDDxDz, avgAbsDDxDz, medianAbsDDxDz, stddevAbsDDxDz);
      FindAbsMinMaxAvgMedianStddev(&vDDyDz, minAbsDDyDz, maxAbsDDyDz, avgAbsDDyDz, medianAbsDDyDz, stddevAbsDDyDz);
      FindAbsMinMaxAvgMedianStddev(&vDx, minAbsDx, maxAbsDx, avgAbsDx, medianAbsDx, stddevAbsDx);
      FindAbsMinMaxAvgMedianStddev(&vDy, minAbsDy, maxAbsDy, avgAbsDy, medianAbsDy, stddevAbsDy);
      FindAbsMinMaxAvgMedianStddev(&vPullDxDz, minAbsPullDxDz, maxAbsPullDxDz, avgAbsPullDxDz, medianAbsPullDxDz, stddevAbsPullDxDz);
      FindAbsMinMaxAvgMedianStddev(&vPullDyDz, minAbsPullDyDz, maxAbsPullDyDz, avgAbsPullDyDz, medianAbsPullDyDz, stddevAbsPullDyDz);
      FindAbsMinMaxAvgMedianStddev(&vPullX, minAbsPullX, maxAbsPullX, avgAbsPullX, medianAbsPullX, stddevAbsPullX);
      FindAbsMinMaxAvgMedianStddev(&vPullY, minAbsPullY, maxAbsPullY, avgAbsPullY, medianAbsPullY, stddevAbsPullY);
      FindAbsMinMaxAvgMedianStddev(&vAngleWrtP, minAbsAngleWrtP, maxAbsAngleWrtP, avgAbsAngleWrtP, medianAbsAngleWrtP, stddevAbsAngleWrtP);
      
      Fill(hRecoMuonDDxDzInDTStation1, Muon_dDxDz->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDxDzInDTStation2, Muon_dDxDz->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDxDzInDTStation3, Muon_dDxDz->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDxDzInDTStation4, Muon_dDxDz->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDxDzInCSCStation1, Muon_dDxDz->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDxDzInCSCStation2, Muon_dDxDz->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDxDzInCSCStation3, Muon_dDxDz->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDxDzInCSCStation4, Muon_dDxDz->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hRecoMuonMinDDxDz, minDDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxDDxDz, maxDDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgDDxDz, avgDDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianDDxDz, medianDDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevDDxDz, stddevDDxDz, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevDDxDz->Fill(stddevDDxDz, avgDDxDz);
      hRecoMuonMedianVsStddevDDxDz->Fill(stddevDDxDz, medianDDxDz);
      Fill(hRecoMuonMinAbsDDxDz, minAbsDDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsDDxDz, maxAbsDDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsDDxDz, avgAbsDDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsDDxDz, medianAbsDDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsDDxDz, stddevAbsDDxDz, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsDDxDz->Fill(stddevAbsDDxDz, avgAbsDDxDz);
      hRecoMuonMedianVsStddevAbsDDxDz->Fill(stddevAbsDDxDz, medianAbsDDxDz);
      
      Fill(hRecoMuonDDyDzInDTStation1, Muon_dDyDz->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDyDzInDTStation2, Muon_dDyDz->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDyDzInDTStation3, Muon_dDyDz->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDyDzInDTStation4, Muon_dDyDz->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDyDzInCSCStation1, Muon_dDyDz->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDyDzInCSCStation2, Muon_dDyDz->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDyDzInCSCStation3, Muon_dDyDz->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hRecoMuonDDyDzInCSCStation4, Muon_dDyDz->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hRecoMuonMinDDyDz, minDDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxDDyDz, maxDDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgDDyDz, avgDDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianDDyDz, medianDDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevDDyDz, stddevDDyDz, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevDDyDz->Fill(stddevDDyDz, avgDDyDz);
      hRecoMuonMedianVsStddevDDyDz->Fill(stddevDDyDz, medianDDyDz);
      Fill(hRecoMuonMinAbsDDyDz, minAbsDDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsDDyDz, maxAbsDDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsDDyDz, avgAbsDDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsDDyDz, medianAbsDDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsDDyDz, stddevAbsDDyDz, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsDDyDz->Fill(stddevAbsDDyDz, avgAbsDDyDz);
      hRecoMuonMedianVsStddevAbsDDyDz->Fill(stddevAbsDDyDz, medianAbsDDyDz);
      
      Fill(hRecoMuonDxInDTStation1, Muon_Dx->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonDxInDTStation2, Muon_Dx->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonDxInDTStation3, Muon_Dx->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonDxInDTStation4, Muon_Dx->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonDxInCSCStation1, Muon_Dx->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hRecoMuonDxInCSCStation2, Muon_Dx->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hRecoMuonDxInCSCStation3, Muon_Dx->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hRecoMuonDxInCSCStation4, Muon_Dx->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hRecoMuonMinDx, minDx, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxDx, maxDx, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgDx, avgDx, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianDx, medianDx, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevDx, stddevDx, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevDx->Fill(stddevDx, avgDx);
      hRecoMuonMedianVsStddevDx->Fill(stddevDx, medianDx);
      Fill(hRecoMuonMinAbsDx, minAbsDx, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsDx, maxAbsDx, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsDx, avgAbsDx, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsDx, medianAbsDx, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsDx, stddevAbsDx, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsDx->Fill(stddevAbsDx, avgAbsDx);
      hRecoMuonMedianVsStddevAbsDx->Fill(stddevAbsDx, medianAbsDx);
      
      Fill(hRecoMuonDyInDTStation1, Muon_Dy->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonDyInDTStation2, Muon_Dy->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonDyInDTStation3, Muon_Dy->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonDyInDTStation4, Muon_Dy->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonDyInCSCStation1, Muon_Dy->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hRecoMuonDyInCSCStation2, Muon_Dy->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hRecoMuonDyInCSCStation3, Muon_Dy->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hRecoMuonDyInCSCStation4, Muon_Dy->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hRecoMuonMinDy, minDy, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxDy, maxDy, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgDy, avgDy, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianDy, medianDy, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevDy, stddevDy, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevDy->Fill(stddevDy, avgDy);
      hRecoMuonMedianVsStddevDy->Fill(stddevDy, medianDy);
      Fill(hRecoMuonMinAbsDy, minAbsDy, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsDy, maxAbsDy, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsDy, avgAbsDy, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsDy, medianAbsDy, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsDy, stddevAbsDy, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsDy->Fill(stddevAbsDy, avgAbsDy);
      hRecoMuonMedianVsStddevAbsDy->Fill(stddevAbsDy, medianAbsDy);
      
      Fill(hRecoMuonPullDxDzInDTStation1, Muon_pullDxDz->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDxDzInDTStation2, Muon_pullDxDz->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDxDzInDTStation3, Muon_pullDxDz->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDxDzInDTStation4, Muon_pullDxDz->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDxDzInCSCStation1, Muon_pullDxDz->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDxDzInCSCStation2, Muon_pullDxDz->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDxDzInCSCStation3, Muon_pullDxDz->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDxDzInCSCStation4, Muon_pullDxDz->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hRecoMuonMinPullDxDz, minPullDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxPullDxDz, maxPullDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgPullDxDz, avgPullDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianPullDxDz, medianPullDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevPullDxDz, stddevPullDxDz, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevPullDxDz->Fill(stddevPullDxDz, avgPullDxDz);
      hRecoMuonMedianVsStddevPullDxDz->Fill(stddevPullDxDz, medianPullDxDz);
      Fill(hRecoMuonMinAbsPullDxDz, minAbsPullDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsPullDxDz, maxAbsPullDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsPullDxDz, avgAbsPullDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsPullDxDz, medianAbsPullDxDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsPullDxDz, stddevAbsPullDxDz, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsPullDxDz->Fill(stddevAbsPullDxDz, avgAbsPullDxDz);
      hRecoMuonMedianVsStddevAbsPullDxDz->Fill(stddevAbsPullDxDz, medianAbsPullDxDz);
      
      Fill(hRecoMuonPullDyDzInDTStation1, Muon_pullDyDz->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDyDzInDTStation2, Muon_pullDyDz->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDyDzInDTStation3, Muon_pullDyDz->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDyDzInDTStation4, Muon_pullDyDz->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDyDzInCSCStation1, Muon_pullDyDz->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDyDzInCSCStation2, Muon_pullDyDz->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDyDzInCSCStation3, Muon_pullDyDz->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullDyDzInCSCStation4, Muon_pullDyDz->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hRecoMuonMinPullDyDz, minPullDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxPullDyDz, maxPullDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgPullDyDz, avgPullDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianPullDyDz, medianPullDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevPullDyDz, stddevPullDyDz, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevPullDyDz->Fill(stddevPullDyDz, avgPullDyDz);
      hRecoMuonMedianVsStddevPullDyDz->Fill(stddevPullDyDz, medianPullDyDz);
      Fill(hRecoMuonMinAbsPullDyDz, minAbsPullDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsPullDyDz, maxAbsPullDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsPullDyDz, avgAbsPullDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsPullDyDz, medianAbsPullDyDz, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsPullDyDz, stddevAbsPullDyDz, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsPullDyDz->Fill(stddevAbsPullDyDz, avgAbsPullDyDz);
      hRecoMuonMedianVsStddevAbsPullDyDz->Fill(stddevAbsPullDyDz, medianAbsPullDyDz);
      
      Fill(hRecoMuonPullXInDTStation1, Muon_pullX->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullXInDTStation2, Muon_pullX->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullXInDTStation3, Muon_pullX->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullXInDTStation4, Muon_pullX->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullXInCSCStation1, Muon_pullX->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullXInCSCStation2, Muon_pullX->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullXInCSCStation3, Muon_pullX->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullXInCSCStation4, Muon_pullX->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hRecoMuonMinPullX, minPullX, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxPullX, maxPullX, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgPullX, avgPullX, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianPullX, medianPullX, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevPullX, stddevPullX, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevPullX->Fill(stddevPullX, avgPullX);
      hRecoMuonMedianVsStddevPullX->Fill(stddevPullX, medianPullX);
      Fill(hRecoMuonMinAbsPullX, minAbsPullX, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsPullX, maxAbsPullX, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsPullX, avgAbsPullX, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsPullX, medianAbsPullX, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsPullX, stddevAbsPullX, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsPullX->Fill(stddevAbsPullX, avgAbsPullX);
      hRecoMuonMedianVsStddevAbsPullX->Fill(stddevAbsPullX, medianAbsPullX);
      
      Fill(hRecoMuonPullYInDTStation1, Muon_pullY->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullYInDTStation2, Muon_pullY->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullYInDTStation3, Muon_pullY->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullYInDTStation4, Muon_pullY->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullYInCSCStation1, Muon_pullY->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullYInCSCStation2, Muon_pullY->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullYInCSCStation3, Muon_pullY->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hRecoMuonPullYInCSCStation4, Muon_pullY->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hRecoMuonMinPullY, minPullY, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxPullY, maxPullY, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgPullY, avgPullY, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianPullY, medianPullY, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevPullY, stddevPullY, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevPullY->Fill(stddevPullY, avgPullY);
      hRecoMuonMedianVsStddevPullY->Fill(stddevPullY, medianPullY);
      Fill(hRecoMuonMinAbsPullY, minAbsPullY, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsPullY, maxAbsPullY, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsPullY, avgAbsPullY, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsPullY, medianAbsPullY, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsPullY, stddevAbsPullY, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsPullY->Fill(stddevAbsPullY, avgAbsPullY);
      hRecoMuonMedianVsStddevAbsPullY->Fill(stddevAbsPullY, medianAbsPullY);
            
      Fill(hRecoMuonDThetaWrtStraightInDTStation1, vAngleWrtPAllStations[0], binningVariableValue,vLowBins);
      Fill(hRecoMuonDThetaWrtStraightInDTStation2, vAngleWrtPAllStations[1], binningVariableValue,vLowBins);
      Fill(hRecoMuonDThetaWrtStraightInDTStation3, vAngleWrtPAllStations[2], binningVariableValue,vLowBins);
      Fill(hRecoMuonDThetaWrtStraightInDTStation4, vAngleWrtPAllStations[3], binningVariableValue,vLowBins);
      Fill(hRecoMuonDThetaWrtStraightInCSCStation1, vAngleWrtPAllStations[4], binningVariableValue,vLowBins);
      Fill(hRecoMuonDThetaWrtStraightInCSCStation2, vAngleWrtPAllStations[5], binningVariableValue,vLowBins);
      Fill(hRecoMuonDThetaWrtStraightInCSCStation3, vAngleWrtPAllStations[6], binningVariableValue,vLowBins);
      Fill(hRecoMuonDThetaWrtStraightInCSCStation4, vAngleWrtPAllStations[7], binningVariableValue,vLowBins);
      Fill(hRecoMuonMinDThetaWrtStraight, minAngleWrtP, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxDThetaWrtStraight, maxAngleWrtP, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgDThetaWrtStraight, avgAngleWrtP, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianDThetaWrtStraight, medianAngleWrtP, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevDThetaWrtStraight, stddevAngleWrtP, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevDThetaWrtStraight->Fill(stddevAngleWrtP, avgAngleWrtP);
      hRecoMuonMedianVsStddevDThetaWrtStraight->Fill(stddevAngleWrtP, medianAngleWrtP);
      Fill(hRecoMuonMinAbsDThetaWrtStraight, minAbsAngleWrtP, binningVariableValue,vLowBins);
      Fill(hRecoMuonMaxAbsDThetaWrtStraight, maxAbsAngleWrtP, binningVariableValue,vLowBins);
      Fill(hRecoMuonAvgAbsDThetaWrtStraight, avgAbsAngleWrtP, binningVariableValue,vLowBins);
      Fill(hRecoMuonMedianAbsDThetaWrtStraight, medianAbsAngleWrtP, binningVariableValue,vLowBins);
      Fill(hRecoMuonStddevAbsDThetaWrtStraight, stddevAbsAngleWrtP, binningVariableValue,vLowBins);
      hRecoMuonAvgVsStddevAbsDThetaWrtStraight->Fill(stddevAbsAngleWrtP, avgAbsAngleWrtP);
      hRecoMuonMedianVsStddevAbsDThetaWrtStraight->Fill(stddevAbsAngleWrtP, medianAbsAngleWrtP);
      
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
    
    for(unsigned int i = 0; i < vSelectedMuons.size(); i++)
    {
      nSelMuons++;
      unsigned short iMuon = vSelectedMuons[i];
      unsigned short iTrack = Muon_trackref->at(iMuon);
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
//       double trackPt = Track_pt->at(iTrack);
//       double trackEta = Track_eta->at(iTrack);
//       double muonPt = Muon_pt->at(iMuon);
//       double muonEta = Muon_eta->at(iMuon);
//       if(fabs(trackPt-muonPt)/muonPt > 0.0001)
//       {
//         std::cout << "PT IS DIFFERENT FOR MUON " << iMuon << "!!!!\n";
//         std::cout << "muon pt = " << muonPt << ", track pt = " << trackPt << std::endl;
//       }
//       if(fabs((trackEta-muonEta)/muonEta) > 0.0001)
//       {
//         std::cout << "ETA IS DIFFERENT FOR MUON " << iMuon << "!!!!\n";
//         std::cout << "muon eta = " << muonEta << ", track eta = " << trackEta << std::endl;
//       }
      Fill(hSelMuonPt,Muon_pt->at(iMuon), binningVariableValue,vLowBins);
      Fill(hSelMuonEta,Muon_eta->at(iMuon), binningVariableValue,vLowBins);
      hSelMuonPtVsEta->Fill(Muon_eta->at(iMuon), Muon_pt->at(iMuon));
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
      if(iTrack != 65535 && iTrack != 2048)
      {
        Fill(hSelMuonTrackValidHits,Track_numberOfValidHits->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackChi2,Track_normalizedChi2->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackNdof,Track_ndof->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackLayersWithMeasurement,Track_layersWithMeasurement->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackValidPixelHits,Track_validPixelHits->at(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackValidFraction, GetTrackValidFraction(iTrack), binningVariableValue,vLowBins);
        Fill(hSelMuonTrackExpOuterHits, GetNumberOfLostTrackerOuterHits(iTrack), binningVariableValue,vLowBins);
        
        if(iSelectedVertex != 65535)
        {
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
        }
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
        Fill(hSelMuonTrackValidFraction,0, binningVariableValue,vLowBins);
        Fill(hSelMuonTrackExpOuterHits,0, binningVariableValue,vLowBins);
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
      
//       int charge = Muon_charge->at(iMuon);
      
//       std::cout << "Charge = " << charge << std::endl;
      TVector3 muonP;
      muonP.SetPtEtaPhi(Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon));
      double muonPMag = muonP.Mag();
      muonP.SetMag(1);
      muonP.RotateZ(atan(muonP.X()/muonP.Y()));
      double muonPDyDz = muonP.Y()/muonP.Z();
//       std::cout << "px = " << muonP.X() << std::endl;
//       std::cout << "py = " << muonP.Y() << std::endl;
//       std::cout << "pz = " << muonP.Z() << std::endl;
//       std::cout << "angle = " << atan(muonPDyDz) << std::endl;

      TVector3 muonMu1State, muonMu2State;
      muonMu1State.SetXYZ(Muon_Mu1State_x->at(iMuon),Muon_Mu1State_y->at(iMuon),Muon_Mu1State_z->at(iMuon));
      muonMu2State.SetXYZ(Muon_Mu2State_x->at(iMuon),Muon_Mu2State_y->at(iMuon),Muon_Mu2State_z->at(iMuon));
      muonMu1State.SetMag(1);
      muonMu2State.SetMag(1);
      muonMu1State.RotateZ(atan(muonMu1State.X()/muonMu1State.Y()));
      muonMu2State.RotateZ(atan(muonMu2State.X()/muonMu2State.Y()));
//       double muonMu1StateDyDz = muonMu1State.Y()/muonMu1State.Z();
//       double muonMu2StateDyDz = muonMu2State.Y()/muonMu2State.Z();
      
//       std::cout << "Mu1State x = " << muonMu1State.X() << std::endl;
//       std::cout << "Mu1State y = " << muonMu1State.Y() << std::endl;
//       std::cout << "Mu1State z = " << muonMu1State.Z() << std::endl;
//       std::cout << "Mu1State angle = " << atan(muonMu1StateDyDz) << std::endl;
//       std::cout << "Mu2State x = " << muonMu2State.X() << std::endl;
//       std::cout << "Mu2State y = " << muonMu2State.Y() << std::endl;
//       std::cout << "Mu2State z = " << muonMu2State.Z() << std::endl;
//       std::cout << "Mu2State angle = " << atan(muonMu2StateDyDz) << std::endl;
      
      double minDDxDz = 9999999;
      double minDDyDz = 9999999;
      double minDx = 9999999;
      double minDy = 9999999;
      double minPullDxDz = 9999999;
      double minPullDyDz = 9999999;
      double minPullX = 9999999;
      double minPullY = 9999999;
      double minAngleWrtP = 9999999;
      double maxDDxDz = 9999999;
      double maxDDyDz = 9999999;
      double maxDx = 9999999;
      double maxDy = 9999999;
      double maxPullDxDz = 9999999;
      double maxPullDyDz = 9999999;
      double maxPullX = 9999999;
      double maxPullY = 9999999;
      double maxAngleWrtP = 9999999;
      double avgDDxDz = 0;
      double avgDDyDz = 0;
      double avgDx = 0;
      double avgDy = 0;
      double avgPullDxDz = 0;
      double avgPullDyDz = 0;
      double avgPullX = 0;
      double avgPullY = 0;
      double avgAngleWrtP = 0;
      double medianDDxDz = 9999999;
      double medianDDyDz = 9999999;
      double medianDx = 9999999;
      double medianDy = 9999999;
      double medianPullDxDz = 9999999;
      double medianPullDyDz = 9999999;
      double medianPullX = 9999999;
      double medianPullY = 9999999;
      double medianAngleWrtP = 9999999;
      double stddevDDxDz = 0;
      double stddevDDyDz = 0;
      double stddevDx = 0;
      double stddevDy = 0;
      double stddevPullDxDz = 0;
      double stddevPullDyDz = 0;
      double stddevPullX = 0;
      double stddevPullY = 0;
      double stddevAngleWrtP = 0;
      double minAbsDDxDz = 9999999;
      double minAbsDDyDz = 9999999;
      double minAbsDx = 9999999;
      double minAbsDy = 9999999;
      double minAbsPullDxDz = 9999999;
      double minAbsPullDyDz = 9999999;
      double minAbsPullX = 9999999;
      double minAbsPullY = 9999999;
      double minAbsAngleWrtP = 9999999;
      double maxAbsDDxDz = 9999999;
      double maxAbsDDyDz = 9999999;
      double maxAbsDx = 9999999;
      double maxAbsDy = 9999999;
      double maxAbsPullDxDz = 9999999;
      double maxAbsPullDyDz = 9999999;
      double maxAbsPullX = 9999999;
      double maxAbsPullY = 9999999;
      double maxAbsAngleWrtP = 9999999;
      double avgAbsDDxDz = 0;
      double avgAbsDDyDz = 0;
      double avgAbsDx = 0;
      double avgAbsDy = 0;
      double avgAbsPullDxDz = 0;
      double avgAbsPullDyDz = 0;
      double avgAbsPullX = 0;
      double avgAbsPullY = 0;
      double avgAbsAngleWrtP = 0;
      double medianAbsDDxDz = 9999999;
      double medianAbsDDyDz = 9999999;
      double medianAbsDx = 9999999;
      double medianAbsDy = 9999999;
      double medianAbsPullDxDz = 9999999;
      double medianAbsPullDyDz = 9999999;
      double medianAbsPullX = 9999999;
      double medianAbsPullY = 9999999;
      double medianAbsAngleWrtP = 9999999;
      double stddevAbsDDxDz = 0;
      double stddevAbsDDyDz = 0;
      double stddevAbsDx = 0;
      double stddevAbsDy = 0;
      double stddevAbsPullDxDz = 0;
      double stddevAbsPullDyDz = 0;
      double stddevAbsPullX = 0;
      double stddevAbsPullY = 0;
      double stddevAbsAngleWrtP = 0;
      std::vector<double> vDDxDz;
      std::vector<double> vDDyDz;
      std::vector<double> vDx;
      std::vector<double> vDy;
      std::vector<double> vPullDxDz;
      std::vector<double> vPullDyDz;
      std::vector<double> vPullX;
      std::vector<double> vPullY;
      std::vector<double> vAngleWrtP;
      std::vector<double> vAngleWrtPAllStations;
      // Hardcoded function: tries to factor out the dependence from momentum of the DTheta distribution
      double factor = exp(-2.07609-1.42445e-1+muonPMag)+4.30656e-2;
      
      for(unsigned int iStation = 0; iStation < 8; iStation++)
      {
        if(Muon_dDxDz->at(iMuon).at(iStation) < 999998)
        {
          vDDxDz.push_back(Muon_dDxDz->at(iMuon).at(iStation));
          Fill(hSelMuonDDxDz, Muon_dDxDz->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hSelMuonAbsDDxDz, fabs(Muon_dDxDz->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_dDyDz->at(iMuon).at(iStation) < 999998)
        {
          vDDyDz.push_back(Muon_dDyDz->at(iMuon).at(iStation));
          Fill(hSelMuonDDyDz, Muon_dDyDz->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hSelMuonAbsDDyDz, fabs(Muon_dDyDz->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_Dx->at(iMuon).at(iStation) < 999998)
        {
          vDx.push_back(Muon_Dx->at(iMuon).at(iStation));
          Fill(hSelMuonDx, Muon_Dx->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hSelMuonAbsDx, fabs(Muon_Dx->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_Dy->at(iMuon).at(iStation) < 999998)
        {
          vDy.push_back(Muon_Dy->at(iMuon).at(iStation));
          Fill(hSelMuonDy, Muon_Dy->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hSelMuonAbsDy, fabs(Muon_Dy->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_pullDxDz->at(iMuon).at(iStation) < 999998)
        {
          vPullDxDz.push_back(Muon_pullDxDz->at(iMuon).at(iStation));
          Fill(hSelMuonPullDxDz, Muon_pullDxDz->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hSelMuonAbsPullDxDz, fabs(Muon_pullDxDz->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_pullDyDz->at(iMuon).at(iStation) < 999998)
        {
          vPullDyDz.push_back(Muon_pullDyDz->at(iMuon).at(iStation));
          Fill(hSelMuonPullDyDz, Muon_pullDyDz->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hSelMuonAbsPullDyDz, fabs(Muon_pullDyDz->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_pullX->at(iMuon).at(iStation) < 999998)
        {
          vPullX.push_back(Muon_pullX->at(iMuon).at(iStation));
          Fill(hSelMuonPullX, Muon_pullX->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hSelMuonAbsPullX, fabs(Muon_pullX->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
        if(Muon_pullY->at(iMuon).at(iStation) < 999998)
        {
          vPullY.push_back(Muon_pullY->at(iMuon).at(iStation));
          Fill(hSelMuonPullY, Muon_pullY->at(iMuon).at(iStation), binningVariableValue,vLowBins);
          Fill(hSelMuonAbsPullY, fabs(Muon_pullY->at(iMuon).at(iStation)), binningVariableValue,vLowBins);
        }
//         if(Muon_segmentDxDz->at(iMuon).at(iStation) < 999998)
//           std::cout << "segment mu" << iStation << " dxdz = " << Muon_segmentDxDz->at(iMuon).at(iStation) << std::endl;
//         if(Muon_segmentDyDz->at(iMuon).at(iStation) < 999998)
//           std::cout << "segment mu" << iStation << " dydz = " << Muon_segmentDyDz->at(iMuon).at(iStation) << std::endl;
        if(Muon_segmentDxDz->at(iMuon).at(iStation) < 999998 && Muon_segmentDyDz->at(iMuon).at(iStation) < 999998)
        {
          TVector3 segmentXYZ;
          if(iStation < 4)
            segmentXYZ.SetXYZ(Muon_segmentDxDz->at(iMuon).at(iStation),1,-Muon_segmentDyDz->at(iMuon).at(iStation));
          else
            segmentXYZ.SetXYZ(Muon_segmentDxDz->at(iMuon).at(iStation),-Muon_segmentDyDz->at(iMuon).at(iStation),1);
          segmentXYZ.SetMag(1);
          segmentXYZ.RotateZ(atan(segmentXYZ.X()/segmentXYZ.Y()));
          double segmentXYZDyDz = segmentXYZ.Y()/segmentXYZ.Z();
//           std::cout << "Segment mu" << iStation << " x = " << segmentXYZ.X() << std::endl;
//           std::cout << "Segment mu" << iStation << " y = " << segmentXYZ.Y() << std::endl;
//           std::cout << "Segment mu" << iStation << " z = " << segmentXYZ.Z() << std::endl;
//           std::cout << "Segment mu" << iStation << " angle = " << atan(segmentXYZDyDz) << std::endl;
          double angleWrtP = fabs(atan(segmentXYZDyDz)) - fabs(atan(muonPDyDz));
//           std::cout << "Delta(angle) wrt P = " << angleWrtP << std::endl;
          vAngleWrtP.push_back(angleWrtP/factor);
          vAngleWrtPAllStations.push_back(angleWrtP/factor);
          Fill(hSelMuonDThetaWrtStraight,angleWrtP/factor,binningVariableValue,vLowBins);
          Fill(hSelMuonAbsDThetaWrtStraight,fabs(angleWrtP/factor),binningVariableValue,vLowBins);
          hSelMuonDThetaWrtStraightVsP->Fill(muonPMag,angleWrtP);
        }
        else
        {
          vAngleWrtPAllStations.push_back(999999);
        }
      }
      if(vAngleWrtP.size() == 0)
      {
        std::cout << "W A R N I N G ! No segments with full angular info found for this muon!\n";
        std::cout << "                Filling the DeltaAngle vector with zero.\n";
        vAngleWrtP.push_back(0);
      }
      FindMinMaxAvgMedianStddev(&vDDxDz, minDDxDz, maxDDxDz, avgDDxDz, medianDDxDz, stddevDDxDz);
      FindMinMaxAvgMedianStddev(&vDDyDz, minDDyDz, maxDDyDz, avgDDyDz, medianDDyDz, stddevDDyDz);
      FindMinMaxAvgMedianStddev(&vDx, minDx, maxDx, avgDx, medianDx, stddevDx);
      FindMinMaxAvgMedianStddev(&vDy, minDy, maxDy, avgDy, medianDy, stddevDy);
      FindMinMaxAvgMedianStddev(&vPullDxDz, minPullDxDz, maxPullDxDz, avgPullDxDz, medianPullDxDz, stddevPullDxDz);
      FindMinMaxAvgMedianStddev(&vPullDyDz, minPullDyDz, maxPullDyDz, avgPullDyDz, medianPullDyDz, stddevPullDyDz);
      FindMinMaxAvgMedianStddev(&vPullX, minPullX, maxPullX, avgPullX, medianPullX, stddevPullX);
      FindMinMaxAvgMedianStddev(&vPullY, minPullY, maxPullY, avgPullY, medianPullY, stddevPullY);
      FindMinMaxAvgMedianStddev(&vAngleWrtP, minAngleWrtP, maxAngleWrtP, avgAngleWrtP, medianAngleWrtP, stddevAngleWrtP);
      
      FindAbsMinMaxAvgMedianStddev(&vDDxDz, minAbsDDxDz, maxAbsDDxDz, avgAbsDDxDz, medianAbsDDxDz, stddevAbsDDxDz);
      FindAbsMinMaxAvgMedianStddev(&vDDyDz, minAbsDDyDz, maxAbsDDyDz, avgAbsDDyDz, medianAbsDDyDz, stddevAbsDDyDz);
      FindAbsMinMaxAvgMedianStddev(&vDx, minAbsDx, maxAbsDx, avgAbsDx, medianAbsDx, stddevAbsDx);
      FindAbsMinMaxAvgMedianStddev(&vDy, minAbsDy, maxAbsDy, avgAbsDy, medianAbsDy, stddevAbsDy);
      FindAbsMinMaxAvgMedianStddev(&vPullDxDz, minAbsPullDxDz, maxAbsPullDxDz, avgAbsPullDxDz, medianAbsPullDxDz, stddevAbsPullDxDz);
      FindAbsMinMaxAvgMedianStddev(&vPullDyDz, minAbsPullDyDz, maxAbsPullDyDz, avgAbsPullDyDz, medianAbsPullDyDz, stddevAbsPullDyDz);
      FindAbsMinMaxAvgMedianStddev(&vPullX, minAbsPullX, maxAbsPullX, avgAbsPullX, medianAbsPullX, stddevAbsPullX);
      FindAbsMinMaxAvgMedianStddev(&vPullY, minAbsPullY, maxAbsPullY, avgAbsPullY, medianAbsPullY, stddevAbsPullY);
      FindAbsMinMaxAvgMedianStddev(&vAngleWrtP, minAbsAngleWrtP, maxAbsAngleWrtP, avgAbsAngleWrtP, medianAbsAngleWrtP, stddevAbsAngleWrtP);
      
      Fill(hSelMuonDDxDzInDTStation1, Muon_dDxDz->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonDDxDzInDTStation2, Muon_dDxDz->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonDDxDzInDTStation3, Muon_dDxDz->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonDDxDzInDTStation4, Muon_dDxDz->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonDDxDzInCSCStation1, Muon_dDxDz->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hSelMuonDDxDzInCSCStation2, Muon_dDxDz->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hSelMuonDDxDzInCSCStation3, Muon_dDxDz->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hSelMuonDDxDzInCSCStation4, Muon_dDxDz->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hSelMuonMinDDxDz, minDDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxDDxDz, maxDDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgDDxDz, avgDDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianDDxDz, medianDDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevDDxDz, stddevDDxDz, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevDDxDz->Fill(stddevDDxDz, avgDDxDz);
      hSelMuonMedianVsStddevDDxDz->Fill(stddevDDxDz, medianDDxDz);
      Fill(hSelMuonMinAbsDDxDz, minAbsDDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsDDxDz, maxAbsDDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsDDxDz, avgAbsDDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsDDxDz, medianAbsDDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsDDxDz, stddevAbsDDxDz, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsDDxDz->Fill(stddevAbsDDxDz, avgAbsDDxDz);
      hSelMuonMedianVsStddevAbsDDxDz->Fill(stddevAbsDDxDz, medianAbsDDxDz);
      
      Fill(hSelMuonDDyDzInDTStation1, Muon_dDyDz->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonDDyDzInDTStation2, Muon_dDyDz->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonDDyDzInDTStation3, Muon_dDyDz->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonDDyDzInDTStation4, Muon_dDyDz->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonDDyDzInCSCStation1, Muon_dDyDz->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hSelMuonDDyDzInCSCStation2, Muon_dDyDz->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hSelMuonDDyDzInCSCStation3, Muon_dDyDz->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hSelMuonDDyDzInCSCStation4, Muon_dDyDz->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hSelMuonMinDDyDz, minDDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxDDyDz, maxDDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgDDyDz, avgDDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianDDyDz, medianDDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevDDyDz, stddevDDyDz, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevDDyDz->Fill(stddevDDyDz, avgDDyDz);
      hSelMuonMedianVsStddevDDyDz->Fill(stddevDDyDz, medianDDyDz);
      Fill(hSelMuonMinAbsDDyDz, minAbsDDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsDDyDz, maxAbsDDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsDDyDz, avgAbsDDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsDDyDz, medianAbsDDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsDDyDz, stddevAbsDDyDz, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsDDyDz->Fill(stddevAbsDDyDz, avgAbsDDyDz);
      hSelMuonMedianVsStddevAbsDDyDz->Fill(stddevAbsDDyDz, medianAbsDDyDz);
      
      Fill(hSelMuonDxInDTStation1, Muon_Dx->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonDxInDTStation2, Muon_Dx->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonDxInDTStation3, Muon_Dx->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonDxInDTStation4, Muon_Dx->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonDxInCSCStation1, Muon_Dx->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hSelMuonDxInCSCStation2, Muon_Dx->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hSelMuonDxInCSCStation3, Muon_Dx->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hSelMuonDxInCSCStation4, Muon_Dx->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hSelMuonMinDx, minDx, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxDx, maxDx, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgDx, avgDx, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianDx, medianDx, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevDx, stddevDx, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevDx->Fill(stddevDx, avgDx);
      hSelMuonMedianVsStddevDx->Fill(stddevDx, medianDx);
      Fill(hSelMuonMinAbsDx, minAbsDx, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsDx, maxAbsDx, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsDx, avgAbsDx, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsDx, medianAbsDx, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsDx, stddevAbsDx, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsDx->Fill(stddevAbsDx, avgAbsDx);
      hSelMuonMedianVsStddevAbsDx->Fill(stddevAbsDx, medianAbsDx);
      
      Fill(hSelMuonDyInDTStation1, Muon_Dy->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonDyInDTStation2, Muon_Dy->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonDyInDTStation3, Muon_Dy->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonDyInDTStation4, Muon_Dy->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonDyInCSCStation1, Muon_Dy->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hSelMuonDyInCSCStation2, Muon_Dy->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hSelMuonDyInCSCStation3, Muon_Dy->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hSelMuonDyInCSCStation4, Muon_Dy->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hSelMuonMinDy, minDy, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxDy, maxDy, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgDy, avgDy, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianDy, medianDy, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevDy, stddevDy, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevDy->Fill(stddevDy, avgDy);
      hSelMuonMedianVsStddevDy->Fill(stddevDy, medianDy);
      Fill(hSelMuonMinAbsDy, minAbsDy, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsDy, maxAbsDy, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsDy, avgAbsDy, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsDy, medianAbsDy, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsDy, stddevAbsDy, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsDy->Fill(stddevAbsDy, avgAbsDy);
      hSelMuonMedianVsStddevAbsDy->Fill(stddevAbsDy, medianAbsDy);
      
      Fill(hSelMuonPullDxDzInDTStation1, Muon_pullDxDz->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDxDzInDTStation2, Muon_pullDxDz->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDxDzInDTStation3, Muon_pullDxDz->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDxDzInDTStation4, Muon_pullDxDz->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDxDzInCSCStation1, Muon_pullDxDz->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDxDzInCSCStation2, Muon_pullDxDz->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDxDzInCSCStation3, Muon_pullDxDz->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDxDzInCSCStation4, Muon_pullDxDz->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hSelMuonMinPullDxDz, minPullDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxPullDxDz, maxPullDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgPullDxDz, avgPullDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianPullDxDz, medianPullDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevPullDxDz, stddevPullDxDz, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevPullDxDz->Fill(stddevPullDxDz, avgPullDxDz);
      hSelMuonMedianVsStddevPullDxDz->Fill(stddevPullDxDz, medianPullDxDz);
      Fill(hSelMuonMinAbsPullDxDz, minAbsPullDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsPullDxDz, maxAbsPullDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsPullDxDz, avgAbsPullDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsPullDxDz, medianAbsPullDxDz, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsPullDxDz, stddevAbsPullDxDz, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsPullDxDz->Fill(stddevAbsPullDxDz, avgAbsPullDxDz);
      hSelMuonMedianVsStddevAbsPullDxDz->Fill(stddevAbsPullDxDz, medianAbsPullDxDz);
      
      Fill(hSelMuonPullDyDzInDTStation1, Muon_pullDyDz->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDyDzInDTStation2, Muon_pullDyDz->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDyDzInDTStation3, Muon_pullDyDz->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDyDzInDTStation4, Muon_pullDyDz->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDyDzInCSCStation1, Muon_pullDyDz->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDyDzInCSCStation2, Muon_pullDyDz->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDyDzInCSCStation3, Muon_pullDyDz->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hSelMuonPullDyDzInCSCStation4, Muon_pullDyDz->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hSelMuonMinPullDyDz, minPullDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxPullDyDz, maxPullDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgPullDyDz, avgPullDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianPullDyDz, medianPullDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevPullDyDz, stddevPullDyDz, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevPullDyDz->Fill(stddevPullDyDz, avgPullDyDz);
      hSelMuonMedianVsStddevPullDyDz->Fill(stddevPullDyDz, medianPullDyDz);
      Fill(hSelMuonMinAbsPullDyDz, minAbsPullDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsPullDyDz, maxAbsPullDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsPullDyDz, avgAbsPullDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsPullDyDz, medianAbsPullDyDz, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsPullDyDz, stddevAbsPullDyDz, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsPullDyDz->Fill(stddevAbsPullDyDz, avgAbsPullDyDz);
      hSelMuonMedianVsStddevAbsPullDyDz->Fill(stddevAbsPullDyDz, medianAbsPullDyDz);
      
      Fill(hSelMuonPullXInDTStation1, Muon_pullX->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonPullXInDTStation2, Muon_pullX->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonPullXInDTStation3, Muon_pullX->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonPullXInDTStation4, Muon_pullX->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonPullXInCSCStation1, Muon_pullX->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hSelMuonPullXInCSCStation2, Muon_pullX->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hSelMuonPullXInCSCStation3, Muon_pullX->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hSelMuonPullXInCSCStation4, Muon_pullX->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hSelMuonMinPullX, minPullX, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxPullX, maxPullX, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgPullX, avgPullX, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianPullX, medianPullX, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevPullX, stddevPullX, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevPullX->Fill(stddevPullX, avgPullX);
      hSelMuonMedianVsStddevPullX->Fill(stddevPullX, medianPullX);
      Fill(hSelMuonMinAbsPullX, minAbsPullX, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsPullX, maxAbsPullX, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsPullX, avgAbsPullX, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsPullX, medianAbsPullX, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsPullX, stddevAbsPullX, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsPullX->Fill(stddevAbsPullX, avgAbsPullX);
      hSelMuonMedianVsStddevAbsPullX->Fill(stddevAbsPullX, medianAbsPullX);
      
      Fill(hSelMuonPullYInDTStation1, Muon_pullY->at(iMuon).at(0), binningVariableValue,vLowBins);
      Fill(hSelMuonPullYInDTStation2, Muon_pullY->at(iMuon).at(1), binningVariableValue,vLowBins);
      Fill(hSelMuonPullYInDTStation3, Muon_pullY->at(iMuon).at(2), binningVariableValue,vLowBins);
      Fill(hSelMuonPullYInDTStation4, Muon_pullY->at(iMuon).at(3), binningVariableValue,vLowBins);
      Fill(hSelMuonPullYInCSCStation1, Muon_pullY->at(iMuon).at(4), binningVariableValue,vLowBins);
      Fill(hSelMuonPullYInCSCStation2, Muon_pullY->at(iMuon).at(5), binningVariableValue,vLowBins);
      Fill(hSelMuonPullYInCSCStation3, Muon_pullY->at(iMuon).at(6), binningVariableValue,vLowBins);
      Fill(hSelMuonPullYInCSCStation4, Muon_pullY->at(iMuon).at(7), binningVariableValue,vLowBins);
      Fill(hSelMuonMinPullY, minPullY, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxPullY, maxPullY, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgPullY, avgPullY, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianPullY, medianPullY, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevPullY, stddevPullY, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevPullY->Fill(stddevPullY, avgPullY);
      hSelMuonMedianVsStddevPullY->Fill(stddevPullY, medianPullY);
      Fill(hSelMuonMinAbsPullY, minAbsPullY, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsPullY, maxAbsPullY, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsPullY, avgAbsPullY, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsPullY, medianAbsPullY, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsPullY, stddevAbsPullY, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsPullY->Fill(stddevAbsPullY, avgAbsPullY);
      hSelMuonMedianVsStddevAbsPullY->Fill(stddevAbsPullY, medianAbsPullY);
            
      Fill(hSelMuonDThetaWrtStraightInDTStation1, vAngleWrtPAllStations[0], binningVariableValue,vLowBins);
      Fill(hSelMuonDThetaWrtStraightInDTStation2, vAngleWrtPAllStations[1], binningVariableValue,vLowBins);
      Fill(hSelMuonDThetaWrtStraightInDTStation3, vAngleWrtPAllStations[2], binningVariableValue,vLowBins);
      Fill(hSelMuonDThetaWrtStraightInDTStation4, vAngleWrtPAllStations[3], binningVariableValue,vLowBins);
      Fill(hSelMuonDThetaWrtStraightInCSCStation1, vAngleWrtPAllStations[4], binningVariableValue,vLowBins);
      Fill(hSelMuonDThetaWrtStraightInCSCStation2, vAngleWrtPAllStations[5], binningVariableValue,vLowBins);
      Fill(hSelMuonDThetaWrtStraightInCSCStation3, vAngleWrtPAllStations[6], binningVariableValue,vLowBins);
      Fill(hSelMuonDThetaWrtStraightInCSCStation4, vAngleWrtPAllStations[7], binningVariableValue,vLowBins);
      Fill(hSelMuonMinDThetaWrtStraight, minAngleWrtP, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxDThetaWrtStraight, maxAngleWrtP, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgDThetaWrtStraight, avgAngleWrtP, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianDThetaWrtStraight, medianAngleWrtP, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevDThetaWrtStraight, stddevAngleWrtP, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevDThetaWrtStraight->Fill(stddevAngleWrtP, avgAngleWrtP);
      hSelMuonMedianVsStddevDThetaWrtStraight->Fill(stddevAngleWrtP, medianAngleWrtP);
      Fill(hSelMuonMinAbsDThetaWrtStraight, minAbsAngleWrtP, binningVariableValue,vLowBins);
      Fill(hSelMuonMaxAbsDThetaWrtStraight, maxAbsAngleWrtP, binningVariableValue,vLowBins);
      Fill(hSelMuonAvgAbsDThetaWrtStraight, avgAbsAngleWrtP, binningVariableValue,vLowBins);
      Fill(hSelMuonMedianAbsDThetaWrtStraight, medianAbsAngleWrtP, binningVariableValue,vLowBins);
      Fill(hSelMuonStddevAbsDThetaWrtStraight, stddevAbsAngleWrtP, binningVariableValue,vLowBins);
      hSelMuonAvgVsStddevAbsDThetaWrtStraight->Fill(stddevAbsAngleWrtP, avgAbsAngleWrtP);
      hSelMuonMedianVsStddevAbsDThetaWrtStraight->Fill(stddevAbsAngleWrtP, medianAbsAngleWrtP);
    }
    
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

    hNRecoMuons->Fill(nRecoMuons);
    hNSelMuons->Fill(nSelMuons);
    hNTriggeringRecoMuons->Fill(nTriggeringRecoMuons);
    hNTriggeringSelMuons->Fill(nTriggeringSelMuons);
    hNRecoMuonsVsRunNumber->Fill(runNumber,nRecoMuons);
    hNSelMuonsVsRunNumber->Fill(runNumber,nSelMuons);
    
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
    
    if(iSelectedVertex != 65535)
    {
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
    }
    hNRecoVertices->Fill(nRecoVertices);
    hNSelVertices->Fill(nSelVertices);
    hNRecoVerticesVsRunNumber->Fill(runNumber, nRecoVertices);
    hNSelVerticesVsRunNumber->Fill(runNumber, nSelVertices);
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
  
  unsigned int nPoints = 0;
  
  std::vector<TGraphAsymmErrors*>* gFakeRateVsGenPt = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateVsGenPt", "Fake rate vs. gen p_{T}","gen p_{T} [GeV]","Fake rate",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateVsGenEta = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateVsGenEta", "Fake rate vs. gen #eta","gen #eta","Fake rate",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateVsGenPtPlus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateVsGenPtPlus", "Fake rate of positive particles vs. gen p_{T}","gen p_{T} [GeV]","Fake rate",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateVsGenEtaPlus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateVsGenEtaPlus", "Fake rate of positive particles vs. gen #eta","gen #eta","Fake rate",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateVsGenPtMinus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateVsGenPtMinus", "Fake rate of negative particles vs. gen p_{T}","gen p_{T} [GeV]","Fake rate",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateVsGenEtaMinus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateVsGenEtaMinus", "Fake rate of negative particles vs. gen #eta","gen #eta","Fake rate",sBinningVariable,vLowBins, nPoints);
  
  std::vector<TGraphAsymmErrors*>* gFakeRateTkVsGenPt = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateTkVsGenPt", "Tracking efficiency vs. gen p_{T}","gen p_{T} [GeV]","Tracking efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateTkVsGenEta = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateTkVsGenEta", "Tracking efficiency vs. gen #eta","gen #eta","Tracking efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateTkVsGenPtPlus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateTkVsGenPtPlus", "Tracking efficiency of positive particles vs. gen p_{T}","gen p_{T} [GeV]","Tracking efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateTkVsGenEtaPlus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateTkVsGenEtaPlus", "Tracking efficiency of positive particles vs. gen #eta","gen #eta","Tracking efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateTkVsGenPtMinus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateTkVsGenPtMinus", "Tracking efficiency of negative particles vs. gen p_{T}","gen p_{T} [GeV]","Tracking efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateTkVsGenEtaMinus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateTkVsGenEtaMinus", "Tracking efficiency of negative particles vs. gen #eta","gen #eta","Tracking efficiency",sBinningVariable,vLowBins, nPoints);
  
  std::vector<TGraphAsymmErrors*>* gFakeRateSelVsRecoPt = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateSelVsRecoPt", "Selection efficiency vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateSelVsRecoEta = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateSelVsRecoEta", "Selection efficiency vs. reco #eta","reco #eta","Selection efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateSelVsRecoPtPlus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateSelVsRecoPtPlus", "Selection efficiency of positive particles vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateSelVsRecoEtaPlus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateSelVsRecoEtaPlus", "Selection efficiency of positive particles vs. reco #eta","reco #eta","Selection efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateSelVsRecoPtMinus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateSelVsRecoPtMinus", "Selection efficiency of negative particles vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency",sBinningVariable,vLowBins, nPoints);
  std::vector<TGraphAsymmErrors*>* gFakeRateSelVsRecoEtaMinus = CreateVectorOfGraphs<TGraphAsymmErrors>("gFakeRateSelVsRecoEtaMinus", "Selection efficiency of negative particles vs. reco #eta","reco #eta","Selection efficiency",sBinningVariable,vLowBins, nPoints);
  
  for(int i = 0; i < std::max((int)vLowBins.size(),1); i++)
  {
    BayesDivide(hAssocGenParticleGenPt->at(i), hAllGenParticleGenPt->at(i), gFakeRateVsGenPt->at(i));
    BayesDivide(hAssocGenParticleGenEta->at(i), hAllGenParticleGenEta->at(i), gFakeRateVsGenEta->at(i));
    BayesDivide(hAssocGenParticlePlusGenPt->at(i), hAllGenParticlePlusGenPt->at(i), gFakeRateVsGenPtPlus->at(i));
    BayesDivide(hAssocGenParticlePlusGenEta->at(i), hAllGenParticlePlusGenEta->at(i), gFakeRateVsGenEtaPlus->at(i));
    BayesDivide(hAssocGenParticleMinusGenPt->at(i), hAllGenParticleMinusGenPt->at(i), gFakeRateVsGenPtMinus->at(i));
    BayesDivide(hAssocGenParticleMinusGenEta->at(i), hAllGenParticleMinusGenEta->at(i), gFakeRateVsGenEtaMinus->at(i));
    BayesDivide(hAssocTkGenParticleGenPt->at(i), hAllGenParticleGenPt->at(i), gFakeRateTkVsGenPt->at(i));
    BayesDivide(hAssocTkGenParticleGenEta->at(i), hAllGenParticleGenEta->at(i), gFakeRateTkVsGenEta->at(i));
    BayesDivide(hAssocTkGenParticlePlusGenPt->at(i), hAllGenParticlePlusGenPt->at(i), gFakeRateTkVsGenPtPlus->at(i));
    BayesDivide(hAssocTkGenParticlePlusGenEta->at(i), hAllGenParticlePlusGenEta->at(i), gFakeRateTkVsGenEtaPlus->at(i));
    BayesDivide(hAssocTkGenParticleMinusGenPt->at(i), hAllGenParticleMinusGenPt->at(i), gFakeRateTkVsGenPtMinus->at(i));
    BayesDivide(hAssocTkGenParticleMinusGenEta->at(i), hAllGenParticleMinusGenEta->at(i), gFakeRateTkVsGenEtaMinus->at(i));
    BayesDivide(hAssocGenParticleRecoPt->at(i),hAssocTkGenParticleRecoPt->at(i), gFakeRateSelVsRecoPt->at(i));
    BayesDivide(hAssocGenParticleRecoEta->at(i),hAssocTkGenParticleRecoEta->at(i), gFakeRateSelVsRecoEta->at(i));
    BayesDivide(hAssocGenParticlePlusRecoPt->at(i),hAssocTkGenParticlePlusRecoPt->at(i), gFakeRateSelVsRecoPtPlus->at(i));
    BayesDivide(hAssocGenParticlePlusRecoEta->at(i),hAssocTkGenParticlePlusRecoEta->at(i), gFakeRateSelVsRecoEtaPlus->at(i));
    BayesDivide(hAssocGenParticleMinusRecoPt->at(i),hAssocTkGenParticleMinusRecoPt->at(i), gFakeRateSelVsRecoPtMinus->at(i));
    BayesDivide(hAssocGenParticleMinusRecoEta->at(i),hAssocTkGenParticleMinusRecoEta->at(i), gFakeRateSelVsRecoEtaMinus->at(i));
    
    gFakeRateVsGenPt->at(i)->Write();
    gFakeRateVsGenEta->at(i)->Write();
    gFakeRateVsGenPtPlus->at(i)->Write();
    gFakeRateVsGenEtaPlus->at(i)->Write();
    gFakeRateVsGenPtMinus->at(i)->Write();
    gFakeRateVsGenEtaMinus->at(i)->Write();
    gFakeRateTkVsGenPt->at(i)->Write();
    gFakeRateTkVsGenEta->at(i)->Write();
    gFakeRateTkVsGenPtPlus->at(i)->Write();
    gFakeRateTkVsGenEtaPlus->at(i)->Write();
    gFakeRateTkVsGenPtMinus->at(i)->Write();
    gFakeRateTkVsGenEtaMinus->at(i)->Write();
    gFakeRateSelVsRecoPt->at(i)->Write();
    gFakeRateSelVsRecoEta->at(i)->Write();
    gFakeRateSelVsRecoPtPlus->at(i)->Write();
    gFakeRateSelVsRecoEtaPlus->at(i)->Write();
    gFakeRateSelVsRecoPtMinus->at(i)->Write();
    gFakeRateSelVsRecoEtaMinus->at(i)->Write();
  }
    
  // Create and write canvases here
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonDeltaRFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hSelMuonDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackDeltaRFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonDeltaRFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hSelMuonDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackDeltaRFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hTrackDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticleGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleRecoPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticleGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticleRecoPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticleGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleRecoEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticleGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticleRecoEta);
  CreateAndWriteCanvas("cAllGenParticleGenPtVsEta", "colz", false, false, true, hAllGenParticleGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticleGenPtVsEta", "colz", false, false, true, hAssocGenParticleGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticleRecoPtVsEta", "colz", false, false, true, hAssocGenParticleRecoPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticleGenPtVsEta", "colz", false, false, true, hAssocTkGenParticleGenPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticleRecoPtVsEta", "colz", false, false, true, hAssocTkGenParticleRecoPtVsEta);  
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticleNDaughters);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleNDaughters);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticleDecayLength);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleDecayLength);
  CreateAndWriteCanvas("cAllGenParticleDecayVertexPosition","colz",false,false,true,hAllGenParticleDecayVertexPosition);
  CreateAndWriteCanvas("cAssocGenParticleDecayVertexPosition","colz",false,false,true,hAssocGenParticleDecayVertexPosition);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateVsGenPt);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateVsGenEta);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateTkVsGenPt);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateTkVsGenEta);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateSelVsRecoPt);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateSelVsRecoEta);
  
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPlusDeltaRFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPlusDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hSelMuonPlusDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackPlusDeltaRFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackPlusDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackPlusDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackPlusDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPlusDeltaRFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonPlusDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hSelMuonPlusDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackPlusDeltaRFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackPlusDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hTrackPlusDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticlePlusGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticlePlusGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticlePlusRecoPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticlePlusGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticlePlusRecoPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticlePlusGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticlePlusGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticlePlusRecoEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticlePlusGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticlePlusRecoEta);
  CreateAndWriteCanvas("cAllGenParticlePlusGenPtVsEta", "colz", false, false, true, hAllGenParticlePlusGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticlePlusGenPtVsEta", "colz", false, false, true, hAssocGenParticlePlusGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticlePlusRecoPtVsEta", "colz", false, false, true, hAssocGenParticlePlusRecoPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticlePlusGenPtVsEta", "colz", false, false, true, hAssocTkGenParticlePlusGenPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticlePlusRecoPtVsEta", "colz", false, false, true, hAssocTkGenParticlePlusRecoPtVsEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticlePlusNDaughters);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticlePlusNDaughters);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticlePlusDecayLength);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticlePlusDecayLength);
  CreateAndWriteCanvas("cAllGenParticlePlusDecayVertexPosition","colz",false,false,true,hAllGenParticlePlusDecayVertexPosition);
  CreateAndWriteCanvas("cAssocGenParticlePlusDecayVertexPosition","colz",false,false,true,hAssocGenParticlePlusDecayVertexPosition);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateVsGenPtPlus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateVsGenEtaPlus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateTkVsGenPtPlus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateTkVsGenEtaPlus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateSelVsRecoPtPlus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateSelVsRecoEtaPlus);
  
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonMinusDeltaRFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonMinusDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hSelMuonMinusDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackMinusDeltaRFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackMinusDeltaPtOverPtFromGenp);
  CreateAndWriteCanvas("cTrackMinusDeltaRVsDeltaPtOverPtFromGenp", "colz", false, false, true, hTrackMinusDeltaRVsDeltaPtOverPtFromGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonMinusDeltaRFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonMinusDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hSelMuonMinusDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackMinusDeltaRFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hTrackMinusDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvas("cTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp", "colz", false, false, true, hTrackMinusDeltaRVsDeltaPtOverPtFromAssocGenp);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticleMinusGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleMinusGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleMinusRecoPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticleMinusGenPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticleMinusRecoPt);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticleMinusGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleMinusGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleMinusRecoEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticleMinusGenEta);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocTkGenParticleMinusRecoEta);
  CreateAndWriteCanvas("cAllGenParticleMinusGenPtVsEta", "colz", false, false, true, hAllGenParticleMinusGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticleMinusGenPtVsEta", "colz", false, false, true, hAssocGenParticleMinusGenPtVsEta);
  CreateAndWriteCanvas("cAssocGenParticleMinusRecoPtVsEta", "colz", false, false, true, hAssocGenParticleMinusRecoPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticleMinusGenPtVsEta", "colz", false, false, true, hAssocTkGenParticleMinusGenPtVsEta);
  CreateAndWriteCanvas("cAssocTkGenParticleMinusRecoPtVsEta", "colz", false, false, true, hAssocTkGenParticleMinusRecoPtVsEta);  
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticleMinusNDaughters);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleMinusNDaughters);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAllGenParticleMinusDecayLength);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hAssocGenParticleMinusDecayLength);
  CreateAndWriteCanvas("cAllGenParticleMinusDecayVertexPosition","colz",false,false,true,hAllGenParticleMinusDecayVertexPosition);
  CreateAndWriteCanvas("cAssocGenParticleMinusDecayVertexPosition","colz",false,false,true,hAssocGenParticleMinusDecayVertexPosition);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateVsGenPtMinus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateVsGenEtaMinus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateTkVsGenPtMinus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateTkVsGenEtaMinus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateSelVsRecoPtMinus);
  CreateAndWriteCanvases(21, 1, false, false, *gFakeRateSelVsRecoEtaMinus);
  
//   CreateAndWriteCanvas("cRecoMuonPt",0, 21, 1, false, true, hRecoMuonPt);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPt);
  CreateAndWriteCanvas("cSelMuonPtVsEta", "colz", false, false, true, hSelMuonPtVsEta);
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
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonTrackValidFraction);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonTrackValidFraction);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonTrackExpOuterHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonTrackExpOuterHits);
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
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonDTStationsWithHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hSelMuonDTStationsWithHits);
  CreateAndWriteCanvases(0, 21, 1, false, false, *hRecoMuonCSCStationsWithHits);
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
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDzInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDzInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDzInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDzInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDzInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDzInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDzInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDzInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDzInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDzInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDzInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDzInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDzInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDzInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDzInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDzInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevDDxDz);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevDDxDz","colz", false, false,false, hRecoMuonAvgVsStddevDDxDz);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevDDxDz","colz", false, false,false, hSelMuonAvgVsStddevDDxDz);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevDDxDz","colz", false, false,false, hRecoMuonMedianVsStddevDDxDz);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevDDxDz","colz", false, false,false, hSelMuonMedianVsStddevDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsDDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsDDxDz);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsDDxDz","colz", false, false,false, hRecoMuonAvgVsStddevAbsDDxDz);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsDDxDz","colz", false, false,false, hSelMuonAvgVsStddevAbsDDxDz);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsDDxDz","colz", false, false,false, hRecoMuonMedianVsStddevAbsDDxDz);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsDDxDz","colz", false, false,false, hSelMuonMedianVsStddevAbsDDxDz);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDzInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDzInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDzInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDzInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDzInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDzInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDzInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDzInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDzInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDzInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDzInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDzInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDzInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDzInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDzInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDzInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevDDyDz);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevDDyDz","colz", false, false,false, hRecoMuonAvgVsStddevDDyDz);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevDDyDz","colz", false, false,false, hSelMuonAvgVsStddevDDyDz);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevDDyDz","colz", false, false,false, hRecoMuonMedianVsStddevDDyDz);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevDDyDz","colz", false, false,false, hSelMuonMedianVsStddevDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsDDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsDDyDz);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsDDyDz","colz", false, false,false, hRecoMuonAvgVsStddevAbsDDyDz);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsDDyDz","colz", false, false,false, hSelMuonAvgVsStddevAbsDDyDz);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsDDyDz","colz", false, false,false, hRecoMuonMedianVsStddevAbsDDyDz);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsDDyDz","colz", false, false,false, hSelMuonMedianVsStddevAbsDDyDz);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDxInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDxInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDxInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDxInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDxInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDxInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDxInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDxInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDxInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevDx);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevDx","colz", false, false,false, hRecoMuonAvgVsStddevDx);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevDx","colz", false, false,false, hSelMuonAvgVsStddevDx);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevDx","colz", false, false,false, hRecoMuonMedianVsStddevDx);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevDx","colz", false, false,false, hSelMuonMedianVsStddevDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsDx);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsDx);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsDx","colz", false, false,false, hRecoMuonAvgVsStddevAbsDx);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsDx","colz", false, false,false, hSelMuonAvgVsStddevAbsDx);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsDx","colz", false, false,false, hRecoMuonMedianVsStddevAbsDx);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsDx","colz", false, false,false, hSelMuonMedianVsStddevAbsDx);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDyInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDyInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDyInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDyInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDyInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDyInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDyInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDyInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDyInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDyInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDyInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDyInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDyInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDyInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDyInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDyInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevDy);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevDy","colz", false, false,false, hRecoMuonAvgVsStddevDy);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevDy","colz", false, false,false, hSelMuonAvgVsStddevDy);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevDy","colz", false, false,false, hRecoMuonMedianVsStddevDy);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevDy","colz", false, false,false, hSelMuonMedianVsStddevDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsDy);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsDy);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsDy","colz", false, false,false, hRecoMuonAvgVsStddevAbsDy);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsDy","colz", false, false,false, hSelMuonAvgVsStddevAbsDy);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsDy","colz", false, false,false, hRecoMuonMedianVsStddevAbsDy);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsDy","colz", false, false,false, hSelMuonMedianVsStddevAbsDy);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDzInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDzInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDzInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDzInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDzInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDzInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDzInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDzInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDzInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDzInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDzInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDzInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDzInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDzInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDzInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDzInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevPullDxDz);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevPullDxDz","colz", false, false,false, hRecoMuonAvgVsStddevPullDxDz);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevPullDxDz","colz", false, false,false, hSelMuonAvgVsStddevPullDxDz);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevPullDxDz","colz", false, false,false, hRecoMuonMedianVsStddevPullDxDz);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevPullDxDz","colz", false, false,false, hSelMuonMedianVsStddevPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsPullDxDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsPullDxDz);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsPullDxDz","colz", false, false,false, hRecoMuonAvgVsStddevAbsPullDxDz);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsPullDxDz","colz", false, false,false, hSelMuonAvgVsStddevAbsPullDxDz);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsPullDxDz","colz", false, false,false, hRecoMuonMedianVsStddevAbsPullDxDz);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsPullDxDz","colz", false, false,false, hSelMuonMedianVsStddevAbsPullDxDz);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDzInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDzInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDzInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDzInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDzInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDzInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDzInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDzInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDzInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDzInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDzInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDzInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDzInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDzInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDzInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDzInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevPullDyDz);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevPullDyDz","colz", false, false,false, hRecoMuonAvgVsStddevPullDyDz);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevPullDyDz","colz", false, false,false, hSelMuonAvgVsStddevPullDyDz);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevPullDyDz","colz", false, false,false, hRecoMuonMedianVsStddevPullDyDz);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevPullDyDz","colz", false, false,false, hSelMuonMedianVsStddevPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsPullDyDz);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsPullDyDz);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsPullDyDz","colz", false, false,false, hRecoMuonAvgVsStddevAbsPullDyDz);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsPullDyDz","colz", false, false,false, hSelMuonAvgVsStddevAbsPullDyDz);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsPullDyDz","colz", false, false,false, hRecoMuonMedianVsStddevAbsPullDyDz);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsPullDyDz","colz", false, false,false, hSelMuonMedianVsStddevAbsPullDyDz);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullXInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullXInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullXInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullXInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullXInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullXInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullXInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullXInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullXInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullXInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullXInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullXInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullXInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullXInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullXInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullXInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevPullX);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevPullX","colz", false, false,false, hRecoMuonAvgVsStddevPullX);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevPullX","colz", false, false,false, hSelMuonAvgVsStddevPullX);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevPullX","colz", false, false,false, hRecoMuonMedianVsStddevPullX);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevPullX","colz", false, false,false, hSelMuonMedianVsStddevPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsPullX);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsPullX);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsPullX","colz", false, false,false, hRecoMuonAvgVsStddevAbsPullX);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsPullX","colz", false, false,false, hSelMuonAvgVsStddevAbsPullX);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsPullX","colz", false, false,false, hRecoMuonMedianVsStddevAbsPullX);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsPullX","colz", false, false,false, hSelMuonMedianVsStddevAbsPullX);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullYInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullYInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullYInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullYInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullYInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullYInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullYInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullYInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullYInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullYInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullYInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullYInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullYInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullYInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullYInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullYInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevPullY);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevPullY","colz", false, false,false, hRecoMuonAvgVsStddevPullY);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevPullY","colz", false, false,false, hSelMuonAvgVsStddevPullY);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevPullY","colz", false, false,false, hRecoMuonMedianVsStddevPullY);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevPullY","colz", false, false,false, hSelMuonMedianVsStddevPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsPullY);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsPullY);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsPullY","colz", false, false,false, hRecoMuonAvgVsStddevAbsPullY);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsPullY","colz", false, false,false, hSelMuonAvgVsStddevAbsPullY);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsPullY","colz", false, false,false, hRecoMuonMedianVsStddevAbsPullY);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsPullY","colz", false, false,false, hSelMuonMedianVsStddevAbsPullY);
  
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraightInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraightInDTStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraightInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraightInDTStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraightInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraightInDTStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraightInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraightInDTStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraightInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraightInCSCStation1);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraightInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraightInCSCStation2);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraightInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraightInCSCStation3);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraightInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraightInCSCStation4);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevDThetaWrtStraight);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevDThetaWrtStraight","colz", false, false,false, hRecoMuonAvgVsStddevDThetaWrtStraight);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevDThetaWrtStraight","colz", false, false,false, hSelMuonAvgVsStddevDThetaWrtStraight);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevDThetaWrtStraight","colz", false, false,false, hRecoMuonMedianVsStddevDThetaWrtStraight);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevDThetaWrtStraight","colz", false, false,false, hSelMuonMedianVsStddevDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMinAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMinAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMaxAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMaxAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonAvgAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonAvgAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonMedianAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonMedianAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hRecoMuonStddevAbsDThetaWrtStraight);
  CreateAndWriteCanvases(0, 21, 1, false, true, *hSelMuonStddevAbsDThetaWrtStraight);
  CreateAndWriteCanvas("cRecoMuonAvgVsStddevAbsDThetaWrtStraight","colz", false, false,false, hRecoMuonAvgVsStddevAbsDThetaWrtStraight);
  CreateAndWriteCanvas("cSelMuonAvgVsStddevAbsDThetaWrtStraight","colz", false, false,false, hSelMuonAvgVsStddevAbsDThetaWrtStraight);
  CreateAndWriteCanvas("cRecoMuonMedianVsStddevAbsDThetaWrtStraight","colz", false, false,false, hRecoMuonMedianVsStddevAbsDThetaWrtStraight);
  CreateAndWriteCanvas("cSelMuonMedianVsStddevAbsDThetaWrtStraight","colz", false, false,false, hSelMuonMedianVsStddevAbsDThetaWrtStraight);
  
  CreateAndWriteCanvas("cRecoMuonDThetaWrtStraightVsP","colz", false, false,false, hRecoMuonDThetaWrtStraightVsP);
  CreateAndWriteCanvas("cSelMuonDThetaWrtStraightVsP","colz", false, false,false, hSelMuonDThetaWrtStraightVsP);
  
  TObjArray hRecoMuonDThetaWrtStraightVsPSlices;
  hRecoMuonDThetaWrtStraightVsP->FitSlicesY(0, 0, -1, 0, "QNR", &hRecoMuonDThetaWrtStraightVsPSlices);
  TH1D *hRecoMuonDThetaWrtStraightVsP_0 = (TH1D*)hRecoMuonDThetaWrtStraightVsPSlices.At(0);
  TH1D *hRecoMuonDThetaWrtStraightVsP_1 = (TH1D*)hRecoMuonDThetaWrtStraightVsPSlices.At(1);
  TH1D *hRecoMuonDThetaWrtStraightVsP_2 = (TH1D*)hRecoMuonDThetaWrtStraightVsPSlices.At(2);
  TH1D *hRecoMuonDThetaWrtStraightVsP_3 = (TH1D*)hRecoMuonDThetaWrtStraightVsPSlices.At(3);
  CreateAndWriteCanvas("hRecoMuonDThetaWrtStraightVsPConstant", 0, 21, 1, false, false, hRecoMuonDThetaWrtStraightVsP_0);
  CreateAndWriteCanvas("hRecoMuonDThetaWrtStraightVsPMean", 0, 21, 1, false, false, hRecoMuonDThetaWrtStraightVsP_1);
  CreateAndWriteCanvas("hRecoMuonDThetaWrtStraightVsPSigma", 0, 21, 1, false, false, hRecoMuonDThetaWrtStraightVsP_2);
  CreateAndWriteCanvas("hRecoMuonDThetaWrtStraightVsPChi2", 0, 21, 1, false, false, hRecoMuonDThetaWrtStraightVsP_3);
  
  TObjArray hSelMuonDThetaWrtStraightVsPSlices;
  hSelMuonDThetaWrtStraightVsP->FitSlicesY(0, 0, -1, 0, "QNR", &hSelMuonDThetaWrtStraightVsPSlices);
  TH1D *hSelMuonDThetaWrtStraightVsP_0 = (TH1D*)hSelMuonDThetaWrtStraightVsPSlices.At(0);
  TH1D *hSelMuonDThetaWrtStraightVsP_1 = (TH1D*)hSelMuonDThetaWrtStraightVsPSlices.At(1);
  TH1D *hSelMuonDThetaWrtStraightVsP_2 = (TH1D*)hSelMuonDThetaWrtStraightVsPSlices.At(2);
  TH1D *hSelMuonDThetaWrtStraightVsP_3 = (TH1D*)hSelMuonDThetaWrtStraightVsPSlices.At(3);
  CreateAndWriteCanvas("hSelMuonDThetaWrtStraightVsPConstant", 0, 21, 1, false, false, hSelMuonDThetaWrtStraightVsP_0);
  CreateAndWriteCanvas("hSelMuonDThetaWrtStraightVsPMean", 0, 21, 1, false, false, hSelMuonDThetaWrtStraightVsP_1);
  CreateAndWriteCanvas("hSelMuonDThetaWrtStraightVsPSigma", 0, 21, 1, false, false, hSelMuonDThetaWrtStraightVsP_2);
  CreateAndWriteCanvas("hSelMuonDThetaWrtStraightVsPChi2", 0, 21, 1, false, false, hSelMuonDThetaWrtStraightVsP_3);
  
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



std::vector<double> MuonBasicDistributionsDataForBmm::GetGenBinningVariablesValues(const unsigned int iGenParticle)
{
  std::vector<double> result;
  result.push_back(1e100);
  result.push_back(Genp_pt->at(iGenParticle));
  result.push_back(fabs(Genp_eta->at(iGenParticle)));
  result.push_back(Genp_charge->at(iGenParticle));
  return result;
}



#endif // MuonBasicDistributionsDataForBmm_cxx
