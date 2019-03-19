#ifndef MuonHistogramsBuilderBase_h
#define MuonHistogramsBuilderBase_h

#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TRandom3.h"

#include "HistoPlotter.C"



class MuonHistogramsBuilderBase : public virtual HistoPlotter
{
  public:
    MuonHistogramsBuilderBase()
    {
      std::cout << "MuonHistogramsBuilderBase: Please call constructor with arguments!\n";
    }
    MuonHistogramsBuilderBase(const std::string outputFileName_) :
      outputFileName(outputFileName_)
    {
      Initialize();
    }
    void Initialize();
    void SetRange(const double axisMin, const double axisMax);
    void SetRebinFactor(unsigned short int factor) {rebinFactor = factor;}
    void SetRemoveBin(unsigned short int binsToRemove) {removeBin = binsToRemove;}
    
  protected:
    std::string outputFileName;
    TFile* outputFile;
    TRandom3 rng;
    double axisMin;
    double axisMax;
    unsigned short int rebinFactor;
    unsigned short int removeBin;
    std::stringstream oss;
    
    std::string axislabel1D;
    std::string axislabel2D;
    std::string titlelabel2D;
};


#endif // MuonHistogramsBuilderBase_h
