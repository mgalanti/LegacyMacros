void runMuonPtRelTemplatesProducerFromData(const std::string sampleName = "QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5", const std::string text = "muonV6Pt4_jetPFV0Pt10", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonPtRelTemplatesProducerFromData.C");
  MuonPtRelTemplatesProducerFromData m(sampleName, text, maxEvents);
  m.Loop();
}
