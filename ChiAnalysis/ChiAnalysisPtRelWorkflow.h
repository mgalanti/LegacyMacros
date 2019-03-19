#ifndef ChiAnalysisPtRelWorkflow_h
#define ChiAnalysisPtRelWorkflow_h



#include <string>



#include "ChiAnalysisBase.C"



class ChiAnalysisPtRelWorkflow : public ChiAnalysisBase
{
 public:
   ChiAnalysisPtRelWorkflow(const std::string propertiesFileName_);
   
   void RunPtRelWorkflow();
   
 private:
   void RunBuildersStep();
   void RunFitStep();
   
   void BuildPtRelTemplates();
   void BuildDataDistribution();
   void BuildDataDistributionFromFakePtRel();
   void BuildDataDistributionFromRealPtRel();
   
   void FindMCFractionsPtRel();
   
   double GetFractionFrom1DIPFit(const std::string fileName, const std::string componentName);
   double GetFractionFrom2DIPFit(const std::string fileName, const std::string componentName);
};



#endif // ChiAnalysisPtRelWorkflow_h
