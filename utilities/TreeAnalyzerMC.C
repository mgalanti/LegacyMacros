#ifndef TreeAnalyzerMC_cxx
#define TreeAnalyzerMC_cxx

#define USING_MC
#undef TREEANALYZER
#define TREEANALYZER TreeAnalyzerMC
#undef PARTICLESELECTOR
#define PARTICLESELECTOR ParticleSelectorMC

// Use correct definition of ntuple leaves
#if NTUPLE_VERSION == 1
#define TREEDEFINITIONRECO TreeDefinitionRecoV1
#define TREEDEFINITIONGEN TreeDefinitionGenV1
#elif NTUPLE_VERSION >= 2 && NTUPLE_VERSION < 5
#define TREEDEFINITIONRECO TreeDefinitionRecoV2
#define TREEDEFINITIONGEN TreeDefinitionGenV2
#else
#error Ntuple versions != [1,2,3,4] not supported.
#endif

#include <typeinfo>

#include "TreeAnalyzerMC.h"
#include "ParticleSelector.C"



void TreeAnalyzerMC::InitSelector()
{
  s = new PARTICLESELECTOR(this, text); // New approach to selectors
}

#endif // TreeAnalyzerMC_cxx

