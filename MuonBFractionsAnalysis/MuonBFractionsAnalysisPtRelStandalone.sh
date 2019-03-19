#!/bin/bash
source /lustre/cmswork/mgalanti/root/v5.28.00c/bin/thisroot.csh

### Define the muon Selector
muonSelector=muonV2Pt4

### Define Data filelist
dataSample=Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711
# dataSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__DoubleMu3-AllSelections_V3

### Define MC filelist
# MCSample=MC__QCD_DoubleMuFilter_OSAndSSMuons_7TeV-pythia6__Summer10-START36_V10-v1
# MCSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__DoubleMu3-AllSelections_V3
# MCSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_4
# MCSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6_Plus_Pt5__Fall10-START38_V12-v2__NoSelections_V3_4
MCSample=QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5
# MCSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V14-v2__NoSelections_V3_StdPlusCutPt5_new
# MCSample=MC__check_Abbiendi
# MCSample=BFromD6T_CDFromZ2_PFromData

### Do you want to use 1D templates?
### Do you want to apply the Fake IP condition?
use1D=0
fakeIP=0

### Choose which step to run
###        Template and Distribution Producers
###        Template Builder
###        Fit to templates
runProducers=1 #0 #MuonTemplatesProducer MuonIPDistributionsProducer
runBuilders=0 #1
runFit=0 #1

### Do you want to apply constraints on fractions or do you want to leave everything free?
constrainFractions=1

### Do you want to build Prompt Muons templates from Data?
### NOTE: check also "outputHistoNamesForTemplates"!
usePromptFromData=0

### ??????????????????????????
smearedTemplatesString="_MRST2001lo-0" # null string = no smearing - or: "_1um" , "_3um", "_5um", "_10um", "_20um"

### Define input histograms according to the last choices made
if [ ${usePromptFromData} == "1" ]
then
  inputHistoNamesForTemplates="hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_2;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_4;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_6"
# inputHistoNamesForTemplates="hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_2;hMuonDxyDIFHalfHFTemplate;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_6"

#   inputHistoNamesForTemplates="hMuonDxySimplifiedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedTemplate${smearedTemplatesString}_2;hMuonDxySimplifiedTemplate_abbiendi${smearedTemplatesString}_4;hMuonDxySimplifiedTemplate${smearedTemplatesString}_6"
  inputFileNameForPromptTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelector}.root"
  inputHistoNameForPromptTemplate="hMuonUpsilonBKGSubDxy"
else
  inputHistoNamesForTemplates="hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_2;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_4;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_6;hMuonDxySimplifiedMergedTemplate${smearedTemplatesString}_3"
#   inputHistoNamesForTemplates="hMuonDxySimplifiedTemplate${smearedTemplatesString}_1;hMuonDxySimplifiedTemplate${smearedTemplatesString}_2;hMuonDxySimplifiedTemplate${smearedTemplatesString}_4;hMuonDxySimplifiedTemplate${smearedTemplatesString}_6;hMuonDxySimplifiedTemplate${smearedTemplatesString}_3"
  inputFileNameForPromptTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelector}.root"
  inputHistoNameForPromptTemplate="hMuonUpsilonBKGSubDxy"
fi

### Define output histograms and templates to be used
outputHistoNamesForTemplates="B;C;D;F;P" # if prompt from data is used, "P" MUST be the last in the list
templatesToUseFor2D="BB;CC;BC;PP;DD;BD;CD"
inputHistoNameForIPDistribution="hMuonDxy2D"
inputHistoNamesForStep1FakeIPDistributions="hMuonDxySimplifiedMergedTemplate_1;hMuonDxySimplifiedMergedTemplate_2;hMuonDxySimplifiedMergedTemplate_3;hMuonDxySimplifiedMergedTemplate_4;hMuonDxySimplifiedMergedTemplate_6"
outputHistoNamesForStep1FakeIPDistributions="B;C;P;D;F"
inputHistoNamesForFakeIPDistributions="hBB;hBC"

nEventsPerComponent="200000-0"

### Define constraints
### These are implemented as constraints to ratios
### For example, if component 0 is -1 and component 3 is 0
### this means that component 0 is free and component 3 is
### constrained to be Component3 = Component0*Value3/Value0
constraintToComponent="-1;-1;0;-1;-1;0;1"
constraintValue="0.;0.;0.;0.;0.;0.;0."
constraintError="0.;0.;0.;0.;0.;0.;0."

### Define fit ranges and rebin factors
fitMin="0."
fitMax="0.2"
rebinFactor="4"
removeBin="0"

### Override for different muon selectors
if [ ${muonSelector} == "muonV2Pt4" ]
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
if [ ${muonSelector} == "muonV2Pt6" ]
then
  constraintToComponent="-1;-1;0;-1;-1;0;1"
  constraintValue="0.7729;0.09035;0.05547;0.;0.008224;0.0473;0.02571" # muonV2Pt6
  constraintError="0.;0.;0.0016;0.;0.;0.0015;0.0104"
  # constraintError="0.;0.;0.01;0.;0.;0.01;0.01"
  if [ ${fitMin} == "0.004" ]
  then
    templatesToUseFor2D="BB;CC;BC;DD;BD;CD"
    constraintToComponent="-1;-1;0;-1;0;1"
    constraintValue="0.8684;0.0454;0.0417;0.003;0.0304;0.0111"
    constraintError="0.;0.;0.0016;0.;0.0015;0.0104"
  fi
fi
if [ ${muonSelector} == "muonV2Pt8" ]
then
  constraintToComponent="-1;-1;0;-1;-1;0;1"
  constraintValue="0.7729;0.09035;0.05547;0.;0.008224;0.0473;0.02571" # muonV2Pt6
  constraintError="0.;0.;0.0016;0.;0.;0.0015;0.0104"
fi



########################################
# No changes by user below this point! #
########################################

# 1 - run template producer on MC sample and build templates with correct axis-binning
if [ $runProducers == "1" ]
then
  cd ../MuonTemplatesProducer
  root -b -q runMuonTemplatesProducer.cc\(\"${MCSample}\"\,\"${muonSelector}\",-1\)
  cp MuonTemplatesProducer__Histograms__${MCSample}__${muonSelector}.root ../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root
  cd ../MuonBFractionsAnalysis
fi

if [ $runBuilders == "1" ]
then
  cd ../MuonTemplatesBuilder
  if [ $usePromptFromData == "1" ]
  then
  root -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Templates__${MCSample}__${muonSelector}${smearedTemplatesString}.root\"\,\"${inputHistoNamesForTemplates}\"\,\"${outputHistoNamesForTemplates}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\,${removeBin}\,\"${inputFileNameForPromptTemplate}\"\,\"${inputHistoNameForPromptTemplate}\"\)
  else
    root -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Templates__${MCSample}__${muonSelector}${smearedTemplatesString}.root\"\,\"${inputHistoNamesForTemplates}\"\,\"${outputHistoNamesForTemplates}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\,${removeBin}\)
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
    root -b -q runMuonTemplatesProducer.cc\(\"${dataSample}\"\,\"${muonSelector}\"\,-1\)
    cp MuonTemplatesProducer__Histograms__${dataSample}__${muonSelector}.root ../MuonBFractionsAnalysis/RawFakeIPDistributions__${dataSample}__${muonSelector}.root
    cd ../MuonBFractionsAnalysis
  fi
  if [ $runBuilders == "1" ]
  then
    cd ../MuonTemplatesBuilder
    root -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/RawFakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForStep1FakeIPDistributions}\"\,\"${outputHistoNamesForStep1FakeIPDistributions}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\,${removeBin}\)
    cd ../MuonBFractionsAnalysis
    cd ../MuonIPDistributionsBuilder
    if [ $use1D == "1" ]
    then
      root -b -q runMuon1DIPDistributionsBuilderFromTemplates.cc\(\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForFakeIPDistributions}\"\,\"${nEventsPerComponent}\"\,${fitMin}\,${fitMax}\)
    else
      root -b -q runMuon2DIPDistributionsBuilderFromTemplates.cc\(\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForFakeIPDistributions}\"\,\"${nEventsPerComponent}\"\,${fitMin}\,${fitMax}\)
    fi
  fi
else
  if [ $runProducers == "1" ]
  then
    cd ../MuonIPDistributionsProducer
    root -b -q runMuonIPDistributionsProducer.cc\(\"${dataSample}\"\,\"${muonSelector}\",-1\)
    cp MuonIPDistributionsProducer__Histograms__${dataSample}__${muonSelector}.root ../MuonBFractionsAnalysis/RawIPDistributions__${dataSample}__${muonSelector}.root
    cd ../MuonBFractionsAnalysis
  fi
  if [ $runBuilders == "1" ]
  then
    cd ../MuonIPDistributionsBuilder
    if [ $use1D == "1" ]
    then
      root -b -q runMuon1DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/RawIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForIPDistribution}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\)
    else
      root -b -q runMuon2DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/RawIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForIPDistribution}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\,${removeBin}\)
    fi
  fi
fi

if [ $fakeIP == 1 ]
then
  dataFile=../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelector}.root
else
  dataFile=../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelector}.root
fi

MCFile=../MuonBFractionsAnalysis/Templates__${MCSample}__${muonSelector}${smearedTemplatesString}.root

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
    cp FitResults.txt ../MuonBFractionsAnalysis/FitResults2D_${muonSelector}${smearedTemplatesString}.txt
    cp FitOutput.root ../MuonBFractionsAnalysis/FitOutput_${muonSelector}${smearedTemplatesString}.root
    cat ../MuonBFractionsAnalysis/FitResults2D_${muonSelector}${smearedTemplatesString}.txt
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
