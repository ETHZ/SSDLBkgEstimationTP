g++ -o CompareMCvsTandP CompareMCvsTandP.cc `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFit -lMinuit -lRooFitCore

#./job2.sh "ResultCombi2" 13  "" "reliso3" 0.15 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
#TEST
#./job2.sh "ResultCombi2" 13  "" "tightid" 0 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
./job2.sh "testmethod" 13  "" "tightid" 0 "Pt" "eta" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
