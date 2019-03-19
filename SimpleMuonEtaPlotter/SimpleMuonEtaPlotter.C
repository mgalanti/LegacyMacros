#ifndef SimpleMuonEtaPlotter_cxx
#define SimpleMuonEtaPlotter_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "SimpleMuonEtaPlotter.h"
#include "../utilities/constants.h"

void SimpleMuonEtaPlotter::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";
  
  const unsigned int nEtaBins = 60;
  const double etaMin = -3.;
  const double etaMax = 3.;
  const char* recoEtaLabel = "reco #eta";
  const char* nMuLabel = "N_{#mu}";
  
  // Book histograms here
  TH1D* hSelMuonEta = Create1DHistogram<TH1D>("hSelMuonEta","#eta of selected muons", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  TH1D* hLucaSelMuonEta = Create1DHistogram<TH1D>("hLucaSelMuonEta","#eta of selected muons (Luca algo)", nEtaBins, etaMin, etaMax, recoEtaLabel, nMuLabel);
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
//     if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    std::vector<int> pdgId;
    pdgId.push_back(13);
    pdgId.push_back(13);
    std::vector<TLorentzVector> Genp_p_saved;
    std::vector<int> Genp_i_saved;
    std::vector<int> Genp_id_saved;
    unsigned int nGenPInAcceptance = 0;
    std::vector<int> used_particles;
    for(unsigned int i = 0; i < pdgId.size(); i++)
    {
      std::cout << "In pdgId loop\n";
      for(unsigned int iGenParticle = 0; iGenParticle < Genp_status->size(); iGenParticle++)
      {
        std::cout << "  In gen particle loop\n";
        if (std::find(used_particles.begin(), used_particles.end(), iGenParticle) == used_particles.end()) 
        {
          std::cout << "    Found non used particle\n";
          if(Genp_status->at(iGenParticle) == 1 || Genp_status->at(iGenParticle) == 8)
          {
            std::cout << "    Found good status\n";
            if(abs(Genp_Id->at(iGenParticle)) == pdgId[i])
            {
              std::cout << "    Found good id\n";
              if(Genp_pt->at(iGenParticle) > 4 && fabs(Genp_eta->at(iGenParticle)) < 2.4)
              {
                std::cout << "    Found good pt and eta\n";
                TLorentzVector p;
                p.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
                Genp_p_saved.push_back(p);
                Genp_i_saved.push_back(iGenParticle);
                Genp_id_saved.push_back(Genp_Id->at(iGenParticle));
                nGenPInAcceptance++;
                used_particles.push_back(iGenParticle);
                std::cout << "    Breaking gen particle loop here\n";
                break;
              }
            }
          }
        }
      }
    }
    
    std::vector<unsigned int> Muon_i_saved;
    std::vector<TLorentzVector> Muon_p_saved;
    std::map<unsigned int, std::vector<int> > mGenp_family_saved;
    
    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      std::cout << "In reco muon loop\n";
      if(Muon_isGlobal->at(iMuon) &&
         Muon_isTracker->at(iMuon) &&
         Muon_pt->at(iMuon) > 4.0 &&
         Track_isHighPurity->at(Muon_trackref->at(iMuon)))
      {
        std::cout << "  Found good preselected muon\n";
        TLorentzVector p;
        p.SetPtEtaPhiM(Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon), constants::muonMass);
        Muon_p_saved.push_back(p);
        Muon_i_saved.push_back(iMuon);
        for(unsigned int iGenParticle = 0; iGenParticle < Genp_status->size(); iGenParticle++)
        {
          std::cout << "    In gen particle loop\n";
          if(Genp_status->at(iGenParticle) == 1 || Genp_status->at(iGenParticle) == 8)
          {
            std::cout << "      Found good status\n";
            if(Genp_pt->at(iGenParticle) > 4 && fabs(Genp_eta->at(iGenParticle)) < 2.4)
            {
              std::cout << "      Found good pt and eta\n";
              unsigned int iMuonTrack = Muon_trackref->at(iMuon);
              if(Track_charge->at(iMuonTrack) == Genp_charge->at(iGenParticle))
              {
                std::cout << "      Found good charge\n";
                TLorentzVector Genp_p;
                Genp_p.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
                if(Genp_p.DeltaR(p) < 0.1)
                {
                  std::cout << "      Found good deltaR\n";
                  std::vector<int> family;
                  unsigned int iMom = Genp_vMom->at(iGenParticle).at(0);
                  unsigned int iGrandMom = Genp_vMom->at(iMom).at(0);
                  while(Genp_Id->at(iGrandMom) > 80 && Genp_Id->at(iGrandMom) < 101)
                  {
                    iGrandMom = Genp_vMom->at(iGrandMom).at(0);
                  }
                  family.push_back(Genp_Id->at(iGenParticle));
                  family.push_back(Genp_Id->at(iMom));
                  family.push_back(Genp_Id->at(iGrandMom));
                  mGenp_family_saved[Muon_p_saved.size()-1] = family;
                  std::cout << "      Breaking gen particle loop\n";
                  break;
                }
              }
            }
          }
        }
      }
    }
    
    std::cout << "Number of saved gen particles: " << nGenPInAcceptance << std::endl;
    std::cout << "Number of saved preselected muons: " << Muon_p_saved.size() << std::endl;
    vector <unsigned int> used_saved_muons;
    vector <unsigned int> used_saved_particles;
    
    for(unsigned int iSavedGenP = 0; iSavedGenP < Genp_p_saved.size(); iSavedGenP++)
    {
      std::cout << "In saved gen particle loop\n";
      for(unsigned int iMuon = 0; iMuon < Muon_p_saved.size(); iMuon++)
      {
        bool used_m = false;
        for(unsigned int i = 0; i < used_saved_muons.size(); i++)
        {
          if (Muon_i_saved[iMuon] == used_saved_muons[i])
          {
            used_m = true;
            break;
          }
        }
        if (used_m) continue;
        bool used_p = false;
        for(unsigned int i = 0; i < used_saved_particles.size(); i++)
        {
          if(Genp_i_saved[iSavedGenP] == used_saved_particles[i]) {
            used_p = true;
            break;
          }
        }
        if (used_p) continue;
        
        std::cout << "  In saved muon loop\n";
        TLorentzVector pMuon;
        pMuon.SetPtEtaPhiM(Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon), constants::muonMass);
        if(Genp_p_saved[iSavedGenP].DeltaR(Muon_p_saved[iMuon]) < 0.1)
        {
          std::cout << "    Found good deltaR\n";
          if(s->SelectMuon(Muon_i_saved[iMuon]))
          {
            std::cout << "    Found good tight muon\n";
            if(mGenp_family_saved.find(iMuon) != mGenp_family_saved.end())
            {
              std::cout << "    Found gen particle family in map\n";
              if(abs(mGenp_family_saved[iMuon][0]) == 13 && abs(mGenp_family_saved[iMuon][1]) == 531)
              {
                used_saved_muons.push_back(Muon_i_saved[iMuon]);
                used_saved_particles.push_back(Genp_i_saved[iSavedGenP]);
                std::cout << "    Found good id and momId\n";
                hLucaSelMuonEta->Fill(Muon_p_saved[iMuon].Eta());
              }
            }
          }
        }
      }
    }
    
    for(int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      if(s->SelectMuon(iMuon)/* && Muon_hasTriggered->at(iMuon)*/)
      {
        hSelMuonEta->Fill(Muon_eta->at(iMuon));
      }
    }
  }

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here
  CreateAndWriteCanvas("cSelMuonEta",0,21,1,false,false,hSelMuonEta);
  CreateAndWriteCanvas("cLucaSelMuonEta",0,21,1,false,false,hLucaSelMuonEta);
  
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // SimpleMuonEtaPlotter_cxx
