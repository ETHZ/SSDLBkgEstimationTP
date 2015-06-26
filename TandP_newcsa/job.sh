#!/bin/bash

#$ -q all.q
#$ -cwd

source $VO_CMS_SW_DIR/cmsset_default.sh

source /swshare/psit3/etc/profile.d/cms_ui_env.sh  # for bash

cd /shome/gaperrin/CMSSW_7_0_6/src/

eval `scramv1 ru -sh`

#added those for roofit
cmsenv
. /swshare/ROOT/root_v5.34.18_slc6_amd64/bin/thisroot.sh
#end added

cd /shome/gaperrin/analysis/TandP_newcsa/ 

#with custom bins
#./CompareMCvsTandP "$1" $2 $3 $4 $5 "$6" "$7" $8 "$9" "${10}" "${11}" "${12}" "${13}"
#./DrawInvMassBkg "$1" $2 $3 $4 $5 "$6" "$7" $8 "$9" "${10}" "${11}" 

#with defined bins
#./DrawInvMassBkg "$1" $2 "$3" "$4" $5 "$6" "${7}" "${8}" 
./DrawInvMassBkg_combi "$1" $2 "$3" "$4" $5 "$6" "${7}" "${8}" 
#./DrawInvMassBkg_combi_phy14 "$1" $2 "$3" "$4" $5 "$6" "${7}" "${8}" 
#./CompareMCvsTandP "$1" $2 "$3" "$4" $5 "$6" "${7}" "${8}" "${9}" "${10}"

#don't use anymore
#root -l -b -q 'CompareMCvsTandP.C++("'$1'",'$2','$3','$4','$5',"'$6'","'$7'",'$8',"'$9'","'${10}'","'"${11}"'","'"${12}"'","'"${13}"'")'
#root -l -b -q 'DrawInvMassBkg.C++("'$1'",'$2','$3','$4','$5',"'$6'","'$7'",'$8',"'$9'","'${10}'","'"${11}"'")'

