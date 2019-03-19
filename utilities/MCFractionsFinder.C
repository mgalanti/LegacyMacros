#ifndef MCFractionsFinder_C
#define MCFractionsFinder_C


#include "MCFractionsFinder.h"



MCFractionsFinder::MCFractionsFinder()
{
  
}



std::map<std::string,std::pair<double,double> > MCFractionsFinder::FindMCFractionsFor1DTemplates()
{
  // To be written
  std::map<std::string,std::pair<double,double> > result;
  
  return result;
}



std::map<std::string,std::pair<double,double> > MCFractionsFinder::FindMCFractionsFor2DTemplates()
{
  inputFile = TFile::Open(inputFileName.c_str());
  std::map<std::string,std::pair<double,double> > templateIntegralsWithinMax; 
  for(unsigned int iTemplateName = 0; iTemplateName < templateNames.size(); iTemplateName++)
  {
    std::string name = templateNames[iTemplateName];
    std::cout << "templateNames[" << iTemplateName << "] = " << name << std::endl;
    
    std::string firstComponent = name.substr(0,1);
    std::string secondComponent = name.substr(1,1);
    
    std::string firstValue;
    std::string secondValue;
    for(std::map<std::string,std::string>::const_iterator it = translationMap.begin(); it != translationMap.end(); it++)
    {
      if(it->second == firstComponent)
        firstValue = it->first;
      if(it->second == secondComponent)
        secondValue = it->first;
    }
    std::cout << "Template " << name << " corresponds to inputs " << firstValue << " and " << secondValue << std::endl;
    std::string histoName = "hNEventsVsMax" + variableNameIP + "And" + variableNamePtRel + sign + classificationName + "_" + firstValue + "_" + secondValue;
    std::cout << "Histo name is " << histoName << std::endl;
    TH2D* histo = (TH2D*)inputFile->Get(histoName.c_str());
    if(histo)
    {
      std::cout << "Histogram read!\n";
      unsigned int xUpBin = FindXBin(histo);
      unsigned int yUpBin = FindYBin(histo);
      double sumOfBinContents = 0;
      double sumOfBinContentsError = 0;
      for(unsigned int xBin = 1; xBin <= xUpBin; xBin++)
      {
        for(unsigned int yBin = 1; yBin <= yUpBin; yBin++)
        {
          sumOfBinContents+=histo->GetBinContent(xBin,yBin);
        }
      }
      sumOfBinContentsError = sqrt(sumOfBinContents);
      std::cout << "sub of bin contents = " << sumOfBinContents << " +/- " << sumOfBinContentsError << std::endl;
      std::pair<double,double> valuePair = std::make_pair(sumOfBinContents, sumOfBinContentsError);
      templateIntegralsWithinMax[name] = valuePair;
    }
    else
    {
      std::cout << "E R R O R ! Could not find histogram in file!\n";
      std::cout << "            histo name = " << histoName << std::endl;
      std::cout << "            file name  = " << inputFileName << std::endl;
      std::cout << "            Exiting...\n";
      exit(1);
    }
    histo->Delete();
  }
  std::map<std::string,std::pair<double,double> > result;
  double totalIntegral = 0;
  for(std::map<std::string,std::pair<double,double> >::const_iterator it = templateIntegralsWithinMax.begin(); it != templateIntegralsWithinMax.end(); it++)
  {
    std::string resultName = "h" + it->first;
    double fractionToRemove = removedBinsFractions[resultName].first;
    double fractionToRemoveError = removedBinsFractions[resultName].second;
    double templateIntegral = templateIntegralsWithinMax[it->first].first;
    double templateIntegralError = templateIntegralsWithinMax[it->first].second;
    std::cout << "Fraction to remove for template " << it->first << " is " << fractionToRemove << " +/- " << fractionToRemoveError << std::endl;
    std::cout << "Integral for template " << it->first << " is " << templateIntegral << " +/- " << templateIntegralError << std::endl;
    double resultValue = templateIntegral * (1. - fractionToRemove);
    double resultError = sqrt((1.-fractionToRemove)*(1.-fractionToRemove) * templateIntegralError*templateIntegralError + (templateIntegral*templateIntegral) * fractionToRemoveError*fractionToRemoveError);
    std::cout << "Result for template " << it->first << " is " << resultValue << " +/- " << resultError << std::endl;
    std::pair<double,double> resultPair = std::make_pair(resultValue,resultError);
    result[resultName] = resultPair;
    totalIntegral+=resultValue;
  }
  // Normalize to 1
  for(std::map<std::string,std::pair<double,double> >::iterator it = result.begin(); it != result.end(); it++)
  {
    it->second.first/=totalIntegral;
    it->second.second/=totalIntegral;
    std::cout << "After normalization: Result for template " << it->first << " is " << it->second.first << " +/- " << it->second.second << std::endl;
  }
  
  inputFile->Close();
  
  return result;
}



int MCFractionsFinder::FindXBin(const TH2D* histo)
{
  TAxis* xAxis = histo->GetXaxis();
  int foundXBin = 0;
  for(int iBin = 0; iBin < xAxis->GetNbins(); iBin++)
  {
    if(xAxis->GetBinUpEdge(iBin) >= maxIP)
    {
      foundXBin = iBin;
      break;
    }
  }
  if(xAxis->GetBinLowEdge(foundXBin) == maxIP)
  {
    return foundXBin - 1;
  }
  else if(xAxis->GetBinUpEdge(foundXBin) == maxIP)
  {
    return foundXBin;
  }
  else
  {
    std::cout << "W A R N I N G ! The fit upper limit chosen for IP does not correspond to a bin edge of the counting histogram!\n";
    std::cout << "                The fraction determination will not be accurate!\n";
    return foundXBin;
  }
}



int MCFractionsFinder::FindYBin(const TH2D* histo)
{
  TAxis* yAxis = histo->GetYaxis();
  int foundYBin = 0;
  for(int iBin = 0; iBin < yAxis->GetNbins(); iBin++)
  {
    if(yAxis->GetBinUpEdge(iBin) >= maxPtRel)
    {
      foundYBin = iBin;
      break;
    }
  }
  if(yAxis->GetBinLowEdge(foundYBin) == maxPtRel)
  {
    return foundYBin - 1;
  }
  else if(yAxis->GetBinUpEdge(foundYBin) == maxPtRel)
  {
    return foundYBin;
  }
  else
  {
    std::cout << "W A R N I N G ! The fit upper limit chosen for PtRel does not correspond to a bin edge of the counting histogram!\n";
    std::cout << "                The fraction determination will not be accurate!\n";
    return foundYBin;
  }
}



#endif // MCFractionsFinder_C
