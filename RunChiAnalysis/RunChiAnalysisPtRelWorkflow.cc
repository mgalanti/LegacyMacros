#include <string>


void RunChiAnalysisPtRelWorkflow(const std::string propertiesFileName)
{
  gSystem->CompileMacro("../ChiAnalysis/ChiAnalysisPtRelWorkflow.C");
  ChiAnalysisPtRelWorkflow* a = new ChiAnalysisPtRelWorkflow(propertiesFileName);
  a->RunPtRelWorkflow();
}