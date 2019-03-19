/****************************************************************/
/* VERSIONE MOLTO VECCHIA DA SISTEMARE                          */
/* Questa dovrebbe essere usata con i dati veri                 */
/* Pier 06 / Maggio / 2010                                      */
/****************************************************************/

#include "TROOT.h"
#include "TFitterMinuit.h"
#include "BinnedLogLikelihood2DConstrainedProb.h"
#include "../utilities/FitResults2DPlotter.C"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ";");

template <typename T>
bool from_string(T& t,
                 const std::string& s,
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}


void BinnedLogLikelihoodCalculator2DConstrainedProb(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged, std::string sRemoveBin)
{

  TFile* inputFileData = new TFile(inputFileNameData.c_str());
  TFile* inputFileMC = new TFile(inputFileNameMC.c_str());

  TH2D* hData = (TH2D*)(inputFileData->Get("hData2D"));
  if(!hData)
  {
    std::cout << "E R R O R ! Data histogram hData2D not found in input file!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  std::vector<std::string> templateNames;
  TokenizeString(templateNamesMerged,templateNames);

  unsigned int removeBinResult;
  unsigned int removeBin;
  if(from_string<unsigned int>(removeBinResult,sRemoveBin,std::dec))
  {
    removeBin = removeBinResult;
  }
  else
  {
    std::cout << "E R R O R ! The value of the sRemoveBin string cannot be converted to an unsigned int!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  std::vector<TH2D*> templates;

  for(unsigned int iTemplate = 0; iTemplate < templateNames.size(); iTemplate++)
  {
    std::string histoName = "h" + templateNames[iTemplate];
    TH2D* hTemp = (TH2D*)inputFileMC->Get(histoName.c_str());
    if(hTemp)
      templates.push_back(hTemp);
    else
    {
      std::cout << "E R R O R ! Template " << histoName << " not found in input file!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }

  double nEvents = hData->Integral();

  std::cout << "nEvents = " << nEvents << std::endl;
  std::cout << "nTemplates = " << templates.size() << std::endl;

  std::cout << "Creating the Likelihood_calculator..." << std::endl;

  BinnedLogLikelihood2DConstrainedProb* likelihood = new BinnedLogLikelihood2DConstrainedProb(templates, hData, removeBin);

  std::cout << "Calling Minuit..." << std::endl;

  TFitterMinuit *minFit = new TFitterMinuit;
  minFit->Clear();
  minFit->CreateMinimizer();
  minFit->SetMinuitFCN(likelihood);

  std::vector<double> par;
  std::vector<double> parError;

  std::cout << "Setting the parameters of the minimization..." << std::endl;

  for(unsigned int iTemplate = 0; iTemplate < templates.size(); iTemplate++)
  {
    par.push_back(nEvents/templates.size());
    parError.push_back(sqrt(nEvents)/templates.size());

    std::string parName = "Par_" + templateNames[iTemplate];
    std::cout << parName << std::endl;
    minFit->SetParameter(iTemplate,parName.c_str(),par[iTemplate],parError[iTemplate],0.,nEvents);
  }

  std::cout << "Starting minimization..." << std::endl;
  int FitRes = minFit->Minimize(1000,0.1);

  if(FitRes != 0) 
  {
    std::cout << "W A R N I N G ! Fit DID NOT converge at first attempt!\n";
    std::cout << "                Let's try a second iteration...\n";

    TFitterMinuit *tempminFit = new TFitterMinuit;
    *tempminFit = *minFit;
    int tempFitRes = FitRes;
    
    // second minimization
    FitRes = minFit->Minimize(1000,0.1);
        
    if(FitRes != 0) 
    {
      std::cout << "W A R N I N G ! Fit DID NOT converge at second attempt!\n";
      std::cout << "                Returning to the results of the first attempt...\n";

      // go back to first minimization results
      *minFit = *tempminFit;
      FitRes = tempFitRes;
    }
    else
    {
      std::cout << "I N F O ! Fit DID converge at second attempt!\n";
      std::cout << "          ...\n";      
    }
  }
    
//   std::cout << "---- MINIMIZATION RESULTS ----" << std::endl;
//   minFit->PrintResults(4,0.);
  

  std::string outFileName("FitOutput.root");
  
  std::vector<unsigned int> ordering;
  if(par.size() > 6)
  {
    ordering.push_back(3);
    ordering.push_back(6);
    ordering.push_back(2);
    ordering.push_back(5);
    ordering.push_back(4);
    ordering.push_back(1);
    ordering.push_back(0);
    for(unsigned int i = ordering.size(); i < par.size(); i++)
      ordering.push_back(i);
  }
  else if(par.size() == 6)
  {
    ordering.push_back(5);
    ordering.push_back(2);
    ordering.push_back(4);
    ordering.push_back(3);
    ordering.push_back(1);
    ordering.push_back(0);
  }
  else
  {
    for(unsigned int i = 0; i < par.size(); i++)
      ordering.push_back(i);
  }
  
  FitResults2DPlotter* frp = new FitResults2DPlotter(templates, hData, minFit, outFileName, FitRes);
  
  frp->SetPlotOrder(ordering);
  frp->CreateAndSavePlots();
  delete frp;
  
}



void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}
