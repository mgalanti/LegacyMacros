#ifndef TreeDefinitionGenV1_h
#define TreeDefinitionGenV1_h

#include "TreeDefinitionBase.h"



class TreeDefinitionGenV1 : public virtual TreeDefinitionBase
{
  public:
    std::vector<std::vector<unsigned short> > *Track_vGenpref;
    std::vector<std::vector<double> > *Track_vGenpChi2;

    std::vector<unsigned short> *Genp_status;
    std::vector<int>     *Genp_Id;
    std::vector<std::vector<unsigned short> > *Genp_vMom;
    std::vector<std::vector<unsigned short> > *Genp_vDau;
    std::vector<float>   *Genp_charge;
    std::vector<double>  *Genp_pt;
    std::vector<double>  *Genp_eta;
    std::vector<double>  *Genp_phi;
    std::vector<double>  *Genp_mass;
    std::vector<double>  *Genp_vx;
    std::vector<double>  *Genp_vy;
    std::vector<double>  *Genp_vz;
    std::vector<double>  *DeltaR_clst;
    std::vector<double>  *DeltaPt_clst;
    std::vector<unsigned short> *Genp_idx_clst;

    // List of branches
    TBranch        *b_Track_vGenpref;   //!
    TBranch        *b_Track_vGenpChi2;   //!

    TBranch        *b_Genp_status;   //!
    TBranch        *b_Genp_Id;   //!
    TBranch        *b_Genp_vMom;   //!
    TBranch        *b_Genp_vDau;   //!
    TBranch        *b_Genp_charge;   //!
    TBranch        *b_Genp_pt;   //!
    TBranch        *b_Genp_eta;   //!
    TBranch        *b_Genp_phi;   //!
    TBranch        *b_Genp_mass;   //!
    TBranch        *b_Genp_vx;   //!
    TBranch        *b_Genp_vy;   //!
    TBranch        *b_Genp_vz;   //!
    TBranch        *b_DeltaR_clst;   //!
    TBranch        *b_DeltaPt_clst;   //!
    TBranch        *b_Genp_idx_clst;   //!

    virtual void     InitGen(TTree *tree);
};



void TreeDefinitionGenV1::InitGen(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set object pointer
  Track_vGenpref = 0;
  Track_vGenpChi2 = 0;

  Genp_status = 0;
  Genp_Id = 0;
  Genp_vMom = 0;
  Genp_vDau = 0;
  Genp_charge = 0;
  Genp_pt = 0;
  Genp_eta = 0;
  Genp_phi = 0;
  Genp_mass = 0;
  Genp_vx = 0;
  Genp_vy = 0;
  Genp_vz = 0;
  DeltaR_clst = 0;
  DeltaPt_clst = 0;
  Genp_idx_clst = 0;

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("Track_vGenpref", &Track_vGenpref, &b_Track_vGenpref);
  fChain->SetBranchAddress("Track_vGenpChi2", &Track_vGenpChi2, &b_Track_vGenpChi2);

  fChain->SetBranchAddress("Genp_status", &Genp_status, &b_Genp_status);
  fChain->SetBranchAddress("Genp_Id", &Genp_Id, &b_Genp_Id);
  fChain->SetBranchAddress("Genp_vMom", &Genp_vMom, &b_Genp_vMom);
  fChain->SetBranchAddress("Genp_vDau", &Genp_vDau, &b_Genp_vDau);
  fChain->SetBranchAddress("Genp_charge", &Genp_charge, &b_Genp_charge);
  fChain->SetBranchAddress("Genp_pt", &Genp_pt, &b_Genp_pt);
  fChain->SetBranchAddress("Genp_eta", &Genp_eta, &b_Genp_eta);
  fChain->SetBranchAddress("Genp_phi", &Genp_phi, &b_Genp_phi);
  fChain->SetBranchAddress("Genp_mass", &Genp_mass, &b_Genp_mass);
  fChain->SetBranchAddress("Genp_vx", &Genp_vx, &b_Genp_vx);
  fChain->SetBranchAddress("Genp_vy", &Genp_vy, &b_Genp_vy);
  fChain->SetBranchAddress("Genp_vz", &Genp_vz, &b_Genp_vz);
  fChain->SetBranchAddress("DeltaR_clst", &DeltaR_clst, &b_DeltaR_clst);
  fChain->SetBranchAddress("DeltaPt_clst", &DeltaPt_clst, &b_DeltaPt_clst);
  fChain->SetBranchAddress("Genp_idx_clst", &Genp_idx_clst, &b_Genp_idx_clst);
}

#endif // TreeDefinitionGenV1_h
