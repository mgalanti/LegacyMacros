#ifndef MuonPromptTemplateMCProducer_cxx
#define MuonPromptTemplateMCProducer_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 2

// #define mgDEBUG

#include "MuonPromptTemplateMCProducer.h"
#include "../utilities/constants.h"

void MuonPromptTemplateMCProducer::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Book histograms here
  TH1D* hMuonJPsiDxyz = Create1DHistogram<TH1D>("hMuonJPsiDxyz","d_{xyz} template for muons from J/#psi",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonJPsiDxy = Create1DHistogram<TH1D>("hMuonJPsiDxy","d_{xy} template for muons from J/#psi",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonJPsiDz = Create1DHistogram<TH1D>("hMuonJPsiDz","d_{z} template for muons from J/#psi",1000,-0.5,0.5,"d_{z}","N_{#mu}");

  TH1D* hMuonUpsilonDxyz = Create1DHistogram<TH1D>("hMuonUpsilonDxyz","d_{xyz} template for muons from #Upsilon",500,0.,0.5,"d_{xyz}","N_{#mu}");
  TH1D* hMuonUpsilonDxy = Create1DHistogram<TH1D>("hMuonUpsilonDxy","d_{xy} template for muons from #Upsilon",500,0.,0.5,"d_{xy}","N_{#mu}");
  TH1D* hMuonUpsilonDz = Create1DHistogram<TH1D>("hMuonUpsilonDz","d_{z} template for muons from #Upsilon",1000,-0.5,0.5,"d_{z}","N_{#mu}");


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
    std::vector<short int> muonClass;
    if(Muon_pt->at(vSelectedMuons[0]) < Muon_pt->at(vSelectedMuons[1]))
    {
      std::cout << "E R R O R : Muons are not pt-ordered!\n";
      exit(1);
    }
    for(unsigned short i = 0; i < vSelectedMuons.size(); i++)
    {
      unsigned short iMuon = vSelectedMuons[i];
      muonClass.push_back(FindMuonClassFromMCAssociation(iMuon));
    }
    if(muonClass.size() == 2)
    {
      // Position of real primary vertex
      // By definition the gen vtx is the point at which the hard interaction happens
      double vtxGenX = Genp_vx->at(2);
      double vtxGenY = Genp_vy->at(2);
      double vtxGenZ = Genp_vz->at(2);
      for(unsigned int iMuon = 0; iMuon < 2 ; iMuon++)
      {
//         if(muonClass[iMuon] != 3)
//           continue;

        unsigned short iTrack = Muon_trackref->at(vSelectedMuons[iMuon]);
        AssociateInnerMuonTrack(vSelectedMuons[iMuon]);
        if(muonInTrkGenp == 65535)
          continue;
        std::cout << "Muon is associated to a good genp.\n";
        if(abs(muonInTrkGenpId) != 13)
          continue;
        std::cout << "Muon is associated to a good genmuon.\n";
        // Find mother
        int mom = Genp_vMom->at(muonInTrkGenp).at(0);
        int momId = Genp_Id->at(mom);
        // Position of the production vertex of genp associated to muon 0
        double genpX = Genp_vx->at(mom);
        double genpY = Genp_vy->at(mom);
        double genpZ = Genp_vz->at(mom);

        if(abs(momId)!=443 && abs(momId)!=553)
          continue;

        std::cout << "Muon comes from J/Psi or Y. Mom Id = " << momId << "\n";
        // Check that first genp is prompt
        if(fabs(genpX-vtxGenX)>0.000001 || fabs(genpY-vtxGenY)>0.000001 || fabs(genpZ-vtxGenZ)>0.000001)
        {
          std::cout << "Mom is not prompt!\n";
          std::cout << "vtx = " << vtxGenX << ", " << vtxGenY << ", " << vtxGenZ << std::endl;
          std::cout << "gen = " << genpX << ", " << genpY << ", " << genpZ << std::endl;
          continue;
        }
        std::cout << "Mom is prompt.\n";

        double trackDxyz = Track_dxyz_PV->at(iTrack).at(iSelectedVertex);
        double trackDxy = Track_dxy_PV->at(iTrack).at(iSelectedVertex);
        double trackDz = Track_dz_PV->at(iTrack).at(iSelectedVertex);
        if(abs(momId) == 443)
        {
          hMuonJPsiDxyz->Fill(trackDxyz);
          hMuonJPsiDxy->Fill(trackDxy);
          hMuonJPsiDz->Fill(trackDz);
        }
        if(abs(momId) == 553)
        {
          hMuonUpsilonDxyz->Fill(trackDxyz);
          hMuonUpsilonDxy->Fill(trackDxy);
          hMuonUpsilonDz->Fill(trackDz);
        }
      }
    }
    else
    {
      std::cout << "muonClass.size() = " << muonClass.size() << std::endl;
      std::cout << "E R R O R: muonClass has != 2 entries! Exiting!\n";
      exit(1);
    }
  }

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here

  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // MuonPromptTemplateMCProducer_cxx
