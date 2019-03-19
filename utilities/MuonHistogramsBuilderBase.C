#ifndef MuonHistogramsBuilderBase_cxx
#define MuonHistogramsBuilderBase_cxx



#include "MuonHistogramsBuilderBase.h"



void MuonHistogramsBuilderBase::Initialize()
{
  outputFile = new TFile(outputFileName.c_str(),"RECREATE");
//   if(!outputFile)
//     outputFile = new TFile(outputFileName.c_str(),"RECREATE");
//   else
//     std::cout << "I N F O : MuonHistogramsBuilderBase::Initialize() - output file already exists.\n";
  rng.SetSeed(12345);
  rebinFactor = 1;
  axisMin = 0.;
  axisMax = 0.2;
  removeBin = 0;
}



void MuonHistogramsBuilderBase::SetRange(const double xMin, const double xMax)
{
  if(xMin < xMax)
  {
    axisMin = xMin;
    axisMax = xMax;
  }
  else
  {
    std::cout << "W A R N I N G! MuonHistogramsBuilderBase::SetRange: xMin >= xMax! Not setting new axis limits!\n";
  }
}

#endif // MuonHistogramsBuilderBase_cxx
