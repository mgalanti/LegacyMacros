#if !defined(ParticleSelectorData_h) || !defined(ParticleSelectorMC_h)
#if PARTICLESELECTOR == ParticleSelectorData
#define ParticleSelectorData_h
#elif PARTICLESELECTOR == ParticleSelectorMC
#define ParticleSelectorMC_h
#endif


#include <map>
#include <string>



#if NTUPLE_VERSION == 1
#include "TreeDefinitionRecoV1.h"
#elif  NTUPLE_VERSION >= 2
#include "TreeDefinitionRecoV2.h"
#endif // NTUPLE_VERSION

#ifdef USING_MC
#if NTUPLE_VERSION == 1
#include "TreeDefinitionGenV1.h"
#elif  NTUPLE_VERSION >= 2
#include "TreeDefinitionGenV2.h"
#endif // NTUPLE_VERSION
// #define TREEANALYZER TreeAnalyzerMC
#include "TreeAnalyzerMC.h"
#else
// #define TREEANALYZER TreeAnalyzerData
#include "TreeAnalyzerData.h"
#endif // USING_MC

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#endif

#ifndef MuonSelectionType_struct
#define MuonSelectionType_struct
// Copied from DataFormats/MuonReco/interface/MuonSelectors.h
namespace muon
{
  enum SelectionType
  {
    All = 0,                      // dummy options - always true
    AllGlobalMuons = 1,           // checks isGlobalMuon flag
    AllStandAloneMuons = 2,       // checks isStandAloneMuon flag
    AllTrackerMuons = 3,          // checks isTrackerMuon flag
    TrackerMuonArbitrated = 4,    // resolve ambiguity of sharing segments
    AllArbitrated = 5,            // all muons with the tracker muon arbitrated
    GlobalMuonPromptTight = 6,    // global muons with tighter fit requirements
    TMLastStationLoose = 7,       // penetration depth loose selector
    TMLastStationTight = 8,       // penetration depth tight selector
    TM2DCompatibilityLoose = 9,   // likelihood based loose selector
    TM2DCompatibilityTight = 10,  // likelihood based tight selector
    TMOneStationLoose = 11,       // require one well matched segment
    TMOneStationTight = 12,       // require one well matched segment
    TMLastStationOptimizedLowPtLoose = 13, // combination of TMLastStation and TMOneStation
    TMLastStationOptimizedLowPtTight = 14, // combination of TMLastStation and TMOneStation
    GMTkChiCompatibility = 15,    // require tk stub have good chi2 relative to glb track
    GMStaChiCompatibility = 16,   // require sta stub have good chi2 compatibility relative to glb track
    GMTkKinkTight = 17,           // require a small kink value in the tracker stub
    TMLastStationAngLoose = 18,   // TMLastStationLoose with additional angular cuts
    TMLastStationAngTight = 19,   // TMLastStationTight with additional angular cuts
    TMOneStationAngLoose = 20,    // TMOneStationLoose with additional angular cuts
    TMOneStationAngTight = 21,    // TMOneStationTight with additional angular cuts
    // The two algorithms that follow are identical to what were known as
    // TMLastStationOptimizedLowPt* (sans the Barrel) as late as revision
    // 1.7 of this file. The names were changed because indeed the low pt
    // optimization applies only to the barrel region, whereas the sel-
    // ectors above are more efficient at low pt in the endcaps, which is
    // what we feel is more suggestive of the algorithm name. This will be
    // less confusing for future generations of CMS members, I hope...
    TMLastStationOptimizedBarrelLowPtLoose = 22, // combination of TMLastStation and TMOneStation but with low pT optimization in barrel only
    TMLastStationOptimizedBarrelLowPtTight = 23  // combination of TMLastStation and TMOneStation but with low pT optimization in barrel only
  };
}
#endif // MuonSelectionType_struct



class PARTICLESELECTOR
{
  public:
    PARTICLESELECTOR( TREEANALYZER* t_ );
    PARTICLESELECTOR( TREEANALYZER* t_, const std::string selectionString_ );
    
    bool MuonSelectorIsValid() {return muStringOk;};
    bool JetSelectorIsValid() {return jetStringOk;};

    std::string GetMuonSelector() const;
    std::string GetMuonVersion() const;
    double GetMuonPtThreshold() const;

    std::string GetJetSelector() const;
    std::string GetJetType() const;
    std::string GetJetVersion() const;
    double GetJetPtThreshold() const;

    inline std::map<std::string,muon::SelectionType> GetMuonSelectionTypeMap() const {return mSelectionType;}
    
    bool SelectTrack(const int iTrack, const std::string selectionType = "");
    bool SelectMuon(const int iMuon, const std::string selectionType = "");
    bool SelectMuonPassingSelector(const int iMuon, const muon::SelectionType selector);
    bool SelectMuonPassingSelector(const int iMuon, const std::string selectorName);
    bool SelectJet(const int iJet, const std::string selectionType = "");
    bool SelectVertex(const int iVertex, const std::string selectionType = "");
    int SelectVertexNearestToTrack(const int iTrack, const std::string method = "");

    
    
#if NTUPLE_VERSION >= 4
    bool SelectMuonPassingTrigger(const int iMuon, const std::string triggerName);
    // Official tight muon definition - refers to a vertex because also the muon impact parameter is checked
    inline bool SelectTightMuon(const int iMuon, const int iVertex);
#endif
    
#if NTUPLE_VERSION >= 3
    bool SelectEventPassingTrigger(const std::string triggerName);
#ifdef USING_MC
    double GetEventPdfWeight(const std::string pdfSetName, const unsigned int index);
#endif // USING_MC
#endif // NTUPLE_VERSION >= 3
    
    bool SelectEventWithMuons(const std::string selectionType,
                              const unsigned int nSelected, const bool exactNumber, std::vector<unsigned short>& indexes);
    bool SelectEventWithMuons(const unsigned int nSelected, const bool exactNumber, std::vector<unsigned short>& indexes);
    bool SelectEventWithMuonsInJets(const std::string muonSelectionType, const std::string jetSelectionType,
                                    const unsigned int nToSelect, const bool exactNumber, std::vector<unsigned short>& indexes);
    bool SelectEventWithMuonsInJets(const unsigned int nToSelect, const bool exactNumber, std::vector<unsigned short>& indexes);
    
    bool SelectEventWithDiMuonV0(const std::string selectionType,
                                 std::vector<unsigned short>& indexes, unsigned short& iVertex);
    bool SelectEventWithDiMuonV0(std::vector<unsigned short>& indexes, unsigned short& iVertex);
    bool SelectEventWithDiMuonInJetsV0(const std::string muonSelectionType, const std::string jetSelectionType,
                                       std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex,
                                       std::vector<unsigned short>& vSelectedJets);
    bool SelectEventWithDiMuonInJetsV0(std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex,
                                       std::vector<unsigned short>& vSelectedJets);


    bool SelectEventWithDiMuonResonanceV0(const std::string selectionType, std::vector<unsigned short>& indexes, unsigned short& iVertex);
    bool SelectEventWithDiMuonResonanceV0(std::vector<unsigned short>& indexes, unsigned short& iVertex);
    bool SelectEventWithDiMuonResonanceInJetsV0(const std::string muonSelectionType, const std::string jetSelectionType,
                                       std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex,
                                       std::vector<unsigned short>& vSelectedJets);
    bool SelectEventWithDiMuonResonanceInJetsV0(std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex,
                                       std::vector<unsigned short>& vSelectedJets);
    
    // Does not reject events with >2 muons in a good vertex. Returns the indexes to the two highest-pT ones instead
    bool SelectEventWithDiMuonV1(const std::string selectionType, std::vector<unsigned short>& indexes, unsigned short& iVertex);

    bool SelectTrack(const unsigned int iTrack, const std::string selectionType);
    bool SelectEvent(const std::string selectionType);
    bool SelectEventAndReturnIndexes(const std::string selectionType, std::vector<unsigned int>& indexes);
//     bool SelectEventWithTracks(const std::string selectionType, const unsigned int nSelected, const bool exactNumber, std::vector<unsigned short>& indexes);


#ifdef USING_MC
    bool SelectGenp(const unsigned int iGenp, const int particleId, const bool absPid = false, const bool stableOnly = true);
#endif // USING_MC

  protected:
    void ParseInputString( const std::string inputString_ );
    bool ParseMuonString(const std::string muonString, std::string& muonVersionString, double& muonPtThreshold);
    bool ParseJetString(const std::string jetString, std::string& jetType, std::string& jetVersion, double& jetPt);

    // Selectors not in public interface. Used only by the public methods.
    bool SelectEventWithMuonsInJets(const std::string muonSelectionType, const std::string jetSelectionType,
                                    const std::string jetType, const unsigned int nToSelect, const bool exactNumber,
                                    std::vector<unsigned short>& indexes);
    bool SelectEventWithDiMuonV0(const std::string selectionType, const std::string selectionVersion,
                                 std::vector<unsigned short>& indexes, unsigned short& iVertex);
    bool SelectEventWithDiMuonInJetsV0(const std::string muonSelectionType, const std::string muonSelectionVersion,
                                       const std::string jetSelectionType, const std::string jetType,
                                       const std::string jetSelectionVersion, std::vector<unsigned short>& vSelectedMuons,
                                       unsigned short& iSelectedVertex, std::vector<unsigned short>& vSelectedJets);
    bool SelectEventWithDiMuonResonanceInJetsV0(const std::string muonSelectionType,
                                                const std::string jetSelectionType, const std::string jetType,
                                                const std::string jetSelectionVersion, std::vector<unsigned short>& vSelectedMuons,
                                                unsigned short& iSelectedVertex, std::vector<unsigned short>& vSelectedJets);
    double FindMuonMVAValueFromEfficiency(const std::string mvaFileName, const std::string method, const double efficiency);
    void InitializeMuonTMVA(const std::string muonSelection, TMVA::Reader*& reader, float& cut);
    
    TREEANALYZER* t;

    std::map<std::string,muon::SelectionType> mSelectionType;
    
    std::string selectionString;
    
    std::string muSelectionString;
    std::string muVersionString;
    double muPtThreshold;
    bool muStringOk;
    
    std::string jetSelectionString;
    std::string jetTypeString;
    std::string jetVersionString;
    double jetPtThreshold;
    bool jetStringOk;
    
    // TMVA part
    // Reader
    TMVA::Reader* muonTMVAReader;
    float muonTMVACut;
    // Variables used to classify muons
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
    float muonTMVAResult;
};

#endif // PARTICLESELECTOR_h



