#ifndef FitResults1DPlotter_cxx
#define FitResults1DPlotter_cxx

#include <iostream>
#include <fstream>
#include <sstream>

#include "TFile.h"

#include "FitResults1DPlotter.h"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodBase.h"



FitResults1DPlotter::~FitResults1DPlotter()
{
  outputFile->Close();
  gROOT->GetListOfCanvases()->Clear();
  TDirectory* dir = gROOT->GetDirectory(curDirName.c_str());
  dir->cd();
}



void FitResults1DPlotter::Initialize()
{
  curDirName = gDirectory->GetPathStatic();
  outputFile = new TFile(outputFileName.c_str(),"RECREATE");
  outputFile->cd();
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



void FitResults1DPlotter::CreateAndSavePlots()
{
  std::ostringstream oss;
  oss.str("");

  std::string name, nameProjX, nameProjY;
  std::string title, titleProjX, titleProjY;
  
  int nXBins = hTemplates[0]->GetXaxis()->GetNbins();
  double xLoEdge =  hTemplates[0]->GetXaxis()->GetXmin();
  double xHiEdge =  hTemplates[0]->GetXaxis()->GetXmax();
  std::string xTitle = hTemplates[0]->GetXaxis()->GetTitle();
  std::string yTitle = "N. muons";
  
  std::cout << "Producing likelihood scan graphs: normalizing using a single variable";
  
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

  std::cout << "Producing likelihood scan graphs: normalizing using all the other variables";
  
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
        //           std::cout <<"Outside limits! shift = " << shift << ", iValueNew = " << iValueNew << std::endl;
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
      //         std::cout << "Par" << iPar << ": " << vParsNew[iPar] << ", likelihood = " << likelihoodValue << std::endl;
      gParameterScan->Set(gParameterScan->GetN()+1);
      gParameterScan->SetPoint(iPoint, iValueNew/nEvents, likelihoodValue);
      iPoint++;
    }
    std::string canvasName = "cParameterScan1D_" + iName;
    CreateAndWriteCanvas(canvasName.c_str(), 21, 1, false, false, gParameterScan);
    std::cout << "." << std::flush;
  }
  
  std::cout << "\n";
  
  std::cout << "Producing likelihood scan histograms: normalizing using a single variable";

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

  std::cout << "Producing likelihood scan histograms: normalizing using all the other variables";
  
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
              std::cout << "Sum outside limits! iValueNew = " << iValueNew << ", jValueNew = " << jValueNew << std::endl;
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

  TH1D* hSumOfTemplates = Create1DHistogram<TH1D>("hSumOfTemplates", "Weighted sum of templates", nXBins, xLoEdge, xHiEdge, xTitle.c_str(), yTitle.c_str());
  
  std::vector<TH1D*> vRescaledTemplates;
  
  for(unsigned int i = 0; i < hTemplates.size(); i++)
  {
    hSumOfTemplates->Add(hSumOfTemplates, hTemplates[i], 1., vPars[i]);
    
    name = hTemplates[i]->GetName();
    name+="_Rescaled";
    title = hTemplates[i]->GetName();
    title+=" - Rescaled";
    
    TH1D* hTemp = Create1DHistogram<TH1D>(name.c_str(), title.c_str(), nXBins, xLoEdge, xHiEdge, xTitle.c_str(), yTitle.c_str());
    hTemp->Add(hTemp, hTemplates[i], 1., vPars[i]);
    vRescaledTemplates.push_back(hTemp);

    std::string canvasName = name.replace(0, 1, "c");
    CreateAndWriteCanvas(canvasName.c_str(), 0, 21, 1, false, true, vRescaledTemplates[i]);
  }
  
//   unsigned int colors1D[10] = {1,2,3,4,5,6,7,8,9,10};
//   unsigned int colors1D[20] = {1,kAzure+6,kBlue+1,kGreen+1,kOrange,kMagenta+3,kViolet-4,kRed,9,10,11,12,13,14,15,16,17,18,19,20};
//   unsigned int colors1D[20] = {1,kYellow,kOrange,kOrange-3,kOrange+5,kBlue+4,kBlue,kBlue-6,9,10,11,12,13,14,15,16,17,18,19,20};
  unsigned int colors1D[20] = {kRed,kOrange,kOrange+5,kSpring+5,kSpring+3,kBlue-8,kBlue+1,8,9,10,11,12,13,14,15,16,17,18,19,20};

  colors1D[ordering.size()] = kWhite;
  
  std::vector<TH1*> vDataPlusTemplateHistograms;
  vDataPlusTemplateHistograms.push_back((TH1*)hData);
  
  std::string legendTitle1D = "";
  std::vector<std::string> legendLabels1D;
  legendLabels1D.push_back("Data");
  
//   for(int i = vRescaledTemplatesProjX.size() - 1; i > -1; i--)
  for(unsigned int i = 0; i < vRescaledTemplates.size(); i++)
  {
    std::string entryName = vRescaledTemplates[ordering[i]]->GetName();
    legendLabels1D.push_back(entryName.substr(1,1));
    vDataPlusTemplateHistograms.push_back((TH1*)vRescaledTemplates[ordering[i]]);
  }
  
  LegendDefinition leg1D = {legendTitle1D, legendLabels1D, "NE"};
  
  CreateAndWriteStackCanvasWithDifferences("cDataFit1DStackDifference", "Data vs. stacked fit components with difference", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistograms);
  CreateAndWriteStackCanvasWithRatios("cDataFit1DStackRatio", "Data vs. stacked fit components with ratio", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistograms);
  CreateAndWriteStackCanvasWithPulls("cDataFit1DStackPull", "Data vs. stacked fit components with pull", 20, colors1D, leg1D, false, true, vDataPlusTemplateHistograms);
  
  TH1D* hDataFitDifference = Create1DHistogram<TH1D>("cDataFitDifference", "Difference (data - fit)", nXBins, xLoEdge, xHiEdge, xTitle.c_str(), "data - fit");
  hDataFitDifference->Add(hData, hSumOfTemplates, 1., -1.);

  TH1D* hDataFitRelDifference1 = Create1DHistogram<TH1D>("cDataFitRelDifference1", "Difference (data - fit)/fit", nXBins, xLoEdge, xHiEdge, xTitle.c_str(), "(data - fit) / fit");
  TH1D* hDataFitRelDifference2 = Create1DHistogram<TH1D>("cDataFitRelDifference2", "Difference (data - fit)/data", nXBins, xLoEdge, xHiEdge, xTitle.c_str(), "(data - fit) / data");

  TH1D* hDataFitPull = Create1DHistogram<TH1D>("cDataFitPull", "Pull (data - fit) / (error)", nXBins, xLoEdge, xHiEdge, xTitle.c_str(), "Pull");
  
  TH1D* hDataFitRatio1 = Create1DHistogram<TH1D>("cDataFitRatio1", "Ratio (data / fit)", nXBins, xLoEdge, xHiEdge, xTitle.c_str(), "data / fit");
  TH1D* hDataFitRatio2 = Create1DHistogram<TH1D>("cDataFitRatio2", "Ratio (fit/ data)", nXBins, xLoEdge, xHiEdge, xTitle.c_str(), "fit / data");

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
    double binContent = (hData->GetBinContent(xBin) - hSumOfTemplates->GetBinContent(xBin)) / hSumOfTemplates->GetBinContent(xBin);
    hDataFitRelDifference1->SetBinContent(xBin, binContent);
    
    binContent = (hData->GetBinContent(xBin) - hSumOfTemplates->GetBinContent(xBin)) / hData->GetBinContent(xBin);
    hDataFitRelDifference2->SetBinContent(xBin, binContent);
    
    double error = sqrt(hData->GetBinError(xBin)*hData->GetBinError(xBin) + hSumOfTemplates->GetBinError(xBin)*hSumOfTemplates->GetBinError(xBin));
    double errorData = hData->GetBinError(xBin);
    binContent = (hData->GetBinContent(xBin) - hSumOfTemplates->GetBinContent(xBin)) / error;
    hDataFitPull->SetBinContent(xBin, binContent);
    hFitPullDistribution->Fill(binContent);
    if(hData->GetBinContent(xBin) > 10)
    {
      hFitPullDistributionGt10->Fill(binContent);
    }
    
    binContent = hData->GetBinContent(xBin) / hSumOfTemplates->GetBinContent(xBin);
    hDataFitRatio1->SetBinContent(xBin, binContent);
    
    binContent = hSumOfTemplates->GetBinContent(xBin) / hData->GetBinContent(xBin);
    hDataFitRatio2->SetBinContent(xBin, binContent);
    
    if(hData->GetBinContent(xBin) > 0)
    {
      double value = (hSumOfTemplates->GetBinContent(xBin) - hData->GetBinContent(xBin))/error;
      double valueErrData = (hSumOfTemplates->GetBinContent(xBin) - hData->GetBinContent(xBin))/errorData;
      //         std::cout << "value*value = " << value*value << std::endl;
      chiSquare+= value*value;
      chiSquareErrData+=valueErrData*valueErrData;
      nPoints++;
      if(hData->GetBinContent(xBin) > 10)
      {
        chiSquareGt10+=value*value;
        chiSquareErrDataGt10+=valueErrData*valueErrData;
        nPointsGt10++;
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
  
  outputTextFile << "\n--- Fit Infos ---\n\n";

  outputTextFile << "FitStatus = " << status << std::endl;

  std::streambuf *psbuf, *backup;
  backup = std::cout.rdbuf();           // back up cout's streambuf
  psbuf = outputTextFile.rdbuf();       // get file's streambuf
  
  std::cout.rdbuf(psbuf);               // assign streambuf to cout

  tfm->PrintResults(1,0);

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
    double val = tfm->GetParameter(iPar)/nDataEvents*100;
    double err = tfm->GetParError(iPar)/nDataEvents*100;
    double rel = (val < .1 ? -999. : err/val*100.);
    outputTextFile << Form("%s\t&\t%.2f\t&\t%.2f\t&\t",tname.c_str(),val,err)
                   << (rel < 0. ? "-\t\\\\\n" : Form("%.2f\t\\\\\n",rel));
  }
  outputTextFile << "\\bottomrule\n"
                 << Form("$\\chi^2$/ndof\t&\t%.2f\t&\t\t&\t\\\\\n",chiSquare/(nPoints-4));

  outputTextFile.close();
  
  CreateAndWriteCanvas("cData", 0, 21, 1, false, true, hData);
  CreateAndWriteCanvas("cSumOfTemplates", 0, 21, 1, false, true, hSumOfTemplates);
  
  CreateAndWriteCanvas("hDataFitDifference", 0, 21, 1, false, false, hDataFitDifference);
  CreateAndWriteCanvas("hDataFitRelDifference1", 0, 21, 1, false, false, hDataFitRelDifference1);
  CreateAndWriteCanvas("hDataFitRelDifference2", 0, 21, 1, false, false, hDataFitRelDifference2);
  CreateAndWriteCanvas("hDataFitPull", 0, 21, 1, false, false, hDataFitPull);
  CreateAndWriteCanvas("hDataFitRatio1", 0, 21, 1, false, false, hDataFitRatio1);
  CreateAndWriteCanvas("hDataFitRatio2", 0, 21, 1, false, false, hDataFitRatio2);
  
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
  
  
  CreateAndWriteCanvasWithDifferences("cDataVsFitDiff", lineStyles, markerStyles, colors, leg, false, true, hData, hSumOfTemplates, NULL);
  CreateAndWriteCanvasWithPulls("cDataVsFitPull", lineStyles, markerStyles, colors, leg, false, true, hData, hSumOfTemplates, NULL);
  CreateAndWriteCanvasWithRatios("cDataVsFitRatio", lineStyles, markerStyles, colors, leg, false, true, hData, hSumOfTemplates, NULL);

  std::vector<TH1*> vTemplates;
  for(unsigned int iTemplate = 0; iTemplate < hTemplates.size(); iTemplate++)
  {
    vTemplates.push_back(dynamic_cast<TH1*>(hTemplates[iTemplate]));
  }
  
  if(vTemplates.size() == 7)
  {
    std::string legendTitle1DForTemplates = "CMS Simulation";
    std::vector<std::string> legendLabels1DForTemplates1;
    std::string name1DTemplate = vTemplates[0]->GetName();
    legendLabels1DForTemplates1.push_back(name1DTemplate.substr(1,1));
    name1DTemplate = vTemplates[1]->GetName();
    legendLabels1DForTemplates1.push_back(name1DTemplate.substr(1,1));
    name1DTemplate = vTemplates[4]->GetName();
    legendLabels1DForTemplates1.push_back(name1DTemplate.substr(1,1));
    name1DTemplate = vTemplates[3]->GetName();
    legendLabels1DForTemplates1.push_back(name1DTemplate.substr(1,1));
  
    unsigned int markerStyles1DTemplates[7] = {0,0,0,0,0,0,0};
    unsigned int lineStyles1DTemplates[7] = {1,1,1,1,1,1,1};
    unsigned int colors1DTemplates1[4] = {colors1D[7]==kWhite?1:colors1D[7],colors1D[6],colors1D[5],colors1D[1]};
    
    LegendDefinition leg1DTemplates1 = {legendTitle1DForTemplates,legendLabels1DForTemplates1,"NE"};
    CreateAndWriteCanvas("cTemplates1",lineStyles1DTemplates,markerStyles1DTemplates,colors1DTemplates1,leg1DTemplates1,false,true, vTemplates[0],vTemplates[1],vTemplates[4],vTemplates[3],NULL);
    
    std::vector<std::string> legendLabels1DForTemplates2;
    name1DTemplate = vTemplates[2]->GetName();
    legendLabels1DForTemplates2.push_back(name1DTemplate.substr(1,1));
    name1DTemplate = vTemplates[5]->GetName();
    legendLabels1DForTemplates2.push_back(name1DTemplate.substr(1,1));
    name1DTemplate = vTemplates[6]->GetName();
    legendLabels1DForTemplates2.push_back(name1DTemplate.substr(1,1));

    unsigned int colors1DTemplates2[3] = {colors1D[3],colors1D[4],colors1D[2]};
    
    LegendDefinition leg1DTemplates2 = {legendTitle1DForTemplates,legendLabels1DForTemplates2,"NE"};
    CreateAndWriteCanvas("cTemplates2",lineStyles1DTemplates,markerStyles1DTemplates,colors1DTemplates2,leg1DTemplates2,false,true, vTemplates[2],vTemplates[5],vTemplates[6],NULL);
  }
  else
  {
    std::string legendTitle1DForTemplates = "CMS Simulation";
    std::vector<std::string> legendLabels1DForTemplates;
    std::string name1DTemplate;
    for(unsigned int iTemplate = 0; iTemplate < vTemplates.size(); iTemplate++)
    {
      name1DTemplate = vTemplates[iTemplate]->GetName();
      legendLabels1DForTemplates.push_back(name1DTemplate.substr(1,1));
    }
    unsigned int markerStyles1DTemplates[20] = {0};
    unsigned int lineStyles1DTemplates[20] = {1};
    unsigned int colors1DTemplates[20] = {colors1D[7]==kWhite?1:colors1D[7],colors1D[6],colors1D[5],colors1D[1]};
    for(unsigned int i = 0; i < 20; i++)
    {
      colors1DTemplates[i] = colors1D[i]==kWhite?1:colors1D[i];
    }
    LegendDefinition leg1DTemplates = {legendTitle1DForTemplates,legendLabels1DForTemplates,"NE"};
    CreateAndWriteCanvas("cTemplates",lineStyles1DTemplates,markerStyles1DTemplates,colors1DTemplates,leg1DTemplates,false,true, vTemplates);
  }
}



#endif // FitResults2DPlotter_cxx
