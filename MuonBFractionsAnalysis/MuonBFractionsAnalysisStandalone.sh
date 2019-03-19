#!/bin/bash
# source /lustre/cmswork/mgalanti/root/v5.28.00c/bin/thisroot.sh

############################
### Define the muon Selector
# muonSelector=muonV2Pt4
muonSelector=muonV6Pt4_jetPFV1Pt10

########################
### Define Data filelist
dataSample=Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711_SKIM
#dataSample=QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5_SKIM

######################
### Define MC filelist
MCSample=QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5_SKIM

####################################
### Do you want to use 1D templates?
### Do you want to apply the Fake IP/PtRel conditions?
use1D=0
fakeIP=0
fakePtRel=0

############################
### Choose which step to run
###        Template and Distribution Producers
###        Template Builder
###        Fit to templates
runProducers=0 #MuonTemplatesProducer MuonDistributionsProducer
runBuilders=0  #1
runFit=1 #1

#################################
### Do you want to fit templates?
### 0 = use non-fitted templates
### 1 = use fitted templates
### For the time being, the switch for PtRel has no effect!
fitTemplatesIP=0
fitTemplatesPtRel=0

#################################################
### Do you want to apply constraints on fractions
### or do you want to leave everything free?
### 0 = no constraints
### 1 = constraints on ratios
### 2 = constraints on sums
constrainFractionsIP=0
constrainFractionsPtRel=0

##########################################################
### Do you want to build Prompt Muons templates from Data?
### NOTE: check also "outputHistoNamesForIPTemplates"!
usePromptFromDataIP=1
useDFromDataPtRel=0
combineDAndCPtRel=0
histoNameFromMCToMergePtRel=hMuonPtOutJetSimplifiedMergedV2Template_3
histoNameFromDataToMergePtRel=hMuonPtOutJetDataDriven
histoNameMergedLightPtRel=hMuonPtOutJetLightTemplate
histoNamesToCopyForPtRelTemplates="hMuonPtOutJetSimplifiedMergedV2Template_1;hMuonPtOutJetSimplifiedMergedV2Template_2;hMuonPtOutJetSimplifiedMergedV2Template_3;hMuonPtOutJetSimplifiedMergedV2Template_4"

######################################
### Do you want to smear IP templates?
smearedIPTemplatesString="" #"_MRST2001lo-0" # null string = no smearing - or: "_1um" , "_3um", "_5um", "_10um", "_20um"

##############################################################
### Define input histograms according to the last choices made
### Switch on Prompt IP template (from Data or MC?)
if [ ${usePromptFromDataIP} == "1" ]
then
  ### Histo names for IP template builder
  inputHistoNamesForIPTemplates="hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_2;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_4;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_5"

  ### File and histo names for Prompt IP template builder
#  inputFileNameForPromptIPTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelector}.root"
  inputFileNameForPromptIPTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__muonV6Pt4.root"
  inputHistoNameForPromptIPTemplate="hMuonUpsilonBKGSubDxy"
else
  ### Histo names for IP template builder
  inputHistoNamesForIPTemplates="hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_2;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_4;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_5;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_3"

  ### File and histo names for Prompt IP template builder
#  inputFileNameForPromptIPTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelector}.root"
  inputFileNameForPromptIPTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__14
7146-149711__muonV6Pt4.root"
  inputHistoNameForPromptIPTemplate="hMuonUpsilonBKGSubDxy"
fi ### Switch on Prompt IP template (from Data or MC?)

### Histo names for PtRel template builder
# inputHistoNamesForPtRelTemplates="hMuonPtOutJetSimplifiedMergedV0Template_1;hMuonPtOutJetSimplifiedMergedV0Template_2;hMuonPtOutJetSimplifiedMergedV0Template_4;hMuonPtOutJetSimplifiedMergedV0Template_5;hMuonPtOutJetSimplifiedMergedV0Template_3;"
if [[ $useDFromDataPtRel == "1" && $combineDAndCPtRel == "1" ]]
then
  inputHistoNamesForPtRelTemplates="hMuonPtOutJetSimplifiedMergedV2Template_1;hMuonPtOutJetSimplifiedMergedV2Template_2;hMuonPtOutJetLightTemplate"
else
  inputHistoNamesForPtRelTemplates="hMuonPtOutJetSimplifiedMergedV1Template_1;hMuonPtOutJetSimplifiedMergedV1Template_2;hMuonPtOutJetSimplifiedMergedV1Template_3"
fi
# inputHistoNamesForPtRelTemplates="hMuonPtOutJetSimplifiedMergedV1TemplateJ1015_1;hMuonPtOutJetSimplifiedMergedV1TemplateJ1015_2;hMuonPtOutJetSimplifiedMergedV1TemplateJ1015_3"

#####################################################
### Define output histograms and templates to be used
### Histo names for IP template builder
outputHistoNamesForIPTemplates="B;C;D;F;P" # 1;2;4;5;3
#outputHistoNamesForIPTemplates="B;C;D" # 1;2;4;5;3
                                           # if prompt from data is used, "P" MUST be the last in the list
templatesIPToUseFor1D="B;C;D"
templatesIPToUseFor2D="BB;CC;BC;PP;DD;BD;CD"

### Histo names for PtRel template builder
# outputHistoNamesForPtRelTemplates="b;c;s;f;p" # 1;2;4;5;3
outputHistoNamesForPtRelTemplates="b;s;l" # 1;2;4;5;3
templatesPtRelToUseFor1D="b;s;l"
templatesPtRelToUseFor2D="bb;bs;bl;ss;sl;ll"

### Histo names for IP distributions

if [ ${use1D} == "1" ]
then
  inputHistoNameForIPDistribution="hMuonDxy"
else
  inputHistoNameForIPDistribution="hMuonDxy2D"  
fi
inputHistoNamesForStep1FakeIPDistributions="hMuonDxySimplifiedMergedTemplate_1;hMuonDxySimplifiedMergedTemplate_2;hMuonDxySimplifiedMergedTemplate_3;hMuonDxySimplifiedMergedTemplate_4;hMuonDxySimplifiedMergedTemplate_5"
outputHistoNamesForStep1FakeIPDistributions="B;C;P;D;F"
inputHistoNamesForFakeIPDistributions="hCC;hBC"
nEventsPerComponent="12000-0"



### Histo names for PtRel distributions
if [ ${use1D} == "1" ]
then
  inputHistoNameForPtRelDistribution="hMuonPtOutJet"
#   inputHistoNameForPtRelDistribution="hMuonPtOutJetJ1015"  
else
  inputHistoNameForPtRelDistribution="hMuonPtOutJet2D"  
fi
inputHistoNamesForStep1FakePtRelDistributions="hMuonPtOutJetSimplifiedMergedV0Template_1;hMuonPtOutJetSimplifiedMergedV0Template_2;hMuonPtOutJetSimplifiedMergedV0Template_3;hMuonPtOutJetSimplifiedMergedV0Template_4;hMuonPtOutJetSimplifiedMergedV0Template_5"
outputHistoNamesForStep1FakePtRelDistributions="bb;cc;bc;pp;ss;bc;bs;cs"
inputHistoNamesForFakePtRelDistributions="hcc;hbc"


######################
### Define constraints
### These are implemented as constraints to ratios
### For example, if component 0 is -1 and component 3 is 0
### this means that component 0 is free and component 3 is
### constrained to be Component3 = Component0*Value3/Value0
constraintToComponentIP="-1;-1;0;-1;-1;0;1"
constraintValueIP="0.;0.;0.;0.;0.;0.;0."
constraintErrorIP="0.;0.;0.;0.;0.;0.;0."

constraintToComponentPtRel="-1;-1;0;-1;-1;0;1"
constraintValuePtRel="0.;0.;0.;0.;0.;0.;0."
constraintErrorPtRel="0.;0.;0.;0.;0.;0.;0."

##############################
### Define constraints on sums
### These are implemented as constraints on the sums of components
### For example, if constraintSumToComponentIP has an entry of value 310,
### this constraints the sum of the 0th, 1st, and 3rd component to be
### equal to the value in constraintSumValueIP, with error
### constraintSumErrorIP
constraintSumToComponentIP=""
constraintSumValueIP=""
constraintSumErrorIP=""

constraintSumToComponentPtRel="310"
constraintSumValuePtRel="0.599"
constraintSumErrorPtRel="0.004"

#######################################
### Define fit ranges and rebin factors
### For IP
fitIPMin="0.0"
fitIPMax="0.2"
rebinFactorIP="4"
removeBinIP="0"
### For PtRel
fitPtRelMin="0."
fitPtRelMax="4.0"
rebinFactorPtRel="10"
removeBinPtRel="0"

#########################################
### Override Fit templates and constraint
### for different muon selectors
if [[ ${muonSelector} = *muonV2Pt4* ]] || [[ ${muonSelector} = *muonV6Pt4* ]] || [[ ${muonSelector} = *muonV9Pt4* ]]
then
  templatesIPToUseFor2D="BB;CC;BC;PP;DD;BD;CD"
  constraintToComponentIP="-1;-1;0;-1;-1;0;1"
  #constraintValueIP="0.7262;0.0935;0.0574;0.0092;0.0152;0.0617;0.0374" # muonV2Pt4
  #constraintErrorIP="0.;0.;0.001;0.;0.;0.001;0.007"
  constraintValueIP="0.716;0.0924;0.0566;0.0184;0.0149;0.0601;0.0369"
  constraintErrorIP="0.002;0.0008;0.0007;0.0004;0.0004;0.0007;0.0005"
  if [ ${fitIPMin} == "0.004" ]
  then
    templatesIPToUseFor2D="BB;CC;BC;DD;BD;CD"
    constraintToComponentIP="-1;-1;0;-1;0;1"
    constraintValueIP="0.8295;0.0520;0.0458;0.00742;0.0457;0.0195"
    constraintErrorIP="0.;0.;0.001;0.;0.001;0.007"
  fi
  if [ ${fitIPMin} == "0.008" ]
  then
    templatesIPToUseFor2D="BB;CC;BC;DD;BD;CD"
    constraintToComponentIP="-1;-1;0;-1;0;1"
    constraintValueIP="0.8835;0.0304;0.0361;0.0041;0.0348;0.0110"
    constraintErrorIP="0.;0.;0.001;0.;0.001;0.007"
  fi
  if [ ${fitIPMin} == "0.012" ]
  then
    templatesIPToUseFor2D="BB;CC;BC;DD;BD;CD"
    constraintToComponentIP="-1;-1;0;-1;0;1"
    constraintValueIP="0.9049;0.0216;0.0308;0.0032;0.0313;0.0083"
    constraintErrorIP="0.;0.;0.001;0.;0.001;0.007"
  fi
  if [ ${removeBinIP} == "1" ]
  then
    constraintValueIP="0.7649;0.07835;0.0556;0.;0.01212;0.05832;0.03068"
    constraintErrorIP="0.;0.;0.001;0.;0.;0.001;0.007"
  fi
  if [ ${removeBinIP} == "2" ]
  then
    constraintValueIP="0.7983;0.06125;0.05271;0.;0.009142;0.05493;0.02359"
    constraintErrorIP="0.;0.;0.001;0.;0.;0.001;0.007"
  fi
fi
if [[ ${muonSelector} = *muonV2Pt6* ]]
then
  constraintToComponentIP="-1;-1;0;-1;-1;0;1"
  #constraintValueIP="0.7729;0.09035;0.05547;0.;0.008224;0.0473;0.02571" # muonV2Pt6
  #constraintErrorIP="0.;0.;0.0016;0.;0.;0.0015;0.0104"
  constraintValueIP="0.746;0.0867;0.0522;0.0343;0.0073;0.0440;0.0253"
  constraintErrorIP="0.004;0.0014;0.0011;0.0008;0.0004;0.0010;0.0008"
  if [ ${fitIPMin} == "0.004" ]
  then
    templatesIPToUseFor2D="BB;CC;BC;DD;BD;CD"
    constraintToComponentIP="-1;-1;0;-1;0;1"
    constraintValueIP="0.8684;0.0454;0.0417;0.003;0.0304;0.0111"
    constraintErrorIP="0.;0.;0.0016;0.;0.0015;0.0104"
  fi
fi

if [[ ${muonSelector} = *jet* ]]
then
  templatesPtRelToUseFor2D="bb;bs;bl;ss;sl;ll"
  constraintToComponentPtRel="-1;-1;0;-1;5;2"
  constraintValuePtRel="0.4231;0.1980;0.1151;0.0259;0.02859;0.2092"
  constraintErrorPtRel="0.0024;0.0016;0.0012;0.0005;0.005;0.0016"

  templatesIPToUseFor2D="BB;CC;PP;BC;DD;BD;CD"

  #templatesIPToUseFor2D="CC;CD"
  #constraintToComponentIP="-1;-1;0;-1;-1;0;1"
  #constraintValueIP="0.;0.;0.;0.;0.;0.;0."
  #constraintErrorIP="0.;0.;0.;0.;0.;0.;0."
  constraintToComponentIP="-1;-1;0;-1;0;1"
  constraintValueIP="0.6668;0.1332;0.0634;0.0205;0.0678;0.0504" #0.458;0.0319;0.1209;0.0698;0.0056;0.0509;0.0134"
  constraintErrorIP="0.0034;0.0013;0.0008;0.0005;0.0009;0.0008" # 0.002;0.0008;0.0007;0.0004;0.0004;0.0007;0.0005"

fi


########################################
# No changes by user below this point! #
########################################

# 1 - run template producer on MC sample and build templates with correct axis-binning
# 1.1 - run template producer
if [ $runProducers == "1" ]
then
  cd ../MuonTemplatesProducer
  echo " #########################"
  echo " # RUN TEMPLATE PRODUCER #"
  echo " #########################"
  root -b -q runMuonTemplatesProducer.cc\(\"${MCSample}\"\,\"${muonSelector}\",-1\)
  cp MuonTemplatesProducer__Histograms__${MCSample}__${muonSelector}.root ../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root
  cd ../MuonBFractionsAnalysis
fi

# 1.2 - run template builder
if [ $runBuilders == "1" ]
then
  cd ../MuonTemplatesBuilder
  # 1.2.1 - switch prompt IP template and run IP template builder
  if [ $usePromptFromDataIP == "1" ]
  then
    echo " ###########################"
    echo " # RUN TEMPLATE IP BUILDER #"
    echo " # use Prompt from Data    #"
    echo " ###########################"
    if [ $fitTemplatesIP == "1" ]
    then
      root -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root\"\,\"${inputHistoNamesForIPTemplates}\"\,\"${outputHistoNamesForIPTemplates}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\,\"${inputFileNameForPromptIPTemplate}\"\,\"${inputHistoNameForPromptIPTemplate}\"\)
    else
      root -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root\"\,\"${inputHistoNamesForIPTemplates}\"\,\"${outputHistoNamesForIPTemplates}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\,\"${inputFileNameForPromptIPTemplate}\"\,\"${inputHistoNameForPromptIPTemplate}\"\)
    fi
  else
    echo " ###########################"
    echo " # RUN TEMPLATE IP BUILDER #"
    echo " # use Prompt from MC      #"
    echo " ###########################"
    if [ $fitTemplatesIP == "1" ]
    then
      root -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root\"\,\"${inputHistoNamesForIPTemplates}\"\,\"${outputHistoNamesForIPTemplates}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\)
    else
      root -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root\"\,\"${inputHistoNamesForIPTemplates}\"\,\"${outputHistoNamesForIPTemplates}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\)
    fi
  fi
  cd ../MuonBFractionsAnalysis
fi

# 2 - if we use the "real" IP distribution (data or MC), then
#     run the Distribution producer on the data sample, otherwise
#     run again the Template producer on it
if [ $fakeIP == "1" ]
then
  echo " #######################"
  echo " # FAKE IP SWITCH: YES #"
  echo " #######################"
  # 2.1 - run the producer
  if [ $runProducers == "1" ]
  then
    cd ../MuonTemplatesProducer
    echo " #########################"
    echo " # RUN TEMPLATE PRODUCER #"
    echo " # for Fake IP           #"
    echo " #########################"
    ####root -b -q runMuonTemplatesProducer.cc\(\"${dataSample}\"\,\"${muonSelector}\"\,-1\)
    cp MuonTemplatesProducer__Histograms__${dataSample}__${muonSelector}.root ../MuonBFractionsAnalysis/RawFakeIPDistributions__${dataSample}__${muonSelector}.root
    cd ../MuonBFractionsAnalysis
  fi
  # 2.2 - run the builder
  if [ $runBuilders == "1" ]
  then
    cd ../MuonTemplatesBuilder
    # 2.2.1 - run the builder on IP templates
    echo " ###########################"
    echo " # RUN TEMPLATE IP BUILDER #"
    echo " # for Fake IP             #"
    echo " ###########################"
    root -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/RawFakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForStep1FakeIPDistributions}\"\,\"${outputHistoNamesForStep1FakeIPDistributions}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\)
    # 2.2.2 - run the builder on PtRel templates
    echo " ##############################"
    echo " # RUN TEMPLATE PtRel BUILDER #"
    echo " # for Fake IP    ?????????   #"
    echo " ##############################"


    cd ../MuonBFractionsAnalysis
    cd ../MuonDistributionsBuilder
    if [ $use1D == "1" ]
    # 2.2.3 - switch 1D or 2D templates
    then
      echo " ###################################"
      echo " # RUN DISTRIBUTIONS IP BUILDER 1D #"
      echo " # for Fake IP                     #"
      echo " ###################################"
      root -b -q runMuon1DIPDistributionsBuilderFromTemplates.cc\(\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForFakeIPDistributions}\"\,\"${nEventsPerComponent}\"\,${fitIPMin}\,${fitIPMax}\)
    else
      echo " ###################################"
      echo " # RUN DISTRIBUTIONS IP BUILDER 2D #"
      echo " # for Fake IP                     #"
      echo " ###################################"
      root -b -q runMuon2DIPDistributionsBuilderFromTemplates.cc\(\"../MuonBFractionsAnalysis/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForFakeIPDistributions}\"\,\"${nEventsPerComponent}\"\,${fitIPMin}\,${fitIPMax}\)
    fi
  fi
# 2 - second part of the previous switch
else
  echo " ######################"
  echo " # FAKE IP SWITCH: NO #"
  echo " ######################"
  # 2.1 - run the producer
  if [ $runProducers == "1" ]
  then
    cd ../MuonDistributionsProducer
    echo " ##############################"
    echo " # RUN DISTRIBUTIONS PRODUCER #"
    echo " ##############################"
    root -b -q runMuonDistributionsProducer.cc\(\"${dataSample}\"\,\"${muonSelector}\",-1\)
    cp MuonDistributionsProducer__Histograms__${dataSample}__${muonSelector}.root ../MuonBFractionsAnalysis/RawDistributions__${dataSample}__${muonSelector}.root
    cd ../MuonBFractionsAnalysis
  fi
  # 2.2 - run the builder
  if [ $runBuilders == "1" ]
  then
    cd ../MuonDistributionsBuilder
    if [ $use1D == "1" ]
    then
      echo " ###################################"
      echo " # RUN DISTRIBUTIONS IP BUILDER 1D #"
      echo " ###################################"
      root -b -q runMuon1DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForIPDistribution}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\)
    else
      echo " ###################################"
      echo " # RUN DISTRIBUTIONS IP BUILDER 2D #"
      echo " ###################################"
      root -b -q runMuon2DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForIPDistribution}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\)
    fi
  fi
fi


if [ $fakeIP == 1 ]
then
  dataFileIP=../MuonBFractionsAnalysis/FakeIPDistributions__${dataSample}__${muonSelector}.root
else
  dataFileIP=../MuonBFractionsAnalysis/IPDistributions__${dataSample}__${muonSelector}.root
fi
    
MCFileIP=../MuonBFractionsAnalysis/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root
    
# 3 - now call the fitter function
if [ $runFit == "1" ]
then
  cd ../BinnedLogLikelihood
  if [ $use1D == "1" ]
  then
    if [ $constrainFractionsIP == "1" ]
    then
      root -b -q runBinnedLogLikelihoodCalculator1DConstrained.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor1D}\"\,\"${constraintToComponentIP}\"\,\"${constraintValueIP}\"\,\"${constraintErrorIP}\"\)
    elif [ $constrainFractionsIP == "2" ]
    then
      root -b -q runBinnedLogLikelihoodCalculator1DConstrainedSum.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor1D}\"\,\"${constraintSumToComponentIP}\"\,\"${constraintSumValueIP}\"\,\"${constraintSumErrorIP}\"\)
    else
      root -b -q runBinnedLogLikelihoodCalculator1D.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor1D}\"\)
    fi
    mv FitResults.txt ../MuonBFractionsAnalysis/FitResultsIP1D_${muonSelector}${smearedIPTemplatesString}.txt
    mv FitOutput.root ../MuonBFractionsAnalysis/FitOutputIP1D_${muonSelector}${smearedIPTemplatesString}.root
    cat ../MuonBFractionsAnalysis/FitResultsIP1D_${muonSelector}${smearedIPTemplatesString}.txt
  else
    if [ $constrainFractionsIP == "1" ]
    then
      root -b -q runBinnedLogLikelihoodCalculator2DConstrained.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor2D}\"\,\"${constraintToComponentIP}\"\,\"${constraintValueIP}\"\,\"${constraintErrorIP}\"\,\"${removeBinIP}\"\)
    elif [ $constrainFractionsIP == "2" ]
    then
      root -b -q runBinnedLogLikelihoodCalculator2DConstrainedSum.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor2D}\"\,\"${constraintSumToComponentIP}\"\,\"${constraintSumValueIP}\"\,\"${constraintSumErrorIP}\"\,\"${removeBinIP}\"\)
    else
      root -b -q runBinnedLogLikelihoodCalculator2D.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor2D}\"\,\"${removeBinIP}\"\)
    fi
    mv FitResults.txt ../MuonBFractionsAnalysis/FitResultsIP2D_${muonSelector}${smearedIPTemplatesString}.txt
    mv FitOutput.root ../MuonBFractionsAnalysis/FitOutputIP2D_${muonSelector}${smearedIPTemplatesString}.root
    cat ../MuonBFractionsAnalysis/FitResultsIP2D_${muonSelector}${smearedIPTemplatesString}.txt
  fi
fi

# combine the C and D into a single L template

if [[ $useDFromDataPtRel == "1" && $combineDAndCPtRel == "1" ]]
then
  if [ $use1D == "0" ]
  then
    CFractionFromIPFit=`../MuonBFractionsAnalysis/getFractionFrom2DIPFits.sh ../MuonBFractionsAnalysis/FitResultsIP2D_${muonSelector}${smearedIPTemplatesString}.txt C`
    DFractionFromIPFit=`../MuonBFractionsAnalysis/getFractionFrom2DIPFits.sh ../MuonBFractionsAnalysis/FitResultsIP2D_${muonSelector}${smearedIPTemplatesString}.txt D`
  else
    CFractionFromIPFit=`../MuonBFractionsAnalysis/getFractionFrom1DIPFits.sh ../MuonBFractionsAnalysis/FitResultsIP1D_${muonSelector}${smearedIPTemplatesString}.txt C`
    DFractionFromIPFit=`../MuonBFractionsAnalysis/getFractionFrom1DIPFits.sh ../MuonBFractionsAnalysis/FitResultsIP1D_${muonSelector}${smearedIPTemplatesString}.txt D`
  fi

  SumCDFractionsFromIPFit=`echo $CFractionFromIPFit+$DFractionFromIPFit | bc`
  CFractionFromIPFit=`echo "scale=7;$CFractionFromIPFit/$SumCDFractionsFromIPFit" | bc`
  DFractionFromIPFit=`echo "scale=7;$DFractionFromIPFit/$SumCDFractionsFromIPFit" | bc`
  
  echo " "
  echo "**** Calculation of the fractions in the combined PtRel light template ****"
  echo From the IP fit, the combined L \(i.e. C+D\) template is made up of $CFractionFromIPFit of C and of $DFractionFromIPFit of D
  echo " "
  
  cd ../MuonHistogramsMerger
  
  root -b -q runMuonHistogramMergerWithCopy.cc\(\"../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/MuonPtRelTemplatesProducerFromData__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/RawPtRelTemplatesWithDataDriven__${MCSample}__${muonSelector}.root\"\,\"${histoNameFromMCToMergePtRel}\"\,\"${histoNameFromDataToMergePtRel}\"\,\"${histoNameMergedLightPtRel}\",${CFractionFromIPFit},\"${histoNamesToCopyForPtRelTemplates}\"\)
  cd ../MuonBFractionsAnalysis
fi



# 3 - remaining distributions to be built: PtRel ones
if [ $runBuilders == "1" ]
then
  if [[ ${muonSelector} = *jet* ]]
  then
    cd ../MuonTemplatesBuilder
    if [[ ${useDFromDataPtRel} == "1" && ${combineDAndCPtRel} == "1" ]]
    then
      rawPtRelTemplatesFileName=../MuonBFractionsAnalysis/RawPtRelTemplatesWithDataDriven__${MCSample}__${muonSelector}.root
    else
      rawPtRelTemplatesFileName=../MuonBFractionsAnalysis/RawTemplates__${MCSample}__${muonSelector}.root
    fi
    # 1.2.2 - run PtRel/PtOut template builder
    echo " ##############################"
    echo " # RUN TEMPLATE PtRel BUILDER #"
    echo " ##############################"
    if [ $fitTemplatesPtRel == "1" ]
    then
      echo "E R R O R ! Fitted PtRel templates are not implemented yet!"
      exit
    else
      root -b -q runMuonTemplatesBuilder.cc\(\"${rawPtRelTemplatesFileName}\"\,\"../MuonBFractionsAnalysis/PtRelTemplates__${MCSample}__${muonSelector}.root\"\,\"${inputHistoNamesForPtRelTemplates}\"\,\"${outputHistoNamesForPtRelTemplates}\"\,${fitPtRelMin}\,${fitPtRelMax}\,${rebinFactorPtRel}\,${removeBinPtRel}\)
    fi
    cd ../MuonBFractionsAnalysis
    cd ../MuonDistributionsBuilder
    # 3.1 - switch 1D or 2D templates
    if [ $use1D == "1" ]
    then
      echo " ######################################"
      echo " # RUN DISTRIBUTIONS PtRel BUILDER 1D #"
      echo " ######################################"
      root -b -q runMuon1DPtRelDistributionsBuilderFromPtRelDistributions.cc\(\"../MuonBFractionsAnalysis/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/PtRelDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForPtRelDistribution}\"\,${fitPtRelMin}\,${fitPtRelMax}\,${rebinFactorPtRel}\)
    else
      echo " ######################################"
      echo " # RUN DISTRIBUTIONS PtRel BUILDER 2D #"
      echo " ######################################"
      root -b -q runMuon2DPtRelDistributionsBuilderFromPtRelDistributions.cc\(\"../MuonBFractionsAnalysis/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/PtRelDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForPtRelDistribution}\"\,${fitPtRelMin}\,${fitPtRelMax}\,${rebinFactorPtRel}\,${removeBinPtRel}\)
    fi
  fi
fi



if [ $fakePtRel == 1 ]
then
  dataFilePtRel=../MuonBFractionsAnalysis/FakePtRelDistributions__${dataSample}__${muonSelector}.root
else
  dataFilePtRel=../MuonBFractionsAnalysis/PtRelDistributions__${dataSample}__${muonSelector}.root
fi


MCFilePtRel=../MuonBFractionsAnalysis/PtRelTemplates__${MCSample}__${muonSelector}.root

if [[ ${muonSelector} = *jet* ]]
then
  # 3 - now call the fitter function for PtRel
  if [ $runFit == "1" ]
  then
    cd ../BinnedLogLikelihood
    if [ $use1D == "1" ]
    then
      if [ $constrainFractionsPtRel == "1" ]
      then
        root -b -q runBinnedLogLikelihoodCalculator1DConstrained.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor1D}\"\,\"${constraintToComponentPtRel}\"\,\"${constraintValuePtRel}\"\,\"${constraintErrorPtRel}\"\)
      elif [ $constrainFractionsPtRel == "2" ]
      then
        root -b -q runBinnedLogLikelihoodCalculator1DConstrainedSum.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor1D}\"\,\"${constraintSumToComponentPtRel}\"\,\"${constraintSumValuePtRel}\"\,\"${constraintSumErrorPtRel}\"\)
      else
        root -b -q runBinnedLogLikelihoodCalculator1D.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor1D}\"\)
      fi
      mv FitResults.txt ../MuonBFractionsAnalysis/FitResultsPtRel1D_${muonSelector}.txt
      mv FitOutput.root ../MuonBFractionsAnalysis/FitOutputPtRel1D_${muonSelector}.root
      cat ../MuonBFractionsAnalysis/FitResultsPtRel1D_${muonSelector}.txt
    else
      if [ $constrainFractionsPtRel == "1" ]
      then
        root -b -q runBinnedLogLikelihoodCalculator2DConstrained.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor2D}\"\,\"${constraintToComponentPtRel}\"\,\"${constraintValuePtRel}\"\,\"${constraintErrorPtRel}\"\,\"${removeBinPtRel}\"\)
      elif [ $constrainFractionsPtRel == "2" ]
      then
        root -b -q runBinnedLogLikelihoodCalculator2DConstrainedSum.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor2D}\"\,\"${constraintSumToComponentPtRel}\"\,\"${constraintSumValuePtRel}\"\,\"${constraintSumErrorPtRel}\"\,\"${removeBinPtRel}\"\)
      else
        root -b -q runBinnedLogLikelihoodCalculator2D.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor2D}\"\,\"${removeBinPtRel}\"\)
      fi
      mv FitResults.txt ../MuonBFractionsAnalysis/FitResultsPtRel2D_${muonSelector}.txt
      mv FitOutput.root ../MuonBFractionsAnalysis/FitOutputPtRel2D_${muonSelector}.root
      cat ../MuonBFractionsAnalysis/FitResultsPtRel2D_${muonSelector}.txt
    fi
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








echo "        "
echo "        "
      cat ../MuonBFractionsAnalysis/FitResultsIP2D_${muonSelector}.txt

