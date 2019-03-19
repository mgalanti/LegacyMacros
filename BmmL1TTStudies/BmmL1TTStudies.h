#ifndef BmmL1TTStudies_h
#define BmmL1TTStudies_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class BmmL1TTStudies : public TreeAnalyzerMC
{
  public:
    BmmL1TTStudies(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) :
      TreeAnalyzerMC("BmmL1TTStudies", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    template <typename T, typename U> void Fill(std::vector<T*>* vHistos, const U value, const double binningValue, std::vector<double> vLowBins)
    {
      unsigned int i = 0;
      for(/* */; i < vLowBins.size(); i++)
      {
        if(binningValue < vLowBins[i])
        {
          break;
        }
      }
      vHistos->at(i)->Fill(value);
    }
    template <typename T, typename U> void Fill(std::vector<T*>* vHistos, const U valueX, const U valueY, const double binningValue, std::vector<double> vLowBins)
    {
      unsigned int i = 0;
      for(/* */; i < vLowBins.size(); i++)
      {
        if(binningValue < vLowBins[i])
        {
          break;
        }
      }
      vHistos->at(i)->Fill(valueX, valueY);
    }
};

#endif // BmmL1TTStudies_h
