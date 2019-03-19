//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 10 16:17:02 2013 by ROOT version 5.32/00
// from TTree SgData_bdt/SgData_bdt
// found on file: small-test.root
//////////////////////////////////////////////////////////

#ifndef CountingTests_h
#define CountingTests_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "../utilities/HistoPlotter.C"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class CountingTests : public HistoPlotter {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           run;
   Int_t           evt;
   Int_t           ls;
   Double_t        m1bdt;
   Double_t        m2bdt;
   Bool_t          muid;
   Double_t        bdt;
   Int_t           pvn;
   Double_t        pt;
   Double_t        eta;
   Double_t        m1pt;
   Double_t        m2pt;
   Double_t        m;
   Double_t        m3;
   Double_t        m4;
   Double_t        me;
   Double_t        tau;
   Double_t        gtau;
   Double_t        m1eta;
   Double_t        m2eta;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_ls;   //!
   TBranch        *b_m1bdt;   //!
   TBranch        *b_m2bdt;   //!
   TBranch        *b_muid;   //!
   TBranch        *b_bdt;   //!
   TBranch        *b_pvn;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_m1pt;   //!
   TBranch        *b_m2pt;   //!
   TBranch        *b_m;   //!
   TBranch        *b_m3;   //!
   TBranch        *b_m4;   //!
   TBranch        *b_me;   //!
   TBranch        *b_tau;   //!
   TBranch        *b_gtau;   //!
   TBranch        *b_m1eta;   //!
   TBranch        *b_m2eta;   //!

   CountingTests(TTree *tree=0);
   virtual ~CountingTests();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef CountingTests_cxx
CountingTests::CountingTests(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("small-test.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("small-test.root");
      }
      f->GetObject("SgData_bdt",tree);

   }
   Init(tree);
}

CountingTests::~CountingTests()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t CountingTests::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t CountingTests::LoadTree(Long64_t entry)
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

void CountingTests::Init(TTree *tree)
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

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("ls", &ls, &b_ls);
   fChain->SetBranchAddress("m1bdt", &m1bdt, &b_m1bdt);
   fChain->SetBranchAddress("m2bdt", &m2bdt, &b_m2bdt);
   fChain->SetBranchAddress("muid", &muid, &b_muid);
   fChain->SetBranchAddress("bdt", &bdt, &b_bdt);
   fChain->SetBranchAddress("pvn", &pvn, &b_pvn);
   fChain->SetBranchAddress("pt", &pt, &b_pt);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("m1pt", &m1pt, &b_m1pt);
   fChain->SetBranchAddress("m2pt", &m2pt, &b_m2pt);
   fChain->SetBranchAddress("m", &m, &b_m);
   fChain->SetBranchAddress("m3", &m3, &b_m3);
   fChain->SetBranchAddress("m4", &m4, &b_m4);
   fChain->SetBranchAddress("me", &me, &b_me);
   fChain->SetBranchAddress("tau", &tau, &b_tau);
   fChain->SetBranchAddress("gtau", &gtau, &b_gtau);
   fChain->SetBranchAddress("m1eta", &m1eta, &b_m1eta);
   fChain->SetBranchAddress("m2eta", &m2eta, &b_m2eta);
   Notify();
}

Bool_t CountingTests::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void CountingTests::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t CountingTests::Cut(Long64_t entry)
{
  (int)entry;
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef CountingTests_cxx
