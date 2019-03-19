void runBinnedLogLikelihoodCalculator1DConstrained(const std::string inputFileNameData, const std::string inputFileNameMC, const std::string templateNamesMerged, std::string sConstraintComponentMerged, std::string sConstraintValueMerged, std::string sConstraintErrorMerged)
{
  gSystem->CompileMacro("BinnedLogLikelihoodCalculator1DConstrained.cc");
  
  BinnedLogLikelihoodCalculator1DConstrained(inputFileNameData,inputFileNameMC,templateNamesMerged,sConstraintComponentMerged,sConstraintValueMerged,sConstraintErrorMerged);
}
