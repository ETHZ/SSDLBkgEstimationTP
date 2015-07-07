#!/bin/bash

#$ -q all.q
#$ -cwd

source $VO_CMS_SW_DIR/cmsset_default.sh
source /swshare/psit3/etc/profile.d/cms_ui_env.sh  # for bash
cd /shome/gaperrin/CMSSW_7_0_6/src/
eval `scramv1 ru -sh`

#roofit
cmsenv
. /swshare/ROOT/root_v5.34.18_slc6_amd64/bin/thisroot.sh

cd /shome/gaperrin/tnp_gael/SSDLBkgEstimationTP/TandP
#$ -N example_job
#$ -o /shome/gaperrin/tnp_gael/SSDLBkgEstimationTP/TandP/Log 
#$ -e /shome/gaperrin/tnp_gael/SSDLBkgEstimationTP/TandP/Log

#with defined bins
./DrawInvMassBkg_combi "$1" $2 "$3" "$4" $5 "$6" "${7}" "${8}" 

