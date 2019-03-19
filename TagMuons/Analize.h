#ifndef Analize_h
#define Analize_h

#include <TLorentzVector.h>
#include <TStopwatch.h>
#include <TH3D.h>
#include <iostream>
#include <vector>

#include "TagMuons.h"

class Analize : public TagMuons
{
  public:
    Analize(const std::string sampleName_, const std::string text_, const int maxEvents_ = -1, TTree* tree=0) :
      TagMuons(sampleName_, text_, maxEvents_, tree) {};
    virtual void BeginJob() ;
    virtual void EndJob( int ) ;
    virtual void AnalizeEvent( int ) ;
    virtual void StatusReport( int ) ;
    int GetMuonClass( unsigned short int iMuon )  ;
    int GetCombinedClass( int i, int j )  ;
    TLorentzVector MakeTLorentzVector( double pt, double eta, double phi, double m ) ;

  private:
    // Nothing by default
    // Add your private variables/methods here

    TStopwatch _timer ;
    double _tCpu, _tReal ;
};

#endif // Analize_h
