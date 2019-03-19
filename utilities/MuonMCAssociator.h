#ifndef MuonMCAssociator_h
#define MuonMCAssociator_h

#include <TLorentzVector.h>

#include "constants.h"
#include "MCTools.h"
#include "TreeDefinitionRecoV2.h"




class MuonMCAssociator : public virtual MCTools, public virtual TreeDefinitionRecoV2
{
  public:
    bool AssociateInnerMuonTrack(const unsigned short int iMuon);
    bool AssociateOuterMuonTrack(const unsigned short int iMuon);
    bool AssociateGlobalMuonTrack(const unsigned short int iMuon);
    bool AssociateInnerMuonTrackNoByHits(const unsigned short int iMuon);
    short int FindMuonClassFromMCAssociation(const unsigned short iMuon);
    short int FindMuonClassFromMCAssociationNoByHits(const unsigned short iMuon);
    short int FindMuonMCProvenance(const unsigned short iMuon, const short int muonClass);
    short int FindGenMuonMCProvenance(const unsigned short iGenMuon);
    short int FindMuonMCCode(const unsigned short iMuon);
    short int MergeMuonClassAndMCProvenance(const short int muonClass, const short int muonMCProvenance);
    short int FindSimplifiedMuonCode(const short int muonCode);
    short int FindSimplifiedMergedMuonCode(const short int muonSimplifiedCode);
    short int FindSimplifiedMuonPtRelCode(const short int muonCode);
    short int FindSimplifiedMergedMuonPtRelCodeV0(const short int muonSimplifiedCode);
    short int FindSimplifiedMergedMuonPtRelCodeV1(const short int muonSimplifiedCode);
    short int FindSimplifiedMergedMuonPtRelCodeV2(const short int muonSimplifiedCode);
    short int FindDIFMuonProvenance(const unsigned short iMuon);
    short int FindMuonFlavour(const short int muonCode);
    bool SelectSignalEvent(const std::vector<unsigned short>* vGenMuons,
                           std::vector<std::pair<unsigned short, unsigned short> >& vGoodMuonPairs,
                           std::vector<std::vector<unsigned short> >& vvBQuarks);
    bool SelectSingleMuEvent(const std::vector<unsigned short>* vGenMuons,
                             std::vector<unsigned short>& vGoodGenMuons);
    std::pair<short int, double> FindTrackNearestToGenParticle(const unsigned short int iGenParticle, const bool selHighPurity=false);
    std::pair<short int, double> FindTrackNearestToGenParticleWithPtThreshold(const unsigned short int iGenParticle, const double ptTrackMin, const bool selHighPurity=false);
    std::pair<short int, double> FindTrackAssociatedToGenParticleByDRDPtLessByDR(const unsigned short int iGenParticle, const double deltaRMax, const double deltaPtOverPtMax, const bool selHighPurity=false);
    std::pair<short int, double> FindGenParticleNearestToTrack(const unsigned short int iTrack);
    std::pair<short int, double> FindGenParticleNearestToTrackWithPtThreshold(const unsigned short int iTrack, const double ptGenParticleMin);
    std::pair<short int, double> FindGenParticleAssociatedToTrackByDRDPtLessByDR(const unsigned short int iTrack, const double deltaRMax, const double deltaPtOverPtMax);
    int FindFakeClassification(const unsigned int iGenp);
#if NTUPLE_VERSION >= 4

    std::pair<short int, double> FindL1TTTrackNearestToGenParticle(const unsigned short int iGenParticle, bool use5ParFit=true);
#endif
    
  protected:
    unsigned int muonInTrkGenp;
    double muonInTrkAssocQuality;
    int muonInTrkGenpId;
    short int muonInTrkGenpSign;
    unsigned int muonOutTrkGenp;
    double muonOutTrkAssocQuality;
    int muonOutTrkGenpId;
    short int muonOutTrkGenpSign;
    unsigned int muonGlbTrkGenp;
    double muonGlbTrkAssocQuality;
    int muonGlbTrkGenpId;
    short int muonGlbTrkGenpSign;
};

#endif // MuonMCAssociator_h


