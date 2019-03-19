#ifndef EfficiencyFactorsGetter_h
#define EfficiencyFactorsGetter_h

#include "TH2D.h"



class EfficiencyFactorsGetter
{
  public:
    EfficiencyFactorsGetter() {InitEfficiencyFactorsGetter();};
    virtual ~EfficiencyFactorsGetter() {CloseEffFile();};
    
    double GetEfficiency(double pt, double eta, const std::string method);
    
    double GetTkEfficiency(double pt, double eta, const std::string method = "data");
    double GetTkEfficiencyData(double pt, double eta);
    double GetTkEfficiencyMC(double pt, double eta);
    double GetTkEfficiencyMCTruth(double pt, double eta);
    double GetTkEfficiencyMCTruthBB(double pt, double eta);

    double GetTkEfficiencyHiErr(double pt, double eta, const std::string method = "data");
    double GetTkEfficiencyDataHiErr(double pt, double eta);
    double GetTkEfficiencyMCHiErr(double pt, double eta);
    double GetTkEfficiencyMCTruthHiErr(double pt, double eta);
    double GetTkEfficiencyMCTruthBBHiErr(double pt, double eta);
    
    double GetTkEfficiencyLoErr(double pt, double eta, const std::string method = "data");
    double GetTkEfficiencyDataLoErr(double pt, double eta);
    double GetTkEfficiencyMCLoErr(double pt, double eta);
    double GetTkEfficiencyMCTruthLoErr(double pt, double eta);
    double GetTkEfficiencyMCTruthBBLoErr(double pt, double eta);
    
    double GetRecoSelEfficiency(double pt, double eta, const std::string method = "data");
    double GetRecoSelEfficiencyData(double pt, double eta);
    double GetRecoSelEfficiencyMC(double pt, double eta);
    double GetRecoSelEfficiencyMCTruth(double pt, double eta);
    double GetRecoSelEfficiencyMCTruthBB(double pt, double eta);
    double GetRecoSelEfficiencyMCTruthBBGen(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetRecoSelEfficiencyHiErr(double pt, double eta, const std::string method = "data");
    double GetRecoSelEfficiencyDataHiErr(double pt, double eta);
    double GetRecoSelEfficiencyMCHiErr(double pt, double eta);
    double GetRecoSelEfficiencyMCTruthHiErr(double pt, double eta);
    double GetRecoSelEfficiencyMCTruthBBHiErr(double pt, double eta);
    double GetRecoSelEfficiencyMCTruthBBGenHiErr(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetRecoSelEfficiencyLoErr(double pt, double eta, const std::string method = "data");
    double GetRecoSelEfficiencyDataLoErr(double pt, double eta);
    double GetRecoSelEfficiencyMCLoErr(double pt, double eta);
    double GetRecoSelEfficiencyMCTruthLoErr(double pt, double eta);
    double GetRecoSelEfficiencyMCTruthBBLoErr(double pt, double eta);
    double GetRecoSelEfficiencyMCTruthBBGenLoErr(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetMuSelEfficiency(double pt, double eta, const std::string method = "data");
    double GetMuSelEfficiencyData(double pt, double eta);
    double GetMuSelEfficiencyMC(double pt, double eta);
    double GetMuSelEfficiencyMCTruth(double pt, double eta);
    double GetMuSelEfficiencyMCTruthBB(double pt, double eta);
    double GetMuSelEfficiencyMCTruthBBGen(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetMuSelEfficiencyHiErr(double pt, double eta, const std::string method = "data");
    double GetMuSelEfficiencyDataHiErr(double pt, double eta);
    double GetMuSelEfficiencyMCHiErr(double pt, double eta);
    double GetMuSelEfficiencyMCTruthHiErr(double pt, double eta);
    double GetMuSelEfficiencyMCTruthBBHiErr(double pt, double eta);
    double GetMuSelEfficiencyMCTruthBBGenHiErr(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetMuSelEfficiencyLoErr(double pt, double eta, const std::string method = "data");
    double GetMuSelEfficiencyDataLoErr(double pt, double eta);
    double GetMuSelEfficiencyMCLoErr(double pt, double eta);
    double GetMuSelEfficiencyMCTruthLoErr(double pt, double eta);
    double GetMuSelEfficiencyMCTruthBBLoErr(double pt, double eta);
    double GetMuSelEfficiencyMCTruthBBGenLoErr(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetTrgEfficiency(double pt, double eta, const std::string method = "data");
    double GetTrgEfficiencyData(double pt, double eta);
    double GetTrgEfficiencyMC(double pt, double eta);
    double GetTrgEfficiencyMCTruth(double pt, double eta);
    double GetTrgEfficiencyMCTruthBB(double pt, double eta);
    double GetTrgEfficiencyMCTruthBBGen(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetTrgEfficiencyHiErr(double pt, double eta, const std::string method = "data");
    double GetTrgEfficiencyDataHiErr(double pt, double eta);
    double GetTrgEfficiencyMCHiErr(double pt, double eta);
    double GetTrgEfficiencyMCTruthHiErr(double pt, double eta);
    double GetTrgEfficiencyMCTruthBBHiErr(double pt, double eta);
    double GetTrgEfficiencyMCTruthBBGenHiErr(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetTrgEfficiencyLoErr(double pt, double eta, const std::string method = "data");
    double GetTrgEfficiencyDataLoErr(double pt, double eta);
    double GetTrgEfficiencyMCLoErr(double pt, double eta);
    double GetTrgEfficiencyMCTruthLoErr(double pt, double eta);
    double GetTrgEfficiencyMCTruthBBLoErr(double pt, double eta);
    double GetTrgEfficiencyMCTruthBBGenLoErr(double pt, double eta); // This is the eff as a function of the gen pt and eta
    
    double GetGlobalEfficiency(double pt, double eta, const std::string method = "data");
//     double GetGlobalEfficiencyData(double pt, double eta);
//     double GetGlobalEfficiencyMC(double pt, double eta);
//     double GetGlobalEfficiencyMCTruth(double pt, double eta);
//     double GetGlobalEfficiencyMCTruthBB(double pt, double eta);
    
    double GetMCNLORescale(double pt, double eta);
    double GetMCNLORescaleHiErr(double pt, double eta);
    double GetMCNLORescaleLoErr(double pt, double eta);
    
    double FindEventEfficiencyFromSingleParticle(std::vector<double> vEfficiencies);
    double FindEventWeightFromSingleParticle(std::vector<double> vWeights);
    
    void FindEventEfficiencyFromSingleParticleWithErrors(std::vector<double> vEfficiencies, std::vector<double> vLoErrors, std::vector<double> vHiErrors, double& result, double& resultLoErr, double& resultHiErr);
    
    void WriteEfficiencyMetadata(TFile *);
    

  private:
    void InitEfficiencyFactorsGetter();
    void InitEfficiencyHistograms();
    void InitEfficiencyHistogramsLimits();
    void CloseEffFile() {fEffInput->Close();};
    TH2D* GetTH2D(const std::string name);
    
    TFile* fEffInput;
    
    TH2D* hTkEffData;
    TH2D* hTkEffData_Hi;
    TH2D* hTkEffData_Lo;
    double tkEffDataPtMin;
    double tkEffDataPtMax;
    double tkEffDataEtaMin;
    double tkEffDataEtaMax;
    TH2D* hTkEffMC;
    TH2D* hTkEffMC_Hi;
    TH2D* hTkEffMC_Lo;
    double tkEffMCPtMin;
    double tkEffMCPtMax;
    double tkEffMCEtaMin;
    double tkEffMCEtaMax;
    TH2D* hTkEffMCTruth;
    TH2D* hTkEffMCTruth_Hi;
    TH2D* hTkEffMCTruth_Lo;
    double tkEffMCTruthPtMin;
    double tkEffMCTruthPtMax;
    double tkEffMCTruthEtaMin;
    double tkEffMCTruthEtaMax;
    TH2D* hTkEffMCTruthBB;
    TH2D* hTkEffMCTruthBB_Hi;
    TH2D* hTkEffMCTruthBB_Lo;
    double tkEffMCTruthBBPtMin;
    double tkEffMCTruthBBPtMax;
    double tkEffMCTruthBBEtaMin;
    double tkEffMCTruthBBEtaMax;
    
    TH2D* hRecoSelEffData;
    TH2D* hRecoSelEffData_Hi;
    TH2D* hRecoSelEffData_Lo;
    double recoSelEffDataPtMin;
    double recoSelEffDataPtMax;
    double recoSelEffDataEtaMin;
    double recoSelEffDataEtaMax;
    TH2D* hRecoSelEffMC;
    TH2D* hRecoSelEffMC_Hi;
    TH2D* hRecoSelEffMC_Lo;
    double recoSelEffMCPtMin;
    double recoSelEffMCPtMax;
    double recoSelEffMCEtaMin;
    double recoSelEffMCEtaMax;
    TH2D* hRecoSelEffMCTruth;
    TH2D* hRecoSelEffMCTruth_Hi;
    TH2D* hRecoSelEffMCTruth_Lo;
    double recoSelEffMCTruthPtMin;
    double recoSelEffMCTruthPtMax;
    double recoSelEffMCTruthEtaMin;
    double recoSelEffMCTruthEtaMax;
    TH2D* hRecoSelEffMCTruthBB;
    TH2D* hRecoSelEffMCTruthBB_Hi;
    TH2D* hRecoSelEffMCTruthBB_Lo;
    double recoSelEffMCTruthBBPtMin;
    double recoSelEffMCTruthBBPtMax;
    double recoSelEffMCTruthBBEtaMin;
    double recoSelEffMCTruthBBEtaMax;
    TH2D* hRecoSelEffMCTruthBBGen;
    TH2D* hRecoSelEffMCTruthBBGen_Hi;
    TH2D* hRecoSelEffMCTruthBBGen_Lo;
    double recoSelEffMCTruthBBGenPtMin;
    double recoSelEffMCTruthBBGenPtMax;
    double recoSelEffMCTruthBBGenEtaMin;
    double recoSelEffMCTruthBBGenEtaMax;

    TH2D* hMuSelEffData;
    TH2D* hMuSelEffData_Hi;
    TH2D* hMuSelEffData_Lo;
    double muSelEffDataPtMin;
    double muSelEffDataPtMax;
    double muSelEffDataEtaMin;
    double muSelEffDataEtaMax;
    TH2D* hMuSelEffMC;
    TH2D* hMuSelEffMC_Hi;
    TH2D* hMuSelEffMC_Lo;
    double muSelEffMCPtMin;
    double muSelEffMCPtMax;
    double muSelEffMCEtaMin;
    double muSelEffMCEtaMax;
    TH2D* hMuSelEffMCTruth;
    TH2D* hMuSelEffMCTruth_Hi;
    TH2D* hMuSelEffMCTruth_Lo;
    double muSelEffMCTruthPtMin;
    double muSelEffMCTruthPtMax;
    double muSelEffMCTruthEtaMin;
    double muSelEffMCTruthEtaMax;
    TH2D* hMuSelEffMCTruthBB;
    TH2D* hMuSelEffMCTruthBB_Hi;
    TH2D* hMuSelEffMCTruthBB_Lo;
    double muSelEffMCTruthBBPtMin;
    double muSelEffMCTruthBBPtMax;
    double muSelEffMCTruthBBEtaMin;
    double muSelEffMCTruthBBEtaMax;
    TH2D* hMuSelEffMCTruthBBGen;
    TH2D* hMuSelEffMCTruthBBGen_Hi;
    TH2D* hMuSelEffMCTruthBBGen_Lo;
    double muSelEffMCTruthBBGenPtMin;
    double muSelEffMCTruthBBGenPtMax;
    double muSelEffMCTruthBBGenEtaMin;
    double muSelEffMCTruthBBGenEtaMax;
    
    TH2D* hTrgEffData;
    TH2D* hTrgEffData_Hi;
    TH2D* hTrgEffData_Lo;
    double trgEffDataPtMin;
    double trgEffDataPtMax;
    double trgEffDataEtaMin;
    double trgEffDataEtaMax;
    TH2D* hTrgEffMC;
    TH2D* hTrgEffMC_Hi;
    TH2D* hTrgEffMC_Lo;
    double trgEffMCPtMin;
    double trgEffMCPtMax;
    double trgEffMCEtaMin;
    double trgEffMCEtaMax;
    TH2D* hTrgEffMCTruth;
    TH2D* hTrgEffMCTruth_Hi;
    TH2D* hTrgEffMCTruth_Lo;
    double trgEffMCTruthPtMin;
    double trgEffMCTruthPtMax;
    double trgEffMCTruthEtaMin;
    double trgEffMCTruthEtaMax;
    TH2D* hTrgEffMCTruthBB;
    TH2D* hTrgEffMCTruthBB_Hi;
    TH2D* hTrgEffMCTruthBB_Lo;
    double trgEffMCTruthBBPtMin;
    double trgEffMCTruthBBPtMax;
    double trgEffMCTruthBBEtaMin;
    double trgEffMCTruthBBEtaMax;
    TH2D* hTrgEffMCTruthBBGen;
    TH2D* hTrgEffMCTruthBBGen_Hi;
    TH2D* hTrgEffMCTruthBBGen_Lo;
    double trgEffMCTruthBBGenPtMin;
    double trgEffMCTruthBBGenPtMax;
    double trgEffMCTruthBBGenEtaMin;
    double trgEffMCTruthBBGenEtaMax;
    
    TH2D* hMCNLORescale;
    TH2D* hMCNLORescale_Hi;
    TH2D* hMCNLORescale_Lo;
    double mcNLORescalePtMin;
    double mcNLORescalePtMax;
    double mcNLORescaleEtaMin;
    double mcNLORescaleEtaMax;
};

#endif // EfficiencyFactorsGetter_h