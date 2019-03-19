#ifndef ChiAnalysisBase_h
#define ChiAnalysisBase_h



#include <map>
#include <string>
#include <vector>

#include "../BinnedLogLikelihood/BinnedLogLikelihoodCalculator1D.cc"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodCalculator1DConstrained.cc"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodCalculator1DConstrainedSum.cc"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodCalculator2D.cc"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodCalculator2DConstrained.cc"
#include "../BinnedLogLikelihood/BinnedLogLikelihoodCalculator2DConstrainedSum.cc"
#include "../utilities/MCFractionsFinder.C"
#include "../utilities/MuonDistributionsBuilder.C"
#include "../utilities/MuonHistogramsMerger.C"
#include "../utilities/MuonTemplatesBuilder.C"
#include "../MuonDistributionsProducer/MuonDistributionsProducer.C"
#include "../MuonTemplatesProducer/MuonTemplatesProducer.C"



// Class for error in string-to-double conversion
// No need to define it here because it is taken from MuonTemplatesProducer
// class BadConversion : public std::runtime_error {
// public:
//   BadConversion(std::string const& str)
//   : std::runtime_error(str)
//   { }
// };



// Global variable setting the output file for the entire program
// USed only if RedirectOutputToFile is set to 1(=true)
FILE* outputLogFile = 0;



class ChiAnalysisBase
{
  public:
    ChiAnalysisBase(const std::string className_, const std::string propertiesFileName_);
    
    virtual void RunProducersStep();
    virtual void RunBuildersStep() = 0;
    
  protected:
    std::string className;
    
    void AssignProperties();
    void AssignDerivedProductionProperties();
    void AssignDerivedIPBuildingProperties();
    void AssignDerivedPtRelBuildingProperties();
    void AssignDerivedIPFitProperties();
    void AssignDerivedPtRelFitProperties();
    void CreateWorkingDirectory();
    void RedirectOutputToFile();
    void DumpJobConfig();
    virtual void FindMCFractionsIP() {};
    virtual void FindMCFractionsPtRel() {};
    std::map<std::string,std::string> SetHistoNamesTranslationMap(const std::string classification);
    
    void GetProperty(const std::string propertyName, std::string& output);
    void GetProperty(const std::string propertyName, bool& output);
    void GetProperty(const std::string propertyName, int& output);
    void GetProperty(const std::string propertyName, double& output);
  
    void ReadProperties(const std::string propsFileName, std::map<std::string,std::string>& props);
    
    double ConvertToDouble(const std::string& str);
    void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ";");
    
    std::string propertiesFileName;
    std::map<std::string,std::string> properties;
    
    std::string unixEpoch;
    
    std::string baseDir;
    std::string workingDir;
    
    //// Job properties
    
    // Meta configuration
    bool createWorkingDirectory;
    bool redirectOutputToFile;
    
    // Samples
    std::string dataSample;
    std::string mcSample;
    
    // Selector
    std::string selector;
    
    // Job configuration
    bool runProducers;
    bool runBuilders;
    bool runFit;
    
    // Template and data distribution production properties
    int maxEvents;
    int useFakeIP;
    int useFakePtRel;
    // Derived properties (not in configuration file)
    std::string rawTemplatesFileName;
    std::string rawDataDistributionsFileName;
    
    // IP template and data distribution building properties
    bool use2DDistributionsIP;
    bool useDefaultInputHistoNameIPDistribution;
    std::string inputHistoNameIPDistribution; // If the previous flag is false, this is automatically set (the value in the cfg is not used)
    std::string variableToUseIP;
    std::string templateClassificationIP;
    std::string templateSignIP;
//     std::string inputHistoNumbersForIPTemplates;
//     std::string outputHistoNamesForIPTemplates;
//     std::string inputHistoNumbersForStep1FakeIPDistributions;
//     std::string outputHistoNamesForStep1FakeIPDistributions;
    std::string inputHistoNamesForFakeIPDistributions;
    std::string nEventsPerEachFakeIPComponent;
    bool fitTemplatesIP;
    std::string smearedIPTemplatesString;
    bool usePromptFromDataIP;
    std::string inputFileNamePromptIP;
    std::string inputHistoNamePromptIP;
    // Derived properties (not in configuration file)
    std::vector<std::string> inputHistoNumbersIP;
    std::vector<std::string> inputHistoNamesIP;
    std::vector<std::string> inputHistoNumbersStep1FakeIP;
    std::vector<std::string> inputHistoNamesStep1FakeIP;
    std::vector<std::string> outputHistoNamesIP;
    std::vector<std::string> outputHistoNamesStep1FakeIP;
    std::vector<std::string> inputHistoNamesFakeIP;
    std::vector<double> nEventsFakeIPComponent;
    std::map<std::string, std::string> histoNamesIPTranslationMap;
    std::string outputTemplateFileNameIP;
    std::string outputDistributionsFileNameIP;
    
    // PtRel template and data distribution building properties
    bool use2DDistributionsPtRel;
    bool useDefaultInputHistoNamePtRelDistribution;
    std::string inputHistoNamePtRelDistribution; // If the previous flag is false, this is automatically set (the value in the cfg is not used)
    std::string variableToUsePtRel;
    std::string templateClassificationPtRel;
    std::string templateSignPtRel;
    std::string inputHistoNamesForFakePtRelDistributions;
    std::string nEventsPerEachFakePtRelComponent;
    bool fitTemplatesPtRel;
    std::string smearedPtRelTemplatesString;
    bool useDFromDataPtRel;
    bool combineDAndCPtRel;
    std::string inputFileNameLightPtRel;
    std::string inputHistoNameDPtRel;
    // Derived properties (not in configuration file)
    std::vector<std::string> inputHistoNumbersPtRel;
    std::vector<std::string> inputHistoNamesPtRel;
    std::vector<std::string> inputHistoNumbersStep1FakePtRel;
    std::vector<std::string> inputHistoNamesStep1FakePtRel;
    std::vector<std::string> outputHistoNamesPtRel;
    std::vector<std::string> outputHistoNamesStep1FakePtRel;
    std::vector<std::string> inputHistoNamesFakePtRel;
    std::vector<double> nEventsFakePtRelComponent;
    std::map<std::string, std::string> histoNamesPtRelTranslationMap;
    std::string outputTemplateFileNamePtRel;
    std::string outputDistributionsFileNamePtRel;
    std::vector<std::string> histoNamesToCopyForPtRelTemplates;
    std::string histoNumberFromMCToMergePtRel;
    std::string histoNameFromMCToMergePtRel;
    std::string histoNameMergedLightPtRel;
    std::string outputFileNameMergedRawPtRel;
    
    // IP fit properties
    double fitMinIP;
    double fitMaxIP;
    int rebinFactorIP;
    int removeBinIP;
    int constrainIP;
    std::string templatesToUseFor1DIPFit;
    std::string templatesToUseFor2DIPFit;
    std::string constrainToComponentIP;
    std::string constrainSumToComponentIP;
    // Derived properties (not in configuration file)
    std::vector<std::string> templates1DIP;
    std::vector<std::string> templates2DIP;
    std::string constraintValuesIP;
    std::string constraintErrorsIP;
    std::string constraintSumValuesIP;
    std::string constraintSumErrorsIP;
    
    // PtRel fit properties
    double fitMinPtRel;
    double fitMaxPtRel;
    int rebinFactorPtRel;
    int removeBinPtRel;
    int constrainPtRel;
    std::string templatesToUseFor1DPtRelFit;
    std::string templatesToUseFor2DPtRelFit;
    std::string constrainToComponentPtRel;
    std::string constrainSumToComponentPtRel;
    std::string fitResult1DFileNameIP; // Put here because it is an input for the PtRel fit
    std::string fitResult2DFileNameIP; // Put here because it is an input for the PtRel fit
    // Derived properties (not in configuration file)
    std::vector<std::string> templates1DPtRel;
    std::vector<std::string> templates2DPtRel;
    std::string constraintValuesPtRel;
    std::string constraintErrorsPtRel;
    std::string constraintSumValuesPtRel;
    std::string constraintSumErrorsPtRel;
    
    //Additional properties (not set during class initialization)
    std::map<std::string,std::pair<double,double> > removedBinsFractionsIP;
    std::map<std::string,std::pair<double,double> > mcFractionsIP;

    std::map<std::string,std::pair<double,double> > removedBinsFractionsPtRel;
    std::map<std::string,std::pair<double,double> > mcFractionsPtRel;
    
//     //// Helper classes
    MCFractionsFinder mcFractionsFinder;
};



#endif // ChiAnalysisBase_h
