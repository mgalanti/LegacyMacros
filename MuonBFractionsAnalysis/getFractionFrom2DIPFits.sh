#!/bin/bash

oldIFS=$IFS
IFS=$'\n'

filename=$1

fractionName=$2

totalFraction=0

for line in `cat $filename`
do
#  echo $line
  name=`echo $line | awk '{print $1}'`
  fraction=`echo $line | awk '{print $2}'`
#  echo name is $name, fraction is $fraction 
  if [[ $name == "h${fractionName}${fractionName}" ]]
  then
#    echo h${fractionName}${fractionName} fraction is $fraction
#    echo totalFraction before sum is $totalFraction
   if [[ $fraction = *e-* ]]
   then
     fraction=0.     
   fi
    totalFraction=`echo $totalFraction+$fraction | bc`
    totalFraction=`echo $totalFraction+$fraction | bc`
#    echo totalFraction after sum is $totalFraction
  fi
  if [[ ( $name =~ h${fractionName}[[:upper:]] || $name =~ h[[:upper:]]${fractionName} ) && $name != "h${fractionName}${fractionName}" ]]
  then
    if [[ $fraction = *e-* ]]
    then
      fraction=0.     
    fi
#    echo single-component fraction $name is $fraction
#    echo totalFraction before sum is $totalFraction
    totalFraction=`echo $totalFraction+$fraction | bc`
#    echo totalFraction after sum is $totalFraction
  fi
done

totalFraction=`echo $totalFraction*0.5 | bc`
echo $totalFraction

IFS=$oldIFS