void RunSkimData(const int last)
{
  gSystem->CompileMacro("TagMuons.C");
  gSystem->CompileMacro("Analize.C");
  Analize m("Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711_SKIM","muonV2Pt4_jetPFV1Pt10",last) ;
  m.Loop();
  gSystem->Exit(0,kTRUE);
}

