void RunSkimMC(const int last)
{
  gSystem->CompileMacro("TagMuons.C");
  gSystem->CompileMacro("Analize.C");
  Analize m("QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5_SKIM","muonV2Pt4_jetPFV1Pt10",last) ;
  m.Loop();
  gSystem->Exit(0,kTRUE);
}

