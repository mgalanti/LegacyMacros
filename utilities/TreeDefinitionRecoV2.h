#ifndef TreeDefinitionRecoV2_h
#define TreeDefinitionRecoV2_h

// #ifndef __CINT__
// #pragma link C++ class std::vector<int>+;
// #pragma link C++ class std::vector<std::vector<double> >+;
// #endif

#include "TreeDefinitionBase.h"



class TreeDefinitionRecoV2 : public virtual TreeDefinitionBase
{
  public:
    // Declaration of leaf types

#if NTUPLE_VERSION >= 3
    // Metadata tree
    std::vector<std::string> *triggerPathNames;
    std::vector<std::string> *pdfWeightNames;
    unsigned int ntupleVersion;
    unsigned int ntupleMinorVersion;
#endif
#if NTUPLE_VERSION >= 4
    bool isRealData;
#endif

    // Event tree
    Int_t           evNumber;
    Int_t           runNumber;
    Int_t           lumiSection;

#if NTUPLE_VERSION >= 3
    std::vector<bool> *Trigger_accepted;
#endif

    Double_t        beamSpot_x;
    Double_t        beamSpot_y;
    Double_t        beamSpot_z;
    Double_t        beamSpot_xError;
    Double_t        beamSpot_yError;
    Double_t        beamSpot_zError;
#if NTUPLE_VERSION >= 3
    Double_t        beamSpot_dxdz;
    Double_t        beamSpot_dydz;
    Double_t        beamSpot_dxdzError;
    Double_t        beamSpot_dydzError;
#endif

#if NTUPLE_VERSION >= 4
    // L1 Track Trigger Tracks - they work only in PHASE 2 UPGRADE samples!!!
    std::vector<unsigned int> *L1TTTracks_theSector;
    std::vector<unsigned int> *L1TTTracks_theWedge;
    std::vector<double> *L1TTTracks_theStubPtConsistency4Par;
    std::vector<double> *L1TTTracks_theMomentum4Par_x;
    std::vector<double> *L1TTTracks_theMomentum4Par_y;
    std::vector<double> *L1TTTracks_theMomentum4Par_z;
    std::vector<double> *L1TTTracks_thePOCA4Par_x;
    std::vector<double> *L1TTTracks_thePOCA4Par_y;
    std::vector<double> *L1TTTracks_thePOCA4Par_z;
    std::vector<double> *L1TTTracks_theRInv4Par;
    std::vector<double> *L1TTTracks_theChi24Par;
    std::vector<double> *L1TTTracks_theChi2Red4Par;
    std::vector<double> *L1TTTracks_theStubPtConsistency5Par;
    std::vector<double> *L1TTTracks_theMomentum5Par_x;
    std::vector<double> *L1TTTracks_theMomentum5Par_y;
    std::vector<double> *L1TTTracks_theMomentum5Par_z;
    std::vector<double> *L1TTTracks_thePOCA5Par_x;
    std::vector<double> *L1TTTracks_thePOCA5Par_y;
    std::vector<double> *L1TTTracks_thePOCA5Par_z;
    std::vector<double> *L1TTTracks_theRInv5Par;
    std::vector<double> *L1TTTracks_theChi25Par;
    std::vector<double> *L1TTTracks_theChi2Red5Par;
    
    // L1 Tracker Muons - they work only in PHASE 2 UPGRADE samples!!!
    std::vector<unsigned short> *L1TkMuons_l1TTTrackRef;
    std::vector<float> *L1TkMuons_trkIsol;
    std::vector<float> *L1TkMuons_trkzVtx;
    //   std::vector<int> *L1TkMuons_bx; // L1TkMuonParticle interface broken!
    std::vector<unsigned int> *L1TkMuons_quality;
#endif
    
    Int_t           numberOfVertex;
    std::vector<double>  *PV_x;
    std::vector<double>  *PV_y;
    std::vector<double>  *PV_z;
    std::vector<double>  *PV_xError;
    std::vector<double>  *PV_yError;
    std::vector<double>  *PV_zError;
    std::vector<double>  *PV_ndof;
    std::vector<double>  *PV_normalizedChi2;
    std::vector<std::vector<unsigned short> > *PV_vTrackref;
#if NTUPLE_VERSION >= 3
    std::vector<std::vector<float> > *PV_vTrackWeight;
#endif
    std::vector<bool>    *PV_isFake;
    
#if NTUPLE_VERSION >= 3
    // Refitted (muon-less) primary vertices
    int numberOfRefitVertex;
    std::vector<double> *PVRefit_x;
    std::vector<double> *PVRefit_y;
    std::vector<double> *PVRefit_z;
    std::vector<double> *PVRefit_xError;
    std::vector<double> *PVRefit_yError;
    std::vector<double> *PVRefit_zError;
    std::vector<double> *PVRefit_ndof;
    std::vector<double> *PVRefit_normalizedChi2;
    std::vector<std::vector<unsigned short> > *PVRefit_vTrackref;
    std::vector<std::vector<float> > *PVRefit_vTrackWeight;
    std::vector<bool> *PVRefit_isFake;
    
    // Secondary vertices
    Int_t numberOfSecondaryVertex;
    std::vector<double> *SV_x;
    std::vector<double> *SV_y;
    std::vector<double> *SV_z;
    std::vector<double> *SV_flightDirection_x;
    std::vector<double> *SV_flightDirection_y;
    std::vector<double> *SV_flightDirection_z;
    std::vector<double> *SV_flightDistance;
    std::vector<double> *SV_flightDistanceError;
    std::vector<double> *SV_mass;
    std::vector<double> *SV_chi2;
    std::vector<double> *SV_ndof;
    std::vector<unsigned short> *SV_jetref;
    std::vector< std::vector<unsigned short> > *SV_vTrackref;
#endif

    Int_t           numberOfTrack;
    std::vector<double>  *Track_pt;
    std::vector<double>  *Track_ptError;
    std::vector<double>  *Track_eta;
    std::vector<double>  *Track_etaError;
    std::vector<double>  *Track_phi;
    std::vector<double>  *Track_phiError;
    std::vector<double>  *Track_dxyz_BS;
    std::vector<double>  *Track_dxyzError_BS;
    std::vector<double>  *Track_dxy_BS;
    std::vector<double>  *Track_dxyError_BS;
    std::vector<double>  *Track_dz_BS;
    std::vector<double>  *Track_dzError_BS;
    std::vector<std::vector<double> > *Track_dxy_PV;
    std::vector<std::vector<double> > *Track_dxyError_PV;
    std::vector<std::vector<double> > *Track_dxyz_PV;
    std::vector<std::vector<double> > *Track_dxyzError_PV;
    std::vector<std::vector<double> > *Track_dz_PV;
    std::vector<std::vector<double> > *Track_dzError_PV;
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
    std::vector<std::vector<double> > *Track_vCovLowerBlock;
    std::vector<bool>    *Track_innerOk;
    std::vector<bool>    *Track_isHighPurity;
    std::vector<unsigned short> *Track_validPixelHits;
    std::vector<unsigned short> *Track_validTIBHits;
    std::vector<unsigned short> *Track_validTIDHits;
    std::vector<unsigned short> *Track_validTOBHits;
    std::vector<unsigned short> *Track_validTECHits;
#if NTUPLE_VERSION >= 4
    std::vector<unsigned short> *Track_layersWithMeasurement;
#endif
    std::vector< std::vector<unsigned short> > *Track_hitPattern;
    std::vector< std::vector<unsigned short> > * Track_expInnHitPattern;
    std::vector< std::vector<unsigned short> > * Track_expOutHitPattern;
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
    std::vector<unsigned short> *Track_trackJetRef;
    std::vector<unsigned short> *Track_algo;
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
    std::vector<unsigned short> *Jet_nConstituents;
    std::vector<unsigned short> *Jet_chargedMultiplicity;
    std::vector<std::vector<unsigned short> > *Jet_vTrackref;
#if NTUPLE_VERSION >= 3
    std::vector<double> *Jet_TrackCountingHighEffBTag;
    std::vector<double> *Jet_TrackCountingHighPurBTag;
    std::vector<double> *Jet_JetProbabilityBTag;
    std::vector<double> *Jet_JetBProbabilityBTag;
    std::vector<double> *Jet_SimpleSecondaryVertexHighEffBTag;
    std::vector<double> *Jet_SimpleSecondaryVertexHighPurBTag;
    std::vector<double> *Jet_CombinedSecondaryVertexBTag;
    std::vector<double> *Jet_CombinedSecondaryVertexMVABTag;
    std::vector<double> *Jet_GhostTrackBTag;
    std::vector<double> *Jet_SoftElectronByIP3dBTag;
    std::vector<double> *Jet_SoftElectronByPtBTag;
    std::vector<double> *Jet_SoftMuonBTag;
    std::vector<double> *Jet_SoftMuonByIP3dBTag;
    std::vector<double> *Jet_SoftMuonByPtBTag;
    std::vector< std::vector<unsigned short> > *Jet_vSecVtxref;

    Int_t            numberOfTrackJet;
    std::vector<double> *TrackJet_pt;
    std::vector<double> *TrackJet_energy;
    std::vector<double> *TrackJet_eta;
    std::vector<double> *TrackJet_phi;
    std::vector<double> *TrackJet_mass;
    std::vector<double> *TrackJet_ene_c3;
    std::vector<unsigned short> *TrackJet_nConstituents;
    std::vector<std::vector<unsigned short> > *TrackJet_vTrackref;
#endif

    Int_t           numberOfMuon;
    std::vector<unsigned short> *Muon_type;
    std::vector<unsigned int> *Muon_selmask;
    std::vector<double>  *Muon_pt;
    std::vector<double>  *Muon_eta;
    std::vector<double>  *Muon_phi;
    std::vector<short>   *Muon_charge;
    std::vector<unsigned short> *Muon_numberOfMatches;
    std::vector<unsigned short> *Muon_stationMask;
    std::vector<unsigned short> *Muon_numberOfMatchedStations;
    std::vector<unsigned short> *Muon_numberOfChambers;
    std::vector<unsigned short> *Muon_numberOfChambersNoRPC;
    std::vector<std::vector<unsigned short> > *Muon_numberOfSegments;
    std::vector<double>  *Muon_caloCompatibility;
    std::vector<double>  *Muon_segmentCompatibility;
    std::vector<double>  *Muon_globalChi2;
#if NTUPLE_VERSION >= 3
    std::vector<float>   *Muon_Kink;
    std::vector<float>   *Muon_Kink_rho;
    std::vector<float>   *Muon_Kink_z;
#endif
#if NTUPLE_VERSION >= 4
    std::vector<float>   *Muon_GlobalKink;
    std::vector<float>   *Muon_GlobalKink_rho;
    std::vector<float>   *Muon_GlobalKink_z;
    std::vector<float>   *Muon_GlobalKink_chi2LocalMomentum;
    std::vector<float>   *Muon_GlobalKink_chi2LocalPosition;
    std::vector<float>   *Muon_glbTrackProbability;
    std::vector<float>   *Muon_GlobalKink_globalDeltaEtaPhi;
    std::vector<float>   *Muon_GlobalKink_localDistance;
    std::vector<float>   *Muon_GlobalKink_staRelChi2;
    std::vector<bool>    *Muon_GlobalKink_tightMatch;
    std::vector<float>   *Muon_GlobalKink_trkRelChi2;
    std::vector<bool> *Muon_isPFMuon;
    std::vector<std::vector<bool> > *Muon_isTight;
#endif
    std::vector<bool>    *Muon_isTracker;
    std::vector<bool>    *Muon_isGlobal;
    std::vector<bool>    *Muon_isGlobalMuonPromptTight;
    std::vector<bool>    *Muon_isTMLastStationTight;
    std::vector<bool>    *Muon_isTMLastStationAngTight;
    std::vector<bool>    *Muon_isTMLastStationOptimizedLowPtTight;
    std::vector<unsigned short> *Muon_globalValidMuonHits;
    std::vector<unsigned short> *Muon_globalValidCSCHits;
    std::vector<unsigned short> *Muon_globalValidDTHits;
    std::vector<unsigned short> *Muon_globalValidRPCHits;
    std::vector<unsigned short> *Muon_outerValidMuonHits;
    std::vector<unsigned short> *Muon_outerValidCSCHits;
    std::vector<unsigned short> *Muon_outerValidDTHits;
    std::vector<unsigned short> *Muon_outerValidRPCHits;
#if NTUPLE_VERSION >= 4
    std::vector<bool>  *Muon_isEnergyValid;
    std::vector<float> *Muon_energy_ecalTime;
    std::vector<float> *Muon_energy_ecalTimeError;
    std::vector<float> *Muon_energy_em;
    std::vector<float> *Muon_energy_emMax;
    std::vector<float> *Muon_energy_emS25;
    std::vector<float> *Muon_energy_emS9;
    std::vector<float> *Muon_energy_hcalTime;
    std::vector<float> *Muon_energy_hcalTimeError;
    std::vector<float> *Muon_energy_had;
    std::vector<float> *Muon_energy_hadMax;
    std::vector<float> *Muon_energy_hadS9;
    std::vector<float> *Muon_energy_ho;
    std::vector<float> *Muon_energy_hoS9;
    std::vector<float> *Muon_energy_tower;
    std::vector<float> *Muon_energy_towerS9;
    std::vector<bool>  *Muon_isIsolationValid;
    std::vector<float> *Muon_isolationR03_emEt;
    std::vector<float> *Muon_isolationR03_emVetoEt;
    std::vector<float> *Muon_isolationR03_hadEt;
    std::vector<float> *Muon_isolationR03_hadVetoEt;
    std::vector<float> *Muon_isolationR03_hoEt;
    std::vector<float> *Muon_isolationR03_hoVetoEt;
    std::vector<unsigned short> *Muon_isolationR03_nJets;
    std::vector<unsigned short> *Muon_isolationR03_nTracks;
    std::vector<float> *Muon_isolationR03_sumPt;
    std::vector<float> *Muon_isolationR03_trackerVetoPt;
    std::vector<bool>  *Muon_isPFIsolationValid;
    std::vector<float> *Muon_pfIsolationR03_sumChargedHadronPt;
    std::vector<float> *Muon_pfIsolationR03_sumChargedParticlePt;
    std::vector<float> *Muon_pfIsolationR03_sumNeutralHadronEt;
    std::vector<float> *Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold;
    std::vector<float> *Muon_pfIsolationR03_sumPhotonEt;
    std::vector<float> *Muon_pfIsolationR03_sumPhotonEtHighThreshold;
    std::vector<float> *Muon_pfIsolationR03_sumPUPt;
    std::vector<float> *Muon_trackIso;
    std::vector<float> *Muon_ecalIso;
    std::vector<float> *Muon_hcalIso;
    std::vector<float> *Muon_caloIso;
    std::vector<std::vector<int> >   *Muon_shower_nStationCorrelatedHits;
    std::vector<std::vector<int> >   *Muon_shower_nStationHits;
    std::vector<std::vector<float> > *Muon_shower_stationShowerDeltaR;
    std::vector<std::vector<float> > *Muon_shower_stationShowerSizeT;
    std::vector<short> *Muon_time_direction;
    std::vector<int>   *Muon_time_ndof;
    std::vector<float> *Muon_time_timeAtIpInOut;
    std::vector<float> *Muon_time_timeAtIpInOutErr;
    std::vector<float> *Muon_time_timeAtIpOutIn;
    std::vector<float> *Muon_time_timeAtIpOutInErr;
#endif
#if NTUPLE_VERSION >= 3
    std::vector<double> *Muon_Mu1State_x;
    std::vector<double> *Muon_Mu1State_y;
    std::vector<double> *Muon_Mu1State_z;
    std::vector<double> *Muon_Mu2State_x;
    std::vector<double> *Muon_Mu2State_y;
    std::vector<double> *Muon_Mu2State_z;
#endif
#if NTUPLE_VERSION >= 4
    std::vector<std::vector<float> > *Muon_dDxDz;
    std::vector<std::vector<float> > *Muon_dDyDz;
    std::vector<std::vector<float> > *Muon_Dx;
    std::vector<std::vector<float> > *Muon_Dy;
    std::vector<std::vector<float> > *Muon_pullDxDz;
    std::vector<std::vector<float> > *Muon_pullDyDz;
    std::vector<std::vector<float> > *Muon_pullX;
    std::vector<std::vector<float> > *Muon_pullY;
    std::vector<std::vector<float> > *Muon_segmentDxDz;
    std::vector<std::vector<float> > *Muon_segmentDxDzError;
    std::vector<std::vector<float> > *Muon_segmentDyDz;
    std::vector<std::vector<float> > *Muon_segmentDyDzError;
    std::vector<std::vector<float> > *Muon_segmentX;
    std::vector<std::vector<float> > *Muon_segmentXError;
    std::vector<std::vector<float> > *Muon_segmentY;
    std::vector<std::vector<float> > *Muon_segmentYError;
    std::vector<std::vector<float> > *Muon_trackDist;
    std::vector<std::vector<float> > *Muon_trackDistError;
    std::vector<std::vector<float> > *Muon_trackDxDz;
    std::vector<std::vector<float> > *Muon_trackDxDzError;
    std::vector<std::vector<float> > *Muon_trackDyDz;
    std::vector<std::vector<float> > *Muon_trackDyDzError;
    std::vector<std::vector<float> > *Muon_trackEdgeX;
    std::vector<std::vector<float> > *Muon_trackEdgeY;
    std::vector<std::vector<float> > *Muon_trackX;
    std::vector<std::vector<float> > *Muon_trackXError;
    std::vector<std::vector<float> > *Muon_trackY;
    std::vector<std::vector<float> > *Muon_trackYError;
    std::vector<unsigned int> *Muon_stationGapMaskDistance;
    std::vector<unsigned int> *Muon_stationGapMaskPull;
#endif
    std::vector<std::vector<unsigned short> > *Muon_vDiMuVtxMuonref;
    std::vector<std::vector<double> > *Muon_vDiMuVtxX;
    std::vector<std::vector<double> > *Muon_vDiMuVtxY;
    std::vector<std::vector<double> > *Muon_vDiMuVtxZ;
    std::vector<std::vector<double> > *Muon_vDiMuVtxProb;
    std::vector<std::vector<double> > *Muon_vDiMuVtxCTau;
    std::vector<std::vector<double> > *Muon_vDiMuVtxCTauError;
#if NTUPLE_VERSION >= 3
    std::vector<std::vector<unsigned short> > *Muon_vDiMuVtxPVRefitRef;
#endif
    std::vector<unsigned short> *Muon_trackref;
    std::vector<bool> *Muon_hasTriggered;
    std::vector<bool> *Muon_hasTriggeredMu3;
#if NTUPLE_VERSION >= 4
    std::vector<std::vector<bool> > *Muon_triggerInfo;
#endif
    
    // List of branches

#if NTUPLE_VERSION >= 3
    // Metadata tree
    TBranch        *b_triggerPathNames;   //!
    TBranch        *b_pdfWeightNames;   //!
    TBranch        *b_ntupleVersion;   //!
    TBranch        *b_ntupleMinorVersion;   //!
#endif
#if NTUPLE_VERSION >= 4
    TBranch        *b_isRealData;   //!
#endif

    TBranch        *b_evNumber;   //!
    TBranch        *b_runNumber;   //!
    TBranch        *b_lumiSection;   //!

#if NTUPLE_VERSION >= 3
    TBranch        *b_Trigger_accepted;   //!
#endif

    TBranch        *b_beamSpot_x;   //!
    TBranch        *b_beamSpot_y;   //!
    TBranch        *b_beamSpot_z;   //!
    TBranch        *b_beamSpot_xError;   //!
    TBranch        *b_beamSpot_yError;   //!
    TBranch        *b_beamSpot_zError;   //!
#if NTUPLE_VERSION >= 3
    TBranch        *b_beamSpot_dxdz;   //!
    TBranch        *b_beamSpot_dydz;   //!
    TBranch        *b_beamSpot_dxdzError;   //!
    TBranch        *b_beamSpot_dydzError;   //!
#endif

#if NTUPLE_VERSION >= 4

// L1 Track Trigger Tracks - they work only in PHASE 2 UPGRADE samples!!!
    TBranch        *b_L1TTTracks_theSector;
    TBranch        *b_L1TTTracks_theWedge;
    TBranch        *b_L1TTTracks_theStubPtConsistency4Par;
    TBranch        *b_L1TTTracks_theMomentum4Par_x;
    TBranch        *b_L1TTTracks_theMomentum4Par_y;
    TBranch        *b_L1TTTracks_theMomentum4Par_z;
    TBranch        *b_L1TTTracks_thePOCA4Par_x;
    TBranch        *b_L1TTTracks_thePOCA4Par_y;
    TBranch        *b_L1TTTracks_thePOCA4Par_z;
    TBranch        *b_L1TTTracks_theRInv4Par;
    TBranch        *b_L1TTTracks_theChi24Par;
    TBranch        *b_L1TTTracks_theChi2Red4Par;
    TBranch        *b_L1TTTracks_theStubPtConsistency5Par;
    TBranch        *b_L1TTTracks_theMomentum5Par_x;
    TBranch        *b_L1TTTracks_theMomentum5Par_y;
    TBranch        *b_L1TTTracks_theMomentum5Par_z;
    TBranch        *b_L1TTTracks_thePOCA5Par_x;
    TBranch        *b_L1TTTracks_thePOCA5Par_y;
    TBranch        *b_L1TTTracks_thePOCA5Par_z;
    TBranch        *b_L1TTTracks_theRInv5Par;
    TBranch        *b_L1TTTracks_theChi25Par;
    TBranch        *b_L1TTTracks_theChi2Red5Par;
// L1 Tracker Muons - they work only in PHASE 2 UPGRADE samples!!!
    TBranch        *b_L1TkMuons_l1TTTrackRef;
    TBranch        *b_L1TkMuons_trkIsol;
    TBranch        *b_L1TkMuons_trkzVtx;
    //   TBranch        *b_L1TkMuons_bx; // L1TkMuonParticle interface broken!
    TBranch        *b_L1TkMuons_quality;
#endif

    TBranch        *b_numberOfVertex;   //!
    TBranch        *b_PV_x;   //!
    TBranch        *b_PV_y;   //!
    TBranch        *b_PV_z;   //!
    TBranch        *b_PV_xError;   //!
    TBranch        *b_PV_yError;   //!
    TBranch        *b_PV_zError;   //!
    TBranch        *b_PV_ndof;   //!
    TBranch        *b_PV_normalizedChi2;   //!
    TBranch        *b_PV_vTrackref;   //!
#if NTUPLE_VERSION >= 3  
    TBranch        *b_PV_vTrackWeight;   //!
#endif
    TBranch        *b_PV_isFake;   //!

#if NTUPLE_VERSION >= 3
    TBranch        *b_numberOfRefitVertex;   //!
    TBranch        *b_PVRefit_x;   //!
    TBranch        *b_PVRefit_y;   //!
    TBranch        *b_PVRefit_z;   //!
    TBranch        *b_PVRefit_xError;   //!
    TBranch        *b_PVRefit_yError;   //!
    TBranch        *b_PVRefit_zError;   //!
    TBranch        *b_PVRefit_ndof;   //!
    TBranch        *b_PVRefit_normalizedChi2;   //!
    TBranch        *b_PVRefit_vTrackref;   //!
    TBranch        *b_PVRefit_vTrackWeight;   //!
    TBranch        *b_PVRefit_isFake;   //!

    TBranch        *b_numberOfSecondaryVertex;
    TBranch        *b_SV_x;
    TBranch        *b_SV_y;
    TBranch        *b_SV_z;
    TBranch        *b_SV_flightDirection_x;
    TBranch        *b_SV_flightDirection_y;
    TBranch        *b_SV_flightDirection_z;
    TBranch        *b_SV_flightDistance;
    TBranch        *b_SV_flightDistanceError;
    TBranch        *b_SV_mass;
    TBranch        *b_SV_chi2;
    TBranch        *b_SV_ndof;
    TBranch        *b_SV_jetref;
    TBranch        *b_SV_vTrackref;
#endif

    TBranch        *b_numberOfTrack;   //!
    TBranch        *b_Track_pt;   //!
    TBranch        *b_Track_ptError;   //!
    TBranch        *b_Track_eta;   //!
    TBranch        *b_Track_etaError;   //!
    TBranch        *b_Track_phi;   //!
    TBranch        *b_Track_phiError;   //!
    TBranch        *b_Track_dxyz_BS;   //!
    TBranch        *b_Track_dxyzError_BS;   //!
    TBranch        *b_Track_dxy_BS;   //!
    TBranch        *b_Track_dxyError_BS;   //!
    TBranch        *b_Track_dz_BS;   //!
    TBranch        *b_Track_dzError_BS;   //!
    TBranch        *b_Track_dxy_PV;   //!
    TBranch        *b_Track_dxyError_PV;   //!
    TBranch        *b_Track_dxyz_PV;   //!
    TBranch        *b_Track_dxyzError_PV;   //!
    TBranch        *b_Track_dz_PV;   //!
    TBranch        *b_Track_dzError_PV;   //!
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
    TBranch        *b_Track_vCovLowerBlock;   //!
    TBranch        *b_Track_innerOk;   //!
    TBranch        *b_Track_isHighPurity;   //!
    TBranch        *b_Track_validPixelHits;   //!
    TBranch        *b_Track_validTIBHits;   //!
    TBranch        *b_Track_validTIDHits;   //!
    TBranch        *b_Track_validTOBHits;   //!
    TBranch        *b_Track_validTECHits;   //!
#if NTUPLE_VERSION >=4
    TBranch        *b_Track_layersWithMeasurement;   //!
#endif
    TBranch        *b_Track_hitPattern;   //!
    TBranch        *b_Track_expInnHitPattern;   //!
    TBranch        *b_Track_expOutHitPattern;   //!
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
    TBranch        *b_Track_trackJetRef;   //!
    TBranch        *b_Track_algo;   //!
    TBranch        *b_Track_numberOfValidHits;   //!
    TBranch        *b_Track_numberOfLostHits;   //!
    TBranch        *b_Track_quality;   //!

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
    TBranch        *b_Jet_nConstituents;   //!
    TBranch        *b_Jet_chargedMultiplicity;   //!
    TBranch        *b_Jet_vTrackref;   //!
#if NTUPLE_VERSION >= 3
    TBranch        *b_Jet_TrackCountingHighEffBTag;
    TBranch        *b_Jet_TrackCountingHighPurBTag;
    TBranch        *b_Jet_JetProbabilityBTag;
    TBranch        *b_Jet_JetBProbabilityBTag;
    TBranch        *b_Jet_SimpleSecondaryVertexHighEffBTag;
    TBranch        *b_Jet_SimpleSecondaryVertexHighPurBTag;
    TBranch        *b_Jet_CombinedSecondaryVertexBTag;
    TBranch        *b_Jet_CombinedSecondaryVertexMVABTag;
    TBranch        *b_Jet_GhostTrackBTag;
    TBranch        *b_Jet_SoftElectronByIP3dBTag;
    TBranch        *b_Jet_SoftElectronByPtBTag;
    TBranch        *b_Jet_SoftMuonBTag;
    TBranch        *b_Jet_SoftMuonByIP3dBTag;
    TBranch        *b_Jet_SoftMuonByPtBTag;
    TBranch        *b_Jet_vSecVtxref;

    TBranch        *b_numberOfTrackJet;   //!
    TBranch        *b_TrackJet_pt;   //!
    TBranch        *b_TrackJet_energy;   //!
    TBranch        *b_TrackJet_eta;   //!
    TBranch        *b_TrackJet_phi;   //!
    TBranch        *b_TrackJet_mass;   //!
    TBranch        *b_TrackJet_ene_c3;   //!
    TBranch        *b_TrackJet_nConstituents;   //!
    TBranch        *b_TrackJet_vTrackref;   //!
#endif

    TBranch        *b_numberOfMuon;   //!
    TBranch        *b_Muon_type;   //!
    TBranch        *b_Muon_selmask;   //!
    TBranch        *b_Muon_pt;   //!
    TBranch        *b_Muon_eta;   //!
    TBranch        *b_Muon_phi;   //!
    TBranch        *b_Muon_charge;   //!
    TBranch        *b_Muon_numberOfMatches;   //!
#if NTUPLE_VERSION >= 3
    TBranch        *b_Muon_stationMask;   //!
    TBranch        *b_Muon_numberOfMatchedStations;   //!
    TBranch        *b_Muon_numberOfChambers;   //!
    TBranch        *b_Muon_numberOfChambersNoRPC;   //!
    TBranch        *b_Muon_numberOfSegments;   //!
#endif
    TBranch        *b_Muon_caloCompatibility;   //!
    TBranch        *b_Muon_segmentCompatibility;   //!
    TBranch        *b_Muon_globalChi2;   //!
#if NTUPLE_VERSION >= 3
    TBranch        *b_Muon_Kink;   //!
    TBranch        *b_Muon_Kink_rho;   //!
    TBranch        *b_Muon_Kink_z;   //!
#endif
#if NTUPLE_VERSION >= 4
    TBranch        *b_Muon_GlobalKink;   //!
    TBranch        *b_Muon_GlobalKink_rho;   //!
    TBranch        *b_Muon_GlobalKink_z;   //!
    TBranch        *b_Muon_GlobalKink_chi2LocalMomentum;   //!
    TBranch        *b_Muon_GlobalKink_chi2LocalPosition;   //!
    TBranch        *b_Muon_glbTrackProbability;   //!
    TBranch        *b_Muon_GlobalKink_globalDeltaEtaPhi;   //!
    TBranch        *b_Muon_GlobalKink_localDistance;   //!
    TBranch        *b_Muon_GlobalKink_staRelChi2;   //!
    TBranch        *b_Muon_GlobalKink_tightMatch;   //!
    TBranch        *b_Muon_GlobalKink_trkRelChi2;   //!
    TBranch        *b_Muon_isPFMuon;   //!
    TBranch        *b_Muon_isTight;   //!
#endif
    TBranch        *b_Muon_isTracker;   //!
    TBranch        *b_Muon_isGlobal;   //!
    TBranch        *b_Muon_isGlobalMuonPromptTight;   //!
    TBranch        *b_Muon_isTMLastStationTight;   //!
    TBranch        *b_Muon_isTMLastStationAngTight;   //!
    TBranch        *b_Muon_isTMLastStationOptimizedLowPtTight;   //!
    TBranch        *b_Muon_globalValidMuonHits;   //!
    TBranch        *b_Muon_globalValidCSCHits;   //!
    TBranch        *b_Muon_globalValidDTHits;   //!
    TBranch        *b_Muon_globalValidRPCHits;   //!
    TBranch        *b_Muon_outerValidMuonHits;   //!
    TBranch        *b_Muon_outerValidCSCHits;   //!
    TBranch        *b_Muon_outerValidDTHits;   //!
    TBranch        *b_Muon_outerValidRPCHits;   //!
#if NTUPLE_VERSION >= 4
    TBranch        *b_Muon_isEnergyValid;    //!
    TBranch        *b_Muon_energy_ecalTime;    //!
    TBranch        *b_Muon_energy_ecalTimeError;    //!
    TBranch        *b_Muon_energy_em;    //!
    TBranch        *b_Muon_energy_emMax;    //!
    TBranch        *b_Muon_energy_emS25;    //!
    TBranch        *b_Muon_energy_emS9;    //!
    TBranch        *b_Muon_energy_hcalTime;    //!
    TBranch        *b_Muon_energy_hcalTimeError;    //!
    TBranch        *b_Muon_energy_had;    //!
    TBranch        *b_Muon_energy_hadMax;    //!
    TBranch        *b_Muon_energy_hadS9;    //!
    TBranch        *b_Muon_energy_ho;    //!
    TBranch        *b_Muon_energy_hoS9;    //!
    TBranch        *b_Muon_energy_tower;    //!
    TBranch        *b_Muon_energy_towerS9;    //!
    TBranch        *b_Muon_isIsolationValid;    //!
    TBranch        *b_Muon_isolationR03_emEt;    //!
    TBranch        *b_Muon_isolationR03_emVetoEt;    //!
    TBranch        *b_Muon_isolationR03_hadEt;    //!
    TBranch        *b_Muon_isolationR03_hadVetoEt;    //!
    TBranch        *b_Muon_isolationR03_hoEt;    //!
    TBranch        *b_Muon_isolationR03_hoVetoEt;    //!
    TBranch        *b_Muon_isolationR03_nJets;    //!
    TBranch        *b_Muon_isolationR03_nTracks;    //!
    TBranch        *b_Muon_isolationR03_sumPt;    //!
    TBranch        *b_Muon_isolationR03_trackerVetoPt;    //!
    TBranch        *b_Muon_isPFIsolationValid;    //!
    TBranch        *b_Muon_pfIsolationR03_sumChargedHadronPt;    //!
    TBranch        *b_Muon_pfIsolationR03_sumChargedParticlePt;    //!
    TBranch        *b_Muon_pfIsolationR03_sumNeutralHadronEt;    //!
    TBranch        *b_Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold;    //!
    TBranch        *b_Muon_pfIsolationR03_sumPhotonEt;    //!
    TBranch        *b_Muon_pfIsolationR03_sumPhotonEtHighThreshold;    //!
    TBranch        *b_Muon_pfIsolationR03_sumPUPt;    //!
    TBranch        *b_Muon_trackIso;    //!
    TBranch        *b_Muon_ecalIso;    //!
    TBranch        *b_Muon_hcalIso;    //!
    TBranch        *b_Muon_caloIso;    //!
    TBranch        *b_Muon_shower_nStationCorrelatedHits;    //!
    TBranch        *b_Muon_shower_nStationHits;    //!
    TBranch        *b_Muon_shower_stationShowerDeltaR;    //!
    TBranch        *b_Muon_shower_stationShowerSizeT;    //!
    TBranch        *b_Muon_time_direction;    //!
    TBranch        *b_Muon_time_ndof;    //!
    TBranch        *b_Muon_time_timeAtIpInOut;    //!
    TBranch        *b_Muon_time_timeAtIpInOutErr;    //!
    TBranch        *b_Muon_time_timeAtIpOutIn;    //!
    TBranch        *b_Muon_time_timeAtIpOutInErr;    //!
#endif
#if NTUPLE_VERSION >= 3
    TBranch        *b_Muon_Mu1State_x;
    TBranch        *b_Muon_Mu1State_y;
    TBranch        *b_Muon_Mu1State_z;
    TBranch        *b_Muon_Mu2State_x;
    TBranch        *b_Muon_Mu2State_y;
    TBranch        *b_Muon_Mu2State_z;
#endif
#if NTUPLE_VERSION >= 3
    TBranch        *b_Muon_dDxDz;    //!
    TBranch        *b_Muon_dDyDz;    //!
    TBranch        *b_Muon_Dx;    //!
    TBranch        *b_Muon_Dy;    //!
    TBranch        *b_Muon_pullDxDz;    //!
    TBranch        *b_Muon_pullDyDz;    //!
    TBranch        *b_Muon_pullX;    //!
    TBranch        *b_Muon_pullY;    //!
    TBranch        *b_Muon_segmentDxDz;    //!
    TBranch        *b_Muon_segmentDxDzError;    //!
    TBranch        *b_Muon_segmentDyDz;    //!
    TBranch        *b_Muon_segmentDyDzError;    //!
    TBranch        *b_Muon_segmentX;    //!
    TBranch        *b_Muon_segmentXError;    //!
    TBranch        *b_Muon_segmentY;    //!
    TBranch        *b_Muon_segmentYError;    //!
    TBranch        *b_Muon_trackDist;    //!
    TBranch        *b_Muon_trackDistError;    //!
    TBranch        *b_Muon_trackDxDz;    //!
    TBranch        *b_Muon_trackDxDzError;    //!
    TBranch        *b_Muon_trackDyDz;    //!
    TBranch        *b_Muon_trackDyDzError;    //!
    TBranch        *b_Muon_trackEdgeX;    //!
    TBranch        *b_Muon_trackEdgeY;    //!
    TBranch        *b_Muon_trackX;    //!
    TBranch        *b_Muon_trackXError;    //!
    TBranch        *b_Muon_trackY;    //!
    TBranch        *b_Muon_trackYError;    //!
    TBranch        *b_Muon_stationGapMaskDistance;    //!
    TBranch        *b_Muon_stationGapMaskPull;    //!
#endif
    TBranch        *b_Muon_vDiMuVtxMuonref;   //!
    TBranch        *b_Muon_vDiMuVtxX;   //!
    TBranch        *b_Muon_vDiMuVtxY;   //!
    TBranch        *b_Muon_vDiMuVtxZ;   //!
    TBranch        *b_Muon_vDiMuVtxProb;   //!
    TBranch        *b_Muon_vDiMuVtxCTau;   //!
    TBranch        *b_Muon_vDiMuVtxCTauError;   //!
#if NTUPLE_VERSION >= 3
    TBranch        *b_Muon_vDiMuVtxPVRefitRef;
#endif
    TBranch        *b_Muon_trackref;   //!
    TBranch        *b_Muon_hasTriggered;   //!
    TBranch        *b_Muon_hasTriggeredMu3;   //!
#if NTUPLE_VERSION >= 4
    TBranch        *b_Muon_triggerInfo;   //!
#endif
#if NTUPLE_VERSION >= 3
    virtual void     InitMetadata(TTree *metadataTree);
#endif
    virtual void     InitReco(TTree *tree);
};



#if NTUPLE_VERSION >= 3
void TreeDefinitionRecoV2::InitMetadata(TTree *metadataTree)
{
  // Set branch addresses and branch pointers
  std::cout << "Setting branch addresses for metadata...\n";
  if (!metadataTree) return;
  fMetadataTree = metadataTree;
  fMetadataCurrent = -1;
  fMetadataTree->SetMakeClass(1);

  InitMetadataVariable("triggerPathNames", triggerPathNames, b_triggerPathNames);
  InitMetadataVariable("pdfWeightNames", pdfWeightNames, b_pdfWeightNames);
  InitMetadataVariable("ntupleVersion", ntupleVersion, b_ntupleVersion);
  InitMetadataVariable("ntupleMinorVersion", ntupleMinorVersion, b_ntupleMinorVersion);
#if NTUPLE_VERSION >=4
  InitMetadataVariable("isRealData", isRealData, b_isRealData);
#endif
  
  CheckMetadataVariable("ntupleVersion", hasMetadata);
}
#endif // NTUPLE_VERSION >= 3



void TreeDefinitionRecoV2::InitReco(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  
  std::cout << "Setting branch addresses for reco...\n";

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  InitVariable("evNumber", evNumber, b_evNumber);
  InitVariable("runNumber", runNumber, b_runNumber);
  InitVariable("lumiSection", lumiSection, b_lumiSection);

#if NTUPLE_VERSION >= 3
  InitVariable("Trigger_accepted",Trigger_accepted,b_Trigger_accepted);
  CheckVariable("Trigger_accepted", hasTriggerInfo);
#endif
  InitVariable("beamSpot_x", beamSpot_x, b_beamSpot_x);
  InitVariable("beamSpot_y", beamSpot_y, b_beamSpot_y);
  InitVariable("beamSpot_z", beamSpot_z, b_beamSpot_z);
    
  InitVariable("beamSpot_xError", beamSpot_xError, b_beamSpot_xError);
  InitVariable("beamSpot_yError", beamSpot_yError, b_beamSpot_yError);
  InitVariable("beamSpot_zError", beamSpot_zError, b_beamSpot_zError);
#if NTUPLE_VERSION >= 3
  InitVariable("beamSpot_dxdz", beamSpot_dxdz, b_beamSpot_dxdz);
  InitVariable("beamSpot_dydz", beamSpot_dydz, b_beamSpot_dydz);
  InitVariable("beamSpot_dxdzError", beamSpot_dxdzError, b_beamSpot_dxdzError);
  InitVariable("beamSpot_dydzError", beamSpot_dydzError, b_beamSpot_dydzError);
#endif
  CheckVariable("beamSpot_x", hasBeamSpot);
  
#if NTUPLE_VERSION >= 4
  // L1 Track Trigger Tracks - they work only in PHASE 2 UPGRADE samples!!!
  InitVariable("L1TTTracks_theSector", L1TTTracks_theSector, b_L1TTTracks_theSector);
  InitVariable("L1TTTracks_theWedge", L1TTTracks_theWedge, b_L1TTTracks_theWedge);
  InitVariable("L1TTTracks_theStubPtConsistency4Par", L1TTTracks_theStubPtConsistency4Par, b_L1TTTracks_theStubPtConsistency4Par);
  InitVariable("L1TTTracks_theMomentum4Par_x", L1TTTracks_theMomentum4Par_x, b_L1TTTracks_theMomentum4Par_x);
  InitVariable("L1TTTracks_theMomentum4Par_y", L1TTTracks_theMomentum4Par_y, b_L1TTTracks_theMomentum4Par_y);
  InitVariable("L1TTTracks_theMomentum4Par_z", L1TTTracks_theMomentum4Par_z, b_L1TTTracks_theMomentum4Par_z);
  InitVariable("L1TTTracks_thePOCA4Par_x", L1TTTracks_thePOCA4Par_x, b_L1TTTracks_thePOCA4Par_x);
  InitVariable("L1TTTracks_thePOCA4Par_y", L1TTTracks_thePOCA4Par_y, b_L1TTTracks_thePOCA4Par_y);
  InitVariable("L1TTTracks_thePOCA4Par_z", L1TTTracks_thePOCA4Par_z, b_L1TTTracks_thePOCA4Par_z);
  InitVariable("L1TTTracks_theRInv4Par", L1TTTracks_theRInv4Par, b_L1TTTracks_theRInv4Par);
  InitVariable("L1TTTracks_theChi24Par", L1TTTracks_theChi24Par, b_L1TTTracks_theChi24Par);
  InitVariable("L1TTTracks_theChi2Red4Par", L1TTTracks_theChi2Red4Par, b_L1TTTracks_theChi2Red4Par);
  InitVariable("L1TTTracks_theStubPtConsistency5Par", L1TTTracks_theStubPtConsistency5Par, b_L1TTTracks_theStubPtConsistency5Par);
  InitVariable("L1TTTracks_theMomentum5Par_x", L1TTTracks_theMomentum5Par_x, b_L1TTTracks_theMomentum5Par_x);
  InitVariable("L1TTTracks_theMomentum5Par_y", L1TTTracks_theMomentum5Par_y, b_L1TTTracks_theMomentum5Par_y);
  InitVariable("L1TTTracks_theMomentum5Par_z", L1TTTracks_theMomentum5Par_z, b_L1TTTracks_theMomentum5Par_z);
  InitVariable("L1TTTracks_thePOCA5Par_x", L1TTTracks_thePOCA5Par_x, b_L1TTTracks_thePOCA5Par_x);
  InitVariable("L1TTTracks_thePOCA5Par_y", L1TTTracks_thePOCA5Par_y, b_L1TTTracks_thePOCA5Par_y);
  InitVariable("L1TTTracks_thePOCA5Par_z", L1TTTracks_thePOCA5Par_z, b_L1TTTracks_thePOCA5Par_z);
  InitVariable("L1TTTracks_theRInv5Par", L1TTTracks_theRInv5Par, b_L1TTTracks_theRInv5Par);
  InitVariable("L1TTTracks_theChi25Par", L1TTTracks_theChi25Par, b_L1TTTracks_theChi25Par);
  InitVariable("L1TTTracks_theChi2Red5Par",L1TTTracks_theChi2Red5Par, b_L1TTTracks_theChi2Red5Par);
  
  // L1 Tracker Muons - they work only in PHASE 2 UPGRADE samples!!!
  InitVariable("L1TkMuons_l1TTTrackRef", L1TkMuons_l1TTTrackRef, b_L1TkMuons_l1TTTrackRef);
  InitVariable("L1TkMuons_trkIsol", L1TkMuons_trkIsol, b_L1TkMuons_trkIsol);
  InitVariable("L1TkMuons_trkzVtx", L1TkMuons_trkzVtx, b_L1TkMuons_trkzVtx);
  //   InitVariable("L1TkMuons_bx", L1TkMuons_bx, b_L1TkMuons_bx); // L1TkMuonParticle interface broken!
  InitVariable("L1TkMuons_quality", L1TkMuons_quality, b_L1TkMuons_quality);
#endif

  InitVariable("numberOfVertex", numberOfVertex, b_numberOfVertex);
  InitVariable("PV_x", PV_x, b_PV_x);
  InitVariable("PV_y", PV_y, b_PV_y);
  InitVariable("PV_z", PV_z, b_PV_z);
  InitVariable("PV_xError", PV_xError, b_PV_xError);
  InitVariable("PV_yError", PV_yError, b_PV_yError);
  InitVariable("PV_zError", PV_zError, b_PV_zError);
  InitVariable("PV_ndof", PV_ndof, b_PV_ndof);
  InitVariable("PV_normalizedChi2", PV_normalizedChi2, b_PV_normalizedChi2);
  InitVariable("PV_vTrackref", PV_vTrackref, b_PV_vTrackref);
#if NTUPLE_VERSION >= 3
  InitVariable("PV_vTrackWeight", PV_vTrackWeight, b_PV_vTrackWeight);
#endif
  InitVariable("PV_isFake", PV_isFake, b_PV_isFake);
  CheckVariable("numberOfVertex", hasPV);
  
#if NTUPLE_VERSION >= 3
  InitVariable("numberOfRefitVertex", numberOfRefitVertex, b_numberOfRefitVertex);
  InitVariable("PVRefit_x", PVRefit_x, b_PVRefit_x);
  InitVariable("PVRefit_y", PVRefit_y, b_PVRefit_y);
  InitVariable("PVRefit_z", PVRefit_z, b_PVRefit_z);
  InitVariable("PVRefit_xError", PVRefit_xError, b_PVRefit_xError);
  InitVariable("PVRefit_yError", PVRefit_yError, b_PVRefit_yError);
  InitVariable("PVRefit_zError", PVRefit_zError, b_PVRefit_zError);
  InitVariable("PVRefit_ndof", PVRefit_ndof, b_PVRefit_ndof);
  InitVariable("PVRefit_normalizedChi2", PVRefit_normalizedChi2, b_PVRefit_normalizedChi2);
  InitVariable("PVRefit_vTrackref", PVRefit_vTrackref, b_PVRefit_vTrackref);
  InitVariable("PVRefit_vTrackWeight", PVRefit_vTrackWeight, b_PVRefit_vTrackWeight);
  InitVariable("PVRefit_isFake", PVRefit_isFake, b_PVRefit_isFake);
  CheckVariable("numberOfRefitVertex", hasPVRefit);
  
  InitVariable("numberOfSecondaryVertex", numberOfSecondaryVertex, b_numberOfSecondaryVertex);
  InitVariable("SV_x", SV_x, b_SV_x);
  InitVariable("SV_y", SV_y, b_SV_y);
  InitVariable("SV_z", SV_z, b_SV_z);
  InitVariable("SV_flightDirection_x", SV_flightDirection_x, b_SV_flightDirection_x);
  InitVariable("SV_flightDirection_y", SV_flightDirection_y, b_SV_flightDirection_y);
  InitVariable("SV_flightDirection_z", SV_flightDirection_z, b_SV_flightDirection_z);
  InitVariable("SV_flightDistance", SV_flightDistance, b_SV_flightDistance);
  InitVariable("SV_flightDistanceError", SV_flightDistanceError, b_SV_flightDistanceError);
  InitVariable("SV_mass", SV_mass, b_SV_mass);
  InitVariable("SV_chi2", SV_chi2, b_SV_chi2);
  InitVariable("SV_ndof", SV_ndof, b_SV_ndof);
  InitVariable("SV_jetref", SV_jetref, b_SV_jetref);
  InitVariable("SV_vTrackref", SV_vTrackref, b_SV_vTrackref);
  CheckVariable("numberOfSecondaryVertex", hasSV);
#endif

  InitVariable("numberOfTrack", numberOfTrack, b_numberOfTrack);
  InitVariable("Track_pt", Track_pt, b_Track_pt);
  InitVariable("Track_ptError", Track_ptError, b_Track_ptError);
  InitVariable("Track_eta", Track_eta, b_Track_eta);
  InitVariable("Track_etaError", Track_etaError, b_Track_etaError);
  InitVariable("Track_phi", Track_phi, b_Track_phi);
  InitVariable("Track_phiError", Track_phiError, b_Track_phiError);
  InitVariable("Track_dxyz_BS", Track_dxyz_BS, b_Track_dxyz_BS);
  InitVariable("Track_dxyzError_BS", Track_dxyzError_BS, b_Track_dxyzError_BS);
  InitVariable("Track_dxy_BS", Track_dxy_BS, b_Track_dxy_BS);
  InitVariable("Track_dxyError_BS", Track_dxyError_BS, b_Track_dxyError_BS);
  InitVariable("Track_dz_BS", Track_dz_BS, b_Track_dz_BS);
  InitVariable("Track_dzError_BS", Track_dzError_BS, b_Track_dzError_BS);
  InitVariable("Track_dxy_PV", Track_dxy_PV, b_Track_dxy_PV);
  InitVariable("Track_dxyError_PV", Track_dxyError_PV, b_Track_dxyError_PV);
  InitVariable("Track_dxyz_PV", Track_dxyz_PV, b_Track_dxyz_PV);
  InitVariable("Track_dxyzError_PV", Track_dxyzError_PV, b_Track_dxyzError_PV);
  InitVariable("Track_dz_PV", Track_dz_PV, b_Track_dz_PV);
  InitVariable("Track_dzError_PV", Track_dzError_PV, b_Track_dzError_PV);
  InitVariable("Track_dxy", Track_dxy, b_Track_dxy);
  InitVariable("Track_dxyError", Track_dxyError, b_Track_dxyError);
  InitVariable("Track_dz", Track_dz, b_Track_dz);
  InitVariable("Track_dzError", Track_dzError, b_Track_dzError);
  InitVariable("Track_d0", Track_d0, b_Track_d0);
  InitVariable("Track_d0Error", Track_d0Error, b_Track_d0Error);
  InitVariable("Track_normalizedChi2", Track_normalizedChi2, b_Track_normalizedChi2);
  InitVariable("Track_dEdx1", Track_dEdx1, b_Track_dEdx1);
  InitVariable("Track_chi2", Track_chi2, b_Track_chi2);
  InitVariable("Track_ndof", Track_ndof, b_Track_ndof);
  InitVariable("Track_vCovLowerBlock", Track_vCovLowerBlock, b_Track_vCovLowerBlock);
  InitVariable("Track_innerOk", Track_innerOk, b_Track_innerOk);
  InitVariable("Track_isHighPurity", Track_isHighPurity, b_Track_isHighPurity);
  InitVariable("Track_validPixelHits", Track_validPixelHits, b_Track_validPixelHits);
  InitVariable("Track_validTIBHits", Track_validTIBHits, b_Track_validTIBHits);
  InitVariable("Track_validTIDHits", Track_validTIDHits, b_Track_validTIDHits);
  InitVariable("Track_validTOBHits", Track_validTOBHits, b_Track_validTOBHits);
  InitVariable("Track_validTECHits", Track_validTECHits, b_Track_validTECHits);
#if NTUPLE_VERSION >=4
  InitVariable("Track_layersWithMeasurement", Track_layersWithMeasurement, b_Track_layersWithMeasurement);
#endif
  InitVariable("Track_hitPattern", Track_hitPattern, b_Track_hitPattern);
  InitVariable("Track_expInnHitPattern", Track_expInnHitPattern, b_Track_expInnHitPattern);
  InitVariable("Track_expOutHitPattern", Track_expOutHitPattern, b_Track_expOutHitPattern);
  InitVariable("Track_referencePoint_x", Track_referencePoint_x, b_Track_referencePoint_x);
  InitVariable("Track_innerPosition_x", Track_innerPosition_x, b_Track_innerPosition_x);
  InitVariable("Track_outerPosition_x", Track_outerPosition_x, b_Track_outerPosition_x);
  InitVariable("Track_referencePoint_y", Track_referencePoint_y, b_Track_referencePoint_y);
  InitVariable("Track_innerPosition_y", Track_innerPosition_y, b_Track_innerPosition_y);
  InitVariable("Track_outerPosition_y", Track_outerPosition_y, b_Track_outerPosition_y);
  InitVariable("Track_referencePoint_z", Track_referencePoint_z, b_Track_referencePoint_z);
  InitVariable("Track_innerPosition_z", Track_innerPosition_z, b_Track_innerPosition_z);
  InitVariable("Track_outerPosition_z", Track_outerPosition_z, b_Track_outerPosition_z);
  InitVariable("Track_charge", Track_charge, b_Track_charge);
  InitVariable("Track_jetRef", Track_jetRef, b_Track_jetRef);
  InitVariable("Track_trackJetRef", Track_trackJetRef, b_Track_trackJetRef);
  InitVariable("Track_algo", Track_algo, b_Track_algo);
  InitVariable("Track_numberOfValidHits", Track_numberOfValidHits, b_Track_numberOfValidHits);
  InitVariable("Track_numberOfLostHits", Track_numberOfLostHits, b_Track_numberOfLostHits);
  InitVariable("Track_quality", Track_quality, b_Track_quality);
  CheckVariable("numberOfTrack", hasTrack);
  
  InitVariable("numberOfJet", numberOfJet, b_numberOfJet);
  InitVariable("Jet_pt", Jet_pt, b_Jet_pt);
  InitVariable("Jet_energy", Jet_energy, b_Jet_energy);
  InitVariable("Jet_eta", Jet_eta, b_Jet_eta);
  InitVariable("Jet_phi", Jet_phi, b_Jet_phi);
  InitVariable("Jet_ecfr", Jet_ecfr, b_Jet_ecfr);
  InitVariable("Jet_enfr", Jet_enfr, b_Jet_enfr);
  InitVariable("Jet_hcfr", Jet_hcfr, b_Jet_hcfr);
  InitVariable("Jet_hnfr", Jet_hnfr, b_Jet_hnfr);
  InitVariable("Jet_mass", Jet_mass, b_Jet_mass);
  InitVariable("Jet_ene_c3", Jet_ene_c3, b_Jet_ene_c3);
  InitVariable("Jet_nConstituents", Jet_nConstituents, b_Jet_nConstituents);
  InitVariable("Jet_chargedMultiplicity", Jet_chargedMultiplicity, b_Jet_chargedMultiplicity);
  InitVariable("Jet_vTrackref", Jet_vTrackref, b_Jet_vTrackref);
  CheckVariable("numberOfJet", hasJet);
#if NTUPLE_VERSION >= 3
  InitVariable("Jet_TrackCountingHighEffBTag", Jet_TrackCountingHighEffBTag, b_Jet_TrackCountingHighEffBTag);
  InitVariable("Jet_TrackCountingHighPurBTag", Jet_TrackCountingHighPurBTag, b_Jet_TrackCountingHighPurBTag);
  InitVariable("Jet_JetProbabilityBTag", Jet_JetProbabilityBTag, b_Jet_JetProbabilityBTag);
  InitVariable("Jet_JetBProbabilityBTag", Jet_JetBProbabilityBTag, b_Jet_JetBProbabilityBTag);
  InitVariable("Jet_SimpleSecondaryVertexHighEffBTag", Jet_SimpleSecondaryVertexHighEffBTag, b_Jet_SimpleSecondaryVertexHighEffBTag);
  InitVariable("Jet_SimpleSecondaryVertexHighPurBTag", Jet_SimpleSecondaryVertexHighPurBTag, b_Jet_SimpleSecondaryVertexHighPurBTag);
  InitVariable("Jet_CombinedSecondaryVertexBTag", Jet_CombinedSecondaryVertexBTag, b_Jet_CombinedSecondaryVertexBTag);
  InitVariable("Jet_CombinedSecondaryVertexMVABTag", Jet_CombinedSecondaryVertexMVABTag, b_Jet_CombinedSecondaryVertexMVABTag);
  InitVariable("Jet_GhostTrackBTag", Jet_GhostTrackBTag, b_Jet_GhostTrackBTag);
  InitVariable("Jet_SoftElectronByIP3dBTag", Jet_SoftElectronByIP3dBTag, b_Jet_SoftElectronByIP3dBTag);
  InitVariable("Jet_SoftElectronByPtBTag", Jet_SoftElectronByPtBTag, b_Jet_SoftElectronByPtBTag);
  InitVariable("Jet_SoftMuonBTag", Jet_SoftMuonBTag, b_Jet_SoftMuonBTag);
  InitVariable("Jet_SoftMuonByIP3dBTag", Jet_SoftMuonByIP3dBTag, b_Jet_SoftMuonByIP3dBTag);
  InitVariable("Jet_SoftMuonByPtBTag", Jet_SoftMuonByPtBTag, b_Jet_SoftMuonByPtBTag);
  InitVariable("Jet_vSecVtxref", Jet_vSecVtxref, b_Jet_vSecVtxref);
  CheckVariable("Jet_TrackCountingHighEffBTag",hasJetBTag);
#endif

#if NTUPLE_VERSION >= 3
  InitVariable("numberOfTrackJet", numberOfTrackJet, b_numberOfTrackJet);
  InitVariable("TrackJet_pt", TrackJet_pt, b_TrackJet_pt);
  InitVariable("TrackJet_energy", TrackJet_energy, b_TrackJet_energy);
  InitVariable("TrackJet_eta", TrackJet_eta, b_TrackJet_eta);
  InitVariable("TrackJet_phi", TrackJet_phi, b_TrackJet_phi);
  InitVariable("TrackJet_mass", TrackJet_mass, b_TrackJet_mass);
  InitVariable("TrackJet_ene_c3", TrackJet_ene_c3, b_TrackJet_ene_c3);
  InitVariable("TrackJet_nConstituents", TrackJet_nConstituents, b_TrackJet_nConstituents);
  InitVariable("TrackJet_vTrackref", TrackJet_vTrackref, b_TrackJet_vTrackref);
  CheckVariable("numberOfTrackJet", hasTrackJet);
#endif

  InitVariable("numberOfMuon", numberOfMuon, b_numberOfMuon);
  InitVariable("Muon_type", Muon_type, b_Muon_type);
  InitVariable("Muon_selmask", Muon_selmask, b_Muon_selmask);
  InitVariable("Muon_pt", Muon_pt, b_Muon_pt);
  InitVariable("Muon_eta", Muon_eta, b_Muon_eta);
  InitVariable("Muon_phi", Muon_phi, b_Muon_phi);
  InitVariable("Muon_charge", Muon_charge, b_Muon_charge);
  InitVariable("Muon_numberOfMatches", Muon_numberOfMatches, b_Muon_numberOfMatches);
#if NTUPLE_VERSION >= 3
  InitVariable("Muon_stationMask", Muon_stationMask, b_Muon_stationMask);
  InitVariable("Muon_numberOfMatchedStations", Muon_numberOfMatchedStations, b_Muon_numberOfMatchedStations);
  InitVariable("Muon_numberOfChambers", Muon_numberOfChambers, b_Muon_numberOfChambers);
  InitVariable("Muon_numberOfChambersNoRPC", Muon_numberOfChambersNoRPC, b_Muon_numberOfChambersNoRPC);
  InitVariable("Muon_numberOfSegments", Muon_numberOfSegments, b_Muon_numberOfSegments);
#endif
  InitVariable("Muon_caloCompatibility", Muon_caloCompatibility, b_Muon_caloCompatibility);
  InitVariable("Muon_segmentCompatibility", Muon_segmentCompatibility, b_Muon_segmentCompatibility);
  InitVariable("Muon_globalChi2", Muon_globalChi2, b_Muon_globalChi2);
#if NTUPLE_VERSION >= 3
  InitVariable("Muon_Kink", Muon_Kink, b_Muon_Kink);
  InitVariable("Muon_Kink_rho", Muon_Kink_rho, b_Muon_Kink_rho);
  InitVariable("Muon_Kink_z", Muon_Kink_z, b_Muon_Kink_z);
#endif
#if NTUPLE_VERSION >= 4
  InitVariable("Muon_GlobalKink", Muon_GlobalKink, b_Muon_GlobalKink);
  InitVariable("Muon_GlobalKink_rho", Muon_GlobalKink_rho, b_Muon_GlobalKink_rho);
  InitVariable("Muon_GlobalKink_z", Muon_GlobalKink_z, b_Muon_GlobalKink_z);
  InitVariable("Muon_GlobalKink_chi2LocalMomentum", Muon_GlobalKink_chi2LocalMomentum, b_Muon_GlobalKink_chi2LocalMomentum);
  InitVariable("Muon_GlobalKink_chi2LocalPosition", Muon_GlobalKink_chi2LocalPosition, b_Muon_GlobalKink_chi2LocalPosition);
  InitVariable("Muon_glbTrackProbability", Muon_glbTrackProbability, b_Muon_glbTrackProbability);
  InitVariable("Muon_GlobalKink_globalDeltaEtaPhi", Muon_GlobalKink_globalDeltaEtaPhi, b_Muon_GlobalKink_globalDeltaEtaPhi);
  InitVariable("Muon_GlobalKink_localDistance", Muon_GlobalKink_localDistance, b_Muon_GlobalKink_localDistance);
  InitVariable("Muon_GlobalKink_staRelChi2", Muon_GlobalKink_staRelChi2, b_Muon_GlobalKink_staRelChi2);
  InitVariable("Muon_GlobalKink_tightMatch", Muon_GlobalKink_tightMatch, b_Muon_GlobalKink_tightMatch);
  InitVariable("Muon_GlobalKink_trkRelChi2", Muon_GlobalKink_trkRelChi2, b_Muon_GlobalKink_trkRelChi2);
  InitVariable("Muon_isPFMuon", Muon_isPFMuon, b_Muon_isPFMuon);
  InitVariable("Muon_isTight", Muon_isTight, b_Muon_isTight);
#endif
  InitVariable("Muon_isTracker", Muon_isTracker, b_Muon_isTracker);
  InitVariable("Muon_isGlobal", Muon_isGlobal, b_Muon_isGlobal);
  InitVariable("Muon_isGlobalMuonPromptTight", Muon_isGlobalMuonPromptTight, b_Muon_isGlobalMuonPromptTight);
  InitVariable("Muon_isTMLastStationTight", Muon_isTMLastStationTight, b_Muon_isTMLastStationTight);
  InitVariable("Muon_isTMLastStationAngTight", Muon_isTMLastStationAngTight, b_Muon_isTMLastStationAngTight);
  InitVariable("Muon_isTMLastStationOptimizedLowPtTight", Muon_isTMLastStationOptimizedLowPtTight, b_Muon_isTMLastStationOptimizedLowPtTight);
  InitVariable("Muon_globalValidMuonHits", Muon_globalValidMuonHits, b_Muon_globalValidMuonHits);
  InitVariable("Muon_globalValidCSCHits", Muon_globalValidCSCHits, b_Muon_globalValidCSCHits);
  InitVariable("Muon_globalValidDTHits", Muon_globalValidDTHits, b_Muon_globalValidDTHits);
  InitVariable("Muon_globalValidRPCHits", Muon_globalValidRPCHits, b_Muon_globalValidRPCHits);
  InitVariable("Muon_outerValidMuonHits", Muon_outerValidMuonHits, b_Muon_outerValidMuonHits);
  InitVariable("Muon_outerValidCSCHits", Muon_outerValidCSCHits, b_Muon_outerValidCSCHits);
  InitVariable("Muon_outerValidDTHits", Muon_outerValidDTHits, b_Muon_outerValidDTHits);
  InitVariable("Muon_outerValidRPCHits", Muon_outerValidRPCHits, b_Muon_outerValidRPCHits);
#if NTUPLE_VERSION >= 4
  InitVariable("Muon_isEnergyValid", Muon_isEnergyValid, b_Muon_isEnergyValid);
  InitVariable("Muon_energy_ecalTime", Muon_energy_ecalTime, b_Muon_energy_ecalTime);
  InitVariable("Muon_energy_ecalTimeError", Muon_energy_ecalTimeError, b_Muon_energy_ecalTimeError);
  InitVariable("Muon_energy_em", Muon_energy_em, b_Muon_energy_em);
  InitVariable("Muon_energy_emMax", Muon_energy_emMax, b_Muon_energy_emMax);
  InitVariable("Muon_energy_emS25", Muon_energy_emS25, b_Muon_energy_emS25);
  InitVariable("Muon_energy_emS9", Muon_energy_emS9, b_Muon_energy_emS9);
  InitVariable("Muon_energy_hcalTime", Muon_energy_hcalTime, b_Muon_energy_hcalTime);
  InitVariable("Muon_energy_hcalTimeError", Muon_energy_hcalTimeError, b_Muon_energy_hcalTimeError);
  InitVariable("Muon_energy_had", Muon_energy_had, b_Muon_energy_had);
  InitVariable("Muon_energy_hadMax", Muon_energy_hadMax, b_Muon_energy_hadMax);
  InitVariable("Muon_energy_hadS9", Muon_energy_hadS9, b_Muon_energy_hadS9);
  InitVariable("Muon_energy_ho", Muon_energy_ho, b_Muon_energy_ho);
  InitVariable("Muon_energy_hoS9", Muon_energy_hoS9, b_Muon_energy_hoS9);
  InitVariable("Muon_energy_tower", Muon_energy_tower, b_Muon_energy_tower);
  InitVariable("Muon_energy_towerS9", Muon_energy_towerS9, b_Muon_energy_towerS9);
  InitVariable("Muon_isIsolationValid", Muon_isIsolationValid, b_Muon_isIsolationValid);
  InitVariable("Muon_isolationR03_emEt", Muon_isolationR03_emEt, b_Muon_isolationR03_emEt);
  InitVariable("Muon_isolationR03_emVetoEt", Muon_isolationR03_emVetoEt, b_Muon_isolationR03_emVetoEt);
  InitVariable("Muon_isolationR03_hadEt", Muon_isolationR03_hadEt, b_Muon_isolationR03_hadEt);
  InitVariable("Muon_isolationR03_hadVetoEt", Muon_isolationR03_hadVetoEt, b_Muon_isolationR03_hadVetoEt);
  InitVariable("Muon_isolationR03_hoEt", Muon_isolationR03_hoEt, b_Muon_isolationR03_hoEt);
  InitVariable("Muon_isolationR03_hoVetoEt", Muon_isolationR03_hoVetoEt, b_Muon_isolationR03_hoVetoEt);
  InitVariable("Muon_isolationR03_nJets", Muon_isolationR03_nJets, b_Muon_isolationR03_nJets);
  InitVariable("Muon_isolationR03_nTracks", Muon_isolationR03_nTracks, b_Muon_isolationR03_nTracks);
  InitVariable("Muon_isolationR03_sumPt", Muon_isolationR03_sumPt, b_Muon_isolationR03_sumPt);
  InitVariable("Muon_isolationR03_trackerVetoPt", Muon_isolationR03_trackerVetoPt, b_Muon_isolationR03_trackerVetoPt);
  InitVariable("Muon_isPFIsolationValid", Muon_isPFIsolationValid, b_Muon_isPFIsolationValid);
  InitVariable("Muon_pfIsolationR03_sumChargedHadronPt", Muon_pfIsolationR03_sumChargedHadronPt, b_Muon_pfIsolationR03_sumChargedHadronPt);
  InitVariable("Muon_pfIsolationR03_sumChargedParticlePt", Muon_pfIsolationR03_sumChargedParticlePt, b_Muon_pfIsolationR03_sumChargedParticlePt);
  InitVariable("Muon_pfIsolationR03_sumNeutralHadronEt", Muon_pfIsolationR03_sumNeutralHadronEt, b_Muon_pfIsolationR03_sumNeutralHadronEt);
  InitVariable("Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold", Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold, b_Muon_pfIsolationR03_sumNeutralHadronEtHighThreshold);
  InitVariable("Muon_pfIsolationR03_sumPhotonEt", Muon_pfIsolationR03_sumPhotonEt, b_Muon_pfIsolationR03_sumPhotonEt);
  InitVariable("Muon_pfIsolationR03_sumPhotonEtHighThreshold", Muon_pfIsolationR03_sumPhotonEtHighThreshold, b_Muon_pfIsolationR03_sumPhotonEtHighThreshold);
  InitVariable("Muon_pfIsolationR03_sumPUPt", Muon_pfIsolationR03_sumPUPt, b_Muon_pfIsolationR03_sumPUPt);
  InitVariable("Muon_trackIso", Muon_trackIso, b_Muon_trackIso);
  InitVariable("Muon_ecalIso", Muon_ecalIso, b_Muon_ecalIso);
  InitVariable("Muon_hcalIso", Muon_hcalIso, b_Muon_hcalIso);
  InitVariable("Muon_caloIso", Muon_caloIso, b_Muon_caloIso);
  InitVariable("Muon_shower_nStationCorrelatedHits", Muon_shower_nStationCorrelatedHits, b_Muon_shower_nStationCorrelatedHits);
  InitVariable("Muon_shower_nStationHits", Muon_shower_nStationHits, b_Muon_shower_nStationHits);
  InitVariable("Muon_shower_stationShowerDeltaR", Muon_shower_stationShowerDeltaR, b_Muon_shower_stationShowerDeltaR);
  InitVariable("Muon_shower_stationShowerSizeT", Muon_shower_stationShowerSizeT, b_Muon_shower_stationShowerSizeT);
  InitVariable("Muon_time_direction", Muon_time_direction, b_Muon_time_direction);
  InitVariable("Muon_time_ndof", Muon_time_ndof, b_Muon_time_ndof);
  InitVariable("Muon_time_timeAtIpInOut", Muon_time_timeAtIpInOut, b_Muon_time_timeAtIpInOut);
  InitVariable("Muon_time_timeAtIpInOutErr", Muon_time_timeAtIpInOutErr, b_Muon_time_timeAtIpInOutErr);
  InitVariable("Muon_time_timeAtIpOutIn", Muon_time_timeAtIpOutIn, b_Muon_time_timeAtIpOutIn);
  InitVariable("Muon_time_timeAtIpOutInErr", Muon_time_timeAtIpOutInErr, b_Muon_time_timeAtIpOutInErr);
#endif
#if NTUPLE_VERSION >= 3
  InitVariable("Muon_Mu1State_x", Muon_Mu1State_x, b_Muon_Mu1State_x);
  InitVariable("Muon_Mu1State_y", Muon_Mu1State_y, b_Muon_Mu1State_y);
  InitVariable("Muon_Mu1State_z", Muon_Mu1State_z, b_Muon_Mu1State_z);
  InitVariable("Muon_Mu2State_x", Muon_Mu2State_x, b_Muon_Mu2State_x);
  InitVariable("Muon_Mu2State_y", Muon_Mu2State_y, b_Muon_Mu2State_y);
  InitVariable("Muon_Mu2State_z", Muon_Mu2State_z, b_Muon_Mu2State_z);
#endif
#if NTUPLE_VERSION >= 3
  InitVariable("Muon_dDxDz", Muon_dDxDz, b_Muon_dDxDz);
  InitVariable("Muon_dDyDz", Muon_dDyDz, b_Muon_dDyDz);
  InitVariable("Muon_Dx", Muon_Dx, b_Muon_Dx);
  InitVariable("Muon_Dy", Muon_Dy, b_Muon_Dy);
  InitVariable("Muon_pullDxDz", Muon_pullDxDz, b_Muon_pullDxDz);
  InitVariable("Muon_pullDyDz", Muon_pullDyDz, b_Muon_pullDyDz);
  InitVariable("Muon_pullX", Muon_pullX, b_Muon_pullX);
  InitVariable("Muon_pullY", Muon_pullY, b_Muon_pullY);
  InitVariable("Muon_segmentDxDz", Muon_segmentDxDz, b_Muon_segmentDxDz);
  InitVariable("Muon_segmentDxDzError", Muon_segmentDxDzError, b_Muon_segmentDxDzError);
  InitVariable("Muon_segmentDyDz", Muon_segmentDyDz, b_Muon_segmentDyDz);
  InitVariable("Muon_segmentDyDzError", Muon_segmentDyDzError, b_Muon_segmentDyDzError);
  InitVariable("Muon_segmentX", Muon_segmentX, b_Muon_segmentX);
  InitVariable("Muon_segmentXError", Muon_segmentXError, b_Muon_segmentXError);
  InitVariable("Muon_segmentY", Muon_segmentY, b_Muon_segmentY);
  InitVariable("Muon_segmentYError", Muon_segmentYError, b_Muon_segmentYError);
  InitVariable("Muon_trackDist", Muon_trackDist, b_Muon_trackDist);
  InitVariable("Muon_trackDistError", Muon_trackDistError, b_Muon_trackDistError);
  InitVariable("Muon_trackDxDz", Muon_trackDxDz, b_Muon_trackDxDz);
  InitVariable("Muon_trackDxDzError", Muon_trackDxDzError, b_Muon_trackDxDzError);
  InitVariable("Muon_trackDyDz", Muon_trackDyDz, b_Muon_trackDyDz);
  InitVariable("Muon_trackDyDzError", Muon_trackDyDzError, b_Muon_trackDyDzError);
  InitVariable("Muon_trackEdgeX", Muon_trackEdgeX, b_Muon_trackEdgeX);
  InitVariable("Muon_trackEdgeY", Muon_trackEdgeY, b_Muon_trackEdgeY);
  InitVariable("Muon_trackX", Muon_trackX, b_Muon_trackX);
  InitVariable("Muon_trackXError", Muon_trackXError, b_Muon_trackXError);
  InitVariable("Muon_trackY", Muon_trackY, b_Muon_trackY);
  InitVariable("Muon_trackYError", Muon_trackYError, b_Muon_trackYError);
  InitVariable("Muon_stationGapMaskDistance", Muon_stationGapMaskDistance, b_Muon_stationGapMaskDistance);
  InitVariable("Muon_stationGapMaskPull", Muon_stationGapMaskPull, b_Muon_stationGapMaskPull);
#endif
  
  CheckVariable("numberOfMuon", hasMuon);
  
  InitVariable("Muon_vDiMuVtxMuonref", Muon_vDiMuVtxMuonref, b_Muon_vDiMuVtxMuonref);
  InitVariable("Muon_vDiMuVtxX", Muon_vDiMuVtxX, b_Muon_vDiMuVtxX);
  InitVariable("Muon_vDiMuVtxY", Muon_vDiMuVtxY, b_Muon_vDiMuVtxY);
  InitVariable("Muon_vDiMuVtxZ", Muon_vDiMuVtxZ, b_Muon_vDiMuVtxZ);
  InitVariable("Muon_vDiMuVtxProb", Muon_vDiMuVtxProb, b_Muon_vDiMuVtxProb);
  InitVariable("Muon_vDiMuVtxCTau", Muon_vDiMuVtxCTau, b_Muon_vDiMuVtxCTau);
  InitVariable("Muon_vDiMuVtxCTauError", Muon_vDiMuVtxCTauError, b_Muon_vDiMuVtxCTauError);
  CheckVariable("Muon_vDiMuVtxMuonref", hasDiMuVtx);
#if NTUPLE_VERSION >= 3
  InitVariable("Muon_vDiMuVtxPVRefitRef", Muon_vDiMuVtxPVRefitRef, b_Muon_vDiMuVtxPVRefitRef);
#endif
  InitVariable("Muon_trackref", Muon_trackref, b_Muon_trackref);
  InitVariable("Muon_hasTriggered", Muon_hasTriggered, b_Muon_hasTriggered);
  InitVariable("Muon_hasTriggeredMu3", Muon_hasTriggeredMu3, b_Muon_hasTriggeredMu3);
#if NTUPLE_VERSION >= 4
  InitVariable("Muon_triggerInfo", Muon_triggerInfo, b_Muon_triggerInfo);
#endif
  Notify();
}

#endif // TreeDefinitionRecoV2_h
