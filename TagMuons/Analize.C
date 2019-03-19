#ifndef Analize_cxx
#define Analize_cxx

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 3

#include "Analize.h"
#include "../utilities/constants.h"
// ======================================================================

const double ptmax(4.0) ;
TH1D *CfrCmb, *CfrSj ;
TH2D *Mmult, *CfrAll ;
TH3D *Ptout, *PtoutSC, *PtoutOC ;
TH3D *PtoutSameB, *PtoutSameBSC, *PtoutSameBOC ;

// ======================================================================

void Analize::StatusReport( int entry ) 
{
 _timer.Stop() ; 
 _tCpu  += _timer.CpuTime() ;
 _tReal += _timer.RealTime() ;
 printf( " At Event %8d  Real Time %6.2f  Total Time %8.2f  CPU Time %8.2f \n", 
	 (int) entry, _timer.RealTime(), _tReal, _tCpu ) ;
 _timer.Start() ;
}
// ======================================================================
void Analize::EndJob( int entry ) 
{
 _timer.Stop() ; 
 _tCpu  += _timer.CpuTime() ;
 _tReal += _timer.RealTime() ;
 printf( " End Run At Event %8d  Real Time %6.2f  Total Time %8.2f  CPU Time %8.2f \n", 
	 (int) entry, _timer.RealTime(), _tReal, _tCpu ) ;
} 
// ======================================================================
void Analize::BeginJob() 
{
  _timer.Start() ;
  _tCpu = _tReal = 0 ;

  CfrSj = new TH1D("CfrSj","CfrSj",11,-0.5,10.5);
  CfrCmb = new TH1D("CfrCmb","CfrCmb",11,-0.5,10.5);

  Mmult = new TH2D("Mmult","Mmult",10,0.,10.,10,0.,10.) ;
  CfrAll = new TH2D("CfrAll","CfrAll",11,-0.5,10.5,11,-0.5,10.5) ;

  Ptout = new TH3D("Ptout","Ptout",10,-0.5,9.5,50,0.,ptmax,50,0.,ptmax) ;
  PtoutSC = new TH3D("PtoutSC","PtoutSC",10,-0.5,9.5,50,0.,ptmax,50,0.,ptmax) ;
  PtoutOC = new TH3D("PtoutOC","PtoutOC",10,-0.5,9.5,50,0.,ptmax,50,0.,ptmax) ;

  Ptout->Sumw2() ;
  PtoutSC->Sumw2() ;
  PtoutOC->Sumw2() ;

  PtoutSameB = new TH3D("PtoutSameB","PtoutSameB",10,-0.5,9.5,50,0.,ptmax,50,0.,ptmax) ;
  PtoutSameBSC = new TH3D("PtoutSameBSC","PtoutSameBSC",10,-0.5,9.5,50,0.,ptmax,50,0.,ptmax) ;
  PtoutSameBOC = new TH3D("PtoutSameBOC","PtoutSameBOC",10,-0.5,9.5,50,0.,ptmax,50,0.,ptmax) ;

  PtoutSameB->Sumw2() ;
  PtoutSameBSC->Sumw2() ;
  PtoutSameBOC->Sumw2() ;

  
} 
// ======================================================================
void Analize::AnalizeEvent( int entry ) 
{
  if( (entry<99 && entry%10 == 0) ||
    (entry<999 && entry%100 == 0) ||
    (entry<9999 && entry%1000 == 0) ||
      (entry<99999 && entry%10000 == 0) || (entry%100000 == 0) ) StatusReport( entry ) ;

  unsigned short iSelectedVertex ;
  vector<unsigned short> vSelectedMuons,  vSelectedJets ;

  if( !s->SelectEventWithDiMuonResonanceInJetsV0( vSelectedMuons, iSelectedVertex, vSelectedJets )) return ;
  if( vSelectedMuons.size()<2 ) return ;


  bool isodd = ( entry%2 == 0 ) ;

  vector <unsigned short> MuonWithJet ;
  vector <unsigned short> JetWithMuon ;
  vector <int> MuonClass ;
  vector <double> ptoutMu ;
  vector <TLorentzVector> JtTLV ; 
  vector <TLorentzVector> MuTLV ; 
  
  for( unsigned short int k=0 ; k< (unsigned int ) vSelectedMuons.size() ; k++ ) 
    {
      unsigned short iMu = vSelectedMuons.at(k) ;

      int i = GetMuonClass(iMu) ;      

      if( Muon_pt->at(iMu) < 4 ) continue ;
      if( Muon_eta->at(iMu) > 2.1 ) continue ;
      if( Muon_eta->at(iMu) <-2.1 ) continue ;
      
      int itk = Muon_trackref->at(iMu) ;
      if( itk < 0 || itk > numberOfTrack-1 ) continue;
      
      unsigned short jet = Track_jetRef->at( itk ) ;
      int ijet = (int) jet ;
      if( ijet > numberOfJet-1 ) continue ;

      TLorentzVector jttlv = MakeTLorentzVector( Jet_pt->at(jet), Jet_eta->at(jet), Jet_phi->at(jet), Jet_mass->at(jet) ) ;   
      TLorentzVector mutlv = MakeTLorentzVector( Muon_pt->at(iMu), Muon_eta->at(iMu), Muon_phi->at(iMu), 0.105 ) ;   
      double ptout = fmin(ptmax*0.99,mutlv.Pt( (jttlv-mutlv).Vect())) ;
      if( ptout < 0.005 ) ptout *= -1 ;

      MuonClass.push_back(i) ;
      MuonWithJet.push_back(iMu) ;
      JetWithMuon.push_back(jet) ; 
      MuTLV.push_back( mutlv ) ;
      JtTLV.push_back( jttlv ) ;
      ptoutMu.push_back( ptout ) ;

    }     

  Mmult->Fill( (double) numberOfMuon , (double) MuonWithJet.size() ) ;
  if( MuonWithJet.size() < 2 ) return ;
  
  for( unsigned short int i=0 ; i< (unsigned int ) MuonWithJet.size() ; i++ ) {
    for( unsigned short int j=i+1 ; j< (unsigned int ) MuonWithJet.size() ; j++ ) {
      
      double mm = (MuTLV.at(i)+MuTLV.at(j)).M() ;
      if( mm>9 && mm <12 ) continue ;
      if( mm>70 ) continue ;

      int iC = MuonClass.at(i) ;
      int jC = MuonClass.at(j) ;
      CfrAll->Fill( iC, jC ) ;

      int ijC = ( hasGenp ? GetCombinedClass(iC,jC) : 0 ) ;

      bool SameCharge = ( Muon_charge->at( MuonWithJet.at(i) ) == Muon_charge->at( MuonWithJet.at(j) ) ) ;
      double pt1 = ( isodd ? ptoutMu.at(i) : ptoutMu.at(j) ) ;
      double pt2 = ( isodd ? ptoutMu.at(j) : ptoutMu.at(i) ) ;

      if( JetWithMuon.at(i) == JetWithMuon.at(j) ) // same jet events
	{
	  CfrSj->Fill( ijC ) ;
	  PtoutSameB->Fill( ijC,pt1,pt2 ) ;
	  if( SameCharge ) PtoutSameBSC->Fill( ijC,pt1,pt2 ) ;
	  else PtoutSameBOC->Fill( ijC,pt1,pt2 ) ;	  
	}


      if( (MuTLV.at(i)+MuTLV.at(j)).M()>5 ) // two different B
	{
	  CfrCmb->Fill( ijC ) ;
	  Ptout->Fill( ijC,pt1,pt2 ) ;
	  if( SameCharge ) PtoutSC->Fill( ijC,pt1,pt2 ) ;
	  else PtoutOC->Fill( ijC,pt1,pt2 ) ;	  
	}
    }
  }
} 
// ======================================================================
int Analize::GetCombinedClass( int iC, int jC ) 
{
  int imin = iC ;
  int imax = jC ;
  if( iC > jC ) { imin = jC ; imax = iC ; }

  if( imin == 1 && imax == 1 ) return 1 ; // bPbP
  if( imin == 1 && imax == 7 ) return 2 ; // bPbS (charm)
  if( imin == 1 && imax == 8 ) return 2 ; // bPbS (tau)
  if( imin == 7 && imax == 7 ) return 3 ; // bSbS (charm charm)
  if( imin == 7 && imax == 8 ) return 3 ; // bSbS (charm tau)
  if( imin == 8 && imax == 8 ) return 3 ; // bSbS (tau tau)
  if( imin == 2 && imax == 2 ) return 4 ; // c c  
  if( imin == 2 && imax == 5 ) return 5 ; // c / DIF
  if( imin == 3 && imax == 3 ) return 6 ; // rho rho

  return 7 ; // whatever else
					
}
// ======================================================================
int Analize::GetMuonClass( unsigned short int iMuon ) 
{

  if( !hasGenp ) return 0 ;

  short unsigned int jMuon = iMuon ;
  short int i = FindMuonClassFromMCAssociation( jMuon ) ;
  short int j = FindMuonMCProvenance( iMuon, i ) ;
  short int k = MergeMuonClassAndMCProvenance( i,j ) ;
  short int l = FindSimplifiedMuonCode( k ) ;

  if( abs(k) == 22 || abs(k) == 23 ) l=8 ;
  if( abs(k) == 15 || abs(k) == 16 ) l=7 ;
  if( abs(k) == 20 || abs(k) == 21 ) l=7 ;
  if( abs(k) == 19 ) l = 9 ;

  return (int) l ;
}
// ======================================================================
TLorentzVector Analize::MakeTLorentzVector( double pt, double eta, 
					    double phi, double mass ) 
{
  TLorentzVector tlv(0,0,0,0) ;
  double P = pt * TMath::CosH( eta ) ;
  tlv.SetXYZM( pt*cos(phi), pt*sin(phi), P*TMath::TanH( eta ), mass ) ;
  return tlv ;
}
// ======================================================================
#endif // Analize_cxx
