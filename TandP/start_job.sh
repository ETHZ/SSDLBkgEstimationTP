#!/bin/bash

g++ -o DrawInvMassBkg_combi DrawInvMassBkg_combi.cc `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFit -lMinuit -lRooFitCore

#_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*
#InvM distribution for MINIAOD comparison
#_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*

#########
#Pt distr
#########
qsub job.sh "MINIAOD_25ns_" 13 "" "tightid" 0 "Pt" "eta" "dyonly" 
qsub job.sh "MINIAOD_25ns_" 13 "" "mediumid" 0 "Pt" "eta" "dyonly" 
qsub job.sh "MINIAOD_25ns_" 13 "" "softid" 0 "Pt" "eta" "dyonly" 
qsub job.sh "MINIAOD_25ns_" 13 "" "pfid" 0 "Pt" "eta" "dyonly" 

#########
#eta distr
#########
qsub job.sh "MINIAOD_25ns_" 13 "" "tightid" 0 "eta" "Pt" "dyonly" 
qsub job.sh "MINIAOD_25ns_" 13 "" "mediumid" 0 "eta" "Pt" "dyonly" 
qsub job.sh "MINIAOD_25ns_" 13 "" "softid" 0 "eta" "Pt" "dyonly" 
qsub job.sh "MINIAOD_25ns_" 13 "" "pfid" 0 "eta" "Pt" "dyonly" 

#TEST the code
#qsub job.sh "testmethod" 13 "" "tightid" 0 "Pt" "eta" "dyonly" 
#qsub job.sh "testmethod" 13 "" "reliso3" 0.12 "Pt" "eta" "dyonly" 

#jobs for comparison with MINIAOD POG tnp
