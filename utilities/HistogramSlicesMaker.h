#ifndef HistogramSlicesMaker_h
#define HistogramSlicesMaker_h

#include "../utilities/HistoPlotter.C"

#include "TFile.h"

#include <sstream>



class HistogramSlicesMaker : public HistoPlotter
{
public:
  HistogramSlicesMaker(std::string inputFileName_,
                       std::string outputFileName_) :
    inputFileName(inputFileName_),
    outputFileName(outputFileName_)
    {Initialize();}
  HistogramSlicesMaker(std::string inputFileName_,
                       std::string outputFileName_,
                       std::vector<double> binLowEdges_,
                       std::vector<double> binHighEdges_) :
    inputFileName(inputFileName_),
    outputFileName(outputFileName_)
    {Initialize(binLowEdges_,binHighEdges_);}
  ~HistogramSlicesMaker();

  void MakeSlicesX(std::string yTitle); // On all the histograms in the input file
  void MakeSlicesY(std::string yTitle); // On all the histograms in the input file
  void MakeSlicesX(const std::string histoName, std::string yTitle); // On a specific histogram in the input file
  void MakeSlicesY(const std::string histoName, std::string yTitle); // On a specific histogram in the input file
  void SetBinEdges(const std::vector<double> lowEdges, const std::vector<double> highEdges);
  std::vector<double> GetBinLowEdges() {return binLowEdges;}
  std::vector<double> GetBinHighEdges() {return binHighEdges;}
  std::vector<std::string> getBinNames() {return binNames;}

private:
  void Initialize();
  void Initialize(const std::vector<double> lowEdges, const std::vector<double> highEdges);
  void MakeSliceX(const TH2* histo, const unsigned int iHisto, const std::string yTitle);
  void MakeSliceY(const TH2* histo, const unsigned int iHisto, const std::string yTitle);

  std::vector<double> binLowEdges;
  std::vector<double> binHighEdges;
  std::vector<std::string> binNames;

  std::string inputFileName;
  std::string outputFileName;
  TFile* inputFile;
  TFile* outputFile;

  std::ostringstream oss;

};

#endif // HistogramSlicesMaker_h
