#ifndef MCTools_h
#define MCTools_h

#include "TreeDefinitionGenV2.h"



class MCTools : public virtual TreeDefinitionGenV2
{
  public:
    virtual ~MCTools() {};
    bool LookForDaughter(const unsigned short iGenp, const unsigned short iDau);
    bool LookForMother(const unsigned short iGenp, const unsigned short iMom);
    bool RecursiveLookForDaughter(const unsigned short iGenp, const unsigned short iDau);
    bool RecursiveLookForMother(const unsigned short iGenp, const unsigned short iMom);
    unsigned short LookForMotherId(const unsigned short iGenp, const int idMom, const bool withSign = true);
    unsigned short LookForDaughterId(const unsigned short iGenp, const int idDau, const bool withSign = true);
    int RecursiveLookForMotherId(const unsigned short iGenp, const int idMom, const bool withSign = true);
    int RecursiveLookForDaughterId(const unsigned short iGenp, const int idDau, const bool withSign = true);
    unsigned short LookForMotherIdInList(const unsigned short iGenp, const int idMom, const std::vector<unsigned short> vIncludedIMom, const bool withSign = true);
    unsigned short LookForDaughterIdInList(const unsigned short iGenp, const int idDau, const std::vector<unsigned short> vIncludedIDau, const bool withSign = true);
    unsigned short LookForMotherIdNotInList(const unsigned short iGenp, const int idMom, const std::vector<unsigned short> vExcludedIMom, const bool withSign = true);
    unsigned short LookForDaughterIdNotInList(const unsigned short iGenp, const int idDau, const std::vector<unsigned short> vExcludedIDau, const bool withSign = true);
    void RecursivePrintMothers(const unsigned short iGenp);
    void RecursivePrintDaughters(const unsigned short iGenp);
    std::vector<unsigned short> RecursiveFindDaughters(const unsigned short iGenp);
//     std::vector<unsigned int> RecursiveFindMothers(const unsigned short iGenp);
    std::pair<unsigned short, unsigned short> LookForMotherString(const unsigned short iGenp);
    std::vector<unsigned short> RecursiveFindMuonDaughters(const unsigned short iGenp);
    
  private:

};

#endif // MCTools_h
