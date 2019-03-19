#ifndef BinnedLogLikelihoodBase_h
#define BinnedLogLikelihoodBase_h



#include "TROOT.h"
#include "TH1.h"
#include "Minuit2/FCNBase.h"



class BinnedLogLikelihoodBase : public ROOT::Minuit2::FCNBase
{

 public:
//    BinnedLogLikelihoodBase(std::vector<TH1*>&, TH1*, Double_t, Double_t);
   ~BinnedLogLikelihoodBase() {}
   virtual double operator() (const std::vector<double>&) const {return 0;};
   double Up() const { return 0.5; }

 protected:
   //number of events of the spectrum
   int nEvents;
   //array dimension
   unsigned int nBins;
   //number of templates
   unsigned int nTemplates;
};



#endif // BinnedLogLikelihoodBase_h
