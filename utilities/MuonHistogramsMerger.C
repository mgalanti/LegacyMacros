#ifndef MuonHistogramsMerger_cxx
#define MuonHistogramsMerger_cxx



#include "TROOT.h"


#include "MuonHistogramsMerger.h"



MuonHistogramsMerger::~MuonHistogramsMerger()
{
  if(outputFile)
    outputFile->Close();
  for(unsigned int iFile = 0; iFile < inputFiles.size(); iFile++)
  {
    if(inputFiles[iFile])
      inputFiles[iFile]->Close();
  }
}



void MuonHistogramsMerger::Initialize()
{
  for(unsigned int iFile = 0; iFile < inputFileNames.size(); iFile++)
  {
    TFile* tempInputFile = TFile::Open(inputFileNames[iFile].c_str(), "READ");
    if(tempInputFile)
    {
      inputFiles.push_back(tempInputFile);
    }
    else
    {
      std::cout << "E R R O R ! MuonHistogramsMerger::Initialize(): file \"" << inputFileNames[iFile] << "\" does not exist!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }
  TFile* tempOutputFile = TFile::Open(outputFileName.c_str(),"RECREATE");
  if(tempOutputFile)
  {
    outputFile = tempOutputFile;
  }
  else
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::Initialize(): output file \"" << outputFileName << "\" could not be created!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
}



void MuonHistogramsMerger::MergeHistograms(const std::string inputHistoName1,
                                           const unsigned int inputFileNumber1,
                                           const std::string inputHistoName2,
                                           const unsigned int inputFileNumber2,
                                           const std::string outputHistoName,
                                           const double fraction1,
                                           const unsigned int preserveNormalization,
                                           const double integral)
{
  // Input sanitization
  if(fraction1 <= 0 || fraction1 >= 1)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): argument \"fraction1\" outside limits!\n";
    std::cout << "            Given value is " << fraction1 << std::endl;
    std::cout << "            Please provide a value in the interval (0,1) (limits excluded).\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  if(preserveNormalization != 0 && preserveNormalization != 1 && preserveNormalization != 2)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): argument \"preserveNormalization\" has invalid value!\n";
    std::cout << "            Given value is " << preserveNormalization << std::endl;
    std::cout << "            Valid values are 0, 1, or 2.\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  if(integral <= 0)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): argument \"integral\" negative or zero!\n";
    std::cout << "            Given value is " << integral << std::endl;
    std::cout << "            Please provide a strictly positive value.\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  // Get input histograms from files
  TH1* inputHisto1 = (TH1*)inputFiles[inputFileNumber1]->Get(inputHistoName1.c_str());
  if(!inputHisto1)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): could not get histogram \"" << inputHistoName1 << "\"\n";
    std::cout << "            from file \"" << inputFileNames[inputFileNumber1] << "\".\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  TH1* inputHisto2 = (TH1*)inputFiles[inputFileNumber2]->Get(inputHistoName2.c_str());
  if(!inputHisto2)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): could not get histogram \"" << inputHistoName2 << "\"\n";
    std::cout << "            from file \"" << inputFileNames[inputFileNumber2] << "\".\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  bool inputHisto1Is2D = false;
  bool inputHisto2Is2D = false;
  if(inputHisto1->GetDimension() == 2)
    inputHisto1Is2D = true;
  if(inputHisto2->GetDimension() == 2)
    inputHisto2Is2D = true;
  if(inputHisto1Is2D != inputHisto2Is2D)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): attempt to merge together 1D and 2D histograms!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  bool histogramsHaveSameBinning = SameBins(inputHisto1, inputHisto2);
  if(!histogramsHaveSameBinning)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): attempt to merge together histograms with different binning!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  TH1* outputHisto = NULL;
  
  if(inputHisto1Is2D)
  {
    outputHisto = Create2DHistogram<TH2D>(outputHistoName.c_str(), "", inputHisto1->GetXaxis()->GetNbins(), inputHisto1->GetXaxis()->GetXmin(), inputHisto1->GetXaxis()->GetXmax(), inputHisto1->GetYaxis()->GetNbins(), inputHisto1->GetYaxis()->GetXmin(), inputHisto1->GetYaxis()->GetXmax(), inputHisto1->GetXaxis()->GetTitle(), inputHisto1->GetYaxis()->GetTitle(), inputHisto1->GetZaxis()->GetTitle());
  }
  else
  {
    outputHisto = Create1DHistogram<TH1D>(outputHistoName.c_str(), "", inputHisto1->GetXaxis()->GetNbins(), inputHisto1->GetXaxis()->GetXmin(), inputHisto1->GetXaxis()->GetXmax(), inputHisto1->GetXaxis()->GetTitle(), inputHisto1->GetYaxis()->GetTitle());
  }
  
  double integral1 = inputHisto1->Integral();
  double integral2 = inputHisto2->Integral();
  if(integral1 == 0)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): histogram \"" << inputHistoName1 << "\" is empty!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  if(integral2 == 0)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::MergeHistograms(): histogram \"" << inputHistoName2 << "\" is empty!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  // Here starts the actual merging of the two histograms
  
  double fraction2 = 1. - fraction1;
  
  double factor1 = 1;
  double factor2 = 1;

  // 0 means that the normalization of both input histograms can be altered in the output one
  if(preserveNormalization == 0)
  {
    factor1 = fraction1 * integral / integral1;
    factor2 = fraction2 * integral / integral2;
  }
  // 1 means that the normalization of the first histogram must be preserved and only the second one must be scaled.
  // The variable integral is not used in this case
  else if(preserveNormalization == 1)
  {
    factor2 = fraction2 * integral1 / integral2;
  }
  // 1 means that the normalization of the second histogram must be preserved and only the first one must be scaled.
  // The variable integral is not used in this case
  else if(preserveNormalization == 2)
  {
    factor1 = fraction1 * integral2 / integral1;
  }
  
  for(int iBinx = 0; iBinx < inputHisto1->GetNbinsX()+1; iBinx++)
  {
    for(int iBiny = 0; iBiny < inputHisto1->GetNbinsY()+1; iBiny++)
    {
      double binContent1 = factor1 * inputHisto1->GetBinContent(iBinx, iBiny);
      double binError1 = factor1 * inputHisto1->GetBinError(iBinx, iBiny); // ignoring the errors on integral1 and integral2
      double binContent2 = factor2 * inputHisto2->GetBinContent(iBinx, iBiny);
      double binError2 = factor2 * inputHisto2->GetBinError(iBinx, iBiny); // ignoring the errors on integral1 and integral2
      
      double binContent = binContent1 + binContent2;
      double binError = sqrt(binError1*binError1 + binError2*binError2);
      outputHisto->SetBinContent(iBinx, iBiny, binContent);
      outputHisto->SetBinError(iBinx, iBiny, binError);
    }
  }

  // Write histogram to output file
  // Save current directory to a string so the calling function is unaffected by what is done here
  // Damn ROOT static properties...
  std::string curDirName = gDirectory->GetPathStatic();
  outputFile->cd();
  outputHisto->Write();
  // Now return to the old directory, before exiting
  TDirectory* dir = gROOT->GetDirectory(curDirName.c_str());
  dir->cd();
}



void MuonHistogramsMerger::CopyHistogram(const std::string inputHistoName, const unsigned int inputFileNumber)
{
  // Get input histograms from files
  TH1* inputHisto = (TH1*)inputFiles[inputFileNumber]->Get(inputHistoName.c_str());
  if(!inputHisto)
  {
    std::cout << "E R R O R ! MuonHistogramsMerger::CopyHistogram(): could not get histogram \"" << inputHistoName << "\"\n";
    std::cout << "            from file \"" << inputFileNames[inputFileNumber] << "\".\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  // Write histogram to output file
  // Save current directory to a string so the calling function is unaffected by what is done here
  // Damn ROOT static properties...
  std::string curDirName = gDirectory->GetPathStatic();
  outputFile->cd();
  inputHisto->Write();
  // Now return to the old directory, before exiting
  TDirectory* dir = gROOT->GetDirectory(curDirName.c_str());
  dir->cd();
}



#endif // MuonHistogramsMerger_cxx

