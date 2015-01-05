/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : MC_Ratio.C

* Purpose : Compute the efficiency's using the MC ratio technique

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

#include "cmath"
#include "TChain.h"
#include <sys/stat.h>
#include "iostream"
#include "fstream"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TLorentzVector.h"
#include "vector"
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TBranch.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TLegend.h"
#include "TObject.h"

#include "setTDRStyle.C"
#include "InvMass.C"
#include "DeltaR.C"
#include "Efficiency3_v3.C"
#include "Efficiency3_v4.C"

int MC_Ratio(int leptonId, double par_low, double par_upp, int nbins, TString sel_den , TString sel_num, double cut_den = 0., double cut_num = 0., TString option = "llsep", TString par_x = "Pt"){

	if(option.Contains("sep")){

return Efficiency_etasep(leptonId, par_low, par_upp, nbins, sel_den , sel_num, cut_den, cut_num, par_x , option);

	}else{ 

return Efficiency(leptonId, par_low, par_upp, nbins, sel_den , sel_num, cut_den, cut_num, par_x , option);

	}
		
}
