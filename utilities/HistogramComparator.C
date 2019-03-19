#ifndef HistogramComparator_cxx
#define HistogramComparator_cxx

#include "HistogramComparator.h"

#include "TGraph.h"
#include "TGraph2D.h"
#include "TClass.h"
#include "TCollection.h"
#include "TKey.h"
#include "TROOT.h"

#include <sstream>



void HistogramComparator::LoopOverHistograms()
{
  std::cout << "Inside HistogramComparator::LoopOverHistograms()\n";
  TKey* key;
  TIter nextkey(inputFiles.at(0)->GetListOfKeys());

  while((key = (TKey*)nextkey()))
  {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    // Only take 1D histograms
    if (cl->InheritsFrom(TH1::Class()) && !cl->InheritsFrom(TH2::Class()))
    {
      std::string histoName(key->GetName());
      std::cout << "Inside HistogramComparator::LoopOverHistograms() - comparing histograms " << histoName << std::endl;
      CompareHistogram(histoName);
    }
  }
}



void HistogramComparator::LoopOverGraphs()
{
  std::cout << "Inside HistogramComparator::LoopOverGraphs()\n";
  TKey* key;
  TIter nextkey(inputFiles.at(0)->GetListOfKeys());
  
  while((key = (TKey*)nextkey()))
  {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    // Only take 1D graphs
    if (cl->InheritsFrom(TGraph::Class()) && !cl->InheritsFrom(TGraph2D::Class()))
    {
      std::string graphName(key->GetName());
      std::cout << "Inside HistogramComparator::LoopOverGraphs() - comparing graphs " << graphName << std::endl;
      CompareGraph(graphName);
    }
  }
}



void HistogramComparator::Initialize()
{
//   std::cout << "Inside HistogramComparator::Initialize()\n";
  OpenInputFiles();
  OpenOutputFile();
  if(normalizationMethod==2 || normalizationMethod==3)
    ComputeNormalizationFactors();
}



void HistogramComparator::Close()
{
  CloseInputFiles();
  CloseOutputFile();
}


void HistogramComparator::OpenInputFiles()
{
  // To force current style on old histograms
  gROOT->ForceStyle();

  for(unsigned int iFile=0; iFile<inputFileNames.size(); iFile++)
  {
    TFile* fInput = 0;
    fInput = TFile::Open(inputFileNames.at(iFile).c_str(), "READ");
    if(fInput)
    {
      inputFiles.push_back(fInput);
    }
    else
    {
      std::cout << "W A R N I N G! Failed to open file:\n\t" << inputFileNames.at(iFile).c_str() << std::endl;
    }
  }
}



void HistogramComparator::CloseInputFiles()
{
  for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++)
  {
    inputFiles.at(iFile)->Close();
  }
}



void HistogramComparator::OpenOutputFile()
{
  outputFile = new TFile(outputFileName.c_str(), "RECREATE");
}



void HistogramComparator::CloseOutputFile()
{
  outputFile->Close();
}



void HistogramComparator::CompareHistogram(std::string histoName)
{
//   std::cout << "Inside HistogramComparator::CompareHistogram(std::string histoName)\n";
  std::string histoNameToCompare;
  std::ostringstream oss;
  oss.str("");
  std::vector<TH1*> histosToCompare;
  // Check that histogram exists in every file
  for(unsigned int iFile=0; iFile<inputFiles.size(); iFile++)
  {
    TH1* histo = (TH1*)(inputFiles.at(iFile)->Get(histoName.c_str()));
    if(histo != 0)
    {
      oss << histoName << "_" << iFile;
      histoNameToCompare = oss.str();
      histosToCompare.push_back((TH1*)(histo->Clone(histoNameToCompare.c_str())));
    }
    else
    {
      std::cout << "E R R O R ! Histogram \"" << histoName << "\" does not exist in all files!\n\tNo comparison plots will be produced.\n";
      return;
    }
  }

  if(normalizationMethod != 0)
  {
    int result = NormalizeHistograms(histosToCompare);
    if(result != 0)
    {
      std::cout << "E R R O R ! Normalization failed for histogram \"" << histoName << "\"!\n\tNo comparison plots will be produced.\n";
      return;
    }
  }

  outputFile->cd();

  std::string cName = histoName;
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

  std::string legendTitle = histosToCompare.at(0)->GetTitle();
  LegendDefinition leg = {legendTitle, legendLabels, "NW"};

  if(!canvasTitle.empty())
    CreateAndWriteCanvasWithRatios(cName.c_str(), canvasTitle.c_str(), lineStyles, markerStyles, colors, leg, false, true, histosToCompare);
  else
    CreateAndWriteCanvasWithRatios(cName.c_str(), lineStyles, markerStyles, colors, leg, false, false, histosToCompare);

}



void HistogramComparator::CompareGraph(std::string graphName)
{
  //   std::cout << "Inside HistogramComparator::CompareGraph(std::string graphName)\n";
  std::string graphNameToCompare;
  std::ostringstream oss;
  oss.str("");
  std::vector<TGraph*> graphsToCompare;
  // Check that graph exists in every file
  for(unsigned int iFile=0; iFile<inputFiles.size(); iFile++)
  {
    TGraph* graph = (TGraph*)(inputFiles.at(iFile)->Get(graphName.c_str()));
    if(graph != 0)
    {
      oss << graphName << "_" << iFile;
      graphNameToCompare = oss.str();
      graphsToCompare.push_back((TGraph*)(graph->Clone(graphNameToCompare.c_str())));
    }
    else
    {
      std::cout << "E R R O R ! Graph \"" << graphName << "\" does not exist in all files!\n\tNo comparison plots will be produced.\n";
      return;
    }
  }
  
  outputFile->cd();
  
  std::string cName = graphName;
  // if cName begins with a "g", then change it to a "c"
  if(cName.find_first_of("g") == 0)
  {
    cName[0] = 'c';
  }
  // Otherwise, simply prepend a "c" to the name
  else
  {
    cName.insert(0, "c");
  }
  
  std::string legendTitle = graphsToCompare.at(0)->GetTitle();
  LegendDefinition leg = {legendTitle, legendLabels, "NW"};
  
//   if(!canvasTitle.empty())
//     CreateAndWriteCanvas(cName.c_str(), canvasTitle.c_str(), markerStyles, colors, leg, false, true, graphsToCompare);
//   else
    CreateAndWriteCanvas(cName.c_str(), leg, markerStyles, colors, false, false, graphsToCompare);
}



void HistogramComparator::CompareHistogram(std::vector<std::string> histoNames)
{
//   std::cout << "Inside HistogramComparator::CompareHistogram(std::vector<std::string> histoNames)\n";
  std::string histoNameToCompare;
  std::ostringstream oss;
  oss.str("");
  std::vector<TH1*> histosToCompare;
  // Check that histogram exists in every file
  for(unsigned int iFile=0; iFile<inputFiles.size(); iFile++)
  {
    TH1* histo = (TH1*)(inputFiles.at(iFile)->Get(histoNames[iFile].c_str()));
    if(histo != 0)
    {
      oss << histoNames[iFile] << "_" << iFile;
      histoNameToCompare = oss.str();
      histosToCompare.push_back((TH1*)(histo->Clone(histoNameToCompare.c_str())));
      histosToCompare[histosToCompare.size()-1]->Rebin(8);
      histosToCompare[histosToCompare.size()-1]->GetXaxis()->SetRangeUser(0.,0.1999);
    }
    else
    {
      std::cout << "E R R O R ! Histogram \"" << histoNames[iFile] << "\" does not exist in file " << iFile << "!\n\tNo comparison plots will be produced.\n";
      return;
    }
  }
  
  if(normalizationMethod != 0)
  {
    int result = NormalizeHistograms(histosToCompare);
    if(result != 0)
    {
      std::cout << "E R R O R ! Normalization failed for histogram \"" << histoNames[0] << "\"!\n\tNo comparison plots will be produced.\n";
      return;
    }
  }
  
  outputFile->cd();
  
  std::string cName = histoNames[0];
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
  
  std::string legendTitle = histosToCompare.at(0)->GetTitle();
  LegendDefinition leg = {legendTitle, legendLabels, "NW"};
  
  if(!canvasTitle.empty())
    CreateAndWriteCanvasWithDifferences(cName.c_str(), canvasTitle.c_str(), lineStyles, markerStyles, colors, leg, false, true, histosToCompare);
  else
    CreateAndWriteCanvasWithDifferences(cName.c_str(), lineStyles, markerStyles, colors, leg, false, true, histosToCompare);
}



int HistogramComparator::ComputeNormalizationFactors()
{
//   std::cout << "Inside HistogramComparator::ComputeNormalizationFactors()\n";
  // Look for histogram in all files
  // Keep first as reference
  TH1* histoReference = (TH1*)(inputFiles.at(0)->Get(normalizationHistoName.c_str()));

  if(histoReference)
  {
    // Underflow bin
    int startBin = 0;
    double startX = histoReference->GetXaxis()->GetXmin();
    // Overflow bin
    int endBin = histoReference->GetNbinsX()+1;
    double endX = histoReference->GetXaxis()->GetXmax();
    double factor = 0;
    if(normalizationMethod == 2)
    {
      // Simple integral
      factor = histoReference->Integral(startBin,endBin);
    }
    else if(normalizationMethod == 3)
    {
      for(int iBin = startBin; iBin<=endBin; iBin++)
      {
        double binCenter = histoReference->GetXaxis()->GetBinCenter(iBin);
        double binWidth = histoReference->GetXaxis()->GetBinWidth(iBin);
        double binContent = histoReference->GetBinContent(iBin);
        if(binWidth != 1)
          std::cout << "W A R N I N G ! Using normalization method 3 but bin width is != 1! Resulting normalization will likely be wrong!\n";
        factor+=binContent*binCenter*binWidth;
      }
    }
    for(unsigned int iFile=1; iFile<inputFiles.size();iFile++)
    {
      TH1* histo = (TH1*)(inputFiles.at(iFile)->Get(normalizationHistoName.c_str()));
      if(histo)
      {
        // Check that the axis definition is the same
        if(histo->GetXaxis()->GetXmin() != startX ||
          histo->GetXaxis()->GetXmax() != endX ||
          histo->GetNbinsX()+1 != endBin)
        {
          std::cout << "E R R O R ! Reference histograms for normalization calculation\n\tdo not have the same x axis!\n\tNo normalization will be made.\n";
          return 1;
        }
        double norm;
        if(normalizationMethod == 2)
        {
          double histoIntegral = histo->Integral(startBin,endBin);
          norm = factor/histoIntegral;
        }
        else if(normalizationMethod == 3)
        {
          double factor2 = 0;
          for(int iBin = startBin; iBin<=endBin; iBin++)
          {
            double binCenter = histo->GetXaxis()->GetBinCenter(iBin);
            double binWidth = histo->GetXaxis()->GetBinWidth(iBin);
            double binContent = histo->GetBinContent(iBin);
            if(binWidth != 1)
              std::cout << "W A R N I N G ! Using normalization method 3 but bin width is != 1! Resulting normalization will likely be wrong!\n";
            factor2+=binContent*binCenter*binWidth;
          }
          norm = factor/factor2;
        }
        normalizationFactors.push_back(norm);
      }
      else
      {
        std::cout << "E R R O R ! Reference histogram missing on file " << iFile << "!!!\n";
        return 2;
      }
    }
  }
  else
  {
    std::cout << "E R R O R ! Reference histogram missing on file 0!!!\n";
    return 2;
  }
  return 0;
}



int HistogramComparator::NormalizeHistograms(std::vector<TH1*> histos)
{
//   std::cout << "Inside HistogramComparator::NormalizeHistograms(std::vector<TH1*> histos)\n";
  if(normalizationMethod == 0)
  {
    // Do nothing!
  }
  else if(normalizationMethod == 1)
  {
    // Underflow bin
    int startBin = 0;
    double startX = histos.at(0)->GetXaxis()->GetXmin();
    // Overflow bin
    int endBin = histos.at(0)->GetNbinsX()+1;
    double endX = histos.at(0)->GetXaxis()->GetXmax();
    double factor = histos.at(0)->Integral(startBin,endBin);
    for(unsigned int iHisto = 1; iHisto<histos.size(); iHisto++)
    {
      // Check that the axis definition is the same
      if(histos.at(iHisto)->GetXaxis()->GetXmin() != startX ||
         histos.at(iHisto)->GetXaxis()->GetXmax() != endX ||
         histos.at(iHisto)->GetNbinsX()+1 != endBin)
      {
        std::cout << "E R R O R ! Histograms to normalize do not have the same x axis!\n\tNo normalization will be made.\n";
        return 1;
      }
      double histoIntegral = histos.at(iHisto)->Integral(startBin,endBin);
      double norm = factor/histoIntegral;
      std::cout << "Method " << normalizationMethod << ": Scaling histo " << iHisto << " by factor " << norm << std::endl;
      histos.at(iHisto)->Scale(norm);
    }
  }
  else
  if(normalizationMethod == 2 || normalizationMethod == 3)
  {
    for(unsigned int iHisto = 1; iHisto<histos.size(); iHisto++)
    {
      if(normalizationFactors.at(iHisto-1)<0)
      {
        std::cout << "E R R O R ! Bogus normalization factor (i = " << iHisto << ").\n\tNo normalization will be made.\n";
        return 2;
      }
      std::cout << "Method " << normalizationMethod << ": Scaling histo " << iHisto << " by factor " << normalizationFactors.at(iHisto-1) << std::endl;
      histos.at(iHisto)->Scale(normalizationFactors.at(iHisto-1));
    }
  }
  else if (normalizationMethod == 4)
  {
    // Underflow bin
    int startBin = 0;
    double startX = histos.at(0)->GetXaxis()->GetXmin();
    // Overflow bin
    int endBin = histos.at(0)->GetNbinsX()+1;
    double endX = histos.at(0)->GetXaxis()->GetXmax();
    for(unsigned int iHisto = 0; iHisto<histos.size(); iHisto++)
    {
      // Check that the axis definition is the same
      if(histos.at(iHisto)->GetXaxis()->GetXmin() != startX ||
        histos.at(iHisto)->GetXaxis()->GetXmax() != endX ||
        histos.at(iHisto)->GetNbinsX()+1 != endBin)
      {
        std::cout << "E R R O R ! Histograms to normalize do not have the same x axis!\n\tNo normalization will be made.\n";
        return 1;
      }
      double histoIntegral = histos.at(iHisto)->Integral(startBin,endBin);
      double norm = 1./histoIntegral;
      std::cout << "Method " << normalizationMethod <<": Scaling histo " << iHisto << " by factor " << norm << std::endl;
      histos.at(iHisto)->Scale(norm);
    }
  }
  return 0;
}


#endif // HistogramComparator_cxx
