#!/bin/bash

#=======================================
#======= PHY14 samples

##tight
#qsub job.sh "PHYS14" 11 "" "tightcut"  0.1 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "" "tightcut"  0.1 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "" "tightcut"  0.1 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "" "tightcut"  0.1 "eta" "Pt" "" 
##reliso
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.1 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.1 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.1 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.1 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.15 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.15 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.15 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.15 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.2 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.2 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.2 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.2 "eta" "Pt" "" 
###dxy
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.005 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.005 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.005 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.005 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.01 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.01 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.01 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.01 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.02 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.02 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.02 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.02 "eta" "Pt" "" 
###dz
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.01 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.01 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.01 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.01 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.02 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.02 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.02 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.02 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.03 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.03 "Pt" "eta" "" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.03 "eta" "Pt" "" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.03 "eta" "Pt" "" 

##T&P MC true
##tight
#qsub job.sh "PHYS14" 11 "" "tightcut"  0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "" "tightcut"  0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "" "tightcut"  0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "" "tightcut"  0.1 "eta" "Pt" "matching dyonly" 
###reliso
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.15 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.15 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.15 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.15 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.2 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.2 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "reliso3" 0.2 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "reliso3" 0.2 "eta" "Pt" "matching dyonly" 
##dxy
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.005 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.005 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.005 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.005 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.01 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.01 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.01 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.01 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.02 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.02 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dxy" 0.02 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dxy" 0.02 "eta" "Pt" "matching dyonly" 
##dz
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.01  "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.01  "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.01  "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.01  "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.02  "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.02  "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.02  "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.02  "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.03 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.03 "Pt" "eta" "matching dyonly" 
#qsub job.sh "PHYS14" 11 "tightcut" "dz" 0.03 "eta" "Pt" "matching dyonly" 
#qsub job.sh "PHYS14" 13 "tightcut" "dz" 0.03 "eta" "Pt" "matching dyonly" 

#with already defined bins
#Take all probes
#TP
#tight
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.1 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.1 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.1 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.1 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.1 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.15 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.15 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.15 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.15 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.2 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.2 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.2 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.2 "eta" "Pt" "" 
##reliso
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.1 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.1 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.1 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.1 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.15 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.15 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.15 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.15 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.2 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.2 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.2 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.2 "eta" "Pt" "" 
qsub job.sh "ResultCombi2" 13 "tightcut" "chiso3" 0.1 "Pt" "eta" "" 
qsub job.sh "ResultCombi2" 13 "tightcut" "chiso3" 0.1 "eta" "Pt" "" 
##dxy
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.005 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.005 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.005 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.005 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.01 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.01 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.01 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.01 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.02 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.02 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.02 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.02 "eta" "Pt" "" 
##dz
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.01 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.01 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.01 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.01 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.02 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.02 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.02 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.02 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.03 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.03 "Pt" "eta" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.03 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.03 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.5 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.2 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.1 "eta" "Pt" "" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.05 "eta" "Pt" "" 
##T&P MC true
##tight
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.15 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.15 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.15 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.15 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.2 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.2 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.2 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.2 "eta" "Pt" "matching dyonly" 
##reliso
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.15 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.15 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.15 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.15 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.2 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.2 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.2 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.2 "eta" "Pt" "matching dyonly" 
qsub job.sh "ResultCombi2" 13 "tightcut" "chiso3" 0.1 "Pt" "eta" "matching dyonly" 
qsub job.sh "ResultCombi2" 13 "tightcut" "chiso3" 0.1 "eta" "Pt" "matching dyonly" 
##dxy
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.005 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.005 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.005 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.005 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.01 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.01 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.01 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.01 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.02 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.02 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.02 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.02 "eta" "Pt" "matching dyonly" 
##dz
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.01  "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.01  "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.01  "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.01  "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.02  "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.02  "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.02  "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.02  "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.03 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.03 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.03 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.03 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.5 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.2 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.05 "eta" "Pt" "matching dyonly" 
##closest M
##reliso
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.1 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.1 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.1 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.1 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.15 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.15 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.15 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.15 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.2 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.2 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "" "tightcut"  0.2 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "" "tightcut"  0.2 "eta" "Pt" "closestZM matching dyonly" 
##reliso
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.1 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.1 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.1 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.1 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.15 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.15 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.15 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.15 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.2 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.2 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "reliso3" 0.2 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.2 "eta" "Pt" "closestZM matching dyonly" 
##dz
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.01 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.01 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.01 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.01 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.02 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.02 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.02 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.02 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.03 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.03 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dz" 0.03 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dz" 0.03 "eta" "Pt" "closestZM matching dyonly" 
##dxy
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.005 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.005 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.005 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.005 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.01 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.01 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.01 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.01 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.02 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.02 "Pt" "eta" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 11 "tightcut" "dxy" 0.02 "eta" "Pt" "closestZM matching dyonly" 
#qsub job.sh "ResultCombi2" 13 "tightcut" "dxy" 0.02 "eta" "Pt" "closestZM matching dyonly" 

####################Combi
##Try DrawMass only
####################Combi
#
##with defaul range
#
##tight
#qsub job.sh "ResultCombi" 11 "" "tightcut" 0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi" 13 "" "tightcut" 0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi" 11 "" "tightcut" 0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi" 13 "" "tightcut" 0.1 "eta" "Pt" "matching dyonly" 
##reliso
#qsub job.sh "ResultCombi" 11 "tightcut" "reliso3" 0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi" 13 "tightcut" "reliso3" 0.1 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi" 11 "tightcut" "reliso3" 0.1 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi" 13 "tightcut" "reliso3" 0.1 "eta" "Pt" "matching dyonly" 
##dz
#qsub job.sh "ResultCombi" 11 "tightcut" "dz" 0.01 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi" 13 "tightcut" "dz" 0.01 "Pt" "eta" "matching dyonly" 
#qsub job.sh "ResultCombi" 11 "tightcut" "dz" 0.01 "eta" "Pt" "matching dyonly" 
#qsub job.sh "ResultCombi" 13 "tightcut" "dz" 0.01 "eta" "Pt" "matching dyonly" 
#
##tight
#qsub job.sh "ResultCombi" 11 "" "tightcut" 0.1 "Pt" "eta" "closestZM" 
#qsub job.sh "ResultCombi" 13 "" "tightcut" 0.1 "Pt" "eta" "closestZM" 
#qsub job.sh "ResultCombi" 11 "" "tightcut" 0.1 "eta" "Pt" "closestZM" 
#qsub job.sh "ResultCombi" 13 "" "tightcut" 0.1 "eta" "Pt" "closestZM" 

##########################
##########################

##Do the T&P from mass
##tight
#qsub job.sh "Result130" 11 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "" "bkg_Cheb" "CBxBW"
##reliso
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "" "bkg_Cheb" "CBxBW"
##dz
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "" "bkg_Cheb" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "" "bkg_Cheb" "CBxBW"

##tight
#qsub job.sh "Result130" 11 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "ll" "matching" "CBxBW"
##reliso
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "ll" "matching" "CBxBW"
##dz
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "ll" "matching" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "ll" "matching" "CBxBW"

##nentries
#
##tight
#qsub job.sh "Result130" 11 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "ll" "nentries" "CBxBW"
##reliso
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "ll" "nentries" "CBxBW"
##dz
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "ll" "nentries" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "ll" "nentries" "CBxBW"
#
##matching nentries
#
##tight
#qsub job.sh "Result130" 11 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "ll" "matching nentries" "CBxBW"
##reliso
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "ll" "matching nentries" "CBxBW"
##dz
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "ll" "matching nentries" "CBxBW"
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "ll" "matching nentries" "CBxBW"



##with the matching
#
##tight
#qsub job.sh "Result130" 11 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "matching" 
#qsub job.sh "Result130" 13 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "matching" 
#qsub job.sh "Result130" 11 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "matching" 
#qsub job.sh "Result130" 13 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "matching" 
##reliso
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "matching" 
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "matching" 
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "matching" 
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "matching" 
##dz
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "matching" 
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "matching" 
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "matching" 
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "matching" 
#
##normal
#
##tight
#qsub job.sh "Result130" 11 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "" 
#qsub job.sh "Result130" 13 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "" 
#qsub job.sh "Result130" 11 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "" 
#qsub job.sh "Result130" 13 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "" 
##reliso
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "" 
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "" 
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "" 
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "" 
##dz
#qsub job.sh "Result130" 11 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "" 
#qsub job.sh "Result130" 13 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "" 
#qsub job.sh "Result130" 11 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "" 
#qsub job.sh "Result130" 13 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "" 


#just to test compiled draw
#The result is a success

##tight
#qsub job.sh "TestCompiledInvMassDraw" 11 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 13 10 250 24 "" "tightcut" 0.1 "Pt" "eta" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 11 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 13 -2.4 2.4 24 "" "tightcut" 0.1 "eta" "Pt" "short matching" 
##reliso
#qsub job.sh "TestCompiledInvMassDraw" 11 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 13 10 250 24 "tightcut" "reliso3" 0.1 "Pt" "eta" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 11 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 13 -2.4 2.4 24 "tightcut" "reliso3" 0.1 "eta" "Pt" "short matching" 
##dz
#qsub job.sh "TestCompiledInvMassDraw" 11 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 13 10 250 24 "tightcut" "dz" 0.01 "Pt" "eta" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 11 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "short matching" 
#qsub job.sh "TestCompiledInvMassDraw" 13 -2.4 2.4 24 "tightcut" "dz" 0.01 "eta" "Pt" "short matching" 










