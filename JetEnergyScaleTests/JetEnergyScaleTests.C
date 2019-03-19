#ifndef JetEnergyScaleTests_cxx
#define JetEnergyScaleTests_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include "JetEnergyScaleTests.h"
#include "../utilities/constants.h"

void JetEnergyScaleTests::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Book histograms here
  std::stringstream oss;
  std::string name,title;
  
  std::map<short int, TH1D*> mMuonPt;
  std::map<short int, TH1D*> mGenMuonPt;
  std::map<short int, TH1D*> mJetPt;
  std::map<short int, TH1D*> mDaughterPt;
  std::map<short int, TH1D*> mMuonPtDifference;
  std::map<short int, TH1D*> mJetPtDifference;
  std::map<short int, TH2D*> mMuonPtDifferenceVsJetPtDifference;
  std::map<short int, TH1D*> mMuonEta;
  std::map<short int, TH1D*> mGenMuonEta;
  std::map<short int, TH1D*> mJetEta;
  std::map<short int, TH1D*> mDaughterEta;
  std::map<short int, TH1D*> mMuonEtaDifference;
  std::map<short int, TH1D*> mJetEtaDifference;
  std::map<short int, TH2D*> mMuonEtaDifferenceVsJetEtaDifference;
  
  std::map<short int, TH2D*> mMuonPtDifferenceVsJetEtaDifference;
  std::map<short int, TH2D*> mMuonEtaDifferenceVsJetPtDifference;
  std::map<short int, TH2D*> mMuonPtDifferenceVsMuonEtaDifference;
  std::map<short int, TH2D*> mJetPtDifferenceVsJetEtaDifference;  
  
  std::map<short int, TH1D*> mMuonPtRel;
  std::map<short int, TH1D*> mGenMuonPtRel;
  
  std::map<short int, TH1D*> mMuonPtRelDifference;
  std::map<short int, TH2D*> mMuonPtRelDifferenceVsJetPtDifference;
  std::map<short int, TH2D*> mMuonPtRelDifferenceVsJetEtaDifference;
    
  for(short int i = 0; i < 5; i++)
  {
    oss.str("");    oss << "hMuonPt_" << i;    name = oss.str();
    oss.str("");    oss << "Muon p_{T} for events of category " << i;    title = oss.str();
    TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 30., "p_{T} [GeV]", "N. Muons");
    mMuonPt[i] = histo;

    oss.str("");    oss << "hGenMuonPt_" << i;    name = oss.str();
    oss.str("");    oss << "Gen muon p_{T} for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 30., "p_{T} [GeV]", "N. Gen muons");
    mGenMuonPt[i] = histo;
    
    oss.str("");    oss << "hJetPt_" << i;    name = oss.str();
    oss.str("");    oss << "Jet p_{T} for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 100., "p_{T} [GeV]", "N. Jets");
    mJetPt[i] = histo;
    
    oss.str("");    oss << "hDaughterPt_" << i;    name = oss.str();
    oss.str("");    oss << "p_{T} of string daughter for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 100., "p_{T} [GeV]", "N. Gen particles");
    mDaughterPt[i] = histo;
    
    oss.str("");    oss << "hMuonPtDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon p_{T} difference for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 200, -1., 1., "p_{T}^{gen} - p_{T}^{reco} [GeV]", "N. Muons");
    mMuonPtDifference[i] = histo;
    
    oss.str("");    oss << "hJetPtDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco jet p_{T} difference for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 200, -100., 100., "p_{T}^{gen} - p_{T}^{reco} [GeV]", "N. Jets");
    mJetPtDifference[i] = histo;

    oss.str("");    oss << "hMuonPtDifferenceVsJetPtDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon p_{T} difference vs. jet p_{T} difference for events of category " << i;    title = oss.str();
    TH2D* histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 200, -100., 100., 200, -1., 1., "jet p_{T}^{gen} - p_{T}^{reco} [GeV]", "muon p_{T}^{gen} - p_{T}^{reco} [GeV]");
    mMuonPtDifferenceVsJetPtDifference[i] = histo2D;

    
    
    oss.str("");    oss << "hMuonEta_" << i;    name = oss.str();
    oss.str("");    oss << "Muon #eta for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -3., 3., "#eta", "N. Muons");
    mMuonEta[i] = histo;
    
    oss.str("");    oss << "hGenMuonEta_" << i;    name = oss.str();
    oss.str("");    oss << "Gen muon #eta for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -3., 3., "#eta", "N. Gen muons");
    mGenMuonEta[i] = histo;

    oss.str("");    oss << "hJetEta_" << i;    name = oss.str();
    oss.str("");    oss << "Jet #eta for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -5., 5., "#eta", "N. Jets");
    mJetEta[i] = histo;
    
    oss.str("");    oss << "hDaughterEta_" << i;    name = oss.str();
    oss.str("");    oss << "#eta of string daughter for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -5., 5., "#eta", "N. Gen particles");
    mDaughterEta[i] = histo;
    
    oss.str("");    oss << "hMuonEtaDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon #eta difference for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 200, -0.01, 0.01, "#eta^{gen} - #eta^{reco} [GeV]", "N. Muons");
    mMuonEtaDifference[i] = histo;
    
    oss.str("");    oss << "hJetEtaDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco jet #eta difference for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 200, -2., 2., "#eta^{gen} - #eta^{reco} [GeV]", "N. Jets");
    mJetEtaDifference[i] = histo;

    oss.str("");    oss << "hMuonEtaDifferenceVsJetEtaDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon #eta difference vs. jet #eta difference for events of category " << i;    title = oss.str();
    histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 200, -2., 2., 200, -0.01, 0.01, "jet #eta^{gen} - #eta^{reco}", "muon #eta^{gen} - #eta^{reco}");
    mMuonEtaDifferenceVsJetEtaDifference[i] = histo2D;



    oss.str("");    oss << "hMuonPtDifferenceVsJetEtaDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon p_{T} difference vs. jet #eta difference for events of category " << i;    title = oss.str();
    histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 200, -2., 2., 200, -1., 1., "jet #eta^{gen} - #eta^{reco}", "muon p_{T}^{gen} - p_{T}^{reco} [GeV]");
    mMuonPtDifferenceVsJetEtaDifference[i] = histo2D;
    
    oss.str("");    oss << "hMuonEtaDifferenceVsJetPtDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon #eta difference vs. jet p_{T} difference for events of category " << i;    title = oss.str();
    histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 200, -100., 100., 200, -0.01, 0.01, "jet p_{T}^{gen} - p_{T}^{reco} [GeV]", "muon #eta^{gen} - #eta^{reco}");
    mMuonEtaDifferenceVsJetPtDifference[i] = histo2D;
    
    oss.str("");    oss << "hMuonPtDifferenceVsMuonEtaDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon p_{T} difference vs. #eta difference for events of category " << i;    title = oss.str();
    histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 200, -0.01, 0.01, 200, -1., 1., "muon #eta^{gen} - #eta^{reco}", "muon p_{T}^{gen} - p_{T}^{reco} [GeV]");
    mMuonPtDifferenceVsMuonEtaDifference[i] = histo2D;
    
    oss.str("");    oss << "hJetPtDifferenceVsJetEtaDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco jet p_{T} difference vs. #eta difference for events of category " << i;    title = oss.str();
    histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 200, -2., 2., 200, -100., 100., "jet #eta^{gen} - #eta^{reco}", "jet p_{T}^{gen} - p_{T}^{reco} [GeV]");
    mJetPtDifferenceVsJetEtaDifference[i] = histo2D;
    

    
    oss.str("");    oss << "hMuonPtRel_" << i;    name = oss.str();
    oss.str("");    oss << "Muon p_{T}^{Rel} for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 5., "p_{T}^{Rel} [GeV]", "N. Muons");
    mMuonPtRel[i] = histo;
    
    oss.str("");    oss << "hGenMuonPtRel_" << i;    name = oss.str();
    oss.str("");    oss << "Gen muon p_{T}^{Rel} for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 5., "p_{T}^{Rel} [GeV]", "N. Gen muons");
    mGenMuonPtRel[i] = histo;

    oss.str("");    oss << "hMuonPtRelDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon p_{T}^{Rel} difference for events of category " << i;    title = oss.str();
    histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 200, -5., 5., "p_{T}^{Rel, gen} - p_{T}^{Rel, reco} [GeV]", "N. Muons");
    mMuonPtRelDifference[i] = histo;    
    
    
    
    oss.str("");    oss << "hMuonPtRelDifferenceVsJetPtDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon p_{T}^{Rel} difference vs. jet p_{T} difference for events of category " << i;    title = oss.str();
    histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 200, -100., 100., 200, -5., 5., "jet p_{T}^{gen} - p_{T}^{reco} [GeV]", "muon p_{T}^{Rel, gen} - p_{T}^{Rel, reco} [GeV]");
    mMuonPtRelDifferenceVsJetPtDifference[i] = histo2D;
    
    oss.str("");    oss << "hMuonPtRelDifferenceVsJetEtaDifference_" << i;    name = oss.str();
    oss.str("");    oss << "Gen - reco muon p_{T}^{Rel} difference vs. jet #eta difference for events of category " << i;    title = oss.str();
    histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 200, -2., 2., 200, -5., 5., "jet #eta^{gen} - #eta^{reco}", "muon p_{T}^{Rel, gen} - p_{T}^{Rel, reco} [GeV]");
    mMuonPtRelDifferenceVsJetEtaDifference[i] = histo2D;
  }

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    std::vector<unsigned short> vMuonIndexes;
    std::vector<unsigned short> vJetIndexes;
    unsigned short iVertex;
    
    if(!s->SelectEventWithDiMuonInJetsV0(vMuonIndexes, iVertex, vJetIndexes))
    {
      continue;
    }
    std::cout << "Sono qui 0\n";
    
    
    // Codes
    std::vector<short int> muonClass;
    std::vector<unsigned short> muonInAssocGenp;
    std::vector<std::pair<unsigned short, unsigned short> > iStringAndDaughter;
    std::vector<short int> muonMCProvenance;
    std::vector<short int> muonMergedCode;
    std::vector<short int> muonSimplifiedIPCode;
    std::vector<short int> muonSimplifiedPtRelCode;
    std::vector<short int> muonSimplifiedMergedIPCode;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV0;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV1;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV2;
    std::cout << "Sono qui 0.1\n";
    
    // Loop on selected muons
    // First time to fill codes
    for(unsigned int i = 0; i < vMuonIndexes.size(); i++)
    {
      unsigned short iMuon = vMuonIndexes[i];
      
      muonClass.push_back(FindMuonClassFromMCAssociation(iMuon));
      muonInAssocGenp.push_back(muonInTrkGenp);
      std::cout <<"muonInTrkGenp = " << muonInTrkGenp << std::endl;
      
      std::pair<unsigned short, unsigned short> iSAndD;
      if(muonInTrkGenp != 65535)
      {
        iSAndD = LookForMotherString(muonInTrkGenp);
      }
      else
      {
        iSAndD = std::make_pair(65535,65535);
      }
      iStringAndDaughter.push_back(iSAndD);
      muonMCProvenance.push_back(FindMuonMCProvenance(iMuon,muonClass[i]));
      muonMergedCode.push_back(MergeMuonClassAndMCProvenance(muonClass[i],muonMCProvenance[i]));
      
      // Get simplified codes
      muonSimplifiedIPCode.push_back(FindSimplifiedMuonCode(muonMergedCode[i]));
      muonSimplifiedMergedIPCode.push_back(FindSimplifiedMergedMuonCode(muonSimplifiedIPCode[i]));
      if (s->JetSelectorIsValid())
      {
        muonSimplifiedPtRelCode.push_back(FindSimplifiedMuonPtRelCode(muonMergedCode[i]));
        muonSimplifiedMergedPtRelCodeV0.push_back(FindSimplifiedMergedMuonPtRelCodeV0(muonSimplifiedPtRelCode[i]));
        muonSimplifiedMergedPtRelCodeV1.push_back(FindSimplifiedMergedMuonPtRelCodeV1(muonSimplifiedPtRelCode[i]));
        muonSimplifiedMergedPtRelCodeV2.push_back(FindSimplifiedMergedMuonPtRelCodeV2(muonSimplifiedPtRelCode[i]));
      }
    }
    std::cout << "Sono qui 0.2\n";
    std::cout << "Size of muonSimplifiedMergedPtRelCodeV2 is "  << std::flush << muonSimplifiedMergedPtRelCodeV2.size() << std::endl;
    unsigned int eventSimplifiedMergedPtRelCodeV2Ordered = muonSimplifiedMergedPtRelCodeV2[0]*10 + muonSimplifiedMergedPtRelCodeV2[1];
    
    // Second time to perform the analysis
    std::cout << "Sono qui 1\n";
    for(unsigned int i = 0; i < vMuonIndexes.size(); i++)
    {
      std::cout << "Sono qui 2\n";
      unsigned short iMuon = vMuonIndexes[i];
      std::cout << "Sono qui 3\n";
      unsigned short iJet = vJetIndexes[i];
      std::cout << "Sono qui 4\n";
      unsigned short iString = iStringAndDaughter[i].first;
      std::cout << "Sono qui 5\n";
      unsigned short iDaughter = iStringAndDaughter[i].second;
      std::cout << "Sono qui 6\n";
      unsigned short iGenMuon = muonInAssocGenp[i];
      std::cout << "Sono qui 7\n";
      short int muonCode = muonSimplifiedMergedPtRelCodeV2[i];
      std::cout << "iMuon = " << iMuon << ", iJet = " << iJet << std::endl;
      std::cout << "iDaughter = " << iDaughter << ", iGenMuon = " << iGenMuon << std::endl;

      TLorentzVector pMuon;
      pMuon.SetPtEtaPhiM(Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon), constants::muonMass);
      
      TLorentzVector pJet;
      pJet.SetPtEtaPhiM(Jet_pt->at(iJet), Jet_eta->at(iJet), Jet_phi->at(iJet), Jet_mass->at(iJet));
      
      double ptMuon = pMuon.Perp();      
      double ptJet = pJet.Perp();
      double etaMuon = pMuon.Eta();      
      double etaJet = pJet.Eta();
      
      TLorentzVector pDaughter;
      pDaughter.SetPtEtaPhiM(99999,99999,constants::PI,1);
      if(iDaughter != 65535)
      {
        pDaughter.SetPtEtaPhiM(Genp_pt->at(iDaughter), Genp_eta->at(iDaughter), Genp_phi->at(iDaughter), Genp_mass->at(iDaughter));
      }
      double ptDaughter = pDaughter.Perp();
      double etaDaughter = pDaughter.Eta();

      TLorentzVector pGenMuon;
      pGenMuon.SetPtEtaPhiM(9999, 9999, -constants::PI, 1);
      if(iGenMuon != 65535)
      {
        pGenMuon.SetPtEtaPhiM(Genp_pt->at(iGenMuon), Genp_eta->at(iGenMuon), Genp_phi->at(iGenMuon), Genp_mass->at(iGenMuon));
      } 
      double ptGenMuon = pGenMuon.Perp();
      double etaGenMuon = pGenMuon.Eta();

      double ptDifferenceMuon = ptGenMuon - ptMuon;
      double ptDifferenceJet = ptDaughter - ptJet;
      double etaDifferenceMuon = etaGenMuon - etaMuon;
      double etaDifferenceJet = etaDaughter - etaJet;

      double ptRelMuon = pMuon.Vect().Perp(pJet.Vect());
      double ptRelGenMuon = pGenMuon.Vect().Perp(pDaughter.Vect());
      
      double ptRelDifferenceMuon = ptRelGenMuon - ptRelMuon; 
      
      std::cout << "Event " << jentry << std::endl;
      std::cout << "Muon: reco pt = " << ptMuon << ", gen pt = " << ptGenMuon << ", difference = " << ptDifferenceMuon << std::endl;
      std::cout << "Jet: reco pt = " << ptJet << ", gen pt = " << ptDaughter << ", difference = " << ptDifferenceJet << std::endl;
      std::cout << "Muon: reco eta = " << etaMuon << ", gen eta = " << etaGenMuon << ", difference = " << etaDifferenceMuon << std::endl;
      std::cout << "Jet: reco eta = " << etaJet << ", gen eta = " << etaDaughter << ", difference = " << etaDifferenceJet << std::endl;
      std::cout << "muonCode = " << muonCode << std::endl;
 
      mMuonPt[muonCode]->Fill(ptMuon);
      mGenMuonPt[muonCode]->Fill(ptGenMuon);
      mJetPt[muonCode]->Fill(ptJet);
      mDaughterPt[muonCode]->Fill(ptDaughter);
      mMuonPtDifference[muonCode]->Fill(ptDifferenceMuon);
      mJetPtDifference[muonCode]->Fill(ptDifferenceJet);
      mMuonPtDifferenceVsJetPtDifference[muonCode]->Fill(ptDifferenceJet,ptDifferenceMuon);
      
      mMuonEta[muonCode]->Fill(etaMuon);
      mGenMuonEta[muonCode]->Fill(etaGenMuon);
      mJetEta[muonCode]->Fill(etaJet);
      mDaughterEta[muonCode]->Fill(etaDaughter);
      mMuonEtaDifference[muonCode]->Fill(etaDifferenceMuon);
      mJetEtaDifference[muonCode]->Fill(etaDifferenceJet);
      mMuonEtaDifferenceVsJetEtaDifference[muonCode]->Fill(etaDifferenceJet,etaDifferenceMuon);
      
      mMuonPtDifferenceVsJetEtaDifference[muonCode]->Fill(etaDifferenceJet,ptDifferenceMuon);
      mMuonEtaDifferenceVsJetPtDifference[muonCode]->Fill(ptDifferenceJet,etaDifferenceMuon);
      mMuonPtDifferenceVsMuonEtaDifference[muonCode]->Fill(etaDifferenceMuon,ptDifferenceMuon);
      mJetPtDifferenceVsJetEtaDifference[muonCode]->Fill(etaDifferenceJet,ptDifferenceJet);
      
      mMuonPtRel[muonCode]->Fill(ptRelMuon);
      mGenMuonPtRel[muonCode]->Fill(ptRelGenMuon);
      
      mMuonPtRelDifference[muonCode]->Fill(ptRelDifferenceMuon);
      
      mMuonPtRelDifferenceVsJetPtDifference[muonCode]->Fill(ptDifferenceJet, ptRelDifferenceMuon);
      mMuonPtRelDifferenceVsJetEtaDifference[muonCode]->Fill(etaDifferenceJet, ptRelDifferenceMuon);
    }
  }

  // Keep this line here!
  outFile->cd();

  // Create and write canvases here
  for(short int i = 0; i < 5; i++)
  {
    oss.str("");    oss << "cMuonPt_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mMuonPt[i]);
    
    oss.str("");    oss << "cGenMuonPt_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mGenMuonPt[i]);

    oss.str("");    oss << "cJetPt_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mJetPt[i]);
    
    oss.str("");    oss << "cDaughterPt_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mDaughterPt[i]);

    oss.str("");    oss << "cMuonPtDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mMuonPtDifference[i]);
    
    oss.str("");    oss << "cJetPtDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mJetPtDifference[i]);
    
    oss.str("");    oss << "cMuonPtDifferenceVsJetPtDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), "colz", false, false, true, mMuonPtDifferenceVsJetPtDifference[i]);

    
    
    oss.str("");    oss << "cMuonEta_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mMuonEta[i]);
    
    oss.str("");    oss << "cGenMuonEta_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mGenMuonEta[i]);
    
    oss.str("");    oss << "cJetEta_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mJetEta[i]);
    
    oss.str("");    oss << "cDaughterEta_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mDaughterEta[i]);
    
    oss.str("");    oss << "cMuonEtaDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mMuonEtaDifference[i]);
    
    oss.str("");    oss << "cJetEtaDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mJetEtaDifference[i]);
    
    oss.str("");    oss << "cMuonEtaDifferenceVsJetEtaDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), "colz", false, false, true, mMuonEtaDifferenceVsJetEtaDifference[i]);



    oss.str("");    oss << "cMuonPtDifferenceVsJetEtaDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), "colz", false, false, true, mMuonPtDifferenceVsJetEtaDifference[i]);
    
    oss.str("");    oss << "cMuonEtaDifferenceVsJetPtDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), "colz", false, false, true, mMuonEtaDifferenceVsJetPtDifference[i]);

    oss.str("");    oss << "cMuonPtDifferenceVsMuonEtaDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), "colz", false, false, true, mMuonPtDifferenceVsMuonEtaDifference[i]);

    oss.str("");    oss << "cJetPtDifferenceVsJetEtaDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), "colz", false, false, true, mJetPtDifferenceVsJetEtaDifference[i]);

    
    
    oss.str("");    oss << "cMuonPtRel_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, false, mMuonPtRel[i]);
    
    oss.str("");    oss << "cGenMuonPtRel_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, false, mGenMuonPtRel[i]);

    oss.str("");    oss << "cMuonPtRelDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, false, mMuonPtRelDifference[i]);
    
    
    
    oss.str("");    oss << "cMuonPtRelDifferenceVsJetPtDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), "colz", false, false, true, mMuonPtRelDifferenceVsJetPtDifference[i]);

    oss.str("");    oss << "cMuonPtRelDifferenceVsJetEtaDifference_" << i;    name = oss.str();
    CreateAndWriteCanvas(name.c_str(), "colz", false, false, true, mMuonPtRelDifferenceVsJetEtaDifference[i]);
  }

  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // JetEnergyScaleTests_cxx
