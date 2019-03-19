#!/bin/bash

#++ -pthread -m64 -I/cvmfs/cms.cern.ch/slc5_amd64_gcc481/cms/cmssw/CMSSW_7_0_0_pre5/external/slc5_amd64_gcc481/bin/../../../../../../lcg/root/5.34.09-cms/include -L/cvmfs/cms.cern.ch/slc5_amd64_gcc481/cms/cmssw/CMSSW_7_0_0_pre5/external/slc5_amd64_gcc481/bin/../../../../../../lcg/root/5.34.09-cms/lib -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lTMVA -lMinuit2 MainChiAnalysisIPWorkflow.cc -o MainChiAnalysisIPWorkflow

g++ `root-config --cflags` `root-config --libs` -lTMVA -lMinuit2 MainChiAnalysisPtRelWorkflow.cc -o MainChiAnalysisPtRelWorkflow
