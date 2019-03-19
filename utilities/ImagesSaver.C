#ifndef ImagesSaver_cxx
#define ImagesSaver_cxx

#include "./ImagesSaver.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>

// #include "TFile.h"
#include "TClass.h"
#include "TCanvas.h"
#include "TKey.h"
#include "TROOT.h"



void ImagesSaver::Initialize()
{
  inputFile = new TFile(inputFileName.c_str(), "READ");
  if(inputFile->IsZombie())
  {
    cout << "ImagesSaver::Initialize - E R R O R : file \n\"" << inputFileName << "\"\n does not exits or is not a valid ROOT file!\nExiting!\n";
    exit(-1);
  }
}



bool ImagesSaver::CheckDirExistence(const std::string dirName)
{
  struct stat st;
  struct stat pippost;
  if(stat("pippo",&pippost) == 0)
    std::cout << "pippo\n";
  if(stat(dirName.c_str(),&st) == 0)
  {
    std::cout << "Directory " << dirName << " already exists!\n";
    return false;
  }
  return true;
}



bool ImagesSaver::CheckFormat(const std::string format)
{
  if(format.compare("png") == 0 ||
     format.compare("pdf") == 0 ||
     format.compare("jpg") == 0 ||
     format.compare("gif") == 0 ||
     format.compare("ps") == 0  ||
     format.compare("eps") == 0 ||
     format.compare("C") == 0 ||
     format.compare("root") == 0)
    return true;
  return false;
}



std::string ImagesSaver::FindDirName(const std::string format)
{
  // Find where the last dot is
  const size_t dotPos = inputFileName.find_last_of(".");
  std::cout << "dotPos = " << dotPos << std::endl;
  const std::string rootExtension = "root";
/*  if(!format.compare(dotPos,4,rootExtension) == 0)
  {
    std::cout << "ImagesSaver::FindDirName - W A R N I N G : input file name does not seem to end in \".root\"\n";
  }*/
  if(dotPos == inputFileName.length())
  {
    std::cout << "ImagesSaver::FindDirName - W A R N I N G : input file name does not seem to have an extension!\n";
  }
  std::string baseDirName = inputFileName.substr(0,dotPos);
  oss.str("");
  oss << baseDirName << "__" << format;
  return oss.str();
}



bool ImagesSaver::CreateDirectory(const std::string dirName)
{
  short int failed = mkdir(dirName.c_str(),0740);
  if(!failed)
  {
    if(!CheckDirExistence(dirName))
    {
      return true;
    }
  }
  return false;
}



bool ImagesSaver::ChangeDirectory(const std::string dirName)
{
  short int failed = chdir(dirName.c_str());
  if(!failed)
  {
    return true;
  }
  return false;
}



void ImagesSaver::SaveAllHistograms(const std::string format)
{
  // Check that format is a supported one
  bool formatOK = CheckFormat(format);
  std::string dirName = FindDirName(format);
  std::cout << "dirName = " << dirName << std::endl;
  bool dirOK = CheckDirExistence(dirName);
  if(!formatOK)
  {
    std::cout << "ImagesSaver::SaveAllHistograms - E R R O R : file format is not among the supported ones!\n\tNo images will be created!\n";
    return;
  }
  if(!dirOK)
  {
    std::cout << "ImagesSaver::SaveAllHistograms - E R R O R : output directory already exists!\n\tNo images will be created!\n";
    return;
  }
  // If everything is OK then create the directory
  bool creationOK = CreateDirectory(dirName);
  if(!creationOK)
  {
    std::cout << "ImagesSaver::SaveAllHistograms - E R R O R : could not create output directory!\n\tNo images will be created!\n";
    return;
  }
  bool chdirOK = ChangeDirectory(dirName);
  if(!chdirOK)
  {
    std::cout << "ImagesSaver::SaveAllHistograms - E R R O R : could not cd to output directory!\n\tNo images will be created!\n";
    return;
  }

  // Now loop inside input file
  TKey* key;
  TIter nextkey(inputFile->GetListOfKeys());

  while((key = (TKey*)nextkey()))
  {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    // Only take histograms
    if (cl->InheritsFrom(TH1::Class()))
    {
      std::string histoName(key->GetName());
      TH1* histo;
      inputFile->GetObject(histoName.c_str(),histo);
      std::string canvasName = "c" + histoName.substr(1,histoName.length());
      TCanvas* c = new TCanvas(canvasName.c_str());
      c->cd();
      c->SetLogy(true);
      histo->Draw();
      c->Update();
      std::string outputFileName = histoName + "." + format;
      c->SaveAs(outputFileName.c_str());
      delete c;
    }
  }
  chdir("..");
}



void ImagesSaver::SaveAllCanvases(const std::string format)
{
  // Check that format is a supported one
  bool formatOK = CheckFormat(format);
  std::string dirName = FindDirName(format);
  bool dirOK = CheckDirExistence(dirName);
  if(!formatOK)
  {
    std::cout << "ImagesSaver::SaveAllCanvases - E R R O R : file format is not among the supported ones!\n\tNo images will be created!\n";
    return;
  }
  if(!dirOK)
  {
    std::cout << "ImagesSaver::SaveAllCanvases - E R R O R : output directory already exists!\n\tNo images will be created!\n";
    return;
  }
  // If everything is OK then create the directory
  bool creationOK = CreateDirectory(dirName);
  if(!creationOK)
  {
    std::cout << "ImagesSaver::SaveAllCanvases - E R R O R : could not create output directory!\n\tNo images will be created!\n";
    return;
  }
  bool chdirOK = ChangeDirectory(dirName);
  if(!chdirOK)
  {
    std::cout << "ImagesSaver::SaveAllCanvases - E R R O R : could not cd to output directory!\n\tNo images will be created!\n";
    return;
  }

  // Now loop inside input file
  TKey* key;
  TIter nextkey(inputFile->GetListOfKeys());

  while((key = (TKey*)nextkey()))
  {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    // Only take histograms
    if (cl->InheritsFrom(TCanvas::Class()))
    {
      std::string canvasName(key->GetName());
      std::cout << "canvas name = " << canvasName << std::endl;
      TCanvas* canvas;
      inputFile->GetObject(canvasName.c_str(),canvas);
      std::string outputFileName = canvasName + "." + format;
//       if(format.compare("C") == 0)
//       {
        canvas->Draw();
        canvas->SaveAs(outputFileName.c_str());
//       }
//       else
//         canvas->SaveAs(outputFileName.c_str());
    }
  }
  chdir("..");
}



void ImagesSaver::SaveAllHistogramsAndCanvases(const std::string format)
{
  // Check that format is a supported one
  bool formatOK = CheckFormat(format);
  std::string dirName = FindDirName(format);
  bool dirOK = CheckDirExistence(dirName);
  if(!formatOK)
  {
    std::cout << "ImagesSaver::SaveAllHistogramsAndCanvases - E R R O R : file format is not among the supported ones!\n\tNo images will be created!\n";
    return;
  }
  if(!dirOK)
  {
    std::cout << "ImagesSaver::SaveAllHistogramsAndCanvases - E R R O R : output directory already exists!\n\tNo images will be created!\n";
    return;
  }
  // If everything is OK then create the directory
  bool creationOK = CreateDirectory(dirName);
  if(!creationOK)
  {
    std::cout << "ImagesSaver::SaveAllHistogramsAndCanvases - E R R O R : could not create output directory!\n\tNo images will be created!\n";
    return;
  }
  bool chdirOK = ChangeDirectory(dirName);
  if(!chdirOK)
  {
    std::cout << "ImagesSaver::SaveAllHistogramsAndCanvases - E R R O R : could not cd to output directory!\n\tNo images will be created!\n";
    return;
  }

  // Now loop inside input file
  TKey* key;

  TIter nextkey(inputFile->GetListOfKeys());
  while((key = (TKey*)nextkey()))
  {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    // Only take histograms
    if (cl->InheritsFrom(TH1::Class()))
    {
      std::string histoName(key->GetName());
      TH1* histo;
      inputFile->GetObject(histoName.c_str(),histo);
      histo->SaveAs(format.c_str());
    }
  }

  nextkey = inputFile->GetListOfKeys();
  while((key = (TKey*)nextkey()))
  {
    const char *classname = key->GetClassName();
    TClass *cl = gROOT->GetClass(classname);
    if (!cl) continue;
    // Only take histograms
    if (cl->InheritsFrom(TCanvas::Class()))
    {
      std::string histoName(key->GetName());
      TH1* histo;
      inputFile->GetObject(histoName.c_str(),histo);
      histo->SaveAs(format.c_str());
    }
  }
  chdir("..");
}



#endif // ImagesSaver_cxx
