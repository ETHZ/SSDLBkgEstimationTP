g++ -o CompareMCvsTandP CompareMCvsTandP.cc `root-config --cflags --glibs` -L $ROOTSYS/lib -lRooFit -lMinuit -lRooFitCore

./job2.sh "ResultCombi2" 13  "" "reliso3" 0.15 "eta" "Pt" "" "bkg_Cheb bkg_fit dyonly" "CBxBW" 
#./job.sh "ResultCombi2" 13  "tightcut" "reliso3" 0.1 "Pt" "eta" "" "bkg_Cheb" "CBxBW" >jobreport2_testn2&
#./job.sh "ResultCombi2" 13  "tightcut" "reliso3" 0.15 "Pt" "eta" "" "bkg_Cheb bkg_fit" "CBxBW" >jobreport3_testn2&
#./job.sh "ResultCombi2" 13  "tightcut" "reliso3" 0.15 "Pt" "eta" "" "bkg_Cheb" "CBxBW" >jobreport4_testn2&
#./job.sh "ResultCombi2" 13  "tightcut" "reliso3" 0.2 "Pt" "eta" "" "bkg_Cheb bkg_fit" "CBxBW" >jobreport5_testn2&
#./job.sh "ResultCombi2" 13  "tightcut" "reliso3" 0.2 "Pt" "eta" "" "bkg_Cheb" "CBxBW" >jobreport6_testn2&
