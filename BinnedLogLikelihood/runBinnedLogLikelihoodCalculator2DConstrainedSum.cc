void runBinnedLogLikelihoodCalculator2DConstrainedSum(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged, std::string sConstraintComponentMerged, std::string sConstraintValueMerged, std::string sConstraintErrorMerged, std::string sRemoveBin)
{
  gSystem->CompileMacro("BinnedLogLikelihoodCalculator2DConstrainedSum.cc");
  
  BinnedLogLikelihoodCalculator2DConstrainedSum(inputFileNameData,inputFileNameMC,templateNamesMerged,sConstraintComponentMerged,sConstraintValueMerged,sConstraintErrorMerged,sRemoveBin);
}
