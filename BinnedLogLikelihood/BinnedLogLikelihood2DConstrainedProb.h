#ifndef BinnedLogLikelihood2DConstrainedProb_h
#define BinnedLogLikelihood2DConstrainedProb_h



#include "BinnedLogLikelihoodBase.h"

#include "TH2.h"
#include "TMath.h"



class BinnedLogLikelihood2DConstrainedProb : public BinnedLogLikelihoodBase
{
 public:
   BinnedLogLikelihood2DConstrainedProb(std::vector<TH2D*>& hMC, TH2D* hData, unsigned int removeBin);
   virtual double operator() (const std::vector<double>&) const;

 private:
   // Definitions of arrays storing 1D templates and data
   // templateBinContents[#template][#binx][#biny];
   // dataBinContents[#bin][#biny];
   std::vector<std::vector<std::vector<double> > > templateBinContents;
   std::vector<std::vector<double> > dataBinContents;
   unsigned int removeBin;

   double totEvents;
};



BinnedLogLikelihood2DConstrainedProb::BinnedLogLikelihood2DConstrainedProb(std::vector<TH2D*>& hMC, TH2D* hData, unsigned int removeBin_)
{
  removeBin = removeBin_;
  nTemplates = hMC.size();
  nBins = hData->GetXaxis()->GetNbins();

  totEvents=hData->Integral();

  for(unsigned int iTemplate = 0; iTemplate < nTemplates; iTemplate++)
  {
    std::vector<std::vector<double> > vecx;
    templateBinContents.push_back(vecx);
    for(unsigned int iBinx = 0; iBinx <= nBins; iBinx++)
    {
      std::vector<double> vecy;
      templateBinContents[iTemplate].push_back(vecy);
      for(unsigned int iBiny = 0; iBiny <= nBins; iBiny++)
      {
        templateBinContents[iTemplate][iBinx].push_back(hMC[iTemplate]->GetBinContent(iBinx,iBiny));
      }
    }
  }
  for(unsigned int iBinx = 0; iBinx <= nBins; iBinx++)
  {
    std::vector<double> vecy;
    dataBinContents.push_back(vecy);
    for(unsigned int iBiny = 0; iBiny <= nBins; iBiny++)
    {
      dataBinContents[iBinx].push_back(hData->GetBinContent(iBinx,iBiny));
    }
  }
}



double BinnedLogLikelihood2DConstrainedProb::operator() (const std::vector<double>& par) const
{
  double l = 0.;
//   double ll = 0.;
  double lh = 0.;

  for(unsigned int iBinx = 1; iBinx <= nBins; iBinx++)
  {
    for(unsigned int iBiny = 1; iBiny <= nBins; iBiny++)
    {
      if(iBinx <= removeBin && iBiny <= removeBin)
        continue;
      l = 0.;
//       ll = 0.;
      for(unsigned int iTemplate = 0; iTemplate < nTemplates; iTemplate++)
      {
        l += par[iTemplate] * templateBinContents[iTemplate][iBinx][iBiny];
      }
      lh += dataBinContents[iBinx][iBiny] * TMath::Log(l) - l;
    }
  }

  // Add constraint 1
  // 4 x SS x BB = BS^2
  double num1 = pow(4*par[3]*par[0] - par[1]*par[1], 2);
  double den1 = pow(4*par[3]*par[0], 2) * ( 1/par[3] + 1/par[0] ) + pow(2*par[1]*sqrt(par[1]), 2);
  lh -= 1./2.*num1/den1;

  // Add constraint 2
  // SL / BL = 2 x BS / BB
  double num2 = pow(par[0]*par[4] - 2*par[1]*par[2], 2);
  double den2 = pow(par[0]*par[4], 2) * ( 1/par[0] + 1/par[4] ) + pow(2*par[1]*par[2], 2) * ( 1/par[1] + 1/par[2] );
  lh -= 1./2.*num2/den2;

  return -2*lh;
}



#endif // BinnedLogLikelihood2DConstrainedProb_h
