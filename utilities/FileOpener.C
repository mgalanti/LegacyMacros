#ifndef FileOpener_cxx
#define FileOpener_cxx

// #include <cstdlib>
#include <fstream>

#include "TChain.h"
#include "FileOpener.h"



#if NTUPLE_VERSION >= 3
TChain* FileOpener::OpenFile(const std::string sampleName, TChain* chain, TChain* metadataChain)
#else
TChain* FileOpener::OpenFile(const std::string sampleName, TChain* chain)
#endif
{
  std::string fileList = "../fileLists/fileList_" + sampleName + ".txt";
  std::ifstream fileNames(fileList.c_str());
  if(!fileNames.good())
  {
    std::cout << "E R R O R ! File list \"" << fileList << "\" does not exist!\n";
    std::cout << "            Exiting...\n";
    exit(1);
  }
  std::string fileName;
#if NTUPLE_VERSION >= 3
  bool metadataLoaded = false;
#endif
  while(fileNames >> fileName)
  {
#if NTUPLE_VERSION >= 3
    if(!metadataLoaded)
    {
      metadataChain->Add(fileName.c_str());
      metadataLoaded = true;
    }
#endif
    std::cout << "I N F O : Adding file " << fileName << " to the chain.\n";
    chain->Add(fileName.c_str());
  }
  return chain;
}

#endif // FileOpener_cxx
