#ifndef ImagesSaver_h
#define ImagesSaver_h

#include "TFile.h"
#include "TH1.h"

#include <sstream>

#include "HistoPlotter.C"

class ImagesSaver : public HistoPlotter
{
public:
  ImagesSaver(std::string inputFileName_) :
    inputFileName(inputFileName_) {Initialize();}
    ~ImagesSaver() {inputFile->Close();}

  void SaveAllHistograms(const std::string format);
  void SaveAllCanvases(const std::string format);
  void SaveAllHistogramsAndCanvases(const std::string format);

  void Initialize();
  bool CheckDirExistence(const std::string dirName);
  bool CheckFormat(const std::string format);
  std::string FindDirName(const std::string format);
  bool CreateDirectory(const std::string dirName);
  bool ChangeDirectory(const std::string dirName);

private:

  std::string inputFileName;
  TFile* inputFile;

  std::ostringstream oss;

};

#endif // ImagesSaver_h
