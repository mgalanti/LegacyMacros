void runPlotFractions2D(const std::string fractionsStringMerged2,const std::string inputHistoNamesMerged,const std::string templateNamesMerged)
{
  gSystem->CompileMacro("PlotFractions2D.cc");
  PlotFractions2D(fractionsStringMerged2,inputHistoNamesMerged,templateNamesMerged);
}
