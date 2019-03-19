#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>



void runMuonHistogramMergerWithCopy(std::string inputFileName1, std::string inputFileName2, std::string outputFileName, std::string inputHistoName1, std::string inputHistoName2, std::string outputHistoName, double fraction1, std::string histoNamesToCopyMerged)
{
  gSystem->CompileMacro("../utilities/HistoPlotter.C");
  gSystem->CompileMacro("../utilities/MuonHistogramsMerger.C");
  MuonHistogramsMerger* m = new MuonHistogramsMerger(inputFileName1, inputFileName2, outputFileName);
  m->MergeHistograms(inputHistoName1, inputHistoName2, outputHistoName, fraction1, 1);
  std::vector<std::string> histoNamesToCopy;
  TokenizeString(histoNamesToCopyMerged,histoNamesToCopy);
  for(unsigned int iHistoName = 0; iHistoName < histoNamesToCopy.size(); iHistoName++)
  {
    m->CopyHistogramFromFirstFile(histoNamesToCopy[iHistoName]);
  }
  delete m;
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
