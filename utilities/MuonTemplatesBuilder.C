#ifndef MuonTemplatesBuilder_cxx
#define MuonTemplatesBuilder_cxx

#include "MuonTemplatesBuilder.h"



MuonTemplatesBuilder::~MuonTemplatesBuilder()
{
  inputFile->Close();
  if(useSeparatePrompt)
  {
    inputFilePrompt->Close();
  }
  outputFile->Close();
}



void MuonTemplatesBuilder::Initialize()
{

  std::cout << "I N F O: Set the axis label according to IP and PtRel/PtOut choice" << std::endl;
  std::cout << "         This may be not \"clean\" ..." << std::endl;

  if (inputHistoNames.size() > 0)
  {
    if (inputHistoNames[0].find("hMuonDxy") != std::string::npos)
    {
      axislabel1D = " cm";
      axislabel2D = " cm^{2}";
      titlelabel2D = " impact parameter";
    }
    else if (inputHistoNames[0].find("hMuonPtRel") != std::string::npos)
    {
      axislabel1D = " GeV/c";
      axislabel2D = " (GeV/c)^{2}";
      titlelabel2D = " p_{T}^{Rel}";
    }
    else if (inputHistoNames[0].find("hMuonPtOut") != std::string::npos)
    {
      axislabel1D = " GeV/c";
      axislabel2D = " (GeV/c)^{2}";
      titlelabel2D = " p_{T}^{Out}";
    }
    else
    {
      std::cout << "W A R N I N G ! No input histos name match" << std::endl;
    }
  }
  else
  {
    std::cout << "W A R N I N G ! No input histos!" << std::endl;
  }

  if(inputFileNamePrompt.compare("") == 0)
  {
    useSeparatePrompt = false;
  }
  else
  {
    useSeparatePrompt = true;
  }
  
  inputFile = new TFile(inputFileName.c_str(),"READ");
  std::cout << "I N F O: MuonTemplatesBuilder::Initialize() - Using input file:\n\t" << inputFileName << std::endl;
  for(unsigned int i = 0; i < inputHistoNames.size(); i++)
  {
    TH1D* tempHisto = 0;
    inputFile->GetObject(inputHistoNames[i].c_str(),tempHisto);
    if(tempHisto)
    {
      inputHistos.push_back(tempHisto);
    }
    else
    {
      std::cout << "E R R O R! MuonTemplatesBuilder::Initialize : no histogram with name " << inputHistoNames[i] << " found in input file!\n";
      exit(1);
    }
  }
  
  if(useSeparatePrompt)
  {
    inputFilePrompt = new TFile(inputFileNamePrompt.c_str(),"READ");
    std::cout << "I N F O: MuonTemplatesBuilder::Initialize() - Using input file for prompt:\n\t" << inputFileNamePrompt << std::endl;
    TH1D* tempHisto = 0;
    inputFilePrompt->GetObject(inputHistoNamePrompt.c_str(),tempHisto);
    if(tempHisto)
    {
      inputHistos.push_back(tempHisto);
    }
    else
    {
      std::cout << "E R R O R! MuonTemplatesBuilder::Initialize : no histogram with name " << inputHistoNamePrompt << " found in input file for prompt!\n";
      exit(1);
    }
    
    if(outputHistoNames.at(outputHistoNames.size()-1).compare("P") != 0)
    {
      std::cout << "E R R O R ! When a separate prompt file is used, the last output histo name must be \"P\"!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }
  
  fitFunctions.clear();
  templates1DFromFit.clear();
  templates1D.clear();
  templates2D.clear();
  distributions1D.clear();
  distributions2D.clear();
  
  outputFile->cd();
}



// TFitResultPtr MuonTemplatesBuilder::FitIPDistribution(TH1D* histo, const std::string name, const bool useGauss, unsigned short nExpos, const double funcMin, const double funcMax)
TFitResultPtr MuonTemplatesBuilder::FitIPDistribution(TH1D* histo, const std::string name, unsigned short nGauss, unsigned short nExpos, const double funcMin, const double funcMax)
{
  TH1D* histoResult;
  TF1* fitFunction;
  TFitResultPtr fitResult = FitIPDistribution(histo, name, nGauss, nExpos, funcMin, funcMax, histoResult, fitFunction, true, true);

  fitFunctions.push_back(fitFunction);
  templates1DFromFit.push_back(histoResult);
  if(fitResult==0)
  {
    return fitResult;
  }
  else
  {
    std::cout << "W A R N I N G! MuonTemplatesBuilder::FitIPDistribution() - Fit did not converge!\n";
    return fitResult;
  }
}



TFitResultPtr MuonTemplatesBuilder::FitIPDistribution(TH1D* histo, const std::string name, unsigned short nGauss, unsigned short nExpos,const double funcMin, const double funcMax, TH1D*& histoResult, TF1*& fitFunction, bool produceValidationPlots, bool doRebin)
{
  if(nExpos>3)
  {
    std::cout << "W A R N I N G! MuonTemplatesBuilder::FitIPDistribution() - Too many exponentials! Using 3.\n";
    nExpos=3;
  }
  if(nGauss>3)
  {
    std::cout << "W A R N I N G! MuonTemplatesBuilder::FitIPDistribution() - Too many gaussians! Using 3.\n";
    nGauss=3;
  }
  TH1D* histoPrepared = PrepareHistogramForFit(histo, doRebin);
  std::string xTitle = histo->GetXaxis()->GetTitle();
  std::string yTitle = histo->GetYaxis()->GetTitle();
  std::string histoName = name + "NoNorm";
  histoPrepared->SetName(histoName.c_str());
  double integral = 0;
  int iStartBin = histoPrepared->FindFixBin(axisMin);
  int iEndBin = histoPrepared->FindFixBin(axisMax);
  
  for(int iBin = iStartBin; iBin <= iEndBin; iBin++)
    integral+=histoPrepared->GetBinContent(iBin);
//   integral = 0;
  std::string functionDefinition = "";
  unsigned int nPars=0;
  std::vector<std::string> parNames;
  //   if(useGauss)
  for(unsigned int iGauss=0; iGauss<nGauss; iGauss++)
  {
    oss.str("");
    if(iGauss==0)
      oss << "gaus(" << nPars << ")";
    else
      oss << "+gaus(" << nPars << ")";
    functionDefinition+=oss.str();
    oss.str("");
    oss << "g" << iGauss+1 << "Norm";
    parNames.push_back(oss.str());
    oss.str("");
    oss << "g" << iGauss+1 << "Mean";
    parNames.push_back(oss.str());
    oss.str("");
    oss << "g" << iGauss+1 << "Sigma";
    parNames.push_back(oss.str());
    nPars+=3;
  }
  for(unsigned int iExpo=0; iExpo<nExpos; iExpo++)
  {
    oss.str("");
    if(iExpo==0 && nGauss == 0)
      oss << "expo(" << nPars << ")";
    else
      oss << "+expo(" << nPars << ")";
    functionDefinition+=oss.str();
    oss.str("");
    oss << "e" << iExpo+1 << "Norm";
    parNames.push_back(oss.str());
    oss.str("");
    oss << "e" << iExpo+1 << "Slope";
    parNames.push_back(oss.str());
    nPars+=2;
  }
  fitFunction = new TF1("fitFunction",functionDefinition.c_str(),funcMin,funcMax);
  if(produceValidationPlots)
  {
    std::cout << "axisMin = " << axisMin << ", axisMax = " << axisMax << ", integral = " << integral << std::endl;
    std::cout << "funcMin = " << funcMin << ", funcMax = " << funcMax << std::endl;
    std::cout << "functionDefinition = " << functionDefinition << std::endl;
    std::cout << "nPars = " << nPars << std::endl;
  }  
  for(unsigned int iPar=0; iPar < nPars; iPar++)
  {
    fitFunction->SetParName(iPar,parNames[iPar].c_str());
  }
  unsigned int iPar=0;
  //   if(useGauss)
  for(unsigned int iGauss=0; iGauss < nGauss; iGauss++)
  {
    fitFunction->SetParLimits(3*iGauss,0,1e5);
//     std::cout << "integral*(iGauss+1)/10 = " << integral*(iGauss+1)/10 << std::endl;
    fitFunction->SetParameter(3*iGauss,integral*(iGauss+1)/10);
    fitFunction->FixParameter(3*iGauss+1,0);
    fitFunction->SetParLimits(3*iGauss+2,0,0.1);
    fitFunction->SetParameter(3*iGauss+2,(0.01*(iGauss*2+1)));
    
    /*    
     *    fitFunction->SetParLimits(0,0,1e5);
     *    fitFunction->SetParameter(0,integral/10);
     *    fitFunction->FixParameter(1,0);
     *    fitFunction->SetParLimits(2,0,0.1);
     *    fitFunction->SetParameter(2,0.01);*/
    iPar+=3;
  }
  for(unsigned int iExpo=0; iExpo < nExpos; iExpo++)
  {
    fitFunction->SetParLimits(2*iExpo+iPar,0.,1e6);
    fitFunction->SetParameter(2*iExpo+iPar,2*(iExpo+1));
    fitFunction->SetParLimits(2*iExpo+iPar+1,-1000,0.);
    fitFunction->SetParameter(2*iExpo+iPar+1,-5*(iExpo+1));
  }
  std::string fitOptions = produceValidationPlots?"LL":"LLQ";
  TFitResultPtr fitResult = histoPrepared->Fit("fitFunction",fitOptions.c_str(),"",funcMin,funcMax);
  unsigned int nBins = (unsigned int)( (axisMax-axisMin)/histoPrepared->GetBinWidth(1) );
  histoResult = CreateTH1FromTF1(fitFunction,(name+"FromTF1"),nBins,axisMin,axisMax,xTitle,yTitle);
  if(produceValidationPlots)
  {
    std::string canvasName = "c" + histoName.substr(1);
    unsigned int lineStyles[2] = {0,1};
    unsigned int markerStyles[2] = {20,0};
    unsigned int colors[2] = {2,1};
    std::string legendTitle = "Fit result for " + histoName.substr(1);
    std::vector<std::string> legendLabels;
    legendLabels.push_back("Histogram");
    legendLabels.push_back("Fit function");
    LegendDefinition leg = {legendTitle,legendLabels,"NE"};
    outputFile->cd();
    double chi2 = 0;
    int nPoints = 0;
    for(int iBin=histoResult->FindFixBin(funcMin); iBin < histoResult->FindFixBin(funcMax); iBin++)
    { 
      //     std::cout << "iBin = " << iBin << std::endl;
      double error = histoPrepared->GetBinError(iBin);
      double value = (histoPrepared->GetBinContent(iBin) - histoResult->GetBinContent(iBin))/error;
      //     std::cout << "value = " << value << ", error = " << error << std::endl;
      if(error <= 0)
        continue;
      if(value != value)
        continue;
      //     std::cout << "value taken\n";
      chi2 += value*value;
      nPoints++;
    }
    int ndof = nPoints - nPars;
    std::cout << "n fitted points: " << nPoints << std::endl;
    std::cout << "ndof = " << ndof << std::endl;
    std::cout << "chi2 = " << chi2 << std::endl;
    std::cout << "chi2/ndof = " << chi2/ndof << std::endl;
    canvasName = "c" + histoName.substr(1)+"Diff";
    CreateAndWriteCanvasWithDifferences(canvasName.c_str(),lineStyles,markerStyles,colors,leg,false,true,histoPrepared,histoResult,NULL);
    canvasName = "c" + histoName.substr(1)+"Ratios";
    CreateAndWriteCanvasWithRatios(canvasName.c_str(),lineStyles,markerStyles,colors,leg,false,true,histoPrepared,histoResult,NULL);
    canvasName = "c" + histoName.substr(1)+"Pulls";
    CreateAndWriteCanvasWithPulls(canvasName.c_str(),lineStyles,markerStyles,colors,leg,false,true,histoPrepared,histoResult,NULL);
    histoPrepared->Write();
    histoResult->Write();
  }
  return fitResult;
}



void MuonTemplatesBuilder::Build1DTemplates()
{
  for(unsigned int i = 0; i < inputHistos.size(); i++)
  {
    std::string name = "h" + outputHistoNames[i];
    TH1D* result = Build1DTemplate(inputHistos[i],name);
    templates1D.push_back(result);
  }
}



void MuonTemplatesBuilder::Build1DTemplatesFromFit()
{
  std::vector<TFitResultPtr> fitResults;
  for(unsigned int i = 0; i < inputHistos.size(); i++)
  {
    double funcMin = 0.;
    double funcMax = axisMax;
    unsigned short nGauss = 1;
    unsigned short nExp = 2;
    FindIPFitParameters(inputHistos[i], outputHistoNames[i], funcMin, funcMax, nGauss, nExp);
    
    std::cout << "Fitting histogram for " << outputHistoNames[i] << std::endl;
    std::cout << "Fit range is " << funcMin << " - " << funcMax << std::endl;
    std::string name = "h" + outputHistoNames[i];
    fitResults.push_back(FitIPDistribution(inputHistos[i],name,nGauss,nExp,funcMin,funcMax));
    TH1D* result = Build1DTemplateFromFit(templates1DFromFit[i],name);
    templates1D.push_back(result);
  }
}



// Backwards compatibility
void MuonTemplatesBuilder::Build1DIPDistributions()
{
  std::cout << "W A R N I N G: MuonTemplatesBuilder::Build1DIPDistributions() is deprecated!\n";
  std::cout << "               Please use MuonTemplatesBuilder::Build1DDistributions() instead.\n";
  Build1DDistributions();
}



void MuonTemplatesBuilder::Build1DDistributions()
{
  for(unsigned int i = 0; i < inputHistos.size(); i++)
  {
    std::string name = "h" + outputHistoNames[i] + "NoNorm";
    TH1D* result = Build1DDistribution(inputHistos[i],name);
    distributions1D.push_back(result);
  }
}



void MuonTemplatesBuilder::Build2DTemplates()
{
  for(unsigned int ix = 0; ix < inputHistos.size(); ix++)
  {
    for(unsigned int iy = ix; iy < inputHistos.size(); iy++)
    {
      std::string name = "h" + outputHistoNames[ix] + outputHistoNames[iy];
      TH2D* result = Build2DTemplate(inputHistos[ix],inputHistos[iy],name);
      templates2D.push_back(result);
    }
  }
}



void MuonTemplatesBuilder::Build2DTemplatesFromFit()
{
  for(unsigned int ix = 0; ix < templates1DFromFit.size(); ix++)
  {
    for(unsigned int iy = ix; iy < templates1DFromFit.size(); iy++)
    {
      std::string name = "h" + outputHistoNames[ix] + outputHistoNames[iy];
      TH2D* result = Build2DTemplateFromFit(templates1DFromFit[ix],templates1DFromFit[iy],name);
      templates2D.push_back(result);
    }
  }
}



// Backwards compatibility
void MuonTemplatesBuilder::Build2DIPDistributions()
{
  std::cout << "W A R N I N G: MuonTemplatesBuilder::Build2DIPDistributions() is deprecated!\n";
  std::cout << "               Please use MuonTemplatesBuilder::Build2DDistributions() instead.\n";
  Build2DDistributions();
}



void MuonTemplatesBuilder::Build2DDistributions()
{
  for(unsigned int ix = 0; ix < inputHistos.size(); ix++)
  {
    for(unsigned int iy = ix; iy < inputHistos.size(); iy++)
    {
      std::string name = "h" + outputHistoNames[ix] + outputHistoNames[iy] + "NoNorm";
      TH2D* result = Build2DDistribution(inputHistos[ix],inputHistos[iy],name);
      distributions2D.push_back(result);
    }
  }
}



TH1D* MuonTemplatesBuilder::Build1DTemplate(TH1D* histo, std::string name, const bool doRebin) /// FIXME ///
{

  const std::string xTitle = histo->GetXaxis()->GetTitle();
  TH1D* histoNormalized = PrepareAndNormalizeHistogram(histo, doRebin);
  const double xBinWidth = histoNormalized->GetXaxis()->GetBinWidth(1);
  const double xMin = axisMin;
  const double xMax = axisMax;
  const unsigned int xBins = histoNormalized->GetXaxis()->FindBin(axisMax) - histoNormalized->GetXaxis()->FindBin(axisMin);

  TH1D* resultHisto = new TH1D(name.c_str(),"",xBins,xMin,xMax);
  resultHisto->GetXaxis()->SetTitle(xTitle.c_str());
  oss.str("");
  oss << "N_{#mu} / " << xBinWidth << axislabel1D.c_str();
  std::string yTitle = oss.str();
  resultHisto->GetYaxis()->SetTitle(yTitle.c_str());
  
  int offset = 0;
  if(axisMin > histoNormalized->GetBinLowEdge(1))
  {
    offset = histoNormalized->FindBin(axisMin) - 1;
  }
      
  for(unsigned int i = 1+offset/*, j = 1*/; i < xBins+1+offset; i++/*, j++*/)
  {
    double binContent = histoNormalized->GetBinContent(i);
    double binError = histoNormalized->GetBinError(i);
    resultHisto->SetBinContent(i-offset,binContent);
    resultHisto->SetBinError(i-offset,binError);
  }

  std::cout << Form(" histo name %s resultHisto Template[%s], offset %d\n", histo->GetName(), name.c_str(), offset);
//   for (unsigned int ibin = 1; ibin < xBins+1 /*10*/; ++ibin){
//     std::cout << Form("bin %d [%f,%f]",ibin,resultHisto->GetBinLowEdge(ibin),resultHisto->GetBinLowEdge(ibin)+xBinWidth) << " content : " << resultHisto->GetBinContent(ibin) << std::endl;
//   }
  std::cout << "Integral " <<  resultHisto->Integral() << std::endl;
  std::cout << "Entries  " <<  resultHisto->GetEntries() << std::endl;

  return resultHisto;
}

/* ORIGINAL VERSION */ /*
{
//   const unsigned int xBins = histo->GetXaxis()->GetNbins();
//   const double xMin = histo->GetXaxis()->GetXmin();
//   const double xMax = histo->GetXaxis()->GetXmax();
  const std::string xTitle = histo->GetXaxis()->GetTitle();
  TH1D* histoNormalized = PrepareAndNormalizeHistogram(histo, doRebin);
  const double xBinWidth = histoNormalized->GetXaxis()->GetBinWidth(1);
  const double xMin = axisMin;
  const double xMax = axisMax;
  const unsigned int xBins = histoNormalized->GetXaxis()->FindBin(axisMax) - histoNormalized->GetXaxis()->FindBin(axisMin);
  // std::cout << "xBinWidth = " << xBinWidth << ", xMin = " << xMin << ", xMax = " << xMax << ", xBins = " << xBins << std::endl;
//   outputFile->cd();
//   histoNormalized->Write();
  TH1D* resultHisto = new TH1D(name.c_str(),"",xBins,xMin,xMax);
  resultHisto->GetXaxis()->SetTitle(xTitle.c_str());
  oss.str("");
  oss << "N_{#mu} / " << xBinWidth << axislabel1D.c_str();
  std::string yTitle = oss.str();
  resultHisto->GetYaxis()->SetTitle(yTitle.c_str());

  
  int offset = 0;
  if(axisMin > histoNormalized->GetBinLowEdge(1))
  {
    offset = histoNormalized->FindBin(axisMin) - 1;
  }
  
  for(unsigned int i = offset; i < xBins+1+offset; i++)
  {
    double binContent = histoNormalized->GetBinContent(i);
    double binError = histoNormalized->GetBinError(i);
//     std::cout << "binContent = " << binContent << ", binError = " << binError << std::endl;
    resultHisto->SetBinContent(i,binContent);
    resultHisto->SetBinError(i,binError);
  }

  return resultHisto;
}
*/


TH1D* MuonTemplatesBuilder::Build1DTemplateFromFit(TH1D* histo, std::string name)
{
  const std::string xTitle = histo->GetXaxis()->GetTitle();
  TH1D* histoNormalized = NormalizeHistogram(histo);
  const double xBinWidth = histoNormalized->GetXaxis()->GetBinWidth(1);
  const double xMin = axisMin;
  const double xMax = axisMax;
  const unsigned int xBins = histoNormalized->GetXaxis()->FindBin(axisMax) - histoNormalized->GetXaxis()->FindBin(axisMin);
  // std::cout << "xBinWidth = " << xBinWidth << ", xMin = " << xMin << ", xMax = " << xMax << ", xBins = " << xBins << std::endl;
  TH1D* resultHisto = new TH1D(name.c_str(),"",xBins,xMin,xMax);
  resultHisto->GetXaxis()->SetTitle(xTitle.c_str());
  oss.str("");
  oss << "N_{#mu} / " << xBinWidth << axislabel1D.c_str();
  std::string yTitle = oss.str();
  resultHisto->GetYaxis()->SetTitle(yTitle.c_str());
  
  int offset = 0;
  if(axisMin > histoNormalized->GetBinLowEdge(1))
  {
    offset = histoNormalized->FindBin(axisMin) - 1;
  }
    
  for(unsigned int i = 1+offset; i < xBins+1+offset; i++)
  {
    double binContent = histoNormalized->GetBinContent(i);
    double binError = histoNormalized->GetBinError(i);
    // std::cout << "binContent = " << binContent << ", binError = " << binError << std::endl;
    resultHisto->SetBinContent(i-offset,binContent);
    resultHisto->SetBinError(i-offset,binError);
  }

  return resultHisto;
}



// Backwards compatibility
TH1D* MuonTemplatesBuilder::Build1DIPDistribution(TH1D* histo, std::string name, const bool doRebin)
{
  std::cout << "W A R N I N G: MuonTemplatesBuilder::Build1DIPDistribution(TH1D*, std::string, const bool) is deprecated!\n";
  std::cout << "               Please use MuonTemplatesBuilder::Build1DDistribution(TH1D*, std::string, const bool) instead.\n";
  return Build1DDistribution(histo, name, doRebin);
}



TH1D* MuonTemplatesBuilder::Build1DDistribution(TH1D* histo, std::string name, const bool doRebin)
{
  //   const unsigned int xBins = histo->GetXaxis()->GetNbins();
  //   const double xMin = histo->GetXaxis()->GetXmin();
  //   const double xMax = histo->GetXaxis()->GetXmax();
  const std::string xTitle = histo->GetXaxis()->GetTitle();
  TH1D* histoPrepared = PrepareHistogram(histo, doRebin);
  const double xBinWidth = histoPrepared->GetXaxis()->GetBinWidth(1);
  const double xMin = axisMin;
  const double xMax = axisMax;
  const unsigned int xBins = histoPrepared->GetXaxis()->FindBin(axisMax) - histoPrepared->GetXaxis()->FindBin(axisMin);
  // std::cout << "xBinWidth = " << xBinWidth << ", xMin = " << xMin << ", xMax = " << xMax << ", xBins = " << xBins << std::endl;
  TH1D* resultHisto = new TH1D(name.c_str(),"",xBins,xMin,xMax);
  resultHisto->GetXaxis()->SetTitle(xTitle.c_str());
  oss.str("");
  oss << "N_{#mu} / " << xBinWidth << axislabel1D.c_str();
  std::string yTitle = oss.str();
  resultHisto->GetYaxis()->SetTitle(yTitle.c_str());

  for(unsigned int i = 0; i < xBins+1; i++)
  {
    double binContent = histoPrepared->GetBinContent(i);
    double binError = histoPrepared->GetBinError(i);
//     std::cout << "binContent = " << binContent << ", binError = " << binError << std::endl;
    resultHisto->SetBinContent(i,binContent);
    resultHisto->SetBinError(i,binError);
  }

  return resultHisto;
}



TH2D* MuonTemplatesBuilder::Build2DTemplate(TH1D* xHisto, TH1D* yHisto, std::string name, const bool doRebin)
{
  const unsigned int xhBins = xHisto->GetXaxis()->GetNbins();
  const double xhMin = xHisto->GetXaxis()->GetXmin();
  const double xhMax = xHisto->GetXaxis()->GetXmax();
  const std::string xTitle = xHisto->GetXaxis()->GetTitle();
  const unsigned int yhBins = yHisto->GetXaxis()->GetNbins();
  const double yhMin = yHisto->GetXaxis()->GetXmin();
  const double yhMax = yHisto->GetXaxis()->GetXmax();
  const std::string yTitle = yHisto->GetXaxis()->GetTitle();

  if(xhBins != yhBins || xhMin != yhMin || xhMax != yhMax)
  {
    std::cout << "E R R O R! MuonTemplatesBuilder::Build2DTemplate: x and y have different axes!\n";
    exit(1);
  }

  TH1D* xHistoNormalized = PrepareAndNormalizeHistogram(xHisto, doRebin);
  TH1D* yHistoNormalized = PrepareAndNormalizeHistogram(yHisto, doRebin);
  const double xBinWidth = xHistoNormalized->GetXaxis()->GetBinWidth(1);
//   const double xMin = xHistoNormalized->GetXaxis()->GetBinLowEdge(axisMin);
//   const double xMax = xHistoNormalized->GetXaxis()->GetBinUpEdge(axisMax);
  const double xMin = axisMin;
  const double xMax = axisMax;

//   const unsigned int xBins = xHistoNormalized->GetXaxis()->FindBin(xMax-xBinWidth) - xHistoNormalized->GetXaxis()->FindBin(xMin);
  const unsigned int xBins = xHistoNormalized->GetXaxis()->FindBin(axisMax) - xHistoNormalized->GetXaxis()->FindBin(axisMin);
//   const double yBinWidth = yHistoNormalized->GetXaxis()->GetBinWidth(1);
  const double yMin = axisMin;
  const double yMax = axisMax;
  //   const double yMin = yHistoNormalized->GetXaxis()->GetBinLowEdge(axisMin);
//   const double yMax = yHistoNormalized->GetXaxis()->GetBinUpEdge(axisMax);
  const unsigned int yBins = yHistoNormalized->GetXaxis()->FindBin(axisMax) - yHistoNormalized->GetXaxis()->FindBin(axisMin);

  // std::cout << "xBinWidth = " << xBinWidth << ", xMin = " << xMin << ", xMax = " << xMax << ", xBins = " << xBins << std::endl;
  // std::cout << "yBinWidth = " << yBinWidth << ", yMin = " << yMin << ", yMax = " << yMax << ", yBins = " << yBins << std::endl;

//   outputFile->cd();
//   xHistoNormalized->Write();
//   yHistoNormalized->Write();

  if(xBins != yBins || xMin != yMin || xMax != yMax)
  {
    std::cout << "E R R O R! MuonTemplatesBuilder::Build2DTemplate: x and y have different axes after rebin!\n";
    exit(1);
  }

  TH2D* resultHisto = new TH2D(name.c_str(),"",xBins,xMin,xMax,yBins,yMin,yMax);
  resultHisto->GetXaxis()->SetTitle(xTitle.c_str());
  resultHisto->GetYaxis()->SetTitle(yTitle.c_str());
  oss.str("");
  oss << "N_{#mu} / (" << xBinWidth << " #times " << xBinWidth << ")"<< axislabel2D.c_str();
  std::string zTitle = oss.str();
  resultHisto->GetZaxis()->SetTitle(zTitle.c_str());

  unsigned int offset = 0;
  if(axisMin > xHistoNormalized->GetBinLowEdge(1))
  {
    offset = xHistoNormalized->FindBin(axisMin) - 1;
  }
  
  for(unsigned int i = 1+offset; i < xBins+1+offset; i++)
  {
    for(unsigned int j = 1+offset; j < yBins+1+offset; j++)
    {
      if(i <= removeBin+offset && j <= removeBin+offset)
      {
        resultHisto->SetBinContent(i-offset,j-offset,0.);
        resultHisto->SetBinError(i-offset,j-offset,0.);
        continue;
      }

      double xi = xHistoNormalized->GetBinContent(i);
      double xj = xHistoNormalized->GetBinContent(j);
      double yi = yHistoNormalized->GetBinContent(i);
      double yj = yHistoNormalized->GetBinContent(j);
      double xErri = xHistoNormalized->GetBinError(i);
      double xErrj = xHistoNormalized->GetBinError(j);
      double yErri = yHistoNormalized->GetBinError(i);
      double yErrj = yHistoNormalized->GetBinError(j);
//       double random = rng.Rndm();
      double binContent, binError;
//       if(random > 0.5)
//       {
//         binContent = xi * yj;
//         if(xi != 0 && yj != 0)
//           binError = sqrt((xErri/xi)*(xErri/xi) + (yErrj/yj)*(yErrj/yj))*binContent;
//         else
//           binError = 0;
//       }
//       else
//       {
//         binContent = xj * yi;
//         if(xj != 0 && yi != 0)
//           binError = sqrt((xErrj/xj)*(xErrj/xj) + (yErri/yi)*(yErri/yi))*binContent;
//         else
//           binError = 0;
//       }
      double ij = xi * yj;
      double ijErr;
      if(ij != 0)
        ijErr = sqrt((xErri/xi)*(xErri/xi) + (yErrj/yj)*(yErrj/yj))*ij;
      else
        ijErr = 0;
      double ji = xj * yi;
      double jiErr;
      if(ji != 0)
        jiErr = sqrt((xErrj/xj)*(xErrj/xj) + (yErri/yi)*(yErri/yi))*ji;
      else
        jiErr = 0;
      /*double */binContent = (ij + ji) / 2;
      /*double */binError = sqrt(ijErr*ijErr + jiErr*jiErr) / 2;
//       std::cout << "binContent = " << binContent << ", binError = " << binError << std::endl;
      resultHisto->SetBinContent(i-offset,j-offset,binContent);
      resultHisto->SetBinError(i-offset,j-offset,binError);
    }
  }

  if(removeBin != 0)
  {
    double integral = resultHisto->Integral();
    std::cout << "integral = " << integral << std::endl;
    resultHisto->Scale(1./integral);
  }

  return resultHisto;
}



TH2D* MuonTemplatesBuilder::Build2DTemplateFromFit(TH1D* xHisto, TH1D* yHisto, std::string name)
{
  const std::string xTitle = xHisto->GetXaxis()->GetTitle();
  const std::string yTitle = yHisto->GetXaxis()->GetTitle();
  TH1D* xHistoNormalized = NormalizeHistogram(xHisto);
  TH1D* yHistoNormalized = NormalizeHistogram(yHisto);
  const double xBinWidth = xHistoNormalized->GetXaxis()->GetBinWidth(1);
  const double xMin = axisMin;
  const double xMax = axisMax;
  const unsigned int xBins = xHistoNormalized->GetXaxis()->FindBin(axisMax) - xHistoNormalized->GetXaxis()->FindBin(axisMin);
//   const double yBinWidth = yHistoNormalized->GetXaxis()->GetBinWidth(1);
  const double yMin = axisMin;
  const double yMax = axisMax;
  const unsigned int yBins = yHistoNormalized->GetXaxis()->FindBin(axisMax) - yHistoNormalized->GetXaxis()->FindBin(axisMin);

  // std::cout << "xBinWidth = " << xBinWidth << ", xMin = " << xMin << ", xMax = " << xMax << ", xBins = " << xBins << std::endl;
  // std::cout << "yBinWidth = " << yBinWidth << ", yMin = " << yMin << ", yMax = " << yMax << ", yBins = " << yBins << std::endl;

  if(xBins != yBins || xMin != yMin || xMax != yMax)
  {
    std::cout << "E R R O R! MuonTemplatesBuilder::Build2DTemplateFromFit: x and y have different axes!\n";
    exit(1);
  }
  TH2D* resultHisto = new TH2D(name.c_str(),"",xBins,xMin,xMax,yBins,yMin,yMax);
  resultHisto->GetXaxis()->SetTitle(xTitle.c_str());
  resultHisto->GetYaxis()->SetTitle(yTitle.c_str());
  oss.str("");
  oss << "N_{#mu} / (" << xBinWidth << " #times " << xBinWidth << ")"<< axislabel2D.c_str();
  std::string zTitle = oss.str();
  resultHisto->GetZaxis()->SetTitle(zTitle.c_str());
  
  unsigned int offset = 0;
  std::cout << name << ": xHistoNormalized->GetBinLowEdge(1) = " << xHistoNormalized->GetBinLowEdge(1) << std::endl;
  std::cout << name << ": axisMin = " << axisMin << ", xHistoNormalized->FindBin(axisMin) = " << xHistoNormalized->FindBin(axisMin) << std::endl;
  
  if(axisMin > xHistoNormalized->GetBinLowEdge(1))
  {
    offset = xHistoNormalized->FindBin(axisMin) - 1;
  }
  
  for(unsigned int i = 1+offset; i < xBins+1+offset; i++)
  {
    for(unsigned int j = 1+offset; j < yBins+1+offset; j++)
    {
      if(i <= removeBin+offset && j <= removeBin+offset)
      {
        resultHisto->SetBinContent(i-offset,j-offset,0.);
        resultHisto->SetBinError(i-offset,j-offset,0.);
        continue;
      }
      double xi = xHistoNormalized->GetBinContent(i);
      double xj = xHistoNormalized->GetBinContent(j);
      double yi = yHistoNormalized->GetBinContent(i);
      double yj = yHistoNormalized->GetBinContent(j);
      double xErri = xHistoNormalized->GetBinError(i);
      double xErrj = xHistoNormalized->GetBinError(j);
      double yErri = yHistoNormalized->GetBinError(i);
      double yErrj = yHistoNormalized->GetBinError(j);
//       double random = rng.Rndm();
      double binContent, binError;
      //       if(random > 0.5)
      //       {
        //         binContent = xi * yj;
      //         if(xi != 0 && yj != 0)
      //           binError = sqrt((xErri/xi)*(xErri/xi) + (yErrj/yj)*(yErrj/yj))*binContent;
      //         else
      //           binError = 0;
      //       }
      //       else
      //       {
        //         binContent = xj * yi;
      //         if(xj != 0 && yi != 0)
      //           binError = sqrt((xErrj/xj)*(xErrj/xj) + (yErri/yi)*(yErri/yi))*binContent;
      //         else
      //           binError = 0;
      //       }
      double ij = xi * yj;
      double ijErr;
      if(ij != 0)
        ijErr = sqrt((xErri/xi)*(xErri/xi) + (yErrj/yj)*(yErrj/yj))*ij;
      else
        ijErr = 0;
      double ji = xj * yi;
      double jiErr;
      if(ji != 0)
        jiErr = sqrt((xErrj/xj)*(xErrj/xj) + (yErri/yi)*(yErri/yi))*ji;
      else
        jiErr = 0;
      binContent = (ij + ji) / 2;
      binError = sqrt(ijErr*ijErr + jiErr*jiErr) / 2;
       //       std::cout << "binContent = " << binContent << ", binError = " << binError << std::endl;
      resultHisto->SetBinContent(i-offset,j-offset,binContent);
      resultHisto->SetBinError(i-offset,j-offset,binError);
    }
  }
  if(removeBin != 0)
  {
    double integral = resultHisto->Integral();
    std::cout << "name = " << name << ", removed fraction = " << (1-integral) << std::endl;
    resultHisto->Scale(1./integral);
  }
  return resultHisto;
}



// Backwards compatibility
TH2D* MuonTemplatesBuilder::Build2DIPDistribution(TH1D* xHisto, TH1D* yHisto, std::string name, const bool doRebin)
{
  std::cout << "W A R N I N G: MuonTemplatesBuilder::Build2DIPDistribution(TH1D*, TH1D*, std::string, const bool) is deprecated!\n";
  std::cout << "               Please use MuonTemplatesBuilder::Build2DDistribution(TH1D*, TH1D*, std::string, const bool) instead.\n";
  return Build2DDistribution(xHisto, yHisto, name, doRebin);
}



TH2D* MuonTemplatesBuilder::Build2DDistribution(TH1D* xHisto, TH1D* yHisto, std::string name, const bool doRebin)
{
  const unsigned int xhBins = xHisto->GetXaxis()->GetNbins();
  const double xhMin = xHisto->GetXaxis()->GetXmin();
  const double xhMax = xHisto->GetXaxis()->GetXmax();
  const std::string xTitle = xHisto->GetXaxis()->GetTitle();
  const unsigned int yhBins = yHisto->GetXaxis()->GetNbins();
  const double yhMin = yHisto->GetXaxis()->GetXmin();
  const double yhMax = yHisto->GetXaxis()->GetXmax();
  const std::string yTitle = yHisto->GetXaxis()->GetTitle();

  if(xhBins != yhBins || xhMin != yhMin || xhMax != yhMax)
  {
    std::cout << "E R R O R! MuonTemplatesBuilder::Build2DDistribution: x and y have different axes!\n";
    exit(1);
  }

  TH1D* xHistoPrepared = PrepareHistogram(xHisto, doRebin);
  TH1D* yHistoPrepared = PrepareHistogram(yHisto, doRebin);
  const double xBinWidth = xHistoPrepared->GetXaxis()->GetBinWidth(1);
  //   const double xMin = xHistoPrepared->GetXaxis()->GetBinLowEdge(axisMin);
  //   const double xMax = xHistoPrepared->GetXaxis()->GetBinUpEdge(axisMax);
  const double xMin = axisMin;
  const double xMax = axisMax;

  //   const unsigned int xBins = xHistoPrepared->GetXaxis()->FindBin(xMax-xBinWidth) - xHistoPrepared->GetXaxis()->FindBin(xMin);
  const unsigned int xBins = xHistoPrepared->GetXaxis()->FindBin(axisMax) - xHistoPrepared->GetXaxis()->FindBin(axisMin);
//   const double yBinWidth = yHistoPrepared->GetXaxis()->GetBinWidth(1);
  const double yMin = axisMin;
  const double yMax = axisMax;
  //   const double yMin = yHistoPrepared->GetXaxis()->GetBinLowEdge(axisMin);
  //   const double yMax = yHistoPrepared->GetXaxis()->GetBinUpEdge(axisMax);
  const unsigned int yBins = yHistoPrepared->GetXaxis()->FindBin(axisMax) - yHistoPrepared->GetXaxis()->FindBin(axisMin);

  // std::cout << "xBinWidth = " << xBinWidth << ", xMin = " << xMin << ", xMax = " << xMax << ", xBins = " << xBins << std::endl;
  // std::cout << "yBinWidth = " << yBinWidth << ", yMin = " << yMin << ", yMax = " << yMax << ", yBins = " << yBins << std::endl;

  //   outputFile->cd();
  //   xHistoPrepared->Write();
  //   yHistoPrepared->Write();

  if(xBins != yBins || xMin != yMin || xMax != yMax)
  {
    std::cout << "E R R O R! MuonTemplatesBuilder::Build2DDistribution: x and y have different axes after rebin!\n";
    exit(1);
  }

  TH2D* resultHisto = new TH2D(name.c_str(),"",xBins,xMin,xMax,yBins,yMin,yMax);
  resultHisto->GetXaxis()->SetTitle(xTitle.c_str());
  resultHisto->GetYaxis()->SetTitle(yTitle.c_str());
  oss.str("");
  oss << "N_{#mu} / (" << xBinWidth << " #times " << xBinWidth << ")" << axislabel2D.c_str();
  std::string zTitle = oss.str();
  resultHisto->GetZaxis()->SetTitle(zTitle.c_str());

  for(unsigned int i = 0; i < xBins+1; i++)
  {
    for(unsigned int j = 0; j < yBins+1; j++)
    {
      double xi = xHistoPrepared->GetBinContent(i);
      double xj = xHistoPrepared->GetBinContent(j);
      double yi = yHistoPrepared->GetBinContent(i);
      double yj = yHistoPrepared->GetBinContent(j);
      double xErri = xHistoPrepared->GetBinError(i);
      double xErrj = xHistoPrepared->GetBinError(j);
      double yErri = yHistoPrepared->GetBinError(i);
      double yErrj = yHistoPrepared->GetBinError(j);
//       double random = rng.Rndm();
      double binContent, binError;
      //       if(random > 0.5)
      //       {
        //         binContent = xi * yj;
      //         if(xi != 0 && yj != 0)
      //           binError = sqrt((xErri/xi)*(xErri/xi) + (yErrj/yj)*(yErrj/yj))*binContent;
      //         else
      //           binError = 0;
      //       }
      //       else
      //       {
        //         binContent = xj * yi;
      //         if(xj != 0 && yi != 0)
      //           binError = sqrt((xErrj/xj)*(xErrj/xj) + (yErri/yi)*(yErri/yi))*binContent;
      //         else
      //           binError = 0;
      //       }
      double ij = xi * yj;
      double ijErr;
      if(ij != 0)
        ijErr = sqrt((xErri/xi)*(xErri/xi) + (yErrj/yj)*(yErrj/yj))*ij;
      else
        ijErr = 0;
      double ji = xj * yi;
      double jiErr;
      if(ji != 0)
        jiErr = sqrt((xErrj/xj)*(xErrj/xj) + (yErri/yi)*(yErri/yi))*ji;
      else
        jiErr = 0;
      binContent = (ij + ji) / 2;
      binError = sqrt(ijErr*ijErr + jiErr*jiErr) / 2;
//       std::cout << "binContent = " << binContent << ", binError = " << binError << std::endl;
      resultHisto->SetBinContent(i,j,binContent);
      resultHisto->SetBinError(i,j,binError);
    }
  }
  return resultHisto;
}



void MuonTemplatesBuilder::SaveTemplates()
{
  outputFile->cd();
  for(unsigned int i = 0; i < templates1D.size(); i++)
  {
    templates1D[i]->Write();
  }
  for(unsigned int i = 0; i < templates2D.size(); i++)
  {
    templates2D[i]->Write();
  }
  for(unsigned int i = 0; i < distributions1D.size(); i++)
  {
    distributions1D[i]->Write();
  }
  for(unsigned int i = 0; i < distributions2D.size(); i++)
  {
    distributions2D[i]->Write();
  }
}



TH1D* MuonTemplatesBuilder::PrepareHistogram(TH1D* histo, const bool doRebin)
{
  TH1::AddDirectory(false);
  TH1D* temp = (TH1D*)(histo->Clone());
  //   outputFile->cd();
//   temp->Write();
  TH1D* temp2;
  if(doRebin && rebinFactor != 1)
    temp2 = (TH1D*)temp->Rebin(rebinFactor);
  else
    temp2 = temp;
    double binWidth = temp2->GetBinWidth(1);
    int nBins = (int)( (axisMax-axisMin)/binWidth );
    TH1D* result = Create1DHistogram<TH1D>(temp2->GetName(),temp2->GetTitle(),nBins,axisMin,axisMax,temp2->GetXaxis()->GetTitle(),temp2->GetYaxis()->GetTitle());
    TH1::AddDirectory(true);
  
//   result->Sumw2();
  
  int offset = 0;
  if(axisMin > temp2->GetBinLowEdge(1))
  {
    offset = temp2->FindBin(axisMin) - 1;
  }
  
  for(int iBin = 1+offset; iBin <= nBins+offset; iBin++)
  {
    result->SetBinContent(iBin-offset,temp2->GetBinContent(iBin));
    result->SetBinError(iBin-offset,sqrt(result->GetBinContent(iBin)));
  }
  //   double histoLow = result->GetXaxis()->GetBinLowEdge(axisMin);
  //   double histoUp = result->GetXaxis()->GetBinUpEdge(axisMax);
  //   std::cout << "histoLow = " << histoLow << ", histoUp = " << histoUp << std::endl;
//   double binWidth = result->GetBinWidth(1);
//   result->SetAxisRange(axisMin,axisMax-binWidth/2);
  return result;
}



TH1D* MuonTemplatesBuilder::PrepareHistogramForFit(TH1D* histo, const bool doRebin)
{
  TH1::AddDirectory(false);
  TH1D* temp = (TH1D*)(histo->Clone());
//   outputFile->cd();
//   temp->Write();
  TH1D* temp2;
  if(doRebin && rebinFactor != 1)
    temp2 = (TH1D*)temp->Rebin(rebinFactor);
  else
    temp2 = temp;
  double binWidth = temp2->GetBinWidth(1);
  int nBins = (int)( (axisMax-0.)/binWidth );
  TH1D* result = Create1DHistogram<TH1D>(temp2->GetName(),temp2->GetTitle(),nBins,0.,axisMax,temp2->GetXaxis()->GetTitle(),temp2->GetYaxis()->GetTitle());
  
//  result->Sumw2();
  
  for(int iBin = 1; iBin <= nBins; iBin++)
  {
    result->SetBinContent(iBin,temp2->GetBinContent(iBin));
    result->SetBinError(iBin,temp2->GetBinError(iBin));
    //     result->SetBinError(iBin,sqrt(result->GetBinContent(iBin)));
  }
  //   double histoLow = result->GetXaxis()->GetBinLowEdge(axisMin);
  //   double histoUp = result->GetXaxis()->GetBinUpEdge(axisMax);
  //   std::cout << "histoLow = " << histoLow << ", histoUp = " << histoUp << std::endl;
  //   double binWidth = result->GetBinWidth(1);
  //   result->SetAxisRange(axisMin,axisMax-binWidth/2);
  TH1::AddDirectory(true);
  return result;
}



TH1D* MuonTemplatesBuilder::NormalizeHistogram(TH1D* histo)
{
  TH1::AddDirectory(false);
  TH1D* result = (TH1D*)(histo->Clone());
  TH1::AddDirectory(true);
  //   result->Sumw2();
  result->SetAxisRange(axisMin,axisMax);
  double binWidth = result->GetXaxis()->GetBinWidth(1);
  int binLow = result->FindBin(axisMin);
  int offset = 0;
  if(axisMin > result->GetBinLowEdge(1))
  {
    offset = result->FindBin(axisMin) - 1;
  }
  int binUp = result->FindBin(axisMax-binWidth+1+offset);
//   std::cout << "inside MuonTemplatesBuilder::NormalizeHistogram(TH1D* histo): binLow = " << binLow << ", binUp = " << binUp << std::endl;
//   outputFile->cd();
//   result->Write();
  double scaleFactor = 1/(result->Integral(binLow,binUp));
  result->Scale(scaleFactor);
//   std::cout << "inside MuonTemplatesBuilder::NormalizeHistogram(TH1D* histo): result->Integral() = " << result->Integral() << std::endl;

  return result;
}



TH1D* MuonTemplatesBuilder::PrepareAndNormalizeHistogram(TH1D* histo, const bool doRebin)
{
  TH1::AddDirectory(false);
  TH1D* temp = (TH1D*)(histo->Clone());
  //   outputFile->cd();
//   temp->Write();
  TH1D* result;
  if(doRebin && rebinFactor != 1)
    result = (TH1D*)temp->Rebin(rebinFactor);
  else
    result = temp;
  TH1::AddDirectory(true);
  //   result->Sumw2();
  //   double histoLow = result->GetXaxis()->GetBinLowEdge(axisMin);
  //   double histoUp = result->GetXaxis()->GetBinUpEdge(axisMax);
  //   std::cout << "histoLow = " << histoLow << ", histoUp = " << histoUp << std::endl;
  result->SetAxisRange(axisMin,axisMax);
  double binWidth = result->GetXaxis()->GetBinWidth(1);
  int binLow = result->FindBin(axisMin);
//   int offset = 0;
//   if(axisMin > result->GetBinLowEdge(1))
//   {
//     offset = result->FindBin(axisMin) - 1;
//   }
  int binUp = result->FindBin(axisMax-binWidth);
  // std::cout << "binLow = " << binLow << ", binUp = " << binUp << std::endl;
//   outputFile->cd();
//   result->Write();
  double scaleFactor = 1/(result->Integral(binLow,binUp));
  result->Scale(scaleFactor);
  return result;
}



TH1D* MuonTemplatesBuilder::CreateTH1FromTF1(const TF1* func, const std::string histoName, const unsigned int nBins, const double xMin, const double xMax, const std::string xTitle, const std::string yTitle)
{
  TH1::AddDirectory(false);
  TH1D* histo = Create1DHistogram<TH1D>(histoName.c_str(),"",nBins,xMin,xMax,xTitle.c_str(),yTitle.c_str());
  TH1::AddDirectory(true);
  for(unsigned int iBin = 1; iBin < nBins+1; iBin++)
  {
    double binCenter = histo->GetBinCenter(iBin);
    double binContent = func->Eval(binCenter);
    histo->SetBinContent(iBin,binContent);
    histo->SetBinError(iBin,sqrt(binContent));
  }
  return histo;
}



void MuonTemplatesBuilder::FindIPFitParameters(const TH1D* histo, const std::string histoName, double& funcMin, double& funcMax, unsigned short& nGauss, unsigned short& nExp)
{
  nGauss = 1;
  nExp = 2;
  funcMin = 0.;
  if(histoName.find("C") != std::string::npos)
  {
//     nGauss = 1;
    funcMax = 0.13;
    if(histo->Integral() < 5000.)
      funcMax = 0.1;
  }
  if(histoName.find("P") != std::string::npos)
  {
    funcMax = 0.05;
    nExp = 1;
    nGauss = 1;
    if(histo->Integral() < 2500.)
    {
      //         nExp = 1; 
      //         nGauss = 1;
      funcMax = 0.025;
    }
  }
  if(histoName.find("A") != std::string::npos)
  {
    if(histo->Integral() > 130000 && histo->Integral() < 140000)
    {
      nGauss = 1;
      nExp = 3;
    }
  }
}



void MuonTemplatesBuilder::CreateAndSavePrettyPlots()
{
  outputFile->cd();
  std::vector<TH1*> projections;
  std::vector<std::string> histoNames;
  std::vector<unsigned int> lineStyles;
  std::vector<unsigned int> markerStyles;
  std::vector<unsigned int> colors;
  for(unsigned int i = 0; i < templates2D.size(); i++)
  {
    if(templates2D[i]->Integral() != templates2D[i]->Integral())
      continue;
    std::string histoName = templates2D[i]->GetName();
    std::string canvasName = "c" + histoName.substr(1);
    std::string canvasTitle = histoName.substr(1) + " di-#mu" + titlelabel2D.c_str();
    CreateAndWriteCanvas3D(canvasName.c_str(),canvasTitle.c_str(),false,false,false,230.,35.,templates2D[i]);
    TH1D* proj = templates2D[i]->ProjectionX();
    projections.push_back(proj);
    histoNames.push_back(histoName.substr(1));
    lineStyles.push_back(1);
    markerStyles.push_back(0);
    colors.push_back(i+1);
  }
  LegendDefinition leg = {"2D Templates",histoNames,"NE"};
  CreateAndWriteCanvas("cProjections", "Projections of 2D templates",&lineStyles[0],&markerStyles[0],&colors[0],leg,false,true,projections);
  
  for(unsigned int i = 0; i < templates1D.size(); i++)
  {
    std::string histoName = templates1D[i]->GetName();
    std::string canvasName = "c" + histoName.substr(1);
    std::string canvasTitle = histoName.substr(1) + " single-#mu" + titlelabel2D.c_str();
    CreateAndWriteCanvas(canvasName.c_str(),canvasTitle.c_str(),0,20,1,false,true,templates1D[i]);
  }
}



std::map<std::string, std::pair<double,double> > MuonTemplatesBuilder::FindIntegralOfRemovedBinsFor1DTemplates()
{
  std::map<std::string,std::pair<double,double> > result;
  int nBins = (int)((axisMax-0.)/(inputHistos[0]->GetBinWidth(1)*rebinFactor));
  int iName = 0;
  for(unsigned int iHisto = 0; iHisto < inputHistos.size(); iHisto++)
  {
    TH1D* histo1D = (TH1D*)(inputHistos[iHisto]->Clone("histo1D"));
    histo1D->Rebin(rebinFactor);
    histo1D->SetAxisRange(0,axisMax);
        std::cout << "I N F O : FindIntegralOfRemovedBinsFor1DTemplates() - nBins = " << nBins << std::endl; 
    double histoIntegral = histo1D->Integral();
        std::cout << "                                                      histo integral = " << histoIntegral << std::endl;
//     histo1D->Scale(1/histoIntegral);
//         std::cout << "                                                      histo integral after rescale = " << histo1D->Integral() << std::endl;
    int offset = histo1D->FindBin(axisMin) - 1;
        std::cout << "                                                      offset = " << offset << std::endl;
    std::string templateName = templates1D[iName++]->GetName();
    std::cout << "templateName = " << templateName << std::endl;
    double integralOfRemovedBins = 0;
    double integralOfRemovedBinsError = 0;
    for(int i = 1; i < nBins; i++)
    {
      double binContent = histo1D->GetBinContent(i);
      if(i <= offset)
      {
        integralOfRemovedBins+=binContent;
      }
    }
    integralOfRemovedBinsError = sqrt(integralOfRemovedBins);
    integralOfRemovedBins/=histoIntegral;
    integralOfRemovedBinsError/=histoIntegral;
    std::cout << "integral of removed bins = " << integralOfRemovedBins << " +/- " << integralOfRemovedBinsError << std::endl;
    std::pair<double,double> resultPair = std::make_pair(integralOfRemovedBins,integralOfRemovedBinsError);
    result[templateName] = resultPair;
    histo1D->Delete();
  }
  return result;
}



std::map<std::string, std::pair<double,double> > MuonTemplatesBuilder::FindIntegralOfRemovedBinsFor2DTemplates()
{
  std::map<std::string,std::pair<double,double> > result;
  int nBins = (int)((axisMax-0.)/(inputHistos[0]->GetBinWidth(1)*rebinFactor));
  int iName = 0;
  for(unsigned int iHisto = 0; iHisto < inputHistos.size(); iHisto++)
  {
    TH1D* xHisto1D = (TH1D*)(inputHistos[iHisto]->Clone("xHisto1D"));
    xHisto1D->Rebin(rebinFactor);
    xHisto1D->SetAxisRange(0,axisMax);
//     std::cout << "I N F O : FindIntegralOfRemovedBinsFor2DTemplates() - nBins = " << nBins << std::endl; 
    double xIntegral = xHisto1D->Integral();
//     std::cout << "                                                      histo integral = " << xIntegral << std::endl;
//     xHisto1D->Scale(1/xIntegral);
//     std::cout << "                                                      histo integral after rescale = " << xHisto1D->Integral() << std::endl;
    int offset = xHisto1D->FindBin(axisMin) - 1;
//     std::cout << "                                                      offset = " << offset << std::endl;
    for(unsigned int jHisto = iHisto; jHisto < inputHistos.size(); jHisto++)
    {
      TH1D* yHisto1D = (TH1D*)(inputHistos[jHisto]->Clone("yHisto1D"));
      yHisto1D->Rebin(rebinFactor);
      yHisto1D->SetAxisRange(0,axisMax);
      double yIntegral = yHisto1D->Integral();
//       yHisto1D->Scale(1/yIntegral);
      std::string templateName = templates2D[iName++]->GetName();
      std::cout << "templateName = " << templateName << std::endl;
      TH2D* histo2D = new TH2D("histo2D","",nBins,0,axisMax,nBins,0,axisMax);
      double integralOfRemovedBins = 0;
      double integralOfRemovedBinsError = 0;
      for(int i = 1; i < nBins; i++)
      {
        for(int j = 1; j < nBins; j++)
        {
          double xi = xHisto1D->GetBinContent(i);
          double xj = xHisto1D->GetBinContent(j);
          double yi = yHisto1D->GetBinContent(i);
          double yj = yHisto1D->GetBinContent(j);
          double ij = xi * yj;
          double ji = xj * yi;
          double binContent = (ij + ji) / 2;
          double binError = sqrt(yj*yj*xi + xi*xi*yj + yi*yi*xj + xj*xj*yi) / 2;
          if(i <= offset && j <= offset)
          {
            integralOfRemovedBins+=binContent;
            integralOfRemovedBinsError = sqrt(integralOfRemovedBinsError*integralOfRemovedBinsError + binError*binError);
          }
          else if(i <= removeBin+offset && j <= removeBin+offset)
          {
            integralOfRemovedBins+=binContent;
            integralOfRemovedBinsError = sqrt(integralOfRemovedBinsError*integralOfRemovedBinsError + binError*binError);
          }
        }
      }
      integralOfRemovedBins/=xIntegral;
      integralOfRemovedBins/=yIntegral;
      integralOfRemovedBinsError/=xIntegral;
      integralOfRemovedBinsError/=yIntegral;
      std::cout << "integral of removed bins = " << integralOfRemovedBins << " +/- " << integralOfRemovedBinsError << std::endl;
      std::pair<double,double> resultPair = std::make_pair(integralOfRemovedBins,integralOfRemovedBinsError);
      result[templateName] = resultPair;
      yHisto1D->Delete();
      histo2D->Delete();
    }
    xHisto1D->Delete();
  }
  return result;
}



#endif // MuonTemplatesBuilder_cxx
