#ifndef MCTools_cxx
#define MCTools_cxx

#include "MCTools.h"



// #define mgDEBUG



bool MCTools::LookForDaughter(const unsigned short iGenp, const unsigned short iDau)
{
#ifdef mgDEBUG
  std::cout << "Looking for daughter #" << iDau << " of Genp #" << iGenp << std::endl;
#endif // mgDEBUG
  for(unsigned int i = 0; i < Genp_vDau->at(iGenp).size(); i++)
  {
#ifdef mgDEBUG
    std::cout << "(" << iGenp << ")" << Genp_Id->at(iGenp) << "-->" << "(" << Genp_vDau->at(iGenp).at(i) << ")" << Genp_Id->at(Genp_vDau->at(iGenp).at(i)) << std::endl;
#endif // mgDEBUG
    if(Genp_vDau->at(iGenp).at(i) == iDau)
      return true;
  }
  return false;
}



bool MCTools::LookForMother(const unsigned short iGenp, const unsigned short iMom)
{
#ifdef mgDEBUG
  std::cout << "Looking for mother #" << iMom << " of Genp #" << iGenp << std::endl;
#endif // mgDEBUG
  for(unsigned int i = 0; i < Genp_vMom->at(iGenp).size(); i++)
  {
#ifdef mgDEBUG
    std::cout << "(" << iGenp << ")" << Genp_Id->at(iGenp) << "<--" << "(" << Genp_vMom->at(iGenp).at(i) << ")" << Genp_Id->at(Genp_vMom->at(iGenp).at(i)) << std::endl;
#endif // mgDEBUG
    if(Genp_vMom->at(iGenp).at(i) == iMom)
      return true;
  }
  return false;
}



bool MCTools::RecursiveLookForDaughter(const unsigned short iGenp, const unsigned short iDau)
{
#ifdef mgDEBUG
  std::cout << "Looking for daughter #" << iDau << " of Genp #" << iGenp << std::endl;
#endif // mgDEBUG
  for(unsigned int i = 0; i < Genp_vDau->at(iGenp).size(); i++)
  {
#ifdef mgDEBUG
    std::cout << "(" << iGenp << ")" << Genp_Id->at(iGenp) << "-->" << "(" << Genp_vDau->at(iGenp).at(i) << ")" << Genp_Id->at(Genp_vDau->at(iGenp).at(i)) << std::endl;
#endif // mgDEBUG
    if(Genp_vDau->at(iGenp).at(i) == iDau)
      return true;
    if(RecursiveLookForDaughter(Genp_vDau->at(iGenp).at(i), iDau))
      return true;
  }
  return false;
}



bool MCTools::RecursiveLookForMother(const unsigned short iGenp, const unsigned short iMom)
{
#ifdef mgDEBUG
  std::cout << "Looking for mother #" << iMom << " of Genp #" << iGenp << std::endl;
#endif // mgDEBUG
  for(unsigned int i = 0; i < Genp_vMom->at(iGenp).size(); i++)
  {
#ifdef mgDEBUG
    std::cout << "(" << iGenp << ")" << Genp_Id->at(iGenp) << "<--" << "(" << Genp_vMom->at(iGenp).at(i) << ")" << Genp_Id->at(Genp_vMom->at(iGenp).at(i)) << std::endl;
#endif // mgDEBUG
    if(Genp_vMom->at(iGenp).at(i) == iMom)
      return true;
    if(RecursiveLookForMother(Genp_vMom->at(iGenp).at(i), iMom))
      return true;
  }
  return false;
}



unsigned short MCTools::LookForMotherId(const unsigned short iGenp, int idMom, const bool withSign)
{
  unsigned short nMoms = Genp_vMom->at(iGenp).size();
  if(nMoms == 0)
  {
    std::cout << "W A R N I N G! Gen particle has no mothers!\n";
    return 65535;
  }
  if(!withSign)
    idMom = abs(idMom);
  for(unsigned int iMom = 0; iMom < nMoms; iMom++)
  {
    int id = Genp_Id->at(Genp_vMom->at(iGenp).at(iMom));
    if(!withSign)
      id = abs(id);
    if(id == idMom)
      return Genp_vMom->at(iGenp).at(iMom);
  }
  return 65535;
}



unsigned short MCTools::LookForDaughterId(const unsigned short iGenp, int idDau, const bool withSign)
{
  unsigned short nDaus = Genp_vDau->at(iGenp).size();
  if(nDaus == 0)
  {
    std::cout << "W A R N I N G! Gen particle has no daughters!\n";
    return 65535;
  }
  if(!withSign)
    idDau = abs(idDau);
  for(unsigned int iDau = 0; iDau < nDaus; iDau++)
  {
    int id = Genp_Id->at(Genp_vDau->at(iGenp).at(iDau));
    if(!withSign)
      id = abs(id);
    if(id == idDau)
      return Genp_vDau->at(iGenp).at(iDau);
  }
  return 65535;
}



int MCTools::RecursiveLookForMotherId(const unsigned short iGenp, int idMom, const bool withSign)
{
  unsigned short nMoms = Genp_vMom->at(iGenp).size();
  if(nMoms == 0)
  {
    std::cout << "W A R N I N G! Gen particle has no mothers!\n";
    return -65535;
  }
  if(!withSign)
    idMom = abs(idMom);
  for(unsigned int i = 0; i < nMoms; i++)
  {
    unsigned short iMom = Genp_vMom->at(iGenp).at(i);
    int id = Genp_Id->at(iMom);
    if(!withSign)
      id = abs(id);
    if(id == idMom)
      return iMom;
    else
    {
      int jMom = RecursiveLookForMotherId(iMom, idMom, withSign);
      if( jMom !=-65535 ) 
      {
        return jMom;
      }
    }
  }
  return 65535;
}



int MCTools::RecursiveLookForDaughterId(const unsigned short iGenp, int idDau, const bool withSign)
{
  unsigned short nDaus = Genp_vDau->at(iGenp).size();
  if(nDaus == 0)
  {
    std::cout << "W A R N I N G! Gen particle has no daughters!\n";
    return -65535;
  }
  if(!withSign)
    idDau = abs(idDau);
  for(unsigned int i = 0; i < nDaus; i++)
  {
    unsigned short iDau = Genp_vDau->at(iGenp).at(i);
    int id = Genp_Id->at(iDau);
    if(!withSign)
      id = abs(id);
    if(id == idDau)
      return iDau;
    else
    {
      int jDau = RecursiveLookForDaughterId(iDau, idDau, withSign);
      if( jDau !=-65535 ) 
      {
        return jDau;
      }
    }
  }
  return 65535;
}



unsigned short MCTools::LookForMotherIdInList(const unsigned short iGenp, int idMom, const std::vector<unsigned short> vIncludedIMom, const bool withSign)
{
  unsigned short nMoms = Genp_vMom->at(iGenp).size();
  if(nMoms == 0)
  {
    std::cout << "W A R N I N G! Gen particle has no mothers!\n";
    return 65535;
  }
  if(!withSign)
    idMom = abs(idMom);
  for(unsigned int i = 0; i < nMoms; i++)
  {
    unsigned short iMom = Genp_vMom->at(iGenp).at(i);
    int id = Genp_Id->at(iMom);
    if(!withSign)
      id = abs(id);
    if(id == idMom)
    {
      bool isIncluded = false;
      for(unsigned short iIncluded = 0; iIncluded != vIncludedIMom.size(); iIncluded++)
      {
        if(iMom == vIncludedIMom[iIncluded])
        {
          isIncluded = true;
          break;
        }
      }
      if(isIncluded)
        return iMom;
    }
  }
  return 65535;
}



unsigned short MCTools::LookForDaughterIdInList(const unsigned short iGenp, int idDau, const std::vector<unsigned short> vIncludedIDau, const bool withSign)
{
  unsigned short nDaus = Genp_vDau->at(iGenp).size();
  if(nDaus == 0)
  {
    std::cout << "W A R N I N G! Gen particle has no daugthers!\n";
    return 65535;
  }
  if(!withSign)
    idDau = abs(idDau);
  for(unsigned int i = 0; i < nDaus; i++)
  {
    unsigned short iDau = Genp_vDau->at(iGenp).at(i);
    int id = Genp_Id->at(iDau);
    if(!withSign)
      id = abs(id);
    if(id == idDau)
    {
      bool isIncluded = false;
      for(unsigned short iIncluded = 0; iIncluded != vIncludedIDau.size(); iIncluded++)
      {
        if(iDau == vIncludedIDau[iIncluded])
        {
          isIncluded = true;
          break;
        }
      }
      if(isIncluded)
        return iDau;
    }
  }
  return 65535;
}



unsigned short MCTools::LookForMotherIdNotInList(const unsigned short iGenp, int idMom, const std::vector<unsigned short> vExcludedIMom, const bool withSign)
{
  unsigned short nMoms = Genp_vMom->at(iGenp).size();
  if(nMoms == 0)
  {
    std::cout << "W A R N I N G! Gen particle has no mothers!\n";
    return 65535;
  }
  if(!withSign)
    idMom = abs(idMom);
  for(unsigned int i = 0; i < nMoms; i++)
  {
    unsigned short iMom = Genp_vMom->at(iGenp).at(i);
    int id = Genp_Id->at(iMom);
    if(!withSign)
      id = abs(id);
    if(id == idMom)
    {
      bool isExcluded = false;
      for(unsigned short iExcluded = 0; iExcluded != vExcludedIMom.size(); iExcluded++)
      {
        if(iMom == vExcludedIMom[iExcluded])
        {
          isExcluded = true;
          break;
        }
      }
      if(!isExcluded)
        return iMom;
    }
  }
  return 65535;
}



unsigned short MCTools::LookForDaughterIdNotInList(const unsigned short iGenp, int idDau, const std::vector<unsigned short> vExcludedIDau, const bool withSign)
{
  unsigned short nDaus = Genp_vDau->at(iGenp).size();
  if(nDaus == 0)
  {
    std::cout << "W A R N I N G! Gen particle has no daugthers!\n";
    return 65535;
  }
  if(!withSign)
    idDau = abs(idDau);
  for(unsigned int i = 0; i < nDaus; i++)
  {
    unsigned short iDau = Genp_vDau->at(iGenp).at(i);
    int id = Genp_Id->at(iDau);
    if(!withSign)
      id = abs(id);
    if(id == idDau)
    {
      bool isExcluded = false;
      for(unsigned short iExcluded = 0; iExcluded != vExcludedIDau.size(); iExcluded++)
      {
        if(iDau == vExcludedIDau[iExcluded])
        {
          isExcluded = true;
          break;
        }
      }
      if(!isExcluded)
        return iDau;
    }
  }
  return 65535;
}



std::vector<unsigned short> MCTools::RecursiveFindDaughters(const unsigned short iGenp)
{
  std::vector<unsigned short> vDaughters;
  unsigned short nDaus = Genp_vDau->at(iGenp).size();
  for(unsigned short i = 0; i < nDaus; i++)
  {
    unsigned short iDau = Genp_vDau->at(iGenp).at(i);
    vDaughters.push_back(iDau);
    std::vector<unsigned short> vDaughterDaughters = RecursiveFindDaughters(iDau);
    for(unsigned short ii = 0; ii < vDaughterDaughters.size(); ii++)
    {
      vDaughters.push_back(vDaughterDaughters[ii]);
    }
  }
  return vDaughters;
}



void MCTools::RecursivePrintMothers(const unsigned short iGenp)
{
//   std::cout << "Inside MCTools::RecursivePrintMothers: genp " << iGenp << " has " << Genp_vMom->at(iGenp).size() << " mothers.\n";
  for(unsigned int i = 0; i < Genp_vMom->at(iGenp).size(); i++)
  {
    std::cout << "(" << iGenp << ")" << Genp_Id->at(iGenp) << "<--" << "(" << Genp_vMom->at(iGenp).at(i) << ")" << Genp_Id->at(Genp_vMom->at(iGenp).at(i));
    std::cout << " (charge " << Genp_charge->at(Genp_vMom->at(iGenp).at(i)) << ")";
    std::cout << " at (" << Genp_vx->at(iGenp) << "," << Genp_vy->at(iGenp) << "," << Genp_vz->at(iGenp) << ")";
    std::cout << " pt,eta,phi,M (" << Genp_pt->at(iGenp) << "," << Genp_eta->at(iGenp) << "," << Genp_phi->at(iGenp) << "," << Genp_mass->at(iGenp) << ")\n";
    RecursivePrintMothers(Genp_vMom->at(iGenp).at(i));
  }
}



void MCTools::RecursivePrintDaughters(const unsigned short iGenp)
{
  for(unsigned int i = 0; i < Genp_vDau->at(iGenp).size(); i++)
  {
    std::cout << "(" << iGenp << ")" << Genp_Id->at(iGenp) << "-->" << "(" << Genp_vDau->at(iGenp).at(i) << ")" << Genp_Id->at(Genp_vDau->at(iGenp).at(i));
    std::cout << " (charge " << Genp_charge->at(Genp_vDau->at(iGenp).at(i)) << ", status " << Genp_status->at(Genp_vDau->at(iGenp).at(i)) << ")";
    std::cout << " at (" << Genp_vx->at(Genp_vDau->at(iGenp).at(i)) << "," << Genp_vy->at(Genp_vDau->at(iGenp).at(i)) << "," << Genp_vz->at(Genp_vDau->at(iGenp).at(i)) << ")";
    std::cout << " pt,eta,phi,M (" << Genp_pt->at(Genp_vDau->at(iGenp).at(i)) << "," << Genp_eta->at(Genp_vDau->at(iGenp).at(i)) << "," << Genp_phi->at(Genp_vDau->at(iGenp).at(i)) << "," << Genp_mass->at(Genp_vDau->at(iGenp).at(i)) << ")\n";
    RecursivePrintDaughters(Genp_vDau->at(iGenp).at(i));
  }
}



std::pair<unsigned short,unsigned short> MCTools::LookForMotherString(const unsigned short iGenp)
{
  unsigned short i = iGenp;
  unsigned short iDau = iGenp;
  int id = Genp_Id->at(iGenp);
  if(id > 80 && id < 101)
  {
    std::cout << "W A R N I N G! Input gen particle is already a string-y object! Returning the same index.\n";
    //    return std::make_pair<unsigned short, unsigned short>(i,iDau);
    return std::make_pair(i,iDau);
  }
  while(id < 81 || id > 100)
  {
#ifdef mgDEBUG
    std::cout << "i = " << i << ", id = " << id << std::endl;
#endif // mgDEBUG
    unsigned short nMoms = Genp_vMom->at(i).size();
    if(nMoms > 1)
    {
//       std::cout << "W A R N I N G! Genp " << i << " has > 1 mother!\n";
//       std::cout << "Printing the genparticle record:\n";
//       for(unsigned int iGenp2 = 0; iGenp2 < Genp_Id->size(); iGenp2++)
//       {
//         std::cout << iGenp2 << ": Id = " << Genp_Id->at(iGenp2);
//         if(Genp_vMom->at(iGenp2).size() != 0)
//           std::cout << ", iMot = ";
//         for(unsigned int iMom = 0; iMom != Genp_vMom->at(iGenp2).size(); iMom++)
//           std::cout << Genp_vMom->at(iGenp2).at(iMom) << " ";
//         if(Genp_vDau->at(iGenp2).size() != 0)
//         {
//           std::cout << ", iDau = ";
//           for(unsigned int iDau2 = 0; iDau2 != Genp_vDau->at(iGenp2).size(); iDau2++)
//             std::cout << Genp_vDau->at(iGenp2).at(iDau2) << " ";
//         }
//         std::cout << std::endl;
//       }
      break;
    }
    if(nMoms == 0)
    {
      if(Genp_Id->at(i) == 2212)
      {
//         std::cout << "W A R N I N G! Genp " << i << " has no mothers!\n";
//         std::cout << "               Particle " << iGenp << " comes directly from the proton without a string in the middle!\n";
        break;
      }
      else
      {
        std::cout << "E R R O R! Genp " << i << " has no mothers and particle " << iGenp << " does not come from the proton!\n";
        std::cout << "           Exiting...\n";
        exit(1);
      }
    }
    iDau = i;
    i = Genp_vMom->at(i).at(0);
    id = Genp_Id->at(i);

  }
#ifdef mgDEBUG
  std::cout << "A string was found!\n";
  std::cout << "i = " << i << ", Genp_id->at(i) = " << Genp_Id->at(i) << ", id = " << id << std::endl;
  std::cout << "iDau = " << iDau << ", Genp_id->at(iDau) = " << Genp_Id->at(iDau) << std::endl;
#endif // mgDEBUG
  //  return std::make_pair<unsigned short, unsigned short>(i,iDau);
  return std::make_pair(i,iDau);
}

std::vector<unsigned short> MCTools::RecursiveFindMuonDaughters(const unsigned short iGenp)
{
  std::vector<unsigned short> vMuDaughters;
  unsigned short nDaus = Genp_vDau->at(iGenp).size();
  for(unsigned short i = 0; i < nDaus; i++)
  {
    unsigned short iDau = Genp_vDau->at(iGenp).at(i);
    if(13 == fabs(Genp_Id->at(iDau)) ) vMuDaughters.push_back(iDau);
    std::vector<unsigned short> vDaughterDaughters = RecursiveFindMuonDaughters(iDau);
    for(unsigned short ii = 0; ii < vDaughterDaughters.size(); ii++)
    {
      if(13 == fabs(Genp_Id->at(ii)) )  vMuDaughters.push_back(vDaughterDaughters[ii]);
    }
  }
  return vMuDaughters;
}


#endif // MCTools_cxx
