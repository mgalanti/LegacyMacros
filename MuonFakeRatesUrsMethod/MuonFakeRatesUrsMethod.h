#ifndef MuonFakeRatesUrsMethod_h
#define MuonFakeRatesUrsMethod_h

#include <iostream>

#include "../utilities/TreeAnalyzerMC.C"

class MuonFakeRatesUrsMethod : public TreeAnalyzerMC
{
  public:
    MuonFakeRatesUrsMethod(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) :
      TreeAnalyzerMC("MuonFakeRatesUrsMethod", sampleName_, text_, maxEvents_, tree) {};
    virtual void Loop();

  private:
    // Nothing by default
    // Add your private variables/methods here
    int getGenIndexWithDeltaR(double pt, double eta, double phi, double charge);
    void setTitles(TH1 *h, const char *sx, const char *sy, 
                   float size = 0.05, float xoff = 1.1, float yoff = 1.1, float lsize = 0.05, int font = 42);
    
    void InitializeMuonTMVA();
    bool SelectTMVAMuonV5(const unsigned int iMuon);
    
    TMVA::Reader* reader;
    
    float muonTMVACut;
    
    float muonTMVATrackValidFraction;
    float muonTMVAGlobalChi2;
    float muonTMVAPt;
    float muonTMVAEta;
    float muonTMVASegmentCompatibility;
    float muonTMVAChi2LocalMomentum;
    float muonTMVAChi2LocalPosition;
    float muonTMVAGlobalTrackProbability;
    float muonTMVATrackNumberOfValidHits;
    float muonTMVATrackExpectedHitsOuter;
};

#endif // MuonFakeRatesUrsMethod_h
