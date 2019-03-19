#ifndef EfficiencyFactorsGetter_cxx
#define EfficiencyFactorsGetter_cxx

#include "EfficiencyFactorsGetter.h"

#include <string>
#include "TSystem.h"



double EfficiencyFactorsGetter::GetEfficiency(double pt, double eta, const std::string method)
{
  unsigned long posTk = method.find("Tk");
  if(posTk != std::string::npos)
  {
    return GetTkEfficiency(pt, eta, method.substr(posTk+2));
  }
  
  unsigned long posRecoSel = method.find("RecoSel");
  if(posRecoSel != std::string::npos)
  {
    return GetRecoSelEfficiency(pt, eta, method.substr(posRecoSel+7));
  }
  
  unsigned long posMuSel = method.find("MuSel");
  if(posMuSel != std::string::npos)
  {
    return GetMuSelEfficiency(pt, eta, method.substr(posMuSel+5));
  }
  
  unsigned long posTrg = method.find("Trg");
  if(posTrg != std::string::npos)
  {
    return GetTrgEfficiency(pt, eta, method.substr(posTrg+3));
  }
  
  std::cout << "E R R O R ! Efficiency method \"" << method << "\" is not a recognized one!\n";
  std::cout << "            Exiting...\n";
  exit(1);
}


double EfficiencyFactorsGetter::GetTkEfficiency(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetTkEfficiencyData(pt, eta);
  }
  else if(method.compare("DataLoErr") == 0)
  {
    return GetTkEfficiencyDataLoErr(pt, eta);
  }
  else if(method.compare("DataHiErr") == 0)
  {
    return GetTkEfficiencyDataHiErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetTkEfficiencyMC(pt, eta);
  }
  else if(method.compare("MCLoErr") == 0)
  {
    return GetTkEfficiencyMCLoErr(pt, eta);
  }
  else if(method.compare("MCHiErr") == 0)
  {
    return GetTkEfficiencyMCHiErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetTkEfficiencyMCTruth(pt, eta);
  }
  else if(method.compare("MCTruthLoErr") == 0)
  {
    return GetTkEfficiencyMCTruthLoErr(pt, eta);
  }
  else if(method.compare("MCTruthHiErr") == 0)
  {
    return GetTkEfficiencyMCTruthHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetTkEfficiencyMCTruthBB(pt, eta);
  }
  else if(method.compare("MCTruthBBLoErr") == 0)
  {
    return GetTkEfficiencyMCTruthBBLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBHiErr") == 0)
  {
    return GetTkEfficiencyMCTruthBBHiErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetTkEfficiencyData(double pt, double eta)
{
  if(pt > tkEffDataPtMax)
    pt = tkEffDataPtMax;
  else if(pt < tkEffDataPtMin)
    pt = tkEffDataPtMin;
  if(eta > tkEffDataEtaMax)
    eta = tkEffDataEtaMax;
  else if(eta < tkEffDataEtaMin)
    eta = tkEffDataEtaMin;
  return hTkEffData->GetBinContent(hTkEffData->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTkEfficiencyMC(double pt, double eta)
{
  if(pt > tkEffMCPtMax)
    pt = tkEffMCPtMax;
  else if(pt < tkEffMCPtMin)
    pt = tkEffMCPtMin;
  if(eta > tkEffMCEtaMax)
    eta = tkEffMCEtaMax;
  else if(eta < tkEffMCEtaMin)
    eta = tkEffMCEtaMin;
  return hTkEffMC->GetBinContent(hTkEffMC->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTkEfficiencyMCTruth(double pt, double eta)
{
  if(pt > tkEffMCTruthPtMax)
    pt = tkEffMCTruthPtMax;
  else if(pt < tkEffMCTruthPtMin)
    pt = tkEffMCTruthPtMin;
  if(eta > tkEffMCTruthEtaMax)
    eta = tkEffMCTruthEtaMax;
  else if(eta < tkEffMCTruthEtaMin)
    eta = tkEffMCTruthEtaMin;
  return hTkEffMCTruth->GetBinContent(hTkEffMCTruth->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTkEfficiencyMCTruthBB(double pt, double eta)
{
  if(pt > tkEffMCTruthBBPtMax)
    pt = tkEffMCTruthBBPtMax;
  else if(pt < tkEffMCTruthBBPtMin)
    pt = tkEffMCTruthBBPtMin;
  if(eta > tkEffMCTruthBBEtaMax)
    eta = tkEffMCTruthBBEtaMax;
  else if(eta < tkEffMCTruthBBEtaMin)
    eta = tkEffMCTruthBBEtaMin;
  return hTkEffMCTruthBB->GetBinContent(hTkEffMCTruthBB->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTkEfficiencyHiErr(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetTkEfficiencyDataHiErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetTkEfficiencyMCHiErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetTkEfficiencyMCTruthHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetTkEfficiencyMCTruthBBHiErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetTkEfficiencyDataHiErr(double pt, double eta)
{
  if(pt > tkEffDataPtMax)
    pt = tkEffDataPtMax;
  else if(pt < tkEffDataPtMin)
    pt = tkEffDataPtMin;
  if(eta > tkEffDataEtaMax)
    eta = tkEffDataEtaMax;
  else if(eta < tkEffDataEtaMin)
    eta = tkEffDataEtaMin;
  return std::min(hTkEffData->GetBinContent(hTkEffData->FindFixBin(eta, pt)) + hTkEffData_Hi->GetBinContent(hTkEffData_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTkEfficiencyMCHiErr(double pt, double eta)
{
  if(pt > tkEffMCPtMax)
    pt = tkEffMCPtMax;
  else if(pt < tkEffMCPtMin)
    pt = tkEffMCPtMin;
  if(eta > tkEffMCEtaMax)
    eta = tkEffMCEtaMax;
  else if(eta < tkEffMCEtaMin)
    eta = tkEffMCEtaMin;
  return std::min(hTkEffMC->GetBinContent(hTkEffMC->FindFixBin(eta, pt)) + hTkEffMC_Hi->GetBinContent(hTkEffMC_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTkEfficiencyMCTruthHiErr(double pt, double eta)
{
  if(pt > tkEffMCTruthPtMax)
    pt = tkEffMCTruthPtMax;
  else if(pt < tkEffMCTruthPtMin)
    pt = tkEffMCTruthPtMin;
  if(eta > tkEffMCTruthEtaMax)
    eta = tkEffMCTruthEtaMax;
  else if(eta < tkEffMCTruthEtaMin)
    eta = tkEffMCTruthEtaMin;
  return std::min(hTkEffMCTruth->GetBinContent(hTkEffMCTruth->FindFixBin(eta, pt)) + hTkEffMCTruth_Hi->GetBinContent(hTkEffMCTruth_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTkEfficiencyMCTruthBBHiErr(double pt, double eta)
{
  if(pt > tkEffMCTruthBBPtMax)
    pt = tkEffMCTruthBBPtMax;
  else if(pt < tkEffMCTruthBBPtMin)
    pt = tkEffMCTruthBBPtMin;
  if(eta > tkEffMCTruthBBEtaMax)
    eta = tkEffMCTruthBBEtaMax;
  else if(eta < tkEffMCTruthBBEtaMin)
    eta = tkEffMCTruthBBEtaMin;
  return std::min(hTkEffMCTruthBB->GetBinContent(hTkEffMCTruthBB->FindFixBin(eta, pt)) + hTkEffMCTruthBB_Hi->GetBinContent(hTkEffMCTruthBB_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTkEfficiencyLoErr(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetTkEfficiencyDataLoErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetTkEfficiencyMCLoErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetTkEfficiencyMCTruthLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetTkEfficiencyMCTruthBBLoErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetTkEfficiencyDataLoErr(double pt, double eta)
{
  if(pt > tkEffDataPtMax)
    pt = tkEffDataPtMax;
  else if(pt < tkEffDataPtMin)
    pt = tkEffDataPtMin;
  if(eta > tkEffDataEtaMax)
    eta = tkEffDataEtaMax;
  else if(eta < tkEffDataEtaMin)
    eta = tkEffDataEtaMin;
  return std::max(hTkEffData->GetBinContent(hTkEffData->FindFixBin(eta, pt)) - fabs(hTkEffData_Lo->GetBinContent(hTkEffData_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetTkEfficiencyMCLoErr(double pt, double eta)
{
  if(pt > tkEffMCPtMax)
    pt = tkEffMCPtMax;
  else if(pt < tkEffMCPtMin)
    pt = tkEffMCPtMin;
  if(eta > tkEffMCEtaMax)
    eta = tkEffMCEtaMax;
  else if(eta < tkEffMCEtaMin)
    eta = tkEffMCEtaMin;
  return std::max(hTkEffMC->GetBinContent(hTkEffMC->FindFixBin(eta, pt)) - fabs(hTkEffMC_Lo->GetBinContent(hTkEffMC_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetTkEfficiencyMCTruthLoErr(double pt, double eta)
{
  if(pt > tkEffMCTruthPtMax)
    pt = tkEffMCTruthPtMax;
  else if(pt < tkEffMCTruthPtMin)
    pt = tkEffMCTruthPtMin;
  if(eta > tkEffMCTruthEtaMax)
    eta = tkEffMCTruthEtaMax;
  else if(eta < tkEffMCTruthEtaMin)
    eta = tkEffMCTruthEtaMin;
  return std::max(hTkEffMCTruth->GetBinContent(hTkEffMCTruth->FindFixBin(eta, pt)) - fabs(hTkEffMCTruth_Lo->GetBinContent(hTkEffMCTruth_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetTkEfficiencyMCTruthBBLoErr(double pt, double eta)
{
  if(pt > tkEffMCTruthBBPtMax)
    pt = tkEffMCTruthBBPtMax;
  else if(pt < tkEffMCTruthBBPtMin)
    pt = tkEffMCTruthBBPtMin;
  if(eta > tkEffMCTruthBBEtaMax)
    eta = tkEffMCTruthBBEtaMax;
  else if(eta < tkEffMCTruthBBEtaMin)
    eta = tkEffMCTruthBBEtaMin;
  return std::max(hTkEffMCTruthBB->GetBinContent(hTkEffMCTruthBB->FindFixBin(eta, pt)) - fabs(hTkEffMCTruthBB_Lo->GetBinContent(hTkEffMCTruthBB_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiency(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetRecoSelEfficiencyData(pt, eta);
  }
  if(method.compare("DataLoErr") == 0)
  {
    return GetRecoSelEfficiencyDataLoErr(pt, eta);
  }
  if(method.compare("DataHiErr") == 0)
  {
    return GetRecoSelEfficiencyDataHiErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetRecoSelEfficiencyMC(pt, eta);
  }
  else if(method.compare("MCLoErr") == 0)
  {
    return GetRecoSelEfficiencyMCLoErr(pt, eta);
  }
  else if(method.compare("MCHiErr") == 0)
  {
    return GetRecoSelEfficiencyMCHiErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetRecoSelEfficiencyMCTruth(pt, eta);
  }
  else if(method.compare("MCTruthLoErr") == 0)
  {
    return GetRecoSelEfficiencyMCTruthLoErr(pt, eta);
  }
  else if(method.compare("MCTruthHiErr") == 0)
  {
    return GetRecoSelEfficiencyMCTruthHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBB(pt, eta);
  }
  else if(method.compare("MCTruthBBLoErr") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBHiErr") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBGen(pt, eta);
  }
  else if(method.compare("MCTruthBBGenLoErr") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBGenLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGenHiErr") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBGenHiErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyData(double pt, double eta)
{
  if(pt > recoSelEffDataPtMax)
    pt = recoSelEffDataPtMax;
  else if(pt < recoSelEffDataPtMin)
    pt = recoSelEffDataPtMin;
  if(eta > recoSelEffDataEtaMax)
    eta = recoSelEffDataEtaMax;
  else if(eta < recoSelEffDataEtaMin)
    eta = recoSelEffDataEtaMin;
  return hRecoSelEffData->GetBinContent(hRecoSelEffData->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMC(double pt, double eta)
{
  if(pt > recoSelEffMCPtMax)
    pt = recoSelEffMCPtMax;
  else if(pt < recoSelEffMCPtMin)
    pt = recoSelEffMCPtMin;
  if(eta > recoSelEffMCEtaMax)
    eta = recoSelEffMCEtaMax;
  else if(eta < recoSelEffMCEtaMin)
    eta = recoSelEffMCEtaMin;
  return hRecoSelEffMC->GetBinContent(hRecoSelEffMC->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruth(double pt, double eta)
{
  if(pt > recoSelEffMCTruthPtMax)
    pt = recoSelEffMCTruthPtMax;
  else if(pt < recoSelEffMCTruthPtMin)
    pt = recoSelEffMCTruthPtMin;
  if(eta > recoSelEffMCTruthEtaMax)
    eta = recoSelEffMCTruthEtaMax;
  else if(eta < recoSelEffMCTruthEtaMin)
    eta = recoSelEffMCTruthEtaMin;
  return hRecoSelEffMCTruth->GetBinContent(hRecoSelEffMCTruth->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruthBB(double pt, double eta)
{
  if(pt > recoSelEffMCTruthBBPtMax)
    pt = recoSelEffMCTruthBBPtMax;
  else if(pt < recoSelEffMCTruthBBPtMin)
    pt = recoSelEffMCTruthBBPtMin;
  if(eta > recoSelEffMCTruthBBEtaMax)
    eta = recoSelEffMCTruthBBEtaMax;
  else if(eta < recoSelEffMCTruthBBEtaMin)
    eta = recoSelEffMCTruthBBEtaMin;
  return hRecoSelEffMCTruthBB->GetBinContent(hRecoSelEffMCTruthBB->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruthBBGen(double pt, double eta)
{
  if(pt > recoSelEffMCTruthBBGenPtMax)
    pt = recoSelEffMCTruthBBGenPtMax;
  else if(pt < recoSelEffMCTruthBBGenPtMin)
    pt = recoSelEffMCTruthBBGenPtMin;
  if(eta > recoSelEffMCTruthBBGenEtaMax)
    eta = recoSelEffMCTruthBBGenEtaMax;
  else if(eta < recoSelEffMCTruthBBGenEtaMin)
    eta = recoSelEffMCTruthBBGenEtaMin;
  return hRecoSelEffMCTruthBBGen->GetBinContent(hRecoSelEffMCTruthBBGen->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyHiErr(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetRecoSelEfficiencyDataHiErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetRecoSelEfficiencyMCHiErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetRecoSelEfficiencyMCTruthHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBGenHiErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyDataHiErr(double pt, double eta)
{
  if(pt > recoSelEffDataPtMax)
    pt = recoSelEffDataPtMax;
  else if(pt < recoSelEffDataPtMin)
    pt = recoSelEffDataPtMin;
  if(eta > recoSelEffDataEtaMax)
    eta = recoSelEffDataEtaMax;
  else if(eta < recoSelEffDataEtaMin)
    eta = recoSelEffDataEtaMin;
  return std::min(hRecoSelEffData->GetBinContent(hRecoSelEffData->FindFixBin(eta, pt)) + hRecoSelEffData_Hi->GetBinContent(hRecoSelEffData_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCHiErr(double pt, double eta)
{
  if(pt > recoSelEffMCPtMax)
    pt = recoSelEffMCPtMax;
  else if(pt < recoSelEffMCPtMin)
    pt = recoSelEffMCPtMin;
  if(eta > recoSelEffMCEtaMax)
    eta = recoSelEffMCEtaMax;
  else if(eta < recoSelEffMCEtaMin)
    eta = recoSelEffMCEtaMin;
  return std::min(hRecoSelEffMC->GetBinContent(hRecoSelEffMC->FindFixBin(eta, pt)) + hRecoSelEffMC_Hi->GetBinContent(hRecoSelEffMC_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruthHiErr(double pt, double eta)
{
  if(pt > recoSelEffMCTruthPtMax)
    pt = recoSelEffMCTruthPtMax;
  else if(pt < recoSelEffMCTruthPtMin)
    pt = recoSelEffMCTruthPtMin;
  if(eta > recoSelEffMCTruthEtaMax)
    eta = recoSelEffMCTruthEtaMax;
  else if(eta < recoSelEffMCTruthEtaMin)
    eta = recoSelEffMCTruthEtaMin;
  return std::min(hRecoSelEffMCTruth->GetBinContent(hRecoSelEffMCTruth->FindFixBin(eta, pt)) + hRecoSelEffMCTruth_Hi->GetBinContent(hRecoSelEffMCTruth_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruthBBHiErr(double pt, double eta)
{
  if(pt > recoSelEffMCTruthBBPtMax)
    pt = recoSelEffMCTruthBBPtMax;
  else if(pt < recoSelEffMCTruthBBPtMin)
    pt = recoSelEffMCTruthBBPtMin;
  if(eta > recoSelEffMCTruthBBEtaMax)
    eta = recoSelEffMCTruthBBEtaMax;
  else if(eta < recoSelEffMCTruthBBEtaMin)
    eta = recoSelEffMCTruthBBEtaMin;
  return std::min(hRecoSelEffMCTruthBB->GetBinContent(hRecoSelEffMCTruthBB->FindFixBin(eta, pt)) + hRecoSelEffMCTruthBB_Hi->GetBinContent(hRecoSelEffMCTruthBB_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruthBBGenHiErr(double pt, double eta)
{
  if(pt > recoSelEffMCTruthBBGenPtMax)
    pt = recoSelEffMCTruthBBGenPtMax;
  else if(pt < recoSelEffMCTruthBBGenPtMin)
    pt = recoSelEffMCTruthBBGenPtMin;
  if(eta > recoSelEffMCTruthBBGenEtaMax)
    eta = recoSelEffMCTruthBBGenEtaMax;
  else if(eta < recoSelEffMCTruthBBGenEtaMin)
    eta = recoSelEffMCTruthBBGenEtaMin;
  return std::min(hRecoSelEffMCTruthBBGen->GetBinContent(hRecoSelEffMCTruthBBGen->FindFixBin(eta, pt)) + hRecoSelEffMCTruthBBGen_Hi->GetBinContent(hRecoSelEffMCTruthBBGen_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyLoErr(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetRecoSelEfficiencyDataLoErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetRecoSelEfficiencyMCLoErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetRecoSelEfficiencyMCTruthLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetRecoSelEfficiencyMCTruthBBGenLoErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyDataLoErr(double pt, double eta)
{
  if(pt > recoSelEffDataPtMax)
    pt = recoSelEffDataPtMax;
  else if(pt < recoSelEffDataPtMin)
    pt = recoSelEffDataPtMin;
  if(eta > recoSelEffDataEtaMax)
    eta = recoSelEffDataEtaMax;
  else if(eta < recoSelEffDataEtaMin)
    eta = recoSelEffDataEtaMin;
  return std::max(hRecoSelEffData->GetBinContent(hRecoSelEffData->FindFixBin(eta, pt)) - fabs(hRecoSelEffData_Lo->GetBinContent(hRecoSelEffData_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCLoErr(double pt, double eta)
{
  if(pt > recoSelEffMCPtMax)
    pt = recoSelEffMCPtMax;
  else if(pt < recoSelEffMCPtMin)
    pt = recoSelEffMCPtMin;
  if(eta > recoSelEffMCEtaMax)
    eta = recoSelEffMCEtaMax;
  else if(eta < recoSelEffMCEtaMin)
    eta = recoSelEffMCEtaMin;
  return std::max(hRecoSelEffMC->GetBinContent(hRecoSelEffMC->FindFixBin(eta, pt)) - fabs(hRecoSelEffMC_Lo->GetBinContent(hRecoSelEffMC_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruthLoErr(double pt, double eta)
{
  if(pt > recoSelEffMCTruthPtMax)
    pt = recoSelEffMCTruthPtMax;
  else if(pt < recoSelEffMCTruthPtMin)
    pt = recoSelEffMCTruthPtMin;
  if(eta > recoSelEffMCTruthEtaMax)
    eta = recoSelEffMCTruthEtaMax;
  else if(eta < recoSelEffMCTruthEtaMin)
    eta = recoSelEffMCTruthEtaMin;
  return std::max(hRecoSelEffMCTruth->GetBinContent(hRecoSelEffMCTruth->FindFixBin(eta, pt)) - fabs(hRecoSelEffMCTruth_Lo->GetBinContent(hRecoSelEffMCTruth_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruthBBLoErr(double pt, double eta)
{
  if(pt > recoSelEffMCTruthBBPtMax)
    pt = recoSelEffMCTruthBBPtMax;
  else if(pt < recoSelEffMCTruthBBPtMin)
    pt = recoSelEffMCTruthBBPtMin;
  if(eta > recoSelEffMCTruthBBEtaMax)
    eta = recoSelEffMCTruthBBEtaMax;
  else if(eta < recoSelEffMCTruthBBEtaMin)
    eta = recoSelEffMCTruthBBEtaMin;
  return std::max(hRecoSelEffMCTruthBB->GetBinContent(hRecoSelEffMCTruthBB->FindFixBin(eta, pt)) - fabs(hRecoSelEffMCTruthBB_Lo->GetBinContent(hRecoSelEffMCTruthBB_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetRecoSelEfficiencyMCTruthBBGenLoErr(double pt, double eta)
{
  if(pt > recoSelEffMCTruthBBGenPtMax)
    pt = recoSelEffMCTruthBBGenPtMax;
  else if(pt < recoSelEffMCTruthBBGenPtMin)
    pt = recoSelEffMCTruthBBGenPtMin;
  if(eta > recoSelEffMCTruthBBGenEtaMax)
    eta = recoSelEffMCTruthBBGenEtaMax;
  else if(eta < recoSelEffMCTruthBBGenEtaMin)
    eta = recoSelEffMCTruthBBGenEtaMin;
  return std::max(hRecoSelEffMCTruthBBGen->GetBinContent(hRecoSelEffMCTruthBBGen->FindFixBin(eta, pt)) - fabs(hRecoSelEffMCTruthBBGen_Lo->GetBinContent(hRecoSelEffMCTruthBBGen_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiency(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetMuSelEfficiencyData(pt, eta);
  }
  if(method.compare("DataLoErr") == 0)
  {
    return GetMuSelEfficiencyDataLoErr(pt, eta);
  }
  if(method.compare("DataHiErr") == 0)
  {
    return GetMuSelEfficiencyDataHiErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetMuSelEfficiencyMC(pt, eta);
  }
  else if(method.compare("MCLoErr") == 0)
  {
    return GetMuSelEfficiencyMCLoErr(pt, eta);
  }
  else if(method.compare("MCHiErr") == 0)
  {
    return GetMuSelEfficiencyMCHiErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetMuSelEfficiencyMCTruth(pt, eta);
  }
  else if(method.compare("MCTruthLoErr") == 0)
  {
    return GetMuSelEfficiencyMCTruthLoErr(pt, eta);
  }
  else if(method.compare("MCTruthHiErr") == 0)
  {
    return GetMuSelEfficiencyMCTruthHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetMuSelEfficiencyMCTruthBB(pt, eta);
  }
  else if(method.compare("MCTruthBBLoErr") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBHiErr") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBGen(pt, eta);
  }
  else if(method.compare("MCTruthBBGenLoErr") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBGenLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGenHiErr") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBGenHiErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyData(double pt, double eta)
{
  if(pt > muSelEffDataPtMax)
    pt = muSelEffDataPtMax;
  else if(pt < muSelEffDataPtMin)
    pt = muSelEffDataPtMin;
  if(eta > muSelEffDataEtaMax)
    eta = muSelEffDataEtaMax;
  else if(eta < muSelEffDataEtaMin)
    eta = muSelEffDataEtaMin;
  return hMuSelEffData->GetBinContent(hMuSelEffData->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMC(double pt, double eta)
{
  if(pt > muSelEffMCPtMax)
    pt = muSelEffMCPtMax;
  else if(pt < muSelEffMCPtMin)
    pt = muSelEffMCPtMin;
  if(eta > muSelEffMCEtaMax)
    eta = muSelEffMCEtaMax;
  else if(eta < muSelEffMCEtaMin)
    eta = muSelEffMCEtaMin;
  return hMuSelEffMC->GetBinContent(hMuSelEffMC->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruth(double pt, double eta)
{
  if(pt > muSelEffMCTruthPtMax)
    pt = muSelEffMCTruthPtMax;
  else if(pt < muSelEffMCTruthPtMin)
    pt = muSelEffMCTruthPtMin;
  if(eta > muSelEffMCTruthEtaMax)
    eta = muSelEffMCTruthEtaMax;
  else if(eta < muSelEffMCTruthEtaMin)
    eta = muSelEffMCTruthEtaMin;
  return hMuSelEffMCTruth->GetBinContent(hMuSelEffMCTruth->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruthBB(double pt, double eta)
{
  if(pt > muSelEffMCTruthBBPtMax)
    pt = muSelEffMCTruthBBPtMax;
  else if(pt < muSelEffMCTruthBBPtMin)
    pt = muSelEffMCTruthBBPtMin;
  if(eta > muSelEffMCTruthBBEtaMax)
    eta = muSelEffMCTruthBBEtaMax;
  else if(eta < muSelEffMCTruthBBEtaMin)
    eta = muSelEffMCTruthBBEtaMin;
  return hMuSelEffMCTruthBB->GetBinContent(hMuSelEffMCTruthBB->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruthBBGen(double pt, double eta)
{
  if(pt > muSelEffMCTruthBBGenPtMax)
    pt = muSelEffMCTruthBBGenPtMax;
  else if(pt < muSelEffMCTruthBBGenPtMin)
    pt = muSelEffMCTruthBBGenPtMin;
  if(eta > muSelEffMCTruthBBGenEtaMax)
    eta = muSelEffMCTruthBBGenEtaMax;
  else if(eta < muSelEffMCTruthBBGenEtaMin)
    eta = muSelEffMCTruthBBGenEtaMin;
  return hMuSelEffMCTruthBBGen->GetBinContent(hMuSelEffMCTruthBBGen->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyHiErr(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetMuSelEfficiencyDataHiErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetMuSelEfficiencyMCHiErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetMuSelEfficiencyMCTruthHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBGenHiErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyDataHiErr(double pt, double eta)
{
  if(pt > muSelEffDataPtMax)
    pt = muSelEffDataPtMax;
  else if(pt < muSelEffDataPtMin)
    pt = muSelEffDataPtMin;
  if(eta > muSelEffDataEtaMax)
    eta = muSelEffDataEtaMax;
  else if(eta < muSelEffDataEtaMin)
    eta = muSelEffDataEtaMin;
  return std::min(hMuSelEffData->GetBinContent(hMuSelEffData->FindFixBin(eta, pt)) + hMuSelEffData_Hi->GetBinContent(hMuSelEffData_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCHiErr(double pt, double eta)
{
  if(pt > muSelEffMCPtMax)
    pt = muSelEffMCPtMax;
  else if(pt < muSelEffMCPtMin)
    pt = muSelEffMCPtMin;
  if(eta > muSelEffMCEtaMax)
    eta = muSelEffMCEtaMax;
  else if(eta < muSelEffMCEtaMin)
    eta = muSelEffMCEtaMin;
  return std::min(hMuSelEffMC->GetBinContent(hMuSelEffMC->FindFixBin(eta, pt)) + hMuSelEffMC_Hi->GetBinContent(hMuSelEffMC_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruthHiErr(double pt, double eta)
{
  if(pt > muSelEffMCTruthPtMax)
    pt = muSelEffMCTruthPtMax;
  else if(pt < muSelEffMCTruthPtMin)
    pt = muSelEffMCTruthPtMin;
  if(eta > muSelEffMCTruthEtaMax)
    eta = muSelEffMCTruthEtaMax;
  else if(eta < muSelEffMCTruthEtaMin)
    eta = muSelEffMCTruthEtaMin;
  return std::min(hMuSelEffMCTruth->GetBinContent(hMuSelEffMCTruth->FindFixBin(eta, pt)) + hMuSelEffMCTruth_Hi->GetBinContent(hMuSelEffMCTruth_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruthBBHiErr(double pt, double eta)
{
  if(pt > muSelEffMCTruthBBPtMax)
    pt = muSelEffMCTruthBBPtMax;
  else if(pt < muSelEffMCTruthBBPtMin)
    pt = muSelEffMCTruthBBPtMin;
  if(eta > muSelEffMCTruthBBEtaMax)
    eta = muSelEffMCTruthBBEtaMax;
  else if(eta < muSelEffMCTruthBBEtaMin)
    eta = muSelEffMCTruthBBEtaMin;
  return std::min(hMuSelEffMCTruthBB->GetBinContent(hMuSelEffMCTruthBB->FindFixBin(eta, pt)) + hMuSelEffMCTruthBB_Hi->GetBinContent(hMuSelEffMCTruthBB_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruthBBGenHiErr(double pt, double eta)
{
  if(pt > muSelEffMCTruthBBGenPtMax)
    pt = muSelEffMCTruthBBGenPtMax;
  else if(pt < muSelEffMCTruthBBGenPtMin)
    pt = muSelEffMCTruthBBGenPtMin;
  if(eta > muSelEffMCTruthBBGenEtaMax)
    eta = muSelEffMCTruthBBGenEtaMax;
  else if(eta < muSelEffMCTruthBBGenEtaMin)
    eta = muSelEffMCTruthBBGenEtaMin;
  return std::min(hMuSelEffMCTruthBBGen->GetBinContent(hMuSelEffMCTruthBBGen->FindFixBin(eta, pt)) + hMuSelEffMCTruthBBGen_Hi->GetBinContent(hMuSelEffMCTruthBBGen_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyLoErr(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetMuSelEfficiencyDataLoErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetMuSelEfficiencyMCLoErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetMuSelEfficiencyMCTruthLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetMuSelEfficiencyMCTruthBBGenLoErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyDataLoErr(double pt, double eta)
{
  if(pt > muSelEffDataPtMax)
    pt = muSelEffDataPtMax;
  else if(pt < muSelEffDataPtMin)
    pt = muSelEffDataPtMin;
  if(eta > muSelEffDataEtaMax)
    eta = muSelEffDataEtaMax;
  else if(eta < muSelEffDataEtaMin)
    eta = muSelEffDataEtaMin;
  return std::max(hMuSelEffData->GetBinContent(hMuSelEffData->FindFixBin(eta, pt)) - fabs(hMuSelEffData_Lo->GetBinContent(hMuSelEffData_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCLoErr(double pt, double eta)
{
  if(pt > muSelEffMCPtMax)
    pt = muSelEffMCPtMax;
  else if(pt < muSelEffMCPtMin)
    pt = muSelEffMCPtMin;
  if(eta > muSelEffMCEtaMax)
    eta = muSelEffMCEtaMax;
  else if(eta < muSelEffMCEtaMin)
    eta = muSelEffMCEtaMin;
  return std::max(hMuSelEffMC->GetBinContent(hMuSelEffMC->FindFixBin(eta, pt)) - fabs(hMuSelEffMC_Lo->GetBinContent(hMuSelEffMC_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruthLoErr(double pt, double eta)
{
  if(pt > muSelEffMCTruthPtMax)
    pt = muSelEffMCTruthPtMax;
  else if(pt < muSelEffMCTruthPtMin)
    pt = muSelEffMCTruthPtMin;
  if(eta > muSelEffMCTruthEtaMax)
    eta = muSelEffMCTruthEtaMax;
  else if(eta < muSelEffMCTruthEtaMin)
    eta = muSelEffMCTruthEtaMin;
  return std::max(hMuSelEffMCTruth->GetBinContent(hMuSelEffMCTruth->FindFixBin(eta, pt)) - fabs(hMuSelEffMCTruth_Lo->GetBinContent(hMuSelEffMCTruth_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruthBBLoErr(double pt, double eta)
{
  if(pt > muSelEffMCTruthBBPtMax)
    pt = muSelEffMCTruthBBPtMax;
  else if(pt < muSelEffMCTruthBBPtMin)
    pt = muSelEffMCTruthBBPtMin;
  if(eta > muSelEffMCTruthBBEtaMax)
    eta = muSelEffMCTruthBBEtaMax;
  else if(eta < muSelEffMCTruthBBEtaMin)
    eta = muSelEffMCTruthBBEtaMin;
  return std::max(hMuSelEffMCTruthBB->GetBinContent(hMuSelEffMCTruthBB->FindFixBin(eta, pt)) - fabs(hMuSelEffMCTruthBB_Lo->GetBinContent(hMuSelEffMCTruthBB_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetMuSelEfficiencyMCTruthBBGenLoErr(double pt, double eta)
{
  if(pt > muSelEffMCTruthBBGenPtMax)
    pt = muSelEffMCTruthBBGenPtMax;
  else if(pt < muSelEffMCTruthBBGenPtMin)
    pt = muSelEffMCTruthBBGenPtMin;
  if(eta > muSelEffMCTruthBBGenEtaMax)
    eta = muSelEffMCTruthBBGenEtaMax;
  else if(eta < muSelEffMCTruthBBGenEtaMin)
    eta = muSelEffMCTruthBBGenEtaMin;
  return std::max(hMuSelEffMCTruthBBGen->GetBinContent(hMuSelEffMCTruthBBGen->FindFixBin(eta, pt)) - fabs(hMuSelEffMCTruthBBGen_Lo->GetBinContent(hMuSelEffMCTruthBBGen_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetTrgEfficiency(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetTrgEfficiencyData(pt, eta);
  }
  if(method.compare("DataLoErr") == 0)
  {
    return GetTrgEfficiencyDataLoErr(pt, eta);
  }
  if(method.compare("DataHiErr") == 0)
  {
    return GetTrgEfficiencyDataHiErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetTrgEfficiencyMC(pt, eta);
  }
  else if(method.compare("MCLoErr") == 0)
  {
    return GetTrgEfficiencyMCLoErr(pt, eta);
  }
  else if(method.compare("MCHiErr") == 0)
  {
    return GetTrgEfficiencyMCHiErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetTrgEfficiencyMCTruth(pt, eta);
  }
  else if(method.compare("MCTruthLoErr") == 0)
  {
    return GetTrgEfficiencyMCTruthLoErr(pt, eta);
  }
  else if(method.compare("MCTruthHiErr") == 0)
  {
    return GetTrgEfficiencyMCTruthHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetTrgEfficiencyMCTruthBB(pt, eta);
  }
  else if(method.compare("MCTruthBBLoErr") == 0)
  {
    return GetTrgEfficiencyMCTruthBBLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBHiErr") == 0)
  {
    return GetTrgEfficiencyMCTruthBBHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetTrgEfficiencyMCTruthBBGen(pt, eta);
  }
  else if(method.compare("MCTruthBBGenLoErr") == 0)
  {
    return GetTrgEfficiencyMCTruthBBGenLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGenHiErr") == 0)
  {
    return GetTrgEfficiencyMCTruthBBGenHiErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetTrgEfficiencyData(double pt, double eta)
{
  if(pt > trgEffDataPtMax)
    pt = trgEffDataPtMax;
  else if(pt < trgEffDataPtMin)
    pt = trgEffDataPtMin;
  if(eta > trgEffDataEtaMax)
    eta = trgEffDataEtaMax;
  else if(eta < trgEffDataEtaMin)
    eta = trgEffDataEtaMin;
  return hTrgEffData->GetBinContent(hTrgEffData->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMC(double pt, double eta)
{
  if(pt > trgEffMCPtMax)
    pt = trgEffMCPtMax;
  else if(pt < trgEffMCPtMin)
    pt = trgEffMCPtMin;
  if(eta > trgEffMCEtaMax)
    eta = trgEffMCEtaMax;
  else if(eta < trgEffMCEtaMin)
    eta = trgEffMCEtaMin;
  return hTrgEffMC->GetBinContent(hTrgEffMC->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruth(double pt, double eta)
{
  if(pt > trgEffMCTruthPtMax)
    pt = trgEffMCTruthPtMax;
  else if(pt < trgEffMCTruthPtMin)
    pt = trgEffMCTruthPtMin;
  if(eta > trgEffMCTruthEtaMax)
    eta = trgEffMCTruthEtaMax;
  else if(eta < trgEffMCTruthEtaMin)
    eta = trgEffMCTruthEtaMin;
  return hTrgEffMCTruth->GetBinContent(hTrgEffMCTruth->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruthBB(double pt, double eta)
{
  if(pt > trgEffMCTruthBBPtMax)
    pt = trgEffMCTruthBBPtMax;
  else if(pt < trgEffMCTruthBBPtMin)
    pt = trgEffMCTruthBBPtMin;
  if(eta > trgEffMCTruthBBEtaMax)
    eta = trgEffMCTruthBBEtaMax;
  else if(eta < trgEffMCTruthBBEtaMin)
    eta = trgEffMCTruthBBEtaMin;
  return hTrgEffMCTruthBB->GetBinContent(hTrgEffMCTruthBB->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruthBBGen(double pt, double eta)
{
  if(pt > trgEffMCTruthBBGenPtMax)
    pt = trgEffMCTruthBBGenPtMax;
  else if(pt < trgEffMCTruthBBGenPtMin)
    pt = trgEffMCTruthBBGenPtMin;
  if(eta > trgEffMCTruthBBGenEtaMax)
    eta = trgEffMCTruthBBGenEtaMax;
  else if(eta < trgEffMCTruthBBGenEtaMin)
    eta = trgEffMCTruthBBGenEtaMin;
  return hTrgEffMCTruthBBGen->GetBinContent(hTrgEffMCTruthBBGen->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetTrgEfficiencyHiErr(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetTrgEfficiencyDataHiErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetTrgEfficiencyMCHiErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetTrgEfficiencyMCTruthHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetTrgEfficiencyMCTruthBBHiErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetTrgEfficiencyMCTruthBBGenHiErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetTrgEfficiencyDataHiErr(double pt, double eta)
{
  if(pt > trgEffDataPtMax)
    pt = trgEffDataPtMax;
  else if(pt < trgEffDataPtMin)
    pt = trgEffDataPtMin;
  if(eta > trgEffDataEtaMax)
    eta = trgEffDataEtaMax;
  else if(eta < trgEffDataEtaMin)
    eta = trgEffDataEtaMin;
  return std::min(hTrgEffData->GetBinContent(hTrgEffData->FindFixBin(eta, pt)) + hTrgEffData_Hi->GetBinContent(hTrgEffData_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCHiErr(double pt, double eta)
{
  if(pt > trgEffMCPtMax)
    pt = trgEffMCPtMax;
  else if(pt < trgEffMCPtMin)
    pt = trgEffMCPtMin;
  if(eta > trgEffMCEtaMax)
    eta = trgEffMCEtaMax;
  else if(eta < trgEffMCEtaMin)
    eta = trgEffMCEtaMin;
  return std::min(hTrgEffMC->GetBinContent(hTrgEffMC->FindFixBin(eta, pt)) + hTrgEffMC_Hi->GetBinContent(hTrgEffMC_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruthHiErr(double pt, double eta)
{
  if(pt > trgEffMCTruthPtMax)
    pt = trgEffMCTruthPtMax;
  else if(pt < trgEffMCTruthPtMin)
    pt = trgEffMCTruthPtMin;
  if(eta > trgEffMCTruthEtaMax)
    eta = trgEffMCTruthEtaMax;
  else if(eta < trgEffMCTruthEtaMin)
    eta = trgEffMCTruthEtaMin;
  return std::min(hTrgEffMCTruth->GetBinContent(hTrgEffMCTruth->FindFixBin(eta, pt)) + hTrgEffMCTruth_Hi->GetBinContent(hTrgEffMCTruth_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruthBBHiErr(double pt, double eta)
{
  if(pt > trgEffMCTruthBBPtMax)
    pt = trgEffMCTruthBBPtMax;
  else if(pt < trgEffMCTruthBBPtMin)
    pt = trgEffMCTruthBBPtMin;
  if(eta > trgEffMCTruthBBEtaMax)
    eta = trgEffMCTruthBBEtaMax;
  else if(eta < trgEffMCTruthBBEtaMin)
    eta = trgEffMCTruthBBEtaMin;
  return std::min(hTrgEffMCTruthBB->GetBinContent(hTrgEffMCTruthBB->FindFixBin(eta, pt)) + hTrgEffMCTruthBB_Hi->GetBinContent(hTrgEffMCTruthBB_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruthBBGenHiErr(double pt, double eta)
{
  if(pt > trgEffMCTruthBBGenPtMax)
    pt = trgEffMCTruthBBGenPtMax;
  else if(pt < trgEffMCTruthBBGenPtMin)
    pt = trgEffMCTruthBBGenPtMin;
  if(eta > trgEffMCTruthBBGenEtaMax)
    eta = trgEffMCTruthBBGenEtaMax;
  else if(eta < trgEffMCTruthBBGenEtaMin)
    eta = trgEffMCTruthBBGenEtaMin;
  return std::min(hTrgEffMCTruthBBGen->GetBinContent(hTrgEffMCTruthBBGen->FindFixBin(eta, pt)) + hTrgEffMCTruthBBGen_Hi->GetBinContent(hTrgEffMCTruthBBGen_Hi->FindFixBin(eta, pt)),1.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyLoErr(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetTrgEfficiencyDataLoErr(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetTrgEfficiencyMCLoErr(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetTrgEfficiencyMCTruthLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetTrgEfficiencyMCTruthBBLoErr(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetTrgEfficiencyMCTruthBBGenLoErr(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetTrgEfficiencyDataLoErr(double pt, double eta)
{
  if(pt > trgEffDataPtMax)
    pt = trgEffDataPtMax;
  else if(pt < trgEffDataPtMin)
    pt = trgEffDataPtMin;
  if(eta > trgEffDataEtaMax)
    eta = trgEffDataEtaMax;
  else if(eta < trgEffDataEtaMin)
    eta = trgEffDataEtaMin;
  return std::max(hTrgEffData->GetBinContent(hTrgEffData->FindFixBin(eta, pt)) - fabs(hTrgEffData_Lo->GetBinContent(hTrgEffData_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCLoErr(double pt, double eta)
{
  if(pt > trgEffMCPtMax)
    pt = trgEffMCPtMax;
  else if(pt < trgEffMCPtMin)
    pt = trgEffMCPtMin;
  if(eta > trgEffMCEtaMax)
    eta = trgEffMCEtaMax;
  else if(eta < trgEffMCEtaMin)
    eta = trgEffMCEtaMin;
  return std::max(hTrgEffMC->GetBinContent(hTrgEffMC->FindFixBin(eta, pt)) - fabs(hTrgEffMC_Lo->GetBinContent(hTrgEffMC_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruthLoErr(double pt, double eta)
{
  if(pt > trgEffMCTruthPtMax)
    pt = trgEffMCTruthPtMax;
  else if(pt < trgEffMCTruthPtMin)
    pt = trgEffMCTruthPtMin;
  if(eta > trgEffMCTruthEtaMax)
    eta = trgEffMCTruthEtaMax;
  else if(eta < trgEffMCTruthEtaMin)
    eta = trgEffMCTruthEtaMin;
  return std::max(hTrgEffMCTruth->GetBinContent(hTrgEffMCTruth->FindFixBin(eta, pt)) - fabs(hTrgEffMCTruth_Lo->GetBinContent(hTrgEffMCTruth_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruthBBLoErr(double pt, double eta)
{
  if(pt > trgEffMCTruthBBPtMax)
    pt = trgEffMCTruthBBPtMax;
  else if(pt < trgEffMCTruthBBPtMin)
    pt = trgEffMCTruthBBPtMin;
  if(eta > trgEffMCTruthBBEtaMax)
    eta = trgEffMCTruthBBEtaMax;
  else if(eta < trgEffMCTruthBBEtaMin)
    eta = trgEffMCTruthBBEtaMin;
  return std::max(hTrgEffMCTruthBB->GetBinContent(hTrgEffMCTruthBB->FindFixBin(eta, pt)) - fabs(hTrgEffMCTruthBB_Lo->GetBinContent(hTrgEffMCTruthBB_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetTrgEfficiencyMCTruthBBGenLoErr(double pt, double eta)
{
  if(pt > trgEffMCTruthBBGenPtMax)
    pt = trgEffMCTruthBBGenPtMax;
  else if(pt < trgEffMCTruthBBGenPtMin)
    pt = trgEffMCTruthBBGenPtMin;
  if(eta > trgEffMCTruthBBGenEtaMax)
    eta = trgEffMCTruthBBGenEtaMax;
  else if(eta < trgEffMCTruthBBGenEtaMin)
    eta = trgEffMCTruthBBGenEtaMin;
  return std::max(hTrgEffMCTruthBBGen->GetBinContent(hTrgEffMCTruthBBGen->FindFixBin(eta, pt)) - fabs(hTrgEffMCTruthBBGen_Lo->GetBinContent(hTrgEffMCTruthBBGen_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::GetGlobalEfficiency(double pt, double eta, const std::string method)
{
  if(method.compare("Data") == 0)
  {
    return GetTkEfficiencyData(pt, eta) * GetRecoSelEfficiencyData(pt, eta) * GetTrgEfficiencyData(pt, eta);
  }
  else if(method.compare("MC") == 0)
  {
    return GetTkEfficiencyMC(pt, eta) * GetRecoSelEfficiencyMC(pt, eta) * GetTrgEfficiencyMC(pt, eta);
  }
  else if(method.compare("MCTruth") == 0)
  {
    return GetTkEfficiencyMCTruth(pt, eta) * GetRecoSelEfficiencyMCTruth(pt, eta) * GetTrgEfficiencyMCTruth(pt, eta);
  }
  else if(method.compare("MCTruthBB") == 0)
  {
    return GetTkEfficiencyMCTruthBB(pt, eta) * GetRecoSelEfficiencyMCTruthBB(pt, eta) * GetTrgEfficiencyMCTruthBB(pt, eta);
  }
  else if(method.compare("MCTruthBBGen") == 0)
  {
    return GetTkEfficiencyMCTruthBB(pt, eta) * GetRecoSelEfficiencyMCTruthBBGen(pt, eta) * GetTrgEfficiencyMCTruthBBGen(pt, eta);
  }
  else
  {
    std::cout << "W A R N I N G ! Method \"" << method << "\" is not recognized!\n";
    std::cout << "                Returning 1!\n";
    return 1;
  }
}



double EfficiencyFactorsGetter::GetMCNLORescale(double pt, double eta)
{
  if(pt > mcNLORescalePtMax)
    pt = mcNLORescalePtMax;
  else if(pt < mcNLORescalePtMin)
    pt = mcNLORescalePtMin;
  if(eta > mcNLORescaleEtaMax)
    eta = mcNLORescaleEtaMax;
  else if(eta < mcNLORescaleEtaMin)
    eta = mcNLORescaleEtaMin;
  return hMCNLORescale->GetBinContent(hMCNLORescale->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetMCNLORescaleHiErr(double pt, double eta)
{
  if(pt > mcNLORescalePtMax)
    pt = mcNLORescalePtMax;
  else if(pt < mcNLORescalePtMin)
    pt = mcNLORescalePtMin;
  if(eta > mcNLORescaleEtaMax)
    eta = mcNLORescaleEtaMax;
  else if(eta < mcNLORescaleEtaMin)
    eta = mcNLORescaleEtaMin;
  return hMCNLORescale->GetBinContent(hMCNLORescale->FindFixBin(eta, pt)) + hMCNLORescale_Hi->GetBinContent(hMCNLORescale_Hi->FindFixBin(eta, pt));
}



double EfficiencyFactorsGetter::GetMCNLORescaleLoErr(double pt, double eta)
{
  if(pt > mcNLORescalePtMax)
    pt = mcNLORescalePtMax;
  else if(pt < mcNLORescalePtMin)
    pt = mcNLORescalePtMin;
  if(eta > mcNLORescaleEtaMax)
    eta = mcNLORescaleEtaMax;
  else if(eta < mcNLORescaleEtaMin)
    eta = mcNLORescaleEtaMin;
  return std::max(hMCNLORescale->GetBinContent(hMCNLORescale->FindFixBin(eta, pt)) - fabs(hMCNLORescale_Lo->GetBinContent(hMCNLORescale_Lo->FindFixBin(eta, pt))),0.);
}



double EfficiencyFactorsGetter::FindEventEfficiencyFromSingleParticle(std::vector<double> vEfficiencies)
{
  unsigned int size = vEfficiencies.size();
  std::vector<unsigned int> vIndexes;
  for(unsigned int i = 0; i < size; i++)
  {
    vIndexes.push_back(i);
  }
  
  double p1 = 0;
  for(unsigned int i = 0; i < vEfficiencies.size(); i++)
  {
    //     std::cout << " i = " << i << std::endl;
    double factor = vEfficiencies[i];
    //     std::cout << "factor = " << factor << std::endl;
    for(unsigned int j = 0; j < vEfficiencies.size(); j++)
    {
      //       std::cout << " j = " << j << std::endl;
      if(i != j)
      {
        factor *= (1 - vEfficiencies[j]);
      }
      //       std::cout << "factor = " << factor << std::endl;
    }
    p1 += factor;
    //     std::cout << "p1 = " << p1 << std::endl;
  }
  
  double p0 = 1;
  for(unsigned int i = 0; i < size; i++)
  {
    p0 *= (1 - vEfficiencies[i]);
  }
  double result = 1-(p0+p1);
  if(result > 1e-7)
    return result;
  else
    return 1.;
}



double EfficiencyFactorsGetter::FindEventWeightFromSingleParticle(std::vector<double> vWeights)
{
  std::cout << "E R R O R ! Method FindEventWeightFromSingleParticle(std::vector<double> vWeights) is not finished yet!\n";
  std::cout << "            Do not use it! Exiting...\n";
  exit(1);
  unsigned int size = vWeights.size();
  size = 1;
  return size;
}



void EfficiencyFactorsGetter::FindEventEfficiencyFromSingleParticleWithErrors(std::vector<double> vEfficiencies, std::vector<double> vLoErrors, std::vector<double> vHiErrors, double& result, double& resultLoErr, double& resultHiErr)
{
  unsigned int size = vEfficiencies.size();
  if(vHiErrors.size() != size || vLoErrors.size() != size)
  {
    std::cout << "E R R O R ! Efficiency error vectors do not have the same size as the efficiency vectors!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  std::vector<unsigned int> vIndexes;
  for(unsigned int i = 0; i < size; i++)
  {
    vIndexes.push_back(i);
  }
  
  double p1 = 0;
  double p1HiErr = 0;
  double p1LoErr = 0;
  for(unsigned int i = 0; i < vEfficiencies.size(); i++)
  {
    //     std::cout << " i = " << i << std::endl;
    double factor = vEfficiencies[i];
    double factorHiErr = vHiErrors[i];
    double factorLoErr = vLoErrors[i];
    
    //    std::cout << "factor = " << factor << std::endl;
    
    for(unsigned int j = 0; j < vEfficiencies.size(); j++)
    {
      //       std::cout << " j = " << j << std::endl;
      if(i != j)
      {
        double term = (1 - vEfficiencies[j]);
        double termHiErr = vLoErrors[j];
        double termLoErr = vHiErrors[j];
        factorHiErr = sqrt(term*term*factorHiErr*factorHiErr + factor*factor*termHiErr*termHiErr);
        factorLoErr = sqrt(term*term*factorLoErr*factorLoErr + factor*factor*termLoErr*termLoErr);
        factor *= term;
      }
      //       std::cout << "factor = " << factor << std::endl;
    }
    p1 += factor;
    p1HiErr = sqrt(p1HiErr*p1HiErr + factorHiErr*factorHiErr);
    p1LoErr = sqrt(p1LoErr*p1LoErr + factorLoErr*factorLoErr);
    
    //     std::cout << "p1 = " << p1 << std::endl;
  }
  
  double p0 = 1;
  double p0HiErr = 0;
  double p0LoErr = 0;
  for(unsigned int i = 0; i < size; i++)
  {
    double term = (1 - vEfficiencies[i]);
    double termHiErr = vLoErrors[i];
    double termLoErr = vHiErrors[i];
    p0HiErr = sqrt(term*term*p0HiErr*p0HiErr + p0*p0*termHiErr*termHiErr);
    p0LoErr = sqrt(term*term*p0LoErr*p0LoErr + p0*p0*termLoErr*termLoErr);
    p0 *= term;
  }
  result = 1-(p0+p1);
  if(result < 1e-7)
    result = 1.;
  resultHiErr = sqrt(p0LoErr*p0LoErr + p1LoErr*p1LoErr);
  if(result + resultHiErr > 1.)
    resultHiErr = 1. - result;
  resultLoErr = sqrt(p0HiErr*p0HiErr + p1HiErr*p1HiErr);
  if(result - resultLoErr < 0.)
    resultLoErr = result;
}



void EfficiencyFactorsGetter::WriteEfficiencyMetadata(TFile* f)
{
  // Save current directory to a string so the calling function is unaffected by what is done here
  // Damn ROOT static properties...
  std::string curDirName = gDirectory->GetPathStatic();
  
  TObjString* tsName = new TObjString(fEffInput->GetName());
  f->cd();
  TDirectory* dEfficiencyMetadata = f->mkdir("EfficiencyMetadata");
  dEfficiencyMetadata->cd();
  tsName->Write();
  // Now return to the old directory, before exiting
  TDirectory* dir = gROOT->GetDirectory(curDirName.c_str());
  dir->cd();
}



void EfficiencyFactorsGetter::InitEfficiencyFactorsGetter()
{
  // Save current directory to a string so the calling function is unaffected by what is done here
  // Damn ROOT static properties...
  std::string curDirName = gDirectory->GetPathStatic();
  
  std::cout << "Initializing EfficiencyFactorsGetter.\n";
  InitEfficiencyHistograms();
  InitEfficiencyHistogramsLimits();
  
  TDirectory* dir = gROOT->GetDirectory(curDirName.c_str());
  dir->cd();
}



void EfficiencyFactorsGetter::InitEfficiencyHistograms()
{
  std::cout << "Initializing efficiency histograms";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_04_29_1.root";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_05_04_1.root";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_05_06_1.root";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_05_12_1.root";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_05_13_1.root";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_06_08_1.root";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_07_13_1.root";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_09_29_1.root";
//   const std::string inputFileName = "../efficiencies/efficiencies_11_09_29_2.root";
  const std::string inputFileName = "../efficiencies/efficiencies_11_11_03_1.root";
  fEffInput = new TFile(inputFileName.c_str(), "READ");
  if(fEffInput->IsZombie())
  {
    std::cout << "\n";
    std::cout << "E R R O R ! A problem occurred while trying to open efficiency file:\n";
    std::cout << "            \"" << inputFileName << "\". Exiting...\n";
    exit(1);
  }
  // Now get histograms from file
  // Tk efficiencies
  hTkEffData = GetTH2D("hTkEffData");
  std::cout << ".";
  hTkEffData_Hi = GetTH2D("hTkEffData_Hi");
  std::cout << ".";
  hTkEffData_Lo = GetTH2D("hTkEffData_Lo");
  std::cout << ".";
  hTkEffMC = GetTH2D("hTkEffMC");
  std::cout << ".";
  hTkEffMC_Hi = GetTH2D("hTkEffMC_Hi");
  std::cout << ".";
  hTkEffMC_Lo = GetTH2D("hTkEffMC_Lo");
  std::cout << ".";
  hTkEffMCTruth = GetTH2D("hTkEffMCTruth");
  std::cout << ".";
  hTkEffMCTruth_Hi = GetTH2D("hTkEffMCTruth_Hi");
  std::cout << ".";
  hTkEffMCTruth_Lo = GetTH2D("hTkEffMCTruth_Lo");
  std::cout << ".";
  hTkEffMCTruthBB = GetTH2D("hTkEffMCTruthBB");
  std::cout << ".";
  hTkEffMCTruthBB_Hi = GetTH2D("hTkEffMCTruthBB_Hi");
  std::cout << ".";
  hTkEffMCTruthBB_Lo = GetTH2D("hTkEffMCTruthBB_Lo");
  std::cout << ".";
  
  // Reco-sel efficiencies
  hRecoSelEffData = GetTH2D("hRecoSelEffData");
  std::cout << ".";
  hRecoSelEffData_Hi = GetTH2D("hRecoSelEffData_Hi");
  std::cout << ".";
  hRecoSelEffData_Lo = GetTH2D("hRecoSelEffData_Lo");
  std::cout << ".";
  hRecoSelEffMC = GetTH2D("hRecoSelEffMC");
  std::cout << ".";
  hRecoSelEffMC_Hi = GetTH2D("hRecoSelEffMC_Hi");
  std::cout << ".";
  hRecoSelEffMC_Lo = GetTH2D("hRecoSelEffMC_Lo");
  std::cout << ".";
  hRecoSelEffMCTruth = GetTH2D("hRecoSelEffMCTruth");
  std::cout << ".";
  hRecoSelEffMCTruth_Hi = GetTH2D("hRecoSelEffMCTruth_Hi");
  std::cout << ".";
  hRecoSelEffMCTruth_Lo = GetTH2D("hRecoSelEffMCTruth_Lo");
  std::cout << ".";
  hRecoSelEffMCTruthBB = GetTH2D("hRecoSelEffMCTruthBB");
  std::cout << ".";
  hRecoSelEffMCTruthBB_Hi = GetTH2D("hRecoSelEffMCTruthBB_Hi");
  std::cout << ".";
  hRecoSelEffMCTruthBB_Lo = GetTH2D("hRecoSelEffMCTruthBB_Lo");
  std::cout << ".";
  hRecoSelEffMCTruthBBGen = GetTH2D("hRecoSelEffMCTruthBBGen");
  std::cout << ".";
  hRecoSelEffMCTruthBBGen_Hi = GetTH2D("hRecoSelEffMCTruthBBGen_Hi");
  std::cout << ".";
  hRecoSelEffMCTruthBBGen_Lo = GetTH2D("hRecoSelEffMCTruthBBGen_Lo");
  std::cout << ".";
  
  // Mu-sel efficiencies
  hMuSelEffData = GetTH2D("hMuSelEffData");
  std::cout << ".";
  hMuSelEffData_Hi = GetTH2D("hMuSelEffData_Hi");
  std::cout << ".";
  hMuSelEffData_Lo = GetTH2D("hMuSelEffData_Lo");
  std::cout << ".";
  hMuSelEffMC = GetTH2D("hMuSelEffMC");
  std::cout << ".";
  hMuSelEffMC_Hi = GetTH2D("hMuSelEffMC_Hi");
  std::cout << ".";
  hMuSelEffMC_Lo = GetTH2D("hMuSelEffMC_Lo");
  std::cout << ".";
  hMuSelEffMCTruth = GetTH2D("hMuSelEffMCTruth");
  std::cout << ".";
  hMuSelEffMCTruth_Hi = GetTH2D("hMuSelEffMCTruth_Hi");
  std::cout << ".";
  hMuSelEffMCTruth_Lo = GetTH2D("hMuSelEffMCTruth_Lo");
  std::cout << ".";
  hMuSelEffMCTruthBB = GetTH2D("hMuSelEffMCTruthBB");
  std::cout << ".";
  hMuSelEffMCTruthBB_Hi = GetTH2D("hMuSelEffMCTruthBB_Hi");
  std::cout << ".";
  hMuSelEffMCTruthBB_Lo = GetTH2D("hMuSelEffMCTruthBB_Lo");
  std::cout << ".";
  hMuSelEffMCTruthBBGen = GetTH2D("hMuSelEffMCTruthBBGen");
  std::cout << ".";
  hMuSelEffMCTruthBBGen_Hi = GetTH2D("hMuSelEffMCTruthBBGen_Hi");
  std::cout << ".";
  hMuSelEffMCTruthBBGen_Lo = GetTH2D("hMuSelEffMCTruthBBGen_Lo");
  std::cout << ".";
  
  // Trigger efficiencies
  hTrgEffData = GetTH2D("hTrgEffData");
  std::cout << ".";
  hTrgEffData_Hi = GetTH2D("hTrgEffData_Hi");
  std::cout << ".";
  hTrgEffData_Lo = GetTH2D("hTrgEffData_Lo");
  std::cout << ".";
  hTrgEffMC = GetTH2D("hTrgEffMC");
  std::cout << ".";
  hTrgEffMC_Hi = GetTH2D("hTrgEffMC_Hi");
  std::cout << ".";
  hTrgEffMC_Lo = GetTH2D("hTrgEffMC_Lo");
  std::cout << ".";
  hTrgEffMCTruth = GetTH2D("hTrgEffMCTruth");
  std::cout << ".";
  hTrgEffMCTruth_Hi = GetTH2D("hTrgEffMCTruth_Hi");
  std::cout << ".";
  hTrgEffMCTruth_Lo = GetTH2D("hTrgEffMCTruth_Lo");
  std::cout << ".";
  hTrgEffMCTruthBB = GetTH2D("hTrgEffMCTruthBB");
  std::cout << ".";
  hTrgEffMCTruthBB_Hi = GetTH2D("hTrgEffMCTruthBB_Hi");
  std::cout << ".";
  hTrgEffMCTruthBB_Lo = GetTH2D("hTrgEffMCTruthBB_Lo");
  std::cout << ".";
  hTrgEffMCTruthBBGen = GetTH2D("hTrgEffMCTruthBBGen");
  std::cout << ".";
  hTrgEffMCTruthBBGen_Hi = GetTH2D("hTrgEffMCTruthBBGen_Hi");
  std::cout << ".";
  hTrgEffMCTruthBBGen_Lo = GetTH2D("hTrgEffMCTruthBBGen_Lo");
  std::cout << ".";
  
  hMCNLORescale = GetTH2D("hMCNLORescale");
  std::cout << ".";
  hMCNLORescale_Hi = GetTH2D("hMCNLORescale_Hi");
  std::cout << ".";
  hMCNLORescale_Lo = GetTH2D("hMCNLORescale_Lo");
  std::cout << ".";

  std::cout << "\n";
  
}



void EfficiencyFactorsGetter::InitEfficiencyHistogramsLimits()
{
  std::cout << "Initializing efficiency histograms limits";
  // To avoid border effects, the max is set in the middle of last bin
  // (because upper edge of a bin belongs to next bin)
  tkEffDataPtMin = hTkEffData->GetYaxis()->GetXmin();
  std::cout << ".";
  tkEffDataPtMax = (hTkEffData->GetYaxis()->GetXmax() + hTkEffData->GetYaxis()->GetBinLowEdge(hTkEffData->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  tkEffDataEtaMin = hTkEffData->GetXaxis()->GetXmin();
  std::cout << ".";
  tkEffDataEtaMax = (hTkEffData->GetXaxis()->GetXmax() + hTkEffData->GetXaxis()->GetBinLowEdge(hTkEffData->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  tkEffMCPtMin = hTkEffMC->GetYaxis()->GetXmin();
  std::cout << ".";
  tkEffMCPtMax = (hTkEffMC->GetYaxis()->GetXmax() + hTkEffMC->GetYaxis()->GetBinLowEdge(hTkEffMC->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  tkEffMCEtaMin = hTkEffMC->GetXaxis()->GetXmin();
  std::cout << ".";
  tkEffMCEtaMax = (hTkEffMC->GetXaxis()->GetXmax() + hTkEffMC->GetXaxis()->GetBinLowEdge(hTkEffMC->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  tkEffMCTruthPtMin = hTkEffMCTruth->GetYaxis()->GetXmin();
  std::cout << ".";
  tkEffMCTruthPtMax = (hTkEffMCTruth->GetYaxis()->GetXmax() + hTkEffMCTruth->GetYaxis()->GetBinLowEdge(hTkEffMCTruth->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  tkEffMCTruthEtaMin = hTkEffMCTruth->GetXaxis()->GetXmin();
  std::cout << ".";
  tkEffMCTruthEtaMax = (hTkEffMCTruth->GetXaxis()->GetXmax() + hTkEffMCTruth->GetXaxis()->GetBinLowEdge(hTkEffMCTruth->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  tkEffMCTruthBBPtMin = hTkEffMCTruthBB->GetYaxis()->GetXmin();
  std::cout << ".";
  tkEffMCTruthBBPtMax = (hTkEffMCTruthBB->GetYaxis()->GetXmax() + hTkEffMCTruthBB->GetYaxis()->GetBinLowEdge(hTkEffMCTruthBB->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  tkEffMCTruthBBEtaMin = hTkEffMCTruthBB->GetXaxis()->GetXmin();
  std::cout << ".";
  tkEffMCTruthBBEtaMax = (hTkEffMCTruthBB->GetXaxis()->GetXmax() + hTkEffMCTruthBB->GetXaxis()->GetBinLowEdge(hTkEffMCTruthBB->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  
  recoSelEffDataPtMin = hRecoSelEffData->GetYaxis()->GetXmin();
  std::cout << ".";
  recoSelEffDataPtMax = (hRecoSelEffData->GetYaxis()->GetXmax() + hRecoSelEffData->GetYaxis()->GetBinLowEdge(hRecoSelEffData->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffDataEtaMin = hRecoSelEffData->GetXaxis()->GetXmin();
  std::cout << ".";
  recoSelEffDataEtaMax = (hRecoSelEffData->GetXaxis()->GetXmax() + hRecoSelEffData->GetXaxis()->GetBinLowEdge(hRecoSelEffData->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffMCPtMin = hRecoSelEffMC->GetYaxis()->GetXmin();
  std::cout << ".";
  recoSelEffMCPtMax = (hRecoSelEffMC->GetYaxis()->GetXmax() + hRecoSelEffMC->GetYaxis()->GetBinLowEdge(hRecoSelEffMC->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffMCEtaMin = hRecoSelEffMC->GetXaxis()->GetXmin();
  std::cout << ".";
  recoSelEffMCEtaMax = (hRecoSelEffMC->GetXaxis()->GetXmax() + hRecoSelEffMC->GetXaxis()->GetBinLowEdge(hRecoSelEffMC->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffMCTruthPtMin = hRecoSelEffMCTruth->GetYaxis()->GetXmin();
  std::cout << ".";
  recoSelEffMCTruthPtMax = (hRecoSelEffMCTruth->GetYaxis()->GetXmax() + hRecoSelEffMCTruth->GetYaxis()->GetBinLowEdge(hRecoSelEffMCTruth->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffMCTruthEtaMin = hRecoSelEffMCTruth->GetXaxis()->GetXmin();
  std::cout << ".";
  recoSelEffMCTruthEtaMax = (hRecoSelEffMCTruth->GetXaxis()->GetXmax() + hRecoSelEffMCTruth->GetXaxis()->GetBinLowEdge(hRecoSelEffMCTruth->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffMCTruthBBPtMin = hRecoSelEffMCTruthBB->GetYaxis()->GetXmin();
  std::cout << ".";
  recoSelEffMCTruthBBPtMax = (hRecoSelEffMCTruthBB->GetYaxis()->GetXmax() + hRecoSelEffMCTruthBB->GetYaxis()->GetBinLowEdge(hRecoSelEffMCTruthBB->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffMCTruthBBEtaMin = hRecoSelEffMCTruthBB->GetXaxis()->GetXmin();
  std::cout << ".";
  recoSelEffMCTruthBBEtaMax = (hRecoSelEffMCTruthBB->GetXaxis()->GetXmax() + hRecoSelEffMCTruthBB->GetXaxis()->GetBinLowEdge(hRecoSelEffMCTruthBB->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffMCTruthBBGenPtMin = hRecoSelEffMCTruthBBGen->GetYaxis()->GetXmin();
  std::cout << ".";
  recoSelEffMCTruthBBGenPtMax = (hRecoSelEffMCTruthBBGen->GetYaxis()->GetXmax() + hRecoSelEffMCTruthBBGen->GetYaxis()->GetBinLowEdge(hRecoSelEffMCTruthBBGen->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  recoSelEffMCTruthBBGenEtaMin = hRecoSelEffMCTruthBBGen->GetXaxis()->GetXmin();
  std::cout << ".";
  recoSelEffMCTruthBBGenEtaMax = (hRecoSelEffMCTruthBBGen->GetXaxis()->GetXmax() + hRecoSelEffMCTruthBBGen->GetXaxis()->GetBinLowEdge(hRecoSelEffMCTruthBBGen->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  
  muSelEffDataPtMin = hMuSelEffData->GetYaxis()->GetXmin();
  std::cout << ".";
  muSelEffDataPtMax = (hMuSelEffData->GetYaxis()->GetXmax() + hMuSelEffData->GetYaxis()->GetBinLowEdge(hMuSelEffData->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffDataEtaMin = hMuSelEffData->GetXaxis()->GetXmin();
  std::cout << ".";
  muSelEffDataEtaMax = (hMuSelEffData->GetXaxis()->GetXmax() + hMuSelEffData->GetXaxis()->GetBinLowEdge(hMuSelEffData->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffMCPtMin = hMuSelEffMC->GetYaxis()->GetXmin();
  std::cout << ".";
  muSelEffMCPtMax = (hMuSelEffMC->GetYaxis()->GetXmax() + hMuSelEffMC->GetYaxis()->GetBinLowEdge(hMuSelEffMC->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffMCEtaMin = hMuSelEffMC->GetXaxis()->GetXmin();
  std::cout << ".";
  muSelEffMCEtaMax = (hMuSelEffMC->GetXaxis()->GetXmax() + hMuSelEffMC->GetXaxis()->GetBinLowEdge(hMuSelEffMC->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffMCTruthPtMin = hMuSelEffMCTruth->GetYaxis()->GetXmin();
  std::cout << ".";
  muSelEffMCTruthPtMax = (hMuSelEffMCTruth->GetYaxis()->GetXmax() + hMuSelEffMCTruth->GetYaxis()->GetBinLowEdge(hMuSelEffMCTruth->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffMCTruthEtaMin = hMuSelEffMCTruth->GetXaxis()->GetXmin();
  std::cout << ".";
  muSelEffMCTruthEtaMax = (hMuSelEffMCTruth->GetXaxis()->GetXmax() + hMuSelEffMCTruth->GetXaxis()->GetBinLowEdge(hMuSelEffMCTruth->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffMCTruthBBPtMin = hMuSelEffMCTruthBB->GetYaxis()->GetXmin();
  std::cout << ".";
  muSelEffMCTruthBBPtMax = (hMuSelEffMCTruthBB->GetYaxis()->GetXmax() + hMuSelEffMCTruthBB->GetYaxis()->GetBinLowEdge(hMuSelEffMCTruthBB->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffMCTruthBBEtaMin = hMuSelEffMCTruthBB->GetXaxis()->GetXmin();
  std::cout << ".";
  muSelEffMCTruthBBEtaMax = (hMuSelEffMCTruthBB->GetXaxis()->GetXmax() + hMuSelEffMCTruthBB->GetXaxis()->GetBinLowEdge(hMuSelEffMCTruthBB->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffMCTruthBBGenPtMin = hMuSelEffMCTruthBBGen->GetYaxis()->GetXmin();
  std::cout << ".";
  muSelEffMCTruthBBGenPtMax = (hMuSelEffMCTruthBBGen->GetYaxis()->GetXmax() + hMuSelEffMCTruthBBGen->GetYaxis()->GetBinLowEdge(hMuSelEffMCTruthBBGen->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  muSelEffMCTruthBBGenEtaMin = hMuSelEffMCTruthBBGen->GetXaxis()->GetXmin();
  std::cout << ".";
  muSelEffMCTruthBBGenEtaMax = (hMuSelEffMCTruthBBGen->GetXaxis()->GetXmax() + hMuSelEffMCTruthBBGen->GetXaxis()->GetBinLowEdge(hMuSelEffMCTruthBBGen->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  
  trgEffDataPtMin = hTrgEffData->GetYaxis()->GetXmin();
  std::cout << ".";
  trgEffDataPtMax = (hTrgEffData->GetYaxis()->GetXmax() + hTrgEffData->GetYaxis()->GetBinLowEdge(hTrgEffData->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffDataEtaMin = hTrgEffData->GetXaxis()->GetXmin();
  std::cout << ".";
  trgEffDataEtaMax = (hTrgEffData->GetXaxis()->GetXmax() + hTrgEffData->GetXaxis()->GetBinLowEdge(hTrgEffData->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffMCPtMin = hTrgEffMC->GetYaxis()->GetXmin();
  std::cout << ".";
  trgEffMCPtMax = (hTrgEffMC->GetYaxis()->GetXmax() + hTrgEffMC->GetYaxis()->GetBinLowEdge(hTrgEffMC->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffMCEtaMin = hTrgEffMC->GetXaxis()->GetXmin();
  std::cout << ".";
  trgEffMCEtaMax = (hTrgEffMC->GetXaxis()->GetXmax() + hTrgEffMC->GetXaxis()->GetBinLowEdge(hTrgEffMC->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffMCTruthPtMin = hTrgEffMCTruth->GetYaxis()->GetXmin();
  std::cout << ".";
  trgEffMCTruthPtMax = (hTrgEffMCTruth->GetYaxis()->GetXmax() + hTrgEffMCTruth->GetYaxis()->GetBinLowEdge(hTrgEffMCTruth->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffMCTruthEtaMin = hTrgEffMCTruth->GetXaxis()->GetXmin();
  std::cout << ".";
  trgEffMCTruthEtaMax = (hTrgEffMCTruth->GetXaxis()->GetXmax() + hTrgEffMCTruth->GetXaxis()->GetBinLowEdge(hTrgEffMCTruth->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffMCTruthBBPtMin = hTrgEffMCTruthBB->GetYaxis()->GetXmin();
  std::cout << ".";
  trgEffMCTruthBBPtMax = (hTrgEffMCTruthBB->GetYaxis()->GetXmax() + hTrgEffMCTruthBB->GetYaxis()->GetBinLowEdge(hTrgEffMCTruthBB->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffMCTruthBBEtaMin = hTrgEffMCTruthBB->GetXaxis()->GetXmin();
  std::cout << ".";
  trgEffMCTruthBBEtaMax = (hTrgEffMCTruthBB->GetXaxis()->GetXmax() + hTrgEffMCTruthBB->GetXaxis()->GetBinLowEdge(hTrgEffMCTruthBB->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffMCTruthBBGenPtMin = hTrgEffMCTruthBBGen->GetYaxis()->GetXmin();
  std::cout << ".";
  trgEffMCTruthBBGenPtMax = (hTrgEffMCTruthBBGen->GetYaxis()->GetXmax() + hTrgEffMCTruthBBGen->GetYaxis()->GetBinLowEdge(hTrgEffMCTruthBBGen->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  trgEffMCTruthBBGenEtaMin = hTrgEffMCTruthBBGen->GetXaxis()->GetXmin();
  std::cout << ".";
  trgEffMCTruthBBGenEtaMax = (hTrgEffMCTruthBBGen->GetXaxis()->GetXmax() + hTrgEffMCTruthBBGen->GetXaxis()->GetBinLowEdge(hTrgEffMCTruthBBGen->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  
  mcNLORescalePtMin = hMCNLORescale->GetYaxis()->GetXmin();
  std::cout << ".";
  mcNLORescalePtMax = (hMCNLORescale->GetYaxis()->GetXmax() + hMCNLORescale->GetYaxis()->GetBinLowEdge(hMCNLORescale->GetYaxis()->GetNbins()))/2;
  std::cout << ".";
  mcNLORescaleEtaMin = hMCNLORescale->GetXaxis()->GetXmin();
  std::cout << ".";
  mcNLORescaleEtaMax = (hMCNLORescale->GetXaxis()->GetXmax() + hMCNLORescale->GetXaxis()->GetBinLowEdge(hMCNLORescale->GetXaxis()->GetNbins()))/2;
  std::cout << ".";
  
  std::cout << "\n";
}



TH2D* EfficiencyFactorsGetter::GetTH2D(const std::string name)
{
  TH2D* result = 0;
  
  fEffInput->GetObject(name.c_str(), result);

  if(!result)
  {
    std::cout << "\n";
    std::cout << "E R R O R ! No TH2D named \"" << name << "\" found in input efficiency file!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  else
  {
    return result;
  }
}



#endif // EfficiencyFactorsGetter_cxx