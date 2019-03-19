#ifndef BmmL1TTImplementation_cxx
#define BmmL1TTImplementation_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "BmmL1TTImplementation.h"
#include "../utilities/constants.h"

#define NMUNOTLTTWO 0
#define MUPTNOTLOW 1
#define MUETANOTHIGH 2
#define DIMUCHARGEOK 3
#define DIMUPTNOTLOW 4
#define DIMUETANOTHIGH 5
#define DIMUMASSNOTLOW 6
#define DIMUMASSNOTHIGH 7
#define DIMUDELTADZNOTHIGH 8
 
void BmmL1TTImplementation::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Book histograms here
  TH1D* hEventCodes = Create1DHistogram<TH1D>("hEventCodes", "Event codes", 512, -0.5, 511.5, "Event code", "N. events");
  TH1D* hMuPt = Create1DHistogram<TH1D>("hMuPt", "Muon transverse momentum", 200, 0., 50., "p_{T} [GeV]", "N. muons");
  TH1D* hMuEta = Create1DHistogram<TH1D>("hMuEta", "Muon pseudorapidity", 200, -5., 5., "#eta", "N. muons");
  TH1D* hMuDz = Create1DHistogram<TH1D>("hMuDz", "Muon dz", 200, -50., 50., "d_{z} [cm]", "N. muons");
  TH1D* hDiMuPt = Create1DHistogram<TH1D>("hDiMuPt", "Di-muon transverse momentum", 200, 0., 50., "p_{T} [GeV]", "N. events");
  TH1D* hDiMuEta = Create1DHistogram<TH1D>("hDiMuEta", "Di-muon pseudorapidity", 200, -5., 5., "#eta", "N. events");
  TH1D* hDiMuCharge = Create1DHistogram<TH1D>("hDiMuCharge", "Di-muon charge", 5, -2.5, 2.5, "Charge", "N. events");
  TH1D* hDiMuInvMass = Create1DHistogram<TH1D>("hDiMuInvMass", "Di-muon invariant mass", 1000, 0., 10., "M [GeV]", "N. events");
  TH1D* hDiMuDeltaDz = Create1DHistogram<TH1D>("hDiMuDeltaDz", "Di-muon #Delta(dz)", 200, -50., 50., "#Delta(d_{z}) [cm]", "N. events");
  
  TH2D* hMuPtVsDiMuPt = Create2DHistogram<TH2D>("hMuPtVsDiMuPt", "Muon p_{T} vs. di-muon p_{T}", 200, 0., 50., 200, 0., 50., "di-muon p_{T} [GeV]", "muon p_{T} [GeV]");
  TH2D* hMuEtaVsDiMuEta = Create2DHistogram<TH2D>("hMuEtaVsDiMuEta", "Muon #eta vs. di-muon #eta", 200, -5., 5., 200, 5., 5., "di-muon #eta", "muon #eta");
  
  double minMuonPtThreshold = 3.;
  double maxMuonEtaThreshold = 2.;
  double maxDiMuonDeltaDz = 1.;
  double minDiMuonPtThreshold = 4.;
  double maxDiMuonEtaThreshold = 2.;
  double minDiMuonInvariantMass = 3.9;
  double maxDiMuonInvariantMass = 6.9;
  double chargeSelection = -1;
  
//   double minMuonPtThreshold = 3.75;
//   double maxMuonEtaThreshold = 99999;
//   double maxDiMuonDeltaDz = 99999;
//   double minDiMuonPtThreshold = 0.;
//   double maxDiMuonEtaThreshold = 99999;
//   double minDiMuonInvariantMass = 0.;
//   double maxDiMuonInvariantMass = 99999;
//   double chargeSelection = 0; // -1 = OC; 0 = no selection; +1 = SC
  
  unsigned int nEventsTriggered = 0;
  unsigned int nEventsTriggeredNewCount = 0;
  unsigned int nEventsWithLessThanTwoMuons = 0;
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    unsigned int eventSelectionBitMask = 0;
    
    unsigned int nMuons = L1TkMuons_l1TTTrackRef->size();
//     std::cout << "Event " << jentry << ", nMuons " << nMuons << std::endl;
    if(nMuons < 2)
    {
      nEventsWithLessThanTwoMuons++;
//       std::cout << "Event not triggered: less than 2 muons.\n";
//       continue;
    }
    else
    {
      eventSelectionBitMask=eventSelectionBitMask|(1<<NMUNOTLTTWO);
    }
//     std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
    
    if(!eventSelectionBitMask)
    {
      hEventCodes->Fill(eventSelectionBitMask);
      continue;
    }
    unsigned int nSelectedDiMuons = 0;
    unsigned int nSelectedDiMuonsNewCount = 0;
//     bool muonPtNotLow = false;
//     bool muonPtNotHigh = false;
//     bool muonEtaNotLow = false;
//     bool muonEtaNotHigh = false;
    
    for(unsigned int iMuon = 0; iMuon < nMuons; iMuon++)
    {
//       std::cout << "iMuon " << iMuon << std::endl;
      short int iTrackRef = L1TkMuons_l1TTTrackRef->at(iMuon);
      TLorentzVector piMuon;
      piMuon.SetXYZM(L1TTTracks_theMomentum5Par_x->at(iTrackRef), L1TTTracks_theMomentum5Par_y->at(iTrackRef), L1TTTracks_theMomentum5Par_z->at(iTrackRef), constants::muonMass);
      
      short int chargeiMuon = L1TTTracks_theRInv5Par->at(iMuon)/fabs(L1TTTracks_theRInv5Par->at(iMuon));
      double dziMuon = L1TTTracks_thePOCA5Par_z->at(iTrackRef);
      for(unsigned int jMuon = iMuon+1; jMuon < nMuons; jMuon++)
      {
//         std::cout << "jMuon " << jMuon << std::endl;
        short int jTrackRef = L1TkMuons_l1TTTrackRef->at(jMuon);
        TLorentzVector pjMuon;
        pjMuon.SetXYZM(L1TTTracks_theMomentum5Par_x->at(jTrackRef), L1TTTracks_theMomentum5Par_y->at(jTrackRef), L1TTTracks_theMomentum5Par_z->at(jTrackRef), constants::muonMass);
        
        short int chargejMuon = L1TTTracks_theRInv5Par->at(jMuon)/fabs(L1TTTracks_theRInv5Par->at(jMuon));
        double dzjMuon = L1TTTracks_thePOCA5Par_z->at(jTrackRef);
        
        TLorentzVector pDiMuon = piMuon + pjMuon;
        
        if(piMuon.Perp() >= minMuonPtThreshold &&
           pjMuon.Perp() >= minMuonPtThreshold &&
           fabs(piMuon.Eta()) < maxMuonEtaThreshold &&
           fabs(pjMuon.Eta()) < maxMuonEtaThreshold &&
           (chargeSelection?(chargejMuon == chargeSelection * chargeiMuon):true) &&
           pDiMuon.Perp() >= minDiMuonPtThreshold &&
           fabs(pDiMuon.Eta()) < maxDiMuonEtaThreshold &&
           pDiMuon.M() > minDiMuonInvariantMass &&
           pDiMuon.M() < maxDiMuonInvariantMass &&
           fabs(dziMuon - dzjMuon) <= maxDiMuonDeltaDz)
        {
          nSelectedDiMuonsNewCount++;
          std::cout << "mu 1 px = " << piMuon.X() << ", py = " << piMuon.Y() << ", pz = " << piMuon.Z() << ", E = " << piMuon.E() << std::endl;
          std::cout << "mu 2 px = " << pjMuon.X() << ", py = " << pjMuon.Y() << ", pz = " << pjMuon.Z() << ", E = " << pjMuon.E() << std::endl;
          std::cout << "mu 1 pt = " << piMuon.Perp() << std::endl;
          std::cout << "mu 2 pt = " << pjMuon.Perp() << std::endl;
          std::cout << "mu 1 eta = " << piMuon.Eta() << std::endl;
          std::cout << "mu 2 eta = " << pjMuon.Eta() << std::endl;
          std::cout << "mu 1 charge = " << chargeiMuon << std::endl;
          std::cout << "mu 2 charge = " << chargejMuon << std::endl;
          std::cout << "mu 1 dz = " << dziMuon << std::endl;
          std::cout << "mu 2 dz = " << dzjMuon << std::endl;
          std::cout << "dimu px = " << pDiMuon.X() << ", py = " << pDiMuon.Y() << ", pz = " << pDiMuon.Z() << ", E = " << pDiMuon.E() << std::endl;
          std::cout << "dimu pt = " << pDiMuon.Perp() << std::endl;
          std::cout << "dimu eta = " << pDiMuon.Eta() << std::endl;
          std::cout << "dimu M = " << pDiMuon.M() << std::endl;
        }
        
        if(fabs(piMuon.Eta()) < maxMuonEtaThreshold &&
           fabs(pjMuon.Eta()) < maxMuonEtaThreshold &&
           (chargeSelection?(chargejMuon == chargeSelection * chargeiMuon):true) &&
           fabs(pDiMuon.Eta()) < maxDiMuonEtaThreshold &&
           pDiMuon.M() > minDiMuonInvariantMass &&
           pDiMuon.M() < maxDiMuonInvariantMass &&
           fabs(dziMuon - dzjMuon) <= maxDiMuonDeltaDz)
        {
          hMuPt->Fill(piMuon.Perp());
          hMuPt->Fill(pjMuon.Perp());
          hDiMuPt->Fill(pDiMuon.Perp());
          hMuPtVsDiMuPt->Fill(pDiMuon.Perp(), piMuon.Perp());
          hMuPtVsDiMuPt->Fill(pDiMuon.Perp(), pjMuon.Perp());
        }
        
        if(piMuon.Perp() >= minMuonPtThreshold &&
          pjMuon.Perp() >= minMuonPtThreshold &&
          (chargeSelection?(chargejMuon == chargeSelection * chargeiMuon):true) &&
          pDiMuon.Perp() >= minDiMuonPtThreshold &&
          pDiMuon.M() > minDiMuonInvariantMass &&
          pDiMuon.M() < maxDiMuonInvariantMass &&
          fabs(dziMuon - dzjMuon) <= maxDiMuonDeltaDz)
        {
          hMuEta->Fill(piMuon.Eta());
          hMuEta->Fill(pjMuon.Eta());
          hDiMuEta->Fill(pDiMuon.Eta());
          hMuEtaVsDiMuEta->Fill(pDiMuon.Eta(),piMuon.Eta());
          hMuEtaVsDiMuEta->Fill(pDiMuon.Eta(),pjMuon.Eta());
        }
        
        if(piMuon.Perp() >= minMuonPtThreshold &&
          pjMuon.Perp() >= minMuonPtThreshold &&
          fabs(piMuon.Eta()) < maxMuonEtaThreshold &&
          fabs(pjMuon.Eta()) < maxMuonEtaThreshold &&
          pDiMuon.Perp() >= minDiMuonPtThreshold &&
          fabs(pDiMuon.Eta()) < maxDiMuonEtaThreshold &&
          pDiMuon.M() > minDiMuonInvariantMass &&
          pDiMuon.M() < maxDiMuonInvariantMass &&
          fabs(dziMuon - dzjMuon) <= maxDiMuonDeltaDz)
        {
          hDiMuCharge->Fill(chargeiMuon + chargejMuon);
        }

        if(piMuon.Perp() >= minMuonPtThreshold &&
          pjMuon.Perp() >= minMuonPtThreshold &&
          fabs(piMuon.Eta()) < maxMuonEtaThreshold &&
          fabs(pjMuon.Eta()) < maxMuonEtaThreshold &&
          (chargeSelection?(chargejMuon == chargeSelection * chargeiMuon):true) &&
          pDiMuon.Perp() >= minDiMuonPtThreshold &&
          fabs(pDiMuon.Eta()) < maxDiMuonEtaThreshold &&
          fabs(dziMuon - dzjMuon) <= maxDiMuonDeltaDz)
        {
          hDiMuInvMass->Fill(pDiMuon.M());
        }
        
        if(piMuon.Perp() >= minMuonPtThreshold &&
          pjMuon.Perp() >= minMuonPtThreshold &&
          fabs(piMuon.Eta()) < maxMuonEtaThreshold &&
          fabs(pjMuon.Eta()) < maxMuonEtaThreshold &&
          (chargeSelection?(chargejMuon == chargeSelection * chargeiMuon):true) &&
          pDiMuon.Perp() >= minDiMuonPtThreshold &&
          fabs(pDiMuon.Eta()) < maxDiMuonEtaThreshold &&
          pDiMuon.M() > minDiMuonInvariantMass &&
          pDiMuon.M() < maxDiMuonInvariantMass)
        {
          hMuDz->Fill(dziMuon);
          hMuDz->Fill(dzjMuon);
          hDiMuDeltaDz->Fill(dziMuon - dzjMuon);
        }
      }
    }
    
    for(unsigned int iMuon = 0; iMuon < nMuons; iMuon++)
    {
//       std::cout << "iMuon " << iMuon << std::endl;
      short int iTrackRef = L1TkMuons_l1TTTrackRef->at(iMuon);
      TLorentzVector piMuon;
      piMuon.SetXYZM(L1TTTracks_theMomentum5Par_x->at(iTrackRef), L1TTTracks_theMomentum5Par_y->at(iTrackRef), L1TTTracks_theMomentum5Par_z->at(iTrackRef), constants::muonMass);
      
      short int chargeiMuon = L1TTTracks_theRInv5Par->at(iMuon)/fabs(L1TTTracks_theRInv5Par->at(iMuon));
      double dziMuon = L1TTTracks_thePOCA5Par_z->at(iTrackRef);
      
      
      if(piMuon.Perp() < minMuonPtThreshold)
      {
//         std::cout << "First muon pt too low: " << piMuon.Perp() << ".\n";
//         std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        continue;
      }
      else
      {
        eventSelectionBitMask = eventSelectionBitMask|(1<<MUPTNOTLOW);
//         std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
      }

      if(fabs(piMuon.Eta()) >= maxMuonEtaThreshold)
      {
//         std::cout << "First muon eta too high: " << piMuon.Eta() << ".\n";
//         std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        continue;
      }
      else
      {
        eventSelectionBitMask = eventSelectionBitMask|(1<<MUETANOTHIGH);
//         std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
      }
//       std::cout << "First Muon OK\n";
      
      for(unsigned int jMuon = iMuon+1; jMuon < nMuons; jMuon++)
      {
//         std::cout << "jMuon " << jMuon << std::endl;
        short int jTrackRef = L1TkMuons_l1TTTrackRef->at(jMuon);
        TLorentzVector pjMuon;
        pjMuon.SetXYZM(L1TTTracks_theMomentum5Par_x->at(jTrackRef), L1TTTracks_theMomentum5Par_y->at(jTrackRef), L1TTTracks_theMomentum5Par_z->at(jTrackRef), constants::muonMass);
      
        short int chargejMuon = L1TTTracks_theRInv5Par->at(jMuon)/fabs(L1TTTracks_theRInv5Par->at(jMuon));
        double dzjMuon = L1TTTracks_thePOCA5Par_z->at(jTrackRef);
        
        if(pjMuon.Perp() < minMuonPtThreshold)
        {
//           std::cout << "Second muon pt too low: " << pjMuon.Perp() << ".\n";
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
          continue;
        }
        else
        {
          eventSelectionBitMask = eventSelectionBitMask|(1<<MUPTNOTLOW);
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        }
        
        if(fabs(pjMuon.Eta()) >= maxMuonEtaThreshold)
        {
//           std::cout << "Second muon eta too high: " << pjMuon.Eta() << ".\n";
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
          continue;
        }
        else
        {
          eventSelectionBitMask = eventSelectionBitMask|(1<<MUETANOTHIGH);
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        }
//         std::cout << "Second Muon OK\n";
        
        if(chargeSelection && chargejMuon != chargeSelection * chargeiMuon)
        {
//           std::cout << "Dimuon charge not good: i " << chargeiMuon << ", j " << chargejMuon << ".\n";
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
          continue;
        }
        else
        {
          eventSelectionBitMask = eventSelectionBitMask|(1<<DIMUCHARGEOK);
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        }
        TLorentzVector pDiMuon = piMuon + pjMuon;
        
        if(pDiMuon.Perp() < minDiMuonPtThreshold)
        {
//           std::cout << "Dimuon pt too low: " << pDiMuon.Perp() << ".\n";
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
          continue;
        }
        else
        {
          eventSelectionBitMask = eventSelectionBitMask|(1<<DIMUPTNOTLOW);
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        }
        if(fabs(pDiMuon.Eta()) >= maxDiMuonEtaThreshold)
        {
//           std::cout << "Dimuon eta too high: " << pDiMuon.Eta() << ".\n";
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
          continue;
        }
        else
        {
          eventSelectionBitMask = eventSelectionBitMask|(1<<DIMUETANOTHIGH);
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        }
        if(pDiMuon.M() <= minDiMuonInvariantMass)
        {
//           std::cout << "Dimuon mass too low: " << pDiMuon.M() << ".\n";
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
          continue;
        }
        else
        {
          eventSelectionBitMask = eventSelectionBitMask|(1<<DIMUMASSNOTLOW);
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        }
        if(pDiMuon.M() >= maxDiMuonInvariantMass)
        {
//           std::cout << "Dimuon mass too high: " << pDiMuon.M() << ".\n";
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
          continue;
        }
        else
        {
          eventSelectionBitMask = eventSelectionBitMask|(1<<DIMUMASSNOTHIGH);
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        }
        if(fabs(dziMuon - dzjMuon) > maxDiMuonDeltaDz)
        {
//           std::cout << "Dimuon dz too high: " << fabs(dziMuon - dzjMuon) << ".\n";
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
          continue;
        }
        else
        {
          eventSelectionBitMask = eventSelectionBitMask|(1<<DIMUDELTADZNOTHIGH);
//           std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
        }
//         std::cout << "Dimuon pair OK\n";
        nSelectedDiMuons++;
      }
    }
    
    if(nSelectedDiMuons > 0)
    {
      nEventsTriggered++;
      std::cout << "EVENT TRIGGERED!!!!!\n";
      std::cout << "event " << jentry << " eventSelectionBitMask " << eventSelectionBitMask << std::endl;
    }
    if(nSelectedDiMuonsNewCount > 0)
    {
      nEventsTriggeredNewCount++;
    }
    hEventCodes->Fill(eventSelectionBitMask);
    
  }

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here
  CreateAndWriteCanvas("cEventCodes", 0, 21, 1, false, true, hEventCodes);
  CreateAndWriteCanvas("cMuPt", 0, 21, 1, false, true, hMuPt);
  CreateAndWriteCanvas("cMuEta", 0, 21, 1, false, false, hMuEta);
  CreateAndWriteCanvas("cMuDz", 0, 21, 1, false, false, hMuDz);
  CreateAndWriteCanvas("cDiMuPt", 0, 21, 1, false, true, hDiMuPt);
  CreateAndWriteCanvas("cDiMuEta", 0, 21, 1, false, false, hDiMuEta);
  CreateAndWriteCanvas("cDiMuCharge", 0, 21, 1, false, false, hDiMuCharge);
  CreateAndWriteCanvas("cDiMuInvMass", 0, 21, 1, false, false, hDiMuInvMass);
  CreateAndWriteCanvas("cDiMuDeltaDz", 0, 21, 1, false, false, hDiMuDeltaDz);
  
  CreateAndWriteCanvas("cMuPtVsDiMuPt", "colz", false, false, false, hMuPtVsDiMuPt);
  CreateAndWriteCanvas("cMuEtaVsDiMuEta", "colz", false, false, false, hMuEtaVsDiMuEta);
  
  std::cout << "nEventsTriggered = " << nEventsTriggered << std::endl;
  std::cout << "nEventsTriggeredNewCount = " << nEventsTriggeredNewCount << std::endl;
  std::cout << "nEventsWithLessThanTwoMuons = " << nEventsWithLessThanTwoMuons << std::endl;
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // BmmL1TTImplementation_cxx
