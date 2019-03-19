#!/bin/bash

source /home/mgalanti/packages/root/v5.28.00b/root/bin/thisroot.sh

muonSelectorData=muonV4Pt4
muonSelectorMC=muonV2Pt4
dataSample=Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711
# dataSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__DoubleMu3-AllSelections_V3
# MCSample=MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1
MCSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__DoubleMu3-AllSelections_V3
# MCSample=MC__check_Abbiendi
# MCSample=BFromD6T_CDFromZ2_PFromData
use1D=0
fakeIP=0
runProducers=0
runBuilders=1
runFit=1
constrainFractions=1
usePromptFromData=1
smearedTemplatesString="" # null string = no smearing - or: "_1um" , "_3_um", "_5_um", "_10um", "_20um"
if [ ${usePromptFromData} == "1" ]
then
  inputHistoNamesForTemplates="hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_2;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_4;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_6"
# inputHistoNamesForTemplates="hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_2;hMuonDxyDIFHalfHFTemplate;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_6"

#   inputHistoNamesForTemplates="hMuonDxySimplifiedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedTemplate${smearedTemplatesString}_2;hMuonDxySimplifiedTemplate_abbiendi${smearedTemplatesString}_4;hMuonDxySimplifiedTemplate${smearedTemplatesString}_6"
  inputFileNameForPromptTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelectorMC}.root"
  inputHistoNameForPromptTemplate="hMuonUpsilonBKGSubDxy"
else
  inputHistoNamesForTemplates="hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_2;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_4;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_6;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_3"
#   inputHistoNamesForTemplates="hMuonDxySimplifiedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedTemplate${smearedTemplatesString}_2;hMuonDxySimplifiedTemplate${smearedTemplatesString}_4;hMuonDxySimplifiedTemplate${smearedTemplatesString}_6;hMuonDxySimplifiedTemplate${smearedTemplatesString}_3"
  inputFileNameForPromptTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelectorMC}.root"
  inputHistoNameForPromptTemplate="hMuonUpsilonBKGSubDxy"
fi
outputHistoNamesForTemplates="B;C;D;F;P" # if prompt from data is used, "P" MUST be the last in the list
templatesToUseFor2D="BB;CC;BC;PP;DD;BD;CD"
inputHistoNameForIPDistribution="hMuonDxy2D"
inputHistoNamesForStep1FakeIPDistributions="hMuonDxySimplifiedMergedTemplate_1;hMuonDxySimplifiedMergedTemplate_2;hMuonDxySimplifiedMergedTemplate_3;hMuonDxySimplifiedMergedTemplate_4;hMuonDxySimplifiedMergedTemplate_6"
outputHistoNamesForStep1FakeIPDistributions="B;C;P;D;F"
inputHistoNamesForFakeIPDistributions="hBB;hBC"
nEventsPerComponent="200000-0"
constraintToComponent="-1;-1;0;-1;-1;0;1"
constraintValue="0.;0.;0.;0.;0.;0.;0."
constraintError="0.;0.;0.;0.;0.;0.;0."
fitMin="0."
fitMax="0.2"
rebinFactor="4"
removeBin="0"

if [ ${muonSelectorMC} == "muonV2Pt4" ]
then
  templatesToUseFor2D="BB;CC;BC;PP;DD;BD;CD"
  constraintToComponent="-1;-1;0;-1;-1;0;1"
  constraintValue="0.7262;0.0935;0.0574;0.0092;0.0152;0.0617;0.0374" # muonV2Pt4
  constraintError="0.;0.;0.001;0.;0.;0.001;0.007"
  # constraintError="0.;0.;0.01;0.;0.;0.01;0.01"
  if [ ${fitMin} == "0.004" ]
  then
  templatesToUseFor2D="BB;CC;BC;DD;BD;CD"
  constraintToComponent="-1;-1;0;-1;0;1"
  constraintValue="0.8295;0.0520;0.0458;0.00742;0.0457;0.0195"
    constraintError="0.;0.;0.001;0.;0.001;0.007"
  fi
  if [ ${fitMin} == "0.008" ]
  then
    templatesToUseFor2D="BB;CC;BC;DD;BD;CD"
    constraintToComponent="-1;-1;0;-1;0;1"
    constraintValue="0.8835;0.0304;0.0361;0.0041;0.0348;0.0110"
    constraintError="0.;0.;0.001;0.;0.001;0.007"
  fi
  if [ ${fitMin} == "0.012" ]
  then
    templatesToUseFor2D="BB;CC;BC;DD;BD;CD"
    constraintToComponent="-1;-1;0;-1;0;1"
    constraintValue="0.9049;0.0216;0.0308;0.0032;0.0313;0.0083"
    constraintError="0.;0.;0.001;0.;0.001;0.007"
  fi
    
  if [ ${removeBin} == "1" ]
  then
    constraintValue="0.7649;0.07835;0.0556;0.;0.01212;0.05832;0.03068"
    constraintError="0.;0.;0.001;0.;0.;0.001;0.007"
  fi
  if [ ${removeBin} == "2" ]
  then
    constraintValue="0.7983;0.06125;0.05271;0.;0.009142;0.05493;0.02359"
    constraintError="0.;0.;0.001;0.;0.;0.001;0.007"
  fi
fi
if [ ${muonSelectorMC} == "muonV2Pt6" ]
then
  constraintToComponent="-1;-1;0;-1;-1;0;1"
  constraintValue="0.7729;0.09035;0.05547;0.;0.008224;0.0473;0.02571" # muonV2Pt6
  constraintError="0.;0.;0.0016;0.;0.;0.0015;0.0104"
fi
if [ ${muonSelectorMC} == "muonV2Pt8" ]
then
  constraintToComponent="-1;-1;0;-1;-1;0;1"
  constraintValue="0.7729;0.09035;0.05547;0.;0.008224;0.0473;0.02571" # muonV2Pt6
  constraintError="0.;0.;0.0016;0.;0.;0.0015;0.0104"
fi



# No changes by user below this point!

# 1 - run template producer on MC sample and build templates with correct axis-binning
if [ $runProducers == "1" ]
then
  cd ../MuonTemplatesProducer
  root -b -q runMuonTemplatesProducer.cc\(\"${MCSample}\"\,\"${muonSelectorMC}\",-1\)
  cp MuonTemplatesProducer__Histograms__${MCSample}__${muonSelectorMC}.root ../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelectorMC}.root
  cd ../MuonBFractionsAnalysis
fi

if [ $runBuilders == "1" ]
then
  cd ../MuonTemplatesBuilder
  if [ $usePromptFromData == "1" ]
  then
  root -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelectorMC}.root\"\,\"../MuonBFractionsAnalysis/Templates__${MCSample}__${muonSelectorMC}${smearedTemplatesString}.root\"\,\"${inputHistoNamesForTemplates}\"\,\"${outputHistoNamesForTemplates}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\,${removeBin}\,\"${inputFileNameForPromptTemplate}\"\,\"${inputHistoNameForPromptTemplate}\"\)
  else
    root -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelectorMC}.root\"\,\"../MuonBFractionsAnalysis/Templates__${MCSample}__${muonSelectorMC}${smearedTemplatesString}.root\"\,\"${inputHistoNamesForTemplates}\"\,\"${outputHistoNamesForTemplates}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\,${removeBin}\)
  fi
  cd ../MuonBFractionsAnalysis
fi

# 2 - if we use the "real" IP distribution (data or MC), then
#     run the IPDistribution producer on the data sample, otherwise
#     run again the Template producer on it
if [ $fakeIP == "1" ]
then
  if [ $runProducers == "1" ]
  then
    cd ../MuonTemplatesProducer
    root -b -q runMuonTemplatesProducer.cc\(\"${dataSample}\"\,\"${muonSelectorData}\"\,-1\)
    cp MuonTemplatesProducer__Histograms__${dataSample}__${muonSelectorData}.root ../MuonBFractionsAnalysis/RawFakeIPDistributions__${dataSample}__${muonSelectorData}.root
    cd ../MuonBFractionsAnalysis
  fi
  if [ $runBuilders == "1" ]
  then
    cd ../MuonTemplatesBuilder
    root -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/RawFakeIPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"${inputHistoNamesForStep1FakeIPDistributions}\"\,\"${outputHistoNamesForStep1FakeIPDistributions}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\,${removeBin}\)
    cd ../MuonBFractionsAnalysis
    cd ../MuonIPDistributionsBuilder
    if [ $use1D == "1" ]
    then
      root -b -q runMuon1DIPDistributionsBuilderFromTemplates.cc\(\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"${inputHistoNamesForFakeIPDistributions}\"\,\"${nEventsPerComponent}\"\,${fitMin}\,${fitMax}\)
    else
      root -b -q runMuon2DIPDistributionsBuilderFromTemplates.cc\(\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"${inputHistoNamesForFakeIPDistributions}\"\,\"${nEventsPerComponent}\"\,${fitMin}\,${fitMax}\)
    fi
  fi
else
  if [ $runProducers == "1" ]
  then
    cd ../MuonIPDistributionsProducer
    root -b -q runMuonIPDistributionsProducer.cc\(\"${dataSample}\"\,\"${muonSelectorData}\",-1\)
    cp MuonIPDistributionsProducer__Histograms__${dataSample}__${muonSelectorData}.root ../MuonBFractionsAnalysis/RawIPDistributions__${dataSample}__${muonSelectorData}.root
    cd ../MuonBFractionsAnalysis
  fi
  if [ $runBuilders == "1" ]
  then
    cd ../MuonIPDistributionsBuilder
    if [ $use1D == "1" ]
    then
      root -b -q runMuon1DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/RawIPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"${inputHistoNameForIPDistribution}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\)
    else
      root -b -q runMuon2DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/RawIPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelectorData}.root\"\,\"${inputHistoNameForIPDistribution}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\,${removeBin}\)
    fi
  fi
fi

if [ $fakeIP == 1 ]
then
  dataFile=../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelectorData}.root
else
  dataFile=../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelectorData}.root
fi

MCFile=../MuonBFractionsAnalysis/Templates__${MCSample}__${muonSelectorMC}${smearedTemplatesString}.root

# 3 - now call the fitter function
if [ $runFit == "1" ]
then
  cd ../BinnedLogLikelihood
  if [ $use1D == "1" ]
  then
    root -b -q runBinnedLogLikelihoodCalculator1D.cc\(\"${dataFile}\"\,\"${MCFile}\"\,\"${outputHistoNamesForTemplates}\"\)
    cp FitResults.txt ../MuonBFractionsAnalysis/FitResults1D_${nEventsPerComponent}${smearedTemplatesString}.txt
  else
    if [ $constrainFractions == "1" ]
    then
    root -b -q runBinnedLogLikelihoodCalculator2DConstrained.cc\(\"${dataFile}\"\,\"${MCFile}\"\,\"${templatesToUseFor2D}\"\,\"${constraintToComponent}\"\,\"${constraintValue}\"\,\"${constraintError}\"\,\"${removeBin}\"\)
    else
      root -b -q runBinnedLogLikelihoodCalculator2D.cc\(\"${dataFile}\"\,\"${MCFile}\"\,\"${templatesToUseFor2D}\"\)
    fi
#     cp FitResults.txt ../MuonBFractionsAnalysis/FitResults2D_${nEventsPerComponent}.txt
    cp FitResults.txt ../MuonBFractionsAnalysis/FitResults2D_${muonSelectorData}${smearedTemplatesString}.txt
    cp FitOutput.root ../MuonBFractionsAnalysis/FitOutput_${muonSelectorData}${smearedTemplatesString}.root
    cat ../MuonBFractionsAnalysis/FitResults2D_${muonSelectorData}${smearedTemplatesString}.txt
  fi
fi
# 
# if [[ ! -d "Template_${MCSample}" ]]
#   mkdir Template_${MCSample}
# fi
# 
# cd Template_${MCSample}
# cp 
# 
# if [[ ! -d "Data_${dataSample}" ]]
#   mkdir Data_${dataSample}
# fi
# 
# cd Data_${dataSample}
# 
# if 
