#ifndef HistogramSlicesMaker_cxx
#define HistogramSlicesMaker_cxx

#include "./HistoPlotter.C"
#include "./HistogramSlicesMaker.h"

// #include "TFile.h"
#include "TKey.h"
#include "TROOT.h"



void HistogramSlicesMaker::Initialize()
{
  inputFile = new TFile(inputFileName.c_str(), "READ");
  outputFile = new TFile(outputFileName.c_str(), "RECREATE");
}



void HistogramSlicesMaker::Initialize(const std::vector<double> lowEdges, const std::vector<double> highEdges)
{
  Initialize();
  SetBinEdges(lowEdges, highEdges);
}



HistogramSlicesMaker::~HistogramSlicesMaker()
{
  inputFile->Close();
  outputFile->Close();
}



void HistogramSlicesMaker::SetBinEdges(const std::vector<double> lowEdges, const std::vector<double> highEdges)
{
  if(lowEdges.size() != highEdges.size())
  {
    std::cout << "E R R O R ! Vectors of bin edges do not have the same size!\n\tNot setting the edges!\n";
    return;
  }
  if(lowEdges.size() < 1)
  {
    std::cout << "E R R O R ! Vectors of bin edges have size 0!\n\tNot setting the edges!\n";
    return;
  }
  if(lowEdges.size() == 1)
  {
    std::cout << "W A R N I N G ! Vectors of bin edges have size 1!\n\tOnly one slice will be produced!\n";
  }
  binLowEdges = lowEdges;
  binHighEdges = highEdges;
  // Setting names
  binNames.clear();
  for(unsigned int i = 0; i < binLowEdges.size(); i++)
  {
    oss.str("");
    oss << binLowEdges.at(i) << "-" << binHighEdges.at(i);
    binNames.push_back(oss.str());
  }
  if(binNames.size()!=binLowEdges.size())
  {
    std::cout << "E R R O R ! Something went bad! Bin names' size is different from bin edges' one!\n\tResetting everything!\n";
    binLowEdges.clear();
    binHighEdges.clear();
    binNames.clear();
  }
}



void HistogramSlicesMaker::MakeSlicesX(std::string yTitle)
{
  TKey* key;
  TIter nextkey(inputFile->GetListOfKeys());

  while((key = (TKey*)nextkey()))
  {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    // Only take 2D histograms
    if (cl->InheritsFrom(TH2::Class()))
    {
      std::string histoName(key->GetName());
      TH2* histo;
      inputFile->GetObject(histoName.c_str(),histo);
      for(unsigned int i = 0; i<binLowEdges.size(); i++)
      {
        std::cout << "Making sliceX " << i << " for histogram " << histoName << std::endl;
        MakeSliceX(histo,i,yTitle);
      }
    }
  }
}



void HistogramSlicesMaker::MakeSlicesY(std::string yTitle)
{
  TKey* key;
  TIter nextkey(inputFile->GetListOfKeys());

  while((key = (TKey*)nextkey()))
  {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    // Only take 2D histograms
    if (cl->InheritsFrom(TH2::Class()))
    {
      std::string histoName(key->GetName());
      TH2* histo;
      inputFile->GetObject(histoName.c_str(),histo);
      for(unsigned int i = 0; i<binLowEdges.size(); i++)
      {
        std::cout << "Making sliceY " << i << " for histogram " << histoName << std::endl;
        MakeSliceY(histo,i,yTitle);
      }
    }
  }
}



void HistogramSlicesMaker::MakeSlicesX(const std::string histoName, std::string yTitle)
{
  TH2* histo;
  inputFile->GetObject(histoName.c_str(),histo);
  if(!histo)
  {
    std::cout << "E R R O R! No TH2* with name " << histoName << " found in the input file!\n\tNo slices will be made!\n";
    return;
  }
  for(unsigned int i=0; i<binLowEdges.size(); i++)
  {
    MakeSliceX(histo,i,yTitle);
  }
}



void HistogramSlicesMaker::MakeSlicesY(const std::string histoName, std::string yTitle)
{
  TH2* histo;
  inputFile->GetObject(histoName.c_str(),histo);
  if(!histo)
  {
    std::cout << "E R R O R! No TH2* with name " << histoName << " found in the input file!\n\tNo slices will be made!\n";
    return;
  }
  for(unsigned int i=0; i<binLowEdges.size(); i++)
  {
    MakeSliceY(histo,i,yTitle);
  }
}



void HistogramSlicesMaker::MakeSliceX(const TH2* histo, const unsigned int iHisto, const std::string yTitle)
{
  unsigned int xLow = histo->GetYaxis()->FindBin(binLowEdges.at(iHisto));
  // Upper edge should not be inclusive - find the previous bin wrt xHigh
  unsigned int xHigh1 = histo->GetYaxis()->FindBin(binHighEdges.at(iHisto));
  double xHighWidth1 = histo->GetYaxis()->GetBinWidth(xHigh1);
  unsigned int xHigh = histo->GetYaxis()->FindBin(binHighEdges.at(iHisto)-xHighWidth1/10);

  std::cout << "xLow = " << xLow << ", xHigh1 = " << xHigh1 << ", xHighWidth1 = " << xHighWidth1 << ", xHigh = " << xHigh << std::endl;

  std::string histoName = histo->GetName();
  histoName += "_SliceX_" + binNames.at(iHisto);
  std::string histoTitle = histo->GetTitle();
  histoTitle += " - " + (std::string)histo->GetYaxis()->GetTitle() + " " + binNames.at(iHisto);

  TH1D* histoSlice = histo->ProjectionX(histoName.c_str(),xLow,xHigh,"e");

  histoSlice->SetTitle(histoTitle.c_str());
  histoSlice->GetYaxis()->SetTitle(yTitle.c_str());

  outputFile->cd();
  histoSlice->Write();
}



void HistogramSlicesMaker::MakeSliceY(const TH2* histo, const unsigned int iHisto, const std::string yTitle)
{
  unsigned int xLow = histo->GetXaxis()->FindBin(binLowEdges.at(iHisto));
  // Upper edge should not be inclusive - find the previous bin wrt xHigh
  unsigned int xHigh1 = histo->GetXaxis()->FindBin(binHighEdges.at(iHisto));
  double xHighWidth1 = histo->GetXaxis()->GetBinWidth(xHigh1);
  unsigned int xHigh = histo->GetXaxis()->FindBin(binHighEdges.at(iHisto)-xHighWidth1/10);

  std::cout << "xLow = " << xLow << ", xHigh1 = " << xHigh1 << ", xHighWidth1 = " << xHighWidth1 << ", xHigh = " << xHigh << std::endl;

  std::string histoName = histo->GetName();
  histoName += "_SliceY_" + binNames.at(iHisto);
  std::string histoTitle = histo->GetTitle();
  histoTitle += " - " + (std::string)histo->GetXaxis()->GetTitle() + " " + binNames.at(iHisto);

  TH1D* histoSlice = histo->ProjectionY(histoName.c_str(),xLow,xHigh,"e");

  histoSlice->SetTitle(histoTitle.c_str());
  histoSlice->GetYaxis()->SetTitle(yTitle.c_str());

  outputFile->cd();
  histoSlice->Write();

}



#endif // HistogramSlicesMaker_cxx
