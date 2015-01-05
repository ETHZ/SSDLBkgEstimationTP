This file contains a description of the code.

/////////////
//Data file//
/////////////

The analysis has been done on two separated data files for now, labeled by 2 and 3.

2: 13tev_50ns_noskim, separate branch for electrons and muons
3: 2014-11-13_skim2ll-mva-softbtag, separate LepGood (passing loose selection) Lep_Other (failing loose selection).

/////////////
//Tag&Probe//
/////////////

Done in two steps. 

-The invariant mass is plotted using "DrawInvMassBkg3.C". The Tag has to pass the selection provided by the parameters "effcut"+"select". The selection is the "effcut parameter".

-The T&P ratio is computed using "FitData.C", which uses "FitIncMassBkg.C" and "FitBkg.C" to do the fit and extract the efficiency.

