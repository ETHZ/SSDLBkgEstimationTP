#!/bin/bash

g++ -o DrawInvMassBkg_combi DrawInvMassBkg_combi.cc `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFit -lMinuit -lRooFitCore

#test the code
#qsub job.sh "ResultCombi2" 13 "" "reliso3" 0.15 "Pt" "eta" "dyonly" 

#jobs for comparison with MINIAOD POG tnp
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.12 "Pt" "eta" "dyonly"
#qsub job.sh "ResultCombi2" 13 "tightcut" "reliso3" 0.12 "eta" "Pt" "dyonly" 
