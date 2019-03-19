#!/bin/bash

SSfile=$1
OSfile=$2
blindChi=$3
forceChi=$4
  
if [[ -f ${SSfile} ]] && [[ -f ${OSfile} ]]
then

  statusSS=`awk '/FitStatus/ {print $3}' $SSfile`
  statusOS=`awk '/FitStatus/ {print $3}' $OSfile`
  
  if [ "${statusSS}" = "0" ] && [ "${statusOS}" = "0" ]
  then
    echo "Starting chi evaluation (both SS/OS FitStatus = 0)..." >> ChiResults.txt
    echo " " >> ChiResults.txt
    computeChi=1
  else
    if [ "${forceChi}" != "0" ] 
    then
      echo "Forcing chi evaluation (even if FitStatus != 0)..." >> ChiResults.txt
      echo " " >> ChiResults.txt
      computeChi=1
    else
      echo " ----------------------------------------------------------"
      echo " "
      echo " E R R O R ! At least one fit did not converge..."
      echo "             STATUS FIT SS : ${statusSS}"
      echo "             STATUS FIT OS : ${statusOS}"
      echo "             Chi is not automatically computed..."
      echo " "      
      computeChi=0
    fi
  fi
    
  if [ "${computeChi}" != "0" ]
  then
          
    # # # get the parameters from the FitResults files
    fbbSS=`awk '/hbb/ {print $2}' $SSfile`
    fbberrSS=`awk '/hbb/ {print $3}' $SSfile`
    nSS="`awk '/nDataEvents/ {print $3}' $SSfile`."
    
    fbbOS=`awk '/hbb/ {print $2}' $OSfile`
    fbberrOS=`awk '/hbb/ {print $3}' $OSfile`
    if [ "${blindChi}" != "0" ]
    then
      nOS="200000."
    else
      nOS="`awk '/nDataEvents/ {print $3}' $OSfile`."
    fi
      
    # # # compute the values of chi and chierr
    nbbSS=`echo "scale = 30; $nSS * $fbbSS"| bc -l`
    nbbOS=`echo "scale = 30; $nOS * $fbbOS"| bc -l`
    
    p0=`echo "scale = 30; $nbbSS / ( $nbbSS + $nbbOS )" | bc -l`
    p1=`echo "scale = 30; $fbbSS^2 * $fbberrOS^2 + $fbbOS^2 * $fbberrSS^2" | bc -l`
    p2=`echo "scale = 30; $nbbOS^2 - $nbbSS^2" | bc -l`
    p3=`echo "scale = 30; sqrt( $p1 / $p2 )" | bc -l`
    p4=`echo "scale = 30; $nSS * $nOS / ( $nbbSS + $nbbOS )" | bc -l`
    
    chiVal=`echo "scale = 10; 0.5 * (1. - sqrt(1. - 2. * $p0 ))" | bc -l`
    chiErr=`echo "scale = 10; 0.5 * $p4 * $p3" | bc -l`
    
    # # # print the results on outfile
    echo "Same Sign" >> ChiResults.txt
    echo "" >> ChiResults.txt
    tail -n 10 ${SSfile} >> ChiResults.txt
    echo "" >> ChiResults.txt  
    echo " nSS(data) = $nSS" >> ChiResults.txt
    echo " fbb(ss)   = $fbbSS \pm $fbberrSS"  >> ChiResults.txt
    echo " Nbbss     = $nbbSS"  >> ChiResults.txt
    
    echo ""  >> ChiResults.txt
    
    echo "Opposite Sign" >> ChiResults.txt
    echo "" >> ChiResults.txt
    tail -n 10 ${OSfile} >> ChiResults.txt
    echo "" >> ChiResults.txt  
    echo " nOS(data) = $nOS"  >> ChiResults.txt
    echo " fbb(os)   = $fbbOS \pm $fbberrOS"  >> ChiResults.txt
    echo " Nbbos     = $nbbOS"  >> ChiResults.txt
    
    echo ""  >> ChiResults.txt
    
    echo " ***********************************"  >> ChiResults.txt
    echo " chi = $chiVal \pm $chiErr "  >> ChiResults.txt
    echo " ***********************************"  >> ChiResults.txt
    
    echo ""  >> ChiResults.txt    

  fi
else
  echo " ----------------------------------------------------------"
  echo " "
  echo " E R R O R ! One of the following files have not been foud "
  echo "             SSfile : ${SSfile}"
  echo "             OSfile : ${OSfile}"
  echo "             Chi is not automatically computed..."
  echo " "        
fi
