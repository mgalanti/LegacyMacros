void runBinnedLogLikelihoodCalculator2D(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged, std::string sRemoveBin)
{
  gSystem->CompileMacro("BinnedLogLikelihoodCalculator2D.cc");
  BinnedLogLikelihoodCalculator2D(inputFileNameData,inputFileNameMC,templateNamesMerged,sRemoveBin);
}
