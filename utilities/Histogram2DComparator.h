#ifndef Histogram2DComparator_h
#define Histogram2DComparator_h

#include "HistoPlotter.C"

#include "TFile.h"

#include <vector>



class Histogram2DComparator : public HistoPlotter
{
  public:
    Histogram2DComparator(std::pair<std::string,std::string> inputFileNames_, std::string outputFileName_, std::string canvasTitle_, std::vector<std::string> legendLabels_) :
      inputFileNames(inputFileNames_),
      outputFileName(outputFileName_),
      canvasTitle(canvasTitle_),
      legendLabels(legendLabels_)
    {
      Initialize();
    };

    ~Histogram2DComparator()
    {
      Close();
    };
    void CompareHistograms(const std::string firstName, const std::string secondName, const std::string hName, const std::string hTitle);
    void CompareHistogramsWithPull(const std::string firstName, const std::string firstErrLoName, const std::string firstErrHiName, const std::string secondName, const std::string secondErrLoName, const std::string secondErrHiName, const std::string hName, const std::string hTitle);
    void CompareHistogramsWithPull(const std::string firstName, const std::string secondName, const std::string hName, const std::string hTitle);
    void CompareHistogramsWithRatio(const std::string firstName, const std::string secondName, const std::string hName, const std::string hTitle);
    
//     TCanvas* ComputePull(TH2* first, TH2* second);
//     TCanvas* ComputePull(TH2* first, TH2* firstHighError, TH2* firstLowError, TH2* second, TH2* secondHighError, TH2* secondLowError);
    
  private:
    void Initialize();
    void Close();
    void OpenInputFiles();
    void CloseInputFiles();
    void OpenOutputFile();
    void CloseOutputFile();
//     void CompareHistogram(const std::string histoName);

    std::pair<std::string,std::string> inputFileNames;
    std::pair<TFile*,TFile*> inputFiles;
    std::string outputFileName;
    TFile* outputFile;
    const unsigned int* lineStyles;
    const unsigned int* markerStyles;
    const unsigned int* colors;
    const std::string canvasTitle;
    std::vector<std::string> legendLabels;
};

#endif // Histogram2DComparator_h