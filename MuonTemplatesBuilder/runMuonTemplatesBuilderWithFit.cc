// #include "../utilities/Muon2DTemplatesBuilder.C"



void runMuonTemplatesBuilderWithFit(std::string inputFileName, std::string outputFileName, std::string inputHistoNamesMerged, std::string outputHistoNamesMerged, double fitMin, double fitMax,int rebinFactor,int removeBin, std::string inputFileNamePrompt="", std::string inputHistoNamePrompt="")
{
  std::vector<std::string> inputHistoNames;
  TokenizeString(inputHistoNamesMerged,inputHistoNames);
  std::vector<std::string> outputHistoNames;
  TokenizeString(outputHistoNamesMerged,outputHistoNames);
  gSystem->CompileMacro("../utilities/MuonHistogramsBuilderBase.C");
  gSystem->CompileMacro("../utilities/MuonTemplatesBuilder.C");
  MuonTemplatesBuilder* mtb = new MuonTemplatesBuilder(inputFileName,outputFileName,inputHistoNames,outputHistoNames,inputFileNamePrompt,inputHistoNamePrompt);
  mtb->SetRange(fitMin,fitMax);
  mtb->SetRebinFactor(rebinFactor);
  mtb->SetRemoveBin(removeBin);
  mtb->Build1DTemplatesFromFit();
  mtb->Build1DDistributions();
  mtb->Build2DTemplatesFromFit();
  mtb->Build2DDistributions();
  mtb->SaveTemplates();
  mtb->CreateAndSavePrettyPlots();
  delete mtb;
}



void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ";")
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}
