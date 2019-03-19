#ifndef MuonHistogramsMerger_h
#define MuonHistogramsMerger_h

#include <string>
#include <vector>
#include <iostream>

#include "TFile.h"

#include "HistoPlotter.C"


class MuonHistogramsMerger : public HistoPlotter
{
  public:
    MuonHistogramsMerger()
    {
      std::cout << "MuonHistogramsBuilderBase: Please call constructor with arguments!\n";
    }
    MuonHistogramsMerger(const std::vector<std::string> inputFileNames_, const std::string outputFileName_) :
      inputFileNames(inputFileNames_),
      outputFileName(outputFileName_)
    {
      Initialize();
    }
    MuonHistogramsMerger(const std::string inputFileName1, const std::string inputFileName2, const std::string outputFileName_) :
      outputFileName(outputFileName_)
    {
      inputFileNames.clear();
      inputFileNames.push_back(inputFileName1);
      inputFileNames.push_back(inputFileName2);
      Initialize();
    }
    ~MuonHistogramsMerger();
    void Initialize();
    void MergeHistograms(const std::string inputHistoName1,
                         const unsigned int inputFileNumber1,
                         const std::string inputHistoName2,
                         const unsigned int inputFileNumber2,
                         const std::string outputHistoName,
                         const double fraction1,
                         const unsigned int preserveNormalization,
                         const double integral = 1);
    
    inline void MergeHistograms(const std::string inputHistoName1,
                                const std::string inputHistoName2,
                                const std::string outputHistoName,
                                const double fraction1,
                                const unsigned int preserveNormalization,
                                const double integral = 1)
    {
      MergeHistograms(inputHistoName1, 0, inputHistoName2, 1, outputHistoName, fraction1, preserveNormalization, integral);
    }
    
    inline void MergeHistograms(const std::string inputHistoName1,
                                const unsigned int inputFileNumber1,
                                const double nEvents1,
                                const std::string inputHistoName2,
                                const unsigned int inputFileNumber2,
                                const double nEvents2,
                                const std::string outputHistoName)
    {
      double integral = nEvents1 + nEvents2;
      if(integral <= 0)
      {
        std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): integral calculated from nEvents1 and nEvents2 is negative or zero!\n";
        std::cout << "            Given values are : nEvents1 = " << nEvents1 << ", nEvents2 = " << nEvents2 << std::endl;
        std::cout << "            Please provide valid values.\n";
        std::cout << "            Exiting...\n";
        exit(1);
      }
      double fraction1 = nEvents1 / integral;
      
      MergeHistograms(inputHistoName1, inputFileNumber1, inputHistoName2, inputFileNumber2, outputHistoName, fraction1, 0, integral);
    }
    
    inline void MergeHistograms(const std::string inputHistoName1,
                                const double nEvents1,
                                const std::string inputHistoName2,
                                const double nEvents2,
                                const std::string outputHistoName)
    {
      MergeHistograms(inputHistoName1, 0, nEvents1, inputHistoName2, 1, nEvents2, outputHistoName);
    }
    
    void CopyHistogram(const std::string inputHistoName, const unsigned int inputFileNumber);
    inline void CopyHistogramFromFirstFile(const std::string inputHistoName)
    {
      CopyHistogram(inputHistoName, 0);
    }
    inline void CopyHistorgamFromSecondFile(const std::string inputHistoName)
    {
      CopyHistogram(inputHistoName, 1);
    }
  private:
    std::vector<std::string> inputFileNames;
    std::string outputFileName;
    
    std::vector<TFile*> inputFiles;
    TFile* outputFile;
};



#endif // MuonHistogramsMerger_h
