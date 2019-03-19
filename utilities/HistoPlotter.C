#ifndef HistoPlotter_cxx
#define HistoPlotter_cxx

#include <cmath>
#include <set>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "TStyle.h"
#include "TList.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TLine.h"

#include "HistoPlotter.h"



void HistoPlotter::SetStyle()
{
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(600); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);

  // For the Pad:
  gStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);

  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);

  // For the histo:
  // gStyle->SetHistFillColor(1);
  // gStyle->SetHistFillStyle(0);
  gStyle->SetHistLineColor(1);
  gStyle->SetHistLineStyle(0);
  gStyle->SetHistLineWidth(1);
  // gStyle->SetLegoInnerR(Float_t rad = 0.5);
  // gStyle->SetNumberContours(Int_t number = 20);

  gStyle->SetEndErrorSize(2);
  //  gStyle->SetErrorMarker(20);
  gStyle->SetErrorX(0.);

  gStyle->SetMarkerStyle(20);

  //For the fit/function:
  gStyle->SetOptFit(1);
  gStyle->SetFitFormat("5.4g");
  gStyle->SetFuncColor(2);
  gStyle->SetFuncStyle(1);
  gStyle->SetFuncWidth(1);

  //For the date:
  gStyle->SetOptDate(0);
  // gStyle->SetDateX(Float_t x = 0.01);
  // gStyle->SetDateY(Float_t y = 0.01);

  // For the statistics box:
  gStyle->SetOptFile(0);
  gStyle->SetOptStat(1110); // To display the mean and RMS:   SetOptStat("mr");
  gStyle->SetStatColor(kWhite);
  gStyle->SetStatFont(42);
  gStyle->SetStatFontSize(0.025);
  gStyle->SetStatTextColor(1);
  gStyle->SetStatFormat("6.4g");
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatW(0.15);
  // gStyle->SetStatStyle(Style_t style = 1001);
  // gStyle->SetStatX(Float_t x = 0);
  // gStyle->SetStatY(Float_t y = 0);

  // Margins:
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.14);
  gStyle->SetPadLeftMargin(0.18);
  gStyle->SetPadRightMargin(0.035);

  // For the Global title:
  gStyle->SetOptTitle(1);
  gStyle->SetTitleFont(42);
  gStyle->SetTitleColor(1);
  gStyle->SetTitleTextColor(1);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleFontSize(0.04);
  // gStyle->SetTitleH(0); // Set the height of the title box
  // gStyle->SetTitleW(0); // Set the width of the title box
  gStyle->SetTitleX(0.55); // Set the position of the title box
  gStyle->SetTitleY(0.95); // Set the position of the title box
  // gStyle->SetTitleStyle(Style_t style = 1001);
  gStyle->SetTitleAlign(22);
  gStyle->SetTitleBorderSize(0);

  // For the axis titles:
  gStyle->SetTitleColor(1, "XYZ");
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetTitleSize(0.05, "XYZ");
  // gStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // gStyle->SetTitleYSize(Float_t size = 0.02);
  gStyle->SetTitleXOffset(1.1);
  gStyle->SetTitleYOffset(1.5);
  // gStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

  // For the axis labels:
  gStyle->SetLabelColor(1, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetLabelOffset(0.007, "XYZ");
  gStyle->SetLabelSize(0.04, "XYZ");

  // For the axis:
  gStyle->SetAxisColor(1, "XYZ");
  gStyle->SetStripDecimals(kTRUE);
  gStyle->SetTickLength(0.03, "XYZ");
  gStyle->SetNdivisions(505, "X");
  gStyle->SetNdivisions(510, "YZ");
  gStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  gStyle->SetPadTickY(1);

  // Default to no-log plots:
  gStyle->SetOptLogx(0);
  gStyle->SetOptLogy(0);
  gStyle->SetOptLogz(0);

  // Postscript options:
  gStyle->SetPaperSize(20.,20.);
  // gStyle->SetLineScalePS(Float_t scale = 3);
  // gStyle->SetLineStyleString(Int_t i, const char* text);
  // gStyle->SetHeaderPS(const char* header);
  // gStyle->SetTitlePS(const char* pstitle);

  // gStyle->SetBarOffset(Float_t baroff = 0.5);
  // gStyle->SetBarWidth(Float_t barwidth = 0.5);
  // gStyle->SetPaintTextFormat(const char* format = "g");
  // gStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // gStyle->SetTimeOffset(Double_t toffset);
  // gStyle->SetHistMinimumZero(kTRUE);

  gStyle->SetPalette(1);
  
  gStyle->cd();


  // Old options - not used
//   // Title
//   gStyle->SetTitleFillColor(0);
//   gStyle->SetTitleBorderSize(1);
//
//   // Statistics box
//   gStyle->SetStatColor(0);
//   gStyle->SetStatBorderSize(1);
//   gStyle->SetStatH(0.15);
//   gStyle->SetOptStat(1110);
//
//   // Canvas and pad
//   gStyle->SetFrameBorderMode(0);
//   gStyle->SetFrameBorderSize(0);
//   gStyle->SetPadRightMargin(0.05);
//   gStyle->SetLabelSize(0.12);
}



template <typename T>
T* HistoPlotter::Create1DHistogram(const char* hName,
                                   const char* hTitle,
                                   const unsigned int xNBins,
                                   const double xMin,
                                   const double xMax,
                                   const char* xTitle,
                                   const char* yTitle)
{
  T* h = new T(hName, hTitle, xNBins, xMin, xMax);
  h->GetXaxis()->SetTitle(xTitle);
  h->GetYaxis()->SetTitle(yTitle);
  h->Sumw2();
  return h;
}



template <typename T>
T* HistoPlotter::Create1DHistogram(const char* hName,
                                   const char* hTitle,
                                   const unsigned int xNBins,
                                   const double* xBins,
                                   const char* xTitle,
                                   const char* yTitle)
{
  T* h = new T(hName, hTitle, xNBins, xBins);
  h->GetXaxis()->SetTitle(xTitle);
  h->GetYaxis()->SetTitle(yTitle);
  h->Sumw2();
  return h;
}



// template <typename T>
// std::vector<T*>* HistoPlotter::CreateVectorOf1DHistograms(const char* hName,
//                                                          const char* hTitle,
//                                                          const unsigned int xNBins,
//                                                          const double xMin,
//                                                          const double xMax,
//                                                          const char* xTitle,
//                                                          const char* yTitle,
//                                                          const std::string binningVariable,
//                                                          const std::vector<double> vLowBins)
// {
//   std::vector<std::string> vTitleLabels;
//   std::vector<std::string> vNameLabels;
//   std::string lowest = "-Inf";
//   std::string highest = "Inf";
// //   std::string lowestName = "mInf";
// //   std::string highestName = "Inf";
//   
//   std::ostringstream oss;
//   oss.str("");
//   oss << " (" << lowest << " <= " << binningVariable << " < " << vLowBins[0] << ")";
//   vTitleLabels.push_back(oss.str());
//   oss.str("");
//   oss << "_" << binningVariable << "_" << lowest << "_" << vLowBins[0];
//   vNameLabels.push_back(oss.str());
//   for(unsigned int i = 0; i < vLowBins.size() - 1; i++)
//   {
//     oss.str("");
//     oss << " (" << vLowBins[i] << " <= " << binningVariable << " < " << vLowBins[i+1] << ")";
//     vTitleLabels.push_back(oss.str());
//     oss.str("");
//     oss << "_" << binningVariable << "_" << vLowBins[i] << "_" << vLowBins[i+1];
//     vNameLabels.push_back(oss.str());
//   }
//   oss.str("");
//   oss << " (" << vLowBins[vLowBins.size()-1] << " <= " << binningVariable << " < " << highest << ")";
//   vTitleLabels.push_back(oss.str());
//   oss.str("");
//   oss << "_" << binningVariable << "_" << vLowBins[vLowBins.size()-1] << "_" << highest;
//   vNameLabels.push_back(oss.str());
//   
//   std::vector<T*> *vResults = new std::vector<T*>;
//   
//   for(unsigned int iBin = 0; iBin < vTitleLabels.size(); iBin++)
//   {
//     std::string hNewName = hName + vNameLabels[iBin];
//     std::string hNewTitle = hTitle + vTitleLabels[iBin];
//     T* result = Create1DHistogram<TH1D>(hNewName.c_str(), hNewTitle.c_str(), xNBins, xMin, xMax, xTitle, yTitle);
//     vResults->push_back(result);
//   }
//   return vResults;
// }



void HistoPlotter::BayesDivide(const TH1* pass, const TH1* total, TGraphAsymmErrors* graph)
{
  const std::string xTitle = graph->GetXaxis()->GetTitle();
  const std::string yTitle = graph->GetYaxis()->GetTitle();

  graph->BayesDivide(pass,total);

  graph->GetXaxis()->SetTitle(xTitle.c_str());
  graph->GetYaxis()->SetTitle(yTitle.c_str());
}



void HistoPlotter::ClopperPearsonDivide(const TH1* pass, const TH1* total, TGraphAsymmErrors* graph)
{
  const std::string xTitle = graph->GetXaxis()->GetTitle();
  const std::string yTitle = graph->GetYaxis()->GetTitle();

  graph->Divide(pass,total,"cp");

  graph->GetXaxis()->SetTitle(xTitle.c_str());
  graph->GetYaxis()->SetTitle(yTitle.c_str());
}



template <typename T>
T* HistoPlotter::CreateGraph(const char* gName,
                             const char* gTitle,
                             const char* xTitle,
                             const char* yTitle,
                             const unsigned int nPoints)
{

  T* g;
  if(nPoints == 0)
    g = new T();
  else
    g = new T(nPoints);
  // Dummy histogram
  g->SetHistogram(new TH1F("","",nPoints!=0?nPoints:10,0.,1.));
  g->SetName(gName);
  g->SetTitle(gTitle);
  g->GetHistogram()->GetXaxis()->SetTitle(xTitle);
  g->GetHistogram()->GetYaxis()->SetTitle(yTitle);
  return g;
}



std::pair<std::vector<double>,std::vector<double> >
HistoPlotter::FindDifferenceBinDivision(const TH2* hFirst,
                                        const TH2* hSecond)
{
  double diffXLowEdge = 0;
  double diffXUpEdge = 0;
  double diffYLowEdge = 0;
  double diffYUpEdge = 0;
  if(hFirst->GetXaxis()->GetXmin() != hSecond->GetXaxis()->GetXmin())
  {
    std::cout << "\nW A R N I N G ! Lower edge of x axis is not the same for the two histograms! The higher between the two will be used!\n";
    diffXLowEdge = std::max(hFirst->GetXaxis()->GetXmin(),hSecond->GetXaxis()->GetXmin());
  }
  else
  {
    diffXLowEdge = hFirst->GetXaxis()->GetXmin();
  }
  if(hFirst->GetYaxis()->GetXmin() != hSecond->GetYaxis()->GetXmin())
  {
    std::cout << "\nW A R N I N G ! Lower edge of y axis is not the same for the two histograms! The higher between the two will be used!\n";
    diffYLowEdge = std::max(hFirst->GetYaxis()->GetXmin(),hSecond->GetYaxis()->GetXmin());
  }
  else
  {
    diffYLowEdge = hFirst->GetYaxis()->GetXmin();
  }
  if(hFirst->GetXaxis()->GetXmax() != hSecond->GetXaxis()->GetXmax())
  {
    std::cout << "\nW A R N I N G ! Upper edge of x axis is not the same for the two histograms! The lower between the two will be used!\n";
    diffXUpEdge = std::min(hFirst->GetXaxis()->GetXmax(),hSecond->GetXaxis()->GetXmax());
  }
  else
  {
    diffXUpEdge = hFirst->GetXaxis()->GetXmax();
  }
  if(hFirst->GetYaxis()->GetXmax() != hSecond->GetYaxis()->GetXmax())
  {
    std::cout << "\nW A R N I N G ! Upper edge of y axis is not the same for the two histograms! The lower between the two will be used!\n";
    diffYUpEdge = std::min(hFirst->GetYaxis()->GetXmax(),hSecond->GetYaxis()->GetXmax());
  }
  else
  {
    diffYUpEdge = hFirst->GetYaxis()->GetXmax();
  }
  
  std::set<double> sXBins;
  std::set<double> sYBins;
  
  for(int i = 1; i <= hFirst->GetXaxis()->GetNbins(); i++)
  {
    sXBins.insert(hFirst->GetXaxis()->GetBinLowEdge(i));
  }
  for(int i = 1; i <= hSecond->GetXaxis()->GetNbins(); i++)
  {
    sXBins.insert(hSecond->GetXaxis()->GetBinLowEdge(i));
  }
  sXBins.insert(hFirst->GetXaxis()->GetXmax());
  sXBins.insert(hSecond->GetXaxis()->GetXmax());
  
  for(int i = 1; i <= hFirst->GetYaxis()->GetNbins(); i++)
  {
    sYBins.insert(hFirst->GetYaxis()->GetBinLowEdge(i));
  }
  for(int i = 1; i <= hSecond->GetYaxis()->GetNbins(); i++)
  {
    sYBins.insert(hSecond->GetYaxis()->GetBinLowEdge(i));
  }
  sYBins.insert(hFirst->GetYaxis()->GetXmax());
  sYBins.insert(hSecond->GetYaxis()->GetXmax());
  
  std::vector<double> vXBins;
  std::copy(sXBins.begin(), sXBins.end(), std::back_inserter(vXBins));
  // Now remove bins that are past the limits
  for(std::vector<double>::iterator it = vXBins.begin(); it != vXBins.end(); /* */)
  {
    if(*it < diffXLowEdge || *it > diffXUpEdge)
      vXBins.erase(it);
    else
      it++;
  }
  
  std::vector<double> vYBins;
  std::copy(sYBins.begin(), sYBins.end(), std::back_inserter(vYBins));
  // Now remove bins that are past the limits
  for(std::vector<double>::iterator it = vYBins.begin(); it != vYBins.end(); /* */)
  {
    if(*it < diffYLowEdge || *it > diffYUpEdge)
      vYBins.erase(it);
    else
      it++;
  }
  
  return std::make_pair(vXBins,vYBins);
}



bool
HistoPlotter::SameBins(const TH1* hFirst,
                       const TH1* hSecond)
{
  if(hFirst->GetDimension() != hSecond->GetDimension())
    return false;
  if(hFirst->GetXaxis()->GetXmin() != hSecond->GetXaxis()->GetXmin())
    return false;
  if(hFirst->GetXaxis()->GetXmax() != hSecond->GetXaxis()->GetXmax())
    return false;
  if(hFirst->GetXaxis()->GetNbins() != hSecond->GetXaxis()->GetNbins())
    return false;
  for(int iBin = 1; iBin <= hFirst->GetXaxis()->GetNbins(); iBin++)
  {
    if(hFirst->GetXaxis()->GetBinLowEdge(iBin) != hSecond->GetXaxis()->GetBinLowEdge(iBin))
      return false;
  }
  if(hFirst->GetDimension() > 1)
  {
    if(hFirst->GetYaxis()->GetXmin() != hSecond->GetYaxis()->GetXmin())
      return false;
    if(hFirst->GetYaxis()->GetXmax() != hSecond->GetYaxis()->GetXmax())
      return false;
    if(hFirst->GetYaxis()->GetNbins() != hSecond->GetYaxis()->GetNbins())
      return false;
    for(int iBin = 1; iBin <= hFirst->GetYaxis()->GetNbins(); iBin++)
    {
      if(hFirst->GetYaxis()->GetBinLowEdge(iBin) != hSecond->GetYaxis()->GetBinLowEdge(iBin))
        return false;
    }
  }
  if(hFirst->GetDimension() > 2)
  {
    if(hFirst->GetZaxis()->GetXmin() != hSecond->GetZaxis()->GetXmin())
      return false;
    if(hFirst->GetZaxis()->GetXmax() != hSecond->GetZaxis()->GetXmax())
      return false;
    if(hFirst->GetZaxis()->GetNbins() != hSecond->GetZaxis()->GetNbins())
      return false;
    for(int iBin = 1; iBin <= hFirst->GetZaxis()->GetNbins(); iBin++)
    {
      if(hFirst->GetZaxis()->GetBinLowEdge(iBin) != hSecond->GetZaxis()->GetBinLowEdge(iBin))
        return false;
    }
  }
  return true;
}



TH2D*
HistoPlotter::SymmetrizeHistogramInX(const TH2D* histo,
                                     bool aroundLowEdge)
{
  double centerOfSymmetry;
  if(aroundLowEdge)
    centerOfSymmetry = histo->GetXaxis()->GetXmin();
  else
    centerOfSymmetry = histo->GetXaxis()->GetXmax();
  
  std::vector<double> vYBins;
  for(int i = 1; i <= histo->GetYaxis()->GetNbins(); i++)
  {
    vYBins.push_back(histo->GetYaxis()->GetBinLowEdge(i));
  }
  vYBins.push_back(histo->GetYaxis()->GetXmax());
  
  std::vector<double> vXBinsIn;
  for(int i = 1; i <= histo->GetXaxis()->GetNbins(); i++)
  {
    vXBinsIn.push_back(histo->GetXaxis()->GetBinLowEdge(i));
  }
  vXBinsIn.push_back(histo->GetXaxis()->GetXmax());
  
  std::vector<double> vXBins;
  
  if(centerOfSymmetry == vXBinsIn[0])
  {
    for(int i = vXBinsIn.size()-1; i >= 0; i--)
    {
      vXBins.push_back(2*centerOfSymmetry - vXBinsIn[i]);
    }
    for(unsigned int i = 1; i < vXBinsIn.size(); i++)
    {
      vXBins.push_back(vXBinsIn[i]);
    }
  }
  else if(centerOfSymmetry == vXBinsIn[vXBinsIn.size()-1])
  {
    for(unsigned int i = 0; i < vXBinsIn.size()-1; i++)
    {
      vXBins.push_back(vXBinsIn[i]);
    }
    for(int i = vXBinsIn.size()-1; i >= 0; i--)
    {
      vXBins.push_back(2*centerOfSymmetry - vXBinsIn[i]);
    }
  }
#ifdef mgDEBUG
  std::cout << "vXBinsIn\n";
  for(unsigned int i = 0; i < vXBinsIn.size(); i++)
  {
    std::cout << i << " " << vXBinsIn[i] << std::endl;
  }
  std::cout << "vXBins\n";
  for(unsigned int i = 0; i < vXBins.size(); i++)
  {
    std::cout << i << " " << vXBins[i] << std::endl;
  }
  std::cout << "vYBins\n";
  for(unsigned int i = 0; i < vYBins.size(); i++)
  {
    std::cout << i << " " << vYBins[i] << std::endl;
  }
#endif //mgDEBUG
  std::string name = histo->GetName();
  std::string title = histo->GetTitle();
  std::string xAxisTitle = histo->GetXaxis()->GetTitle();
  std::string yAxisTitle = histo->GetYaxis()->GetTitle();
  
  TH2D* histoNew = Create2DHistogram<TH2D>(name.c_str(), title.c_str(), vXBins.size()-1, vXBins, vYBins.size()-1, vYBins, xAxisTitle.c_str(), yAxisTitle.c_str());
  
  for(unsigned int ix = 0; ix < vXBinsIn.size()-1; ix++)
  {
    for(unsigned int iy = 0; iy < vYBins.size()-1; iy++)
    {
      int xBin1, xBin2;
      if(aroundLowEdge)
      {
        xBin1 = ix + vXBinsIn.size();
        xBin2 = vXBinsIn.size() - ix - 1;
      }
      else
      {
        xBin1 = ix;
        xBin2 = 2 * vXBinsIn.size() - ix - 1;
      }
      double content = histo->GetBinContent(ix+1, iy+1);
      histoNew->SetBinContent(xBin1, iy+1, content);
      histoNew->SetBinContent(xBin2, iy+1, content);
    }
  }
  
  return histoNew;
}



template <typename T>
T* HistoPlotter::Create2DHistogram(const char* hName,
                                   const char* hTitle,
                                   const unsigned int xNBins,
                                   const double xMin,
                                   const double xMax,
                                   const unsigned int yNBins,
                                   const double yMin,
                                   const double yMax,
                                   const char* xTitle,
                                   const char* yTitle)
{
  T* h = new T(hName, hTitle, xNBins, xMin, xMax, yNBins, yMin, yMax);
  h->GetXaxis()->SetTitle(xTitle);
  h->GetYaxis()->SetTitle(yTitle);
  h->Sumw2();
  return h;
}



template <typename T>
T* HistoPlotter::Create2DHistogram(const char* hName,
                                   const char* hTitle,
                                   const unsigned int xNBins,
                                   const double xMin,
                                   const double xMax,
                                   const unsigned int yNBins,
                                   const double yMin,
                                   const double yMax,
                                   const char* xTitle,
                                   const char* yTitle,
                                   const char* zTitle)
{
  T* h = Create2DHistogram<T>(hName, hTitle, xNBins, xMin, xMax, yNBins, yMin, yMax, xTitle, yTitle);
  h->GetZaxis()->SetTitle(zTitle);

  return h;
}



template <typename T>
T* HistoPlotter::Create2DHistogram(const char* hName,
                                   const char* hTitle,
                                   const unsigned int xNBins,
                                   const std::vector<double> xBins,
                                   const unsigned int yNBins,
                                   const std::vector<double> yBins,
                                   const char* xTitle,
                                   const char* yTitle)
{
  if(xBins.size() != xNBins+1 || yBins.size() != yNBins+1)
  {
    std::cout << "E R R O R ! Create2DHistogram: vector of bin sizes must have nbins+1 entries!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  T* h = new T(hName, hTitle, xNBins, &xBins[0], yNBins, &yBins[0]);
  h->GetXaxis()->SetTitle(xTitle);
  h->GetYaxis()->SetTitle(yTitle);
  h->Sumw2();
  return h;
}



void HistoPlotter::SetCanvasStyle(TPad* c, const bool logScaleX, const bool logScaleY, const bool logScaleZ)
{
  SetCanvasStyle(c, logScaleX, logScaleY);
  if(logScaleZ)
    c->SetLogz(1);
  else
    c->SetLogz(0);
}



void HistoPlotter::SetCanvasStyle(TPad* c, const bool logScaleX, const bool logScaleY)
{
  c->UseCurrentStyle();
//   c->SetBatch(kTRUE);
//   c->SetFillColor(0);
//   c->SetFrameBorderMode(0);
  if(logScaleX)
    c->SetLogx(1);
  else
    c->SetLogx(0);
  if(logScaleY)
    c->SetLogy(1);
  else
    c->SetLogy(0);
//   c->SetCanvasSize(600,600);
//   c->SetWindowSize(600,600);
  c->Update();
}



void HistoPlotter::SetProfileStyle(TProfile* p)
{
  p->UseCurrentStyle();
  p->SetMarkerStyle(3);
}



void HistoPlotter::SetHistoStyle(TH1* h, const unsigned int lineStyle, const unsigned int markerStyle, const unsigned int lineColor)
{
  h->UseCurrentStyle();
  h->SetLineWidth(2);
  h->SetLineStyle(lineStyle);
  h->SetMarkerStyle(markerStyle);
  h->SetMarkerColor(lineColor);
  h->SetLineColor(lineColor);
  h->SetTitleFont(gStyle->GetTitleFont());
  h->SetTitleSize(gStyle->GetTitleFontSize(), "");
  h->SetStats(kFALSE);
}



void HistoPlotter::SetStackHistoStyle(TH1* h, const unsigned int color)
{
  h->UseCurrentStyle();
  h->SetLineWidth(1);
  h->SetMarkerColor(1);
  h->SetLineColor(1);
  h->SetFillColor(color);
  h->SetTitleFont(gStyle->GetTitleFont());
  h->SetTitleSize(gStyle->GetTitleFontSize(), "");
  h->SetStats(kFALSE);
}



void HistoPlotter::SetGraphStyle(TGraph* g, const unsigned int markerStyle, const unsigned int lineColor)
{
  g->UseCurrentStyle();
  g->SetMarkerStyle(markerStyle);
  g->SetMarkerColor(lineColor);
  g->SetLineColor(lineColor);
}



template <typename T>
void HistoPlotter::CreateAndWriteCanvases(const unsigned int lineStyle, 
                                          const unsigned int markerStyle, 
                                          const unsigned int color, 
                                          const bool logScaleX, 
                                          const bool logScaleY, 
                                          std::vector<T*> vHistos)
{
  if(vHistos.size() == 0)
  {
    std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvases(...) : vector of histograms to be plotted is empty!\n";
    std::cout << "                No canvases will be created!\n";
    return;
  }
  else
  {
    for(unsigned int iHisto = 0; iHisto < vHistos.size(); iHisto++)
    {
      std::string canvasName = vHistos[iHisto]->GetName();
//       std::cout << "before: canvasName = " << canvasName << std::endl;
      if(canvasName.substr(0,1) == "h")
      {
        canvasName.replace(0,1,"c");
//         std::cout << "after:  canvasName = " << canvasName << std::endl;
      }
      else
      {
        canvasName = "c" + canvasName;
      }
      CreateAndWriteCanvas(canvasName.c_str(),lineStyle,markerStyle,color,logScaleX,logScaleY,vHistos[iHisto]);
    }
  }
}



template <typename T>
void HistoPlotter::CreateAndWriteCanvases(const char* plotOptions,
                                          const bool logScaleX,
                                          const bool logScaleY,
                                          const bool logScaleZ,
                                          std::vector<T*> vHistos)
{
  if(vHistos.size() == 0)
  {
    std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvases(...) : vector of histograms to be plotted is empty!\n";
    std::cout << "                No canvases will be created!\n";
    return;
  }
  else
  {
    for(unsigned int iHisto = 0; iHisto < vHistos.size(); iHisto++)
    {
      std::string canvasName = vHistos[iHisto]->GetName();
      std::cout << "before: canvasName = " << canvasName << std::endl;
      if(canvasName.substr(0,1) == "h")
      {
        canvasName.replace(0,1,"c");
        std::cout << "after:  canvasName = " << canvasName << std::endl;
      }
      else
      {
        canvasName = "c" + canvasName;
      }
      CreateAndWriteCanvas(canvasName.c_str(),plotOptions,logScaleX,logScaleY,logScaleZ,vHistos[iHisto]);
    }
  }
}


template <typename T>
void HistoPlotter::CreateAndWriteCanvases(const unsigned int markerStyle, 
                                          const unsigned int color, 
                                          const bool logScaleX, 
                                          const bool logScaleY, 
                                          std::vector<T*> vGraphs)
{
  if(vGraphs.size() == 0)
  {
    std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvases(...) : vector of graphs to be plotted is empty!\n";
    std::cout << "                No canvases will be created!\n";
    return;
  }
  else
  {
    for(unsigned int iGraph = 0; iGraph < vGraphs.size(); iGraph++)
    {
      std::string canvasName = vGraphs[iGraph]->GetName();
      //       std::cout << "before: canvasName = " << canvasName << std::endl;
      if(canvasName.substr(0,1) == "g")
      {
        canvasName.replace(0,1,"c");
        //         std::cout << "after:  canvasName = " << canvasName << std::endl;
      }
      else
      {
        canvasName = "c" + canvasName;
      }
      CreateAndWriteCanvas(canvasName.c_str(),markerStyle,color,logScaleX,logScaleY,vGraphs[iGraph]);
    }
  }
}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const unsigned int lineStyle,
                                        const unsigned int markerStyle,
                                        const unsigned int color,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        TH1* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY);

  std::string plotOptions;

  SetHistoStyle(histo, lineStyle, markerStyle, color);
  if(lineStyle!=0 || markerStyle==0)
  {
    plotOptions = "hist";

  }
  else
  {
    plotOptions = "e1";
  }

  // Actually draw the histogram
  c->cd();
  histo->Draw(plotOptions.c_str());

  // Write the canvas to file and exit
  c->RedrawAxis();
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const char* cTitle,
                                        const unsigned int lineStyle,
                                        const unsigned int markerStyle,
                                        const unsigned int color,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        TH1* histo)
{
  histo->SetTitle(cTitle);
  CreateAndWriteCanvas(cName, lineStyle, markerStyle, color, logScaleX, logScaleY, histo);
}


void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const unsigned int lineStyle,
                                        const unsigned int markerStyle,
                                        const unsigned int color,
                                        const LegendDefinition leg,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        const TH1* histo)
{
  const unsigned int lineStyles[1] = {lineStyle};
  const unsigned int markerStyles[1] = {markerStyle};
  const unsigned int colors[1] = {color};

  CreateAndWriteCanvas(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histo, NULL);

}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const char* cTitle,
                                        const unsigned int lineStyle,
                                        const unsigned int markerStyle,
                                        const unsigned int color,
                                        const LegendDefinition leg,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        TH1* histo)
{
  const unsigned int lineStyles[1] = {lineStyle};
  const unsigned int markerStyles[1] = {markerStyle};
  const unsigned int colors[1] = {color};

  histo->SetTitle(cTitle);

  CreateAndWriteCanvas(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histo, NULL);

}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const unsigned int* lineStyles,
                                        const unsigned int* markerStyles,
                                        const unsigned int* colors,
                                        const LegendDefinition leg,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  std::cout << "histos.size = " << histos.size() << std::endl;

  CreateAndWriteCanvas(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);

  va_end(arguments);
  return;
}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const char* cTitle,
                                        const unsigned int* lineStyles,
                                        const unsigned int* markerStyles,
                                        const unsigned int* colors,
                                        const LegendDefinition leg,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }

  CreateAndWriteCanvas(cName, cTitle, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);

  va_end(arguments);
  return;
}



void HistoPlotter::CreateAndWriteCanvasWithPulls(const char* cName,
                                                 const unsigned int* lineStyles,
                                                 const unsigned int* markerStyles,
                                                 const unsigned int* colors,
                                                 const LegendDefinition leg,
                                                 const bool logScaleX,
                                                 const bool logScaleY,
                                                 ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }

  CreateAndWriteCanvasWithPulls(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);

  va_end(arguments);
  return;
}



void HistoPlotter::CreateAndWriteCanvasWithRatios(const char* cName,
                                                  const unsigned int* lineStyles,
                                                  const unsigned int* markerStyles,
                                                  const unsigned int* colors,
                                                  const LegendDefinition leg,
                                                  const bool logScaleX,
                                                  const bool logScaleY,
                                                  ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteCanvasWithRatios(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}





void HistoPlotter::CreateAndWriteCanvasWithDifferences(const char* cName,
                                                       const unsigned int* lineStyles,
                                                       const unsigned int* markerStyles,
                                                       const unsigned int* colors,
                                                       const LegendDefinition leg,
                                                       const bool logScaleX,
                                                       const bool logScaleY,
                                                       ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteCanvasWithDifferences(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}





void HistoPlotter::CreateAndWriteStackCanvasWithDifferences(const char* cName,
                                                            const unsigned int markerStyle,
                                                            const unsigned int* colors,
                                                            const LegendDefinition leg,
                                                            const bool logScaleX,
                                                            const bool logScaleY,
                                                            ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteStackCanvasWithDifferences(cName, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}






void HistoPlotter::CreateAndWriteStackCanvasWithPulls(const char* cName,
                                                      const unsigned int markerStyle,
                                                      const unsigned int* colors,
                                                      const LegendDefinition leg,
                                                      const bool logScaleX,
                                                      const bool logScaleY,
                                                      ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteStackCanvasWithPulls(cName, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}





void HistoPlotter::CreateAndWriteStackCanvasWithRatios(const char* cName,
                                                       const unsigned int markerStyle,
                                                       const unsigned int* colors,
                                                       const LegendDefinition leg,
                                                       const bool logScaleX,
                                                       const bool logScaleY,
                                                       ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteStackCanvasWithRatios(cName, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}





void HistoPlotter::CreateAndWriteCanvasWithPulls(const char* cName,
                                                 const char* cTitle,
                                                 const unsigned int* lineStyles,
                                                 const unsigned int* markerStyles,
                                                 const unsigned int* colors,
                                                 const LegendDefinition leg,
                                                 const bool logScaleX,
                                                 const bool logScaleY,
                                                 ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }

  CreateAndWriteCanvasWithPulls(cName, cTitle, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);

  va_end(arguments);
  return;
}



void HistoPlotter::CreateAndWriteCanvasWithRatios(const char* cName,
                                                  const char* cTitle,
                                                  const unsigned int* lineStyles,
                                                  const unsigned int* markerStyles,
                                                  const unsigned int* colors,
                                                  const LegendDefinition leg,
                                                  const bool logScaleX,
                                                  const bool logScaleY,
                                                  ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteCanvasWithRatios(cName, cTitle, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}






void HistoPlotter::CreateAndWriteCanvasWithDifferences(const char* cName,
                                                       const char* cTitle,
                                                       const unsigned int* lineStyles,
                                                       const unsigned int* markerStyles,
                                                       const unsigned int* colors,
                                                       const LegendDefinition leg,
                                                       const bool logScaleX,
                                                       const bool logScaleY,
                                                       ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteCanvasWithDifferences(cName, cTitle, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}







void HistoPlotter::CreateAndWriteStackCanvasWithDifferences(const char* cName,
                                                            const char* cTitle,
                                                            const unsigned int markerStyle,
                                                            const unsigned int* colors,
                                                            const LegendDefinition leg,
                                                            const bool logScaleX,
                                                            const bool logScaleY,
                                                            ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteStackCanvasWithDifferences(cName, cTitle, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}






void HistoPlotter::CreateAndWriteStackCanvasWithPulls(const char* cName,
                                                      const char* cTitle,
                                                      const unsigned int markerStyle,
                                                      const unsigned int* colors,
                                                      const LegendDefinition leg,
                                                      const bool logScaleX,
                                                      const bool logScaleY,
                                                      ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteStackCanvasWithPulls(cName, cTitle, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}






void HistoPlotter::CreateAndWriteStackCanvasWithRatios(const char* cName,
                                                       const char* cTitle,
                                                       const unsigned int markerStyle,
                                                       const unsigned int* colors,
                                                       const LegendDefinition leg,
                                                       const bool logScaleX,
                                                       const bool logScaleY,
                                                       ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TH1*> histos;
  for(unsigned int i = 0; /* */; i++)
  {
    histos.push_back(va_arg(arguments,TH1*));
    // A null pointer must be the last argument in the list
    if(*(histos.end()-1) == NULL)
    {
      histos.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }
  
  CreateAndWriteStackCanvasWithRatios(cName, cTitle, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
  va_end(arguments);
  return;
}





void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const char* cTitle,
                                        const unsigned int* lineStyles,
                                        const unsigned int* markerStyles,
                                        const unsigned int* colors,
                                        const LegendDefinition leg,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        std::vector<TH1*> histos)
{
  // If there are no arguments, complain and return
  if(histos.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return;
  }
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    histos.at(iHisto)->SetTitle(cTitle);
  }

  CreateAndWriteCanvas(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);

}




void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const unsigned int* lineStyles,
                                        const unsigned int* markerStyles,
                                        const unsigned int* colors,
                                        const LegendDefinition leg,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        std::vector<TH1*> histos)
{
  // If there are no arguments, complain and return
  if(histos.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return;
  }

  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY);

  std::vector<std::string> plotOptions;

  for(unsigned int i = 0; i < histos.size(); i++)
  {
    SetHistoStyle(histos[i], lineStyles[i], markerStyles[i], colors[i]);
    if(lineStyles[i]!=0 || markerStyles[i]==0)
      plotOptions.push_back("hist");
    else
      plotOptions.push_back("e1");
  }
  // Actually draw the histograms
  c->cd();
  histos[0]->Draw(plotOptions[0].c_str());
  for(unsigned int i = 1; i < histos.size(); i++)
  {
    if(plotOptions[i].find("e1") != plotOptions[i].npos || plotOptions[i].find("hist") != plotOptions[i].npos)
      histos[i]->Draw((plotOptions[i]+" same").c_str());
    else
      histos[i]->Draw("same");
  }

  PlotLegend(c, histos, plotOptions, leg);
  // Write the canvas to file and exit
  c->RedrawAxis();
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvasWithPulls(const char* cName,
                                                 const unsigned int* lineStyles,
                                                 const unsigned int* markerStyles,
                                                 const unsigned int* colors,
                                                 const LegendDefinition leg,
                                                 const bool logScaleX,
                                                 const bool logScaleY,
                                                 std::vector<TH1*> histos)
{
//   std::cout << "Inside HistoPlotter::CreateAndWriteCanvasWithPulls(...,std::vector<TH1*> histos)\n";
  // If there are less than two histograms, complain and return
  if(histos.size()<2)
  {
    std::cout << "ERROR!!! Less than two histograms!!!\nReturning.\n";
    return;
  }
  // The differences make sense only for 1D histograms
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
//     const char* className = histos.at(iHisto)->GetClassName();
//     TClass* cl = gROOT->GetClass(className);
    if(histos[iHisto]->InheritsFrom(TH2::Class()) || histos[iHisto]->InheritsFrom(TH3::Class()))
    {
      std::cout << "ERROR!!! Difference plots are supported only for 1D histograms!!!\nReturning.\n";
      return;
    }
  }

  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName,histos[0]->GetTitle());
  SetCanvasStyle(c, logScaleX, logScaleY);

  c->cd();
//   c->Divide(1,2,0,0);
  TPad* pad1 = new TPad("pad1","",0,0.33,1,1,0,0,0);
  TPad* pad2 = new TPad("pad2","",0,0,1,0.33,0,0,0);
  SetCanvasStyle(pad1,logScaleX,logScaleY);
  pad1->SetBottomMargin(0.001);
  pad2->SetTopMargin(0.005);
  pad2->SetGridy(kTRUE);
  pad2->SetBottomMargin(pad2->GetBottomMargin()*3);
  pad1->Draw();
  pad2->Draw();
//   c->cd(1);
//   c->SetPad("pad1","",0,0.3,1,1,0,0,0);
//   c->cd(2);
//   c->SetPad("pad2","",0,0,1,0.3,0,0,0);

//   c->cd(1);

  pad1->cd();

  std::vector<std::string> plotOptions;

  for(unsigned int i = 0; i < histos.size(); i++)
  {
    SetHistoStyle(histos[i], lineStyles[i], markerStyles[i], colors[i]);
    if(lineStyles[i]!=0 || markerStyles[i]==0)
      plotOptions.push_back("hist");
    else
      plotOptions.push_back("e1");
  }
  
  double labelSizeFactor1 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad1->GetHNDC();
  double labelSizeFactor2 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad2->GetHNDC();
//   std::cout << "labelSizeFactor1 = " << labelSizeFactor1 << std::endl;
//   std::cout << "labelSizeFactor2 = " << labelSizeFactor2 << std::endl;
  
  // Actually draw the histograms
//   c->cd(1);
  pad1->cd();
  histos[0]->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor1, "XYZ");
  histos[0]->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor1, "XYZ");
  histos[0]->GetYaxis()->SetTitleOffset(histos[0]->GetYaxis()->GetTitleOffset()/labelSizeFactor1);
  histos[0]->Draw(plotOptions[0].c_str());
  double yMin = histos[0]->GetMinimum();
  double yMax = histos[0]->GetMaximum();
  //std::cout << "yMin = " << yMin << ", yMax = " << yMax << std::endl;
  double yMinNew = yMin + (yMax-yMin)/100000000;
  double yMaxNew = yMax + (yMax-yMin)*0.2;
//   if(yMaxNew<yMax)
//     yMaxNew = yMax;
  histos[0]->GetYaxis()->SetRangeUser(yMinNew,yMaxNew);
//   histos[0]->GetYaxis()->SetBinLabel(1,"");
  for(unsigned int i = 1; i < histos.size(); i++)
  {
    pad1->cd();
    histos[i]->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor1, "XYZ");
    histos[i]->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor1, "XYZ");
    if(plotOptions[i].find("e1") != plotOptions[i].npos)
      histos[i]->Draw((plotOptions[i]+" same").c_str());
    else
      histos[i]->Draw("same");

    pad2->cd();
    TH1::AddDirectory(false);
    TH1D* histoDiff = (TH1D*)(histos[i]->Clone());
    TH1::AddDirectory(true);
    //     histoDiff->Add(histos[0],histos[i],1,-1);
    
    for(int iBin = 1; iBin <= histos[0]->GetNbinsX(); iBin++)
    {
      // std::cout << "iBin = " << iBin << std::endl;
      double binContent1 = ((TH1D*)histos[0])->GetBinContent(iBin);
      double binContent2 = ((TH1D*)histos[i])->GetBinContent(iBin);
      double binError1 = ((TH1D*)histos[0])->GetBinError(iBin);
      double binError2 = ((TH1D*)histos[i])->GetBinError(iBin);
      double error = sqrt(binError1*binError1 + binError2*binError2);
      double result = 0;
      if(error != 0)
        result = (binContent1-binContent2)/error;
      else
        result = 9999;
      // std::cout << "result = " << result << std::endl;
      histoDiff->SetBinContent(iBin, result);
      histoDiff->SetBinError(iBin,0.01);
    }
//     histoDiff->Add(histos[0],histos[i],1,-1);
    
    
//     histoDiff->Divide(histos[0],histos[i]);
    histoDiff->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor2, "XYZ");
    histoDiff->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor2, "XYZ");
    histoDiff->GetYaxis()->SetTitleOffset(histoDiff->GetYaxis()->GetTitleOffset()/labelSizeFactor2);
    if(histos.size() == 2)
      histoDiff->GetYaxis()->SetTitle("Pull");
    else
      histoDiff->GetYaxis()->SetTitle("Pulls");
    histoDiff->SetTitle("");
    histoDiff->SetStats(kFALSE);
//     std::string yTitle = histos[0]->GetYaxis()->GetTitle();
//     std::string yTitleDiff = "#Delta(" + yTitle + ") / " + yTitle;
//     histoDiff->GetYaxis()->SetTitle(yTitleDiff.c_str());

    // TODO: take plot options
    if(i==1)
    {

      histoDiff->Draw("e1");
      histoDiff->GetYaxis()->SetRangeUser(-2.999,2.999);
      histoDiff->GetYaxis()->SetNdivisions(505);
    }
    else
      histoDiff->Draw("e1 same");

  }

//   TLine* lineAtOne = new TLine(histos[0]->GetXaxis()->GetXmin(),1,histos[0]->GetXaxis()->GetXmax(),1);
//   lineAtOne->Draw();

  pad1->cd();

  PlotLegend(pad1, histos, plotOptions, leg);
  // Write the canvas to file and exit
  pad1->Update();
  pad2->Update();
  c->Update();
  c->Write();

  delete c;

  return;
}



void HistoPlotter::CreateAndWriteCanvasWithRatios(const char* cName,
                                                  const unsigned int* lineStyles,
                                                  const unsigned int* markerStyles,
                                                  const unsigned int* colors,
                                                  const LegendDefinition leg,
                                                  const bool logScaleX,
                                                  const bool logScaleY,
                                                  std::vector<TH1*> histos)
{
//   std::cout << "Inside HistoPlotter::CreateAndWriteCanvasWithRatios(...,std::vector<TH1*> histos)\n";
  // If there are less than two histograms, complain and return
  if(histos.size()<2)
  {
    std::cout << "ERROR!!! Less than two histograms!!!\nReturning.\n";
    return;
  }
  // The differences make sense only for 1D histograms
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    //     const char* className = histos.at(iHisto)->GetClassName();
    //     TClass* cl = gROOT->GetClass(className);
    if(histos[iHisto]->InheritsFrom(TH2::Class()) || histos[iHisto]->InheritsFrom(TH3::Class()))
    {
      std::cout << "ERROR!!! Difference plots are supported only for 1D histograms!!!\nReturning.\n";
      return;
    }
  }
  
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName,histos[0]->GetTitle());
  SetCanvasStyle(c, logScaleX, logScaleY);
  
  c->cd();
  //   c->Divide(1,2,0,0);
  TPad* pad1 = new TPad("pad1","",0,0.33,1,1,0,0,0);
  TPad* pad2 = new TPad("pad2","",0,0,1,0.33,0,0,0);
  SetCanvasStyle(pad1,logScaleX,logScaleY);
  pad1->SetBottomMargin(0.001);
  pad2->SetTopMargin(0.005);
  pad2->SetGridy(kTRUE);
  pad2->SetBottomMargin(pad2->GetBottomMargin()*3);
  pad1->Draw();
  pad2->Draw();
  //   c->cd(1);
  //   c->SetPad("pad1","",0,0.3,1,1,0,0,0);
  //   c->cd(2);
  //   c->SetPad("pad2","",0,0,1,0.3,0,0,0);
  
  //   c->cd(1);
  
  pad1->cd();
  
  std::vector<std::string> plotOptions;
  
  for(unsigned int i = 0; i < histos.size(); i++)
  {
    SetHistoStyle(histos[i], lineStyles[i], markerStyles[i], colors[i]);
    if(lineStyles[i]!=0 || markerStyles[i]==0)
      plotOptions.push_back("hist");
    else
      plotOptions.push_back("e1");
  }
  
  double labelSizeFactor1 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad1->GetHNDC();
  double labelSizeFactor2 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad2->GetHNDC();
//   std::cout << "labelSizeFactor1 = " << labelSizeFactor1 << std::endl;
//   std::cout << "labelSizeFactor2 = " << labelSizeFactor2 << std::endl;
  
  // Actually draw the histograms
  //   c->cd(1);
  pad1->cd();
  histos[0]->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor1, "XYZ");
  histos[0]->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor1, "XYZ");
  histos[0]->GetYaxis()->SetTitleOffset(histos[0]->GetYaxis()->GetTitleOffset()/labelSizeFactor1);
  histos[0]->Draw(plotOptions[0].c_str());
  double yMin = histos[0]->GetMinimum();
  double yMax = histos[0]->GetMaximum();
  // std::cout << "yMin = " << yMin << ", yMax = " << yMax << std::endl;
  double yMinNew = yMin + (yMax-yMin)/100000000;
  double yMaxNew = yMax + (yMax-yMin)*0.2;
  //   if(yMaxNew<yMax)
  //     yMaxNew = yMax;
  histos[0]->GetYaxis()->SetRangeUser(yMinNew,yMaxNew);
  //   histos[0]->GetYaxis()->SetBinLabel(1,"");
  for(unsigned int i = 1; i < histos.size(); i++)
  {
    pad1->cd();
    if(plotOptions[i].find("e1") != plotOptions[i].npos)
      histos[i]->Draw((plotOptions[i]+" same").c_str());
    else
      histos[i]->Draw("same");
    
    pad2->cd();
    TH1::AddDirectory(false);
    TH1D* histoDiff = (TH1D*)(histos[i]->Clone());
    TH1::AddDirectory(true);
    //     histoDiff->Add(histos[0],histos[i],1,-1);
    
    for(int iBin = 1; iBin <= histos[0]->GetNbinsX(); iBin++)
    {
//       std::cout << "iBin = " << iBin << std::endl;
      double binContent1 = ((TH1D*)histos[0])->GetBinContent(iBin);
      double binContent2 = ((TH1D*)histos[i])->GetBinContent(iBin);
      double binError1 = ((TH1D*)histos[0])->GetBinError(iBin);
      double binError2 = ((TH1D*)histos[i])->GetBinError(iBin);
      double error = 0;
      double result = 0;
      if(binContent2 != 0 && binContent1 != 0)
      {
//         result = (binContent1/binContent2);
        result = (binContent2/binContent1);
        error = sqrt(binError1/binContent1*binError1/binContent1 + binError2/binContent2*binError2/binContent2)*result;
      }
      else
      {
        result = 9999;
        error = 1;
      }
      // std::cout << "result = " << result << std::endl;
      histoDiff->SetBinContent(iBin, result);
      histoDiff->SetBinError(iBin,error);
    }
    //     histoDiff->Add(histos[0],histos[i],1,-1);
    
    
    //     histoDiff->Divide(histos[0],histos[i]);
    histoDiff->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor2, "XYZ");
    histoDiff->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor2, "XYZ");
    histoDiff->GetYaxis()->SetTitleOffset(histoDiff->GetYaxis()->GetTitleOffset()/labelSizeFactor2);
    if(histos.size() == 2)
      histoDiff->GetYaxis()->SetTitle("Ratio");
    else
      histoDiff->GetYaxis()->SetTitle("Ratios");
    histoDiff->SetTitle("");
    histoDiff->SetStats(kFALSE);
    //     std::string yTitle = histos[0]->GetYaxis()->GetTitle();
    //     std::string yTitleDiff = "#Delta(" + yTitle + ") / " + yTitle;
    //     histoDiff->GetYaxis()->SetTitle(yTitleDiff.c_str());
    
    // TODO: take plot options
    if(i==1)
    {
      
      histoDiff->Draw("e1");
//       histoDiff->GetYaxis()->SetRangeUser(0.501,1.499);
      histoDiff->GetYaxis()->SetRangeUser(0.,1.999);
      histoDiff->GetYaxis()->SetNdivisions(505);
    }
    else
      histoDiff->Draw("e1 same");
    
  }
  
  //   TLine* lineAtOne = new TLine(histos[0]->GetXaxis()->GetXmin(),1,histos[0]->GetXaxis()->GetXmax(),1);
  //   lineAtOne->Draw();
  
  pad1->cd();
  
  PlotLegend(pad1, histos, plotOptions, leg);
  // Write the canvas to file and exit
  pad1->Update();
  pad2->Update();
  c->Update();
  c->Write();
  
  delete c;
  
  return;
}



void HistoPlotter::CreateAndWriteCanvasWithDifferences(const char* cName,
                                                       const unsigned int* lineStyles,
                                                       const unsigned int* markerStyles,
                                                       const unsigned int* colors,
                                                       const LegendDefinition leg,
                                                       const bool logScaleX,
                                                       const bool logScaleY,
                                                       std::vector<TH1*> histos)
{
//   std::cout << "Inside HistoPlotter::CreateAndWriteCanvasWithDifferences(...,std::vector<TH1*> histos)\n";
  // If there are less than two histograms, complain and return
  if(histos.size()<2)
  {
    std::cout << "ERROR!!! Less than two histograms!!!\nReturning.\n";
    return;
  }
  // The differences make sense only for 1D histograms
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    //     const char* className = histos.at(iHisto)->GetClassName();
    //     TClass* cl = gROOT->GetClass(className);
    if(histos[iHisto]->InheritsFrom(TH2::Class()) || histos[iHisto]->InheritsFrom(TH3::Class()))
    {
      std::cout << "ERROR!!! Difference plots are supported only for 1D histograms!!!\nReturning.\n";
      return;
    }
  }
  
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName,histos[0]->GetTitle());
  SetCanvasStyle(c, logScaleX, logScaleY);
  
  c->cd();
  //   c->Divide(1,2,0,0);
  TPad* pad1 = new TPad("pad1","",0,0.33,1,1,0,0,0);
  TPad* pad2 = new TPad("pad2","",0,0,1,0.33,0,0,0);
  SetCanvasStyle(pad1,logScaleX,logScaleY);
  pad1->SetBottomMargin(0.001);
  pad2->SetTopMargin(0.005);
  pad2->SetGridy(kTRUE);
  pad2->SetBottomMargin(pad2->GetBottomMargin()*3);
  pad1->Draw();
  pad2->Draw();
  //   c->cd(1);
  //   c->SetPad("pad1","",0,0.3,1,1,0,0,0);
  //   c->cd(2);
  //   c->SetPad("pad2","",0,0,1,0.3,0,0,0);
  
  //   c->cd(1);
  
  pad1->cd();
  
  std::vector<std::string> plotOptions;
  
  for(unsigned int i = 0; i < histos.size(); i++)
  {
    SetHistoStyle(histos[i], lineStyles[i], markerStyles[i], colors[i]);
    if(lineStyles[i]!=0 || markerStyles[i]==0)
      plotOptions.push_back("");
    else
      plotOptions.push_back("e1");
  }
  
  double labelSizeFactor1 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad1->GetHNDC();
  double labelSizeFactor2 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad2->GetHNDC();
  //   std::cout << "labelSizeFactor1 = " << labelSizeFactor1 << std::endl;
  //   std::cout << "labelSizeFactor2 = " << labelSizeFactor2 << std::endl;
  
  // Actually draw the histograms
  //   c->cd(1);
  pad1->cd();
  histos[0]->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor1, "XYZ");
  histos[0]->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor1, "XYZ");
  histos[0]->GetYaxis()->SetTitleOffset(histos[0]->GetYaxis()->GetTitleOffset()/labelSizeFactor1);
  histos[0]->Draw(plotOptions[0].c_str());
  double yMin = histos[0]->GetMinimum();
  double yMax = histos[0]->GetMaximum();
  // std::cout << "yMin = " << yMin << ", yMax = " << yMax << std::endl;
  double yMinNew = yMin + (yMax-yMin)/100000000;
  double yMaxNew = yMax + (yMax-yMin)*0.2;
  //   if(yMaxNew<yMax)
  //     yMaxNew = yMax;
  histos[0]->GetYaxis()->SetRangeUser(yMinNew,yMaxNew);
  //   histos[0]->GetYaxis()->SetBinLabel(1,"");
  for(unsigned int i = 1; i < histos.size(); i++)
  {
    pad1->cd();
    if(plotOptions[i].find("e1") != plotOptions[i].npos)
      histos[i]->Draw((plotOptions[i]+" same").c_str());
    else
      histos[i]->Draw("same");
    
//     double labelSizeFactor = pad1->GetHNDC() / pad2->GetHNDC();
    // std::cout << "labelSizeFactor = " << labelSizeFactor << std::endl;
    
    pad2->cd();
    TH1::AddDirectory(false);
    TH1D* histoDiff = (TH1D*)(histos[i]->Clone());
    TH1::AddDirectory(true);
    //     histoDiff->Add(histos[0],histos[i],1,-1);
    
    for(int iBin = 1; iBin <= histos[0]->GetNbinsX(); iBin++)
    {
      // std::cout << "iBin = " << iBin << std::endl;
      double binContent1 = ((TH1D*)histos[0])->GetBinContent(iBin);
      double binContent2 = ((TH1D*)histos[i])->GetBinContent(iBin);
//       double binError1 = ((TH1D*)histos[0])->GetBinError(iBin);
//       double binError2 = ((TH1D*)histos[i])->GetBinError(iBin);
//       double error = sqrt(binError1*binError1 + binError2*binError2);
      double result = 0;
      if(binContent1 != 0)
        result = (binContent1-binContent2)/binContent1;
      else
        result = 9999;
      // std::cout << "result = " << result << std::endl;
      histoDiff->SetBinContent(iBin, result);
      histoDiff->SetBinError(iBin,0.01);
    }
    //     histoDiff->Add(histos[0],histos[i],1,-1);
    
    
    //     histoDiff->Divide(histos[0],histos[i]);
    histoDiff->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor2, "XYZ");
    histoDiff->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor2, "XYZ");
    histoDiff->GetYaxis()->SetTitleOffset(histoDiff->GetYaxis()->GetTitleOffset()/labelSizeFactor2);
    if(histos.size() == 2)
      histoDiff->GetYaxis()->SetTitle("Difference");
    else
      histoDiff->GetYaxis()->SetTitle("Differences");
    histoDiff->SetTitle("");
    histoDiff->SetStats(kFALSE);
    //     std::string yTitle = histos[0]->GetYaxis()->GetTitle();
    //     std::string yTitleDiff = "#Delta(" + yTitle + ") / " + yTitle;
    //     histoDiff->GetYaxis()->SetTitle(yTitleDiff.c_str());
    
    // TODO: take plot options
    if(i==1)
    {
      
      histoDiff->Draw("e1");
      histoDiff->GetYaxis()->SetRangeUser(-0.499,0.499);
      histoDiff->GetYaxis()->SetNdivisions(505);
    }
    else
      histoDiff->Draw("e1 same");
    
  }
  
  //   TLine* lineAtOne = new TLine(histos[0]->GetXaxis()->GetXmin(),1,histos[0]->GetXaxis()->GetXmax(),1);
  //   lineAtOne->Draw();
  
  pad1->cd();
  
  PlotLegend(pad1, histos, plotOptions, leg);
  // Write the canvas to file and exit
  pad1->Update();
  pad2->Update();
  c->Update();
  c->Write();
  
  delete c;
  
  return;
}



void HistoPlotter::CreateAndWriteCanvasWithPulls(const char* cName,
                                                 const char* cTitle,
                                                 const unsigned int* lineStyles,
                                                 const unsigned int* markerStyles,
                                                 const unsigned int* colors,
                                                 const LegendDefinition leg,
                                                 const bool logScaleX,
                                                 const bool logScaleY,
                                                 std::vector<TH1*> histos)
{
  if(histos.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return;
  }

  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    histos.at(iHisto)->SetTitle(cTitle);
  }

  CreateAndWriteCanvasWithPulls(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);

}



void HistoPlotter::CreateAndWriteCanvasWithRatios(const char* cName,
                                                  const char* cTitle,
                                                  const unsigned int* lineStyles,
                                                  const unsigned int* markerStyles,
                                                  const unsigned int* colors,
                                                  const LegendDefinition leg,
                                                  const bool logScaleX,
                                                  const bool logScaleY,
                                                  std::vector<TH1*> histos)
{
  if(histos.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return;
  }
  
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    histos.at(iHisto)->SetTitle(cTitle);
  }
  
  CreateAndWriteCanvasWithRatios(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);
  
}



void HistoPlotter::CreateAndWriteCanvasWithDifferences(const char* cName,
                                                       const char* cTitle,
                                                       const unsigned int* lineStyles,
                                                       const unsigned int* markerStyles,
                                                       const unsigned int* colors,
                                                       const LegendDefinition leg,
                                                       const bool logScaleX,
                                                       const bool logScaleY,
                                                       std::vector<TH1*> histos)
{
  if(histos.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return;
  }
  
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    histos.at(iHisto)->SetTitle(cTitle);
  }
  
  CreateAndWriteCanvasWithDifferences(cName, lineStyles, markerStyles, colors, leg, logScaleX, logScaleY, histos);
  
}



void HistoPlotter::CreateAndWriteStackCanvasWithDifferences(const char* cName,
                                                            const char* cTitle,
                                                            const unsigned int markerStyle,
                                                            const unsigned int* colors,
                                                            const LegendDefinition leg,
                                                            const bool logScaleX,
                                                            const bool logScaleY,
                                                            std::vector<TH1*> histos)
{
  if(histos.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return;
  }
  
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    histos.at(iHisto)->SetTitle(cTitle);
  }
  
  CreateAndWriteStackCanvasWithDifferences(cName, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
}



void HistoPlotter::CreateAndWriteStackCanvasWithPulls(const char* cName,
                                                      const char* cTitle,
                                                      const unsigned int markerStyle,
                                                      const unsigned int* colors,
                                                      const LegendDefinition leg,
                                                      const bool logScaleX,
                                                      const bool logScaleY,
                                                      std::vector<TH1*> histos)
{
  if(histos.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return;
  }
  
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    histos.at(iHisto)->SetTitle(cTitle);
  }
  
  CreateAndWriteStackCanvasWithPulls(cName, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
}



void HistoPlotter::CreateAndWriteStackCanvasWithRatios(const char* cName,
                                                       const char* cTitle,
                                                       const unsigned int markerStyle,
                                                       const unsigned int* colors,
                                                       const LegendDefinition leg,
                                                       const bool logScaleX,
                                                       const bool logScaleY,
                                                       std::vector<TH1*> histos)
{
  if(histos.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return;
  }
  
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    histos.at(iHisto)->SetTitle(cTitle);
  }
  
  CreateAndWriteStackCanvasWithRatios(cName, markerStyle, colors, leg, logScaleX, logScaleY, histos);
  
}



void HistoPlotter::CreateAndWriteStackCanvasWithDifferences(const char* cName,
                                                            const unsigned int markerStyle,
                                                            const unsigned int* colors,
                                                            const LegendDefinition leg,
                                                            const bool logScaleX,
                                                            const bool logScaleY,
                                                            std::vector<TH1*> histos)
{
//    std::cout << "Inside HistoPlotter::CreateAndWriteStackCanvasWithDifferences(...,std::vector<TH1*> histos)\n";
  // If there are less than two histograms, complain and return
  if(histos.size()<2)
  {
    std::cout << "ERROR!!! Less than two histograms!!!\nReturning.\n";
    return;
  }
  // The differences make sense only for 1D histograms
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    //     const char* className = histos.at(iHisto)->GetClassName();
    //     TClass* cl = gROOT->GetClass(className);
    if(histos[iHisto]->InheritsFrom(TH2::Class()) || histos[iHisto]->InheritsFrom(TH3::Class()))
    {
      std::cout << "ERROR!!! Difference plots are supported only for 1D histograms!!!\nReturning.\n";
      return;
    }
  }
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName,histos[0]->GetTitle());
  SetCanvasStyle(c, logScaleX, logScaleY);
  
  c->cd();
  //   c->Divide(1,2,0,0);
  TPad* pad1 = new TPad("pad1","",0,0.33,1,1,0,0,0);
  TPad* pad2 = new TPad("pad2","",0,0,1,0.33,0,0,0);
  SetCanvasStyle(pad1,logScaleX,logScaleY);
  pad1->SetBottomMargin(0.001);
  pad2->SetTopMargin(0.005);
  pad2->SetGridy(kTRUE);
  pad2->SetBottomMargin(pad2->GetBottomMargin()*3);
  pad1->Draw();
  pad2->Draw();
  //   c->cd(1);
  //   c->SetPad("pad1","",0,0.3,1,1,0,0,0);
  //   c->cd(2);
  //   c->SetPad("pad2","",0,0,1,0.3,0,0,0);
  
  //   c->cd(1);
  
  pad1->cd();
  
  std::vector<std::string> plotOptions;
  
  SetHistoStyle(histos[0], 0, markerStyle, colors[0]);
  plotOptions.push_back("e x0");
  std::string canvasName = cName;
  std::string stackName = "hs" + canvasName.substr(1,canvasName.size()-1);
  THStack* histoStack = new THStack(stackName.c_str(),"");
  TH1D* hSum = (TH1D*)histos[1]->Clone("hSum");
  hSum->SetMarkerStyle(0);
  hSum->SetFillStyle(0);
  
  for(unsigned int i = 1; i < histos.size(); i++)
  {
    SetStackHistoStyle(histos[i], colors[i]);
    histoStack->Add(histos[i]);
    plotOptions.push_back("fill");
    if(i > 1)
      hSum->Add(hSum,histos[i]);
  }
  
  double labelSizeFactor1 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad1->GetHNDC();
  double labelSizeFactor2 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad2->GetHNDC();
  //   std::cout << "labelSizeFactor1 = " << labelSizeFactor1 << std::endl;
  //   std::cout << "labelSizeFactor2 = " << labelSizeFactor2 << std::endl;
  
  // Actually draw the histograms
  //   c->cd(1);
  pad1->cd();
  histos[0]->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor1, "XYZ");
  histos[0]->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor1, "XYZ");
  histos[0]->GetYaxis()->SetTitleOffset(histos[0]->GetYaxis()->GetTitleOffset()/labelSizeFactor1);
  histos[0]->Draw(plotOptions[0].c_str());
  double yMin = histos[0]->GetMinimum();
  double yMax = histos[0]->GetMaximum();
  // std::cout << "yMin = " << yMin << ", yMax = " << yMax << std::endl;
  double yMinNew = yMin + (yMax-yMin)/100000000;
  double yMaxNew = yMax + (yMax-yMin)*0.2;
  //   if(yMaxNew<yMax)
  //     yMaxNew = yMax;
  histos[0]->GetYaxis()->SetRangeUser(yMinNew,yMaxNew);
  //   histos[0]->GetYaxis()->SetBinLabel(1,"");
  pad1->cd();
  histoStack->Draw("hist same");
  hSum->SetLineWidth(3);
  hSum->Draw("hist same");
  histos[0]->Draw((plotOptions[0]+"same").c_str());
 
  pad2->cd();
  TH1::AddDirectory(false);
  TH1D* histoDiff = (TH1D*)(histos[0]->Clone());
  TH1::AddDirectory(true);
  //     histoDiff->Add(histos[0],histos[i],1,-1);
  histoDiff->SetMarkerColor(1);
  histoDiff->SetLineColor(1);
  
  for(int iBin = 1; iBin <= histos[0]->GetNbinsX(); iBin++)
  {
    // std::cout << "iBin = " << iBin << std::endl;
    double binContent1 = ((TH1D*)histos[0])->GetBinContent(iBin);
    double binContent2 = 0.;
    for(unsigned int iHisto = 1; iHisto < histos.size(); iHisto++)
    {
      binContent2 += ((TH1D*)histos[iHisto])->GetBinContent(iBin);
    }     
    //       double binError1 = ((TH1D*)histos[0])->GetBinError(iBin);
    //       double binError2 = ((TH1D*)histos[i])->GetBinError(iBin);
    //       double error = sqrt(binError1*binError1 + binError2*binError2);
    double result = 0;
    if(binContent1 != 0)
      result = (binContent1-binContent2)/binContent1;
    else
      result = 9999;
    // std::cout << "result = " << result << std::endl;
      histoDiff->SetBinContent(iBin, result);
      histoDiff->SetBinError(iBin,0.01);
  }
  //     histoDiff->Add(histos[0],histos[i],1,-1);
  
  
  //     histoDiff->Divide(histos[0],histos[i]);
  histoDiff->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor2, "XYZ");
  histoDiff->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor2, "XYZ");
  histoDiff->GetYaxis()->SetTitleOffset(histoDiff->GetYaxis()->GetTitleOffset()/labelSizeFactor2*labelSizeFactor1);
  histoDiff->SetTitle("");
  histoDiff->GetYaxis()->SetTitle("Difference");
  histoDiff->SetStats(kFALSE);
  //     std::string yTitle = histos[0]->GetYaxis()->GetTitle();
  //     std::string yTitleDiff = "#Delta(" + yTitle + ") / " + yTitle;
  //     histoDiff->GetYaxis()->SetTitle(yTitleDiff.c_str());
  
  // TODO: take plot options
  histoDiff->Draw("e x0");
  histoDiff->GetYaxis()->SetRangeUser(-0.499,0.499);
  histoDiff->GetYaxis()->SetNdivisions(505);
  
  //   TLine* lineAtOne = new TLine(histos[0]->GetXaxis()->GetXmin(),1,histos[0]->GetXaxis()->GetXmax(),1);
  //   lineAtOne->Draw();
  
  pad1->cd();
  
  PlotLegend(pad1, histos[0], histoStack, plotOptions, leg);
  // Write the canvas to file and exit
  pad1->RedrawAxis();
  pad2->RedrawAxis();
  pad1->Update();
  pad2->Update();
  c->Update();
  c->Write();
  
  delete c;
  
  return;
}



void HistoPlotter::CreateAndWriteStackCanvasWithPulls(const char* cName,
                                                      const unsigned int markerStyle,
                                                      const unsigned int* colors,
                                                      const LegendDefinition leg,
                                                      const bool logScaleX,
                                                      const bool logScaleY,
                                                      std::vector<TH1*> histos)
{
  //    std::cout << "Inside HistoPlotter::CreateAndWriteStackCanvasWithPulls(...,std::vector<TH1*> histos)\n";
  // If there are less than two histograms, complain and return
  if(histos.size()<2)
  {
    std::cout << "ERROR!!! Less than two histograms!!!\nReturning.\n";
    return;
  }
  // The differences make sense only for 1D histograms
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    //     const char* className = histos.at(iHisto)->GetClassName();
    //     TClass* cl = gROOT->GetClass(className);
    if(histos[iHisto]->InheritsFrom(TH2::Class()) || histos[iHisto]->InheritsFrom(TH3::Class()))
    {
      std::cout << "ERROR!!! Difference plots are supported only for 1D histograms!!!\nReturning.\n";
      return;
    }
  }
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName,histos[0]->GetTitle());
  SetCanvasStyle(c, logScaleX, logScaleY);
  
  c->cd();
  //   c->Divide(1,2,0,0);
  TPad* pad1 = new TPad("pad1","",0,0.33,1,1,0,0,0);
  TPad* pad2 = new TPad("pad2","",0,0,1,0.33,0,0,0);
  SetCanvasStyle(pad1,logScaleX,logScaleY);
  pad1->SetBottomMargin(0.001);
  pad2->SetTopMargin(0.005);
  pad2->SetGridy(kTRUE);
  pad2->SetBottomMargin(pad2->GetBottomMargin()*3);
  pad1->Draw();
  pad2->Draw();
  //   c->cd(1);
  //   c->SetPad("pad1","",0,0.3,1,1,0,0,0);
  //   c->cd(2);
  //   c->SetPad("pad2","",0,0,1,0.3,0,0,0);
  
  //   c->cd(1);
  
  pad1->cd();
  
  std::vector<std::string> plotOptions;
  
  SetHistoStyle(histos[0], 0, markerStyle, colors[0]);
  plotOptions.push_back("e x0");
  std::string canvasName = cName;
  std::string stackName = "hs" + canvasName.substr(1,canvasName.size()-1);
  THStack* histoStack = new THStack(stackName.c_str(),"");
  TH1D* hSum = (TH1D*)histos[1]->Clone("hSum");
  hSum->SetMarkerStyle(0);
  hSum->SetFillStyle(0);
  
  for(unsigned int i = 1; i < histos.size(); i++)
  {
    SetStackHistoStyle(histos[i], colors[i]);
    histoStack->Add(histos[i]);
    plotOptions.push_back("fill");
    if(i > 1)
      hSum->Add(hSum,histos[i]);
  }
  
  double labelSizeFactor1 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad1->GetHNDC();
  double labelSizeFactor2 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad2->GetHNDC();
//   std::cout << "labelSizeFactor1 = " << labelSizeFactor1 << std::endl;
//   std::cout << "labelSizeFactor2 = " << labelSizeFactor2 << std::endl;
  
  // Actually draw the histograms
  //   c->cd(1);
  pad1->cd();
  histos[0]->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor1, "XYZ");
  histos[0]->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor1, "XYZ");
  histos[0]->GetYaxis()->SetTitleOffset(histos[0]->GetYaxis()->GetTitleOffset()/labelSizeFactor1);
  histos[0]->Draw(plotOptions[0].c_str());
  double yMin = histos[0]->GetMinimum();
  double yMax = histos[0]->GetMaximum();
  // std::cout << "yMin = " << yMin << ", yMax = " << yMax << std::endl;
  double yMinNew = yMin + (yMax-yMin)/100000000;
  double yMaxNew = yMax + (yMax-yMin)*0.2;
  //   if(yMaxNew<yMax)
  //     yMaxNew = yMax;
  histos[0]->GetYaxis()->SetRangeUser(yMinNew,yMaxNew);
  //   histos[0]->GetYaxis()->SetBinLabel(1,"");
  pad1->cd();
  histoStack->Draw("hist same");
  hSum->SetLineWidth(3);
  hSum->Draw("hist same");
  histos[0]->Draw((plotOptions[0]+"same").c_str());
  
  pad2->cd();
  TH1::AddDirectory(false);
  TH1D* histoDiff = (TH1D*)(histos[0]->Clone());
  TH1::AddDirectory(true);
  //     histoDiff->Add(histos[0],histos[i],1,-1);
  histoDiff->SetMarkerColor(1);
  histoDiff->SetLineColor(1);
  
  for(int iBin = 1; iBin <= histos[0]->GetNbinsX(); iBin++)
  {
    // std::cout << "iBin = " << iBin << std::endl;
    double binContent1 = ((TH1D*)histos[0])->GetBinContent(iBin);
    double binContent2 = 0.;
    for(unsigned int iHisto = 1; iHisto < histos.size(); iHisto++)
    {
      binContent2 += ((TH1D*)histos[iHisto])->GetBinContent(iBin);
    }
    double binError1 = ((TH1D*)histos[0])->GetBinError(iBin);
    double binError2 = 0;
    for(unsigned int iHisto = 1; iHisto < histos.size(); iHisto++)
    {
      double iError = ((TH1D*)histos[iHisto])->GetBinError(iBin);
      binError2 += sqrt(binError2*binError2 + iError*iError);
    }
    double error = sqrt(binError1*binError1 + binError2*binError2);
//     double error = binError1;
    double result = 0;
    if(error != 0)
      result = (binContent1-binContent2)/error;
    else
      result = 9999;
    // std::cout << "result = " << result << std::endl;
      histoDiff->SetBinContent(iBin, result);
      histoDiff->SetBinError(iBin,0.01);
  }
  //     histoDiff->Add(histos[0],histos[i],1,-1);
  
  
  //     histoDiff->Divide(histos[0],histos[i]);
  histoDiff->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor2, "XYZ");
  histoDiff->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor2, "XYZ");
  histoDiff->GetYaxis()->SetTitleOffset(histoDiff->GetYaxis()->GetTitleOffset()/labelSizeFactor2*labelSizeFactor1);
  histoDiff->SetTitle("");
  histoDiff->GetYaxis()->SetTitle("Pull");
  histoDiff->SetStats(kFALSE);
  //     std::string yTitle = histos[0]->GetYaxis()->GetTitle();
  //     std::string yTitleDiff = "#Delta(" + yTitle + ") / " + yTitle;
  //     histoDiff->GetYaxis()->SetTitle(yTitleDiff.c_str());
  
  // TODO: take plot options
  histoDiff->Draw("e x0");
  histoDiff->GetYaxis()->SetRangeUser(-2.999,2.999);
  histoDiff->GetYaxis()->SetNdivisions(505);
  
  //   TLine* lineAtOne = new TLine(histos[0]->GetXaxis()->GetXmin(),1,histos[0]->GetXaxis()->GetXmax(),1);
  //   lineAtOne->Draw();
  
  pad1->cd();
  
  PlotLegend(pad1, histos[0], histoStack, plotOptions, leg);
  // Write the canvas to file and exit
  pad1->RedrawAxis();
  pad2->RedrawAxis();
  pad1->Update();
  pad2->Update();
  c->Update();
  c->Write();
  
  delete c;
  
  return;
  
}



void HistoPlotter::CreateAndWriteStackCanvasWithRatios(const char* cName,
                                                       const unsigned int markerStyle,
                                                       const unsigned int* colors,
                                                       const LegendDefinition leg,
                                                       const bool logScaleX,
                                                       const bool logScaleY,
                                                       std::vector<TH1*> histos)
{
  //    std::cout << "Inside HistoPlotter::CreateAndWriteStackCanvasWithRatios(...,std::vector<TH1*> histos)\n";
  // If there are less than two histograms, complain and return
  if(histos.size()<2)
  {
    std::cout << "ERROR!!! Less than two histograms!!!\nReturning.\n";
    return;
  }
  // The differences make sense only for 1D histograms
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    //     const char* className = histos.at(iHisto)->GetClassName();
    //     TClass* cl = gROOT->GetClass(className);
    if(histos[iHisto]->InheritsFrom(TH2::Class()) || histos[iHisto]->InheritsFrom(TH3::Class()))
    {
      std::cout << "ERROR!!! Difference plots are supported only for 1D histograms!!!\nReturning.\n";
      return;
    }
  }
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName,histos[0]->GetTitle());
  SetCanvasStyle(c, logScaleX, logScaleY);
  
  c->cd();
  //   c->Divide(1,2,0,0);
  TPad* pad1 = new TPad("pad1","",0,0.33,1,1,0,0,0);
  TPad* pad2 = new TPad("pad2","",0,0,1,0.33,0,0,0);
  SetCanvasStyle(pad1,logScaleX,logScaleY);
  pad1->SetBottomMargin(0.001);
  pad2->SetTopMargin(0.005);
  pad2->SetGridy(kTRUE);
  pad2->SetBottomMargin(pad2->GetBottomMargin()*3);
  pad1->Draw();
  pad2->Draw();
  //   c->cd(1);
  //   c->SetPad("pad1","",0,0.3,1,1,0,0,0);
  //   c->cd(2);
  //   c->SetPad("pad2","",0,0,1,0.3,0,0,0);
  
  //   c->cd(1);
  
  pad1->cd();
  
  std::vector<std::string> plotOptions;
  
  SetHistoStyle(histos[0], 0, markerStyle, colors[0]);
  plotOptions.push_back("e x0");
  std::string canvasName = cName;
  std::string stackName = "hs" + canvasName.substr(1,canvasName.size()-1);
  THStack* histoStack = new THStack(stackName.c_str(),"");
  TH1D* hSum = (TH1D*)histos[1]->Clone("hSum");
  hSum->SetMarkerStyle(0);
  hSum->SetFillStyle(0);
  
  for(unsigned int i = 1; i < histos.size(); i++)
  {
    SetStackHistoStyle(histos[i], colors[i]);
    histoStack->Add(histos[i]);
    plotOptions.push_back("fill");
    if(i > 1)
      hSum->Add(hSum,histos[i]);
  }
  
  double labelSizeFactor1 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad1->GetHNDC();
  double labelSizeFactor2 = (pad1->GetHNDC()+pad2->GetHNDC()) / pad2->GetHNDC();
  //   std::cout << "labelSizeFactor1 = " << labelSizeFactor1 << std::endl;
  //   std::cout << "labelSizeFactor2 = " << labelSizeFactor2 << std::endl;
  
  // Actually draw the histograms
  //   c->cd(1);
  pad1->cd();
  histos[0]->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor1, "XYZ");
  histos[0]->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor1, "XYZ");
  histos[0]->GetYaxis()->SetTitleOffset(histos[0]->GetYaxis()->GetTitleOffset()/labelSizeFactor1);
  histos[0]->Draw(plotOptions[0].c_str());
  double yMin = histos[0]->GetMinimum();
  double yMax = histos[0]->GetMaximum();
  // std::cout << "yMin = " << yMin << ", yMax = " << yMax << std::endl;
  double yMinNew = yMin + (yMax-yMin)/100000000;
  double yMaxNew = yMax + (yMax-yMin)*0.2;
  //   if(yMaxNew<yMax)
  //     yMaxNew = yMax;
  histos[0]->GetYaxis()->SetRangeUser(yMinNew,yMaxNew);
  //   histos[0]->GetYaxis()->SetBinLabel(1,"");
  pad1->cd();
  histoStack->Draw("hist same");
  hSum->SetLineWidth(3);
  hSum->Draw("hist same");
  histos[0]->Draw((plotOptions[0]+"same").c_str());
  
  pad2->cd();
  TH1::AddDirectory(false);
  TH1D* histoDiff = (TH1D*)(histos[0]->Clone());
  TH1::AddDirectory(true);
  //     histoDiff->Add(histos[0],histos[i],1,-1);
  histoDiff->SetMarkerColor(1);
  histoDiff->SetLineColor(1);
  
  for(int iBin = 1; iBin <= histos[0]->GetNbinsX(); iBin++)
  {
    // std::cout << "iBin = " << iBin << std::endl;
    double binContent1 = ((TH1D*)histos[0])->GetBinContent(iBin);
    double binContent2 = 0.;
    for(unsigned int iHisto = 1; iHisto < histos.size(); iHisto++)
    {
      binContent2 += ((TH1D*)histos[iHisto])->GetBinContent(iBin);
    }
    double binError1 = ((TH1D*)histos[0])->GetBinError(iBin);
    double binError2 = 0;
    for(unsigned int iHisto = 1; iHisto < histos.size(); iHisto++)
    {
      double iError = ((TH1D*)histos[iHisto])->GetBinError(iBin);
      binError2 += sqrt(binError2*binError2 + iError*iError);
    }
    
    double error = 0;
    double result = 0;
    if(binContent2 != 0 && binContent1 != 0)
    {
      result = (binContent1/binContent2);
      error = sqrt(binError1/binContent1*binError1/binContent1 + binError2/binContent2*binError2/binContent2)*result;
    }
    else
    {
      result = 9999;
      error = 1;
    }
    
// std::cout << "result = " << result << std::endl;
      histoDiff->SetBinContent(iBin, result);
      histoDiff->SetBinError(iBin,error);
  }
  //     histoDiff->Add(histos[0],histos[i],1,-1);
  
  
  //     histoDiff->Divide(histos[0],histos[i]);
  histoDiff->SetLabelSize(gStyle->GetLabelSize()*labelSizeFactor2, "XYZ");
  histoDiff->SetTitleSize(gStyle->GetTitleSize()*labelSizeFactor2, "XYZ");
  histoDiff->GetYaxis()->SetTitleOffset(histoDiff->GetYaxis()->GetTitleOffset()/labelSizeFactor2*labelSizeFactor1);
  histoDiff->SetTitle("");
  histoDiff->GetYaxis()->SetTitle("Ratio");
  histoDiff->SetStats(kFALSE);
  //     std::string yTitle = histos[0]->GetYaxis()->GetTitle();
  //     std::string yTitleDiff = "#Delta(" + yTitle + ") / " + yTitle;
  //     histoDiff->GetYaxis()->SetTitle(yTitleDiff.c_str());
  
  // TODO: take plot options
  histoDiff->Draw("e x0");
  histoDiff->GetYaxis()->SetRangeUser(0.501,1.499);
  histoDiff->GetYaxis()->SetNdivisions(505);
  
  //   TLine* lineAtOne = new TLine(histos[0]->GetXaxis()->GetXmin(),1,histos[0]->GetXaxis()->GetXmax(),1);
  //   lineAtOne->Draw();
  
  pad1->cd();
  
  PlotLegend(pad1, histos[0], histoStack, plotOptions, leg);
  // Write the canvas to file and exit
  pad1->RedrawAxis();
  pad2->RedrawAxis();
  pad1->Update();
  pad2->Update();
  c->Update();
  c->Write();
  
  delete c;
  
  return;
  
}



TCanvas* HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                            const unsigned int* markerStyles,
                                            const unsigned int* colors,
                                            const LegendDefinition leg,
                                            const bool logScaleX,
                                            const bool logScaleY,
                                            ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TGraph*> graphs;
  for(unsigned int i = 0; /* */; i++)
  {
    graphs.push_back(va_arg(arguments,TGraph*));
    // A null pointer must be the last argument in the list
    if(*(graphs.end()-1) == NULL)
    {
      graphs.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }

  // If there are no arguments, complain and return
  if(graphs.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    va_end(arguments);
    return 0;
  }

  TCanvas* c = CreateAndWriteCanvas(cName, leg, markerStyles, colors, logScaleX, logScaleY, graphs);

  va_end(arguments);
  return c;
}



void HistoPlotter::CreateAndWriteCanvasWith2DDifference(const char* cName,
                                                        const char* cTitle,
                                                        const std::string plotOptions,
                                                        const bool logScaleX,
                                                        const bool logScaleY,
                                                        const bool logScaleZ,
                                                        TH2* histo1,
                                                        TH2* histo2)
{
  if(!SameBins(histo1, histo2))
  {
    std::cout << "E R R O R ! HistoPlotter::CreateAndWriteCanvasWith2DDifference: the two histograms do not have the same binning!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  c->SetRightMargin(c->GetRightMargin()*4.5);
  
  
  TH2D* hDifference = Create2DHistogram<TH2D>("hDifference", cTitle, histo1->GetXaxis()->GetNbins(), histo1->GetXaxis()->GetXmin(), histo1->GetXaxis()->GetXmax(), histo1->GetYaxis()->GetNbins(), histo1->GetYaxis()->GetXmin(), histo1->GetYaxis()->GetXmax(), histo1->GetXaxis()->GetTitle(),  histo1->GetYaxis()->GetTitle());
  hDifference->Add(histo1, histo2, 1., -1.);
  
  // Actually draw the histogram
  c->cd();
  hDifference->SetStats(kFALSE);
  hDifference->Draw(plotOptions.c_str());
  
  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvasWith2DRelDifference(const char* cName,
                                                           const char* cTitle,
                                                           const std::string plotOptions,
                                                           const bool logScaleX,
                                                           const bool logScaleY,
                                                           const bool logScaleZ,
                                                           TH2* histo1,
                                                           TH2* histo2)
{
  if(!SameBins(histo1, histo2))
  {
    std::cout << "E R R O R ! HistoPlotter::CreateAndWriteCanvasWith2DRelDifference: the two histograms do not have the same binning!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  c->SetRightMargin(c->GetRightMargin()*4.5);
  
  
  TH2D* hRelDifference = Create2DHistogram<TH2D>("hRelDifference", cTitle, histo1->GetXaxis()->GetNbins(), histo1->GetXaxis()->GetXmin(), histo1->GetXaxis()->GetXmax(), histo1->GetYaxis()->GetNbins(), histo1->GetYaxis()->GetXmin(), histo1->GetYaxis()->GetXmax(), histo1->GetXaxis()->GetTitle(),  histo1->GetYaxis()->GetTitle());
  
  for(int xBin = 1; xBin <= histo1->GetXaxis()->GetNbins(); xBin++)
  {
    for(int yBin = 1; yBin <= histo1->GetYaxis()->GetNbins(); yBin++)
    {
      double binContent = (histo1->GetBinContent(xBin, yBin) - histo2->GetBinContent(xBin, yBin)) / histo1->GetBinContent(xBin, yBin);
      hRelDifference->SetBinContent(xBin, yBin, binContent);
    }
  }
  
  // Actually draw the histogram
  c->cd();
  hRelDifference->SetStats(kFALSE);
  hRelDifference->Draw(plotOptions.c_str());
  
  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvasWith2DRatio(const char* cName,
                                                   const char* cTitle,
                                                   const std::string plotOptions,
                                                   const bool logScaleX,
                                                   const bool logScaleY,
                                                   const bool logScaleZ,
                                                   TH2* hNumerator,
                                                   TH2* hDenominator)
{
  if(!SameBins(hNumerator, hDenominator))
  {
    std::cout << "E R R O R ! HistoPlotter::CreateAndWriteCanvasWith2DRatio: numerator and denominator histograms do not have the same binning!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  c->SetRightMargin(c->GetRightMargin()*4.5);
  
  
  TH2D* hRatio = Create2DHistogram<TH2D>("hRatio", cTitle, hNumerator->GetXaxis()->GetNbins(), hNumerator->GetXaxis()->GetXmin(), hNumerator->GetXaxis()->GetXmax(), hNumerator->GetYaxis()->GetNbins(), hNumerator->GetYaxis()->GetXmin(), hNumerator->GetYaxis()->GetXmax(), hNumerator->GetXaxis()->GetTitle(),  hNumerator->GetYaxis()->GetTitle());
  
  for(int xBin = 1; xBin <= hNumerator->GetXaxis()->GetNbins(); xBin++)
  {
    for(int yBin = 1; yBin <= hNumerator->GetYaxis()->GetNbins(); yBin++)
    {
      double binContent = hNumerator->GetBinContent(xBin, yBin) / hDenominator->GetBinContent(xBin, yBin);
      hRatio->SetBinContent(xBin, yBin, binContent);
    }
  }
  
  // Actually draw the histogram
  c->cd();
  hRatio->SetStats(kFALSE);
  hRatio->Draw(plotOptions.c_str());
  
  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvasWith2DPull(const char* cName,
                                                  const char* cTitle,
                                                  const std::string plotOptions,
                                                  const bool logScaleX,
                                                  const bool logScaleY,
                                                  const bool logScaleZ,
                                                  TH2* histo1,
                                                  TH2* histo2)
{
  if(!SameBins(histo1, histo2))
  {
    std::cout << "E R R O R ! HistoPlotter::CreateAndWriteCanvasWith2DPull: the two histograms do not have the same binning!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  c->SetRightMargin(c->GetRightMargin()*4.5);
  
  
  TH2D* hPull = Create2DHistogram<TH2D>("hPull", cTitle, histo1->GetXaxis()->GetNbins(), histo1->GetXaxis()->GetXmin(), histo1->GetXaxis()->GetXmax(), histo1->GetYaxis()->GetNbins(), histo1->GetYaxis()->GetXmin(), histo1->GetYaxis()->GetXmax(), histo1->GetXaxis()->GetTitle(),  histo1->GetYaxis()->GetTitle());
  
  for(int xBin = 1; xBin <= histo1->GetXaxis()->GetNbins(); xBin++)
  {
    for(int yBin = 1; yBin <= histo1->GetYaxis()->GetNbins(); yBin++)
    {
      double error = sqrt(histo1->GetBinError(xBin, yBin)*histo1->GetBinError(xBin, yBin) + histo2->GetBinError(xBin, yBin)*histo2->GetBinError(xBin, yBin));
      double binContent = (histo1->GetBinContent(xBin, yBin) - histo2->GetBinContent(xBin, yBin)) / error;
      hPull->SetBinContent(xBin, yBin, binContent);
    }
  }
  
  // Actually draw the histogram
  c->cd();
  hPull->SetStats(kFALSE);
  hPull->Draw(plotOptions.c_str());
  
  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



TCanvas* HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                            const unsigned int* markerStyles,
                                            const unsigned int* colors,
                                            const bool logScaleX,
                                            const bool logScaleY,
                                            ...)
{
  // Read arguments in the ellipsis ("...") and write them in the vector
  va_list arguments;
  va_start(arguments, logScaleY);
  std::vector<TGraph*> graphs;
  for(unsigned int i = 0; /* */; i++)
  {
    graphs.push_back(va_arg(arguments,TGraph*));
    // A null pointer must be the last argument in the list
    if(*(graphs.end()-1) == NULL)
    {
      graphs.pop_back();
      break;
    }
    // Protection against too many arguments
    if(i==20)
    {
      std::cout << "WARNING: More than 20 arguments. Skipping all the others!!!\nCheck that you put 'NULL' as the last argument of the function!!!\n";
      break;
    }
  }

  // If there are no arguments, complain and return
  if(graphs.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    va_end(arguments);
    return 0;
  }

  TCanvas* c = CreateAndWriteCanvas(cName, markerStyles, colors, logScaleX, logScaleY, graphs);

  va_end(arguments);
  return c;
}



TCanvas* HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                            const LegendDefinition leg,
                                            const unsigned int* markerStyles,
                                            const unsigned int* colors,
                                            const bool logScaleX,
                                            const bool logScaleY,
                                            std::vector<TGraph*> graphs)
{
  // If there are no arguments, complain and return
  if(graphs.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return 0;
  }

  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY);
  c->SetRightMargin(c->GetRightMargin()*3.5);

  std::vector<std::string> plotOptions;

  for(unsigned int i = 0; i < graphs.size(); i++)
  {
    SetGraphStyle(graphs[i], markerStyles[i], colors[i]);
    //     if(markerStyles[i]==0)
    //       plotOptions.push_back("");
    //     else
    if(i==0)
      plotOptions.push_back("A P");
    else
      plotOptions.push_back("P");
  }

  // Actually draw the graphs
  c->cd();
  bool isFirst = true;
  if(graphs[0]->GetN() != 0)
  {
    graphs[0]->Draw(plotOptions[0].c_str());
    isFirst = false;
  }
  else
  {
    std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvas(...) : graph #0 has zero points, it will not be drawn!\n";
  }
  for(unsigned int i = 1; i < graphs.size(); i++)
  {
    if(graphs[i]->GetN() != 0)
    {
      if(isFirst)
      {
        graphs[i]->Draw(plotOptions[0].c_str());
        isFirst = false;
      }
      else
      {
        if(plotOptions[i].find("P") != plotOptions[i].npos)
          graphs[i]->Draw((plotOptions[i]+" same").c_str());
        else
          graphs[i]->Draw("same");
      }
    }
    else
    {
      std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvas(...) : graph #" << i << " has zero points, it will not be drawn!\n";
    }
  }

  PlotLegend(c, graphs, plotOptions, leg);

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  return c;
}



TCanvas* HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                            const unsigned int* markerStyles,
                                            const unsigned int* colors,
                                            const bool logScaleX,
                                            const bool logScaleY,
                                            std::vector<TGraph*> graphs)
{
  // If there are no arguments, complain and return
  if(graphs.size()==0)
  {
    std::cout << "ERROR!!! No arguments passed!!!\nReturning.\n";
    return 0;
  }

  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY);
  c->SetRightMargin(c->GetRightMargin()*3.5);

  std::vector<std::string> plotOptions;

  for(unsigned int i = 0; i < graphs.size(); i++)
  {
    SetGraphStyle(graphs[i], markerStyles[i], colors[i]);
    //     if(markerStyles[i]==0)
    //       plotOptions.push_back("");
    //     else
    if(i==0)
      plotOptions.push_back("A P");
    else
      plotOptions.push_back("P");
  }

//   for(unsigned int i = 0; i < plotOptions.size(); i++)
//     std::cout << "plotOptions[" << i << "] = " << plotOptions[i] << std::endl;
  
  // Actually draw the graphs
  c->cd();
  bool isFirst = true;
  if(graphs[0]->GetN() != 0)
  {
    graphs[0]->Draw(plotOptions[0].c_str());
    isFirst = false;
  }
  else
  {
    std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvas(...) : graph #0 has zero points, it will not be drawn!\n";
  }
  for(unsigned int i = 1; i < graphs.size(); i++)
  {
    if(graphs[i]->GetN() != 0)
    {
      if(isFirst)
      {
        graphs[i]->Draw(plotOptions[0].c_str());
        isFirst = false;
      }
      else
      {
        if(plotOptions[i].find("P") != plotOptions[i].npos)
          graphs[i]->Draw((plotOptions[i]+" same").c_str());
        else
          graphs[i]->Draw("same");
      }
    }
    else
    {
      std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvas(...) : graph #" << i << " has zero points, it will not be drawn!\n";
    }
  }
  
  // Write the canvas to file and exit
  c->Update();
  c->Write();
  return c;
}



TCanvas* HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                            const unsigned int markerStyle,
                                            const unsigned int color,
                                            const bool logScaleX,
                                            const bool logScaleY,
                                            TGraph* graph)
{
  // If there are no arguments, complain and return
  if(graph==0)
  {
    std::cout << "ERROR!!! Null pointer passed!!!\nReturning.\n";
    return 0;
  }

  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY);
  c->SetRightMargin(c->GetRightMargin()*3.5);

  SetGraphStyle(graph, markerStyle, color);
  std::string plotOptions("A P");

  // Actually draw the graph
  c->cd();
  if(graph->GetN() != 0)
  {
    graph->Draw(plotOptions.c_str());
  }
  else
  {
    std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvas(...) : graph has zero points, it will not be drawn!\n";
  }
//   graph->GetXaxis()->SetTitle(graph->GetHistogram()->GetXaxis()->GetTitle());
//   graph->GetYaxis()->SetTitle(graph->GetHistogram()->GetYaxis()->GetTitle());
  
  // Write the canvas to file and exit
  c->Update();
  c->Write();
  return c;
}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const std::string plotOptions,
                                        const LegendDefinition leg,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        const bool logScaleZ,
                                        TH2* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  c->SetRightMargin(c->GetRightMargin()*4.5);

  // Actually draw the histograms
  c->cd();
  histo->SetStats(kFALSE);
  histo->Draw(plotOptions.c_str());

  PlotLegend(c, histo, plotOptions, leg);

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const std::string plotOptions,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        const bool logScaleZ,
                                        TH2* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  c->SetRightMargin(c->GetRightMargin()*4.5);

  // Actually draw the histograms
  c->cd();
  histo->SetStats(kFALSE);
  histo->Draw(plotOptions.c_str());

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const char* cTitle,
                                        const std::string plotOptions,
                                        const LegendDefinition leg,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        const bool logScaleZ,
                                        TH2* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  c->SetRightMargin(c->GetRightMargin()*4.5);

  // Actually draw the histograms
  c->cd();
  histo->SetTitle(cTitle);
  histo->SetStats(kFALSE);
  histo->Draw(plotOptions.c_str());

  PlotLegend(c, histo, plotOptions, leg);

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvas(const char* cName,
                                        const char* cTitle,
                                        const std::string plotOptions,
                                        const bool logScaleX,
                                        const bool logScaleY,
                                        const bool logScaleZ,
                                        TH2* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  c->SetRightMargin(c->GetRightMargin()*4.5);

  // Actually draw the histograms
  c->cd();
  histo->SetTitle(cTitle);
  histo->SetStats(kFALSE);
  histo->Draw(plotOptions.c_str());

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvas3D(const char* cName,
                                          const LegendDefinition leg,
                                          const bool logScaleX,
                                          const bool logScaleY,
                                          const bool logScaleZ,
                                          const double phi,
                                          const double theta,
                                          TH2* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  if(histo->GetMinimum() != histo->GetMaximum())
  {
    SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  }
  else
  {
    SetCanvasStyle(c, false, false, false);
  }
  c->SetRightMargin(c->GetRightMargin()*6.);

  // Actually draw the histograms
  c->cd();

  histo->SetStats(kFALSE);
  histo->Draw("lego2 z");
  c->SetPhi(phi);
  c->SetTheta(theta);

  PlotLegend(c, histo, "lego2 z", leg);

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvas3D(const char* cName,
                                          const bool logScaleX,
                                          const bool logScaleY,
                                          const bool logScaleZ,
                                          const double phi,
                                          const double theta,
                                          TH2* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  bool histoIsValid = (histo->GetMinimum() < histo->GetMaximum());
  if(histoIsValid)
  {
    SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  }
  else
  {
    std::cout << "\nW A R N I N G ! HistoPlotter::CreateAndWriteCanvas3D(...) : Histo has invalid minimum-maximum!\n";
    std::cout << "                It will not be drawn.\n";
    SetCanvasStyle(c, false, false, false);
  }
  c->SetRightMargin(c->GetRightMargin()*6.);

  // Actually draw the histograms
  c->cd();
  histo->SetStats(kFALSE);
  if(histoIsValid)
    histo->Draw("lego2 z");
  c->SetPhi(phi);
  c->SetTheta(theta);

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvas3D(const char* cName,
                                          const char* cTitle,
                                          const LegendDefinition leg,
                                          const bool logScaleX,
                                          const bool logScaleY,
                                          const bool logScaleZ,
                                          const double phi,
                                          const double theta,
                                          TH2* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  if(histo->GetMinimum() != histo->GetMaximum())
  {
    SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  }
  else
  {
    SetCanvasStyle(c, false, false, false);
  }
  c->SetRightMargin(c->GetRightMargin()*6.);

  // Actually draw the histograms
  c->cd();
  histo->SetTitle(cTitle);
  histo->SetStats(kFALSE);
  histo->Draw("lego2 z");
  c->SetPhi(phi);
  c->SetTheta(theta);

  PlotLegend(c, histo, "lego2 z", leg);

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::CreateAndWriteCanvas3D(const char* cName,
                                          const char* cTitle,
                                          const bool logScaleX,
                                          const bool logScaleY,
                                          const bool logScaleZ,
                                          const double phi,
                                          const double theta,
                                          TH2* histo)
{
  // Create canvas, and assign the right style to it
  TCanvas *c = new TCanvas(cName);
  if(histo->GetMinimum() != histo->GetMaximum())
  {
    SetCanvasStyle(c, logScaleX, logScaleY, logScaleZ);
  }
  else
  {
    SetCanvasStyle(c, false, false, false);
  }
  c->SetRightMargin(c->GetRightMargin()*6.);

  // Actually draw the histograms
  c->cd();
  histo->SetTitle(cTitle);
  histo->SetStats(kFALSE);
  histo->Draw("lego2 z");
  c->SetPhi(phi);
  c->SetTheta(theta);

  // Write the canvas to file and exit
  c->Update();
  c->Write();
  delete c;
  return;
}



void HistoPlotter::PlotLegend(TPad* c, const std::vector<TGraph*> graphs, std::vector<std::string> plotOptions, const LegendDefinition leg)
{
  std::vector<TObject*> objects;
  for(unsigned int i = 0; i < graphs.size(); i++)
  {
    objects.push_back(graphs[i]);
  }
  
  PlotLegend(c, objects, plotOptions, leg);
}



void HistoPlotter::PlotLegend(TPad* c, const std::vector<TH1*> histos, std::vector<std::string> plotOptions, const LegendDefinition leg)
{
  std::vector<TObject*> objects;
  for(unsigned int i = 0; i < histos.size(); i++)
  {
    objects.push_back(histos[i]);
  }
  
  PlotLegend(c, objects, plotOptions, leg);
}



void HistoPlotter::PlotLegend(TPad* c, TH1* histo, THStack* stack, std::vector<std::string> plotOptions, const LegendDefinition leg)
{
  std::vector<TObject*> objects;
  objects.push_back(histo);
  TList* histoList = stack->GetHists();
  for(int iHisto = histoList->GetSize() - 1; iHisto > -1; iHisto--)
  {
    objects.push_back(histoList->At(iHisto));
  }
//   objects.push_back(stack);
  std::vector<std::string> legendLabelsOrdered;
  legendLabelsOrdered.push_back(leg.labels[0]);
  for(unsigned int iLabel = leg.labels.size() - 1; iLabel > 0; iLabel--)
    legendLabelsOrdered.push_back(leg.labels[iLabel]);
  LegendDefinition legOrdered = {leg.title, legendLabelsOrdered, leg.position};
  PlotLegend(c, objects, plotOptions, legOrdered);
}



void HistoPlotter::PlotLegend(TPad* c, const std::vector<TObject*> histos, std::vector<std::string> plotOptions, const LegendDefinition leg)
{
  // Create the legend
  // Set the legend position according to LegendDefinition::position variable
  double legX1, legX2, legY1, legY2;
  std::string legPosition = leg.position;
  double padLeftMargin = c->GetLeftMargin();
  double padRightMargin = c->GetRightMargin();
  double padTopMargin = c->GetTopMargin();
  double padBottomMargin = c->GetBottomMargin();
  double plotWidth = 1. - (padLeftMargin + padRightMargin);
  double plotHeight = 1. - (padTopMargin + padBottomMargin);
  int nEntries = histos.size();
  unsigned int maxTextSize = 0;
  if(leg.title.size() > maxTextSize)
    maxTextSize = leg.title.size();
  if(maxTextSize > 40)
    maxTextSize = 40;
  for(unsigned int iEntry = 0; iEntry < leg.labels.size(); iEntry++)
  {
    if(leg.labels[iEntry].size() > maxTextSize)
      maxTextSize = leg.labels[iEntry].size();
  }
  if(leg.title.length() > 0)
    nEntries++;
  if(legPosition.find("e")!= legPosition.npos || legPosition.find("E")!= legPosition.npos)
  {
    legX1 = (0.95-maxTextSize*0.012-0.1)*plotWidth + padLeftMargin;
    legX2 = 0.95*plotWidth + padLeftMargin;
  }
  else if(legPosition.find("w")!= legPosition.npos || legPosition.find("W")!= legPosition.npos)
  {
    legX1 = 0.05*plotWidth + padLeftMargin;
    legX2 = (0.05+maxTextSize*0.012+0.1)*plotWidth + padLeftMargin;
  }
  else
  {
    legX1 = 0.325*plotWidth + padLeftMargin;
    legX2 = 0.675*plotWidth + padLeftMargin;
  }

  if(legPosition.find("n")!= legPosition.npos || legPosition.find("N")!= legPosition.npos)
  {
    double y1 = 0.95 - (0.1*nEntries);
    if(y1 < 0.5)
      y1 = 0.5;
    legY1 = y1*plotHeight + padBottomMargin;
    legY2 = 0.95*plotHeight + padBottomMargin;
  }
  else if(legPosition.find("s")!= legPosition.npos || legPosition.find("S")!= legPosition.npos)
  {
    double y2 = 0.05 + (0.1*nEntries);
    if(y2 > 0.5)
      y2 = 0.5;
    legY1 = 0.05*plotHeight + padBottomMargin;
    legY2 = y2*plotHeight + padBottomMargin;
  }
  else
  {
    double y1 = 0.5 - (0.05*nEntries);
    if(y1 < 0.25)
      y1 = 0.25;
    double y2 = 0.5 + (0.05*nEntries);
    if(y2 > 0.75)
      y2 = 0.75;
    legY1 = y1*plotHeight + padBottomMargin;
    legY2 = y2*plotHeight + padBottomMargin;
  }

  // Set legend options
  std::vector<std::string> legendOptions;
  for(unsigned int i = 0; i < plotOptions.size(); i++)
  {
    if(plotOptions[i].find("e") != plotOptions[i].npos || plotOptions[i].find("P") != plotOptions[i].npos)
      legendOptions.push_back("lp");
    else if(plotOptions[i].find("fill") != plotOptions[i].npos)
      legendOptions.push_back("f");
    else
      legendOptions.push_back("l");
  }

  TLegend * legend = new TLegend(legX1, legY1, legX2, legY2, leg.title.c_str(),"NDC");

  // Set generic options
  legend->UseCurrentStyle();
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.045);

  for(unsigned int i = 0; i < histos.size(); i++)
  {
    legend->AddEntry(histos[i],leg.labels[i].c_str(),legendOptions[i].c_str());
  }

  c->cd();
  legend->Draw();
}



void HistoPlotter::PlotLegend(TPad* c, const TH2* histo, std::string plotOptions, const LegendDefinition leg)
{
  // Create the legend
  // Set the legend position according to LegendDefinition::position variable
  double legX1, legX2, legY1, legY2;
  std::string legPosition = leg.position;
  double padLeftMargin = c->GetLeftMargin();
  double padRightMargin = c->GetRightMargin();
  double padTopMargin = c->GetTopMargin();
  double padBottomMargin = c->GetBottomMargin();
  double plotWidth = 1. - (padLeftMargin + padRightMargin);
  double plotHeight = 1. - (padTopMargin + padBottomMargin);
  if(legPosition.find("e")!= legPosition.npos || legPosition.find("E")!= legPosition.npos)
  {
    legX1 = 0.6*plotWidth + padLeftMargin;
    legX2 = 0.95*plotWidth + padLeftMargin;
  }
  else if(legPosition.find("w")!= legPosition.npos || legPosition.find("W")!= legPosition.npos)
  {
    legX1 = 0.05*plotWidth + padLeftMargin;
    legX2 = 0.4*plotWidth + padLeftMargin;
  }
  else
  {
    legX1 = 0.325*plotWidth + padLeftMargin;
    legX2 = 0.675*plotWidth + padLeftMargin;
  }

  if(legPosition.find("n")!= legPosition.npos || legPosition.find("N")!= legPosition.npos)
  {
    legY1 = 0.6*plotHeight + padBottomMargin;
    legY2 = 0.95*plotHeight + padBottomMargin;
  }
  else if(legPosition.find("s")!= legPosition.npos || legPosition.find("S")!= legPosition.npos)
  {
    legY1 = 0.05*plotHeight + padBottomMargin;
    legY2 = 0.4*plotHeight + padBottomMargin;
  }
  else
  {
    legY1 = 0.325*plotHeight + padBottomMargin;
    legY2 = 0.675*plotHeight + padBottomMargin;
  }

  // Set legend options
  std::string legendOptions;
  if(plotOptions.find("e1") != plotOptions.npos || plotOptions.find("P") != plotOptions.npos)
    legendOptions = "lp";
  else
    legendOptions = "l";

  TLegend * legend = new TLegend(legX1, legY1, legX2, legY2, leg.title.c_str(),"NDC");

  // Set generic options
  legend->UseCurrentStyle();
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->SetFillStyle(0);
  legend->SetTextFont(42);
  legend->SetTextSize(0.045);

  legend->AddEntry(histo,leg.labels[0].c_str(),legendOptions.c_str());

  c->cd();
  legend->Draw();
}



#endif // HistoPlotter_cxx
