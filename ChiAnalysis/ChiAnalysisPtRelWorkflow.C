#ifndef ChiAnalysisPtRelWorkflow_C
#define ChiAnalysisPtRelWorkflow_C



#include <sstream>



#include "ChiAnalysisPtRelWorkflow.h"



ChiAnalysisPtRelWorkflow::ChiAnalysisPtRelWorkflow(const std::string propertiesFileName_) : ChiAnalysisBase("ChiAnalysisPtRelWorkflow",propertiesFileName_) 
{
  // Empty body
}



void ChiAnalysisPtRelWorkflow::RunPtRelWorkflow()
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



void ChiAnalysisPtRelWorkflow::RunBuildersStep()
{
  BuildPtRelTemplates();
  BuildDataDistribution();
}



void ChiAnalysisPtRelWorkflow::RunFitStep()
{
  std::ostringstream oss;
  oss.str("");
  oss << removeBinPtRel;
  std::string sRemoveBinPtRel = oss.str();
  
  FindMCFractionsPtRel();
  
  if(use2DDistributionsPtRel)
  {
    if(constrainPtRel == 1)
    {
      constraintValuesPtRel = "";
      constraintErrorsPtRel = "";
      for(unsigned int i = 0; i < templates2DPtRel.size()-1; i++)
      {
        std::cout << "templates2DPtRel[" << i << "] = " << templates2DPtRel[i] << std::endl;
        double constraintValue = mcFractionsPtRel["h" + templates2DPtRel[i]].first;
        oss.str(""); oss << constraintValue;
        constraintValuesPtRel+=oss.str();
        constraintValuesPtRel+=";";
        double constraintError = mcFractionsPtRel["h" + templates2DPtRel[i]].second;
        oss.str(""); oss << constraintError;
        constraintErrorsPtRel+=oss.str();
        constraintErrorsPtRel+=";";
      }
      std::cout << "templates2DPtRel[" << templates2DPtRel.size()-1 << "] = " << templates2DPtRel[templates2DPtRel.size()-1] << std::endl;
      double constraintValue = mcFractionsPtRel["h" + templates2DPtRel[templates2DPtRel.size()-1]].first;
      oss.str(""); oss << constraintValue;
      constraintValuesIP+=oss.str();
      std::cout << "constraintValuesPtRel = " << constraintValuesPtRel << std::endl;
      double constraintError = mcFractionsPtRel["h" + templates2DPtRel[templates2DPtRel.size()-1]].second;
      oss.str(""); oss << constraintError;
      constraintErrorsPtRel+=oss.str();
      std::cout << "constraintErrorsPtRel = " << constraintErrorsPtRel << std::endl;
      
      BinnedLogLikelihoodCalculator2DConstrained(outputDistributionsFileNamePtRel,outputTemplateFileNamePtRel,templatesToUseFor2DPtRelFit,constrainToComponentPtRel,constraintValuesPtRel,constraintErrorsPtRel,sRemoveBinPtRel);
    }
    else if(constrainPtRel == 2)
    {
      BinnedLogLikelihoodCalculator2DConstrainedSum(outputDistributionsFileNamePtRel,outputTemplateFileNamePtRel,templatesToUseFor2DPtRelFit,constrainSumToComponentPtRel,constraintSumValuesPtRel,constraintSumErrorsPtRel,sRemoveBinPtRel);
    }
    else
    {
      BinnedLogLikelihoodCalculator2D(outputDistributionsFileNamePtRel,outputTemplateFileNamePtRel,templatesToUseFor2DPtRelFit,sRemoveBinPtRel);
    }
    // Move results to correct places
    std::string shellCmd = "mv FitResults.txt FitResults" + variableToUsePtRel + "2D_" + selector + "_" + templateSignPtRel + "_" + smearedPtRelTemplatesString + ".txt";
    gSystem->Exec(shellCmd.c_str());
    shellCmd = "mv FitOutput.root FitOutput" + variableToUsePtRel + "2D_" + selector + "_" + templateSignPtRel + "_" + smearedPtRelTemplatesString + ".root";
    gSystem->Exec(shellCmd.c_str());
  }
  else
  {
    if(constrainPtRel == 1)
    {
      BinnedLogLikelihoodCalculator1DConstrained(outputDistributionsFileNamePtRel,outputTemplateFileNamePtRel,templatesToUseFor1DPtRelFit,constrainToComponentPtRel,constraintValuesPtRel,constraintErrorsPtRel);
    }
    else if (constrainIP == 2)
    {
      BinnedLogLikelihoodCalculator1DConstrainedSum(outputDistributionsFileNamePtRel,outputTemplateFileNamePtRel,templatesToUseFor1DPtRelFit,constrainSumToComponentPtRel,constraintSumValuesPtRel,constraintSumErrorsPtRel);
    }
    else
    {
      BinnedLogLikelihoodCalculator1D(outputDistributionsFileNamePtRel,outputTemplateFileNamePtRel,templatesToUseFor1DPtRelFit);
    }
    // Move results to correct places
    std::string shellCmd = "mv FitResults.txt FitResults" + variableToUsePtRel + "1D_" + selector + "_" + templateSignPtRel + "_" + smearedPtRelTemplatesString + ".txt";
    gSystem->Exec(shellCmd.c_str());
    shellCmd = "mv FitOutput.root FitOutput" + variableToUsePtRel + "1D_" + selector + "_" + templateSignPtRel + "_" + smearedPtRelTemplatesString + ".root";
    gSystem->Exec(shellCmd.c_str());
  }
}



void ChiAnalysisPtRelWorkflow::BuildPtRelTemplates()
{
  // Build PtRel Templates
  // Copy raw templates file to current directory
  // Attention: AccessPathName returns FALSE if file exists!
  if(createWorkingDirectory && gSystem->AccessPathName(rawTemplatesFileName.c_str()))
  {
    rawTemplatesFileName = "../" + rawTemplatesFileName;
//     std::string shellCmd = "cp ../" + rawTemplatesFileName + " .";
//     gSystem->Exec(shellCmd.c_str());
  }
  
  MuonTemplatesBuilder* mtb;

  if(useDFromDataPtRel && combineDAndCPtRel)
  {
    double fractionOfCFromIPFit;
    double fractionOfDFromIPFit;
    if(use2DDistributionsPtRel)
    {
      fractionOfCFromIPFit = GetFractionFrom2DIPFit(fitResult2DFileNameIP, "C");
      fractionOfDFromIPFit = GetFractionFrom2DIPFit(fitResult2DFileNameIP, "D");
    }
    else
    {
      fractionOfCFromIPFit = GetFractionFrom1DIPFit(fitResult1DFileNameIP, "C");
      fractionOfDFromIPFit = GetFractionFrom1DIPFit(fitResult1DFileNameIP, "D");
    }
    double sumOfCDFractionsFromIPFit = fractionOfCFromIPFit + fractionOfDFromIPFit;
    fractionOfCFromIPFit = fractionOfCFromIPFit / sumOfCDFractionsFromIPFit;
    fractionOfDFromIPFit = fractionOfDFromIPFit / sumOfCDFractionsFromIPFit;

    std::cout << "\n**** Calculation of the fractions in the combined PtRel light template ****\n";
    std::cout << "From the IP fit, the combined L (i.e. C+D) template is made up of " << fractionOfCFromIPFit << " of C and of " << fractionOfDFromIPFit << " of D\n\n";
    
    MuonHistogramsMerger* mhm = new MuonHistogramsMerger(rawTemplatesFileName, inputFileNameLightPtRel, outputFileNameMergedRawPtRel);
    mhm->MergeHistograms(histoNameFromMCToMergePtRel, inputHistoNameDPtRel, histoNameMergedLightPtRel, fractionOfCFromIPFit, 1);
    for(unsigned int iHistoName = 0; iHistoName < histoNamesToCopyForPtRelTemplates.size(); iHistoName++)
    {
      mhm->CopyHistogramFromFirstFile(histoNamesToCopyForPtRelTemplates[iHistoName]);
    }
    delete mhm;
  }
  
  std::string fileNameOfPtRelRawTemplates;
  if(useDFromDataPtRel && combineDAndCPtRel)
  {
    fileNameOfPtRelRawTemplates = outputFileNameMergedRawPtRel;
  }
  else
  {
    fileNameOfPtRelRawTemplates = rawTemplatesFileName;
  }
  
  std::cout << " ##############################\n";
  std::cout << " # RUN TEMPLATE PtRel BUILDER #\n";
  std::cout << " ##############################\n";
  
  if(fitTemplatesPtRel)
  {
    std::cout << "E R R O R ! Fitted PtRel templates are not implemented yet!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  else
  {
    mtb = new MuonTemplatesBuilder(fileNameOfPtRelRawTemplates,
                                   outputTemplateFileNamePtRel,
                                   inputHistoNamesPtRel,
                                   outputHistoNamesPtRel,
                                   "",
                                   "");
    mtb->SetRange(fitMinPtRel,fitMaxPtRel);
    mtb->SetRebinFactor(rebinFactorPtRel);
    mtb->SetRemoveBin(removeBinPtRel);
    mtb->Build1DTemplates();
    mtb->Build1DDistributions();
    mtb->Build2DTemplates();
    mtb->Build2DDistributions();
    mtb->SaveTemplates();
    mtb->CreateAndSavePrettyPlots();
  }
  delete mtb;
}



void ChiAnalysisPtRelWorkflow::BuildDataDistribution()
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
  
  if(useFakePtRel)
  {
    BuildDataDistributionFromFakePtRel();
  }
  else
  {
    BuildDataDistributionFromRealPtRel();
  }
}



void ChiAnalysisPtRelWorkflow::BuildDataDistributionFromFakePtRel()
{
  std::string step1FileName = "Step1FakePtRelDistributions__" + dataSample + "__" + selector + ".root";

  if(useFakePtRel == 1)
  {
    std::cout << " ##############################\n";
    std::cout << " # RUN TEMPLATE PTREL BUILDER #\n";
    std::cout << " # FOR FAKE FACTORIZED PTREL  #\n";
    std::cout << " ##############################\n";
    
    MuonTemplatesBuilder* mtb = new MuonTemplatesBuilder(rawTemplatesFileName, step1FileName, inputHistoNamesStep1FakePtRel, outputHistoNamesStep1FakePtRel, "", "");
    mtb->SetRange(fitMinPtRel,fitMaxPtRel);
    mtb->SetRebinFactor(rebinFactorPtRel);
    mtb->SetRemoveBin(removeBinPtRel);
    mtb->Build1DTemplates();
    mtb->Build1DDistributions();
    mtb->Build2DTemplates();
    mtb->Build2DDistributions();
    mtb->SaveTemplates();
    mtb->CreateAndSavePrettyPlots();
    delete mtb;
  }
  std::vector<std::string> dummy;
  
  if(!use2DDistributionsPtRel)
  {
    if(useFakePtRel == 1)
    {
      std::cout << " ######################################\n";
      std::cout << " # RUN DISTRIBUTIONS PTREL BUILDER 1D #\n";
      std::cout << " # FOR FAKE PTREL                     #\n";
      std::cout << " ######################################\n";
      
      MuonDistributionsBuilder* mdb = new MuonDistributionsBuilder(step1FileName,outputDistributionsFileNamePtRel,inputHistoNamesFakePtRel,dummy,"hData","hData2D");
      mdb->SetRange(fitMinPtRel,fitMaxPtRel);
      mdb->SetScaleFactors(nEventsFakePtRelComponent);
      mdb->SetRebinFactor(1.);
      mdb->Build1DDistribution();
      mdb->SaveDistributions();
      mdb->CreateAndSavePrettyPlots();
      delete mdb;
    }
    else if(useFakePtRel == 2)
    {
      std::cout << " ######################################\n";
      std::cout << " # RUN DISTRIBUTIONS PTREL BUILDER 1D #\n";
      std::cout << " # FOR FAKE PTREL                     #\n";
      std::cout << " ######################################\n";
      MuonDistributionsBuilder* mdb = new MuonDistributionsBuilder(rawTemplatesFileName,outputDistributionsFileNamePtRel,inputHistoNamesFakePtRel,dummy,"hData","");
      mdb->SetRange(fitMinPtRel,fitMaxPtRel);
      // TODO: scale factors do not work! Need to normalize histograms before passing them through MuonDistributionsBuilder
      //       mdb->SetScaleFactors(nEventsFakePtRelComponent);
      mdb->SetRebinFactor(rebinFactorPtRel);
      mdb->Build1DDistribution();
//       mdb->Build2DDistribution();
      mdb->SaveDistributions();
      mdb->CreateAndSavePrettyPlots();
      delete mdb;
    }
  }
  else
  {
    if(useFakePtRel == 1)
    {
      std::cout << " ######################################\n";
      std::cout << " # RUN DISTRIBUTIONS PTREL BUILDER 2D #\n";
      std::cout << " # FOR FAKE FACTORIZED PTREL          #\n";
      std::cout << " ######################################\n";
      
      MuonDistributionsBuilder* mdb = new MuonDistributionsBuilder(step1FileName,outputDistributionsFileNamePtRel,dummy,inputHistoNamesFakePtRel,"hData","hData2D");
      mdb->SetRange(fitMinPtRel,fitMaxPtRel);
      mdb->SetScaleFactors(nEventsFakePtRelComponent);
      mdb->SetRebinFactor(1.);
      //   mdb->Build1DDistribution();
      mdb->Build2DDistribution();
      mdb->SaveDistributions();
      mdb->CreateAndSavePrettyPlots();
      delete mdb;
    }    
    else if(useFakePtRel == 2)
    {
      std::cout << " ######################################\n";
      std::cout << " # RUN DISTRIBUTIONS PTREL BUILDER 2D #\n";
      std::cout << " # FOR FAKE NON FACTORIZED PTREL      #\n";
      std::cout << " ######################################\n";
      MuonDistributionsBuilder* mdb = new MuonDistributionsBuilder(rawTemplatesFileName,outputDistributionsFileNamePtRel,dummy,inputHistoNamesFakePtRel,"","hData2D");
      mdb->SetRange(fitMinPtRel,fitMaxPtRel);
      // TODO: scale factors do not work! Need to normalize histograms before passing them through MuonDistributionsBuilder
      //       mdb->SetScaleFactors(nEventsFakePtRelComponent);
      mdb->SetRebinFactor(rebinFactorPtRel);
      mdb->SetRemoveBin(removeBinPtRel);
      //   ipdb->Build1DDistribution();
      mdb->Build2DDistribution();
      mdb->SaveDistributions();
      mdb->CreateAndSavePrettyPlots();
      delete mdb;
    }
  }
}



void ChiAnalysisPtRelWorkflow::BuildDataDistributionFromRealPtRel()
{
  std::vector<std::string> dummy;
  
  if(!use2DDistributionsPtRel)
  {
    std::cout << " ######################################\n";
    std::cout << " # RUN DISTRIBUTIONS PTREL BUILDER 1D #\n";
    std::cout << " ######################################\n";
    std::vector<std::string> input;
    input.push_back(inputHistoNamePtRelDistribution);
    MuonDistributionsBuilder* mdb = new MuonDistributionsBuilder(rawDataDistributionsFileName,outputDistributionsFileNamePtRel,input,dummy,"hData","");
    mdb->SetRange(fitMinPtRel,fitMaxPtRel);
    mdb->SetRebinFactor(rebinFactorPtRel);
    mdb->Build1DDistribution();
    mdb->SaveDistributions();
    mdb->CreateAndSavePrettyPlots();
    delete mdb;
  }
  else
  {
    std::cout << " ######################################\n";
    std::cout << " # RUN DISTRIBUTIONS PTREL BUILDER 2D #\n";
    std::cout << " ######################################\n";
    std::vector<std::string> input;
    input.push_back(inputHistoNamePtRelDistribution);    
    MuonDistributionsBuilder* mdb = new MuonDistributionsBuilder(rawDataDistributionsFileName,outputDistributionsFileNamePtRel,dummy,input,"","hData2D");
    mdb->SetRange(fitMinPtRel,fitMaxPtRel);
    mdb->SetRebinFactor(rebinFactorPtRel);
    mdb->SetRemoveBin(removeBinPtRel);
    //   ipdb->Build1DDistribution();
    mdb->Build2DDistribution();
    mdb->SaveDistributions();
    mdb->CreateAndSavePrettyPlots();
    delete mdb;
  }
}



void ChiAnalysisPtRelWorkflow::FindMCFractionsPtRel()
{
  mcFractionsFinder.SetInputFileName(rawTemplatesFileName);
  mcFractionsFinder.SetVariableNameIP(variableToUseIP);
  mcFractionsFinder.SetVariableNamePtRel(variableToUsePtRel);
  mcFractionsFinder.SetClassificationName(templateClassificationPtRel);
  mcFractionsFinder.SetSign(templateSignPtRel);
  mcFractionsFinder.SetTranslationMap(histoNamesPtRelTranslationMap);
  mcFractionsFinder.SetMaxIP(fitMaxIP);
  mcFractionsFinder.SetMaxPtRel(fitMaxPtRel);
  if(use2DDistributionsPtRel)
    mcFractionsFinder.SetTemplateNames(templatesToUseFor2DPtRelFit);
  else
    mcFractionsFinder.SetTemplateNames(templatesToUseFor1DPtRelFit);
  mcFractionsFinder.SetRemovedBinsFractions(removedBinsFractionsPtRel);
  if(use2DDistributionsPtRel)
    mcFractionsPtRel = mcFractionsFinder.FindMCFractionsFor2DTemplates();
  else
    mcFractionsPtRel = mcFractionsFinder.FindMCFractionsFor1DTemplates();
}



double ChiAnalysisPtRelWorkflow::GetFractionFrom1DIPFit(const std::string fileName, const std::string componentName)
{
  std::ifstream fit1DIPResultsFile(fileName.c_str());
  
  std::string line;
  
  double totalFraction = 0.;
  
  if(fit1DIPResultsFile.is_open())
  {
    while(getline(fit1DIPResultsFile,line))
    {
      if(line.c_str() == "")
      {
        continue;
      }
      std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom1DIPFit(...): line is " << line << std::endl;
      std::stringstream ss; ss.str(line);
      std::string name;
      std::string sFraction;
      ss >> name >> sFraction;
      std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom1DIPFit(...): Name is " << name << ", sFraction is " << sFraction << std::endl;
      std::string componentNameToCompare = "h" + componentName;
      if(name == componentNameToCompare)
      {
        double fraction = ConvertToDouble(sFraction);
        std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom1DIPFit(...): h" << componentName << " fraction is " << fraction << std::endl;
        std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom1DIPFit(...): totalFraction before sum is " << totalFraction << std::endl;
        totalFraction+=fraction;
        std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom1DIPFit(...): totalFraction after sum is " << totalFraction << std::endl;
      }
    }
    fit1DIPResultsFile.close();
  }
  else
  {
    std::cout << "E R R O R ! ChiAnalysisPtRelWorkflow::GetFractionFrom1DIPFit(...): Unable to open file " << fileName << "\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom1DIPFit(...): totalFraction is " << totalFraction << std::endl;
  return totalFraction;
}



double ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(const std::string fileName, const std::string componentName)
{
  std::ifstream fit2DIPResultsFile(fileName.c_str());
  
  std::string line;
  
  double totalFraction = 0.;
  
  if(fit2DIPResultsFile.is_open())
  {
    while(getline(fit2DIPResultsFile,line))
    {
      if(line.c_str() == "")
      {
        continue;
      }
      std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): line is " << line << std::endl;
      std::stringstream ss; ss.str(line);
      std::string name;
      std::string sFraction;
      ss >> name >> sFraction;
      std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): Name is " << name << ", sFraction is " << sFraction << std::endl;
      std::string componentNameToCompare = "h" + componentName + componentName;
      if(name == componentNameToCompare)
      {
        double fraction = ConvertToDouble(sFraction);
        std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): h" << componentName << componentName << " fraction is " << fraction << std::endl;
        std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): totalFraction before sum is " << totalFraction << std::endl;
        totalFraction+=fraction;
        totalFraction+=fraction;
        std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): totalFraction after sum is " << totalFraction << std::endl;
      }
      else
      {
        if(name.size() != 3)
        {
          continue;
        }
        if(name.substr(0,1).compare("h") != 0)
        {
          continue;
        }
        std::string substr2 = name.substr(1,1);
        std::string substr3 = name.substr(2,1);
        if(componentName == substr2 || componentName == substr3)
        {
          double fraction = ConvertToDouble(sFraction);
          std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): single-component " << name << " fraction is " << fraction << std::endl;
          std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): totalFraction before sum is " << totalFraction << std::endl;
          totalFraction+=fraction;
          totalFraction+=fraction;
          std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): totalFraction after sum is " << totalFraction << std::endl;
        }
      }
    }
    fit2DIPResultsFile.close();
  }
  else
  {
    std::cout << "E R R O R ! ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): Unable to open file " << fileName << "\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  totalFraction*=0.5;
  
  std::cout << "ChiAnalysisPtRelWorkflow::GetFractionFrom2DIPFit(...): totalFraction is " << totalFraction << std::endl;
  return totalFraction;
}



#endif // ChiAnalysisPtRelWorkflow_C
