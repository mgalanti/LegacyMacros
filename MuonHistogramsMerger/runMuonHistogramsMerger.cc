void runMuonHistogramsMerger(std::string inputFileName1, std::string inputFileName2, std::string outputFileName, std::string inputHistoName1, std::string inputHistoName2, std::string outputHistoName, double nEvents1, double nEvents2)
{
  gSystem->CompileMacro("../utilities/HistoPlotter.C");
  gSystem->CompileMacro("../utilities/MuonHistogramsMerger.C");
  MuonHistogramsMerger* m = new MuonHistogramsMerger(inputFileName1, inputFileName2, outputFileName);
  m->MergeHistograms(inputHistoName1, nEvents1, inputHistoName2, nEvents2, outputHistoName),
  delete m;
}
