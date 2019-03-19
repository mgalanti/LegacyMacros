#ifndef MuonDistributionsProducer_cxx
#define MuonDistributionsProducer_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "MuonDistributionsProducer.h"

#include "../utilities/constants.h"

void MuonDistributionsProducer::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Random number generator seed initialization
  rng.SetSeed(12345);
  
  // Book histograms here
  // IP
  TH2D* hMuonDxyz2D = Create2DHistogram<TH2D>("hMuonDxyz2D", "Symmetrized 2D distribution of d_{xyz}", 500, 0., 0.5, 500, 0., 0.5, "d_{xyz} [cm]", "d_{xyz} [cm]");
  TH2D* hMuonDxy2D = Create2DHistogram<TH2D>("hMuonDxy2D", "Symmetrized 2D distribution of d_{xy}", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonDxy2D_1um = Create2DHistogram<TH2D>("hMuonDxy2D_1um", "Symmetrized 2D distribution of d_{xy} - 1 #mu m smearing", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonDxy2D_3um = Create2DHistogram<TH2D>("hMuonDxy2D_3um", "Symmetrized 2D distribution of d_{xy} - 3 #mu m smearing", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonDxy2D_5um = Create2DHistogram<TH2D>("hMuonDxy2D_5um", "Symmetrized 2D distribution of d_{xy} - 5 #mu m smearing", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonDxy2D_8um = Create2DHistogram<TH2D>("hMuonDxy2D_8um", "Symmetrized 2D distribution of d_{xy} - 8 #mu m smearing", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonDxy2D_10um = Create2DHistogram<TH2D>("hMuonDxy2D_10um", "Symmetrized 2D distribution of d_{xy} - 10 #mu m smearing", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonDxy2D_12um = Create2DHistogram<TH2D>("hMuonDxy2D_12um", "Symmetrized 2D distribution of d_{xy} - 12 #mu m smearing", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonDxy2D_20um = Create2DHistogram<TH2D>("hMuonDxy2D_20um", "Symmetrized 2D distribution of d_{xy} - 20 #mu m smearing", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonDz2D = Create2DHistogram<TH2D>("hMuonDz2D", "Symmetrized 2D distribution of d_{z}",1000,-0.5,0.5,1000,-0.5,0.5,"d_{z} [cm]", "d_{z} [cm]");
  TH2D* hMuonOSDxyz2D = Create2DHistogram<TH2D>("hMuonOSDxyz2D", "Symmetrized 2D distribution of d_{xyz} for DS di-#mu", 500, 0., 0.5, 500, 0., 0.5, "d_{xyz} [cm]", "d_{xyz} [cm]");
  TH2D* hMuonOSDxy2D = Create2DHistogram<TH2D>("hMuonOSDxy2D", "Symmetrized 2D distribution of d_{xy} for DS di-#mu", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonOSDz2D = Create2DHistogram<TH2D>("hMuonOSDz2D", "Symmetrized 2D distribution of d_{z} for DS di-#mu",1000,-0.5,0.5,1000,-0.5,0.5,"d_{z} [cm]", "d_{z} [cm]");
  TH2D* hMuonSSDxyz2D = Create2DHistogram<TH2D>("hMuonSSDxyz2D", "Symmetrized 2D distribution of d_{xyz} for SS di-#mu", 500, 0., 0.5, 500, 0., 0.5, "d_{xyz} [cm]", "d_{xyz} [cm]");
  TH2D* hMuonSSDxy2D = Create2DHistogram<TH2D>("hMuonSSDxy2D", "Symmetrized 2D distribution of d_{xy} for SS di-#mu", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonSSDz2D = Create2DHistogram<TH2D>("hMuonSSDz2D", "Symmetrized 2D distribution of d_{z} for SS di-#mu",1000,-0.5,0.5,1000,-0.5,0.5,"d_{z} [cm]", "d_{z} [cm]");
  TH2D* hMuonSSPlusDxyz2D = Create2DHistogram<TH2D>("hMuonSSPlusDxyz2D", "Symmetrized 2D distribution of d_{xyz} for SSPlus di-#mu", 500, 0., 0.5, 500, 0., 0.5, "d_{xyz} [cm]", "d_{xyz} [cm]");
  TH2D* hMuonSSPlusDxy2D = Create2DHistogram<TH2D>("hMuonSSPlusDxy2D", "Symmetrized 2D distribution of d_{xy} for SSPlus di-#mu", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonSSPlusDz2D = Create2DHistogram<TH2D>("hMuonSSPlusDz2D", "Symmetrized 2D distribution of d_{z} for SSPlus di-#mu",1000,-0.5,0.5,1000,-0.5,0.5,"d_{z} [cm]", "d_{z} [cm]");
  TH2D* hMuonSSMinusDxyz2D = Create2DHistogram<TH2D>("hMuonSSMinusDxyz2D", "Symmetrized 2D distribution of d_{xyz} for SSMinus di-#mu", 500, 0., 0.5, 500, 0., 0.5, "d_{xyz} [cm]", "d_{xyz} [cm]");
  TH2D* hMuonSSMinusDxy2D = Create2DHistogram<TH2D>("hMuonSSMinusDxy2D", "Symmetrized 2D distribution of d_{xy} for SSMinus di-#mu", 500, 0., 0.5, 500, 0., 0.5,"d_{xy} [cm]", "d_{xy} [cm]");
  TH2D* hMuonSSMinusDz2D = Create2DHistogram<TH2D>("hMuonSSMinusDz2D", "Symmetrized 2D distribution of d_{z} for SSMinus di-#mu",1000,-0.5,0.5,1000,-0.5,0.5,"d_{z} [cm]", "d_{z} [cm]");
  TH1D* hMuonDxyz = Create1DHistogram<TH1D>("hMuonDxyz", "d_{xyz} distribution for #mu", 500,0.,0.5,"d_{xyz} [cm]", "N_{#mu}");
  TH1D* hMuonPlusDxyz = Create1DHistogram<TH1D>("hMuonPlusDxyz", "d_{xyz} distribution for #mu^{+}", 500,0.,0.5,"d_{xyz} [cm]", "N_{#mu}");
  TH1D* hMuonMinusDxyz = Create1DHistogram<TH1D>("hMuonMinusDxyz", "d_{xyz} distribution for #mu^{-}", 500,0.,0.5,"d_{xyz} [cm]", "N_{#mu}");
  TH1D* hMuon1Dxyz = Create1DHistogram<TH1D>("hMuon1Dxyz", "d_{xyz} distribution for #mu_{1}", 500,0.,0.5,"d_{xyz} [cm]", "N_{#mu}");
  TH1D* hMuon2Dxyz = Create1DHistogram<TH1D>("hMuon2Dxyz", "d_{xyz} distribution for #mu_{2}", 500,0.,0.5,"d_{xyz} [cm]", "N_{#mu}");
  TH1D* hMuonDxy = Create1DHistogram<TH1D>("hMuonDxy", "d_{xy} distribution for #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonPlusDxy = Create1DHistogram<TH1D>("hMuonPlusDxy", "d_{xy} distribution for #mu^{+}", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonMinusDxy = Create1DHistogram<TH1D>("hMuonMinusDxy", "d_{xy} distribution for #mu^{-}", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuon1Dxy = Create1DHistogram<TH1D>("hMuon1Dxy", "d_{xy} distribution for #mu_{1}", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuon2Dxy = Create1DHistogram<TH1D>("hMuon2Dxy", "d_{xy} distribution for #mu_{2}", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDz = Create1DHistogram<TH1D>("hMuonDz", "d_{z} distribution for #mu", 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
  TH1D* hMuonPlusDz = Create1DHistogram<TH1D>("hMuonPlusDz", "d_{z} distribution for #mu^{+}", 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
  TH1D* hMuonMinusDz = Create1DHistogram<TH1D>("hMuonMinusDz", "d_{z} distribution for #mu^{-}", 1000,-0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
  TH1D* hMuon1Dz = Create1DHistogram<TH1D>("hMuon1Dz", "d_{z} distribution for #mu_{1}", 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");
  TH1D* hMuon2Dz = Create1DHistogram<TH1D>("hMuon2Dz", "d_{z} distribution for #mu_{2}", 1000, -0.5, 0.5, "d_{z} [cm]", "N_{#mu}");

  TH1D* hMuonOSDxy = Create1DHistogram<TH1D>("hMuonOSDxy", "d_{xy} distribution for OS #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonSSDxy = Create1DHistogram<TH1D>("hMuonSSDxy", "d_{xy} distribution for SS #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonSSPlusDxy = Create1DHistogram<TH1D>("hMuonSSPlusDxy", "d_{xy} distribution for SSPlus #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonSSMinusDxy = Create1DHistogram<TH1D>("hMuonSSMinusDxy", "d_{xy} distribution for SSMinus #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");

  TH1D* hMuonDxyJ1015 = Create1DHistogram<TH1D>("hMuonDxyJ1015", "d_{xy} distribution for J1015 #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyJ1520 = Create1DHistogram<TH1D>("hMuonDxyJ1520", "d_{xy} distribution for J1520 #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyJ2030 = Create1DHistogram<TH1D>("hMuonDxyJ2030", "d_{xy} distribution for J2030 #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  TH1D* hMuonDxyJ30xx = Create1DHistogram<TH1D>("hMuonDxyJ30xx", "d_{xy} distribution for J30xx #mu", 500,0.,0.5,"d_{xy} [cm]", "N_{#mu}");
  
  
  // PtRel/PtOut
  TH2D* hMuonPtRelJet2D = Create2DHistogram<TH2D>("hMuonPtRelJet2D", "Symmetrized 2D distribution of p_{T}^{Rel} using jet", 500, 0., 5, 500, 0., 5, "p_{T}^{Rel} [GeV]", "p_{T}^{Rel} [GeV]");
  TH2D* hMuonPtRelSV2D = Create2DHistogram<TH2D>("hMuonPtRelSV2D", "Symmetrized 2D distribution of p_{T}^{Rel} using SV", 500, 0., 5, 500, 0., 5, "p_{T}^{Rel} [GeV]", "p_{T}^{Rel} [GeV]");
  TH2D* hMuonPtRelSVorJet2D = Create2DHistogram<TH2D>("hMuonPtRelSVorJet2D", "Symmetrized 2D distribution of p_{T}^{Rel} using SV or jet", 500, 0., 5, 500, 0., 5, "p_{T}^{Rel} [GeV]", "p_{T}^{Rel} [GeV]");
  TH2D* hMuonOSPtRelJet2D = Create2DHistogram<TH2D>("hMuonOSPtRelJet2D", "Symmetrized 2D distribution of p_{T}^{Rel} for DS di-#mu", 500, 0., 5, 500, 0., 5, "p_{T}^{Rel} [GeV]", "p_{T}^{Rel} [GeV]");
  TH2D* hMuonSSMinusPtRelJet2D = Create2DHistogram<TH2D>("hMuonSSMinusPtRelJet2D", "Symmetrized 2D distribution of p_{T}^{Rel} for SSMinus di-#mu", 500, 0., 5, 500, 0., 5, "p_{T}^{Rel} [GeV]", "p_{T}^{Rel} [GeV]");
  TH2D* hMuonSSPlusPtRelJet2D = Create2DHistogram<TH2D>("hMuonSSPlusPtRelJet2D", "Symmetrized 2D distribution of p_{T}^{Rel} for SSPlus di-#mu", 500, 0., 5, 500, 0., 5, "p_{T}^{Rel} [GeV]", "p_{T}^{Rel} [GeV]");
  TH2D* hMuonSSPtRelJet2D = Create2DHistogram<TH2D>("hMuonSSPtRelJet2D", "Symmetrized 2D distribution of p_{T}^{Rel} for SS di-#mu", 500, 0., 5, 500, 0., 5, "p_{T}^{Rel} [GeV]", "p_{T}^{Rel} [GeV]");
  TH1D* hMuonPtRelJet = Create1DHistogram<TH1D>("hMuonPtRelJet", "p_{T}^{Rel} distribution for #mu", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");
  TH1D* hMuonPlusPtRelJet = Create1DHistogram<TH1D>("hMuonPlusPtRelJet", "p_{T}^{Rel} distribution for #mu^{+}", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");
  TH1D* hMuonMinusPtRelJet = Create1DHistogram<TH1D>("hMuonMinusPtRelJet", "p_{T}^{Rel} distribution for #mu^{-}", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");
  TH1D* hMuon1PtRelJet = Create1DHistogram<TH1D>("hMuon1PtRelJet", "p_{T}^{Rel} distribution for #mu_{1}", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");
  TH1D* hMuon2PtRelJet = Create1DHistogram<TH1D>("hMuon2PtRelJet", "p_{T}^{Rel} distribution for #mu_{2}", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");
  TH2D* hMuonIPvsPtRelJet = Create2DHistogram<TH2D>("hMuonIPvsPtRelJet", "2D distribution of p_{T}^{Rel} vs d_{xy}", 500, 0., 0.5, 500, 0., 5, "d_{xy} [cm]", "p_{T}^{Rel} [GeV]");

  TH1D* hMuonOSPtRelJet = Create1DHistogram<TH1D>("hMuonOSPtRelJet", "p_{T}^{Rel} distribution for OS #mu", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");
  TH1D* hMuonSSPtRelJet = Create1DHistogram<TH1D>("hMuonSSPtRelJet", "p_{T}^{Rel} distribution for SS #mu", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");
  TH1D* hMuonSSPlusPtRelJet = Create1DHistogram<TH1D>("hMuonSSPlusPtRelJet", "p_{T}^{Rel} distribution for SSPlus #mu", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");
  TH1D* hMuonSSMinusPtRelJet = Create1DHistogram<TH1D>("hMuonSSMinusPtRelJet", "p_{T}^{Rel} distribution for SSMinus #mu", 500, 0., 5, "p_{T}^{Rel} [GeV]", "N_{#mu}");

  
  TH2D* hMuonPtOutJet2D = Create2DHistogram<TH2D>("hMuonPtOutJet2D", "Symmetrized 2D distribution of p_{T}^{Out} using jet", 500, 0., 5, 500, 0., 5, "p_{T}^{Out} [GeV]", "p_{T}^{Out} [GeV]");
  TH2D* hMuonPtOutSV2D = Create2DHistogram<TH2D>("hMuonPtOutSV2D", "Symmetrized 2D distribution of p_{T}^{Out} using SV", 500, 0., 5, 500, 0., 5, "p_{T}^{Out} [GeV]", "p_{T}^{Out} [GeV]");
  TH2D* hMuonPtOutSVorJet2D = Create2DHistogram<TH2D>("hMuonPtOutSVorJet2D", "Symmetrized 2D distribution of p_{T}^{Out} using SV or jet", 500, 0., 5, 500, 0., 5, "p_{T}^{Out} [GeV]", "p_{T}^{Out} [GeV]");
  TH2D* hMuonOSPtOutJet2D = Create2DHistogram<TH2D>("hMuonOSPtOutJet2D", "Symmetrized 2D distribution of p_{T}^{Out} for DS di-#mu", 500, 0., 5, 500, 0., 5, "p_{T}^{Out} [GeV]", "p_{T}^{Out} [GeV]");
  TH2D* hMuonSSMinusPtOutJet2D = Create2DHistogram<TH2D>("hMuonSSMinusPtOutJet2D", "Symmetrized 2D distribution of p_{T}^{Out} for SSMinus di-#mu", 500, 0., 5, 500, 0., 5, "p_{T}^{Out} [GeV]", "p_{T}^{Out} [GeV]");
  TH2D* hMuonSSPlusPtOutJet2D = Create2DHistogram<TH2D>("hMuonSSPlusPtOutJet2D", "Symmetrized 2D distribution of p_{T}^{Out} for SSPlus di-#mu", 500, 0., 5, 500, 0., 5, "p_{T}^{Out} [GeV]", "p_{T}^{Out} [GeV]");
  TH2D* hMuonSSPtOutJet2D = Create2DHistogram<TH2D>("hMuonSSPtOutJet2D", "Symmetrized 2D distribution of p_{T}^{Out} for SS di-#mu", 500, 0., 5, 500, 0., 5, "p_{T}^{Out} [GeV]", "p_{T}^{Out} [GeV]");
  TH1D* hMuonPtOutJet = Create1DHistogram<TH1D>("hMuonPtOutJet", "p_{T}^{Out} distribution for #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuonPlusPtOutJet = Create1DHistogram<TH1D>("hMuonPlusPtOutJet", "p_{T}^{Out} distribution for #mu^{+}", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuonMinusPtOutJet = Create1DHistogram<TH1D>("hMuonMinusPtOutJet", "p_{T}^{Out} distribution for #mu^{-}", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuon1PtOutJet = Create1DHistogram<TH1D>("hMuon1PtOutJet", "p_{T}^{Out} distribution for #mu_{1}", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuon2PtOutJet = Create1DHistogram<TH1D>("hMuon2PtOutJet", "p_{T}^{Out} distribution for #mu_{2}", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH2D* hMuonIPvsPtOutJet = Create2DHistogram<TH2D>("hMuonIPvsPtOutJet", "2D distribution of p_{T}^{Out} vs d_{xy}", 500, 0., 0.5, 500, 0., 5, "d_{xy} [cm]", "p_{T}^{Out} [GeV]");
  
  TH1D* hMuonOSPtOutJet = Create1DHistogram<TH1D>("hMuonOSPtOutJet", "p_{T}^{Out} distribution for OS #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuonSSPtOutJet = Create1DHistogram<TH1D>("hMuonSSPtOutJet", "p_{T}^{Out} distribution for SS #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuonSSPlusPtOutJet = Create1DHistogram<TH1D>("hMuonSSPlusPtOutJet", "p_{T}^{Out} distribution for SSPlus #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuonSSMinusPtOutJet = Create1DHistogram<TH1D>("hMuonSSMinusPtOutJet", "p_{T}^{Out} distribution for SSMinus #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  
  TH1D* hMuonPtOutJetJ1015 = Create1DHistogram<TH1D>("hMuonPtOutJetJ1015", "p_{T}^{Out} distribution for J1015 #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuonPtOutJetJ1520 = Create1DHistogram<TH1D>("hMuonPtOutJetJ1520", "p_{T}^{Out} distribution for J1520 #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuonPtOutJetJ2030 = Create1DHistogram<TH1D>("hMuonPtOutJetJ2030", "p_{T}^{Out} distribution for J2030 #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");
  TH1D* hMuonPtOutJetJ30xx = Create1DHistogram<TH1D>("hMuonPtOutJetJ30xx", "p_{T}^{Out} distribution for J30xx #mu", 500, 0., 5, "p_{T}^{Out} [GeV]", "N_{#mu}");

  
  

  TH2D* hMuonEmuJet2D = Create2DHistogram<TH2D>("hMuonEmuJet2D", "Symmetrized 2D distribution of E_{#mu}(jet) using jet", 600, 0., 6., 600, 0., 6., "E_{#mu}(jet) [GeV]", "E_{#mu}(jet) [GeV]");
  TH2D* hMuonOSEmuJet2D = Create2DHistogram<TH2D>("hMuonOSEmuJet2D", "Symmetrized 2D distribution of E_{#mu}(jet) for DS di-#mu", 600, 0., 6., 600, 0., 6., "E_{#mu}(jet) [GeV]", "E_{#mu}(jet) [GeV]");
  TH2D* hMuonSSMinusEmuJet2D = Create2DHistogram<TH2D>("hMuonSSMinusEmuJet2D", "Symmetrized 2D distribution of E_{#mu}(jet) for SSMinus di-#mu", 600, 0., 6., 600, 0., 6., "E_{#mu}(jet) [GeV]", "E_{#mu}(jet) [GeV]");
  TH2D* hMuonSSPlusEmuJet2D = Create2DHistogram<TH2D>("hMuonSSPlusEmuJet2D", "Symmetrized 2D distribution of E_{#mu}(jet) for SSPlus di-#mu", 600, 0., 6., 600, 0., 6., "E_{#mu}(jet) [GeV]", "E_{#mu}(jet) [GeV]");
  TH2D* hMuonSSEmuJet2D = Create2DHistogram<TH2D>("hMuonSSEmuJet2D", "Symmetrized 2D distribution of E_{#mu}(jet) for SS di-#mu", 600, 0., 6., 600, 0., 6., "E_{#mu}(jet) [GeV]", "E_{#mu}(jet) [GeV]");

  TH1D* hMuonEmuJet = Create1DHistogram<TH1D>("hMuonEmuJet", "E_{#mu}(jet) distribution for #mu", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
  TH1D* hMuonPlusEmuJet = Create1DHistogram<TH1D>("hMuonPlusEmuJet", "E_{#mu}(jet) distribution for #mu^{+}", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
  TH1D* hMuonMinusEmuJet = Create1DHistogram<TH1D>("hMuonMinusEmuJet", "E_{#mu}(jet) distribution for #mu^{-}", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
  TH1D* hMuon1EmuJet = Create1DHistogram<TH1D>("hMuon1EmuJet", "E_{#mu}(jet) distribution for #mu_{1}", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
  TH1D* hMuon2EmuJet = Create1DHistogram<TH1D>("hMuon2EmuJet", "E_{#mu}(jet) distribution for #mu_{2}", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
  TH2D* hMuonIPvsEmuJet = Create2DHistogram<TH2D>("hMuonIPvsEmuJet", "2D distribution of E_{#mu}(jet) vs d_{xy}", 500, 0., 0.5, 600, 0., 6., "d_{xy} [cm]", "E_{#mu}(jet) [GeV]");
  TH2D* hMuonPtOutvsEmuJet = Create2DHistogram<TH2D>("hMuonPtOutvsEmuJet", "2D distribution of E_{#mu}(jet) vs p_{T}^{Out}", 500, 0., 5., 600, 0., 6., "p_{T}^{Out} [GeV]", "E_{#mu}(jet) [GeV]");
  
  TH1D* hMuonOSEmuJet = Create1DHistogram<TH1D>("hMuonOSEmuJet", "E_{#mu}(jet) distribution for OS #mu", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
  TH1D* hMuonSSEmuJet = Create1DHistogram<TH1D>("hMuonSSEmuJet", "E_{#mu}(jet) distribution for SS #mu", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
  TH1D* hMuonSSPlusEmuJet = Create1DHistogram<TH1D>("hMuonSSPlusEmuJet", "E_{#mu}(jet) distribution for SSPlus #mu", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");
  TH1D* hMuonSSMinusEmuJet = Create1DHistogram<TH1D>("hMuonSSMinusEmuJet", "E_{#mu}(jet) distribution for SSMinus #mu", 600, 0., 6., "E_{#mu}(jet) [GeV]", "N_{#mu}");



  // This is to run on MC while keeping this class a TreeAnalyzerData
  bool hasDataTrigger = false;
  std::string chosenTriggerName = "HLT_DoubleMu3_v2";

  for (unsigned int tentry = 0; tentry < triggerPathNames->size() && !hasDataTrigger; tentry++)
  {
    if (chosenTriggerName.compare( triggerPathNames->at(tentry) ) == 0)
      hasDataTrigger = true;
  }

  if (!hasDataTrigger)
    chosenTriggerName = "HLT_DoubleMu3";

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
    if(!s->SelectEventPassingTrigger(chosenTriggerName))
    {
      continue;
    }

    // PtRel/PtOut stuff
    std::vector<double> CosThetaJet_PtOut, PtOutJet, PtOutSV, PtOutSVorJet, DRMuonJet_PtOut, CosThetaSV_PtOut, DRMuonSV_PtOut, DRJetSV_PtOut,
                        CosThetaJet_PtRel, PtRelJet, PtRelSV, PtRelSVorJet, DRMuonJet_PtRel, CosThetaSV_PtRel, DRMuonSV_PtRel, DRJetSV_PtRel,
                        SignedDxy,
                        EmuJet;
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

      // Get IP information from Ntuples
      vDxyz.push_back(Track_dxyz_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));
      vDxy.push_back(Track_dxy_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));
      vDz.push_back(Track_dz_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));
      vCharge.push_back(Track_charge->at(Muon_trackref->at(iMuon)));

      // Calculate PtRel/PtOut information
      if (s->JetSelectorIsValid())
      {
        unsigned short iJet = vSelectedJets[i];

        if (Jet_pt->at(vSelectedJets[i]) >= 10.0)
        {
          if (Jet_pt->at(vSelectedJets[i]) < 15.0)
            hMuonDxyJ1015->Fill(vDxy[i]);
          else if (Jet_pt->at(vSelectedJets[i]) < 20.0)
            hMuonDxyJ1520->Fill(vDxy[i]);
          else if (Jet_pt->at(vSelectedJets[i]) < 30.0)
            hMuonDxyJ2030->Fill(vDxy[i]);
          else
            hMuonDxyJ30xx->Fill(vDxy[i]);
        }        
        
        // Retrieve PV infos
        TVector3 pVtx( PV_x->at(iSelectedVertex), PV_y->at(iSelectedVertex), PV_z->at(iSelectedVertex) );

        // Build TLorentzVectors
        TLorentzVector pMuon, pJet;
        pMuon.SetPtEtaPhiM( Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon), constants::muonMass );
        if(s->GetJetType().compare("Tk") == 0)
          pJet.SetPtEtaPhiM( TrackJet_pt->at(iJet), TrackJet_eta->at(iJet), TrackJet_phi->at(iJet), TrackJet_mass->at(iJet) );
        else
          pJet.SetPtEtaPhiM( Jet_pt->at(iJet), Jet_eta->at(iJet), Jet_phi->at(iJet), Jet_mass->at(iJet) );

        // Track reference point (corrected for PV)
        TVector3 pTkRef( Track_referencePoint_x->at(Muon_trackref->at(iMuon)) - pVtx.X(),
                         Track_referencePoint_y->at(Muon_trackref->at(iMuon)) - pVtx.Y(),
                         Track_referencePoint_z->at(Muon_trackref->at(iMuon)) - pVtx.Z() );

        // Use SecVtx if present inside jet
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
        if(iSecVtx > -1)
        {
          pSVtx.SetXYZM( SV_flightDirection_x->at(iSecVtx), SV_flightDirection_y->at(iSecVtx), SV_flightDirection_z->at(iSecVtx), SV_mass->at(iSecVtx) );
          pSVtx.SetRho( pJet.Rho() );
        }
        else
        {
          pSVtx.SetXYZM( 9e9, 9e9, 9e9, 9e9 );
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
        

      } // PtRel/PtOut

      /// Fill IP distributions
      hMuonDxyz->Fill(vDxyz[i]);
      hMuonDxy->Fill(vDxy[i]);
      hMuonDz->Fill(vDz[i]);

      if (i==0)
      {
        hMuon1Dxyz->Fill(vDxyz[i]);
        hMuon1Dxy->Fill(vDxy[i]);
        hMuon1Dz->Fill(vDz[i]);
      }
      else if (i==1)
      {
        hMuon2Dxyz->Fill(vDxyz[i]);
        hMuon2Dxy->Fill(vDxy[i]);
        hMuon2Dz->Fill(vDz[i]);        
      }

      // Separate by muon charge
      if(vCharge[i]>0)
      {
        hMuonPlusDxyz->Fill(vDxyz[i]);
        hMuonPlusDxy->Fill(vDxy[i]);
        hMuonPlusDz->Fill(vDz[i]);
      }
      else
      {
        hMuonMinusDxyz->Fill(vDxyz[i]);
        hMuonMinusDxy->Fill(vDxy[i]);
        hMuonMinusDz->Fill(vDz[i]);
      }
      
      /// Separate SS from OS
      if(vCharge[0] != vCharge[1])
      {
        hMuonOSDxy->Fill(vDxy[i]);
      } // OppositeSign
      else
      {
        hMuonSSDxy->Fill(vDxy[i]);
        
        if (vCharge[0]>0)
        {
          hMuonSSPlusDxy->Fill(vDxy[i]);
        }
        else
        {
          hMuonSSMinusDxy->Fill(vDxy[i]);          
        }
      } // SameSign
      

    } // Loop over selected muons

    /// Fill 2D IP Distributions
    hMuonDxyz2D->Fill(vDxyz[0],vDxyz[1],0.5);
    hMuonDxyz2D->Fill(vDxyz[1],vDxyz[0],0.5);
    hMuonDxy2D->Fill(vDxy[0],vDxy[1],0.5);
    hMuonDxy2D->Fill(vDxy[1],vDxy[0],0.5);
    hMuonDz2D->Fill(vDz[0],vDz[1],0.5);
    hMuonDz2D->Fill(vDz[1],vDz[0],0.5);
    
    /// Smeared Dxy ones!
    double smearedValue0 = vDxy[0] + rng.Gaus(0.,0.0001);
    double smearedValue1 = vDxy[1] + rng.Gaus(0.,0.0001);
    hMuonDxy2D_1um->Fill(smearedValue0, smearedValue1, 0.5);
    hMuonDxy2D_1um->Fill(smearedValue1, smearedValue0, 0.5);
    smearedValue0 = vDxy[0] + rng.Gaus(0.,0.0003);
    smearedValue1 = vDxy[1] + rng.Gaus(0.,0.0003);
    hMuonDxy2D_3um->Fill(smearedValue0, smearedValue1, 0.5);
    hMuonDxy2D_3um->Fill(smearedValue1, smearedValue0, 0.5);
    smearedValue0 = vDxy[0] + rng.Gaus(0.,0.0005);
    smearedValue1 = vDxy[1] + rng.Gaus(0.,0.0005);
    hMuonDxy2D_5um->Fill(smearedValue0, smearedValue1, 0.5);
    hMuonDxy2D_5um->Fill(smearedValue1, smearedValue0, 0.5);
    smearedValue0 = vDxy[0] + rng.Gaus(0.,0.0008);
    smearedValue1 = vDxy[1] + rng.Gaus(0.,0.0008);
    hMuonDxy2D_8um->Fill(smearedValue0, smearedValue1, 0.5);
    hMuonDxy2D_8um->Fill(smearedValue1, smearedValue0, 0.5);
    smearedValue0 = vDxy[0] + rng.Gaus(0.,0.0010);
    smearedValue1 = vDxy[1] + rng.Gaus(0.,0.0010);
    hMuonDxy2D_10um->Fill(smearedValue0, smearedValue1, 0.5);
    hMuonDxy2D_10um->Fill(smearedValue1, smearedValue0, 0.5);
    smearedValue0 = vDxy[0] + rng.Gaus(0.,0.0012);
    smearedValue1 = vDxy[1] + rng.Gaus(0.,0.0012);
    hMuonDxy2D_12um->Fill(smearedValue0, smearedValue1, 0.5);
    hMuonDxy2D_12um->Fill(smearedValue1, smearedValue0, 0.5);
    smearedValue0 = vDxy[0] + rng.Gaus(0.,0.0020);
    smearedValue1 = vDxy[1] + rng.Gaus(0.,0.0020);
    hMuonDxy2D_20um->Fill(smearedValue0, smearedValue1, 0.5);
    hMuonDxy2D_20um->Fill(smearedValue1, smearedValue0, 0.5);
    
    /// Separate SS from OS
    if(vCharge[0] != vCharge[1])
    {
      hMuonOSDxyz2D->Fill(vDxyz[0], vDxyz[1], 0.5);
      hMuonOSDxyz2D->Fill(vDxyz[1], vDxyz[0], 0.5);
      hMuonOSDxy2D->Fill(vDxy[0], vDxy[1], 0.5);
      hMuonOSDxy2D->Fill(vDxy[1], vDxy[0], 0.5);
      hMuonOSDz2D->Fill(vDz[0], vDz[1], 0.5);
      hMuonOSDz2D->Fill(vDz[1], vDz[0], 0.5);
    } // OppositeSign
    else
    {
      hMuonSSDxyz2D->Fill(vDxyz[0], vDxyz[1], 0.5);
      hMuonSSDxyz2D->Fill(vDxyz[1], vDxyz[0], 0.5);
      hMuonSSDxy2D->Fill(vDxy[0], vDxy[1], 0.5);
      hMuonSSDxy2D->Fill(vDxy[1], vDxy[0], 0.5);
      hMuonSSDz2D->Fill(vDz[0], vDz[1], 0.5);
      hMuonSSDz2D->Fill(vDz[1], vDz[0], 0.5);
      
      if (vCharge[0]>0)
      {
        hMuonSSPlusDxyz2D->Fill(vDxyz[0],vDxyz[1], 0.5);
        hMuonSSPlusDxyz2D->Fill(vDxyz[1],vDxyz[0], 0.5);
        hMuonSSPlusDxy2D->Fill(vDxy[0],vDxy[1], 0.5);
        hMuonSSPlusDxy2D->Fill(vDxy[1],vDxy[0], 0.5);
        hMuonSSPlusDz2D->Fill(vDz[0],vDz[1], 0.5);
        hMuonSSPlusDz2D->Fill(vDz[1],vDz[0], 0.5);
      }
      else
      {
        hMuonSSMinusDxyz2D->Fill(vDxyz[0],vDxyz[1], 0.5);
        hMuonSSMinusDxyz2D->Fill(vDxyz[1],vDxyz[0], 0.5);
        hMuonSSMinusDxy2D->Fill(vDxy[0],vDxy[1], 0.5);
        hMuonSSMinusDxy2D->Fill(vDxy[1],vDxy[0], 0.5);
        hMuonSSMinusDz2D->Fill(vDz[0],vDz[1], 0.5);
        hMuonSSMinusDz2D->Fill(vDz[1],vDz[0], 0.5);
      }
    } // SameSign

    // PtRel/PtOut
    if (s->JetSelectorIsValid())
    {
      // Loop over selected Muons
      for(unsigned short i = 0; i < vSelectedMuons.size(); i++)
      {
        hMuonPtRelJet->Fill(PtRelJet[i]);
        hMuonIPvsPtRelJet->Fill(vDxy[i],PtRelJet[i]);
        
        if (i==0)
        {
          hMuon1PtRelJet->Fill(PtRelJet[i]);
        }
        else if (i==1)
        {
          hMuon2PtRelJet->Fill(PtRelJet[i]);
        }
        
        // Separate by muon charge
        if(vCharge[i]>0)
        {
          hMuonPlusPtRelJet->Fill(PtRelJet[i]);
        }
        else
        {
          hMuonMinusPtRelJet->Fill(PtRelJet[i]);
        }
        
        /// Separate SS from OS
        if(vCharge[0] != vCharge[1])
        {
          hMuonOSPtRelJet->Fill(PtRelJet[i]);
        } // OppositeSign
        else
        {
          hMuonSSPtRelJet->Fill(PtRelJet[i]);
          
          if (vCharge[0]>0)
          {
            hMuonSSPlusPtRelJet->Fill(PtRelJet[i]);
          }
          else
          {
            hMuonSSMinusPtRelJet->Fill(PtRelJet[i]);          
          }
        } // SameSign        
        
      } /// End of loop over selected muons
      
      /// 2D distributions
      hMuonPtRelJet2D->Fill(PtRelJet[0], PtRelJet[1], 0.5);
      hMuonPtRelJet2D->Fill(PtRelJet[1], PtRelJet[0], 0.5);
      hMuonPtRelSV2D->Fill(PtRelSV[0], PtRelSV[1], 0.5);
      hMuonPtRelSV2D->Fill(PtRelSV[1], PtRelSV[0], 0.5);
      hMuonPtRelSVorJet2D->Fill(PtRelSVorJet[0], PtRelSVorJet[1], 0.5);
      hMuonPtRelSVorJet2D->Fill(PtRelSVorJet[1], PtRelSVorJet[0], 0.5);
      
      /// Separate SS from OS
      if(vCharge[0] != vCharge[1])
      {
        hMuonOSPtRelJet2D->Fill(PtRelJet[0], PtRelJet[1], 0.5);
        hMuonOSPtRelJet2D->Fill(PtRelJet[1], PtRelJet[0], 0.5);
      } // OppositeSign
      else
      {
        hMuonSSPtRelJet2D->Fill(PtRelJet[0], PtRelJet[1], 0.5);
        hMuonSSPtRelJet2D->Fill(PtRelJet[1], PtRelJet[0], 0.5);
        if (vCharge[0]>0)
        {
          hMuonSSPlusPtRelJet2D->Fill(PtRelJet[0], PtRelJet[1], 0.5);
          hMuonSSPlusPtRelJet2D->Fill(PtRelJet[1], PtRelJet[0], 0.5);
        }
        else
        {
          hMuonSSMinusPtRelJet2D->Fill(PtRelJet[0], PtRelJet[1], 0.5);
          hMuonSSMinusPtRelJet2D->Fill(PtRelJet[1], PtRelJet[0], 0.5);
        }
      } // SameSign
      
      
      // PtOut
      
      // Loopover selected Muons
      for(unsigned short i = 0; i < vSelectedMuons.size(); i++)
      {
        hMuonPtOutJet->Fill(PtOutJet[i]);
        hMuonIPvsPtOutJet->Fill(vDxy[i],PtOutJet[i]);
        
        if (Jet_pt->at(vSelectedJets[i]) >= 10.0)
        {
          if (Jet_pt->at(vSelectedJets[i]) < 15.0)
            hMuonPtOutJetJ1015->Fill(PtOutJet[i]);
          else if (Jet_pt->at(vSelectedJets[i]) < 20.0)
            hMuonPtOutJetJ1520->Fill(PtOutJet[i]);
          else if (Jet_pt->at(vSelectedJets[i]) < 30.0)
            hMuonPtOutJetJ2030->Fill(PtOutJet[i]);
          else
            hMuonPtOutJetJ30xx->Fill(PtOutJet[i]);
        }
        
        if (i==0)
        {
          hMuon1PtOutJet->Fill(PtOutJet[i]);
        }
        else if (i==1)
        {
          hMuon2PtOutJet->Fill(PtOutJet[i]);
        }
        
        // Separate by muon charge
        if(vCharge[i]>0)
        {
          hMuonPlusPtOutJet->Fill(PtOutJet[i]);
        }
        else
        {
          hMuonMinusPtOutJet->Fill(PtOutJet[i]);
        }
        
        /// Separate SS from OS
        if(vCharge[0] != vCharge[1])
        {
          hMuonOSPtOutJet->Fill(PtOutJet[i]);
        } // OppositeSign
        else
        {
          hMuonSSPtOutJet->Fill(PtOutJet[i]);
          
          if (vCharge[0]>0)
          {
            hMuonSSPlusPtOutJet->Fill(PtOutJet[i]);
          }
          else
          {
            hMuonSSMinusPtOutJet->Fill(PtOutJet[i]);          
          }
        } // SameSign                
        
      } /// End of loop over selected muons
      
      /// 2D distributions
      hMuonPtOutJet2D->Fill(PtOutJet[0], PtOutJet[1], 0.5);
      hMuonPtOutJet2D->Fill(PtOutJet[1], PtOutJet[0], 0.5);
      hMuonPtOutSV2D->Fill(PtOutSV[0], PtOutSV[1], 0.5);
      hMuonPtOutSV2D->Fill(PtOutSV[1], PtOutSV[0], 0.5);
      hMuonPtOutSVorJet2D->Fill(PtOutSVorJet[0], PtOutSVorJet[1], 0.5);
      hMuonPtOutSVorJet2D->Fill(PtOutSVorJet[1], PtOutSVorJet[0], 0.5);
      
      /// Separate SS from OS
      if(vCharge[0] != vCharge[1])
      {
        hMuonOSPtOutJet2D->Fill(PtOutJet[0], PtOutJet[1], 0.5);
        hMuonOSPtOutJet2D->Fill(PtOutJet[1], PtOutJet[0], 0.5);
      } // OppositeSign
      else
      {
        hMuonSSPtOutJet2D->Fill(PtOutJet[0], PtOutJet[1], 0.5);
        hMuonSSPtOutJet2D->Fill(PtOutJet[1], PtOutJet[0], 0.5);
        if (vCharge[0]>0)
        {
          hMuonSSPlusPtOutJet2D->Fill(PtOutJet[0], PtOutJet[1], 0.5);
          hMuonSSPlusPtOutJet2D->Fill(PtOutJet[1], PtOutJet[0], 0.5);
        }
        else
        {
          hMuonSSMinusPtOutJet2D->Fill(PtOutJet[0], PtOutJet[1], 0.5);
          hMuonSSMinusPtOutJet2D->Fill(PtOutJet[1], PtOutJet[0], 0.5);
        }
      } // SameSign
      
      
      // Emu
      
      // Loopover selected Muons
      for(unsigned short i = 0; i < vSelectedMuons.size(); i++)
      {
        hMuonEmuJet->Fill(EmuJet[i]);
        hMuonIPvsEmuJet->Fill(vDxy[i],EmuJet[i]);
        hMuonPtOutvsEmuJet->Fill(PtOutJet[i],EmuJet[i]);
                
        if (i==0)
        {
          hMuon1EmuJet->Fill(EmuJet[i]);
        }
        else if (i==1)
        {
          hMuon2EmuJet->Fill(EmuJet[i]);
        }
        
        // Separate by muon charge
        if(vCharge[i]>0)
        {
          hMuonPlusEmuJet->Fill(EmuJet[i]);
        }
        else
        {
          hMuonMinusEmuJet->Fill(EmuJet[i]);
        }
        
        /// Separate SS from OS
        if(vCharge[0] != vCharge[1])
        {
          hMuonOSEmuJet->Fill(EmuJet[i]);
        } // OppositeSign
        else
        {
          hMuonSSEmuJet->Fill(EmuJet[i]);
          
          if (vCharge[0]>0)
          {
            hMuonSSPlusEmuJet->Fill(EmuJet[i]);
          }
          else
          {
            hMuonSSMinusEmuJet->Fill(EmuJet[i]);          
          }
        } // SameSign                
        
      } /// End of loop over selected muons
      
      /// 2D distributions
      hMuonEmuJet2D->Fill(EmuJet[0], EmuJet[1], 0.5);
      hMuonEmuJet2D->Fill(EmuJet[1], EmuJet[0], 0.5);
      
      /// Separate SS from OS
      if(vCharge[0] != vCharge[1])
      {
        hMuonOSEmuJet2D->Fill(EmuJet[0], EmuJet[1], 0.5);
        hMuonOSEmuJet2D->Fill(EmuJet[1], EmuJet[0], 0.5);
      } // OppositeSign
      else
      {
        hMuonSSEmuJet2D->Fill(EmuJet[0], EmuJet[1], 0.5);
        hMuonSSEmuJet2D->Fill(EmuJet[1], EmuJet[0], 0.5);
        if (vCharge[0]>0)
        {
          hMuonSSPlusEmuJet2D->Fill(EmuJet[0], EmuJet[1], 0.5);
          hMuonSSPlusEmuJet2D->Fill(EmuJet[1], EmuJet[0], 0.5);
        }
        else
        {
          hMuonSSMinusEmuJet2D->Fill(EmuJet[0], EmuJet[1], 0.5);
          hMuonSSMinusEmuJet2D->Fill(EmuJet[1], EmuJet[0], 0.5);
        }
      } // SameSign         
      
    } /// PtRel/PtOut
    
  } /// End of loop over events

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here

  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // MuonDistributionsProducer_cxx

