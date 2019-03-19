#include "TSystem.h"

#include "../utilities/MuonTemplatesBuilder.C"
#include "../utilities/MuonDistributionsBuilder.C"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodCalculator2D.cc"




void MuonTemplatesShapeTests()
{
  std::string fileNameOfRawTemplates = "../RunChiAnalysis/RawTemplates__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5_SKIM__muonV6Pt4_jetPFV1Pt10.root";
  std::stringstream oss;
  std::string name;
  std::vector<std::string> variables;
  variables.push_back("DxySimplifiedMerged");
  variables.push_back("PtOutJetSimplifiedMergedV1");
  variables.push_back("PtOutJetSimplifiedMergedV2");
  std::vector<short int> maxComponents;
  maxComponents.push_back(7);
  maxComponents.push_back(4);
  maxComponents.push_back(5);
  std::vector<std::string> signs;
  signs.push_back("");
  signs.push_back("OS");
  signs.push_back("SS");
  std::vector<double> fitMinValues;
  fitMinValues.push_back(0);
  fitMinValues.push_back(0);
  fitMinValues.push_back(0);
  std::vector<double> fitMaxValues;
  fitMaxValues.push_back(0.2);
  fitMaxValues.push_back(3.5);
  fitMaxValues.push_back(3.5);
  std::vector<unsigned short> rebinFactors;
  rebinFactors.push_back(4);
  rebinFactors.push_back(50);
  rebinFactors.push_back(50);
  
  for(unsigned short int variableIndex = 0; variableIndex < variables.size(); variableIndex++)
  {
    for(unsigned short int i = 1; i < maxComponents[variableIndex]; i++)
    {
      for(unsigned short int j = i; j < maxComponents[variableIndex]; j++)
      {
        for(unsigned short int signIndex = 0; signIndex < signs.size(); signIndex++)
        {
          std::cout << "Sono qui 1\n";
          short int eventCode = j<=i?10*j+i:10*i+j;
          oss.str(""); oss << eventCode;
          std::string eventCodeString = oss.str();
//           std::pair<short int, short int> iIndexPair = std::make_pair(i,eventCode);
//           std::pair<short int, short int> jIndexPair = std::make_pair(j,eventCode);
          
          oss.str(""); oss << "hMuon" << variables[variableIndex] << "2DTemplate" << signs[signIndex] << "_" << eventCode;
          std::string histoName2DTemplate = oss.str();
          oss.str(""); oss << "hMuon" << variables[variableIndex] << "Template" << signs[signIndex] << "ByEventCode_" << i << "_" << eventCode;
          std::string histoNameFirst1DTemplate = oss.str();
          oss.str(""); oss << "hMuon" << variables[variableIndex] << "Template" << signs[signIndex] << "ByEventCode_" << j << "_" << eventCode;
          std::string histoNameSecond1DTemplate = oss.str();
          
          oss.str(""); oss << "Templates_" << variables[variableIndex] << "_ShapeTests_" << eventCode << ".root";
          std::string outputTemplateFileName = oss.str();
          
          std::vector<std::string> inputHistoNames;
          inputHistoNames.push_back(histoNameFirst1DTemplate);
          inputHistoNames.push_back(histoNameSecond1DTemplate);
          
          std::vector<std::string> outputHistoNames;
          oss.str(""); oss << i;
          outputHistoNames.push_back(oss.str());
          oss.str(""); oss << j;
          outputHistoNames.push_back(oss.str());
          
          std::cout << "Sono qui 2\n";
          MuonTemplatesBuilder* mtb;
          mtb = new MuonTemplatesBuilder(fileNameOfRawTemplates,
                                         outputTemplateFileName,
                                         inputHistoNames,
                                         outputHistoNames,
                                         "",
                                         "");
          std::cout << "Sono qui 2.1\n";
          mtb->SetRange(fitMinValues[variableIndex],fitMaxValues[variableIndex]);
          std::cout << "Sono qui 2.2\n";
          mtb->SetRebinFactor(rebinFactors[variableIndex]);
          std::cout << "Sono qui 2.3\n";
          mtb->SetRemoveBin(0);
          std::cout << "Sono qui 2.4\n";
          mtb->Build1DTemplates();
          std::cout << "Sono qui 2.5\n";
          mtb->Build1DDistributions();
          std::cout << "Sono qui 2.6\n";
          mtb->Build2DTemplates();
          std::cout << "Sono qui 2.7\n";
          mtb->Build2DDistributions();
          std::cout << "Sono qui 2.8\n";
          mtb->SaveTemplates();
          std::cout << "Sono qui 2.9\n";
          mtb->CreateAndSavePrettyPlots();
          std::cout << "Sono qui 2.10\n";
          delete mtb;
          std::cout << "Sono qui 3\n";
          
          oss.str(""); oss << "Distributions_" << variables[variableIndex] << "_ShapeTests_" << eventCode << ".root";
          std::string outputDistributionsFileName = oss.str();
          
          std::vector<std::string> inputHistoNamesPseudoDataDistributions;
          inputHistoNamesPseudoDataDistributions.push_back(histoName2DTemplate);
          std::vector<std::string> dummy;
          std::cout << "Sono qui 4\n";
          
          MuonDistributionsBuilder* mdb = new MuonDistributionsBuilder(fileNameOfRawTemplates,outputDistributionsFileName,dummy,inputHistoNamesPseudoDataDistributions,"","hData2D");
          mdb->SetRange(fitMinValues[variableIndex],fitMaxValues[variableIndex]);
          mdb->SetRebinFactor(rebinFactors[variableIndex]);
          mdb->SetRemoveBin(0);
          //   ipdb->Build1DDistribution();
          mdb->Build2DDistribution();
          mdb->SaveDistributions();
          mdb->CreateAndSavePrettyPlots();
          delete mdb;
          std::cout << "Sono qui 5\n";
          
          BinnedLogLikelihoodCalculator2D(outputDistributionsFileName,outputTemplateFileName,eventCodeString,"0");
          std::cout << "Sono qui 6\n";
          std::string shellCmd = "mv FitResults.txt FitResults" + variables[variableIndex] + "2D_ShapeTests_" + eventCodeString + "_" + signs[signIndex] + ".txt";
          gSystem->Exec(shellCmd.c_str());
          shellCmd = "mv FitOutput.root FitOutput" + variables[variableIndex] + "2D_ShapeTests_" + eventCodeString + "_" + signs[signIndex] + ".root";
          gSystem->Exec(shellCmd.c_str());
          std::cout << "Sono qui 7\n";
        }
      }
    }
  }
}
