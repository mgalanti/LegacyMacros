#ifndef TreeDefinitionBase_h
#define TreeDefinitionBase_h

#include "TChain.h"



class TreeDefinitionBase
{
  public:
    TreeDefinitionBase();
    virtual ~TreeDefinitionBase(){};
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain
#if NTUPLE_VERSION >= 3
    TTree          *fMetadataTree;   //!pointer to the metadata tree
    Int_t           fMetadataCurrent;
#endif
    
    bool hasMetadata;
    
    bool hasTriggerInfo;
    bool hasBeamSpot;
    bool hasPV;
    bool hasPVRefit;
    bool hasSV;
    bool hasTrack;
    bool hasJet;
    bool hasJetBTag;
    bool hasTrackJet;
    bool hasTrackJetBTag;
    bool hasMuon;
    bool hasDiMuVtx;
    
    bool hasGenp;
    bool hasPDFWeight;
    bool hasTkGenpRefByChi2;
    bool hasTkGenpRefByHits;
    bool hasTkGenpRefByDeltaR;
    bool hasMuGenpRefByHits;

    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);
    virtual Int_t    GetEntry(Long64_t entry);

    void CheckVariable(std::string branchName, bool& isOk);
    void CheckMetadataVariable(std::string branchName, bool& isOk);
    
    template<typename T> inline void InitVariable(std::string branchName, T*& variable, TBranch* branch)
    {
      variable = 0;
      if(fChain->GetBranchStatus(branchName.c_str()))
      {
        fChain->SetBranchAddress(branchName.c_str(), &variable, &branch);
      }
      else
      {
        std::cout << "W A R N I N G! Variable \"" << branchName << "\" is missing from the ntuple! Trying to use it will result in a crash!\n";
      }
    };
    
    template<typename T> inline void InitVariable(std::string branchName, T& variable, TBranch* branch)
    {
      if(fChain->GetBranchStatus(branchName.c_str()))
      {
        fChain->SetBranchAddress(branchName.c_str(), &variable, &branch);
      }
      else
      {
        std::cout << "W A R N I N G! Variable \"" << branchName << "\" is missing from the ntuple! Trying to use it will result in a crash!\n";
      }
    };

    template<typename T> inline void InitMetadataVariable(std::string branchName, T*& variable, TBranch* branch)
    {
      variable = 0;
      if(fMetadataTree->GetBranchStatus(branchName.c_str()))
      {
        fMetadataTree->SetBranchAddress(branchName.c_str(), &variable, &branch);
      }
      else
      {
        std::cout << "W A R N I N G! Variable \"" << branchName << "\" is missing from the metadata! Trying to use it will result in a crash!\n";
      }
    };
    
    template<typename T> inline void InitMetadataVariable(std::string branchName, T& variable, TBranch* branch)
    {
      if(fMetadataTree->GetBranchStatus(branchName.c_str()))
      {
        fMetadataTree->SetBranchAddress(branchName.c_str(), &variable, &branch);
      }
      else
      {
        std::cout << "W A R N I N G! Variable \"" << branchName << "\" is missing from the metadata! Trying to use it will result in a crash!\n";
      }
    };
};



TreeDefinitionBase::TreeDefinitionBase()
{
  hasMetadata = false;
  
  hasTriggerInfo = false;
  hasBeamSpot = false;
  hasPV = false;
  hasPVRefit = false;
  hasSV = false;
  hasTrack = false;
  hasJet = false;
  hasJetBTag = false;
  hasTrackJet = false;
  hasTrackJetBTag = false;
  hasMuon = false;
  hasDiMuVtx = false;
  
  hasGenp = false;
  hasPDFWeight = false;
  hasTkGenpRefByChi2 = false;
  hasTkGenpRefByHits = false;
  hasTkGenpRefByDeltaR = false;
  hasMuGenpRefByHits = false;
}


Int_t TreeDefinitionBase::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}



Bool_t TreeDefinitionBase::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}



void TreeDefinitionBase::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}



void TreeDefinitionBase::CheckVariable(std::string branchName, bool& isOk)
{
  if(fChain->GetBranchStatus(branchName.c_str()))
    isOk = true;
}



void TreeDefinitionBase::CheckMetadataVariable(std::string branchName, bool& isOk)
{
  if(fMetadataTree->GetBranchStatus(branchName.c_str()))
    isOk = true;
}



#endif // TreeDefinitionBase_h
