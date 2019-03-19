#include <exception>

void runSimpleTMVAResultsAnalyzer(const std::string inputFileName, const int muonClassId, const std::string muonClassName, const int maxEvents = -1)
{
  gSystem->CompileMacro("SimpleTMVAResultsAnalyzer.C");
  SimpleTMVAResultsAnalyzer m(inputFileName);
  m.Loop(muonClassId, muonClassName);
}
