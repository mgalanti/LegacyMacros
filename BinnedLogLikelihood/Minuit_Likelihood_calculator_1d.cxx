/****************************************************************/
/* VERSIONE MOLTO VECCHIA DA SISTEMARE                          */
/* Questa dovrebbe essere usata con i dati veri                 */
/* Pier 06 / Maggio / 2010                                      */
/****************************************************************/

#include "TROOT.h"
#include "TFitterMinuit.h"
#include "BinnedLogLikelihood1D.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include <vector>
#include <iostream>
#include <sstream>

void Minuit_Likelihood_calculator_1d()
{

  Double_t tfraction_fitrange_min = 0.0;
  Double_t tfraction_fitrange_max = 0.19;

  TFile* MC_File = new TFile("../MuonTemplatesProducer__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__muonV0Pt3.root");
  TFile* Data_File = new TFile("../MuonTemplatesProducer__Histograms__MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1__muonV0Pt3.root");
  Data_File->cd();
  TH1D*data_original_newtemplates[3];

  data_original_newtemplates[0]=(TH1D*)Data_File->Get("hMuonDxyTemplate_-17");  // BB
  data_original_newtemplates[1]=(TH1D*)Data_File->Get("hMuonDxyTemplate_-24");  // CC
  data_original_newtemplates[2]=(TH1D*)Data_File->Get("hMuonDxyTemplate_-25");  // PP

  TH1D* data_original=(TH1D*)data_original_newtemplates[0]->Clone("data_original_init");  // BB
  data_original->Add(data_original_newtemplates[1]);
  data_original->Add(data_original_newtemplates[2]);

  TH1D *hbb_MC_original, *hcc_MC_original, *hpp_MC_original;
  hbb_MC_original = (TH1D*)MC_File->Get("hMuonDxyTemplate_17");
  hbb_MC_original->Scale(1/hbb_MC_original->Integral(hbb_MC_original->GetXaxis()->FindBin(tfraction_fitrange_min),hbb_MC_original->GetXaxis()->FindBin(tfraction_fitrange_max)-1));
  hcc_MC_original = (TH1D*)MC_File->Get("hMuonDxyTemplate_24");
  hcc_MC_original->Scale(1/hcc_MC_original->Integral(hcc_MC_original->GetXaxis()->FindBin(tfraction_fitrange_min),hcc_MC_original->GetXaxis()->FindBin(tfraction_fitrange_max)-1));
  hpp_MC_original = (TH1D*)MC_File->Get("hMuonDxyTemplate_25");
  hpp_MC_original->Scale(1/hpp_MC_original->Integral(hpp_MC_original->GetXaxis()->FindBin(tfraction_fitrange_min),hpp_MC_original->GetXaxis()->FindBin(tfraction_fitrange_max)-1));


  // std::vector<TH2D *> template_vector;
  std::vector<TH1D *> template_vector;

  std::cout << "Creating the vector of TH1D..." << std::endl;

  //all components
  template_vector.push_back(hbb_MC_original);
  template_vector.push_back(hcc_MC_original);
  template_vector.push_back(hpp_MC_original);

  cout << endl;
  cout << "template_vector.size() = "<<template_vector.size()<< endl;
  cout << endl;
  cout << "hbb_MC_original->Integral() = "<<hbb_MC_original->Integral(hbb_MC_original->GetXaxis()->FindBin(tfraction_fitrange_min),hbb_MC_original->GetXaxis()->FindBin(tfraction_fitrange_max)-1)<< endl;
  cout << endl;

  cout << "hcc_MC_original->Integral() = "<<hcc_MC_original->Integral(hcc_MC_original->GetXaxis()->FindBin(tfraction_fitrange_min),hcc_MC_original->GetXaxis()->FindBin(tfraction_fitrange_max)-1)<< endl;
  cout << endl;

  cout << "hpp_MC_original->Integral() = "<<hpp_MC_original->Integral(hpp_MC_original->GetXaxis()->FindBin(tfraction_fitrange_min),hpp_MC_original->GetXaxis()->FindBin(tfraction_fitrange_max)-1)<< endl;
  cout << endl;
  cout << endl;

  TH1D *dxy1_dxy2_spectrum = (TH1D*)data_original;
  Double_t events = dxy1_dxy2_spectrum->Integral(dxy1_dxy2_spectrum->GetXaxis()->FindBin(tfraction_fitrange_min),dxy1_dxy2_spectrum->GetXaxis()->FindBin(tfraction_fitrange_max)-1);

  cout << "dxy1_dxy2_spectrum->Integral() = "<<dxy1_dxy2_spectrum->Integral(dxy1_dxy2_spectrum->GetXaxis()->FindBin(tfraction_fitrange_min),dxy1_dxy2_spectrum->GetXaxis()->FindBin(tfraction_fitrange_max)-1)<< endl;
  cout << endl;
  cout << endl;

  std::cout << "Creating the Likelihood_calculator..." << std::endl;

  BinnedLogLikelihood1D* likelihood_min = new BinnedLogLikelihood1D(template_vector, dxy1_dxy2_spectrum);

  std::cout << "Calling the Minuit..." << std::endl;

  //queste definizioni servono se uso Minfit->ExecuteCommand(char *method,double *arg, int narg);
  //   double * args;
  //   args[0] = 100;
  //   args[1] = 0.1;

  TFitterMinuit *Minfit = new TFitterMinuit;
  Minfit->Clear();
  Minfit->CreateMinimizer();
  Minfit->SetMinuitFCN(likelihood_min);

  vector<Double_t> par;
  vector<Double_t> err_par;

  std::cout << "Setting the parameters of the minimisation..." << std::endl;

  for(int i = 0; i < (int)template_vector.size(); i++){
    err_par.push_back(TMath::Sqrt(events));
  }

  Minfit->SetParameter(0,"Par_BB", 0.80*events,err_par.at(0),0.,events);
  Minfit->SetParameter(1,"Par_CC", 0.1*events, err_par.at(1),0.,events);
  Minfit->SetParameter(2,"Par_PP", 0.1*events,  err_par.at(2),0.,events);

  //   std::cout << "Execute command ..." << std::endl;
  //   Minfit->ExecuteCommand("sca",args,2);

  cout << endl;
  std::cout << "Inizio la minimizzazione" << std::endl;
  Minfit->Minimize(1000,0.1);

  std::cout << "---- Risultati della minimizzazione ----" << std::endl;
  Minfit->PrintResults(4,0.);

  cout << "BB = " << Form("%3.2f",(Minfit->GetParameter(0) * 100 / events )) <<" +/- "<< Form("%3.2f",(Minfit->GetParError(0) * 100 / events )) <<
  "  -  MC truth = " << Form("%3.2f",data_original_newtemplates[0]->Integral(data_original_newtemplates[0]->GetXaxis()->FindBin(tfraction_fitrange_min),data_original_newtemplates[0]->GetXaxis()->FindBin(tfraction_fitrange_max)-1)
  *100./ events)
  << endl;
  cout << "CC = " << Form("%3.2f",(Minfit->GetParameter(1) * 100 / events )) <<" +/- "<< Form("%3.2f",(Minfit->GetParError(1) * 100 / events )) <<
  "  -  MC truth = " << Form("%3.2f",data_original_newtemplates[1]->Integral(data_original_newtemplates[1]->GetXaxis()->FindBin(tfraction_fitrange_min),data_original_newtemplates[1]->GetXaxis()->FindBin(tfraction_fitrange_max)-1)
  *100./events)
  << endl;
  cout << "PP = " << Form("%3.2f",(Minfit->GetParameter(2)* 100 / events )) <<" +/- "<< Form("%3.2f",(Minfit->GetParError(2) * 100 / events )) <<
  "  -  MC truth = " << Form("%3.2f",data_original_newtemplates[2]->Integral(data_original_newtemplates[2]->GetXaxis()->FindBin(tfraction_fitrange_min),data_original_newtemplates[2]->GetXaxis()->FindBin(tfraction_fitrange_max)-1)
  *100./events)
  << endl;

  cout << endl;

}
