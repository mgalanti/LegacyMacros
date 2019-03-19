#ifndef BinnedLogLikelihood1D_h
#define BinnedLogLikelihood1D_h



#include "BinnedLogLikelihoodBase.h"

#include "TH1.h"
#include "TMath.h"



class BinnedLogLikelihood1D : public BinnedLogLikelihoodBase
{
 public:
   BinnedLogLikelihood1D(std::vector<TH1D*>& hMC, TH1D* hData);
   virtual double operator() (const std::vector<double>&) const;

 private:
   // Definitions of arrays storing 1D templates and data
   // templateBinContents[#template][#bin];
   // dataBinContents[#bin];
   std::vector<std::vector<double> >  templateBinContents;
   std::vector<double>  dataBinContents;
};



BinnedLogLikelihood1D::BinnedLogLikelihood1D(std::vector<TH1D*>& hMC, TH1D* hData)
{
  nTemplates = hMC.size();
  nBins = hData->GetXaxis()->GetNbins();
  
  for(unsigned int iTemplate = 0; iTemplate < nTemplates; iTemplate++)
  {
    std::vector<double> vec;
    templateBinContents.push_back(vec);
    for(unsigned int iBin = 0; iBin <= nBins; iBin++)
    {
      templateBinContents[iTemplate].push_back(hMC[iTemplate]->GetBinContent(iBin));      
    }
  }
  for(unsigned int iBin = 0; iBin <= nBins; iBin++)
  {
    dataBinContents.push_back(hData->GetBinContent(iBin));
  }  

}



double BinnedLogLikelihood1D::operator() (const std::vector<double>& par) const
{
  double l = 0.;
//   double ll = 0.;
  double lh = 0.;
//   double logFactorial;

  for(unsigned int iBin = 1; iBin <= nBins; iBin++)
  {
    l = 0.;
//     ll = 0.;
//     logFactorial = 0.;
    for(unsigned int iTemplate = 0; iTemplate < nTemplates; iTemplate++)
    {
      l += (par[iTemplate] * templateBinContents[iTemplate][iBin]);
    }
    lh += dataBinContents[iBin] * TMath::Log(l) - l;
  }
  return -2*lh;
}



#endif // BinnedLogLikelihood1D_h
