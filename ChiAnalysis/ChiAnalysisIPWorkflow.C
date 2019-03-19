#ifndef ChiAnalysisIPWorkflow_C
#define ChiAnalysisIPWorkflow_C



#include <sstream>



#include "ChiAnalysisIPWorkflow.h"



ChiAnalysisIPWorkflow::ChiAnalysisIPWorkflow(const std::string propertiesFileName_) : ChiAnalysisBase("ChiAnalysisIPWorkflow",propertiesFileName_) 
{
  // Empty body
}



void ChiAnalysisIPWorkflow::RunIPWorkflow()
{
  // Step 1: producers
  if(runProducers)
  {
    RunProducersStep();
  }
  // Step 2: builders
  if(runBuilders)
  {
    RunBuildersStep();
  }
  if(runFit)
  {
    RunFitStep();
  }
}



void ChiAnalysisIPWorkflow::RunBuildersStep()
{
  BuildIPTemplates();
  BuildDataDistribution();
}



void ChiAnalysisIPWorkflow::RunFitStep()
{
  std::ostringstream oss;
  oss.str("");
  oss << removeBinIP;
  std::string sRemoveBinIP = oss.str();
  
  FindMCFractionsIP();
  
  if(use2DDistributionsIP)
  {
    if(constrainIP == 1)
    {
      constraintValuesIP = "";
      constraintErrorsIP = "";
      for(unsigned int i = 0; i < templates2DIP.size()-1; i++)
      {
        std::cout << "templates2DIP[" << i << "] = " << templates2DIP[i] << std::endl;
        double constraintValue = mcFractionsIP["h" + templates2DIP[i]].first;
        oss.str(""); oss << constraintValue;
        constraintValuesIP+=oss.str();
        constraintValuesIP+=";";
        double constraintError = mcFractionsIP["h" + templates2DIP[i]].second;
        oss.str(""); oss << constraintError;
        constraintErrorsIP+=oss.str();
        constraintErrorsIP+=";";
      }
      std::cout << "templates2DIP[" << templates2DIP.size()-1 << "] = " << templates2DIP[templates2DIP.size()-1] << std::endl;
      double constraintValue = mcFractionsIP["h" + templates2DIP[templates2DIP.size()-1]].first;
      oss.str(""); oss << constraintValue;
      constraintValuesIP+=oss.str();
      std::cout << "constraintValuesIP = " << constraintValuesIP << std::endl;
      double constraintError = mcFractionsIP["h" + templates2DIP[templates2DIP.size()-1]].second;
      oss.str(""); oss << constraintError;
      constraintErrorsIP+=oss.str();
      std::cout << "constraintErrorsIP = " << constraintErrorsIP << std::endl;
      
      BinnedLogLikelihoodCalculator2DConstrained(outputDistributionsFileNameIP,outputTemplateFileNameIP,templatesToUseFor2DIPFit,constrainToComponentIP,constraintValuesIP,constraintErrorsIP,sRemoveBinIP);
    }
    else if(constrainIP == 2)
    {
      BinnedLogLikelihoodCalculator2DConstrainedSum(outputDistributionsFileNameIP,outputTemplateFileNameIP,templatesToUseFor2DIPFit,constrainSumToComponentIP,constraintSumValuesIP,constraintSumErrorsIP,sRemoveBinIP);
    }
    else
    {
      BinnedLogLikelihoodCalculator2D(outputDistributionsFileNameIP,outputTemplateFileNameIP,templatesToUseFor2DIPFit,sRemoveBinIP);
    }
    // Move results to correct places
    std::string shellCmd = "mv FitResults.txt FitResults" + variableToUseIP + "2D_" + selector + "_" + templateSignIP + "_" + smearedIPTemplatesString + ".txt";
    gSystem->Exec(shellCmd.c_str());
    shellCmd = "mv FitOutput.root FitOutput" + variableToUseIP + "2D_" + selector + "_" + templateSignIP + "_" + smearedIPTemplatesString + ".root";
    gSystem->Exec(shellCmd.c_str());
  }
  else
  {
    if(constrainIP == 1)
    {
      BinnedLogLikelihoodCalculator1DConstrained(outputDistributionsFileNameIP,outputTemplateFileNameIP,templatesToUseFor1DIPFit,constrainToComponentIP,constraintValuesIP,constraintErrorsIP);
    }
    else if (constrainIP == 2)
    {
      BinnedLogLikelihoodCalculator1DConstrainedSum(outputDistributionsFileNameIP,outputTemplateFileNameIP,templatesToUseFor1DIPFit,constrainSumToComponentIP,constraintSumValuesIP,constraintSumErrorsIP);
    }
    else
    {
      BinnedLogLikelihoodCalculator1D(outputDistributionsFileNameIP,outputTemplateFileNameIP,templatesToUseFor1DIPFit);
    }
    // Move results to correct places
    std::string shellCmd = "mv FitResults.txt FitResults" + variableToUseIP + "1D_" + selector + "_" + templateSignIP + "_" + smearedIPTemplatesString + ".txt";
    gSystem->Exec(shellCmd.c_str());
    shellCmd = "mv FitOutput.root FitOutput" + variableToUseIP + "1D_" + selector + "_" + templateSignIP + "_" + smearedIPTemplatesString + ".root";
    gSystem->Exec(shellCmd.c_str());
  }
}


void ChiAnalysisIPWorkflow::BuildIPTemplates()
{
  // Build IP Templates
  // Copy raw templates file to current directory
  // Attention: AccessPathName returns FALSE if file exists!
  if(createWorkingDirectory && gSystem->AccessPathName(rawTemplatesFileName.c_str()))
  {
    rawTemplatesFileName = "../" + rawTemplatesFileName;
//     std::string shellCmd = "cp ../" + rawTemplatesFileName + " .";
//     gSystem->Exec(shellCmd.c_str());
  }
  
  MuonTemplatesBuilder* mtb;
  
  if(usePromptFromDataIP)
  {
    std::cout << " ###########################\n";
    std::cout << " # RUN TEMPLATE IP BUILDER #\n";
    std::cout << " # use Prompt from Data    #\n";
    std::cout << " ###########################\n";
    if(fitTemplatesIP)
    {
      mtb = new MuonTemplatesBuilder(rawTemplatesFileName,
                                     outputTemplateFileNameIP,
                                     inputHistoNamesIP,
                                     outputHistoNamesIP,
                                     inputFileNamePromptIP,
                                     inputHistoNamePromptIP);
      mtb->SetRange(fitMinIP,fitMaxIP);
      mtb->SetRebinFactor(rebinFactorIP);
      mtb->SetRemoveBin(removeBinIP);
      mtb->Build1DTemplatesFromFit();
      mtb->Build1DDistributions();
      mtb->Build2DTemplatesFromFit();
      mtb->Build2DDistributions();
      mtb->SaveTemplates();
      mtb->CreateAndSavePrettyPlots();
    }
    else
    {
      mtb = new MuonTemplatesBuilder(rawTemplatesFileName,
                                     outputTemplateFileNameIP,
                                     inputHistoNamesIP,
                                     outputHistoNamesIP,
                                     inputFileNamePromptIP,
                                     inputHistoNamePromptIP);
      mtb->SetRange(fitMinIP,fitMaxIP);
      mtb->SetRebinFactor(rebinFactorIP);
      mtb->SetRemoveBin(removeBinIP);
      mtb->Build1DTemplates();
      mtb->Build1DDistributions();
      mtb->Build2DTemplates();
      mtb->Build2DDistributions();
      mtb->SaveTemplates();
      mtb->CreateAndSavePrettyPlots();
    }
  }
  else
  {
    std::cout << " ###########################\n";
    std::cout << " # RUN TEMPLATE IP BUILDER #\n";
    std::cout << " # use Prompt from MC      #\n";
    std::cout << " ###########################\n";
    if(fitTemplatesIP)
    {
      mtb = new MuonTemplatesBuilder(rawTemplatesFileName,
                                     outputTemplateFileNameIP,
                                     inputHistoNamesIP,
                                     outputHistoNamesIP,
                                     "",
                                     "");
      mtb->SetRange(fitMinIP,fitMaxIP);
      mtb->SetRebinFactor(rebinFactorIP);
      mtb->SetRemoveBin(removeBinIP);
      mtb->Build1DTemplatesFromFit();
      mtb->Build1DDistributions();
      mtb->Build2DTemplatesFromFit();
      mtb->Build2DDistributions();
      mtb->SaveTemplates();
      mtb->CreateAndSavePrettyPlots();
    }
    else
    {
      mtb = new MuonTemplatesBuilder(rawTemplatesFileName,
                                     outputTemplateFileNameIP,
                                     inputHistoNamesIP,
                                     outputHistoNamesIP,
                                     "",
                                     "");
      mtb->SetRange(fitMinIP,fitMaxIP);
      mtb->SetRebinFactor(rebinFactorIP);
      mtb->SetRemoveBin(removeBinIP);
      mtb->Build1DTemplates();
      mtb->Build1DDistributions();
      mtb->Build2DTemplates();
      mtb->Build2DDistributions();
      mtb->SaveTemplates();
      mtb->CreateAndSavePrettyPlots();
    }
  }
  if(use2DDistributionsIP)
  {
    removedBinsFractionsIP = mtb->FindIntegralOfRemovedBinsFor2DTemplates();
  }
  else
  {
    removedBinsFractionsIP = mtb->FindIntegralOfRemovedBinsFor1DTemplates();
  }
  delete mtb;
}



void ChiAnalysisIPWorkflow::BuildDataDistribution()
{
  // Build Data distributions
  // Copy raw distributions file to current directory
  // Attention: AccessPathName returns FALSE if file exists!
  if(createWorkingDirectory && gSystem->AccessPathName(rawDataDistributionsFileName.c_str()))
  {
    rawDataDistributionsFileName = "../" + rawDataDistributionsFileName;
//     std::string shellCmd = "cp ../" + rawDataDistributionsFileName + " .";
//     gSystem->Exec(shellCmd.c_str());
  }
  
  if(useFakeIP)
  {
    BuildDataDistributionFromFakeIP();
  }
  else
  {
    BuildDataDistributionFromRealIP();
  }
}



void ChiAnalysisIPWorkflow::BuildDataDistributionFromFakeIP()
{
  std::string step1FileName = "Step1FakeIPDistributions__" + dataSample + "__" + selector + ".root";

  if(useFakeIP == 1)
  {
    std::cout << " ###########################\n";
    std::cout << " # RUN TEMPLATE IP BUILDER #\n";
    std::cout << " # FOR FAKE FACTORIZED IP  #\n";
    std::cout << " ###########################\n";
    
    std::string step1FileName = "Step1FakeIPDistributions__" + dataSample + "__" + selector + ".root";
    
    MuonTemplatesBuilder* mtb = new MuonTemplatesBuilder(rawTemplatesFileName, step1FileName, inputHistoNamesStep1FakeIP, outputHistoNamesStep1FakeIP, "", "");
    mtb->SetRange(fitMinIP,fitMaxIP);
    mtb->SetRebinFactor(rebinFactorIP);
    mtb->SetRemoveBin(removeBinIP);
    mtb->Build1DTemplates();
    mtb->Build1DDistributions();
    mtb->Build2DTemplates();
    mtb->Build2DDistributions();
    mtb->SaveTemplates();
    mtb->CreateAndSavePrettyPlots();
    delete mtb;
  }
  std::vector<std::string> dummy;
  
  if(!use2DDistributionsIP)
  {
    if(useFakeIP == 1)
    {
      std::cout << " ###################################\n";
      std::cout << " # RUN DISTRIBUTIONS IP BUILDER 1D #\n";
      std::cout << " # FOR FAKE IP                     #\n";
      std::cout << " ###################################\n";
      
      MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(step1FileName,outputDistributionsFileNameIP,inputHistoNamesFakeIP,dummy,"hData","hData2D");
      ipdb->SetRange(fitMinIP,fitMaxIP);
      ipdb->SetScaleFactors(nEventsFakeIPComponent);
      ipdb->SetRebinFactor(1.);
      ipdb->Build1DDistribution();
      ipdb->SaveDistributions();
      ipdb->CreateAndSavePrettyPlots();
      delete ipdb;
    }
    else if(useFakeIP == 2)
    {
      std::cout << " ###################################\n";
      std::cout << " # RUN DISTRIBUTIONS IP BUILDER 1D #\n";
      std::cout << " # FOR FAKE IP                     #\n";
      std::cout << " ###################################\n";
      MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(rawTemplatesFileName,outputDistributionsFileNameIP,inputHistoNamesFakeIP,dummy,"hData","");
      ipdb->SetRange(fitMinIP,fitMaxIP);
      // TODO: scale factors do not work! Need to normalize histograms before passing them through MuonDistributionsBuilder
      //       ipdb->SetScaleFactors(nEventsFakeIPComponent);
      ipdb->SetRebinFactor(rebinFactorIP);
      ipdb->Build1DDistribution();
      //       ipdb->Build2DDistribution();
      ipdb->SaveDistributions();
      ipdb->CreateAndSavePrettyPlots();
      delete ipdb;
    }
  }
  else
  {
    if(useFakeIP == 1)
    {
      std::cout << " ###################################\n";
      std::cout << " # RUN DISTRIBUTIONS IP BUILDER 2D #\n";
      std::cout << " # FOR FAKE FACTORIZED IP          #\n";
      std::cout << " ###################################\n";
      
      MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(step1FileName,outputDistributionsFileNameIP,dummy,inputHistoNamesFakeIP,"hData","hData2D");
      ipdb->SetRange(fitMinIP,fitMaxIP);
      ipdb->SetScaleFactors(nEventsFakeIPComponent);
      ipdb->SetRebinFactor(1.);
      //   ipdb->Build1DDistribution();
      ipdb->Build2DDistribution();
      ipdb->SaveDistributions();
      ipdb->CreateAndSavePrettyPlots();
      delete ipdb;
    }
    else if(useFakeIP == 2)
    {
      std::cout << " ###################################\n";
      std::cout << " # RUN DISTRIBUTIONS IP BUILDER 2D #\n";
      std::cout << " # FOR FAKE NON FACTORIZED IP      #\n";
      std::cout << " ###################################\n";
      MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(rawTemplatesFileName,outputDistributionsFileNameIP,dummy,inputHistoNamesFakeIP,"","hData2D");
      ipdb->SetRange(fitMinIP,fitMaxIP);
      // TODO: scale factors do not work! Need to normalize histograms before passing them through MuonDistributionsBuilder
      //       ipdb->SetScaleFactors(nEventsFakeIPComponent);
      ipdb->SetRebinFactor(rebinFactorIP);
      ipdb->SetRemoveBin(removeBinIP);
      //   ipdb->Build1DDistribution();
      ipdb->Build2DDistribution();
      ipdb->SaveDistributions();
      ipdb->CreateAndSavePrettyPlots();
      delete ipdb;
    }
  }
}



void ChiAnalysisIPWorkflow::BuildDataDistributionFromRealIP()
{
  std::vector<std::string> dummy;
  
  if(!use2DDistributionsIP)
  {
    std::cout << " ###################################\n";
    std::cout << " # RUN DISTRIBUTIONS IP BUILDER 1D #\n";
    std::cout << " ###################################\n";
    std::vector<std::string> input;
    input.push_back(inputHistoNameIPDistribution);
    MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(rawDataDistributionsFileName,outputDistributionsFileNameIP,input,dummy,"hData","");
    ipdb->SetRange(fitMinIP,fitMaxIP);
    ipdb->SetRebinFactor(rebinFactorIP);
    ipdb->Build1DDistribution();
    ipdb->SaveDistributions();
    ipdb->CreateAndSavePrettyPlots();
    delete ipdb;
  }
  else
  {
    std::cout << " ###################################\n";
    std::cout << " # RUN DISTRIBUTIONS IP BUILDER 2D #\n";
    std::cout << " ###################################\n";
    std::vector<std::string> input;
    input.push_back(inputHistoNameIPDistribution);    
    MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(rawDataDistributionsFileName,outputDistributionsFileNameIP,dummy,input,"","hData2D");
    ipdb->SetRange(fitMinIP,fitMaxIP);
    ipdb->SetRebinFactor(rebinFactorIP);
    ipdb->SetRemoveBin(removeBinIP);
    //   ipdb->Build1DDistribution();
    ipdb->Build2DDistribution();
    ipdb->SaveDistributions();
    ipdb->CreateAndSavePrettyPlots();
    delete ipdb;
  }
}



void ChiAnalysisIPWorkflow::FindMCFractionsIP()
{
  mcFractionsFinder.SetInputFileName(rawTemplatesFileName);
  mcFractionsFinder.SetVariableNameIP(variableToUseIP);
  mcFractionsFinder.SetVariableNamePtRel(variableToUsePtRel);
  mcFractionsFinder.SetClassificationName(templateClassificationIP);
  mcFractionsFinder.SetSign(templateSignIP);
  mcFractionsFinder.SetTranslationMap(histoNamesIPTranslationMap);
  mcFractionsFinder.SetMaxIP(fitMaxIP);
  mcFractionsFinder.SetMaxPtRel(fitMaxPtRel);
  if(use2DDistributionsIP)
    mcFractionsFinder.SetTemplateNames(templatesToUseFor2DIPFit);
  else
    mcFractionsFinder.SetTemplateNames(templatesToUseFor1DIPFit);
  mcFractionsFinder.SetRemovedBinsFractions(removedBinsFractionsIP);
  if(use2DDistributionsIP)
    mcFractionsIP = mcFractionsFinder.FindMCFractionsFor2DTemplates();
  else
    mcFractionsIP = mcFractionsFinder.FindMCFractionsFor1DTemplates();
}



#endif // ChiAnalysisIPWorkflow_C
