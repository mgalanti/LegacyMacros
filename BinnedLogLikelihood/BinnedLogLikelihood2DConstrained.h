#ifndef BinnedLogLikelihood2DConstrained_h
#define BinnedLogLikelihood2DConstrained_h

#include "BinnedLogLikelihoodBase.h"

#include "TH2.h"
#include "TMath.h"



class BinnedLogLikelihood2DConstrained : public BinnedLogLikelihoodBase
{
 public:
   // The i-th element of mConstraints is [j, (r, e)], where r is the ratio f_i/f_j in MC and e its error
   BinnedLogLikelihood2DConstrained(std::vector<TH2D*>& hMC, TH2D* hData, std::vector<int> vConstraintComponent, std::vector<double> vConstraintValue, std::vector<double> vConstraintError, unsigned int removeBin /*std::map<unsigned int, std::pair<double, double> > mConstraints*/);
   virtual double operator() (const std::vector<double>&) const;

 private:
   // Definitions of arrays storing 1D templates and data
   // templateBinContents[#template][#binx][#biny];
   // dataBinContents[#bin][#biny];
   std::vector<std::vector<std::vector<double> > > templateBinContents;
   std::vector<int> vConstraintComponent;
   std::vector<double> vConstraintValue;
   std::vector<double> vConstraintError;
   std::vector<std::vector<double> > dataBinContents;
   unsigned int removeBin;
   
   double totEvents;
};



BinnedLogLikelihood2DConstrained::BinnedLogLikelihood2DConstrained(std::vector<TH2D*>& hMC, TH2D* hData, std::vector<int> vConstraintComponent_, std::vector<double> vConstraintValue_, std::vector<double> vConstraintError_, unsigned int removeBin_ /*std::map<unsigned int, std::pair<double, double> > mConstraints*/)
{
  removeBin = removeBin_;
  
  nTemplates = hMC.size();
  
  totEvents=hData->Integral();
  
  int constraintComponentSize = vConstraintComponent_.size();
  
  if(constraintComponentSize != (int)nTemplates)
  {
    std::cout << "E R R O R ! The vector vConstraintComponent does not have the same size of the MC histogram vector!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  for(int i = 0; i < constraintComponentSize; i++)
  {
    if(vConstraintComponent_[i] >= (int)nTemplates || vConstraintComponent_[i] < -1)
    {
      std::cout << "E R R O R ! One of the constraints refers to a non-existing MC component!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }
  
  if(vConstraintValue_.size() != nTemplates)
  {
    std::cout << "E R R O R ! The vector vConstraintValue does not have the same size of the MC histogram vector!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  for(unsigned int i = 0; i < vConstraintValue_.size(); i++)
  {
    if(vConstraintValue_[i] > 1. || vConstraintValue_[i] < 0.)
    {
      std::cout << "E R R O R ! One of the constraints has a value outside the range [0,1]!\n";
      std::cout << "            Exiting...\n";
      exit(1);
    }
  }
  
  if(vConstraintError_.size() != nTemplates)
  {
    std::cout << "E R R O R ! The vector vConstraintError does not have the same size of the MC histogram vector!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  for(unsigned int i = 0; i < vConstraintError_.size(); i++)
  {
    if(vConstraintComponent_[i]>-1)
    {
      ///PROVAAAAAA
      vConstraintError_[i] = vConstraintValue_[i]/vConstraintValue_[vConstraintComponent_[i]] *
                             sqrt( pow(vConstraintError_[i]/vConstraintValue_[i], 2) +
                                   pow(vConstraintError_[vConstraintComponent_[i]]/vConstraintValue_[vConstraintComponent_[i]], 2) );

      //if(vConstraintValue_[i]/vConstraintValue_[vConstraintComponent_[i]]+vConstraintError_[i] > 1. || vConstraintValue_[i]/vConstraintValue_[vConstraintComponent_[i]]-vConstraintError_[i] < 0.)
      if (vConstraintValue_[i] + vConstraintValue_[vConstraintComponent_[i]]*vConstraintError_[i] > 1. ||
          vConstraintValue_[i] - vConstraintValue_[vConstraintComponent_[i]]*vConstraintError_[i] < 0.)
      {
        std::cout << "E R R O R ! One of the constraints has a error that brings it outside the range [0,1]!\n";
        std::cout << "            Exiting...\n";
        exit(1);
      }
    }
  }
  
  vConstraintComponent = vConstraintComponent_;
  vConstraintValue = vConstraintValue_;
  vConstraintError = vConstraintError_;

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
  
  for(unsigned int i = 0; i < vConstraintComponent.size(); i++)
  {
    std::cout << "Component " << hMC[i]->GetName() << std::endl;
    if(vConstraintComponent[i]>-1)
    {
      std::cout << "Is constrained to be " << vConstraintValue[i] << "/" << vConstraintValue[vConstraintComponent[i]] << " of component " << hMC[vConstraintComponent[i]]->GetName() << " with error " << vConstraintError[i] << std::endl;
    }
    else
    {
      std::cout << "Is NOT constrained\n";
    }
  }
}



double BinnedLogLikelihood2DConstrained::operator() (const std::vector<double>& par) const
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
      for(unsigned int iTemplate = 0; iTemplate < nTemplates; iTemplate++){
        l += par[iTemplate] * templateBinContents[iTemplate][iBinx][iBiny];
      }
      lh += dataBinContents[iBinx][iBiny] * TMath::Log(l) - l;
    }
  }
  
//   cout << "@@@@@ lh0 = " << lh ;
  
  for(unsigned int iTemplate = 0; iTemplate < nTemplates; iTemplate++)
  {
    if(vConstraintComponent[iTemplate] > -1)
    {
      double num = (par[iTemplate] / totEvents - par[vConstraintComponent[iTemplate]] / totEvents * vConstraintValue[iTemplate] / vConstraintValue[vConstraintComponent[iTemplate]]);
//       cout << "   constr=" << 1./2.*(num*num)/(vConstraintError[iTemplate]*vConstraintError[iTemplate]);
      lh -= 1./2.*(num*num)/(vConstraintError[iTemplate]*vConstraintError[iTemplate]);
    }
  }

//   cout << "    lhf = " << lh << endl;
  
  return -2*lh;
}



#endif // BinnedLogLikelihood2DConstrained_h
