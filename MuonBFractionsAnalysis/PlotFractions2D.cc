#include <stdexcept>

#include "../utilities/HistoPlotter.C"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TFile.h"

#include <fstream>
#include <sstream>
#include <iostream>

void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ";");



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



void PlotFractions2D(const std::string fractionsStringMerged2,const std::string inputHistoNamesMerged,const std::string templateNamesMerged)
{
  std::vector<std::string> fractionsStringMerged;
  TokenizeString(fractionsStringMerged2,fractionsStringMerged,"_");
  std::vector<std::string> inputHistoNames;
  TokenizeString(inputHistoNamesMerged,inputHistoNames);
  std::vector<std::string> templateNames;
  TokenizeString(templateNamesMerged,templateNames);

  std::vector<std::string> fractionsString;
  std::vector<double> otherPercentages;
  TokenizeString(fractionsStringMerged[fractionsStringMerged.size()-1],fractionsString,"-");
  double total = 0;
  for(unsigned int i = 1; i < fractionsString.size(); i++)
  {
    double fraction = ConvertToDouble(fractionsString[i]);
    total+=fraction;
    otherPercentages.push_back(fraction);
  }
  for(unsigned int i = 0; i < otherPercentages.size(); i++)
  {
    otherPercentages[i]*=100;
    otherPercentages[i]/=total;
  }

  HistoPlotter hp;

  std::vector<TGraphErrors*> graphs;
  std::vector<unsigned int> markerStyles;
  std::vector<unsigned int> colors;

  std::cout << "inputHistoNamesMerged = " << inputHistoNamesMerged << std::endl;
  std::cout << "templateNamesMerged = " << templateNamesMerged<<std::endl;

  for(unsigned int iTemplate = 0; iTemplate < templateNames.size(); iTemplate++)
  {
    std::string name = "gNEvts" + templateNames[iTemplate];
    TGraphErrors* graph = hp.CreateGraph<TGraphErrors>(name.c_str(),"","N_{REAL}","(N_{FIT}-N_{REAL})/TOT",0);
    graphs.push_back(graph);
    markerStyles.push_back(20+iTemplate);
    colors.push_back(1+iTemplate);
  }

  for(unsigned int i = 0; i < fractionsStringMerged.size(); i++)
  {
    std::vector<std::string> fractionsString;
    TokenizeString(fractionsStringMerged[i],fractionsString,"-");
    std::vector<double> fractions;
    std::string fileName = "FitResults2D_"+fractionsStringMerged[i]+".txt";
    std::cout << "Looking at file " << fileName << std::endl;
    std::fstream fInput(fileName.c_str(),ios_base::in);
    std::string templateName;
    double nEventsFromFit,nEventsErrorFromFit;
    double total = 0;
    std::cout << "fractionsString.size() = " << fractionsString.size() << std::endl;
    for(unsigned int j = 0; j < fractionsString.size(); j++)
    {
      fractions.push_back(ConvertToDouble(fractionsString[j]));
      total+=fractions[j];
    }
    for(unsigned int j = 0; j < templateNames.size(); j++)
    {
      fInput >> templateName >> nEventsFromFit >> nEventsErrorFromFit;
      std::cout << "name = " << templateName << std::endl;
      std::cout << "frac = " << nEventsFromFit << std::endl;
      std::cout << "erro = " << nEventsErrorFromFit << std::endl;
      std::cout << "input= " << fractions[j] << std::endl;
      std::cout << "total= " << total << std::endl;
      graphs[j]->Set(graphs[j]->GetN()+1);
      graphs[j]->SetPoint(graphs[j]->GetN()-1,fractions[0],(nEventsFromFit-fractions[j])/total);
      graphs[j]->SetPointError(graphs[j]->GetN()-1,0.,nEventsErrorFromFit/total);
      graphs[j]->GetXaxis()->SetTitle("N_{BB}(input)");
      graphs[j]->GetYaxis()->SetTitle("(N(fit)-N(input))/Tot");
    }
  }


  std::string outFileName = "FitGraph";
  std::string outImageName = "";
  std::string graphTitle = "";
  std::stringstream oss,ossTitle;
  oss.str("");
  ossTitle.str("");
  oss << "_" << inputHistoNames[0].substr(1);
  ossTitle << inputHistoNames[0].substr(1);
  for(unsigned int i = 1; i < fractionsString.size(); i++)
  {
    oss << "-" << otherPercentages[i-1] << inputHistoNames[i].substr(1);
    ossTitle << " + " << otherPercentages[i-1] << "% " << inputHistoNames[i].substr(1);
  }
  outFileName+=oss.str();
  outImageName = outFileName + ".png";
  outFileName+=".root";
  graphTitle = ossTitle.str();
  graphs[0]->SetTitle(graphTitle.c_str());
  TFile* outFile = new TFile(outFileName.c_str(), "RECREATE");
  outFile->cd();

  HistoPlotter::LegendDefinition leg = {"Templates",templateNames,"SW"};
  TCanvas* c = hp.CreateAndWriteCanvas("cFractions",leg,&markerStyles[0],&colors[0],false,false,*((const std::vector<TGraph*>*)(&graphs)));

  c->SaveAs(outImageName.c_str());

  delete c;

  for(unsigned int iFrac = 0; iFrac < graphs.size(); iFrac++)
    graphs[iFrac]->Write();

  outFile->Close();
}



void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
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
