#ifndef PseudoExperiments_cxx
#define PseudoExperiments_cxx

#include "TFitterMinuit.h"
#include "PseudoExperiments.h"



void PseudoExperiments::InitializePseudoExperiments()
{
  use1D = false;
  use2D = false;
  useIP = false;
  usePtRel = false;
  useFittedTemplates = false;
  useSmoothed0thIterHistograms = false;
  saveValidationPlotsPerIteration = false;
  linestyles[0] = 0;
  linestyles[1] = 0;
  markerstyles[0] = 21;
  markerstyles[1] = 21;
  colors[0] = 1;
  colors[1] = 2;
}


TH1D* PseudoExperiments::CreateTH1FromOscillatedTF1(const TF1* func, const std::string histoName, const unsigned int nBins, const double xMin, const double xMax, const std::string xTitle, const std::string yTitle)
{
  TH1::AddDirectory(false);
  TH1D* histo = Create1DHistogram<TH1D>(histoName.c_str(),"",nBins,xMin,xMax,xTitle.c_str(),yTitle.c_str());
  TH1::AddDirectory(true);
  for(unsigned int iBin = 1; iBin < nBins+1; iBin++)
  {
    double binCenter = histo->GetBinCenter(iBin);
    double binContent = func->Eval(binCenter);
    double newBinContent = rng.PoissonD(binContent);
    histo->SetBinContent(iBin,newBinContent);
    histo->SetBinError(iBin,sqrt(binContent));
  }
  return histo;
}



TH1D* PseudoExperiments::OscillateTH1(const TH1D* histo)
{
  std::string newName = histo->GetName();
  newName+="Oscillated";
  std::string newTitle = histo->GetTitle();
  newTitle+=" - Oscillated";
  unsigned int nBins = histo->GetXaxis()->GetNbins();
  TH1::AddDirectory(false);
  TH1D* result = Create1DHistogram<TH1D>(newName.c_str(),newTitle.c_str(),nBins,histo->GetXaxis()->GetXmin(),histo->GetXaxis()->GetXmax(),histo->GetXaxis()->GetTitle(),histo->GetYaxis()->GetTitle());
  TH1::AddDirectory(true);
  for(unsigned int i = 0; i < nBins+1; i++)
  {
    double newBinContent = rng.PoissonD(histo->GetBinContent(i));
    double newBinError = sqrt(newBinContent);
    result->SetBinContent(i, newBinContent);
    result->SetBinError(i, newBinError);
  }
  return result;
}



TH2D* PseudoExperiments::OscillateTH2(const TH2D* histo)
{
  std::string newName = histo->GetName();
  newName+="Oscillated";
  std::string newTitle = histo->GetTitle();
  newTitle+=" - Oscillated";
  unsigned int nBinsX = histo->GetXaxis()->GetNbins();
  unsigned int nBinsY = histo->GetYaxis()->GetNbins();
  TH1::AddDirectory(false);
  TH2D* result = Create2DHistogram<TH2D>(newName.c_str(),newTitle.c_str(),nBinsX,histo->GetXaxis()->GetXmin(),histo->GetXaxis()->GetXmax(),nBinsY,histo->GetYaxis()->GetXmin(),histo->GetYaxis()->GetXmax(),histo->GetXaxis()->GetTitle(),histo->GetYaxis()->GetTitle());
  TH1::AddDirectory(true);
  for(unsigned int ix = 0; ix < nBinsX+1; ix++)
  {
    for(unsigned int iy = 0; iy < nBinsY+1; iy++)
    {
      double newBinContent = rng.PoissonD(histo->GetBinContent(ix,iy));
      double newBinError = sqrt(newBinContent);
      result->SetBinContent(ix, iy, newBinContent);
      result->SetBinError(ix, iy, newBinError);
    }
  }
  return result;
}



void PseudoExperiments::BuildFitted1DIPHistos0thIter()
{
  std::vector<TFitResultPtr> fitResults;
  for(unsigned int i = 0; i < inputHistos.size(); i++)
  {
    double funcMin = 0.;
    double funcMax = axisMax;
    //     bool useGauss = true;
    unsigned short nGauss = 1;
    unsigned short nExp = 2;
    FindIPFitParameters(inputHistos[i], outputHistoNames[i], funcMin, funcMax, nGauss, nExp);
    
    std::cout << "Fitting histogram for " << outputHistoNames[i] << std::endl;
    std::cout << "Fit range is " << funcMin << " - " << funcMax << std::endl;
    std::string name = "h" + outputHistoNames[i] + "Iteration0";
    TH1D* histoResult;
    TF1* fitFunction;
    fitResults.push_back(FitIPDistribution(inputHistos[i], name, nGauss, nExp, funcMin, funcMax, histoResult, fitFunction, true));
    double sf = 1.;
    if(nEventsIPTemplates.size() == inputHistos.size())
    {
      sf = (double)nEventsIPTemplates[i]/histoResult->Integral();
    }
    histoResult->Scale(sf);
    fitted1DIPHistos0thIter.push_back(histoResult);
//     fitFunctions1DIP0thIter.push_back(fitFunction);
  }
}



void PseudoExperiments::BuildPseudoDataIPHisto0thIter()
{
//   if(use1D)
//   {
    MuonDistributionsBuilder::Build1DDistribution(pseudoData1DIPHisto0thIter);
    if(pseudoData1DIPHisto0thIter)
    {
      std::string name = "h" + outputHistoName1D + "Iteration0NoNorm";
      pseudoData1DIPHisto0thIter->SetName(name.c_str());
      if(useSmoothed0thIterHistograms)
      {
        pseudoData1DIPHisto0thIter->Smooth(1);
      }
      pseudoData1DIPHisto0thIter->Write();
      std::vector<std::string> labels;
      labels.push_back("Pseudo - 0th iteration");
      labels.push_back("Reference");
      LegendDefinition legend = {"", labels, "NE"};
      CreateAndWriteCanvasWithPulls("cPseudoData1DIPHisto0thIter","IP 1D pseudo data - 0th iteration", linestyles, markerstyles, colors, legend, false, true, pseudoData1DIPHisto0thIter, distribution1D, NULL);
    }
//   }
//   else if(use2D)
//   {
    MuonDistributionsBuilder::Build2DDistribution(pseudoData2DIPHisto0thIter);
    if(pseudoData2DIPHisto0thIter)
    {
      std::string name = "h" + outputHistoName2D + "Iteration0NoNorm";
      pseudoData2DIPHisto0thIter->SetName(name.c_str());
       if(useSmoothed0thIterHistograms)
      {
        // k3a applies the lightest smoothing possible (on a 3x3 cell square around the given bin)
        pseudoData2DIPHisto0thIter->Smooth(1, "k3a");
      }
      pseudoData2DIPHisto0thIter->Write();
      TH2D* hDifferenceIP2D0thIter = Create2DHistogram<TH2D>("hDifferenceIP2D0thIter","Difference between 2D IP 0th iteration pseudo data and reference data", pseudoData2DIPHisto0thIter->GetXaxis()->GetNbins(),pseudoData2DIPHisto0thIter->GetXaxis()->GetXmin(),pseudoData2DIPHisto0thIter->GetXaxis()->GetXmax(),pseudoData2DIPHisto0thIter->GetYaxis()->GetNbins(),pseudoData2DIPHisto0thIter->GetYaxis()->GetXmin(),pseudoData2DIPHisto0thIter->GetYaxis()->GetXmax(),pseudoData2DIPHisto0thIter->GetXaxis()->GetTitle(),pseudoData2DIPHisto0thIter->GetYaxis()->GetTitle());
      hDifferenceIP2D0thIter->Add(pseudoData2DIPHisto0thIter, distribution2D, 1, -1);
      CreateAndWriteCanvas("cDifferenceIP2D0thIter", "colz", false, false, true, hDifferenceIP2D0thIter);
    }
//   }
}


void PseudoExperiments::BuildIP1DTemplates(std::vector<TH1D*>& vTemplates)
{
  vTemplates.clear();
  for(unsigned int i = 0; i < fitted1DIPHistos0thIter.size(); i++)
  {
    std::string resultName = "h" + outputHistoNames[i];
    TH1D* histo = OscillateTH1(fitted1DIPHistos0thIter[i]);
    TH1D* resultHisto = Build1DTemplate(histo, resultName, false);
    vTemplates.push_back(resultHisto);
  }
}



void PseudoExperiments::BuildIP1DTemplatesFromFit(std::vector<TH1D*>& vTemplates)
{
  vTemplates.clear();
  for(unsigned int i = 0; i < fitted1DIPHistos0thIter.size(); i++)
  {
    TH1D* histo = OscillateTH1(fitted1DIPHistos0thIter[i]);
    double funcMin = 0.;
    double funcMax = axisMax;
    unsigned short nGauss = 1;
    unsigned short nExp = 2;
    FindIPFitParameters(histo, outputHistoNames[i], funcMin, funcMax, nGauss, nExp);
    std::string name = "h" + outputHistoNames[i];
    TH1D* histoFitted;
    TF1* fitFunction;
    FitIPDistribution(histo, name, nGauss, nExp, funcMin, funcMax, histoFitted, fitFunction, false, false);
    TH1D* histoResult = Build1DTemplateFromFit(histoFitted, name);
    vTemplates.push_back(histoResult);
  }
}



void PseudoExperiments::BuildIP2DTemplates(std::vector<TH2D*>& vTemplates)
{
  vTemplates.clear();
  std::vector<TH1D*> vOscillatedHistos;
  for(unsigned int i = 0; i < fitted1DIPHistos0thIter.size(); i++)
  {
    TH1::AddDirectory(false);
    TH1D* histo = OscillateTH1(fitted1DIPHistos0thIter[i]);
    TH1::AddDirectory(true);
    vOscillatedHistos.push_back(histo);
  }

  for(unsigned int ix = 0; ix < vOscillatedHistos.size(); ix++)
  {
    for(unsigned int iy = ix; iy < vOscillatedHistos.size(); iy++)
    {
      std::string name = "h" + outputHistoNames[ix] + outputHistoNames[iy];
      TH2D* resultHisto = Build2DTemplate(vOscillatedHistos[ix], vOscillatedHistos[iy], name, false);
      vTemplates.push_back(resultHisto);
    }
  }
  CleanVectorOfHistograms(vOscillatedHistos);
}



void PseudoExperiments::BuildIP2DTemplatesFromFit(std::vector<TH2D*>& vTemplates)
{
  vTemplates.clear();
  std::vector<TH1D*> vTemplates1DFromFit;
  BuildIP1DTemplatesFromFit(vTemplates1DFromFit);
  for(unsigned int ix = 0; ix < vTemplates1DFromFit.size(); ix++)
  {
    for(unsigned int iy = ix; iy < vTemplates1DFromFit.size(); iy++)
    {
      std::string name = "h" + outputHistoNames[ix] + outputHistoNames[iy];
      TH2D* result = Build2DTemplateFromFit(vTemplates1DFromFit[ix],vTemplates1DFromFit[iy],name);
      vTemplates.push_back(result);
    }
  }
  CleanVectorOfHistograms(vTemplates1DFromFit);
}



void PseudoExperiments::BuildReferences()
{
  if(use1D)
  {
    if(useIP)
    {
      if(useFittedTemplates)
      {
        Build1DTemplatesFromFit();
        AppendToTemplateNames("Reference");
        MuonTemplatesBuilder::CreateAndSavePrettyPlots();
        SaveTemplates();
      }
      else
      {
        Build1DTemplates();
        AppendToTemplateNames("Reference");
        MuonTemplatesBuilder::CreateAndSavePrettyPlots();
        SaveTemplates();
      }
    }
  }
  else if(use2D)
  {
    if(useFittedTemplates)
    {
      Build1DTemplatesFromFit();
      Build2DTemplatesFromFit();
      AppendToTemplateNames("Reference");
      MuonTemplatesBuilder::CreateAndSavePrettyPlots();
      SaveTemplates();
    }
    else
    {
      Build2DTemplates();
      AppendToTemplateNames("Reference");
      MuonTemplatesBuilder::CreateAndSavePrettyPlots();
      SaveTemplates();
    }
  }
  if(useIP)
  {
    MuonDistributionsBuilder::Build1DDistribution();
    MuonDistributionsBuilder::Build2DDistribution();
    AppendToDistributionNames("Reference");
    MuonDistributionsBuilder::CreateAndSavePrettyPlots();
    SaveDistributions();
  }
}



TFitterMinuit* PseudoExperiments::DoDefaultFit(std::string templateSuffix,std::string outputReferenceFileName)
{
  std::cout << "\n\n\nDoing default fit...\n";
  TFitterMinuit* tfm;
  double dataIntegral = 1;
  if(use1D)
  {
    std::vector<TH1D*> vTemplatesForFit;
    CreateTemplatesForFit(templates1D, vTemplatesForFit, templateSuffix);
    tfm = DoBinnedLikelihoodFit(vTemplatesForFit, distribution1D);

    if(outputReferenceFileName.size() > 0)
    {
      PlotFitResults(vTemplatesForFit, distribution1D, tfm, outputReferenceFileName);
    }
    dataIntegral = distribution1D->Integral();
  }
  else if(use2D)
  {
    std::vector<TH2D*> vTemplatesForFit;
    CreateTemplatesForFit(templates2D, vTemplatesForFit, templateSuffix);
    if(useConstrainedLikelihood)
      tfm = DoConstrainedBinnedLikelihoodFit(vTemplatesForFit, distribution2D);
    else
      tfm = DoBinnedLikelihoodFit(vTemplatesForFit, distribution2D);
   
    if(outputReferenceFileName.size() > 0)
    {
      PlotFitResults(vTemplatesForFit, distribution2D, tfm, outputReferenceFileName);
    }
    dataIntegral = distribution2D->Integral();
  }
  
  for(int iPar = 0; iPar < tfm->GetNumberTotalParameters(); iPar++)
  {
    defaultFractions.push_back(tfm->GetParameter(iPar) / dataIntegral);
    defaultFractionErrors.push_back(tfm->GetParError(iPar) / dataIntegral);
    std::string parName = tfm->GetParName(iPar);
    parameterNames.push_back(parName.erase(parName.size() - templateSuffix.size(), templateSuffix.size()));
  }

  return tfm;
}



void PseudoExperiments::ParseOptions(const std::string options)
{
  if(options.find("1") != std::string::npos)
  {
    use1D = true;
  }
  if(options.find("2") != std::string::npos)
  {
    use2D = true;
  } 
  if(use1D && use2D)
  {
    std::cout << "E R R O R ! PseudoExperiments::ParseOptions(...): options \"1\" and \"2\" are incompatible!\n";
    std::cout << "            Please use only one of them!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  if(options.find("I") != std::string::npos || options.find("i") != std::string::npos)
  {
    useIP = true;
  }
  if(options.find("P") != std::string::npos || options.find("p") != std::string::npos)
  {
    usePtRel = true;
    std::cout << "W A R N I N G ! PseudoExperiments::ParseOptions(...): PtRel pseudo-experiments are not yet implemented!\n";
    std::cout << "                Option \"P\" will have no effect!\n";
  }
  if(options.find("F") != std::string::npos || options.find("f") != std::string::npos)
  {
    useFittedTemplates = true;
  }
  if(options.find("C") != std::string::npos || options.find("c") != std::string::npos)
  {
    useConstrainedLikelihood = true;
  }
  if(options.find("S") != std::string::npos || options.find("s") != std::string::npos)
  {
    useSmoothed0thIterHistograms = true;
  }
  if(options.find("V") != std::string::npos || options.find("v") != std::string::npos)
  {
    saveValidationPlotsPerIteration = true;
  }
}



void PseudoExperiments::AppendToTemplateNames(const std::string suffix)
{
  if(use1D)
  {
    for(unsigned int i = 0; i < templates1D.size(); i++)
    {
      std::string oldName = templates1D[i]->GetName();
      std::string newName = oldName+=suffix;
      templates1D[i]->SetName(newName.c_str());
    }
  }
  else if(use2D)
  {
//     for(unsigned int i = 0; i < templates1DFromFit.size(); i++)
//     {
//       std::string oldName = templates1DFromFit[i]->GetName();
//       std::string newName = oldName+=suffix;
//       templates1DFromFit[i]->SetName(newName.c_str());
//     }
    for(unsigned int i = 0; i < templates1D.size(); i++)
    {
      std::string oldName = templates1D[i]->GetName();
      std::string newName = oldName+=suffix;
      templates1D[i]->SetName(newName.c_str());
    }
    for(unsigned int i = 0; i < templates2D.size(); i++)
    {
      std::string oldName = templates2D[i]->GetName();
      std::string newName = oldName+=suffix;
      templates2D[i]->SetName(newName.c_str());
    }
  }
}



void PseudoExperiments::AppendToDistributionNames(const std::string suffix)
{
  std::string oldName;
  std::string newName;
  if(distribution1D)
  {
    oldName = distribution1D->GetName();
    newName = oldName+=suffix;
    distribution1D->SetName(newName.c_str());
  } 
  if(distribution2D)
  {
    oldName = distribution2D->GetName();
    newName = oldName+=suffix;
    distribution2D->SetName(newName.c_str());
  }
}



void PseudoExperiments::DoPseudoExperiments(const std::string options)
{
  ParseOptions(options);

  BuildReferences();
//   std::cout << "Size of templates2D = " << templates2D.size() << std::endl;
  
  
  // Let's start the routine
  // Step 0: We create the smoothed templates in step 0 in any case, since they are the starting point for everything
  std::cout << "\n\n\nCreating 0th iteration fitted templates...\n";
  BuildFitted1DIPHistos0thIter();
  BuildPseudoDataIPHisto0thIter();
  TFitterMinuit* referenceTfm = DoDefaultFit("Reference","PseudoExperiments_ReferenceFit_prova.root");
  
  // Now we start the pseudoexperiment cycle
  outputFile->cd();

  for(unsigned int iPseudoExp = 1; iPseudoExp <= nPseudoExp; iPseudoExp++)
  {
    std::cout << "PSEUDOEXPERIMENTS: ITERATION #" << iPseudoExp << std::endl;
    
    TFitterMinuit* tfm;
    if(use1D)
    {
      std::vector<TH1D*> vTemplates;
      TH1D* pseudoData;
      if(useFittedTemplates)
      {
        BuildIP1DTemplatesFromFit(vTemplates);
      }
      else
      {
        BuildIP1DTemplates(vTemplates);
      }
      pseudoData = OscillateTH1(pseudoData1DIPHisto0thIter);
      
      std::vector<TH1D*> vTemplatesForFit;
      CreateTemplatesForFit(vTemplates, vTemplatesForFit);
      tfm = DoBinnedLikelihoodFit(vTemplatesForFit, pseudoData);
      
      if(saveValidationPlotsPerIteration && iPseudoExp <= 10)
      {
        Do1DValidationPlotsPerIteration(vTemplatesForFit, pseudoData, tfm, iPseudoExp);
      }
      
      DoPseudoExperimentsValidationPlots(vTemplatesForFit, pseudoData, tfm);
      
      CleanVectorOfHistograms(vTemplates);
    }
    else if(use2D)
    {
      std::vector<TH2D*> vTemplates;
      TH2D* pseudoData;
      if(useFittedTemplates)
      {
        BuildIP2DTemplatesFromFit(vTemplates);
      }
      else
      {
        BuildIP2DTemplates(vTemplates);
      }
      pseudoData = OscillateTH2(pseudoData2DIPHisto0thIter);
      
      std::vector<TH2D*> vTemplatesForFit;
      CreateTemplatesForFit(vTemplates, vTemplatesForFit);
      if(useConstrainedLikelihood)
        tfm = DoConstrainedBinnedLikelihoodFit(vTemplatesForFit, pseudoData);
      else
        tfm = DoBinnedLikelihoodFit(vTemplatesForFit, pseudoData);
      
      if(saveValidationPlotsPerIteration && iPseudoExp <= 10)
      {
        Do2DValidationPlotsPerIteration(vTemplatesForFit, pseudoData, tfm, iPseudoExp);
      }
      
      DoPseudoExperimentsValidationPlots(vTemplatesForFit, pseudoData, tfm);

      CleanVectorOfHistograms(vTemplates);
    }
  }
  std::cout << "Iterations over! Saving plots...\n";
  SavePseudoExperimentsValidationPlots();
}



TFitterMinuit* PseudoExperiments::DoConstrainedBinnedLikelihoodFit(std::vector<TH2D*> vTemplates, TH2D* pseudoData)
{
  double nEvents = pseudoData->Integral();
  
  BinnedLogLikelihood2DConstrained* likelihood = new BinnedLogLikelihood2DConstrained(vTemplates, pseudoData, vConstraintToComponent, vConstraintValues, vConstraintErrors, 0);
  TFitterMinuit *minFit = new TFitterMinuit;
  minFit->Clear();
  minFit->SetPrintLevel(-1);
  minFit->CreateMinimizer();
  minFit->SetMinuitFCN(likelihood);
  
  std::vector<double> par;
  std::vector<double> parError;
  
  for(unsigned int iTemplate = 0; iTemplate < vTemplates.size(); iTemplate++)
  {
    par.push_back(nEvents/vTemplates.size());
    parError.push_back(sqrt(nEvents)/vTemplates.size());
    
    std::string parName = "";
    std::string templateName = vTemplates[iTemplate]->GetName();
    parName+=templateName.erase(0,1);
    minFit->SetParameter(iTemplate,parName.c_str(),par[iTemplate],parError[iTemplate],0.,nEvents);
  }
  minFit->Minimize(1000,0.0000001);
  
  std::cout << "---- MINIMIZATION RESULTS ----" << std::endl;
  minFit->PrintResults(4,0.);
  
  return minFit;
}



// vTemplates and pseudoData cannot be const... Please explain me why ROOT needs to modify an object just to paint it! (MG)
void PseudoExperiments::Do1DValidationPlotsPerIteration(std::vector<TH1D*> vTemplates, TH1D* pseudoData, TFitterMinuit* tfm, const unsigned int iPseudoExp)
{

  oss.str("");
  oss << iPseudoExp;
  std::vector<std::string> labels;
  labels.push_back("Pseudo - iteration " + oss.str());
  labels.push_back("Reference");
  LegendDefinition legend = {"", labels, "NE"};
  for(unsigned int iTemplate = 0; iTemplate < vTemplates.size(); iTemplate++)
  {
    std::string canvasName = "c";
    canvasName+=vTemplates[iTemplate]->GetName();
    canvasName+=oss.str();
    CreateAndWriteCanvasWithPulls(canvasName.c_str(), linestyles, markerstyles, colors, legend, false, true, vTemplates[iTemplate], templates1D[iTemplate], NULL);
  }
  std::string canvasName = "c";
  canvasName+=outputHistoName1D;
  canvasName+=oss.str();
  CreateAndWriteCanvasWithPulls(canvasName.c_str(), linestyles, markerstyles, colors, legend, false, true, pseudoData, distribution1D, NULL);
  
  std::string fileName = "PseudoExperiments_1DFitResults_iteration" + oss.str() + ".root";
  PlotFitResults(vTemplates, pseudoData, tfm, fileName);
}



// vTemplates and pseudoData cannot be const... Please explain me why ROOT needs to modify an object just to paint it! (MG)
void PseudoExperiments::Do2DValidationPlotsPerIteration(std::vector<TH2D*> vTemplates, TH2D* pseudoData, TFitterMinuit* tfm, unsigned int iPseudoExp)
{
  for(unsigned int iTemplate = 0; iTemplate < vTemplates.size(); iTemplate++)
  {
    oss.str("");
    std::string canvasName = "c";
    oss << iPseudoExp;
    canvasName+=vTemplates[iTemplate]->GetName();
    canvasName+=oss.str();
    CreateAndWriteCanvas3D(canvasName.c_str(), false, false, true, 230., 35., vTemplates[iTemplate]);
  }
  std::string canvasName = "c";
  canvasName+=outputHistoName2D;
  canvasName+=oss.str();
  CreateAndWriteCanvas3D(canvasName.c_str(), false, false, true, 230., 35., pseudoData);

  std::string fileName = "PseudoExperiments_2DFitResults_iteration" + oss.str() + ".root";
  PlotFitResults(vTemplates, pseudoData, tfm, fileName);
}



void PseudoExperiments::SavePseudoExperimentsValidationPlots()
{
  outputFile->cd();
  for(unsigned int iPar = 0; iPar < fractionsPseudoExp.size(); iPar++)
  {
    fractionsPseudoExp[iPar]->Write();
    std::string canvasName = "c" + parameterNames[iPar] + "FractionsPseudoExp";
    CreateAndWriteCanvas(canvasName.c_str(), 0, 21, 1, false, false, fractionsPseudoExp[iPar]);
  }
  for(unsigned int iPar = 0; iPar < fractionsVsFractionsPseudoExp.size(); iPar++)
  {
    for(unsigned int jPar = 0; jPar < fractionsVsFractionsPseudoExp[iPar].size(); jPar++)
    {
      fractionsVsFractionsPseudoExp[iPar][jPar]->Write();
      std::string canvasName = "c" + parameterNames[iPar] + "FractionVs" + parameterNames[jPar] + "PseudoExp";
      CreateAndWriteCanvas(canvasName.c_str(), "colz", false, false, false, fractionsVsFractionsPseudoExp[iPar][jPar]);
    }
  }
  
  for(unsigned int iPar = 0; iPar < fractionErrorsPseudoExp.size(); iPar++)
  {
    fractionErrorsPseudoExp[iPar]->Write();
    std::string canvasName = "c" + parameterNames[iPar] + "FractionErrorsPseudoExp";
    CreateAndWriteCanvas(canvasName.c_str(), 0, 21, 1, false, false, fractionErrorsPseudoExp[iPar]);
  }
  for(unsigned int iPar = 0; iPar < fractionsVsErrorsPseudoExp.size(); iPar++)
  {
    fractionsVsErrorsPseudoExp[iPar]->Write();
    std::string canvasName = "c" + parameterNames[iPar] + "FractionsVsErrorsPseudoExp";
    CreateAndWriteCanvas(canvasName.c_str(), "colz", false, false, false, fractionsVsErrorsPseudoExp[iPar]);
  }
  for(unsigned int iPar = 0; iPar < fractionsPseudoExpDifferences.size(); iPar++)
  {
    fractionsPseudoExpDifferences[iPar]->Write();
    std::string canvasName = "c" + parameterNames[iPar] + "FractionPseudoExpDifference";
    CreateAndWriteCanvas(canvasName.c_str(), 0, 21, 1, false, false, fractionsPseudoExpDifferences[iPar]);
  }
  for(unsigned int iPar = 0; iPar < fractionsPseudoExpPulls.size(); iPar++)
  {
    fractionsPseudoExpPulls[iPar]->Write();
    std::string canvasName = "c" + parameterNames[iPar] + "FractionPseudoExpPull";
    CreateAndWriteCanvas(canvasName.c_str(), 0, 21, 1, false, false, fractionsPseudoExpPulls[iPar]);
  }
}

#endif // PseudoExperiments_cxx
