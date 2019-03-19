#ifndef FileOpener_h
#define FileOpener_h



class FileOpener
{
  public:
#if NTUPLE_VERSION >= 3
    TChain* OpenFile(const std::string sampleName, TChain* tree = 0, TChain* metadataTree = 0);
#else
    TChain* OpenFile(const std::string sampleName, TChain* tree = 0);
#endif
};

#endif // FileOpener_h
