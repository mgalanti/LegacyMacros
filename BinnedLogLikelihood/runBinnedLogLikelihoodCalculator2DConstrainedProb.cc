void runBinnedLogLikelihoodCalculator2DConstrainedProb(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged, std::string sRemoveBin)
{
  gSystem->CompileMacro("BinnedLogLikelihoodCalculator2DConstrainedProb.cc");
  BinnedLogLikelihoodCalculator2DConstrainedProb(inputFileNameData,inputFileNameMC,templateNamesMerged,sRemoveBin);
}
