#ifndef TreeDefinitionRecoV1_h
#define TreeDefinitionRecoV1_h

#include "TreeDefinitionBase.h"



class TreeDefinitionRecoV1 : public virtual TreeDefinitionBase
{
  public:
    // Declaration of leaf types
    Int_t           evNumber;
    Int_t           runNumber;
    Int_t           lumiSection;
    Double_t        beamSpot_x;
    Double_t        beamSpot_y;
    Double_t        beamSpot_z;
    Int_t           numberOfVertex;
    std::vector<double>  *PV_x;
    std::vector<double>  *PV_y;
    std::vector<double>  *PV_z;
    std::vector<double>  *PV_xError;
    std::vector<double>  *PV_yError;
    std::vector<double>  *PV_zError;
    std::vector<double>  *PV_ndof;
    std::vector<double>  *PV_normalizedChi2;
    std::vector<bool>    *PV_isFake;
    Int_t           numberOfTrack;
    std::vector<double>  *Track_pt;
    std::vector<double>  *Track_ptError;
    std::vector<double>  *Track_eta;
    std::vector<double>  *Track_etaError;
    std::vector<double>  *Track_phi;
    std::vector<double>  *Track_phiError;
    std::vector<double>  *Track_dxy_BS;
    std::vector<std::vector<double> > *Track_dxy_PV;
    std::vector<double>  *Track_dxy;
    std::vector<double>  *Track_dxyError;
    std::vector<double>  *Track_dz;
    std::vector<double>  *Track_dzError;
    std::vector<double>  *Track_d0;
    std::vector<double>  *Track_d0Error;
    std::vector<double>  *Track_normalizedChi2;
    std::vector<double>  *Track_dEdx1;
    std::vector<double>  *Track_chi2;
    std::vector<double>  *Track_ndof;
    std::vector<bool>    *Track_innerOk;
    std::vector<bool>    *Track_isHighPurity;
    std::vector<unsigned short> *Track_algo;
    std::vector<std::vector<double> > *Track_vCovLowerBlock;
    std::vector<unsigned short> *Track_validPixelHits;
    std::vector<unsigned short> *Track_validTIBHits;
    std::vector<unsigned short> *Track_validTIDHits;
    std::vector<unsigned short> *Track_validTOBHits;
    std::vector<unsigned short> *Track_validTECHits;
    std::vector<double>  *Track_referencePoint_x;
    std::vector<double>  *Track_innerPosition_x;
    std::vector<double>  *Track_outerPosition_x;
    std::vector<double>  *Track_referencePoint_y;
    std::vector<double>  *Track_innerPosition_y;
    std::vector<double>  *Track_outerPosition_y;
    std::vector<double>  *Track_referencePoint_z;
    std::vector<double>  *Track_innerPosition_z;
    std::vector<double>  *Track_outerPosition_z;
    std::vector<short>   *Track_charge;
    std::vector<unsigned short> *Track_jetRef;
    std::vector<unsigned short> *Track_numberOfValidHits;
    std::vector<unsigned short> *Track_numberOfLostHits;
    std::vector<short>   *Track_quality;
    Int_t           numberOfJet;
    std::vector<double>  *Jet_pt;
    std::vector<double>  *Jet_energy;
    std::vector<double>  *Jet_eta;
    std::vector<double>  *Jet_phi;
    std::vector<double>  *Jet_ecfr;
    std::vector<double>  *Jet_enfr;
    std::vector<double>  *Jet_hcfr;
    std::vector<double>  *Jet_hnfr;
    std::vector<double>  *Jet_mass;
    std::vector<double>  *Jet_ene_c3;
    std::vector<std::vector<unsigned short> > *Jet_vTrackref;
    Int_t           numberOfMuon;
    std::vector<unsigned short> *Muon_type;
    std::vector<unsigned int> *Muon_selmask;
    std::vector<double>  *Muon_pt;
    std::vector<double>  *Muon_eta;
    std::vector<double>  *Muon_phi;
    std::vector<short>   *Muon_charge;
    std::vector<unsigned short> *Muon_numberOfMatches;
    std::vector<double>  *Muon_globalChi2;
    std::vector<bool>    *Muon_isTracker;
    std::vector<bool>    *Muon_isGlobal;
    std::vector<bool>    *Muon_isGlobalMuonPromptTight;
    std::vector<bool>    *Muon_isTMLastStationTight;
    std::vector<bool>    *Muon_isTMLastStationAngTight;
    std::vector<bool>    *Muon_isTMLastStationOptimizedLowPtTight;
    std::vector<unsigned short> *Muon_validMuonHits;
    std::vector<unsigned short> *Muon_validCSCHits;
    std::vector<unsigned short> *Muon_validDTHits;
    std::vector<unsigned short> *Muon_validRPCHits;
    std::vector<unsigned short> *Muon_trackref;

    // List of branches
    TBranch        *b_evNumber;   //!
    TBranch        *b_runNumber;   //!
    TBranch        *b_lumiSection;   //!
    TBranch        *b_beamSpot_x;   //!
    TBranch        *b_beamSpot_y;   //!
    TBranch        *b_beamSpot_z;   //!
    TBranch        *b_numberOfVertex;   //!
    TBranch        *b_PV_x;   //!
    TBranch        *b_PV_y;   //!
    TBranch        *b_PV_z;   //!
    TBranch        *b_PV_xError;   //!
    TBranch        *b_PV_yError;   //!
    TBranch        *b_PV_zError;   //!
    TBranch        *b_PV_ndof;   //!
    TBranch        *b_PV_normalizedChi2;   //!
    TBranch        *b_PV_isFake;   //!
    TBranch        *b_numberOfTrack;   //!
    TBranch        *b_Track_pt;   //!
    TBranch        *b_Track_ptError;   //!
    TBranch        *b_Track_eta;   //!
    TBranch        *b_Track_etaError;   //!
    TBranch        *b_Track_phi;   //!
    TBranch        *b_Track_phiError;   //!
    TBranch        *b_Track_dxy_BS;   //!
    TBranch        *b_Track_dxy_PV;   //!
    TBranch        *b_Track_dxy;   //!
    TBranch        *b_Track_dxyError;   //!
    TBranch        *b_Track_dz;   //!
    TBranch        *b_Track_dzError;   //!
    TBranch        *b_Track_d0;   //!
    TBranch        *b_Track_d0Error;   //!
    TBranch        *b_Track_normalizedChi2;   //!
    TBranch        *b_Track_dEdx1;   //!
    TBranch        *b_Track_chi2;   //!
    TBranch        *b_Track_ndof;   //!
    TBranch        *b_Track_innerOk;   //!
    TBranch        *b_Track_isHighPurity;   //!
    TBranch        *b_Track_algo;   //!
    TBranch        *b_Track_vCovLowerBlock;   //!
    TBranch        *b_Track_validPixelHits;   //!
    TBranch        *b_Track_validTIBHits;   //!
    TBranch        *b_Track_validTIDHits;   //!
    TBranch        *b_Track_validTOBHits;   //!
    TBranch        *b_Track_validTECHits;   //!
    TBranch        *b_Track_referencePoint_x;   //!
    TBranch        *b_Track_innerPosition_x;   //!
    TBranch        *b_Track_outerPosition_x;   //!
    TBranch        *b_Track_referencePoint_y;   //!
    TBranch        *b_Track_innerPosition_y;   //!
    TBranch        *b_Track_outerPosition_y;   //!
    TBranch        *b_Track_referencePoint_z;   //!
    TBranch        *b_Track_innerPosition_z;   //!
    TBranch        *b_Track_outerPosition_z;   //!
    TBranch        *b_Track_charge;   //!
    TBranch        *b_Track_jetRef;   //!
    TBranch        *b_Track_numberOfValidHits;   //!
    TBranch        *b_Track_numberOfLostHits;   //!
    TBranch        *b_Track_quality;   //!
    TBranch        *b_Track_vGenpref;   //!
    TBranch        *b_Track_vGenpChi2;   //!
    TBranch        *b_numberOfJet;   //!
    TBranch        *b_Jet_pt;   //!
    TBranch        *b_Jet_energy;   //!
    TBranch        *b_Jet_eta;   //!
    TBranch        *b_Jet_phi;   //!
    TBranch        *b_Jet_ecfr;   //!
    TBranch        *b_Jet_enfr;   //!
    TBranch        *b_Jet_hcfr;   //!
    TBranch        *b_Jet_hnfr;   //!
    TBranch        *b_Jet_mass;   //!
    TBranch        *b_Jet_ene_c3;   //!
    TBranch        *b_Jet_vTrackref;   //!
    TBranch        *b_numberOfMuon;   //!
    TBranch        *b_Muon_type;   //!
    TBranch        *b_Muon_selmask;   //!
    TBranch        *b_Muon_pt;   //!
    TBranch        *b_Muon_eta;   //!
    TBranch        *b_Muon_phi;   //!
    TBranch        *b_Muon_charge;   //!
    TBranch        *b_Muon_numberOfMatches;   //!
    TBranch        *b_Muon_globalChi2;   //!
    TBranch        *b_Muon_isTracker;   //!
    TBranch        *b_Muon_isGlobal;   //!
    TBranch        *b_Muon_isGlobalMuonPromptTight;   //!
    TBranch        *b_Muon_isTMLastStationTight;   //!
    TBranch        *b_Muon_isTMLastStationAngTight;   //!
    TBranch        *b_Muon_isTMLastStationOptimizedLowPtTight;   //!
    TBranch        *b_Muon_validMuonHits;   //!
    TBranch        *b_Muon_validCSCHits;   //!
    TBranch        *b_Muon_validDTHits;   //!
    TBranch        *b_Muon_validRPCHits;   //!
    TBranch        *b_Muon_trackref;   //!

    virtual void     InitReco(TTree *tree);
};



void TreeDefinitionRecoV1::InitReco(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set object pointer
  PV_x = 0;
  PV_y = 0;
  PV_z = 0;
  PV_xError = 0;
  PV_yError = 0;
  PV_zError = 0;
  PV_ndof = 0;
  PV_normalizedChi2 = 0;
  PV_isFake = 0;
  Track_pt = 0;
  Track_ptError = 0;
  Track_eta = 0;
  Track_etaError = 0;
  Track_phi = 0;
  Track_phiError = 0;
  Track_dxy_BS = 0;
  Track_dxy_PV = 0;
  Track_dxy = 0;
  Track_dxyError = 0;
  Track_dz = 0;
  Track_dzError = 0;
  Track_d0 = 0;
  Track_d0Error = 0;
  Track_normalizedChi2 = 0;
  Track_dEdx1 = 0;
  Track_chi2 = 0;
  Track_ndof = 0;
  Track_innerOk = 0;
  Track_isHighPurity = 0;
  Track_algo = 0;
  Track_vCovLowerBlock = 0;
  Track_validPixelHits = 0;
  Track_validTIBHits = 0;
  Track_validTIDHits = 0;
  Track_validTOBHits = 0;
  Track_validTECHits = 0;
  Track_referencePoint_x = 0;
  Track_innerPosition_x = 0;
  Track_outerPosition_x = 0;
  Track_referencePoint_y = 0;
  Track_innerPosition_y = 0;
  Track_outerPosition_y = 0;
  Track_referencePoint_z = 0;
  Track_innerPosition_z = 0;
  Track_outerPosition_z = 0;
  Track_charge = 0;
  Track_jetRef = 0;
  Track_numberOfValidHits = 0;
  Track_numberOfLostHits = 0;
  Track_quality = 0;
  Jet_pt = 0;
  Jet_energy = 0;
  Jet_eta = 0;
  Jet_phi = 0;
  Jet_ecfr = 0;
  Jet_enfr = 0;
  Jet_hcfr = 0;
  Jet_hnfr = 0;
  Jet_mass = 0;
  Jet_ene_c3 = 0;
  Jet_vTrackref = 0;
  Muon_type = 0;
  Muon_selmask = 0;
  Muon_pt = 0;
  Muon_eta = 0;
  Muon_phi = 0;
  Muon_charge = 0;
  Muon_numberOfMatches = 0;
  Muon_globalChi2 = 0;
  Muon_isTracker = 0;
  Muon_isGlobal = 0;
  Muon_isGlobalMuonPromptTight = 0;
  Muon_isTMLastStationTight = 0;
  Muon_isTMLastStationAngTight = 0;
  Muon_isTMLastStationOptimizedLowPtTight = 0;
  Muon_validMuonHits = 0;
  Muon_validCSCHits = 0;
  Muon_validDTHits = 0;
  Muon_validRPCHits = 0;
  Muon_trackref = 0;

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("evNumber", &evNumber, &b_evNumber);
  fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
  fChain->SetBranchAddress("lumiSection", &lumiSection, &b_lumiSection);
  fChain->SetBranchAddress("beamSpot_x", &beamSpot_x, &b_beamSpot_x);
  fChain->SetBranchAddress("beamSpot_y", &beamSpot_y, &b_beamSpot_y);
  fChain->SetBranchAddress("beamSpot_z", &beamSpot_z, &b_beamSpot_z);
  fChain->SetBranchAddress("numberOfVertex", &numberOfVertex, &b_numberOfVertex);
  fChain->SetBranchAddress("PV_x", &PV_x, &b_PV_x);
  fChain->SetBranchAddress("PV_y", &PV_y, &b_PV_y);
  fChain->SetBranchAddress("PV_z", &PV_z, &b_PV_z);
  fChain->SetBranchAddress("PV_xError", &PV_xError, &b_PV_xError);
  fChain->SetBranchAddress("PV_yError", &PV_yError, &b_PV_yError);
  fChain->SetBranchAddress("PV_zError", &PV_zError, &b_PV_zError);
  fChain->SetBranchAddress("PV_ndof", &PV_ndof, &b_PV_ndof);
  fChain->SetBranchAddress("PV_normalizedChi2", &PV_normalizedChi2, &b_PV_normalizedChi2);
  fChain->SetBranchAddress("PV_isFake", &PV_isFake, &b_PV_isFake);
  fChain->SetBranchAddress("numberOfTrack", &numberOfTrack, &b_numberOfTrack);
  fChain->SetBranchAddress("Track_pt", &Track_pt, &b_Track_pt);
  fChain->SetBranchAddress("Track_ptError", &Track_ptError, &b_Track_ptError);
  fChain->SetBranchAddress("Track_eta", &Track_eta, &b_Track_eta);
  fChain->SetBranchAddress("Track_etaError", &Track_etaError, &b_Track_etaError);
  fChain->SetBranchAddress("Track_phi", &Track_phi, &b_Track_phi);
  fChain->SetBranchAddress("Track_phiError", &Track_phiError, &b_Track_phiError);
  fChain->SetBranchAddress("Track_dxy_BS", &Track_dxy_BS, &b_Track_dxy_BS);
  fChain->SetBranchAddress("Track_dxy_PV", &Track_dxy_PV, &b_Track_dxy_PV);
  fChain->SetBranchAddress("Track_dxy", &Track_dxy, &b_Track_dxy);
  fChain->SetBranchAddress("Track_dxyError", &Track_dxyError, &b_Track_dxyError);
  fChain->SetBranchAddress("Track_dz", &Track_dz, &b_Track_dz);
  fChain->SetBranchAddress("Track_dzError", &Track_dzError, &b_Track_dzError);
  fChain->SetBranchAddress("Track_d0", &Track_d0, &b_Track_d0);
  fChain->SetBranchAddress("Track_d0Error", &Track_d0Error, &b_Track_d0Error);
  fChain->SetBranchAddress("Track_normalizedChi2", &Track_normalizedChi2, &b_Track_normalizedChi2);
  fChain->SetBranchAddress("Track_dEdx1", &Track_dEdx1, &b_Track_dEdx1);
  fChain->SetBranchAddress("Track_chi2", &Track_chi2, &b_Track_chi2);
  fChain->SetBranchAddress("Track_ndof", &Track_ndof, &b_Track_ndof);
  fChain->SetBranchAddress("Track_innerOk", &Track_innerOk, &b_Track_innerOk);
  fChain->SetBranchAddress("Track_isHighPurity", &Track_isHighPurity, &b_Track_isHighPurity);
  fChain->SetBranchAddress("Track_algo", &Track_algo, &b_Track_algo);
  fChain->SetBranchAddress("Track_vCovLowerBlock", &Track_vCovLowerBlock, &b_Track_vCovLowerBlock);
  fChain->SetBranchAddress("Track_validPixelHits", &Track_validPixelHits, &b_Track_validPixelHits);
  fChain->SetBranchAddress("Track_validTIBHits", &Track_validTIBHits, &b_Track_validTIBHits);
  fChain->SetBranchAddress("Track_validTIDHits", &Track_validTIDHits, &b_Track_validTIDHits);
  fChain->SetBranchAddress("Track_validTOBHits", &Track_validTOBHits, &b_Track_validTOBHits);
  fChain->SetBranchAddress("Track_validTECHits", &Track_validTECHits, &b_Track_validTECHits);
  fChain->SetBranchAddress("Track_referencePoint_x", &Track_referencePoint_x, &b_Track_referencePoint_x);
  fChain->SetBranchAddress("Track_innerPosition_x", &Track_innerPosition_x, &b_Track_innerPosition_x);
  fChain->SetBranchAddress("Track_outerPosition_x", &Track_outerPosition_x, &b_Track_outerPosition_x);
  fChain->SetBranchAddress("Track_referencePoint_y", &Track_referencePoint_y, &b_Track_referencePoint_y);
  fChain->SetBranchAddress("Track_innerPosition_y", &Track_innerPosition_y, &b_Track_innerPosition_y);
  fChain->SetBranchAddress("Track_outerPosition_y", &Track_outerPosition_y, &b_Track_outerPosition_y);
  fChain->SetBranchAddress("Track_referencePoint_z", &Track_referencePoint_z, &b_Track_referencePoint_z);
  fChain->SetBranchAddress("Track_innerPosition_z", &Track_innerPosition_z, &b_Track_innerPosition_z);
  fChain->SetBranchAddress("Track_outerPosition_z", &Track_outerPosition_z, &b_Track_outerPosition_z);
  fChain->SetBranchAddress("Track_charge", &Track_charge, &b_Track_charge);
  fChain->SetBranchAddress("Track_jetRef", &Track_jetRef, &b_Track_jetRef);
  fChain->SetBranchAddress("Track_numberOfValidHits", &Track_numberOfValidHits, &b_Track_numberOfValidHits);
  fChain->SetBranchAddress("Track_numberOfLostHits", &Track_numberOfLostHits, &b_Track_numberOfLostHits);
  fChain->SetBranchAddress("Track_quality", &Track_quality, &b_Track_quality);
  fChain->SetBranchAddress("numberOfJet", &numberOfJet, &b_numberOfJet);
  fChain->SetBranchAddress("Jet_pt", &Jet_pt, &b_Jet_pt);
  fChain->SetBranchAddress("Jet_energy", &Jet_energy, &b_Jet_energy);
  fChain->SetBranchAddress("Jet_eta", &Jet_eta, &b_Jet_eta);
  fChain->SetBranchAddress("Jet_phi", &Jet_phi, &b_Jet_phi);
  fChain->SetBranchAddress("Jet_ecfr", &Jet_ecfr, &b_Jet_ecfr);
  fChain->SetBranchAddress("Jet_enfr", &Jet_enfr, &b_Jet_enfr);
  fChain->SetBranchAddress("Jet_hcfr", &Jet_hcfr, &b_Jet_hcfr);
  fChain->SetBranchAddress("Jet_hnfr", &Jet_hnfr, &b_Jet_hnfr);
  fChain->SetBranchAddress("Jet_mass", &Jet_mass, &b_Jet_mass);
  fChain->SetBranchAddress("Jet_ene_c3", &Jet_ene_c3, &b_Jet_ene_c3);
  fChain->SetBranchAddress("Jet_vTrackref", &Jet_vTrackref, &b_Jet_vTrackref);
  fChain->SetBranchAddress("numberOfMuon", &numberOfMuon, &b_numberOfMuon);
  fChain->SetBranchAddress("Muon_type", &Muon_type, &b_Muon_type);
  fChain->SetBranchAddress("Muon_selmask", &Muon_selmask, &b_Muon_selmask);
  fChain->SetBranchAddress("Muon_pt", &Muon_pt, &b_Muon_pt);
  fChain->SetBranchAddress("Muon_eta", &Muon_eta, &b_Muon_eta);
  fChain->SetBranchAddress("Muon_phi", &Muon_phi, &b_Muon_phi);
  fChain->SetBranchAddress("Muon_charge", &Muon_charge, &b_Muon_charge);
  fChain->SetBranchAddress("Muon_numberOfMatches", &Muon_numberOfMatches, &b_Muon_numberOfMatches);
  fChain->SetBranchAddress("Muon_globalChi2", &Muon_globalChi2, &b_Muon_globalChi2);
  fChain->SetBranchAddress("Muon_isTracker", &Muon_isTracker, &b_Muon_isTracker);
  fChain->SetBranchAddress("Muon_isGlobal", &Muon_isGlobal, &b_Muon_isGlobal);
  fChain->SetBranchAddress("Muon_isGlobalMuonPromptTight", &Muon_isGlobalMuonPromptTight, &b_Muon_isGlobalMuonPromptTight);
  fChain->SetBranchAddress("Muon_isTMLastStationTight", &Muon_isTMLastStationTight, &b_Muon_isTMLastStationTight);
  fChain->SetBranchAddress("Muon_isTMLastStationAngTight", &Muon_isTMLastStationAngTight, &b_Muon_isTMLastStationAngTight);
  fChain->SetBranchAddress("Muon_isTMLastStationOptimizedLowPtTight", &Muon_isTMLastStationOptimizedLowPtTight, &b_Muon_isTMLastStationOptimizedLowPtTight);
  fChain->SetBranchAddress("Muon_validMuonHits", &Muon_validMuonHits, &b_Muon_validMuonHits);
  fChain->SetBranchAddress("Muon_validCSCHits", &Muon_validCSCHits, &b_Muon_validCSCHits);
  fChain->SetBranchAddress("Muon_validDTHits", &Muon_validDTHits, &b_Muon_validDTHits);
  fChain->SetBranchAddress("Muon_validRPCHits", &Muon_validRPCHits, &b_Muon_validRPCHits);
  fChain->SetBranchAddress("Muon_trackref", &Muon_trackref, &b_Muon_trackref);
  Notify();
}

#endif // TreeDefinitionRecoV1_h
