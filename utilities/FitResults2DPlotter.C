#ifndef FitResults2DPlotter_cxx
#define FitResults2DPlotter_cxx

#include <iostream>
#include <fstream>
#include <sstream>

#include "TFile.h"

#include "FitResults2DPlotter.h"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodBase.h"



FitResults2DPlotter::~FitResults2DPlotter()
{
  outputFile->Close();
  gROOT->GetListOfCanvases()->Clear();
  TDirectory* dir = gROOT->GetDirectory(curDirName.c_str());
  dir->cd();
}



void FitResults2DPlotter::Initialize()
{
  curDirName = gDirectory->GetPathStatic();
  outputFile = new TFile(outputFileName.c_str(),"RECREATE");
  int nPars = tfm->GetNumberTotalParameters();
  for(int iPar = 0; iPar < nPars; iPar++)
  {
    vPars.push_back(tfm->GetParameter(iPar));
    vParErrors.push_back(tfm->GetParError(iPar));
    vParNames.push_back(tfm->GetParName(iPar));
  }
  
  if(vPars.size() != hTemplates.size())
  {
    std::cout << "E R R O R ! The number of fit parameters is not equal to the number of templates!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  
  nDataEvents = hData->Integral();
  
  nEvents = 0;
  for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
  {
    nEvents += vPars[iPar];
  }
  likelihoodFCN = (BinnedLogLikelihoodBase*)tfm->GetMinuitFCN();
}



void FitResults2DPlotter::CreateAndSavePlots()
{
  std::ostringstream oss;
  oss.str("");

  std::string name, nameProjX, nameProjY;
  std::string title, titleProjX, titleProjY;
  
  int nXBins = hTemplates[0]->GetXaxis()->GetNbins();
  double xLoEdge =  hTemplates[0]->GetXaxis()->GetXmin();
  double xHiEdge =  hTemplates[0]->GetXaxis()->GetXmax();
  std::string xTitle = hTemplates[0]->GetXaxis()->GetTitle();
  int nYBins = hTemplates[0]->GetYaxis()->GetNbins();
  double yLoEdge =  hTemplates[0]->GetYaxis()->GetXmin();
  double yHiEdge =  hTemplates[0]->GetYaxis()->GetXmax();
  std::string yTitle = hTemplates[0]->GetYaxis()->GetTitle();
  
  std::cout << "Producing likelihood scan graphs: normalizing using a single variable" << std::flush;
  
  for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
  {
    for(unsigned int jPar = 0; jPar < vPars.size(); jPar++)
    {
      if (iPar == jPar)
        continue;
      
      double iValue = vPars[iPar];
      double jValue = vPars[jPar];
      double iError = vParErrors[iPar];
//       double jError = vParErrors[jPar];
      std::string iName = vParNames[iPar];
      std::string jName = vParNames[jPar];
      std::string graphName = "gParameterScan_" + iName + "-" + jName;
      std::string graphTitle = "Scan around minimum for parameters " + iName + " and " + jName;
      std::string graphXTitle = iName + " fraction";
      std::string graphYTitle = "Likelihood value";
      
      TGraph* gParameterScan = CreateGraph<TGraph>(graphName.c_str(),graphTitle.c_str(), graphXTitle.c_str(),graphYTitle.c_str());
      unsigned int iPoint=0;
      for(double shift = -5; shift <= 5; shift+=0.5)
      {
        double delta = shift*iError;
        double iValueNew = iValue + delta;
        double jValueNew = jValue - delta;
        if(iValueNew < 0 || iValueNew > nEvents || jValueNew < 0 || jValueNew > nEvents)
        {
//           std::cout <<"Outside limits! shift = " << shift << ", iValueNew = " << iValueNew << ", jValueNew = " << jValueNew << std::endl;
          continue;
        }
        std::vector<double> vParsNew;
        for(unsigned int iParNew = 0; iParNew < vPars.size(); iParNew++)
        {
          if(iParNew == iPar)
            vParsNew.push_back(iValueNew);
          else if(iParNew == jPar)
            vParsNew.push_back(jValueNew);
          else
            vParsNew.push_back(vPars[iParNew]);
        }
        double likelihoodValue = (*likelihoodFCN)(vParsNew);
//         std::cout << "Par" << iPar << ": " << vParsNew[iPar] << ", Par " << jPar << ": " << vParsNew[jPar] << ", likelihood = " << likelihoodValue << std::endl;
        gParameterScan->Set(gParameterScan->GetN()+1);
        gParameterScan->SetPoint(iPoint, iValueNew/nEvents, likelihoodValue);
        iPoint++;
      }
      std::string canvasName = "cParameterScan1D_" + iName + "-" + jName;
      CreateAndWriteCanvas(canvasName.c_str(), 21, 1, false, false, gParameterScan);
      std::cout << "." << std::flush;
    }
  }
  
  std::cout << "\n";

  std::cout << "Producing likelihood scan graphs: normalizing using all the other variables" << std::flush;
  
  for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
  {
    
    double iValue = vPars[iPar];
    double iError = vParErrors[iPar];
    std::string iName = vParNames[iPar];
    std::string graphName = "gParameterScan_" + iName;
    std::string graphTitle = "Scan around minimum for parameter " + iName;
    std::string graphXTitle = iName + " fraction";
    std::string graphYTitle = "Likelihood value";
      
    TGraph* gParameterScan = CreateGraph<TGraph>(graphName.c_str(),graphTitle.c_str(), graphXTitle.c_str(),graphYTitle.c_str());
    unsigned int iPoint=0;
    for(double shift = -5; shift <= 5; shift+=0.5)
    {
      double delta = shift*iError;
      double iValueNew = iValue + delta;
      if(iValueNew < 0 || iValueNew > nEvents)
      {
//         std::cout <<"Outside limits! shift = " << shift << ", iValueNew = " << iValueNew << std::endl;
        continue;
      }
      double scaleFactor = (1.-iValueNew)/(1.-iValue);
      std::vector<double> vParsNew;
      for(unsigned int iParNew = 0; iParNew < vPars.size(); iParNew++)
      {
        if(iParNew == iPar)
          vParsNew.push_back(iValueNew);
        else
          vParsNew.push_back(vPars[iParNew]*scaleFactor);
      }
      double likelihoodValue = (*likelihoodFCN)(vParsNew);
//       std::cout << "Par" << iPar << ": " << vParsNew[iPar] << ", likelihood = " << likelihoodValue << std::endl;
      gParameterScan->Set(gParameterScan->GetN()+1);
      gParameterScan->SetPoint(iPoint, iValueNew/nEvents, likelihoodValue);
      iPoint++;
    }
    std::string canvasName = "cParameterScan1D_" + iName;
    CreateAndWriteCanvas(canvasName.c_str(), 21, 1, false, false, gParameterScan);
    std::cout << "." << std::flush;
  }
  
  std::cout << "\n";
  
  std::cout << "Producing likelihood scan histograms: normalizing using a single variable" << std::flush;

  if(vPars.size() > 2)
  {
    for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
    {
      for(unsigned int jPar = iPar+1; jPar < vPars.size(); jPar++)
      {
        for(unsigned int kPar = 0; kPar < vPars.size(); kPar++)
        {
          if(kPar == iPar || kPar == jPar)
          {
            continue;
          }
          
          double iValue = vPars[iPar];
          double jValue = vPars[jPar];
          double kValue = vPars[kPar];
          double iError = vParErrors[iPar];
          double jError = vParErrors[jPar];
//           double kError = vParErrors[kPar];
          std::string iName = vParNames[iPar];
          std::string jName = vParNames[jPar];
          std::string kName = vParNames[kPar];
          std::string histoName = "hParameterScan_" + iName + "-" + jName + "-" + kName;
          std::string histoTitle = "Scan around minimum for parameters " + iName + ", " + jName + ", and " + kName;
          std::string histoXTitle = iName + " fraction";
          std::string histoYTitle = jName + " fraction";
          std::string histoZTitle = "Likelihood value";
          int nBinsXScan = 21;
          int nBinsYScan = 21;
          double xMinScan = (iValue - 5*iError) / nEvents;
          double xMaxScan = (iValue + 5*iError) / nEvents;
          double yMinScan = (jValue - 5*jError) / nEvents;
          double yMaxScan = (jValue + 5*jError) / nEvents;
          
          double minLikelihoodValue = 1;
          double maxLikelihoodValue = -1e99;
          
          TH2D* hParameterScan = Create2DHistogram<TH2D>(histoName.c_str(), histoTitle.c_str(), nBinsXScan, xMinScan, xMaxScan, nBinsYScan, yMinScan, yMaxScan, histoXTitle.c_str(), histoYTitle.c_str(), histoZTitle.c_str());
          for(int iBinX = 1 ; iBinX < hParameterScan->GetXaxis()->GetNbins()+1; iBinX++)
          {
            for(int iBinY = 1 ; iBinY < hParameterScan->GetYaxis()->GetNbins()+1; iBinY++)
            {
              double iValueNew = hParameterScan->GetXaxis()->GetBinCenter(iBinX) * nEvents;
              double jValueNew = hParameterScan->GetYaxis()->GetBinCenter(iBinY) * nEvents;
              double iDelta = iValueNew - iValue;
              double jDelta = jValueNew - jValue;
              double kValueNew = kValue - iDelta - jDelta;
              if(iValueNew < 0 || iValueNew > nEvents || jValueNew < 0 || jValueNew > nEvents || kValueNew < 0 || kValueNew > nEvents)
              {
//                 std::cout <<"Outside limits! iValueNew = " << iValueNew << ", jValueNew = " << jValueNew << ", kValueNew = " << kValueNew << std::endl;
                hParameterScan->SetBinContent(iBinX, iBinY,-1e99);
                continue;
              }
              std::vector<double> vParsNew;
              for(unsigned int iParNew = 0; iParNew < vPars.size(); iParNew++)
              {
                if(iParNew == iPar)
                  vParsNew.push_back(iValueNew);
                else if(iParNew == jPar)
                  vParsNew.push_back(jValueNew);
                else if(iParNew == kPar)
                  vParsNew.push_back(kValueNew);
                else
                  vParsNew.push_back(vPars[iParNew]);
              }
              double likelihoodValue = (*likelihoodFCN)(vParsNew);
              if(likelihoodValue < minLikelihoodValue)
                minLikelihoodValue = likelihoodValue;
              if(likelihoodValue > maxLikelihoodValue)
                maxLikelihoodValue = likelihoodValue;
//               std::cout << "Par" << iPar << ": " << vParsNew[iPar] << ", Par " << jPar << ": " << vParsNew[jPar] << ", Par " << kPar << ": " << vParsNew[kPar] <<", likelihood = " << likelihoodValue << std::endl;
              hParameterScan->SetBinContent(iBinX, iBinY,likelihoodValue);
            }
          }
          hParameterScan->SetMinimum(minLikelihoodValue-(maxLikelihoodValue-minLikelihoodValue)/10.);
          hParameterScan->SetMaximum(maxLikelihoodValue+(maxLikelihoodValue-minLikelihoodValue)/10.);
          std::string canvasName = "cParameterScan2D_" + iName + "-" + jName + "-" + kName;
          CreateAndWriteCanvas3D(canvasName.c_str(), false, false, false, 230., 35., hParameterScan);
          std::cout << "." << std::flush;
        }
      }
    }
  }
  
  std::cout << "\n";

  std::cout << "Producing likelihood scan histograms: normalizing using all the other variables" << std::flush;
  
  if(vPars.size() > 1)
  {
    for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
    {
      for(unsigned int jPar = iPar+1; jPar < vPars.size(); jPar++)
      {
        double iValue = vPars[iPar];
        double jValue = vPars[jPar];
        double iError = vParErrors[iPar];
        double jError = vParErrors[jPar];
        std::string iName = vParNames[iPar];
        std::string jName = vParNames[jPar];
        std::string histoName = "hParameterScan_" + iName + "-" + jName;
        std::string histoTitle = "Scan around minimum for parameters " + iName + ", " + jName;
        std::string histoXTitle = iName + " fraction";
        std::string histoYTitle = jName + " fraction";
        std::string histoZTitle = "Likelihood value";
        int nBinsXScan = 21;
        int nBinsYScan = 21;
        double xMinScan = (iValue - 5*iError) / nEvents;
        double xMaxScan = (iValue + 5*iError) / nEvents;
        double yMinScan = (jValue - 5*jError) / nEvents;
        double yMaxScan = (jValue + 5*jError) / nEvents;
        
        double minLikelihoodValue = 1;
        double maxLikelihoodValue = -1e99;
        
        TH2D* hParameterScan = Create2DHistogram<TH2D>(histoName.c_str(), histoTitle.c_str(), nBinsXScan, xMinScan, xMaxScan, nBinsYScan, yMinScan, yMaxScan, histoXTitle.c_str(), histoYTitle.c_str(), histoZTitle.c_str());
        for(int iBinX = 1 ; iBinX < hParameterScan->GetXaxis()->GetNbins()+1; iBinX++)
        {
          for(int iBinY = 1 ; iBinY < hParameterScan->GetYaxis()->GetNbins()+1; iBinY++)
          {
            double iValueNew = hParameterScan->GetXaxis()->GetBinCenter(iBinX) * nEvents;
            double jValueNew = hParameterScan->GetYaxis()->GetBinCenter(iBinY) * nEvents;
//             double iDelta = iValueNew - iValue;
//             double jDelta = jValueNew - jValue;
            if(iValueNew < 0 || iValueNew > nEvents || jValueNew < 0 || jValueNew > nEvents)
            {
              //                 std::cout <<"Outside limits! iValueNew = " << iValueNew << ", jValueNew = " << jValueNew << std::endl;
              hParameterScan->SetBinContent(iBinX, iBinY,-1e99);
              continue;
            }
            double scaleFactor = (1.-iValueNew-jValueNew)/(1.-iValue-jValue);
            if(scaleFactor < 0)
            {
//               std::cout << "Sum outside limits! iValueNew = " << iValueNew << ", jValueNew = " << jValueNew << std::endl;
              hParameterScan->SetBinContent(iBinX, iBinY,-1e99);
              continue;
            }
            std::vector<double> vParsNew;
            for(unsigned int iParNew = 0; iParNew < vPars.size(); iParNew++)
            {
              if(iParNew == iPar)
                vParsNew.push_back(iValueNew);
              else if(iParNew == jPar)
                vParsNew.push_back(jValueNew);
              else
                vParsNew.push_back(vPars[iParNew]*scaleFactor);
            }
            double likelihoodValue = (*likelihoodFCN)(vParsNew);
            if(likelihoodValue < minLikelihoodValue)
              minLikelihoodValue = likelihoodValue;
            if(likelihoodValue > maxLikelihoodValue)
              maxLikelihoodValue = likelihoodValue;
            //               std::cout << "Par" << iPar << ": " << vParsNew[iPar] << ", Par " << jPar << ": " << vParsNew[jPar] << ", Par " << ", likelihood = " << likelihoodValue << std::endl;
            hParameterScan->SetBinContent(iBinX, iBinY,likelihoodValue);
          }
        }
        hParameterScan->SetMinimum(minLikelihoodValue-(maxLikelihoodValue-minLikelihoodValue)/10.);
        hParameterScan->SetMaximum(maxLikelihoodValue+(maxLikelihoodValue-minLikelihoodValue)/10.);
        std::string canvasName = "cParameterScan2D_" + iName + "-" + jName;
        CreateAndWriteCanvas3D(canvasName.c_str(), false, false, false, 230., 35., hParameterScan);
        std::cout << "." << std::flush;
      }
    }
  }
  
  std::cout << "\n";
  
  std::cout << "Producing validation plots and printing results...\n\n";

  TH2D* hSumOfTemplates = Create2DHistogram<TH2D>("hSumOfTemplates", "Weighted sum of templates", nXBins, xLoEdge, xHiEdge, nYBins, yLoEdge, yHiEdge, xTitle.c_str(), yTitle.c_str());
  
  std::vector<TH1D*> vTemplatesProj;
  for(unsigned int i = 0; i < hTemplates.size(); i++)
  {
    name = hTemplates[i]->GetName();
    nameProjX = name + "_P";
    title = hTemplates[i]->GetName();
    titleProjX = title + " - projection";
    TH1D* hTempProj = hTemplates[i]->ProjectionX(nameProjX.c_str());
    hTempProj->SetTitle(titleProjX.c_str());
    vTemplatesProj.push_back(hTempProj);
  }
  
  
  std::vector<TH2D*> vRescaledTemplates;
  std::vector<TH1D*> vRescaledTemplatesProjX;
  std::vector<TH1D*> vRescaledTemplatesProjY;
  
  for(unsigned int i = 0; i < hTemplates.size(); i++)
  {
    hSumOfTemplates->Add(hSumOfTemplates, hTemplates[i], 1., vPars[i]);
    
    name = hTemplates[i]->GetName();
    name+="_Rescaled";
    nameProjX = name + "_Px";
    nameProjY = name + "_Py";
    title = hTemplates[i]->GetName();
    title+=" - Rescaled";
    titleProjX = title + " - projection X";
    titleProjY = title + " - projection Y";
    
    TH2D* hTemp = Create2DHistogram<TH2D>(name.c_str(), title.c_str(), nXBins, xLoEdge, xHiEdge, nYBins, yLoEdge, yHiEdge, xTitle.c_str(), yTitle.c_str());
    hTemp->Add(hTemp, hTemplates[i], 1., vPars[i]);
    vRescaledTemplates.push_back(hTemp);

    TH1D* hTempProjX = hTemp->ProjectionX(nameProjX.c_str());
    hTempProjX->SetTitle(titleProjX.c_str());
    vRescaledTemplatesProjX.push_back(hTempProjX);
    
    TH1D* hTempProjY = hTemp->ProjectionY(nameProjY.c_str());
    hTempProjY->SetTitle(titleProjY.c_str());
    vRescaledTemplatesProjY.push_back(hTempProjY);
    
    std::string canvasName = name.replace(0, 1, "c");
//     std::string canvasName = "prova";
//     oss << i;
//     canvasName += oss.str();
    CreateAndWriteCanvas(canvasName.c_str(), "colz", false, false, true, vRescaledTemplates[i]);

    std::string canvasNameProjX = nameProjX.replace(0, 1, "c");
    CreateAndWriteCanvas(canvasNameProjX.c_str(), 0, 21, 1, false, true, vRescaledTemplatesProjX[i]);
    
    std::string canvasNameProjY = nameProjY.replace(0, 1, "c");
    CreateAndWriteCanvas(canvasNameProjY.c_str(), 0, 21, 1, false, true, vRescaledTemplatesProjY[i]);
    
  }
  
  TH1D* hSumOfTemplatesProjected = hSumOfTemplates->ProjectionX("hSumOfTemplatesProjected");
  TH1D* hDataProjectedX = hData->ProjectionX("hDataProjectedX");
  TH1D* hDataProjectedY = hData->ProjectionY("hDataProjectedY");
  
//   unsigned int colors1D[10] = {1,2,3,4,5,6,7,8,9,10};
//   unsigned int colors1D[20] = {1,kAzure+6,kBlue+1,kGreen+1,kOrange,kMagenta+3,kViolet-4,kRed,9,10,11,12,13,14,15,16,17,18,19,20};
//   unsigned int colors1D[20] = {1,kYellow,kOrange,kOrange-3,kOrange+5,kBlue+4,kBlue,kBlue-6,9,10,11,12,13,14,15,16,17,18,19,20};
  unsigned int colors1D[20] = {kRed,kOrange,kOrange+5,kSpring+5,kSpring+3,kBlue-8,kBlue+1,8,9,10,11,12,13,14,15,16,17,18,19,20};
  
  // BB is always white in this way
  if(vPars.size() < 20)
  {
    colors1D[vPars.size()] = kWhite;
  }
  
  std::vector<TH1*> vDataPlusTemplateHistogramsProjX;
  vDataPlusTemplateHistogramsProjX.push_back((TH1*)hDataProjectedX);
  std::vector<TH1*> vDataPlusTemplateHistogramsProjY;
  vDataPlusTemplateHistogramsProjY.push_back((TH1*)hDataProjectedY);
  
  std::string legendTitle1D = "";
  std::vector<std::string> legendLabels1D;
  legendLabels1D.push_back("Data");
  
//   for(int i = vRescaledTemplatesProjX.size() - 1; i > -1; i--)
  for(unsigned int i = 0; i < vRescaledTemplatesProjX.size(); i++)
  {
    std::string entryName = vRescaledTemplates[ordering[i]]->GetName();
    legendLabels1D.push_back(entryName.substr(1,2));
    vDataPlusTemplateHistogramsProjX.push_back((TH1*)vRescaledTemplatesProjX[ordering[i]]);
    vDataPlusTemplateHistogramsProjY.push_back((TH1*)vRescaledTemplatesProjY[ordering[i]]);
  }
  
  LegendDefinition leg1D = {legendTitle1D, legendLabels1D, "NE"};
  
  CreateAndWriteStackCanvasWithDifferences("cDataFit1DStackDifferenceProjX", "Data vs. stacked fit components with difference - Projection along X", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistogramsProjX);
  CreateAndWriteStackCanvasWithRatios("cDataFit1DStackRatioProjX", "Data vs. stacked fit components with ratio - Projection along X", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistogramsProjX);
  CreateAndWriteStackCanvasWithPulls("cDataFit1DStackPullProjX", "Data vs. stacked fit components with pull - Projection along X", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistogramsProjX);
  CreateAndWriteStackCanvasWithDifferences("cDataFit1DStackDifferenceProjY", "Data vs. stacked fit components with difference - Projection along Y", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistogramsProjY);
  CreateAndWriteStackCanvasWithRatios("cDataFit1DStackRatioProjY", "Data vs. stacked fit components with ratio - Projection along Y", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistogramsProjY);
  CreateAndWriteStackCanvasWithPulls("cDataFit1DStackPullProjY", "Data vs. stacked fit components with pull - Projection along Y", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistogramsProjY);
  
  TH2D* hDataFit2DDifference = Create2DHistogram<TH2D>("cDataFit2DDifference", "Difference (data - fit)", nXBins, xLoEdge, xHiEdge, nYBins, yLoEdge, yHiEdge, xTitle.c_str(), yTitle.c_str());
  hDataFit2DDifference->Add(hData, hSumOfTemplates, 1., -1.);

  TH2D* hDataFit2DRelDifference1 = Create2DHistogram<TH2D>("cDataFit2DRelDifference1", "Difference (data - fit)/fit", nXBins, xLoEdge, xHiEdge, nYBins, yLoEdge, yHiEdge, xTitle.c_str(), yTitle.c_str());
  TH2D* hDataFit2DRelDifference2 = Create2DHistogram<TH2D>("cDataFit2DRelDifference2", "Difference (data - fit)/data", nXBins, xLoEdge, xHiEdge, nYBins, yLoEdge, yHiEdge, xTitle.c_str(), yTitle.c_str());

  TH2D* hDataFit2DPull = Create2DHistogram<TH2D>("cDataFit2DPull", "Pull (data - fit) / (error)", nXBins, xLoEdge, xHiEdge, nYBins, yLoEdge, yHiEdge, xTitle.c_str(), yTitle.c_str());
  
  TH2D* hDataFit2DRatio1 = Create2DHistogram<TH2D>("cDataFit2DRatio1", "Ratio (data / fit)", nXBins, xLoEdge, xHiEdge, nYBins, yLoEdge, yHiEdge, xTitle.c_str(), yTitle.c_str());
  TH2D* hDataFit2DRatio2 = Create2DHistogram<TH2D>("cDataFit2DRatio2", "Ratio (fit/ data)", nXBins, xLoEdge, xHiEdge, nYBins, yLoEdge, yHiEdge, xTitle.c_str(), yTitle.c_str());

  TH1D* hFitPullDistribution = Create1DHistogram<TH1D>("cFitPullDistribution", "Distribution of bin-by-bin fit pulls", 100, -10., 10., "Pulls", "Bins");
  TH1D* hFitPullDistributionGt10 = Create1DHistogram<TH1D>("cFitPullDistributionGt10", "Distribution of bin-by-bin fit pulls for bins with >10 entries", 100, -10., 10., "Pulls", "Bins");
  
  double chiSquare = 0;
  double chiSquareErrData = 0;
  int nPoints = 0;
  double chiSquareGt10 = 0;
  double chiSquareErrDataGt10 = 0;
  int nPointsGt10 = 0;
  for(int xBin = 1; xBin <= nXBins; xBin++)
  {
    for(int yBin = 1; yBin <= nXBins; yBin++)
    {
      double binContent = (hData->GetBinContent(xBin, yBin) - hSumOfTemplates->GetBinContent(xBin, yBin)) / hSumOfTemplates->GetBinContent(xBin, yBin);
      hDataFit2DRelDifference1->SetBinContent(xBin, yBin, binContent);
      
      binContent = (hData->GetBinContent(xBin, yBin) - hSumOfTemplates->GetBinContent(xBin, yBin)) / hData->GetBinContent(xBin, yBin);
      hDataFit2DRelDifference2->SetBinContent(xBin, yBin, binContent);
      
      double error = sqrt(hData->GetBinError(xBin, yBin)*hData->GetBinError(xBin, yBin) + hSumOfTemplates->GetBinError(xBin, yBin)*hSumOfTemplates->GetBinError(xBin, yBin));
      double errorData = hData->GetBinError(xBin, yBin);
      binContent = (hData->GetBinContent(xBin, yBin) - hSumOfTemplates->GetBinContent(xBin, yBin)) / error;
      hDataFit2DPull->SetBinContent(xBin, yBin, binContent);
      hFitPullDistribution->Fill(binContent);
      if(hData->GetBinContent(xBin, yBin) > 10)
      {
        hFitPullDistributionGt10->Fill(binContent);
      }
      
      binContent = hData->GetBinContent(xBin, yBin) / hSumOfTemplates->GetBinContent(xBin, yBin);
      hDataFit2DRatio1->SetBinContent(xBin, yBin, binContent);

      binContent = hSumOfTemplates->GetBinContent(xBin, yBin) / hData->GetBinContent(xBin, yBin);
      hDataFit2DRatio2->SetBinContent(xBin, yBin, binContent);
      
      if(hData->GetBinContent(xBin, yBin) > 0)
      {
        double value = (hSumOfTemplates->GetBinContent(xBin, yBin) - hData->GetBinContent(xBin, yBin))/error;
        double valueErrData = (hSumOfTemplates->GetBinContent(xBin, yBin) - hData->GetBinContent(xBin, yBin))/errorData;
        //         std::cout << "value*value = " << value*value << std::endl;
        chiSquare+= value*value;
        chiSquareErrData+=valueErrData*valueErrData;
        nPoints++;
        if(hData->GetBinContent(xBin, yBin) > 10)
        {
          chiSquareGt10+=value*value;
          chiSquareErrDataGt10+=valueErrData*valueErrData;
          nPointsGt10++;
        }
      }
    }
  }

  std::fstream outputTextFile("FitResults.txt",std::ios_base::out);
  double integralFromFit = 0;
  for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
  {
    integralFromFit+=tfm->GetParameter(iPar);
  }
  
  for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
  {
    outputTextFile << hTemplates[iPar]->GetName() << "\t" << tfm->GetParameter(iPar)/integralFromFit << "\t" << tfm->GetParError(iPar)/integralFromFit << std::endl;
  }
  
  outputTextFile << "chi2 = " << chiSquare << std::endl;
  outputTextFile << "chi2 (only data error) = " << chiSquareErrData << std::endl;
  outputTextFile << "nPoints = " << nPoints << std::endl;
  outputTextFile << "reduced chi2 (assuming 4 free parameters) = " << chiSquare/(nPoints-4) << std::endl;
  outputTextFile << "reduced chi2 (only data error, assuming 4 free parameters) = " << chiSquareErrData/(nPoints-4) << std::endl;
  outputTextFile << "chi2 (bins with >10 entries) = " << chiSquareGt10 << std::endl;
  outputTextFile << "chi2 (bins with >10 entries, only data error) = " << chiSquareErrDataGt10 << std::endl;
  outputTextFile << "nPoints (bins with >10 entries) = " << nPointsGt10 << std::endl;
  outputTextFile << "reduced chi2 (bins with >10 entries, assuming 4 free parameters) = " << chiSquareGt10/(nPointsGt10-4) << std::endl;
  outputTextFile << "reduced chi2 (bins with >10 entries, only data error, assuming 4 free parameters) = " << chiSquareErrDataGt10/(nPointsGt10-4) << std::endl;
  outputTextFile << "\nChi2 test probabilities:\n";
  outputTextFile << "UU: " << hData->Chi2Test(hSumOfTemplates,"UU P") << std::endl;
  outputTextFile << "UW: " << hData->Chi2Test(hSumOfTemplates,"UW P") << std::endl;
  outputTextFile << "WW: " << hData->Chi2Test(hSumOfTemplates,"WW P") << std::endl;
  outputTextFile << "UU NORM: " << hData->Chi2Test(hSumOfTemplates,"UU NORM P") << std::endl;
  outputTextFile << "\nKolmogorov tests:\n";
  outputTextFile << "Probability: " << hData->KolmogorovTest(hSumOfTemplates,"D") << std::endl;
  outputTextFile << "Maximum distance: " << hData->KolmogorovTest(hSumOfTemplates,"M D") << std::endl;
  outputTextFile << "Probability N: " << hData->KolmogorovTest(hSumOfTemplates,"N D") << std::endl;
  outputTextFile << "Maximum distance N: " << hData->KolmogorovTest(hSumOfTemplates,"M N D") << std::endl;
  outputTextFile << "Probability X: " << hData->KolmogorovTest(hSumOfTemplates,"D X") << std::endl;
  outputTextFile << "Maximum distance X: " << hData->KolmogorovTest(hSumOfTemplates,"M D X") << std::endl;
  
  outputTextFile << "\nChi2 test probabilities of projections:\n";
  outputTextFile << "UU: " << hDataProjectedX->Chi2Test(hSumOfTemplatesProjected,"UU P") << std::endl;
  outputTextFile << "UW: " << hDataProjectedX->Chi2Test(hSumOfTemplatesProjected,"UW P") << std::endl;
  outputTextFile << "WW: " << hDataProjectedX->Chi2Test(hSumOfTemplatesProjected,"WW P") << std::endl;
  outputTextFile << "UU NORM: " << hDataProjectedX->Chi2Test(hSumOfTemplatesProjected,"UU NORM P") << std::endl;
  outputTextFile << "\nKolmogorov tests of projections:\n";
  outputTextFile << "Probability: " << hDataProjectedX->KolmogorovTest(hSumOfTemplatesProjected,"D") << std::endl;
  outputTextFile << "Maximum distance: " << hDataProjectedX->KolmogorovTest(hSumOfTemplatesProjected,"M D") << std::endl;
  outputTextFile << "Probability N: " << hDataProjectedX->KolmogorovTest(hSumOfTemplatesProjected,"N D") << std::endl;
  outputTextFile << "Maximum distance N: " << hDataProjectedX->KolmogorovTest(hSumOfTemplatesProjected,"M N D") << std::endl;
  outputTextFile << "Probability X: " << hDataProjectedX->KolmogorovTest(hSumOfTemplatesProjected,"D X") << std::endl;
  outputTextFile << "Maximum distance X: " << hDataProjectedX->KolmogorovTest(hSumOfTemplatesProjected,"M D X") << std::endl;
  
  
  outputTextFile << "\n--- Fit Infos ---\n\n";

  outputTextFile << "FitStatus = " << status << std::endl;
  
  std::streambuf *psbuf, *backup;
  backup = std::cout.rdbuf();           // back up cout's streambuf
  psbuf = outputTextFile.rdbuf();       // get file's streambuf
  
  std::cout.rdbuf(psbuf);               // assign streambuf to cout

  tfm->PrintResults(4,0);

  std::cout.rdbuf(backup);              // restore cout's original streambuf
  
  outputTextFile << "\nnDataEvents = " << nDataEvents << std::endl;

  // print latex table of number of events
  outputTextFile << "\n"
                 << "\\toprule\n"
                 << "comp\t&\tvalue\t&\terror\t&\trelerr\t\\\\\n"
                 << "\\midrule\n";
  for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
  {
    std::string tname(tfm->GetParName(iPar));
    tname.erase(0,4);
    double val = tfm->GetParameter(iPar);
    double err = tfm->GetParError(iPar);
    double rel = (val < 1. ? -999. : err/val*100.);
    outputTextFile << Form("%s\t&\t%.0f\t&\t%.0f\t&\t",tname.c_str(),val,err)
                   << (rel < 0. ? "-\t\\\\\n" : Form("%.2f\t\\\\\n",rel));   
  }
  outputTextFile << "\\bottomrule\n"
                 << Form("$\\chi^2$/ndof\t&\t%.2f\t&\t\t&\t\\\\\n",chiSquare/(nPoints-4));
  
  
  // print latex table of number of fractions
  outputTextFile << "\n"
       << "\\toprule\n"
       << "comp\t&\tvalue\t&\terror\t&\trelerr\t\\\\\n"
       << "\\midrule\n";
  for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
  {    
    std::string tname(tfm->GetParName(iPar));
    tname.erase(0,4);
    double val = tfm->GetParameter(iPar)/nEvents*100;
    double err = tfm->GetParError(iPar)/nEvents*100;
    double rel = (val < .1 ? -999. : err/val*100.);
    outputTextFile << Form("%s\t&\t%.2f\t&\t%.2f\t&\t",tname.c_str(),val,err)
              << (rel < 0. ? "-\t\\\\\n" : Form("%.2f\t\\\\\n",rel));
  }
  outputTextFile << "\\bottomrule\n"
                 << Form("$\\chi^2$/ndof\t&\t%.2f\t&\t\t&\t\\\\\n",chiSquare/(nPoints-4));

  outputTextFile.close();
  
//   for(unsigned int iPar = 0; iPar < vPars.size(); iPar++)
//   {
//     std::cout << hTemplates[iPar]->GetName() << "\t" << tfm->GetParameter(iPar)/integralFromFit << "\t" << tfm->GetParError(iPar)/integralFromFit << std::endl;
//   }
//   
//   std::cout << "chi2 = " << chiSquare << std::endl;
//   std::cout << "chi2 (only data error) = " << chiSquareErrData << std::endl;
//   std::cout << "nPoints = " << nPoints << std::endl;
//   std::cout << "reduced chi2 (assuming 4 free parameters) = " << chiSquare/(nPoints-4) << std::endl;
//   std::cout << "reduced chi2 (only data error, assuming 4 free parameters) = " << chiSquareErrData/(nPoints-4) << std::endl;
//   std::cout << "chi2 (bins with >10 entries) = " << chiSquareGt10 << std::endl;
//   std::cout << "chi2 (bins with >10 entries, only data error) = " << chiSquareErrDataGt10 << std::endl;
//   std::cout << "nPoints (bins with >10 entries) = " << nPointsGt10 << std::endl;
//   std::cout << "reduced chi2 (bins with >10 entries, assuming 4 free parameters) = " << chiSquareGt10/(nPointsGt10-4) << std::endl;
//   std::cout << "reduced chi2 (bins with >10 entries, only data error, assuming 4 free parameters) = " << chiSquareErrDataGt10/(nPointsGt10-4) << std::endl;
  
  
  
  CreateAndWriteCanvas("cSumOfTemplates", "colz", false, false, true, hSumOfTemplates);
  CreateAndWriteCanvas("cData", "colz", false, false, true, hData);
  CreateAndWriteCanvas3D("cData3D", false, false, true, 230., 35., hData);
  CreateAndWriteCanvas("hDataFit2DDifference", "colz", false, false, false, hDataFit2DDifference);
  CreateAndWriteCanvas("hDataFit2DRelDifference1", "colz", false, false, false, hDataFit2DRelDifference1);
  CreateAndWriteCanvas("hDataFit2DRelDifference2", "colz", false, false, false, hDataFit2DRelDifference2);
  CreateAndWriteCanvas("hDataFit2DPull", "colz", false, false, false, hDataFit2DPull);
  CreateAndWriteCanvas("hDataFit2DRatio1", "colz", false, false, false, hDataFit2DRatio1);
  CreateAndWriteCanvas("hDataFit2DRatio2", "colz", false, false, false, hDataFit2DRatio2);
  
  CreateAndWriteCanvas("cSumOfTemplatesProjected", 0, 21, 1, false, true, hSumOfTemplatesProjected);
  
  CreateAndWriteCanvas("cDataProjectedX", 0, 21, 1, false, true, hDataProjectedX);
  CreateAndWriteCanvas("cDataProjectedY", 0, 21, 1, false, true, hDataProjectedY);

  CreateAndWriteCanvas("cFitPullDistribution", 0, 21, 1, false, false, hFitPullDistribution);
  CreateAndWriteCanvas("cFitPullDistributionGt10", 0, 21, 1, false, false, hFitPullDistributionGt10);
  
  unsigned int lineStyles[2] = {0, 1};
  unsigned int markerStyles[2] = {21, 0};
  unsigned int colors[2] = {1, 2};

  std::string legendTitle = "CMS preliminary - 7 TeV";
  std::vector<std::string> legendLabels;
  legendLabels.push_back("Data");
  legendLabels.push_back("Fit results");
  std::string legendPosition = "NE";
  LegendDefinition leg = {legendTitle, legendLabels, legendPosition};
  
  
  CreateAndWriteCanvasWithDifferences("cDataVsFitDiffProjX", lineStyles, markerStyles, colors, leg, false, true, hDataProjectedX, hSumOfTemplatesProjected, NULL);
  CreateAndWriteCanvasWithPulls("cDataVsFitPullProjX", lineStyles, markerStyles, colors, leg, false, true, hDataProjectedX, hSumOfTemplatesProjected, NULL);
  CreateAndWriteCanvasWithRatios("cDataVsFitRatioProjX", lineStyles, markerStyles, colors, leg, false, true, hDataProjectedX, hSumOfTemplatesProjected, NULL);

  CreateAndWriteCanvasWithDifferences("cDataVsFitDiffProjY", lineStyles, markerStyles, colors, leg, false, true, hDataProjectedY, hSumOfTemplatesProjected, NULL);
  CreateAndWriteCanvasWithPulls("cDataVsFitPullProjY", lineStyles, markerStyles, colors, leg, false, true, hDataProjectedY, hSumOfTemplatesProjected, NULL);
  CreateAndWriteCanvasWithRatios("cDataVsFitRatioProjY", lineStyles, markerStyles, colors, leg, false, true, hDataProjectedY, hSumOfTemplatesProjected, NULL);

  
  TH1D* hFitProjXPullDistribution = Create1DHistogram<TH1D>("hFitProjXPullDistribution", "Distribution of bin-by-bin fit pulls for x projection of fit",50,-5.,5.,"Pulls","Bins");
  TH1D* hFitProjYPullDistribution = Create1DHistogram<TH1D>("hFitProjYPullDistribution", "Distribution of bin-by-bin fit pulls for y projection of fit",50,-5.,5.,"Pulls","Bins");
  
  for(int xBin = 1; xBin <= nXBins; xBin++)
  {
    double errorProjX = hDataProjectedX->GetBinError(xBin);
//     double errorTemplatesProjX = hSumOfTemplatesProjected->GetBinError(xBin);
    double errorTemplatesProjX = sqrt(hSumOfTemplatesProjected->GetBinContent(xBin));
//     double binContentProjX =  (hDataProjectedX->GetBinContent(xBin) - hSumOfTemplatesProjected->GetBinContent(xBin)) / errorProjX;
    double binContentProjX =  (hDataProjectedX->GetBinContent(xBin) - hSumOfTemplatesProjected->GetBinContent(xBin)) / sqrt(errorProjX*errorProjX + errorTemplatesProjX*errorTemplatesProjX);
    hFitProjXPullDistribution->Fill(binContentProjX);
  }
  for(int yBin = 1; yBin <= nYBins; yBin++)
  {
    double errorProjY = hDataProjectedY->GetBinError(yBin);
//     double errorTemplatesProjY = hSumOfTemplatesProjected->GetBinError(yBin);
    double errorTemplatesProjY = sqrt(hSumOfTemplatesProjected->GetBinContent(yBin));
//     double binContentProjY =  (hDataProjectedY->GetBinContent(yBin) - hSumOfTemplatesProjected->GetBinContent(yBin)) / errorProjY;
    double binContentProjY =  (hDataProjectedY->GetBinContent(yBin) - hSumOfTemplatesProjected->GetBinContent(yBin)) / sqrt(errorProjY*errorProjY + errorTemplatesProjY*errorTemplatesProjY);
    hFitProjYPullDistribution->Fill(binContentProjY);
  }
  
  CreateAndWriteCanvas("hcFitProjXPullDistribution", 1, 0, 1, false, false, hFitProjXPullDistribution);
  CreateAndWriteCanvas("hcFitProjYPullDistribution", 1, 0, 1, false, false, hFitProjYPullDistribution);
  
  if(vTemplatesProj.size() == 7)
  {
    std::string legendTitle1DForTemplates = "CMS Simulation";
    std::vector<std::string> legendLabels1DForTemplates1;
    std::string name1DTemplate = vTemplatesProj[0]->GetName();
    legendLabels1DForTemplates1.push_back(name1DTemplate.substr(1,2));
    name1DTemplate = vTemplatesProj[1]->GetName();
    legendLabels1DForTemplates1.push_back(name1DTemplate.substr(1,2));
    name1DTemplate = vTemplatesProj[4]->GetName();
    legendLabels1DForTemplates1.push_back(name1DTemplate.substr(1,2));
    name1DTemplate = vTemplatesProj[3]->GetName();
    legendLabels1DForTemplates1.push_back(name1DTemplate.substr(1,2));
  
    unsigned int markerStyles1DTemplates[7] = {0,0,0,0,0,0,0};
    unsigned int lineStyles1DTemplates[7] = {1,1,1,1,1,1,1};
    unsigned int colors1DTemplates1[4] = {colors1D[7]==kWhite?1:colors1D[7],colors1D[6],colors1D[5],colors1D[1]};
    
    LegendDefinition leg1DTemplates1 = {legendTitle1DForTemplates,legendLabels1DForTemplates1,"NE"};
    CreateAndWriteCanvas("cTemplates1",lineStyles1DTemplates,markerStyles1DTemplates,colors1DTemplates1,leg1DTemplates1,false,true, vTemplatesProj[0],vTemplatesProj[1],vTemplatesProj[4],vTemplatesProj[3],NULL);
    
    std::vector<std::string> legendLabels1DForTemplates2;
    name1DTemplate = vTemplatesProj[2]->GetName();
    legendLabels1DForTemplates2.push_back(name1DTemplate.substr(1,2));
    name1DTemplate = vTemplatesProj[5]->GetName();
    legendLabels1DForTemplates2.push_back(name1DTemplate.substr(1,2));
    name1DTemplate = vTemplatesProj[6]->GetName();
    legendLabels1DForTemplates2.push_back(name1DTemplate.substr(1,2));

    unsigned int colors1DTemplates2[3] = {colors1D[3],colors1D[4],colors1D[2]};
    
    LegendDefinition leg1DTemplates2 = {legendTitle1DForTemplates,legendLabels1DForTemplates2,"NE"};
    CreateAndWriteCanvas("cTemplates2",lineStyles1DTemplates,markerStyles1DTemplates,colors1DTemplates2,leg1DTemplates2,false,true, vTemplatesProj[2],vTemplatesProj[5],vTemplatesProj[6],NULL);
  }
}



#endif // FitResults2DPlotter_cxx
