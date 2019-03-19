#ifndef JetSelectionTests_cxx
#define JetSelectionTests_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 4

#include <iomanip>

#include "JetSelectionTests.h"
#include "../utilities/constants.h"

void JetSelectionTests::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Book histograms here
  std::stringstream oss;
  std::string name,title;
  
  std::map<short int, TH1D*> mMuonPt;
  std::map<short int, TH1D*> mMuonPtNotJet;
  std::map<short int, TH1D*> mMuonPtWithJet;

  std::map<short int, TH1D*> mMuonEta;
  std::map<short int, TH1D*> mMuonEtaNotJet;
  std::map<short int, TH1D*> mMuonEtaWithJet; 
  
  std::map<short int, TH1D*> mDeltaR;
  std::map<short int, TH1D*> mDeltaRNotJet;
  std::map<short int, TH1D*> mDeltaRWithJet;
  
  std::map<short int, TH1D*> mAllJetPt;
  std::map<short int, TH1D*> mAllJetPtNotJet;
  std::map<short int, TH1D*> mAllJetPtWithJet;
  
  std::map<short int, TH1D*> mAllJetEta;
  std::map<short int, TH1D*> mAllJetEtaNotJet;
  std::map<short int, TH1D*> mAllJetEtaWithJet;

  std::map<short int, TH2D*> mDeltaRVsPt;
  std::map<short int, TH2D*> mDeltaRVsPtNotJet;
  std::map<short int, TH2D*> mDeltaRVsPtWithJet;
  
  std::map<short int, TH2D*> mDeltaRVsJetPt;
  std::map<short int, TH2D*> mDeltaRVsJetPtNotJet;
  std::map<short int, TH2D*> mDeltaRVsJetPtWithJet;
  
  for(short int i = 0; i < 5; i++)
  {
    for(short int j = 0; j < 5; j++)
    {
      short int k = 10*i + j;
      oss.str("");    oss << "hMuonPt_" << k;    name = oss.str();
      oss.str("");    oss << "Muon pt for all selected events of category " << k;    title = oss.str();
      TH1D* histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 30., "p_{T} [GeV]", "N. Muons");
      mMuonPt[k] = histo;    
      oss.str("");    oss << "hMuonPtNotJet_" << k;    name = oss.str();
      oss.str("");    oss << "Muon pt for events not passing jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 30., "p_{T} [GeV]", "N. Muons");
      mMuonPtNotJet[k] = histo;
      oss.str("");    oss << "hMuonPtWithJet_" << k;    name = oss.str();
      oss.str("");    oss << "Muon pt for events passing also jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 30., "p_{T} [GeV]", "N. Muons");
      mMuonPtWithJet[k] = histo;
      
      oss.str("");    oss << "hMuonEta_" << k;    name = oss.str();
      oss.str("");    oss << "Muon eta for all selected events of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -3., 3., "#eta", "N. Muons");
      mMuonEta[k] = histo;    
      oss.str("");    oss << "hMuonEtaNotJet_" << k;    name = oss.str();
      oss.str("");    oss << "Muon eta for events not passing jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -3., 3., "#eta", "N. Muons");
      mMuonEtaNotJet[k] = histo;
      oss.str("");    oss << "hMuonEtaWithJet_" << k;    name = oss.str();
      oss.str("");    oss << "Muon eta for events passing also jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -3., 3., "#eta", "N. Muons");
      mMuonEtaWithJet[k] = histo;
      
      oss.str("");    oss << "hDeltaR_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) between muon and jet for all selected events of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 5., "#Delta(R)", "N. Muons");
      mDeltaR[k] = histo;    
      oss.str("");    oss << "hDeltaRNotJet_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) between muon and jet for events not passing jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 5., "#Delta(R)", "N. Muons");
      mDeltaRNotJet[k] = histo;
      oss.str("");    oss << "hDeltaRWithJet_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) between muon and jet for events passing also jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 5., "#Delta(R)", "N. Muons");
      mDeltaRWithJet[k] = histo;
      
      oss.str("");    oss << "hAllJetPt_" << k;    name = oss.str();
      oss.str("");    oss << "Jet pt for all selected events of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 100., "p_{T} [GeV]", "N. Jets");
      mAllJetPt[k] = histo;
      oss.str("");    oss << "hAllJetPtNotJet_" << k;    name = oss.str();
      oss.str("");    oss << "Jet pt for events not passing jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 100., "p_{T} [GeV]", "N. Jets");
      mAllJetPtNotJet[k] = histo;
      oss.str("");    oss << "hAllJetPtWithJet_" << k;    name = oss.str();
      oss.str("");    oss << "Jet pt for events passing also jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 100., "p_{T} [GeV]", "N. Jets");
      mAllJetPtWithJet[k] = histo;
      
      oss.str("");    oss << "hAllJetEta_" << k;    name = oss.str();
      oss.str("");    oss << "Jet #eta for all selected events of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -3., 3., "#eta", "N. Jets");
      mAllJetEta[k] = histo;
      oss.str("");    oss << "hAllJetEtaNotJet_" << k;    name = oss.str();
      oss.str("");    oss << "Jet #eta for events not passing jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -3., 3., "#eta", "N. Jets");
      mAllJetEtaNotJet[k] = histo;
      oss.str("");    oss << "hAllJetEtaWithJet_" << k;    name = oss.str();
      oss.str("");    oss << "Jet #eta for events passing also jet selection of category " << k;    title = oss.str();
      histo = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, -3., 3., "#eta", "N. Jets");
      mAllJetEtaWithJet[k] = histo;
      
      oss.str("");    oss << "hDeltaRVsPt_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) vs. muon pt for all selected events of category " << k;    title = oss.str();
      TH2D* histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 100, 0., 30., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
      mDeltaRVsPt[k] = histo2D;
      oss.str("");    oss << "hDeltaRVsPtNotJet_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) vs. muon pt for events not passing jet selection of category " << k;    title = oss.str();
      histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 100, 0., 30., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
      mDeltaRVsPtNotJet[k] = histo2D;
      oss.str("");    oss << "hDeltaRVsPtWithJet_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) vs. muon pt for events passing also jet selection of category " << k;    title = oss.str();
      histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 100, 0., 30., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
      mDeltaRVsPtWithJet[k] = histo2D;

      oss.str("");    oss << "hDeltaRVsJetPt_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) vs. jet pt for all selected events of category " << k;    title = oss.str();
      histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 100, 0., 100., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
      mDeltaRVsJetPt[k] = histo2D;
      oss.str("");    oss << "hDeltaRVsJetPtNotJet_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) vs. jet pt for events not passing jet selection of category " << k;    title = oss.str();
      histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 100, 0., 100., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
      mDeltaRVsJetPtNotJet[k] = histo2D;
      oss.str("");    oss << "hDeltaRVsJetPtWithJet_" << k;    name = oss.str();
      oss.str("");    oss << "#Delta(R) vs. jet pt for events passing also jet selection of category " << k;    title = oss.str();
      histo2D = Create2DHistogram<TH2D>(name.c_str(),title.c_str(), 100, 0., 100., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
      mDeltaRVsJetPtWithJet[k] = histo2D;
    }
  }
  
  TH1D* hMuonPt = Create1DHistogram<TH1D>("hMuonPt", "Muon pt for all selected events", 100, 0., 30., "p_{T} [GeV]", "N. Muons");
  TH1D* hMuonPtNotJet = Create1DHistogram<TH1D>("hMuonPtNotJet", "Muon pt for events not passing jet selection", 100, 0., 30., "p_{T} [GeV]", "N. Muons");
  TH1D* hMuonPtWithJet = Create1DHistogram<TH1D>("hMuonPtWithJet", "Muon pt for events passing also jet selection", 100, 0., 30., "p_{T} [GeV]", "N. Muons");
  
  TH1D* hMuonEta = Create1DHistogram<TH1D>("hMuonEta", "Muon eta for all selected events", 100, -3., 3., "#eta", "N. Muons");
  TH1D* hMuonEtaNotJet = Create1DHistogram<TH1D>("hMuonEtaNotJet", "Muon eta for events not passing jet selection", 100, -3., 3., "#eta", "N. Muons");
  TH1D* hMuonEtaWithJet = Create1DHistogram<TH1D>("hMuonEtaWithJet", "Muon eta for events passing also jet selection", 100, -3., 3., "#eta", "N. Muons");
  
  TH1D* hDeltaR = Create1DHistogram<TH1D>("hDeltaR", "#Delta(R) between muon and jet for all selected events", 100, 0., 5., "#Delta(R)", "N. Muons");
  TH1D* hDeltaRNotJet = Create1DHistogram<TH1D>("hDeltaRNotJet", "#Delta(R) between muon and jet for events not passing jet selection", 100, 0., 5., "#Delta(R)", "N. Muons");
  TH1D* hDeltaRWithJet = Create1DHistogram<TH1D>("hDeltaRWithJet", "#Delta(R) between muon and jet for events passing also jet selection", 100, 0., 5., "#Delta(R)", "N. Muons");

  TH1D* hAllJetPt = Create1DHistogram<TH1D>("hAllJetPt", "Jet pt for all selected events", 100, 0., 100., "p_{T} [GeV]", "N. Jets");
  TH1D* hAllJetPtNotJet = Create1DHistogram<TH1D>("hAllJetPtNotJet", "Jet pt for events not passing jet selection", 100, 0., 100., "p_{T} [GeV]", "N. Jets");
  TH1D* hAllJetPtWithJet = Create1DHistogram<TH1D>("hAllJetPtWithJet", "Jet pt for events passing also jet selection", 100, 0., 100., "p_{T} [GeV]", "N. Jets");
  
  TH1D* hAllJetEta = Create1DHistogram<TH1D>("hAllJetEta", "Jet #eta for all selected events", 100, -3., 3., "#eta", "N. Jets");
  TH1D* hAllJetEtaNotJet = Create1DHistogram<TH1D>("hAllJetEtaNotJet", "Jet #eta for events not passing jet selection", 100, -3., 3., "#eta", "N. Jets");
  TH1D* hAllJetEtaWithJet = Create1DHistogram<TH1D>("hAllJetEtaWithJet", "Jet #eta for events passing also jet selection", 100, -3., 3., "#eta", "N. Jets");
  
  TH2D* hDeltaRVsPt = Create2DHistogram<TH2D>("hDeltaRVsPt", "#Delta(R) vs. muon pt for all selected events", 100, 0., 30., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
  TH2D* hDeltaRVsPtNotJet = Create2DHistogram<TH2D>("hDeltaRVsPtNotJet", "#Delta(R) vs. muon pt for events not passing jet selection", 100, 0., 30., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
  TH2D* hDeltaRVsPtWithJet = Create2DHistogram<TH2D>("hDeltaRVsPtWithJet", "#Delta(R) vs. muon pt for events passing also jet selection", 100, 0., 30., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
  
  TH2D* hDeltaRVsJetPt = Create2DHistogram<TH2D>("hDeltaRVsJetPt", "#Delta(R) vs. jet pt for all selected events", 100, 0., 100., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
  TH2D* hDeltaRVsJetPtNotJet = Create2DHistogram<TH2D>("hDeltaRVsJetPtNotJet", "#Delta(R) vs. jet pt for events not passing jet selection", 100, 0., 100., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
  TH2D* hDeltaRVsJetPtWithJet = Create2DHistogram<TH2D>("hDeltaRVsJetPtWithJet", "#Delta(R) vs. jet pt for events passing also jet selection", 100, 0., 100., 100, 0., 5., "p_{T} [GeV]", "#Delta(R)");
  
  std::map<unsigned int, unsigned int> nEventsPassingOnlyMu;
  std::map<unsigned int, unsigned int> nEventsWithJetNearLeadingMuon;
  std::map<unsigned int, unsigned int> nEventsWithJet1PtPass;
  std::map<unsigned int, unsigned int> nEventsWithJet1EtaPass;
  std::map<unsigned int, unsigned int> nEventsWithJet1QualityPass;
  std::map<unsigned int, unsigned int> nEventsWithJet1DeltaPtPass;
  std::map<unsigned int, unsigned int> nEventsWithJetNearSecondMuon;
  std::map<unsigned int, unsigned int> nEventsWithJet2PtPass;
  std::map<unsigned int, unsigned int> nEventsWithJet2EtaPass;
  std::map<unsigned int, unsigned int> nEventsWithJet2QualityPass;
  std::map<unsigned int, unsigned int> nEventsWithJet2DeltaPtPass;
  std::map<unsigned int, unsigned int> nEventsWithDifferentJetPass;
  
  std::map<unsigned int, unsigned int> nEventsPassingMuWithJet;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      nEventsPassingOnlyMu[index] = 0;
      nEventsWithJetNearLeadingMuon[index] = 0;
      nEventsWithJet1PtPass[index] = 0;
      nEventsWithJet1EtaPass[index] = 0;
      nEventsWithJet1QualityPass[index] = 0;
      nEventsWithJet1DeltaPtPass[index] = 0;
      nEventsWithJetNearSecondMuon[index] = 0;
      nEventsWithJet2PtPass[index] = 0;
      nEventsWithJet2EtaPass[index] = 0;
      nEventsWithJet2QualityPass[index] = 0;
      nEventsWithJet2DeltaPtPass[index] = 0;
      nEventsWithDifferentJetPass[index] = 0;
      nEventsPassingMuWithJet[index] = 0;
    }
  }
  
  double jetPtThreshold = s->GetJetPtThreshold();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  {
//     std::cout << "Sono qui 1\n";
    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    // Codes
    std::vector<short int> muonClass;
    std::vector<unsigned short> muonInAssocGenp;
    std::vector<short int> muonMCProvenance;
    std::vector<short int> muonMergedCode;
    std::vector<short int> muonSimplifiedIPCode;
    std::vector<short int> muonSimplifiedPtRelCode;
    std::vector<short int> muonSimplifiedMergedIPCode;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV0;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV1;
    std::vector<short int> muonSimplifiedMergedPtRelCodeV2;
//     std::cout << "Sono qui 2\n";
    
    std::vector<unsigned short> vMuonIndexesOnlyMuSel;
    unsigned short iVertexOnlyMuSel;
    bool eventPassesOnlyMu;
    eventPassesOnlyMu = s->SelectEventWithDiMuonV0(s->GetMuonSelector(), vMuonIndexesOnlyMuSel, iVertexOnlyMuSel);
    if(!eventPassesOnlyMu)
      continue;
//     std::cout << "Sono qui 3\n";
    
    std::vector<unsigned short> vMuonIndexesMuWithJetSel;
    std::vector<unsigned short> vJetIndexesMuWithJetSel;
    unsigned short iVertexMuWithJetSel;
    bool eventPassesMuWithJet;
    eventPassesMuWithJet = s->SelectEventWithDiMuonInJetsV0(vMuonIndexesMuWithJetSel, iVertexMuWithJetSel, vJetIndexesMuWithJetSel);
//     std::cout << "Sono qui 4\n";
    
    std::vector<unsigned short> vJetIndexesAllSel;
    for(unsigned int iJet = 0; iJet < Jet_pt->size(); iJet++)
    {
      if(s->SelectJet(iJet, s->GetJetSelector()))
      {
        vJetIndexesAllSel.push_back(iJet);
      }
    }
//     std::cout << "Sono qui 5\n";
    // Loop on selected muons
    // First time to fill codes
    for(unsigned int i = 0; i < vMuonIndexesOnlyMuSel.size(); i++)
    {
      unsigned short iMuon = vMuonIndexesOnlyMuSel[i];
      
      muonClass.push_back(FindMuonClassFromMCAssociation(iMuon));
      muonInAssocGenp.push_back(muonInTrkGenp);
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
//     std::cout << "Sono qui 6\n";
    
    unsigned int eventSimplifiedMergedPtRelCodeV2Ordered = muonSimplifiedMergedPtRelCodeV2[0]*10 + muonSimplifiedMergedPtRelCodeV2[1];
//     std::cout << "Sono qui 7\n";
    
    // Second time to fill histograms
    for(unsigned int i = 0; i < vMuonIndexesOnlyMuSel.size(); i++)
    {
//       std::cout << "Sono qui 7.1\n";
      unsigned short iMuon = vMuonIndexesOnlyMuSel[i];
      TLorentzVector pMuon;
      pMuon.SetPtEtaPhiM(Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon), constants::muonMass);
      //Find the nearest jet, even if the event is not selected by mu+jet selector
      double minDeltaRMuJet = 9999;
      unsigned short iNearestJet = 9999;
//       std::cout << "Sono qui 7.2\n";
      for(unsigned int j = 0; j < vJetIndexesAllSel.size(); j++)
      {
        unsigned short iJet = vJetIndexesAllSel[j];
        TLorentzVector pJet;
        pJet.SetPtEtaPhiM(Jet_pt->at(iJet), Jet_eta->at(iJet), Jet_phi->at(iJet), Jet_mass->at(iJet));
        double deltaR = pMuon.DeltaR(pJet);
        if(deltaR < minDeltaRMuJet)
        {
          minDeltaRMuJet = deltaR;
          iNearestJet = iJet;
        }
      }
//       std::cout << "Sono qui 7.3\n";
      if(eventPassesOnlyMu)
      {
//         std::cout << "Sono qui 7.3.1\n";
        mMuonPt[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_pt->at(iMuon));
        mMuonEta[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_eta->at(iMuon));
        mDeltaR[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(minDeltaRMuJet);
        mDeltaRVsPt[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_pt->at(iMuon), minDeltaRMuJet);
        hMuonPt->Fill(Muon_pt->at(iMuon));
        hMuonEta->Fill(Muon_eta->at(iMuon));
        hDeltaR->Fill(minDeltaRMuJet);
        hDeltaRVsPt->Fill(Muon_pt->at(iMuon), minDeltaRMuJet);
//         std::cout << "Sono qui 7.3.2\n";
        if(iNearestJet != 9999)
        {
          mDeltaRVsJetPt[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_pt->at(iNearestJet), minDeltaRMuJet);
          hDeltaRVsJetPt->Fill(Jet_pt->at(iNearestJet), minDeltaRMuJet);
        }
//         std::cout << "Sono qui 7.3.3\n";
        for(unsigned int iJet = 0; iJet < vJetIndexesAllSel.size(); iJet++)
        {
          mAllJetPt[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_pt->at(vJetIndexesAllSel[iJet]));
          hAllJetPt->Fill(Jet_pt->at(vJetIndexesAllSel[iJet]));
          mAllJetEta[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_eta->at(vJetIndexesAllSel[iJet]));
          hAllJetEta->Fill(Jet_eta->at(vJetIndexesAllSel[iJet]));
        }
//         std::cout << "Sono qui 7.3.4\n";
        if(!eventPassesMuWithJet)
        {     
//           std::cout << "Sono qui 7.3.4.1\n";
          mMuonPtNotJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_pt->at(iMuon));
          mMuonEtaNotJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_eta->at(iMuon));
          mDeltaRNotJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(minDeltaRMuJet);
          mDeltaRVsPtNotJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_pt->at(iMuon), minDeltaRMuJet);
          hMuonPtNotJet->Fill(Muon_pt->at(iMuon));
          hMuonEtaNotJet->Fill(Muon_eta->at(iMuon));
          hDeltaRNotJet->Fill(minDeltaRMuJet);
          hDeltaRVsPtNotJet->Fill(Muon_pt->at(iMuon), minDeltaRMuJet);
//           std::cout << "Sono qui 7.3.4.2\n";
          if(iNearestJet != 9999)
          {
            mDeltaRVsJetPtNotJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_pt->at(iNearestJet), minDeltaRMuJet);
            hDeltaRVsJetPtNotJet->Fill(Jet_pt->at(iNearestJet), minDeltaRMuJet);
          }
//           std::cout << "Sono qui 7.3.4.3\n";
          for(unsigned int iJet = 0; iJet < vJetIndexesAllSel.size(); iJet++)
          {
            mAllJetPtNotJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_pt->at(vJetIndexesAllSel[iJet]));
            hAllJetPtNotJet->Fill(Jet_pt->at(vJetIndexesAllSel[iJet]));
            mAllJetEtaNotJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_eta->at(vJetIndexesAllSel[iJet]));
            hAllJetEtaNotJet->Fill(Jet_eta->at(vJetIndexesAllSel[iJet]));
          }
//           std::cout << "Sono qui 7.3.4.4\n";
        }
        else
        {
//           std::cout << "Sono qui 7.3.4.5\n";
          mMuonPtWithJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_pt->at(iMuon));
          mMuonEtaWithJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_eta->at(iMuon));
          mDeltaRWithJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(minDeltaRMuJet);
          mDeltaRVsPtWithJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Muon_pt->at(iMuon), minDeltaRMuJet);
          hMuonPtWithJet->Fill(Muon_pt->at(iMuon));
          hMuonEtaWithJet->Fill(Muon_eta->at(iMuon));
          hDeltaRWithJet->Fill(minDeltaRMuJet);
          hDeltaRVsPtWithJet->Fill(Muon_pt->at(iMuon), minDeltaRMuJet);
//           std::cout << "Sono qui 7.3.4.6\n";
          if(iNearestJet != 9999)
          {
            mDeltaRVsJetPtWithJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_pt->at(iNearestJet), minDeltaRMuJet);
            hDeltaRVsJetPtWithJet->Fill(Jet_pt->at(iNearestJet), minDeltaRMuJet);
          }
//           std::cout << "Sono qui 7.3.4.7\n";
          for(unsigned int iJet = 0; iJet < vJetIndexesAllSel.size(); iJet++)
          {
            mAllJetPtWithJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_pt->at(vJetIndexesAllSel[iJet]));
            hAllJetPtWithJet->Fill(Jet_pt->at(vJetIndexesAllSel[iJet]));
            mAllJetEtaWithJet[eventSimplifiedMergedPtRelCodeV2Ordered]->Fill(Jet_eta->at(vJetIndexesAllSel[iJet]));
            hAllJetEtaWithJet->Fill(Jet_eta->at(vJetIndexesAllSel[iJet]));
          }
//           std::cout << "Sono qui 7.3.4.8\n";
        }
//         std::cout << "Sono qui 7.3.5\n";
      }
//       std::cout << "Sono qui 7.4\n";
    }
//     std::cout << "Sono qui 8\n";
    
//     std::cout << " eventSimplifiedMergedPtRelCodeV2Ordered = " << eventSimplifiedMergedPtRelCodeV2Ordered << std::endl;
    // Check the cut flow
    bool cutFlow = true;
    if(eventPassesOnlyMu)
    {
      nEventsPassingOnlyMu[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 9\n";
    
    short iJetNearLeadingMuon = -1;
    iJetNearLeadingMuon = Track_jetRef->at(Muon_trackref->at(vMuonIndexesOnlyMuSel[0])) ;
    if(cutFlow && iJetNearLeadingMuon >-1)
    {
      nEventsWithJetNearLeadingMuon[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 10\n";
    
    if(cutFlow && Jet_pt->at(iJetNearLeadingMuon) > jetPtThreshold)
    {
      nEventsWithJet1PtPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 11\n";
    
    if(cutFlow && fabs(Jet_eta->at(iJetNearLeadingMuon))<2.6)
    {
      nEventsWithJet1EtaPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 12\n";
    
    if(cutFlow && s->SelectJet(iJetNearLeadingMuon))
    {
      nEventsWithJet1QualityPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 13\n";
    
    double jet1DPtOverPt = 0;
    if(cutFlow)
    {
      jet1DPtOverPt = (Jet_pt->at(iJetNearLeadingMuon) - Muon_pt->at(vMuonIndexesOnlyMuSel[0])) / Jet_pt->at(iJetNearLeadingMuon);
      if(jet1DPtOverPt >= 0.3)
      {
        nEventsWithJet1DeltaPtPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
      }
      else
      {
        cutFlow = false;
      }
    }
//     std::cout << "Sono qui 14\n";
    
    short iJetNearSecondMuon = -1;
    iJetNearSecondMuon = Track_jetRef->at(Muon_trackref->at(vMuonIndexesOnlyMuSel[1])) ;
    if(cutFlow && iJetNearSecondMuon > -1)
    {
      nEventsWithJetNearSecondMuon[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 15\n";
    
    if(cutFlow && Jet_pt->at(iJetNearSecondMuon) > jetPtThreshold)
    {
      nEventsWithJet2PtPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 16\n";
    
    if(cutFlow && fabs(Jet_eta->at(iJetNearSecondMuon))<2.6)
    {
      nEventsWithJet2EtaPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 17\n";
    
    if(cutFlow && s->SelectJet(iJetNearSecondMuon))
    {
      nEventsWithJet2QualityPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
    else
    {
      cutFlow = false;
    }
//     std::cout << "Sono qui 18\n";
    
    double jet2DPtOverPt = 0;
    if(cutFlow)
    {
      jet2DPtOverPt = (Jet_pt->at(iJetNearSecondMuon) - Muon_pt->at(vMuonIndexesOnlyMuSel[1])) / Jet_pt->at(iJetNearSecondMuon);
      if(jet2DPtOverPt >= 0.3)
      {
        nEventsWithJet2DeltaPtPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
      }
      else
      {
        cutFlow = false;
      }
    }
//     std::cout << "Sono qui 19\n";
    
    if(cutFlow && iJetNearLeadingMuon != iJetNearSecondMuon)
    {
      nEventsWithDifferentJetPass[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
//     std::cout << "Sono qui 20\n";
    
    if(eventPassesMuWithJet)
    {
      nEventsPassingMuWithJet[eventSimplifiedMergedPtRelCodeV2Ordered]++;
    }
//     std::cout << "Sono qui 21\n";
  }

  std::cout << "Event counts\n";
  std::cout << "nEventsPassingOnlyMu" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  std::cout << "nEventsWithJetNearLeadingMuon" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJetNearLeadingMuon[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJetNearLeadingMuon[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJetNearLeadingMuon[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }

  std::cout << "nEventsWithJet1PtPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJet1PtPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet1PtPass[index]/nEventsWithJetNearLeadingMuon[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet1PtPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithJet1EtaPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJet1EtaPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet1EtaPass[index]/nEventsWithJet1PtPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet1EtaPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithJet1QualityPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJet1QualityPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet1QualityPass[index]/nEventsWithJet1EtaPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet1QualityPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithJet1DeltaPtPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJet1DeltaPtPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet1DeltaPtPass[index]/nEventsWithJet1QualityPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet1DeltaPtPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithJetNearSecondMuon" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJetNearSecondMuon[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJetNearSecondMuon[index]/nEventsWithJet1DeltaPtPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJetNearSecondMuon[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithJet2PtPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJet2PtPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet2PtPass[index]/nEventsWithJetNearSecondMuon[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet2PtPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithJet2EtaPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJet2EtaPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet2EtaPass[index]/nEventsWithJet2PtPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet2EtaPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithJet2QualityPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJet2QualityPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet2QualityPass[index]/nEventsWithJet2EtaPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet2QualityPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithJet2DeltaPtPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithJet2DeltaPtPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet2DeltaPtPass[index]/nEventsWithJet2QualityPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithJet2DeltaPtPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsWithDifferentJetPass" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsWithDifferentJetPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithDifferentJetPass[index]/nEventsWithJet2DeltaPtPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsWithDifferentJetPass[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  std::cout << "nEventsPassingMuWithJet" << std::endl;
  for(unsigned int i = 0; i < 5; i++)
  {
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << nEventsPassingMuWithJet[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsPassingMuWithJet[index]/nEventsWithDifferentJetPass[index] << "\t";
    }
    for(unsigned int j = 0; j < 5; j++)
    {
      unsigned int index = i + 10*j;
      std::cout << std::setiosflags(ios::fixed) << std::setprecision(3) << (double)nEventsPassingMuWithJet[index]/nEventsPassingOnlyMu[index] << "\t";
    }
    std::cout << "\n";
  }
  
  // Keep this line here!
  outFile->cd();

  // Create and write canvases here
  const unsigned int lineStyles[2] = {0, 0};
  const unsigned int markerStyles[2] = {21, 20};
  const unsigned int colors[2] = {1,2};
  std::string legendTitle = "";
  std::vector<std::string> legendLabels1;
  legendLabels1.push_back("mu");
  legendLabels1.push_back("mu and !jet");
  LegendDefinition leg1 = {legendTitle,legendLabels1, "NE"};
  std::vector<std::string> legendLabels2;
  legendLabels2.push_back("mu");
  legendLabels2.push_back("mu and jet");
  LegendDefinition leg2 = {legendTitle,legendLabels2, "NE"};
  std::vector<std::string> legendLabels3;
  legendLabels3.push_back("mu and jet");
  legendLabels3.push_back("mu and !jet");
  LegendDefinition leg3 = {legendTitle,legendLabels3, "NE"};
  
  for(short int i = 0; i < 5; i++)
  {
    for(short int j = 0; j < 5; j++)
    {
      short int k = 10*i + j;
      oss.str("");    oss << "cMuonPt_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mMuonPt[k]);
      oss.str("");    oss << "cMuonPtNotJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mMuonPtNotJet[k]);
      oss.str("");    oss << "cMuonPtWithJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mMuonPtWithJet[k]);
      oss.str("");    oss << "cMuonEta_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, false, mMuonEta[k]);
      oss.str("");    oss << "cMuonEtaNotJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, false, mMuonEtaNotJet[k]);
      oss.str("");    oss << "cMuonEtaWithJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, false, mMuonEtaWithJet[k]);
      oss.str("");    oss << "cDeltaR_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mDeltaR[k]);
      oss.str("");    oss << "cDeltaRNotJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mDeltaRNotJet[k]);
      oss.str("");    oss << "cDeltaRWithJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mDeltaRWithJet[k]);      
      oss.str("");    oss << "cAllJetPt_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mAllJetPt[k]);
      oss.str("");    oss << "cAllJetPtNotJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mAllJetPtNotJet[k]);
      oss.str("");    oss << "cAllJetPtWithJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mAllJetPtWithJet[k]);
      oss.str("");    oss << "cAllJetEta_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mAllJetEta[k]);
      oss.str("");    oss << "cAllJetEtaNotJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mAllJetEtaNotJet[k]);
      oss.str("");    oss << "cAllJetEtaWithJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), 0, 21, 1, false, true, mAllJetEtaWithJet[k]);
      
      oss.str("");    oss << "cDeltaRVsPt_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), "colz", false, false, false, mDeltaRVsPt[k]);
      oss.str("");    oss << "cDeltaRVsPtNotJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), "colz", false, false, false, mDeltaRVsPtNotJet[k]);
      oss.str("");    oss << "cDeltaRVsPtWithJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), "colz", false, false, false, mDeltaRVsPtWithJet[k]);
      oss.str("");    oss << "cDeltaRVsJetPt_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), "colz", false, false, false, mDeltaRVsJetPt[k]);
      oss.str("");    oss << "cDeltaRVsJetPtNotJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), "colz", false, false, false, mDeltaRVsJetPtNotJet[k]);
      oss.str("");    oss << "cDeltaRVsJetPtWithJet_" << k;    name = oss.str();
      CreateAndWriteCanvas(name.c_str(), "colz", false, false, false, mDeltaRVsJetPtWithJet[k]);
      
      oss.str("");    oss << "cMuonPtRatio1_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg1, false, true, mMuonPt[k], mMuonPtNotJet[k], NULL);
      oss.str("");    oss << "cMuonPtRatio2_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg2, false, true, mMuonPt[k], mMuonPtWithJet[k], NULL);
      oss.str("");    oss << "cMuonPtRatio3_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg3, false, true, mMuonPtWithJet[k], mMuonPtNotJet[k], NULL);
      oss.str("");    oss << "cMuonEtaRatio1_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg1, false, false, mMuonEta[k], mMuonEtaNotJet[k], NULL);
      oss.str("");    oss << "cMuonEtaRatio2_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg2, false, false, mMuonEta[k], mMuonEtaWithJet[k], NULL);
      oss.str("");    oss << "cMuonEtaRatio3_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg3, false, false, mMuonEtaWithJet[k], mMuonEtaNotJet[k], NULL);
      oss.str("");    oss << "cDeltaRRatio1_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg1, false, true, mDeltaR[k], mDeltaRNotJet[k], NULL);
      oss.str("");    oss << "cDeltaRRatio2_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg2, false, true, mDeltaR[k], mDeltaRWithJet[k], NULL);
      oss.str("");    oss << "cDeltaRRatio3_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg3, false, true, mDeltaRWithJet[k], mDeltaRNotJet[k], NULL);
      oss.str("");    oss << "cAllJetPtRatio1_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg1, false, true, mAllJetPt[k], mAllJetPtNotJet[k], NULL);
      oss.str("");    oss << "cAllJetPtRatio2_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg2, false, true, mAllJetPt[k], mAllJetPtWithJet[k], NULL);
      oss.str("");    oss << "cAllJetPtRatio3_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg3, false, true, mAllJetPtWithJet[k], mAllJetPtNotJet[k], NULL);
      oss.str("");    oss << "cAllJetEtaRatio1_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg1, false, true, mAllJetEta[k], mAllJetEtaNotJet[k], NULL);
      oss.str("");    oss << "cAllJetEtaRatio2_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg2, false, true, mAllJetEta[k], mAllJetEtaWithJet[k], NULL);
      oss.str("");    oss << "cAllJetEtaRatio3_" << k;    name = oss.str();
      CreateAndWriteCanvasWithRatios(name.c_str(), lineStyles, markerStyles, colors, leg3, false, true, mAllJetEtaWithJet[k], mAllJetEtaNotJet[k], NULL);
    }
  }
  
  CreateAndWriteCanvas("cMuonPt", 0, 21, 1, false, true, hMuonPt);
  CreateAndWriteCanvas("cMuonPtNotJet", 0, 21, 1, false, true, hMuonPtNotJet);
  CreateAndWriteCanvas("cMuonPtWithJet", 0, 21, 1, false, true, hMuonPtWithJet);
  CreateAndWriteCanvas("cMuonEta", 0, 21, 1, false, false, hMuonEta);
  CreateAndWriteCanvas("cMuonEtaNotJet", 0, 21, 1, false, false, hMuonEtaNotJet);
  CreateAndWriteCanvas("cMuonEtaWithJet", 0, 21, 1, false, false, hMuonEtaWithJet);
  CreateAndWriteCanvas("cDeltaR", 0, 21, 1, false, true, hDeltaR);
  CreateAndWriteCanvas("cDeltaRNotJet", 0, 21, 1, false, true, hDeltaRNotJet);
  CreateAndWriteCanvas("cDeltaRWithJet", 0, 21, 1, false, true, hDeltaRWithJet);
  CreateAndWriteCanvas("cAllJetPt", 0, 21, 1, false, true, hAllJetPt);
  CreateAndWriteCanvas("cAllJetPtNotJet", 0, 21, 1, false, true, hAllJetPtNotJet);
  CreateAndWriteCanvas("cAllJetPtWithJet", 0, 21, 1, false, true, hAllJetPtWithJet);
  CreateAndWriteCanvas("cAllJetEta", 0, 21, 1, false, true, hAllJetEta);
  CreateAndWriteCanvas("cAllJetEtaNotJet", 0, 21, 1, false, true, hAllJetEtaNotJet);
  CreateAndWriteCanvas("cAllJetEtaWithJet", 0, 21, 1, false, true, hAllJetEtaWithJet);
  
  CreateAndWriteCanvas("cDeltaRVsPt", "colz", false, false, false, hDeltaRVsPt);
  CreateAndWriteCanvas("cDeltaRVsPtNotJet", "colz", false, false, false, hDeltaRVsPtNotJet);
  CreateAndWriteCanvas("cDeltaRVsPtWithJet", "colz", false, false, false, hDeltaRVsPtWithJet);  
  CreateAndWriteCanvas("cDeltaRVsJetPt", "colz", false, false, false, hDeltaRVsJetPt);
  CreateAndWriteCanvas("cDeltaRVsJetPtNotJet", "colz", false, false, false, hDeltaRVsJetPtNotJet);
  CreateAndWriteCanvas("cDeltaRVsJetPtWithJet", "colz", false, false, false, hDeltaRVsJetPtWithJet);
    
  CreateAndWriteCanvasWithRatios("cMuonPtRatio1", lineStyles, markerStyles, colors, leg1, false, true, hMuonPt, hMuonPtNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cMuonPtRatio2", lineStyles, markerStyles, colors, leg2, false, true, hMuonPt, hMuonPtWithJet, NULL);
  CreateAndWriteCanvasWithRatios("cMuonPtRatio3", lineStyles, markerStyles, colors, leg3, false, true, hMuonPtWithJet, hMuonPtNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cMuonEtaRatio1", lineStyles, markerStyles, colors, leg1, false, false, hMuonEta, hMuonEtaNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cMuonEtaRatio2", lineStyles, markerStyles, colors, leg2, false, false, hMuonEta, hMuonEtaWithJet, NULL);
  CreateAndWriteCanvasWithRatios("cMuonEtaRatio3", lineStyles, markerStyles, colors, leg3, false, false, hMuonEtaWithJet, hMuonEtaNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cDeltaRRatio1", lineStyles, markerStyles, colors, leg1, false, true, hDeltaR, hDeltaRNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cDeltaRRatio2", lineStyles, markerStyles, colors, leg2, false, true, hDeltaR, hDeltaRWithJet, NULL);
  CreateAndWriteCanvasWithRatios("cDeltaRRatio3", lineStyles, markerStyles, colors, leg3, false, true, hDeltaRWithJet, hDeltaRNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cAllJetPtRatio1", lineStyles, markerStyles, colors, leg1, false, true, hAllJetPt, hAllJetPtNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cAllJetPtRatio2", lineStyles, markerStyles, colors, leg2, false, true, hAllJetPt, hAllJetPtWithJet, NULL);
  CreateAndWriteCanvasWithRatios("cAllJetPtRatio3", lineStyles, markerStyles, colors, leg3, false, true, hAllJetPtWithJet, hAllJetPtNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cAllJetEtaRatio1", lineStyles, markerStyles, colors, leg1, false, true, hAllJetEta, hAllJetEtaNotJet, NULL);
  CreateAndWriteCanvasWithRatios("cAllJetEtaRatio2", lineStyles, markerStyles, colors, leg2, false, true, hAllJetEta, hAllJetEtaWithJet, NULL);
  CreateAndWriteCanvasWithRatios("cAllJetEtaRatio3", lineStyles, markerStyles, colors, leg3, false, true, hAllJetEtaWithJet, hAllJetEtaNotJet, NULL);
  
  // Uncomment this line to write also the histograms to the file
  outFile->Write();
}

#endif // JetSelectionTests_cxx
