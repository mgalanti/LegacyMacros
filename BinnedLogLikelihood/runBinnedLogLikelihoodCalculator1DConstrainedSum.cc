void runBinnedLogLikelihoodCalculator1DConstrainedSum(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged, std::string sConstraintComponentMerged, std::string sConstraintValueMerged, std::string sConstraintErrorMerged)
{
  gSystem->CompileMacro("BinnedLogLikelihoodCalculator1DConstrainedSum.cc");
  
  BinnedLogLikelihoodCalculator1DConstrainedSum(inputFileNameData,inputFileNameMC,templateNamesMerged,sConstraintComponentMerged,sConstraintValueMerged,sConstraintErrorMerged);
}