#if !defined(ParticleSelectorData_cxx) || !defined(ParticleSelectorMC_cxx)
#if PARTICLESELECTOR == ParticleSelectorData
#define ParticleSelectorData_cxx
#elif PARTICLESELECTOR == ParticleSelectorMC
#define ParticleSelectorMC_cxx
#endif

// This is included in order to use the system sleep() function below
#include <unistd.h>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>

#include <TLorentzVector.h>

#include "constants.h"
#include "ParticleSelector.h"

#if NTUPLE_VERSION == 1
#include "TreeDefinitionRecoV1.h"
#elif  NTUPLE_VERSION >= 2
#include "TreeDefinitionRecoV2.h"
#endif // NTUPLE_VERSION

#ifdef USING_MC
#if NTUPLE_VERSION == 1
#include "TreeDefinitionGenV1.h"
#elif  NTUPLE_VERSION == 2
#include "TreeDefinitionGenV2.h"
#endif // NTUPLE_VERSION
#endif // USING_MC



// #define mgDEBUG

// Constructors
// New approach to selection string
PARTICLESELECTOR::PARTICLESELECTOR(TREEANALYZER* t_, const std::string selectionString_)
{
  // Initialize map of standard CMSSW muon selectors
  mSelectionType["All"] = muon::All;
  mSelectionType["AllGlobalMuons"] = muon::AllGlobalMuons;
  mSelectionType["AllStandAloneMuons"] = muon::AllStandAloneMuons;
  mSelectionType["AllTrackerMuons"] = muon::AllTrackerMuons;
  mSelectionType["TrackerMuonArbitrated"] = muon::TrackerMuonArbitrated;
  mSelectionType["AllArbitrated"] = muon::AllArbitrated;
  mSelectionType["GlobalMuonPromptTight"] = muon::GlobalMuonPromptTight;
  mSelectionType["TMLastStationLoose"] = muon::TMLastStationLoose;
  mSelectionType["TMLastStationTight"] = muon::TMLastStationTight;
  mSelectionType["TM2DCompatibilityLoose"] = muon::TM2DCompatibilityLoose;
  mSelectionType["TM2DCompatibilityTight"] = muon::TM2DCompatibilityTight;
  mSelectionType["TMOneStationLoose"] = muon::TMOneStationLoose;
  mSelectionType["TMOneStationTight"] = muon::TMOneStationTight;
  mSelectionType["TMLastStationOptimizedLowPtLoose"] = muon::TMLastStationOptimizedLowPtLoose;
  mSelectionType["TMLastStationOptimizedLowPtTight"] = muon::TMLastStationOptimizedLowPtTight;
  mSelectionType["GMTkChiCompatibility"] = muon::GMTkChiCompatibility;
  mSelectionType["GMStaChiCompatibility"] = muon::GMStaChiCompatibility;
  mSelectionType["GMTkKinkTight"] = muon::GMTkKinkTight;
  mSelectionType["TMLastStationAngLoose"] = muon::TMLastStationAngLoose;
  mSelectionType["TMLastStationAngTight"] = muon::TMLastStationAngTight;
  mSelectionType["TMOneStationAngLoose"] = muon::TMOneStationAngLoose;
  mSelectionType["TMOneStationAngTight"] = muon::TMOneStationAngTight;
  mSelectionType["TMLastStationOptimizedBarrelLowPtLoose"] = muon::TMLastStationOptimizedBarrelLowPtLoose;
  mSelectionType["TMLastStationOptimizedBarrelLowPtTight"] = muon::TMLastStationOptimizedBarrelLowPtTight;
  
  // Initialize selectors  
  t=t_;
  selectionString = selectionString_;
  muSelectionString = "";
  jetSelectionString = "";

  muVersionString = ""; 
  muPtThreshold = -999.9;
  jetTypeString = ""; 
  jetPtThreshold = -999.9;
  
  muStringOk = false;
  jetStringOk = false;
  
  std::cout << "Parsing input string to extract selection parameters...\n";

  ParseInputString(selectionString);

  std::cout << "Selection strings:";
  std::cout << "\n\t input\t" << selectionString;
  std::cout << "\n\t muon\t" << muSelectionString << "\t" << (MuonSelectorIsValid()?"(valid)":"\t(INVALID!)");
  std::cout << "\n\t jet\t" << jetSelectionString << "\t" << (JetSelectorIsValid()?"(valid)":"\t(INVALID!)") << std::endl;

  std::cout << "Muon selection parameters:";
  std::cout << "\n\t V\t" << muVersionString;
  std::cout << "\n\t Pt\t" << muPtThreshold << std::endl;
  
  std::cout << "Jet selection parameters:";
  std::cout << "\n\t Type\t" << jetTypeString;
  std::cout << "\n\t V\t" << jetVersionString;
  std::cout << "\n\t Pt\t" << jetPtThreshold << "\n\n";
  
  // Initialize TMVA for muon selection
  InitializeMuonTMVA(muVersionString, muonTMVAReader, muonTMVACut);
}


// Backward compatibility
PARTICLESELECTOR::PARTICLESELECTOR(TREEANALYZER* t_)
{
  t=t_;
  selectionString = "";
  muSelectionString = "";
  jetSelectionString = "";
  muVersionString = ""; 
  muPtThreshold = -999.9;
  jetTypeString = "";
  jetVersionString = "";
  jetPtThreshold = -999.9;
  muStringOk = false;
  jetStringOk = false;
}



// Parse the selection string and separate muon and jets in a more efficient
// way so that the whole string manipulation is done only once
void PARTICLESELECTOR::ParseInputString( const std::string inputString_ )
{
  // Standard format for the selection string is now
  // muSelectionString_jetSelectionString

  muStringOk = false;
  jetStringOk = false;

  // Check if the string is empty or not
  if (inputString_.compare("") == 0)
  {
    // Do not do anything
    // The selection methods will go with a default selection
    // which is already implemented in the constructor
    std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : input string is empty.\n";
//     std::cout << "               Use default selection with " << muSelectionString << " and " << jetSelectionString << ".\n";
    muStringOk = false;
    jetStringOk = false;
  }
  else
  {
    // First step is to identify the underscore to
    // split (if possible) the string into two
    if (inputString_.find("_") != std::string::npos)
    {
      // ONLY ONE underscore for double strings
      if (inputString_.find_first_of("_") == inputString_.find_last_of("_") )
      {
        // Check the string can be a muSelectionString_jetSelectionString
        if ((inputString_.find("muon") != std::string::npos && 
             inputString_.find("muon") < inputString_.find("_")) ||
            ((inputString_.find("jet") != std::string::npos) &&
             inputString_.find("_") == inputString_.find("jet") - 1))
        {
          // We can divide the string into two
          // Search for "muon" and for the underscore to get the muon substring
          // Analogous procedure for the jet substring
          muSelectionString = inputString_.substr(0, inputString_.find("_"));
          if(inputString_.find("muon") != std::string::npos && 
             inputString_.find("muon") < inputString_.find("_"))
          {
            muStringOk = true;
          }
          jetSelectionString = inputString_.substr(inputString_.find("_") + 1, inputString_.size());
          if((inputString_.find("jet") != std::string::npos) &&
             inputString_.find("_") == inputString_.find("jet") - 1)
          {
            jetStringOk = true;
          }
        }
        else
        {
          // The string can be divided into two parts, but it does not cope
          // with the standard requirements, so print a warning and put
          // to zero the muon and jet substrings to make
          // the code crash somewhere else in the future
          std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : badly formed input string (mu_jet).\n";
          std::cout << "               Muon and jet substrings are forced to be empty.\n";
          muStringOk = false;
          jetStringOk = false;
        } // If the string is a good input / else
      }
      else
      {
        // We have a multiple underscore string
        // This is bad formed, so print a warning and put
        // to zero the muon and jet substrings to make
        // the code crash somewhere else in the future
        std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : badly formed input string (multiple _).\n";
        std::cout << "               Muon and jet substrings are forced to be empty.\n";
        muStringOk = false;
        jetStringOk = false;
      } // If there is ONLY ONE underscore / else
    }
    else
    {
      // The string cannot be divided into two with a single underscore
      // If at least one of the two substrings can be found, let the
      // the other one to keep being empty
      muStringOk = false;
      jetStringOk = false;
      if (inputString_.find("muon") != std::string::npos && inputString_.find("jet") == std::string::npos)
      {
        muSelectionString = inputString_.substr(inputString_.find("muon"), inputString_.size());
        muStringOk = true;
      }
      if (inputString_.find("muon") == std::string::npos && inputString_.find("jet") != std::string::npos)
      {
        jetSelectionString = inputString_.substr(inputString_.find("jet"), inputString_.size());
        jetStringOk = true;
      }

      if(!muStringOk && !jetStringOk)
      {
        std::cout << "W A R N I N G! Selection string could not be split into two parts (three with HLT)!\n";
        std::cout << "               (A single underscore must be used to separate the muon and jet substrings.)\n";
        std::cout << "               Keep default value.\n";
      }
    } // If inputString_ can be split into two
  } // If inputString_ is empty / else

  // Last steps:
  // Extract any Threshold in the muon and jet string
  // and store them into doubles
  // do that only for non-empty Strings
  if(muStringOk)
  {
    muStringOk = ParseMuonString(muSelectionString, muVersionString, muPtThreshold);
  }

//   if(muStringOk)
//   {
//     // Standard muon string is muon-VVersion-PtThreshold
//     // Since Version is not always a number, we use a String to store it
//     if (muSelectionString.find("muonV") == 0)
//     {
//       if (muSelectionString.find("Pt") != std::string::npos)
//       {
//         muVersionString = muSelectionString.substr(5, muSelectionString.find("Pt")-5);
//         if (muVersionString.compare("") == 0)
//         {
//           std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString() : empty muon selection Version!\n";
//           std::cout << "               Keep default value.\n";
//           muStringOk = false;
//         }
//       }
//       else
//       {
//         std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Muon substring with \"muonV\" but without \"Pt\".\n";
//         std::cout << "               Keep the default value.\n";
//         muStringOk = false;
//       }
//     }
//     else
//     {
//       std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Muon substring without \"muonV\".\n";
//       std::cout << "               Keep the default value.\n";
//       muStringOk = false;
//     }
// 
//     // Pt threshold is a double defined as 3p5 instead of 3.5
//     if (muSelectionString.find("Pt") != std::string::npos)
//     {
//       std::string tempString = muSelectionString.substr(muSelectionString.find("Pt")+2, muSelectionString.size()-muSelectionString.find("Pt")+2);
//       if (tempString.find("p") != std::string::npos)
//         tempString.replace(tempString.find("p"),1,".");
// 
//       std::istringstream iss(tempString);
//       iss >> std::dec >> muPtThreshold;
//       if (iss.rdbuf()->in_avail() == 0)
//       {
//         if (muPtThreshold < 0) 
//         {
//           std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString() : negative Pt cut!\n";
//           std::cout << "               Keep default value.\n";
//           muStringOk = false;
//         }
//         else
//         {
//           std::cout << "MUON THRESHOLD IS " << muPtThreshold << std::endl;
//         }
//       }
//       else
//       {
//         std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Muon substring " << iss.str() << " is badly encoded Pt.\n";
//         std::cout << "               Force to the default value.\n";
//         muStringOk = false;
//       }
//     }
//     else
//     {
//       std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Muon substring without \"Pt\".\n";
//       std::cout << "               Keep the default value.\n";
//       muStringOk = false;
//     }
//   }

  // Standard jet string is jet-PF/Tk-PtThreshold
  if(jetStringOk)
  {
    jetStringOk = ParseJetString(jetSelectionString, jetTypeString, jetVersionString, jetPtThreshold);
  }
  
//   if(jetStringOk && jetSelectionString.find("jet") == 0)
//   {
//     jetTypeString = jetSelectionString.substr(3, jetSelectionString.find("V")-3);
//     if(jetTypeString != "PF" ||
//        jetTypeString != "Tk" ||
//        jetTypeString != "Calo" ||
//        jetTypeString != "Pat" ||
//        jetTypeString != "JPT")
//     {
//       std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Jet substring does not define a valid jet type.\n";
//       std::cout << "               Keep the default value.\n";
//       jetStringOk = false;
//     }
//     if (jetStringOk && jetSelectionString.find("Pt") == std::string::npos)
//     {
//       std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Jet substring with \"jet\" but without \"Pt\".\n";
//       std::cout << "               Keep the default value.\n";
//       jetStringOk = false
//     }
//     if(jetStringOk && jetSelectionString.substr(3 + jetTypeString.size(),1) == "V")
//     {
//       jetVersionString = jetSelectionString.substr(3 + jetTypeString.size() + 1, jetSelectionString.find("Pt") - 3 - jetTypeString.size() - 1);
//     }
//     else
//     {
//       std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Jet substring without valid version.\n";
//       std::cout << "               Keep the default value.\n";
//       jetStringOk = false;
//     }
//   }
//   else
//   {
//     std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Jet substring without \"jet\".\n";
//     std::cout << "               Keep the default value.\n";
//     jetStringOk = false;
//   }
//   
//   // Pt threshold is a double defined as 3p5 instead of 3.5
//   if (jetStringOk)
//   {
//     std::string tempString = jetSelectionString.substr(jetSelectionString.find("Pt")+2, jetSelectionString.size()-jetSelectionString.find("Pt")+2);
//     if (tempString.find("p") != std::string::npos)
//     {
//       tempString.replace(tempString.find("p"),1,".");
//     }
//     std::istringstream iss(tempString);
//     iss >> std::dec >> jetPtThreshold;
//     if (iss.rdbuf()->in_avail() == 0)
//     {
//       std::cout << "JET THRESHOLD IS " << jetPtThreshold << std::endl;
//     }
//     else
//     {
//       std::cout << "W A R N I N G! PARTICLESELECTOR::ParseInputString(...) : Jet substring " << iss.str() << " is badly encoded Pt.\n";
//       std::cout << "               Force to the default value.\n";
//       jetStringOk = false;
//     }
//   }
  
  // Reset values to default for any that is not Ok
  if(!muStringOk)
  {
//     muSelectionString = "";
    muVersionString = ""; 
    muPtThreshold = -999.9;
  }
  if(!jetStringOk)
  {
//     jetSelectionString = "";
    jetTypeString = "";
    jetVersionString = "";
    jetPtThreshold = -999.9;
  }
}



bool PARTICLESELECTOR::ParseMuonString(const std::string muonString, std::string& muonVersionString, double& muonPtThreshold)
{
  // Standard muon string is muon-VVersion-PtThreshold
  // Since Version is not always a number, we use a String to store it
  if (muonString.find("muonV") == 0)
  {
    if (muonString.find("Pt") != std::string::npos)
    {
      muonVersionString = muonString.substr(5, muonString.find("Pt")-5);
      if (muonVersionString.compare("") == 0)
      {
        std::cout << "W A R N I N G! PARTICLESELECTOR::ParseMuonString() : empty muon selection Version!\n";
        std::cout << "               Keep default value.\n";
        return false;
      }
    }
    else
    {
      std::cout << "W A R N I N G! PARTICLESELECTOR::ParseMuonString(...) : Muon substring with \"muonV\" but without \"Pt\".\n";
      std::cout << "               Keep the default value.\n";
      return false;
    }
  }
  else
  {
    std::cout << "W A R N I N G! PARTICLESELECTOR::ParseMuonString(...) : Muon substring without \"muonV\".\n";
    std::cout << "               Keep the default value.\n";
    return false;
  }
  
  // Pt threshold is a double defined as 3p5 instead of 3.5
  if (muonString.find("Pt") != std::string::npos)
  {
    std::string tempString = muonString.substr(muonString.find("Pt")+2, muonString.size()-muonString.find("Pt")+2);
    if (tempString.find("p") != std::string::npos)
      tempString.replace(tempString.find("p"),1,".");
    
    std::istringstream iss(tempString);
    iss >> std::dec >> muonPtThreshold;
    if (iss.rdbuf()->in_avail() == 0)
    {
      if (muonPtThreshold < 0) 
      {
        std::cout << "W A R N I N G! PARTICLESELECTOR::ParseMuonString() : negative Pt cut!\n";
        std::cout << "               Keep default value.\n";
        return false;
      }
#ifdef mgDEBUG
      else
      {
        std::cout << "MUON THRESHOLD IS " << muPtThreshold << std::endl;
      }
#endif // mgDEBUG
    }
    else
    {
      std::cout << "W A R N I N G! PARTICLESELECTOR::ParseMuonString(...) : Muon substring " << iss.str() << " is badly encoded Pt.\n";
      std::cout << "               Force to the default value.\n";
      return false;
    }
  }
  else
  {
    std::cout << "W A R N I N G! PARTICLESELECTOR::ParseMuonString(...) : Muon substring without \"Pt\".\n";
    std::cout << "               Keep the default value.\n";
    return false;
  }
  return true;
}



bool PARTICLESELECTOR::ParseJetString(const std::string jetString, std::string& jetType, std::string& jetVersion, double& jetPt)
{
  if(jetString.find("jet") == 0)
  {
    jetType = jetString.substr(3, jetString.find("V")-3);
    if(jetType != "PF" &&
       jetType != "Tk" &&
       jetType != "Calo" &&
       jetType != "Pat" &&
       jetType != "JPT")
    {
      std::cout << "W A R N I N G! PARTICLESELECTOR::ParseJetString(...) : Jet substring not defining a valid jet type\n";
      std::cout << "               or missing version string!\n";
      std::cout << "               Valid types are: PF, Tk, Calo, Pat and JPT\n";
      std::cout << "               Keep the default value.\n";
      return false;
    }
    if (jetString.find("Pt") == std::string::npos)
    {
      std::cout << "W A R N I N G! PARTICLESELECTOR::ParseJetString(...) : Jet substring with \"jet\" but without \"Pt\".\n";
      std::cout << "               Keep the default value.\n";
      return false;
    }
    if(jetString.substr(3 + jetType.size(),1) == "V")
    {
      jetVersion = jetString.substr(3 + jetType.size() + 1, jetString.find("Pt") - 3 - jetType.size() - 1);
      if (jetVersion.compare("") == 0)
      {
        std::cout << "W A R N I N G! PARTICLESELECTOR::ParseJetString() : empty jet selection Version!\n";
        std::cout << "               Keep default value.\n";
        return false;
      }
    }
    else
    {
      std::cout << "W A R N I N G! PARTICLESELECTOR::ParseJetString(...) : Jet substring without valid version.\n";
      std::cout << "               Keep the default value.\n";
      return false;
    }
  }
  else
  {
    std::cout << "W A R N I N G! PARTICLESELECTOR::ParseJetString(...) : Jet substring without \"jet\".\n";
    std::cout << "               Keep the default value.\n";
    return false;
  }
  
  // Pt threshold is a double defined as 3p5 instead of 3.5
  std::string tempString = jetString.substr(jetString.find("Pt")+2, jetString.size()-jetString.find("Pt")+2);
  if (tempString.find("p") != std::string::npos)
  {
    tempString.replace(tempString.find("p"),1,".");
  }
  std::istringstream iss(tempString);
  iss >> std::dec >> jetPt;
  if (iss.rdbuf()->in_avail() == 0)
  {
    if (jetPt < 0) 
    {
      std::cout << "W A R N I N G! PARTICLESELECTOR::ParseJetString() : negative Pt cut!\n";
      std::cout << "               Keep default value.\n";
      return false;
    }
#ifdef mgDEBUG
    else
    {
      std::cout << "JET THRESHOLD IS " << jetPtThreshold << std::endl;
    }
#endif // mgDEBUG
  }
  else
  {
    std::cout << "W A R N I N G! PARTICLESELECTOR::ParseJetString(...) : Jet substring " << iss.str() << " is badly encoded Pt.\n";
    std::cout << "               Force to the default value.\n";
    return false;
  }
  return true;
}


/// Return the stored information
std::string PARTICLESELECTOR::GetMuonSelector() const
{
  return muSelectionString;
}
std::string PARTICLESELECTOR::GetMuonVersion() const
{
  if (muStringOk) return muVersionString;
  else return "";
}
double PARTICLESELECTOR::GetMuonPtThreshold() const
{
  if (muStringOk) return muPtThreshold;
  else return -999.9;
}

std::string PARTICLESELECTOR::GetJetSelector() const
{
  return jetSelectionString;
}
std::string PARTICLESELECTOR::GetJetType() const
{
  if (jetStringOk) return jetTypeString;
  else return "";
}
std::string PARTICLESELECTOR::GetJetVersion() const
{
  if (jetStringOk) return jetVersionString;
  else return "";
}
double PARTICLESELECTOR::GetJetPtThreshold() const
{
  if (jetStringOk) return jetPtThreshold;
  else return -999.9;
}






/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
bool PARTICLESELECTOR::SelectTrack(const int iTrack, const std::string selectionType)
{
  // Check if there are the track and the String
  if (iTrack >= t->numberOfTrack)
  {
    std::cout << "W A R N I N G! PARTICLESELECTOR::SelectTrack() : asking for a muon out of range\n";
    std::cout << "               Returning false.\n";
    return false;
  }

  // Check if the string is empty and if the values stored in the
  // dedicated variables (Version and Pt) are correctly encoded
  std::string tempSelectionVersion = "";
  double tempSelectionPt = -999.9;

  if (selectionType.compare("") == 0)
  {
    if(muStringOk)
    {
      // Retrieve the default values from Event selection
      tempSelectionPt = muPtThreshold;
      tempSelectionVersion = muVersionString;
    }
    else
    {
      std::cout << "E R R O R! PARTICLESELECTOR::SelectTrack(...): Asking for the default selector but without a valid string!\n";
      std::cout << "           Exiting...\n";
      exit(1);
    }
  }
  else
  {
    // Default selection is overriden with a specific one
    // Muon selection string is assumed to be as the Event selection muon substring
    // eg: muonV123abcPt23p41 = Version 123abc Pt 23.41

    if(!ParseMuonString(selectionType, tempSelectionVersion, tempSelectionPt))
    {
      std::cout << "E R R O R! PARTICLESELECTOR::SelectTrack(...): Using custom selection but with a malformed string!\n";
      std::cout << "           Selection string is:\"" << selectionType << "\"\n";
      std::cout << "           Exiting...\n";
      exit(1);
    }

  }

  // First, reject according to Pt
  if (t->Track_pt->at(iTrack) <= tempSelectionPt) return false;

  // If here, we are confident that we have correctly encoded and stored information and that
  // candidate muon to be selected has the Pt above threshold, now check quality starting from latest version

  // NOTE: in C++ one cannot easily implement switch/case on strings,
  // therefore we must rely on the "cascade" if/else (ONLY "if" because of the "returns")
  // Nicola is unhappy with that because it is not elegant

  // NOTE: avoid recalling recursively SelectTrack within itself
  if(tempSelectionVersion.compare("highPurity") == 0)
  {
    return(t->Track_isHighPurity->at(iTrack));
  }
  
  if(tempSelectionVersion.compare("7") == 0)
  {
    if(!(fabs(t->Track_eta->at(iTrack)) < 2.1 &&
         t->Track_numberOfValidHits->at(iTrack) > 11 &&
         t->Track_normalizedChi2->at(iTrack) < 2 &&
         t->Track_validPixelHits->at(iTrack) > 1 ) )
      return false;


    // Veto on the first pixel layer
    // Loop on the tracker hit pattern
    bool hasHitInPXB = false;
    bool hasHitInPXF1 = false;
    for(unsigned short iHit = 0; iHit < t->Track_hitPattern->at(iTrack).size(); iHit++)
    {
      unsigned short hit = t->Track_hitPattern->at(iTrack).at(iHit);
      unsigned short hitCopy = hit;
      std::string hitString;
      std::string digits = "01";
      do
      {
        hitString = digits[hitCopy%2] + hitString;
        hitCopy/=2;
      }
      while(hitCopy);
      //       std::cout << "hit " << iHit << " has bitmask: " << hitString << " (value " << hit << ")" << std::endl;
      unsigned short substructure = (hit>>7)&7;
      //       std::cout << "substructure is " << substructure << std::endl;
      unsigned short subsubstructure = (hit>>3)&15;
      //       std::cout << "subsubstructure is " << subsubstructure << std::endl;
      unsigned short type = hit&3;
      //       std::cout << "type is " << type << std::endl;
      if(substructure == 1 && subsubstructure == 1 && type == 0)
        return false;
      if(substructure == 1 && type == 0)
        hasHitInPXB = true;
      if(substructure == 2 && subsubstructure == 1 && type == 0)
        hasHitInPXF1 = true;
    }
    if(!hasHitInPXB && hasHitInPXF1)
      return false;
    std::cout << "Found a muon without a hit in the first pixel layers!\n";
    std::cout << "Pt = " << t->Muon_pt->at(iTrack) << ", eta = " << t->Muon_eta->at(iTrack) << ", phi = " << t->Muon_phi->at(iTrack) << std::endl;

  }

  // muonV2NOTMOST
  // muonV6
  if (tempSelectionVersion.compare("2NOTMOST") == 0 ||
      tempSelectionVersion.compare("6") == 0)
  {
    return (t->Track_numberOfValidHits->at(iTrack) > 11 &&
            t->Track_normalizedChi2->at(iTrack) < 2 &&
            t->Track_validPixelHits->at(iTrack) > 1 &&
            fabs(t->Track_eta->at(iTrack)) < 2.1);
  }

  // muonVSkim
  if (tempSelectionVersion.compare("Skim") == 0)
  {
    return (t->Track_numberOfValidHits->at(iTrack) > 11 &&
            t->Track_normalizedChi2->at(iTrack) < 2 &&
            t->Track_validPixelHits->at(iTrack) > 1 &&
            t->Track_pt->at(iTrack) > 3. &&
            fabs(t->Track_eta->at(iTrack)) < 2.4);
  }

  // Default:
  std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectTrack(...) - selection version \"" << tempSelectionVersion << "\" is not defined!\n";
  std::cout << "                Returning false.\n";
  return false;

}



bool PARTICLESELECTOR::SelectMuon(const int iMuon, const std::string selectionType)
{
  // Check if there are the muon and the String
  if (iMuon >= t->numberOfMuon)
  {
    std::cout << "W A R N I N G! PARTICLESELECTOR::SelectMuon() : asking for a muon out of range\n";
    std::cout << "               Returning false.\n";
    return false;
  }

  // Check if the string is empty and if the values stored in the
  // dedicated variables (Version and Pt) are correctly encoded
  std::string tempSelectionVersion = "";
  double tempSelectionPt = -999.9;
  // Reader
  TMVA::Reader* tempMuonTMVAReader = 0;
  float tempMuonTMVACut = -999.9;

  if (selectionType.compare("") == 0)
  {
    if(muStringOk)
    {
      // Retrieve the default values from Event selection
      tempSelectionPt = muPtThreshold;
      tempSelectionVersion = muVersionString;
      tempMuonTMVAReader = muonTMVAReader;
      tempMuonTMVACut = muonTMVACut;
    }
    else
    {
      std::cout << "E R R O R! PARTICLESELECTOR::SelectMuon(...): Asking for the default selector but without a valid string!\n";
      std::cout << "           Exiting...\n";
      exit(1);
    }
  }
  else
  {
    // Default selection is overriden with a specific one
    // Muon selection string is assumed to be as the Event selection muon substring
    // eg: muonV123abcPt23p41 = Version 123abc Pt 23.41
    
    if(!ParseMuonString(selectionType, tempSelectionVersion, tempSelectionPt))
    {
      std::cout << "E R R O R! PARTICLESELECTOR::SelectMuon(...): Using custom selection but with a malformed string!\n";
      std::cout << "           Selection string is:\"" << selectionType << "\"\n";
      std::cout << "           Exiting...\n";
      exit(1);
    }
//     // Discussed with Mario whether it was better to decode the string herein or
//     // to recall here ParseInputString: decided to recall it herein
//     if (selectionType.find("muonV") == 0)
//     {
//       if (selectionType.find("Pt") != std::string::npos)
//       {
//         tempSelectionVersion = selectionType.substr(5, selectionType.find("Pt")-5);
//       }
//       else
//       {
//         std::cout << "E R R O R! PARTICLESELECTOR::SelectMuon(...) : Selection string with \"muonV\" but without \"Pt\".\n";
//         std::cout << "           Keep the default value.\n";
//       }
//     }
//     else
//     {
//       std::cout << "E R R O R! PARTICLESELECTOR::SelectMuon(...) : Selection string without \"muonV\".\n";
//       std::cout << "           Keep the default value.\n";
//     }
// 
//     // Pt threshold is a double defined as 3p5 instead of 3.5
//     if (selectionType.find("Pt") != std::string::npos)
//     {
//       std::string tempString = selectionType.substr(selectionType.find("Pt")+2, selectionType.size()-selectionType.find("Pt")+2);
//       if (tempString.find("p") != std::string::npos)
//         tempString.replace(tempString.find("p"),1,".");
// 
//       std::istringstream iss(tempString);
//       iss >> std::dec >> tempSelectionPt;
//       if (iss.rdbuf()->in_avail() != 0) 
//       {
//         std::cout << "E R R O R! PARTICLESELECTOR::SelectMuon(...) : Selection substring " << iss.str() << " is badly encoded Pt.\n";
//         std::cout << "           Force to the default value.\n";
//         tempSelectionPt = -999.9;
//       }
//     }
//     else
//     {
//       std::cout << "E R R O R! PARTICLESELECTOR::SelectMuon(...) : Selection substring without \"Pt\".\n";
//       std::cout << "           Keep the default value.\n";
//     }
// 
//     // Cross check that retrieved values are compatible with the interface criteria
//     if (tempSelectionPt < 0) 
//     {
//       std::cout << "E R R O R! PARTICLESELECTOR::SelectMuon() : asking for custom selection but badly formed Pt\n";
//       std::cout << "           Return false.\n";
//       return false;
//     }
//     if (tempSelectionVersion.compare("") == 0)
//     {
//       std::cout << "E R R O R! PARTICLESELECTOR::SelectMuon() : asking for custom selection but empty Version\n";
//       std::cout << "           Return false.\n";
//       return false;
//     }
    
    // If we ask for a custom TMVA selection, then we have to load the weight file here
    // This is AWFULLY slow, but this is the price to pay for doing non-standard things...
    InitializeMuonTMVA(tempSelectionVersion, tempMuonTMVAReader, tempMuonTMVACut);
  }

  // After long debugging, we are confident that here we have correctly encoded and stored information

  // First, reject according to Pt
  if (t->Muon_pt->at(iMuon) <= tempSelectionPt) return false;

  // If here, we are confident that we have correctly encoded and stored information and that
  // candidate muon to be selected has the Pt above threshold, now check quality starting from latest version

  // NOTE: in C++ one cannot easily implement switch/case on strings,
  // therefore we must rely on the "cascade" if/else (ONLY "if" because of the "returns")
  // Nicola is unhappy with that because it is not elegant

  // NOTE: avoid recalling recursively SelectMuon within itself
  if(tempSelectionVersion.compare("4TMVABmm") == 0 || tempSelectionVersion.compare("5TMVABmm") == 0)
  {
    unsigned int trackRef = t->Muon_trackref->at(iMuon);
    if(trackRef == 65535)
    {
      if(tempMuonTMVAReader != muonTMVAReader)
      {
        std::cout << "I N F O : Deleting temporary TMVA reader.\n";
        delete tempMuonTMVAReader;
      }
      return false;
    }
    if(trackRef == 2048)
    {
      std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectMuon() : Selection \"muonV" << tempSelectionVersion << "\" cannot be made\n";
      std::cout << "                because inner track of muon is not present in the ntuple!\n";
      std::cout << "                Returning false...\n";
      if(tempMuonTMVAReader != muonTMVAReader)
      {
        std::cout << "I N F O : Deleting temporary TMVA reader.\n";
        delete tempMuonTMVAReader;
      }
      return false;
    }
    muonTMVATrackValidFraction = t->GetTrackValidFraction(trackRef);
    muonTMVAGlobalChi2 = t->Muon_globalChi2->at(iMuon);
    muonTMVAPt = t->Muon_pt->at(iMuon);
    muonTMVAEta = t->Muon_eta->at(iMuon);
    muonTMVASegmentCompatibility = t->Muon_segmentCompatibility->at(iMuon);
    muonTMVAChi2LocalMomentum = t->Muon_GlobalKink_chi2LocalMomentum->at(iMuon);
    muonTMVAChi2LocalPosition = t->Muon_GlobalKink_chi2LocalPosition->at(iMuon);
    muonTMVAGlobalTrackProbability = t->Muon_glbTrackProbability->at(iMuon);
    muonTMVATrackNumberOfValidHits = t->Track_numberOfValidHits->at(trackRef);
    muonTMVATrackExpectedHitsOuter = t->GetNumberOfLostTrackerOuterHits(trackRef);
    muonTMVAResult = tempMuonTMVAReader->EvaluateMVA("BDT");
    if(tempMuonTMVAReader != muonTMVAReader)
    {
      std::cout << "I N F O : Deleting temporary TMVA reader.\n";
      delete tempMuonTMVAReader;
    }
#ifdef mgDEBUG
    std::cout << "Value of MVA for this muon is " << muonTMVAResult << std::endl;
#endif // mgDEBUG
    return (t->Muon_isTracker->at(iMuon) &&
    t->Muon_isGlobal->at(iMuon) &&
    t->Muon_isGlobalMuonPromptTight->at(iMuon) &&
    t->Muon_numberOfMatchedStations->at(iMuon) > 1 &&
    t->Track_layersWithMeasurement->at(trackRef) > 5 &&
    t->Track_validPixelHits->at(trackRef) > 0 &&
    t->Track_isHighPurity->at(trackRef) &&
    muonTMVAResult > tempMuonTMVACut);
  }    
  
  if(tempSelectionVersion.compare("3TMVABmm") == 0 || tempSelectionVersion.compare("2TMVABmm") == 0)
  {
    unsigned int trackRef = t->Muon_trackref->at(iMuon);
    if(trackRef == 65535)
    {
      if(tempMuonTMVAReader != muonTMVAReader)
      {
        std::cout << "I N F O : Deleting temporary TMVA reader.\n";
        delete tempMuonTMVAReader;
      }
      return false;
    }
    if(trackRef == 2048)
    {
      std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectMuon() : Selection \"muonV" << tempSelectionVersion << "\" cannot be made\n";
      std::cout << "                because inner track of muon is not present in the ntuple!\n";
      std::cout << "                Returning false...\n";
      if(tempMuonTMVAReader != muonTMVAReader)
      {
        std::cout << "I N F O : Deleting temporary TMVA reader.\n";
        delete tempMuonTMVAReader;
      }
      return false;
    }
    muonTMVATrackValidFraction = t->GetTrackValidFraction(trackRef);
    muonTMVAGlobalChi2 = t->Muon_globalChi2->at(iMuon);
    muonTMVAPt = t->Muon_pt->at(iMuon);
    muonTMVAEta = t->Muon_eta->at(iMuon);
    muonTMVASegmentCompatibility = t->Muon_segmentCompatibility->at(iMuon);
    muonTMVAChi2LocalMomentum = t->Muon_GlobalKink_chi2LocalMomentum->at(iMuon);
    muonTMVAChi2LocalPosition = t->Muon_GlobalKink_chi2LocalPosition->at(iMuon);
    muonTMVAGlobalTrackProbability = t->Muon_glbTrackProbability->at(iMuon);
    muonTMVATrackNumberOfValidHits = t->Track_numberOfValidHits->at(trackRef);
    muonTMVATrackExpectedHitsOuter = t->GetNumberOfLostTrackerOuterHits(trackRef);
    muonTMVAResult = tempMuonTMVAReader->EvaluateMVA("BDT");
    if(tempMuonTMVAReader != muonTMVAReader)
    {
      std::cout << "I N F O : Deleting temporary TMVA reader.\n";
      delete tempMuonTMVAReader;
    }
#ifdef mgDEBUG
    std::cout << "Value of MVA for this muon is " << muonTMVAResult << std::endl;
#endif // mgDEBUG
    return (t->Muon_isTracker->at(iMuon) &&
            t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isGlobalMuonPromptTight->at(iMuon) &&
            t->Muon_numberOfMatchedStations->at(iMuon) > 1 &&
            t->Track_layersWithMeasurement->at(trackRef) > 5 &&
            t->Track_validPixelHits->at(trackRef) > 0 &&
            muonTMVAResult > tempMuonTMVACut);
  }    
  if(tempSelectionVersion.compare("1TMVABmm") == 0)
  {
    std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectMuon() : Selection \"muonV1TMVABmm\" is BUGGY and DEPRECATED!\n";
    std::cout << "                You should use \"muonV2TMVABmm\" instead. Continue at your own risk...\n";
    
    unsigned int trackRef = t->Muon_trackref->at(iMuon);
    if(trackRef == 65535)
    {
      if(tempMuonTMVAReader != muonTMVAReader)
      {
        std::cout << "I N F O : Deleting temporary TMVA reader.\n";
        delete tempMuonTMVAReader;
      }
      return false;
    }
    if(trackRef == 2048)
    {
      std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectMuon() : Selection \"muonV1TMVABmm\" cannot be made\n";
      std::cout << "                because inner track of muon is not present in the ntuple!\n";
      std::cout << "                Returning false...\n";
      if(tempMuonTMVAReader != muonTMVAReader)
      {
        std::cout << "I N F O : Deleting temporary TMVA reader.\n";
        delete tempMuonTMVAReader;
      }
      return false;
    }
    muonTMVATrackValidFraction = t->GetTrackValidFraction(trackRef);
    muonTMVAGlobalChi2 = t->Muon_globalChi2->at(iMuon);
    muonTMVAPt = t->Muon_pt->at(iMuon);
    muonTMVAEta = t->Muon_eta->at(iMuon);
    muonTMVASegmentCompatibility = t->Muon_segmentCompatibility->at(iMuon);
    muonTMVAChi2LocalMomentum = t->Muon_GlobalKink_chi2LocalMomentum->at(iMuon);
    muonTMVAChi2LocalPosition = t->Muon_GlobalKink_chi2LocalPosition->at(iMuon);
    muonTMVAGlobalTrackProbability = t->Muon_glbTrackProbability->at(iMuon);
    muonTMVATrackNumberOfValidHits = t->Track_numberOfValidHits->at(trackRef);
    muonTMVATrackExpectedHitsOuter = t->GetNumberOfValidTrackerOuterHits(trackRef);
    muonTMVAResult = tempMuonTMVAReader->EvaluateMVA("BDT");
    if(tempMuonTMVAReader != muonTMVAReader)
    {
      std::cout << "I N F O : Deleting temporary TMVA reader.\n";
      delete tempMuonTMVAReader;
    }
#ifdef mgDEBUG
    std::cout << "Value of MVA for this muon is " << muonTMVAResult << std::endl;
#endif // mgDEBUG
    return (t->Muon_isTracker->at(iMuon) &&
            t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isGlobalMuonPromptTight->at(iMuon) &&
            t->Muon_numberOfMatchedStations->at(iMuon) > 1 &&
            t->Track_layersWithMeasurement->at(trackRef) > 5 &&
            t->Track_validPixelHits->at(trackRef) > 0 &&
            muonTMVAResult > tempMuonTMVACut );
  }
  
  if(tempSelectionVersion.compare("3Bmm") == 0)
  {
    unsigned int trackRef = t->Muon_trackref->at(iMuon);
    if(trackRef == 65535)
      return false;
    if(trackRef == 2048)
    {
      std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectMuon() : Selection \"muonV3Bmm\" cannot be made\n";
      std::cout << "                because inner track of muon is not present in the ntuple!\n";
      std::cout << "                Returning false...\n";
      return false;
    }
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_globalChi2->at(iMuon) < 10. &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0. &&
            t->Muon_numberOfMatchedStations->at(iMuon) > 1 &&
            t->Track_isHighPurity->at(trackRef) &&
            t->Track_validPixelHits->at(trackRef) > 0);
  }
  
  if(tempSelectionVersion.compare("2Bmm") == 0)
  {
    unsigned int trackRef = t->Muon_trackref->at(iMuon);
    if(trackRef == 65535)
      return false;
    if(trackRef == 2048)
    {
      std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectMuon() : Selection \"muonV2Bmm\" cannot be made\n";
      std::cout << "                because inner track of muon is not present in the ntuple!\n";
      std::cout << "                Returning false...\n";
      return false;
    }
    return (t->Muon_isGlobal->at(iMuon) &&
    t->Muon_globalChi2->at(iMuon) < 10. &&
    t->Muon_globalValidMuonHits->at(iMuon) > 0. &&
    t->Muon_numberOfMatchedStations->at(iMuon) > 1 &&
    t->Track_validPixelHits->at(trackRef) > 0);
  }
  
  if(tempSelectionVersion.compare("1Bmm") == 0)
  {
    unsigned int trackRef = t->Muon_trackref->at(iMuon);
    if(trackRef == 65535)
      return false;
    if(trackRef == 2048)
    {
      std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectMuon() : Selection \"muonV1Bmm\" cannot be made\n";
      std::cout << "                because inner track of muon is not present in the ntuple!\n";
      std::cout << "                Returning false...\n";
      return false;
    }
    return (t->Muon_isTracker->at(iMuon) &&
            t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isGlobalMuonPromptTight->at(iMuon) &&
            t->Muon_numberOfMatchedStations->at(iMuon) > 1 &&
            t->Track_layersWithMeasurement->at(trackRef) > 5 &&
            t->Track_validPixelHits->at(trackRef) > 0);
  }
  
  if(tempSelectionVersion.compare("0Bmm") == 0)
  {
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon));
  }
  
  if(tempSelectionVersion.compare("Loose2012") == 0)
  {
    return(t->Muon_isPFMuon->at(iMuon) && 
           (t->Muon_isGlobal->at(iMuon) || 
            t->Muon_isTracker->at(iMuon)));
  }
  
  if(tempSelectionVersion.compare("Tight2012AnyVtx") == 0)
  {
    for(unsigned int iVtx = 0; iVtx < t->Muon_isTight->at(iMuon).size(); iVtx++)
    {
      if(t->Muon_isTight->at(iMuon).at(iVtx))
      {
        return true;
      }
    }
    return false;
  }
  
  if(tempSelectionVersion.compare("7") == 0)
  {
    if(!(t->Muon_isGlobal->at(iMuon) &&
         t->Muon_isTracker->at(iMuon) &&
         fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
         t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
         t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 2 &&
         t->Muon_globalChi2->at(iMuon) < 10 &&
         t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 1 &&
         t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
         t->Muon_numberOfMatches->at(iMuon) > 1 &&
         (t->Muon_selmask->at(iMuon)&1<<4) &&
         (t->Muon_selmask->at(iMuon)&1<<12)))
      return false;
    
    // Veto on the first pixel layer
    // Loop on the tracker hit pattern
    unsigned short iTrack = t->Muon_trackref->at(iMuon);
    bool hasHitInPXB = false;
    bool hasHitInPXF1 = false;
    for(unsigned short iHit = 0; iHit < t->Track_hitPattern->at(iTrack).size(); iHit++)
    {
      unsigned short hit = t->Track_hitPattern->at(iTrack).at(iHit);
      unsigned short hitCopy = hit;
      std::string hitString;
      std::string digits = "01";
      do
      {
        hitString = digits[hitCopy%2] + hitString;
        hitCopy/=2;
      }
      while(hitCopy);
      //       std::cout << "hit " << iHit << " has bitmask: " << hitString << " (value " << hit << ")" << std::endl;
      unsigned short substructure = (hit>>7)&7;
      //       std::cout << "substructure is " << substructure << std::endl;
      unsigned short subsubstructure = (hit>>3)&15;
      //       std::cout << "subsubstructure is " << subsubstructure << std::endl;
      unsigned short type = hit&3;
      //       std::cout << "type is " << type << std::endl;
      if(substructure == 1 && subsubstructure == 1 && type == 0)
        return false;
      if(substructure == 1 && type == 0)
        hasHitInPXB = true;
      if(substructure == 2 && subsubstructure == 1 && type == 0)
        hasHitInPXF1 = true;
    }
    if(!hasHitInPXB && hasHitInPXF1)
      return false;
    std::cout << "Found a muon without a hit in the first pixel layers!\n";
    std::cout << "Pt = " << t->Muon_pt->at(iMuon) << ", eta = " << t->Muon_eta->at(iMuon) << ", phi = " << t->Muon_phi->at(iMuon) << std::endl;
    //     std::cout << "Printing the bitmask:\n";
    //     for(unsigned short iHit = 0; iHit < t->Track_hitPattern->at(iTrack).size(); iHit++)
    //     {
    //       unsigned short hit = t->Track_hitPattern->at(iTrack).at(iHit);
    //       unsigned short hitCopy = hit;
    //       std::string hitString;
    //       std::string digits = "01";
    //       do
    //       {
    //         hitString = digits[hitCopy%2] + hitString;
    //         hitCopy/=2;
    //       }
    //       while(hitCopy);
    //       std::cout << "hit " << iHit << " has bitmask: " << hitString << " (value " << hit << ")" << std::endl;
    //     }
    return true;
  }
  // muonV2NOTMOST
  // muonV6
  if (tempSelectionVersion.compare("2NOTMOST") == 0 ||
      tempSelectionVersion.compare("6") == 0 ||
      tempSelectionVersion.compare("8") == 0 ||
      tempSelectionVersion.compare("9") == 0 )
  {
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 2 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 1 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1 &&
            (t->Muon_selmask->at(iMuon)&1<<4)&&
            fabs(t->Muon_eta->at(iMuon)) < 2.1);
  }

  // muonVSkim
  if (tempSelectionVersion.compare("Skim") == 0)
  {
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 2 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 1 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1 &&
            (t->Muon_selmask->at(iMuon)&1<<4)&&
            t->Muon_pt->at(iMuon) > 3. &&
            fabs(t->Muon_eta->at(iMuon)) < 2.4);
  }

  // muonV5, muonV4, muonV3, muonV2
  if (tempSelectionVersion.compare("5") == 0 ||
      tempSelectionVersion.compare("4") == 0 ||
      tempSelectionVersion.compare("3") == 0 ||
      tempSelectionVersion.compare("2") == 0)
  {
#ifdef mgDEBUG
    if(!t->Muon_isGlobal->at(iMuon))
      std::cout << "\t\tMuon is NOT global\n";
    if(!t->Muon_isTracker->at(iMuon))
      std::cout << "\t\tMuon is NOT tracker\n";
    if(!(t->Muon_pt->at(iMuon) > 4.))
      std::cout << "\t\tMuon pt = " << t->Muon_pt->at(iMuon) << std::endl;
    if(!(fabs(t->Muon_eta->at(iMuon)) < 2.1))
      std::cout << "\t\tMuon eta = " << t->Muon_eta->at(iMuon) << std::endl;
    if(t->Muon_trackref->at(iMuon) == 65535)
      std::cout << "\t\tMuon has NO inner track\n";
    else
    {
      if(t->Muon_trackref->at(iMuon) != 2048)
      {
        if(!(t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11))
          std::cout << "\t\tMuon track # valid hits = " << t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) << std::endl;
        if(!(t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 2.))
          std::cout << "\t\tMuon track chi2 = " << t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) << std::endl;
        if(!(t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 1.))
          std::cout << "\t\tMuon track # pixel hits = " << t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) << std::endl;
      }
    }
    if(!(t->Muon_globalChi2->at(iMuon) < 10))
      std::cout << "\t\tMuon global chi2 = " << t->Muon_globalChi2->at(iMuon) << std::endl;
    if(!(t->Muon_globalValidMuonHits->at(iMuon) > 0))
      std::cout << "\t\tMuon global valid muon hits = " << t->Muon_globalValidMuonHits->at(iMuon) << std::endl;
    if(!(t->Muon_numberOfMatches->at(iMuon) > 1))
      std::cout << "\t\tMuon number of matches = " << t->Muon_numberOfMatches->at(iMuon) << std::endl;
    if(!(t->Muon_selmask->at(iMuon)&1<<4))
      std::cout << "\t\tMuon is NOT TrackerMuonArbitrated\n";
    if(!(t->Muon_selmask->at(iMuon)&1<<12))
      std::cout << "\t\tMuon is NOT TMOneStationTight\n";
#endif // mgDEBUG

    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 2 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 1 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1 &&
            (t->Muon_selmask->at(iMuon)&1<<4) &&
            (t->Muon_selmask->at(iMuon)&1<<12));
  }

  // muonV2Eff
  if (tempSelectionVersion.compare("2Eff") == 0)
  {
    unsigned short iTrk = t->Muon_trackref->at(iMuon);
    unsigned int iVtx = SelectVertexNearestToTrack(iTrk,"V0");
    if(iVtx == 65535)
      return false;
    if(t->Track_dxy_PV->at(iTrk).at(iVtx) > 0.2)
      return false;
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 2 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 1 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1 &&
            (t->Muon_selmask->at(iMuon)&1<<4) &&
            (t->Muon_selmask->at(iMuon)&1<<12));
  }

  // muonV1 muonV0
  if (tempSelectionVersion.compare("1") == 0 ||
      tempSelectionVersion.compare("0") == 0)
  {
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 10 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 0 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1);
  }

  // muonV1a muonV0a
  if (tempSelectionVersion.compare("1a") == 0 ||
      tempSelectionVersion.compare("0a") == 0)
  {
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 10 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 1 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1);

  }

  // muonV1b muonV0b
  if (tempSelectionVersion.compare("1b") == 0 ||
      tempSelectionVersion.compare("0b") == 0)
  {
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 2 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 0 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1);
  }

  // muonV1c muonV0c
  if (tempSelectionVersion.compare("1c") == 0 ||
      tempSelectionVersion.compare("0c") == 0)
  {
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 2 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 1 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1);
  }

  // muonV1d muonV0d
  if (tempSelectionVersion.compare("1d") == 0 ||
      tempSelectionVersion.compare("0d") == 0)
  {
    // 4th selector is TrackerMuonArbitrated , 12th is TMOneStationTight
#ifdef mgDEBUG
    std::cout << "\t\tt->Muon_selmask->at(iMuon)&1<<4 = " << (t->Muon_selmask->at(iMuon)&1<<4) << std::endl;
    std::cout << "\t\tt->Muon_selmask->at(iMuon)&1<<12 = " << (t->Muon_selmask->at(iMuon)&1<<12) << std::endl;
    if(!(t->Muon_selmask->at(iMuon)&1<<4))
      std::cout << "\t\tMuon is NOT TrackerMuonArbitrated\n";
    if(!(t->Muon_selmask->at(iMuon)&1<<12))
      std::cout << "\t\tMuon is NOT TMOneStationTight\n";
#endif // mgDEBUG
    return (t->Muon_isGlobal->at(iMuon) &&
            t->Muon_isTracker->at(iMuon) &&
            fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
            t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
            t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 10 &&
            t->Muon_globalChi2->at(iMuon) < 10 &&
            t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 0 &&
            t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
            t->Muon_numberOfMatches->at(iMuon) > 1 &&
            (t->Muon_selmask->at(iMuon)&1<<4) &&
            (t->Muon_selmask->at(iMuon)&1<<12));
  }

  // Default:
  std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectMuon(...) - selection version \"" << tempSelectionVersion << "\" is not defined!\n";
  std::cout << "                Returning false.\n";
  return false;

  /////////////////////////////////////////////////////////////////////
  // COMMENTED SELECTORS TO BE REMOVED AT THE SECOND ONLINE COMMITTMENT
  /*
  else if(selectionType.compare("tracker") == 0)
  {
    return t->Muon_isTracker->at(iMuon);
  }
  else if(selectionType.compare("soft") == 0)
  {
    return t->Muon_isTracker->at(iMuon) &&
           t->Muon_isTMLastStationAngTight->at(iMuon);
  }
  else if(selectionType.compare("global") == 0)
  {
    return t->Muon_isGlobal->at(iMuon);
  }
  else if(selectionType.compare("tight_nopix") == 0)
  {
    return t->Muon_isGlobal->at(iMuon) &&
    t->Muon_isTracker->at(iMuon) &&
    t->Muon_pt->at(iMuon) > 3. &&
    fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
    t->Track_dz->at(t->Muon_trackref->at(iMuon)) < 20 &&
    t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
    t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 10 &&
    t->Muon_globalChi2->at(iMuon) < 10;
  }
  else if(selectionType.compare("tight_compat") == 0)
  {
    return t->Muon_isGlobal->at(iMuon) &&
    t->Muon_isTracker->at(iMuon) &&
    t->Muon_pt->at(iMuon) > 3. &&
    fabs(t->Muon_eta->at(iMuon)) < 2.1 &&
    fabs(t->Track_dz->at(t->Muon_trackref->at(iMuon))) < 20 &&
    t->Track_numberOfValidHits->at(t->Muon_trackref->at(iMuon)) > 11 &&
    t->Track_normalizedChi2->at(t->Muon_trackref->at(iMuon)) < 10 &&
    t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 0;
  }
  else if(selectionType.compare("tight") == 0)
  {
    // This is supposed to be similar to the selection applied in BPH-10-007
    // Warning: with respect to that paper, the pt cut is lowered from 6 to 3
    return SelectMuon(iMuon,"tight_nopix") &&
           t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) > 0;
  }
  else if(selectionType.compare("tightV4") == 0)
  {
    return SelectMuon(iMuon,"tight") &&
           t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
           t->Muon_isTMLastStationAngTight->at(iMuon) &&
           t->Muon_pt->at(iMuon) > 5.;
  }
  else if(selectionType.compare("tightV5") == 0)
  {
    if(!SelectMuon(iMuon,"tightV4"))
      return false;
    int minDzVertex = SelectVertexNearestToTrack(t->Muon_trackref->at(iMuon),"tightMinDz");
    if(minDzVertex != 65535)
      return fabs(t->Track_dxy_PV->at(t->Muon_trackref->at(iMuon)).at(minDzVertex))<0.2 &&
             fabs(t->Track_dz_PV->at(t->Muon_trackref->at(iMuon)).at(minDzVertex))<1.;
    else
      return false;
  }
  else if(selectionType.compare("nearToTightBSVertex") == 0)
  {
    if(t->Muon_trackref->at(iMuon) == 65535)
      return false;
    int minDzVertex = SelectVertexNearestToTrack(t->Muon_trackref->at(iMuon),"tightBSMinDz");
    if(minDzVertex != 65535)
      return fabs(t->Track_dxy_PV->at(t->Muon_trackref->at(iMuon)).at(minDzVertex))<0.2 &&
      fabs(t->Track_dz_PV->at(t->Muon_trackref->at(iMuon)).at(minDzVertex))<1.;
    else
      return false;
  }
  else if(selectionType.compare("tightV6") == 0)
  {
    return SelectMuon(iMuon,"tightV5") &&
           t->Muon_hasTriggered->at(iMuon);
  }
  else if(selectionType.compare("tightV7") == 0)
  {
    if(!SelectMuon(iMuon,"tightV4"))
      return false;
    int minDzVertex = SelectVertexNearestToTrack(t->Muon_trackref->at(iMuon),"tightBSMinDz");
    if(minDzVertex != 65535)
      return fabs(t->Track_dxy_PV->at(t->Muon_trackref->at(iMuon)).at(minDzVertex))<0.2 &&
      fabs(t->Track_dz_PV->at(t->Muon_trackref->at(iMuon)).at(minDzVertex))<1. &&
      t->Muon_hasTriggered->at(iMuon);
    else
      return false;
  }
  else if(selectionType.compare("tightV8") == 0) // Suggested by Giovanni - remove TMLastStationAngTight and increase #matches
  {
    return SelectMuon(iMuon,"tight") &&
           t->Muon_globalValidMuonHits->at(iMuon) > 0 &&
           t->Muon_pt->at(iMuon) > 5. &&
           SelectMuon(iMuon,"nearToTightBSVertex") &&
           t->Muon_hasTriggered->at(iMuon) &&
           t->Muon_numberOfMatches->at(iMuon) > 1;
  }
  else if(selectionType.compare("pixelless") == 0)
  {
    // If there is no inner track then it is pixelless
    return t->Muon_trackref->at(iMuon) == 65535 || t->Track_validPixelHits->at(t->Muon_trackref->at(iMuon)) == 0;
  }
  else if(selectionType.compare("triggered") == 0)
  {
    // Trigger information only defined in ntuples >= V2
#if NTUPLE_VERSION >= 2
    return t->Muon_hasTriggered->at(iMuon);
#else
    return false;
#endif
  }
  */

}



bool PARTICLESELECTOR::SelectMuonPassingSelector(const int iMuon, const muon::SelectionType selector)
{
  // Check if there is the muon
  if (iMuon >= t->numberOfMuon)
  {
    std::cout << "W A R N I N G! PARTICLESELECTOR::SelectMuonPassingSelector() : asking for a muon out of range\n";
    std::cout << "               Returning false.\n";
    return false;
  }
  return(t->Muon_selmask->at(iMuon)&1<<(int)selector);
}



bool PARTICLESELECTOR::SelectMuonPassingSelector(const int iMuon, const std::string selectorName)
{
  return(SelectMuonPassingSelector(iMuon,mSelectionType[selectorName]));
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectJet(const int iJet, const std::string selectionType)
{

//   std::cout << "          - WHAT ABOUT TRACKJETS?? (Nick)\n";
//   std::cout << "          - They are juicy! (Mario)\n";
// 
//   // Check if there are the jet and the String
//   if (iJet >= t->numberOfJet)
//   {
//     std::cout << " W A R N I N G! PARTICLESELECTOR::SelectJet() : asking for a jet out of range\n";
//     std::cout << "                Returning false.\n";
//     return false;
//   }

  // Check if the string is empty and if the values stored in the
  // dedicated variables (Type and Pt) are correctly encoded
  std::string tempSelectionType = "";
  std::string tempSelectionVersion = "";
  double tempSelectionPt = -999.9;

  if (selectionType.compare("") == 0)
  {
    if(jetStringOk)
    {
      // Retrieve the default values from Event selection
      tempSelectionType = jetTypeString;
      tempSelectionVersion = jetVersionString;
      tempSelectionPt = jetPtThreshold;
    }
    else
    {
      std::cout << "E R R O R! PARTICLESELECTOR::SelectJet(...): Asking for the default selector but without a valid string!\n";
      std::cout << "           Exiting...\n";
      exit(1);
    }
  }
  else
  {
    // Default selection is overridden with a specific one
    // Jet selection string is assumed to be as the Event selection jet substring
    // eg: jetTYPEabcPt23p41 = Type TYPEabc Pt 23.41
    if(!ParseJetString(selectionType, tempSelectionType, tempSelectionVersion, tempSelectionPt))
    {
      std::cout << "E R R O R! PARTICLESELECTOR::SelectJet(...): Using custom selection but with a malformed string!\n";
      std::cout << "           Selection string is:\"" << selectionType << "\"\n";
      std::cout << "           Exiting...\n";
      exit(1);
    }
//     // Discussed with Mario whether it was better to decode the string herein or
//     // to recall here ParseInputString: decided to recall it herei
//     if (selectionType.find("jet") == 0)
//     {
//       if (selectionType.find("Pt") != std::string::npos)
//       {
//         tempSelectionType = selectionType.substr(3, selectionType.find("Pt")-3);
//       }
//       else
//       {
//         std::cout << "E R R O R! PARTICLESELECTOR::SelectJet(...) : Selection string with \"jet\" but without \"Pt\".\n";
//         std::cout << "           Keep the default value.\n";
//       }
//     }
//     else
//     {
//       std::cout << "E R R O R! PARTICLESELECTOR::SelectJet(...) : Selection string without \"jet\".\n";
//       std::cout << "           Keep the default value.\n";
//     }
// 
//     // Pt threshold is a double defined as 3p5 instead of 3.5
//     if (selectionType.find("Pt") != std::string::npos)
//     {
//       std::string tempString = selectionType.substr(selectionType.find("Pt")+2, selectionType.size()-selectionType.find("Pt")+2);
//       if (tempString.find("p") != std::string::npos)
//         tempString.replace(tempString.find("p"),1,".");
// 
//       std::istringstream iss(tempString);
//       iss >> std::dec >> tempSelectionPt;
//       if (iss.rdbuf()->in_avail() != 0) 
//       {
//         std::cout << "E R R O R! PARTICLESELECTOR::SelectJet(...) : Selection substring " << iss.str() << " is badly encoded Pt.\n";
//         std::cout << "           Force to the default value.\n";
//         tempSelectionPt = -999.9;
//       }
//     }
//     else
//     {
//       std::cout << "E R R O R! PARTICLESELECTOR::SelectJet(...) : Selection substring without \"Pt\".\n";
//       std::cout << "           Keep the default value.\n";
//     }
// 
//     // Cross check that retrieved values are compatible with the interface criteria
//     if (tempSelectionPt < 0) 
//     {
//       std::cout << "E R R O R! PARTICLESELECTOR::SelectJet() : asking for custom selection but badly formed Pt\n";
//       std::cout << "           Return false.\n";
//       return false;
//     }
//     if (tempSelectionType.compare("") == 0)
//     {
//       std::cout << "E R R O R! PARTICLESELECTOR::SelectJet() : asking for custom selection but empty Type\n";
//       std::cout << "           Return false.\n";
//       return false;
//     }
  }

  // After long debugging, we are confident that here we have correctly encoded and stored information

  // First, check range and reject according to Pt
  if(tempSelectionType == "PF")
  {
    if (iJet >= t->numberOfJet)
    {
      std::cout << " W A R N I N G! PARTICLESELECTOR::SelectJet() : asking for a PF jet out of range\n";
      std::cout << "                Returning false.\n";
      return false;
    }
    if(t->Jet_pt->at(iJet) <= tempSelectionPt)
    {
      return false;
    }
  }
#if NTUPLE_VERSION >= 3
  else if(tempSelectionType == "Tk")
  {
    if (iJet >= t->numberOfTrackJet)
    {
      std::cout << " W A R N I N G! PARTICLESELECTOR::SelectJet() : asking for a Tk jet out of range\n";
      std::cout << "                Returning false.\n";
      return false;
    }
    if(t->TrackJet_pt->at(iJet) <= tempSelectionPt)
    {
      return false;
    }
  }
  else if(tempSelectionType != "Tk" || tempSelectionType != "PF")
  {
    std::cout << "E R R O R! PARTICLESELECTOR::SelectJet() : only PF or Tk jets are implemented at the moment.\n";
    std::cout << "           No selection available for jets of type \"" << tempSelectionType << "\"";
    std::cout << "           Exiting...\n";
    exit(1);
  }
#else
  else if(tempSelectionType != "PF")
  {
    std::cout << "E R R O R! PARTICLESELECTOR::SelectJet() : only PF jets are implemented for older (V<3) ntuples.\n";
    std::cout << "           No selection available for jets of type \"" << tempSelectionType << "\"";
    std::cout << "           Exiting...\n";
    exit(1);
  }
#endif

  
  // If here, we are confident that we have correctly encoded and stored information and that
  // candidate muon to be selected has the Pt above threshold, now check Type starting from latest version

  // NOTE: in C++ one cannot easily implement switch/case on strings,
  // therefore we must rely on the "cascade" if/else (ONLY "if" because of the "returns")
  // Nicola is unhappy with that because it is not elegant

  // NOTE: avoid recalling recursively SelectJet within itself

  // jetPF
  if (tempSelectionType.compare("PF") == 0)
  {

    // V0
    if (tempSelectionVersion.compare("0") == 0 ||
        tempSelectionVersion.compare("1") == 0)
    {
      return (fabs(t->Jet_eta->at(iJet))<2.6 &&
              t->Jet_hnfr->at(iJet)<1.0 &&
              t->Jet_hcfr->at(iJet)>0.0 &&
              t->Jet_enfr->at(iJet)<1.0 &&
              t->Jet_ecfr->at(iJet)<1.0 &&
              // The b-tag validation uses
              //   chargedMultiplicity>0 && nConstituents>1
              t->Jet_nConstituents->at(iJet)>1 &&
              t->Jet_chargedMultiplicity->at(iJet)>0);
    }

    // VSkim
    if (tempSelectionVersion.compare("Skim") == 0)
    {
      return (//t->Jet_pt->at(iJet)>10 && // Does it make sense?
              fabs(t->Jet_eta->at(iJet))<3.0 &&
              t->Jet_hnfr->at(iJet)<1.0 &&
              t->Jet_hcfr->at(iJet)>0.0 &&
              t->Jet_enfr->at(iJet)<1.0 &&
              t->Jet_ecfr->at(iJet)<1.0 &&
              // The b-tag validation uses
              //   chargedMultiplicity>0 && nConstituents>1
              t->Jet_nConstituents->at(iJet)>1&&
              t->Jet_chargedMultiplicity->at(iJet)>0
              // For the time being I only use the cut below
              // t->Jet_vTrackref->at(iJet).size()>1
              );
    }


  /// ONLY FOR REFERENCE!
  /*
  // jetbTagValidation
  if (tempSelectionType.compare("bTagValidation") == 0)
  {
    return (t->Jet_pt->at(iJet)>30 &&
            fabs(t->Jet_eta->at(iJet))<2.4 &&
            t->Jet_hnfr->at(iJet)<1.0 &&
            t->Jet_hcfr->at(iJet)>0.0 &&
            t->Jet_enfr->at(iJet)<1.0 &&
            t->Jet_ecfr->at(iJet)<1.0 &&
            // The b-tag validation uses
            //   chargedMultiplicity>0 && nConstituents>1
            //   For the time being I only use the cut below
            t->Jet_vTrackref->at(iJet).size()>1);
  }

  // jetbTagValidationTighter
  if (tempSelectionType.compare("bTagValidationTighter") == 0)
  {
    return (t->Jet_pt->at(iJet)>30 &&
            fabs(t->Jet_eta->at(iJet))<2.4 &&
            t->Jet_hnfr->at(iJet)<1.0 &&
            t->Jet_hcfr->at(iJet)>0.0 &&
            t->Jet_enfr->at(iJet)<1.0 &&
            t->Jet_ecfr->at(iJet)<1.0 &&
            // The b-tag validation uses
            //   chargedMultiplicity>0 && nConstituents>1
            //   For the time being I only use the cut below
            t->Jet_vTrackref->at(iJet).size()>2);
  }
  */

    // Default:
    std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectJet(...) - selection version \"" << tempSelectionVersion << "\" is not defined for PF Jets!\n";
    std::cout << "                Returning false.\n";
    return false;
  }

#if NTUPLE_VERSION >= 3
  // jetTk
  // jetTrkV0
  if (tempSelectionType.compare("Tk") == 0 )
  {
    // V0
    if (tempSelectionVersion.compare("0") == 0)
    {
      return (fabs(t->TrackJet_eta->at(iJet))<2.6 &&
              t->TrackJet_nConstituents->at(iJet)>1);
    }

    // Default:
    std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectJet(...) - selection version \"" << tempSelectionVersion << "\" is not defined for Tk Jets!\n";
    std::cout << "                Returning false.\n";
    return false;
  }
#endif

  std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectJet(...) - selection type \"" << selectionType << "\" is not defined!\n";
  std::cout << "                Returning false.\n";
  return false;
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectVertex(const int iVertex, const std::string selectionType)
{
  // Check if the track is in range
  if (iVertex >= t->numberOfVertex)
  {
    std::cout << "E R R O R! PARTICLESELECTOR::SelectVertex() : asking for a vertex out of range\n";
    std::cout << "           Exiting...\n";
    exit(1);
  }
  // default (empty) and V0
  if (selectionType.compare("") == 0 ||
      selectionType.compare("V0") == 0)
  {
    double xPVwrtBS = t->PV_x->at(iVertex) - t->beamSpot_x;
    double yPVwrtBS = t->PV_y->at(iVertex) - t->beamSpot_y;
    double zPVwrtBS = t->PV_z->at(iVertex) - t->beamSpot_z;
    return (!(t->PV_isFake->at(iVertex)) &&
            t->PV_ndof->at(iVertex) > 7 &&
            fabs(zPVwrtBS) <= 24 &&
            sqrt(xPVwrtBS*xPVwrtBS + yPVwrtBS*yPVwrtBS) <= 1.8);
  }

  // stdGoodVertexFilter
  if (selectionType.compare("stdGoodVertexFilter") == 0)
  {
    return (!(t->PV_isFake->at(iVertex)) &&
            t->PV_ndof->at(iVertex) > 4 &&
            fabs(t->PV_z->at(iVertex)) <= 15 &&
            sqrt(t->PV_x->at(iVertex)*t->PV_x->at(iVertex) + t->PV_y->at(iVertex)*t->PV_y->at(iVertex)) <= 2);
  }

  /*
  if(selectionType.compare("tight") == 0)
  {
    return !(t->PV_isFake->at(iVertex)) &&
           t->PV_ndof->at(iVertex) > 20 &&
           fabs(t->PV_z->at(iVertex)) <= 15 &&
           sqrt(t->PV_x->at(iVertex)*t->PV_x->at(iVertex) + t->PV_y->at(iVertex)*t->PV_y->at(iVertex)) <= 2;
  }
  if(selectionType.compare("tightBS") == 0)
  {
    double xPVwrtBS = t->PV_x->at(iVertex) - t->beamSpot_x;
    double yPVwrtBS = t->PV_y->at(iVertex) - t->beamSpot_y;
    double zPVwrtBS = t->PV_z->at(iVertex) - t->beamSpot_z;
    return !(t->PV_isFake->at(iVertex)) &&
           t->PV_ndof->at(iVertex) > 20 &&
           fabs(zPVwrtBS) <= 14 &&
           sqrt(xPVwrtBS*xPVwrtBS + yPVwrtBS*yPVwrtBS) <= 1.8;
  }
  */

  std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectVertex(...) - selection string \"" << selectionType << "\" is not defined!\n";
  std::cout << "                Returning false (REDUNDANT).\n";
  return false;
}



/**************************************************************************************/
int PARTICLESELECTOR::SelectVertexNearestToTrack(const int iTrack, const std::string method)
{
  // Check if the track is valid or not
  if(iTrack == 65535 && iTrack != 2048)
  {
    std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectVertexNearestToTrack(...) - Null track pointer!\n";
    std::cout << "                Returning 65535\n";
    return 65535;
  }

  // Check if the track is in range
  if (iTrack >= t->numberOfTrack)
  {
    std::cout << "E R R O R! PARTICLESELECTOR::SelectVertexNearestToTrack() : asking for a track out of range\n";
    std::cout << "           Exiting...\n";
    exit(1);
  }

  // Here the track index should be fine
  // Check the vertex selection method

  // TEST VERSION
  if (method.compare("V1") == 0)
  {
#ifdef mgDEBUG
    std::cout << "\t\tSelecting vertex with method V1\n";
#endif
    unsigned int iVertexMinDz=65535;
    double minDzTrackVertex=9999999;
    for(int iVertex = 0; iVertex < t->numberOfVertex; iVertex++)
    {
      if(fabs(t->Track_dz_PV->at(iTrack).at(iVertex)) < fabs(minDzTrackVertex))
      {
        minDzTrackVertex = t->Track_dz_PV->at(iTrack).at(iVertex);
        iVertexMinDz = iVertex;
      }
    }

    if(fabs(minDzTrackVertex) < 1)
    {
      if(SelectVertex(iVertexMinDz,"V0"))
        return iVertexMinDz;
      else
        return 65535;
    }
    return 65535;
  }





  // default (empty) and V0
  if (method.compare("") == 0 ||
      method.compare("V0") == 0)
  {
#ifdef mgDEBUG
    std::cout << "\t\tSelecting vertex with method V0\n";
#endif // mgDEBUG
    unsigned int iVertexMinDz=65535;
    double minDzTrackVertex=9999999;
    for(int iVertex = 0; iVertex < t->numberOfVertex; iVertex++)
    {
#ifdef mgDEBUG
      std::cout << "\t\tLooking at vertex " << iVertex << std::endl;
      std::cout << "\t\tdz(PV) = " << t->Track_dz_PV->at(iTrack).at(iVertex) << std::endl;
#endif // mgDEBUG
      if(SelectVertex(iVertex,"V0") && fabs(t->Track_dz_PV->at(iTrack).at(iVertex)) < fabs(minDzTrackVertex))
      {
        minDzTrackVertex = t->Track_dz_PV->at(iTrack).at(iVertex);
        iVertexMinDz = iVertex;
      }
    }
    if(fabs(minDzTrackVertex) < 1)
      return iVertexMinDz;
    else
      return 65535;
  }
/*
  // minDz
  if(method.compare("minDz") == 0)
  {
#ifdef mgDEBUG
    std::cout << "\t\tSelecting vertex with method minDz\n";
#endif // mgDEBUG
    unsigned int iVertexMinDz=65535;
    double minDzTrackVertex=9999999;
    for(int iVertex = 0; iVertex < t->numberOfVertex; iVertex++)
    {
      if(fabs(t->Track_dz_PV->at(iTrack).at(iVertex)) < fabs(minDzTrackVertex))
      {
        minDzTrackVertex = t->Track_dz_PV->at(iTrack).at(iVertex);
        iVertexMinDz = iVertex;
      }
    }
    return iVertexMinDz;
  }

  // stdGoodMinDz
  if(method.compare("stdGoodMinDz") == 0)
  {
#ifdef mgDEBUG
    std::cout << "\t\tSelecting vertex with method stdGoodMinDz\n";
#endif // mgDEBUG
    unsigned int iVertexMinDz=65535;
    double minDzTrackVertex=9999999;
    for(int iVertex = 0; iVertex < t->numberOfVertex; iVertex++)
    {
      if(SelectVertex(iVertex,"stdGoodVertexFilter") && fabs(t->Track_dz_PV->at(iTrack).at(iVertex)) < fabs(minDzTrackVertex))
      {
        minDzTrackVertex = t->Track_dz_PV->at(iTrack).at(iVertex);
        iVertexMinDz = iVertex;
      }
    }
    return iVertexMinDz;
  }

  // tightMinDz
  if(method.compare("tightMinDz") == 0)
  {
#ifdef mgDEBUG
    std::cout << "\t\tSelecting vertex with method tightMinDz\n";
#endif // mgDEBUG
    unsigned int iVertexMinDz=65535;
    double minDzTrackVertex=9999999;
    for(int iVertex = 0; iVertex < t->numberOfVertex; iVertex++)
    {
      if(SelectVertex(iVertex,"tight") && fabs(t->Track_dz_PV->at(iTrack).at(iVertex)) < fabs(minDzTrackVertex))
      {
        minDzTrackVertex = t->Track_dz_PV->at(iTrack).at(iVertex);
        iVertexMinDz = iVertex;
      }
    }
    return iVertexMinDz;
  }

  // tightBSMinDz
  if(method.compare("tightBSMinDz") == 0)
  {
#ifdef mgDEBUG
    std::cout << "\t\tSelecting vertex with method tightBSMinDz\n";
#endif // mgDEBUG
    unsigned int iVertexMinDz=65535;
    double minDzTrackVertex=9999999;
    for(int iVertex = 0; iVertex < t->numberOfVertex; iVertex++)
    {
      if(SelectVertex(iVertex,"tightBS") && fabs(t->Track_dz_PV->at(iTrack).at(iVertex)) < fabs(minDzTrackVertex))
      {
        minDzTrackVertex = t->Track_dz_PV->at(iTrack).at(iVertex);
        iVertexMinDz = iVertex;
      }
    }
    return iVertexMinDz;
  }
*/

#ifdef mgDEBUG
  std::cout << "W A R N I N G ! PARTICLESELECTOR::SelectVertexNearestToTrack(...) - selection method \"" << method << "\" is not defined!\n";
  std::cout << "                Returning 65535 (REDUNDANT).\n";
#endif // mgDEBUG
  return 65535;
}



#if NTUPLE_VERSION >= 4
bool PARTICLESELECTOR::SelectMuonPassingTrigger(const int iMuon, const std::string triggerName)
{  
  for(unsigned int iTrigger = 0; iTrigger < t->triggerPathNames->size(); iTrigger++)
  {
    if(t->triggerPathNames->at(iTrigger).compare(triggerName) == 0)
    {
#ifdef mgDEBUG
      std::cout << "\tFound trigger path " << t->triggerPathNames->at(iTrigger) << std::endl;
#endif
      if(t->Muon_triggerInfo->at(iMuon).at(iTrigger) == 1)
      {
#ifdef mgDEBUG
        std::cout << "\tMuon " << i << " is firing the trigger " << t->triggerPathNames->at(iTrigger) << "!\n";
#endif
        return true;
      }
      else
      {
#ifdef mgDEBUG
        std::cout << "\tMuon " << i << " is not firing the trigger " << t->triggerPathNames->at(iTrigger) << "!\n";
#endif
        return false;
      }  
    }
  }
  std::cout << "W A R N I N G ! Trigger path " << triggerName << " not found!\n";
  std::cout << "                SelectMuonPassingTrigger rejecting muon " << iMuon << "!\n";
  return false;
}



bool PARTICLESELECTOR::SelectTightMuon(const int iMuon, const int iVertex)
{
  return (t->Muon_isTight->at(iMuon).at(iVertex));
}
#endif



/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
#if NTUPLE_VERSION >= 3
bool PARTICLESELECTOR::SelectEventPassingTrigger(const std::string triggerName)
{
  for(unsigned int iTrigger = 0; iTrigger < t->triggerPathNames->size(); iTrigger++)
  {
    if(t->triggerPathNames->at(iTrigger).compare(triggerName) == 0)
    {
#ifdef mgDEBUG
      std::cout << "\tFound trigger path " << t->triggerPathNames->at(iTrigger) << std::endl;
#endif
      if(t->Trigger_accepted->at(iTrigger) == true)
      {
#ifdef mgDEBUG
        std::cout << "\tPath is accepting the event!\n";
#endif
        return true;
      }
      else
      {
#ifdef mgDEBUG
        std::cout << "\tPath is rejecting the event!\n";
#endif
        return false;
      }
    }
  }
  std::cout << "W A R N I N G ! Trigger path " << triggerName << " not found!\n";
  std::cout << "                Rejecting the event!\n";
  return false;
}



/**************************************************************************************/
#ifdef USING_MC
double PARTICLESELECTOR::GetEventPdfWeight(const std::string pdfSetName, const unsigned int index)
{
  for(unsigned int iPdfSet = 0; iPdfSet < t->pdfWeightNames->size(); iPdfSet++)
  {
    if(t->pdfWeightNames->at(iPdfSet).compare(pdfSetName) == 0)
    {
#ifdef mgDEBUG
      std::cout << "\tFound PDF set " << t->pdfWeightNames->at(iPdfSet) << std::endl;
#endif
      return t->Genevt_pdfWeights->at(iPdfSet).at(index);
    }
  }
  std::cout << "W A R N I N G ! PDF set " << pdfSetName << " not found!\n";
  std::cout << "                Giving weight 0 to the event!\n";
  return 0;
};
#endif // USING_MC
#endif // NTUPLE_VERSION >= 3



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithMuons(const unsigned int nToSelect, const bool exactNumber, std::vector<unsigned short>& indexes)
{
  // Call method with empty selector to trigger use of default one
  return SelectEventWithMuons("", nToSelect, exactNumber, indexes);
  
//   // Using default muon selection
//   indexes.clear();
//   if(t->numberOfMuon < (int)nToSelect)
//   {
//     return false;
//   }
//   unsigned short int selectedMuons = 0;
//   for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
//   {
//     if(SelectMuon(i))
//     {
//       selectedMuons++;
//       indexes.push_back(i);
//     }
//     if(exactNumber && selectedMuons > nToSelect)
//     {
//       return false;
//     }
//   }
//   if(exactNumber)
//   {
//     if(selectedMuons == nToSelect)
//       return true;
//     else
//       return false;
//   }
//   else
//   {
//     if(selectedMuons >= nToSelect)
//       return true;
//   }
//   return false;
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithMuons(const std::string selectionType, const unsigned int nToSelect,
                                            const bool exactNumber, std::vector<unsigned short>& indexes)
{
  // Using custom muon selection
  indexes.clear();
  if(t->numberOfMuon < (int)nToSelect)
  {
    return false;
  }
  unsigned short int selectedMuons = 0;
  for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
  {
    if(SelectMuon(i,selectionType))
    {
      selectedMuons++;
      indexes.push_back(i);
    }
    if(exactNumber && selectedMuons > nToSelect)
    {
      return false;
    }
  }
  if(exactNumber)
  {
    if(selectedMuons == nToSelect)
      return true;
    else
      return false;
  }
  else
  {
    if(selectedMuons >= nToSelect)
      return true;
  }
  return false;
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithMuonsInJets(const unsigned int nToSelect, const bool exactNumber, std::vector<unsigned short>& indexes)
{
  
  return SelectEventWithMuonsInJets("", "", jetTypeString, nToSelect, exactNumber, indexes);
  
//   // Using default muon and jet selection
//   indexes.clear();
//   if(t->numberOfMuon < (int)nToSelect)
//   {
//     return false;
//   }
//   unsigned short int selectedMuons = 0;
// 
//   // Loop over muons
//   for(unsigned int iMuon = 0; iMuon < t->Muon_isTracker->size(); iMuon++)
//   {
//     // Select the muon and get associated track
//     // hence get the jet from the track
//     if(SelectMuon(iMuon))
//     {
//       short int iAssociatedJet = -1;
// 
//       int iTrack = t->Muon_trackref->at(iMuon) ;
//       if( (iTrack < 0) || (iTrack > t->numberOfTrack-1) ) 
//         continue;
// 
//       int totalNumberOfJets = 0;
//       if(jetSelectionString.find("PF") != std::string::npos)
//       {
//         iAssociatedJet = t->Track_jetRef->at( iTrack ) ;
//         totalNumberOfJets = t->numberOfJet;
//       }
//       else if(jetSelectionString.find("Tk") != std::string::npos ||
//               jetSelectionString.find("Trk") != std::string::npos)
//       {
//         iAssociatedJet = t->Track_trackJetRef->at( iTrack ) ;
//         totalNumberOfJets = t->numberOfTrackJet;
//       }
// 
//       // Select the jet associated to the muon, if available
//       if (iAssociatedJet < 0 ||
//           iAssociatedJet > totalNumberOfJets-1 ||
//           !SelectJet(iAssociatedJet))
//         continue;
// 
//       // Store the muon if selected within jet
//       selectedMuons++;
//       indexes.push_back(iMuon);
//     }
// 
//     // Do you really want to select exactly nToSelect muons?
//     if(exactNumber && selectedMuons > nToSelect)
//     {
//       return false;
//     }
//   } // Loop over muons
// 
//   // Further cross check on exactly nToSelect muons selection
//   if(exactNumber)
//   {
//     if(selectedMuons == nToSelect)
//       return true;
//     else
//       return false;
//   }
//   else
//   {
//     if(selectedMuons >= nToSelect)
//       return true;
//   }
//   return false;
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithMuonsInJets(const std::string muonSelectionType, const std::string jetSelectionType,
                                                  const unsigned int nToSelect, const bool exactNumber, std::vector<unsigned short>& indexes)
{
  std::string jetType, jetVersion;
  double jetPt;
  ParseJetString(jetSelectionType, jetType, jetVersion, jetPt);

  return SelectEventWithMuonsInJets(muonSelectionType, jetSelectionType, jetType, nToSelect, exactNumber, indexes);
  
//   // Using custom muon and jet selection
//   indexes.clear();
//   if(t->numberOfMuon < (int)nToSelect)
//   {
//     return false;
//   }
//   unsigned short int selectedMuons = 0;
// 
//   // Loop over muons
//   for(unsigned int iMuon = 0; iMuon < t->Muon_isTracker->size(); iMuon++)
//   {
//     // Select the muon and get associated track
//     // hence get the jet from the track
//     if(SelectMuon(iMuon,muonSelectionType))
//     {
//       short int iAssociatedJet = -1;
// 
//       int iTrack = t->Muon_trackref->at(iMuon) ;
//       if( (iTrack < 0) || (iTrack > t->numberOfTrack-1) ) 
//         continue;
// 
//       int totalNumberOfJets = 0;
//       if(jetSelectionType.find("PF") != std::string::npos)
//       {
//         iAssociatedJet = t->Track_jetRef->at( iTrack ) ;
//         totalNumberOfJets = t->numberOfJet;
//       }
//       else if(jetSelectionType.find("Tk") != std::string::npos ||
//               jetSelectionType.find("Trk") != std::string::npos)
//       {
//         iAssociatedJet = t->Track_trackJetRef->at( iTrack ) ;
//         totalNumberOfJets = t->numberOfTrackJet;
//       }
// 
//       // Select the jet associated to the muon, if available
//       if (iAssociatedJet < 0 ||
//           iAssociatedJet > totalNumberOfJets-1 ||
//           !SelectJet(iAssociatedJet,jetSelectionType))
//         continue;
// 
//       // Store the muon if selected within jet
//       selectedMuons++;
//       indexes.push_back(iMuon);
//     }
// 
//     // Do you really want to select exactly nToSelect muons?
//     if(exactNumber && selectedMuons > nToSelect)
//     {
//       return false;
//     }
//   } // Loop over muons
// 
//   // Further cross check on exactly nToSelect muons selection
//   if(exactNumber)
//   {
//     if(selectedMuons == nToSelect)
//       return true;
//     else
//       return false;
//   }
//   else
//   {
//     if(selectedMuons >= nToSelect)
//       return true;
//   }
//   return false;
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithMuonsInJets(const std::string muonSelectionType, const std::string jetSelectionType,
                                                  const std::string jetType, const unsigned int nToSelect, const bool exactNumber,
                                                  std::vector<unsigned short>& indexes)
{
  // Using custom muon and jet selection
  indexes.clear();
  if(t->numberOfMuon < (int)nToSelect)
  {
    return false;
  }
  unsigned short int selectedMuons = 0;
  
  // Loop over muons
  for(unsigned int iMuon = 0; iMuon < t->Muon_isTracker->size(); iMuon++)
  {
    // Select the muon and get associated track
    // hence get the jet from the track
    if(SelectMuon(iMuon,muonSelectionType))
    {
      short int iAssociatedJet = -1;
      
      int iTrack = t->Muon_trackref->at(iMuon) ;
      if( (iTrack < 0) || (iTrack > t->numberOfTrack-1) ) 
        continue;
      
      int totalNumberOfJets = 0;
      if(jetType.compare("PF") == 0)
      {
        iAssociatedJet = t->Track_jetRef->at( iTrack ) ;
        totalNumberOfJets = t->numberOfJet;
      }
      else if(jetType.compare("Tk") == 0 ||
        jetType.compare("Trk") == 0)
      {
        iAssociatedJet = t->Track_trackJetRef->at( iTrack ) ;
        totalNumberOfJets = t->numberOfTrackJet;
      }
      
      // Select the jet associated to the muon, if available
      if (iAssociatedJet < 0 ||
        iAssociatedJet > totalNumberOfJets-1 ||
        !SelectJet(iAssociatedJet,jetSelectionType))
        continue;
      
      // Store the muon if selected within jet
        selectedMuons++;
        indexes.push_back(iMuon);
    }
    
    // Do you really want to select exactly nToSelect muons?
    if(exactNumber && selectedMuons > nToSelect)
    {
      return false;
    }
  } // Loop over muons
  
  // Further cross check on exactly nToSelect muons selection
  if(exactNumber)
  {
    if(selectedMuons == nToSelect)
      return true;
    else
      return false;
  }
  else
  {
    if(selectedMuons >= nToSelect)
      return true;
  }
  return false;
}



/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonV0(const std::string selectionType, std::vector<unsigned short>& indexes, unsigned short& iVertex)
{
#ifdef mgDEBUG
  std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonV0\n";
#endif // mgDEBUG

  std::string selectionVersion;
  double ptThreshold;
  ParseMuonString(selectionType, selectionVersion, ptThreshold);
  
  return SelectEventWithDiMuonV0(selectionType, selectionVersion, indexes, iVertex);
  
//   // Return false if there are less than 2 muons
//   if(t->numberOfMuon < 2)
//   {
// #ifdef mgDEBUG
//     std::cout << "\tLess than 2 muons: returning false.\n";
// #endif // mgDEBUG
//     return false;
//   }
// 
//   std::vector<unsigned short> vSelectedMuons;
//   std::map<unsigned short, std::vector<unsigned short> > muonVerticesMap;
// 
//   // Loop over muons
//   for(unsigned short i = 0; i < t->numberOfMuon; i++)
//   {
// #ifdef mgDEBUG
//     std::cout << "\tMuon " << i << std::endl;
// #endif // mgDEBUG
// 
//     // Select the muon
//     if(SelectMuon(i,selectionType))
//     {
// #ifdef mgDEBUG
//       std::cout << "\t\tMuon " << i << " is selected.\n";
// #endif // mgDEBUG
// 
//       // Get also the vertex associated to the muon
//       vSelectedMuons.push_back(i);
//       unsigned short iVtx = SelectVertexNearestToTrack(t->Muon_trackref->at(vSelectedMuons[vSelectedMuons.size()-1]),"V0");
//       if(iVtx != 65535)
//       {
// #ifdef mgDEBUG
//         std::cout << "\t\tGood vertex " << iVtx << " is selected.\n";
// #endif // mgDEBUG
// 
//         // Update the map of associated vertices
//         if(muonVerticesMap.find(iVtx) != muonVerticesMap.end())
//         {
// #ifdef mgDEBUG
//           std::cout << "\t\tVertex " << iVtx << " already in map. Adding muon " << i << " to list.\n";
// #endif // mgDEBUG
// 
//           muonVerticesMap[iVtx].push_back(vSelectedMuons[vSelectedMuons.size()-1]);
//         }
//         else
//         {
// #ifdef mgDEBUG
//           std::cout << "\t\tVertex " << iVtx << " not in map. Creating new vector for muon" << i << std::endl;
// #endif // mgDEBUG
// 
//           std::vector<unsigned short> vMuonIndexes;
//           vMuonIndexes.push_back(vSelectedMuons[vSelectedMuons.size()-1]);
//           muonVerticesMap[iVtx] = vMuonIndexes;
//         }
//       }
// 
// #ifdef mgDEBUG
//       else
//         std::cout << "\t\tNo good vertex found for muon " << i << ".\n";
// #endif // mgDEBUG
//     }
// 
// #ifdef mgDEBUG
//     else
//       std::cout << "\t\tMuon " << i << " is not selected.\n";
// #endif // mgDEBUG
// 
//   } // Loop over muons
// 
//   // Now we have two good muons
//   if(vSelectedMuons.size() < 2)
//   {
// #ifdef mgDEBUG
//     std::cout << "Less than 2 good muons in the event. Returning false.\n";
// #endif // mgDEBUG
//     return false;
//   }
// 
//   bool found = false;
//   unsigned short iSelectedVertex = 65535;
// 
// #ifdef mgDEBUG
//   std::cout << "Checking muon multiplicity of vertices.\n";
// #endif // mgDEBUG
// 
//   // Check muon vertex multiplicities
//   // Loop over vertices in the map
//   for(std::map<unsigned short, std::vector<unsigned short> >::const_iterator it = muonVerticesMap.begin(); it != muonVerticesMap.end(); it++)
//   {
//     // Two vertices with exactly two muons
//     if(found && it->second.size() == 2)
//     {
// #ifdef mgDEBUG
//       std::cout << "\tVertex " << it->first << " is the second dimuon vertex found in the event. Returning false.\n";
// #endif // mgDEBUG
//       return false;
//     }
// 
//     if(it->second.size() == 2)
//     {
// #ifdef mgDEBUG
//       std::cout << "\tVertex " << it->first << " is a good dimuon vertex.\n";
// #endif // mgDEBUG
//       found = true;
//       iSelectedVertex = it->first;
//     }
//   }
//   if(found)
//   {
// #ifdef mgDEBUG
//     std::cout << "Found one and only one good dimuon vertex.\n";
//     std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
//     std::cout << "Now check that the dimuon has a good invariant mass.\n";
// #endif // mgDEBUG
// 
//     unsigned int iMuon1 = muonVerticesMap[iSelectedVertex].at(0);
//     unsigned int iMuon2 = muonVerticesMap[iSelectedVertex].at(1);
// 
// #ifdef mgDEBUG
//     std::cout << "iMuon1 = " << iMuon1 << ", iMuon2 = " << iMuon2 << std::endl;
// #endif // mgDEBUG
// 
//     // Once the muons are selected, check the dimuon mass
//     // if muon mass in the range of a resonance, then return false
//     TLorentzVector pMuon1,pMuon2;
//     pMuon1.SetPtEtaPhiM(t->Muon_pt->at(iMuon1),t->Muon_eta->at(iMuon1),t->Muon_phi->at(iMuon1),constants::muonMass);
//     pMuon2.SetPtEtaPhiM(t->Muon_pt->at(iMuon2),t->Muon_eta->at(iMuon2),t->Muon_phi->at(iMuon2),constants::muonMass);
//     double diMuonMass = (pMuon1 + pMuon2).M();
// 
// #ifdef mgDEBUG
//     std::cout << "diMuonMass = " << diMuonMass << std::endl;
// #endif // mgDEBUG
// 
//     // Mass rejection is different for different muon selection versions
//     if(selectionType.find("V0") != std::string::npos)
//     {
//       if(diMuonMass>70 ||
//          (diMuonMass>0.75 && diMuonMass<0.8) ||
//          (diMuonMass>1 && diMuonMass<1.04) ||
//          (diMuonMass>2.95 && diMuonMass<3.8) ||
//          (diMuonMass>8.9 && diMuonMass<10.6) ||
//          (diMuonMass<0.5))
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
// /*
//     else if(selectionType.find("V1") != std::string::npos || selectionType.find("V2") != std::string::npos)
//     {
//       if(diMuonMass < 5.
//         || (diMuonMass>8.9 && diMuonMass<10.6)
//         || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
// */
//     else if(selectionType.find("V3") != std::string::npos)
//     {
//       if(diMuonMass<10.6 || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
//     else if(selectionType.find("V4") != std::string::npos)
//     {
//       if(diMuonMass < 5. || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
//     else if(selectionType.find("V5") != std::string::npos)
//     {
//       if(diMuonMass < 5.
//         || (diMuonMass>8.9 && diMuonMass<10.6)
//         || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//       if(pMuon1.DeltaR(pMuon2) < 1.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon has Delta(R) < 1: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
//     /// Default DiMu Mass rejection
//     else
//     {
//       if( diMuonMass < 5.
//           || (diMuonMass>8.9 && diMuonMass<10.6)
//           || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
// 
// 
//     iVertex = iSelectedVertex;
//     indexes = muonVerticesMap[iSelectedVertex];
// 
// #ifdef mgDEBUG
//     std::cout << "\tiVertex = " << iVertex << std::endl;
//     for(unsigned int i = 0; i < indexes.size(); i++)
//       std::cout << "\tindexes[" << i << "] = " << indexes[i] << std::endl;
// #endif // mgDEBUG
//     return true;
//   }
// 
// #ifdef mgDEBUG
//   std::cout << "SOME CONDITION FAILED! RETURNING FALSE!\n";
// #endif // mgDEBUG
//   return false;
}



/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonV0(const std::string selectionType, const std::string selectionVersion, std::vector<unsigned short>& indexes, unsigned short& iVertex)
{
#ifdef mgDEBUG
  std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonV0\n";
#endif // mgDEBUG
  
  // Return false if there are less than 2 muons
  if(t->numberOfMuon < 2)
  {
#ifdef mgDEBUG
    std::cout << "\tLess than 2 muons: returning false.\n";
#endif // mgDEBUG
    return false;
  }
  
  std::vector<unsigned short> vSelectedMuons;
  std::map<unsigned short, std::vector<unsigned short> > muonVerticesMap;
  
  // Loop over muons
  for(unsigned short i = 0; i < t->numberOfMuon; i++)
  {
#ifdef mgDEBUG
    std::cout << "\tMuon " << i << std::endl;
#endif // mgDEBUG
    
    // Select the muon
    if(SelectMuon(i,selectionType))
    {
#ifdef mgDEBUG
      std::cout << "\t\tMuon " << i << " is selected.\n";
#endif // mgDEBUG
      
      // Get also the vertex associated to the muon
      vSelectedMuons.push_back(i);
      unsigned short iVtx;

      if (selectionVersion.compare("8") == 0)
      {
        iVtx = SelectVertexNearestToTrack(t->Muon_trackref->at(vSelectedMuons[vSelectedMuons.size()-1]),"V1");
      }
      else
      {
        iVtx = SelectVertexNearestToTrack(t->Muon_trackref->at(vSelectedMuons[vSelectedMuons.size()-1]),"V0");
      }

      if(iVtx != 65535)
      {
#ifdef mgDEBUG
        std::cout << "\t\tGood vertex " << iVtx << " is selected.\n";
#endif // mgDEBUG
        
        // Update the map of associated vertices
        if(muonVerticesMap.find(iVtx) != muonVerticesMap.end())
        {
#ifdef mgDEBUG
          std::cout << "\t\tVertex " << iVtx << " already in map. Adding muon " << i << " to list.\n";
#endif // mgDEBUG
          
          muonVerticesMap[iVtx].push_back(vSelectedMuons[vSelectedMuons.size()-1]);
        }
        else
        {
#ifdef mgDEBUG
          std::cout << "\t\tVertex " << iVtx << " not in map. Creating new vector for muon" << i << std::endl;
#endif // mgDEBUG
          
          std::vector<unsigned short> vMuonIndexes;
          vMuonIndexes.push_back(vSelectedMuons[vSelectedMuons.size()-1]);
          muonVerticesMap[iVtx] = vMuonIndexes;
        }
      }
      
#ifdef mgDEBUG
      else
        std::cout << "\t\tNo good vertex found for muon " << i << ".\n";
#endif // mgDEBUG
    }
    
#ifdef mgDEBUG
    else
      std::cout << "\t\tMuon " << i << " is not selected.\n";
#endif // mgDEBUG  
  } // Loop over muons
  
  // Now we have two good muons
  if(vSelectedMuons.size() < 2)
  {
#ifdef mgDEBUG
    std::cout << "Less than 2 good muons in the event. Returning false.\n";
#endif // mgDEBUG
    return false;
  }
  
  bool found = false;
  unsigned short iSelectedVertex = 65535;
  
#ifdef mgDEBUG
  std::cout << "Checking muon multiplicity of vertices.\n";
#endif // mgDEBUG
  
  // Check muon vertex multiplicities
  // Loop over vertices in the map
  for(std::map<unsigned short, std::vector<unsigned short> >::const_iterator it = muonVerticesMap.begin(); it != muonVerticesMap.end(); it++)
  {
    // Two vertices with exactly two muons
    if(found && it->second.size() == 2)
    {
#ifdef mgDEBUG
      std::cout << "\tVertex " << it->first << " is the second dimuon vertex found in the event. Returning false.\n";
#endif // mgDEBUG
      return false;
    }
    
    if(it->second.size() == 2)
    {
#ifdef mgDEBUG
      std::cout << "\tVertex " << it->first << " is a good dimuon vertex.\n";
#endif // mgDEBUG
      found = true;
      iSelectedVertex = it->first;
    }
  }
  if(found)
  {
#ifdef mgDEBUG
    std::cout << "Found one and only one good dimuon vertex.\n";
    std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
    std::cout << "Now check that the dimuon has a good invariant mass.\n";
#endif // mgDEBUG
    
    unsigned int iMuon1 = muonVerticesMap[iSelectedVertex].at(0);
    unsigned int iMuon2 = muonVerticesMap[iSelectedVertex].at(1);
   
    if (selectionVersion.compare("9") == 0)
    {
      if ( fabs(t->Track_dz_PV->at(t->Muon_trackref->at(iMuon1)).at(iSelectedVertex)) > 0.15 &&
           fabs(t->Track_dz_PV->at(t->Muon_trackref->at(iMuon2)).at(iSelectedVertex)) > 0.15 )   
      {
        return false;
      }
    }

 
#ifdef mgDEBUG
    std::cout << "iMuon1 = " << iMuon1 << ", iMuon2 = " << iMuon2 << std::endl;
#endif // mgDEBUG
    
    // Once the muons are selected, check the dimuon mass
    // if muon mass in the range of a resonance, then return false
    TLorentzVector pMuon1,pMuon2;
    pMuon1.SetPtEtaPhiM(t->Muon_pt->at(iMuon1),t->Muon_eta->at(iMuon1),t->Muon_phi->at(iMuon1),constants::muonMass);
    pMuon2.SetPtEtaPhiM(t->Muon_pt->at(iMuon2),t->Muon_eta->at(iMuon2),t->Muon_phi->at(iMuon2),constants::muonMass);
    double diMuonMass = (pMuon1 + pMuon2).M();
    
#ifdef mgDEBUG
    std::cout << "diMuonMass = " << diMuonMass << std::endl;
#endif // mgDEBUG
    
    // Mass rejection is different for different muon selection versions
    if(selectionVersion.compare("0") == 0)
    {
      if(diMuonMass>70 ||
        (diMuonMass>0.75 && diMuonMass<0.8) ||
        (diMuonMass>1 && diMuonMass<1.04) ||
        (diMuonMass>2.95 && diMuonMass<3.8) ||
        (diMuonMass>8.9 && diMuonMass<10.6) ||
        (diMuonMass<0.5))
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    /*
     *    else if(selectionVersion.compare("1") == 0 || selectionVersion.compare("2") == 0)
     *    {
     *      if(diMuonMass < 5.
     *        || (diMuonMass>8.9 && diMuonMass<10.6)
     *        || diMuonMass > 70.)
     *      {
     * #ifdef mgDEBUG
     *        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
     * #endif // mgDEBUG
     *        return false;
     }
     }
     */
    else if(selectionVersion.compare("3") == 0)
    {
      if(diMuonMass<10.6 || diMuonMass > 70.)
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    else if(selectionVersion.compare("4") == 0)
    {
      if(diMuonMass < 5. || diMuonMass > 70.)
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    else if(selectionVersion.compare("5") == 0)
    {
      if(diMuonMass < 5.
        || (diMuonMass>8.9 && diMuonMass<10.6)
        || diMuonMass > 70.)
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
      if(pMuon1.DeltaR(pMuon2) < 1.)
      {
#ifdef mgDEBUG
        std::cout << "DiMuon has Delta(R) < 1: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    /// Default DiMu Mass rejection
    else
    {
      if( diMuonMass < 5.
        || (diMuonMass>8.9 && diMuonMass<10.6)
        || diMuonMass > 70.)
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    
    
    iVertex = iSelectedVertex;
    indexes = muonVerticesMap[iSelectedVertex];
    
#ifdef mgDEBUG
    std::cout << "\tiVertex = " << iVertex << std::endl;
    for(unsigned int i = 0; i < indexes.size(); i++)
      std::cout << "\tindexes[" << i << "] = " << indexes[i] << std::endl;
#endif // mgDEBUG
    return true;
  }
     
#ifdef mgDEBUG
  std::cout << "SOME CONDITION FAILED! RETURNING FALSE!\n";
#endif // mgDEBUG
  return false;
}
  


/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonV0(std::vector<unsigned short>& indexes, unsigned short& iVertex)
{
  
  return SelectEventWithDiMuonV0("", muVersionString, indexes, iVertex);
  
// #ifdef mgDEBUG
//   std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonV0\n";
// #endif // mgDEBUG
// 
//   // Return false if there are less than 2 muons
//   if(t->numberOfMuon < 2)
//   {
// #ifdef mgDEBUG
//     std::cout << "\tLess than 2 muons: returning false.\n";
// #endif // mgDEBUG
//     return false;
//   }
// 
//   std::vector<unsigned short> vSelectedMuons;
//   std::map<unsigned short, std::vector<unsigned short> > muonVerticesMap;
// 
//   // Loop over muons
//   for(unsigned short i = 0; i < t->numberOfMuon; i++)
//   {
// #ifdef mgDEBUG
//     std::cout << "\tMuon " << i << std::endl;
// #endif // mgDEBUG
// 
//     // Select the muon
//     if(SelectMuon(i))
//     {
// #ifdef mgDEBUG
//       std::cout << "\t\tMuon " << i << " is selected.\n";
// #endif // mgDEBUG
// 
//       // Get also the vertex associated to the muon
//       vSelectedMuons.push_back(i);
//       unsigned short iVtx = SelectVertexNearestToTrack(t->Muon_trackref->at(vSelectedMuons[vSelectedMuons.size()-1]),"V0");
//       if(iVtx != 65535)
//       {
// #ifdef mgDEBUG
//         std::cout << "\t\tGood vertex " << iVtx << " is selected.\n";
// #endif // mgDEBUG
// 
//         // Update the map of associated vertices
//         if(muonVerticesMap.find(iVtx) != muonVerticesMap.end())
//         {
// #ifdef mgDEBUG
//           std::cout << "\t\tVertex " << iVtx << " already in map. Adding muon " << i << " to list.\n";
// #endif // mgDEBUG
// 
//           muonVerticesMap[iVtx].push_back(vSelectedMuons[vSelectedMuons.size()-1]);
//         }
//         else
//         {
// #ifdef mgDEBUG
//           std::cout << "\t\tVertex " << iVtx << " not in map. Creating new vector for muon" << i << std::endl;
// #endif // mgDEBUG
// 
//           std::vector<unsigned short> vMuonIndexes;
//           vMuonIndexes.push_back(vSelectedMuons[vSelectedMuons.size()-1]);
//           muonVerticesMap[iVtx] = vMuonIndexes;
//         }
//       }
// 
// #ifdef mgDEBUG
//       else
//         std::cout << "\t\tNo good vertex found for muon " << i << ".\n";
// #endif // mgDEBUG
//     }
// 
// #ifdef mgDEBUG
//     else
//       std::cout << "\t\tMuon " << i << " is not selected.\n";
// #endif // mgDEBUG
// 
//   } // Loop over muons
// 
//   // Now we have two good muons
//   if(vSelectedMuons.size() < 2)
//   {
// #ifdef mgDEBUG
//     std::cout << "Less than 2 good muons in the event. Returning false.\n";
// #endif // mgDEBUG
//     return false;
//   }
// 
//   bool found = false;
//   unsigned short iSelectedVertex = 65535;
// 
// #ifdef mgDEBUG
//   std::cout << "Checking muon multiplicity of vertices.\n";
// #endif // mgDEBUG
// 
//   // Check muon vertex multiplicities
//   // Loop over vertices in the map
//   for(std::map<unsigned short, std::vector<unsigned short> >::const_iterator it = muonVerticesMap.begin(); it != muonVerticesMap.end(); it++)
//   {
//     // Two vertices with exactly two muons
//     if(found && it->second.size() == 2)
//     {
// #ifdef mgDEBUG
//       std::cout << "\tVertex " << it->first << " is the second dimuon vertex found in the event. Returning false.\n";
// #endif // mgDEBUG
//       return false;
//     }
// 
//     if(it->second.size() == 2)
//     {
// #ifdef mgDEBUG
//       std::cout << "\tVertex " << it->first << " is a good dimuon vertex.\n";
// #endif // mgDEBUG
//       found = true;
//       iSelectedVertex = it->first;
//     }
//   }
//   if(found)
//   {
// #ifdef mgDEBUG
//     std::cout << "Found one and only one good dimuon vertex.\n";
//     std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
//     std::cout << "Now check that the dimuon has a good invariant mass.\n";
// #endif // mgDEBUG
// 
//     unsigned int iMuon1 = muonVerticesMap[iSelectedVertex].at(0);
//     unsigned int iMuon2 = muonVerticesMap[iSelectedVertex].at(1);
// 
// #ifdef mgDEBUG
//     std::cout << "iMuon1 = " << iMuon1 << ", iMuon2 = " << iMuon2 << std::endl;
// #endif // mgDEBUG
// 
//     // Once the muons are selected, check the dimuon mass
//     // if muon mass in the range of a resonance, then return false
//     TLorentzVector pMuon1,pMuon2;
//     pMuon1.SetPtEtaPhiM(t->Muon_pt->at(iMuon1),t->Muon_eta->at(iMuon1),t->Muon_phi->at(iMuon1),constants::muonMass);
//     pMuon2.SetPtEtaPhiM(t->Muon_pt->at(iMuon2),t->Muon_eta->at(iMuon2),t->Muon_phi->at(iMuon2),constants::muonMass);
//     double diMuonMass = (pMuon1 + pMuon2).M();
// 
// #ifdef mgDEBUG
//     std::cout << "diMuonMass = " << diMuonMass << std::endl;
// #endif // mgDEBUG
// 
//     // Mass rejection is different for different muon selection versions
//     if(muVersionString.compare("0") == 0)
//     {
//       if(diMuonMass>70 ||
//          (diMuonMass>0.75 && diMuonMass<0.8) ||
//          (diMuonMass>1 && diMuonMass<1.04) ||
//          (diMuonMass>2.95 && diMuonMass<3.8) ||
//          (diMuonMass>8.9 && diMuonMass<10.6) ||
//          (diMuonMass<0.5))
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
// /*
//     else if(muVersionString.compare("1") == 0 || muVersionString.compare("2") == 0)
//     {
//       if(diMuonMass < 5.
//         || (diMuonMass>8.9 && diMuonMass<10.6)
//         || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
// */
//     else if(muVersionString.compare("3") == 0)
//     {
//       if(diMuonMass<10.6 || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
//     else if(muVersionString.compare("4") == 0)
//     {
//       if(diMuonMass < 5. || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
//     else if(muVersionString.compare("5") == 0)
//     {
//       if( diMuonMass < 5.
//           || (diMuonMass>8.9 && diMuonMass<10.6)
//           || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//       if(pMuon1.DeltaR(pMuon2) < 1.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon has Delta(R) < 1: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
//     /// Default DiMu Mass rejection
//     else
//     {
//       if( diMuonMass < 5.
//           || (diMuonMass>8.9 && diMuonMass<10.6)
//           || diMuonMass > 70.)
//       {
// #ifdef mgDEBUG
//         std::cout << "DiMuon is in a resonance region: rejecting event!\n";
// #endif // mgDEBUG
//         return false;
//       }
//     }
// 
// 
// 
//     iVertex = iSelectedVertex;
//     indexes = muonVerticesMap[iSelectedVertex];
// 
// #ifdef mgDEBUG
//     std::cout << "\tiVertex = " << iVertex << std::endl;
//     for(unsigned int i = 0; i < indexes.size(); i++)
//       std::cout << "\tindexes[" << i << "] = " << indexes[i] << std::endl;
// #endif // mgDEBUG
//     return true;
//   }
// 
// #ifdef mgDEBUG
//   std::cout << "SOME CONDITION FAILED! RETURNING FALSE!\n";
// #endif // mgDEBUG
//   return false;
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonInJetsV0(const std::string muonSelectionType, const std::string jetSelectionType, std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex, std::vector<unsigned short>& vSelectedJets)
{
#ifdef mgDEBUG
  std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonInJetsV0" << endl;
#endif // mgDEBUG

  std::string muonVersion, jetType, jetVersion;
  double muonPtThr, jetPtThr;
  ParseMuonString(muonSelectionType, muonVersion, muonPtThr);
  ParseJetString(jetSelectionType, jetType, jetVersion, jetPtThr);
  
  return SelectEventWithDiMuonInJetsV0(muonSelectionType, muonVersion, jetSelectionType, jetType, jetVersion, vSelectedMuons, iSelectedVertex, vSelectedJets);
  
//   // Select event with dimuons!
//   if(!SelectEventWithDiMuonV0(muonSelectionType, vSelectedMuons, iSelectedVertex))
//     return false;
// 
//   // If the event has dimuons, check if the selected muons are inside jets
//   for(unsigned int iMuon=0; iMuon<vSelectedMuons.size(); iMuon++)
//   {
//     short int iAssociatedJet = -1;
// 
//     int iTrack = t->Muon_trackref->at(vSelectedMuons[iMuon]) ;
//     if( (iTrack < 0) || (iTrack > t->numberOfTrack-1) ) 
//       continue;
// 
//     int totalNumberOfJets = 0;
//     if(jetSelectionType.find("PF") != std::string::npos)
//     {
//       iAssociatedJet = t->Track_jetRef->at( iTrack ) ;
//       totalNumberOfJets = t->numberOfJet;
//     }
//     else if(jetSelectionType.find("Tk") != std::string::npos ||
//             jetSelectionType.find("Trk") != std::string::npos)
//     {
//       iAssociatedJet = t->Track_trackJetRef->at( iTrack ) ;
//       totalNumberOfJets = t->numberOfTrackJet;
//     }
// 
//     // Select the jet associated to the muon, if available
//     if (iAssociatedJet < 0 ||
//         iAssociatedJet > totalNumberOfJets-1 ||
//         !SelectJet(iAssociatedJet,jetSelectionType))
//       return false;
// 
//     // Reject DeltaPt/Pt < 0.3 for PF Jets
//     if(jetSelectionType.find("PFV1") != std::string::npos)
//     {
//       double jetPt = t->Jet_pt->at(iAssociatedJet);
//       double muPt = t->Muon_pt->at(vSelectedMuons[iMuon]);
//       if ((jetPt-muPt)/jetPt < 0.3)
//         return false;
//     }
// 
//     vSelectedJets.push_back(iAssociatedJet);
//   }  
//   if(vSelectedJets[0]!=vSelectedJets[1])
//     return true;
//   else
//   {
// #ifdef mgDEBUG
//     std::cout << "\tSKIPPING selected muons belonging to the same jet!\n";
// #endif // mgDEBUG
//     return false;
//   }
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonInJetsV0(const std::string muonSelectionType, const std::string muonSelectionVersion,
                                                     const std::string jetSelectionType, const std::string jetType,
                                                     const std::string jetVersion,
                                                     std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex,
                                                     std::vector<unsigned short>& vSelectedJets)
{
#ifdef mgDEBUG
  std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonInJetsV0" << endl;
#endif // mgDEBUG
  
  // Select event with dimuons!
  if(!SelectEventWithDiMuonV0(muonSelectionType, muonSelectionVersion, vSelectedMuons, iSelectedVertex))
    return false;
  
  // If the event has dimuons, check if the selected muons are inside jets
  for(unsigned int iMuon=0; iMuon<vSelectedMuons.size(); iMuon++)
  {
    short int iAssociatedJet = -1;
    
    int iTrack = t->Muon_trackref->at(vSelectedMuons[iMuon]) ;
    if( (iTrack < 0) || (iTrack > t->numberOfTrack-1) ) 
      continue;
    
    int totalNumberOfJets = 0;
    if(jetType.compare("PF") == 0)
    {
      iAssociatedJet = t->Track_jetRef->at( iTrack ) ;
      totalNumberOfJets = t->numberOfJet;
    }
    else if(jetType.compare("Tk") == 0 ||
      jetType.compare("Trk") == 0)
    {
      iAssociatedJet = t->Track_trackJetRef->at( iTrack ) ;
      totalNumberOfJets = t->numberOfTrackJet;
    }
    
    // Select the jet associated to the muon, if available
    if (iAssociatedJet < 0 ||
        iAssociatedJet > totalNumberOfJets-1 ||
        !SelectJet(iAssociatedJet,jetSelectionType))
      return false;
        
    // Reject DeltaPt/Pt < 0.3 for PF Jets
    if(jetType.compare("PF") == 0 && jetVersion.compare("1") == 0)
    {
      double jetPt = t->Jet_pt->at(iAssociatedJet);
      double muPt = t->Muon_pt->at(vSelectedMuons[iMuon]);
      if ((jetPt-muPt)/jetPt < 0.3)
        return false;
    }
    
    vSelectedJets.push_back(iAssociatedJet);
  }  
  if(vSelectedJets[0]!=vSelectedJets[1])
    return true;
  else
  {
#ifdef mgDEBUG
    std::cout << "\tSKIPPING selected muons belonging to the same jet!\n";
#endif // mgDEBUG
    return false;
  }
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonInJetsV0(std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex, std::vector<unsigned short>& vSelectedJets)
{
 
  return SelectEventWithDiMuonInJetsV0(muSelectionString, muVersionString, jetSelectionString, jetTypeString, jetVersionString, vSelectedMuons, iSelectedVertex, vSelectedJets);
  
// #ifdef mgDEBUG
//   std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonInJetsV0" << endl;
// #endif // mgDEBUG
// 
//   // Select event with dimuons!
//   if(!SelectEventWithDiMuonV0(vSelectedMuons, iSelectedVertex))
//     return false;
// 
//   // If the event has dimuons, check if the selected muons are inside jets
//   for(unsigned int iMuon=0; iMuon<vSelectedMuons.size(); iMuon++)
//   {
//     short int iAssociatedJet = -1;
// 
//     int iTrack = t->Muon_trackref->at(vSelectedMuons[iMuon]) ;
//     if( (iTrack < 0) || (iTrack > t->numberOfTrack-1) ) 
//       continue;
// 
//     int totalNumberOfJets = 0;
//     if(jetTypeString.find("PF") != std::string::npos)
//     {
//       iAssociatedJet = t->Track_jetRef->at( iTrack ) ;
//       totalNumberOfJets = t->numberOfJet;
//     }
//     else if(jetTypeString.find("Tk") != std::string::npos ||
//             jetTypeString.find("Trk") != std::string::npos)
//     {
//       iAssociatedJet = t->Track_trackJetRef->at( iTrack ) ;
//       totalNumberOfJets = t->numberOfTrackJet;
//     }
// 
//     // Select the jet associated to the muon, if available
//     if (iAssociatedJet < 0 ||
//         iAssociatedJet > totalNumberOfJets-1 ||
//         !SelectJet(iAssociatedJet))
//       return false;
// 
//     // Reject DeltaPt/Pt < 0.3 for PF Jets
//     if(jetTypeString.find("PF") != std::string::npos &&
//        jetVersionString.compare("1") == 0)
//     {
//       double jetPt = t->Jet_pt->at(iAssociatedJet);
//       double muPt = t->Muon_pt->at(vSelectedMuons[iMuon]);
//       if ((jetPt-muPt)/jetPt < 0.3)        return false;
//     }
// 
//     vSelectedJets.push_back(iAssociatedJet);
//   }  
//   if(vSelectedJets[0]!=vSelectedJets[1])
//     return true;
//   else
//   {
// #ifdef mgDEBUG
//     std::cout << "\tSKIPPING selected muons belonging to the same jet!\n";
// #endif // mgDEBUG
//     return false;
//   }
}




/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonResonanceV0(const std::string selectionType, std::vector<unsigned short>& indexes, unsigned short& iVertex)
{
  #ifdef mgDEBUG
  std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonResonanceV0\n";
  #endif // mgDEBUG
  if(t->numberOfMuon < 2)
  {
#ifdef mgDEBUG
    std::cout << "\tLess than 2 muons: returning false.\n";
#endif // mgDEBUG
    return false;
  }
  std::vector<unsigned short> vSelectedMuons;
  std::map<unsigned short, std::vector<unsigned short> > muonVerticesMap;
  for(unsigned short i = 0; i < t->numberOfMuon; i++)
  {
#ifdef mgDEBUG
    std::cout << "\tMuon " << i << std::endl;
#endif // mgDEBUG
    if(SelectMuon(i,selectionType))
    {
#ifdef mgDEBUG
      std::cout << "\t\tMuon " << i << " is selected.\n";
#endif // mgDEBUG
      vSelectedMuons.push_back(i);
      unsigned short iVtx = SelectVertexNearestToTrack(t->Muon_trackref->at(vSelectedMuons[vSelectedMuons.size()-1]),"V0");
      if(iVtx != 65535)
      {
#ifdef mgDEBUG
        std::cout << "\t\tGood vertex " << iVtx << " is selected.\n";
#endif // mgDEBUG
        if(muonVerticesMap.find(iVtx) != muonVerticesMap.end())
        {
#ifdef mgDEBUG
          std::cout << "\t\tVertex " << iVtx << " already in map. Adding muon " << i << " to list.\n";
#endif // mgDEBUG
          muonVerticesMap[iVtx].push_back(vSelectedMuons[vSelectedMuons.size()-1]);
        }
        else
        {
#ifdef mgDEBUG
          std::cout << "\t\tVertex " << iVtx << " not in map. Creating new vector for muon" << i << std::endl;
#endif // mgDEBUG
          std::vector<unsigned short> vMuonIndexes;
          vMuonIndexes.push_back(vSelectedMuons[vSelectedMuons.size()-1]);
          muonVerticesMap[iVtx] = vMuonIndexes;
        }
      }
#ifdef mgDEBUG
      else
        std::cout << "\t\tNo good vertex found for muon " << i << ".\n";
#endif // mgDEBUG
    }
#ifdef mgDEBUG
    else
      std::cout << "\t\tMuon " << i << " is not selected.\n";
#endif // mgDEBUG
  }
  if(vSelectedMuons.size() < 2)
  {
#ifdef mgDEBUG
    std::cout << "Less than 2 good muons in the event. Returning false.\n";
#endif // mgDEBUG
    return false;
  }
  bool found = false;
  unsigned short iSelectedVertex = 65535;
#ifdef mgDEBUG
  std::cout << "Checking muon multiplicity of vertices.\n";
#endif // mgDEBUG
  for(std::map<unsigned short, std::vector<unsigned short> >::const_iterator it = muonVerticesMap.begin(); it != muonVerticesMap.end(); it++)
  {
    // Two vertices with exactly two muons
    if(found && it->second.size() == 2)
    {
#ifdef mgDEBUG
      std::cout << "\tVertex " << it->first << " is the second dimuon vertex found in the event. Returning false.\n";
#endif // mgDEBUG
      return false;
    }
    if(it->second.size() == 2)
    {
#ifdef mgDEBUG
      std::cout << "\tVertex " << it->first << " is a good dimuon vertex.\n";
#endif // mgDEBUG
      found = true;
      iSelectedVertex = it->first;
//       std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
    }
  }
  if(found)
  {
#ifdef mgDEBUG
    std::cout << "Found one and only one good dimuon vertex.\n";
    std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
#endif // mgDEBUG
    iVertex = iSelectedVertex;
    indexes = muonVerticesMap[iSelectedVertex];
#ifdef mgDEBUG
    std::cout << "\tiVertex = " << iVertex << std::endl;
    for(unsigned int i = 0; i < indexes.size(); i++)
      std::cout << "\tindexes[" << i << "] = " << indexes[i] << std::endl;
#endif // mgDEBUG
      return true;
  }
#ifdef mgDEBUG
  std::cout << "SOME CONDITION FAILED! RETURNING FALSE!\n";
#endif // mgDEBUG
  return false;
}



bool PARTICLESELECTOR::SelectEventWithDiMuonResonanceV0(std::vector<unsigned short>& indexes, unsigned short& iVertex)
{
  return SelectEventWithDiMuonResonanceV0(muSelectionString, indexes, iVertex);
  
//   #ifdef mgDEBUG
//   std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonResonanceV0\n";
//   #endif // mgDEBUG
//   if(t->numberOfMuon < 2)
//   {
// #ifdef mgDEBUG
//     std::cout << "\tLess than 2 muons: returning false.\n";
// #endif // mgDEBUG
//     return false;
//   }
//   std::vector<unsigned short> vSelectedMuons;
//   std::map<unsigned short, std::vector<unsigned short> > muonVerticesMap;
//   for(unsigned short i = 0; i < t->numberOfMuon; i++)
//   {
// #ifdef mgDEBUG
//     std::cout << "\tMuon " << i << std::endl;
// #endif // mgDEBUG
//     if(SelectMuon(i))
//     {
// #ifdef mgDEBUG
//       std::cout << "\t\tMuon " << i << " is selected.\n";
// #endif // mgDEBUG
//       vSelectedMuons.push_back(i);
//       unsigned short iVtx = SelectVertexNearestToTrack(t->Muon_trackref->at(vSelectedMuons[vSelectedMuons.size()-1]),"V0");
//       if(iVtx != 65535)
//       {
// #ifdef mgDEBUG
//         std::cout << "\t\tGood vertex " << iVtx << " is selected.\n";
// #endif // mgDEBUG
//         if(muonVerticesMap.find(iVtx) != muonVerticesMap.end())
//         {
// #ifdef mgDEBUG
//           std::cout << "\t\tVertex " << iVtx << " already in map. Adding muon " << i << " to list.\n";
// #endif // mgDEBUG
//           muonVerticesMap[iVtx].push_back(vSelectedMuons[vSelectedMuons.size()-1]);
//         }
//         else
//         {
// #ifdef mgDEBUG
//           std::cout << "\t\tVertex " << iVtx << " not in map. Creating new vector for muon" << i << std::endl;
// #endif // mgDEBUG
//           std::vector<unsigned short> vMuonIndexes;
//           vMuonIndexes.push_back(vSelectedMuons[vSelectedMuons.size()-1]);
//           muonVerticesMap[iVtx] = vMuonIndexes;
//         }
//       }
// #ifdef mgDEBUG
//       else
//         std::cout << "\t\tNo good vertex found for muon " << i << ".\n";
// #endif // mgDEBUG
//     }
// #ifdef mgDEBUG
//     else
//       std::cout << "\t\tMuon " << i << " is not selected.\n";
// #endif // mgDEBUG
//   }
//   if(vSelectedMuons.size() < 2)
//   {
// #ifdef mgDEBUG
//     std::cout << "Less than 2 good muons in the event. Returning false.\n";
// #endif // mgDEBUG
//     return false;
//   }
//   bool found = false;
//   unsigned short iSelectedVertex = 65535;
// #ifdef mgDEBUG
//   std::cout << "Checking muon multiplicity of vertices.\n";
// #endif // mgDEBUG
//   for(std::map<unsigned short, std::vector<unsigned short> >::const_iterator it = muonVerticesMap.begin(); it != muonVerticesMap.end(); it++)
//   {
//     // Two vertices with exactly two muons
//     if(found && it->second.size() == 2)
//     {
// #ifdef mgDEBUG
//       std::cout << "\tVertex " << it->first << " is the second dimuon vertex found in the event. Returning false.\n";
// #endif // mgDEBUG
//       return false;
//     }
//     if(it->second.size() == 2)
//     {
// #ifdef mgDEBUG
//       std::cout << "\tVertex " << it->first << " is a good dimuon vertex.\n";
// #endif // mgDEBUG
//       found = true;
//       iSelectedVertex = it->first;
// //       std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
//     }
//   }
//   if(found)
//   {
// #ifdef mgDEBUG
//     std::cout << "Found one and only one good dimuon vertex.\n";
//     std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
// #endif // mgDEBUG
//     iVertex = iSelectedVertex;
//     indexes = muonVerticesMap[iSelectedVertex];
// #ifdef mgDEBUG
//     std::cout << "\tiVertex = " << iVertex << std::endl;
//     for(unsigned int i = 0; i < indexes.size(); i++)
//       std::cout << "\tindexes[" << i << "] = " << indexes[i] << std::endl;
// #endif // mgDEBUG
//       return true;
//   }
// #ifdef mgDEBUG
//   std::cout << "SOME CONDITION FAILED! RETURNING FALSE!\n";
// #endif // mgDEBUG
//   return false;
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonResonanceInJetsV0(const std::string muonSelectionType, const std::string jetSelectionType, std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex, std::vector<unsigned short>& vSelectedJets)
{
#ifdef mgDEBUG
  std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonResonanceInJetsV0" << endl;
#endif // mgDEBUG
  
  std::string muonVersion, jetType, jetVersion;
  double muonPtThr, jetPtThr;
  ParseMuonString(muonSelectionType, muonVersion, muonPtThr);
  ParseJetString(jetSelectionType, jetType, jetVersion, jetPtThr);
  
  return SelectEventWithDiMuonResonanceInJetsV0(muonSelectionType, jetSelectionType, jetType, jetVersion, vSelectedMuons, iSelectedVertex, vSelectedJets);
}






/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonResonanceInJetsV0(const std::string muonSelectionType,
                                                              const std::string jetSelectionType, const std::string jetType,
                                                              const std::string jetVersion,
                                                              std::vector<unsigned short>& vSelectedMuons,
                                                              unsigned short& iSelectedVertex,
                                                              std::vector<unsigned short>& vSelectedJets)
{
#ifdef mgDEBUG
  std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonResonanceInJetsV0" << endl;
#endif // mgDEBUG
  
  // Select event with dimuons!
  if(!SelectEventWithDiMuonResonanceV0(muonSelectionType, vSelectedMuons, iSelectedVertex))
    return false;
  
  // If the event has dimuons, check if the selected muons are inside jets
  for(unsigned int iMuon=0; iMuon<vSelectedMuons.size(); iMuon++)
  {
    short int iAssociatedJet = -1;
    
    int iTrack = t->Muon_trackref->at(vSelectedMuons[iMuon]) ;
    if( (iTrack < 0) || (iTrack > t->numberOfTrack-1) ) 
      continue;
    
    int totalNumberOfJets = 0;
    if(jetType.compare("PF") == 0)
    {
      iAssociatedJet = t->Track_jetRef->at( iTrack ) ;
      totalNumberOfJets = t->numberOfJet;
    }
    else if(jetType.compare("Tk") == 0 ||
            jetType.compare("Trk") == 0)
    {
      iAssociatedJet = t->Track_trackJetRef->at( iTrack ) ;
      totalNumberOfJets = t->numberOfTrackJet;
    }
      
    // Select the jet associated to the muon, if available
    if (iAssociatedJet < 0 ||
        iAssociatedJet > totalNumberOfJets-1 ||
        !SelectJet(iAssociatedJet,jetSelectionType))
      return false;
    
    // Reject DeltaPt/Pt < 0.3 for PF Jets
    if(jetType.compare("PF") == 0 && jetVersion.compare("1") == 0)
    {
      double jetPt = t->Jet_pt->at(iAssociatedJet);
      double muPt = t->Muon_pt->at(vSelectedMuons[iMuon]);
      if ((jetPt-muPt)/jetPt < 0.3)
        return false;
    }
    
    vSelectedJets.push_back(iAssociatedJet);
  }
  return true;
}



/**************************************************************************************/
bool PARTICLESELECTOR::SelectEventWithDiMuonResonanceInJetsV0(std::vector<unsigned short>& vSelectedMuons, unsigned short& iSelectedVertex, std::vector<unsigned short>& vSelectedJets)
{
  
  return SelectEventWithDiMuonResonanceInJetsV0(muSelectionString, jetSelectionString, jetTypeString, jetVersionString, vSelectedMuons, iSelectedVertex, vSelectedJets);
}



bool PARTICLESELECTOR::SelectEventWithDiMuonV1(const std::string selectionType, std::vector<unsigned short>& indexes, unsigned short& iVertex)
{
  #ifdef mgDEBUG
  std::cout << "Inside PARTICLESELECTOR::SelectEventWithDiMuonV1\n";
  #endif // mgDEBUG
  if(t->numberOfMuon < 2)
  {
    #ifdef mgDEBUG
    std::cout << "\tLess than 2 muons: returning false.\n";
    #endif // mgDEBUG
    return false;
  }
// #if NTUPLE_VERSION >= 3
// #ifdef mgDEBUG
//   std::cout << "\tChecking trigger condition.\n";
// #endif // mgDEBUG
//   if(!SelectEventPassingTrigger("HLT_DoubleMu3") && !SelectEventPassingTrigger("HLT_DoubleMu3_v2"))
//   {
//     return false;
//   }
// #endif // NTUPLE_VERSION >= 3
  std::vector<unsigned short> vSelectedMuons;
  std::map<unsigned short, std::vector<unsigned short> > muonVerticesMap;
  for(unsigned short i = 0; i < t->numberOfMuon; i++)
  {
#ifdef mgDEBUG
    std::cout << "\tMuon " << i << std::endl;
#endif // mgDEBUG
    if(SelectMuon(i,selectionType))
    {
#ifdef mgDEBUG
      std::cout << "\t\tMuon " << i << " is selected.\n";
#endif // mgDEBUG
      vSelectedMuons.push_back(i);
      unsigned short iVtx = SelectVertexNearestToTrack(t->Muon_trackref->at(vSelectedMuons[vSelectedMuons.size()-1]),"V0");
      if(iVtx != 65535)
      {
#ifdef mgDEBUG
        std::cout << "\t\tGood vertex " << iVtx << " is selected.\n";
#endif // mgDEBUG
        if(muonVerticesMap.find(iVtx) != muonVerticesMap.end())
        {
#ifdef mgDEBUG
          std::cout << "\t\tVertex " << iVtx << " already in map. Adding muon " << i << " to list.\n";
#endif // mgDEBUG
          muonVerticesMap[iVtx].push_back(vSelectedMuons[vSelectedMuons.size()-1]);
        }
        else
        {
#ifdef mgDEBUG
          std::cout << "\t\tVertex " << iVtx << " not in map. Creating new vector for muon" << i << std::endl;
#endif // mgDEBUG
          std::vector<unsigned short> vMuonIndexes;
          vMuonIndexes.push_back(vSelectedMuons[vSelectedMuons.size()-1]);
          muonVerticesMap[iVtx] = vMuonIndexes;
        }
      }
#ifdef mgDEBUG
      else
        std::cout << "\t\tNo good vertex found for muon " << i << ".\n";
#endif // mgDEBUG
    }
#ifdef mgDEBUG
    else
      std::cout << "\t\tMuon " << i << " is not selected.\n";
#endif // mgDEBUG
  }
  if(vSelectedMuons.size() < 2)
  {
#ifdef mgDEBUG
    std::cout << "Less than 2 good muons in the event. Returning false.\n";
#endif // mgDEBUG
    return false;
  }
//   std::map<unsigned short, std::vector<unsigned int> > muonVerticesMap;
//   for(unsigned int i = 0; i < vSelectedMuons.size(); i++)
//   {
//     iVtx = SelectVertexNearestToTrack(t->Muon_trackref->at(vSelectedMuons[i]),"tightBSMinDz"));
//     if(iVtx != 65535)
//     {
//       if(muonVerticesMap.find(iVtx) != muonVerticesMap.size())
//       {
//         muonVerticesMap[iVtx].second.push_back(vSelectedMuons[i]);
//       }
//       else
//       {
//         std::vector<unsigned short> vMuonIndexes;
//         vMuonIndexes.push_back(vSelectedMuons[i]);
//         muonVerticesMap[iVtx].second = vMuonIndexes;
//       }
//     }
//   }
  bool found = false;
  unsigned short iSelectedVertex = 65535;
  double ptLeadingMuon = 0;
#ifdef mgDEBUG
  std::cout << "Checking muon multiplicity of vertices.\n";
#endif // mgDEBUG
  for(std::map<unsigned short, std::vector<unsigned short> >::const_iterator it = muonVerticesMap.begin(); it != muonVerticesMap.end(); it++)
  {
    // Two vertices with exactly two muons
    if(found && it->second.size() >= 2)
    {
#ifdef mgDEBUG
      std::cout << "\tVertex " << it->first << " is the second dimuon vertex found in the event. Checking which one has higher-pt muons.\n";
#endif // mgDEBUG
      for(unsigned int i = 0; i < it->second.size(); i++)
      {
        double pt = t->Muon_pt->at(it->second.at(i));
        if(pt > ptLeadingMuon)
        {
#ifdef mgDEBUG
          std::cout  << "Found a muon in vertex " << it->first << " having higher pt that the previous saved one. This vertex becomes the selected one.\n";
#endif // mgDEBUG
          ptLeadingMuon = pt;
          iSelectedVertex = it->first;
          break;
        }
      }
    }
    if(!found && it->second.size() >= 2)
    {
#ifdef mgDEBUG
      std::cout << "\tVertex " << it->first << " is a good dimuon vertex.\n";
#endif // mgDEBUG
      found = true;
      iSelectedVertex = it->first;
      double ptMax = 0;
      for(unsigned int i = 0; i < it->second.size(); i++)
      {
        double pt = t->Muon_pt->at(it->second.at(i));
        if(pt > ptMax)
          ptMax = pt;
      }
      ptLeadingMuon = ptMax;
//       std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
    }
  }
  if(found)
  {
#ifdef mgDEBUG
    std::cout << "Found at least one good dimuon vertex.\n";
    std::cout << "iSelectedVertex = " << iSelectedVertex << std::endl;
    std::cout << "Now check that the dimuon has a good invariant mass.\n";
    for(unsigned int i = 0; i < muonVerticesMap[iSelectedVertex].size(); i++)
    {
      std::cout << "Muon " << i << " pt = " << t->Muon_pt->at(i) << std::endl;
    }
#endif // mgDEBUG
    
    unsigned int iMuon1 = muonVerticesMap[iSelectedVertex].at(0);
    unsigned int iMuon2 = muonVerticesMap[iSelectedVertex].at(1);
#ifdef mgDEBUG
    std::cout << "iMuon1 = " << iMuon1 << ", iMuon2 = " << iMuon2 << std::endl;
#endif // mgDEBUG
    TLorentzVector pMuon1,pMuon2;
    pMuon1.SetPtEtaPhiM(t->Muon_pt->at(iMuon1),t->Muon_eta->at(iMuon1),t->Muon_phi->at(iMuon1),constants::muonMass);
    pMuon2.SetPtEtaPhiM(t->Muon_pt->at(iMuon2),t->Muon_eta->at(iMuon2),t->Muon_phi->at(iMuon2),constants::muonMass);
    double diMuonMass = (pMuon1 + pMuon2).M();
// if muon mass in the range of a resonance, then return false
#ifdef mgDEBUG
    std::cout << "diMuonMass = " << diMuonMass << std::endl;
#endif // mgDEBUG
    if(selectionType.find("V0") != std::string::npos)
    {
      if(diMuonMass>70
        || (diMuonMass>0.75 && diMuonMass<0.8)
        || (diMuonMass>1 && diMuonMass<1.04)
        || (diMuonMass>2.95 && diMuonMass<3.8)
        || (diMuonMass>8.9 && diMuonMass<10.6)
        || (diMuonMass<0.5))
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    else if(selectionType.find("V1") != std::string::npos || selectionType.find("V2") != std::string::npos)
    {
      if(diMuonMass < 5.
        || (diMuonMass>8.9 && diMuonMass<10.6)
        || diMuonMass > 70.)
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    else if(selectionType.find("V3") != std::string::npos)
    {
      if(diMuonMass<10.6 || diMuonMass > 70.)
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    else if(selectionType.find("V4") != std::string::npos)
    {
      if(diMuonMass < 5. || diMuonMass > 70.)
      {
#ifdef mgDEBUG
        std::cout << "DiMuon is in a resonance region: rejecting event!\n";
#endif // mgDEBUG
        return false;
      }
    }
    
    iVertex = iSelectedVertex;
    std::vector<unsigned short> twoMuonsIndexes;
    for(unsigned int i = 0; i < 2; i++)
    {
      twoMuonsIndexes.push_back(muonVerticesMap[iSelectedVertex].at(i));
    }
    indexes = twoMuonsIndexes;
#ifdef mgDEBUG
    std::cout << "\tiVertex = " << iVertex << std::endl;
    for(unsigned int i = 0; i < indexes.size(); i++)
      std::cout << "\tindexes[" << i << "] = " << indexes[i] << std::endl;
#endif // mgDEBUG
      return true;
  }
#ifdef mgDEBUG
  std::cout << "SOME CONDITION FAILED! RETURNING FALSE!\n";
#endif // mgDEBUG
  return false;
}



#ifdef USING_MC
bool PARTICLESELECTOR::SelectGenp(const unsigned int iGenp, const int particleId, const bool absPid, const bool stableOnly)
{
  bool isStable = stableOnly ? (t->Genp_status->at(iGenp) == 1 || t->Genp_status->at(iGenp) == 8) : true;
  bool isPid = absPid ? (abs(t->Genp_Id->at(iGenp)) == abs(particleId)) : (t->Genp_Id->at(iGenp) == particleId);

  return isStable && isPid;
}
#endif // USING_MC




















///////////////////////////////////////////////////////////////
// OLD SELECTORS TO BE REMOVED AT THE SECOND ONLINE COMMITTMENT

bool PARTICLESELECTOR::SelectEvent(const std::string selectionType)
{
  if(selectionType.compare("atLeastTwoTightMuons") == 0)
  {
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tight"))
      {
        selectedMuons++;
      }
      if(selectedMuons==2)
      {
        return true;
      }
    }
  }
  return false;
}



bool PARTICLESELECTOR::SelectEventAndReturnIndexes(const std::string selectionType, std::vector<unsigned int>& indexes)
{
  indexes.clear();
  if(selectionType.compare("twoTrackerMuons") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tracker"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2)
      return true;
  }
  else if(selectionType.compare("twoTightCompatMuons") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tight_compat"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2)
      return true;
  }
  else if(selectionType.compare("twoTightMuons") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tight"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2)
      return true;
  }
  else if(selectionType.compare("twoTightV4Muons") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tightV4"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2)
      return true;
  }
  else if(selectionType.compare("twoTightV5Muons") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tightV5"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2)
      return true;
  }
  else if(selectionType.compare("twoTightV6Muons") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tightV6"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2)
      return true;
  }
  else if(selectionType.compare("twoTightV6MuonsSameVtx") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tightV6"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2){
      int iVtx1 = SelectVertexNearestToTrack(t->Muon_trackref->at(indexes[0]),"tightMinDz");
      int iVtx2 = SelectVertexNearestToTrack(t->Muon_trackref->at(indexes[1]),"tightMinDz");
      if(iVtx1 == iVtx2 && iVtx1 != 65535)
        return true;
      else
        return false;
    }
  }
  else if(selectionType.compare("twoTightV7MuonsSameVtx") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tightV7"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2){
      int iVtx1 = SelectVertexNearestToTrack(t->Muon_trackref->at(indexes[0]),"tightBSMinDz");
      int iVtx2 = SelectVertexNearestToTrack(t->Muon_trackref->at(indexes[1]),"tightBSMinDz");
      if(iVtx1 == iVtx2 && iVtx1 != 65535)
        return true;
      else
        return false;
    }
  }
  else if(selectionType.compare("twoTightV8MuonsSameVtx") == 0)
  {
    // At least two reco muons to start with
    if(t->Muon_isTracker->size()<2)
    {
      return false;
    }
    unsigned short int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tightV8"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
      if(selectedMuons>2)
      {
        return false;
      }
    }
    if(selectedMuons==2){
      int iVtx1 = SelectVertexNearestToTrack(t->Muon_trackref->at(indexes[0]),"tightBSMinDz");
      int iVtx2 = SelectVertexNearestToTrack(t->Muon_trackref->at(indexes[1]),"tightBSMinDz");
      if(iVtx1 == iVtx2 && iVtx1 != 65535)
        return true;
      else
        return false;
    }
  }
  else if(selectionType.compare("atLeastOneTightBPHMuon") == 0)
  {
    // At least one reco muons to start with
    if(t->Muon_isTracker->size()<1)
      return false;
    unsigned int selectedMuons = 0;
    for(unsigned int i = 0; i < t->Muon_isTracker->size(); i++)
    {
      if(SelectMuon(i,"tightBPH"))
      {
        selectedMuons++;
        indexes.push_back(i);
      }
    }
    if(selectedMuons>0)
      return true;
  }
  return false;
}



double PARTICLESELECTOR::FindMuonMVAValueFromEfficiency(const std::string mvaFileName, const std::string method, const double efficiency)
{
  if (efficiency < 0. || efficiency > 1.)
  {
    std::cout << "E R R O R ! PARTICLESELECTOR::FindMuonMVAValueFromEfficiency : cut must be in [0,1]!" << std::endl;
    std::cout << "            Exiting...\n";
    exit(1);
  }
  std::cout << "Sono qui 1 \n";

  TFile* fTemp = TFile::Open(mvaFileName.c_str());
  std::cout << "Sono qui 2 \n";
  
  TH1D* output = (TH1D*)fTemp->Get(method.c_str());
  std::cout << "Sono qui 3 \n";
  
  int bins = output->GetNbinsX();
  std::cout << "Sono qui 3.1 \n";
  
  int bin = -1;
  double total = output->Integral();
  for (int i = bins; i >= 0; i--)
  {
    double integral = output->Integral(i, bins);
    if (integral / total > efficiency)
    {
      bin = i;
      break;
    }
  }
  std::cout << "Sono qui 4 \n";
  
  double cut = output->GetBinCenter(bin);
  std::cout << "Sono qui 5 \n";
  // #ifdef mgDEBUG
  std::cout << "I N F O : PARTICLESELECTOR::FindMuonMVAValueFromEfficiency : for efficiency = " << efficiency << " the MVA cut is = " << cut << std::endl;
// #endif //mgDEBUG
  std::cout << "Sono qui 6 \n";
  
  fTemp->Close();
  std::cout << "Sono qui 7 \n";
  
  return cut;
}



void PARTICLESELECTOR::InitializeMuonTMVA(const std::string muonSelection, TMVA::Reader*& reader, float& cut)
{
  if(muonSelection.compare("3TMVABmm") == 0 || muonSelection.compare("4TMVABmm") == 0 || muonSelection.compare("5TMVABmm") == 0)
  {
    std::cout << "I N F O : PARTICLESELECTOR::InitializeMuonTMVA(...) - Initializing TMVAReader for muons...\n";
    std::cout << "          Recognized muon selector from command line: \"" << muonSelection << "\", adding variables to the TMVAReader...\n";
    reader = new TMVA::Reader("V:Color:Silent");
    reader->AddVariable("trkValidFract", &muonTMVATrackValidFraction);
    reader->AddVariable("glbNChi2", &muonTMVAGlobalChi2);
    reader->AddVariable("pt", &muonTMVAPt);
    reader->AddVariable("eta", &muonTMVAEta);
    reader->AddVariable("segComp", &muonTMVASegmentCompatibility);
    reader->AddVariable("chi2LocMom", &muonTMVAChi2LocalMomentum);
    reader->AddVariable("chi2LocPos", &muonTMVAChi2LocalPosition);
    reader->AddVariable("glbTrackProb", &muonTMVAGlobalTrackProbability);
    reader->AddVariable("NTrkVHits", &muonTMVATrackNumberOfValidHits);
    reader->AddVariable("NTrkEHitsOut", &muonTMVATrackExpectedHitsOuter);
    std::cout << "          Checking weight file: \"../tmvaWeightFiles/TMVAClassification_BDT.weights.130507v1.xml\" for TMVA method \"BDT\"...\n";
    if(!reader->FindMVA("BDT"))
      reader->BookMVA("BDT", "../tmvaWeightFiles/TMVAClassification_BDT.weights.130507v1.xml");
    // Cut value from L. Martini. This corresponds to an efficiency for real muons equal to 0.9
    if(muonSelection.compare("3TMVABmm") == 0 || muonSelection.compare("4TMVABmm") == 0)
    {
      std::cout << "          Initializing BDT cut for 90% muon efficiency. BDT cut set at 0.360.\n";
      cut = 0.360;
    }
    if(muonSelection.compare("5TMVABmm") == 0)
    {
      std::cout << "          Initializing BDT cut to match Urs' selection. BDT cut set at 0.3.\n";
      std::cout << "W A R N I N G ! This selector is BUGGY, use it at your own risk! It must only be used if you KNOW what you are doing!\n";
      std::cout << "                If you do not have a specific reason to use it, please KILL NOW the job.\n";
      sleep(10);
      cut = 0.3;
    }
  }
  else if(muonSelection.compare("2TMVABmm") == 0 || muonSelection.compare("1TMVABmm") == 0)
  {
    std::cout << "I N F O : PARTICLESELECTOR::InitializeMuonTMVA(...) - Initializing TMVAReader for muons...\n";
    std::cout << "          Recognized muon selector from command line: \"" << muonSelection << "\", adding variables to the TMVAReader...\n";
    reader = new TMVA::Reader("V:Color:Silent");
    reader->AddVariable("trkValidFract", &muonTMVATrackValidFraction);
    reader->AddVariable("glbNChi2", &muonTMVAGlobalChi2);
    reader->AddVariable("pt", &muonTMVAPt);
    reader->AddVariable("eta", &muonTMVAEta);
    reader->AddVariable("segComp", &muonTMVASegmentCompatibility);
    reader->AddVariable("chi2LocMom", &muonTMVAChi2LocalMomentum);
    reader->AddVariable("chi2LocPos", &muonTMVAChi2LocalPosition);
    reader->AddVariable("glbTrackProb", &muonTMVAGlobalTrackProbability);
    reader->AddVariable("NTrkVHits", &muonTMVATrackNumberOfValidHits);
    reader->AddVariable("NTrkEHitsOut", &muonTMVATrackExpectedHitsOuter);
    std::cout << "          Checking weight file: \"../tmvaWeightFiles/TMVAClassification_BDT.weights.130410v1.xml\" for TMVA method \"BDT\"...\n";
    if(!reader->FindMVA("BDT"))
      reader->BookMVA("BDT", "../tmvaWeightFiles/TMVAClassification_BDT.weights.130410v1.xml");
    // Cut value from L. Martini. This corresponds to an efficiency for real muons equal to 0.9
    std::cout << "          Initializing BDT cut for 90% muon efficiency. BDT cut set at 0.3.\n";
    cut = 0.3;
  }
}


#endif // PARTICLESELECTOR_cxx
