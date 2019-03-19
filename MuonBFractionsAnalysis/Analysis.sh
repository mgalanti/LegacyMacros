#!/bin/bash
# source /lustre/cmswork/mgalanti/root/v5.28.00c/bin/thisroot.sh

############################
### Define the muon Selector
muonSelector=muonV6Pt4_jetPFV1Pt10

########################
### Define Data filelist
# dataSample=QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5_SKIM
dataSample=Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711_SKIM

######################
### Define MC filelist
corrMCSample=QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5      ### CORRECTED TEMPLATE
MCSample=QCD_2MuPtFilter_OSAndSSMuons_7TeV-pythia6__Fall10-START38_V12-v2__NoSelections_V3_5_SKIM ### TEMPLATE STANDARD

######################
### Define ChiEvaluation parameters
blindChi=0  # use a fake nDataEvents for OS
forceChi=1  # evaluate chi even if Fit did not converge

######################
### Define COMMENT STRING TO APPEND TO RESULTS
# commstr="_DDlight"
# commstr="_constrRatio"
# commstr="_constrSum"
# commstr="_unconstr"
commstr="_Closure1D"

######################
### Define DIRNAME
dirname=`date +"%Y-%m-%d_%H-%M"`

if [[ ! -d "FitResults/${dirname}" ]]
then
  echo " Creating folder FitResults/${dirname}"
  mkdir FitResults/${dirname}
else
  echo " E R R O R !"
  echo "   folder FitResults/${dirname} already exists"
  echo "   exit..."
  exit
fi


####################################
### Define Fit Variable
fitVar=PtOut
# fitVar=PtIn
# fitVar=Emu

######################
### Define PYTHIA correction
# correction=_DoubleMuFilter_PtOut
# correction=_DoubleMuFilter_HalfStat_PtOut
# correction=_DoubleMuFilter_FourthStat_PtOut
# correction=_DoubleMuFilter_TenthStat_PtOut
# correction=_DoubleMuFilter_Emu
correction=

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
runBuilders=1  #1
runFit=1 #1

#################################
### Do you want to fit templates?
### 0 = use non-fitted templates
### 1 = use fitted templates
### For the time being, the switch for PtRel has no effect!
fitTemplatesIP=1
fitTemplatesPtRel=0

#################################################
### Do you want to apply constraints on fractions
### or do you want to leave everything free?
### 0 = no constraints
### 1 = constraints on ratios
### 2 = constraints on sums
constrainFractionsIP=1
constrainFractionsPtRel=1

#######################################
### Define fit ranges and rebin factors
### For IP
fitIPMin="0.0"
fitIPMax="0.2"
rebinFactorIP="4"
removeBinIP="0"
### For PtRel
fitPtRelMin="0."
fitPtRelMax="4.5"
rebinFactorPtRel="10"
removeBinPtRel="0"
### For Emu
fitEmuMin="0.2"
fitEmuMax="6.0"
rebinFactorEmu="10"
removeBinEmu="0"

##########################################################
### Do you want to build Prompt Muons templates from Data?
### NOTE: check also "outputHistoNamesForIPTemplates"!
usePromptFromDataIP=1
useDFromDataPtRel=0
combineDAndCPtRel=0
histoNameFromMCToMergePtRel=hMuon${fitVar}JetSimplifiedMergedV2Template_3
histoNameFromDataToMergePtRel=hMuon${fitVar}JetDataDriven
histoNameMergedLightPtRel=hMuon${fitVar}JetLightTemplate
histoNamesToCopyForPtRelTemplates="hMuon${fitVar}JetSimplifiedMergedV2Template_1;hMuon${fitVar}JetSimplifiedMergedV2Template_2;hMuon${fitVar}JetSimplifiedMergedV2Template_3;hMuon${fitVar}JetSimplifiedMergedV2Template_4"

######################################
### Do you want to smear IP templates?
smearedIPTemplatesString="" #"_MRST2001lo-0" # null string = no smearing - or: "_1um" , "_3um", "_5um", "_10um", "_20um"

##############################################################
### Define input histograms according to the last choices made
### Switch on Prompt IP template (from Data or MC?)
if [ ${usePromptFromDataIP} == "1" ]
then
  ## Histo names for IP template builder
  inputHistoNamesForIPTemplates="hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_2;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_4;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_5"

  ## File and histo names for Prompt IP template builder
#   inputFileNameForPromptIPTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelector}.root"
  inputFileNameForPromptIPTemplate="../MuonBFractionsAnalysis/Templates/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__muonV6Pt4.root"
  inputHistoNameForPromptIPTemplate="hMuonUpsilonBKGSubDxy"
else
  ## Histo names for IP template builder
  inputHistoNamesForIPTemplates="hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_1;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_2;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_4;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_5;hMuonDxySimplifiedMergedTemplate${smearedIPTemplatesString}_3"

  ## File and histo names for Prompt IP template builder
#   inputFileNameForPromptIPTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelector}.root"
  inputFileNameForPromptIPTemplate="../MuonBFractionsAnalysis/MuonPromptTemplateDataProducer__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__muonV6Pt4.root"
  inputHistoNameForPromptIPTemplate="hMuonUpsilonBKGSubDxy"
fi ## Switch on Prompt IP template (from Data or MC?)

### Histo names for PtRel template builder
if [[ $useDFromDataPtRel == "1" && $combineDAndCPtRel == "1" ]]
then
  inputHistoNamesForPtRelTemplates="hMuon${fitVar}JetSimplifiedMergedV2Template_1;hMuon${fitVar}JetSimplifiedMergedV2Template_2;hMuon${fitVar}JetLightTemplate"
else
  inputHistoNamesForPtRelTemplates="hMuon${fitVar}JetSimplifiedMergedV1Template_1;hMuon${fitVar}JetSimplifiedMergedV1Template_2;hMuon${fitVar}JetSimplifiedMergedV1Template_3"
fi


#####################################################
#                                                   #
#           SET DISTRIBUTION HISTO NAMES            #
#                                                   #
#####################################################

#####################################################
### Define input data distributions to be used
### Histo names for IP distributions
if [ ${use1D} == "1" ]
then
#   inputHistoNameForIPDistribution="hMuonDxy"
  inputHistoNameForIPDistribution=( [0]="hMuonDxy" [1]="hMuonOSDxy" [2]="hMuonSSDxy" [3]="hMuonSSPlusDxy" [4]="hMuonSSMinusDxy" )
else
#   inputHistoNameForIPDistribution="hMuonDxy2D"
  inputHistoNameForIPDistribution=( [0]="hMuonDxy2D" [1]="hMuonOSDxy2D" [2]="hMuonSSDxy2D" [3]="hMuonSSPlusDxy2D" [4]="hMuonSSMinusDxy2D" )
fi
inputHistoNamesForStep1FakeIPDistributions="hMuonDxySimplifiedMergedTemplate_1;hMuonDxySimplifiedMergedTemplate_2;hMuonDxySimplifiedMergedTemplate_3;hMuonDxySimplifiedMergedTemplate_4;hMuonDxySimplifiedMergedTemplate_5"
outputHistoNamesForStep1FakeIPDistributions="B;C;P;D;F"
inputHistoNamesForFakeIPDistributions="hCC;hBC"
nEventsPerComponent="12000-0"

#####################################################
### Define input data distributions to be used
### Histo names for IP distributions
if [ ${use1D} == "1" ]
then
#   inputHistoNameForPtRelDistribution="hMuon${fitVar}"
  inputHistoNameForPtRelDistribution=( [0]="hMuon${fitVar}Jet" [1]="hMuonOS${fitVar}Jet" [2]="hMuonSS${fitVar}Jet" [3]="hMuonSSPlus${fitVar}Jet" [4]="hMuonSSMinus${fitVar}Jet" )
else
#   inputHistoNameForPtRelDistribution="hMuonDxy2D"
  inputHistoNameForPtRelDistribution=( [0]="hMuon${fitVar}Jet2D" [1]="hMuonOS${fitVar}Jet2D" [2]="hMuonSS${fitVar}Jet2D" [3]="hMuonSSPlus${fitVar}Jet2D" [4]="hMuonSSMinus${fitVar}Jet2D" )
fi
inputHistoNamesForStep1FakePtRelDistributions="hMuon${fitVar}JetSimplifiedMergedV0Template_1;hMuon${fitVar}JetSimplifiedMergedV0Template_2;hMuon${fitVar}JetSimplifiedMergedV0Template_3;hMuon${fitVar}JetSimplifiedMergedV0Template_4;hMuon${fitVar}JetSimplifiedMergedV0Template_5"
outputHistoNamesForStep1FakePtRelDistributions="bb;cc;bc;pp;ss;bc;bs;cs"
inputHistoNamesForFakePtRelDistributions="hcc;hbc"

#####################################################
#                                                   #
#    SET TEMPLATE HISTO NAMES AND CONSTRAINTS       #
#                                                   #
#####################################################

#####################################################
### Define output histograms and templates to be used
### Histo names for IP template builder
# outputHistoNamesForIPTemplates="B;C;D;F;P" # 1;2;4;5;3
# if prompt from data is used, "P" MUST be the last in the list
outputHistoNamesForIPTemplates="B;C;D;F;P" # 1;2;4;5;3
templatesIPToUseFor1D="B;C;D"
templatesIPToUseFor2D="BB;CC;BC;DD;BD;CD"

#####################################################
### Define output histograms and templates to be used
### Histo names for PtRel template builder
# outputHistoNamesForPtRelTemplates="b;c;s;f;p" # 1;2;4;5;3
outputHistoNamesForPtRelTemplates="b;s;l" # 1;2;4;5;3
templatesPtRelToUseFor1D="b;s;l"
templatesPtRelToUseFor2D="bb;bs;bl;ss;sl;ll"

######################
### Define constraints
### These are implemented as constraints to ratios
### For example, if component 0 is -1 and component 3 is 0
### this means that component 0 is free and component 3 is
### constrained to be Component3 = Component0*Value3/Value0
# constraintToComponentIP="-1;-1;0;-1;-1;0;1"
# constraintValueIP="0.;0.;0.;0.;0.;0.;0."
# constraintErrorIP="0.;0.;0.;0.;0.;0.;0."

# 1D IP Fit
constraintToComponentIP1D=( [0]="-1;-1;-1" [1]="-1;-1;-1" [2]="-1;-1;-1" [3]="-1;-1;-1" [4]="-1;-1;-1" )
constraintValueIP1D=(       [0]="0.72196;0.19457;0.08346" [1]="0.;0.;0." [2]="0.;0.;0." [3]="0.;0.;0." [4]="0.;0.;0." )
constraintErrorIP1D=(       [0]="0.00005;0.00006;0.00006" [1]="0.;0.;0." [2]="0.;0.;0." [3]="0.;0.;0." [4]="0.;0.;0." )

# 1D PtRel Fit
constraintToComponentPtRel1D=( [0]="-1;-1;-1" [1]="-1;-1;-1" [2]="-1;-1;-1" [3]="-1;-1;-1" [4]="-1;-1;-1" )
constraintValuePtRel1D=(       [0]="0.57973;0.13923;0.28105" [1]="0.60748;0.08993;0.30259" [2]="0.53019;0.22719;0.24261" [3]="0.53111;0.22486;0.24403" [4]="0.52928;0.22953;0.24119" )
constraintErrorPtRel1D=(       [0]="0.00005;0.00006;0.00006" [1]="0.00007;0.00008;0.00008" [2]="0.00012;0.00013;0.00013"[3]="0.00020;0.00022;0.00022" [4]="0.00020;0.00022;0.00022" )

# 2D IP Fit
constraintToComponentIP=( [0]="-1;-1;0;-1;0;1"  [1]="-1;-1;0;-1;0;1"  [2]="-1;-1;0;-1;0;1"  [3]="-1;-1;0;-1;0;1"  [4]="-1;-1;0;-1;0;1"  )
constraintValueIP=(       [0]="0.65249;0.13340;0.06908;0.02078;0.07195;0.05230" [1]="0.64528;0.18700;0.05376;0.01654;0.05705;0.04037" [2]="0.66540;0.03746;0.09652;0.02837;0.09861;0.07364" [3]="0.66337;0.03829;0.09566;0.02970;0.10116;0.07182" [4]="0.66744;0.03661;0.09737;0.02704;0.09606;0.07547" )
constraintErrorIP=(       [0]="0.00009;0.00010;0.00010;0.00008;0.00010;0.00009" [1]="0.00012;0.00015;0.00013;0.00010;0.00013;0.00013" [2]="0.00019;0.00019;0.00022;0.00018;0.00022;0.00021" [3]="0.00032;0.00032;0.00037;0.00031;0.00037;0.00036" [4]="0.00032;0.00032;0.00037;0.00030;0.00037;0.00036" )

# 2D PtRel Fit
constraintToComponentPtRel=( [0]="-1;-1;-1;1;2;-1"  [1]="-1;-1;-1;1;2;-1"  [2]="-1;-1;-1;1;2;-1"  [3]="-1;-1;-1;1;2;-1"  [4]="-1;-1;-1;1;2;-1"  )
constraintValuePtRel=(       [0]="0.42315;0.19804;0.11511;0.02591;0.02859;0.20919" [1]="0.50596;0.11128;0.09177;0.02342;0.02174;0.24584" [2]="0.27537;0.35288;0.15677;0.03034;0.04083;0.14381" [3]="0.27818;0.34840;0.15747;0.02996;0.04140;0.14459" [4]="0.27255;0.35737;0.15608;0.03071;0.04026;0.14302" )
constraintErrorPtRel=(       [0]="0.00010;0.00010;0.00010;0.00008;0.00008;0.00010" [1]="0.00013;0.00014;0.00014;0.00011;0.00011;0.00014" [2]="0.00022;0.00021;0.00022;0.00018;0.00019;0.00022" [3]="0.00037;0.00036;0.00038;0.00031;0.00032;0.00038" [4]="0.00037;0.00036;0.00038;0.00031;0.00032;0.00038" )




##############################
### Define constraints on sums
### These are implemented as constraints on the sums of components
### For example, if constraintSumToComponentIP has an entry of value 310,
### this constraints the sum of the 0th, 1st, and 3rd component to be
### equal to the value in constraintSumValueIP, with error
### constraintSumErrorIP

# 2D PtRel Fit
# 310 -> bb+bs+ss = BB from IP FIT (hardcoded) ### FIXME ###
constraintSumToComponentIP=( [0]="310"   [1]="310"   [2]="310"   [3]="310"   [4]="310"   )
constraintSumValueIP=(       [0]="0.599" [1]="0.599" [2]="0.599" [3]="0.599" [4]="0.599" )
constraintSumErrorIP=(       [0]="0.004" [1]="0.004" [2]="0.004" [3]="0.004" [4]="0.004" )

# 1D PtRel Fit
constraintSumToComponentIP1D=( [0]="12"     [1]="12"     [2]="12"     [3]="12"     [4]="12"     )
constraintSumValueIP1D=(       [0]="0.6642" [1]="0.4775" [2]="0.8322" [3]="0.8262" [4]="0.8383" )
constraintSumErrorIP1D=(       [0]="0.0016" [1]="0.0013" [2]="0.0037" [3]="0.0052" [4]="0.0053" )



#####################################################
#                                                   #
#           SET WHICH DISTRIBUTION TO FIT           #
#                                                   #
#####################################################
distr2FitName=([0]="" [1]="OS" [2]="SS" [3]="SSPlus" [4]="SSMinus")


# for Distr2Fit in 0 1 2 3 4  # Fit all samples
# for Distr2Fit in 0 1 2  # Fit Overall / OS / SS
for Distr2Fit in 1 2  # Fit OS / SS
# for Distr2Fit in 1  # Fit OS
# for Distr2Fit in 0  # Fit Overall
do    

  ########################################
  # No changes by user below this point! #
  ########################################


echo " "
echo " *****************************************************"
echo " #####################################################"
echo " #                                                   #"
echo " #       START RUNNING ON ${distr2FitName[Distr2Fit]}     "
echo " #                                                   #"
echo " #####################################################"
echo " *****************************************************"
echo " "

  # 1 - run template producer on MC sample and build templates with correct axis-binning
  # 1.1 - run template producer
  if [ $runProducers == "1" ]
  then
    cd ../MuonTemplatesProducer
    echo " #########################"
    echo " # RUN TEMPLATE PRODUCER #"
    echo " #########################"
    root -l -b -q runMuonPtRelTemplatesProducer.cc\(\"${MCSample}\"\,\"${muonSelector}\",-1\)
    cp MuonTemplatesProducer__Histograms__${MCSample}__${muonSelector}.root ../MuonBFractionsAnalysis/Templates/RawTemplates__${MCSample}__${muonSelector}.root
    cd ../MuonBFractionsAnalysis
  fi

  # 1.2 - run template builder
  if [ $runBuilders == "1" ]
  then
    cd ../MuonTemplatesBuilder
    # 1.2.1 - switch prompt IP template and run IP template builder
    if [ $usePromptFromDataIP == "1" ]
    then
      if [ $fitTemplatesIP == "1" ]
      then   
        echo " ###########################"
        echo " # RUN TEMPLATE IP BUILDER #"
        echo " #  use Prompt from Data   #"
        echo " #     FIT TEMPLATES       #"
        echo " ###########################"    
        root -l -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/Templates/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Templates/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root\"\,\"${inputHistoNamesForIPTemplates}\"\,\"${outputHistoNamesForIPTemplates}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\,\"${inputFileNameForPromptIPTemplate}\"\,\"${inputHistoNameForPromptIPTemplate}\"\)
      else
        echo " ###########################"
        echo " # RUN TEMPLATE IP BUILDER #"
        echo " # use Prompt from Data    #"
        echo " ###########################"
        root -l -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/Templates/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Templates/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root\"\,\"${inputHistoNamesForIPTemplates}\"\,\"${outputHistoNamesForIPTemplates}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\,\"${inputFileNameForPromptIPTemplate}\"\,\"${inputHistoNameForPromptIPTemplate}\"\)
      fi
    else
      if [ $fitTemplatesIP == "1" ]
      then
        echo " ###########################"
        echo " # RUN TEMPLATE IP BUILDER #"
        echo " #   use Prompt from MC    #"
        echo " #     FIT TEMPLATES       #"
        echo " ###########################"
        root -l -b -q runMuonTemplatesBuilderWithFit.cc\(\"../MuonBFractionsAnalysis/Templates/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Templates/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root\"\,\"${inputHistoNamesForIPTemplates}\"\,\"${outputHistoNamesForIPTemplates}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\)
      else
        echo " ###########################"
        echo " # RUN TEMPLATE IP BUILDER #"
        echo " #   use Prompt from MC    #"
        echo " ###########################"
        root -l -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/Templates/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Templates/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root\"\,\"${inputHistoNamesForIPTemplates}\"\,\"${outputHistoNamesForIPTemplates}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\)
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
      ####root -l -b -q runMuonTemplatesProducer.cc\(\"${dataSample}\"\,\"${muonSelector}\"\,-1\)
      cp MuonTemplatesProducer__Histograms__${dataSample}__${muonSelector}.root ../MuonBFractionsAnalysis/FakeDistributions/RawFakeIPDistributions__${dataSample}__${muonSelector}.root
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
      root -l -b -q runMuonTemplatesBuilder.cc\(\"../MuonBFractionsAnalysis/FakeDistributions/RawFakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/FakeDistributions/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForStep1FakeIPDistributions}\"\,\"${outputHistoNamesForStep1FakeIPDistributions}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\)
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
        root -l -b -q runMuon1DIPDistributionsBuilderFromTemplates.cc\(\"../MuonBFractionsAnalysis/FakeDistributions/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/FakeDistributions/FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForFakeIPDistributions}\"\,\"${nEventsPerComponent}\"\,${fitIPMin}\,${fitIPMax}\)
      else
        echo " ###################################"
        echo " # RUN DISTRIBUTIONS IP BUILDER 2D #"
        echo " # for Fake IP                     #"
        echo " ###################################"
        root -l -b -q runMuon2DIPDistributionsBuilderFromTemplates.cc\(\"../MuonBFractionsAnalysis/FakeDistributions/Step1FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/FakeDistributions/FakeIPDistributions__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNamesForFakeIPDistributions}\"\,\"${nEventsPerComponent}\"\,${fitIPMin}\,${fitIPMax}\)
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
      root -l -b -q runMuonDistributionsProducer.cc\(\"${dataSample}\"\,\"${muonSelector}\",-1\)
      cp MuonDistributionsProducer__Histograms__${dataSample}__${muonSelector}.root ../MuonBFractionsAnalysis/Distributions/RawDistributions__${dataSample}__${muonSelector}.root
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
        root -l -b -q runMuon1DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/Distributions/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Distributions/IPDistributions${distr2FitName[Distr2Fit]}__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForIPDistribution[Distr2Fit]}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\)
      else
        echo " ###################################"
        echo " # RUN DISTRIBUTIONS IP BUILDER 2D #"
        echo " ###################################"
        root -l -b -q runMuon2DIPDistributionsBuilderFromIPDistributions.cc\(\"../MuonBFractionsAnalysis/Distributions/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Distributions/IPDistributions${distr2FitName[Distr2Fit]}__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForIPDistribution[Distr2Fit]}\"\,${fitIPMin}\,${fitIPMax}\,${rebinFactorIP}\,${removeBinIP}\)
      fi
    fi
  fi


  if [ $fakeIP == 1 ]
  then
    dataFileIP=../MuonBFractionsAnalysis/FakeDistributions/FakeIPDistributions__${dataSample}__${muonSelector}.root
  else
    dataFileIP=../MuonBFractionsAnalysis/Distributions/IPDistributions${distr2FitName[Distr2Fit]}__${dataSample}__${muonSelector}.root
  fi
      
  MCFileIP=../MuonBFractionsAnalysis/Templates/IPTemplates__${MCSample}__${muonSelector}${smearedIPTemplatesString}.root
      
  # 3 - now call the fitter function
  if [ $runFit == "1" ]
  then
    cd ../BinnedLogLikelihood
    if [ $use1D == "1" ]
    then
      if [ $constrainFractionsIP == "1" ]
      then
        echo " ######################################"
        echo " # RUN IP 1D FIT - CONSTR ON RATIO    #"
        echo " ######################################"
        root -l -b -q runBinnedLogLikelihoodCalculator1DConstrained.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor1D}\"\,\"${constraintToComponentIP1D[Distr2Fit]}\"\,\"${constraintValueIP1D[Distr2Fit]}\"\,\"${constraintErrorIP1D[Distr2Fit]}\"\)
      elif [ $constrainFractionsIP == "2" ]
      then
        echo " ######################################"
        echo " # RUN IP 1D FIT - CONSTR ON SUM      #"
        echo " ######################################"
        root -l -b -q runBinnedLogLikelihoodCalculator1DConstrainedSum.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor1D}\"\,\"${constraintSumToComponentIP1D[Distr2Fit]}\"\,\"${constraintSumValueIP1D[Distr2Fit]}\"\,\"${constraintSumErrorIP1D[Distr2Fit]}\"\)
      else
        echo " ######################################"
        echo " # RUN IP 1D FIT - NO CONSTR          #"
        echo " ######################################"      
        root -l -b -q runBinnedLogLikelihoodCalculator1D.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor1D}\"\)
      fi
      mv FitResults.txt ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResultsIP1D_${distr2FitName[Distr2Fit]}_${muonSelector}${smearedIPTemplatesString}${commstr}.txt
      mv FitOutput.root ../MuonBFractionsAnalysis/FitResults/${dirname}/FitOutputIP1D_${distr2FitName[Distr2Fit]}_${muonSelector}${smearedIPTemplatesString}${commstr}.root
    else
      if [ $constrainFractionsIP == "1" ]
      then
        echo " ######################################"
        echo " # RUN IP 2D FIT - CONSTR ON RATIO    #"
        echo " ######################################"    
        root -l -b -q runBinnedLogLikelihoodCalculator2DConstrained.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor2D}\"\,\"${constraintToComponentIP[Distr2Fit]}\"\,\"${constraintValueIP[Distr2Fit]}\"\,\"${constraintErrorIP[Distr2Fit]}\"\,\"${removeBinIP}\"\)
      elif [ $constrainFractionsIP == "2" ]
      then
        echo " ######################################"
        echo " # RUN IP 2D FIT - CONSTR ON SUM      #"
        echo " ######################################"    
        root -l -b -q runBinnedLogLikelihoodCalculator2DConstrainedSum.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor2D}\"\,\"${constraintSumToComponentIP[Distr2Fit]}\"\,\"${constraintSumValueIP[Distr2Fit]}\"\,\"${constraintSumErrorIP[Distr2Fit]}\"\,\"${removeBinIP}\"\)
      else
        echo " ######################################"
        echo " # RUN IP 2D FIT - NO CONSTR          #"
        echo " ######################################"
        root -l -b -q runBinnedLogLikelihoodCalculator2D.cc\(\"${dataFileIP}\"\,\"${MCFileIP}\"\,\"${templatesIPToUseFor2D}\"\,\"${removeBinIP}\"\)
      fi
      mv FitResults.txt ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResultsIP2D_${distr2FitName[Distr2Fit]}_${muonSelector}${smearedIPTemplatesString}${commstr}.txt
      mv FitOutput.root ../MuonBFractionsAnalysis/FitResults/${dirname}/FitOutputIP2D_${distr2FitName[Distr2Fit]}_${muonSelector}${smearedIPTemplatesString}${commstr}.root
    fi
  fi


  # combine the C and D into a single L template
  if [[ $useDFromDataPtRel == "1" && $combineDAndCPtRel == "1" ]]
  then
    if [ $use1D == "0" ]
    then
      CFractionFromIPFit=`../MuonBFractionsAnalysis/getFractionFrom2DIPFits.sh ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResultsIP2D_${distr2FitName[Distr2Fit]}_${muonSelector}${smearedIPTemplatesString}${commstr}.txt C`
      DFractionFromIPFit=`../MuonBFractionsAnalysis/getFractionFrom2DIPFits.sh ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResultsIP2D_${distr2FitName[Distr2Fit]}_${muonSelector}${smearedIPTemplatesString}${commstr}.txt D`
    else
      CFractionFromIPFit=`../MuonBFractionsAnalysis/getFractionFrom1DIPFits.sh ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResultsIP1D_${distr2FitName[Distr2Fit]}_${muonSelector}${smearedIPTemplatesString}${commstr}.txt C`
      DFractionFromIPFit=`../MuonBFractionsAnalysis/getFractionFrom1DIPFits.sh ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResultsIP1D_${distr2FitName[Distr2Fit]}_${muonSelector}${smearedIPTemplatesString}${commstr}.txt D`
    fi

    SumCDFractionsFromIPFit=`echo $CFractionFromIPFit+$DFractionFromIPFit | bc`
    CFractionFromIPFit=`echo "scale=7;$CFractionFromIPFit/$SumCDFractionsFromIPFit" | bc`
    DFractionFromIPFit=`echo "scale=7;$DFractionFromIPFit/$SumCDFractionsFromIPFit" | bc`
    
    echo " "
    echo "**** Calculation of the fractions in the combined PtRel light template ****"
    echo From the IP fit, the combined L \(i.e. C+D\) template is made up of $CFractionFromIPFit of C and of $DFractionFromIPFit of D
    echo " "
    
    cd ../MuonHistogramsMerger

    echo " ###################################"
    echo " # ddL = $CFractionFromIPFit C + $DFractionFromIPFit ddD #"
    echo " ###################################"
    echo " "

    root -l -b -q runMuonHistogramMergerWithCopy.cc\(\"../MuonBFractionsAnalysis/Templates/RawTemplates__${MCSample}__${muonSelector}.root\"\,\"../MuonPtRelTemplatesProducerFromData/MuonPtRelTemplatesProducerFromData__Histograms__Data__Mu__Run2010B-Nov4ReReco_v1__DoubleMu3_v2-AllSelections_V3__147146-149711__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Templates/RawPtRelTemplatesWithDataDriven__${MCSample}__${muonSelector}.root\"\,\"${histoNameFromMCToMergePtRel}\"\,\"${histoNameFromDataToMergePtRel}\"\,\"${histoNameMergedLightPtRel}\",${CFractionFromIPFit},\"${histoNamesToCopyForPtRelTemplates}\"\)
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
  #       rawPtRelTemplatesFileName=../MuonPtRelTemplatesProducerFromData/MuonPtRelTemplatesProducerFromData__Histograms__${MCSample}__${muonSelector}.root
        rawPtRelTemplatesFileName=../MuonBFractionsAnalysis/Templates/RawPtRelTemplatesWithDataDriven__${MCSample}__${muonSelector}.root
      else      
        if [[ -z "$correction" ]]
        then
          rawPtRelTemplatesFileName=../MuonBFractionsAnalysis/Templates/RawTemplates__${MCSample}__${muonSelector}.root
        else
          rawPtRelTemplatesFileName=../CorrectMuonPtRelTemplatesProducer/CorrectMuonPtRelTemplatesProducer${correction}__Histograms__${corrMCSample}__${muonSelector}.root
        fi      
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
        if [[ ${fitVar} = *PtOut* ]]
        then
          root -l -b -q runMuonTemplatesBuilder.cc\(\"${rawPtRelTemplatesFileName}\"\,\"../MuonBFractionsAnalysis/Templates/PtRelTemplates${correction}__${MCSample}__${muonSelector}.root\"\,\"${inputHistoNamesForPtRelTemplates}\"\,\"${outputHistoNamesForPtRelTemplates}\"\,${fitPtRelMin}\,${fitPtRelMax}\,${rebinFactorPtRel}\,${removeBinPtRel}\)
        elif [[ ${fitVar} = *Emu* ]]
        then
          root -l -b -q runMuonTemplatesBuilder.cc\(\"${rawPtRelTemplatesFileName}\"\,\"../MuonBFractionsAnalysis/Templates/PtRelTemplates${correction}__${MCSample}__${muonSelector}.root\"\,\"${inputHistoNamesForPtRelTemplates}\"\,\"${outputHistoNamesForPtRelTemplates}\"\,${fitEmuMin}\,${fitEmuMax}\,${rebinFactorEmu}\,${removeBinEmu}\)
        fi
      fi
      cd ../MuonBFractionsAnalysis
      cd ../MuonDistributionsBuilder
      # 3.1 - switch 1D or 2D templates
      if [ $use1D == "1" ]
      then
        echo " ######################################"
        echo " # RUN DISTRIBUTIONS PtRel BUILDER 1D #"
        echo " ######################################"
        if [[ ${fitVar} = *PtOut* ]]
        then
          root -l -b -q runMuon1DPtRelDistributionsBuilderFromPtRelDistributions.cc\(\"../MuonBFractionsAnalysis/Distributions/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Distributions/PtRelDistributions${distr2FitName[Distr2Fit]}__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForPtRelDistribution[Distr2Fit]}\"\,${fitPtRelMin}\,${fitPtRelMax}\,${rebinFactorPtRel}\)
        elif [[ ${fitVar} = *Emu* ]]
        then
          root -l -b -q runMuon1DPtRelDistributionsBuilderFromPtRelDistributions.cc\(\"../MuonBFractionsAnalysis/Distributions/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Distributions/PtRelDistributions${distr2FitName[Distr2Fit]}__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForPtRelDistribution[Distr2Fit]}\"\,${fitEmuMin}\,${fitEmuMax}\,${rebinFactorEmu}\)
        fi      
      else
        echo " ######################################"
        echo " # RUN DISTRIBUTIONS PtRel BUILDER 2D #"
        echo " ######################################"
        
        if [[ ${fitVar} = *PtOut* ]]
        then
          root -l -b -q runMuon2DPtRelDistributionsBuilderFromPtRelDistributions.cc\(\"../MuonBFractionsAnalysis/Distributions/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Distributions/PtRelDistributions${distr2FitName[Distr2Fit]}__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForPtRelDistribution[Distr2Fit]}\"\,${fitPtRelMin}\,${fitPtRelMax}\,${rebinFactorPtRel}\,${removeBinPtRel}\)
        elif [[ ${fitVar} = *Emu* ]]
        then
          root -l -b -q runMuon2DPtRelDistributionsBuilderFromPtRelDistributions.cc\(\"../MuonBFractionsAnalysis/Distributions/RawDistributions__${dataSample}__${muonSelector}.root\"\,\"../MuonBFractionsAnalysis/Distributions/PtRelDistributions${distr2FitName[Distr2Fit]}__${dataSample}__${muonSelector}.root\"\,\"${inputHistoNameForPtRelDistribution[Distr2Fit]}\"\,${fitEmuMin}\,${fitEmuMax}\,${rebinFactorEmu}\,${removeBinEmu}\)
        fi            
      fi
    fi
  fi


  if [ $fakePtRel == 1 ]
  then
    dataFilePtRel=../MuonBFractionsAnalysis/FakeDistributions/FakePtRelDistributions__${dataSample}__${muonSelector}.root
  else
    dataFilePtRel=../MuonBFractionsAnalysis/Distributions/PtRelDistributions${distr2FitName[Distr2Fit]}__${dataSample}__${muonSelector}.root
  fi

  MCFilePtRel=../MuonBFractionsAnalysis/Templates/PtRelTemplates${correction}__${MCSample}__${muonSelector}.root

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
          echo " ######################################"
          echo " # RUN PtRel 1D FIT - CONSTR ON RATIO #"
          echo " ######################################"
          root -l -b -q runBinnedLogLikelihoodCalculator1DConstrained.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor1D}\"\,\"${constraintToComponentPtRel1D[Distr2Fit]}\"\,\"${constraintValuePtRel1D[Distr2Fit]}\"\,\"${constraintErrorPtRel1D[Distr2Fit]}\"\)
        elif [ $constrainFractionsPtRel == "2" ]
        then
          echo " ######################################"
          echo " # RUN PtRel 1D FIT - CONSTR ON SUM   #"
          echo " ######################################"
          root -l -b -q runBinnedLogLikelihoodCalculator1DConstrainedSum.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor1D}\"\,\"${constraintSumToComponentIP1D[Distr2Fit]}\"\,\"${constraintSumValueIP1D[Distr2Fit]}\"\,\"${constraintSumErrorIP1D[Distr2Fit]}\"\)
        else
          echo " ######################################"
          echo " # RUN PtRel 1D FIT - NO CONSTR       #"
          echo " ######################################"
          root -l -b -q runBinnedLogLikelihoodCalculator1D.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor1D}\"\)
#           root -l -b -q runTFractionFitterCalculator1D.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor1D}\"\)      
        fi
        mv FitResults.txt ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResults${fitVar}1D_${distr2FitName[Distr2Fit]}${correction}_${muonSelector}${commstr}.txt
        mv FitOutput.root ../MuonBFractionsAnalysis/FitResults/${dirname}/FitOutput${fitVar}1D_${distr2FitName[Distr2Fit]}${correction}_${muonSelector}${commstr}.root
      else
        if [ $constrainFractionsPtRel == "1" ]
        then
          echo " ######################################"
          echo " # RUN PtRel 2D FIT - CONSTR ON RATIO #"
          echo " ######################################"
          root -l -b -q runBinnedLogLikelihoodCalculator2DConstrained.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor2D}\"\,\"${constraintToComponentPtRel[Distr2Fit]}\"\,\"${constraintValuePtRel[Distr2Fit]}\"\,\"${constraintErrorPtRel[Distr2Fit]}\"\,\"${removeBinPtRel}\"\)
        elif [ $constrainFractionsPtRel == "2" ]
        then
          echo " ######################################"
          echo " # RUN PtRel 2D FIT - CONSTR ON SUM   #"
          echo " ######################################"
          root -l -b -q runBinnedLogLikelihoodCalculator2DConstrainedSum.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor2D}\"\,\"${constraintSumToComponentPtRel}\"\,\"${constraintSumValuePtRel}\"\,\"${constraintSumErrorPtRel}\"\,\"${removeBinPtRel}\"\)
        else
          echo " ######################################"
          echo " # RUN PtRel 2D FIT - NO CONSTR       #"
          echo " ######################################"
          root -l -b -q runBinnedLogLikelihoodCalculator2D.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor2D}\"\,\"${removeBinPtRel}\"\)          
#           root -l -b -q runBinnedLogLikelihoodCalculator2DIterative.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor2D}\"\,\"${removeBinPtRel}\"\)
#           root -l -b -q runTFractionFitterCalculator2D.cc\(\"${dataFilePtRel}\"\,\"${MCFilePtRel}\"\,\"${templatesPtRelToUseFor2D}\"\,\"${removeBinPtRel}\"\)
        fi
        mv FitResults.txt ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResults${fitVar}2D_${distr2FitName[Distr2Fit]}${correction}_${muonSelector}${commstr}.txt
        mv FitOutput.root ../MuonBFractionsAnalysis/FitResults/${dirname}/FitOutput${fitVar}2D_${distr2FitName[Distr2Fit]}${correction}_${muonSelector}${commstr}.root
      fi
    fi
  fi

#   echo " ######################################"
#   echo " # FIT RESULTS - ${distr2FitName[Distr2Fit]}       "
#   echo " ######################################"
#   echo ""
# 
#   if [ $use1D == "0" ]
#   then
#     BFractionFromPtRelFit=`../MuonBFractionsAnalysis/getFractionFrom2DPtRelFits.sh ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResults${fitVar}2D_${distr2FitName[Distr2Fit]}${correction}_${muonSelector}${commstr}.txt bb`
#     BFractErrFromPtRelFit=`../MuonBFractionsAnalysis/getFractErrFrom2DPtRelFits.sh ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResults${fitVar}2D_${distr2FitName[Distr2Fit]}${correction}_${muonSelector}${commstr}.txt bb`
#     echo " ***********************************"
#     echo " bb fraction is $BFractionFromPtRelFit \pm $BFractErrFromPtRelFit"
#     echo " ***********************************"
#   else
#     BFractionFromPtRelFit=`../MuonBFractionsAnalysis/getFractionFrom1DPtRelFits.sh ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResults${fitVar}1D_${distr2FitName[Distr2Fit]}${correction}_${muonSelector}${commstr}.txt b`
#     BFractErrFromPtRelFit=`../MuonBFractionsAnalysis/getFractErrFrom1DPtRelFits.sh ../MuonBFractionsAnalysis/FitResults/${dirname}/FitResults${fitVar}1D_${distr2FitName[Distr2Fit]}${correction}_${muonSelector}${commstr}.txt b`
#     echo " ***********************************"
#     echo " b fraction is $BFractionFromPtRelFit \pm $BFractErrFromPtRelFit"
#     echo " ***********************************"
#   fi
  
done
  
  

if [ $use1D == "0" ]
then
  
  cd ../MuonBFractionsAnalysis/FitResults/${dirname}
  
  SSfile="FitResults${fitVar}2D_SS${correction}_${muonSelector}${commstr}.txt"
  OSfile="FitResults${fitVar}2D_OS${correction}_${muonSelector}${commstr}.txt"
    
  `./../../EvaluateChi.sh ${SSfile} ${OSfile} ${blindChi} ${forceChi}`

fi
      