#ifndef TreeDefinitionGenV2_h
#define TreeDefinitionGenV2_h

#include "TreeDefinitionBase.h"



class TreeDefinitionGenV2 : public virtual TreeDefinitionBase
{
  public:
    
    std::vector<std::vector<unsigned short> > *Track_vGenprefByChi2;
    std::vector<std::vector<double> > *Track_vGenpChi2;
    std::vector<std::vector<unsigned short> > *Track_vGenprefByHits;
    std::vector<std::vector<double> > *Track_vGenpQualityByHits;
    
    std::vector<double>  *DeltaR_clst;
    std::vector<double>  *DeltaPt_clst;
    std::vector<unsigned short> *Genp_idx_clst;
    
    std::vector<std::vector<double> > *Genevt_pdfWeights;

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
    std::vector<double>  *Genp_dxy;
    std::vector<double>  *Genp_dxyError;
    std::vector<double>  *Genp_dz;
    std::vector<double>  *Genp_dzError;
    std::vector<double>  *Genp_dxyz;
    std::vector<double>  *Genp_dxyzError;
    std::vector<std::vector<double> > *Genp_dxy_PV;
    std::vector<std::vector<double> > *Genp_dxyError_PV;
    std::vector<std::vector<double> > *Genp_dxyz_PV;
    std::vector<std::vector<double> > *Genp_dxyzError_PV;
    std::vector<std::vector<double> > *Genp_dz_PV;
    std::vector<std::vector<double> > *Genp_dzError_PV;

    std::vector< std::vector<unsigned short> > *Muon_vGlobalGenprefByHits;
    std::vector< std::vector<double> > *Muon_vGlobalGenpQualityByHits;
    std::vector< std::vector<unsigned short> > *Muon_vInnerGenprefByHits;
    std::vector< std::vector<double> > *Muon_vInnerGenpQualityByHits;
    std::vector< std::vector<unsigned short> > *Muon_vOuterGenprefByHits;
    std::vector< std::vector<double> > *Muon_vOuterGenpQualityByHits;

    // List of branches
    
    TBranch        *b_Track_vGenprefByChi2;   //!
    TBranch        *b_Track_vGenpChi2;   //!
    TBranch        *b_Track_vGenprefByHits;   //!
    TBranch        *b_Track_vGenpQualityByHits;   //!
    TBranch        *b_DeltaR_clst;   //!
    TBranch        *b_DeltaPt_clst;   //!
    TBranch        *b_Genp_idx_clst;   //!

    TBranch        *b_Genevt_pdfWeights;   //!

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
    TBranch        *b_Genp_dxy;   //!
    TBranch        *b_Genp_dxyError;   //!
    TBranch        *b_Genp_dz;   //!
    TBranch        *b_Genp_dzError;   //!
    TBranch        *b_Genp_dxyz;   //!
    TBranch        *b_Genp_dxyzError;   //!
    TBranch        *b_Genp_dxy_PV;   //!
    TBranch        *b_Genp_dxyError_PV;   //!
    TBranch        *b_Genp_dxyz_PV;   //!
    TBranch        *b_Genp_dxyzError_PV;   //!
    TBranch        *b_Genp_dz_PV;   //!
    TBranch        *b_Genp_dzError_PV;   //!

    TBranch        *b_Muon_vGlobalGenprefByHits;   //!
    TBranch        *b_Muon_vGlobalGenpQualityByHits;   //!
    TBranch        *b_Muon_vInnerGenprefByHits;   //!
    TBranch        *b_Muon_vInnerGenpQualityByHits;   //!
    TBranch        *b_Muon_vOuterGenprefByHits;   //!
    TBranch        *b_Muon_vOuterGenpQualityByHits;   //!

    virtual void     InitGen(TTree *tree);
};



void TreeDefinitionGenV2::InitGen(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  std::cout << "Setting branch addresses for gen...\n";
  
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  InitVariable("Track_vGenprefByChi2", Track_vGenprefByChi2, b_Track_vGenprefByChi2);
  InitVariable("Track_vGenpChi2", Track_vGenpChi2, b_Track_vGenpChi2);
  InitVariable("Track_vGenprefByHits", Track_vGenprefByHits, b_Track_vGenprefByHits);
  InitVariable("Track_vGenpQualityByHits", Track_vGenpQualityByHits, b_Track_vGenpQualityByHits);
  InitVariable("DeltaR_clst", DeltaR_clst, b_DeltaR_clst);
  InitVariable("DeltaPt_clst", DeltaPt_clst, b_DeltaPt_clst);
  InitVariable("Genp_idx_clst", Genp_idx_clst, b_Genp_idx_clst);
  CheckVariable("Track_vGenprefByChi2", hasTkGenpRefByChi2);
  CheckVariable("Track_vGenprefByHits", hasTkGenpRefByHits);
  CheckVariable("DeltaR_clst", hasTkGenpRefByDeltaR);

  InitVariable("Genevt_pdfWeights", Genevt_pdfWeights, b_Genevt_pdfWeights);
  CheckVariable("Genevt_pdfWeights", hasPDFWeight);
  
  InitVariable("Genp_status", Genp_status, b_Genp_status);
  InitVariable("Genp_Id", Genp_Id, b_Genp_Id);
  InitVariable("Genp_vMom", Genp_vMom, b_Genp_vMom);
  InitVariable("Genp_vDau", Genp_vDau, b_Genp_vDau);
  InitVariable("Genp_charge", Genp_charge, b_Genp_charge);
  InitVariable("Genp_pt", Genp_pt, b_Genp_pt);
  InitVariable("Genp_eta", Genp_eta, b_Genp_eta);
  InitVariable("Genp_phi", Genp_phi, b_Genp_phi);
  InitVariable("Genp_mass", Genp_mass, b_Genp_mass);
  InitVariable("Genp_vx", Genp_vx, b_Genp_vx);
  InitVariable("Genp_vy", Genp_vy, b_Genp_vy);
  InitVariable("Genp_vz", Genp_vz, b_Genp_vz);
  InitVariable("Genp_dxy", Genp_dxy, b_Genp_dxy);
  InitVariable("Genp_dxyError", Genp_dxyError, b_Genp_dxyError);
  InitVariable("Genp_dz", Genp_dz, b_Genp_dz);
  InitVariable("Genp_dzError", Genp_dzError, b_Genp_dzError);
  InitVariable("Genp_dxyz", Genp_dxyz, b_Genp_dxyz);
  InitVariable("Genp_dxyzError", Genp_dxyzError, b_Genp_dxyzError);
  InitVariable("Genp_dxy_PV", Genp_dxy_PV, b_Genp_dxy_PV);
  InitVariable("Genp_dxyError_PV", Genp_dxyError_PV, b_Genp_dxyError_PV);
  InitVariable("Genp_dxyz_PV", Genp_dxyz_PV, b_Genp_dxyz_PV);
  InitVariable("Genp_dxyzError_PV", Genp_dxyzError_PV, b_Genp_dxyzError_PV);
  InitVariable("Genp_dz_PV", Genp_dz_PV, b_Genp_dz_PV);
  InitVariable("Genp_dzError_PV", Genp_dzError_PV, b_Genp_dzError_PV);
  CheckVariable("Genp_Id", hasGenp);
  
  InitVariable("Muon_vGlobalGenprefByHits", Muon_vGlobalGenprefByHits, b_Muon_vGlobalGenprefByHits);
  InitVariable("Muon_vGlobalGenpQualityByHits", Muon_vGlobalGenpQualityByHits, b_Muon_vGlobalGenpQualityByHits);
  InitVariable("Muon_vInnerGenprefByHits", Muon_vInnerGenprefByHits, b_Muon_vInnerGenprefByHits);
  InitVariable("Muon_vInnerGenpQualityByHits", Muon_vInnerGenpQualityByHits, b_Muon_vInnerGenpQualityByHits);
  InitVariable("Muon_vOuterGenprefByHits", Muon_vOuterGenprefByHits, b_Muon_vOuterGenprefByHits);
  InitVariable("Muon_vOuterGenpQualityByHits", Muon_vOuterGenpQualityByHits, b_Muon_vOuterGenpQualityByHits);
  CheckVariable("Muon_vOuterGenprefByHits", hasMuGenpRefByHits);
}

#endif // TreeDefinitionGenV2_h
