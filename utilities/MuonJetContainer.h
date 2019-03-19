#ifndef MuonJetContainer_h
#define MuonJetContainer_h

#include "TLorentzVector.h"
#include "TVector3.h"

class MuonJetContainer
{
  public:
    MuonJetContainer( TLorentzVector pMuon_, TLorentzVector pJet_, TVector3 pVtx_, TVector3 pTkRef_, TLorentzVector pSVtx_, bool SubtractMuonFromAxis_ , bool SmearJetAxis_ = false) :
      pMuon(pMuon_), pJet(pJet_), pVtx(pVtx_), pTkRef(pTkRef_), pSVtx(pSVtx_), SubtractMuonFromAxis(SubtractMuonFromAxis_), SmearJetAxis(SmearJetAxis_)
    {

      if (SmearJetAxis)
      {      
        //FIXME! -> Smearing Parameters Hardcoded!!!   
        TRandom randN;
       
        double etaSmear[4] = {0.0533, 0.0323, 0.0279, 0.0286};
        double phiSmear[4] = {0.0334, 0.0249, 0.0221, 0.0237};

        unsigned int ptBin = 3;
        if (pJet.Pt() >= 10.0)
        {
          if (pJet.Pt() < 15.0)
            ptBin = 0;
          else if (pJet.Pt() < 20.0)
            ptBin = 1;
          else if (pJet.Pt() < 30.0)
            ptBin = 2;
        }
        else
        {
          std::cout << "W A R N I N G! you should never get here" << std::endl; 
        }

        pJet.SetPtEtaPhiM( pJet.Pt(),
                           randN.Gaus(0,etaSmear[ptBin])+pJet.Eta(),
                           randN.Gaus(0,phiSmear[ptBin])+pJet.Phi(),
                           pJet.M() );
      }
      
      // Clone the jet 4Momentum to allow muon subtraction
      TLorentzVector pJetNoMuonSub = pJet;

      // Compute Signed IP
      if( (pTkRef.X()*pJet.Px() + pTkRef.Y()*pJet.Py()) > 0. )
        SignedDxy = +1.; //Track_dxy_PV->at(itk).at(iVertex);
      else
        SignedDxy = -1.; //-Track_dxy_PV->at(itk).at(iVertex);

      // Subtract muon momentum if needed
      if(SubtractMuonFromAxis) pJet -= pMuon;

      // Calculate angles, separations and PtRel
      CosThetaJet = cos( pMuon.Angle( pJet.Vect() ) );
      DRMuonJet = pMuon.DeltaR( pJet );
      PtRelJet = pMuon.Pt( pJet.Vect() );  


      /// evaluate the boost of Bancestor
      TVector3 JetBoost     = -(pJet).BoostVector();      
      
      /// apply boost to the Muon -> move Mu in the Jet ref.frame
      TLorentzVector pBoostedMu = pMuon;
      pBoostedMu.Boost(JetBoost);
      
      /// evaluate Energy/Momentum of the muon in the Jet ref.frame       
      EmuJet = pBoostedMu.E();        
      PmuJet = pBoostedMu.P();        


      PtRelSV=1e10;
      PtRelSVorJet = PtRelJet;
      CosThetaSV=1e10;
      DRMuonSV=1e10;
      DRJetSV=1e10;

      if(pSVtx.Z() < 9e9)
      {
        // Subtract muon momentum if needed
        if(SubtractMuonFromAxis) pSVtx -= pMuon;

        // Calculate angles, separations and PtRel
        CosThetaSV = cos( pMuon.Angle( pSVtx.Vect() ) );
        DRMuonSV = pMuon.DeltaR( pSVtx );
        DRJetSV = pJet.DeltaR( pSVtx );
        PtRelSV = pMuon.Pt( pSVtx.Vect() );
        PtRelSVorJet = PtRelSV;
      }
    };

    ~MuonJetContainer(){};

    double getPtRelJet() { return PtRelJet; };
    double getPtRelSV() { return PtRelSV; };
    double getPtRelSVorJet() { return PtRelSVorJet; };
    double getCosThetaJet() { return CosThetaJet; };
    double getCosThetaSV() { return CosThetaSV; };
    double getDRMuonJet() { return DRMuonJet; };
    double getDRMuonSV() { return DRMuonSV; };
    double getDRJetSV() { return DRJetSV; };
    double getSignedDxy() { return SignedDxy; };

    double getPmuJet() { return PmuJet; };
    double getEmuJet() { return EmuJet; };

  private:
    double PtRelJet;
    double PtRelSV;
    double PtRelSVorJet;
    double CosThetaJet;
    double CosThetaSV;
    double DRMuonJet;
    double DRMuonSV;
    double DRJetSV;
    double SignedDxy;
    TLorentzVector pMuon;
    TLorentzVector pJet;
    TVector3 pVtx;
    TVector3 pTkRef;
    TLorentzVector pSVtx;
    bool SubtractMuonFromAxis;
    bool SmearJetAxis;
    
    double PmuJet;
    double EmuJet;

};

#endif // MuonJetContainer_h



