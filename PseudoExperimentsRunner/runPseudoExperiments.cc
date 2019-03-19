void runPseudoExperiments()
{
  // FIXME: this is just a test runner! A proper one must be written! (MG)
  // TODO: rebin and axis limits are missing! (MG)
  
  std::vector<std::string> inputHistoNames;
  inputHistoNames.push_back("hMuonDxySimplifiedMergedTemplate_1");
  inputHistoNames.push_back("hMuonDxySimplifiedMergedTemplate_2");
  inputHistoNames.push_back("hMuonDxySimplifiedMergedTemplate_4");
//   inputHistoNames.push_back("hMuonDxySimplifiedMergedTemplate_5");
  
  std::vector<std::string> outputHistoNames;
  outputHistoNames.push_back("B");
  outputHistoNames.push_back("C");
  outputHistoNames.push_back("D");
//   outputHistoNames.push_back("F");
  outputHistoNames.push_back("P");
  
  std::vector<std::string> inputHistoNamesDistrib;
  inputHistoNamesDistrib.clear();
  inputHistoNamesDistrib.push_back("hMuonDxy");
  
  std::vector<std::string> input2DHistoNamesDistrib;
  input2DHistoNamesDistrib.clear();
  input2DHistoNamesDistrib.push_back("hMuonDxy2D");
  
  std::vector<std::string> templatesForFit;
  std::vector<int> vConstraintsToComponent;
  std::vector<double> vConstraintValues;
  std::vector<double> vConstraintErrors;
  
  std::string selector = "muonV6Pt4_jetPFV0Pt10";
  bool use1D = true;
  
  if(selector.compare("muonV2Pt4") == 0)
  {
    templatesForFit.push_back("hBB");
    templatesForFit.push_back("hCC");
    templatesForFit.push_back("hBC");
    templatesForFit.push_back("hPP");
    templatesForFit.push_back("hDD");
    templatesForFit.push_back("hBD");
    templatesForFit.push_back("hCD");
    
    vConstraintsToComponent.push_back(-1);
    vConstraintsToComponent.push_back(-1);
    vConstraintsToComponent.push_back(0);
    vConstraintsToComponent.push_back(-1);
    vConstraintsToComponent.push_back(-1);
    vConstraintsToComponent.push_back(0);
    vConstraintsToComponent.push_back(1);
    
    // muonV2Pt4
    vConstraintValues.push_back(0.7262);
    vConstraintValues.push_back(0.0935);
    vConstraintValues.push_back(0.0574);
    vConstraintValues.push_back(0.0092);
    vConstraintValues.push_back(0.0152);
    vConstraintValues.push_back(0.0617);
    vConstraintValues.push_back(0.0374);
    
    // muonV2Pt4
    vConstraintErrors.push_back(0.);
    vConstraintErrors.push_back(0.);
    vConstraintErrors.push_back(0.001);
    vConstraintErrors.push_back(0.);
    vConstraintErrors.push_back(0.);
    vConstraintErrors.push_back(0.001);
    vConstraintErrors.push_back(0.007);
  }
  else if(selector.compare("muonV6Pt4_jetPFV0Pt10") == 0)
  {
    if(use1D)
    {
      templatesForFit.push_back("hB");
      templatesForFit.push_back("hC");
      //     templatesForFit.push_back("hP");
      templatesForFit.push_back("hD");
    }
    else
    {
      templatesForFit.push_back("hBB");
      templatesForFit.push_back("hCC");
      templatesForFit.push_back("hBC");
      //     templatesForFit.push_back("hPP");
      templatesForFit.push_back("hDD");
      templatesForFit.push_back("hBD");
      templatesForFit.push_back("hCD");
    }
    
    vConstraintsToComponent.push_back(-1);
    vConstraintsToComponent.push_back(-1);
    vConstraintsToComponent.push_back(0);
//     vConstraintsToComponent.push_back(-1);
    vConstraintsToComponent.push_back(-1);
    vConstraintsToComponent.push_back(0);
    vConstraintsToComponent.push_back(1);
    
    // muonV2Pt4
    vConstraintValues.push_back(0.6668);
    vConstraintValues.push_back(0.1332);
    vConstraintValues.push_back(0.0634);
//     vConstraintValues.push_back(0.0205);
    vConstraintValues.push_back(0.0205);
    vConstraintValues.push_back(0.0678);
    vConstraintValues.push_back(0.0504);
    
    // muonV2Pt4
    vConstraintErrors.push_back(0.0034);
    vConstraintErrors.push_back(0.0013);
    vConstraintErrors.push_back(0.0008);
//     vConstraintErrors.push_back(0.0005);
    vConstraintErrors.push_back(0.0005);
    vConstraintErrors.push_back(0.0009);
    vConstraintErrors.push_back(0.0008);
  }
  
  gSystem->CompileMacro("../utilities/MuonHistogramsBuilderBase.C");
  gSystem->CompileMacro("../utilities/MuonTemplatesBuilder.C");
  gSystem->CompileMacro("../utilities/MuonDistributionsBuilder.C");
  gSystem->CompileMacro("../utilities/PseudoExperiments.C");
  
  PseudoExperiments* pe = new PseudoExperiments("../MuonBFractionsAnalysis/RawTemplates__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5__muonV6Pt4_jetPFV0Pt10.root",
                                                inputHistoNames,
                                                outputHistoNames,
                                                "../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__muonV6Pt4.root",
                                                "hMuonUpsilonBKGSubDxy",
                                                "../MuonBFractionsAnalysis/RawDistributions__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5__muonV6Pt4_jetPFV0Pt10.root",
                                                inputHistoNamesDistrib,
                                                input2DHistoNamesDistrib,
                                                "hDxy1D",
                                                "hDxy2D",
                                                "PseudoExperiments_1IV_muonV6Pt4_jetPFV0Pt10.root");
  
  pe->SetRebinFactor(4);
  pe->SetNPseudoExp(10000);
  pe->UseTemplatesForFit(templatesForFit);
  pe->SetConstraintToComponent(vConstraintsToComponent);
  pe->SetConstraintValues(vConstraintValues);
  pe->SetConstraintErrors(vConstraintErrors);
  pe->DoPseudoExperiments("1IV");
}
