#ifndef BinnedLogLikelihood2D_h
#define BinnedLogLikelihood2D_h



#include "BinnedLogLikelihoodBase.h"

#include "TH2.h"
#include "TMath.h"



class BinnedLogLikelihood2D : public BinnedLogLikelihoodBase
{
 public:
   BinnedLogLikelihood2D(std::vector<TH2D*>& hMC, TH2D* hData, unsigned int removeBin);
   virtual double operator() (const std::vector<double>&) const;

 private:
   // Definitions of arrays storing 1D templates and data
   // templateBinContents[#template][#binx][#biny];
   // dataBinContents[#bin][#biny];
   std::vector<std::vector<std::vector<double> > > templateBinContents;
   std::vector<std::vector<double> > dataBinContents;
   unsigned int removeBin;
};



BinnedLogLikelihood2D::BinnedLogLikelihood2D(std::vector<TH2D*>& hMC, TH2D* hData, unsigned int removeBin_)
{
  removeBin = removeBin_;
  nTemplates = hMC.size();
  nBins = hData->GetXaxis()->GetNbins();
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



double BinnedLogLikelihood2D::operator() (const std::vector<double>& par) const
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

  return -2*lh;
}



#endif // BinnedLogLikelihood2D_h
