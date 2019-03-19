void runBinnedLogLikelihoodCalculator1D(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged)
{
  gSystem->CompileMacro("BinnedLogLikelihoodCalculator1D.cc");
  BinnedLogLikelihoodCalculator1D(inputFileNameData,inputFileNameMC,templateNamesMerged);
}