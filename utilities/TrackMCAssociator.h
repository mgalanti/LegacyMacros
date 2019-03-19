#ifndef TrackMCAssociator_h
#define TrackMCAssociator_h

#include <TLorentzVector.h>

#include "constants.h"
#include "MCTools.h"
#include "TreeDefinitionRecoV2.h"



class TrackMCAssociator : public virtual MCTools, public virtual TreeDefinitionRecoV2
{
  public:
    bool AssociateTrack(const unsigned short int iTrack);
    
  private:
    unsigned int trkGenp;
    double trkAssocQuality;
    int trkGenpId;
    short int trkGenpSign;
};



#endif // TrackMCAssociator_h
