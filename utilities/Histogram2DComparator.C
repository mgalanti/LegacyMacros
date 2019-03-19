#ifndef Histogram2DComparator_cxx
#define Histogram2DComparator_cxx

#include "Histogram2DComparator.h"

#include "TClass.h"
#include "TCollection.h"
#include "TKey.h"
#include "TROOT.h"

#include <set>
#include <sstream>
#include <algorithm>



// void HistogramComparator::LoopOverHistograms()
// {
//   std::cout << "Inside HistogramComparator::LoopOverHistograms()\n";
//   TKey* key;
//   TIter nextkey(inputFiles.at(0)->GetListOfKeys());
// 
//   while((key = (TKey*)nextkey()))
//   {
//     const char *classname = key->GetClassName();
//     TClass *cl = gROOT->GetClass(classname);
//     if (!cl) continue;
//     // Only take 2D histograms
//     if (cl->InheritsFrom(TH2::Class()) && !cl->InheritsFrom(TH3::Class()))
//     {
//       std::string histoName(key->GetName());
//       CompareHistogram(histoName);
//     }
//   }
// }



void Histogram2DComparator::Initialize()
{
  std::cout << "Inside Histogram2DComparator::Initialize()\n";
  OpenInputFiles();
  OpenOutputFile();
}



void Histogram2DComparator::Close()
{
  CloseInputFiles();
  CloseOutputFile();
}


void Histogram2DComparator::OpenInputFiles()
{
  // To force current style on old histograms
  gROOT->ForceStyle();

  TFile* fFirstInput = 0;
  fFirstInput = TFile::Open(inputFileNames.first.c_str(), "READ");
  if(fFirstInput)
  {
    inputFiles.first = fFirstInput;
  }
  else
  {
    std::cout << "E R R O R! Failed to open file:\n\t" << inputFileNames.first.c_str() << std::endl;
    exit(1);
  }

  TFile* fSecondInput = 0;
  fSecondInput = TFile::Open(inputFileNames.second.c_str(), "READ");
  if(fSecondInput)
  {
    inputFiles.second = fSecondInput;
  }
  else
  {
    std::cout << "E R R O R! Failed to open file:\n\t" << inputFileNames.second.c_str() << std::endl;
    exit(1);
  }
}


void Histogram2DComparator::CloseInputFiles()
{
  inputFiles.first->Close();
  inputFiles.second->Close();
}



void Histogram2DComparator::OpenOutputFile()
{
  outputFile = new TFile(outputFileName.c_str(), "RECREATE");
}



void Histogram2DComparator::CloseOutputFile()
{
  outputFile->Close();
}



// void Histogram2DComparator::CompareHistogram(std::string histoName)
// {
//   std::cout << "Inside HistogramComparator::CompareHistogram(std::string histoName)\n";
//   std::string histoNameToCompare;
//   std::ostringstream oss;
//   oss.str("");
//   std::vector<TH1*> histosToCompare;
//   // Check that histogram exists in every file
//   for(unsigned int iFile=0; iFile<inputFiles.size(); iFile++)
//   {
//     TH1* histo = (TH1*)(inputFiles.at(iFile)->Get(histoName.c_str()));
//     if(histo != 0)
//     {
//       oss << histoName << "_" << iFile;
//       histoNameToCompare = oss.str();
//       histosToCompare.push_back((TH1*)(histo->Clone(histoNameToCompare.c_str())));
//     }
//     else
//     {
//       std::cout << "E R R O R ! Histogram \"" << histoName << "\" does not exist in all files!\n\tNo comparison plots will be produced.\n";
//       return;
//     }
//   }
// 
//   if(normalizationMethod != 0)
//   {
//     int result = NormalizeHistograms(histosToCompare);
//     if(result != 0)
//     {
//       std::cout << "E R R O R ! Normalization failed for histogram \"" << histoName << "\"!\n\tNo comparison plots will be produced.\n";
//       return;
//     }
//   }
// 
//   outputFile->cd();
// 
//   std::string cName = histoName;
//   // if cName begins with a "h", then change it to a "c"
//   if(cName.find_first_of("h") == 0)
//   {
//     cName[0] = 'c';
//   }
//   // Otherwise, simply prepend a "c" to the name
//   else
//   {
//     cName.insert(0, "c");
//   }
// 
//   std::string legendTitle = histosToCompare.at(0)->GetTitle();
//   LegendDefinition leg = {legendTitle, legendLabels, "N"};
// 
//   if(!canvasTitle.empty())
//     CreateAndWriteCanvasWithDifferences(cName.c_str(), canvasTitle.c_str(), lineStyles, markerStyles, colors, leg, false, false, histosToCompare);
//   else
//     CreateAndWriteCanvasWithDifferences(cName.c_str(), lineStyles, markerStyles, colors, leg, false, false, histosToCompare);
// 
// }



// int HistogramComparator::ComputeNormalizationFactors()
// {
//   std::cout << "Inside HistogramComparator::ComputeNormalizationFactors()\n";
//   // Look for histogram in all files
//   // Keep first as reference
//   TH1* histoReference = (TH1*)(inputFiles.at(0)->Get(normalizationHistoName.c_str()));
// 
//   if(histoReference)
//   {
//     // Underflow bin
//     int startBin = 0;
//     double startX = histoReference->GetXaxis()->GetXmin();
//     // Overflow bin
//     int endBin = histoReference->GetNbinsX()+1;
//     double endX = histoReference->GetXaxis()->GetXmax();
//     double factor = 0;
//     if(normalizationMethod == 2)
//     {
//       // Simple integral
//       factor = histoReference->Integral(startBin,endBin);
//     }
//     else if(normalizationMethod == 3)
//     {
//       for(unsigned int iBin = startBin; iBin<=endBin; iBin++)
//       {
//         double binCenter = histoReference->GetXaxis()->GetBinCenter(iBin);
//         double binWidth = histoReference->GetXaxis()->GetBinWidth(iBin);
//         double binContent = histoReference->GetBinContent(iBin);
//         if(binWidth != 1)
//           std::cout << "W A R N I N G ! Using normalization method 3 but bin width is != 1! Resulting normalization will likely be wrong!\n";
//         factor+=binContent*binCenter*binWidth;
//       }
//     }
//     for(unsigned int iFile=1; iFile<inputFiles.size();iFile++)
//     {
//       TH1* histo = (TH1*)(inputFiles.at(iFile)->Get(normalizationHistoName.c_str()));
//       if(histo)
//       {
//         // Check that the axis definition is the same
//         if(histo->GetXaxis()->GetXmin() != startX ||
//           histo->GetXaxis()->GetXmax() != endX ||
//           histo->GetNbinsX()+1 != endBin)
//         {
//           std::cout << "E R R O R ! Reference histograms for normalization calculation\n\tdo not have the same x axis!\n\tNo normalization will be made.\n";
//           return 1;
//         }
//         double norm;
//         if(normalizationMethod == 2)
//         {
//           double histoIntegral = histo->Integral(startBin,endBin);
//           norm = factor/histoIntegral;
//         }
//         else if(normalizationMethod == 3)
//         {
//           double factor2 = 0;
//           for(unsigned int iBin = startBin; iBin<=endBin; iBin++)
//           {
//             double binCenter = histo->GetXaxis()->GetBinCenter(iBin);
//             double binWidth = histo->GetXaxis()->GetBinWidth(iBin);
//             double binContent = histo->GetBinContent(iBin);
//             if(binWidth != 1)
//               std::cout << "W A R N I N G ! Using normalization method 3 but bin width is != 1! Resulting normalization will likely be wrong!\n";
//             factor2+=binContent*binCenter*binWidth;
//           }
//           norm = factor/factor2;
//         }
//         normalizationFactors.push_back(norm);
//       }
//       else
//       {
//         std::cout << "E R R O R ! Reference histogram missing on file " << iFile << "!!!\n";
//         return 2;
//       }
//     }
//   }
//   else
//   {
//     std::cout << "E R R O R ! Reference histogram missing on file 0!!!\n";
//     return 2;
//   }
//   return 0;
// }



// int HistogramComparator::NormalizeHistograms(std::vector<TH1*> histos)
// {
//   std::cout << "Inside HistogramComparator::NormalizeHistograms(std::vector<TH1*> histos)\n";
//   if(normalizationMethod == 0)
//   {
//     // Do nothing!
//   }
//   else if(normalizationMethod == 1)
//   {
//     // Underflow bin
//     int startBin = 0;
//     double startX = histos.at(0)->GetXaxis()->GetXmin();
//     // Overflow bin
//     int endBin = histos.at(0)->GetNbinsX()+1;
//     double endX = histos.at(0)->GetXaxis()->GetXmax();
//     double factor = histos.at(0)->Integral(startBin,endBin);
//     for(unsigned int iHisto = 1; iHisto<histos.size(); iHisto++)
//     {
//       // Check that the axis definition is the same
//       if(histos.at(iHisto)->GetXaxis()->GetXmin() != startX ||
//          histos.at(iHisto)->GetXaxis()->GetXmax() != endX ||
//          histos.at(iHisto)->GetNbinsX()+1 != endBin)
//       {
//         std::cout << "E R R O R ! Histograms to normalize do not have the same x axis!\n\tNo normalization will be made.\n";
//         return 1;
//       }
//       double histoIntegral = histos.at(iHisto)->Integral(startBin,endBin);
//       double norm = factor/histoIntegral;
//       std::cout << "Scaling histo " << iHisto << "by factor " << norm << std::endl;
//       histos.at(iHisto)->Scale(norm);
//     }
//   }
//   else
//   if(normalizationMethod == 2 || normalizationMethod == 3)
//   {
//     for(unsigned int iHisto = 1; iHisto<histos.size(); iHisto++)
//     {
//       if(normalizationFactors.at(iHisto-1)<0)
//       {
//         std::cout << "E R R O R ! Bogus normalization factor (i = " << iHisto << ").\n\tNo normalization will be made.\n";
//         return 2;
//       }
//       std::cout << "Scaling histo " << iHisto << " by factor " << normalizationFactors.at(iHisto-1) << std::endl;
//       histos.at(iHisto)->Scale(normalizationFactors.at(iHisto-1));
//     }
//   }
//   return 0;
// }



void Histogram2DComparator::CompareHistograms(const std::string firstName, const std::string secondName, const std::string hName, const std::string hTitle)
{
  TH2* hFirst = (TH2*)(inputFiles.first->Get(firstName.c_str()));
  TH2* hSecond = (TH2*)(inputFiles.second->Get(secondName.c_str()));
  if(hFirst == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << firstName << "\" does not exist in first file!\n\tNo difference will be produced.\n";
    return;
  }
  if(hSecond == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << secondName << "\" does not exist in second file!\n\tNo difference will be produced.\n";
    return;
  }
  std::pair<std::vector<double>,std::vector<double> > differenceAxes = FindDifferenceBinDivision(hFirst, hSecond);

  unsigned int nXBins = differenceAxes.first.size()-1;
  unsigned int nYBins = differenceAxes.second.size()-1;

  TH2D* hDifference = Create2DHistogram<TH2D>(hName.c_str(),hTitle.c_str(),nXBins,differenceAxes.first,nYBins,differenceAxes.second,hFirst->GetXaxis()->GetTitle(),hFirst->GetYaxis()->GetTitle());
  for(unsigned int xBin = 0 ; xBin <= nXBins; xBin++)
  {
    for(unsigned int yBin = 0; yBin <= nYBins; yBin++)
    {
      double x = hDifference->GetXaxis()->GetBinLowEdge(xBin);
      double y = hDifference->GetYaxis()->GetBinLowEdge(yBin);
      int binFirst = hFirst->FindBin(x,y);
      int binSecond = hSecond->FindBin(x,y);
      double result = hFirst->GetBinContent(binFirst) - hSecond->GetBinContent(binSecond);
      hDifference->SetBinContent(xBin,yBin,result);
    }
  }
  
  std::string cName = hName;
  // if cName begins with a "h", then change it to a "c"
  if(cName.find_first_of("h") == 0)
  {
    cName[0] = 'c';
  }
  // Otherwise, simply prepend a "c" to the name
  else
  {
    cName.insert(0, "c");
  }
  outputFile->cd();
  CreateAndWriteCanvas(cName.c_str(),"COLZ",false,false,false,hDifference);
}



void Histogram2DComparator::CompareHistogramsWithPull(const std::string firstName,
                                                      const std::string firstErrLoName,
                                                      const std::string firstErrHiName,
                                                      const std::string secondName,
                                                      const std::string secondErrLoName,
                                                      const std::string secondErrHiName,
                                                      const std::string hName,
                                                      const std::string hTitle)
{
  TH2* hFirst = (TH2*)(inputFiles.first->Get(firstName.c_str()));
  TH2* hFirstErrLo = (TH2*)(inputFiles.first->Get(firstErrLoName.c_str()));
  TH2* hFirstErrHi = (TH2*)(inputFiles.first->Get(firstErrHiName.c_str()));
  TH2* hSecond = (TH2*)(inputFiles.second->Get(secondName.c_str()));
  TH2* hSecondErrLo = (TH2*)(inputFiles.second->Get(secondErrLoName.c_str()));
  TH2* hSecondErrHi = (TH2*)(inputFiles.second->Get(secondErrHiName.c_str()));
  if(hFirst == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << firstName << "\" does not exist in first file!\n\tNo pull will be produced.\n";
    return;
  }
  if(hFirstErrLo == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << firstErrLoName << "\" does not exist in first file!\n\tNo pull will be produced.\n";
    return;
  }
  if(hFirstErrHi == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << firstErrHiName << "\" does not exist in first file!\n\tNo pull will be produced.\n";
    return;
  }
  if(hSecond == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << secondName << "\" does not exist in second file!\n\tNo pull will be produced.\n";
    return;
  }
  if(hSecondErrLo == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << secondErrLoName << "\" does not exist in second file!\n\tNo pull will be produced.\n";
    return;
  }
  if(hSecondErrHi == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << secondErrHiName << "\" does not exist in second file!\n\tNo pull will be produced.\n";
    return;
  }
  std::pair<std::vector<double>,std::vector<double> > differenceAxes = FindDifferenceBinDivision(hFirst, hSecond);
  
  // Check bins consistency
  if(!SameBins(hFirst, hFirstErrLo))
  {
    std::cout << "E R R O R ! Value histogram \"" << firstName << "\" and low error histogram \"" << firstErrLoName << "\" do not have the same bins!\n";
    std::cout << "            No pull will be produced!\n";
    return;
  }
  if(!SameBins(hFirst, hFirstErrHi))
  {
    std::cout << "E R R O R ! Value histogram \"" << firstName << "\" and hi error histogram \"" << firstErrHiName << "\" do not have the same bins!\n";
    std::cout << "            No pull will be produced!\n";
    return;
  }
  if(!SameBins(hSecond, hSecondErrLo))
  {
    std::cout << "E R R O R ! Value histogram \"" << secondName << "\" and low error histogram \"" << secondErrLoName << "\" do not have the same bins!\n";
    std::cout << "            No pull will be produced!\n";
    return;
  }
  if(!SameBins(hSecond, hSecondErrHi))
  {
    std::cout << "E R R O R ! Value histogram \"" << secondName << "\" and hi error histogram \"" << secondErrHiName << "\" do not have the same bins!\n";
    std::cout << "            No pull will be produced!\n";
    return;
  }
  
  unsigned int nXBins = differenceAxes.first.size()-1;
  unsigned int nYBins = differenceAxes.second.size()-1;
  
  TH2D* hDifference = Create2DHistogram<TH2D>(hName.c_str(),hTitle.c_str(),nXBins,differenceAxes.first,nYBins,differenceAxes.second,hFirst->GetXaxis()->GetTitle(),hFirst->GetYaxis()->GetTitle());
  for(unsigned int xBin = 0 ; xBin <= nXBins; xBin++)
  {
    for(unsigned int yBin = 0; yBin <= nYBins; yBin++)
    {
      double x = hDifference->GetXaxis()->GetBinLowEdge(xBin);
      double y = hDifference->GetYaxis()->GetBinLowEdge(yBin);
      int binFirst = hFirst->FindBin(x,y);
      int binSecond = hSecond->FindBin(x,y);
      double difference = hFirst->GetBinContent(binFirst) - hSecond->GetBinContent(binSecond);
      double error = 1;
      if(difference > 0)
      {
        error = sqrt(hFirstErrLo->GetBinContent(binFirst)*hFirstErrLo->GetBinContent(binFirst) + hSecondErrHi->GetBinContent(binSecond)*hSecondErrHi->GetBinContent(binSecond));
      }
      else if(difference < 0)
      {
        error = sqrt(hFirstErrHi->GetBinContent(binFirst)*hFirstErrHi->GetBinContent(binFirst) + hSecondErrLo->GetBinContent(binSecond)*hSecondErrLo->GetBinContent(binSecond));
      }
      hDifference->SetBinContent(xBin,yBin,difference/error);
    }
  }
  std::string cName = hName;
  // if cName begins with a "h", then change it to a "c"
  if(cName.find_first_of("h") == 0)
  {
    cName[0] = 'c';
  }
  // Otherwise, simply prepend a "c" to the name
  else
  {
    cName.insert(0, "c");
  }
  outputFile->cd();
  CreateAndWriteCanvas(cName.c_str(),"COLZ",false,false,false,hDifference);
}



void Histogram2DComparator::CompareHistogramsWithPull(const std::string firstName,
                                                      const std::string secondName,
                                                      const std::string hName,
                                                      const std::string hTitle)
{
  TH2* hFirst = (TH2*)(inputFiles.first->Get(firstName.c_str()));
  TH2* hSecond = (TH2*)(inputFiles.second->Get(secondName.c_str()));
  if(hFirst == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << firstName << "\" does not exist in first file!\n\tNo pull will be produced.\n";
    return;
  }
  if(hSecond == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << secondName << "\" does not exist in second file!\n\tNo pull will be produced.\n";
    return;
  }
  std::pair<std::vector<double>,std::vector<double> > differenceAxes = FindDifferenceBinDivision(hFirst, hSecond);
  
  // Check bins consistency
  if(!SameBins(hFirst, hSecond))
  {
    std::cout << "E R R O R ! First histogram \"" << firstName << "\" and second histogram \"" << secondName << "\" do not have the same bins!\n";
    std::cout << "            No pull will be produced!\n";
    return;
  }
  
  unsigned int nXBins = differenceAxes.first.size()-1;
  unsigned int nYBins = differenceAxes.second.size()-1;
  
  TH2D* hDifference = Create2DHistogram<TH2D>(hName.c_str(),hTitle.c_str(),nXBins,differenceAxes.first,nYBins,differenceAxes.second,hFirst->GetXaxis()->GetTitle(),hFirst->GetYaxis()->GetTitle());
  for(unsigned int xBin = 0 ; xBin <= nXBins; xBin++)
  {
    for(unsigned int yBin = 0; yBin <= nYBins; yBin++)
    {
      double x = hDifference->GetXaxis()->GetBinLowEdge(xBin);
      double y = hDifference->GetYaxis()->GetBinLowEdge(yBin);
      int binFirst = hFirst->FindBin(x,y);
      int binSecond = hSecond->FindBin(x,y);
      double difference = hFirst->GetBinContent(binFirst) - hSecond->GetBinContent(binSecond);
      double error = sqrt(hFirst->GetBinError(binFirst)*hFirst->GetBinError(binFirst) + hSecond->GetBinError(binSecond)*hSecond->GetBinError(binSecond));

      hDifference->SetBinContent(xBin,yBin,difference/error);
    }
  }
  std::string cName = hName;
  // if cName begins with a "h", then change it to a "c"
  if(cName.find_first_of("h") == 0)
  {
    cName[0] = 'c';
  }
  // Otherwise, simply prepend a "c" to the name
  else
  {
    cName.insert(0, "c");
  }
  outputFile->cd();
  CreateAndWriteCanvas(cName.c_str(),"COLZ",false,false,false,hDifference);
}



void Histogram2DComparator::CompareHistogramsWithRatio(const std::string firstName,
                                                       const std::string secondName,
                                                       const std::string hName,
                                                       const std::string hTitle)
{
  TH2* hFirst = (TH2*)(inputFiles.first->Get(firstName.c_str()));
  TH2* hSecond = (TH2*)(inputFiles.second->Get(secondName.c_str()));
  if(hFirst == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << firstName << "\" does not exist in first file!\n\tNo pull will be produced.\n";
    return;
  }
  if(hSecond == 0)
  {
    std::cout << "E R R O R ! Histogram \"" << secondName << "\" does not exist in second file!\n\tNo pull will be produced.\n";
    return;
  }
  std::pair<std::vector<double>,std::vector<double> > differenceAxes = FindDifferenceBinDivision(hFirst, hSecond);
  
  // Check bins consistency
  if(!SameBins(hFirst, hSecond))
  {
    std::cout << "E R R O R ! First histogram \"" << firstName << "\" and second histogram \"" << secondName << "\" do not have the same bins!\n";
    std::cout << "            No pull will be produced!\n";
    return;
  }
  
  unsigned int nXBins = differenceAxes.first.size()-1;
  unsigned int nYBins = differenceAxes.second.size()-1;
  
  TH2D* hRatio = Create2DHistogram<TH2D>(hName.c_str(),hTitle.c_str(),nXBins,differenceAxes.first,nYBins,differenceAxes.second,hFirst->GetXaxis()->GetTitle(),hFirst->GetYaxis()->GetTitle());
  for(unsigned int xBin = 0 ; xBin <= nXBins; xBin++)
  {
    for(unsigned int yBin = 0; yBin <= nYBins; yBin++)
    {
      double x = hRatio->GetXaxis()->GetBinLowEdge(xBin);
      double y = hRatio->GetYaxis()->GetBinLowEdge(yBin);
      int binFirst = hFirst->FindBin(x,y);
      int binSecond = hSecond->FindBin(x,y);
      double ratio = 0;
      double error = 0;
      if(hSecond->GetBinContent(binSecond) != 0)
      {
        ratio = hFirst->GetBinContent(binFirst) / hSecond->GetBinContent(binSecond);
        error = sqrt((hFirst->GetBinError(binFirst)*hFirst->GetBinError(binFirst))/(hFirst->GetBinContent(binFirst)*hFirst->GetBinContent(binFirst)) + (hSecond->GetBinError(binSecond)*hSecond->GetBinError(binSecond))/(hSecond->GetBinContent(binSecond)*hSecond->GetBinContent(binSecond))) * ratio;
      }
      hRatio->SetBinContent(xBin,yBin,ratio);
      hRatio->SetBinError(xBin,yBin,error);
    }
  }
  std::string cName = hName;
  // if cName begins with a "h", then change it to a "c"
  if(cName.find_first_of("h") == 0)
  {
    cName[0] = 'c';
  }
  // Otherwise, simply prepend a "c" to the name
  else
  {
    cName.insert(0, "c");
  }
  outputFile->cd();
  CreateAndWriteCanvas(cName.c_str(),"COLZ",false,false,false,hRatio);
}



#endif // Histogram2DComparator_cxx
