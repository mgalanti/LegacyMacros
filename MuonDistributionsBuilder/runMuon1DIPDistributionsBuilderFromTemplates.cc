#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>

void runMuon1DIPDistributionsBuilderFromTemplates(std::string inputFileName, std::string outputFileName, std::string inputHistoNames1DMerged, std::string nEventsPerTemplateStringMerged, const double fitMin=0., const double fitMax=0.2)
{
  std::vector<std::string> inputHistoNames1D;
  std::vector<std::string> inputHistoNames2D;
  std::vector<std::string> nEventsPerTemplateString;
  TokenizeString(inputHistoNames1DMerged,inputHistoNames1D);
  TokenizeString(nEventsPerTemplateStringMerged,nEventsPerTemplateString,"-");

  std::vector<double> nEventsPerTemplate;
  for(unsigned int i = 0; i < nEventsPerTemplateString.size(); i++)
  {
    nEventsPerTemplate.push_back(ConvertToDouble(nEventsPerTemplateString[i]));
  }

  std::string outputHistoName1D = "hData";
  std::string outputHistoName2D = "hData2D";
  gSystem->CompileMacro("../utilities/MuonHistogramsBuilderBase.C");
  gSystem->CompileMacro("../utilities/MuonDistributionsBuilder.C");
//   MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder("MuonIPDistributionsProducer__Histograms__Data__Mu__Run2010A-PromptReco-v4__141001-144114__muonV0Pt3.root","MuonIPDistributions__Histograms__Data__Mu__Run2010A-PromptReco-v4__141001-144114__muonV0Pt3.root",inputHistoNames1D,inputHistoNames2D,outputHistoName1D,outputHistoName2D);
  MuonDistributionsBuilder* ipdb = new MuonDistributionsBuilder(inputFileName,outputFileName,inputHistoNames1D,inputHistoNames2D,outputHistoName1D,outputHistoName2D);
  ipdb->SetRange(fitMin,fitMax);
  ipdb->SetScaleFactors(nEventsPerTemplate);
  ipdb->SetRebinFactor(1.);
  ipdb->Build1DDistribution();
//   ipdb->Build2DDistribution();
  ipdb->SaveDistributions();
  ipdb->CreateAndSavePrettyPlots();
  delete ipdb;
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



class BadConversion : public std::runtime_error
{
public:
  BadConversion(std::string const& s)
  : std::runtime_error(s)
  { }
};



double ConvertToDouble(std::string const& s)
{
  std::istringstream i(s);
  double x;
  if (!(i >> x))
    throw BadConversion("convertToDouble(\"" + s + "\")");
  return x;
}
