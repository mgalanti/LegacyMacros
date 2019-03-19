#ifndef TestReco_cxx
#define TestReco_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "TestReco.h"
#include "../utilities/constants.h"

void TestReco::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Book histograms here
  TH1D* hMuonPt = Create1DHistogram<TH1D>("hMuonPt", "Muon p_{T}", 100, 0., 10., "p_{T} [GeV]", "N. muons");

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    for(unsigned int iMuon = 0; iMuon < numberOfMuon; iMuon++)
    {
      std::cout << "Muon pt = " << Muon_pt->at(iMuon) << std::endl;
      
      hMuonPt->Fill(Muon_pt->at(iMuon));
    }
    
  }

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here
  CreateAndWriteCanvas("cMuonPt", 1, 21, 1, false, false, hMuonPt);
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // TestReco_cxx
