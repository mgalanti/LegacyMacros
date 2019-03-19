#ifndef ChiAnalysisIPWorkflow_h
#define ChiAnalysisIPWorkflow_h



#include <string>



#include "ChiAnalysisBase.C"



class ChiAnalysisIPWorkflow : public ChiAnalysisBase
{
 public:
   ChiAnalysisIPWorkflow(const std::string propertiesFileName_);
   
   void RunIPWorkflow();
   
 private:
   void RunBuildersStep();
   void RunFitStep();
   
   void BuildIPTemplates();
   void BuildDataDistribution();
   void BuildDataDistributionFromFakeIP();
   void BuildDataDistributionFromRealIP();
   
   void FindMCFractionsIP();
};



#endif // ChiAnalysisIPWorkflow_h
