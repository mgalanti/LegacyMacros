#ifndef MuonPromptTemplateDataProducer_cxx
#define MuonPromptTemplateDataProducer_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 3 //2

#include "TF1.h"
#include "TFitResultPtr.h"

#include "MuonPromptTemplateDataProducer.h"
#include "../utilities/constants.h"

void MuonPromptTemplateDataProducer::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Book histograms here
  TH1D* hDiMuonJPsiInvMass = Create1DHistogram<TH1D>("hDiMuonJPsiInvMass", "Di-#mu invariant mass in the J/#psi region", 50, 2.,4.5,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuonUpsilonInvMass = Create1DHistogram<TH1D>("hDiMuonUpsilonInvMass", "Di-#mu invariant mass in the #Upsilon region", 80, 8.,12.,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuonJPsiGoodVtxInvMass = Create1DHistogram<TH1D>("hDiMuonJPsiGoodVtxInvMass", "Di-#mu invariant mass in the J/#psi region - good vtx", 50, 2.,4.5,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuonUpsilonGoodVtxInvMass = Create1DHistogram<TH1D>("hDiMuonUpsilonGoodVtxInvMass", "Di-#mu invariant mass in the #Upsilon region - good vtx", 80, 8.,12.,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuonJPsiSBInvMass = Create1DHistogram<TH1D>("hDiMuonJPsiSBInvMass", "Di-#mu invariant mass in the J/#psi sideband region", 50, 2.,4.5,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuonUpsilonSBInvMass = Create1DHistogram<TH1D>("hDiMuonUpsilonSBInvMass", "Di-#mu invariant mass in the #Upsilon sideband region", 80, 8.,12.,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuonJPsiPKInvMass = Create1DHistogram<TH1D>("hDiMuonJPsiPKInvMass", "Di-#mu invariant mass in the J/#psi peak region", 50, 2.,4.5,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuonUpsilonPKInvMass = Create1DHistogram<TH1D>("hDiMuonUpsilonPKInvMass", "Di-#mu invariant mass in the #Upsilon peak region", 80, 8.,12.,"M_{#mu#mu} [GeV]","N_{#mu#mu}");

  TH1D* hDiMuonJPsiVtxProb = Create1DHistogram<TH1D>("hDiMuonJPsiVtxProb","Vertex probability of di-#mu in J/#psi region",1000,0.,1.,"Prob","N_{#mu#mu}");
  TH1D* hDiMuonUpsilonVtxProb = Create1DHistogram<TH1D>("hDiMuonUpsilonVtxProb","Vertex probability of di-#mu in #Upsilon region",1000,0.,1.,"Prob","N_{#mu#mu}");
  TH1D* hDiMuonJPsiVtxCTau = Create1DHistogram<TH1D>("hDiMuonJPsiVtxCTau","Vertex c#tau of di-#mu in J/#psi region",1000,-1.,1.,"c#tau","N_{#mu#mu}");
  TH1D* hDiMuonUpsilonVtxCTau = Create1DHistogram<TH1D>("hDiMuonUpsilonVtxCTau","Vertex c#tau of di-#mu in #Upsilon region",1000,-1.,1.,"c#tau","N_{#mu#mu}");
  TH1D* hDiMuonJPsiVtxCTauError = Create1DHistogram<TH1D>("hDiMuonJPsiVtxCTauError","Vertex c#tau error of di-#mu in J/#psi region",1000,0.,1.,"#sigma(c#tau)","N_{#mu#mu}");
  TH1D* hDiMuonUpsilonVtxCTauError = Create1DHistogram<TH1D>("hDiMuonUpsilonVtxCTauError","Vertex c#tau error of di-#mu in #Upsilon region",1000,0.,1.,"#sigma(c#tau)","N_{#mu#mu}");
  TH1D* hDiMuonJPsiVtxCTauSignificance = Create1DHistogram<TH1D>("hDiMuonJPsiVtxCTauSignificance","Vertex c#tau significance of di-#mu in J/#psi region",1000,-5.,5.,"c#tau/#sigma(c#tau)","N_{#mu#mu}");
  TH1D* hDiMuonUpsilonVtxCTauSignificance = Create1DHistogram<TH1D>("hDiMuonUpsilonVtxCTauSignificance","Vertex c#tau significance of di-#mu in #Upsilon region",1000,-5.,5.,"c#tau/#sigma(c#tau)","N_{#mu#mu}");

  TH1D* hMuonJPsiDxyz = Create1DHistogram<TH1D>("hMuonJPsiDxyz","d_{xyz} template for muons in J/#psi region",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonJPsiDxy = Create1DHistogram<TH1D>("hMuonJPsiDxy","d_{xy} template for muons in J/#psi region",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonJPsiDz = Create1DHistogram<TH1D>("hMuonJPsiDz","d_{z} template for muons in J/#psi region",1000,-0.5,0.5,"d_{z}","N_{#mu}");
  TH1D* hMuonJPsiGoodVtxDxyz = Create1DHistogram<TH1D>("hMuonJPsiGoodVtxDxyz","d_{xyz} template for muons in J/#psi region - good vtx",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonJPsiGoodVtxDxy = Create1DHistogram<TH1D>("hMuonJPsiGoodVtxDxy","d_{xy} template for muons in J/#psi region - good vtx",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonJPsiGoodVtxDz = Create1DHistogram<TH1D>("hMuonJPsiGoodVtxDz","d_{z} template for muons in J/#psi region - good vtx",1000,-0.5,0.5,"d_{z}","N_{#mu}");
  TH1D* hMuonJPsiSBDxyz = Create1DHistogram<TH1D>("hMuonJPsiSBDxyz","d_{xyz} template for muons in J/#psi sideband region",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonJPsiSBDxy = Create1DHistogram<TH1D>("hMuonJPsiSBDxy","d_{xy} template for muons in J/#psi sideband region",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonJPsiSBDz = Create1DHistogram<TH1D>("hMuonJPsiSBDz","d_{z} template for muons in J/#psi sideband region",1000,-0.5,0.5,"d_{z}","N_{#mu}");
  TH1D* hMuonJPsiPKDxyz = Create1DHistogram<TH1D>("hMuonJPsiPKDxyz","d_{xyz} template for muons in J/#psi peak region",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonJPsiPKDxy = Create1DHistogram<TH1D>("hMuonJPsiPKDxy","d_{xy} template for muons in J/#psi peak region",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonJPsiPKDz = Create1DHistogram<TH1D>("hMuonJPsiPKDz","d_{z} template for muons in J/#psi peak region",1000,-0.5,0.5,"d_{z}","N_{#mu}");

  TH1D* hMuonUpsilonDxyz = Create1DHistogram<TH1D>("hMuonUpsilonDxyz","d_{xyz} template for muons in #Upsilon region",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonUpsilonDxy = Create1DHistogram<TH1D>("hMuonUpsilonDxy","d_{xy} template for muons in #Upsilon region",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonUpsilonDz = Create1DHistogram<TH1D>("hMuonUpsilonDz","d_{z} template for muons in #Upsilon region",1000,-0.5,0.5,"d_{z}","N_{#mu}");
  TH1D* hMuonUpsilonGoodVtxDxyz = Create1DHistogram<TH1D>("hMuonUpsilonGoodVtxDxyz","d_{xyz} template for muons in #Upsilon region - good vtx",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonUpsilonGoodVtxDxy = Create1DHistogram<TH1D>("hMuonUpsilonGoodVtxDxy","d_{xy} template for muons in #Upsilon region - good vtx",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonUpsilonGoodVtxDz = Create1DHistogram<TH1D>("hMuonUpsilonGoodVtxDz","d_{z} template for muons in #Upsilon region - good vtx",500,-0.5,0.5,"d_{z}","N_{#mu}");
  TH1D* hMuonUpsilonSBDxyz = Create1DHistogram<TH1D>("hMuonUpsilonSBDxyz","d_{xyz} template for muons in #Upsilon sideband region",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonUpsilonSBDxy = Create1DHistogram<TH1D>("hMuonUpsilonSBDxy","d_{xy} template for muons in #Upsilon sideband region",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonUpsilonSBDz = Create1DHistogram<TH1D>("hMuonUpsilonSBDz","d_{z} template for muons in #Upsilon sideband region",1000,-0.5,0.5,"d_{z}","N_{#mu}");
  TH1D* hMuonUpsilonPKDxyz = Create1DHistogram<TH1D>("hMuonUpsilonPKDxyz","d_{xyz} template for muons in #Upsilon peak region",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonUpsilonPKDxy = Create1DHistogram<TH1D>("hMuonUpsilonPKDxy","d_{xy} template for muons in #Upsilon peak region",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonUpsilonPKDz = Create1DHistogram<TH1D>("hMuonUpsilonPKDz","d_{z} template for muons in #Upsilon peak region",1000,-0.5,0.5,"d_{z}","N_{#mu}");

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
    unsigned short iSelectedVertex;
    if(!s->SelectEventWithDiMuonResonanceV0(text.c_str(), vSelectedMuons, iSelectedVertex))
    {
      //       std::cout << "Event " << jentry << " not selected!\n";
      continue;
    }
    if(vSelectedMuons.size() != 2)
      continue;
    unsigned int iMuon0 = vSelectedMuons[0];
    unsigned int iMuon1 = vSelectedMuons[1];
    unsigned int iTrack0 = Muon_trackref->at(iMuon0);
    unsigned int iTrack1 = Muon_trackref->at(iMuon1);
    int muon0Charge = Track_charge->at(iTrack0);
    int muon1Charge = Track_charge->at(iTrack1);
    if(muon0Charge==muon1Charge)
      continue;
    TLorentzVector pMuon0, pMuon1;
    pMuon0.SetPtEtaPhiM(Track_pt->at(iTrack0),Track_eta->at(iTrack0),Track_phi->at(iTrack0),constants::muonMass);
    pMuon1.SetPtEtaPhiM(Track_pt->at(iTrack1),Track_eta->at(iTrack1),Track_phi->at(iTrack1),constants::muonMass);
    double dxyz0 = Track_dxyz_PV->at(iTrack0).at(iSelectedVertex);
    double dxyz1 = Track_dxyz_PV->at(iTrack1).at(iSelectedVertex);
    double dxy0 = Track_dxy_PV->at(iTrack0).at(iSelectedVertex);
    double dxy1 = Track_dxy_PV->at(iTrack1).at(iSelectedVertex);
    double dz0 = Track_dz_PV->at(iTrack0).at(iSelectedVertex);
    double dz1 = Track_dz_PV->at(iTrack1).at(iSelectedVertex);
    double invariantMass = (pMuon0+pMuon1).M();
    unsigned int iVtx = 65535;
    double diMuonVtxProb = 1;
    double diMuonVtxCTau = 999;
    double diMuonVtxCTauError = 999;
    double diMuonVtxCTauSignificance = 999;
    for(unsigned int iDiMuVtx = 0; iDiMuVtx < Muon_vDiMuVtxMuonref->at(iMuon0).size(); iDiMuVtx++)
    {
      if(Muon_vDiMuVtxMuonref->at(iMuon0).at(iDiMuVtx) == iMuon1)
      {
        iVtx = iDiMuVtx;
        break;
      }
    }
    if(iVtx==65535)
    {
      std::cout << "No good di-muon vertex found!\n";
    }
    else
    {
      diMuonVtxProb = Muon_vDiMuVtxProb->at(iMuon0).at(iVtx);
      diMuonVtxCTau = Muon_vDiMuVtxCTau->at(iMuon0).at(iVtx);
      diMuonVtxCTauError = Muon_vDiMuVtxCTauError->at(iMuon0).at(iVtx);
      diMuonVtxCTauSignificance = diMuonVtxCTau/diMuonVtxCTauError;
    }
    if(invariantMass < 5)
    {
      hDiMuonJPsiInvMass->Fill(invariantMass);
      hMuonJPsiDxyz->Fill(dxyz0);
      hMuonJPsiDxyz->Fill(dxyz1);
      hMuonJPsiDxy->Fill(dxy0);
      hMuonJPsiDxy->Fill(dxy1);
      hMuonJPsiDz->Fill(dz0);
      hMuonJPsiDz->Fill(dz1);
      if(iVtx!=65535)
      {
        hDiMuonJPsiVtxProb->Fill(diMuonVtxProb);
        hDiMuonJPsiVtxCTau->Fill(diMuonVtxCTau);
        hDiMuonJPsiVtxCTauError->Fill(diMuonVtxCTauError);
        hDiMuonJPsiVtxCTauSignificance->Fill(diMuonVtxCTauSignificance);
        if(diMuonVtxProb > 0.25 && fabs(diMuonVtxCTauSignificance)<1.)
        {
          hDiMuonJPsiGoodVtxInvMass->Fill(invariantMass);
          hMuonJPsiGoodVtxDxyz->Fill(dxyz0);
          hMuonJPsiGoodVtxDxyz->Fill(dxyz1);
          hMuonJPsiGoodVtxDxy->Fill(dxy0);
          hMuonJPsiGoodVtxDxy->Fill(dxy1);
          hMuonJPsiGoodVtxDz->Fill(dz0);
          hMuonJPsiGoodVtxDz->Fill(dz1);
        }
        if((invariantMass>2.0 && invariantMass<2.5) ||
//            (invariantMass>3.3 && invariantMass<3.5) ||
           (invariantMass>4.0 && invariantMass<4.5))
        {
          hDiMuonJPsiSBInvMass->Fill(invariantMass);
          hMuonJPsiSBDxyz->Fill(dxyz0);
          hMuonJPsiSBDxyz->Fill(dxyz1);
          hMuonJPsiSBDxy->Fill(dxy0);
          hMuonJPsiSBDxy->Fill(dxy1);
          hMuonJPsiSBDz->Fill(dz0);
          hMuonJPsiSBDz->Fill(dz1);
        }
        if((invariantMass>3.0 && invariantMass<3.2) ||
           (invariantMass>3.65 && invariantMass<3.75))
        {
          hDiMuonJPsiPKInvMass->Fill(invariantMass);
          hMuonJPsiPKDxyz->Fill(dxyz0);
          hMuonJPsiPKDxyz->Fill(dxyz1);
          hMuonJPsiPKDxy->Fill(dxy0);
          hMuonJPsiPKDxy->Fill(dxy1);
          hMuonJPsiPKDz->Fill(dz0);
          hMuonJPsiPKDz->Fill(dz1);
        }
      }
    }
    else
    {
      hDiMuonUpsilonInvMass->Fill(invariantMass);
      hMuonUpsilonDxyz->Fill(dxyz0);
      hMuonUpsilonDxyz->Fill(dxyz1);
      hMuonUpsilonDxy->Fill(dxy0);
      hMuonUpsilonDxy->Fill(dxy1);
      hMuonUpsilonDz->Fill(dz0);
      hMuonUpsilonDz->Fill(dz1);
      if(iVtx!=65535)
      {
        hDiMuonUpsilonVtxProb->Fill(diMuonVtxProb);
        hDiMuonUpsilonVtxCTau->Fill(diMuonVtxCTau);
        hDiMuonUpsilonVtxCTauError->Fill(diMuonVtxCTauError);
        hDiMuonUpsilonVtxCTauSignificance->Fill(diMuonVtxCTauSignificance);
        if(diMuonVtxProb > 0.25 && fabs(diMuonVtxCTauSignificance)<1.)
        {
          hDiMuonUpsilonGoodVtxInvMass->Fill(invariantMass);
          hMuonUpsilonGoodVtxDxyz->Fill(dxyz0);
          hMuonUpsilonGoodVtxDxyz->Fill(dxyz1);
          hMuonUpsilonGoodVtxDxy->Fill(dxy0);
          hMuonUpsilonGoodVtxDxy->Fill(dxy1);
          hMuonUpsilonGoodVtxDz->Fill(dz0);
          hMuonUpsilonGoodVtxDz->Fill(dz1);
        }
        if((invariantMass>8. && invariantMass<9.) ||
          (invariantMass>10.7 && invariantMass<12.))
        {
          hDiMuonUpsilonSBInvMass->Fill(invariantMass);
          hMuonUpsilonSBDxyz->Fill(dxyz0);
          hMuonUpsilonSBDxyz->Fill(dxyz1);
          hMuonUpsilonSBDxy->Fill(dxy0);
          hMuonUpsilonSBDxy->Fill(dxy1);
          hMuonUpsilonSBDz->Fill(dz0);
          hMuonUpsilonSBDz->Fill(dz1);
        }
        if((invariantMass>9.2 && invariantMass<9.7))
        {
          hDiMuonUpsilonPKInvMass->Fill(invariantMass);
          hMuonUpsilonPKDxyz->Fill(dxyz0);
          hMuonUpsilonPKDxyz->Fill(dxyz1);
          hMuonUpsilonPKDxy->Fill(dxy0);
          hMuonUpsilonPKDxy->Fill(dxy1);
          hMuonUpsilonPKDz->Fill(dz0);
          hMuonUpsilonPKDz->Fill(dz1);
        }
      }
    }
  }

  // Perform the sideband subtraction
  // Fit the SB plots
  TF1* fitFunctionJPsi = new TF1("fitFunctionJPsi","[0]+[1]*x+[2]*x*x",2.0,4.5);
  TF1* fitFunctionUpsilon = new TF1("fitFunctionUpsilon","[0]+[1]*x+[2]*x*x",8.,12.);

  TFitResultPtr fitResultJPsi = hDiMuonJPsiSBInvMass->Fit(fitFunctionJPsi);
  TFitResultPtr fitResultUpsilon = hDiMuonUpsilonSBInvMass->Fit(fitFunctionUpsilon);
  // Evaluate the bkg integral in the signal and sideband regions
  double bkgBinWidthJPsi = hDiMuonJPsiSBInvMass->GetBinWidth(1);
  double bkgIntegralSB1JPsi = hDiMuonJPsiSBInvMass->Integral(hDiMuonJPsiSBInvMass->FindBin(2.0),hDiMuonJPsiSBInvMass->FindBin(2.4999));
//   double bkgIntegralSB2JPsi = hDiMuonJPsiSBInvMass->Integral(hDiMuonJPsiSBInvMass->FindBin(3.25),hDiMuonJPsiSBInvMass->FindBin(3.4999));
  double bkgIntegralSB3JPsi = hDiMuonJPsiSBInvMass->Integral(hDiMuonJPsiSBInvMass->FindBin(4.),hDiMuonJPsiSBInvMass->FindBin(4.4999));
  double bkgIntegralSBJPsi = bkgIntegralSB1JPsi + /*bkgIntegralSB2JPsi +*/ bkgIntegralSB3JPsi;
  std::cout << "JPsi BKG integral in the sidebands: " << bkgIntegralSB1JPsi << " + " /*<< bkgIntegralSB2JPsi << " + "*/ << bkgIntegralSB3JPsi << " = " << bkgIntegralSBJPsi << std::endl;
  double bkgIntegralPK1JPsi = fitFunctionJPsi->Integral(3.0,3.2)/bkgBinWidthJPsi;
  double bkgIntegralPK2JPsi = fitFunctionJPsi->Integral(3.65,3.75)/bkgBinWidthJPsi;
  double bkgIntegralPKJPsi = bkgIntegralPK1JPsi + bkgIntegralPK2JPsi;
  std::cout << "JPsi BKG integral in the 2 peaks: " << bkgIntegralPK1JPsi << " + " << bkgIntegralPK2JPsi << " = " << bkgIntegralPKJPsi << std::endl;
  double bkgScaleRatioJPsi = bkgIntegralPKJPsi / bkgIntegralSBJPsi;
  std::cout << "JPSi BKG scale ratio: " << bkgScaleRatioJPsi << std::endl;
  TH1D* hMuonJPsiBKGSubDxyz = (TH1D*)hMuonJPsiPKDxyz->Clone("hMuonJPsiBKGSubDxyz");
  TH1D* hMuonJPsiSBScaledDxyz = (TH1D*)hMuonJPsiSBDxyz->Clone("hMuonJPsiSBScaledDxyz");
  hMuonJPsiSBScaledDxyz->Scale(bkgScaleRatioJPsi);
  hMuonJPsiBKGSubDxyz->Add(hMuonJPsiSBScaledDxyz,-1);
  TH1D* hMuonJPsiBKGSubDxy = (TH1D*)hMuonJPsiPKDxy->Clone("hMuonJPsiBKGSubDxy");
  TH1D* hMuonJPsiSBScaledDxy = (TH1D*)hMuonJPsiSBDxy->Clone("hMuonJPsiSBScaledDxy");
  hMuonJPsiSBScaledDxy->Scale(bkgScaleRatioJPsi);
  hMuonJPsiBKGSubDxy->Add(hMuonJPsiSBScaledDxy,-1);
  TH1D* hMuonJPsiBKGSubDz = (TH1D*)hMuonJPsiPKDz->Clone("hMuonJPsiBKGSubDz");
  TH1D* hMuonJPsiSBScaledDz = (TH1D*)hMuonJPsiSBDz->Clone("hMuonJPsiSBScaledDz");
  hMuonJPsiSBScaledDz->Scale(bkgScaleRatioJPsi);
  hMuonJPsiBKGSubDz->Add(hMuonJPsiSBScaledDz,-1);

  double bkgBinWidthUpsilon = hDiMuonUpsilonSBInvMass->GetBinWidth(1);
  double bkgIntegralSB1Upsilon = hDiMuonUpsilonSBInvMass->Integral(hDiMuonUpsilonSBInvMass->FindBin(8.),hDiMuonUpsilonSBInvMass->FindBin(8.9999));
  double bkgIntegralSB2Upsilon = hDiMuonUpsilonSBInvMass->Integral(hDiMuonUpsilonSBInvMass->FindBin(10.7),hDiMuonUpsilonSBInvMass->FindBin(11.9999));
  double bkgIntegralSBUpsilon = bkgIntegralSB1Upsilon + bkgIntegralSB2Upsilon;
  std::cout << "Upsilon BKG integral in the 2 sidebands: " << bkgIntegralSB1Upsilon << " + " << bkgIntegralSB2Upsilon << " = " << bkgIntegralSBUpsilon << std::endl;
  double bkgIntegralPKUpsilon = fitFunctionUpsilon->Integral(9.2,9.7)/bkgBinWidthUpsilon;
  std::cout << "Upsilon BKG integral in the peak: " << bkgIntegralPKUpsilon << std::endl;
  double bkgScaleRatioUpsilon = bkgIntegralPKUpsilon / bkgIntegralSBUpsilon;
  std::cout << "Upsilon BKG scale ratio: " << bkgScaleRatioUpsilon << std::endl;
  TH1D* hMuonUpsilonBKGSubDxyz = (TH1D*)hMuonUpsilonPKDxyz->Clone("hMuonUpsilonBKGSubDxyz");
  TH1D* hMuonUpsilonSBScaledDxyz = (TH1D*)hMuonUpsilonSBDxyz->Clone("hMuonUpsilonSBScaledDxyz");
  hMuonUpsilonSBScaledDxyz->Scale(bkgScaleRatioUpsilon);
  hMuonUpsilonBKGSubDxyz->Add(hMuonUpsilonSBScaledDxyz,-1);
  TH1D* hMuonUpsilonBKGSubDxy = (TH1D*)hMuonUpsilonPKDxy->Clone("hMuonUpsilonBKGSubDxy");
  TH1D* hMuonUpsilonSBScaledDxy = (TH1D*)hMuonUpsilonSBDxy->Clone("hMuonUpsilonSBScaledDxy");
  hMuonUpsilonSBScaledDxy->Scale(bkgScaleRatioUpsilon);
  hMuonUpsilonBKGSubDxy->Add(hMuonUpsilonSBScaledDxy,-1);
  TH1D* hMuonUpsilonBKGSubDz = (TH1D*)hMuonUpsilonPKDz->Clone("hMuonUpsilonBKGSubDz");
  TH1D* hMuonUpsilonSBScaledDz = (TH1D*)hMuonUpsilonSBDz->Clone("hMuonUpsilonSBScaledDz");
  hMuonUpsilonSBScaledDz->Scale(bkgScaleRatioUpsilon);
  hMuonUpsilonBKGSubDz->Add(hMuonUpsilonSBScaledDz,-1);


  // Keep this line here!
  outFile->cd();

  // Create and write canvases here

  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // MuonPromptTemplateDataProducer_cxx
