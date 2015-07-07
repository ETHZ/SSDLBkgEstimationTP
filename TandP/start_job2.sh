g++ -o CompareMCvsTandP CompareMCvsTandP.cc `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFit -lMinuit -lRooFitCore

#_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*
#InvM distribution for MINIAOD comparison
#_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*_*

#########
#Pt distr
#########
qsub job2.sh "MINIAOD_25ns_" 13 "" "tightid" 0 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
qsub job2.sh "MINIAOD_25ns_" 13 "" "mediumid" 0 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
qsub job2.sh "MINIAOD_25ns_" 13 "" "softid" 0 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
qsub job2.sh "MINIAOD_25ns_" 13 "" "pfid" 0 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 

#########
#eta distr
#########
qsub job2.sh "MINIAOD_25ns_" 13 "" "tightid" 0 "eta" "Pt" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
qsub job2.sh "MINIAOD_25ns_" 13 "" "mediumid" 0 "eta" "Pt" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
qsub job2.sh "MINIAOD_25ns_" 13 "" "softid" 0 "eta" "Pt" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
qsub job2.sh "MINIAOD_25ns_" 13 "" "pfid" 0 "eta" "Pt" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 

#TEST
#./job2.sh "testmethod" 13  "" "tightid" 0 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
#./job2.sh "testmethod" 13  "" "tightcut" 0 "Pt" "eta" "short" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
#./job2.sh "testmethod" 13  "" "reliso3" 0.12 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
