#ifndef MuonTemplatesProducer_h
#define MuonTemplatesProducer_h

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "../utilities/TreeAnalyzerMC.C"
#include "../utilities/MuonMCAssociator.h"
#include "../utilities/MuonJetContainer.h"

/// Class for error in string-to-double conversion
class BadConversion : public std::runtime_error {
public:
  BadConversion(std::string const& str)
  : std::runtime_error(str)
  { }
};

class MuonTemplatesProducer : public TreeAnalyzerMC/*, public MuonMCAssociator*/
{
  public:
    MuonTemplatesProducer(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) :
      TreeAnalyzerMC("MuonTemplatesProducer", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    double DxyD6TScaling(double dxy);
    inline double ConvertToDouble(const std::string& str)
    {
      std::istringstream i(str);
      double x;
      if (!(i >> x))
        throw BadConversion("convertToDouble(\"" + str + "\")");
      return x;
    }
};

#endif // MuonTemplatesProducer_h

