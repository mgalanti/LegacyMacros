#ifndef FragmentationAnalysis_cxx
#define FragmentationAnalysis_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "FragmentationAnalysis.h"
#include "../utilities/constants.h"

void FragmentationAnalysis::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Book histograms here
  TH1D* hz = Create1DHistogram<TH1D>("hz", "z", 100, 0., 2., "z","N_{b}");

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    for(unsigned int iGen = 0; iGen < Genp_Id->size(); iGen++)
    {
      if(abs(Genp_Id->at(iGen)) == 92)
      {
//         std::cout << "Found a string!\n";
        int iBQuark = LookForMotherId(iGen, 5, false);
        if(iBQuark != 65535)
        {
//           std::cout << "Found a b!\n";
          TLorentzVector* pb = new TLorentzVector();
          pb->SetPtEtaPhiM(Genp_pt->at(iBQuark), Genp_eta->at(iBQuark), Genp_phi->at(iBQuark), Genp_mass->at(iBQuark));
          int iBHad = LookForDaughterId(iGen, 521, false);
          if(iBHad != 65535)
          {
            std::cout << "Found a daughter B hadron!\n";
            std::cout << "status of the B hadron = " << Genp_status->at(iBHad) << std::endl;
            TLorentzVector* pBHad = new TLorentzVector();
            pBHad->SetPtEtaPhiM(Genp_pt->at(iBHad), Genp_eta->at(iBHad), Genp_phi->at(iBHad), Genp_mass->at(iBHad));
           double eBHad = pBHad->E();
            std::cout << "eBHad = " << eBHad << std::endl;
            double eb = pb->E();
            std::cout << "eb = " << eb << std::endl;
            TVector3 pbSpatial = pb->Vect();
            TVector3 pBHadSpatial = pBHad->Vect();
            double angle = pBHadSpatial.Angle(pbSpatial);
            std::cout << "angle = " << angle << std::endl;
            double pBHadSpatialProj = fabs(pBHadSpatial.Mag() * cos(angle));
            std::cout << "pBHadSpatialProj = " << pBHadSpatialProj << std::endl;
            double pbSpatialMag = pbSpatial.Mag();
            std::cout << "pbSpatialMag = " << pbSpatialMag << std::endl;
            double z = (eBHad+pBHadSpatialProj)/(eb+pbSpatialMag);
            std::cout << "z = " << z << std::endl;
            hz->Fill(z);
            
          }
        }
      }
    }
  }

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here
  CreateAndWriteCanvas("cz", 0,21,1,false,false,hz);
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // FragmentationAnalysis_cxx
