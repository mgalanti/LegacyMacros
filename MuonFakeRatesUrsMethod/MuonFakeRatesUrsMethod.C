#ifndef MuonFakeRatesUrsMethod_cxx
#define MuonFakeRatesUrsMethod_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "MuonFakeRatesUrsMethod.h"
#include "../utilities/constants.h"

void MuonFakeRatesUrsMethod::Loop()
{
  
  InitializeMuonTMVA();
  
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  double binsx[7] = {0,4,5,7,8,10,20};
  
  unsigned int fDoTruthMatching = 3;
  int fPdgId = 321;
  int fNchan = 2;
  // Book histograms here
  std::vector<TH1D*> fhptPosAll, fhptPosPass, fhptNegAll, fhptNegPass;
  TH2D *fh2PosAll, *fh2PosPass, *fh2NegAll, *fh2NegPass;
  
  TH1D *h1(0); 
  for (int i = 0; i < fNchan; ++i) {
    h1 = new TH1D(Form("hptPosAll%d", i), Form("hptPosAll%d", i), 6, binsx); h1->Sumw2(); setTitles(h1, "p_{T} [GeV]", "Entries/bin"); 
    fhptPosAll.push_back(h1);  
    h1 = new TH1D(Form("hptPosPass%d", i), Form("hptPosAll%d", i), 6, binsx);h1->Sumw2(); setTitles(h1, "p_{T} [GeV]", "Entries/bin"); 
    fhptPosPass.push_back(h1); 
    
    h1 = new TH1D(Form("hptNegAll%d", i), Form("hptNegAll%d", i), 6, binsx);h1->Sumw2(); setTitles(h1, "p_{T} [GeV]", "Entries/bin"); 
    fhptNegAll.push_back(h1); 
    h1 = new TH1D(Form("hptNegPass%d", i), Form("hptNegAll%d", i), 6, binsx);h1->Sumw2(); setTitles(h1, "p_{T} [GeV]", "Entries/bin"); 
    fhptNegPass.push_back(h1); 
    
  }
  
  // -- 2D versions for PidTables
  fh2PosAll = new TH2D("h2PosAll", "hptPosAll", 2, 0., 2.8, 150, 0., 30.); 
  fh2PosAll->Sumw2(); setTitles(fh2PosAll, "p_{T} [GeV]", "#eta");
  
  fh2PosPass = new TH2D("h2PosPass", "hptPosPass", 2, 0., 2.8, 150, 0., 30.); 
  fh2PosPass->Sumw2(); setTitles(fh2PosPass, "p_{T} [GeV]", "#eta");
  
  fh2NegAll = new TH2D("h2NegAll", "hptNegAll", 2., 0., 2.8, 150, 0., 30.); 
  fh2NegAll->Sumw2(); setTitles(fh2NegAll, "p_{T} [GeV]", "#eta"); 
  
  fh2NegPass = new TH2D("h2NegPass", "hptNegPass", 2, 0., 2.8, 150, 0., 30.); 
  fh2NegPass->Sumw2(); setTitles(fh2NegPass, "p_{T} [GeV]", "#eta"); 
  
  int fVerbose = 0;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
//     if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    unsigned int id1 = 321;
    unsigned int id2 = 321;
    unsigned int iC = 0;
    unsigned int iM1 = 0;
    unsigned int iM2 = 0;
    unsigned int iB = 0;
    unsigned int fNGenPhotons = 0;
    int fGenBTmi = 0;
    int fMu1GenID = 0;
    int fMu2GenID = 0;
    double fGenLifeTime = 0;
    int fGenM1Tmi = 0;
    int fGenM2Tmi = 0;
    bool goodMatch(false); 
    for (unsigned int i = 0; i < Genp_Id->size(); ++i)
    {
      if (531 == TMath::Abs(Genp_Id->at(i)))
      {
        iB = i;
        iM1 = iM2 = 0; 
        for (unsigned int id = 0; id < Genp_vDau->at(iB).size(); ++id)
        {
          iC = Genp_vDau->at(iB).at(id);
          if (id1 == (unsigned int)TMath::Abs(Genp_Id->at(iC)) || id2 == (unsigned int)TMath::Abs(Genp_Id->at(iC)))
          {
            if (0 == iM1)
            {
              iM1 = iC; 
            }
            else
            {
              iM2 = iC; 
            }
          }
        }
        if (0 != iM1 && 0 != iM2)
        {
          goodMatch = true; 
          fNGenPhotons = Genp_vDau->at(iB).size() - 2; 
//           if (fVerbose > 10)
//           {
//             cout << "found gen match for B gen idx = " << pB->fNumber << endl;
//           }
          break;
        }
      }
    }
    
    fGenBTmi = -1; 
    if (goodMatch) 
    {
      fGenBTmi = iB; 
      fMu1GenID = Genp_Id->at(iM1);
      fMu2GenID = Genp_Id->at(iM2);
      TLorentzVector pB;
      pB.SetPtEtaPhiM(Genp_pt->at(iB),Genp_eta->at(iB),Genp_phi->at(iB),Genp_mass->at(iB));
      double m = Genp_mass->at(iB);
      double p = pB.P();
      // Meson pointer
      //    TGenCand *pM = fpEvt->getGenCand(pB->fMom1); 
      int iM = Genp_vMom->at(iB).at(0);
      // the meson is the original except if it oscillated
      if (531 != TMath::Abs(Genp_Id->at(iM))) iM = iB;
      TVector3 vM1(Genp_vx->at(iM1),Genp_vy->at(iM1),Genp_vz->at(iM1));
      TVector3 vM(Genp_vx->at(iM),Genp_vy->at(iM),Genp_vz->at(iM));
      
      double x = (vM1 - vM).Mag(); 
      fGenLifeTime = x*m/p/TMath::Ccgs();
      
      if (Genp_pt->at(iM1)> Genp_pt->at(iM2))
      {
        fMu1GenID = Genp_Id->at(iM1);
        fMu2GenID = Genp_Id->at(iM2);
        fGenM1Tmi = iM1; 
        fGenM2Tmi = iM2; 
      }
      else
      {
        fMu1GenID = Genp_Id->at(iM2);
        fMu2GenID = Genp_Id->at(iM1);
        fGenM1Tmi = iM2; 
        fGenM2Tmi = iM1; 
      }
    }
    else
    {
      fGenM1Tmi = -1; 
      fGenM2Tmi = -1; 
    }
    
    if (fVerbose > 10) {
      std::cout << "fGenM1Tmi = " << fGenM1Tmi << std::endl;
      std::cout << "fGenM2Tmi = " << fGenM2Tmi << std::endl;
    }
    
    
    int genIdx(-1); 
    for (int i = 0; i < numberOfTrack; ++i)
    {    
      
      // -- Muon?
      int mid = 0; 
      for (int im = 0; im < numberOfMuon; ++im)
      {
        if (i == Muon_trackref->at(im))
        {
          mid = 1; 
          break;
        }
      }
      
      // Does not seem important for this macro
//       // -- truth matching with deltaR comparision
//       genIdx = -1; 
//       if (3 == fDoTruthMatching)
//       {    
//         genIdx  = getGenIndexWithDeltaR(Track_pt->at(i),Track_eta->at(i), Track_phi->at(i),Track_charge->at(i)); 
//       }
    }
    
    int fRecM1Tmi = -1;
    int fRecM2Tmi = -1; 
    for (int i = 0; i < numberOfTrack; ++i)
    {
//       std::cout << "Find gen index for track " << i << std::endl;
      genIdx = getGenIndexWithDeltaR(Track_pt->at(i),Track_eta->at(i), Track_phi->at(i),Track_charge->at(i));
      
      if (genIdx < 0) continue;
      
      // -- muon 1
      if (genIdx == fGenM1Tmi)
      {
//         std::cout << "        Found index for track 1\n";
        fRecM1Tmi = i; 
      }
      
      // -- muon 2
      if (genIdx == fGenM2Tmi)
      {
//         std::cout << "        Found index for track 2\n";
        fRecM2Tmi = i; 
      }
//       std::cout << "fRecM1Tmi = " << fRecM1Tmi << ", fRecM2Tmi = " << fRecM2Tmi << std::endl;
      // -- skip rest if both matches found
      if (fRecM1Tmi > -1 && fRecM2Tmi > -1) break;
    }
    
    if (fVerbose > 10)
    {
      std::cout << "fRecM1Tmi = " << fRecM1Tmi << " matched to fGenM1Tmi = " << fGenM1Tmi << std::endl;
      std::cout << "fRecM2Tmi = " << fRecM2Tmi << " matched to fGenM2Tmi = " << fGenM2Tmi << std::endl;
    }
    
    bool fGoodEffCand = false;
    
    // -- gen level 
    unsigned int pB(0), pM1(0), pM2(0); 
    if (-1 == fGenM1Tmi || -1 == fGenM2Tmi) 
    {
      if (fVerbose > 2 ) std::cout << "--------------------> No matched signal decay found" << std::endl;
      return;
    }
    pB  = fGenBTmi; 
    pM1 = fGenM1Tmi; 
    pM2 = fGenM2Tmi; 
    
    // -- reco level
    unsigned int prM1(0), prM2(0); 
    double bla(0); 
    int m1Matched(0), m2Matched(0), m1ID(0), m1tmID(0), m1mvaID(0), m2ID(0), m2tmID(0), m2mvaID(0), m1GT(0), m2GT(0);
    if (fRecM1Tmi > -1) 
    {
      m1Matched = 1; 
      prM1 = fRecM1Tmi; 
      unsigned int pM(0); 
      int idx =prM1;
      for (int i = 0; i < numberOfMuon; ++i)
      {
        pM = i;
        if (idx == Muon_trackref->at(pM))
        {
//           if (s->SelectMuon(pM)) m1tmID = 1; // tight muon w/o highPurity flag 
//           if (s->SelectMuon(pM)) m1mvaID = 1;  // state-of-the-art MVA selection, cut at 0.36
          
          if (s->SelectMuon(pM, "muonV2BmmPt0")) m1tmID = 1; // tight muon w/o highPurity flag 
          if (s->SelectMuon(pM)) m1mvaID = 1;  // Use the state-of-the-art MVA selection here!
        }
      }
      if (Track_isHighPurity->at(prM1))
      {
        m1GT = 1; 
      }
      else
      {
        m1GT = 0;
      }
    }
    
    if (fRecM2Tmi > -1) 
    {
      m2Matched = 1; 
      prM2 = fRecM2Tmi; 
      unsigned int pM(0); 
      int idx = prM2;
      for (int i = 0; i < numberOfMuon; ++i)
      {
        pM = i;
        if (idx == Muon_trackref->at(pM))
        {
          if (s->SelectMuon(pM, "muonV2BmmPt0")) m1tmID = 1; // tight muon w/o highPurity flag 
          if (s->SelectMuon(pM)) m2mvaID = 1;  // Use the state-of-the-art MVA selection here!
        }
      }
      if (Track_isHighPurity->at(prM2))
      {
        m2GT = 1; 
      } 
      else
      {
        m2GT = 0;
      }
    } 
    
    m1ID = m1tmID; 
    m2ID = m2tmID; 
    
//     // -- cand level 
//     TAnaCand *pCand(0);
//     if (fCandTmi > -1) {
//       pCand = fpEvt->getCand(fCandTmi);
//     }
//     
//     // -- EffTree filling for all events with a signal decay
//     fETgpt   = pB->fP.Perp(); 
//     fETgeta  = pB->fP.Eta(); 
//     fETg1pt  = pM1->fP.Perp(); 
//     fETg1eta = pM1->fP.Eta(); 
//     fETg2pt  = pM2->fP.Perp(); 
//     fETg2eta = pM2->fP.Eta(); 
//     
//     if (m1Matched) {
//       fETm1pt    = prM1->getP().Perp(); 
//       fETm1eta   = prM1->getP().Eta(); 
//       fETm1q     = prM1->getCharge();
//       fETm1gt    = (m1GT>0?true:false); 
//       fETm1id    = (m1ID>0?true:false);
//       fETm1tmid  = (m1tmID>0?true:false);
//       fETm1mvaid = (m1mvaID>0?true:false);
//     } else {
//       fETm1pt  = -99.; 
//       fETm1eta = -99.; 
//       fETm1q   = -99;
//       fETm1gt  = false; 
//       fETm1id  = false;
//       fETm1tmid  = false;
//       fETm1mvaid = false;
//     }
//     if (m2Matched) {
//       fETm2pt    = prM2->getP().Perp(); 
//       fETm2eta   = prM2->getP().Eta(); 
//       fETm2q     = prM2->getCharge();
//       fETm2gt    = (m2GT>0?true:false); 
//       fETm2id    = (m2ID>0?true:false);
//       fETm2tmid  = (m2tmID>0?true:false);
//       fETm2mvaid = (m2mvaID>0?true:false);
//     } else {
//       fETm2pt  = -99.; 
//       fETm2eta = -99.; 
//       fETm2q   = -99;
//       fETm2gt  = false; 
//       fETm2id  = false;
//       fETm2tmid  = false;
//       fETm2mvaid = false;
//     }
//     if (pCand) {
//       fETcandMass = pCand->fMass; 
//     } else {
//       fETcandMass = -99.;
//     }
//     fEffTree->Fill();
    
    
    
    
    
    
    
    
    
    
    int id(0),igenp(0),itrk(0); 
    bool mu(false), rmvaid(false), highpur(false); 
    double pt(0.), eta(0.); 
    int chan(-1); 
    for (int i = 0; i < 2; ++i)
    {
      if (0 == i)
      {
        id = fMu1GenID;
        igenp = fGenM1Tmi;
        if(fRecM1Tmi > -1)
          pt = Track_pt->at(prM1);
        else
          pt = -99;
        
//         mu = (fb.m1rmvabdt > fBDTcut);
        unsigned int pM(0); 
        int idx = prM1;
        itrk = prM1;
        bool muset(false);
        for (int j = 0; j < numberOfMuon; ++j)
        {
          pM = j;
          if (idx == Muon_trackref->at(pM))
          {
//             mu = SelectTMVAMuonV5(pM);
            mu = s->SelectMuon(pM);
            muset=true;
          }
        }
        if(!muset)
          mu=false;
//         rmvaid =  fb.m1rmvaid;
        rmvaid = m1mvaID;
        eta = Track_eta->at(prM1);
        highpur = Track_isHighPurity->at(prM1);
        chan = (TMath::Abs(eta)<1.4?0:1); 
//         std::cout << "i = " << i << ", id = " << fMu1GenID << ", eta = " << Track_eta->at(prM1) << ", pt = " << Track_pt->at(prM1) << std::endl;
      }
      
      if (1 == i)
      {
        id = fMu2GenID; 
        igenp = fGenM2Tmi;
        if(fRecM2Tmi > -1)
          pt = Track_pt->at(prM2);
        else
          pt = -99;
//         mu = (fb.m2rmvaid > fBDTcut);
        unsigned int pM(0); 
        int idx = prM2;
        itrk = prM2;
        bool muset(false);
        for (int j = 0; j < numberOfMuon; ++j)
        {
          pM = j;
          if (idx == Muon_trackref->at(pM))
          {
            mu = s->SelectMuon(pM);
            muset=true;
          }
        }
        if(!muset)
          mu=false;
//         rmvaid = fb.m2rmvaid;
        rmvaid = m2mvaID;
        eta = Track_eta->at(prM2);
        highpur = Track_isHighPurity->at(prM2);
        chan = (TMath::Abs(eta)<1.4?0:1); 
//         std::cout << "i = " << i << ", id = " << fMu2GenID << ", eta = " << Track_eta->at(prM2) << ", pt = " << Track_pt->at(prM2) << std::endl;
      }
      
      if (pt < 4. || TMath::Abs(eta) > 2.4 ) continue;
      
      if(!highpur) continue;
      
      if (fPdgId == TMath::Abs(id))
      {
        if (id > 0)
        {
//           std::cout << "URS Ev. " << evNumber << ": itrk = " << itrk << ", id = " << id << ", pos all eta = " << eta << ", pt = " << pt << std::endl;
//           std::cout << "                   Genp: i = " << igenp << ", id = " << Genp_Id->at(igenp) << ", eta = " << Genp_eta->at(igenp) << ", pt = " << Genp_pt->at(igenp) << std::endl;
          fh2PosAll->Fill(eta, pt); 
          fhptPosAll[chan]->Fill(pt);
          if (mu)
          {
            if (!rmvaid)
            {
              std::cout << "???????????????????????????????????" << std::endl;
            }
            std::cout << "URS Ev. " << evNumber << ": itrk = " << itrk << ", id = " << id << ", pos pass eta = " << eta << ", pt = " << pt << std::endl;
            std::cout << "                   Genp: i = " << igenp << ", id = " << Genp_Id->at(igenp) << ", eta = " << Genp_eta->at(igenp) << ", pt = " << Genp_pt->at(igenp) << std::endl;
            fhptPosPass[chan]->Fill(pt);
            fh2PosPass->Fill(eta, pt); 
          }
        }
        else
        {
//           std::cout << "URS Ev. " << evNumber << ": itrk = " << itrk << ", id = " << id << ", neg all eta = " << eta << ", pt = " << pt << std::endl;
//           std::cout << "                   Genp: i = " << igenp << ", id = " << Genp_Id->at(igenp) << ", eta = " << Genp_eta->at(igenp) << ", pt = " << Genp_pt->at(igenp) << std::endl;
          fh2NegAll->Fill(eta, pt); 
          fhptNegAll[chan]->Fill(pt);
          if (mu)
          {
            std::cout << "URS Ev. " << evNumber << ": itrk = " << itrk << ", id = " << id << ", neg pass eta = " << eta << ", pt = " << pt << std::endl;
            std::cout << "                   Genp: i = " << igenp << ", id = " << Genp_Id->at(igenp) << ", eta = " << Genp_eta->at(igenp) << ", pt = " << Genp_pt->at(igenp) << std::endl;
            fhptNegPass[chan]->Fill(pt);
            fh2NegPass->Fill(eta, pt); 
          }
        }
        
      }
      
    }
  }
  

  // Keep this line here!
  outFile->cd();
  
  TGraphAsymmErrors* gFakeRatePos0 = CreateGraph<TGraphAsymmErrors>("gFakeRatePos0", "Selection efficiency vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency");
  TGraphAsymmErrors* gFakeRateNeg0 = CreateGraph<TGraphAsymmErrors>("gFakeRateNeg0", "Selection efficiency vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency");
  TGraphAsymmErrors* gFakeRatePos1 = CreateGraph<TGraphAsymmErrors>("gFakeRatePos1", "Selection efficiency vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency");
  TGraphAsymmErrors* gFakeRateNeg1 = CreateGraph<TGraphAsymmErrors>("gFakeRateNeg1", "Selection efficiency vs. reco p_{T}","reco p_{T} [GeV]","Selection efficiency");
  
  BayesDivide(fhptPosPass[0],fhptPosAll[0],gFakeRatePos0);
  BayesDivide(fhptPosPass[1],fhptPosAll[1],gFakeRatePos1);
  BayesDivide(fhptNegPass[0],fhptNegAll[0],gFakeRateNeg0);
  BayesDivide(fhptNegPass[1],fhptNegAll[1],gFakeRateNeg1);
  
  // Create and write canvases here
  gFakeRatePos0->Write();
  gFakeRatePos1->Write();
  gFakeRateNeg0->Write();
  gFakeRateNeg1->Write();
  
  
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}



int MuonFakeRatesUrsMethod::getGenIndexWithDeltaR(double pt, double eta, double phi, double charge)
{
  int index(-1);
  int tmp_index[30];
  TVector3 track, gencand;
  int count(0);
  track.SetPtEtaPhi(pt, eta, phi);
  
  for (int i = 0; i < Genp_Id->size(); i++)
  {
    if ((Genp_status->at(i) == 1 || Genp_status->at(i) == 8) && (charge*Genp_charge->at(i)> 0))
    {
      gencand.SetPtEtaPhi(Genp_pt->at(i), Genp_eta->at(i), Genp_phi->at(i));
      double dR = track.DeltaR(gencand);
//       double dpt = TMath::Abs((track.Perp() - gencand.Perp())/track.Perp());
      double dpt = TMath::Abs((track.Perp() - gencand.Perp())/gencand.Perp());
      if (dR < 0.12 && dpt < 0.3  && count < 30) 
      {
        tmp_index[count] = i;
        count++;
      }
    }
  }
//   std::cout << "getGenIndexWithDeltaR: count = " << count << std::endl;
//   std::cout << "getGenIndexWithDeltaR: index = " << index << ", id = " << Genp_Id->at(index) << std::endl;
  
  if (0 == count) return index;
//   std::cout << "getGenIndexWithDeltaR: tmp_index[0] = " << tmp_index[0] << ", id = " << Genp_Id->at(tmp_index[0]) << std::endl;
  
  if (1 == count) return tmp_index[0];  
  
  double deltaR_min(0.12);
  int  i_min(-1);       
  double dr(1.); 
  for (int i = 0; i < count; i++)
  {
    gencand.SetPtEtaPhi(Genp_pt->at(tmp_index[i]), Genp_eta->at(tmp_index[i]), Genp_phi->at(tmp_index[i]));
    dr = track.DeltaR(gencand);
    if (dr < deltaR_min)
    {
      deltaR_min = dr;
      i_min = tmp_index[i];
    }
  }
  index = i_min;
  return index;
}



void MuonFakeRatesUrsMethod::InitializeMuonTMVA()
{
  std::cout << "I N F O : MuonFakeRatesUrsMethod::InitializeMuonTMVA(...) - Initializing TMVAReader for muons...\n";
  reader = new TMVA::Reader("V:Color:Silent");
  reader->AddVariable("trkValidFract", &muonTMVATrackValidFraction);
  reader->AddVariable("glbNChi2", &muonTMVAGlobalChi2);
  reader->AddVariable("pt", &muonTMVAPt);
  reader->AddVariable("eta", &muonTMVAEta);
  reader->AddVariable("segComp", &muonTMVASegmentCompatibility);
  reader->AddVariable("chi2LocMom", &muonTMVAChi2LocalMomentum);
  reader->AddVariable("chi2LocPos", &muonTMVAChi2LocalPosition);
  reader->AddVariable("glbTrackProb", &muonTMVAGlobalTrackProbability);
  reader->AddVariable("NTrkVHits", &muonTMVATrackNumberOfValidHits);
  reader->AddVariable("NTrkEHitsOut", &muonTMVATrackExpectedHitsOuter);
  std::cout << "          Checking weight file: \"../tmvaWeightFiles/TMVAClassification_BDT.weights.130507v1.xml\" for TMVA method \"BDT\"...\n";
  if(!reader->FindMVA("BDT"))
  {
    reader->BookMVA("BDT", "../tmvaWeightFiles/TMVAClassification_BDT.weights.130507v1.xml");
  }
  std::cout << "          Initializing BDT cut to match Urs' selection. BDT cut set at 0.3.\n";
  std::cout << "W A R N I N G ! This selector is BUGGY, use it at your own risk! It must only be used if you KNOW what you are doing!\n";
  std::cout << "                If you do not have a specific reason to use it, please KILL NOW the job.\n";
  muonTMVACut = 0.3;
}



bool MuonFakeRatesUrsMethod::SelectTMVAMuonV5(const unsigned int iMuon)
{
  // Check if there are the muon and the String
  if (iMuon >= numberOfMuon)
  {
    std::cout << "W A R N I N G! MuonFakeRatesUrsMethod::SelectMuon() : asking for a muon out of range\n";
    std::cout << "               Returning false.\n";
    return false;
  }
  
  unsigned int trackRef = Muon_trackref->at(iMuon);
  if(trackRef == 65535)
  {
    return false;
  }
  if(trackRef == 2048)
  {
    std::cout << "W A R N I N G ! MuonFakeRatesUrsMethod::SelectMuon() : Selection cannot be made\n";
    std::cout << "                because inner track of muon is not present in the ntuple!\n";
    std::cout << "                Returning false...\n";
    return false;
  }
  muonTMVATrackValidFraction = GetTrackValidFraction(trackRef);
  muonTMVAGlobalChi2 = Muon_globalChi2->at(iMuon);
  muonTMVAPt = Muon_pt->at(iMuon);
  muonTMVAEta = Muon_eta->at(iMuon);
  muonTMVASegmentCompatibility = Muon_segmentCompatibility->at(iMuon);
  muonTMVAChi2LocalMomentum = Muon_GlobalKink_chi2LocalMomentum->at(iMuon);
  muonTMVAChi2LocalPosition = Muon_GlobalKink_chi2LocalPosition->at(iMuon);
  muonTMVAGlobalTrackProbability = Muon_glbTrackProbability->at(iMuon);
  muonTMVATrackNumberOfValidHits = Track_numberOfValidHits->at(trackRef);
  muonTMVATrackExpectedHitsOuter = GetNumberOfLostTrackerOuterHits(trackRef);
  float muonTMVAResult = reader->EvaluateMVA("BDT");
#ifdef mgDEBUG
  std::cout << "Value of MVA for this muon is " << muonTMVAResult << std::endl;
#endif // mgDEBUG
  return (Muon_isTracker->at(iMuon) &&
          Muon_isGlobal->at(iMuon) &&
          Muon_isGlobalMuonPromptTight->at(iMuon) &&
          Muon_numberOfMatchedStations->at(iMuon) > 1 &&
          Track_layersWithMeasurement->at(trackRef) > 5 &&
          Track_validPixelHits->at(trackRef) > 0 &&
          Track_isHighPurity->at(trackRef) &&
          muonTMVAResult > muonTMVACut);
}



void MuonFakeRatesUrsMethod::setTitles(TH1 *h, const char *sx, const char *sy, float size, 
               float xoff, float yoff, float lsize, int font) 
{
  if (h == 0) 
  {
    std::cout << " Histogram not defined" << std::endl;
  } 
  else
  {
    h->SetXTitle(sx);                  h->SetYTitle(sy); 
    h->SetTitleOffset(xoff, "x");      h->SetTitleOffset(yoff, "y");
    h->SetTitleSize(size, "x");        h->SetTitleSize(size, "y");
    h->SetLabelSize(lsize, "x");       h->SetLabelSize(lsize, "y");
    h->SetLabelFont(font, "x");        h->SetLabelFont(font, "y");
    h->GetXaxis()->SetTitleFont(font); h->GetYaxis()->SetTitleFont(font);
    h->SetNdivisions(508, "X");
  }
}

#ifndef __CINT__
int main(int argc, char** argv)
{
  std::string sampleName= "MC__BsToKK_EtaPtFilter_8TeV-pythia6-evtgen__Summer12_DR53X-PU_S10_START53_V7C-v1__NoSelections";
  std::string text = "muonV4TMVABmmPt0";
  int maxEvents = 50000;
//   for(int i = 1; i < argc; i++)
//     cout << atoi(argv[i]) << endl;
//   return 0; 
  MuonFakeRatesUrsMethod m(sampleName, text, maxEvents);
  m.Loop();
}
#endif // __CINT__

#endif // MuonFakeRatesUrsMethod_cxx
