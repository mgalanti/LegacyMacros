#include <TROOT.h>
#include <Minuit2/FCNBase.h>
#include <TH2.h>
#include <TH3.h>
#include <iostream>
#include <TMath.h>
#include <TFile.h>

//this is the class definition needed by Minuit to minimize the function here defined
class Likelihood_calculator : public ROOT::Minuit2::FCNBase
{

public:

  //contructor
  Likelihood_calculator(std::vector<TH2D*>&, TH2D*, Double_t, Double_t);
  Likelihood_calculator(std::vector<TH1D*>&, TH1D*, Double_t, Double_t);

  //destructor
  ~Likelihood_calculator(){;}

  //operatr needed to manipulate the parameter to minimize
  Double_t operator() (const std::vector<double>&) const;

  //function fUP nedded by Minuit. fUp = 1. means that the minimization is with chi2. fUp = 0.5 means likelihood.
  //ma se la funzione gli passa -2Ln(Likelihood(x)) allora va bene up = 1.
  // http://seal.web.cern.ch/seal/documents/minuit/mnerror.pdf
  double Up() const { return 1.0; }

private:

  //number of events of the spectrum
  Int_t nevents;
  //array dimension
  Int_t array_dim;
  //number of templates
  Int_t mc_size;

  //definition of storing array for bidimensional templates and data
  // Double_t  mc_array[5][50][50];
  // Double_t  data[50][50];
  vector<vector<vector<Double_t> > >  mc_array;
  vector<vector<Double_t> >  data;

  //definition of storing arrau for monodimensional templates and data
  // Double_t  one_mc_array[5][50];
  // Double_t  one_data[50];
  vector<vector<Double_t> >  one_mc_array;
  vector<Double_t>  one_data;

  Bool_t bidimensional  ;
  Bool_t monodimensional;

};

//class constructor bidiensional fit
Likelihood_calculator::Likelihood_calculator(std::vector<TH2D*>& h_mc, TH2D* h_data, Double_t tfraction_fitrange_min, Double_t tfraction_fitrange_max){

  bidimensional = true;
  monodimensional = false;

  std::cout << "Using 2D histograms..." << std::endl;

  mc_size = (Int_t)h_mc.size(); // Numero dei parametri:BB,BP,CC,CP,PP
  cout << "mc_size = " << mc_size << endl;

  array_dim = h_data->GetXaxis()->FindBin(tfraction_fitrange_max)-1;
  cout << "array_dim = " << array_dim << endl;

  std::cout << "Creating the array from the Distribution Functions..." << std::endl;

  //filling MC con allocazione fissa

  for(int dim = 0; dim < mc_size ; dim++){
    vector<vector<Double_t> >  dummyy;
    mc_array.push_back(dummyy);
    for(int i = 0; i < array_dim; i++){
    vector<Double_t>  dummyy2;
    mc_array.at(dim).push_back(dummyy2);
      for(int j = 0; j < array_dim; j++){
        // mc_array[dim][i][j] = h_mc[dim]->GetBinContent(i+1,j+1);
        mc_array.at(dim).at(i).push_back( h_mc[dim]->GetBinContent(i+1,j+1) );
      // cout << "h_mc["<<dim<<"]->GetBinContent("<<i+1<<","<<j+1<<")= " <<
      // /* h_mc[dim]->GetBinCenter(i+1) << " : " << */ h_mc[dim]->GetBinContent(i+1,j+i) << endl;
      }
    }
  }
    cout << "mc_array.size() = "<< mc_array.size() << endl;
  cout << "mc_array.at(0).size() = "<< mc_array.at(0).size() << endl;
  cout << "mc_array.at(0).at(0).size() = "<< mc_array.at(0).at(0).size() << endl;


  std::cout << "Creating the array from the data..." << std::endl;

  nevents = h_data->Integral(h_data->GetXaxis()->FindBin(tfraction_fitrange_min),h_data->GetXaxis()->FindBin(tfraction_fitrange_max)-1,
  h_data->GetYaxis()->FindBin(tfraction_fitrange_min),h_data->GetYaxis()->FindBin(tfraction_fitrange_max)-1);

  std::cout << "N events = " << nevents << std::endl;

  //filling data array con allocazione fissa
  for(int i = 0; i < array_dim; i++){
    vector<Double_t>  dummyy2;
    data.push_back(dummyy2);
    for(int j = 0; j < array_dim; j++){
      // data[i][j] = h_data->GetBinContent(i+1,j+1);
      data.at(i).push_back(h_data->GetBinContent(i+1,j+1));
      // cout << "h_data->GetBinContent("<<i+1<<","<<j+1<<")= " <<
      // /* h_mc[dim]->GetBinCenter(i+1) << " : " << */ h_data->GetBinContent(i+1,j+i) << endl;
    }
  }
  cout << "data.size() = "<< data.size() << endl;
  cout << "data.at(0).size() = "<< data.at(0).size() << endl;

}


//class constructor monodimensional fit
Likelihood_calculator::Likelihood_calculator(std::vector<TH1D*>& h_mc, TH1D* h_data, Double_t tfraction_fitrange_min, Double_t tfraction_fitrange_max){

  monodimensional = true;
  bidimensional = false;

  std::cout << "Using 1D histograms..." << std::endl;

  mc_size = (Int_t)h_mc.size(); // Numero dei parametri:BB,BP,CC,CP,PP

  cout << "mc_size = " << mc_size << endl;

  array_dim = h_data->GetXaxis()->FindBin(tfraction_fitrange_max)-1;
  cout << "array_dim = " << array_dim << endl;

  std::cout << "Creating the array from the Distribution Functions..." << std::endl;

  //filling MC con allocazione fissa

  for(int dim = 0; dim < mc_size ; dim++){
    vector<Double_t>  dummyy;
    one_mc_array.push_back(dummyy);
    for(int i = 0; i < array_dim; i++){
      // one_mc_array[dim][i] = h_mc[dim]->GetBinContent(i+1);
      one_mc_array.at(dim).push_back( h_mc[dim]->GetBinContent(i+1) );
      // cout << "h_mc["<<dim<<"]->GetBinContent("<<i+1<<")= " <<
      // h_mc[dim]->GetBinCenter(i+1) << " : " << h_mc[dim]->GetBinContent(i+1) << endl;
    }
    cout << endl;
  }

  cout << "one_mc_array.size() = "<< one_mc_array.size() << endl;
  cout << "one_mc_array.at(0).size() = "<< one_mc_array.at(0).size() << endl;

  std::cout << "Creating the array from the data..." << std::endl;

  nevents = h_data->Integral(h_data->GetXaxis()->FindBin(tfraction_fitrange_min),h_data->GetXaxis()->FindBin(tfraction_fitrange_max)-1);

  std::cout << "N events = " << nevents << std::endl;

  //filling data array con allocazione fissa
  for(int i = 0; i < array_dim; i++){
  // one_data[i] = h_data->GetBinContent(i);
  one_data.push_back(h_data->GetBinContent(i+1));
  // cout << "h_data->GetBinContent("<<i+1<<")= " << h_data->GetBinContent(i+1) << endl;
  }
  cout << "one_data.size() = "<< one_data.size() << endl;

}


Double_t Likelihood_calculator::operator()(const std::vector<double>& par) const
{

  // SIMPLE REFERENCE: http://hepunx.rl.ac.uk/~adye/thesis/html/node51.html

  Double_t l;
  Double_t ll;
  Double_t lh;
  Double_t logFactorial;

  //   std::cout << "Calcolo della likelihood..." << std::endl;

  lh = 0.;

  if(bidimensional == true){

    //computation of the bidimensional likelihood to maximize
    for(int i = 1; i < array_dim; i++){
      for(int j = 0; j < array_dim; j++){
        l = 0.;
        ll = 0.;
        logFactorial = 0.;

        for(int dim = 0; dim < mc_size ; dim++) l += (par[dim] * mc_array[dim][i][j]);

        // for(int k = 1; k <= data[i][j]; k++) logFactorial += TMath::Log(k);

        lh += data[i][j] * TMath::Log(l)  - l/*  - logFactorial  */;

      }
    }
  }

  else if(monodimensional == true){

    //computation of the monodimensional likelihood to maximize
    for(int i = 0; i < array_dim; i++){
      l = 0.;
      ll = 0.;
      logFactorial = 0.;

      for(int dim = 0; dim < mc_size ; dim++) l += (par[dim] * /* one_mc_array[dim][i] */ one_mc_array.at(dim).at(i));

      // for(int k = 1; k <= one_data.at(i); k++) logFactorial += TMath::Log(k);

      lh += one_data.at(i) * TMath::Log(l)  - l /* - logFactorial */;

      // cout <<"array_dim = " << array_dim << " lh = " << lh << endl;

    }

  }

    // std::cout << "LIKELIHOOD FINALE =  " << (-2*lh) << std::endl;

  return (-2*lh);

}

