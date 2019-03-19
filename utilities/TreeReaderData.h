//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 12 19:03:11 2010 by ROOT version 5.22/00d
// from TTree my_tree/my_tree
// found on file: Ntuple__MinBias_7TeV-pythia8__Spring10-START3X_V26B-v1__AllSelections_100_1.root
//////////////////////////////////////////////////////////

#ifndef TreeReaderData_h
#define TreeReaderData_h

#include <vector>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#if NTUPLE_VERSION == 1
#include "TreeDefinitionRecoV1.h"
#elif NTUPLE_VERSION >= 2
#include "TreeDefinitionRecoV2.h"
#endif

#include "FileOpener.C"



class TreeReaderData : public virtual TREEDEFINITIONRECO , public FileOpener {
public :

   TreeReaderData() { std::cout << "TreeReaderData: Please call constructor with arguments!\n";};
#if NTUPLE_VERSION >= 3
   TreeReaderData(const std::string sampleName, TTree *tree=0, TTree *metadataTree=0);
   virtual void     Init(TTree *tree, TTree *metadataTree);
   virtual Long64_t LoadMetadataTree(Long64_t entry);
   virtual void     GetMetadata();
#else
   TreeReaderData(const std::string sampleName, TTree *tree=0);
   virtual void     Init(TTree *tree);
#endif // NTUPLE_VERSION >= 3
   virtual ~TreeReaderData();
   virtual Int_t    Cut(Long64_t entry);
//    virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
//    virtual void     Loop(); // Reimplemented in daughter classes
//    virtual Bool_t   Notify();
//    virtual void     Show(Long64_t entry = -1);
};

#endif // TreeReaderData_h

#ifdef TreeReaderData_cxx
#if NTUPLE_VERSION >= 3
TreeReaderData::TreeReaderData(const std::string sampleName, TTree *tree, TTree *metadataTree)
#else
TreeReaderData::TreeReaderData(const std::string sampleName, TTree *tree)
#endif
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
#if NTUPLE_VERSION >= 3
     TChain* chain = new TChain("MyNtupleMaking/my_tree");
     TChain* metadataChain = new TChain("MyNtupleMaking/metadata_tree");
     OpenFile(sampleName, chain, metadataChain);
     tree = chain;
     metadataTree = metadataChain;
#else
     TChain* chain = new TChain("MyNtupleMaking/my_tree");
     OpenFile(sampleName, chain);
     tree = chain;
#endif
   }
#if NTUPLE_VERSION >= 3
   Init(tree, metadataTree);
#else
   Init(tree);
#endif
}



TreeReaderData::~TreeReaderData()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}



Long64_t TreeReaderData::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    Notify();
  }
  return centry;
}



#if NTUPLE_VERSION >= 3
Long64_t TreeReaderData::LoadMetadataTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fMetadataTree) return -5;
  Long64_t centry = fMetadataTree->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fMetadataTree->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fMetadataTree;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    Notify();
  }
  return centry;
}



void TreeReaderData::GetMetadata()
{
  if (fMetadataTree == 0) return;
  Long64_t ientry = LoadMetadataTree(0);
  if (ientry < 0) return;
  fMetadataTree->GetEntry(0);
}
#endif // NTUPLE_VERSION >= 3



#if NTUPLE_VERSION >= 3
void TreeReaderData::Init(TTree *tree, TTree *metadataTree)
{
  InitReco(tree);
  InitMetadata(metadataTree);
  GetMetadata();
  Notify();
}
#else
void TreeReaderData::Init(TTree *tree)
{
  InitReco(tree);
  Notify();
}
#endif



Int_t TreeReaderData::Cut(Long64_t entry)
{
  // Avoid warning about unused parameter
  // This line should be removed if the function is changed so to use the parameter entry
  (void) entry;
  
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}

#endif // #ifdef TreeReaderData_cxx
