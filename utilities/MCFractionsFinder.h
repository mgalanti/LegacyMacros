#ifndef MCFractionsFinder_h
#define MCFractionsFinder_h



#ifndef TokenizeString_dec
#define TokenizeString_dec
void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters = ";");
#endif



class MCFractionsFinder
{
  public:
    MCFractionsFinder();
    std::map<std::string,std::pair<double,double> > FindMCFractionsFor1DTemplates();
    std::map<std::string,std::pair<double,double> > FindMCFractionsFor2DTemplates();
    inline void SetInputFileName(const std::string name) {inputFileName = name;};
    inline std::string GetInputFileName() const {return inputFileName;};
    inline void SetVariableNameIP(const std::string name) {variableNameIP = name;};
    inline std::string GetVariableNameIP() const {return variableNameIP;};
    inline void SetMaxIP(const double value) {maxIP = value;};
    inline double GetMaxIP() const {return maxIP;};
    inline void SetVariableNamePtRel(const std::string name) {variableNamePtRel = name;};
    inline std::string GetVariableNamePtRel() const {return variableNamePtRel;};
    inline void SetMaxPtRel(const double value) {maxPtRel = value;};
    inline double GetMaxPtRel() const {return maxPtRel;};
    inline void SetClassificationName(const std::string name) {classificationName = name;};
    inline std::string GetClassificationName() const {return classificationName;};
    inline void SetSign(const std::string name) {sign = name;};
    inline std::string GetSign() const {return sign;};
    inline void SetTranslationMap(const std::map<std::string,std::string> inputMap) {translationMap = inputMap;};
    inline std::map<std::string,std::string> GetTranslationMap() const {return translationMap;};
    inline void SetTemplateNames(const std::string mergedNames) {TokenizeString(mergedNames, templateNames);};
    inline void SetTemplateNames(const std::vector<std::string> names) {templateNames = names;};
    inline std::vector<std::string> GetTemplateNames() const {return templateNames;};
    inline void SetRemovedBinsFractions(const std::map<std::string,std::pair<double,double> > fractionsMap) {removedBinsFractions = fractionsMap;};
    inline std::map<std::string,std::pair<double,double> > GetRemovedBinsFractions() const {return removedBinsFractions;};
    
    
  private:
    int FindXBin(const TH2D* histo);
    int FindYBin(const TH2D* histo);
    std::string inputFileName;
    TFile* inputFile;
    std::string variableNameIP;
    std::string variableNamePtRel;
    std::string classificationName;
    std::string sign;
    std::map<std::string, std::string> translationMap;
    std::vector<std::string> templateNames;
    std::vector<std::string> inputHistoNames;
    double maxIP;
    double maxPtRel;
    std::map<std::string,std::pair<double,double> > removedBinsFractions;
};



#ifndef TokenizeString_def
#define TokenizeString_def
void TokenizeString(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters)
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);
  
  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}
#endif // TokenizeString_def



#endif // MCFractionsFinder_h
