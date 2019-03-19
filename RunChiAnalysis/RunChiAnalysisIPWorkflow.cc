#include <string>


void RunChiAnalysisIPWorkflow(const std::string propertiesFileName)
{
  gSystem->CompileMacro("../ChiAnalysis/ChiAnalysisIPWorkflow.C");
  ChiAnalysisIPWorkflow* a = new ChiAnalysisIPWorkflow(propertiesFileName);
  a->RunIPWorkflow();
}
