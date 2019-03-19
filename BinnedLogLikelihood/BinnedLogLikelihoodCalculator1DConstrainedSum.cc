#ifndef BinnedLogLikelihoodCalculator1DConstrainedSum_cc
#define BinnedLogLikelihoodCalculator1DConstrainedSum_cc

/****************************************************************/
/* VERSIONE MOLTO VECCHIA DA SISTEMARE                          */
/* Questa dovrebbe essere usata con i dati veri                 */
/* Pier 06 / Maggio / 2010                                      */
/****************************************************************/

#include "TROOT.h"
#include "TFitterMinuit.h"
#include "BinnedLogLikelihood1DConstrainedSum.h"
#include "../utilities/FitResults1DPlotter.C"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include <vector>
#include <iostream>
#include <sstream>



#ifndef TokenizeString_dec
#define TokenizeString_dec
void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ";");
#endif



#ifndef from_string_def
#define from_string_def
template <typename T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}
#endif



void BinnedLogLikelihoodCalculator1DConstrainedSum(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged, std::string sConstraintComponentMerged, std::string sConstraintValueMerged, std::string sConstraintErrorMerged)
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

  std::vector<std::string> vsConstraintComponent;
  TokenizeString(sConstraintComponentMerged,vsConstraintComponent);

  std::vector<int> vConstraintComponent;
  for(unsigned int i = 0; i < vsConstraintComponent.size(); i++)
  {
    int result;
    if(from_string<int>(result, vsConstraintComponent[i], std::dec))
    {
      vConstraintComponent.push_back(result);
    }
    else
    {
      std::cout << "E R R O R ! One of the values in the ConstraintComponent vector cannot be converted to a int!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }

  std::vector<std::string> vsConstraintValue;
  TokenizeString(sConstraintValueMerged,vsConstraintValue);

  std::vector<double> vConstraintValue;
  for(unsigned int i = 0; i < vsConstraintValue.size(); i++)
  {
    double result;
    if(from_string<double>(result, vsConstraintValue[i], std::dec))
    {
      vConstraintValue.push_back(result);
    }
    else
    {
      std::cout << "E R R O R ! One of the values in the ConstraintValue vector cannot be converted to a double!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }

  std::vector<std::string> vsConstraintError;
  TokenizeString(sConstraintErrorMerged,vsConstraintError);
  std::vector<double> vConstraintError;
  for(unsigned int i = 0; i < vsConstraintError.size(); i++)
  {
    double result;
    if(from_string<double>(result, vsConstraintError[i], std::dec))
    {
      vConstraintError.push_back(result);
    }
    else
    {
      std::cout << "E R R O R ! One of the values in the ConstraintError vector cannot be converted to a double!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }  

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
  
  double nEvents = hData->Integral();

  std::cout << "nEvents = " << nEvents << std::endl;
  std::cout << "nTemplates = " << templates.size() << std::endl;

  std::cout << "Creating the Likelihood_calculator..." << std::endl;

  BinnedLogLikelihood1DConstrainedSum* likelihood = new BinnedLogLikelihood1DConstrainedSum(templates, hData, vConstraintComponent, vConstraintValue, vConstraintError);

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

#endif // BinnedLogLikelihoodCalculator1DConstrainedSum_cc
