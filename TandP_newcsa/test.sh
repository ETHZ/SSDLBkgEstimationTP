#!/bin/bash

#$ -q all.q
#$ -cwd

source $VO_CMS_SW_DIR/cmsset_default.sh

source /swshare/psit3/etc/profile.d/cms_ui_env.sh  # for bash

cd /shome/gaperrin/CMSSW_7_0_6/src/

eval `scramv1 ru -sh`

cd /shome/gaperrin/analysis/TandP_newcsa/ 

#list of arguments: filetage, pid, par1_low, par1_upp, npar1bins, sel_den, sel_num, cut, par1, par2, option

./test_job1.sh "test_para" 11 10 110 10 "tightcut" "reliso3" 0.1 "Pt" "eta" "short" 

#root -l -b -q 'DrawInvMassBkg.C++("test_input",11,10,$1,$2,"tightcut","reliso3",0.1,"Pt","eta","short")'
#root -l -b -q test.C++
#root -l -b -q test.C++

