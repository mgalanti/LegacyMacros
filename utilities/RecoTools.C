#ifndef RecoTools_cxx
#define RecoTools_cxx

#include <vector>
#include <iostream>

#include "TVector3.h"

#include "RecoTools.h"



// #define mgDEBUG



short RecoTools::FindJetNearToMuon(const unsigned short iMuon, const double deltaRThreshold, const std::string jetType)
{
  TVector3 pMuon, pJet;
  pMuon.SetPtEtaPhi(Muon_pt->at(iMuon), Muon_eta->at(iMuon), Muon_phi->at(iMuon));
  short nearestJetIndex = -1;
  double maxDeltaR = 99999;
  double deltaR = 99999;
  if(jetType == "PF")
  {
    for(int iJet = 0; iJet < numberOfJet; iJet++)
    {
      pJet.SetPtEtaPhi(Jet_pt->at(iJet), Jet_eta->at(iJet), Jet_phi->at(iJet));
      deltaR = pMuon.DeltaR(pJet);
      if(deltaR < maxDeltaR && deltaR < deltaRThreshold)
      {
        nearestJetIndex = iJet;
        maxDeltaR = deltaR;
      }
    }
  }
  return nearestJetIndex;
}



double RecoTools::GetTrackValidFraction(const unsigned short iTrack)
{
  int valid = GetNumberOfValidTrackerHits(iTrack);
  int lost  = GetNumberOfLostTrackerHits(iTrack);
  int lostIn = GetNumberOfLostTrackerInnerHits(iTrack);
  int lostOut = GetNumberOfLostTrackerOuterHits(iTrack);
  if ((valid+lost+lostIn+lostOut)==0)
    return -1;
  return valid/(1.0*(valid+lost+lostIn+lostOut));
}



int RecoTools::GetNumberOfValidTrackerHits(const unsigned short iTrack)
{
  int count = 0;
  unsigned int size = Track_hitPattern->at(iTrack).size();
  for (unsigned int i = 0; i < size; i++)
  {
    unsigned int pattern = Track_hitPattern->at(iTrack).at(i);
    if (pattern == 0) break;
    if (ValidHitFilter(pattern) && TrackerHitFilter(pattern)) ++count;
  }
  return count;
}



int RecoTools::GetNumberOfValidTrackerInnerHits(const unsigned short iTrack)
{
  int count = 0;
  unsigned int size = Track_expInnHitPattern->at(iTrack).size();
  for (unsigned int i = 0; i < size; i++)
  {
    unsigned int pattern = Track_expInnHitPattern->at(iTrack).at(i);
    if (pattern == 0) break;
    if (ValidHitFilter(pattern) && TrackerHitFilter(pattern)) ++count;
  }
  return count;
}



int RecoTools::GetNumberOfValidTrackerOuterHits(const unsigned short iTrack)
{
  int count = 0;
  unsigned int size = Track_expOutHitPattern->at(iTrack).size();
  for (unsigned int i = 0; i < size; i++)
  {
    unsigned int pattern = Track_expOutHitPattern->at(iTrack).at(i);
    if (pattern == 0) break;
    if (ValidHitFilter(pattern) && TrackerHitFilter(pattern)) ++count;
  }
  return count;
}



int RecoTools::GetNumberOfLostTrackerHits(const unsigned short iTrack)
{
  int count = 0;
  unsigned int size = Track_hitPattern->at(iTrack).size();
  for (unsigned int i = 0; i < size; i++)
  {
    unsigned int pattern = Track_hitPattern->at(iTrack).at(i);
    if (pattern == 0) break;
    if (Type1HitFilter(pattern) && TrackerHitFilter(pattern)) ++count;
  }
  return count;
}



int RecoTools::GetNumberOfLostTrackerInnerHits(const unsigned short iTrack)
{
  int count = 0;
  unsigned int size = Track_expInnHitPattern->at(iTrack).size();
  for (unsigned int i = 0; i < size; i++)
  {
    unsigned int pattern = Track_expInnHitPattern->at(iTrack).at(i);
    if (pattern == 0) break;
    if (Type1HitFilter(pattern) && TrackerHitFilter(pattern)) ++count;
  }
  return count;
}



int RecoTools::GetNumberOfLostTrackerOuterHits(const unsigned short iTrack)
{
  int count = 0;
  unsigned int size = Track_expOutHitPattern->at(iTrack).size();
  for (unsigned int i = 0; i < size; i++)
  {
    unsigned int pattern = Track_expOutHitPattern->at(iTrack).at(i);
    if (pattern == 0) break;
    if (Type1HitFilter(pattern) && TrackerHitFilter(pattern)) ++count;
  }
  return count;
}



int RecoTools::CountTrackTypedHits(const std::vector<unsigned short>& hitPattern, const hitFilterType typeFilter)
{
  int count = 0;
  unsigned int size = hitPattern.size();
  for (unsigned int i = 0; i < size; i++)
  {
    unsigned int pattern = hitPattern.at(i);
    if (pattern == 0) break;
    if (typeFilter(pattern) && TrackerHitFilter(pattern)) ++count;
  }
  return count;
}



unsigned int RecoTools::GetTrackHitType(const unsigned int hitPattern) const
{
  if(hitPattern == 0)
  {
    return 999999;
  } 
  return ((hitPattern>>HitTypeOffset) & HitTypeMask);
}



inline bool RecoTools::ValidHitFilter(const unsigned int hitPattern)
{
  if (GetTrackHitType(hitPattern) == 0) return true; 
  return false;
}



inline bool RecoTools::Type1HitFilter(const unsigned int hitPattern)
{
  if (GetTrackHitType(hitPattern) == 1) return true; 
  return false;
}



inline bool RecoTools::Type2HitFilter(const unsigned int hitPattern)
{
  if (GetTrackHitType(hitPattern) == 2) return true; 
  return false;
}



inline bool RecoTools::Type3HitFilter(const unsigned int hitPattern)
{
  if (GetTrackHitType(hitPattern) == 3) return true; 
  return false;
}



inline bool RecoTools::TrackerHitFilter(const unsigned int hitPattern)
{
  if(hitPattern == 0)
  {
    return false;
  }  
  if(((hitPattern>>HitSubDetectorOffset) & HitSubDetectorMask) == 1)
  {
    return true;
  }
  return false;
}

#endif // RecoTools_cxx
