//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 12 19:03:11 2010 by ROOT version 5.22/00d
// from TTree my_tree/my_tree
// found on file: Ntuple__MinBias_7TeV-pythia8__Spring10-START3X_V26B-v1__AllSelections_100_1.root
//////////////////////////////////////////////////////////

#ifndef TreeReaderMC_h
#define TreeReaderMC_h

#include <vector>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#if NTUPLE_VERSION == 1
#include "TreeDefinitionGenV1.h"
#elif NTUPLE_VERSION >= 2
#include "TreeDefinitionGenV2.h"
#endif

#include "FileOpener.C"
#include "TreeReaderData.C"


class TreeReaderMC : public TreeReaderData , public virtual TREEDEFINITIONGEN {
public :
   TreeReaderMC() { std::cout << "TreeReaderMC: Please call constructor with arguments!\n";};
#if NTUPLE_VERSION >= 3
   TreeReaderMC(const std::string sampleName, TTree *tree=0, TTree *metadataTree = 0) :
     TreeReaderData(sampleName, tree, metadataTree) {InitGen(fChain);};
#else
   TreeReaderMC(const std::string sampleName, TTree *tree=0) :
     TreeReaderData(sampleName, tree) {InitGen(fChain);};
#endif
};

#endif

#ifdef TreeReaderMC_cxx

// Nothing here!

#endif // #ifdef TreeReaderMC_cxx
