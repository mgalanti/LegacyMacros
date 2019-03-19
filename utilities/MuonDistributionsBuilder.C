#ifndef MuonDistributionsBuilder_cxx
#define MuonDistributionsBuilder_cxx


#include "MuonDistributionsBuilder.h"


MuonDistributionsBuilder::~MuonDistributionsBuilder()
{
  inputFile->Close();
  outputFile->Close();
}



void MuonDistributionsBuilder::Initialize()
{

  std::cout << "I N F O: Set the axis label according to IP and PtRel/PtOut choice" << std::endl;
  std::cout << "         This may be not \"clean\" ..." << std::endl;

  if (inputHistoNames1D.size() > 0)
  {
    if (inputHistoNames1D[0].find("hMuonDxy") != std::string::npos)
    {
      axislabel1D = " cm";
    }
    else if (    (inputHistoNames1D[0].find("hMuonPtRel") != std::string::npos)
              || (inputHistoNames1D[0].find("hMuonOSPtRel") != std::string::npos)
              || (inputHistoNames1D[0].find("hMuonSSPtRel") != std::string::npos)
              || (inputHistoNames1D[0].find("hMuonSSPlusPtRel") != std::string::npos)
              || (inputHistoNames1D[0].find("hMuonSSMinusPtRel") != std::string::npos) )
    {
      axislabel1D = " GeV/c";
    }
    else if (    (inputHistoNames1D[0].find("hMuonPtOut") != std::string::npos)
              || (inputHistoNames1D[0].find("hMuonOSPtOut") != std::string::npos)
              || (inputHistoNames1D[0].find("hMuonSSPtOut") != std::string::npos)
              || (inputHistoNames1D[0].find("hMuonSSPlusPtOut") != std::string::npos)
              || (inputHistoNames1D[0].find("hMuonSSMinusPtOut") != std::string::npos) )
    {
      axislabel1D = " GeV/c";
    }
    else
    {
      std::cout << "W A R N I N G ! No input histos 1D name match" << std::endl;
    }
  }
  else
  {
    std::cout << "I N F O: No input histos 1D (if you are building the 2D distributions only, this message is harmless)" << std::endl;
  }
    
  if (inputHistoNames2D.size() > 0)
  {
    if (inputHistoNames2D[0].find("hMuonDxy") != std::string::npos)
    {
      axislabel2D = " cm^{2}";
    }
    else if (    (inputHistoNames2D[0].find("hMuonPtRel") != std::string::npos)
              || (inputHistoNames2D[0].find("hMuonOSPtRel") != std::string::npos)
              || (inputHistoNames2D[0].find("hMuonSSPtRel") != std::string::npos)
              || (inputHistoNames2D[0].find("hMuonSSPlusPtRel") != std::string::npos)
              || (inputHistoNames2D[0].find("hMuonSSMinusPtRel") != std::string::npos) )
    {
      axislabel2D = " (GeV/c)^{2}";      
    }
    else if (    (inputHistoNames2D[0].find("hMuonPtOut") != std::string::npos)
              || (inputHistoNames2D[0].find("hMuonOSPtOut") != std::string::npos)
              || (inputHistoNames2D[0].find("hMuonSSPtOut") != std::string::npos)
              || (inputHistoNames2D[0].find("hMuonSSPlusPtOut") != std::string::npos)
              || (inputHistoNames2D[0].find("hMuonSSMinusPtOut") != std::string::npos) )
    {
      axislabel2D = " (GeV/c)^{2}";
    }
    else
    {
      std::cout << "W A R N I N G ! No input histos 2D name match" << std::endl;
    }
  }
  else
  {
    std::cout << "I N F O: No input histos 2D (if you are building the 1D distributions only, this message is harmless)" << std::endl;
  }



  inputFile = new TFile(inputFileName.c_str(),"READ");
  for(unsigned int i = 0; i < inputHistoNames1D.size(); i++)
  {
    TH1D* tempHisto = 0;
    inputFile->GetObject(inputHistoNames1D[i].c_str(),tempHisto);
    if(tempHisto)
    {
      inputHistos1D.push_back(tempHisto);
    }
    else
    {
      std::cout << "E R R O R! MuonDistributionsBuilder::Initialize : no 1D histogram with name " << inputHistoNames1D[i] << " found in input file!\n";
      exit(1);
    }
  }

  for(unsigned int i = 0; i < inputHistoNames2D.size(); i++)
  {
    TH2D* tempHisto = 0;
    inputFile->GetObject(inputHistoNames2D[i].c_str(),tempHisto);
    if(tempHisto)
    {
      inputHistos2D.push_back(tempHisto);
    }
    else
    {
      std::cout << "E R R O R! MuonDistributionsBuilder::Initialize : no 2D histogram with name " << inputHistoNames2D[i] << " found in input file!\n";
      exit(1);
    }
  }

  for(unsigned int i = 0; i < std::max(inputHistos2D.size(),inputHistos1D.size()); i++)
  {
    scaleFactors.push_back(1.);
  }
  distribution1D = 0;
  distribution2D = 0;
}



void MuonDistributionsBuilder::Build1DDistribution()
{
  TH1D* result;
  Build1DDistribution(result);
  distribution1D = result;
}



void MuonDistributionsBuilder::Build1DDistribution(TH1D*& result) /// FIXME ///
{ 
  if(inputHistos1D.size() == 0)
  {
    std::cout << "W A R N I N G ! MuonDistributionsBuilder::Build1DDistribution(TH1D*&): vector of input histograms is empty!\n";
    std::cout << "                \"result\" is being set to 0.\n";
    result = 0;
    return;
  }
  const std::string xTitle = inputHistos1D[0]->GetXaxis()->GetTitle();
  std::vector<TH1D*> preparedHistos = PrepareHistograms(inputHistos1D);
  const double xBinWidth = preparedHistos[0]->GetXaxis()->GetBinWidth(1);
  const double xMin = axisMin;
  const double xMax = axisMax;
  const unsigned int xBins = preparedHistos[0]->GetXaxis()->FindBin(axisMax) - preparedHistos[0]->GetXaxis()->FindBin(axisMin);
  
  oss.str("");
  oss << "N_{#mu} / " << xBinWidth << axislabel1D.c_str();
  std::string yTitle = oss.str();
  result = Create1DHistogram<TH1D>(outputHistoName1D.c_str(),outputHistoName1D.c_str(),xBins,xMin,xMax,xTitle.c_str(),yTitle.c_str());

  int offset = 0;
  if(axisMin > preparedHistos[0]->GetBinLowEdge(1))
  {
    offset = preparedHistos[0]->FindBin(axisMin) - 1;
  }

  for(unsigned int iBin = 1+offset; iBin < xBins+1+offset; iBin++)
  {
    double binContent = 0;
    double binErrorSquared = 0;
    for(unsigned int iHisto = 0; iHisto < preparedHistos.size(); iHisto++)
    {
      binContent+=scaleFactors[iHisto]*preparedHistos[iHisto]->GetBinContent(iBin);
      binErrorSquared+=scaleFactors[iHisto]*scaleFactors[iHisto]*preparedHistos[iHisto]->GetBinError(iBin)*preparedHistos[iHisto]->GetBinError(iBin);
    }
    double binError = sqrt(binErrorSquared);
    result->SetBinContent(iBin-offset,binContent);
    result->SetBinError(iBin-offset,binError);
  }
  result->SetEntries(result->Integral());
  
  std::cout << Form("resultHisto DataDistribution 1D\nOffset   %d\n", offset);
//   for (unsigned int ibin = 1; ibin < xBins+1 /*10*/; ++ibin){
//     std::cout << Form("bin %d [%f,%f]",ibin,result->GetBinLowEdge(ibin),result->GetBinLowEdge(ibin)+xBinWidth) << " content : " << result->GetBinContent(ibin) << std::endl;
//   }
  std::cout << "Integral " <<  result->Integral() << std::endl;
  std::cout << "Entries  " <<  result->GetEntries() << std::endl;
  
}

/* ORIGINAL VERSION */ /*
{
  if(inputHistos1D.size() == 0)
  {
    std::cout << "W A R N I N G ! MuonDistributionsBuilder::Build1DDistribution(TH1D*&): vector of input histograms is empty!\n";
    std::cout << "                \"result\" is being set to 0.\n";
    result = 0;
    return;
  }
  const std::string xTitle = inputHistos1D[0]->GetXaxis()->GetTitle();
  std::vector<TH1D*> preparedHistos = PrepareHistograms(inputHistos1D);
  const double xBinWidth = preparedHistos[0]->GetXaxis()->GetBinWidth(1);
  const double xMin = axisMin;
  const double xMax = axisMax;
  int offset = 0;
  if(axisMin > preparedHistos[0]->GetBinLowEdge(1))
  {
    offset = preparedHistos[0]->FindBin(axisMin) - 1;
  }
  const unsigned int xBins = preparedHistos[0]->GetXaxis()->FindBin(axisMax) - preparedHistos[0]->GetXaxis()->FindBin(axisMin);
//   std::cout << "xBinWidth = " << xBinWidth << ", xMin = " << xMin << ", xMax = " << xMax << ", xBins = " << xBins << std::endl;

  oss.str("");
  oss << "N_{#mu} / " << xBinWidth << axislabel1D.c_str();
  std::string yTitle = oss.str();
  result = Create1DHistogram<TH1D>(outputHistoName1D.c_str(),outputHistoName1D.c_str(),xBins,xMin,xMax,xTitle.c_str(),yTitle.c_str());
  for(unsigned int iBin = 1+offset; iBin < xBins+1+offset; iBin++)
  {
    double binContent = 0;
    double binErrorSquared = 0;
    for(unsigned int iHisto = 0; iHisto < preparedHistos.size(); iHisto++)
    {
      binContent+=scaleFactors[iHisto]*preparedHistos[iHisto]->GetBinContent(iBin);
      binErrorSquared+=scaleFactors[iHisto]*scaleFactors[iHisto]*preparedHistos[iHisto]->GetBinError(iBin)*preparedHistos[iHisto]->GetBinError(iBin);
    }
    double binError = sqrt(binErrorSquared);
    result->SetBinContent(iBin-offset,binContent);
    result->SetBinError(iBin-offset,binError);
  }
  result->SetEntries(result->Integral());
}
*/


void MuonDistributionsBuilder::Build2DDistribution()
{
  TH2D* result;
  Build2DDistribution(result);
  distribution2D = result;
}



void MuonDistributionsBuilder::Build2DDistribution(TH2D*& result)
{
  if(inputHistos2D.size() == 0)
  {
    std::cout << "W A R N I N G ! MuonDistributionsBuilder::Build2DDistribution(TH2D*&): vector of input histograms is empty!\n";
    std::cout << "                \"result\" is being set to 0.\n";
    result = 0;
    return;
  }
  const std::string xTitle = inputHistos2D[0]->GetXaxis()->GetTitle();
  const std::string yTitle = inputHistos2D[0]->GetYaxis()->GetTitle();
  std::vector<TH2D*> preparedHistos = PrepareHistograms(inputHistos2D);
  const double xBinWidth = preparedHistos[0]->GetXaxis()->GetBinWidth(1);
  const double xMin = axisMin;
  const double xMax = axisMax;
  const unsigned int xBins = preparedHistos[0]->GetXaxis()->FindBin(axisMax) - preparedHistos[0]->GetXaxis()->FindBin(axisMin);
  std::cout << "xBinWidth = " << xBinWidth << ", xMin = " << xMin << ", xMax = " << xMax << ", xBins = " << xBins << std::endl;
  const double yBinWidth = preparedHistos[0]->GetYaxis()->GetBinWidth(1);
  const double yMin = axisMin;
  const double yMax = axisMax;
  const unsigned int yBins = preparedHistos[0]->GetYaxis()->FindBin(axisMax) - preparedHistos[0]->GetYaxis()->FindBin(axisMin);
  std::cout << "yBinWidth = " << yBinWidth << ", yMin = " << yMin << ", yMax = " << yMax << ", yBins = " << yBins << std::endl;
  if(xBins != yBins || xMin != yMin || xMax != yMax)
  {
    std::cout << "E R R O R! MuonDistributionsBuilder::Build2DDistribution: x and y have different axes!\n";
    exit(1);
  }
  oss.str("");
  oss << "N_{#mu} / (" << xBinWidth << " #times " << xBinWidth << ")" << axislabel2D.c_str();
  std::string zTitle = oss.str();
  
  result = Create2DHistogram<TH2D>(outputHistoName2D.c_str(),outputHistoName2D.c_str(),xBins,xMin,xMax,yBins,yMin,yMax,xTitle.c_str(),yTitle.c_str(),zTitle.c_str());
  
  unsigned int offset = 0;
  if(axisMin > preparedHistos[0]->GetXaxis()->GetBinLowEdge(1))
  {
    offset = preparedHistos[0]->GetXaxis()->FindBin(axisMin) - 1;
  }
  
  std::cout << Form("resultHisto DataDistribution 2D\nOffset   %d\n", offset);

  for(unsigned int ixBin = 1+offset; ixBin < xBins+1+offset; ixBin++)
  {
    for(unsigned int iyBin = 1+offset; iyBin < yBins+1+offset; iyBin++)
    {
      if(ixBin <= removeBin+offset && iyBin <= removeBin+offset)
      {
        result->SetBinContent(ixBin-offset,iyBin-offset,0.);
        result->SetBinError(ixBin-offset,iyBin-offset,0.);
        continue;
      }
      double binContent = 0;
      double binErrorSquared = 0;
      for(unsigned int iHisto = 0; iHisto < preparedHistos.size(); iHisto++)
      {
        binContent+=scaleFactors[iHisto]*preparedHistos[iHisto]->GetBinContent(ixBin,iyBin);
        binErrorSquared+=scaleFactors[iHisto]*scaleFactors[iHisto]*preparedHistos[iHisto]->GetBinError(ixBin,iyBin)*preparedHistos[iHisto]->GetBinError(ixBin,iyBin);
      }
      double binError = sqrt(binErrorSquared);
      result->SetBinContent(ixBin-offset,iyBin-offset,binContent);
      result->SetBinError(ixBin-offset,iyBin-offset,binError);
    }
  }
  
  std::cout << "Integral " <<  result->Integral() << std::endl;
  std::cout << "Entries  " <<  result->GetEntries() << std::endl;

}



void MuonDistributionsBuilder::SaveDistributions()
{
  outputFile->cd();
  if(distribution1D)
    distribution1D->Write();
  if(distribution2D)
    distribution2D->Write();
}



std::vector<TH1D*> MuonDistributionsBuilder::PrepareHistograms(std::vector<TH1D*> histos)
{
  std::vector<TH1D*> results;
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    TH1D* temp = (TH1D*)(histos[iHisto]->Clone());
    outputFile->cd();
    temp->Write();
    TH1D* result;
    if(rebinFactor != 1)
      result = (TH1D*)temp->Rebin(rebinFactor);
    else
      result = temp;
//     result->Sumw2();
    result->SetAxisRange(axisMin,axisMax);
    results.push_back(result);
  }
    return results;
}



std::vector<TH2D*> MuonDistributionsBuilder::PrepareHistograms(std::vector<TH2D*> histos)
{
  std::vector<TH2D*> results;
  for(unsigned int iHisto = 0; iHisto < histos.size(); iHisto++)
  {
    TH2D* temp = (TH2D*)(histos[iHisto]->Clone());
    outputFile->cd();
    temp->Write();
    TH2D* result;
    if(rebinFactor != 1)
      result = (TH2D*)temp->Rebin2D(rebinFactor,rebinFactor);
    else
      result = temp;
    result->Sumw2();
    result->SetAxisRange(axisMin,axisMax,"X");
    result->SetAxisRange(axisMin,axisMax,"Y");
    results.push_back(result);
  }
  return results;
}



void MuonDistributionsBuilder::CreateAndSavePrettyPlots()
{
  if(distribution2D)
  {
    std::vector<TH1*> projections;
    std::vector<std::string> legendTexts;
    std::vector<unsigned int> lineStyles;
    std::vector<unsigned int> markerStyles;
    std::vector<unsigned int> colors;
    std::string histoName2D = distribution2D->GetName();
    std::string canvasName2D = "c" + histoName2D.substr(1);
    std::string canvasTitle2D = histoName2D.substr(1);
    CreateAndWriteCanvas3D(canvasName2D.c_str(),canvasTitle2D.c_str(),false,false,true,230.,35.,distribution2D);
    TH1D* projectionX = distribution2D->ProjectionX();
    double xBinWidthX = projectionX->GetXaxis()->GetBinWidth(1);
    oss.str("");
    oss << "N_{#mu} / " << xBinWidthX << axislabel1D.c_str();
    std::string yTitleX = oss.str();
    projectionX->GetYaxis()->SetTitle(yTitleX.c_str());
    projections.push_back(projectionX);
    lineStyles.push_back(0);
    markerStyles.push_back(21);
    colors.push_back(2);
    legendTexts.push_back("Projection along x");
    TH1D* projectionY = distribution2D->ProjectionY();
    double xBinWidthY = projectionY->GetXaxis()->GetBinWidth(1);
    oss.str("");
    oss << "N_{#mu} / " << xBinWidthY << axislabel1D.c_str();
    std::string yTitleY = oss.str();
    projectionY->GetYaxis()->SetTitle(yTitleY.c_str());
    projections.push_back(projectionY);
    lineStyles.push_back(0);
    markerStyles.push_back(20);
    colors.push_back(4);
    legendTexts.push_back("Projection along y");
    LegendDefinition leg = {"2D IP distribution",legendTexts,"NE"};
    CreateAndWriteCanvas("cProjections", "Projections of 2D IP Distribution",&lineStyles[0],&markerStyles[0],&colors[0],leg,false,true,projections);
  }
  if(distribution1D)
  {
    std::string histoName1D = distribution1D->GetName();
    std::string canvasName1D = "c" + histoName1D.substr(1);
    std::string canvasTitle1D = histoName1D.substr(1);
    CreateAndWriteCanvas(canvasName1D.c_str(),canvasTitle1D.c_str(),0,20,1,false,true,distribution1D);
  }
}


#endif // MuonDistributionsBuilder_cxx



