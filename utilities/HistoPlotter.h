#ifndef HistoPlotter_h
#define HistoPlotter_h

#include <stdarg.h>

#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "THStack.h"

#include "HistoPlotter.C"



class HistoPlotter
{
  public:

    HistoPlotter() {SetStyle();};

    struct LegendDefinition
    {
      std::string title;
      std::vector<std::string> labels;
      std::string position;
    };

//   private:

    void SetStyle();

    template <typename T> T* Create1DHistogram(const char* hName, const char* hTitle, const unsigned int xNBins, const double xMin, const double xMax, const char* xTitle, const char* yTitle);
    template <typename T> T* Create1DHistogram(const char* hName, const char* hTitle, const unsigned int xNBins, const double* xBins, const char* xTitle, const char* yTitle);
    
//     template <typename T> std::vector<T*> CreateVectorOf1DHistograms(const char* hName, const char* hTitle, const unsigned int xNBins, const double xMin, const double xMax, const char* xTitle, const char* yTitle, const std::string binningVariable, const std::vector<double> vLowBins);
    template <typename T> std::vector<T*>* CreateVectorOf1DHistograms(const char* hName, const char* hTitle, const unsigned int xNBins, const double xMin, const double xMax, const char* xTitle, const char* yTitle, const std::string binningVariable, const std::vector<double> vLowBins)
    {
      std::vector<T*>* vResults = new std::vector<T*>();
      // std::cout << "vLowBins.size() = " << vLowBins.size() << std::endl;
      if(vLowBins.size() == 0)
      {
        T* result = Create1DHistogram<TH1D>(hName, hTitle, xNBins, xMin, xMax, xTitle, yTitle);
        vResults->push_back(result);
        return vResults;
      }
      
      std::vector<std::string> vTitleLabels;
      std::vector<std::string> vNameLabels;
      std::string lowest = "-Inf";
      std::string highest = "Inf";
      //   std::string lowestName = "mInf";
      //   std::string highestName = "Inf";
      
      std::ostringstream oss;
      oss.str("");
      oss << " (" << lowest << " <= " << binningVariable << " < " << vLowBins[0] << ")";
      vTitleLabels.push_back(oss.str());
      oss.str("");
      oss << "_" << binningVariable << "_" << lowest << "_" << vLowBins[0];
      vNameLabels.push_back(oss.str());
      for(unsigned int i = 0; i < vLowBins.size() - 1; i++)
      {
        oss.str("");
        oss << " (" << vLowBins[i] << " <= " << binningVariable << " < " << vLowBins[i+1] << ")";
        vTitleLabels.push_back(oss.str());
        oss.str("");
        oss << "_" << binningVariable << "_" << vLowBins[i] << "_" << vLowBins[i+1];
        vNameLabels.push_back(oss.str());
      }
      oss.str("");
      oss << " (" << vLowBins[vLowBins.size()-1] << " <= " << binningVariable << " < " << highest << ")";
      vTitleLabels.push_back(oss.str());
      oss.str("");
      oss << "_" << binningVariable << "_" << vLowBins[vLowBins.size()-1] << "_" << highest;
      vNameLabels.push_back(oss.str());
      
      
      for(unsigned int iBin = 0; iBin < vTitleLabels.size(); iBin++)
      {
        std::string hNewName = hName + vNameLabels[iBin];
        std::string hNewTitle = hTitle + vTitleLabels[iBin];
        T* result = Create1DHistogram<TH1D>(hNewName.c_str(), hNewTitle.c_str(), xNBins, xMin, xMax, xTitle, yTitle);
        vResults->push_back(result);
      }
      return vResults;
    };
    
    template <typename T> std::vector<T*>* CreateVectorOf2DHistograms(const char* hName, const char* hTitle, const unsigned int xNBins, const double xMin, const double xMax, const unsigned int yNBins, const double yMin, const double yMax, const char* xTitle, const char* yTitle, const std::string binningVariable, const std::vector<double> vLowBins)
    {
      std::vector<T*>* vResults = new std::vector<T*>();
      if(vLowBins.size() == 0)
      {
        T* result = Create2DHistogram<TH2D>(hName, hTitle, xNBins, xMin, xMax, yNBins, yMin, yMax, xTitle, yTitle);
        vResults->push_back(result);
        return vResults;
      }
      
      std::vector<std::string> vTitleLabels;
      std::vector<std::string> vNameLabels;
      std::string lowest = "-Inf";
      std::string highest = "Inf";
      //   std::string lowestName = "mInf";
      //   std::string highestName = "Inf";
      
      std::ostringstream oss;
      oss.str("");
      oss << " (" << lowest << " <= " << binningVariable << " < " << 
      vLowBins[0] << ")";
      vTitleLabels.push_back(oss.str());
      oss.str("");
      oss << "_" << binningVariable << "_" << lowest << "_" << vLowBins[0];
      vNameLabels.push_back(oss.str());
      for(unsigned int i = 0; i < vLowBins.size() - 1; i++)
      {
        oss.str("");
        oss << " (" << vLowBins[i] << " <= " << binningVariable << " < " << 
        vLowBins[i+1] << ")";
        vTitleLabels.push_back(oss.str());
        oss.str("");
        oss << "_" << binningVariable << "_" << vLowBins[i] << "_" << 
        vLowBins[i+1];
        vNameLabels.push_back(oss.str());
      }
      oss.str("");
      oss << " (" << vLowBins[vLowBins.size()-1] << " <= " << 
      binningVariable << " < " << highest << ")";
      vTitleLabels.push_back(oss.str());
      oss.str("");
      oss << "_" << binningVariable << "_" << vLowBins[vLowBins.size()-1] 
      << "_" << highest;
      vNameLabels.push_back(oss.str());
      
      for(unsigned int iBin = 0; iBin < vTitleLabels.size(); iBin++)
      {
        std::string hNewName = hName + vNameLabels[iBin];
        std::string hNewTitle = hTitle + vTitleLabels[iBin];
        T* result = Create2DHistogram<TH2D>(hNewName.c_str(), hNewTitle.c_str(), xNBins, xMin, xMax, yNBins, yMin, yMax, xTitle, yTitle);
        vResults->push_back(result);
      }
      return vResults;
    };

    // for variable-binning x axis of histogram
    template <typename T> std::vector<T*>* CreateVectorOf1DHistograms(const char* hName, const char* hTitle, const unsigned int xNBins, const double* xBins, const char* xTitle, const char* yTitle, const std::string binningVariable, const std::vector<double> vLowBins)
    {
      std::vector<T*>* vResults = new std::vector<T*>();
      // std::cout << "vLowBins.size() = " << vLowBins.size() << std::endl;
      if(vLowBins.size() == 0)
      {
        T* result = Create1DHistogram<TH1D>(hName, hTitle, xNBins, xBins, xTitle, yTitle);
        vResults->push_back(result);
        return vResults;
      }
      
      std::vector<std::string> vTitleLabels;
      std::vector<std::string> vNameLabels;
      std::string lowest = "-Inf";
      std::string highest = "Inf";
      //   std::string lowestName = "mInf";
      //   std::string highestName = "Inf";
      
      std::ostringstream oss;
      oss.str("");
      oss << " (" << lowest << " <= " << binningVariable << " < " << vLowBins[0] << ")";
      vTitleLabels.push_back(oss.str());
      oss.str("");
      oss << "_" << binningVariable << "_" << lowest << "_" << vLowBins[0];
      vNameLabels.push_back(oss.str());
      for(unsigned int i = 0; i < vLowBins.size() - 1; i++)
      {
        oss.str("");
        oss << " (" << vLowBins[i] << " <= " << binningVariable << " < " << vLowBins[i+1] << ")";
        vTitleLabels.push_back(oss.str());
        oss.str("");
        oss << "_" << binningVariable << "_" << vLowBins[i] << "_" << vLowBins[i+1];
        vNameLabels.push_back(oss.str());
      }
      oss.str("");
      oss << " (" << vLowBins[vLowBins.size()-1] << " <= " << binningVariable << " < " << highest << ")";
      vTitleLabels.push_back(oss.str());
      oss.str("");
      oss << "_" << binningVariable << "_" << vLowBins[vLowBins.size()-1] << "_" << highest;
      vNameLabels.push_back(oss.str());
      
      
      for(unsigned int iBin = 0; iBin < vTitleLabels.size(); iBin++)
      {
        std::string hNewName = hName + vNameLabels[iBin];
        std::string hNewTitle = hTitle + vTitleLabels[iBin];
        T* result = Create1DHistogram<TH1D>(hNewName.c_str(), hNewTitle.c_str(), xNBins, xBins, xTitle, yTitle);
        vResults->push_back(result);
      }
      return vResults;
    };
    
    
    template <typename T> T* Create2DHistogram(const char* hName, const char* hTitle, const unsigned int xNBins, const double xMin, const double xMax, const unsigned int yNBins, const double yMin, const double yMax, const char* xTitle, const char* yTitle);
    
    template <typename T> T* Create2DHistogram(const char* hName, const char* hTitle, const unsigned int xNBins, const double xMin, const double xMax, const unsigned int yNBins, const double yMin, const double yMax, const char* xTitle, const char* yTitle, const char* zTitle);
    // For variable-size bins
    template <typename T> T* Create2DHistogram(const char* hName, const char* hTitle, const unsigned int xNBins, const std::vector<double> xBins, const unsigned int yNBins, const std::vector<double> yBins, const char* xTitle, const char* yTitle);
    
    template <typename T> T* CreateGraph(const char* gName, const char* gTitle, const char* xTitle, const char* yTitle, const unsigned int nPoints = 0);
    
    template <typename T> std::vector<T*>* CreateVectorOfGraphs(const char* gName, const char* gTitle, const char* xTitle, const char* yTitle, const std::string binningVariable, const std::vector<double> vLowBins, const unsigned int nPoints = 0)
    {
      std::vector<T*>* vResults = new std::vector<T*>();
      // std::cout << "vLowBins.size() = " << vLowBins.size() << std::endl;
      if(vLowBins.size() == 0)
      {
        T* result = CreateGraph<T>(gName, gTitle, xTitle, yTitle, nPoints);
        vResults->push_back(result);
        return vResults;
      }
      std::vector<std::string> vTitleLabels;
      std::vector<std::string> vNameLabels;
      std::string lowest = "-Inf";
      std::string highest = "Inf";
      //   std::string lowestName = "mInf";
      //   std::string highestName = "Inf";
      
      std::ostringstream oss;
      oss.str("");
      oss << " (" << lowest << " <= " << binningVariable << " < " << vLowBins[0] << ")";
      vTitleLabels.push_back(oss.str());
      oss.str("");
      oss << "_" << binningVariable << "_" << lowest << "_" << vLowBins[0];
      vNameLabels.push_back(oss.str());
      for(unsigned int i = 0; i < vLowBins.size() - 1; i++)
      {
        oss.str("");
        oss << " (" << vLowBins[i] << " <= " << binningVariable << " < " << vLowBins[i+1] << ")";
        vTitleLabels.push_back(oss.str());
        oss.str("");
        oss << "_" << binningVariable << "_" << vLowBins[i] << "_" << vLowBins[i+1];
        vNameLabels.push_back(oss.str());
      }
      oss.str("");
      oss << " (" << vLowBins[vLowBins.size()-1] << " <= " << binningVariable << " < " << highest << ")";
      vTitleLabels.push_back(oss.str());
      oss.str("");
      oss << "_" << binningVariable << "_" << vLowBins[vLowBins.size()-1] << "_" << highest;
      vNameLabels.push_back(oss.str());
      
      
      for(unsigned int iBin = 0; iBin < vTitleLabels.size(); iBin++)
      {
        std::string gNewName = gName + vNameLabels[iBin];
        std::string gNewTitle = gTitle + vTitleLabels[iBin];
        T* result = CreateGraph<T>(gNewName.c_str(), gNewTitle.c_str(), xTitle, yTitle, nPoints);
        vResults->push_back(result);
      }
      return vResults;
    };
    

    // Finds the bin division to be used to compare histograms with different binning
    // Just for 2D histograms for the moment
    // I.e. histo 1 has bin edges [0,1,2,3,5,7] and histo2 has bin edges [1,2,4,6,8]
    // Resulting binning will be [1,2,3,4,5,6]
    std::pair<std::vector<double>,std::vector<double> > FindDifferenceBinDivision(const TH2* hFirst, const TH2* hSecond);
    
    // Checks that the two histograms have same bins along x and y
    bool SameBins(const TH1* hFirst, const TH1* hSecond);
    
    // Only for 2D histograms and only around the X axis for the time being
    TH2D* SymmetrizeHistogramInX(const TH2D* histo, bool aroundLowEdge = true);
    
    // To overcome stupid bug in ROOT
    void BayesDivide(const TH1* pass, const TH1* total, TGraphAsymmErrors* graph);
    void ClopperPearsonDivide(const TH1* pass, const TH1* total, TGraphAsymmErrors* graph);
    
    void SetCanvasStyle(TPad* c, const bool logScaleX, const bool logScaleY);
    void SetCanvasStyle(TPad* c, const bool logScaleX, const bool logScaleY, const bool logScaleZ);
    void SetProfileStyle(TProfile* p);
    void SetHistoStyle(TH1* h, const unsigned int lineStyle, const unsigned int markerStyle, const unsigned int lineColor);
    void SetStackHistoStyle(TH1* h, const unsigned int color);
    void SetGraphStyle(TGraph* g, const unsigned int markerStyle, const unsigned int lineColor);
    template <typename T>
    void CreateAndWriteCanvases(const unsigned int lineStyle, const unsigned int markerStyle, const unsigned int color, const bool logScaleX, const bool logScaleY, std::vector<T*> vHistos);
    template <typename T>
    void CreateAndWriteCanvases(const char* plotOptions, const bool logScaleX, const bool logScaleY, const bool logScaleZ, std::vector<T*> vHistos);
    template <typename T>
    void CreateAndWriteCanvases(const unsigned int markerStyle, const unsigned int color, const bool logScaleX, const bool logScaleY, std::vector<T*> vGraphs);
    
    void CreateAndWriteCanvas(const char* cName, const unsigned int lineStyle, const unsigned int markerStyle, const unsigned int color, const bool logScaleX, const bool logScaleY, TH1* histo);
    void CreateAndWriteCanvas(const char* cName, const char* cTitle, const unsigned int lineStyle, const unsigned int markerStyle, const unsigned int color, const bool logScaleX, const bool logScaleY, TH1* histo);
    void CreateAndWriteCanvas(const char* cName, const unsigned int lineStyle, const unsigned int markerStyle, const unsigned int color, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, const TH1* histo);
    void CreateAndWriteCanvas(const char* cName, const char* cTitle, const unsigned int lineStyle, const unsigned int markerStyle, const unsigned int color, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, TH1* histo);
    void CreateAndWriteCanvas(const char* cName, const std::string plotOptions, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, const bool logScaleZ, TH2* histo);
    void CreateAndWriteCanvas(const char* cName, const std::string plotOptions, const bool logScaleX, const bool logScaleY, const bool logScaleZ, TH2* histo);
    void CreateAndWriteCanvas(const char* cName, const char* cTitle, const std::string plotOptions, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, const bool logScaleZ, TH2* histo);
    void CreateAndWriteCanvas(const char* cName, const char* cTitle, const std::string plotOptions, const bool logScaleX, const bool logScaleY, const bool logScaleZ, TH2* histo);
    void CreateAndWriteCanvas3D(const char* cName, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, const bool logScaleZ, const double phi, const double theta, TH2* histo);
    void CreateAndWriteCanvas3D(const char* cName, const bool logScaleX, const bool logScaleY, const bool logScaleZ, const double phi, const double theta, TH2* histo);
    void CreateAndWriteCanvas3D(const char* cName, const char* cTitle, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, const bool logScaleZ, const double phi, const double theta, TH2* histo);
    void CreateAndWriteCanvas3D(const char* cName, const char* cTitle, const bool logScaleX, const bool logScaleY, const bool logScaleZ, const double phi, const double theta, TH2* histo);

//     void CreateAndWriteCanvas(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvas(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvas(const char* cName, const char* cTitle, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvas(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    void CreateAndWriteCanvas(const char* cName, const char* cTitle, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);

    void CreateAndWriteCanvasWithDifferences(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvasWithDifferences(const char* cName, const char* cTitle, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvasWithDifferences(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    void CreateAndWriteCanvasWithDifferences(const char* cName, const char* cTitle, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);

    void CreateAndWriteCanvasWithPulls(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvasWithPulls(const char* cName, const char* cTitle, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvasWithPulls(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    void CreateAndWriteCanvasWithPulls(const char* cName, const char* cTitle, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);

    void CreateAndWriteCanvasWithRatios(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvasWithRatios(const char* cName, const char* cTitle, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteCanvasWithRatios(const char* cName, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    void CreateAndWriteCanvasWithRatios(const char* cName, const char* cTitle, const unsigned int* lineStyles, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    
    // These canvases are plotted by default with the first histogram as points and the others stacked as fill area
    void CreateAndWriteStackCanvasWithDifferences(const char* cName, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteStackCanvasWithDifferences(const char* cName, const char* cTitle, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteStackCanvasWithDifferences(const char* cName, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    void CreateAndWriteStackCanvasWithDifferences(const char* cName, const char* cTitle, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    
    void CreateAndWriteStackCanvasWithPulls(const char* cName, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteStackCanvasWithPulls(const char* cName, const char* cTitle, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteStackCanvasWithPulls(const char* cName, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    void CreateAndWriteStackCanvasWithPulls(const char* cName, const char* cTitle, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    
    void CreateAndWriteStackCanvasWithRatios(const char* cName, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteStackCanvasWithRatios(const char* cName, const char* cTitle, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    void CreateAndWriteStackCanvasWithRatios(const char* cName, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    void CreateAndWriteStackCanvasWithRatios(const char* cName, const char* cTitle, const unsigned int markerStyle, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, std::vector<TH1*> histos);
    
    void CreateAndWriteCanvasWith2DDifference(const char* cName, const char* cTitle, const std::string plotOptions, const bool logScaleX, const bool logScaleY, const bool logScaleZ, TH2* histo1, TH2* histo2);
    void CreateAndWriteCanvasWith2DRelDifference(const char* cName, const char* cTitle, const std::string plotOptions, const bool logScaleX, const bool logScaleY, const bool logScaleZ, TH2* histo1, TH2* histo2);
    void CreateAndWriteCanvasWith2DRatio(const char* cName, const char* cTitle, const std::string plotOptions, const bool logScaleX, const bool logScaleY, const bool logScaleZ, TH2* hNumerator, TH2* hDenominator);
    void CreateAndWriteCanvasWith2DPull(const char* cName, const char* cTitle, const std::string plotOptions, const bool logScaleX, const bool logScaleY, const bool logScaleZ, TH2* histo1, TH2* histo2);
    
    // To use with TGraphs
    TCanvas* CreateAndWriteCanvas(const char* cName, const unsigned int* markerStyles, const unsigned int* colors, const LegendDefinition leg, const bool logScaleX, const bool logScaleY, ...);
    TCanvas* CreateAndWriteCanvas(const char* cName, const unsigned int* markerStyles, const unsigned int* colors, const bool logScaleX, const bool logScaleY, ...);
    TCanvas* CreateAndWriteCanvas(const char* cName, const LegendDefinition leg, const unsigned int* markerStyles, const unsigned int* colors, const bool logScaleX, const bool logScaleY, std::vector<TGraph*> graphs);
    TCanvas* CreateAndWriteCanvas(const char* cName, const unsigned int* markerStyles, const unsigned int* colors, const bool logScaleX, const bool logScaleY, std::vector<TGraph*> graphs);
    TCanvas* CreateAndWriteCanvas(const char* cName, const unsigned int markerStyle, const unsigned int color, const bool logScaleX, const bool logScaleY, TGraph* graph);
    void PlotLegend(TPad* c, const std::vector<TObject*> histos, std::vector<std::string> plotOptions, const LegendDefinition leg);
    void PlotLegend(TPad* c, const std::vector<TH1*> histos, std::vector<std::string> plotOptions, const LegendDefinition leg);
    void PlotLegend(TPad* c, TH1* histo, THStack* stack, std::vector<std::string> plotOptions, const LegendDefinition leg);
    void PlotLegend(TPad* c, const std::vector<TGraph*> graphs, std::vector<std::string> plotOptions, const LegendDefinition leg);
    void PlotLegend(TPad* c, const TH2* histo, std::string plotOptions, const LegendDefinition leg);
};

#endif // HistoPlotter_h
