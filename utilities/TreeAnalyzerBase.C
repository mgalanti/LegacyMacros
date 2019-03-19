#ifndef TreeAnalyzerBase_cxx
#define TreeAnalyzerBase_cxx

#include <typeinfo>

#include "TreeAnalyzerBase.h"



void TreeAnalyzerBase::Initialize(const std::string myName_, const std::string sampleName_, const std::string text_, const int maxEvents_)
{
  //   className();
  // Does not work :(
  //   std::string outFileName = "MC__" + className() + "__Histograms__" + sampleName + "__" + text + ".root";
  maxEvents = maxEvents_;
  myName = myName_;
  text = text_;
  sampleName = sampleName_;
  std::string outFileName = myName + "__Histograms__" + sampleName + "__" + text + ".root";
  outFile = new TFile(outFileName.c_str(), "RECREATE");
}



TreeAnalyzerBase::~TreeAnalyzerBase()
{
  outFile->Close();
}



// const std::string TreeAnalyzerBase::ClassName()
// {
//   TreeAnalyzerBase& myClass = *this;
//   const std::string s(typeid(myClass).name());
//   std::cout << "Full class/function name is: " << __PRETTY_FUNCTION__ << std::endl;
//   std::cout << "Class information from typeid is: " << s << std::endl;
//   return s;
// }

#endif // TreeAnalyzerBase_cxx
