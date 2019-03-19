#ifndef RecoTools_h
#define RecoTools_h

#include "TreeDefinitionRecoV2.h"



class RecoTools : public virtual TreeDefinitionRecoV2
{
  public:
    typedef bool hitFilterType(const unsigned int);
    
    virtual ~RecoTools() {};
    inline short FindJetNearToMuon(const unsigned short iMuon, const double deltaRThreshold, const std::string jetType = "PF");
    inline double GetTrackValidFraction(const unsigned short iTrack);
    inline int GetNumberOfValidTrackerHits(const unsigned short iTrack);
    inline int GetNumberOfValidTrackerInnerHits(const unsigned short iTrack);
    inline int GetNumberOfValidTrackerOuterHits(const unsigned short iTrack);
    inline int GetNumberOfLostTrackerHits(const unsigned short iTrack);
    inline int GetNumberOfLostTrackerInnerHits(const unsigned short iTrack);
    inline int GetNumberOfLostTrackerOuterHits(const unsigned short iTrack);
    inline int CountTrackTypedHits(const std::vector<unsigned short>& hitPattern, const hitFilterType typeFilter);
    inline unsigned int GetTrackHitType(const unsigned int hitPattern) const;
    inline bool ValidHitFilter(const unsigned int hitPattern);
    inline bool Type1HitFilter(const unsigned int hitPattern);
    inline bool Type2HitFilter(const unsigned int hitPattern);
    inline bool Type3HitFilter(const unsigned int hitPattern);
    inline bool TrackerHitFilter(const unsigned int hitPattern);
  private:
    static const unsigned short HitSize = 11;
    static const unsigned short HitTypeMask = 0x3;
    static const unsigned short HitTypeOffset = 0;
    static const unsigned short HitLayerMask = 0xF;
    static const unsigned short HitLayerOffset = 3;
    static const unsigned short HitPatternSize = 25;
    static const unsigned short HitSideMask = 0x1;
    static const unsigned short HitSideOffset = 2;
    static const unsigned short HitSubDetectorMask = 0x1;
    static const unsigned short HitSubDetectorOffset = 10;
    static const unsigned short HitSubstrMask = 0x7;
    static const unsigned short HitSubstrOffset = 7;
};

#endif // RecoTools_h
