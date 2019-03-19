#include "../utilities/HistoPlotter.C"

#include <iostream>
#include "TSystem.h"
#include "TFile.h"
#include "TLegendEntry.h"
#include "TColor.h"
#include "TPaveText.h"
#include "TRandom3.h"

void makeInvariantMassPlots()
{
//   gSystem->CompileMacro("../utilities/HistoPlotter.C");
  
  int rebinFactor = 2;
  
  HistoPlotter* p = new HistoPlotter();
  
  TFile* fileBs = new TFile("BmmL1TTImplementation__Histograms__MC__Bs2MuMu_TuneZ2star_14TeV_Pythia6__TTI2023Upg14D-PU140bx25_PH2_1K_FB_V3-v3__NoSelections_TTStudies_1__20140803.root", "READ");
  TFile* fileBd = new TFile("BmmL1TTImplementation__Histograms__MC__Bd2MuMu_TuneZ2star_14TeV_Pythia6__TTI2023Upg14D-PU140bx25_PH2_1K_FB_V3-v1__NoSelections_TTStudies_1__20140803.root", "READ");
  
  std::cout << "sono qui 1\n";
  
  TH1D* histoBs = (TH1D*)(fileBs->Get("hDiMuInvMass"));
  TH1D* histoBd = (TH1D*)(fileBd->Get("hDiMuInvMass"));
  std::cout << "sono qui 2\n";
  
  int realBsIntegral = histoBs->Integral();
  int realBdIntegral = histoBd->Integral();
  
  int targetBsIntegral = 15895;
  int targetBdIntegral = 1695;
  
  histoBs->Scale((double)targetBsIntegral/(double)realBsIntegral);
  histoBd->Scale((double)targetBdIntegral/(double)realBdIntegral);
  
  histoBd->Rebin(rebinFactor);
  histoBs->Rebin(rebinFactor);
  
  // Randomize bin counts in the sidebands
  double totalIntegralBsLeft = 0;
  double totalIntegralBsRight = 0;
  double totalIntegralBdLeft = 0;
  double totalIntegralBdRight = 0;
  double totalIntegralBsTail = 0;
  double totalIntegralBdTail = 0;
  // Step 1: get integrals in the sidebands
  for(unsigned int iBin = 1; iBin < histoBs->GetXaxis()->GetNbins(); iBin++)
  {
    double totalIntegral = 0;
    double binCenter = histoBs->GetXaxis()->GetBinCenter(iBin);
    if(binCenter < 4.8)
    {
      totalIntegralBsLeft+=histoBs->GetBinContent(iBin);
      totalIntegralBdLeft+=histoBd->GetBinContent(iBin);
    }
    if(binCenter < 5.0 && binCenter >= 4.8)
    {
      totalIntegralBsTail+=histoBs->GetBinContent(iBin);
      totalIntegralBdTail+=histoBd->GetBinContent(iBin);
    }
    if(binCenter > 5.8)
    {
      totalIntegralBsRight+=histoBs->GetBinContent(iBin);
      totalIntegralBdRight+=histoBd->GetBinContent(iBin);
    }
  }
  // Step 2: put contents to zero
  for(unsigned int iBin = 1; iBin < histoBs->GetXaxis()->GetNbins(); iBin++)
  {
    double binCenter = histoBs->GetXaxis()->GetBinCenter(iBin);
    if(binCenter < 5.0)
    {
      histoBs->SetBinContent(iBin,0);
      histoBd->SetBinContent(iBin,0);
    }
    if(binCenter > 5.8)
    {
      histoBs->SetBinContent(iBin,0);
      histoBd->SetBinContent(iBin,0);
    }
  }
  // Step 3: put random contents
  double totalIntegralBdSidebands = totalIntegralBdLeft + totalIntegralBdRight;
  double totalIntegralBsSidebands = totalIntegralBsLeft + totalIntegralBsRight;
  TRandom3* rnd = new TRandom3(999);
  for(unsigned int i = 0; i < totalIntegralBsSidebands * 4.8/9.1; i++)
  {
    histoBs->Fill(rnd->Rndm()*4.8);
  }
  for(unsigned int i = 0; i < totalIntegralBsSidebands * 4.3/9.1; i++)
  {
    histoBs->Fill(5.7+rnd->Rndm()*4.3);
  }
  for(unsigned int i = 0; i < totalIntegralBdSidebands * 4.8/9.1; i++)
  {
    histoBd->Fill(rnd->Rndm()*4.8);
  }
  for(unsigned int i = 0; i < totalIntegralBdSidebands * 4.2/9.1; i++)
  {
    histoBd->Fill(5.7+rnd->Rndm()*4.3);
  }
  for(unsigned int i = 0; i < totalIntegralBdTail; i++)
  {
    histoBd->Fill(4.8+rnd->Rndm()*0.2);
  }
  for(unsigned int i = 0; i < totalIntegralBsTail; i++)
  {
    histoBs->Fill(4.8+rnd->Rndm()*0.2);
  }
  
  TH1D* histoSum = (TH1D*)histoBs->Clone("histoSum");
  std::cout << "sono qui 3\n";
  
  histoSum->Add(histoBd);
  std::cout << "sono qui 4\n";
  
  
  TFile* fileOut = new TFile("superimposedPlots_20141016.root","RECREATE");
  std::cout << "sono qui 5\n";
  
  fileOut->cd();
  std::cout << "sono qui 6\n";
  
  unsigned int lineStyles[4] = {1,1,1,1};
  unsigned int markerStyles[4] = {0,0,0,0};
  unsigned int colors[4] = {2,4,1,3};
  std::cout << "sono qui 7\n";
  
  HistoPlotter::LegendDefinition leg;
  leg.title = "";
  leg.labels.push_back("B_{s}#rightarrow#mu^{+}#mu^{-}");
  leg.labels.push_back("B_{d}#rightarrow#mu^{+}#mu^{-}");
  leg.labels.push_back("Sum");
  leg.position = "NE";
  std::cout << "sono qui 8\n";
  
  std::cout << "histoSum = " << histoSum << std::endl;
  std::cout << "histoBd = " << histoBd << std::endl;
  std::cout << "histoBs = " << histoBs << std::endl;
 
  unsigned int nBinsBkg = 1000 / rebinFactor;
  double integralBkg = 2e10;
  double binContentBkg = integralBkg / nBinsBkg;
  TH1D *histoBkg = new TH1D("histoBkg","",nBinsBkg,0,10);
  for(unsigned int iBin = 1; iBin < nBinsBkg; iBin++)
  {
    histoBkg->SetBinContent(iBin,binContentBkg);
  }
  histoBkg->SetMinimum(3.062794);
  histoBkg->SetMaximum(8.771587e+08);
  histoBkg->SetEntries(2e+10);
  histoBkg->SetDirectory(0);
  histoBkg->SetStats(0);
  histoBkg->SetLineColor(2);
  histoBkg->SetLineStyle(2);
  histoBkg->SetLineWidth(2);
  histoBkg->SetMarkerColor(2);
  histoBkg->SetMarkerStyle(0);
  histoBkg->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
  histoBkg->GetXaxis()->SetRangeUser(3.9,6.9);
  histoBkg->GetXaxis()->SetLabelFont(42);
  histoBkg->GetXaxis()->SetLabelOffset(0.007);
  histoBkg->GetXaxis()->SetLabelSize(0.035);
  histoBkg->GetXaxis()->SetTitleSize(0.05);
  histoBkg->GetXaxis()->SetTitleOffset(1.1);
  histoBkg->GetXaxis()->SetTitleFont(42);
  histoBkg->GetYaxis()->SetTitle("Events / (0.02 GeV) ");
  histoBkg->GetYaxis()->SetLabelFont(42);
  histoBkg->GetYaxis()->SetLabelOffset(0.007);
  histoBkg->GetYaxis()->SetLabelSize(0.035);
  histoBkg->GetYaxis()->SetTitleSize(0.05);
  histoBkg->GetYaxis()->SetTitleOffset(1.5);
  histoBkg->GetYaxis()->SetTitleFont(42);
  histoBkg->GetZaxis()->SetLabelFont(42);
  histoBkg->GetZaxis()->SetLabelOffset(0.007);
  histoBkg->GetZaxis()->SetLabelSize(0.035);
  histoBkg->GetZaxis()->SetTitleSize(0.05);
  histoBkg->GetZaxis()->SetTitleFont(42);
  
  TCanvas *cInvariantMass_logScale = new TCanvas("cInvariantMass_logScale", "",0,85,600,660);
  cInvariantMass_logScale->SetFillColor(0);
  cInvariantMass_logScale->SetBorderMode(0);
  cInvariantMass_logScale->SetBorderSize(2);
  cInvariantMass_logScale->SetLogy();
  cInvariantMass_logScale->SetTickx(1);
  cInvariantMass_logScale->SetTicky(1);
  cInvariantMass_logScale->SetLeftMargin(0.18);
  cInvariantMass_logScale->SetRightMargin(0.035);
  cInvariantMass_logScale->SetBottomMargin(0.14);
  cInvariantMass_logScale->SetFrameFillStyle(0);
  cInvariantMass_logScale->SetFrameBorderMode(0);
  cInvariantMass_logScale->SetFrameFillStyle(0);
  cInvariantMass_logScale->SetFrameBorderMode(0);
  histoBs->SetMinimum(0.5);
  histoBs->SetMaximum(4e+08);
  histoBs->SetEntries(2329);
  histoBs->SetDirectory(0);
  histoBs->SetStats(0);
  histoBs->SetFillColor(2);
  histoBs->SetFillStyle(3001);
  histoBs->SetLineColor(2);
  histoBs->SetLineWidth(2);
  histoBs->SetMarkerColor(2);
  histoBs->SetMarkerStyle(0);
  histoBs->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
  histoBs->GetXaxis()->SetRangeUser(3.9,6.9);
  histoBs->GetXaxis()->SetLabelFont(42);
  histoBs->GetXaxis()->SetLabelOffset(0.007);
  histoBs->GetXaxis()->SetLabelSize(0.035);
  histoBs->GetXaxis()->SetTitleSize(0.05);
  histoBs->GetXaxis()->SetTitleOffset(1.1);
  histoBs->GetXaxis()->SetTitleFont(42);
  histoBs->GetYaxis()->SetTitle("Events / (0.02 GeV) ");
  histoBs->GetYaxis()->SetLabelFont(42);
  histoBs->GetYaxis()->SetLabelOffset(0.007);
  histoBs->GetYaxis()->SetLabelSize(0.035);
  histoBs->GetYaxis()->SetTitleSize(0.05);
  histoBs->GetYaxis()->SetTitleOffset(1.5);
  histoBs->GetYaxis()->SetTitleFont(42);
  histoBs->GetZaxis()->SetLabelFont(42);
  histoBs->GetZaxis()->SetLabelOffset(0.007);
  histoBs->GetZaxis()->SetLabelSize(0.035);
  histoBs->GetZaxis()->SetTitleSize(0.05);
  histoBs->GetZaxis()->SetTitleFont(42);
  histoBs->SetTitle("");
  histoBs->Draw("hist");
  
  histoBkg->Draw("hist same");
  
  histoBd->SetMinimum(3.062794);
  histoBd->SetMaximum(8.771587e+08);
  histoBd->SetEntries(1651);
  histoBd->SetDirectory(0);
  histoBd->SetStats(0);
  
  Int_t ci;      // for color index setting
  TColor *color; // for color definition with alpha
  ci = TColor::GetColor("#cc00ff");
  histoBd->SetFillColor(ci);
  histoBd->SetFillStyle(3001);
  
  ci = TColor::GetColor("#cc00ff");
  histoBd->SetLineColor(ci);
  histoBd->SetLineWidth(2);
  
  ci = TColor::GetColor("#cc00ff");
  histoBd->SetMarkerColor(ci);
  histoBd->SetMarkerStyle(0);
  histoBd->GetXaxis()->SetTitle("M [GeV]");
  histoBd->GetXaxis()->SetRangeUser(3.9,6.9);
  histoBd->GetXaxis()->SetNdivisions(505);
  histoBd->GetXaxis()->SetLabelFont(42);
  histoBd->GetXaxis()->SetLabelOffset(0.007);
  histoBd->GetXaxis()->SetLabelSize(0.035);
  histoBd->GetXaxis()->SetTitleSize(0.05);
  histoBd->GetXaxis()->SetTitleOffset(1.1);
  histoBd->GetXaxis()->SetTitleFont(42);
  histoBd->GetYaxis()->SetTitle("N. events");
  histoBd->GetYaxis()->SetLabelFont(42);
  histoBd->GetYaxis()->SetLabelOffset(0.007);
  histoBd->GetYaxis()->SetLabelSize(0.035);
  histoBd->GetYaxis()->SetTitleSize(0.05);
  histoBd->GetYaxis()->SetTitleOffset(1.5);
  histoBd->GetYaxis()->SetTitleFont(42);
  histoBd->GetZaxis()->SetLabelFont(42);
  histoBd->GetZaxis()->SetLabelOffset(0.007);
  histoBd->GetZaxis()->SetLabelSize(0.035);
  histoBd->GetZaxis()->SetTitleSize(0.05);
  histoBd->GetZaxis()->SetTitleFont(42);
  histoBd->Draw("hist f same");
  
  histoSum->SetMinimum(3.062794);
  histoSum->SetMaximum(8.771587e+08);
  histoSum->SetEntries(3980);
  histoSum->SetDirectory(0);
  histoSum->SetStats(0);
  histoSum->SetLineWidth(2);
  histoSum->SetMarkerStyle(0);
  histoSum->GetXaxis()->SetTitle("M [GeV]");
  histoSum->GetXaxis()->SetRangeUser(3.9,6.9);
  histoSum->GetXaxis()->SetNdivisions(505);
  histoSum->GetXaxis()->SetLabelFont(42);
  histoSum->GetXaxis()->SetLabelOffset(0.007);
  histoSum->GetXaxis()->SetLabelSize(0.035);
  histoSum->GetXaxis()->SetTitleSize(0.05);
  histoSum->GetXaxis()->SetTitleOffset(1.1);
  histoSum->GetXaxis()->SetTitleFont(42);
  histoSum->GetYaxis()->SetTitle("N. events");
  histoSum->GetYaxis()->SetLabelFont(42);
  histoSum->GetYaxis()->SetLabelOffset(0.007);
  histoSum->GetYaxis()->SetLabelSize(0.035);
  histoSum->GetYaxis()->SetTitleSize(0.05);
  histoSum->GetYaxis()->SetTitleOffset(1.5);
  histoSum->GetYaxis()->SetTitleFont(42);
  histoSum->GetZaxis()->SetLabelFont(42);
  histoSum->GetZaxis()->SetLabelOffset(0.007);
  histoSum->GetZaxis()->SetLabelSize(0.035);
  histoSum->GetZaxis()->SetTitleSize(0.05);
  histoSum->GetZaxis()->SetTitleFont(42);
  histoSum->Draw("hist same");
  
  TPaveText *pt = new TPaveText(3.9,0.25e+8,6.27041,2.617948e+08,"br");
  pt->SetBorderSize(0);
  pt->SetFillColor(0);
  pt->SetFillStyle(0);
  pt->SetTextAlign(11);
  pt->SetTextFont(42);
  pt->SetTextSize(0.036);
  TText *text = pt->AddText("CMS Simulation Preliminary");
//   text = pt->AddText("Scaled to L = 3000 fb^{-1}");
  pt->Draw();
  
  TLegend *leg1 = new TLegend(5.6,50000.,6.8394295,2.3527407e+07,NULL,"br");
  leg1->SetBorderSize(0); 
  leg1->SetTextSize(0.036);
  leg1->SetLineColor(1);
  leg1->SetLineStyle(1);
  leg1->SetLineWidth(1);
  leg1->SetFillColor(0);
  leg1->SetFillStyle(0);
  TLegendEntry *entry=leg1->AddEntry("NULL","B_{s}#rightarrow#mu^{+}#mu^{-}","f");
  entry->SetFillColor(2);
  entry->SetFillStyle(3001);
  entry->SetLineColor(2);
  entry->SetLineStyle(1);
  entry->SetLineWidth(2);
  entry->SetMarkerColor(1);
  entry->SetMarkerStyle(21);
  entry->SetMarkerSize(1);
  entry->SetTextFont(42);
  entry=leg1->AddEntry("NULL","B_{d}#rightarrow#mu^{+}#mu^{-}","f");
  
  ci = TColor::GetColor("#cc00ff");
  entry->SetFillColor(ci);
  entry->SetFillStyle(3001);
  
  ci = TColor::GetColor("#cc00ff");
  entry->SetLineColor(ci);
  entry->SetLineStyle(1);
  entry->SetLineWidth(2);
  entry->SetMarkerColor(1);
  entry->SetMarkerStyle(21);
  entry->SetMarkerSize(1);
  entry->SetTextFont(42);
  entry=leg1->AddEntry("histoBkg","Background","l");
  entry->SetLineColor(2);
  entry->SetLineStyle(2);
  entry->SetLineWidth(2);
  entry->SetMarkerColor(1);
  entry->SetMarkerStyle(21);
  entry->SetMarkerSize(1);
  entry->SetTextFont(42);
  entry=leg1->AddEntry("NULL","Total signal","l");
  entry->SetLineColor(1);
  entry->SetLineStyle(1);
  entry->SetLineWidth(2);
  entry->SetMarkerColor(1);
  entry->SetMarkerStyle(21);
  entry->SetMarkerSize(1);
  entry->SetTextFont(42);
  leg1->Draw();
  
  pt = new TPaveText(3.9,1509.0895,6.475899,1.3527407e+07,"br");
  pt->SetBorderSize(0);
  pt->SetFillColor(0);
  pt->SetFillStyle(0);
  pt->SetLineColor(0);
  pt->SetTextAlign(11);
  pt->SetTextFont(42);
  pt->SetTextSize(0.036);
//   text = pt->AddText("L1 Muon + Track Trigger");
  text = pt->AddText("Scaled to L = 3000 fb^{-1}");
  text = pt->AddText("");
  text = pt->AddText("L1TrkMu (PhaseII) Trigger");
  text = pt->AddText("p_{T}(#mu) > 3 GeV");
  text = pt->AddText("|#eta(#mu)| < 2");
  text = pt->AddText("p_{T}(#mu#mu) > 4 GeV");
  text = pt->AddText("|#eta(#mu#mu)| < 2");
  text = pt->AddText("|#Delta d_{z}(#mu#mu)| < 1 cm");
  text = pt->AddText("3.9 < m(#mu#mu) < 6.9 GeV");
  pt->Draw();
  
  cInvariantMass_logScale->Write();
  
  std::cout << "sono qui 9\n";
  
  p->CreateAndWriteCanvas("cInvariantMass",lineStyles,markerStyles,colors,leg1, false, false, histoBs, histoBd, histoSum, NULL);
  std::cout << "sono qui 10\n";

  
  histoBd->Fit("gaus","","",4.,7.);
  p->CreateAndWriteCanvas("cBd",0, 21, 1, false, false, histoBd);
  
  histoBs->Fit("gaus","","",4.,7.);
  p->CreateAndWriteCanvas("cBs",0, 21, 1, false, false, histoBs);
  
  fileOut->Write();
  fileOut->Close();
  fileBd->Close();
  fileBs->Close();
  std::cout << "sono qui 11\n";
  
}
