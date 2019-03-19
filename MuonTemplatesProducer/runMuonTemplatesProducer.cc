void runMuonTemplatesProducer(const std::string sampleName = "QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5_SKIM", const std::string text = "muonV6Pt4_jetPFV1Pt10", const int maxEvents = -1)
{
  gSystem->CompileMacro("MuonTemplatesProducer.C");
  MuonTemplatesProducer m(sampleName, text, maxEvents);
  m.Loop();
}
