#!/bin/bash

g++ -o DrawInvMassBkg_combi DrawInvMassBkg_combi.cc `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFit -lMinuit -lRooFitCore

#test the code
qsub job.sh "testmethod" 13 "" "tightid" 0 "Pt" "eta" "dyonly" 

#jobs for comparison with MINIAOD POG tnp
