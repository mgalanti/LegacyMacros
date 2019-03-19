#ifndef MuonIPTemplatesProducerFromData_cxx
#define MuonIPTemplatesProducerFromData_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 3

#include <sstream>

#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "MuonIPTemplatesProducerFromData.h"

#include "../utilities/constants.h"

void MuonIPTemplatesProducerFromData::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Setup the random number engine for template smearing
  TRandom3* rndm = new TRandom3();
  rndm->SetSeed(12345);

  // This is used to set an upper limit to
  // the Muon PtRel range to be used for templates
  const double ptRelMax = 5.;

  // Book histograms here
  // PtRel templates
  TH1D* hMuonDxyzDataDriven;
  TH1D* hMuonDxyDataDriven;
  TH1D* hMuonDzDataDriven;

  std::ostringstream oss;
  std::string name, title;

  if (s->JetSelectorIsValid()) {

    oss.str("");  oss << "hMuonDxyzDataDriven";   name = oss.str();
    oss.str("");  oss << "d_{xyz} template";   title = oss.str();
    TH1D* histoDxyz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xyz} [cm]", "N_{#mu}");
    hMuonDxyzDataDriven = histoDxyz;

    oss.str("");  oss << "hMuonDxyDataDriven";   name = oss.str();
    oss.str("");  oss << "d_{xy} template";   title = oss.str();
    TH1D* histoDxy = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{xy} [cm]", "N_{#mu}");
    hMuonDxyDataDriven = histoDxy;

    oss.str("");  oss << "hMuonDzDataDriven";   name = oss.str();
    oss.str("");  oss << "d_{z} template";   title = oss.str();
    TH1D* histoDz = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 0.5, "d_{z} [cm]", "N_{#mu}");
    hMuonDzDataDriven = histoDz;

  } /// JetSelectorIsValid()

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
    if(!s->MuonSelectorIsValid()) continue;
    
    // Decide if Muon+Jet combined selection or Muon selection only
    if(!s->JetSelectorIsValid()) continue;

    // Select EventWithDiMuon
    if(!s->SelectEventWithDiMuonInJetsV0(vSelectedMuons, iSelectedVertex, vSelectedJets))
    {
      continue;
    }

    // This is mantained anyway
    // Get also trigger selection
    if(!s->SelectEventPassingTrigger("HLT_DoubleMu3_v2"))
    {
      continue;
    }

    // Loop over selected Muons
    for(unsigned short i = 0; i < vSelectedMuons.size(); i++)
    {
      unsigned short iMuon = vSelectedMuons[i];
      unsigned short iJet = vSelectedJets[i];

      // Anti-btag jets
      // Not TCHEL
      if (Jet_TrackCountingHighEffBTag->at(iJet) >= 1.7 ) continue;

      // Get IP information from Ntuples
      hMuonDxyzDataDriven->Fill(Track_dxyz_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));
      hMuonDxyDataDriven->Fill(Track_dxy_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));
      hMuonDzDataDriven->Fill(Track_dz_PV->at(Muon_trackref->at(iMuon)).at(iSelectedVertex));

    }
  }

  // Keep this line here!
  outFile->cd();

  // Uncomment this line to write also the histograms to the file
  outFile->Write();

}

double MuonIPTemplatesProducerFromData::DxyD6TScaling(double dxy)
{
  return 1./(0.958708+0.76623*pow(dxy,1./1.49011));
}

#endif // MuonIPTemplatesProducerFromData_cxx




