#ifndef BinnedLogLikelihood2DConstrainedSum_h
#define BinnedLogLikelihood2DConstrainedSum_h

#include "BinnedLogLikelihoodBase.h"

#include "TH2.h"
#include "TMath.h"



class BinnedLogLikelihood2DConstrainedSum : public BinnedLogLikelihoodBase
{
 public:
   // The i-th element of mConstraints is [j, (r, e)], where 
   // j is the code specifing which component must be summed (i.e. 20 means f0+f2, 310 means f0+f1+f3, etc)
   // r is the value of the constraint
   // e is the erorr on r
   BinnedLogLikelihood2DConstrainedSum(std::vector<TH2D*>& hMC, TH2D* hData, std::vector<int> vConstraintComponent, std::vector<double> vConstraintValue, std::vector<double> vConstraintError, unsigned int removeBin /*std::map<unsigned int, std::pair<double, double> > mConstraints*/);
   virtual double operator() (const std::vector<double>&) const;

 private:
   // Definitions of arrays storing 2D templates and data
   // templateBinContents[#template][#bin];
   // dataBinContents[#bin];
   std::vector<std::vector<std::vector<double> > >  templateBinContents;
   std::vector< std::vector<unsigned int> > vvConstraintComponent;
   std::vector<double> vConstraintValue;
   std::vector<double> vConstraintError;
   std::vector<std::vector<double> > dataBinContents;
   unsigned int removeBin;
   
   double totEvents;
};



BinnedLogLikelihood2DConstrainedSum::BinnedLogLikelihood2DConstrainedSum(std::vector<TH2D*>& hMC, TH2D* hData, std::vector<int> vConstraintComponent_, std::vector<double> vConstraintValue_, std::vector<double> vConstraintError_, unsigned int removeBin_ /*std::map<unsigned int, std::pair<double, double> > mConstraints*/)
{
  removeBin = removeBin_;
  
  nTemplates = hMC.size();
  
  totEvents=hData->Integral();
  
  if( vConstraintValue_.size() != vConstraintError_.size() )
  {
    std::cout << "E R R O R ! The vectors vConstraintValue and vConstraintError do not have the same size!\n";
    std::cout << "            Exiting...\n";
    exit(1);    
  }

  if( vConstraintValue_.size() != vConstraintComponent_.size() )
  {
    std::cout << "E R R O R ! The vector vConstraintComponent does not have the same size as vConstraintValue and vConstraintError!\n";
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
  
//   for(unsigned int i = 0; i < vConstraintError_.size(); i++)
//   {
//     if(vConstraintComponent_[i]>-1)
//     {
//       //if(vConstraintValue_[i]/vConstraintValue_[vConstraintComponent_[i]]+vConstraintError_[i] > 1. || vConstraintValue_[i]/vConstraintValue_[vConstraintComponent_[i]]-vConstraintError_[i] < 0.)
//       if (vConstraintValue_[i] + vConstraintValue_[vConstraintComponent_[i]]*vConstraintError_[i] > 1. ||
//           vConstraintValue_[i] - vConstraintValue_[vConstraintComponent_[i]]*vConstraintError_[i] < 0.)
//       {
//         std::cout << "E R R O R ! One of the constraints has a error that brings it outside the range [0,1]!\n";
//         std::cout << "            Exiting...\n";
//         exit(1);
//       }
//     }
//   }
  
  ///
  
  for(unsigned int i = 0; i < vConstraintComponent_.size(); i++)
  {
    for(unsigned int j = i+1; j < vConstraintComponent_.size(); j++)
    {
      if (vConstraintComponent_[i] == vConstraintComponent_[j])
      {
        std::cout << "E R R O R ! Constraints "<< i <<" and "<< j <<" define the same sum!\n";
        std::cout << "            Exiting...\n";
        exit(1);
      }
    }
  }
  
  std::vector < std::vector <unsigned int> > vvConstraintComponent_;
  
  for(unsigned int i = 0; i < vConstraintComponent_.size(); i++)
  {
    unsigned int n = vConstraintComponent_[i];
    std::vector <unsigned int> v;
    vvConstraintComponent_.push_back(v);
    
    if(n<10)
    {
/*      std::cout << "E R R O R ! Constraint code " << n << " smaller than 10!\n";
      std::cout << "            Exiting...\n";
      exit(1);*/
      std::cout << "W A R N I N G ! Constraint code " << n << " smaller than 10!\n";
      std::cout << "                It's OK if you are constraining only one fraction\n";
    }
    
    while(n>0)
    {
      unsigned int p = n%10;
//       std::cout << " p : " << p << std::endl;
      
      if(p >= nTemplates)
      {
        std::cout << "E R R O R ! In constraint " << i << " found index "<< p <<" greater than the number of templates!\n";
        std::cout << "            Exiting...\n";
        exit(1);
      }
      
      if( (vvConstraintComponent_[i].size()>0) && (p <= vvConstraintComponent_[i].back()) )
      {
        std::cout << "E R R O R ! In constraint " << i << " found a wrong code : "<< vConstraintComponent_[i] <<"!\n";
        std::cout << "            Exiting...\n";
        exit(1);
      }

      vvConstraintComponent_[i].push_back(p);
      n = ( n - p )/10;
//       std::cout << " n : " << n << std::endl;
    }
  }
  
  vvConstraintComponent = vvConstraintComponent_;
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
  
  std::cout << "I N F O : BinnedLogLikelihood2DConstrainedSum - Listing constraints...\n";
  for(unsigned int i = 0; i < vvConstraintComponent.size(); i++)
  {
    std::cout << "Constraint " << i << " : constraining sum of components " << std::flush;
    for(unsigned int j = 0; j < vvConstraintComponent[i].size(); j++)
    {
      std::cout << vvConstraintComponent[i][j] << ' ' << "(" << hMC[vvConstraintComponent[i][j]]->GetName() << ") " << std::flush;
    }
    std::cout << "to be equal to " << vConstraintValue[i] << "+/-" << vConstraintError[i] << std::endl;
  }
}



double BinnedLogLikelihood2DConstrainedSum::operator() (const std::vector<double>& par) const
{
  double l = 0.;
//   double ll = 0.;
  double lh = 0.;
//   double logFactorial;

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
  
  for(unsigned int i = 0; i < vvConstraintComponent.size(); i++)
  {    
    double sum = 0;
    for(unsigned int j = 0; j < vvConstraintComponent[i].size(); j++)
    {
      sum += par[vvConstraintComponent[i][j]];
    }     
    lh-= 1./2.*TMath::Power( (sum/totEvents - vConstraintValue[i]), 2)/TMath::Power(vConstraintError[i]/vConstraintValue[i], 2);   
  }
  
  return -2*lh;
}



#endif // BinnedLogLikelihood2DConstrainedSum_h
