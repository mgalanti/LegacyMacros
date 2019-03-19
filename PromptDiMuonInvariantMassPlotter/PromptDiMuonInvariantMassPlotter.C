#ifndef PromptDiMuonInvariantMassPlotter_cxx
#define PromptDiMuonInvariantMassPlotter_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 3

#include "PromptDiMuonInvariantMassPlotter.h"
#include "../utilities/constants.h"

void PromptDiMuonInvariantMassPlotter::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Book histograms here
  TH1D* hDiMuOSInvariantMass = Create1DHistogram<TH1D>("hDiMuOSInvariantMass","OS di-#mu invariant mass", 20000.,0.,200.,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuOSPromptInvariantMass = Create1DHistogram<TH1D>("hDiMuOSPromptInvariantMass","OS di-#mu invariant mass", 20000.,0.,200.,"M_{#mu#mu} [GeV]","N_{#mu#mu}");
  TH1D* hDiMuOSLogInvariantMass = Create1DHistogram<TH1D>("hDiMuOSLogInvariantMass","OS di-#mu invariant mass (log)", 3000.,-0.7,2.7,"log_{10}(M_{#mu#mu})","N_{#mu#mu}");
  TH1D* hDiMuOSPromptLogInvariantMass = Create1DHistogram<TH1D>("hDiMuOSPromptLogInvariantMass","OS di-#mu invariant mass (log)", 3000.,-0.7,2.7,"log_{10}(M_{#mu#mu})","N_{#mu#mu}");
  TH1D* hDiMuOSVtxCTau = Create1DHistogram<TH1D>("hDiMuOSVtxCTau", "c#tau of OS di-#mu vertex", 1000,-10.,10.,"c#tau [cm]","N_{#mu#mu}");
  TH1D* hDiMuOSVtxCTauError = Create1DHistogram<TH1D>("hDiMuOSVtxCTauError", "#sigma(c#tau) of OS di-#mu vertex", 1000,0.,2.,"#sigma(c#tau) [cm]","N_{#mu#mu}");
  TH1D* hDiMuOSVtxCTauSignificance = Create1DHistogram<TH1D>("hDiMuOSVtxCTauSignificance", "c#tau/#sigma(c#tau) of OS di-#mu vertex", 1000,-50.,50.,"c#tau/#sigma(c#tau)","N_{#mu#mu}");
  TH1D* hDiMuOSVtxProb = Create1DHistogram<TH1D>("hDiMuOSVtxProb", "Probability of OS di-#mu vertex", 1000,0.,1.,"P","N_{#mu#mu}");


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
    if(!s->SelectEventWithDiMuonResonanceV0(text.c_str(), vSelectedMuons, iSelectedVertex))
    {
      //       std::cout << "Event " << jentry << " not selected!\n";
      continue;
    }
    //     std::cout << "Event " << jentry << " selected!\n";
    unsigned short iMuon0 = vSelectedMuons[0];
    unsigned short iMuon1 = vSelectedMuons[1];
    unsigned short iTrack0 = Muon_trackref->at(iMuon0);
    unsigned short iTrack1 = Muon_trackref->at(iMuon1);
    short charge0 = Track_charge->at(iTrack0);
    short charge1 = Track_charge->at(iTrack1);
    if(charge0 != charge1)
    {
      TLorentzVector pMuon0,pMuon1;
      pMuon0.SetPtEtaPhiM(Track_pt->at(iTrack0),Track_eta->at(iTrack0),Track_phi->at(iTrack0),constants::muonMass);
      pMuon1.SetPtEtaPhiM(Track_pt->at(iTrack1),Track_eta->at(iTrack1),Track_phi->at(iTrack1),constants::muonMass);
      double invariantMass = (pMuon0 + pMuon1).M();
      double logInvariantMass = log10(invariantMass);
      hDiMuOSInvariantMass->Fill(invariantMass);
      hDiMuOSLogInvariantMass->Fill(logInvariantMass);
      unsigned int iVtx = 65535;
      double diMuonVtxProb = 0.;
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
        if(invariantMass < 3.0 || invariantMass > 3.2)
          continue;
        if((pMuon0+pMuon1).Perp() < 10)
          continue;
        diMuonVtxProb = Muon_vDiMuVtxProb->at(iMuon0).at(iVtx);
        diMuonVtxCTau = Muon_vDiMuVtxCTau->at(iMuon0).at(iVtx);
        diMuonVtxCTauError = Muon_vDiMuVtxCTauError->at(iMuon0).at(iVtx);
        diMuonVtxCTauSignificance = diMuonVtxCTau/diMuonVtxCTauError;
        hDiMuOSVtxCTau->Fill(diMuonVtxCTau);
        hDiMuOSVtxCTauError->Fill(diMuonVtxCTauError);
        hDiMuOSVtxCTauSignificance->Fill(diMuonVtxCTauSignificance);
        hDiMuOSVtxProb->Fill(diMuonVtxProb);
        if(diMuonVtxProb > 0.25)
        {
          hDiMuOSPromptInvariantMass->Fill(invariantMass);
          hDiMuOSPromptLogInvariantMass->Fill(logInvariantMass);
        }
      }
    }
  }

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here

  // Uncomment this line to write also the histograms to the file
 outFile->Write();
}

#endif // PromptDiMuonInvariantMassPlotter_cxx
