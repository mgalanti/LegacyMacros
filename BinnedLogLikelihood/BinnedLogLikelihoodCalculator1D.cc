#ifndef BinnedLogLikelihoodCalculator1D_cc
#define BinnedLogLikelihoodCalculator1D_cc

/****************************************************************/
/* VERSIONE MOLTO VECCHIA DA SISTEMARE                          */
/* Questa dovrebbe essere usata con i dati veri                 */
/* Pier 06 / Maggio / 2010                                      */
/****************************************************************/

#include "TROOT.h"
#include "TFitterMinuit.h"
#include "BinnedLogLikelihood1D.h"
#include "../utilities/FitResults1DPlotter.C"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>      // std::setprecision



#ifndef TokenizeString_dec
#define TokenizeString_dec
void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ";");
#endif



void BinnedLogLikelihoodCalculator1D(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged)
{

  TFile* inputFileData = new TFile(inputFileNameData.c_str());
  TFile* inputFileMC = new TFile(inputFileNameMC.c_str());

  TH1D* hData = (TH1D*)(inputFileData->Get("hData"));
  if(!hData)
  {
    std::cout << "E R R O R ! Data histogram hData not found in input file!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  std::vector<std::string> templateNames;
  TokenizeString(templateNamesMerged,templateNames);

  std::vector<TH1D*> templates;
  
  for(unsigned int iTemplate = 0; iTemplate < templateNames.size(); iTemplate++)
  {
    std::string histoName = "h" + templateNames[iTemplate];
    TH1D* hTemp = (TH1D*)inputFileMC->Get(histoName.c_str());
    if(hTemp)
      templates.push_back(hTemp);
    else
    {
      std::cout << "E R R O R ! Template " << histoName << " not found in input file!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }
  
  unsigned int nBins = hData->GetXaxis()->GetNbins();
  
  for(unsigned int iBin = 1; iBin <= nBins; iBin++)
  {
    double TemplateBinSum = 0.;
    for(unsigned int iTemplate = 0; iTemplate < templateNames.size(); iTemplate++)
    {
      TemplateBinSum = TemplateBinSum +  templates[iTemplate]->GetBinContent(iBin);
    }
    if ( TemplateBinSum <= 0. ) 
    {
      std::cout << "E R R O R ! All the templates have NO entries in bin " << iBin << "\n";
      std::cout << "            Exiting...\n";
      exit(1);      
    }
  }
  
  double nEvents = hData->Integral();

  std::cout << "nEvents = " << nEvents << std::endl;
  std::cout << "nTemplates = " << templates.size() << std::endl;

  std::cout << "Creating the Likelihood_calculator..." << std::endl;

  BinnedLogLikelihood1D* likelihood = new BinnedLogLikelihood1D(templates, hData);

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

  // first minimization
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

  if(par.size() < 3)
  {
    for(unsigned int i = 0; i < par.size(); i++)
      ordering.push_back(i);
  }
  else if(par.size() == 3)
  {
    ordering.push_back(2);
    ordering.push_back(1);
    ordering.push_back(0);
  }
  else if(par.size() > 3)
  {
    ordering.push_back(2);
    ordering.push_back(3);
    ordering.push_back(1);
    ordering.push_back(0);
    for(unsigned int i = ordering.size(); i < par.size(); i++)
      ordering.push_back(i);
  }
  
  FitResults1DPlotter* frp = new FitResults1DPlotter(templates, hData, minFit, outFileName, FitRes);
  
  frp->SetPlotOrder(ordering);
  frp->CreateAndSavePlots();
  delete frp;
  
}



#ifndef TokenizeString_def
#define TokenizeString_def
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
#endif // TokenizeString_def

#endif // BinnedLogLikelihoodCalculator1D_cc
