#ifndef MuonMCAssociator_cxx
#define MuonMCAssociator_cxx

#include "MuonMCAssociator.h"



// #define mgDEBUG



bool MuonMCAssociator::AssociateInnerMuonTrack(const unsigned short int iMuon)
{
  if(iMuon >= numberOfMuon)
    return false;

  muonInTrkGenp = 65535;
  muonInTrkAssocQuality = 0;
  muonInTrkGenpId = 0;
  muonInTrkGenpSign = 0;

  if(Muon_vInnerGenprefByHits->at(iMuon).size() > 0)
  {
    muonInTrkGenp = Muon_vInnerGenprefByHits->at(iMuon).at(0);
    muonInTrkAssocQuality = Muon_vInnerGenpQualityByHits->at(iMuon).at(0);
    muonInTrkGenpId = Genp_Id->at(muonInTrkGenp);
    muonInTrkGenpId>0?muonInTrkGenpSign=1:muonInTrkGenpSign=-1;
  }

#ifdef mgDEBUG
  std::cout << "muon " << iMuon << " inner track is associated to genParticle " << muonInTrkGenp << " with quality " << muonInTrkAssocQuality << std::endl;
  std::cout << "\tR: pt = " << Muon_pt->at(iMuon) << ", eta = " << Muon_eta->at(iMuon) << ", phi = " << Muon_phi->at(iMuon) << std::endl;
  if(muonInTrkGenp != 65535)
    std::cout << "\tG: pt = " << Genp_pt->at(muonInTrkGenp) << ", eta = " << Genp_eta->at(muonInTrkGenp) << ", phi = " << Genp_phi->at(muonInTrkGenp) << std::endl;
  std::cout << "\tgenp Id = " << muonInTrkGenpId << std::endl;
#endif // mgDEBUG

  return true;
}



bool MuonMCAssociator::AssociateOuterMuonTrack(const unsigned short int iMuon)
{
  if(iMuon >= numberOfMuon)
    return false;

  muonOutTrkGenp = 65535;
  muonOutTrkAssocQuality = 0;
  muonOutTrkGenpId = 0;
  muonOutTrkGenpSign = 0;

  if(Muon_vOuterGenprefByHits->at(iMuon).size() > 0)
  {
    muonOutTrkGenp = Muon_vOuterGenprefByHits->at(iMuon).at(0);
    muonOutTrkAssocQuality = Muon_vOuterGenpQualityByHits->at(iMuon).at(0);
    muonOutTrkGenpId = Genp_Id->at(muonOutTrkGenp);
    muonOutTrkGenpId>0?muonOutTrkGenpSign=1:muonOutTrkGenpSign=-1;
  }

#ifdef mgDEBUG
  std::cout << "muon " << iMuon << " outer track is associated to genParticle " << muonOutTrkGenp << " with quality " << muonOutTrkAssocQuality << std::endl;
  std::cout << "\tR: pt = " << Muon_pt->at(iMuon) << ", eta = " << Muon_eta->at(iMuon) << ", phi = " << Muon_phi->at(iMuon) << std::endl;
  if(muonOutTrkGenp != 65535)
    std::cout << "\tG: pt = " << Genp_pt->at(muonOutTrkGenp) << ", eta = " << Genp_eta->at(muonOutTrkGenp) << ", phi = " << Genp_phi->at(muonOutTrkGenp) << std::endl;
  std::cout << "\tgenp Id = " << muonOutTrkGenpId << std::endl;
#endif // mgDEBUG

  return true;
}



bool MuonMCAssociator::AssociateGlobalMuonTrack(const unsigned short int iMuon)
{
  if(iMuon >= numberOfMuon)
    return false;

  muonGlbTrkGenp = 65535;
  muonGlbTrkAssocQuality = 0;
  muonGlbTrkGenpId = 0;
  muonGlbTrkGenpSign = 0;

  if(Muon_vGlobalGenprefByHits->at(iMuon).size() > 0)
  {
    muonGlbTrkGenp = Muon_vGlobalGenprefByHits->at(iMuon).at(0);
    muonGlbTrkAssocQuality = Muon_vGlobalGenpQualityByHits->at(iMuon).at(0);
    muonGlbTrkGenpId = Genp_Id->at(muonGlbTrkGenp);
    muonGlbTrkGenpId>0?muonGlbTrkGenpSign=1:muonGlbTrkGenpSign=-1;
  }

#ifdef mgDEBUG
  std::cout << "muon " << iMuon << " global track is associated to genParticle " << muonGlbTrkGenp << " with quality " << muonGlbTrkAssocQuality << std::endl;
  std::cout << "\tR: pt = " << Muon_pt->at(iMuon) << ", eta = " << Muon_eta->at(iMuon) << ", phi = " << Muon_phi->at(iMuon) << std::endl;
  if(muonGlbTrkGenp != 65535)
    std::cout << "\tG: pt = " << Genp_pt->at(muonGlbTrkGenp) << ", eta = " << Genp_eta->at(muonGlbTrkGenp) << ", phi = " << Genp_phi->at(muonGlbTrkGenp) << std::endl;
  std::cout << "\tgenp Id = " << muonGlbTrkGenpId << std::endl;
#endif // mgDEBUG

  return true;
}



bool MuonMCAssociator::AssociateInnerMuonTrackNoByHits(const unsigned short int iMuon)
{
  if(iMuon >= numberOfMuon)
    return false;

  muonInTrkGenp = 65535;
  muonInTrkAssocQuality = 0;
  muonInTrkGenpId = 0;
  muonInTrkGenpSign = 0;
    
  // Looking for association of inner, outer and global track - take only the best match
  unsigned short iTrack = Muon_trackref->at(iMuon);
  if(iTrack == 65535)
  {
    // No inner track - association is "succeeded"
    // Calling function will figure out that associated particle does not exist
    return true;
  }
  
  unsigned short iGenp = Genp_idx_clst->at(iTrack);
  if(iGenp == 65535)
  {
    // No genp associated to inner track - again association is "succeeded"
    // Calling function will figure out that associated particle does not exist
    return true;
  }
  
  muonInTrkGenp = iGenp;
  muonInTrkGenpId = Genp_Id->at(iGenp);
  muonInTrkGenpSign = muonInTrkGenpId>0?1:-1;
  
#ifdef mgDEBUG
  std::cout << "muon " << iMuon << " inner track is associated by deltaR to genParticle " << muonInTrkGenp << std::endl;
  std::cout << "\tR: pt = " << Muon_pt->at(iMuon) << ", eta = " << Muon_eta->at(iMuon) << ", phi = " << Muon_phi->at(iMuon) << std::endl;
  if(muonInTrkGenp != 65535)
    std::cout << "\tG: pt = " << Genp_pt->at(muonInTrkGenp) << ", eta = " << Genp_eta->at(muonInTrkGenp) << ", phi = " << Genp_phi->at(muonInTrkGenp) << std::endl;
  std::cout << "\tgenp Id = " << muonInTrkGenpId << std::endl;
#endif // mgDEBUG

  return true;
}



short int MuonMCAssociator::FindMuonClassFromMCAssociation(unsigned short iMuon)
{
  // Looking for association of inner, outer and global track - take only the best match
  bool isInnerAssociated = AssociateInnerMuonTrack(iMuon);
  if(!isInnerAssociated)
  {
    std::cout << "E R R O R! Inner track association failed!\n";
    exit(1);
  }
  bool isOuterAssociated = AssociateOuterMuonTrack(iMuon);
  if(!isOuterAssociated)
  {
    std::cout << "E R R O R! Outer track association failed!\n";
    exit(1);
  }
  bool isGlobalAssociated = AssociateGlobalMuonTrack(iMuon); // Even if the bool is not used HERE, it sets some aux variables which are used elsewhere!
  if(!isGlobalAssociated)
  {
    std::cout << "E R R O R! Global track association failed!\n";
    exit(1);
  }

  // Several cases:
  // In and out are both 13 -> Look at gen information
  // In != 13 and out == 13 -> Possible DIF (from charged)
  // In == 0 and out == 13 -> Possible DIF (from neutral)
  // In != 13 and out != 13-> Possible PT
  // In == 0 and out == 0 -> Possible fake
  // In != 13 and out == 0 -> Fake or PT
  // Single-mu classification:
  // 0 = Fake
  // +/- 1 = real muon
  // +/- 2 = charge flip (positive if outer is muon, negative if antimuon)
  // +/- 3 = DIF from charged
  // +/- 4 = DIF from neutral
  // 5 = PT
  // 6 = Fake or PT
  // +/- 7 = Mis-reconstructed (inner and outer stubs point to unrelated gen particles)
  // +/- 8 = Inner track points to a real muon and outer track is not associated

  // Both inner and outer track (or inner and global track) are associated to a gen muon
  // This is a real muon inside the detector (from short-range DIF or prompt sources)
  // Case Inner==Global is currently commented because Global info is correlated with
  // Inner info: using Inner==Outer is cleaner and more significant
  if( (muonInTrkGenpId == muonOutTrkGenpId && abs(muonInTrkGenpId) == 13)/* ||
    (muonInTrkGenpId == muonGlbTrkGenpId && abs(muonInTrkGenpId) == 13)*/ )
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " Real muon!\n";
#endif // mgDEBUG
    return(1*muonOutTrkGenpSign); // NOTE: ...GenpSign is NOT charge: 1 for MU-, -1 for MU+
  }
  else if(abs(muonInTrkGenpId) == 13 && muonOutTrkGenpId == 0)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " Inner track only!\n";
    std::cout << "See if muon has some daughters:\n";
    RecursivePrintDaughters(muonInTrkGenp);
#endif // mgDEBUG
    return(8*muonInTrkGenpSign);
  }
  // Should not happen, but who knows...
  // Inner is muon and outer antimuon (or vice-versa)
  else if(muonInTrkGenpId == -muonOutTrkGenpId && abs(muonInTrkGenpId) == 13)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " Charge flip!\n";
#endif // mgDEBUG
    return(2*muonOutTrkGenpSign);
  }
  // Outer is a muon and inner is not a muon: can be DIF inside the detector volume
  else if(abs(muonInTrkGenpId) != 13 && muonInTrkGenpId != 0 && abs(muonOutTrkGenpId) == 13)
  {
    // See if genmuon associated to outer track is among the descendants of inner particle
    bool isDIF = RecursiveLookForDaughter(muonInTrkGenp,muonOutTrkGenp);
    // If it is not...
    if(!isDIF)
    {
      // ...Try the other way: Look for inner gen particle among mothers of muon
      isDIF = RecursiveLookForMother(muonOutTrkGenp,muonInTrkGenp);
      // If, again, nothing is found...
      if(!isDIF)
      {
        // ... then it means that the inner and outer track belong to unrelated
        // (i.e. no mother-child) gen particles: assign reco muon to "misreconstructed" class
#ifdef mgDEBUG
        std::cout << "Muon " << iMuon << " Mis-reconstructed muon!\n";
#endif // mgDEBUG
        return(7*muonOutTrkGenpSign);
      }
      else
      {
        std::cout << "E R R O R! It is either a DIF and not a DIF! Impossible!\n";
        exit(1);
      }
    }
    // If some mother-daughter relation was found, then isDIF should be true now
    if(isDIF)
    {
      // Assign reco muon to "DIF from charged particle" class
#ifdef mgDEBUG
      std::cout << "Muon " << iMuon << " DIF from charged!\n";
#endif // mgDEBUG
//       return(3*muonOutTrkGenpSign);
      return(FindGenMuonMCProvenance(muonOutTrkGenp));
    }
  }
  // Inner track is not associated to any genparticle (this is weird per se)
  else if(muonInTrkGenpId == 0 && abs(muonOutTrkGenpId) == 13)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " Possible DIF from neutral!\n";
    RecursivePrintMothers(muonOutTrkGenp);
    // First see if inner track can be associated with other ways to a muon
    // DeltaR
    unsigned short iTrack = Muon_trackref->at(iMuon);
    if(iTrack != 65535)
    {
      unsigned short iGenpDeltaR = Genp_idx_clst->at(iTrack);
      std::cout << "Genp associated by deltaR = (" << iGenpDeltaR << ")" << Genp_Id->at(iGenpDeltaR) << std::endl;
      std::cout << "Production vertex = (" << Genp_vx->at(iGenpDeltaR) << "," << Genp_vy->at(iGenpDeltaR) << "," << Genp_vz->at(iGenpDeltaR) << ")\n";
    }
#endif // mgDEBUG
//     return(4*muonOutTrkGenpSign);
    return(FindGenMuonMCProvenance(muonOutTrkGenp));
  }
  else if(abs(muonInTrkGenpId) != 13 && muonInTrkGenpId != 0 && abs(muonOutTrkGenpId) != 13 && muonOutTrkGenpId != 0)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " Possible PT!\n";
#endif // mgDEBUG
    // First see if inner track can be associated with other ways to a muon
    // DeltaR
    unsigned short iTrack = Muon_trackref->at(iMuon);
    unsigned short iGenpDeltaR = Genp_idx_clst->at(iTrack);
//     short iGenpDeltaRSign = Genp_Id->at(iGenpDeltaR)>0?1:-1;
#ifdef mgDEBUG
    std::cout << "Genp associated by deltaR = (" << iGenpDeltaR << ")" << Genp_Id->at(iGenpDeltaR) << std::endl;
    std::cout << "Production vertex = (" << Genp_vx->at(iGenpDeltaR) << "," << Genp_vy->at(iGenpDeltaR) << "," << Genp_vz->at(iGenpDeltaR) << ")\n";
#endif // mgDEBUG
    bool isDIF = false;
    if(abs(Genp_Id->at(iGenpDeltaR) == 13))
    {
#ifdef mgDEBUG
      std::cout << "Association by hits gives PT but association by deltaR gives real muon!\n";
      std::cout << "Check if it is a DIF!\n";
#endif // mgDEBUG
      isDIF = RecursiveLookForDaughter(muonInTrkGenp,iGenpDeltaR);
      if(isDIF)
      {
#ifdef mgDEBUG
        std::cout << "Muon " << iMuon <<  " is a decay-in-flight\n";
#endif // mgDEBUG
//         return(3*iGenpDeltaRSign);
        return(FindGenMuonMCProvenance(iGenpDeltaR));
      }
    }
//     // Chi2
//     for(unsigned int iAssoc = 0; iAssoc < Track_vGenprefByChi2->at(iTrack).size(); iAssoc++)
//     {
//       unsigned short iGenpChi2 = Track_vGenprefByChi2->at(iTrack).at(iAssoc);
//       std::cout << "Genp " << iAssoc << " associated by chi2 = (" << iGenpChi2 << ")" << Genp_Id->at(iGenpChi2) << std::endl;
//       std::cout << "Production vertex = (" << Genp_vx->at(iGenpChi2) << "," << Genp_vy->at(iGenpChi2) << "," << Genp_vz->at(iGenpChi2) << ")\n";
//       if(abs(Genp_Id->at(iGenpChi2) == 13))
//       {
//         std::cout << "E R R O R! Association by hits gives PT but association by chi2 gives real muon!\n";
//         exit(1);
//       }
//     }
    return(5*muonOutTrkGenpSign);
  }
  else if(abs(muonInTrkGenpId) != 13 && muonInTrkGenpId != 0 && muonOutTrkGenpId == 0)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " Possible fake or PT!\n";
#endif // mgDEBUG
    // DeltaR
    bool isDIF = false;
    bool isMuon = false;
    unsigned short iTrack = Muon_trackref->at(iMuon);
    unsigned short iGenpDeltaR = Genp_idx_clst->at(iTrack);
#ifdef mgDEBUG
    std::cout << "Genp associated by deltaR = (" << iGenpDeltaR << ")" << Genp_Id->at(iGenpDeltaR) << ", deltaR = " << DeltaR_clst->at(iTrack) << ", deltaPt = " << DeltaPt_clst->at(iTrack) << std::endl;
    std::cout << "Production vertex = (" << Genp_vx->at(iGenpDeltaR) << "," << Genp_vy->at(iGenpDeltaR) << "," << Genp_vz->at(iGenpDeltaR) << ")\n";
#endif // mgDEBUG
    if(abs(Genp_Id->at(iGenpDeltaR)) == 13)
    {
      isMuon = true;
      isDIF = RecursiveLookForMother(iGenpDeltaR,muonInTrkGenp);
#ifdef mgDEBUG
      if(isDIF)
        std::cout << "This is really a DIF.\n";
#endif // mgDEBUG
    }
    if(isMuon && isDIF)
    {
//       short int genpDeltaRSign = Genp_Id->at(iGenpDeltaR)>0?1:-1;
//       return(3*genpDeltaRSign);
      return(FindGenMuonMCProvenance(iGenpDeltaR));
    }
    if(!isMuon)
    {
      return(6*muonInTrkGenpSign);
    }
    if(isMuon && !isDIF)
    {
#ifdef mgDEBUG
      std::cout << "This is really a mis-reconstructed muon:\nAssociation by hits gives PT but association by deltaR gives real muon, and it is not a DIF!\n";
#endif // mgDEBUG
      return(7*muonInTrkGenpSign);
    }
  }
  else if(abs(muonInTrkGenpId) == 13 && abs(muonOutTrkGenpId) == 11)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " outer associated to electron!\n";
    RecursivePrintDaughters(muonInTrkGenp);
    std::cout << "This is put into the \"Inner track only\" class.\n";
#endif // mgDEBUG
    return(8*muonInTrkGenpSign);
  }
  else if(muonInTrkGenpId == 0 && muonOutTrkGenpId == 0)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " Possible ghost!\n";
#endif // mgDEBUG
    return(0);
  }
  else if(muonInTrkGenpId == 0 && abs(muonOutTrkGenpId) == 11)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " inner not associated and outer associated to an electron.\n";
#endif // mgDEBUG
    unsigned short muonI = LookForMotherId(muonOutTrkGenp,13,false);
    if(muonI != 65535)
    {
#ifdef mgDEBUG
      std::cout << "This is really a muon decayed to an electron, coming from a DIF in the tracker volume\n";
#endif // mgDEBUG
//       short int muonSign = Genp_Id->at(muonI)>0?1:-1;
//       return(4*muonSign);
      return(FindGenMuonMCProvenance(muonI));
    }
  }
  else if(muonInTrkGenpId == 0 && abs(muonOutTrkGenpId) != 11 && abs(muonOutTrkGenpId) != 13)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " inner not associated and outer associated to a particle that is not a muon or an electron.\n";
    std::cout << "This is a PT.\n";
#endif // mgDEBUG
    short int muonSign = Genp_Id->at(muonOutTrkGenp)>0?1:-1;
    return(5*muonSign);
  }
  else if(abs(muonInTrkGenpId) == 13 && abs(muonOutTrkGenpId) != 11 && abs(muonOutTrkGenpId) != 13)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " inner associated to a muon and outer associated to a particle that is not a muon or an electron.\n";
    std::cout << "This is a mis-reconstructed muon.\n";
#endif // mgDEBUG
    return(7*muonInTrkGenpSign);
  }
  std::cout << "E R R O R! We should never be here!\n";
  std::cout << "muonInTrkGenp = " << muonInTrkGenp << std::endl;
  if(muonInTrkGenp != 65535)
  {
  std::cout << "Printing mothers of genp associated to inner track:\n";
  RecursivePrintMothers(muonInTrkGenp);
  }
  std::cout << "muonOutTrkGenp = " << muonOutTrkGenp << std::endl;
  if(muonOutTrkGenp != 65535)
  {
    std::cout << "Printing mothers of genp associated to outer track:\n";
    RecursivePrintMothers(muonOutTrkGenp);
  }
  std::cout << "muonGlbTrkGenp = " << muonGlbTrkGenp << std::endl;
  if(muonGlbTrkGenp != 65535)
  {
    std::cout << "Printing mothers of genp associated to global track:\n";
    RecursivePrintMothers(muonGlbTrkGenp);
  }
  exit(1);
}



short int MuonMCAssociator::FindMuonClassFromMCAssociationNoByHits(unsigned short iMuon)
{
  // Very limited functionality - use with care!
  // For serious associations, use the method MuonMCAssociator::FindMuonClassFromMCAssociation(iMuon)!!!
  
  AssociateInnerMuonTrackNoByHits(iMuon);
  
  if(muonInTrkGenp == 0)
  {
    return 6;
  }
  
  // Inner track associated to a gen muon
  // This is a real muon inside the detector
  if(abs(muonInTrkGenpId) == 13)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " Real muon!\n";
#endif // mgDEBUG
    return(1*muonInTrkGenpSign);
  }
  else if(abs(muonInTrkGenpId) != 13)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " is probably a PT!\n";
#endif // mgDEBUG
    return(5*muonInTrkGenpSign);
  }
  
  // Several cases:
  // No Inner track -> Possible fake or PT
  // Inner exists but associated genp == 0 -> Possible fake or PT
  // In 13 -> Look at gen information
  // In != 13 -> Possible PT
  // Single-mu classification:
  // +/- 1 = real muon
  // 5 = PT
  // 6 = Fake or PT
  
  // This is an unmanaged case - give an error and print the event record
  std::cout << "E R R O R! We should never be here!\n";
  if(muonInTrkGenp != 65535)
  {
    std::cout << "Printing mothers of genp associated to inner track:\n";
    RecursivePrintMothers(muonInTrkGenp);
  }
  exit(1);
}


short int MuonMCAssociator::FindMuonMCProvenance(const unsigned short iMuon, const short int muonClass)
{
  if(abs(muonClass) == 1 || abs(muonClass) == 2 || abs(muonClass) == 8)
  {
#ifdef mgDEBUG
    std::cout << "Muon " << iMuon << " is a real muon. Looking at its provenance.\n";
#endif // mgDEBUG
    return FindGenMuonMCProvenance(muonInTrkGenp);
  }
  else
  {
#ifdef mgDEBUG
    std::cout << "Returning 0\n";
#endif // mgDEBUG
    return(0);
  }
  std::cout << "E R R O R! We should never be here!\n";
#ifndef mgDEBUG
  // To avoid an "unused variable" warning
  // I put it here so it does not affect the normal execution time
  (void) iMuon;
#endif
  exit(1);
}



short int MuonMCAssociator::FindGenMuonMCProvenance(const unsigned short iGenMuon)
{
  int idGenMuon = Genp_Id->at(iGenMuon);
  if(abs(idGenMuon) != 13)
  {
    std::cout << "E R R O R ! Method FindGenMuonMCProvenance can be called only for generated muons!\n";
    std::cout << "            The Id of the gen particle passed as argument is instead " << idGenMuon << std::endl;
    exit(1);
  }
  short int muonSign = idGenMuon>0?1:-1;
  std::pair<unsigned short, unsigned short> iStringAndDaughter = LookForMotherString(iGenMuon);
  if(!LookForDaughter(iStringAndDaughter.first,iStringAndDaughter.second))
  {
    std::cout << "iDau is not really the daughter of the string!\n";
    exit(1);
  }
  int firstId = Genp_Id->at(iStringAndDaughter.second);
  int firstType = firstId%10000;
  int firstSubType;
  if(abs(firstType) < 1000)
    firstSubType = firstType/100;
  else
    firstSubType = firstType/1000;
  if(abs(firstId) == 130)
    firstSubType = firstId>0?3:-3;
  if(abs(firstType) < 1000 && (abs(firstSubType) == 3 || abs(firstSubType) == 5))
    firstSubType = -firstSubType;
#ifdef mgDEBUG
  std::cout << "first: i = " << iStringAndDaughter.second << ", id = " << firstId << ", type = " << firstType << ", subtype = " << firstSubType << std::endl;
#endif // mgDEBUG
  unsigned short momI = Genp_vMom->at(iGenMuon).at(0);
  int momId = Genp_Id->at(momI);
  int momType = momId%10000;
  int momSubType;
  if(abs(momType) < 1000)
    momSubType = momType/100;
  else
    momSubType = momType/1000;
  // K_L need a special treatment
  if(abs(momId) == 130)
    momSubType = momId>0?3:-3;
  if(abs(momType) < 1000 && (abs(momSubType) == 3 || abs(momSubType) == 5))
    momSubType = -momSubType;
#ifdef mgDEBUG
  std::cout << "mom: i = " << Genp_vMom->at(iGenMuon).at(0) << ", id = " << momId << ", type = " << momType << ", subtype = " << momSubType << std::endl;
#endif // mgDEBUG
  //         if(abs(momSubType) == 1 || abs(momSubType) == 2)
  if(abs(momId) == 111 || abs(momId) == 211)
  {
    if(abs(firstSubType) == 5)
    {
#ifdef mgDEBUG
      std::cout << "Pion decay-in-flight from b\n";
      std::cout << "Returning 9\n";
#endif // mgDEBUG
      return(9*muonSign);
    }
    else if(abs(firstSubType) == 4)
    {
#ifdef mgDEBUG
      std::cout << "Pion decay-in-flight from c\n";
      std::cout << "Returning 10\n";
#endif // mgDEBUG
      return(10*muonSign);
    }
    else
    {
#ifdef mgDEBUG
      std::cout << "Pion decay-in-flight from non-heavy flavor\n";
      std::cout << "Returning 11\n";
#endif // mgDEBUG
      return(11*muonSign);
    }
    //           RecursivePrintMothers(iGenMuon);
  }
  //         else if(abs(momSubType) == 3)
  else if(abs(momId) == 311 || abs(momId) == 321 || abs(momId) == 130)
  {
    if(abs(firstSubType) == 5)
    {
#ifdef mgDEBUG
      std::cout << "Kaon decay-in-flight from b\n";
      std::cout << "Returning 12\n";
#endif // mgDEBUG
      return(12*muonSign);
    }
    else if(abs(firstSubType) == 4)
    {
#ifdef mgDEBUG
      std::cout << "Kaon decay-in-flight from c\n";
      std::cout << "Returning 13\n";
#endif // mgDEBUG
      return(13*muonSign);
    }
    else
    {
#ifdef mgDEBUG
      std::cout << "Kaon decay-in-flight from non-heavy flavor\n";
      std::cout << "Returning 14\n";
#endif // mgDEBUG
      return(14*muonSign);
    }
    //           RecursivePrintMothers(iGenMuon);
  }
  else
  {
    if(abs(firstSubType) == 5)
    {
      if(abs(momSubType) == 4)
      {
#ifdef mgDEBUG
        std::cout << "B to C to mu\n";
        RecursivePrintMothers(iStringAndDaughter.first);
#endif // mgDEBUG
        unsigned short bQuarkI = LookForMotherId(iStringAndDaughter.first,5,false);
#ifdef mgDEBUG
        std::cout << "i of mother b quark = " << bQuarkI << std::endl;
#endif // mgDEBUG
        if(bQuarkI == 65535)
        {
          std::cout << "E R R O R! No mother b quark found for string!\n";
          exit(1);
        }
        unsigned int code = abs(firstSubType+Genp_Id->at(iGenMuon));
#ifdef mgDEBUG
        int bQuarkId = Genp_Id->at(bQuarkI);
        unsigned int codeBefore = abs(bQuarkId+Genp_Id->at(iGenMuon));
        if(code != codeBefore)
        {
          std::cout << "code and codeBefore are different!\n";
          std::cout << "code = " << code << ", codeBefore = " << codeBefore << std::endl;
        }
#endif // mgDEBUG
        if(code == 18)
        {
#ifdef mgDEBUG
          std::cout << "\tWith mixing!\n";
          std::cout << "Returning 16\n";
#endif // mgDEBUG
          return(16*muonSign);
        }
        else if(code == 8)
        {
#ifdef mgDEBUG
          std::cout << "Without mixing!\n";
          std::cout << "Returning 15\n";
#endif // mgDEBUG
          return(15*muonSign);
        }
        else
        {
          std::cout << "E R R O R! Unmanaged case!\n";
          exit(1);
        }
      }
      else if(abs(momSubType) == 5)
      {
#ifdef mgDEBUG
        std::cout << "B to mu\n";
        std::cout << "Printing all muon ancestors:\n";
        RecursivePrintMothers(iGenMuon);
        //RecursivePrintMothers(iStringAndDaughter.first);
#endif // mgDEBUG
        unsigned short bQuarkI = LookForMotherId(iStringAndDaughter.first,5,false);
#ifdef mgDEBUG
        std::cout << "i of mother b quark = " << bQuarkI << std::endl;
#endif // mgDEBUG
        if(bQuarkI == 65535)
        {
          std::cout << "E R R O R! No mother b quark found for string!\n";
          exit(1);
        }
        unsigned int code = abs(firstSubType+Genp_Id->at(iGenMuon));
#ifdef mgDEBUG
        int bQuarkId = Genp_Id->at(bQuarkI);
        unsigned int codeBefore = abs(bQuarkId+Genp_Id->at(iGenMuon));
        if(code != codeBefore)
        {
          std::cout << "code and codeBefore are different!\n";
          std::cout << "code = " << code << ", codeBefore = " << codeBefore << std::endl;
        }
#endif // mgDEBUG
        if(code == 18)
        {
#ifdef mgDEBUG
          std::cout << "\tWithout mixing!\n";
          std::cout << "Returning 17\n";
#endif // mgDEBUG
          return(17*muonSign);
        }
        else if(code == 8)
        {
#ifdef mgDEBUG
          std::cout << "With mixing!\n";
          std::cout << "Return 18\n";
#endif // mgDEBUG
          return(18*muonSign);
        }
        else
        {
          std::cout << "E R R O R! Unmanaged case!\n";
          exit(1);
        }
      }
      else if(abs(momSubType) == 1 || abs(momSubType) == 2 || abs(momSubType) == 3)
      {
#ifdef mgDEBUG
        std::cout << "B to resonance to mu\n";
        std::cout << "Return 19\n";
#endif // mgDEBUG
        return(19*muonSign);
      }
      else if(momSubType == 0)
      {
        //               RecursivePrintMothers(iGenMuon);
        if(abs(momId) == 15)
        {
#ifdef mgDEBUG
          std::cout << "tau to mu\n";
#endif // mgDEBUG
          if(abs(momId + Genp_Id->at(iGenMuon)) != 28)
          {
            std::cout << "E R R O R! Charge flip in tau decay!\n";
            exit(1);
          }
#ifdef mgDEBUG
          std::cout << "Looking at tau mother\n";
#endif // mgDEBUG
          unsigned short tauMomI = Genp_vMom->at(momI).at(0);
          int tauMomId = Genp_Id->at(tauMomI);
          int tauMomType = tauMomId%10000;
          int tauMomSubType;
          if(abs(tauMomType) < 1000)
            tauMomSubType = tauMomType/100;
          else
            tauMomSubType = tauMomType/1000;
          if(abs(tauMomId) == 130)
            tauMomSubType = tauMomId>0?3:-3;
#ifdef mgDEBUG
          std::cout << "tau mom: i = " << tauMomI << ", id = " << tauMomId << ", type = " << tauMomType << ", subtype = " << tauMomSubType << std::endl;
#endif // mgDEBUG
          if(abs(tauMomSubType) == 4)
          {
#ifdef mgDEBUG
            std::cout << "B to C to tau to mu\n";
#endif // mgDEBUG
            unsigned short bQuarkI = LookForMotherId(iStringAndDaughter.first,5,false);
#ifdef mgDEBUG
            std::cout << "i of mother b quark = " << bQuarkI << std::endl;
#endif // mgDEBUG
            if(bQuarkI == 65535)
            {
              std::cout << "E R R O R! No mother b quark found for string!\n";
              exit(1);
            }
            unsigned int code = abs(firstSubType+Genp_Id->at(iGenMuon));
#ifdef mgDEBUG
            int bQuarkId = Genp_Id->at(bQuarkI);
            unsigned int codeBefore = abs(bQuarkId+Genp_Id->at(iGenMuon));
            if(code != codeBefore)
            {
              std::cout << "code and codeBefore are different!\n";
              std::cout << "code = " << code << ", codeBefore = " << codeBefore << std::endl;
            }
#endif // mgDEBUG
            if(code == 18)
            {
#ifdef mgDEBUG
              std::cout << "\tWith mixing!\n";
              std::cout << "Returning 21\n";
#endif // mgDEBUG
              return(21*muonSign);
            }
            else if(code == 8)
            {
#ifdef mgDEBUG
              std::cout << "Without mixing!\n";
              std::cout << "Returning 20\n";
#endif // mgDEBUG
              return(20*muonSign);
            }
            else
            {
              std::cout << "E R R O R! Unmanaged case!\n";
              exit(1);
            }
          }
          else if(abs(tauMomSubType) == 5)
          {
#ifdef mgDEBUG
            std::cout << "B to tau to mu\n";
#endif // mgDEBUG
            unsigned short bQuarkI = LookForMotherId(iStringAndDaughter.first,5,false);
#ifdef mgDEBUG
            std::cout << "i of mother b quark = " << bQuarkI << std::endl;
#endif // mgDEBUG
            if(bQuarkI == 65535)
            {
              std::cout << "E R R O R! No mother b quark found for string!\n";
              exit(1);
            }
            unsigned int code = abs(firstSubType+Genp_Id->at(iGenMuon));
#ifdef mgDEBUG
            int bQuarkId = Genp_Id->at(bQuarkI);
            unsigned int codeBefore = abs(bQuarkId+Genp_Id->at(iGenMuon));
            if(code != codeBefore)
            {
              std::cout << "code and codeBefore are different!\n";
              std::cout << "code = " << code << ", codeBefore = " << codeBefore << std::endl;
            }
#endif // mgDEBUG
            if(code == 18)
            {
#ifdef mgDEBUG
              std::cout << "\tWithout mixing!\n";
              std::cout << "Returning 22\n";
#endif // mgDEBUG
              return(22*muonSign);
            }
            else if(code == 8)
            {
#ifdef mgDEBUG
              std::cout << "With mixing!\n";
              std::cout << "Returning 23\n";
#endif // mgDEBUG
              return(23*muonSign);
            }
            else
            {
              std::cout << "E R R O R! Unmanaged case!\n";
              exit(1);
            }
          }
          else
          {
            std::cout << "E R R O R! tau mother is neither a b nor a c!\n";
            exit(1);
          }
        }
        else
        {
          std::cout << "E R R O R! Unmanaged case!\n";
          exit(1);
        }
      }
    }
    else if(abs(firstSubType) == 4)
    {
#ifdef mgDEBUG
      std::cout << "Direct C to mu\n";
      std::cout << "Printing all muon ancestors:\n";
      RecursivePrintMothers(iGenMuon);
      std::cout << "Returning 24\n";
#endif // mgDEBUG
      return(24*muonSign);
    }
    else
    {
#ifdef mgDEBUG
      std::cout << "Prompt Muon\n";
      RecursivePrintMothers(iGenMuon);
      std::cout << "Returning 25\n";
#endif // mgDEBUG
      return(25*muonSign);
    }
  }
  std::cout << "E R R O R! We should never be here!\n";
  exit(1);
}



/// //////////////// ///
/// MUON CODE AND MC ///
/// //////////////// ///



short int MuonMCAssociator::FindMuonMCCode(const unsigned short iMuon)
{
  short int muonClass = FindMuonClassFromMCAssociation(iMuon);
  short int muonMCProvenance = FindMuonMCProvenance(iMuon, muonClass);
  return(MergeMuonClassAndMCProvenance(muonClass, muonMCProvenance));
}



short int MuonMCAssociator::MergeMuonClassAndMCProvenance(const short int muonClass, const short int muonMCProvenance)
{
  // Meaning of merged code returned by this function
  // General: sign means muon (+) or antimuon (-)
  // 0: fake (reco muon has no gen particle associated to it)
  // 1: not assigned
  // 2: not assigned
  // 3: DIF (long range)
  // 4: DIF (short range)
  // 5: punch through (reco muon is associated to a generated hadron)
  // 6: possible fake or punch through (MC association does not give enough information)
  // 7: mis-reconstructed (inner track and outer track are associated to two unrelated genparticles)
  // 8: not assigned
  // 9: pion DIF coming from B
  // 10: pion DIF coming from C
  // 11: pion DIF coming from light flavor
  // 12: kaon DIF coming from B
  // 13: kaon DIF coming from C
  // 14: kaon DIF coming from light flavor
  // 15: B to C to muon (no mixing)
  // 16: B to C to muon (with mixing)
  // 17: B to muon (no mixing)
  // 18: B to muon (with mixing)
  // 19: B to hadron resonance to muon
  // 20: B to C to tau to muon (no mixing)
  // 21: B to C to tau to muon (with mixing)
  // 22: B to tau to muon (no mixing)
  // 23: B to tau to muon (with mixing)
  // 24: C to muon
  // 25: prompt
  if(muonMCProvenance != 0)
    return(muonMCProvenance);
  else
    return(muonClass);
}



/// //////////////////////////// ///
/// SIMPLIFIED AND MERGED, PtRel ///
/// //////////////////////////// ///



short int MuonMCAssociator::FindSimplifiedMuonPtRelCode(const short int muonCode)
{
  // Meaning of simplified code returned by this function
  // 0: Fakes
  // 1: B (NO sequentials)
  // 2: Direct C
  // 3: Prompt
  // 4: Decays in flight
  // 5: B to C to mu (sequentials, taus included)
  // 6: Punch-through
  if(muonCode == 0)
    return 0;
  else if( abs(muonCode) >= 17 && abs(muonCode) <= 19 )
    return 1;
  else if(abs(muonCode) == 24)
    return 2;
  else if(abs(muonCode) == 25)
    return 3;
  else if((abs(muonCode) >= 9 && abs(muonCode) <= 11) || (abs(muonCode) >= 12 && abs(muonCode) <= 14))
    return 4;
  else if((abs(muonCode) >= 15 && abs(muonCode) <= 16) || (abs(muonCode) >= 20 && abs(muonCode) <= 23))
    return 5;
  else if(abs(muonCode) >= 5 && abs(muonCode) <= 7)
    return 6;
  else
    return -99;
}



short int MuonMCAssociator::FindSimplifiedMergedMuonPtRelCodeV0(const short int muonCode)
{
  // This method merges the direct C with the decays in flight
  if(muonCode < 4)
    return muonCode;
  else if(muonCode == 4)
    return 2;
  else
    return muonCode - 1;
  // 0: Fakes
  // 1: B (NO sequentials)
  // 2: Direct C + DIF
  // 3: Prompt
  // 4: B to C to mu (sequentials)
  // 5: Punch-through
}



short int MuonMCAssociator::FindSimplifiedMergedMuonPtRelCodeV1(const short int muonCode)
{
  // This method merges the direct C with ALL but B Direct and B Seq
  if (muonCode == 1)
    return 1;
  else if (muonCode == 5)
    return 2;
  else
    return 3;
  // 0: not assigned
  // 1: B (NO sequentials)
  // 2: B to C to mu (sequentials)
  // 3: Resto del mondo
}



short int MuonMCAssociator::FindSimplifiedMergedMuonPtRelCodeV2(const short int muonCode)
{
  // This method merges the direct C with ALL but B Direct and B Seq
  if (muonCode == 1)
    return 1;
  else if (muonCode == 5)
    return 2;
  else if (muonCode == 2)
    return 3;
  else
    return 4;
  // 0: not assigned
  // 1: B (NO sequentials)
  // 2: B to C to mu (sequentials)
  // 3: C direct
  // 4: Resto del mondo
}


/// ///////////////////////// ///
/// SIMPLIFIED AND MERGED, IP ///
/// ///////////////////////// ///



short int MuonMCAssociator::FindSimplifiedMuonCode(const short int muonCode)
{
  // Meaning of simplified code returned by this function
  // 0: Fakes
  // 1: B (INCLUDING sequentials)
  // 2: Direct C
  // 3: Prompt
  // 4: Pion decays in flight
  // 5: Kaon decays in flight
  // 6: Punch-through
  if(muonCode == 0)
    return 0;
  else if(abs(muonCode) >= 15 && abs(muonCode) <= 23)
    return 1;
  else if(abs(muonCode) == 24)
    return 2;
  else if(abs(muonCode) == 25)
    return 3;
  else if(abs(muonCode) >= 9 && abs(muonCode) <= 11)
    return 4;
  else if(abs(muonCode) >= 12 && abs(muonCode) <= 14)
    return 5;
  else if(abs(muonCode) >= 5 && abs(muonCode) <= 7)
    return 6;
  else
    return -99;
}



short int MuonMCAssociator::FindSimplifiedMergedMuonCode(const short int muonCode)
{
  if(muonCode > 4)
    return muonCode - 1;
  else
    return muonCode;
}



short int MuonMCAssociator::FindDIFMuonProvenance(const short unsigned int iMuon)
{
  const short int muonCode = FindMuonMCCode(iMuon);
  if(abs(muonCode) >=9 && abs(muonCode) <= 14)
  {
    // It is a DIF and we know where it comes from - fine
    return muonCode;
  }
  else if(abs(muonCode) == 3 || abs(muonCode) == 4)
  {
    // It is a DIF but we don't know where it comes from
#ifdef mgDEBUG
    std::cout << "It is a DIF but we don't know where it comes from.\n";
#endif // mgDEBUG
    if(muonOutTrkGenpId != 0)
    {
      if(abs(muonOutTrkGenpId) == 13)
      {
        // Real muon
#ifdef mgDEBUG
        std::cout << "It is a real muon.\n";
#endif // mgDEBUG
        return FindGenMuonMCProvenance(muonOutTrkGenp);
      }
      else if(abs(muonOutTrkGenpId) == 11)
      {
        // Electron coming from a muon
#ifdef mgDEBUG
        std::cout << "It is an electron coming from a muon.\n";
#endif // mgDEBUG
        unsigned short muonI = LookForMotherId(muonOutTrkGenp,13,false);
        if(muonI != 65535)
        {
          return FindGenMuonMCProvenance(muonI);
        }
        else
        {
          std::cout <<"E R R O R ! Electron does not come from a muon!\n";
          exit(1);
        }
      }
      else
      {
        // No electron or muon - but a real muon must be near the reconstructed one
#ifdef mgDEBUG
        std::cout << "It is not an electron or a muon.\n";
#endif // mgDEBUG
        unsigned short iTrack = Muon_trackref->at(iMuon);
        unsigned short iGenpDeltaR = Genp_idx_clst->at(iTrack);
        return FindGenMuonMCProvenance(iGenpDeltaR);
      }
    }
    else
    {
#ifdef mgDEBUG
      std::cout << "There is no gen particle associated to the reco muon.\n";
#endif // mgDEBUG
      // No gen particle associated - but a real muon must be near the reconstructed one
      unsigned short iTrack = Muon_trackref->at(iMuon);
      unsigned short iGenpDeltaR = Genp_idx_clst->at(iTrack);
      return FindGenMuonMCProvenance(iGenpDeltaR);
    }
  }
  else
  {
    return 0;
  }
}



short int MuonMCAssociator::FindMuonFlavour(const short int muonCode)
{
  // Meaning of simplified code returned by this function
  // 1: B (including sequentials AND DIF)
  // 2: Direct C (including DIF)
  // 3: ALL Prompt & Light (including PT and fakes)
  if((abs(muonCode) >= 15 && abs(muonCode) <= 23) || abs(muonCode) == 9 || abs(muonCode) == 12 )
    return 1;
  else if(abs(muonCode) == 24 || abs(muonCode) == 10 || abs(muonCode) == 13)
    return 2;
  else
    return 3;
}



bool MuonMCAssociator::SelectSignalEvent(const std::vector<unsigned short>* vGenMuons,
                                         std::vector<std::pair<unsigned short, unsigned short> >& vGoodMuonPairs,
                                         std::vector<std::vector<unsigned short> >& vvBQuarks/*,
                                         const std::vector<short>* vGenMuonsProvenance,
                                         const std::vector<unsigned short>* vGenMuonsSimplifiedCode*/)
{
  if(vGenMuons->size()<2)
  {
    return false;
  }
  std::vector<short> vGenMuonsProvenance;
  std::vector<unsigned short> vGenMuonsSimplifiedCode;
  std::vector<unsigned short> vNGenMuonsSC(7,0);
  std::vector<unsigned short> vGenMuonsFromB;
  std::vector<unsigned short> vStrings; // indices of parent strings
  std::vector<unsigned short> vStringDaughters; // indices of string daughters
//   std::vector<std::vector<unsigned short> > vvBQuarks; // indices of b quarks mothers of the strings (a string can come from a b-bbar pair)
  std::vector<std::vector<unsigned short> > vvBQuarkMothers; // indices of mothers of b quarks defined above
  std::vector<std::vector<int> > vvPIDBQuarkMothers; // PID of mothers of b quarks defined above
  std::vector<std::vector<unsigned short> > vvBQuarkGMothers; // indices of grandmothers of b quarks defined above
  std::vector<std::vector<int> > vvPIDBQuarkGMothers; // PID of grandmothers of b quarks defined above
  for(unsigned int i = 0; i < vGenMuons->size(); i++)
  {
    unsigned short iGenMuon = vGenMuons->at(i);
    vGenMuonsProvenance.push_back(FindGenMuonMCProvenance(iGenMuon));
    vGenMuonsSimplifiedCode.push_back(FindSimplifiedMuonCode(vGenMuonsProvenance[vGenMuonsProvenance.size()-1]));
    if(vGenMuonsSimplifiedCode[vGenMuonsSimplifiedCode.size()-1] == 1)
    {
      vGenMuonsFromB.push_back(iGenMuon);
    }
    vNGenMuonsSC[vGenMuonsSimplifiedCode[vGenMuonsSimplifiedCode.size()-1]]++;
    std::pair<unsigned short, unsigned short> stringAndFirst = LookForMotherString(iGenMuon);
    vStrings.push_back(stringAndFirst.first);
    vStringDaughters.push_back(stringAndFirst.second);
    std::vector<unsigned short> vBQuarks;
    std::vector<unsigned short> vBQuarkMothers;
    std::vector<int> vPIDBQuarkMothers;
    std::vector<unsigned short> vBQuarkGMothers;
    std::vector<int> vPIDBQuarkGMothers;
    for( ; ; )
    {
      unsigned short motherOfString = LookForMotherIdNotInList(vStrings[vStrings.size()-1], 5, vBQuarks, false);
      if(motherOfString != 65535)
      {
        vBQuarks.push_back(motherOfString);
        vBQuarkMothers = Genp_vMom->at(motherOfString);

        unsigned short nBQuarkMothers = vBQuarkMothers.size();
        if(nBQuarkMothers > 1)
        {
          std::cout << "E R R O R ! One of the parent b quarks has more than 1 mothers!\n";
          std::cout << "            Unmanaged case! Exiting...\n";
          exit(1);
        }
        if(nBQuarkMothers == 0)
        {
          std::cout << "E R R O R ! One of the parent b quarks has no mothers!\n";
          std::cout << "            Unmanaged case! Exiting...\n";
          exit(1);
        }
        vBQuarkGMothers = Genp_vMom->at(vBQuarkMothers.at(0));
      }
      else
      {
        break;
      }
    }
    vvBQuarks.push_back(vBQuarks);
    vvBQuarkMothers.push_back(vBQuarkMothers);
    vvPIDBQuarkMothers.push_back(vPIDBQuarkMothers);
  }
  
  unsigned short nGoodMuonPairs = 0;
  short int relativeBSign = -9;
//   short int sameMother = -9;
  std::vector<short int> vRelativeBSign;
  std::vector<short int> vSameMother;
  
  for(unsigned int iGenMuon = 0; iGenMuon < vGenMuons->size(); iGenMuon++)
  {
    if(vvBQuarks[iGenMuon].size() == 0) // muon does not come from b
          continue;
    if(vGenMuonsSimplifiedCode[iGenMuon] != 1) // muon does not come from B decay
          continue;
    for(unsigned int jGenMuon = iGenMuon + 1; jGenMuon < vGenMuons->size(); jGenMuon++)
    {
      if(vvBQuarks[jGenMuon].size() == 0) // muon does not come from b
            continue;
      if(vGenMuonsSimplifiedCode[jGenMuon] != 1) // muon does not come from B decay
            continue;
      if(vStringDaughters[iGenMuon] != vStringDaughters[jGenMuon])
      {
        //             std::cout << "    Muons come from two different b hadrons!\n";
        nGoodMuonPairs++;
        std::pair<unsigned short, unsigned short> goodMuonPair = std::make_pair(vGenMuons->at(iGenMuon), vGenMuons->at(jGenMuon));
        vGoodMuonPairs.push_back(goodMuonPair);
        unsigned short iBMother = vvBQuarkMothers[iGenMuon].at(0);
        unsigned short jBMother = vvBQuarkMothers[jGenMuon].at(0);
        if(iBMother == jBMother)
        {
//           sameMother = 1;
          vSameMother.push_back(1);
        }
        else
        {
//           sameMother = 0;
          vSameMother.push_back(0);
        }
        if(vvBQuarks[iGenMuon].size() > 1 || vvBQuarks[jGenMuon].size() > 1)
        {
          relativeBSign = 0; // at least one muon comes from the hadronization of a b-bbar pair
          vRelativeBSign.push_back(0);
          //               std::cout << "    At least one of them comes from a b-bbar pair. Sign is 0.\n";
        }
        else
        {
          int iPID = Genp_Id->at(vvBQuarks[iGenMuon].at(0));
          int jPID = Genp_Id->at(vvBQuarks[jGenMuon].at(0));
          //               std::cout << "    iPID = " << iPID << ", jPID = " << jPID << std::endl;
          if((abs(iPID) == 5) && (abs(jPID) == 5))
          {
            if(jPID == -iPID)
            {
              relativeBSign = 0;
              vRelativeBSign.push_back(0);
            }
            else if(jPID == iPID)
            {
              iPID>0?relativeBSign=1:relativeBSign=-1;
              iPID>0?vRelativeBSign.push_back(1):vRelativeBSign.push_back(-1);
            }
          }
          else
          {
            std::cout << "E R R O R ! Wrong PID for some parent b quark!\n";
            std::cout << "            iPID = " << iPID << ", jPID = " << jPID << std::endl;
            exit(1);
          }
        }
      }
    }
  }
  
  if(vRelativeBSign.size() != nGoodMuonPairs)
  {
    std::cout << "E R R O R ! Size of vRelativeBSign is not equal to nGoodMuonPairs!\n";
    std::cout << "            vRelativeBSign.size() = " << vRelativeBSign.size() << ", nGoodMuonPairs = " << nGoodMuonPairs << std::endl;
    exit(1);
  }
  // true if in the event there is at least a good di-muon pair from different-sign b quarks
  // If at least one of the entries of vRelativeBSign is = 0, then it is set to true
  // It is needed in order to check that we do not have contamination from non b-bbar pairs
  bool eventHasGoodPairFromDSB = false;
  for(unsigned int iPair = 0; iPair < nGoodMuonPairs; iPair++)
  {
    if(vRelativeBSign[iPair] == 0)
    {
      eventHasGoodPairFromDSB = true;
      break;
    }
  }
  if(nGoodMuonPairs != 0 && eventHasGoodPairFromDSB)
  {
    return true;
  }
  return false;
}



/////////////////////////////////////////////// TEST TEST TEST

bool MuonMCAssociator::SelectSingleMuEvent(const std::vector<unsigned short>* vGenMuons,
                                           std::vector<unsigned short>& vGoodGenMuons)
{
  if(vGenMuons->size()<1)
  {
    return false;
  }
  
  std::vector<short> vGenMuonsProvenance;
  std::vector<unsigned short> vGenMuonsSimplifiedPtRelCode;
//   std::vector<unsigned short> vGenMuonsFromB;
  std::vector<unsigned short> vStrings; // indices of parent strings
  std::vector<unsigned short> vStringDaughters; // indices of string daughters
  std::vector<std::vector<unsigned short> > vvBQuarks; // indices of b quarks mothers of the strings (a string can come from a b-bbar pair)
  
  for(unsigned int i = 0; i < vGenMuons->size(); i++)
  {
    unsigned short iGenMuon = vGenMuons->at(i);
    
    vGenMuonsProvenance.push_back(FindGenMuonMCProvenance(iGenMuon));
    vGenMuonsSimplifiedPtRelCode.push_back(FindSimplifiedMuonPtRelCode(vGenMuonsProvenance[vGenMuonsProvenance.size()-1]));
    
//     if(vGenMuonsSimplifiedPtRelCode[vGenMuonsSimplifiedPtRelCode.size()-1] == 1)
//     {
//       vGenMuonsFromB.push_back(iGenMuon);
//     }
    
    std::pair<unsigned short, unsigned short> stringAndFirst = LookForMotherString(iGenMuon);
    vStrings.push_back(stringAndFirst.first);
    vStringDaughters.push_back(stringAndFirst.second);
    
    std::vector<unsigned short> vBQuarks;
    std::vector<unsigned short> vBQuarkMothers;
    
    for( ; ; )
    {
      unsigned short motherOfString = LookForMotherIdNotInList(vStrings[vStrings.size()-1], 5, vBQuarks, false);
      if(motherOfString != 65535)
      {
        vBQuarks.push_back(motherOfString);
        vBQuarkMothers = Genp_vMom->at(motherOfString);
        
        unsigned short nBQuarkMothers = vBQuarkMothers.size();
        if(nBQuarkMothers > 1)
        {
          std::cout << "E R R O R ! One of the parent b quarks has more than 1 mothers!\n";
          std::cout << "            Unmanaged case! Exiting...\n";
          exit(1);
        }
        if(nBQuarkMothers == 0)
        {
          std::cout << "E R R O R ! One of the parent b quarks has no mothers!\n";
          std::cout << "            Unmanaged case! Exiting...\n";
          exit(1);
        }
      }
      else
      {
        break;
      }
    }
    vvBQuarks.push_back(vBQuarks);
    
  }
  
  unsigned short nGoodMuons = 0;
  
  for(unsigned int iGenMuon = 0; iGenMuon < vGenMuons->size(); iGenMuon++)
  {
    if(vvBQuarks[iGenMuon].size() == 0) // muon does not come from b
      continue;
    if(vGenMuonsSimplifiedPtRelCode[iGenMuon] != 1) // muon does not come from B decay
      continue;
    
    nGoodMuons++;
    unsigned short goodMuon = vGenMuons->at(iGenMuon);
    vGoodGenMuons.push_back(goodMuon);
    
  }
  
  if(nGoodMuons != 0)
  {
    return true;
  }
  return false;
}

/////////////////////////////////////////////// end of TEST TEST TEST



std::pair<short int, double> MuonMCAssociator::FindTrackNearestToGenParticle(const unsigned short int iGenParticle, const bool selHighPurity)
{
  if(iGenParticle > Genp_status->size())
  {
    std::cout << "E R R O R ! MuonMCAssociator::FindTrackNearestToGenParticle(): iGenParticle beyond limits!\n";
    std::cout << "            iGenParticle = " << iGenParticle << ", Number of gen particles = " << Genp_status->size() << std::endl;
    exit(1);
  }
  TLorentzVector pGenParticle;
  pGenParticle.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
  short int iAssociatedTrack = -1;
  double deltaRGenParticleTrack = 99999;
  for(int iTrack = 0; iTrack < numberOfTrack; iTrack++)
  {
    if(selHighPurity && !Track_isHighPurity->at(iTrack))
    {
      continue;
    }
    if(Track_charge->at(iTrack) != Genp_charge->at(iGenParticle))
    {
      continue;
    }
    TVector3 pTrack;
    pTrack.SetPtEtaPhi(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack));
    double deltaR = pGenParticle.Vect().DeltaR(pTrack);
    if(deltaR < deltaRGenParticleTrack)
    {
      deltaRGenParticleTrack = deltaR;
      iAssociatedTrack = iTrack;
    }
  }
  //  return std::make_pair<short int, double>(iAssociatedTrack, deltaRGenParticleTrack);
  return std::make_pair(iAssociatedTrack, deltaRGenParticleTrack);
}



std::pair<short int, double> MuonMCAssociator::FindTrackNearestToGenParticleWithPtThreshold(const unsigned short int iGenParticle, const double ptTrackMin, const bool selHighPurity)
{
  if(iGenParticle > Genp_status->size())
  {
    std::cout << "E R R O R ! MuonMCAssociator::FindTrackNearestToGenParticleWithPtThreshold(): iGenParticle beyond limits!\n";
    std::cout << "            iGenParticle = " << iGenParticle << ", Number of gen particles = " << Genp_status->size() << std::endl;
    exit(1);
  }
  TLorentzVector pGenParticle;
  pGenParticle.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
  short int iAssociatedTrack = -1;
  double deltaRGenParticleTrack = 99999;
  for(int iTrack = 0; iTrack < numberOfTrack; iTrack++)
  {
    if(Track_pt->at(iTrack) > ptTrackMin)
    {
      if(selHighPurity && !Track_isHighPurity->at(iTrack))
      {
        continue;
      }
      if(Track_charge->at(iTrack) != Genp_charge->at(iGenParticle))
      {
        continue;
      }
      TVector3 pTrack;
      pTrack.SetPtEtaPhi(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack));
      double deltaR = pGenParticle.Vect().DeltaR(pTrack);
      if(deltaR < deltaRGenParticleTrack)
      {
        deltaRGenParticleTrack = deltaR;
        iAssociatedTrack = iTrack;
      }
    }
  }
  //  return std::make_pair<short int, double>(iAssociatedTrack, deltaRGenParticleTrack);
  return std::make_pair(iAssociatedTrack, deltaRGenParticleTrack);
}



std::pair<short int, double> MuonMCAssociator::FindTrackAssociatedToGenParticleByDRDPtLessByDR(const unsigned short int iGenParticle, const double deltaRMax, const double deltaPtOverPtMax, const bool selHighPurity)
{
  if(iGenParticle > Genp_status->size())
  {
    std::cout << "E R R O R ! MuonMCAssociator::FindTrackAssociatedToGenParticleByDRDPtLessByDR(): iGenParticle beyond limits!\n";
    std::cout << "            iGenParticle = " << iGenParticle << ", Number of gen particles = " << Genp_status->size() << std::endl;
    exit(1);
  }
  TLorentzVector pGenParticle;
  pGenParticle.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
  short int iAssociatedTrack = -1;
  double deltaRGenParticleTrack = 99999;
  for(int iTrack = 0; iTrack < numberOfTrack; iTrack++)
  {
    if(selHighPurity && !Track_isHighPurity->at(iTrack))
    {
      continue;
    }
    if(Track_charge->at(iTrack) != Genp_charge->at(iGenParticle))
    {
      continue;
    }
    TVector3 pTrack;
    pTrack.SetPtEtaPhi(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack));
    double deltaR = pGenParticle.Vect().DeltaR(pTrack);
    double deltaPtOverPt = fabs((pGenParticle.Perp()-pTrack.Perp())/pGenParticle.Perp());
    if(deltaR < deltaRMax && deltaPtOverPt < deltaPtOverPtMax)
    {
      if(deltaR < deltaRGenParticleTrack)
      {
        deltaRGenParticleTrack = deltaR;
        iAssociatedTrack = iTrack;
      }
    }
  }
  //  return std::make_pair<short int, double>(iAssociatedTrack, deltaRGenParticleTrack);
  return std::make_pair(iAssociatedTrack, deltaRGenParticleTrack);
}



std::pair<short int, double> MuonMCAssociator::FindGenParticleNearestToTrack(const unsigned short int iTrack)
{
  if(iTrack > numberOfTrack)
  {
    std::cout << "E R R O R ! MuonMCAssociator::FindGenParticleNearestToTrack(): iTrack beyond limits!\n";
    std::cout << "            iTrack = " << iTrack << ", Number of tracks = " << numberOfTrack << std::endl;
    exit(1);
  }
  TVector3 pTrack;
  pTrack.SetPtEtaPhi(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack));
  short int iAssociatedGenParticle = -1;
  double deltaRGenParticleTrack = 99999;
  for(unsigned int iGenParticle = 0; iGenParticle < Genp_Id->size(); iGenParticle++)
  {
    // Do not consider particles with pt=0 to avoid warnings
    if(Genp_pt->at(iGenParticle) == 0)
    {
      continue;
    }
    // Avoid unphysical case of neutral particles
    if(Genp_charge->at(iGenParticle) == 0)
    {
      continue;
    }
    if(Track_charge->at(iTrack) != Genp_charge->at(iGenParticle))
    {
      continue;
    }
    TLorentzVector pGenParticle;
    pGenParticle.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
    double deltaR = pGenParticle.Vect().DeltaR(pTrack);
    if(deltaR < deltaRGenParticleTrack)
    {
      deltaRGenParticleTrack = deltaR;
      iAssociatedGenParticle = iGenParticle;
    }
  }
  //  return std::make_pair<short int, double>(iAssociatedGenParticle, deltaRGenParticleTrack);
  return std::make_pair(iAssociatedGenParticle, deltaRGenParticleTrack);
}



std::pair<short int, double> MuonMCAssociator::FindGenParticleNearestToTrackWithPtThreshold(const unsigned short int iTrack, const double ptGenParticleMin)
{
  if(iTrack > numberOfTrack)
  {
    std::cout << "E R R O R ! MuonMCAssociator::FindGenParticleNearestToTrackWithPtThreshold(): iTrack beyond limits!\n";
    std::cout << "            iTrack = " << iTrack << ", Number of tracks = " << numberOfTrack << std::endl;
    exit(1);
  }
  TVector3 pTrack;
  pTrack.SetPtEtaPhi(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack));
  short int iAssociatedGenParticle = -1;
  double deltaRGenParticleTrack = 99999;
  for(unsigned int iGenParticle = 0; iGenParticle < Genp_Id->size(); iGenParticle++)
  {
    // Do not consider particles with pt=0 to avoid warnings
    if(Genp_pt->at(iGenParticle) == 0)
    {
      continue;
    }
    // Avoid unphysical case of neutral particles
    if(Genp_charge->at(iGenParticle) == 0)
    {
      continue;
    }
    if(Track_charge->at(iTrack) != Genp_charge->at(iGenParticle))
    {
      continue;
    }
    if(Genp_pt->at(iGenParticle) > ptGenParticleMin)
    {
      TLorentzVector pGenParticle;
      pGenParticle.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
      double deltaR = pGenParticle.Vect().DeltaR(pTrack);
      if(deltaR < deltaRGenParticleTrack)
      {
        deltaRGenParticleTrack = deltaR;
        iAssociatedGenParticle = iGenParticle;
      }
    }
  }
  //  return std::make_pair<short int, double>(iAssociatedGenParticle, deltaRGenParticleTrack);
  return std::make_pair(iAssociatedGenParticle, deltaRGenParticleTrack);
}



std::pair<short int, double> MuonMCAssociator::FindGenParticleAssociatedToTrackByDRDPtLessByDR(const unsigned short int iTrack, const double deltaRMax, const double deltaPtOverPtMax)
{
  if(iTrack > numberOfTrack)
  {
    std::cout << "E R R O R ! MuonMCAssociator::FindGenParticleAssociatedToTrackByDRDPtLessByDR(): iTrack beyond limits!\n";
    std::cout << "            iTrack = " << iTrack << ", Number of tracks = " << numberOfTrack << std::endl;
    exit(1);
  }
  TVector3 pTrack;
  pTrack.SetPtEtaPhi(Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack));
  short int iAssociatedGenParticle = -1;
  double deltaRGenParticleTrack = 99999;
  for(unsigned int iGenParticle = 0; iGenParticle < Genp_Id->size(); iGenParticle++)
  {
    // Do not consider particles with pt=0 to avoid warnings
    if(Genp_pt->at(iGenParticle) == 0)
    {
      continue;
    }
    // Avoid unphysical case of neutral particles
    if(Genp_charge->at(iGenParticle) == 0)
    {
      continue;
    }
    if(Track_charge->at(iTrack) != Genp_charge->at(iGenParticle))
    {
      continue;
    }
    TLorentzVector pGenParticle;
    pGenParticle.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
    double deltaR = pGenParticle.Vect().DeltaR(pTrack);
    double deltaPtOverPt = fabs((pGenParticle.Perp()-pTrack.Perp())/pGenParticle.Perp());
    if(deltaR < deltaRMax && deltaPtOverPt < deltaPtOverPtMax)
    {
      if(deltaR < deltaRGenParticleTrack)
      {
        deltaRGenParticleTrack = deltaR;
        iAssociatedGenParticle = iGenParticle;
      }
    }
  }
  //  return std::make_pair<short int, double>(iAssociatedGenParticle, deltaRGenParticleTrack);
  return std::make_pair(iAssociatedGenParticle, deltaRGenParticleTrack);
}



// Classification of fake muons based on their id and decay
// Classes are:
// 1 = real mu
// 
// 10 = pi sail-through (no daughters)
// 11 = pi DIF (pi->muX)
// 12 = pi cascade DIF (pi->X->muY)
// 13 = pi punch-through (pi->X with no muons in the progeny)
// 20 = K sail-through (no daughters)
// 21 = K DIF (pi->muX)
// 22 = K cascade DIF (pi->X->muY)
// 23 = K punch-through (pi->X with no muons in the progeny)
// 30 = p sail-through (no daughters)
// 31 = p DIF (pi->muX)
// 32 = p cascade DIF (pi->X->muY)
// 33 = p punch-through (pi->X with no muons in the progeny)
// 40 = other
int MuonMCAssociator::FindFakeClassification(const unsigned int iGenp)
{
  if(iGenp > Genp_pt->size() - 1)
  {
    std::cout << "E R R O R ! MuonMCAssociator::FindFakeClassification(...) : input iGenp bigger than size of genp vector!\n";
    std::cout << "            iGenp = " << iGenp << ", size of genp vector = " << Genp_pt->size() << std::endl;
    exit(1);
  }
  int id = Genp_Id->at(iGenp);
  unsigned int absId = abs(id);
  short sign = (id<0)?-1:1;
  bool hasDaughters = Genp_vDau->at(iGenp).size()>0?true:false;
  bool hasMuDaughter = false;
  if(LookForDaughterId(iGenp,13,false) != 65535)
  {
    hasMuDaughter = true;
  }
  bool hasMuInProgeny = false;
  if(RecursiveLookForDaughterId(iGenp, 13, false) != 65535)
  {
    hasMuInProgeny = true;
  }
  // Real mu
  if(absId == 13)
  {
    return 1*sign; 
  }
  // pion
  if(absId == 211)
  {
    // Sail-through
    if(!hasDaughters)
    {
      return 10*sign;
    }
    // DIF
    if(hasMuDaughter)
    {
      return 11*sign;
    }
    // Cascade DIF
    if(hasMuInProgeny)
    {
      return 12*sign;
    }
    // Punch-through
    return 13*sign;
  }
  // kaon
  if(absId == 321)
  {
    // Sail-through
    if(!hasDaughters)
    {
      return 20*sign;
    }
    // DIF
    if(hasMuDaughter)
    {
      return 21*sign;
    }
    // Cascade DIF
    if(hasMuInProgeny)
    {
      return 22*sign;
    }
    // Punch-through
    return 23*sign;
  }
  // proton
  if(absId == 2212)
  {
    // Sail-through
    if(!hasDaughters)
    {
      return 30*sign;
    }
    // DIF
    if(hasMuDaughter)
    {
      return 31*sign;
    }
    // Cascade DIF
    if(hasMuInProgeny)
    {
      return 32*sign;
    }
    // Punch-through
    return 33*sign;
  }
  // Other
  return 40*sign;
}



#if NTUPLE_VERSION >= 4
std::pair<short int, double> MuonMCAssociator::FindL1TTTrackNearestToGenParticle(const unsigned short int iGenParticle, bool use5ParFit)
{
  if(iGenParticle > Genp_status->size())
  {
    std::cout << "E R R O R ! MuonMCAssociator::FindL1TTTrackNearestToGenParticle(): iGenParticle beyond limits!\n";
    std::cout << "            iGenParticle = " << iGenParticle << ", Number of gen particles = " << Genp_status->size() << std::endl;
    exit(1);
  }
  TLorentzVector pGenParticle;
  pGenParticle.SetPtEtaPhiM(Genp_pt->at(iGenParticle), Genp_eta->at(iGenParticle), Genp_phi->at(iGenParticle), Genp_mass->at(iGenParticle));
  short int iAssociatedL1TTTrack = -1;
  double deltaRGenParticleL1TTTrack = 99999;
  for(unsigned int iL1TTTrack = 0; iL1TTTrack < L1TTTracks_theMomentum4Par_x->size(); iL1TTTrack++)
  {
    TVector3 pL1TTTrack;
    if(use5ParFit)
    {
      pL1TTTrack.SetXYZ(L1TTTracks_theMomentum5Par_x->at(iL1TTTrack), L1TTTracks_theMomentum5Par_y->at(iL1TTTrack), L1TTTracks_theMomentum5Par_z->at(iL1TTTrack));
    }
    else
    {
      pL1TTTrack.SetXYZ(L1TTTracks_theMomentum4Par_x->at(iL1TTTrack), L1TTTracks_theMomentum4Par_y->at(iL1TTTrack), L1TTTracks_theMomentum4Par_z->at(iL1TTTrack));
    }
    double deltaR = pGenParticle.Vect().DeltaR(pL1TTTrack);
    if(deltaR < deltaRGenParticleL1TTTrack)
    {
      deltaRGenParticleL1TTTrack = deltaR;
      iAssociatedL1TTTrack = iL1TTTrack;
    }
  }
  //  return std::make_pair<short int, double>(iAssociatedL1TTTrack, deltaRGenParticleL1TTTrack);
  return std::make_pair(iAssociatedL1TTTrack, deltaRGenParticleL1TTTrack);
}

#endif



#endif // MuonMCAssociator_cxx


