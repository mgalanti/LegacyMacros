//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Apr 24 10:58:17 2013 by ROOT version 5.32/00
// from TTree TrainTree/TrainTree
// found on file: TMVA_Mu_K.130422v1.root
//////////////////////////////////////////////////////////

#ifndef SimpleTMVAResultsAnalyzer_h
#define SimpleTMVAResultsAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "../utilities/HistoPlotter.C"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class SimpleTMVAResultsAnalyzer : public HistoPlotter {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           classID;
   Char_t          className[11];
   Float_t         trkValidFract;
   Float_t         glbNChi2;
   Float_t         pt;
   Float_t         eta;
   Float_t         segComp;
   Float_t         chi2LocMom;
   Float_t         chi2LocPos;
   Float_t         glbTrackProb;
   Float_t         NTrkVHits;
   Float_t         NTrkEHitsOut;
   Float_t         weight;
   Float_t         BDT;

   // List of branches
   TBranch        *b_classID;   //!
   TBranch        *b_className;   //!
   TBranch        *b_trkValidFract;   //!
   TBranch        *b_glbNChi2;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_segComp;   //!
   TBranch        *b_chi2LocMom;   //!
   TBranch        *b_chi2LocPos;   //!
   TBranch        *b_glbTrackProb;   //!
   TBranch        *b_NTrkVHits;   //!
   TBranch        *b_NTrkEHitsOut;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_BDT;   //!

   SimpleTMVAResultsAnalyzer(const std::string inputFileName_, TTree *tree=0);
   virtual ~SimpleTMVAResultsAnalyzer();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const int muonClassId, const std::string muonClassName);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   
   std::string inputFileName;
};

#endif

#ifdef SimpleTMVAResultsAnalyzer_cxx
SimpleTMVAResultsAnalyzer::SimpleTMVAResultsAnalyzer(const std::string inputFileName_, TTree *tree) : fChain(0), inputFileName(inputFileName_)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(inputFileName.c_str());
      std::string inputDirectoryAndFileName="../tmvaWeightFiles/" + inputFileName;
      if (!f || !f->IsOpen()) {
         f = new TFile(inputDirectoryAndFileName.c_str());
      }
      f->GetObject("TrainTree",tree);

   }
   Init(tree);
}

SimpleTMVAResultsAnalyzer::~SimpleTMVAResultsAnalyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SimpleTMVAResultsAnalyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t SimpleTMVAResultsAnalyzer::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void SimpleTMVAResultsAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("classID", &classID, &b_classID);
   fChain->SetBranchAddress("className", className, &b_className);
   fChain->SetBranchAddress("trkValidFract", &trkValidFract, &b_trkValidFract);
   fChain->SetBranchAddress("glbNChi2", &glbNChi2, &b_glbNChi2);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("segComp", &segComp, &b_segComp);
   fChain->SetBranchAddress("chi2LocMom", &chi2LocMom, &b_chi2LocMom);
   fChain->SetBranchAddress("chi2LocPos", &chi2LocPos, &b_chi2LocPos);
   fChain->SetBranchAddress("glbTrackProb", &glbTrackProb, &b_glbTrackProb);
   fChain->SetBranchAddress("NTrkVHits", &NTrkVHits, &b_NTrkVHits);
   fChain->SetBranchAddress("NTrkEHitsOut", &NTrkEHitsOut, &b_NTrkEHitsOut);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("BDT", &BDT, &b_BDT);
   Notify();
}

Bool_t SimpleTMVAResultsAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void SimpleTMVAResultsAnalyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t SimpleTMVAResultsAnalyzer::Cut(Long64_t entry)
{
  // Avoid warning about unused parameter
  // This line should be removed if the function is changed so to use the parameter entry
  (void) entry;
  
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef SimpleTMVAResultsAnalyzer_cxx
