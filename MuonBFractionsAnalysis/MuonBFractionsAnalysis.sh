#!/bin/bash

source /home/mgalanti/packages/root/v5.28.00b/root/bin/thisroot.sh

muonSelector=muonV2Pt4
dataSample=Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711
MCSample=MC__QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__DoubleMu3-AllSelections_V3
use1D=0
fakeIP=0
runProducers=1
runBuilders=1
runFit=1
constrainFractions=1
inputHistoNamesForTemplates="hMuonDxySimplifiedTemplate_1;hMuonDxySimplifiedTemplate_2;hMuonDxySimplifiedTemplate_3;hMuonDxySimplifiedTemplate_4;hMuonDxySimplifiedTemplate_5;hMuonDxySimplifiedTemplate_6"
outputHistoNamesForTemplates="B;C;P;D;E;F"
templatesToUseFor2D=$2
inputHistoNameForIPDistribution="hMuonDxy2D"
inputHistoNamesForStep1FakeIPDistributions="hMuonDxySimplifiedTemplate_1;hMuonDxySimplifiedTemplate_2;hMuonDxySimplifiedTemplate_3;hMuonDxySimplifiedTemplate_4;hMuonDxySimplifiedTemplate_5;hMuonDxySimplifiedTemplate_6"
outputHistoNamesForStep1FakeIPDistributions="B;C;P;D;E;F"
inputHistoNamesForFakeIPDistributions=$3
nEventsPerComponent=$1
fitMin="0."
fitMax="0.2"
rebinFactor="4"



# No changes by user below this point!

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
  root -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Templates__${MCSample}__${muonSelector}.root\"\,\"${inputHistoNamesForTemplates}\"\,\"${outputHistoNamesForTemplates}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\)
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
    root -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/RawFakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForStep1FakeIPDistributions}\"\,\"${outputHistoNamesForStep1FakeIPDistributions}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\)
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
      root -b -q runMuon2DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/RawIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForIPDistribution}\"\,${fitMin}\,${fitMax}\,${rebinFactor}\)
    fi
  fi
fi

if [ $fakeIP == 1 ]
then
  dataFile=../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelector}.root
else
  dataFile=../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelector}.root
fi

MCFile=../MuonBFractionsAnalysis/Templates__${MCSample}__${muonSelector}.root

# 3 - now call the fitter function
if [ $runFit == "1" ]
then
  cd ../BinnedLogLikelihood
  if [ $use1D == "1" ]
  then
    root -b -q runBinnedLogLikelihoodCalculator1D.cc\(\"${dataFile}\"\,\"${MCFile}\"\,\"${outputHistoNamesForTemplates}\"\)
    cp FitResults.txt ../MuonBFractionsAnalysis/FitResults1D_${nEventsPerComponent}.txt
  else
    if [ $constrainFractions == "1" ]
    then
      root -b -q runBinnedLogLikelihoodCalculator2DConstrained.cc\(\"${dataFile}\"\,\"${MCFile}\"\,\"${templatesToUseFor2D}\"\)
    else
      root -b -q runBinnedLogLikelihoodCalculator2D.cc\(\"${dataFile}\"\,\"${MCFile}\"\,\"${templatesToUseFor2D}\"\)
    fi
    cp FitResults.txt ../MuonBFractionsAnalysis/FitResults2D_${nEventsPerComponent}.txt
  fi
fi
