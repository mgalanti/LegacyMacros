#!/bin/bash

doPairChecks=0
doTripletChecks=0
doMissingDIFChecks=1

if [ doPairChecks == "1" ]
then
  fractions="200000-0_180000-20000_160000-40000_140000-60000_120000-80000_100000-100000"
  templateNames="BB;CC BB;PP BB;DD BB;BC BB;BP BB;BD BB;CP BB;CD BB;PD"
  inputHistoNames="hBB;hCC hBB;hPP hBB;hDD hBB;hBC hBB;hBP hBB;hBD hBB;hCP hBB;hCD hBB;hPD"

  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done
fi # doPairChecks



if [ doTripletChecks == "1" ]
then
  fractions="200000-0-0_180000-15000-5000_160000-30000-10000_140000-45000-15000_120000-60000-20000_100000-75000-25000"
  templateNames="BB;CC;BD BB;CD;CC BB;DD;BC BB;PP;DD BB;CD;BD BB;DD;CD"
  inputHistoNames="hBB;hCC;hBD hBB;hCD;hCC hBB;hDD;hBC hBB;hPP;hDD hBB;hCD;hBD hBB;hDD;hCD"
  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done

  fractions="200000-0-0_180000-10000-10000__160000-20000-20000_140000-30000-30000_120000-40000-40000_100000-50000-50000"
  templateNames="BB;CC;PP BB;CC;DD BB;DD;PP BB;CC;BC BB;CC;CD BB;BC;PP BB;BC;DD BB;BD;BC BB;BD;CC BB;BD;PP BB;BD;DD BB;BD;CD BB;CD;PP BB;CD;DD BB;CD;BC"
  inputHistoNames="hBB;hCC;hPP hBB;hCC;hDD hBB;hDD;hPP hBB;hCC;hBC hBB;hCC;hCD hBB;hBC;hPP hBB;hBC;hDD hBB;hBD;hBC hBB;hBD;hCC hBB;hBD;hPP hBB;hBD;hDD hBB;hBD;hCD hBB;hCD;hPP hBB;hCD;hDD hBB;hCD;hBC"

  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done

  fractions="200000-0-0_180000-15000-5000_160000-30000-10000_140000-45000-15000_120000-60000-20000_100000-75000-25000"
#   templateNames="BB;CC;PP BB;CC;DD BB;CC;BC BB;CC;BD BB;CC;CD BB;BC;PP BB;BD;PP BB;BD;CD"
#   inputHistoNames="hBB;hCC;hPP hBB;hCC;hDD hBB;hCC;hBC hBB;hCC;hBD hBB;hCC;hCD hBB;hBC;hPP hBB;hBD;hPP hBB;hBD;hCD"

  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done

  fractions="200000-0-0_180000-16000-4000_160000-32000-8000_140000-48000-12000_120000-64000-16000_100000-80000-20000"
#   templateNames="BB;CC;PP BB;CC;DD BB;CC;BC BB;CC;BD BB;CC;CD BB;BC;PP BB;BD;PP BB;BD;CD"
#   inputHistoNames="hBB;hCC;hPP hBB;hCC;hDD hBB;hCC;hBC hBB;hCC;hBD hBB;hCC;hCD hBB;hBC;hPP hBB;hBD;hPP hBB;hBD;hCD"

  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done

  fractions="200000-0-0_180000-17500-2500_160000-35000-5000_140000-52500-7500_120000-70000-10000_100000-87500-12500"
#   templateNames="BB;CC;PP BB;CC;DD BB;CC;BC BB;CC;BD BB;CC;CD BB;BC;PP BB;BD;PP BB;BD;CD"
#   inputHistoNames="hBB;hCC;hPP hBB;hCC;hDD hBB;hCC;hBC hBB;hCC;hBD hBB;hCC;hCD hBB;hBC;hPP hBB;hBD;hPP hBB;hBD;hCD"

  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done

  fractions="200000-0-0_180000-18000-2000_160000-36000-4000_140000-54000-6000_120000-72000-8000_100000-90000-10000"
#   templateNames="BB;CC;PP BB;CC;DD BB;CC;BC BB;CC;BD BB;CC;CD BB;BC;PP BB;BD;PP BB;BD;CD"
#   inputHistoNames="hBB;hCC;hPP hBB;hCC;hDD hBB;hCC;hBC hBB;hCC;hBD hBB;hCC;hCD hBB;hBC;hPP hBB;hBD;hPP hBB;hBD;hCD"

  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done
fi # doTripletChecks



if [ $doMissingDIFChecks == "1" ]
then

  fractions="200000-0-0_180000-10000-10000__160000-20000-20000_140000-30000-30000_120000-40000-40000_100000-50000-50000"
  templateNames="BB;CC BB;PP BB;CC BB;PP BB;CC BB;PP BB;CC BB;PP BB;CC BB;PP BB;CC BB;PP BB;CC BB;PP"
  inputHistoNames="hBB;hCC;hDD hBB;hPP;hDD hBB;hCC;hBD hBB;hPP;hBD hBB;hCC;hCD hBB;hPP;hCD hBB;hCC;hEE hBB;hPP;hEE hBB;hCC;hBE hBB;hPP;hBE hBB;hCC;hCE hBB;hPP;hCE hBB;hCC;hDE hBB;hPP;hDE"

  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done

  fractions="200000-0-0-0_180000-7500-2500-10000_160000-15000-5000-20000_140000-22500-7500-30000_120000-30000-10000-40000_100000-37500-12500-50000"
  templateNames="BB;CC;PP BB;CC;PP BB;CC;PP BB;CC;PP BB;CC;PP BB;CC;PP BB;CC;PP BB;CC;PP BB;CC;PP"
  inputHistoNames="hBB;hCC;hPP;hDD hBB;hCC;hPP;hEE hBB;hCC;hPP;hBD hBB;hCC;hPP;hCD hBB;hCC;hPP;hPD hBB;hCC;hPP;hDE hBB;hCC;hPP;hBE hBB;hCC;hPP;hCE hBB;hCC;hPP;hPE"

  fractions2=`echo ${fractions} | sed s#_#\ #g`
  counter=0
  for templateName in $templateNames
  do
    let counter++
    inputHistoName=`echo ${inputHistoNames} | awk -v j=$counter {'print $j'}`
    for fraction in $fractions2
    do
      ./MuonBFractionsAnalysis.sh ${fraction} ${templateName} ${inputHistoName}
    done

    root -b -q runPlotFractions2D.cc\(\"${fractions}\"\,\"${inputHistoName}\",\"${templateName}\"\)

  done

fi # doMissingDIFChecks
