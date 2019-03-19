#!/bin/bash

if [[ $1 == '-h' || $1 == '--help' ]]
  then echo Instructions:
  echo "Invoke the script with $0 MACRONAME MODE"
  echo "   MACRONAME is the name of the new macro to create"
  echo "   MODE can be either:"
  echo "       \"reco\" to analyze ntuples containing only reco information"
  echo "              (Real data or MC)"
  echo "       \"recoV2\" to analyze ntuples produced before February 2011"
  echo "                containing only reco information (Real data or MC)"
  echo "       \"recoV1\" to analyze old-format ntuples containing only reco"
  echo "                information (Real data or MC)"
  echo "       \"MC\" to analyze ntuples containing reco and generator"
  echo "            information (MC only)"
  echo "       \"MCV2\" to analyze ntuples produced before February 2011"
  echo "              containing reco and generator information (MC only)"
  echo "       \"MCV1\" to analyze old-format ntuples containing reco and"
  echo "              generator information (MC only)"
  echo "EXAMPLE: $0 MyMacro reco"
  echo ""
  echo "The script will create a directory called MACRONAME"
  echo "and 3 files in it:"
  echo "   MACRONAME.h is the header file of the new nacro"
  echo "   MACRONAME.C is the file with the Loop() function"
  echo "       (this is the only file that the user normally"
  echo "        needs to modify)"
  echo "   runMACRONAME.cc is the file to be called to execute the macro"
  echo ""
  echo "MACRO EXECUTION"
  echo "Macro is executed by running the file runMACRONAME.cc from within ROOT:"
  echo "   root [0] .x runMACRONAME.cc(\"SAMPLENAME\",\"TEXT\",NEvts)"
  echo "The command arguments are:"
  echo "   SAMPLENAME is the name of the sample to analyze"
  echo "   TEXT is a free text (this is available from within the macro class,"
  echo "        in the variable \"std::string text\")"
  echo "   NEvts is the number of events to analyze (if NEvts = -1 then it runs on the"
  echo "         whole sample)"
  echo ""
  echo "INPUT FILES"
  echo "Input sample must be defined in the form of a text file, containing a list of"
  echo "ROOT ntuple files. The file must be located in the fileLists directory and"
  echo "its name must follow this convention:"
  echo "   fileList_SAMPLENAME.txt"
  echo "The string SAMPLENAME is the same that is used in the macro invokation."
  echo "Example files containing lists of ntuples in local or remote locations"
  echo "are already in the fileLists directory. It is suggested to be as descriptive"
  echo "as possible in the naming of the file lists. In particular, the sample name"
  echo "should always begin with the string \"MC__\" if it is a simulated one,"
  echo "or with the string \"Data__\" if it contains real data."
  echo ""
  echo "OUTPUT FILES"
  echo "Output file for histograms, canvases, etc, is automatically managed by"
  echo "the framework. Every time the macro is invoked, a new output file is created"
  echo "with the name:"
  echo "   MACRONAME__Histograms__SAMPLENAME__TEXT.root"
  echo "If the file already existed in the working directory, it is overwritten."
  echo ""
  echo "HISTOGRAM BOOKING AND PLOTTING"
  echo "An utility class to book and plot, with good-looking styles consistent"
  echo "across all the macros, 1D and 2D histograms and graphs, is provided in"
  echo "utilities/histoPlotter.C[h]. The class is perfectly usable and 90% complete."
  echo "Documentation is to be written..."
  echo ""
  echo "EVENT AND PHYSICS OBJECT SELECTION"
  echo "An utility class to define and apply selections in a consistent way across"
  echo "all the macros is provided in utilities/particleSelector.C[h]."
  echo "The class is currently about 50% complete, but some functions to select muons,"
  echo "generator particles and events are already available."
  echo "Documentation is to be written..."
  echo ""
  echo "PHYSICAL AND MATHEMATICAL CONSTANTS"
  echo "Although constants can be defined in the body of the macro, a file in which all"
  echo "the constants definitions can be collected is provided in utilities/constants.h."
  echo "Constants defined in this file will be automatically visible to all the macros."
  echo "Within a macro, a constant is called by using \"constants::CONSTANTNAME\". To"
  echo "avoid to use \"constants::\" before the constant name, you can write"
  echo "   using namespace constants;"
  echo "in MACRONAME.C, just before the beginning of the Loop() function."
  exit
fi

if [[ $2 == '' || $3 != '' ]]
  then 
    echo Script accepts exactly two arguments!
    echo Usage:
    echo   $0 MACRONAME MODE
    echo Write \"$0 --help\" for more help.
  exit
fi

if [[ $2 == 'reco' || $2 == 'recoV1' || $2 == "recoV2" ||  $2 == 'MC' || $2 == "MCV1" || $2 == "MCV2" ]]
  then echo Creating tree analyzer for $2
  else echo Second argument must be \"reco\", \"recoV1\", \"recoV2\", \"MC\", \"MCV1\" or \"MCV2\"! ; exit
fi

macroName=$1
mode=$2

if [[ $mode == 'reco' || $mode == "MC" ]]
  then ntupleVersion=4
fi
if [[ $mode == 'recoV2' || $mode == "MCV2" ]]
  then ntupleVersion=2
fi
if [[ $mode == 'recoV1' || $mode == "MCV1" ]]
  then ntupleVersion=1
fi

echo Name of the macro to be created: $macroName
echo Defining ntuple version: $ntupleVersion
echo

if [[ -e $macroName ]]
  then echo ERROR: directory $macroName exists!
  echo Aborting macro creation!
  exit
fi

echo Creating directory $macroName
mkdir $macroName

if [[ $mode == "MC" || $mode == "MCV1" || $mode == "MCV2" ]]
  then
  echo Creating $macroName/$macroName.h...
  if [[ -e $macroName/$macroName.h ]]
    then echo ERROR: file $macroName/$macroName.h exists!
    echo Aborting macro creation!
    exit
  fi
  sed "s#MACRONAMEMC#$macroName#g" Templates/template_MacroMC.h > $macroName/$macroName.h
  echo Creating $macroName/$macroName.C...
  if [[ -e $macroName/$macroName.C ]]
    then echo ERROR: file $macroName/$macroName.C exists!
    echo Aborting macro creation!
    exit
  fi
  sed "s#MACRONAMEMC#$macroName#g" Templates/template_MacroMC.C | sed "s#NTUPLEVERSIONNUMBER#$ntupleVersion#g" > $macroName/$macroName.C
  echo Creating $macroName/run$macroName.cc...
  if [[ -e $macroName/run$macroName.cc ]]
    then echo ERROR: file $macroName/run$macroName.cc exists!
    echo Aborting macro creation!
    exit
  fi
  sed "s#MACRONAMEMC#$macroName#g" Templates/template_runMacroMC.cc > $macroName/run$macroName.cc
  echo File creation completed! You can find the macro skeleton in directory ./$macroName
elif [[ $mode == "reco" || $mode == "recoV1" || $mode == "recoV2" ]]
  then
  echo Creating $macroName/$macroName.h...
  if [[ -e $macroName/$macroName.h ]]
    then echo ERROR: file $macroName/$macroName.h exists!
    echo Aborting macro creation!
    exit
  fi
  sed "s#MACRONAMERECO#$macroName#g" Templates/template_MacroReco.h > $macroName/$macroName.h
  echo Creating $macroName/$macroName.C...
  if [[ -e $macroName/$macroName.C ]]
    then echo ERROR: file $macroName/$macroName.C exists!
    echo Aborting macro creation!
    exit
  fi
  sed "s#MACRONAMERECO#$macroName#g" Templates/template_MacroReco.C | sed "s#NTUPLEVERSIONNUMBER#$ntupleVersion#g" > $macroName/$macroName.C
  echo Creating $macroName/run$macroName.cc...
  if [[ -e $macroName/run$macroName.cc ]]
    then echo ERROR: file $macroName/run$macroName.cc exists!
    echo Aborting macro creation!
    exit
  fi
  sed "s#MACRONAMERECO#$macroName#g" Templates/template_runMacroReco.cc > $macroName/run$macroName.cc
  echo File creation completed! You can find the macro skeleton in directory ./$macroName
else echo ERROR: unrecognized mode! Exiting.
  exit
fi
