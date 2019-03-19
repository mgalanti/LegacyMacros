#ifndef ChiAnalysisBase_C
#define ChiAnalysisBase_C



#include <fstream>
#include <iostream>

#include "TSystem.h"

#include "ChiAnalysisBase.h"



ChiAnalysisBase::ChiAnalysisBase(const std::string className_, const std::string propertiesFileName_) // : mtp(0) , mtb(0)
{
  className = className_;
  propertiesFileName = propertiesFileName_;
  unixEpoch = (gSystem->GetFromPipe("date +%s")).Data();
  baseDir = (gSystem->GetFromPipe("dirname `pwd`")).Data();
  
  ReadProperties(propertiesFileName, properties);
  
  AssignProperties();
  
  if(createWorkingDirectory)
  {
    CreateWorkingDirectory();
  }
  
  workingDir = gSystem->pwd();
  
  if(redirectOutputToFile)
  {
    RedirectOutputToFile();
  }
  
  std::cout << "baseDir = " << baseDir << std::endl;
  std::cout << "workingDir = " << workingDir << std::endl;
  
  DumpJobConfig();
}



void ChiAnalysisBase::RunProducersStep()
{
  // 1- Produce raw templates file
  MuonTemplatesProducer* mtp = new MuonTemplatesProducer(mcSample, selector, maxEvents);
  mtp->Loop();
  
  // Copy/move output files
  std::string oldRawTemplatesFileName = "MuonTemplatesProducer__Histograms__" + mcSample + "__" + selector + ".root";

  std::string shellCmd;
  shellCmd = "cp " + oldRawTemplatesFileName + " " + rawTemplatesFileName;
  gSystem->Exec(shellCmd.c_str());
  if(createWorkingDirectory)
  {
    shellCmd = "mv " + oldRawTemplatesFileName + " ../../MuonTemplatesProducer/";
    gSystem->Exec(shellCmd.c_str());
    shellCmd = "cp " + rawTemplatesFileName + " ../";
    gSystem->Exec(shellCmd.c_str());    
  }
  else
  {
    shellCmd = "mv " + oldRawTemplatesFileName + " ../MuonTemplatesProducer/";
    gSystem->Exec(shellCmd.c_str());
  }
  delete mtp;
  
  // 2- Produce raw data distributions file
  std::string oldRawDataDistributionsFileName;
  if(useFakeIP)
  {
    std::cout << " #######################\n";
    std::cout << " # FAKE IP SWITCH: YES #\n";
    std::cout << " #######################\n";
    std::cout << " #########################\n";
    std::cout << " # RUN TEMPLATE PRODUCER #\n";
    std::cout << " # for Fake IP           #\n";
    std::cout << " #########################\n";
    
    MuonTemplatesProducer* mtpFakeIP = new MuonTemplatesProducer(dataSample, selector, maxEvents);
    mtpFakeIP->Loop();
    delete mtpFakeIP;
    oldRawDataDistributionsFileName = "MuonTemplatesProducer__Histograms__" + dataSample + "__" + selector + ".root";
    shellCmd = "cp " + oldRawDataDistributionsFileName + " " + rawDataDistributionsFileName;
    gSystem->Exec(shellCmd.c_str());
  }
  else
  {
    std::cout << " ######################\n";
    std::cout << " # FAKE IP SWITCH: NO #\n";
    std::cout << " ######################\n";
    std::cout << " ##############################\n";
    std::cout << " # RUN DISTRIBUTIONS PRODUCER #\n";
    std::cout << " ##############################\n";
        
    MuonDistributionsProducer* mdp = new MuonDistributionsProducer(dataSample, selector, maxEvents);
    mdp->Loop();
    delete mdp;
    oldRawDataDistributionsFileName = "MuonDistributionsProducer__Histograms__" + dataSample + "__" + selector + ".root";
    shellCmd = "cp " + oldRawDataDistributionsFileName + " " + rawDataDistributionsFileName;
    gSystem->Exec(shellCmd.c_str());
  }
  
  // Copy/move output files
  if(createWorkingDirectory)
  {
    if(useFakeIP)
      shellCmd = "mv " + oldRawDataDistributionsFileName + " ../../MuonDistributionsProducer/";
    else
      shellCmd = "mv " + oldRawDataDistributionsFileName + " ../../MuonTemplatesProducer/";
    gSystem->Exec(shellCmd.c_str());
    shellCmd = "cp " + rawDataDistributionsFileName + " ../";
    gSystem->Exec(shellCmd.c_str());
  }
  else
  {
    if(useFakeIP)
      shellCmd = "mv " + oldRawDataDistributionsFileName + " ../../MuonDistributionsProducer/";
    else
      shellCmd = "mv " + oldRawDataDistributionsFileName + " ../../MuonTemplatesProducer/";
    gSystem->Exec(shellCmd.c_str());
  }
}



void ChiAnalysisBase::AssignProperties()
{
  // Meta configuration
  GetProperty("CreateWorkingDirectory", createWorkingDirectory);
  GetProperty("RedirectOutputToFile", redirectOutputToFile);
  
  // Samples
  GetProperty("DataSample", dataSample);
  GetProperty("MCSample", mcSample);
  
  // Selector
  GetProperty("Selector", selector);
  
  // Job configuration
  GetProperty("RunProducers", runProducers);
  GetProperty("RunBuilders", runBuilders);
  GetProperty("RunFit", runFit);

  // Template and data distribution production properties
  GetProperty("MaxEvents", maxEvents);
  GetProperty("UseFakeIP", useFakeIP);
  GetProperty("UseFakePtRel", useFakePtRel);
  // Derived properties (not in configuration file)
  AssignDerivedProductionProperties();
  
  // IP template and data distributions building properties
  GetProperty("Use2DDistributionsIP", use2DDistributionsIP);
  GetProperty("UseDefaultInputHistoNameIPDistribution",useDefaultInputHistoNameIPDistribution);
  GetProperty("InputHistoNameIPDistribution", inputHistoNameIPDistribution);
  GetProperty("VariableToUseIP", variableToUseIP);
  GetProperty("TemplateClassificationIP", templateClassificationIP);
  GetProperty("TemplateSignIP", templateSignIP);
//   GetProperty("InputHistoNumbersForIPTemplates", inputHistoNumbersForIPTemplates);
//   GetProperty("OutputHistoNamesForIPTemplates", outputHistoNamesForIPTemplates);
  if(useFakeIP)
  {
//     GetProperty("InputHistoNumbersForStep1FakeIPDistributions", inputHistoNumbersForStep1FakeIPDistributions);
//     GetProperty("OutputHistoNamesForStep1FakeIPDistributions", outputHistoNamesForStep1FakeIPDistributions);
    GetProperty("InputHistoNamesForFakeIPDistributions", inputHistoNamesForFakeIPDistributions);
    GetProperty("NEventsPerEachFakeIPComponent", nEventsPerEachFakeIPComponent);
  }
  GetProperty("FitTemplatesIP", fitTemplatesIP);
  GetProperty("SmearedIPTemplatesString", smearedIPTemplatesString);
  GetProperty("UsePromptFromDataIP", usePromptFromDataIP);
  GetProperty("InputFileNamePromptIP", inputFileNamePromptIP);
  GetProperty("InputHistoNamePromptIP", inputHistoNamePromptIP);
  // Derived properties (not in configuration file)
  AssignDerivedIPBuildingProperties();
  
  // PtRel template and data distributions building properties
  GetProperty("Use2DDistributionsPtRel", use2DDistributionsPtRel);
  GetProperty("UseDefaultInputHistoNamePtRelDistribution",useDefaultInputHistoNamePtRelDistribution);
  GetProperty("InputHistoNamePtRelDistribution", inputHistoNamePtRelDistribution);
  GetProperty("VariableToUsePtRel", variableToUsePtRel);
  GetProperty("TemplateClassificationPtRel", templateClassificationPtRel);
  GetProperty("TemplateSignPtRel", templateSignPtRel);
  if(useFakePtRel)
  {
    GetProperty("InputHistoNamesForFakePtRelDistributions", inputHistoNamesForFakePtRelDistributions);
    GetProperty("NEventsPerEachFakePtRelComponent", nEventsPerEachFakePtRelComponent);
  }
  GetProperty("FitTemplatesPtRel", fitTemplatesPtRel);
  GetProperty("SmearedPtRelTemplatesString", smearedPtRelTemplatesString);
  GetProperty("UseDFromDataPtRel", useDFromDataPtRel);
  GetProperty("CombineDAndCPtRel", combineDAndCPtRel);
  GetProperty("InputFileNameLightPtRel", inputFileNameLightPtRel);
//   GetProperty("InputHistoNameDPtRel", inputHistoNameDPtRel);
  // Derived properties (not in configuration file)
  AssignDerivedPtRelBuildingProperties();
  
  // IP fit properties
  GetProperty("FitMinIP", fitMinIP);
  GetProperty("FitMaxIP", fitMaxIP);
  GetProperty("RebinFactorIP", rebinFactorIP);
  GetProperty("RemoveBinIP", removeBinIP);
  GetProperty("ConstrainIP", constrainIP);
  GetProperty("TemplatesToUseFor1DIPFit", templatesToUseFor1DIPFit);
  GetProperty("TemplatesToUseFor2DIPFit", templatesToUseFor2DIPFit);
  GetProperty("ConstrainToComponentIP", constrainToComponentIP);
  GetProperty("ConstrainSumToComponentIP", constrainSumToComponentIP);
  // Derived properties (not in configuration file)
  AssignDerivedIPFitProperties();
  
  // PtRel fit properties
  GetProperty("FitMinPtRel", fitMinPtRel);
  GetProperty("FitMaxPtRel", fitMaxPtRel);
  GetProperty("RebinFactorPtRel", rebinFactorPtRel);
  GetProperty("RemoveBinPtRel", removeBinPtRel);
  GetProperty("ConstrainPtRel", constrainPtRel);
  GetProperty("TemplatesToUseFor1DPtRelFit", templatesToUseFor1DPtRelFit);
  GetProperty("TemplatesToUseFor2DPtRelFit", templatesToUseFor2DPtRelFit);
  GetProperty("ConstrainToComponentPtRel", constrainToComponentPtRel);
  GetProperty("ConstrainSumToComponentPtRel", constrainSumToComponentPtRel);
  GetProperty("FitResult1DFileNameIP", fitResult1DFileNameIP);
  GetProperty("FitResult2DFileNameIP", fitResult2DFileNameIP);
  // Derived properties (not in configuration file)
  AssignDerivedPtRelFitProperties();  
}



void ChiAnalysisBase::AssignDerivedProductionProperties()
{
  rawTemplatesFileName = "RawTemplates__" + mcSample + "__" + selector + ".root";
  if(useFakeIP)
    rawDataDistributionsFileName = "RawFakeDistributions__" + dataSample + "__" + selector + ".root";
  else
    rawDataDistributionsFileName = "RawDistributions__" + dataSample + "__" + selector + ".root";
  
}



void ChiAnalysisBase::AssignDerivedIPBuildingProperties()
{
  histoNamesIPTranslationMap = SetHistoNamesTranslationMap(templateClassificationIP);
  for(std::map<std::string,std::string>::const_iterator it = histoNamesIPTranslationMap.begin(); it != histoNamesIPTranslationMap.end(); it++)
  {
    if(!(usePromptFromDataIP && it->second.compare("P") == 0))
    {
      inputHistoNumbersIP.push_back(it->first);
      outputHistoNamesIP.push_back(it->second);
    }
    inputHistoNumbersStep1FakeIP.push_back(it->first);
    outputHistoNamesStep1FakeIP.push_back(it->second);
  }
  // If using prompt from data, add "P" at the end, otherwise remove last ";" from string
  if(usePromptFromDataIP)
  {
    outputHistoNamesIP.push_back("P");
  }
  for(unsigned int i = 0 ; i < inputHistoNumbersIP.size(); i++)
  {
    std::string histoName = "hMuon" + variableToUseIP + templateClassificationIP + "Template";
    if(smearedIPTemplatesString.size() != 0)
    {
      histoName+=("_" + smearedIPTemplatesString);
    }
    histoName+=("_" + inputHistoNumbersIP[i]);
    inputHistoNamesIP.push_back(histoName);
  }
  
  outputTemplateFileNameIP = "IPTemplates__" + mcSample + "__" + selector + smearedIPTemplatesString + ".root";
  if(createWorkingDirectory)
    inputFileNamePromptIP = "../" + inputFileNamePromptIP;
  if(useFakeIP)
  {
    for(unsigned int i = 0 ; i < inputHistoNumbersStep1FakeIP.size(); i++)
    {
      std::string histoName = "hMuon" + variableToUseIP + templateClassificationIP/* + templateSignIP*/ + "Template" + "_" + inputHistoNumbersStep1FakeIP[i];
      inputHistoNamesStep1FakeIP.push_back(histoName);
    }
    outputDistributionsFileNameIP = "FakeIPDistributions__" + dataSample + "__" + selector + ".root";
    TokenizeString(inputHistoNamesForFakeIPDistributions, inputHistoNamesFakeIP);
    std::vector<std::string> tmp;
    TokenizeString(nEventsPerEachFakeIPComponent,tmp,"-");
    for(unsigned int i = 0; i < tmp.size(); i++)
    {
      nEventsFakeIPComponent.push_back(ConvertToDouble(tmp[i]));
    }
  }
  else
  {
    outputDistributionsFileNameIP = "IPDistributions__" + dataSample + "__" + selector + ".root";
  }
  if(useDefaultInputHistoNameIPDistribution)
  {
    if(use2DDistributionsIP)
    {
      inputHistoNameIPDistribution = "hMuon" + templateSignIP + variableToUseIP + "2D";
    }
    else
    {
      inputHistoNameIPDistribution = "hMuon" + templateSignIP + variableToUseIP;
    }
  }
}



void ChiAnalysisBase::AssignDerivedPtRelBuildingProperties()
{
  histoNamesPtRelTranslationMap = SetHistoNamesTranslationMap(templateClassificationPtRel);
  for(std::map<std::string,std::string>::const_iterator it = histoNamesPtRelTranslationMap.begin(); it != histoNamesPtRelTranslationMap.end(); it++)
  {
    if(!(useDFromDataPtRel && it->second.compare("l") == 0))
    {
      inputHistoNumbersPtRel.push_back(it->first);
      outputHistoNamesPtRel.push_back(it->second);
    }
    if(it->second.compare("c") == 0)
    {
      histoNumberFromMCToMergePtRel = it->first;
    }
    inputHistoNumbersStep1FakePtRel.push_back(it->first);
    outputHistoNamesStep1FakePtRel.push_back(it->second);
  }
  // If using prompt from data, add "P" at the end, otherwise remove last ";" from string
  if(useDFromDataPtRel)
  {
    outputHistoNamesPtRel.push_back("l");
    inputHistoNameDPtRel = "hMuon" + variableToUsePtRel + "DataDriven";
  }
  for(unsigned int i = 0 ; i < inputHistoNumbersPtRel.size(); i++)
  {
    std::string histoName = "hMuon" + variableToUsePtRel + templateSignPtRel + templateClassificationPtRel + "Template";
    if(smearedPtRelTemplatesString.size() != 0)
    {
      histoName+=("_" + smearedPtRelTemplatesString);
    }
    histoName+=("_" + inputHistoNumbersPtRel[i]);
    inputHistoNamesPtRel.push_back(histoName);
  }
  
  outputFileNameMergedRawPtRel = "RawMergedPtRelTemplates__" + selector  + ".root";
  histoNameFromMCToMergePtRel = "hMuon" + variableToUsePtRel + templateClassificationPtRel + "Template" + "_" + histoNumberFromMCToMergePtRel;
  histoNameMergedLightPtRel = "hMuon" + variableToUsePtRel + "LightTemplate";
  histoNamesToCopyForPtRelTemplates = inputHistoNamesPtRel;
  
  outputTemplateFileNamePtRel = "PtRelTemplates__" + mcSample + "__" + selector + smearedPtRelTemplatesString + ".root";
  if(createWorkingDirectory)
  {
    inputFileNameLightPtRel = "../" + inputFileNameLightPtRel;
  }
  if(useFakePtRel)
  {
    for(unsigned int i = 0 ; i < inputHistoNumbersStep1FakePtRel.size(); i++)
    {
      std::string histoName = "hMuon" + variableToUsePtRel/* + templateSignPtRel*/ + templateClassificationPtRel + "Template" + "_" + inputHistoNumbersStep1FakePtRel[i];
      inputHistoNamesStep1FakePtRel.push_back(histoName);
    }
    outputDistributionsFileNamePtRel = "FakePtRelDistributions__" + dataSample + "__" + selector + ".root";
    TokenizeString(inputHistoNamesForFakePtRelDistributions, inputHistoNamesFakePtRel);
    std::vector<std::string> tmp;
    TokenizeString(nEventsPerEachFakePtRelComponent,tmp,"-");
    for(unsigned int i = 0; i < tmp.size(); i++)
    {
      double tmpDouble = ConvertToDouble(tmp[i]);
      nEventsFakePtRelComponent.push_back(ConvertToDouble(tmp[i]));
    }
  }
  else
  {
    outputDistributionsFileNamePtRel = "PtRelDistributions__" + dataSample + "__" + selector + ".root";
  }
  if(useDefaultInputHistoNamePtRelDistribution)
  {
    if(use2DDistributionsPtRel)
    {
      inputHistoNamePtRelDistribution = "hMuon" + templateSignPtRel + variableToUsePtRel + "2D";
    }
    else
    {
      inputHistoNamePtRelDistribution = "hMuon" + templateSignPtRel + variableToUsePtRel;    
    }
  }
}



void ChiAnalysisBase::AssignDerivedIPFitProperties()
{
  TokenizeString(templatesToUseFor1DIPFit, templates1DIP);
  TokenizeString(templatesToUseFor2DIPFit, templates2DIP);
//  constraintValuesIP = "0.716;0.0924;0.0566;0.0184;0.0149;0.0601;0.0369"; // TODO: a method should calculate these automatically! 
//  constraintErrorsIP = "0.002;0.0008;0.0007;0.0004;0.0004;0.0007;0.0005"; // TODO: a method should calculate these automatically!
//  constraintErrorsIP = "0.002;0.0008;0.0007;0.0004;0.0007;0.0005"; // TODO: a method should calculate these automatically!
  constraintSumValuesIP = "0.790";
  constraintSumErrorsIP = "0.004";
}



void ChiAnalysisBase::AssignDerivedPtRelFitProperties()
{
  TokenizeString(templatesToUseFor1DPtRelFit, templates1DPtRel);
  TokenizeString(templatesToUseFor2DPtRelFit, templates2DPtRel);
  //  constraintValuesIP = "0.716;0.0924;0.0566;0.0184;0.0149;0.0601;0.0369"; // TODO: a method should calculate these automatically! 
  //  constraintErrorsIP = "0.002;0.0008;0.0007;0.0004;0.0004;0.0007;0.0005"; // TODO: a method should calculate these automatically!
  //  constraintErrorsIP = "0.002;0.0008;0.0007;0.0004;0.0007;0.0005"; // TODO: a method should calculate these automatically!
  constraintSumValuesPtRel = "0.790";
  constraintSumErrorsPtRel = "0.004";
  if(createWorkingDirectory)
  {
    fitResult1DFileNameIP = "../" + fitResult1DFileNameIP;
    fitResult2DFileNameIP = "../" + fitResult2DFileNameIP;
  }
}



void ChiAnalysisBase::CreateWorkingDirectory()
{
  std::string directoryName = className + "_" + unixEpoch;
  gSystem->mkdir(directoryName.c_str());
  gSystem->cd(directoryName.c_str());
}



void ChiAnalysisBase::RedirectOutputToFile()
{
  std::string logFileName;
  if(createWorkingDirectory)
  {
    logFileName = className + "_output.log";
  }
  else
  {
    logFileName = className + "_output_" + unixEpoch + ".log";
  }
  outputLogFile = std::freopen(logFileName.c_str(), "w", stdout);
  std::freopen(logFileName.c_str(), "a", stderr);
  
//   outStream = new std::ofstream(logFileName.c_str());
//   std::cout.rdbuf(outStream->rdbuf()); // redirect std::cout!
//   std::cerr.rdbuf(outStream->rdbuf()); // redirect std::cerr!
}



void ChiAnalysisBase::DumpJobConfig()
{
  std::string outFileName;
  if(createWorkingDirectory)
    outFileName = className + "_job.cfg";
  else
    outFileName = className + "_job_" + unixEpoch + ".cfg";
  ofstream outFile;
  outFile.open(outFileName.c_str());
  
  for(std::map<std::string,std::string>::iterator it = properties.begin(); it != properties.end(); it++)
  {
    outFile << it->first << " " << it->second << std::endl;
  }
  outFile.close();  
}



std::map<std::string,std::string> ChiAnalysisBase::SetHistoNamesTranslationMap(const std::string classification)
{
  std::map<std::string,std::string> result;
  //TODO: could be made configurable, for the time being it is hardcoded
  if(classification.compare("SimplifiedMerged") == 0)
  {
    result["0"] = "N"; // Null, i.e. fakes
    result["1"] = "B"; // Direct and sequential b
    result["2"] = "C"; // Direct c
    result["3"] = "P"; // Prompt
    result["4"] = "D"; // DIF
    result["5"] = "F"; // PT, i.e. fakes
  }
  else if(classification.compare("SimplifiedMergedOS") == 0 || classification.compare("SimplifiedMergedSS") == 0)
  {
    result["0"] = "N"; // Null, i.e. fakes
    result["1"] = "B"; // Direct and sequential b - only for non-diagonal BX templates
    result["2"] = "C"; // Direct c
    result["3"] = "P"; // Prompt
    result["4"] = "D"; // DIF
    result["5"] = "F"; // PT, i.e. fakes
    result["6"] = "A"; // Direct and sequential b - only for diagonal BB templates
  }
  else if(classification.compare("SimplifiedMergedV0") == 0)
  {
    result["0"] = "n"; // Null, i.e. fakes
    result["1"] = "b"; // Direct b (no sequentials)
    result["2"] = "l"; // direct c + DIF
    result["3"] = "p"; // Prompt
    result["4"] = "s"; // Sequentials, i.e. b to c to mu
    result["5"] = "f"; // PT, i.e. fakes
  }
  else if(classification.compare("SimplifiedMergedV1") == 0)
  {
    result["0"] = "n"; // Not assigned!
    result["1"] = "b"; // Direct b (no sequentials)
    result["2"] = "s"; // Sequentials i.e. b to c to mu
    result["3"] = "l"; // Light, i.e. everything else
  }
  else if(classification.compare("SimplifiedMergedV2") == 0)
  {
    result["0"] = "n"; // Not assigned!
    result["1"] = "b"; // Direct b (no sequentials)
    result["2"] = "s"; // Sequentials i.e. b to c to mu
    result["3"] = "c"; // Direct c
    result["4"] = "d"; // Light, i.e. everything else
  }
  return result;
}



void ChiAnalysisBase::GetProperty(const std::string propertyName, std::string& output)
{
  if(properties.find(propertyName.c_str()) != properties.end())
  {
    output = properties[propertyName.c_str()];
  }
  else
  {
    std::cout << "E R R O R ! Needed property \"" << propertyName << "\" is missing from properties map!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
}



void ChiAnalysisBase::GetProperty(const std::string propertyName, bool& output)
{
  if(properties.find(propertyName) != properties.end())
  {
    std::istringstream iss(properties[propertyName]);
    int tmp;
    if(iss >> tmp)
    {
      tmp?output=true:output=false;
    }
    else
    {
      std::cout << "E R R O R ! Conversion of property \"" << propertyName << "\" to bool failed!\n";
      std::cout << "            Property value is \"" << properties[propertyName] << "\".\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }
  else
  {
    std::cout << "E R R O R ! Needed property \"" << propertyName << "\" is missing from properties map!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
}



void ChiAnalysisBase::GetProperty(const std::string propertyName, int& output)
{
  if(properties.find(propertyName) != properties.end())
  {
    std::istringstream iss(properties[propertyName]);
    int tmp;
    if(iss >> tmp)
    {
      output = tmp;
    }
    else
    {
      std::cout << "E R R O R ! Conversion of property \"" << propertyName << "\" to int failed!\n";
      std::cout << "            Property value is \"" << properties[propertyName] << "\".\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }
  else
  {
    std::cout << "E R R O R ! Needed property \"" << propertyName << "\" is missing from properties map!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
}



void ChiAnalysisBase::GetProperty(const std::string propertyName, double& output)
{
  if(properties.find(propertyName) != properties.end())
  {
    output = ConvertToDouble(properties[propertyName]);
  }
  else
  {
    std::cout << "E R R O R ! Needed property \"" << propertyName << "\" is missing from properties map!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
}



void ChiAnalysisBase::ReadProperties(const std::string propsFileName, std::map<std::string,std::string>& props)
{
  ifstream propertiesFile;
  propertiesFile.open(propsFileName.c_str());
  if(propertiesFile.is_open())
  {
    std::string line;
    while(!propertiesFile.eof())
    {
      getline(propertiesFile,line);
      std::vector<std::string> tmp;
      TokenizeString(line,tmp," ");
      if(tmp.size() == 2)
      {
        if(tmp[0].compare("INCLUDE") == 0)
        {
          ReadProperties(tmp[1],props);
        }
        else
        {
          props[tmp[0]] = tmp[1];
        }
      }
      else if(tmp.size() == 1)
      {
        props[tmp[0]] = "";
      }
      else if(tmp.size() == 0)
      {
        continue;
      }
    }
  }
  else
  {
    std::cout << "E R R O R ! Unable to open properties file \"" << propsFileName << "\"!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
}



double ChiAnalysisBase::ConvertToDouble(const std::string& str)
{
  std::istringstream i(str);
  double x;
  if (!(i >> x))
    throw BadConversion("convertToDouble(\"" + str + "\")");
  return x;
}



void ChiAnalysisBase::TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
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



#endif // ChiAnalysisBase_C
