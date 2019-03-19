void runFragmentationAnalysis(const std::string sampleName = "", const std::string text = "", const int maxEvents = -1)
{
  gSystem->CompileMacro("FragmentationAnalysis.C");
  FragmentationAnalysis m(sampleName, text, maxEvents);
  m.Loop();
}
