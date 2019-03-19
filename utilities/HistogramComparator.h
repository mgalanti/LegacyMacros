#ifndef HistogramComparator_h
#define HistogramComparator_h

#include "HistoPlotter.C"

#include "TFile.h"

#include <vector>



class HistogramComparator : public HistoPlotter
{
  public:
    HistogramComparator(std::vector<std::string> inputFileNames_, std::string outputFileName_, unsigned short int normalizationMethod_, std::string normalizationHistoName_, unsigned int* lineStyles_, unsigned int* markerStyles_, unsigned int* colors_, std::string canvasTitle_, std::vector<std::string> legendLabels_) :
      inputFileNames(inputFileNames_),
      outputFileName(outputFileName_),
      normalizationMethod(normalizationMethod_),
      normalizationHistoName(normalizationHistoName_),
      lineStyles(lineStyles_),
      markerStyles(markerStyles_),
      colors(colors_),
      canvasTitle(canvasTitle_),
      legendLabels(legendLabels_)
    {
      Initialize();
    };

    ~HistogramComparator()
    {
      Close();
    };
    void LoopOverHistograms();
    void CompareHistogram(const std::vector<std::string> histoNames);
    
    void LoopOverGraphs();
    
  private:
    void Initialize();
    void Close();
    void OpenInputFiles();
    void CloseInputFiles();
    void OpenOutputFile();
    void CloseOutputFile();
    void CompareHistogram(const std::string histoName);
    void CompareGraph(const std::string graphName);
    int ComputeNormalizationFactors();
    int NormalizeHistograms(std::vector<TH1*> histos);
    void SetNormalizationFactors(std::vector<double> factors) {if(normalizationMethod == 4) normalizationFactors = factors;};
    
    std::vector<std::string> inputFileNames;
    std::vector<TFile*> inputFiles;
    std::string outputFileName;
    TFile* outputFile;
    // Normalization method can be
    // 0 - No normalization
    // 1 - Same area
    // 2 - Use the ratio of entries of a given histogram for all the histograms
    // 3 - Use the ratio of counts (= sum of bin entries times bin x-value) of a
    //     given histogram for all the histograms
    //     (In cases 2 and 3 the name of an histogram must be provided in the constructor)
    // 4 - Normalized to unity
    unsigned short int normalizationMethod;
    std::string normalizationHistoName;
    std::vector<double> normalizationFactors; // Used for method 2
    const unsigned int* lineStyles;
    const unsigned int* markerStyles;
    const unsigned int* colors;
    const std::string canvasTitle;
    std::vector<std::string> legendLabels;
};

#endif // HistogramComparator_h