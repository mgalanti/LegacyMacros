#ifndef MuonBasicDistributionsMC_cxx
#define MuonBasicDistributionsMC_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include <sstream>

#include "MuonBasicDistributionsMC.h"
#include "../utilities/constants.h"

void MuonBasicDistributionsMC::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  const std::string effMethod = "MCTruthBB";
  
  // Book histograms here
  // Distributions for each muon category
  std::map<short int,TH1D*> mMuonGenpRProd;
  std::map<short int,TH1D*> mMuonGenpZProd;
  std::map<short int,TH2D*> mMuonGenpRVsZProd;
  std::map<short int,TH1D*> mMuonGenStatus;
  std::map<short int,TH1D*> mMuonPt;
  std::map<short int,TH1D*> mMuonPtEffCor;
  std::map<short int,TH1D*> mMuonEta;
  std::map<short int,TH1D*> mMuonEtaEffCor;
  std::map<short int,TH1D*> mMuonPhi;
  std::map<short int,TH1D*> mMuonPhiEffCor;
  std::map<short int,TH1D*> mMuonCharge;
  std::map<short int,TH1D*> mMuonChargeEffCor;
  std::map<short int,TH1D*> mMuonCaloCompatibility;
  std::map<short int,TH1D*> mMuonCaloCompatibilityEffCor;
  std::map<short int,TH1D*> mMuonSegmentCompatibility;
  std::map<short int,TH1D*> mMuonSegmentCompatibilityEffCor;
  std::map<short int,TH1D*> mMuonTrackValidHits;
  std::map<short int,TH1D*> mMuonTrackValidHitsEffCor;
  std::map<short int,TH1D*> mMuonTrackValidPixelHits;
  std::map<short int,TH1D*> mMuonTrackValidPixelHitsEffCor;
  std::map<short int,TH1D*> mMuonTrackChi2;
  std::map<short int,TH1D*> mMuonTrackChi2EffCor;
  std::map<short int,TH1D*> mMuonTrackNdof;
  std::map<short int,TH1D*> mMuonTrackNdofEffCor;
  std::map<short int,TH1D*> mMuonGlobalChi2;
  std::map<short int,TH1D*> mMuonGlobalChi2EffCor;
  std::map<short int,TH1D*> mMuonGlobalValidMuonHits;
  std::map<short int,TH1D*> mMuonGlobalValidMuonHitsEffCor;
  std::map<short int,TH1D*> mMuonNumberOfMatches;
  std::map<short int,TH1D*> mMuonNumberOfMatchesEffCor;
  std::stringstream oss;
  std::string name,title;
  for(short int i = -25; i < 26; i++)
  {
    oss.str("");
    oss << "hMuonGenpRProd_" << i;
    name = oss.str();
    oss.str("");
    oss << "Prod. R for genp associated to muon category " << i;
    title = oss.str();
    TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),500,0.,500.,"R [cm]","N_{#mu}");
    mMuonGenpRProd[i] = histo;
    
    oss.str("");
    oss << "hMuonGenpZProd_" << i;
    name = oss.str();
    oss.str("");
    oss << "Prod. z for genp associated to muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),1000,-500.,500.,"z [cm]","N_{#mu}");
    mMuonGenpZProd[i] = histo;
    
    oss.str("");
    oss << "hMuonGenpRVsZProd_" << i;
    name = oss.str();
    oss.str("");
    oss << "Prod. R vs. z for genp associated to muon category " << i;
    title = oss.str();
    TH2D* histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(),1000,-500.,500.,500,0.,500.,"z [cm]","R [cm]");
    mMuonGenpRVsZProd[i] = histo2D;
    
    oss.str("");
    oss << "hMuonGenStatus_" << i;
    name = oss.str();
    oss.str("");
    oss << "Status of genp associated to muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"Status","N_{#mu}");
    mMuonGenStatus[i] = histo;
    
    oss.str("");
    oss << "hMuonPt_" << i;
    name = oss.str();
    oss.str("");
    oss << "p_{T} for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,30.,"p_{T} [GeV/c]","N_{#mu}");
    mMuonPt[i] = histo;

    oss.str("");
    oss << "hMuonPtEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "p_{T} (eff.corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,30.,"p_{T} [GeV/c]","N_{#mu}");
    mMuonPtEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonEta_" << i;
    name = oss.str();
    oss.str("");
    oss << "#eta for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-3.,3.,"#eta","N_{#mu}");
    mMuonEta[i] = histo;
    
    oss.str("");
    oss << "hMuonEtaEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "#eta (eff.corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-3.,3.,"#eta","N_{#mu}");
    mMuonEtaEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonPhi_" << i;
    name = oss.str();
    oss.str("");
    oss << "#phi for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-constants::PI,constants::PI,"#phi","N_{#mu}");
    mMuonPhi[i] = histo;
    
    oss.str("");
    oss << "hMuonPhiEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "#phi (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-constants::PI,constants::PI,"#phi","N_{#mu}");
    mMuonPhiEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonCharge_" << i;
    name = oss.str();
    oss.str("");
    oss << "Charge for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),3,-1.5,1.5,"Charge","N_{#mu}");
    mMuonCharge[i] = histo;
    
    oss.str("");
    oss << "hMuonChargeEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "Charge (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),3,-1.5,1.5,"Charge","N_{#mu}");
    mMuonChargeEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonCaloCompatibility_" << i;
    name = oss.str();
    oss.str("");
    oss << "Calo compatibility for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),100,0.,1.,"Calo compatibility","N_{#mu}");
    mMuonCaloCompatibility[i] = histo;
    
    oss.str("");
    oss << "hMuonCaloCompatibilityEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "Calo compatibility (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),100,0.,1.,"Calo compatibility","N_{#mu}");
    mMuonCaloCompatibilityEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonSegmentCompatibility_" << i;
    name = oss.str();
    oss.str("");
    oss << "Segment compatibility for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),100,0.,1.,"Segment compatibility","N_{#mu}");
    mMuonSegmentCompatibility[i] = histo;
    
    oss.str("");
    oss << "hMuonSegmentCompatibilityEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "Segment compatibility (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),100,0.,1.,"Segment compatibility","N_{#mu}");
    mMuonSegmentCompatibilityEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackValidHits_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid hits in inner track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),41,-0.5,40.5,"N_{hits}","N_{#mu}");
    mMuonTrackValidHits[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackValidHitsEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid hits in inner track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),41,-0.5,40.5,"N_{hits}","N_{#mu}");
    mMuonTrackValidHitsEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackChi2_" << i;
    name = oss.str();
    oss.str("");
    oss << "#chi^{2}/ndof in inner track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,15.,"#chi^{2}/ndof","N_{#mu}");
    mMuonTrackChi2[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackChi2EffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "#chi^{2}/ndof in inner track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,15.,"#chi^{2}/ndof","N_{#mu}");
    mMuonTrackChi2EffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackNdof_" << i;
    name = oss.str();
    oss.str("");
    oss << "ndof of inner track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),51,-0.5,50.5,"ndof","N_{#mu}");
    mMuonTrackNdof[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackNdofEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "ndof of inner track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),51,-0.5,50.5,"ndof","N_{#mu}");
    mMuonTrackNdofEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackValidPixelHits_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid pixel hits in inner track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"N_{hits}","N_{#mu}");
    mMuonTrackValidPixelHits[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackValidPixelHitsEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid pixel hits in inner track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"N_{hits}","N_{#mu}");
    mMuonTrackValidPixelHitsEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonGlobalChi2_" << i;
    name = oss.str();
    oss.str("");
    oss << "#chi^{2}/ndof in global track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,15.,"#chi^{2}/ndof","N_{#mu}");
    mMuonGlobalChi2[i] = histo;
    
    oss.str("");
    oss << "hMuonGlobalChi2EffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "#chi^{2}/ndof in global track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,15.,"#chi^{2}/ndof","N_{#mu}");
    mMuonGlobalChi2EffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonGlobalValidMuonHits_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid muon hits in global track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),81,-0.5,80.5,"N_{hits}","N_{#mu}");
    mMuonGlobalValidMuonHits[i] = histo;
    
    oss.str("");
    oss << "hMuonGlobalValidMuonHitsEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid muon hits in global track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),81,-0.5,80.5,"N_{hits}","N_{#mu}");
    mMuonGlobalValidMuonHitsEffCor[i] = histo;
    
    oss.str("");
    oss << "hMuonNumberOfMatches_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of matches for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"N_{matches}","N_{#mu}");
    mMuonNumberOfMatches[i] = histo;

    oss.str("");
    oss << "hMuonNumberOfMatchesEffCor_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of matches (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"N_{matches}","N_{#mu}");
    mMuonNumberOfMatchesEffCor[i] = histo;
  }

  // Distributions separated by Simplified Code
  std::map<short int,TH1D*> mMuonGenpRProdSC;
  std::map<short int,TH1D*> mMuonGenpZProdSC;
  std::map<short int,TH2D*> mMuonGenpRVsZProdSC;
  std::map<short int,TH1D*> mMuonGenStatusSC;
  std::map<short int,TH1D*> mMuonPtSC;
  std::map<short int,TH1D*> mMuonPtEffCorSC;
  std::map<short int,TH1D*> mMuonEtaSC;
  std::map<short int,TH1D*> mMuonEtaEffCorSC;
  std::map<short int,TH1D*> mMuonPhiSC;
  std::map<short int,TH1D*> mMuonPhiEffCorSC;
  std::map<short int,TH1D*> mMuonChargeSC;
  std::map<short int,TH1D*> mMuonChargeEffCorSC;
  std::map<short int,TH1D*> mMuonCaloCompatibilitySC;
  std::map<short int,TH1D*> mMuonCaloCompatibilityEffCorSC;
  std::map<short int,TH1D*> mMuonSegmentCompatibilitySC;
  std::map<short int,TH1D*> mMuonSegmentCompatibilityEffCorSC;
  std::map<short int,TH1D*> mMuonTrackValidHitsSC;
  std::map<short int,TH1D*> mMuonTrackValidHitsEffCorSC;
  std::map<short int,TH1D*> mMuonTrackValidPixelHitsSC;
  std::map<short int,TH1D*> mMuonTrackValidPixelHitsEffCorSC;
  std::map<short int,TH1D*> mMuonTrackChi2SC;
  std::map<short int,TH1D*> mMuonTrackChi2EffCorSC;
  std::map<short int,TH1D*> mMuonTrackNdofSC;
  std::map<short int,TH1D*> mMuonTrackNdofEffCorSC;
  std::map<short int,TH1D*> mMuonGlobalChi2SC;
  std::map<short int,TH1D*> mMuonGlobalChi2EffCorSC;
  std::map<short int,TH1D*> mMuonGlobalValidMuonHitsSC;
  std::map<short int,TH1D*> mMuonGlobalValidMuonHitsEffCorSC;
  std::map<short int,TH1D*> mMuonNumberOfMatchesSC;
  std::map<short int,TH1D*> mMuonNumberOfMatchesEffCorSC;
  
  std::map<short int,TH1D*> mMuonDxySC;
  std::map<short int,TH1D*> mMuonDxyEffCorSC;
  std::map<short int,TH1D*> mMuonDxySFCorSC;
  std::map<short int,TH1D*> mMuonDxyD6TCorSC;
  
  for(short int i = 0; i < 7; i++)
  {
    oss.str("");
    oss << "hMuonGenpRProdSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Prod. R for genp associated to muon category " << i;
    title = oss.str();
    TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),500,0.,500.,"R [cm]","N_{#mu}");
    mMuonGenpRProdSC[i] = histo;

    oss.str("");
    oss << "hMuonGenpZProdSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Prod. z for genp associated to muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),1000,-500.,500.,"z [cm]","N_{#mu}");
    mMuonGenpZProdSC[i] = histo;
    
    oss.str("");
    oss << "hMuonGenpRVsZProdSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Prod. R vs. z for genp associated to muon category " << i;
    title = oss.str();
    TH2D* histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(),1000,-500.,500.,500,0.,500.,"z [cm]","R [cm]");
    mMuonGenpRVsZProdSC[i] = histo2D;
    
    oss.str("");
    oss << "hMuonGenStatusSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Status of genp associated to muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"Status","N_{#mu}");
    mMuonGenStatusSC[i] = histo;
    
    oss.str("");
    oss << "hMuonPtSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "p_{T} for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,30.,"p_{T} [GeV/c]","N_{#mu}");
    mMuonPtSC[i] = histo;
    
    oss.str("");
    oss << "hMuonPtEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "p_{T} (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,30.,"p_{T} [GeV/c]","N_{#mu}");
    mMuonPtEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonEtaSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "#eta for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-3.,3.,"#eta","N_{#mu}");
    mMuonEtaSC[i] = histo;
    
    oss.str("");
    oss << "hMuonEtaEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "#eta (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-3.,3.,"#eta","N_{#mu}");
    mMuonEtaEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonPhiSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "#phi for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-constants::PI,constants::PI,"#phi","N_{#mu}");
    mMuonPhiSC[i] = histo;
    
    oss.str("");
    oss << "hMuonPhiEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "#phi (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-constants::PI,constants::PI,"#phi","N_{#mu}");
    mMuonPhiEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonChargeSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Charge for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),3,-1.5,1.5,"Charge","N_{#mu}");
    mMuonChargeSC[i] = histo;
    
    oss.str("");
    oss << "hMuonChargeEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Charge (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),3,-1.5,1.5,"Charge","N_{#mu}");
    mMuonChargeEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonCaloCompatibilitySC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Calo compatibility for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),100,0.,1.,"Calo compatibility","N_{#mu}");
    mMuonCaloCompatibilitySC[i] = histo;
    
    oss.str("");
    oss << "hMuonCaloCompatibilityEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Calo compatibility (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),100,0.,1.,"Calo compatibility","N_{#mu}");
    mMuonCaloCompatibilityEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonSegmentCompatibilitySC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Segment compatibility for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),100,0.,1.,"Segment compatibility","N_{#mu}");
    mMuonSegmentCompatibilitySC[i] = histo;
    
    oss.str("");
    oss << "hMuonSegmentCompatibilityEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "Segment compatibility (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),100,0.,1.,"Segment compatibility","N_{#mu}");
    mMuonSegmentCompatibilityEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackValidHitsSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid hits in inner track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),41,-0.5,40.5,"N_{hits}","N_{#mu}");
    mMuonTrackValidHitsSC[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackValidHitsEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid hits in inner track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),41,-0.5,40.5,"N_{hits}","N_{#mu}");
    mMuonTrackValidHitsEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackChi2SC_" << i;
    name = oss.str();
    oss.str("");
    oss << "#chi^{2}/ndof in inner track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,15.,"#chi^{2}/ndof","N_{#mu}");
    mMuonTrackChi2SC[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackChi2EffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "#chi^{2}/ndof in inner track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,15.,"#chi^{2}/ndof","N_{#mu}");
    mMuonTrackChi2EffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackNdofSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "ndof of inner track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),51,-0.5,50.5,"ndof","N_{#mu}");
    mMuonTrackNdofSC[i] = histo;

    oss.str("");
    oss << "hMuonTrackNdofEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "ndof of inner track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),51,-0.5,50.5,"ndof","N_{#mu}");
    mMuonTrackNdofEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonTrackValidPixelHitsSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid pixel hits in inner track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"N_{hits}","N_{#mu}");
    mMuonTrackValidPixelHitsSC[i] = histo;
   
    oss.str("");
    oss << "hMuonTrackValidPixelHitsEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid pixel hits in inner track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"N_{hits}","N_{#mu}");
    mMuonTrackValidPixelHitsEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonGlobalChi2SC_" << i;
    name = oss.str();
    oss.str("");
    oss << "#chi^{2}/ndof in global track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,15.,"#chi^{2}/ndof","N_{#mu}");
    mMuonGlobalChi2SC[i] = histo;
    
    oss.str("");
    oss << "hMuonGlobalChi2EffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "#chi^{2}/ndof in global track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,15.,"#chi^{2}/ndof","N_{#mu}");
    mMuonGlobalChi2EffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonGlobalValidMuonHitsSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid muon hits in global track for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),81,-0.5,80.5,"N_{hits}","N_{#mu}");
    mMuonGlobalValidMuonHitsSC[i] = histo;
    
    oss.str("");
    oss << "hMuonGlobalValidMuonHitsEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of valid muon hits in global track (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),81,-0.5,80.5,"N_{hits}","N_{#mu}");
    mMuonGlobalValidMuonHitsEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonNumberOfMatchesSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of matches for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"N_{matches}","N_{#mu}");
    mMuonNumberOfMatchesSC[i] = histo;
  
    oss.str("");
    oss << "hMuonNumberOfMatchesEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "N. of matches (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),11,-0.5,10.5,"N_{matches}","N_{#mu}");
    mMuonNumberOfMatchesEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonDxySC_" << i;
    name = oss.str();
    oss.str("");
    oss << "d_{xy} for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),500,0.,0.5,"d_{xy} [cm]","N_{#mu}");
    mMuonDxySC[i] = histo;
    
    oss.str("");
    oss << "hMuonDxyEffCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "d_{xy} (eff. corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),500,0.,0.5,"d_{xy} [cm]","N_{#mu}");
    mMuonDxyEffCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonDxySFCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "d_{xy} (SF corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),500,0.,0.5,"d_{xy} [cm]","N_{#mu}");
    mMuonDxySFCorSC[i] = histo;
    
    oss.str("");
    oss << "hMuonDxyD6TCorSC_" << i;
    name = oss.str();
    oss.str("");
    oss << "d_{xy} (D6T/Z2 corr.) for muon category " << i;
    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),500,0.,0.5,"d_{xy} [cm]","N_{#mu}");
    mMuonDxyD6TCorSC[i] = histo;
  }
  
  std::map<short int,TH1D*> mMuonLeadingPtEvCode;
  std::map<short int,TH1D*> mMuonLeadingPtEffCorEvCode;
  std::map<short int,TH1D*> mMuonSecondPtEvCode;
  std::map<short int,TH1D*> mMuonSecondPtEffCorEvCode;
  std::map<short int,TH1D*> mMuonLeadingEtaEvCode;
  std::map<short int,TH1D*> mMuonLeadingEtaEffCorEvCode;
  std::map<short int,TH1D*> mMuonSecondEtaEvCode;
  std::map<short int,TH1D*> mMuonSecondEtaEffCorEvCode;
  std::map<short int,TH1D*> mDiMuonInvMassEvCode;
  std::map<short int,TH1D*> mDiMuonInvMassEffCorEvCode;
  for(unsigned int i = 1; i < 5; i++)
  {
    for(unsigned int j = i; j < 5; j++)
    {
      unsigned int evCode = i*10 + j;

      oss.str("");
      oss << "hMuonLeadingPtEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "p_{T} of leading muon for event code " << evCode;
      title = oss.str();
      TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,30.,"p_{T} [GeV/c]","N_{#mu}");
      mMuonLeadingPtEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hMuonLeadingPtEffCorEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "p_{T} (eff. corr.) of leading muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,30.,"p_{T} [GeV/c]","N_{#mu}");
      mMuonLeadingPtEffCorEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hMuonSecondPtEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "p_{T} of second-leading muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,30.,"p_{T} [GeV/c]","N_{#mu}");
      mMuonSecondPtEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hMuonSecondPtEffCorEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "p_{T} (eff. corr.) of second-leading muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),150,0.,30.,"p_{T} [GeV/c]","N_{#mu}");
      mMuonSecondPtEffCorEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hMuonLeadingEtaEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "#eta of leading p_{T} muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-3.,3.,"#eta","N_{#mu}");
      mMuonLeadingEtaEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hMuonLeadingEtaEffCorEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "#eta (eff. corr.) of leading p_{T} muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-3.,3.,"#eta","N_{#mu}");
      mMuonLeadingEtaEffCorEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hMuonSecondEtaEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "#eta of second-leading p_{T} muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-3.,3.,"#eta","N_{#mu}");
      mMuonSecondEtaEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hMuonSecondEtaEffCorEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "#eta (eff. corr.) of second-leading p_{T} muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),60,-3.,3.,"#eta","N_{#mu}");
      mMuonSecondEtaEffCorEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hDiMuonInvMassEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "Invariant mass of selected di-muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),500,0.,100.,"M_{#mu#mu} [GeV/c^{2}]","N_{#mu#mu}");
      mDiMuonInvMassEvCode[evCode] = histo;
      
      oss.str("");
      oss << "hDiMuonInvMassEffCorEvCode_" << evCode;
      name = oss.str();
      oss.str("");
      oss << "Invariant mass (eff. corr.) of selected di-muon for event code " << evCode;
      title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(),500,0.,100.,"M_{#mu#mu} [GeV/c^{2}]","N_{#mu#mu}");
      mDiMuonInvMassEffCorEvCode[evCode] = histo;
    }
  }
  
  TLorentzVector pMu0;
  TLorentzVector pMu1;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    //     std::cout << "Event " << evNumber << std::endl;
    std::vector<unsigned short> vSelectedMuons;
    unsigned short iSelectedVertex;
    if(!s->SelectEventWithDiMuonV0(text.c_str(), vSelectedMuons, iSelectedVertex))
    {
      //       std::cout << "Event " << jentry << " not selected!\n";
      continue;
    }
    //     std::cout << "Event " << jentry << " selected!\n";
    std::vector<short int> muonClass;
    std::vector<unsigned short> muonInAssocGenp;
    std::vector<unsigned short> muonOutAssocGenp;
    std::vector<unsigned short> muonGlbAssocGenp;
    std::vector<short int> muonMCProvenance;
    std::vector<short int> muonMergedCode;
    std::vector<short int> muonSimplifiedCode0;
    std::vector<short int> muonSimplifiedCode;
    std::vector<double> vEffMuon;
    std::vector<double> vEffMuonData;
    std::vector<double> vSFMuon;
    if(Muon_pt->at(vSelectedMuons[0]) < Muon_pt->at(vSelectedMuons[1]))
    {
      std::cout << "E R R O R : Muons are not pt-ordered!\n";
      exit(1);
    }
    
    for(unsigned short i = 0; i < vSelectedMuons.size(); i++)
    {
      unsigned short iMuon = vSelectedMuons[i];
      unsigned short iTrack = Muon_trackref->at(iMuon);

      double effMuon = GetGlobalEfficiency(Muon_pt->at(iMuon), Muon_eta->at(iMuon), effMethod);
      double effMuonData = GetGlobalEfficiency(Muon_pt->at(iMuon), Muon_eta->at(iMuon), "Data");
       // Protection against infinities...
      if(effMuon == 0.)
        effMuon = 1.;
      if(effMuonData == 0.)
        effMuonData = 1.;
      double sfMuon = effMuon/effMuonData;
      vEffMuon.push_back(effMuon);
      vEffMuonData.push_back(effMuonData);
      vSFMuon.push_back(sfMuon);
      
      muonClass.push_back(FindMuonClassFromMCAssociation(iMuon));
      muonInAssocGenp.push_back(muonInTrkGenp);
      muonOutAssocGenp.push_back(muonOutTrkGenp);
      muonGlbAssocGenp.push_back(muonGlbTrkGenp);
      muonMCProvenance.push_back(FindMuonMCProvenance(iMuon,muonClass[i]));
      muonMergedCode.push_back(MergeMuonClassAndMCProvenance(muonClass[i],muonMCProvenance[i]));
      muonSimplifiedCode0.push_back(FindSimplifiedMuonCode(muonMergedCode[i]));
      muonSimplifiedCode.push_back(FindSimplifiedMergedMuonCode(muonSimplifiedCode0[i]));
      //       std::cout << "muonClass[" << i << "] = " << muonClass[i] << std::endl;
      //       std::cout << "muonMCProvenance[" << i << "] = " << muonMCProvenance[i] << std::endl;
      //       std::cout << "muonMergedCode[" << i << "] = " << muonMergedCode[i] << std::endl;
      //       std::cout << "muonSimplifiedCode[" << i << "] = " << muonSimplifiedCode[i] << std::endl;

      unsigned short iGenp = muonInAssocGenp[i];
      if(iGenp == 65535 || (iGenp != 65535 && abs(Genp_Id->at(iGenp)) != 13))
      {
        if(muonOutAssocGenp[i] != 65535 && abs(Genp_Id->at(muonOutAssocGenp[i])) == 13)
        {
          iGenp = muonOutAssocGenp[i];
        }
      }
      if(iGenp != 65535)
      {
        double genpRProd = sqrt(Genp_vx->at(iGenp)*Genp_vx->at(iGenp) + Genp_vy->at(iGenp)*Genp_vy->at(iGenp));
        double genpZProd = Genp_vz->at(iGenp);
        mMuonGenpRProd[muonMergedCode[i]]->Fill(genpRProd);
        mMuonGenpZProd[muonMergedCode[i]]->Fill(genpZProd);
        mMuonGenpRVsZProd[muonMergedCode[i]]->Fill(genpZProd,genpRProd);
        mMuonGenStatus[muonMergedCode[i]]->Fill(Genp_status->at(iGenp));
        mMuonGenpRProdSC[muonSimplifiedCode[i]]->Fill(genpRProd);
        mMuonGenpZProdSC[muonSimplifiedCode[i]]->Fill(genpZProd);
        mMuonGenpRVsZProdSC[muonSimplifiedCode[i]]->Fill(genpZProd,genpRProd);
        mMuonGenStatusSC[muonSimplifiedCode[i]]->Fill(Genp_status->at(iGenp));
      }
      
      mMuonPt[muonMergedCode[i]]->Fill(Muon_pt->at(iMuon));
      mMuonPtEffCor[muonMergedCode[i]]->Fill(Muon_pt->at(iMuon), 1./effMuon);
      mMuonEta[muonMergedCode[i]]->Fill(Muon_eta->at(iMuon));
      mMuonEtaEffCor[muonMergedCode[i]]->Fill(Muon_eta->at(iMuon), 1./effMuon);
      mMuonPhi[muonMergedCode[i]]->Fill(Muon_phi->at(iMuon));
      mMuonPhiEffCor[muonMergedCode[i]]->Fill(Muon_phi->at(iMuon), 1./effMuon);
      mMuonCharge[muonMergedCode[i]]->Fill(Muon_charge->at(iMuon));
      mMuonChargeEffCor[muonMergedCode[i]]->Fill(Muon_charge->at(iMuon), 1./effMuon);
      mMuonCaloCompatibility[muonMergedCode[i]]->Fill(Muon_caloCompatibility->at(iMuon));
      mMuonCaloCompatibilityEffCor[muonMergedCode[i]]->Fill(Muon_caloCompatibility->at(iMuon), 1./effMuon);
      mMuonSegmentCompatibility[muonMergedCode[i]]->Fill(Muon_segmentCompatibility->at(iMuon));
      mMuonSegmentCompatibilityEffCor[muonMergedCode[i]]->Fill(Muon_segmentCompatibility->at(iMuon), 1./effMuon);
      mMuonTrackValidHits[muonMergedCode[i]]->Fill(Track_numberOfValidHits->at(iTrack));
      mMuonTrackValidHitsEffCor[muonMergedCode[i]]->Fill(Track_numberOfValidHits->at(iTrack), 1./effMuon);
      mMuonTrackValidPixelHits[muonMergedCode[i]]->Fill(Track_validPixelHits->at(iTrack));
      mMuonTrackValidPixelHitsEffCor[muonMergedCode[i]]->Fill(Track_validPixelHits->at(iTrack), 1./effMuon);
      mMuonTrackChi2[muonMergedCode[i]]->Fill(Track_normalizedChi2->at(iTrack));
      mMuonTrackChi2EffCor[muonMergedCode[i]]->Fill(Track_normalizedChi2->at(iTrack), 1./effMuon);
      mMuonTrackNdof[muonMergedCode[i]]->Fill(Track_ndof->at(iTrack));
      mMuonTrackNdofEffCor[muonMergedCode[i]]->Fill(Track_ndof->at(iTrack), 1./effMuon);
      mMuonGlobalChi2[muonMergedCode[i]]->Fill(Muon_globalChi2->at(iMuon));
      mMuonGlobalChi2EffCor[muonMergedCode[i]]->Fill(Muon_globalChi2->at(iMuon), 1./effMuon);
      mMuonGlobalValidMuonHits[muonMergedCode[i]]->Fill(Muon_globalValidMuonHits->at(iMuon));
      mMuonGlobalValidMuonHitsEffCor[muonMergedCode[i]]->Fill(Muon_globalValidMuonHits->at(iMuon), 1./effMuon);
      mMuonNumberOfMatches[muonMergedCode[i]]->Fill(Muon_numberOfMatches->at(iMuon));
      mMuonNumberOfMatchesEffCor[muonMergedCode[i]]->Fill(Muon_numberOfMatches->at(iMuon), 1./effMuon);
      mMuonPtSC[muonSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon));
      mMuonPtEffCorSC[muonSimplifiedCode[i]]->Fill(Muon_pt->at(iMuon), 1./effMuon);
      mMuonEtaSC[muonSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon));
      mMuonEtaEffCorSC[muonSimplifiedCode[i]]->Fill(Muon_eta->at(iMuon), 1./effMuon);
      mMuonPhiSC[muonSimplifiedCode[i]]->Fill(Muon_phi->at(iMuon));
      mMuonPhiEffCorSC[muonSimplifiedCode[i]]->Fill(Muon_phi->at(iMuon), 1./effMuon);
      mMuonChargeSC[muonSimplifiedCode[i]]->Fill(Muon_charge->at(iMuon));
      mMuonChargeEffCorSC[muonSimplifiedCode[i]]->Fill(Muon_charge->at(iMuon), 1./effMuon);
      mMuonCaloCompatibilitySC[muonSimplifiedCode[i]]->Fill(Muon_caloCompatibility->at(iMuon));
      mMuonCaloCompatibilityEffCorSC[muonSimplifiedCode[i]]->Fill(Muon_caloCompatibility->at(iMuon), 1./effMuon);
      mMuonSegmentCompatibilitySC[muonSimplifiedCode[i]]->Fill(Muon_segmentCompatibility->at(iMuon));
      mMuonSegmentCompatibilityEffCorSC[muonSimplifiedCode[i]]->Fill(Muon_segmentCompatibility->at(iMuon), 1./effMuon);
      mMuonTrackValidHitsSC[muonSimplifiedCode[i]]->Fill(Track_numberOfValidHits->at(iTrack));
      mMuonTrackValidHitsEffCorSC[muonSimplifiedCode[i]]->Fill(Track_numberOfValidHits->at(iTrack), 1./effMuon);
      mMuonTrackValidPixelHitsSC[muonSimplifiedCode[i]]->Fill(Track_validPixelHits->at(iTrack));
      mMuonTrackValidPixelHitsEffCorSC[muonSimplifiedCode[i]]->Fill(Track_validPixelHits->at(iTrack), 1./effMuon);
      mMuonTrackChi2SC[muonSimplifiedCode[i]]->Fill(Track_normalizedChi2->at(iTrack));
      mMuonTrackChi2EffCorSC[muonSimplifiedCode[i]]->Fill(Track_normalizedChi2->at(iTrack), 1./effMuon);
      mMuonTrackNdofSC[muonSimplifiedCode[i]]->Fill(Track_ndof->at(iTrack));
      mMuonTrackNdofEffCorSC[muonSimplifiedCode[i]]->Fill(Track_ndof->at(iTrack), 1./effMuon);
      mMuonGlobalChi2SC[muonSimplifiedCode[i]]->Fill(Muon_globalChi2->at(iMuon));
      mMuonGlobalChi2EffCorSC[muonSimplifiedCode[i]]->Fill(Muon_globalChi2->at(iMuon), 1./effMuon);
      mMuonGlobalValidMuonHitsSC[muonSimplifiedCode[i]]->Fill(Muon_globalValidMuonHits->at(iMuon));
      mMuonGlobalValidMuonHitsEffCorSC[muonSimplifiedCode[i]]->Fill(Muon_globalValidMuonHits->at(iMuon), 1./effMuon);
      mMuonNumberOfMatchesSC[muonSimplifiedCode[i]]->Fill(Muon_numberOfMatches->at(iMuon));
      mMuonNumberOfMatchesEffCorSC[muonSimplifiedCode[i]]->Fill(Muon_numberOfMatches->at(iMuon), 1./effMuon);
      
      mMuonDxySC[muonSimplifiedCode[i]]->Fill(Track_dxy_PV->at(iTrack).at(iSelectedVertex));
      mMuonDxyEffCorSC[muonSimplifiedCode[i]]->Fill(Track_dxy_PV->at(iTrack).at(iSelectedVertex), 1./effMuon);
      mMuonDxySFCorSC[muonSimplifiedCode[i]]->Fill(Track_dxy_PV->at(iTrack).at(iSelectedVertex), 1./sfMuon);
      
      if(muonInAssocGenp[i] != 65535)
      {
        double genpDxy = Genp_dxy->at(muonInAssocGenp[i]);
        if(genpDxy == 65535)
        {
          std::cout << "E R R O R !\n";
        }
        double weight = DxyD6TScaling(genpDxy);
        mMuonDxyD6TCorSC[muonSimplifiedCode[i]]->Fill(Track_dxy_PV->at(iTrack).at(iSelectedVertex), weight);
      }
      else
      {
        mMuonDxyD6TCorSC[muonSimplifiedCode[i]]->Fill(0.499);
      }
    }
    
    double muonLeadingPt = 0;
    double muonSecondPt = 0;
    double muonLeadingEta = 0;
    double muonSecondEta = 0;
    
    if(Muon_pt->at(vSelectedMuons[0]) > Muon_pt->at(vSelectedMuons[1]))
    {
      muonLeadingPt = Muon_pt->at(vSelectedMuons[0]);
      muonSecondPt = Muon_pt->at(vSelectedMuons[1]);
      muonLeadingEta = Muon_eta->at(vSelectedMuons[0]);
      muonSecondEta = Muon_eta->at(vSelectedMuons[1]);
    }
    else
    {
      muonLeadingPt = Muon_pt->at(vSelectedMuons[1]);
      muonSecondPt = Muon_pt->at(vSelectedMuons[0]);
      muonLeadingEta = Muon_eta->at(vSelectedMuons[1]);
      muonSecondEta = Muon_eta->at(vSelectedMuons[0]);
    }
    
    pMu0.SetPtEtaPhiM(Muon_pt->at(vSelectedMuons[0]), Muon_eta->at(vSelectedMuons[0]), Muon_phi->at(vSelectedMuons[0]), constants::muonMass);
    pMu1.SetPtEtaPhiM(Muon_pt->at(vSelectedMuons[1]), Muon_eta->at(vSelectedMuons[1]), Muon_phi->at(vSelectedMuons[1]), constants::muonMass);
    
    double diMuonInvMass = (pMu0 + pMu1).M();
    
    unsigned int evCode = 0;
    unsigned int muoncode1 = std::min(muonSimplifiedCode[0],muonSimplifiedCode[1]);
    unsigned int muoncode2 = std::max(muonSimplifiedCode[0],muonSimplifiedCode[1]);
    
    // Merge the 2 DIF classes
    if(muoncode1 == 5)
      muoncode1 = 4;
    if(muoncode2 == 5)
      muoncode2 = 4;
    
    if(muoncode1 == 0 || muoncode2 == 0 || muoncode1 > 4 || muoncode2 > 4)
    {
      evCode = 99;
    }
    else
    {
      evCode = muoncode1*10 + muoncode2;
    }
    if(evCode != 99)
    {
      // Yes, I know... but it already halts if pt(mu0) < pt(mu1), so it should be safe...
      mMuonLeadingPtEvCode[evCode]->Fill(muonLeadingPt);
      mMuonLeadingPtEffCorEvCode[evCode]->Fill(muonLeadingPt, 1./vEffMuon[0]);
      mMuonSecondPtEvCode[evCode]->Fill(muonSecondPt);
      mMuonSecondPtEffCorEvCode[evCode]->Fill(muonSecondPt, 1./vEffMuon[1]);
      mMuonLeadingEtaEvCode[evCode]->Fill(muonLeadingEta);
      mMuonLeadingEtaEffCorEvCode[evCode]->Fill(muonLeadingEta, 1./vEffMuon[0]);
      mMuonSecondEtaEvCode[evCode]->Fill(muonSecondEta);
      mMuonSecondEtaEffCorEvCode[evCode]->Fill(muonSecondEta, 1./vEffMuon[1]);
      
      mDiMuonInvMassEvCode[evCode]->Fill(diMuonInvMass);
      mDiMuonInvMassEffCorEvCode[evCode]->Fill(diMuonInvMass, 1./(vEffMuon[0]*vEffMuon[1]));
    }
  }

  // Keep this line here!
  outFile->cd();
  
  // Writes into the outFile the name of the efficiency file used in this run
  WriteEfficiencyMetadata(outFile);

  // Create and write canvases here
  std::string canvasName;
  for(short int i = -25; i < 26; i++)
  {
    oss.str("");
    oss << i;
    canvasName = "cMuonGenpRProd_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGenpRProd[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGenpZProd_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGenpZProd[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGenpRVsZProd_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(), "colz", false, false, true, mMuonGenpRVsZProd[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGenStatus_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonGenStatus[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonPt_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonPt[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonPtEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonPtEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonEta_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonEta[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonEtaEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonEtaEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonPhi_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonPhi[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonPhiEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonPhiEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonCharge_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonCharge[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonChargeEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonChargeEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonCaloCompatibility_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonCaloCompatibility[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonCaloCompatibilityEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonCaloCompatibilityEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonSegmentCompatibility_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonSegmentCompatibility[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonSegmentCompatibilityEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonSegmentCompatibilityEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackValidHits_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackValidHits[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackValidHitsEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackValidHitsEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackValidPixelHits_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackValidPixelHits[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackValidPixelHitsEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackValidPixelHitsEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackChi2_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackChi2[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackChi2EffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackChi2EffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackNdof_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackNdof[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackNdofEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackNdofEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGlobalChi2_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGlobalChi2[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGlobalChi2EffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGlobalChi2EffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGlobalValidMuonHits_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGlobalValidMuonHits[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGlobalValidMuonHitsEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGlobalValidMuonHitsEffCor[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonNumberOfMatches_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonNumberOfMatches[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonNumberOfMatchesEffCor_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonNumberOfMatchesEffCor[i]);
  }
  
  for(short int i = 0; i < 7; i++)
  {
    oss.str("");
    oss << i;
    canvasName = "cMuonGenpRProdSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGenpRProdSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGenpZProdSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGenpZProdSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGenpRVsZProdSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(), "colz", false, false, true, mMuonGenpRVsZProdSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGenStatusSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonGenStatusSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonPtSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonPtSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonPtEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonPtEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonEtaSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonEtaSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonEtaEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonEtaEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonPhiSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonPhiSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonPhiEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonPhiEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonChargeSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonChargeSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonChargeEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, false, mMuonChargeEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonCaloCompatibilitySC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonCaloCompatibilitySC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonCaloCompatibilityEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonCaloCompatibilityEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonSegmentCompatibilitySC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonSegmentCompatibilitySC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonSegmentCompatibilityEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonSegmentCompatibilityEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackValidHitsSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackValidHitsSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackValidHitsEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackValidHitsEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackValidPixelHitsSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackValidPixelHitsSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackValidPixelHitsEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackValidPixelHitsEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackChi2SC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackChi2SC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackChi2EffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackChi2EffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackNdofSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackNdofSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonTrackNdofEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonTrackNdofEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGlobalChi2SC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGlobalChi2SC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGlobalChi2EffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGlobalChi2EffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGlobalValidMuonHitsSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGlobalValidMuonHitsSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonGlobalValidMuonHitsEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonGlobalValidMuonHitsEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonNumberOfMatchesSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonNumberOfMatchesSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonNumberOfMatchesEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonNumberOfMatchesEffCorSC[i]);

    oss.str("");
    oss << i;
    canvasName = "cMuonDxySC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonDxySC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonDxyEffCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonDxyEffCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonDxySFCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonDxySFCorSC[i]);
    
    oss.str("");
    oss << i;
    canvasName = "cMuonDxyD6TCorSC_" + oss.str();
    CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonDxyD6TCorSC[i]);
  }
  
  for(unsigned int i = 1; i < 5; i++)
  {
    for(unsigned int j = i; j < 5; j++)
    {
      unsigned int evCode = i*10 + j;
      
      oss.str("");
      oss << evCode;
      canvasName = "cMuonLeadingPtEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonLeadingPtEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cMuonLeadingPtEffCorEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonLeadingPtEffCorEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cMuonSecondPtEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonSecondPtEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cMuonSecondPtEffCorEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonSecondPtEffCorEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cMuonLeadingEtaEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonLeadingEtaEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cMuonLeadingEtaEffCorEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonLeadingEtaEffCorEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cMuonSecondEtaEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonSecondEtaEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cMuonSecondEtaEffCorEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mMuonSecondEtaEffCorEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cDiMuonInvMassEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mDiMuonInvMassEvCode[evCode]);
      
      oss.str("");
      oss << evCode;
      canvasName = "cDiMuonInvMassEffCorEvCode_" + oss.str();
      CreateAndWriteCanvas(canvasName.c_str(),0, 21, 1, false, true, mDiMuonInvMassEffCorEvCode[evCode]);
    }
  }
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}



double MuonBasicDistributionsMC::DxyD6TScaling(double dxy)
{
  return 1./(0.958708+0.76623*pow(dxy,1./1.49011));
}



#endif // MuonBasicDistributionsMC_cxx
