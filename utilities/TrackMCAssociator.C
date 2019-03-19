#ifndef TrackMCAssociator_cxx
#define TrackMCAssociator_cxx

#include "TrackMCAssociator.h"



// #define mgDEBUG



bool TrackMCAssociator::AssociateTrack(const unsigned short int iTrack)
{
  if(iTrack >= numberOfTrack)
    return false;
  trkGenp = 65535;
  trkAssocQuality = 0;
  trkGenpId = 0;
  trkGenpSign = 0;
  if(Track_vGenprefByHits->at(iTrack).size() > 0)
  {
    trkGenp = Track_vGenprefByHits->at(iTrack).at(0);
    trkAssocQuality = Track_vGenpQualityByHits->at(iTrack).at(0);
    trkGenpId = Genp_Id->at(trkGenp);
    trkGenpId>0?trkGenpSign=1:trkGenpSign=-1;
  }
#ifdef mgDEBUG
  std::cout << "Track " << iTrack << " is associated to genParticle " << trkGenp << " with quality " << trkAssocQuality << std::endl;
  std::cout << "\tR: pt = " << Track_pt->at(iTrack) << ", eta = " << Track_eta->at(iTrack) << ", phi = " << Track_phi->at(iTrack) << std::endl;
  if(trkGenp != 65535)
    std::cout << "\tG: pt = " << Genp_pt->at(trkGenp) << ", eta = " << Genp_eta->at(trkGenp) << ", phi = " << Genp_phi->at(trkGenp) << std::endl;
  std::cout << "\tgenp Id = " << trkGenpId << std::endl;
#endif // mgDEBUG
  return true;
}



#endif // TrackMCAssociator_cxx
