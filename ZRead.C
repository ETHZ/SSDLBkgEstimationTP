/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


* File Name : ZRead.C

* Purpose : This compute the efficiency for the second TTrees. The name of the branches is different than the previous trees and therefor this it is necessary to rewrite a new file.

* Creation Date : 20-12-2008

* Last Modified : Sat 20 Dec 2008 09:37:30 AM PST

* Created By :

_._._._._._._._._._._._._._._._._._._._._.*/

#include "cmath"
#include "TChain.h"
#include "iostream"
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
#include "FitInvMass.C"
#include "InvMass.C"
#include "DeltaR.C"
#include "vector"
//#include "efficiency.C"
//#include "eff_plots2_v2.C"
//#include "eff_plots2.C"
#include "eff_plots2_v3.C"
//#include "efficiency2.C"
//#include "TandPv2.C"
//#include "TandP2_v2.C"
#include "TandP2_v3.C"

void ZRead()
{
	setTDRStyle();

	//The corresponding three is labeled by the next variable. It should increase with the tree version
	int tree_n = 2;

	//Read Tree
	bool all_data = true;
	if(tree_n == 1){
		//Z tree given by Mario that include the status
		TChain* tree = new TChain("treeProducerSusyFullHad");
		if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/DYJetsM50_HT200to400_PU_S14_POSTLS170_babytree_QG_1.root");}
		//efficiency(tree, 11, 23,0);
		//TandP(tree, 11,23,0,0.09);
		//TandPv2(tree, 11,23,0,0.09);

	} else if (tree_n == 2){
		//tree provided by Constantin and Mathieu
		TChain* tree = new TChain("treeProducerSusySSDL");
		if (all_data == false){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/DYJetsM50_HT100to200_PU_S14_POSTLS170.root");}
		else if (all_data == true){tree->Add("/Users/GLP/Desktop/CERN_data/TTree_C_and_M/postprocessed/DYJetsM50_HT*.root");}
		//efficiency2(tree, 11, 23,0);
		//eff_plots2(tree, 11, 23, 0);
		//eff_plots2_v2(tree, 11, 23, 0);
		eff_plots2_v3(tree,13, 23);
		//eff_plots2_v3(tree,13, 23);
		//TandP2_v3(tree, 11, 23, 0, 0.2);

	}




}

