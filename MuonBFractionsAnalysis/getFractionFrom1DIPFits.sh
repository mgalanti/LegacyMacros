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
  if [[ $name == "h${fractionName}" ]]
  then
    if [[ $fraction = *e-* ]]
    then
      fraction=0.     
    fi
#     echo h${fractionName}${fractionName} fraction is $fraction
#     echo totalFraction before sum is $totalFraction
    totalFraction=`echo $totalFraction+$fraction | bc`
 #    echo totalFraction after sum is $totalFraction
  fi
done

echo $totalFraction

IFS=$oldIFS