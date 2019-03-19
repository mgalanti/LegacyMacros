#ifndef TreeAnalyzerData_cxx
#define TreeAnalyzerData_cxx

#include <typeinfo>

// Use correct definition of ntuple leaves
#if NTUPLE_VERSION == 1
#define TREEDEFINITIONRECO TreeDefinitionRecoV1
#elif NTUPLE_VERSION >= 2 && NTUPLE_VERSION < 5
#define TREEDEFINITIONRECO TreeDefinitionRecoV2
#else
#error Ntuple versions != [1,2,3,4] not supported.
#endif

// Undefine USING_MC if already defined before
#undef USING_MC
#undef TREEANALYZER
#undef PARTICLESELECTOR
#define TREEANALYZER TreeAnalyzerData
#define PARTICLESELECTOR ParticleSelectorData

#include "TreeAnalyzerData.h"
#include "ParticleSelector.C"



// void TreeAnalyzerData::CheckRecoTreeStructure()
// {
//   
//   if(!fChain->GetBranchStatus("numberOfTrackJet"))
//     std::cout << "W A R N I N G! Branch \"numberOfTrackJet\" is missing from the ntuple! Trying to access it will result in a crash!\n";
// }

void TreeAnalyzerData::InitSelector()
{
  s = new PARTICLESELECTOR(this, text); // New approach to selectors
}

#endif // TreeAnalyzerData_cxx

