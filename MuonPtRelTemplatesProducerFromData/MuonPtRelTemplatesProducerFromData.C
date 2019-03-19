#ifndef MuonPtRelTemplatesProducerFromData_cxx
#define MuonPtRelTemplatesProducerFromData_cxx

// #define mgDEBUG

// Keep this define here!
#ifdef NTUPLE_VERSION
#undef NTUPLE_VERSION
#endif // NTUPLE_VERSION
#define NTUPLE_VERSION 3

#include <sstream>

#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "MuonPtRelTemplatesProducerFromData.h"

#include "../utilities/constants.h"

void MuonPtRelTemplatesProducerFromData::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = (maxEvents == -1) ? fChain->GetEntries() : std::min((int)fChain->GetEntries(), maxEvents);

  std::cout << "Analyzing " << nentries << " events.\n";

  // Setup the random number engine for template smearing
  TRandom3* rndm = new TRandom3();
  rndm->SetSeed(12345);

  // This is used to set an upper limit to
  // the Muon PtRel range to be used for templates
  const double ptRelMax = 5.;

  // Book histograms here
  // PtRel templates
  TH1D* hMuonPtRelJetDataDriven;
  TH1D* hMuonPtRelSVDataDriven;
  TH1D* hMuonPtRelSVorJetDataDriven;
  TH1D* hMuonPtRelJetNoSVDataDriven;
  TH1D* hMuonPtRelJetYesSVDataDriven;
  TH1D* hMuonDRPtRelDataDriven;
  TH1D* hMuonPtRelJetPlusDataDriven;
  TH1D* hMuonPtRelJetMinusDataDriven;
  TH1D* hMuonPtRelSVPlusDataDriven;
  TH1D* hMuonPtRelSVMinusDataDriven;
  TH1D* hMuonPtRelSVorJetPlusDataDriven;
  TH1D* hMuonPtRelSVorJetMinusDataDriven;
  TH1D* hMuonPtRelJetNoSVPlusDataDriven;
  TH1D* hMuonPtRelJetNoSVMinusDataDriven;
  TH1D* hMuonPtRelJetYesSVPlusDataDriven;
  TH1D* hMuonPtRelJetYesSVMinusDataDriven;

  // PtOut templates
  TH1D* hMuonPtOutJetDataDriven;
  TH1D* hMuonPtOutSVDataDriven;
  TH1D* hMuonPtOutSVorJetDataDriven;
  TH1D* hMuonPtOutJetNoSVDataDriven;
  TH1D* hMuonPtOutJetYesSVDataDriven;
  TH1D* hMuonDRPtOutDataDriven;
  TH1D* hMuonPtOutJetPlusDataDriven;
  TH1D* hMuonPtOutJetMinusDataDriven;
  TH1D* hMuonPtOutSVPlusDataDriven;
  TH1D* hMuonPtOutSVMinusDataDriven;
  TH1D* hMuonPtOutSVorJetPlusDataDriven;
  TH1D* hMuonPtOutSVorJetMinusDataDriven;
  TH1D* hMuonPtOutJetNoSVPlusDataDriven;
  TH1D* hMuonPtOutJetNoSVMinusDataDriven;
  TH1D* hMuonPtOutJetYesSVPlusDataDriven;
  TH1D* hMuonPtOutJetYesSVMinusDataDriven;



  TH1D* hMuonPtOutJetDataDrivenM0406;
  TH1D* hMuonPtOutJetDataDrivenM0608;
  TH1D* hMuonPtOutJetDataDrivenM0810;
  TH1D* hMuonPtOutJetDataDrivenM1012;
  TH1D* hMuonPtOutJetDataDrivenM1215;
  TH1D* hMuonPtOutJetDataDrivenM1520;
  TH1D* hMuonPtOutJetDataDrivenM20xx;

  TH1D* hMuonPtOutJetDataDrivenJ1015;
  TH1D* hMuonPtOutJetDataDrivenJ1520;
  TH1D* hMuonPtOutJetDataDrivenJ2030;
  TH1D* hMuonPtOutJetDataDrivenJ30xx;

  TH1D* hMuonPtOutJetDataDrivenD0025;
  TH1D* hMuonPtOutJetDataDrivenD2550;
  TH1D* hMuonPtOutJetDataDrivenD5075;
  TH1D* hMuonPtOutJetDataDrivenD7500;


  std::ostringstream oss;
  std::string name, title;

  if (s->JetSelectorIsValid()) {

    oss.str("");  oss << "hMuonPtRelJetDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} template";   title = oss.str();
    TH1D* histoPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetDataDriven = histoPtRelJet;

    oss.str("");  oss << "hMuonPtRelSVDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} template";   title = oss.str();
    TH1D* histoPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelSVDataDriven = histoPtRelSV;

    oss.str("");  oss << "hMuonPtRelSVorJetDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} template";   title = oss.str();
    TH1D* histoPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelSVorJetDataDriven = histoPtRelSVorJet;

    oss.str("");  oss << "hMuonPtRelJetNoSVDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} template";   title = oss.str();
    TH1D* histoPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetNoSVDataDriven = histoPtRelJetNoSV;

    oss.str("");  oss << "hMuonPtRelJetYesSVDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} template";   title = oss.str();
    TH1D* histoPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetYesSVDataDriven = histoPtRelJetYesSV;

    oss.str("");  oss << "hMuonDRPtRelDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} template";   title = oss.str();
    TH1D* histoDRPtRel = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
    hMuonDRPtRelDataDriven = histoDRPtRel;

    oss.str("");  oss << "hMuonPtRelJetPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetPlusDataDriven = histoPtPlusPtRelJet;

    oss.str("");  oss << "hMuonPtRelSVPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelSVPlusDataDriven = histoPtPlusPtRelSV;

    oss.str("");  oss << "hMuonPtRelSVorJetPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelSVorJetPlusDataDriven = histoPtPlusPtRelSVorJet;

    oss.str("");  oss << "hMuonPtRelJetNoSVPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetNoSVPlusDataDriven = histoPtPlusPtRelJetNoSV;

    oss.str("");  oss << "hMuonPtRelJetYesSVPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetYesSVPlusDataDriven = histoPtPlusPtRelJetYesSV;

    oss.str("");  oss << "hMuonPtRelJetMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtRelJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetMinusDataDriven = histoPtMinusPtRelJet;

    oss.str("");  oss << "hMuonPtRelSVMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtRelSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelSVMinusDataDriven = histoPtMinusPtRelSV;

    oss.str("");  oss << "hMuonPtRelSVorJetMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtRelSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelSVorJetMinusDataDriven = histoPtMinusPtRelSVorJet;

    oss.str("");  oss << "hMuonPtRelJetNoSVMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtRelJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetNoSVMinusDataDriven = histoPtMinusPtRelJetNoSV;

    oss.str("");  oss << "hMuonPtRelJetYesSVMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Rel} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtRelJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Rel} [GeV/c]", "N_{#mu}");
    hMuonPtRelJetYesSVMinusDataDriven = histoPtMinusPtRelJetYesSV;

    oss.str("");  oss << "hMuonPtOutJetDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template";   title = oss.str();
    TH1D* histoPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDriven = histoPtOutJet;

    oss.str("");  oss << "hMuonPtOutSVDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template";   title = oss.str();
    TH1D* histoPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutSVDataDriven = histoPtOutSV;

    oss.str("");  oss << "hMuonPtOutSVorJetDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template";   title = oss.str();
    TH1D* histoPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutSVorJetDataDriven = histoPtOutSVorJet;

    oss.str("");  oss << "hMuonPtOutJetNoSVDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template";   title = oss.str();
    TH1D* histoPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetNoSVDataDriven = histoPtOutJetNoSV;

    oss.str("");  oss << "hMuonPtOutJetYesSVDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template";   title = oss.str();
    TH1D* histoPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetYesSVDataDriven = histoPtOutJetYesSV;

    oss.str("");  oss << "hMuonDRPtOutDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template";   title = oss.str();
    TH1D* histoPtDRPtOut = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 100, 0., 0.5, "#DeltaR #mu Jet", "N_{#mu}");
    hMuonDRPtOutDataDriven = histoPtDRPtOut;

    oss.str("");  oss << "hMuonPtOutJetPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetPlusDataDriven = histoPtPlusPtOutJet;

    oss.str("");  oss << "hMuonPtOutSVPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutSVPlusDataDriven = histoPtPlusPtOutSV;

    oss.str("");  oss << "hMuonPtOutSVorJetPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutSVorJetPlusDataDriven = histoPtPlusPtOutSVorJet;

    oss.str("");  oss << "hMuonPtOutJetNoSVPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetNoSVPlusDataDriven = histoPtPlusPtOutJetNoSV;

    oss.str("");  oss << "hMuonPtOutJetYesSVPlusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} PlusTemplate";   title = oss.str();
    TH1D* histoPtPlusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetYesSVPlusDataDriven = histoPtPlusPtOutJetYesSV;

    oss.str("");  oss << "hMuonPtOutJetMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtOutJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetMinusDataDriven = histoPtMinusPtOutJet;

    oss.str("");  oss << "hMuonPtOutSVMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtOutSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutSVMinusDataDriven = histoPtMinusPtOutSV;

    oss.str("");  oss << "hMuonPtOutSVorJetMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtOutSVorJet = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutSVorJetMinusDataDriven = histoPtMinusPtOutSVorJet;

    oss.str("");  oss << "hMuonPtOutJetNoSVMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtOutJetNoSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetNoSVMinusDataDriven = histoPtMinusPtOutJetNoSV;

    oss.str("");  oss << "hMuonPtOutJetYesSVMinusDataDriven";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} MinusTemplate";   title = oss.str();
    TH1D* histoPtMinusPtOutJetYesSV = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetYesSVMinusDataDriven = histoPtMinusPtOutJetYesSV;









    oss.str("");  oss << "hMuonPtOutJetDataDrivenM0406";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template M0406";   title = oss.str();
    TH1D* histoPtOutJetM0406 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenM0406 = histoPtOutJetM0406;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenM0608";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template M0608";   title = oss.str();
    TH1D* histoPtOutJetM0608 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenM0608 = histoPtOutJetM0608;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenM0810";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template M0810";   title = oss.str();
    TH1D* histoPtOutJetM0810 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenM0810 = histoPtOutJetM0810;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenM1012";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template M1012";   title = oss.str();
    TH1D* histoPtOutJetM1012 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenM1012 = histoPtOutJetM1012;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenM1215";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template M1215";   title = oss.str();
    TH1D* histoPtOutJetM1215 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenM1215 = histoPtOutJetM1215;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenM1520";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template M1520";   title = oss.str();
    TH1D* histoPtOutJetM1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenM1520 = histoPtOutJetM1520;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenM20xx";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template M20xx";   title = oss.str();
    TH1D* histoPtOutJetM20xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenM20xx = histoPtOutJetM20xx;



    oss.str("");  oss << "hMuonPtOutJetDataDrivenJ1015";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template J1015";   title = oss.str();
    TH1D* histoPtOutJetJ1015 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenJ1015 = histoPtOutJetJ1015;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenJ1520";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template J1520";   title = oss.str();
    TH1D* histoPtOutJetJ1520 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenJ1520 = histoPtOutJetJ1520;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenJ2030";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template J2030";   title = oss.str();
    TH1D* histoPtOutJetJ2030 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenJ2030 = histoPtOutJetJ2030;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenJ30xx";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template J30xx";   title = oss.str();
    TH1D* histoPtOutJetJ30xx = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenJ30xx = histoPtOutJetJ30xx;



    oss.str("");  oss << "hMuonPtOutJetDataDrivenD0025";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template D0025";   title = oss.str();
    TH1D* histoPtOutJetD0025 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenD0025 = histoPtOutJetD0025;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenD2550";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template D2550";   title = oss.str();
    TH1D* histoPtOutJetD2550 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenD2550 = histoPtOutJetD2550;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenD5075";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template D5075";   title = oss.str();
    TH1D* histoPtOutJetD5075 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenD5075 = histoPtOutJetD5075;

    oss.str("");  oss << "hMuonPtOutJetDataDrivenD7500";   name = oss.str();
    oss.str("");  oss << "p_{T}^{Out} template D7500";   title = oss.str();
    TH1D* histoPtOutJetD7500 = Create1DHistogram<TH1D>(name.c_str(),title.c_str(), 500, 0., 5., "p_{T}^{Out} [GeV/c]", "N_{#mu}");
    hMuonPtOutJetDataDrivenD7500 = histoPtOutJetD7500;












  } /// JetSelectorIsValid()
  
  // Loop over events
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries; jentry++)
  {

    if(jentry%1000 == 0)
      std::cout << "Loop over entry " << jentry << "/" << nentries << ".\n";
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
    {
      break;
    }
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    // Select Muons and Jets according to requirements!
    // Store here selected Muons, Jets and Vtx
    std::vector<unsigned short> vSelectedMuons, vSelectedJets;
    unsigned short iSelectedVertex;
    
    // Use only default selection
    // Selection string is passed to ParticleSelector
    // since the constructor of TreeAnalyzer*
    if(!s->MuonSelectorIsValid()) continue;
    
    // Decide if Muon+Jet combined selection or Muon selection only
    if(!s->JetSelectorIsValid()) continue;

    // Anti-select EventWithDiMuon
    if(s->SelectEventWithDiMuonResonanceV0(vSelectedMuons, iSelectedVertex))
    {
      continue;
    }
/*    else // Check it is not a DiMuon in resonances!
    {
      if (vSelectedMuons.size() == 2 && iSelectedVertex != 65535)
      {
        int iMuon1 = vSelectedMuons[0];
        int iMuon2 = vSelectedMuons[1];
        TLorentzVector pMuon1,pMuon2;
        pMuon1.SetPtEtaPhiM(Muon_pt->at(iMuon1),Muon_eta->at(iMuon1),Muon_phi->at(iMuon1),constants::muonMass);
        pMuon2.SetPtEtaPhiM(Muon_pt->at(iMuon2),Muon_eta->at(iMuon2),Muon_phi->at(iMuon2),constants::muonMass);
        double diMuonMass = (pMuon1 + pMuon2).M();
        if( diMuonMass < 5.
            || (diMuonMass>8.9 && diMuonMass<10.6)
            || diMuonMass > 70.)
          continue;
      }
    }
*/

    // This is mantained anyway
    // Get also trigger selection
    if (hasGenp)
    {
      if(!s->SelectEventPassingTrigger("HLT_DoubleMu3"))
      {
        continue;
      }
    } 
    else
    {
      if(!s->SelectEventPassingTrigger("HLT_DoubleMu3_v2"))
      {
        continue;
      }
    }

    if(s->GetJetSelector().find("PF") == std::string::npos) continue;

    std::vector<unsigned int> vSelTracks;
    std::vector<unsigned int> vSelJets;

    // Loop over tracks
    for(unsigned short iTrack = 0; iTrack < numberOfTrack; iTrack++)
    {
      // Reject Muons!
      bool isMuon = false;
      for(unsigned short iMuon = 0; iMuon < numberOfMuon && !isMuon; iMuon++)
      {
        if (Muon_trackref->at(iMuon) == iTrack) isMuon = true;
      }
      if (isMuon) continue;

      // First, reject according to Pt
      if (Track_pt->at(iTrack) <= s->GetMuonPtThreshold()) continue;

      // Select track with muon criteria
      // but using tracker information only
      if (!s->SelectTrack(iTrack)) continue;

      // Find the associated jet
      unsigned short iAssociatedJet = Track_jetRef->at(iTrack) ;
      if (iAssociatedJet > numberOfJet) continue;
      
      // Reject according to Pt
      if (Jet_pt->at(iAssociatedJet) <= s->GetJetPtThreshold()) continue;

      // SelectJet
      if (!s->SelectJet(iAssociatedJet)) continue;

      // Anti btag

      // Not muons inside jet
      bool isMuonJet = false;
      for(unsigned short iMuon = 0; iMuon < numberOfMuon && !isMuonJet; iMuon++)
      {
        if (Muon_trackref->at(iMuon) >= numberOfTrack) continue;
        if (Track_jetRef->at(Muon_trackref->at(iMuon)) == iAssociatedJet) isMuonJet = true;
      }

      if (isMuonJet) continue;

      if (s->GetJetVersion().compare("1") == 0)
      {
        if ((Jet_pt->at(iAssociatedJet) - Track_pt->at(iTrack))/Jet_pt->at(iAssociatedJet) < 0.3 ) continue;
      }

      // Not TCHEL
      if (Jet_TrackCountingHighEffBTag->at(iAssociatedJet) >= 1.7 ) continue;

      vSelTracks.push_back(iTrack);
      vSelJets.push_back(iAssociatedJet);

    }

    // PtRel/PtOut stuff
    std::vector<double> CosThetaJet_PtOut, PtOutJet, PtOutSV, PtOutSVorJet, DRMuonJet_PtOut, CosThetaSV_PtOut, DRMuonSV_PtOut, DRJetSV_PtOut,
                        CosThetaJet_PtRel, PtRelJet, PtRelSV, PtRelSVorJet, DRMuonJet_PtRel, CosThetaSV_PtRel, DRMuonSV_PtRel, DRJetSV_PtRel,
                        SignedDxy;

    std::vector<short int> vCharge;

    std::vector<short int> vSelTracks0, vSelJets0;

    // Loop over selected Tracks
    for(unsigned short i = 0; i < vSelTracks.size(); i++)
    {
      unsigned short iTrack = vSelTracks[i];
      vCharge.push_back(Track_charge->at(iTrack));

      // Calculate PtRel/PtOut information
      if (s->JetSelectorIsValid())
      {
        unsigned short iJet = vSelJets[i];

        // Retrieve PV infos
        iSelectedVertex = s->SelectVertexNearestToTrack(iTrack,"V0");
        if(iSelectedVertex == 65535) continue;
        TVector3 pVtx( PV_x->at(iSelectedVertex), PV_y->at(iSelectedVertex), PV_z->at(iSelectedVertex) );

        // Build TLorentzVectors
        TLorentzVector pTrack, pJet;
        pTrack.SetPtEtaPhiM( Track_pt->at(iTrack), Track_eta->at(iTrack), Track_phi->at(iTrack), 0.1396 );
        if(s->GetJetType().compare("Tk") == 0)
          pJet.SetPtEtaPhiM( TrackJet_pt->at(iJet), TrackJet_eta->at(iJet), TrackJet_phi->at(iJet), TrackJet_mass->at(iJet) );
        else
          pJet.SetPtEtaPhiM( Jet_pt->at(iJet), Jet_eta->at(iJet), Jet_phi->at(iJet), Jet_mass->at(iJet) );

        // Track reference point (corrected for PV)
        TVector3 pTkRef( Track_referencePoint_x->at(iTrack) - pVtx.X(),
                         Track_referencePoint_y->at(iTrack) - pVtx.Y(),
                         Track_referencePoint_z->at(iTrack) - pVtx.Z() );

        // Use SecVtx if present inside jet
        int iSecVtx = -1;
        double jetPt=0;
        for (unsigned int j = 0; j < numberOfSecondaryVertex; j++)
        {
          if(SV_jetref->at(j) == iJet)
          {
            iSecVtx = j;
            break;
          }
        }

        TLorentzVector pSVtx;
        if(iSecVtx > -1)
        {
          pSVtx.SetXYZM( SV_flightDirection_x->at(iSecVtx), SV_flightDirection_y->at(iSecVtx), SV_flightDirection_z->at(iSecVtx), SV_mass->at(iSecVtx) );
          pSVtx.SetRho( pJet.Rho() );
        }
        else
        {
          pSVtx.SetXYZM( 9e9, 9e9, 9e9, 9e9 );
        }

        // Muon not subtracted from the Jet ---> PtRel
        MuonJetContainer contPtRel = MuonJetContainer( pTrack, pJet, pVtx, pTkRef, pSVtx, false );
        CosThetaJet_PtRel.push_back( contPtRel.getCosThetaJet() );
        CosThetaSV_PtRel.push_back( contPtRel.getCosThetaSV() );
        DRMuonJet_PtRel.push_back( contPtRel.getDRMuonJet() );
        PtRelJet.push_back( contPtRel.getPtRelJet() );
        PtRelSV.push_back( contPtRel.getPtRelSV() );
        PtRelSVorJet.push_back( contPtRel.getPtRelSVorJet() );
        SignedDxy.push_back( contPtRel.getSignedDxy() );
        DRMuonSV_PtRel.push_back( contPtRel.getDRMuonSV() );
        DRJetSV_PtRel.push_back( contPtRel.getDRJetSV() );

        // Muon subtracted from the Jet ---> PtOut
        MuonJetContainer contPtOut = MuonJetContainer( pTrack, pJet, pVtx, pTkRef, pSVtx, true );
        CosThetaJet_PtOut.push_back( contPtOut.getCosThetaJet() );
        CosThetaSV_PtOut.push_back( contPtOut.getCosThetaJet() );
        DRMuonJet_PtOut.push_back( contPtOut.getDRMuonJet() );
        PtOutJet.push_back( contPtOut.getPtRelJet() );
        PtOutSVorJet.push_back( contPtOut.getPtRelSVorJet() );
        PtOutSV.push_back( contPtOut.getPtRelSV() );
        DRMuonSV_PtOut.push_back( contPtOut.getDRMuonSV() );
        DRJetSV_PtOut.push_back( contPtOut.getDRJetSV() );

        vSelTracks0.push_back(iTrack);
        vSelJets0.push_back(iJet);

      } // PtRel/PtOut

    } // Loop over selected tracks

    // PtRel/PtOut
    if (s->JetSelectorIsValid())
    {
      for (unsigned int k = 0; k < PtRelJet.size(); k++)
      {
        // Skip if PtRel is out of range
        //if (PtRelJet[k]>ptRelMax) continue;

        hMuonPtRelJetDataDriven->Fill(PtRelJet[k]);
        hMuonPtRelSVorJetDataDriven->Fill(PtRelSVorJet[k]);
        hMuonPtRelSVDataDriven->Fill(PtRelSV[k]);
        if (PtRelSV[k]<1e9) hMuonPtRelJetYesSVDataDriven->Fill(PtRelJet[k]);
        else                hMuonPtRelJetNoSVDataDriven->Fill(PtRelJet[k]);
        hMuonDRPtRelDataDriven->Fill(DRMuonJet_PtRel[k]);

        // Separate by muon charge
        if(vCharge[k]>0)
        { 
          hMuonPtRelJetPlusDataDriven->Fill(PtRelJet[k]);
          hMuonPtRelSVorJetPlusDataDriven->Fill(PtRelSVorJet[k]);
          hMuonPtRelSVPlusDataDriven->Fill(PtRelSV[k]);
          if (PtRelSV[k]<1e9) hMuonPtRelJetYesSVPlusDataDriven->Fill(PtRelJet[k]);
          else                hMuonPtRelJetNoSVPlusDataDriven->Fill(PtRelJet[k]);
        }
        else
        {
          hMuonPtRelJetMinusDataDriven->Fill(PtRelJet[k]);
          hMuonPtRelSVorJetMinusDataDriven->Fill(PtRelSVorJet[k]);
          hMuonPtRelSVMinusDataDriven->Fill(PtRelSV[k]);
          if (PtRelSV[k]<1e9) hMuonPtRelJetYesSVMinusDataDriven->Fill(PtRelJet[k]);
          else                hMuonPtRelJetNoSVMinusDataDriven->Fill(PtRelJet[k]);
        } // Separate by muon charge

      }

      for (unsigned int k = 0; k < PtOutJet.size(); k++)
      {
        // Skip if PtOut is out of range
        //if (PtOutJet[k]>ptRelMax) continue;

        hMuonPtOutJetDataDriven->Fill(PtOutJet[k]);
        hMuonPtOutSVorJetDataDriven->Fill(PtOutSVorJet[k]);
        hMuonPtOutSVDataDriven->Fill(PtOutSV[k]);
        if (PtOutSV[k]<1e9) hMuonPtOutJetYesSVDataDriven->Fill(PtOutJet[k]);
        else                hMuonPtOutJetNoSVDataDriven->Fill(PtOutJet[k]);
        hMuonDRPtOutDataDriven->Fill(DRMuonJet_PtOut[k]);

        // Separate by muon charge
        if(vCharge[k]>0)
        {
          hMuonPtOutJetPlusDataDriven->Fill(PtOutJet[k]);
          hMuonPtOutSVorJetPlusDataDriven->Fill(PtOutSVorJet[k]);
          hMuonPtOutSVPlusDataDriven->Fill(PtOutSV[k]);
          if (PtOutSV[k]<1e9) hMuonPtOutJetYesSVPlusDataDriven->Fill(PtOutJet[k]);
          else                hMuonPtOutJetNoSVPlusDataDriven->Fill(PtOutJet[k]);
        }
        else
        {
          hMuonPtOutJetMinusDataDriven->Fill(PtOutJet[k]);
          hMuonPtOutSVorJetMinusDataDriven->Fill(PtOutSVorJet[k]);
          hMuonPtOutSVMinusDataDriven->Fill(PtOutSV[k]);
          if (PtOutSV[k]<1e9) hMuonPtOutJetYesSVMinusDataDriven->Fill(PtOutJet[k]);
          else                hMuonPtOutJetNoSVMinusDataDriven->Fill(PtOutJet[k]);
        } // Separate by muon charge



        if (Track_pt->at(vSelTracks0[k]) >= 4.0)
        {
          if (Track_pt->at(vSelTracks0[k]) < 6.0)
            hMuonPtOutJetDataDrivenM0406->Fill(PtOutJet[k]);
          else if (Track_pt->at(vSelTracks0[k]) < 8.0)
            hMuonPtOutJetDataDrivenM0608->Fill(PtOutJet[k]);
          else if (Track_pt->at(vSelTracks0[k]) < 10.0)
            hMuonPtOutJetDataDrivenM0810->Fill(PtOutJet[k]);
          else if (Track_pt->at(vSelTracks0[k]) < 12.0)
            hMuonPtOutJetDataDrivenM1012->Fill(PtOutJet[k]);
          else if (Track_pt->at(vSelTracks0[k]) < 15.0)
            hMuonPtOutJetDataDrivenM1215->Fill(PtOutJet[k]);
          else if (Track_pt->at(vSelTracks0[k]) < 20.0)
            hMuonPtOutJetDataDrivenM1520->Fill(PtOutJet[k]);
          else
            hMuonPtOutJetDataDrivenM20xx->Fill(PtOutJet[k]);
        }

        if (Jet_pt->at(vSelJets0[k]) >= 10.0)
        {
          if (Jet_pt->at(vSelJets0[k]) < 15.0)
            hMuonPtOutJetDataDrivenJ1015->Fill(PtOutJet[k]);
          else if (Jet_pt->at(vSelJets0[k]) < 20.0)
            hMuonPtOutJetDataDrivenJ1520->Fill(PtOutJet[k]);
          else if (Jet_pt->at(vSelJets0[k]) < 30.0)
            hMuonPtOutJetDataDrivenJ2030->Fill(PtOutJet[k]);
          else
            hMuonPtOutJetDataDrivenJ30xx->Fill(PtOutJet[k]);
        }

        double deltaRelPt = ( Jet_pt->at(vSelJets0[k]) - Track_pt->at(vSelTracks0[k]) ) / Jet_pt->at(vSelJets0[k]);
        if (deltaRelPt <= 0.25)
          hMuonPtOutJetDataDrivenD0025->Fill(PtOutJet[k]);
        else if (deltaRelPt <= 0.50)
          hMuonPtOutJetDataDrivenD2550->Fill(PtOutJet[k]);
        else if (deltaRelPt <= 0.75)
          hMuonPtOutJetDataDrivenD5075->Fill(PtOutJet[k]);
        else if (deltaRelPt <= 1.00)
          hMuonPtOutJetDataDrivenD7500->Fill(PtOutJet[k]);

      }
    }
  }

  // Keep this line here!
  outFile->cd();

  // Uncomment this line to write also the histograms to the file
  outFile->Write();

}

double MuonPtRelTemplatesProducerFromData::DxyD6TScaling(double dxy)
{
  return 1./(0.958708+0.76623*pow(dxy,1./1.49011));
}

#endif // MuonPtRelTemplatesProducerFromData_cxx





