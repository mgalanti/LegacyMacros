void runMuonIPTemplatesProducerFromData(const std::string sampleName = "Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711", const std::string text = "muonV6Pt4_jetPFV0Pt10", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonIPTemplatesProducerFromData.C");
  MuonIPTemplatesProducerFromData m(sampleName, text, maxEvents);
  m.Loop();
}
